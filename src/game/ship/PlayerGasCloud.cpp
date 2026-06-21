#include "game/ship/PlayerGasCloud.h"
#include "platform/libc.h"
#include "engine/core/AERandom.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "engine/math/Transform.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/mission/Mission.h"
#include "game/ui/Hud.h"
#include "game/world/Level.h"
#include "game/ship/Ship.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "game/core/PaintCanvasClass.h"
#include "game/world/Station.h"

// VectorLength is an AEMath engine helper not surfaced by AEGeometry.h.
namespace AbyssEngine { namespace AEMath {
float VectorLength(const Vector& value);
} }

using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::operator-;
using AbyssEngine::AEMath::VectorNormalize;
using AbyssEngine::AEMath::VectorLength;
using AbyssEngine::AEMath::MatrixGetDir;
using AbyssEngine::AEMath::MatrixGetUp;

// Status singleton, reached via the global Status pointer (same pattern as Hud/Player).

// Engine globals reached through the GOT.
extern PaintCanvas** g_pgc_canvas;        // paint canvas (intact-cloud geometry)
extern void*  g_pgc_itemList;      // root item table
extern PaintCanvas** g_pgc_canvasRoot;    // paint canvas (spark geometry)
extern AbyssEngine::AERandom** g_pgc_rng; // random generator
extern PaintCanvas** g_pgc_canvas2;       // paint canvas (render camera)
extern PaintCanvas** g_pgcu_canvasRoot;   // paint canvas (update transforms)
extern void*  g_pgcu_itemDefs;     // item-definition table
extern FModSound* g_pgcu_pickupSound;     // pickup FModSound
extern void*  g_pgcu_campaign;     // campaign state

// Tuning constants.
extern float g_pgc_countScale;
extern float g_pgc_attrDiv;
extern float g_pgc_spread;
extern float g_pgc_lifeDiv;
extern float g_pgcu_velScale;
extern float g_pgcu_catchDist;
extern int   g_pgcu_minTimer;
extern int   g_pgcu_resetTimer;
extern float g_pgcu_fadeLo;
extern float g_pgcu_fadeAdd;
extern float g_pgcu_fadeDiv;
extern float g_pgcu_growDiv;

PlayerGasCloud::PlayerGasCloud(int itemId, ParticleSystemManager* /*particles*/,
                               AEGeometry* geometry, const Vector& position)
    : KIPlayer(itemId, -1, new Player(0, 9999999, 0, 0, 0), geometry,
               position.x, position.y, position.z, false)
{
    // player/geometry and setKIPlayer are wired by the KIPlayer base ctor above.
    this->center.x = 0;
    this->center.y = 0;
    this->center.z = 0;
    this->player->setMaxHitpoints(1);

    this->setPosition(position);

    this->elapsedSinceExplosion = 0;
    this->crateGeometry = 0;   // KIPlayer base slot (was field_0x78)
    this->field_0x25 = 0;

    int cloudMesh = 0x4a35;
    if ((unsigned int)(itemId - 0xc9) < 4)
        cloudMesh = itemId + 0x496c;
    int sparkMesh = 0x4a39;
    if ((unsigned int)(itemId - 0xc9) < 4)
        sparkMesh = itemId + 0x4970;
    this->itemId = itemId;
    this->sparkMeshId = sparkMesh;
    this->cloudMeshId = cloudMesh;

    this->sparkGeometries = 0;
    this->sparkVelocities = 0;
    this->sparkLife = 0;
    this->sparkLifeMin = 0;
    this->sparkScale = 0;
    this->sparkTimers = 0;
    this->sparkInSight = 0;
    this->exploded = 0;

    this->center = position;

    this->modelGeometry = new AEGeometry(this->cloudMeshId, *g_pgc_canvas, false);
    this->modelGeometry->setPosition(position);

    this->hasCargo = 1;   // KIPlayer base slot (was field_0x4c)
    this->field_0x44 = 1;
    this->state = 0;
    this->visibleFlag = 1;   // KIPlayer base slot (was active)
    this->settled = 0;
}

