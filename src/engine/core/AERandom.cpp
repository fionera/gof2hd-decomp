#include "engine/core/AERandom.h"
#include <ctime>

namespace AbyssEngine {
    namespace {
        const int64_t kMultiplier = 0x5deece66dLL;
        const int64_t kAddend = 0xbLL;
        const int64_t kMask = (1LL << 48) - 1;
    }

    AERandom::AERandom() {
        seed = (time(0) ^ kMultiplier) & kMask;
    }

    AERandom::AERandom(long long seed) {
        setSeed(seed);
    }

    AERandom::~AERandom() {
    }

    void AERandom::setSeed(long long seed) {
        this->seed = (seed ^ kMultiplier) & kMask;
    }

    void AERandom::reset() {
        seed = (time(0) ^ kMultiplier) & kMask;
    }

    uint32_t AERandom::next(int bits) {
        seed = (seed * kMultiplier + kAddend) & kMask;
        return static_cast<uint32_t>(static_cast<uint64_t>(seed) >> (48 - bits));
    }

    int AERandom::nextInt() {
        seed = (seed * kMultiplier + kAddend) & kMask;
        return static_cast<int>(static_cast<uint64_t>(seed) >> 16);
    }

    int AERandom::nextInt(int bound) {
        if ((bound & -bound) == bound) {
            seed = (seed * kMultiplier + kAddend) & kMask;
            int bits = static_cast<int>(static_cast<uint64_t>(seed) >> 17);
            return static_cast<int>((static_cast<int64_t>(bits) * bound) >> 31);
        }

        int bits;
        int value;
        do {
            seed = (seed * kMultiplier + kAddend) & kMask;
            bits = static_cast<int>(static_cast<uint64_t>(seed) >> 17);
            value = bits % bound;
        } while (bits - value + (bound - 1) < 0);
        return value;
    }
}
