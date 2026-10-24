#include "game/weapons/RepairBeam.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "game/core/Globals.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/ship/Ship.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/world/Level.h"
#include "game/ship/KIPlayer.h"
#include "engine/audio/FModSound.h"

// Weapon sound event per item index (header-static table, one copy per TU in the original).
static const int kWeaponSounds[233] = {
    52, 53, 54, 56, 55, 57, 58, 59, 60, 50, 49, 50,
    78, 75, 79, 77, 72, 73, 74, 76, 80, 81, 66, 67,
    63, 64, 68, 65, 69, 70, 71, 82, 83, 84, 85, 86,
    87, 88, 89, 90, 91, 6, 7, 8, 9, 10, 11, 92,
    93, 94, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    1098, 1100, 1099, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, 1111, 1113, 1112, 1117,
    1095, 1096, 1097, 1101, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 1118, -1, -1, -1, 2254, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, 2271, 2272, -1, -1, 2263, 2263, 2263, 2265, 2265,
    2265, -1, -1, -1, -1, 2254, 2267, 2268, 2252, -1, 2269, -1,
    2275, 2276, 2278, 2277, 2280,
};





RepairBeam::RepairBeam(int itemIndex, int sort) {
    this->beamPosition.x = 0;
    this->beamPosition.y = 0;
    this->beamPosition.z = 0;
    this->sort = sort;
    this->itemIndex = itemIndex;

    int count = Globals::status->getShip()->getFirstEquipmentOfSort(this->sort)->getAttribute(
            /*RepairBeamCount*/ 0x37);

    this->geometries = new Array<AEGeometry *>();
    ArraySetLength<AEGeometry *>(count, *this->geometries);

    uint16_t geoId = (sort == 0x25) ? 0x4a94 : 0x4a95;
    for (int i = 0; i < count; ++i)
        (*this->geometries)[i] = new AEGeometry(geoId, Globals::Canvas, false);

    this->targetIds = new Array<int>();
    ArraySetLength(count, *(this->targetIds));
    this->timer = 0x9c4;
    for (unsigned k = 0; k < this->targetIds->size(); ++k)
        (*this->targetIds)[k] = -1;

    this->charges = new Array<float>();
    ArraySetLength<float>(count, *this->charges);
    for (unsigned j = 0; j < this->charges->size(); ++j)
        (*this->charges)[j] = 0.0f;
}

RepairBeam::~RepairBeam() {
    if (this->geometries != nullptr) {
        ArrayReleaseClasses(*this->geometries);
        delete this->geometries;
        this->geometries = nullptr;
    }
    delete this->targetIds;
    this->targetIds = nullptr;
    delete this->charges;
    this->charges = nullptr;
}

