#include "engine/core/ArrayReleaseClasses.h"
#include "game/core/CutScene.h"
#include "engine/core/AERandom.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/render/PaintCanvas.h"
#include "game/mission/Status.h"
#include "game/core/Globals.h"
#include "game/world/SolarSystem.h"
#include "engine/render/AEGeometry.h"
#include "game/mission/Item.h"
#include "engine/render/LODManager.h"
#include "engine/math/Transform.h"
#include "engine/core/ApplicationManager.h"
#include "game/world/Level.h"
#include "game/world/LevelScript.h"
#include "game/ship/PlayerEgo.h"
#include "game/ship/PlayerFighter.h"
#include "game/ship/Ship.h"
#include "engine/file/FileRead.h"

using AbyssEngine::AERandom;
using AbyssEngine::AnimationMode;

// Per-ship vertical offset of the hangar cutscene ship (original .rodata @ 0x1fb934).
static const int CUTSCENE_SHIP_HEIGHT[64] = {
    160, 150, 220, 310, 160, 180, 140, 170, 170, 110, 250, 205, 240, 250, 250, 250,
    170, 170, 190, 460, 170, 210, 140, 170, 160, 190, 170, 240, 220, 280, 200, 200,
    140, 160, 180, 150, 310, 390, 480, 200, 300, 300, 170, 200, 170, 370, 370, 300,
    440, 340, 170, 470, 170, 170, 170, 170, 200, 170, 250, 150, 170, 250, 270, 170,
};

CutScene::CutScene(int mode) {
    this->shipPosY = 0;
    this->shipPosZ = 0;
    this->mode = mode;
    this->level = nullptr;
    this->turretGeom = nullptr;
    this->followCamera = nullptr;
    this->cameraId6c = (uint32_t) -1;
    this->cameraId70 = (uint32_t) -1;
    this->initialized = 0;
    this->geometries = nullptr;
    this->vec8.x = 0.0f;
    this->vec8.y = 0.0f;
    this->vec8.z = 0.0f;
    this->vec8w = 0.0f;
    this->geom28 = nullptr;
    this->geom2c = nullptr;
    this->geom30 = nullptr;
    this->geom34 = nullptr;
    this->rotationSpeed = 0.00005f;
}

CutScene::~CutScene() {
    delete this->rootGeom;
    this->rootGeom = nullptr;

    delete this->turretGeom;
    this->turretGeom = nullptr;

    delete this->level;
    this->level = nullptr;

    Globals::Canvas->FogEnable(0, AbyssEngine::FogMode_1);

    delete this->geom28;
    this->geom28 = nullptr;
    delete this->geom2c;
    this->geom2c = nullptr;
    delete this->geom30;
    this->geom30 = nullptr;
    delete this->geom34;
    this->geom34 = nullptr;

    if (this->geometries != nullptr) {
        ArrayReleaseClasses(*this->geometries);
        delete this->geometries;
    }
    this->geometries = nullptr;
}

uint8_t CutScene::isInitialized() {
    return this->initialized;
}

void CutScene::update() {
    // The original forwards an uninitialised delta here (process() ignores it): the
    // tail call leaves r1 untouched.
    int delta;
    this->process(delta);
}

void CutScene::update(int delta) {
    this->process(delta);
}

void CutScene::render3D() {
    if (this->level != nullptr) {
        uint32_t t = (uint32_t) Globals::appManager->GetElapsedTimeMillis();
        this->frameDelta = t;
        this->level->update((long long) (int) t, 0u);
        this->level->render(this->frameDelta);
    }
    if (this->geom28 != nullptr) this->geom28->render();
    if (this->geom2c != nullptr) this->geom2c->render();
    if (this->geom30 != nullptr) this->geom30->render();
    if (this->geom34 != nullptr) this->geom34->render();

    if (this->geometries != nullptr) {
        for (unsigned i = 0; i < this->geometries->size(); ++i)
            (*this->geometries)[i]->render();
    }
}

void CutScene::render2D() {
    this->level->render2D();
}

