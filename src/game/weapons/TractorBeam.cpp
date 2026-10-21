#include "game/weapons/TractorBeam.h"
#include "game/core/Globals.h"
#include "game/ship/Ship.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "engine/math/Transform.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/PlayerEgo.h"
#include "game/mission/Status.h"
#include "engine/render/PaintCanvas.h"
#include "game/weapons/Radar.h"
#include "game/ship/Player.h"

TractorBeam::TractorBeam(AEGeometry * /*unused*/, int kind) {
    this->direction.x = 0.0f;
    this->direction.y = 0.0f;
    this->direction.z = 0.0f;
    this->grabbedCrate = nullptr;
    this->active = 0;
    this->soundPlaying = 0;

    this->beamGeometry = new AEGeometry((uint16_t) (kind + 0x3798), Globals::Canvas, false);
    this->storedHitpoints = 0;
}

TractorBeam::~TractorBeam() {
    delete this->beamGeometry;
    this->beamGeometry = nullptr;
}

void TractorBeam::update(int frameTime, Radar *radar, Level *level, Hud *hud) {
    KIPlayer *radarCrate = radar->field_0x1c;
    if (this->grabbedCrate == nullptr && radarCrate == nullptr)
        return;

    if (this->grabbedCrate == nullptr && !level->getPlayer()->isInTurretMode()) {
        if (radarCrate->cargoAvailable() == 0) {
            this->active = 0;
            radar->field_0x1c = nullptr;
        } else {
            if (radarCrate->crateGeometry == nullptr)
                radarCrate->createCrate(0);
            this->grabbedCrate = radarCrate;
            this->storedHitpoints = radarCrate->player->getHitpoints();
            this->active = 1;
        }
    } else if (this->grabbedCrate == nullptr || this->grabbedCrate->crateGeometry == nullptr ||
        (this->grabbedCrate->stealFlagByte == 0 && !this->grabbedCrate->player->isActive())) {
        radar->field_0x1c = nullptr;
        radar->dockNavPtr = nullptr;
        this->grabbedCrate = nullptr;
        this->active = 0;
        this->soundPlaying = 0;
        Globals::sound->stop(0);
    } else {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->beamGeometry->transform))
                ->Update((long long) frameTime, false);

        this->direction = this->grabbedCrate->crateGeometry->getPosition();
        this->direction -= level->getPlayer()->getPosition();
        float distance = VectorLength(this->direction);

        // Beam emitter offset of the ships with a non-central tractor beam.
        Vector offset = Globals::status->getShip()->getIndex() == 44
                        ? level->getPlayer()->GetDirVector() * -300.0f
                        : Globals::status->getShip()->getIndex() == 49
                          ? level->getPlayer()->GetDirVector() * -300.0f + level->getPlayer()->GetUpVector() * 200.0f
                          : Vector{0.0f, 0.0f, 0.0f};

        this->beamGeometry->setScaling(0.5f, 0.5f, distance);
        this->beamGeometry->setDirection(VectorNormalize(this->direction - offset), Vector{0.0f, 1.0f, 0.0f});
        this->beamGeometry->setPosition(level->getPlayer()->getPosition() + offset);

        if (!(distance <= 400.0f)) {
            radar->field_0x1c = nullptr;
            this->direction = VectorNormalize(this->direction);
            this->direction *= (float) (frameTime * 10);
            this->grabbedCrate->crateGeometry->translate(-this->direction);
            if ((this->grabbedCrate->isDead() || this->grabbedCrate->isDying()) &&
                !this->grabbedCrate->field_0x40 && !this->grabbedCrate->deadFlag)
                this->grabbedCrate->translate(-this->direction);
            if (!this->soundPlaying) {
                Globals::sound->play(0, nullptr, nullptr, 0.0f);
                this->soundPlaying = 1;
            }
        } else {
            this->grabbedCrate->captureCrate(hud);
            this->active = 0;
            this->grabbedCrate = nullptr;
            radar->field_0x1c = nullptr;
            radar->dockNavPtr = nullptr;
            this->soundPlaying = 0;
            Globals::sound->stop(0);
            Globals::sound->play(4, nullptr, nullptr, 0.0f);
        }
    }
}

void TractorBeam::render() {
    if (!this->active)
        return;
    this->beamGeometry->render();
}
