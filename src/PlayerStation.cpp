#include "gof2/PlayerStation.h"
#include "gof2/AEGeometry.h"
#include "gof2/BoundingVolume.h"
#include "gof2/FileRead.h"
#include "gof2/Status.h"
#include "gof2/Transform.h"
#include "gof2/Player.h"

// Transform / AEGeometry are only used as opaque pointers and via byte-offset reads
// on ambiguous locals (helper F below); avoid pulling in the conflicting Transform.h.
struct Transform;
struct PlayerStaticFar;

// Note: the byte-offset accessor F<T>(void*, off) is provided by gof2/common.h.
// The underlying classes (AEGeometry, Transform, BoundingVolume) aren't modeled as
// named structs here, so the recovered code accesses their fields positionally via F.

// Function-pointer globals used by update() — declared with their typedefs below.
typedef long long (*TransformGetFn)(void *canvas, int mesh);
typedef void (*TransformUpdateFn)(uint32_t lo, uint32_t hi, int delta, int deltaHigh,
                                  int zero);

// Base-class constructor of PlayerStaticFar, invoked at the head of PlayerStation's ctor.
extern "C" void PlayerStaticFar_ctor(void *self, int playerId, AEGeometry *geometry,
                                     float x, float y, float z);

extern "C" void *PlayerStation_vtable __attribute__((visibility("hidden")));
extern "C" void *AEGeometry_destructor(void *geometry) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_BoundingVolumePtr(void *array) __attribute__((nothrow));
extern "C" void *Array_BoundingVolumePtr_destructor(void *array) __attribute__((nothrow));
extern "C" void *Status_holder __attribute__((visibility("hidden")));
extern "C" void *PaintCanvas_holder __attribute__((visibility("hidden")));
namespace AbyssEngine { namespace PaintCanvas { void *TransformGetTransform(void *canvas, uint32_t handle); } }
extern "C" TransformGetFn PlayerStation_transformGet __attribute__((visibility("hidden")));
extern "C" TransformUpdateFn PlayerStation_transformUpdate __attribute__((visibility("hidden")));
extern "C" int Station_getIndex(Station *station);
extern "C" void *FileRead_constructor(void *self);
extern "C" void *FileRead_destructor(void *self) __attribute__((nothrow));
extern "C" int SolarSystem_getRace(void *system);
extern "C" void AEGeometry_constructor(void *self, uint16_t mesh, void *canvas, bool flag);
namespace AbyssEngine { namespace PaintCanvas { void TransformCreate(void *canvas, uint32_t *out); } }
extern "C" void Transform_UpdatePtr(void *transform, uint32_t lo, uint32_t hi, int zero);
extern "C" void Transform_SetActive(void *transform);
extern "C" void Transform_SetInactive(void *transform);
extern "C" void Array_int_destructor(void *array) __attribute__((nothrow));
extern "C" void ArrayRelease_int(void *array) __attribute__((nothrow));
extern "C" void Array_BoundingVolumePtr_constructor(void *array);
extern "C" void ArraySetLength_BoundingVolumePtr(uint32_t length, void *array);
extern "C" void Vector_scale(Vector *vector, float scale);
extern "C" void *PlayerStation_destructor_body(PlayerStation *self) __attribute__((nothrow));

// ---- setVisible_122760.cpp ----
void PlayerStation::setVisible(bool visible)
{
    this->visible = visible;
}

// ---- _PlayerStation_122588.cpp ----
PlayerStation::~PlayerStation() noexcept(false)
{
    P(this, 0x0) = (char *)PlayerStation_vtable + 8;

    void *geometry = P(this, 0x140);
    if (geometry != 0) {
        operator_delete(AEGeometry_destructor(geometry));
    }
    P(this, 0x140) = 0;

    geometry = P(this, 0x14c);
    if (geometry != 0) {
        operator_delete(AEGeometry_destructor(geometry));
    }
    P(this, 0x14c) = 0;

    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        ArrayReleaseClasses_BoundingVolumePtr(volumes);
        volumes = P(this, 0x130);
        if (volumes != 0) {
            operator_delete(Array_BoundingVolumePtr_destructor(volumes));
        }
    }
    P(this, 0x130) = 0;

}

