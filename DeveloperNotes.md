# Overview

This document lays out the general architectural idea behind how SimpleRPC
deals with messages.

It covers the structure of a message. It also covers how
messages move through the system: how they are constructed, serialized,
dispatched, and how responses are matched back to their originating calls. In general,
this should cover the bulk of the Architecture of the RPC framework.

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

# Message Format

The message format will vary slightly based on the message type. Below we list the various kinds of message formats.

- The following message format will be used for Msg::Send

`| Message Type | Message size | Function name | Args |`

- The following message format will be used for Msg::Recv

`| Message Type | Message Size | Return Value |`

- The following message format will be used for Msg::Void

`| Message Type |`

- The following message format will be used for Msg::Err

`| Message Type | Error Message |`

# Serialization and Deserialization

Serialization and Deserialization support is relatively minimal for this RPC framework. Most primitives are supported although a matching size and format between server and client cannot be
guaranteed. *Support for padded primitives are unsupported*. Support for strings is explicitly given in this framework. Support for other objects in libraries and frameworks do not exist. User-defined objects are supported through the *rpcCompliant* concept.

Serialization happens through an Encoder. The Encoder will serialize a data type and place it into
the given Message object. Once serialized, the Message object will be sent to either to the server or client where it will be deserialized. Deserialization happens through the decoder, which deserializes the data and applies it to the desired function.