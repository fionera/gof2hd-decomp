#include "engine/core/ArrayReleaseClasses.h"
#include "game/ship/PlayerStation.h"
#include "game/core/Globals.h"

#include "game/world/SolarSystem.h"
#include "engine/render/AEGeometry.h"
#include "engine/math/BoundingVolume.h"
#include "engine/math/BoundingAAB.h"
#include "engine/math/BoundingSphere.h"
#include "engine/file/FileRead.h"
#include "game/mission/Status.h"
#include "engine/math/Transform.h"
#include "game/ship/Player.h"
#include "game/world/Station.h"
#include "engine/render/PaintCanvas.h"

static inline PaintCanvas *paintCanvas() {
    return Globals::Canvas;
}

typedef Array<BoundingVolume *> BoundingVolumeList;

PlayerStation::PlayerStation(Station *station)
    : PlayerStaticFar(-1, nullptr, 0.0f, 0.0f, 0.0f) {
    int index;
    this->field_0x158 = 0;
    this->field_0x15c = 0;
    this->field_0x160 = 0;
    this->player->setRadius(15000);
    this->posX = 0;
    this->posY = 0;
    this->posZ = 0;
    this->boundingVolumes = nullptr;
    this->field_0x25 = 0;
    this->player->setMaxHitpoints(9999999);
    this->secondGeometry = nullptr;
    this->rootGeometry = nullptr;
    this->collisionRadius = 0;
    index = station->getIndex();
    this->stationIndex = index;

    FileRead *reader = new FileRead();
    Array<int> *collision = reader->loadStationCollision(index);
    delete reader;
    bool alienOrbit = Globals::status->inAlienOrbit();

    // stations without their own collision data use one of the generic station models
    if ((uint32_t) (index - 109) >= 2 && collision == nullptr) {
        if (!alienOrbit) {
            AEGeometry *&root = this->rootGeometry;
            root = new AEGeometry((uint16_t) 0x4034, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry((uint16_t) 0x4037, Globals::Canvas, false);
            root->addChild(part1->transform);
            AEGeometry *part2 = new AEGeometry((uint16_t) 0x403a, Globals::Canvas, false);
            root->addChild(part2->transform);
            delete part1;
            delete part2;
            FileRead *r1000 = new FileRead();
            collision = r1000->loadStationCollision(1000);
            delete r1000;
        } else if (Globals::status->dlc1Won()) {
            AEGeometry *&root = this->rootGeometry;
            root = new AEGeometry((uint16_t) 0x4220, Globals::Canvas, false);
            uint32_t t1 = 0xffffffff;
            Globals::Canvas->TransformCreate(t1);
            Globals::Canvas->TransformAddMesh(t1, 0x4221, false);
            root->addChild(t1);
            uint32_t t2 = 0xffffffff;
            Globals::Canvas->TransformCreate(t2);
            Globals::Canvas->TransformAddMesh(t2, 0x4222, false);
            root->addChild(t2);
            reader = new FileRead();
            collision = reader->loadStationCollision(1003);
            delete reader;
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->transform))
                    ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->transform))->animationLength,
                             false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->childTransform))
                    ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->childTransform))->animationLength,
                             false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->parentTransform))
                    ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(root->parentTransform))->animationLength,
                             false);
        } else {
            AEGeometry *&root = this->rootGeometry;
            root = new AEGeometry((uint16_t) 0x403b, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry((uint16_t) 0x403e, Globals::Canvas, false);
            root->addChild(part1->transform);
            AEGeometry *part2 = new AEGeometry((uint16_t) 0x4041, Globals::Canvas, false);
            root->addChild(part2->transform);
            delete part1;
            delete part2;
            FileRead *r1001 = new FileRead();
            collision = r1001->loadStationCollision(1001);
            delete r1001;
        }
    }

    if (this->rootGeometry == nullptr) {
        if ((uint32_t) (this->stationIndex - 109) <= 2) {
            this->rootGeometry = new AEGeometry((uint16_t) 0x5254, Globals::Canvas, false);
        } else {
            this->rootGeometry = new AEGeometry((uint16_t) (this->stationIndex + 21000), Globals::Canvas, false);
        }
        int race = Globals::status->getSystem() != 0
                           ? ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()
                           : 9;

        if (this->stationIndex == 101) {
            AEGeometry *&root = this->rootGeometry;
            root = new AEGeometry((uint16_t) 0x4220, Globals::Canvas, false);
            uint32_t base0 = 0xffffffff;
            Globals::Canvas->TransformCreate(base0);
            Globals::Canvas->TransformAddMesh(base0, 0x4221, false);
            root->addChild(base0);
            uint32_t event1;
            uint32_t base1 = 0xffffffff;
            Globals::Canvas->TransformCreate(base1);
            Globals::Canvas->TransformAddMesh(base1, 0x4222, false);
            root->addChild(base1);
            if (Globals::status->getCurrentCampaignMission() == 157 &&
                Globals::status->getStation()->getIndex() == 112) {
                uint32_t event0 = 0xffffffff;
                Globals::Canvas->TransformCreate(event0);
                Globals::Canvas->TransformAddMesh(event0, 0x4950, false);
                this->rootGeometry->addChild(event0);
                event1 = 0xffffffff;
                Globals::Canvas->TransformCreate(event1);
                Globals::Canvas->TransformAddMesh(event1, 0x4952, false);
                this->rootGeometry->addChild(event1);
                uint32_t event2 = 0xffffffff;
                Globals::Canvas->TransformCreate(event2);
                Globals::Canvas->TransformAddMesh(event2, 0x4951, false);
                this->rootGeometry->addChild(event2);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->transform))
                        ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->transform))->animationLength,
                                 false);
            } else if (Globals::status->getCurrentCampaignMission() >= 80) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->transform))
                        ->Update(((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->transform))->animationLength,
                                 false);
            }
        } else if (this->stationIndex == 111 && Globals::status->getCurrentCampaignMission() <= 93) {
            AEGeometry *&root = this->rootGeometry;
            root = new AEGeometry((uint16_t) 0x5254, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry((uint16_t) 0x5574, Globals::Canvas, false);
            root->addChild(part1->transform);
            AEGeometry *part2 = new AEGeometry((uint16_t) 0x563c, Globals::Canvas, false);
            root->addChild(part2->transform);
            AEGeometry *part3 = new AEGeometry((uint16_t) 0x495d, Globals::Canvas, false);
            root->addChild(part3->transform);
            AEGeometry *part4 = new AEGeometry((uint16_t) 0x495e, Globals::Canvas, false);
            root->addChild(part4->transform);
            delete part1;
            delete part2;
            delete part3;
            delete part4;
        } else if ((uint32_t) (this->stationIndex - 109) < 2 ||
                   (this->stationIndex == 111 && Globals::status->getCurrentCampaignMission() > 94)) {
            this->rootGeometry = new AEGeometry((uint16_t) 0x4953, Globals::Canvas, false);
            AEGeometry *part1 = new AEGeometry((uint16_t) 0x4954, Globals::Canvas, false);
            this->rootGeometry->addChild(part1->transform);
            AEGeometry *part2 = new AEGeometry((uint16_t) 0x4955, Globals::Canvas, false);
            this->rootGeometry->addChild(part2->transform);
            AEGeometry *part3 = new AEGeometry((uint16_t) 0x4956, Globals::Canvas, false);
            this->rootGeometry->addChild(part3->transform);
            Array<int> *oldCollision = collision;
            delete oldCollision;
            reader = new FileRead();
            collision = reader->loadStaticCollision(2002);
            delete reader;
            delete part1;
            delete part2;
            delete part3;
        } else if (this->stationIndex == 100) {
            int mission = Globals::status->getCurrentCampaignMission();
            bool dlcWon = Globals::status->dlc1Won();
            bool animated = mission == 80 || dlcWon;
            uint16_t rootMesh = mission == 80 ? 0x381f : (dlcWon ? 0x3823 : 0x4223);
            uint16_t mesh1 = animated ? 0x422a : 0x4228;
            uint16_t mesh2 = mission == 80 ? 0x3820 : (dlcWon ? 0x3824 : 0x4224);
            uint16_t mesh3 = mission == 80 ? 0x3821 : (dlcWon ? 0x3825 : 0x4225);
            this->rootGeometry = new AEGeometry(rootMesh, Globals::Canvas, false);
            uint32_t t0 = 0xffffffff;
            Globals::Canvas->TransformCreate(t0);
            Globals::Canvas->TransformAddMesh(t0, mesh1, false);
            this->rootGeometry->addChild(t0);
            this->meshTransform = t0;
            uint32_t t1 = 0xffffffff;
            Globals::Canvas->TransformCreate(t1);
            Globals::Canvas->TransformAddMesh(t1, mesh2, false);
            this->rootGeometry->addChild(t1);
            uint32_t t2 = 0xffffffff;
            Globals::Canvas->TransformCreate(t2);
            Globals::Canvas->TransformAddMesh(t2, mesh3, false);
            this->rootGeometry->addChild(t2);
            if (animated) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->transform))
                        ->SetAnimationState(AbyssEngine::AnimationMode_0, nullptr);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(t1))
                        ->SetAnimationState(AbyssEngine::AnimationMode_0, nullptr);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(t2))
                        ->SetAnimationState(AbyssEngine::AnimationMode_0, nullptr);
                Array<int> *oldCollision = collision;
                delete oldCollision;
                reader = new FileRead();
                collision = reader->loadStationCollision(1005);
                delete reader;
            } else {
                uint16_t lodMesh = 0x4226;
                int lodDist = 70000;
                this->rootGeometry->setLodMeshes(&lodMesh, &lodDist, 1);
                uint16_t lodChildMesh = 0x4227;
                this->rootGeometry->setLodChildMeshes(&lodChildMesh);
            }
        } else if (race == 0 || race == 2) {
            AEGeometry *part1 = new AEGeometry((uint16_t) (this->stationIndex + 21800), Globals::Canvas, false);
            this->rootGeometry->addChild(part1->transform);
            AEGeometry *part2 = new AEGeometry((uint16_t) (this->stationIndex + 22000), Globals::Canvas, false);
            this->rootGeometry->addChild(part2->transform);
            delete part1;
            delete part2;
        } else if (race == 3) {
            AEGeometry *part1 = new AEGeometry((uint16_t) (this->stationIndex + 21800), Globals::Canvas, false);
            this->rootGeometry->addChild(part1->transform);
            AEGeometry *part2 = new AEGeometry((uint16_t) (this->stationIndex + 22000), Globals::Canvas, false);
            this->rootGeometry->addChild(part2->transform);
            if (this->stationIndex == 108) {
                AEGeometry *anim = new AEGeometry((uint16_t) 0x5974, Globals::Canvas, false);
                this->rootGeometry->addChild(anim->transform);
                this->animTransform0 = anim->transform;
                anim = new AEGeometry((uint16_t) 0x5975, Globals::Canvas, false);
                this->rootGeometry->addChild(anim->transform);
                this->animTransform1 = anim->transform;
                anim = new AEGeometry((uint16_t) 0x5976, Globals::Canvas, false);
                this->rootGeometry->addChild(anim->transform);
                this->animTransform2 = anim->transform;
                anim = new AEGeometry((uint16_t) 0x5977, Globals::Canvas, false);
                this->rootGeometry->addChild(anim->transform);
                this->animTransform3 = anim->transform;
            }
            delete part1;
            delete part2;
        }
    }

    // collision data: count, then per volume a type tag (0 = sphere, 1 = box) and its integer parameters
    if (collision != nullptr) {
        int count = collision->data()[0];
        Vector extent = {0.0f, 0.0f, 0.0f};
        this->boundingVolumes = new BoundingVolumeList();
        ArraySetLength(count, *(BoundingVolumeList *) this->boundingVolumes);
        for (int cursor = 1, i = 0; i < count; i++) {
            BoundingVolume *volume;
            float x, y, z;
            switch (collision->data_[cursor++]) {
                case 0: {
                    x = (float) -collision->data_[cursor];
                    z = (float) collision->data_[cursor + 1];
                    y = (float) collision->data_[cursor + 2];
                    extent.x = (float) collision->data_[cursor + 3];
                    cursor += 4;
                    if (alienOrbit) {
                        extent *= 0.4f;
                    } else {
                        extent *= 0.5f;
                    }
                    if (extent.x < 0.0f) {
                        extent *= -1.0f;
                    }
                    volume = new BoundingSphere(x, y, z, 0.0f, 0.0f, 0.0f, extent.x);
                    break;
                }
                case 1: {
                    z = (float) collision->data_[cursor + 1];
                    y = (float) collision->data_[cursor + 2];
                    x = (float) -collision->data_[cursor];
                    extent.x = (float) collision->data_[cursor + 3];
                    extent.z = (float) collision->data_[cursor + 4];
                    extent.y = (float) collision->data_[cursor + 5];
                    cursor += 6;
                    if (alienOrbit) {
                        extent *= 0.9f;
                    } else {
                        extent *= 1.0f;
                    }
                    volume = new BoundingAAB(x, y, z, 0.0f, 0.0f, 0.0f, extent.x + extent.x,
                                             extent.y + extent.y, extent.z + extent.z);
                    break;
                }
                default:
                    continue;
            }
            (*(BoundingVolumeList *) this->boundingVolumes)[i] = volume;
        }
        ArrayRelease(*collision);
        delete collision;
    }

    this->field_0x71 = 1;
    this->rootGeometry->setRotation(0.0f, 3.1415927f, 0.0f);
    this->collisionRadius =
            (int) (((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->transform))
                           ->boundingSphere.radius +
                   5000.0f);
}

