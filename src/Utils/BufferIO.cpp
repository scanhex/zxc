#include <cstdint>
#include <cstddef>
#include "BufferIO.h"

void BufferIO::writeUInt8(uint8_t d, size_t start_idx, uint8_t (&buffer)[1024]) {
    buffer[start_idx] = d;
}

void BufferIO::writeDouble(double d, size_t start_idx, uint8_t (&buffer)[1024]) {
    binaryDouble u;
    u.dValue = d;
    writeInt64(u.iValue, start_idx, buffer);
}

void BufferIO::writeInt32(int32_t d, size_t start_idx, uint8_t (&buffer)[1024]) {
    for (size_t i = 0; i < 4; i++)
        buffer[start_idx + 3 - i] = (d >> (i * 8));
}

void BufferIO::writeInt64(int64_t d, size_t start_idx, uint8_t (&buffer)[1024]) {
    for (size_t i = 0; i < 8; i++)
        buffer[start_idx + 7 - i] = (d >> (i * 8));
}


uint8_t BufferIO::readUInt8(size_t start_idx, uint8_t (&buffer)[1024]) {
    return buffer[start_idx];
}

double BufferIO::readDouble(size_t start_idx, uint8_t (&buffer)[1024]) {
    binaryDouble u;
    u.iValue = BufferIO::readInt64(start_idx, buffer);
    return u.dValue;
}

int32_t BufferIO::readInt32(size_t start_idx, uint8_t (&buffer)[1024]) {
    int32_t result = 0;
    for (size_t i = 0; i < 4; i++)
        result = (result << 8) + buffer[start_idx + i];
    return result;
}

int64_t BufferIO::readInt64(size_t start_idx, uint8_t (&buffer)[1024]) {
    int64_t result = 0;
    for (size_t i = 0; i < 8; i++)
        result = (result << 8) + buffer[start_idx + i];
    return result;
}
