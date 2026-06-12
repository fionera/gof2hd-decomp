#ifndef GOF2_BEAMGUN_H
#define GOF2_BEAMGUN_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- BeamGun (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level "BeamGun::..." symbols.

struct Gun;
struct Level;
struct Player;
struct PlayerEgo;
struct AEGeometry;
struct PaintCanvas;
struct Transform;

// Vector/Matrix already provided at global scope by common.h (math.h).

class BeamGun {
public:
    void* field_0x0;                    // +0x0  vtable
    int32_t field_0x4;                  // +0x4
    Gun* field_0x8;                     // +0x8  gun
    Level* field_0xc;                   // +0xc  level
    int32_t field_0x10;                 // +0x10
    int32_t field_0x14;                 // +0x14
    AEGeometry* field_0x18;             // +0x18 primary geometry
    AEGeometry* field_0x1c;             // +0x1c secondary geometry
    uint8_t field_0x20;                 // +0x20 hasSecondary
    uint8_t field_0x21;                 // +0x21 secondaryVisible

    BeamGun(int param_1, Gun *gun, int param_3, Level *level);
    void render();
    void update(int elapsed);
    void setEnemies(Array<Player *> *enemies);
    void setEnemy(Player *enemy);

    // Tail-call veneer fragments: the terminal b.w of the matching method into a
    // relocated slot reaching the inherited engine implementation (the enemy
    // handler / AEGeometry::render). They carry no static body of their own.
    void setEnemies_tail(void *data);
    void setEnemy_tail(void *data);
    void render_tail(AEGeometry *self);
};
#endif
