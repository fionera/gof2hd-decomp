#ifndef GOF2_SPRITEGUN_H
#define GOF2_SPRITEGUN_H
#include <cstdint>
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- SpriteGun: a lightweight sprite-based weapon wrapper. It
// carries a vtable and a single zero-initialised field; the constructor's gun
// and kind arguments are unused. Top-level class (no namespace).

class Gun;
class Player;

class SpriteGun {
public:
    int32_t field_0x4;                  // zero-initialised; semantics not recovered

    SpriteGun(Gun* gun, int kind);
    virtual ~SpriteGun();

    // The deleting destructor variant (D0) traps in the original image: the
    // class-scope operator delete aborts, so `delete`-ing a SpriteGun is a hard
    // fault rather than a heap free.
    static void operator delete(void*) { __builtin_trap(); }

    // Virtual methods in vtable order: setEnemies, setEnemy, update, render.
    virtual void setEnemies(Array<Player*>* enemies);
    virtual void setEnemy(Player* enemy);
    virtual void update(int elapsed);
    virtual void render();
};
#endif
