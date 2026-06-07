#include "class.h"

extern "C" char PlayerCreature_vtable;
extern "C" void KIPlayer_ctor(KIPlayer *self, int kind, int route, Player *player,
                              AEGeometry *geometry, float x, float y, float z, bool active);
extern "C" void Matrix_ctor(Matrix *self);
extern "C" int Player_getHitpoints(Player *self, int keepOne, int keepMax);
extern "C" int PlayerCreature_enduranceTable[] __attribute__((visibility("hidden")));

struct PlayerCreature {
    PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                   float x, float y, float z);
    void reset();
};

PlayerCreature::PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                               float x, float y, float z)
{
    KIPlayer_ctor((KIPlayer *)this, kind, -1, player, geometry, x, y, z, false);
    F<char *>(this, 0x0) = &PlayerCreature_vtable + 8;
    Matrix_ctor((Matrix *)((char *)this + 0x144));

    Player *playerObject = F<Player *>(this, 0x4);
    int oneBits = 0x3f800000;
    F<uint8_t>(this, 0x12a) = 0;
    F<int>(this, 0x140) = itemIndex;
    F<int>(this, 0x12c) = oneBits;
    F<int>(this, 0x130) = 0;
    F<uint16_t>(this, 0x128) = 0;
    int previousMaxEndurance = F<int>(this, 0x134);
    F<int>(this, 0x138) = previousMaxEndurance;

    int hitpoints = Player_getHitpoints(playerObject, oneBits, previousMaxEndurance);
    int endurance = (int)(((float)PlayerCreature_enduranceTable[kind] / 10.0f) * 8000.0f) + 8000;
    F<int>(this, 0x134) = endurance;
    F<int>(this, 0x138) = endurance;
    F<int>(this, 0x13c) = hitpoints;
    reset();
}
