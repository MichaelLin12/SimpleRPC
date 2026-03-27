# Overview

This document lays out the general architectural idea behind how SimpleRPC
deals with messages.

It covers the structure of a message. It also covers how
messages move through the system: how they are constructed, serialized,
dispatched, and how responses are matched back to their originating calls. In general,
this should cover the bulk of the Architecture of the RPC framework.


# Design Decisions

TCP was chosen as the transport layer for its ordering and exactly-once delivery guarantees, which are necessary for correct RPC semantics. UDP would require reimplementing these guarantees to avoid duplicate or out-of-order execution.




