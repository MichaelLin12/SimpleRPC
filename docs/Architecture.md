
- When you create a message, you must specify the size of the message to include the message type. The message type and size are automatically added for you within the buffer at construction time.

- boost_concurrent_flat_map was chosen for various reasons
    - current LTS versions of c++ do not support flat_map in the standard library.
    - [READ](https://bannalia.blogspot.com/2023/07/inside-boostconcurrentflatmap.html)
    - [Read](https://bannalia.blogspot.com/2022/11/inside-boostunorderedflatmap.html)
    - open addressing is faster in performance compared to close addressing