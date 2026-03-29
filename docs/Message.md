# Message class

## Methods

- `void addData(const T& arg)`

Add data *arg* to *buffer* and update the internal *offset*

- `std::span<const std::byte> getBuffer() const`

Get a read only view of the internal *buffer*

- `Message(Msg type, std::size_t size)`

Constructor constructs a Message. Note that type and size are automatically added to the buffer. 
size = size_bytes(type) + size_bytes(size) + size_bytes(data)

- `std::size_t getSize() const`

Get the size of the message

- `std::size_t getOffset() const`

get the current offset of the buffer

- `Msg getType() const`

Get the type of the message

- `void setOffset(std::size_t noffset)`

set buffer offset to noffset

- `void addRawData(std::span<std::byte> data)`

add data in the form of bytes to buffer

## Instance members

- `std::vector<std::byte> buffer`

buffer to hold data, size, and type of message

- `std::size_t offset`

current offset of buffer

- `std::size_t sz`

size of message

- `Msg type`

type of message -> see[Message Types](./MessageTypes.md) for more information