# SimpleRPC


# IDEA
---

We are going to heavily leverage generic programming and try to stick to c++ 17.
We are going to have a Client and Server obj that will take in a Serializer and Deserializer
They are going to produce a message object that will be sent over. This will be over TCP.
The Server will run it and then pass back the return value to the client. This will all be synchronous for now.

Much of the code will be written with Templates and will support a small set of data ...
objects with serialize and deserialize function, integers, floats, bool, string, char