PlayerGasCloud::~PlayerGasCloud()
{
    if (this->sparkGeometries != 0) {
        for (AEGeometry* g : *this->sparkGeometries)
            delete g;
        this->sparkGeometries->clear();
        delete this->sparkGeometries;
        this->sparkGeometries = 0;
    }

    if (this->sparkVelocities != 0) {
        for (Vector* v : *this->sparkVelocities)
            delete v;
        this->sparkVelocities->clear();
        delete this->sparkVelocities;
        this->sparkVelocities = 0;
    }

    delete this->sparkLife;
    this->sparkLife = 0;

    delete this->sparkLifeMin;
    this->sparkLifeMin = 0;

    delete this->sparkScale;
    this->sparkScale = 0;

    delete this->sparkTimers;
    this->sparkTimers = 0;

    delete this->modelGeometry;
    this->modelGeometry = 0;
}

void PlayerGasCloud::translate(const Vector& v)
{
    this->geometry->translate(v);
}

bool PlayerGasCloud::isSparkAlive(int index)
{
    Array<AEGeometry*>* arr = this->sparkGeometries;
    if (arr == 0)
        return false;
    if (arr->size() <= (unsigned int)index)
        return false;
    return -1500 < (*this->sparkTimers)[index];
}

void PlayerGasCloud::setSparkInSight(int index, bool inSight)
{
    Array<AEGeometry*>* arr = this->sparkGeometries;
    if (arr == 0)
        return;
    if ((unsigned int)index < arr->size())
        (*this->sparkInSight)[index] = inSight;
}

void PlayerGasCloud::setPosition(const Vector& position)
{
    this->geometry->setPosition(position);
}

void* PlayerGasCloud::getSparks()
{
    return this->sparkGeometries;
}

Vector PlayerGasCloud::getPosition()
{
    return this->geometry->getPosition();
}

uint8_t PlayerGasCloud::hasExploded()
{
    return this->exploded;
}

void PlayerGasCloud::explode(int itemIndex, Vector src, float radius)
{
    if (this->exploded != 0)
        return;

    this->state = 3;
    this->exploded = 1;

    this->sparkGeometries = new Array<AEGeometry*>();
    this->sparkVelocities = new Array<Vector*>();
    this->sparkLife = new Array<float>();
    this->sparkLifeMin = new Array<float>();
    this->sparkTimers = new Array<int>();
    this->sparkInSight = new Array<bool>();
    this->sparkScale = new Array<float>();

    // Distance from the explosion source to the cloud centre.
    Vector delta = src - this->center;
    float dist = VectorLength(delta);
    float t = 1.5f - dist / radius;
    float countBase = t * g_pgc_countScale;

    // Item-defined intensity multiplier.
    void* itemTable = *(void**)((char*)*(void**)g_pgc_itemList);
    Item* item = *(Item**)((char*)*(void**)((char*)itemTable + 4) + itemIndex * 4);
    int attr = item->getAttribute(0);
    float attrF = (float)attr / g_pgc_attrDiv;
    float spread = t * g_pgc_spread;
    float lifeDiv = g_pgc_lifeDiv;

    AbyssEngine::AERandom* rng = *g_pgc_rng;

    int count = (int)((countBase / 1.5f + 10.0f) * attrF);
    for (int i = 0; i < count; i++) {
        AEGeometry* shard = new AEGeometry(this->sparkMeshId,
                                           *g_pgc_canvasRoot, false);
        shard->setPosition(this->center);

        float jx = (float)rng->next(10000);
        float jy = (float)rng->next(10000);
        float jz = (float)rng->next(10000);

        Vector p;
        p.x = ((this->center.x + delta.x) - spread) + t * jx;
        p.y = ((this->center.y + delta.y) - spread) + t * jy;
        p.z = ((this->center.z + delta.z) - spread) + t * jz;

        // Direction = normalized (p - center).
        Vector d = p - this->center;
        Vector dn;
        VectorNormalize(&dn, &d);

        float life = ((float)rng->next(200) / lifeDiv) * 3.0f + 3.0f;
        int timer = rng->next(14000);

        this->sparkLife->push_back(life * 7.0f);
        this->sparkLifeMin->push_back(life);
        this->sparkTimers->push_back(timer + 8000);

        this->sparkVelocities->push_back(new Vector(dn));
        this->sparkGeometries->push_back(shard);
        this->sparkInSight->push_back(false);
        this->sparkScale->push_back(1.0f);
    }
}

