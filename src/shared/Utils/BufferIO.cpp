#include <cstdint>
#include <cstddef>
#include <cassert>
#include "BufferIO.h"

void BufferIO::BufferWriter::writeUInt8(uint8_t d) {
    assert(idx_ + 1 <= MAX_BUF);
    write_buffer_[idx_] = d;
    idx_++;
}

void BufferIO::BufferWriter::writeDouble(double d) {
    binaryDouble u;
    u.dValue = d;
    writeInt64(u.iValue);
}

void BufferIO::BufferWriter::writeInt32(int32_t d) {
    assert(idx_ + 4 <= MAX_BUF);
    for (size_t i = 0; i < 4; i++)
        write_buffer_[idx_ + 3 - i] = (d >> (i * 8));
    idx_ += 4;
}

void BufferIO::BufferWriter::writeInt64(int64_t d) {
    assert(idx_ + 8 <= MAX_BUF);
    for (size_t i = 0; i < 8; i++)
        write_buffer_[idx_ + 7 - i] = (d >> (i * 8));
    idx_ += 8;
}

void BufferIO::BufferWriter::flushBuffer() {
    idx_ = 0;
}

uint8_t BufferIO::BufferReader::readUInt8() {
    assert(idx_ + 1 <= 1024);
    return read_buffer_[idx_++];
}

double BufferIO::BufferReader::readDouble() {
    binaryDouble u;
    u.iValue = BufferIO::BufferReader::readInt64();
    return u.dValue;
}

int32_t BufferIO::BufferReader::readInt32() {
    assert(idx_ + 4 <= 1024);
    int32_t result = 0;
    for (size_t i = 0; i < 4; i++)
        result = (result << 8) + read_buffer_[idx_ + i];
    idx_ += 4;
    return result;
}

int64_t BufferIO::BufferReader::readInt64() {
    assert(idx_ + 4 <= 1024);
    int64_t result = 0;
    for (size_t i = 0; i < 8; i++)
        result = (result << 8) + read_buffer_[idx_ + i];
    idx_ += 8;
    return result;
}

void BufferIO::BufferReader::flushBuffer() {
    idx_ = 0;
}
