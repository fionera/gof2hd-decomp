#include "game/weapons/TractorBeam.h"
#include "game/ship/Ship.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "engine/math/Transform.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/PlayerEgo.h"
#include "game/mission/Status.h"
#include "game/core/PaintCanvasClass.h"

using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::VectorLength;
using AbyssEngine::AEMath::VectorNormalize;

// The original binary mangles the radar parameter as a global ::Radar (P5Radar),
// not AbyssEngine::Radar. Radar.h (which defines AbyssEngine::Radar) is therefore
// not included; only the two lock-slot fields this function touches are modeled
// here, mirroring the real layout (field_0x8 at +0x08, field_0x1c at +0x1c).
struct Radar {
    void* level;          // +0x00
    void* field_0x4;      // +0x04
    void* field_0x8;      // +0x08
    void* lockedAsteroid; // +0x0c
    int   field_0x10;     // +0x10
    int   field_0x14;     // +0x14
    int   field_0x18;     // +0x18
    void* field_0x1c;     // +0x1c
};

// The grabbed crate's hitpoints/active state live on its owning Player object,
// reached through the crate's player slot. Player both declares a 'turnedEnemy'
// field and a 'turnedEnemy()' method in one struct, so its full header is not
// self-compilable; TractorBeam only needs these two accessors.
struct Player {
    int getHitpoints();
    unsigned char isActive();
};

// Engine singletons / roots resolved at load time.
extern AbyssEngine::PaintCanvas* gCanvasRoot;
extern FModSound* gPullSound;
extern FModSound* gCaptureSound;
extern const float gCaptureDistance;

// Draws the beam only while it is active; forwards to the geometry's render.
void TractorBeam::render() {
    if (!this->active)
        return;
    this->beamGeometry->render();
}

// Drives the tractor beam: locks onto a nearby crate, animates the beam mesh
// from the player ship toward the crate, pulls the crate in, and captures it
// once it is close enough.
void TractorBeam::update(int frameTime, Radar* radar, Level* level, Hud* hud) {
    KIPlayer* crate = this->grabbedCrate;
    KIPlayer* radarCrate = (KIPlayer*)radar->field_0x1c;

    if (radarCrate == nullptr && crate == nullptr)
        return;

    PlayerEgo* player = (PlayerEgo*)(intptr_t)level->getPlayer();

    if (crate == nullptr) {
        if (!player->isInTurretMode()) {
            if (radarCrate->cargoAvailable() == 0) {
                this->active = 0;
                radar->field_0x1c = nullptr;
            } else {
                if (radarCrate->crateGeometry == nullptr)
                    radarCrate->createCrate(0);
                this->grabbedCrate = radarCrate;
                this->storedHitpoints = ((Player*)radarCrate->player)->getHitpoints();
                this->active = 1;
            }
            return;
        }
        crate = this->grabbedCrate;
        if (crate == nullptr)
            goto detach;
    }

    // Drop the beam if the crate has lost its body or the carrier is gone.
    if (crate->crateGeometry == nullptr ||
        (crate->stealFlag == 0 && ((Player*)crate)->isActive() == 0)) {
    detach:
        radar->field_0x8 = nullptr;
        radar->field_0x1c = nullptr;
        this->grabbedCrate = nullptr;
        this->active = 0;
        return;
    }

    // Advance the canvas transform for this frame.
    AbyssEngine::PaintCanvas* canvas = gCanvasRoot;
    AbyssEngine::Transform* tf = (AbyssEngine::Transform*)canvas->TransformGetTransform(0);
    tf->Update(frameTime, false);

    // Working vector = crate position - player position.
    Vector cratePos = ((AEGeometry*)this->grabbedCrate->crateGeometry)->getPosition();
    Vector playerPos = player->getPosition();
    Vector working = cratePos - playerPos;
    this->dirX = working.x;
    this->dirY = working.y;
    this->dirZ = working.z;
    float dist = VectorLength(working);

    // Beam length depends on the equipped ship hull.
    int shipIndex = gStatus->getShip()->getIndex();
    Vector offset = {0.0f, 0.0f, 0.0f};
    if (shipIndex == 0x2c) {
        offset = player->GetDirVector() * 0.5f;
    } else if (shipIndex == 0x31) {
        Vector dir = player->GetDirVector();
        Vector tmp = dir * 0.5f;
        offset.x = tmp.x + dir.x;
        offset.y = tmp.y + dir.y;
        offset.z = tmp.z + dir.z;
    }
    this->beamGeometry->setScaling(offset.x);

    // Aim the beam at the crate and place its tail at the player position.
    AEGeometry* beam = this->beamGeometry;
    Vector dirN = VectorNormalize(offset - working);
    beam->setDirection(dirN, dirN);
    beam->setPosition(player->getPosition());

    if (dist > gCaptureDistance) {
        // Crate still far -- pull it toward the ship along the beam.
        radar->field_0x1c = nullptr;
        dirN = VectorNormalize(working);
        working = dirN * (float)(frameTime * 10);
        Vector pull = dirN - working;
        ((AEGeometry*)this->grabbedCrate->crateGeometry)->translate(pull);

        if (this->soundPlaying == 0) {
            gPullSound->play(0, nullptr, nullptr, 0.0f);
            this->soundPlaying = 1;
        }
    } else {
        // Crate reached the ship -- capture it.
        this->grabbedCrate->captureCrate(hud);
        this->grabbedCrate = nullptr;
        this->active = 0;
        radar->field_0x8 = nullptr;
        radar->field_0x1c = nullptr;
        this->soundPlaying = 0;
        gCaptureSound->stop((void*)nullptr);
        gCaptureSound->play(4, nullptr, nullptr, 0.0f);
    }
}

// Zeroes the working state then constructs the beam mesh geometry from base
// mesh id 0x3798 offset by the beam kind.
TractorBeam::TractorBeam(AEGeometry* /*unused*/, int kind) {
    this->dirX = 0.0f;
    this->dirY = 0.0f;
    this->dirZ = 0.0f;
    this->grabbedCrate = nullptr;
    this->active = 0;
    this->soundPlaying = 0;

    uint16_t meshId = (uint16_t)((short)kind + 0x3798);
    this->beamGeometry = new AEGeometry(meshId, gCanvasRoot, false);
    this->storedHitpoints = 0;
}

// Destroys and frees the beam geometry, then clears the slot.
TractorBeam::~TractorBeam() {
    delete this->beamGeometry;
    this->beamGeometry = nullptr;
}

// Allocates and constructs a beam. PlayerEgo::equip reaches the beam through
// this factory when a tractor-beam module is equipped.
TractorBeam* TractorBeam::create(AEGeometry* geo, int kind) {
    return new TractorBeam(geo, kind);
}
