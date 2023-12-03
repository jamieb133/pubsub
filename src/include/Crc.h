#pragma once

#include <cstdint>
#include <cstddef>

namespace pubsub
{
    using crc = uint8_t;
    static constexpr int WIDTH { 8 * sizeof(uint8_t) };
    static constexpr int TOPBIT (1 << (WIDTH - 1));
    static constexpr crc POLYNOMIAL {0xD8};

    class Crc
    {
    private:
        crc mLookupTable[256];
        Crc();

    public:
        static Crc& get_instance();
        crc const generate(crc const message[],
                            size_t const size);
    };
}
