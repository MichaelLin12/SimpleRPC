#pragma once
#include <cstdint>

enum class Msg : uint8_t
{
    Send = 0x01,
    Resp = 0x02,
    Void = 0x03,
    Err = 0x04,
    Close = 0x05
};