// ---- setPosition_1226f8.cpp ----
typedef void (*BoundingVolumeSetPositionFn)(void *volume, float x, float y, float z);

void PlayerStation::setPosition(const Vector &position)
{
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
    ((AEGeometry *)(P(this, 0x140)))->setPosition(position);

    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        for (uint32_t i = 0; i < F<uint32_t>(volumes, 0x0); ++i) {
            void *volume = *(void **)((char *)P(volumes, 0x4) + i * 4);
            BoundingVolumeSetPositionFn fn =
                *(BoundingVolumeSetPositionFn *)((char *)P(volume, 0x0) + 0x4);
            fn(volume, position.x, position.y, position.z);
            volumes = P(this, 0x130);
        }
    }
}

// ---- projectCollisionOnSurface_12287c.cpp ----

Vector PlayerStation::projectCollisionOnSurface(const Vector &position)
{
    // staticProjectCollisionOnSurface seeds a BoundingVolume's center with `position`,
    // projects it onto every colliding volume in the list, and leaves the result in the
    // volume's center. Reconstruct that: run it on a scratch volume and return its center.
    BoundingVolume scratch(0, 0, 0, 0, 0, 0);
    scratch.staticProjectCollisionOnSurface(position,
                                            (Array<BoundingVolume *> *)P(this, 0x130));
    return *(Vector *)&scratch.centerX;
}

// ---- getRoot_12275a.cpp ----
void *PlayerStation::getRoot()
{
    return P(this, 0x140);
}

// ---- getProjectionVector_122858.cpp ----
Vector PlayerStation::getProjectionVector(const Vector &position)
{
    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        void *items = P(volumes, 0x4);
        uint32_t index = this->collisionIndex;
        void *volume = *(void **)((char *)items + index * 4);
        return ((BoundingVolume *)(volume))->getProjectionVector(position);
    }

    Vector result = {0.0f, 0.0f, 0.0f};
    return result;
}

// ---- render_1226e8.cpp ----
void PlayerStation::render()
{
    if (this->visible == 0) {
        return;
    }
    return ((AEGeometry *)(P(this, 0x140)))->render();
}

// ---- outerCollide_12283c.cpp ----
typedef void (*OuterCollideVectorFn)(PlayerStation *self, float x, float y, float z);

void PlayerStation::outerCollide(const Vector &position)
{
    OuterCollideVectorFn fn = *(OuterCollideVectorFn *)((char *)P(this, 0x0) + 0x3c);
    return fn(this, position.x, position.y, position.z);
}

// ---- getPosition_12274c.cpp ----
Vector PlayerStation::getPosition()
{
    return ((AEGeometry *)(P(this, 0x140)))->getPosition();
}

// ---- collide_12276e.cpp ----
typedef bool (*CollideFn)(PlayerStation *self, float x, float y, float z);

bool PlayerStation::collide(float x, float y, float z)
{
    CollideFn fn = *(CollideFn *)((char *)P(this, 0x0) + 0x3c);
    return fn(this, x, y, z);
}