void CutScene::process(int /*delta*/) {
    if (!this->initialized)
        return;

    this->currentTime = Globals::appManager->GetCurrentTimeMillis();
    this->frameDelta = (int) (this->currentTime - this->lastTime);
    this->totalTime += this->frameDelta;
    this->lastTime = this->currentTime;

    if (this->followCamera != nullptr)
        this->followCamera->update(30);

    if (this->mode == 2) {
        this->cameraRotX += this->rotationSpeed * (float) this->frameDelta;
        Matrix camera = *(Matrix *) Globals::Canvas->CameraGetLocal(this->cameraId74);
        AbyssEngine::AEMath::MatrixSetRotation(camera, 0.0f, this->cameraRotX, 0.0f);
        Globals::Canvas->CameraSetLocal(this->cameraId74, camera);

        if (Globals::status->getPlayingTime() > 0) {
            Array<KIPlayer *> *enemies = this->level->getEnemies();
            if (enemies != nullptr && enemies->size() >= 2) {
                KIPlayer *first = (*enemies)[enemies->size() - 2];
                if (first != nullptr && first->geometry != nullptr) {
                    first->geometry->translate((float) this->frameDelta * 0.1f, 0.0f,
                                               (float) this->frameDelta * 0.001f);
                    (*enemies)[enemies->size() - 2]->geometry->rotate((float) -this->frameDelta * 0.0002f,
                                                                      (float) this->frameDelta * 0.0002f,
                                                                      (float) -this->frameDelta * 0.00018f);
                }
                KIPlayer *second = (*enemies)[enemies->size() - 1];
                if (second != nullptr && second->geometry != nullptr) {
                    second->geometry->translate((float) this->frameDelta * 0.1f, 0.0f, 0.0f);
                    (*enemies)[enemies->size() - 1]->geometry->rotate((float) this->frameDelta * 0.0002f,
                                                                      (float) -this->frameDelta * 0.0005f,
                                                                      (float) -this->frameDelta * 0.0003f);
                }
            }
        }
    } else if (this->mode == 23) {
        int kind;
        if (Globals::status->getStation()->getIndex() == 101)
            kind = 10;
        else if (Globals::status->getStation()->getIndex() == 100)
            kind = 7;
        else
            kind = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() | 2;

        this->fogTimer84 += this->frameDelta;
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 1) {
            Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 30000.0f, 1.0f, 0x11e0cff);
        } else if (kind == 2 && this->geometries != nullptr) {
            for (unsigned int i = 0; i < this->geometries->size(); ++i)
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->geometries)[i]->transform))->Update(this->frameDelta, false);

            if (this->fogTimer84 > 3000) {
                this->fogTimer84 = 0;
                for (unsigned int i = 0; i < this->geometries->size(); ++i) {
                    if (Globals::rnd->nextInt(100) < 20 &&
                        !((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->geometries)[i]->transform))->IsRunning()) {
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->geometries)[i]->transform))->SetAnimationState(AbyssEngine::AnimationMode(3), nullptr);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->geometries)[i]->transform))->SetAnimationState(AbyssEngine::AnimationMode(1), nullptr);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->geometries)[i]->childTransform))->SetAnimationState(AbyssEngine::AnimationMode(3), nullptr);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->geometries)[i]->childTransform))->SetAnimationState(AbyssEngine::AnimationMode(1), nullptr);
                    }
                }
            }
        }

        Matrix local = *(Matrix *) Globals::Canvas->TransformGetLocal(
                (*this->level->getEnemies())[0]->geometry->transform);
        AbyssEngine::AEMath::MatrixSetRotation(local, 0.0f, this->cameraRotX, 0.0f);
        Globals::Canvas->TransformSetLocal((*this->level->getEnemies())[0]->geometry->transform, local);

        for (unsigned int i = 0; i < this->level->getEnemies()->size(); ++i) {
            if ((*this->level->getEnemies())[i] != nullptr)
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->level->getEnemies())[i]->geometry->transform))->Update(this->frameDelta, false);
        }
    } else if (this->mode == 4) {
        for (unsigned int i = 0; i < this->level->getEnemies()->size(); ++i) {
            if ((*this->level->getEnemies())[i] != nullptr)
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->level->getEnemies())[i]->geometry->transform))->Update(this->frameDelta, false);
        }

        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 0) {
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom30->transform))->Update(this->frameDelta, false);
        } else if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 1) {
            Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 5000.0f, 1.0f, 0x11e0cff);
            Globals::Canvas->FogEnable(true, AbyssEngine::FogMode_1);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                    (*this->level->getEnemies())[this->level->getEnemies()->size() - 3]->geometry->transform))
                    ->Update(this->frameDelta, false);

            if (this->geom30 != nullptr) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom30->transform))->Update(this->frameDelta, false);
                this->animTimer7c += this->frameDelta;
                if (this->animTimer7c > 20000) {
                    this->animTimer7c = 0;
                    if (Globals::rnd->nextInt(100) < 100) {
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom30->transform))->SetAnimationState(AbyssEngine::AnimationMode(3), nullptr);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom30->transform))->SetAnimationState(AbyssEngine::AnimationMode(1), nullptr);
                    }
                }
            }
            if (this->geom34 != nullptr) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom34->transform))->Update(this->frameDelta, false);
                this->animTimer80 += this->frameDelta;
                if (this->animTimer80 > 22000) {
                    this->animTimer80 = 0;
                    if (Globals::rnd->nextInt(100) < 100) {
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom34->transform))->SetAnimationState(AbyssEngine::AnimationMode(3), nullptr);
                        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom34->transform))->SetAnimationState(AbyssEngine::AnimationMode(1), nullptr);
                    }
                }
            }
        } else if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 3) {
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                    (*this->level->getEnemies())[this->level->getEnemies()->size() - 3]->geometry->transform))
                    ->Update(this->frameDelta, false);
            if (this->geom28 != nullptr)
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom28->transform))->Update(this->frameDelta, false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom2c->transform))->Update(this->frameDelta, false);

            this->animTimer7c += this->frameDelta;
            this->animTimer80 += this->frameDelta;
            if (this->geom28 != nullptr && this->animTimer7c > 1000) {
                this->animTimer7c = 0;
                if (Globals::rnd->nextInt(100) < 40) {
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom28->transform))->SetAnimationState(AbyssEngine::AnimationMode(3), nullptr);
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom28->transform))->SetAnimationState(AbyssEngine::AnimationMode(1), nullptr);
                }
            }
            if (this->animTimer80 > 2000) {
                this->animTimer80 = 0;
                if (Globals::rnd->nextInt(100) < 30) {
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom2c->transform))->SetAnimationState(AbyssEngine::AnimationMode(3), nullptr);
                    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom2c->transform))->SetAnimationState(AbyssEngine::AnimationMode(1), nullptr);
                }
            }
        } else if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 2) {
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                    (*this->level->getEnemies())[this->level->getEnemies()->size() - 2]->geometry->transform))
                    ->Update(this->frameDelta, false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(
                    (*this->level->getEnemies())[this->level->getEnemies()->size() - 1]->geometry->transform))
                    ->Update(this->frameDelta, false);
        }
    }
}