PlayerStation::~PlayerStation() {
    if (this->rootGeometry != nullptr) {
        delete this->rootGeometry;
    }
    this->rootGeometry = nullptr;

    if (this->secondGeometry != nullptr) {
        delete this->secondGeometry;
    }
    this->secondGeometry = nullptr;

    if (this->boundingVolumes != nullptr) {
        ArrayReleaseClasses(*(BoundingVolumeList *) this->boundingVolumes);
        delete (BoundingVolumeList *) this->boundingVolumes;
    }
    this->boundingVolumes = nullptr;
}

void PlayerStation::update(int delta) {
    if ((int32_t) this->rootGeometry->childTransform == -1 || this->stationIndex == 101) {
        return;
    }
    if (Globals::status->inAlienOrbit()) {
        return;
    }
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->rootGeometry->childTransform))
            ->Update((long long) delta, false);
    if (this->stationIndex == 108) {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->animTransform0))
                ->Update((long long) delta, false);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->animTransform1))
                ->Update((long long) delta, false);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->animTransform2))
                ->Update((long long) delta, false);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->animTransform3))
                ->Update((long long) delta, false);
    } else if (this->stationIndex == 100) {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->meshTransform))
                ->Update((long long) delta, false);
    }
}

void PlayerStation::render() {
    if (!this->visibleFlag) {
        return;
    }
    this->rootGeometry->render();
}

