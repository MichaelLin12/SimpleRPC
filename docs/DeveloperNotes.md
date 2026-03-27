# Overview

This document lays out the general architectural idea behind how SimpleRPC
deals with messages.

It covers the structure of a message. It also covers how
messages move through the system: how they are constructed, serialized,
dispatched, and how responses are matched back to their originating calls. In general,
this should cover the bulk of the Architecture of the RPC framework.





# Serialization and Deserialization

Serialization and Deserialization support is relatively minimal for this RPC framework. Most primitives are supported although a matching size and format between server and client cannot be
guaranteed. *Support for padded primitives are unsupported*. Support for strings is explicitly given in this framework. Support for other objects in libraries and frameworks do not exist. User-defined objects are supported through the *rpcCompliant* concept.

Serialization happens through an Encoder. The Encoder will serialize a data type and place it into
the given Message object. Once serialized, the Message object will be sent to either to the server or client where it will be deserialized. Deserialization happens through the decoder, which deserializes the data and applies it to the desired function.