void PlayerGasCloud::update(int dt)
{
    if (dt == 0)
        return;

    Array<AEGeometry*>* arr = this->sparkGeometries;
    if (this->exploded == 0 || this->settled != 0 || arr == 0) {
        // Idle / pre-explosion: just advance the bound transform.
        AbyssEngine::Transform* t = (AbyssEngine::Transform*)
            (*g_pgcu_canvasRoot)->TransformGetTransform(
                *(unsigned int*)((char*)this->modelGeometry + 0xc));
        t->Update(1, (bool)dt);
        return;
    }

    float dtf = (float)dt;
    this->settled = 1;
    this->elapsedSinceExplosion += dt;
    float velStep = dtf * g_pgcu_velScale;

    unsigned int i = 0;
    while (i < arr->size()) {
        int* timer = &(*this->sparkTimers)[i];
        *timer -= dt;

        float* life = &(*this->sparkLife)[i];
        float newLife = *life - velStep;
        *life = newLife;
        float lifeMin = (*this->sparkLifeMin)[i];
        if (newLife < lifeMin)
            *life = lifeMin;

        // Distance from this shard to the player's turret.
        PlayerEgo* player = (PlayerEgo*)(intptr_t)this->level->getPlayer();
        Vector turretPos = player->getTurretPosition();
        Vector shardPos;
        Vector delta;
        AEGeometry* geom = (*this->sparkGeometries)[i];
        shardPos = geom->getPosition();
        delta = turretPos - shardPos;
        float dist = VectorLength(delta);

        bool collected = false;
        if (dist < g_pgcu_catchDist && this->elapsedSinceExplosion >= 2000) {
            PlayerEgo* p2 = (PlayerEgo*)(intptr_t)this->level->getPlayer();
            if (p2->isInTurretMode() != 0 &&
                (*this->sparkTimers)[i] >= g_pgcu_minTimer) {
                (*this->sparkTimers)[i] = g_pgcu_resetTimer;
                Ship* ship = gStatus->getShip();
                int itemId = this->itemId;
                if (ship->getFreeSpace() < 1) {
                    if (this->level->getPlayer() != 0) {
                        g_pgcu_pickupSound->stop(0x8d0);
                        g_pgcu_pickupSound->play(0x8d0, 0, 0, 0.0f);
                        PlayerEgo* ego = (PlayerEgo*)(intptr_t)this->level->getPlayer();
                        Hud* hud = (Hud*)(intptr_t)ego->getHUD();
                        hud->catchCargo(this->itemId, false, true, false, true, false, 0, 0);
                    }
                } else {
                    Item* def = *(Item**)((char*)*(void**)((char*)*(void**)g_pgcu_itemDefs + 4) + itemId * 4);
                    def->makeItem();
                    if (this->level->getPlayer() != 0) {
                        PlayerEgo* ego = (PlayerEgo*)(intptr_t)this->level->getPlayer();
                        Hud* hud = (Hud*)(intptr_t)ego->getHUD();
                        hud->catchCargo(this->itemId, true, false, false, false, false, 0, 0);

                        char* camp = (char*)*(void**)g_pgcu_campaign;
                        if (camp[0x2d] == 0 &&
                            gStatus->getCurrentCampaignMission() > 0x8e) {
                            Mission* mission = gStatus->getMission();
                            if (mission->isEmpty() != 0 && this->itemId == 0xcc) {
                                camp[0x2d] = 1;
                                this->level->createRadioMessage(0x1a, 0);
                            }
                        }
                    }
                    g_pgcu_pickupSound->stop(0x8d0);
                    g_pgcu_pickupSound->play(0x8d0, 0, 0, 0.0f);
                    Ship* ship2 = gStatus->getShip();
                    ship2->addCargo(def);
                }
                (*this->sparkScale)[i] = 0.0f;
                collected = true;
            }
        }

        if (!collected) {
            // Fade / grow the shard's scale based on its remaining timer.
            int tval = (*this->sparkTimers)[i];
            float* scale = &(*this->sparkScale)[i];
            if (tval <= 0) {
                float sc = (float)tval / g_pgcu_growDiv + 1.0f;
                *scale = sc;
                if (sc < 0.0f)
                    *scale = 0.0f;
            } else if (dist < g_pgcu_fadeLo) {
                *scale = (dist + g_pgcu_fadeAdd) / g_pgcu_fadeDiv;
            }
            AbyssEngine::Transform* t = (AbyssEngine::Transform*)
                (*g_pgcu_canvasRoot)->TransformGetTransform(
                    *(unsigned int*)((char*)geom + 0xc));
            t->Update(1, (bool)dt);
        }

        // Advance the shard's position.
        AEGeometry* moveGeom;
        Vector moved;
        bool homing = (*this->sparkInSight)[i] != 0 &&
                      this->elapsedSinceExplosion >= 2000;
        bool steered = false;
        if (homing) {
            Ship* ship = gStatus->getShip();
            if (ship->getFirstEquipmentOfSort(0x23) != 0) {
                // Steer toward the turret.
                Vector dir = turretPos - shardPos;
                Vector dn;
                VectorNormalize(&dn, &dir);
                *(*this->sparkVelocities)[i] = dn;
                moveGeom = (*this->sparkGeometries)[i];

                Ship* ship2 = gStatus->getShip();
                Item* eq = ship2->getFirstEquipmentOfSort(0x23);
                int attr = eq->getAttribute(0);
                float step = (float)(attr * dt);
                Vector vel = turretPos;
                vel *= step;
                vel += shardPos;
                moved = vel;
                moveGeom->setPosition(moved);
                steered = true;
            }
        }
        if (!steered) {
            moveGeom = (*this->sparkGeometries)[i];
            Vector vel = *(*this->sparkVelocities)[i];
            vel *= (*this->sparkLife)[i] * dtf;
            vel += shardPos;
            moved = vel;
            moveGeom->setPosition(moved);
        }

        if (g_pgcu_minTimer <= (*this->sparkTimers)[i])
            this->settled = 0;
        i++;
        arr = this->sparkGeometries;
    }

    if (this->settled != 0)
        this->visibleFlag = 0;
}

