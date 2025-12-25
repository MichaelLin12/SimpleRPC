#pragma once


// WE NEED TO FIGURE OUT HOW TO HANDLE ERRORS
// use std::vexpected and ENUM CLASS
// maybe stick to static obj creation


enum class ERROR{
    SUCCESS, //default or expected
    CONN_CLOSED, // connection closed unexpectedly
    CONN_FAILED, // failed to connect
    ERR_NETWORK_RESOLVE, // getaddrinfo failed
    SEND_FAILURE, //send failure ... check errno
    RECV_FAILURE, //receive failure ... check errno
    SETSOCKOPT_FAILURE, //setsockopt failed ... check errno
    BIND_FAILURE, // bind failed ... check errno
    LISTEN_FAILURE, // listen failed ... check errno
    ACCEPT_FAILURE // accept failed ... check errno
};