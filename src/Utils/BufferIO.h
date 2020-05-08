#pragma once

#include <cstdint>
#include <cstddef>

namespace BufferIO {

    static constexpr int MAX_BUF = 1024;

    union binaryDouble {
        double dValue;
        uint64_t iValue;
    };

/*
*  Classes to parse args to bytes and back
*/
    class BufferWriter final {
    public:
        void writeUInt8(uint8_t d);

        void writeDouble(double d);

        void writeInt32(int32_t d);

        void writeInt64(int64_t d);

        void flushBuffer();

    public:
        uint8_t write_buffer_[MAX_BUF]{};

    private:
        std::size_t idx_{0};
    };

    class BufferReader final {
    public:

        uint8_t readUInt8();

        double readDouble();

        int32_t readInt32();

        int64_t readInt64();

        void flushBuffer();

    public:
        uint8_t read_buffer_[MAX_BUF]{};

    private:
        size_t idx_{0};
    };

}
