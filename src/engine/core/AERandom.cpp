#include "engine/core/AERandom.h"
#include <ctime>

namespace AbyssEngine {
    AERandom::AERandom() {
        long long now = time(0);
        seedLow = 0xdeece66dU ^ static_cast<uint32_t>(now);
        seedHigh = (static_cast<uint32_t>(now >> 31) & 0xffffU) ^ 5U;
    }

    AERandom::AERandom(long long seed) {
        setSeed(seed);
    }

    AERandom::~AERandom() {
    }

    void AERandom::setSeed(long long seed) {
        seedLow = 0xdeece66dU ^ static_cast<uint32_t>(seed);
        seedHigh = (static_cast<uint32_t>(seed >> 32) & 0xffffU) ^ 5U;
    }

    void AERandom::reset() {
        long long now = time(0);
        seedLow = 0xdeece66dU ^ static_cast<uint32_t>(now);
        seedHigh = (static_cast<uint32_t>(now >> 31) & 0xffffU) ^ 5U;
    }

    uint32_t AERandom::next(int bits) {
        uint64_t seed = (static_cast<uint64_t>(seedHigh) << 32) | seedLow;
        seed = seed * 0x5deece66dULL + 0xbULL;
        uint32_t newLo = static_cast<uint32_t>(seed);
        uint32_t newHi = static_cast<uint32_t>(seed >> 32) & 0xffffU;

        seedLow = newLo;
        seedHigh = newHi;

        return static_cast<uint32_t>(((static_cast<uint64_t>(newHi) << 32) | newLo) >> (48 - bits));
    }

    int AERandom::nextInt() {
        uint64_t seed = (static_cast<uint64_t>(seedHigh) << 32) | seedLow;
        seed = seed * 0x5deece66dULL + 0xbULL;
        uint32_t newLo = static_cast<uint32_t>(seed);
        uint32_t newHi = static_cast<uint32_t>(seed >> 32) & 0xffffU;
        seedLow = newLo;
        seedHigh = newHi;
        return static_cast<int>(static_cast<uint32_t>(seed >> 16));
    }

    int AERandom::nextInt(int bound) {
        if ((bound & -bound) == bound) {
            uint64_t seed = (static_cast<uint64_t>(seedHigh) << 32) | seedLow;
            seed = (seed * 0x5deece66dULL + 0xbULL) & 0xffffffffffffULL;
            int bits = static_cast<int>(static_cast<uint32_t>(seed >> 17));
            seedLow = static_cast<uint32_t>(seed);
            seedHigh = static_cast<uint32_t>(seed >> 32);
            return static_cast<int>((static_cast<int64_t>(bits) * bound) >> 31);
        }

        int bits;
        int value;
        do {
            uint64_t seed = (static_cast<uint64_t>(seedHigh) << 32) | seedLow;
            seed = (seed * 0x5deece66dULL + 0xbULL) & 0xffffffffffffULL;
            bits = static_cast<int>(static_cast<uint32_t>(seed >> 17));
            value = bits % bound;
            seedLow = static_cast<uint32_t>(seed);
            seedHigh = static_cast<uint32_t>(seed >> 32);
        } while (bits - value + (bound - 1) < 0);
        return value;
    }
}