// ---- update_1225fc.cpp ----
void PlayerStation::update(int delta)
{
    register int savedDelta = delta;
    bool active = F<int32_t>(P(this, 0x140), 0x14) != -1;
    int type = 0;
    if (active) {
        type = this->stationIndex;
    }
    if (!active || type == 0x65) {
        return;
    }

    if (((Status *)(P(Status_holder, 0x0)))->inAlienOrbit() != 0) {
        return;
    }

    void **canvasHolder = (void **)PaintCanvas_holder;
    void *canvas = *canvasHolder;
    void *root = P(this, 0x140);
    long long delta64 = (long long)savedDelta;
    ((AbyssEngine::Transform *)(AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<int32_t>(root, 0x14))))->Update(delta64, 0);

    type = this->stationIndex;
    if (type == 100) {
        ((AbyssEngine::Transform *)(AbyssEngine::PaintCanvas::TransformGetTransform(canvas, this->field_0x144)))->Update(delta64, 0);
    } else if (type == 0x6c) {
        TransformGetFn getTransform = PlayerStation_transformGet;
        long long transform = getTransform(canvas, this->field_0x164);
        uint32_t updateSlot = (uint32_t)((uint64_t)transform >> 32);
        TransformUpdateFn updateTransform = PlayerStation_transformUpdate;
        int deltaHigh = savedDelta >> 31;
        updateTransform((uint32_t)transform, updateSlot, savedDelta, deltaHigh, 0);

        transform = getTransform(canvas, this->field_0x168);
        updateTransform((uint32_t)transform, (uint32_t)((uint64_t)transform >> 32),
                        savedDelta, deltaHigh, 0);
        transform = getTransform(canvas, this->field_0x16c);
        updateTransform((uint32_t)transform, (uint32_t)((uint64_t)transform >> 32),
                        savedDelta, deltaHigh, 0);
        transform = getTransform(canvas, this->field_0x170);
        updateTransform((uint32_t)transform, (uint32_t)((uint64_t)transform >> 32),
                        savedDelta, deltaHigh, 0);
    }
}

// ---- PlayerStation_121818.cpp ----
namespace AbyssEngine { namespace PaintCanvas { void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh,
                                             bool flag); } }
extern "C" void BoundingSphere_constructor(void *self, float x, float y, float z, float radius,
                                           float a, float b, float c);
extern "C" void BoundingAAB_constructor(void *self, float x, float y, float z, float sx,
                                        float sy, float sz, float a, float b, float c);