void PlayerGasCloud::render()
{
    char cameraLocal[60];

    if (this->visibleFlag == 0)
        return;
    int mode = this->state;
    if (mode != 3 && mode != 0)
        return;

    PaintCanvas* cam = *g_pgc_canvas2;
    unsigned int cur = cam->CameraGetCurrent();
    memcpy(cameraLocal, cam->CameraGetLocal(cur), 0x3c);

    const Matrix& camMatrix = *(const Matrix*)cameraLocal;
    Vector dir = MatrixGetDir(camMatrix);

    Array<AEGeometry*>* arr = this->sparkGeometries;
    if (this->exploded == 0 || arr == 0) {
        AEGeometry* geom = this->modelGeometry;
        geom->setDirection(dir, MatrixGetUp(camMatrix));
        geom->render();
    } else {
        for (unsigned int i = 0; i < arr->size(); i++) {
            AEGeometry* g = (*this->sparkGeometries)[i];
            g->setScaling((*this->sparkScale)[i]);
            g->setDirection(dir, MatrixGetUp(camMatrix));
            g->render();
            arr = this->sparkGeometries;
        }
    }
}

// Out-of-line container helpers the original emits for the spark arrays (Array<bool>
// sparkInSight, Array<float> sparkLife/Scale) and the mission/station tables reached
// through this translation unit.
template void ArrayAdd<bool>(bool, Array<bool>&);
template void ArrayAdd<float>(float, Array<float>&);
template void ArraySetLength<Mission*>(unsigned int, Array<Mission*>&);
template void ArraySetLength<Station*>(unsigned int, Array<Station*>&);

