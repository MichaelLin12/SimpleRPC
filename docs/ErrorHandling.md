# Response Handling

Errors are handled through [Msg::Err](./MessageTypes.md). These are usually passed from server to client to let the client know that an error had happened on the called function. For a function that is registered on the Server, it should make use of the std::except. This means that for a given registered function, it should return an std::except object of the types R and ErrMessage:

std::except<R, ErrMessage> result

The server should check the result value R. If R is void, the server should send a Msg::Void. If R exists and is not void, the server should return Msg::Resp. If R doesn't exist, ErrMessage should exist. The server should send back ErrMessage via Msg::Err.
To be clear, the order of how things should work is

1. If ErrMessage exists, the server will send back ErrMessage via Msg::Err
2. If R is void, the server sends back Msg::Void
3. If R is not void, the server sends back Msg::Resp

This should guard against the issue of R being void and checking for void first when the function would return an ErrMessage.

# Server Error Handling

Errors that appear during server creation should immediately be logged and aborted.

Errors can appear on the server as well. This might be due to networking issues or something of that nature. On the server side of things, if a networking issue does happen, it should be logged first and foremost. If it occurs in the worker thread, then:

- if the connection is still open from the other side, add it back into the epoll
- if the connection is not open from the other side, close the socket

Errors that appear on server close, should also be logged

# Client Error Handling

There should be no errors on the client side assuming the server doesn't abruptly shut down