PlayerStation::PlayerStation(Station *station)
{
    PlayerStaticFar_ctor(this, -1, 0, 0.0f, 0.0f, 0.0f);

    float local_64;
    float local_60;
    float local_5c;
    uint32_t local_58[3];
    uint16_t local_4e;
    uint32_t local_4c;
    uint32_t local_48;

    P(this, 0x0) = (char *)PlayerStation_vtable + 8;
    this->field_0x158 = 0;
    this->field_0x15c = 0;
    this->field_0x160 = 0;
    ((Player *)(P(this, 0x4)))->setRadius(15000);
    this->posX = 0;
    this->posY = 0;
    this->posZ = 0;
    this->boundingVolumes = 0;
    this->field_0x25 = 0;
    ((Player *)(P(this, 0x4)))->setMaxHitpoints(0x0161eb02);
    this->rootGeometry = 0;
    this->field_0x14c = 0;
    this->collisionRadius = 0;

    int stationIndex = Station_getIndex(station);
    this->stationIndex = stationIndex;

    void *reader = operator_new(1);
    FileRead_constructor(reader);
    void *collision = ((FileRead *)(reader))->loadStationCollision(stationIndex);
    operator_delete(FileRead_destructor(reader));

    void *status = P(Status_holder, 0x0);
    int alienOrbit = ((Status *)(status))->inAlienOrbit();
    void **canvasHolder = (void **)PaintCanvas_holder;
    void *canvas = *canvasHolder;
    void **rootSlot = (void **)((char *)this + 0x140);

    if ((uint32_t)(stationIndex - 0x6d) > 2 && collision == 0) {
        if (alienOrbit == 0) {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, 0x4034, canvas, false);
            *rootSlot = root;
            void *child = operator_new(0xc0);
            AEGeometry_constructor(child, 0x4037, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = operator_new(0xc0);
            AEGeometry_constructor(child2, 0x403a, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            operator_delete(AEGeometry_destructor(child));
            operator_delete(AEGeometry_destructor(child2));
            reader = operator_new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStationCollision(0x3e8);
            operator_delete(FileRead_destructor(reader));
        } else if (((Status *)(status))->dlc1Won() != 0) {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, 0x4220, canvas, false);
            *rootSlot = root;
            local_64 = -1.0f;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, (uint32_t *)&local_64);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, (uint32_t)local_64, 0x4221, true);
            ((AEGeometry *)(*rootSlot))->addChild((uint32_t)local_64);
            local_58[1] = 0xffffffff;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, local_58 + 1);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_58[1], 0x4222, true);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[1]);
            reader = operator_new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStationCollision(0x3eb);
            operator_delete(FileRead_destructor(reader));
            void *transform = AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0xc));
            void *transformAgain =
                AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0xc));
            Transform_UpdatePtr(transform, F<uint32_t>(transformAgain, 0xf8),
                                F<uint32_t>(transformAgain, 0xfc), 0);
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0x14));
            transformAgain = AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0x14));
            Transform_UpdatePtr(transform, F<uint32_t>(transformAgain, 0xf8),
                                F<uint32_t>(transformAgain, 0xfc), 0);
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0x10));
            transformAgain = AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0x10));
            Transform_UpdatePtr(transform, F<uint32_t>(transformAgain, 0xf8),
                                F<uint32_t>(transformAgain, 0xfc), 0);
        } else {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, 0x403b, canvas, false);
            *rootSlot = root;
            void *child = operator_new(0xc0);
            AEGeometry_constructor(child, 0x403e, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = operator_new(0xc0);
            AEGeometry_constructor(child2, 0x4041, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            operator_delete(AEGeometry_destructor(child));
            operator_delete(AEGeometry_destructor(child2));
            reader = operator_new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStationCollision(0x3e9);
            operator_delete(FileRead_destructor(reader));
        }
    }

    if (*rootSlot == 0) {
        if ((uint32_t)(stationIndex - 0x6d) < 3) {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, 0x5254, canvas, false);
            *rootSlot = root;
        } else {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, (uint16_t)(stationIndex + 21000), canvas, false);
            *rootSlot = root;
        }

        void *system = (void *)(long)((Status *)(status))->getSystem();
        uint32_t race = system == 0 ? 9u : (uint32_t)SolarSystem_getRace(system);
        stationIndex = this->stationIndex;
        if (stationIndex == 0x65) {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, 0x4220, canvas, false);
            *rootSlot = root;
            local_64 = -1.0f;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, (uint32_t *)&local_64);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, (uint32_t)local_64, 0x4221, true);
            ((AEGeometry *)(*rootSlot))->addChild((uint32_t)local_64);
            local_58[1] = 0xffffffff;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, local_58 + 1);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_58[1], 0x4222, true);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[1]);
            if (((Status *)(status))->getCurrentCampaignMission() == 0x9d &&
                Station_getIndex(((Status *)(status))->getStation()) == 0x70) {
                local_58[0] = 0xffffffff;
                AbyssEngine::PaintCanvas::TransformCreate(canvas, local_58);
                AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_58[0], 0x4950, true);
                ((AEGeometry *)(*rootSlot))->addChild(local_58[0]);
                local_48 = 0xffffffff;
                AbyssEngine::PaintCanvas::TransformCreate(canvas, &local_48);
                AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_48, 0x4952, true);
                ((AEGeometry *)(*rootSlot))->addChild(local_48);
                local_4c = 0xffffffff;
                AbyssEngine::PaintCanvas::TransformCreate(canvas, &local_4c);
                AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_4c, 0x4951, true);
                ((AEGeometry *)(*rootSlot))->addChild(local_4c);
            }
        } else if ((uint32_t)(stationIndex - 0x6d) < 2 ||
                   (stationIndex == 0x6f && ((Status *)(status))->getCurrentCampaignMission() > 0x5e)) {
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, 0x4953, canvas, false);
            *rootSlot = root;
            void *child = operator_new(0xc0);
            AEGeometry_constructor(child, 0x4954, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = operator_new(0xc0);
            AEGeometry_constructor(child2, 0x4955, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            void *child3 = operator_new(0xc0);
            AEGeometry_constructor(child3, 0x4956, canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child3, 0xc));
            if (collision != 0) {
                operator_delete((Array_int_destructor(collision), collision));
            }
            reader = operator_new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStaticCollision(stationIndex);
            FileRead_destructor(reader);
            operator_delete(reader);
        } else if (stationIndex == 100) {
            int mission = ((Status *)(status))->getCurrentCampaignMission();
            int dlcWon = ((Status *)(status))->dlc1Won();
            uint16_t rootMesh = dlcWon != 0 ? 0x3823 : 0x4223;
            if (mission == 0x50) {
                rootMesh = 0x381f;
            }
            uint16_t mesh1 = (mission == 0x50 || dlcWon != 0) ? 0x422a : 0x4228;
            uint16_t mesh2 = mission == 0x50 ? 0x3820 : (dlcWon != 0 ? 0x3824 : 0x4224);
            uint16_t mesh3 = mission == 0x50 ? 0x3821 : (dlcWon != 0 ? 0x3825 : 0x4225);
            void *root = operator_new(0xc0);
            AEGeometry_constructor(root, rootMesh, canvas, false);
            *rootSlot = root;
            local_64 = 0xffffffffu;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, (uint32_t *)&local_64);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, (uint32_t)local_64, mesh1, false);
            ((AEGeometry *)(*rootSlot))->addChild((uint32_t)local_64);
            this->field_0x144 = (uint32_t)local_64;
            local_58[1] = 0xffffffff;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, local_58 + 1);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_58[1], mesh2, false);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[1]);
            local_58[0] = 0xffffffff;
            AbyssEngine::PaintCanvas::TransformCreate(canvas, local_58);
            AbyssEngine::PaintCanvas::TransformAddMesh(canvas, local_58[0], mesh3, false);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[0]);
            if (mission == 0x50 || dlcWon != 0) {
                ((AbyssEngine::Transform *)(AbyssEngine::PaintCanvas::TransformGetTransform(canvas, F<uint32_t>(root, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)0, 0);
                ((AbyssEngine::Transform *)(AbyssEngine::PaintCanvas::TransformGetTransform(canvas, local_58[1])))->SetAnimationState((AbyssEngine::AnimationMode)0, 0);
                ((AbyssEngine::Transform *)(AbyssEngine::PaintCanvas::TransformGetTransform(canvas, local_58[0])))->SetAnimationState((AbyssEngine::AnimationMode)0, 0);
                if (collision != 0) {
                    operator_delete((Array_int_destructor(collision), collision));
                }
                reader = operator_new(1);
                FileRead_constructor(reader);
                collision = ((FileRead *)(reader))->loadStationCollision(0x3ed);
                operator_delete(FileRead_destructor(reader));
            } else {
                local_4c = 0x4226;
                local_48 = 0xf72c2200;
                ((AEGeometry *)(*rootSlot))->setLodMeshes((uint16_t *)&local_4c, (int *)&local_48, 1);
                local_4e = 0x4227;
                ((AEGeometry *)(*rootSlot))->setLodChildMeshes(&local_4e);
            }
        } else if ((race | 2) == 2 || race == 3) {
            void *child = operator_new(0xc0);
            AEGeometry_constructor(child, (uint16_t)(stationIndex + 0x5528), canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = operator_new(0xc0);
            AEGeometry_constructor(child2, (uint16_t)(this->stationIndex + 22000), canvas,
                                   false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            if (race == 3 && stationIndex == 0x6c) {
                void *extra = operator_new(0xc0);
                AEGeometry_constructor(extra, 0x5974, canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x164 = F<uint32_t>(extra, 0xc);
                extra = operator_new(0xc0);
                AEGeometry_constructor(extra, 0x5975, canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x168 = F<uint32_t>(extra, 0xc);
                extra = operator_new(0xc0);
                AEGeometry_constructor(extra, 0x5976, canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x16c = F<uint32_t>(extra, 0xc);
                extra = operator_new(0xc0);
                AEGeometry_constructor(extra, 0x5977, canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x170 = F<uint32_t>(extra, 0xc);
            }
            operator_delete(AEGeometry_destructor(child));
            operator_delete(AEGeometry_destructor(child2));
        }
    }

    if (collision != 0) {
        uint32_t count = **(uint32_t **)((char *)collision + 4);
        local_64 = 0.0f;
        local_60 = 0.0f;
        local_5c = 0.0f;
        void *volumes = operator_new(0xc);
        Array_BoundingVolumePtr_constructor(volumes);
        P(this, 0x130) = volumes;
        ArraySetLength_BoundingVolumePtr(count, volumes);
        int cursor = 1;
        for (uint32_t i = 0; i < count; ++i) {
            int *data = *(int **)((char *)collision + 4);
            int next = cursor + 1;
            int type = data[cursor];
            if (type == 1) {
                int *entry = data + cursor;
                local_64 = (float)entry[4];
                local_5c = (float)entry[5];
                local_60 = (float)entry[6];
                Vector_scale((Vector *)&local_64, local_60);
                void *volume = operator_new(0x2c);
                BoundingAAB_constructor(volume, local_64 + local_64, (float)entry[3],
                                        local_60 + local_60, (float)entry[2],
                                        local_5c + local_5c, (float)-data[next], 0.0f, 0.0f,
                                        0.0f);
                next = cursor + 7;
                *(void **)((char *)P(P(this, 0x130), 4) + i * 4) = volume;
            } else if (type == 0) {
                int *entry = data + cursor;
                float radius = (float)-data[next];
                local_64 = (float)entry[4];
                Vector_scale((Vector *)&local_64, radius);
                if (local_64 < 0.0f) {
                    Vector_scale((Vector *)&local_64, local_64);
                }
                void *volume = operator_new(0x48);
                BoundingSphere_constructor(volume, local_64, (float)entry[3], (float)entry[2],
                                           radius, 0.0f, 0.0f, 0.0f);
                next = cursor + 5;
                *(void **)((char *)P(P(this, 0x130), 4) + i * 4) = volume;
            }
            cursor = next;
        }
        ArrayRelease_int(collision);
        operator_delete((Array_int_destructor(collision), collision));
    }

    this->field_0x71 = 1;
    ((AEGeometry *)(P(this, 0x140)))->setRotation(0.0f, 0.0f, 0.0f);
    void *transform =
        AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, F<uint32_t>(P(this, 0x140), 0xc));
    this->collisionRadius = (int)(F<float>(transform, 0xe0) + 10.0f);
}

// ---- outerCollide_122778.cpp ----
typedef bool (*BoundingVolumeCollideFn)(void *volume, uint32_t x, uint32_t y, uint32_t z);

bool PlayerStation::outerCollide(float x, float y, float z)
{
    uint32_t rawX = F<uint32_t>(&x, 0);
    uint32_t rawY = F<uint32_t>(&y, 0);
    uint32_t rawZ = F<uint32_t>(&z, 0);
    float radius = (float)this->collisionRadius;

    if (x < this->posX + radius) {
        if (this->posX - radius < x) {
            if (y < this->posY + radius) {
                if (this->posY - radius < y) {
                    if (z < this->posZ + radius) {
                        if (this->posZ - radius < z) {
                            void *volumes = P(this, 0x130);
                            if (volumes != 0) {
                                for (uint32_t i = 0; i < F<uint32_t>(volumes, 0x0);
                                     ++i) {
                                    void *volume =
                                        *(void **)((char *)P(volumes, 0x4) + i * 4);
                                    BoundingVolumeCollideFn fn =
                                        *(BoundingVolumeCollideFn *)((char *)P(volume, 0x0) +
                                                                    0xc);
                                    if (fn(volume, rawX, rawY, rawZ)) {
                                        this->collisionIndex = i;
                                        return true;
                                    }
                                    volumes = P(this, 0x130);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

// ---- _PlayerStation_1225ec.cpp ----
// Deleting destructor variant (D0): runs the base destructor body then frees.
// Emitted as a named function so it doesn't collide with the in-place ~PlayerStation() above.
void PlayerStation::deleting_dtor() {
    PlayerStation *self = this;
    operator_delete(PlayerStation_destructor_body(self));
}

// ---- translate_122766.cpp ----
void PlayerStation::translate(float x, float y, float z)
{
    return ((AEGeometry *)(P(this, 0x140)))->translate(x, y, z);
}