void PlayerStation::setPosition(const Vector &position) {
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
    this->rootGeometry->setPosition(position);

    BoundingVolumeList *volumes = (BoundingVolumeList *) this->boundingVolumes;
    if (volumes != nullptr) {
        for (uint32_t i = 0; i < volumes->size(); ++i) {
            (*volumes)[i]->update(position.x, position.y, position.z);
            volumes = (BoundingVolumeList *) this->boundingVolumes;
        }
    }
}

Vector PlayerStation::getPosition() {
    return this->rootGeometry->getPosition();
}

void *PlayerStation::getRoot() { // lint: void_ptr (exported method return type baked into ABI)
    // lint: void_ptr (exported method return type baked into ABI)
    return this->rootGeometry;
}

void PlayerStation::setVisible(bool visible) {
    this->visibleFlag = visible;
}

void PlayerStation::translate(float x, float y, float z) {
    this->rootGeometry->translate(x, y, z);
}

int PlayerStation::collide(float x, float y, float z) {
    return this->outerCollide(x, y, z);
}

int PlayerStation::outerCollide(float x, float y, float z) {
    float radius = (float) this->collisionRadius;

    if (x < this->posX + radius && this->posX - radius < x &&
        y < this->posY + radius && this->posY - radius < y &&
        z < this->posZ + radius && this->posZ - radius < z) {
        BoundingVolumeList *volumes = (BoundingVolumeList *) this->boundingVolumes;
        if (volumes != nullptr) {
            for (uint32_t i = 0; i < volumes->size(); ++i) {
                BoundingVolume *volume = (*volumes)[i];
                if (volume->outerCollide(x, y, z)) {
                    this->collisionIndex = i;
                    return true;
                }
                volumes = (BoundingVolumeList *) this->boundingVolumes;
            }
        }
    }

    return false;
}

int PlayerStation::outerCollide(const Vector &position) {
    return this->outerCollide(position.x, position.y, position.z);
}

Vector PlayerStation::getProjectionVector(const Vector &position) {
    BoundingVolumeList *volumes = (BoundingVolumeList *) this->boundingVolumes;
    if (volumes != nullptr) {
        BoundingVolume *volume = (*volumes)[this->collisionIndex];
        return volume->getProjectionVector(position);
    }

    Vector result = {0.0f, 0.0f, 0.0f};
    return result;
}

Vector PlayerStation::projectCollisionOnSurface(const Vector &position) {
    return BoundingVolume::staticProjectCollisionOnSurface(position, (BoundingVolumeList *) this->boundingVolumes);
}
