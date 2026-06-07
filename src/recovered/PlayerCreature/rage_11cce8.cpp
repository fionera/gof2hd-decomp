#include "class.h"

extern "C" int PlayerCreature_rageTable[] __attribute__((visibility("hidden")));

struct PlayerCreature {
    void rage(int amount);
};

void PlayerCreature::rage(int amount)
{
    float amountScale = (float)amount / -100.0f;
    float typeScale = (float)PlayerCreature_rageTable[F<int>(this, 0xac)];
    F<int>(this, 0x130) = 0;
    F<uint16_t>(this, 0x128) = 0x101;
    F<float>(this, 0x12c) = (amountScale + 1.0f) * (typeScale + 1.0f);
}
