#ifndef GOF2_SENTRYGUN_H
#define GOF2_SENTRYGUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/weapons/ObjectGun.h"

class Gun;
class Level;

class SentryGun : public ObjectGun {
public:
    int cooldown; // spawn index/cooldown, seeded from the gun's item index

    SentryGun(Gun *gun, int mesh, int unused, int p4, Level *level);

    ~SentryGun();

    void update(int dt);

    void render();
};
#endif
