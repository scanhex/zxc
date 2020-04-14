#pragma once

namespace BufferIO {
    /*
    *  Functions to parse args to bytes and back
    */
    void writeUInt8(uint8_t d, size_t start_idx, uint8_t (&buffer)[1024]);

    void writeDouble(double d, size_t start_idx, uint8_t (&buffer)[1024]);

    void writeInt32(int32_t d, size_t start_idx, uint8_t (&buffer)[1024]);

    void writeInt64(int64_t d, size_t start_idx, uint8_t (&buffer)[1024]);

    uint8_t readUInt8(size_t start_idx, uint8_t (&buffer)[1024]);

    double readDouble(size_t start_idx, uint8_t (&buffer)[1024]);

    int32_t readInt32(size_t start_idx, uint8_t (&buffer)[1024]);

    int64_t readInt64(size_t start_idx, uint8_t (&buffer)[1024]);

    union binaryDouble {
        double dValue;
        uint64_t iValue;
    };
}