void RepairBeam::update(int dt, Radar * /*radar*/, Level *level, Hud * /*hud*/) {
    Array<KIPlayer *> *enemies = level->getEnemies();
    this->timer -= dt;
    if (enemies != nullptr && !level->getPlayer()->isDead()) {
        int range = Globals::status->getShip()->getFirstEquipmentOfSort(this->sort)->getAttribute(0x35);

        if (this->timer < 0) {
            // Re-pick targets every 2.5s.
            for (unsigned i = 0; i < this->targetIds->size(); ++i) {
                (*this->targetIds)[i] = -1;
                (*this->charges)[i] = 0.0f;
            }
            this->timer += 2500;

            for (unsigned e = 0; e < enemies->size(); ++e) {
                KIPlayer *candidate = (*enemies)[e];
                if (candidate->isDead() || candidate->isDying())
                    continue;
                if (!((this->sort == 0x25 && candidate->player->carriesFriendCargoFlag != 0 &&
                       candidate->player->getHitpoints() < candidate->player->getMaxHitpoints()) ||
                      (this->sort == 0x29 && candidate->field_0x74 == 0 &&
                       candidate->player->enemyFlagsLo != 0 &&
                       level->getPlayer()->player->getShieldDamageRate() < 100 &&
                       Globals::status->getShip()->getFirstEquipmentOfSort(9) != nullptr)))
                    continue;

                this->beamPosition = candidate->player->getPosition();
                this->beamPosition -= level->getPlayer()->getPosition();
                if (!(AbyssEngine::AEMath::VectorLength(this->beamPosition) <= (float) range))
                    continue;

                Array<int> *targets = this->targetIds;
                bool placed = false;
                for (unsigned slot = 0; !placed && slot < targets->size(); ++slot) {
                    if ((*targets)[slot] == -1) {
                        (*targets)[slot] = e;
                        placed = true;
                    } else {
                        placed = false;
                    }
                }
                if (placed)
                    continue;

                // All beams busy: take over the healthiest target that is still worse off than us.
                int hitpoints = candidate->player->getHitpoints();
                int lowest = 9999999;
                int replace = -1;
                for (unsigned slot = 0; slot < this->targetIds->size(); ++slot) {
                    if ((*this->targetIds)[slot] == -1)
                        continue;
                    int other = (*enemies)[(*this->targetIds)[slot]]->player->getHitpoints();
                    if (hitpoints < other && other < lowest) {
                        lowest = other;
                        replace = slot;
                    }
                }
                if (replace != -1)
                    (*this->targetIds)[replace] = e;
            }
        }

        float healStep = (float) dt * 0.03f;
        float shieldStep = (float) dt * 0.01f;
        int idle = 1;
        for (unsigned i = 0; i < this->targetIds->size(); ++i) {
            if ((*this->targetIds)[i] == -1)
                continue;

            static_cast<AbyssEngine::Transform *>(
                    Globals::Canvas->TransformGetTransform((*this->geometries)[i]->transform))
                    ->Update(dt, false);

            this->beamPosition = (*enemies)[(*this->targetIds)[i]]->player->getPosition();
            // Aim point on the target (big hulls get the beam on their flank).
            KIPlayer *target = (*enemies)[(*this->targetIds)[i]];
            Vector targetOffset =
                    target->shipGroupFlag == 0x2c
                            ? AbyssEngine::AEMath::VectorNormalize(target->geometry->getDirection()) * -300.0f
                    : target->shipGroupFlag == 0x31
                            ? AbyssEngine::AEMath::VectorNormalize(target->geometry->getDirection()) * -300.0f +
                                      AbyssEngine::AEMath::VectorNormalize(
                                              (*enemies)[(*this->targetIds)[i]]->geometry->getUpVector()) * 200.0f
                            : Vector{0.0f, 0.0f, 0.0f};
            this->beamPosition += targetOffset;
            this->beamPosition -= level->getPlayer()->getPosition();

            // Emitter offset on our own ship (rear-mounted on these two hulls).
            Vector egoOffset =
                    Globals::status->getShip()->getIndex() == 0x2c
                            ? level->getPlayer()->GetDirVector() * -300.0f
                    : Globals::status->getShip()->getIndex() == 0x31
                            ? level->getPlayer()->GetDirVector() * -300.0f + level->getPlayer()->GetUpVector() * 200.0f
                            : Vector{0.0f, 0.0f, 0.0f};

            (*this->geometries)[i]->setScaling(0.5f, 0.5f, AbyssEngine::AEMath::VectorLength(this->beamPosition));
            (*this->geometries)[i]->setDirection(
                    AbyssEngine::AEMath::VectorNormalize(this->beamPosition - egoOffset), Vector{0.0f, 1.0f, 0.0f});
            (*this->geometries)[i]->setPosition(level->getPlayer()->getPosition() + egoOffset);
            static_cast<AbyssEngine::Transform *>(
                    Globals::Canvas->TransformGetTransform((*this->geometries)[i]->transform))
                    ->Update(dt, false);

            if (this->sort == 0x25) {
                (*enemies)[(*this->targetIds)[i]]->player->heal(
                        healStep *
                        (float) Globals::status->getShip()->getFirstEquipmentOfSort(this->sort)->getAttribute(0x36) /
                        100.0f);
            }
            if (this->sort == 0x29) {
                if (level->getPlayer()->player->getShieldDamageRate() < 100) {
                    (*this->charges)[i] +=
                            shieldStep *
                            (float) Globals::status->getShip()->getFirstEquipmentOfSort(this->sort)->getAttribute(0x36) /
                            100.0f;
                    if ((*this->charges)[i] >= 1.0f) {
                        (*enemies)[(*this->targetIds)[i]]->player->damage(1, false, -1);
                        (*this->charges)[i] += -1.0f;
                    }
                    level->getPlayer()->player->regenerateShield(
                            shieldStep *
                            (float) Globals::status->getShip()->getFirstEquipmentOfSort(this->sort)->getAttribute(0x36) /
                            100.0f);
                }
            }

            if (!Globals::sound->isPlaying(kWeaponSounds[this->itemIndex]))
                Globals::sound->play(kWeaponSounds[this->itemIndex], nullptr, nullptr, 0.0f);
            idle = 0;
        }
        if (idle)
            Globals::sound->stop(kWeaponSounds[this->itemIndex]);
    }

    if (level->getPlayer()->isDead() && Globals::sound->isPlaying(kWeaponSounds[this->itemIndex]))
        Globals::sound->stop(kWeaponSounds[this->itemIndex]);

    if (Globals::options[0xf] != 0 && Globals::sound->isPlaying(kWeaponSounds[this->itemIndex])) {
        int sound = kWeaponSounds[this->itemIndex];
        this->beamPosition = level->getPlayer()->getPosition();
        Globals::sound->updateEvent3DAttributes(sound, &this->beamPosition, nullptr, false);
    }
}

void RepairBeam::render() {
    for (unsigned i = 0; i < this->targetIds->size(); ++i) {
        if ((*this->targetIds)[i] != -1)
            (*this->geometries)[i]->render();
    }
}

