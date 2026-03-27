# Message Types

The RPC framework has a few message types to which the client and server can
each send and receive. Each message type has its own purpose and should be used accordingly.
Below are a list of all message types and their meaning.

- Msg::Send=0x1
    - A send message refers to when a client sends a message to the server.
- Msg::Resp=0x2
    - A receive message refers to when a server responds to a message the client sent. This message
    indicates that the server was able to execute the desired function and will respond with a return value.
- Msg::Void=0x3
    - A void message refers to when a server executes a void function at the client's request. A void message will be accompanied by no extra bytes afterwards.
- Msg::Err=0x4
    - An error message refers to when a server executes a function and something goes wrong, the server cannot find the desired function, or there is sometype of networking error.
- Msg::Close=0x5
    - A client will send a message close to the server before it destroys itself. The server will close the given socket accordingly.