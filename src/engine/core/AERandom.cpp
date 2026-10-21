#include "engine/core/AERandom.h"
#include <ctime>

namespace AbyssEngine {
    AERandom::AERandom() {
        long long now = time(0);
        seed = (now ^ 0x5deece66dLL) & ((1LL << 48) - 1);
    }

    AERandom::AERandom(long long seed) {
        setSeed(seed);
    }

    AERandom::~AERandom() {
    }

    void AERandom::setSeed(long long seed) {
        this->seed = (seed ^ 0x5deece66dLL) & ((1LL << 48) - 1);
    }

    void AERandom::reset() {
        long long now = time(0);
        seed = (now ^ 0x5deece66dLL) & ((1LL << 48) - 1);
    }

    uint32_t AERandom::next(int bits) {
        seed = (seed * 0x5deece66dLL + 0xbLL) & ((1LL << 48) - 1);
        return static_cast<uint32_t>(static_cast<unsigned long long>(seed) >> (48 - bits));
    }

    int AERandom::nextInt() {
        return static_cast<int>(next(32));
    }

    int AERandom::nextInt(int bound) {
        if ((bound & -bound) == bound)
            return static_cast<int>((bound * static_cast<long long>(next(31))) >> 31);

        int bits;
        int value;
        do {
            bits = static_cast<int>(next(31));
            value = bits % bound;
        } while (bits - value + (bound - 1) < 0);
        return value;
    }
}