void CutScene::renderBG() {
    this->level->renderBG((int) this->frameDelta);
}

void CutScene::replacePlayerShip(int shipIndex, int race) {
    if (this->level->getEnemies() == nullptr)
        return;

    AEGeometry *oldGeom = (*this->level->getEnemies())[0]->geometry;
    if (oldGeom != nullptr) {
        if (this->turretGeom != nullptr)
            Globals::Canvas->TransformRemoveChild((*this->level->getEnemies())[0]->geometry->transform,
                                                  this->turretGeom->transform);

        Matrix matrix = oldGeom->getMatrix();
        (*this->level->getEnemies())[0]->geometry = Globals::globals->getShipGroup(shipIndex, race, false);
        (*this->level->getEnemies())[0]->geometry->setMatrix(matrix);
        (*this->level->getEnemies())[0]->setPosition(0.0f, (float) CUTSCENE_SHIP_HEIGHT[shipIndex], 0.0f);
        ((PlayerFighter *) (*this->level->getEnemies())[0])->setExhaustVisible(false);
        this->level->lodManager->removeObject(oldGeom);
        delete oldGeom;
    }

    this->checkForTurret();
}

void CutScene::initialize() {
    if (this->level == nullptr)
        this->level = new Level(this->mode);
    while (!this->level->init()) {
    }

    this->player = this->level->getPlayer();
    if (this->player != nullptr)
        this->player->setActive(false);

    this->level->initParticleSystems();

    if (this->mode == 4) {
        Globals::Canvas->CameraCreate(this->cameraId6c);
        Globals::Canvas->CameraSetPerspective(this->cameraId6c, 1.2f, 200.0f, 48000.0f);
        if (this->followCamera != nullptr) {
            delete this->followCamera;
            this->followCamera = nullptr;
        }
        Globals::Canvas->CameraSetCurrent(this->cameraId6c);
        this->resetCamera();
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 3) {
            this->geom2c = new AEGeometry((uint16_t) 14038, Globals::Canvas, false);
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geom2c->transform))->SetAnimationState(AbyssEngine::AnimationMode(0), nullptr);
        } else if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 0) {
            this->geom30 = new AEGeometry((uint16_t) 14280, Globals::Canvas, false);
            this->geom34 = new AEGeometry((uint16_t) 14279, Globals::Canvas, false);
            this->geom30->addChild(this->geom34->transform);
            delete this->geom34;
            this->geom34 = nullptr;
        } else if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 2) {
        }
    } else if (this->mode == 2) {
        Globals::Canvas->CameraCreate(this->cameraId74);
        Globals::Canvas->CameraSetPerspective(this->cameraId74, 0.92f, 200.0f, 200000.0f);
        Globals::Canvas->CameraSetCurrent(this->cameraId74);
        Matrix camera = *(Matrix *) Globals::Canvas->CameraGetLocal(this->cameraId74);
        int offsetX = Globals::rnd->nextInt(20000);
        int cameraX = offsetX - 20000;
        int offsetZ = Globals::rnd->nextInt(60000);
        AbyssEngine::AEMath::MatrixSetTranslation(camera, (float) cameraX, 0.0f, (float) (offsetZ + 40000));
        this->cameraRotX = -0.7853982f;
        AbyssEngine::AEMath::MatrixSetRotation(camera, 0.0f, -0.7853982f, 0.0f);
        Globals::Canvas->CameraSetLocal(this->cameraId74, camera);

        if (Globals::status->getPlayingTime() > 0) {
            Array<KIPlayer *> *enemies = this->level->getEnemies();
            if (enemies != nullptr && enemies->size() >= 2 && (*enemies)[enemies->size() - 2] != nullptr &&
                (*enemies)[enemies->size() - 1] != nullptr) {
                (*enemies)[enemies->size() - 2]->setPosition(
                        Vector{(float) (offsetX - 24000), -30.0f, (float) (offsetZ + 39500)});
                (*enemies)[enemies->size() - 1]->setPosition(
                        Vector{(float) (offsetX - 23400), 50.0f, (float) (offsetZ + 38600)});
            }
        }
    } else if (this->mode == 23) {
        this->turretGeom = nullptr;
        Globals::Canvas->CameraCreate(this->cameraId70);
        Globals::Canvas->CameraSetPerspective(this->cameraId70, 0.8f, 200.0f, 100000.0f);
        Globals::Canvas->CameraSetCurrent(this->cameraId70);
        Matrix camera = *(Matrix *) Globals::Canvas->CameraGetLocal(this->cameraId70);
        AbyssEngine::AEMath::MatrixSetRotation(camera, 0.0f, 0.39269908f, 0.0f);
        AbyssEngine::AEMath::MatrixSetTranslation(camera, 300.0f, 700.0f, 3000.0f);
        Globals::Canvas->CameraSetLocal(this->cameraId70, camera);
        Globals::Canvas->TransformCreate(this->transformId78);
        Vector position = (*this->level->getEnemies())[0]->geometry->getPosition();
        AbyssEngine::AEMath::MatrixSetTranslation(*(Matrix *) Globals::Canvas->TransformGetLocal(this->transformId78),
                                                  position.x, position.y, position.z);
        Globals::Canvas->TransformAddChild(this->transformId78, this->cameraId70);
        this->resetCamera();
        this->checkForTurret();
    }

    this->rootGeom = new AEGeometry(Globals::Canvas);
    this->rootGeom->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
    this->totalTime = 0;
    this->animTimer7c = 0;
    this->animTimer80 = 0;
    this->fogTimer84 = 0;
    this->currentTime = Globals::appManager->GetCurrentTimeMillis();
    this->initialized = 1;
    this->lastTime = this->currentTime;
}

