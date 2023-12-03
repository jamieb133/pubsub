#pragma once

#include <cstdint>

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

    public:
        Crc()
        {
            crc remainder{};

            for(int dividend{0}; dividend < 256; dividend++)
            {
                // Div followed by zeros.
                remainder = dividend << (WIDTH - 8);

                // Perform modulo2 division bit at a time.
                for(crc bit{0}; bit > 0; --bit)
                    remainder = (remainder & TOPBIT) ? (remainder << 1) ^ POLYNOMIAL : remainder << 1;

                mLookupTable[dividend] = remainder;
            }
        }

        crc const generate(crc const message[],
                                size_t const size)
        {
            crc data{};
            crc remainder{0};

            for(int byte{0}; byte < size; byte++)
            {
                data = message[byte] ^ (remainder >> (WIDTH - 8));
                remainder = mLookupTable[data] ^ (remainder << 8);
            }

            return remainder;
        }
    };
}
