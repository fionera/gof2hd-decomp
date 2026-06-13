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
    void* vtable;                       // +0x0  vtable
    int32_t field_0x4;                  // +0x4  (zero-initialised; semantics not recovered)
    Gun* gun;                           // +0x8  owning gun
    Level* level;                       // +0xc  level
    int32_t owner;                      // +0x10 owner id (ctor param_1)
    int32_t meshKind;                   // +0x14 mesh-kind index (param_3-derived)
    AEGeometry* primaryGeometry;        // +0x18 primary beam geometry
    AEGeometry* secondaryGeometry;      // +0x1c secondary beam geometry
    uint8_t hasSecondary;               // +0x20 secondary geometry present
    uint8_t secondaryVisible;           // +0x21 secondary visible (tracks gun->delayActive)

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

    // Inherited gun-hierarchy enemy landing pads reached by the tail veneers; each
    // forwards the extracted data pointer through the resolved engine handler slot.
    void setEnemiesEngine(void *data);
    void setEnemyEngine(void *data);
};
#endif
