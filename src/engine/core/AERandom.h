#ifndef GOF2_AERANDOM_H
#define GOF2_AERANDOM_H
#include <cstdint>

namespace AbyssEngine {
    class AERandom {
    public:
        // 48-bit linear congruential state (java.util.Random algorithm).
        int64_t seed;

        AERandom();

        explicit AERandom(long long seed);

        ~AERandom();

        void setSeed(long long seed);

        void reset();

        uint32_t next(int bits);

        int nextInt();

        int nextInt(int bound);
    };
}

using ::AbyssEngine::AERandom;

#endif