void CutScene::resetCamera() {
    if (this->mode == 4) {
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 1) {
            Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 5000.0f, 1.0f, 0x11e0cff);
            Globals::Canvas->FogEnable(true, AbyssEngine::FogMode_1);
        }
        Globals::Canvas->CameraSetCurrent(this->cameraId6c);
        Globals::Canvas->CameraSetPerspective(this->cameraId6c, 1.2f, 200.0f, 48000.0f);
    } else if (this->mode == 23) {
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 1) {
            Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 30000.0f, 1.0f, 0x11e0cff);
            Globals::Canvas->FogEnable(true, AbyssEngine::FogMode_1);
        }
        Globals::Canvas->CameraSetCurrent(this->cameraId70);
        Globals::Canvas->CameraSetPerspective(this->cameraId70, 0.8f, 200.0f, 100000.0f);
        (*this->level->getEnemies())[0]->getType();
    }
}

void CutScene::checkForTurret() {
    if (this->turretGeom != nullptr)
        Globals::Canvas->TransformRemoveChild((*this->level->getEnemies())[0]->geometry->transform,
                                              this->turretGeom->transform);

    Array<Item *> *turrets = Globals::status->getShip()->getEquipment(2);
    if (turrets == nullptr || turrets->size() == 0 || (*turrets)[0] == nullptr)
        return;

    // Per-turret meshes: base, barrel and up to three child parts, plus the offset of the
    // turret on its mounting point.
    int index = (*turrets)[0]->getIndex();
    int baseMesh = 0xffff;
    int barrelMesh = 0xffff;
    int baseChild = -1;
    int barrelChild = -1;
    int barrelChild2 = -1;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float offsetZ = 0.0f;
    switch (index) {
        case 47:
            baseMesh = 6770;
            barrelMesh = 6771;
            offsetY = 60.0f;
            break;
        case 224:
            baseMesh = 18842;
            barrelMesh = 18843;
            baseChild = 18844;
            barrelChild = 18845;
            offsetY = 82.0f;
            break;
        case 49:
            baseMesh = 6774;
            barrelMesh = 6775;
            offsetY = 60.0f;
            break;
        case 180:
            baseMesh = 6805;
            barrelMesh = 6806;
            offsetY = 84.0f;
            break;
        case 181:
            baseMesh = 6807;
            barrelMesh = 6808;
            offsetY = 88.0f;
            offsetZ = -11.0f;
            break;
        case 182:
            baseMesh = 6809;
            barrelMesh = 6810;
            offsetX = 34.0f;
            offsetY = 88.0f;
            offsetZ = 1.6f;
            break;
        case 198:
            baseMesh = 18787;
            barrelMesh = 18791;
            baseChild = 18788;
            barrelChild = 18790;
            offsetY = 80.0f;
            break;
        case 199:
            baseMesh = 18792;
            barrelMesh = 18795;
            baseChild = 18793;
            barrelChild = 18794;
            offsetY = 38.0f;
            break;
        case 200:
            baseMesh = 18796;
            barrelMesh = 18799;
            baseChild = 18797;
            barrelChild = 18798;
            barrelChild2 = 18800;
            offsetY = 75.0f;
            break;
        case 48:
            baseMesh = 6772;
            barrelMesh = 6773;
            offsetY = 55.0f;
            break;
        default:
            break;
    }

    AEGeometry *base = new AEGeometry((uint16_t) baseMesh, Globals::Canvas, false);
    AEGeometry *barrel = new AEGeometry((uint16_t) barrelMesh, Globals::Canvas, false);
    barrel->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
    if (baseChild != -1) {
        AEGeometry *part = new AEGeometry((uint16_t) baseChild, Globals::Canvas, false);
        base->addChild(part->transform);
        delete part;
    }
    if (barrelChild != -1) {
        AEGeometry *part = new AEGeometry((uint16_t) barrelChild, Globals::Canvas, false);
        barrel->addChild(part->transform);
        delete part;
    }
    if (barrelChild2 != -1) {
        AEGeometry *part = new AEGeometry((uint16_t) barrelChild2, Globals::Canvas, false);
        barrel->addChild(part->transform);
        delete part;
    }

    delete this->turretGeom;
    this->turretGeom = nullptr;
    this->turretGeom = new AEGeometry(Globals::Canvas);

    FileRead *fileRead = new FileRead();
    Array<Array<Vector *> *> *positions = fileRead->loadWeaponPositions(Globals::status->getShip()->getIndex());
    delete fileRead;

    base->setPosition(*(*(*positions)[2])[0]);
    barrel->setPosition(*(*(*positions)[2])[0]);
    barrel->translate(offsetX, offsetY, offsetZ);
    if ((*turrets)[0]->getIndex() < 198 || (*turrets)[0]->getIndex() > 200) {
        base->rotate(0.0f, 3.1415927f, 0.0f);
        barrel->rotate(0.0f, 3.1415927f, 0.0f);
    }

    this->turretGeom->addChild(base->transform);
    this->turretGeom->addChild(barrel->transform);
    (*this->level->getEnemies())[0]->geometry->addChild(this->turretGeom->transform);

    if (positions != nullptr) {
        for (unsigned int i = 0; i < positions->size(); ++i) {
            if ((*positions)[i] != nullptr) {
                ArrayReleaseClasses(*(*positions)[i]);
                delete (*positions)[i];
                (*positions)[i] = nullptr;
            }
        }
        ArrayReleaseClasses(*positions);
        delete positions;
    }
}
