# How do we handle Internal Errors

Internal errors should be handled by passing back an error code `ErrorCode`. This is done through `std::expected<T,ErrorCode>`

# To see how this might be used, lets look at a few examples displayed throughout the codebase.

```
inline void sendAll(int socket, const Message& m)
{
    auto buffer = m.getBuffer();
    LOGGING(LogLevel::DEBUG, "buffer size is: {}", buffer.size());
    std::size_t sent = 0;
    while (sent < buffer.size())
    {
        auto remainingView = buffer.subspan(sent);
        ssize_t data =
            send(socket, reinterpret_cast<const void*>(remainingView.data()),
                 remainingView.size(), 0);
        if (data == -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }

        if (data == 0)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }
        sent += data;
    }
    LOGGING(LogLevel::DEBUG, "sent: {}", sent);
}
```

Starting off with the sendAll function. One possible place that we see is that `data == 0`. According to various documentation, this happens due to the connection being unexpectedly closed. To deal with this issue, we return `std::unexpected(ErrorCode::ConnectionClosed)` instead of `std::abort`. 


*What about when `data == -1`?*

Well there are many possibilities in this scenario as defined by `errno`. I will only cover the ones in detail that are most likely.

EINTR
    - A signal has been given to the process. It should be handled but not by the worker thread. It'll be handled by the main thread instead. Therefore, work should be continued on.
EAGAIN
    - This happens when the socket is non-blocking. There is no real error here. Its just that the kernel is not able to handle the data at the moment. We should port back the file descriptor and check for epollout and continue sending the data from the server side that we didn't send.
    - `ErrorCode::Retry`
EWOULDBLOCK
    - Same as EAGAIN
    - `ErrorCode::Retry`
EPIPE
    - remote has closed the socket
    - `ErrorCode::ConnClosed`
ENOBUFS
    - the kernel cannot allocate network buffers needed to complete the send. We should close the connection.
    - `ErrorCode::SysErr`
ENOMEN
    - the kernel is out of memory. We should close the connection.
    - - `ErrorCode::SysErr`
ECONNRESET
    - remote peer forcibly closed the connection.
    - `ErrorCode::ConnClosed`

The other errors will all be aggregated under `ErrorCode::WriteError`

## lets look at receiveMsg now. This should give us an idea on how to handle the other receive variants

```
inline std::size_t receiveAll(int socket, Message& msg)
{
    std::size_t received = 0;
    std::size_t sz = msg.getSize() - msg.getOffset();
    std::vector<std::byte> buffer{sz};
    while (received < sz)
    {
        ssize_t data = recv(
            socket, reinterpret_cast<unsigned char*>(buffer.data()) + received,
            sz - received, 0);
        if (data == -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }

        if (data == 0)
        {
            LOGGING(LogLevel::ERROR, "Client connection closed");
            return 0;
            // std::abort();
        }
        received += data;
    }

    msg.addRawData(buffer);
    return received;
}
```

**data == 0**

when data equals 0, the connection has closed. We return back `ErrorCode::ConnectionClosed` and close the socket.

**data == 1**

- EAGAIN or EWOULDBLOCK
    - There is nothing wrong here. We essentially need to just retry by porting back the file descriptor
    - `ErrorCode::Retry`
- EINTR
    - A signal has occurred. Signals are handled through the main thread and should not be the worry of the worker thread


The other errors will all be aggregated under `ErrorCode::ReadError`