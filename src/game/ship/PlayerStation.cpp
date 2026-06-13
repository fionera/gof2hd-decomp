#include "gof2/game/ship/PlayerStation.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/engine/math/BoundingVolume.h"
#include "gof2/engine/file/FileRead.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/core/PaintCanvasClass.h"

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
extern "C" void ArrayReleaseClasses_BoundingVolumePtr(void *array) __attribute__((nothrow));
extern "C" void *Array_BoundingVolumePtr_destructor(void *array) __attribute__((nothrow));
extern "C" void *Status_holder __attribute__((visibility("hidden")));
extern void *g_PaintCanvas;   // PaintCanvas singleton pointer (externs.h)
extern "C" TransformGetFn PlayerStation_transformGet __attribute__((visibility("hidden")));
extern "C" TransformUpdateFn PlayerStation_transformUpdate __attribute__((visibility("hidden")));
extern "C" int Station_getIndex(Station *station);
extern "C" void *FileRead_constructor(void *self);
extern "C" void *FileRead_destructor(void *self) __attribute__((nothrow));
extern "C" void Transform_UpdatePtr(void *transform, uint32_t lo, uint32_t hi, int zero);
extern "C" void Transform_SetActive(void *transform);
extern "C" void Transform_SetInactive(void *transform);
extern "C" void Array_int_destructor(void *array) __attribute__((nothrow));
extern "C" void ArrayRelease_int(void *array) __attribute__((nothrow));
extern "C" void Array_BoundingVolumePtr_constructor(void *array);
extern "C" void ArraySetLength_BoundingVolumePtr(uint32_t length, void *array);

void PlayerStation::setVisible(bool visible)
{
    this->visible = visible;
}

PlayerStation::~PlayerStation() noexcept(false)
{
    P(this, 0x0) = (char *)PlayerStation_vtable + 8;

    void *geometry = P(this, 0x140);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    P(this, 0x140) = 0;

    geometry = P(this, 0x14c);
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }
    P(this, 0x14c) = 0;

    void *volumes = P(this, 0x130);
    if (volumes != 0) {
        ArrayReleaseClasses_BoundingVolumePtr(volumes);
        volumes = P(this, 0x130);
        if (volumes != 0) {
            ::operator delete(Array_BoundingVolumePtr_destructor(volumes));
        }
    }
    P(this, 0x130) = 0;

}

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

void *PlayerStation::getRoot()
{
    return P(this, 0x140);
}

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

void PlayerStation::render()
{
    if (this->visible == 0) {
        return;
    }
    return ((AEGeometry *)(P(this, 0x140)))->render();
}

typedef void (*OuterCollideVectorFn)(PlayerStation *self, float x, float y, float z);

void PlayerStation::outerCollide(const Vector &position)
{
    OuterCollideVectorFn fn = *(OuterCollideVectorFn *)((char *)P(this, 0x0) + 0x3c);
    return fn(this, position.x, position.y, position.z);
}

Vector PlayerStation::getPosition()
{
    return ((AEGeometry *)(P(this, 0x140)))->getPosition();
}

typedef bool (*CollideFn)(PlayerStation *self, float x, float y, float z);

bool PlayerStation::collide(float x, float y, float z)
{
    CollideFn fn = *(CollideFn *)((char *)P(this, 0x0) + 0x3c);
    return fn(this, x, y, z);
}

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

    void **canvasHolder = (void **)g_PaintCanvas;
    void *canvas = *canvasHolder;
    void *root = P(this, 0x140);
    long long delta64 = (long long)savedDelta;
    ((AbyssEngine::Transform *)(((PaintCanvas*)canvas)->TransformGetTransform(F<int32_t>(root, 0x14))))->Update(delta64, 0);

    type = this->stationIndex;
    if (type == 100) {
        ((AbyssEngine::Transform *)(((PaintCanvas*)canvas)->TransformGetTransform(this->field_0x144)))->Update(delta64, 0);
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

    void *reader = ::operator new(1);
    FileRead_constructor(reader);
    void *collision = ((FileRead *)(reader))->loadStationCollision(stationIndex);
    ::operator delete(FileRead_destructor(reader));

    void *status = P(Status_holder, 0x0);
    int alienOrbit = ((Status *)(status))->inAlienOrbit();
    void **canvasHolder = (void **)g_PaintCanvas;
    void *canvas = *canvasHolder;
    void **rootSlot = (void **)((char *)this + 0x140);

    if ((uint32_t)(stationIndex - 0x6d) > 2 && collision == 0) {
        if (alienOrbit == 0) {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(0x4034), (PaintCanvas*)canvas, false);
            *rootSlot = root;
            void *child = ::operator new(0xc0);
            new ((void*)child) AEGeometry((uint16_t)(0x4037), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = ::operator new(0xc0);
            new ((void*)child2) AEGeometry((uint16_t)(0x403a), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            ((AEGeometry *)child)->~AEGeometry();
            ::operator delete(child);
            ((AEGeometry *)child2)->~AEGeometry();
            ::operator delete(child2);
            reader = ::operator new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStationCollision(0x3e8);
            ::operator delete(FileRead_destructor(reader));
        } else if (((Status *)(status))->dlc1Won() != 0) {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(0x4220), (PaintCanvas*)canvas, false);
            *rootSlot = root;
            local_64 = -1.0f;
            ((PaintCanvas*)canvas)->TransformCreate((uint32_t *)&local_64);
            ((PaintCanvas*)canvas)->TransformAddMesh((uint32_t)local_64, 0x4221, true);
            ((AEGeometry *)(*rootSlot))->addChild((uint32_t)local_64);
            local_58[1] = 0xffffffff;
            ((PaintCanvas*)canvas)->TransformCreate(local_58 + 1);
            ((PaintCanvas*)canvas)->TransformAddMesh(local_58[1], 0x4222, true);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[1]);
            reader = ::operator new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStationCollision(0x3eb);
            ::operator delete(FileRead_destructor(reader));
            void *transform = ((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0xc));
            void *transformAgain =
                ((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0xc));
            Transform_UpdatePtr(transform, F<uint32_t>(transformAgain, 0xf8),
                                F<uint32_t>(transformAgain, 0xfc), 0);
            transform = ((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0x14));
            transformAgain = ((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0x14));
            Transform_UpdatePtr(transform, F<uint32_t>(transformAgain, 0xf8),
                                F<uint32_t>(transformAgain, 0xfc), 0);
            transform = ((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0x10));
            transformAgain = ((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0x10));
            Transform_UpdatePtr(transform, F<uint32_t>(transformAgain, 0xf8),
                                F<uint32_t>(transformAgain, 0xfc), 0);
        } else {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(0x403b), (PaintCanvas*)canvas, false);
            *rootSlot = root;
            void *child = ::operator new(0xc0);
            new ((void*)child) AEGeometry((uint16_t)(0x403e), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = ::operator new(0xc0);
            new ((void*)child2) AEGeometry((uint16_t)(0x4041), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            ((AEGeometry *)child)->~AEGeometry();
            ::operator delete(child);
            ((AEGeometry *)child2)->~AEGeometry();
            ::operator delete(child2);
            reader = ::operator new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStationCollision(0x3e9);
            ::operator delete(FileRead_destructor(reader));
        }
    }

    if (*rootSlot == 0) {
        if ((uint32_t)(stationIndex - 0x6d) < 3) {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(0x5254), (PaintCanvas*)canvas, false);
            *rootSlot = root;
        } else {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)((uint16_t)(stationIndex + 21000)), (PaintCanvas*)canvas, false);
            *rootSlot = root;
        }

        void *system = (void *)(long)((Status *)(status))->getSystem();
        uint32_t race = system == 0 ? 9u : (uint32_t)((SolarSystem *)(system))->getRace();
        stationIndex = this->stationIndex;
        if (stationIndex == 0x65) {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(0x4220), (PaintCanvas*)canvas, false);
            *rootSlot = root;
            local_64 = -1.0f;
            ((PaintCanvas*)canvas)->TransformCreate((uint32_t *)&local_64);
            ((PaintCanvas*)canvas)->TransformAddMesh((uint32_t)local_64, 0x4221, true);
            ((AEGeometry *)(*rootSlot))->addChild((uint32_t)local_64);
            local_58[1] = 0xffffffff;
            ((PaintCanvas*)canvas)->TransformCreate(local_58 + 1);
            ((PaintCanvas*)canvas)->TransformAddMesh(local_58[1], 0x4222, true);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[1]);
            if (((Status *)(status))->getCurrentCampaignMission() == 0x9d &&
                Station_getIndex(((Status *)(status))->getStation()) == 0x70) {
                local_58[0] = 0xffffffff;
                ((PaintCanvas*)canvas)->TransformCreate(local_58);
                ((PaintCanvas*)canvas)->TransformAddMesh(local_58[0], 0x4950, true);
                ((AEGeometry *)(*rootSlot))->addChild(local_58[0]);
                local_48 = 0xffffffff;
                ((PaintCanvas*)canvas)->TransformCreate(&local_48);
                ((PaintCanvas*)canvas)->TransformAddMesh(local_48, 0x4952, true);
                ((AEGeometry *)(*rootSlot))->addChild(local_48);
                local_4c = 0xffffffff;
                ((PaintCanvas*)canvas)->TransformCreate(&local_4c);
                ((PaintCanvas*)canvas)->TransformAddMesh(local_4c, 0x4951, true);
                ((AEGeometry *)(*rootSlot))->addChild(local_4c);
            }
        } else if ((uint32_t)(stationIndex - 0x6d) < 2 ||
                   (stationIndex == 0x6f && ((Status *)(status))->getCurrentCampaignMission() > 0x5e)) {
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(0x4953), (PaintCanvas*)canvas, false);
            *rootSlot = root;
            void *child = ::operator new(0xc0);
            new ((void*)child) AEGeometry((uint16_t)(0x4954), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = ::operator new(0xc0);
            new ((void*)child2) AEGeometry((uint16_t)(0x4955), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            void *child3 = ::operator new(0xc0);
            new ((void*)child3) AEGeometry((uint16_t)(0x4956), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child3, 0xc));
            if (collision != 0) {
                ::operator delete((Array_int_destructor(collision), collision));
            }
            reader = ::operator new(1);
            FileRead_constructor(reader);
            collision = ((FileRead *)(reader))->loadStaticCollision(stationIndex);
            FileRead_destructor(reader);
            ::operator delete(reader);
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
            void *root = ::operator new(0xc0);
            new ((void*)root) AEGeometry((uint16_t)(rootMesh), (PaintCanvas*)canvas, false);
            *rootSlot = root;
            local_64 = 0xffffffffu;
            ((PaintCanvas*)canvas)->TransformCreate((uint32_t *)&local_64);
            ((PaintCanvas*)canvas)->TransformAddMesh((uint32_t)local_64, mesh1, false);
            ((AEGeometry *)(*rootSlot))->addChild((uint32_t)local_64);
            this->field_0x144 = (uint32_t)local_64;
            local_58[1] = 0xffffffff;
            ((PaintCanvas*)canvas)->TransformCreate(local_58 + 1);
            ((PaintCanvas*)canvas)->TransformAddMesh(local_58[1], mesh2, false);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[1]);
            local_58[0] = 0xffffffff;
            ((PaintCanvas*)canvas)->TransformCreate(local_58);
            ((PaintCanvas*)canvas)->TransformAddMesh(local_58[0], mesh3, false);
            ((AEGeometry *)(*rootSlot))->addChild(local_58[0]);
            if (mission == 0x50 || dlcWon != 0) {
                ((AbyssEngine::Transform *)(((PaintCanvas*)canvas)->TransformGetTransform(F<uint32_t>(root, 0xc))))->SetAnimationState((AbyssEngine::AnimationMode)0, 0);
                ((AbyssEngine::Transform *)(((PaintCanvas*)canvas)->TransformGetTransform(local_58[1])))->SetAnimationState((AbyssEngine::AnimationMode)0, 0);
                ((AbyssEngine::Transform *)(((PaintCanvas*)canvas)->TransformGetTransform(local_58[0])))->SetAnimationState((AbyssEngine::AnimationMode)0, 0);
                if (collision != 0) {
                    ::operator delete((Array_int_destructor(collision), collision));
                }
                reader = ::operator new(1);
                FileRead_constructor(reader);
                collision = ((FileRead *)(reader))->loadStationCollision(0x3ed);
                ::operator delete(FileRead_destructor(reader));
            } else {
                local_4c = 0x4226;
                local_48 = 0xf72c2200;
                ((AEGeometry *)(*rootSlot))->setLodMeshes((uint16_t *)&local_4c, (int *)&local_48, 1);
                local_4e = 0x4227;
                ((AEGeometry *)(*rootSlot))->setLodChildMeshes(&local_4e);
            }
        } else if ((race | 2) == 2 || race == 3) {
            void *child = ::operator new(0xc0);
            new ((void*)child) AEGeometry((uint16_t)((uint16_t)(stationIndex + 0x5528)), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child, 0xc));
            void *child2 = ::operator new(0xc0);
            new ((void*)child2) AEGeometry((uint16_t)((uint16_t)(this->stationIndex + 22000)), (PaintCanvas*)canvas, false);
            ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(child2, 0xc));
            if (race == 3 && stationIndex == 0x6c) {
                void *extra = ::operator new(0xc0);
                new ((void*)extra) AEGeometry((uint16_t)(0x5974), (PaintCanvas*)canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x164 = F<uint32_t>(extra, 0xc);
                extra = ::operator new(0xc0);
                new ((void*)extra) AEGeometry((uint16_t)(0x5975), (PaintCanvas*)canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x168 = F<uint32_t>(extra, 0xc);
                extra = ::operator new(0xc0);
                new ((void*)extra) AEGeometry((uint16_t)(0x5976), (PaintCanvas*)canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x16c = F<uint32_t>(extra, 0xc);
                extra = ::operator new(0xc0);
                new ((void*)extra) AEGeometry((uint16_t)(0x5977), (PaintCanvas*)canvas, false);
                ((AEGeometry *)(*rootSlot))->addChild(F<uint32_t>(extra, 0xc));
                this->field_0x170 = F<uint32_t>(extra, 0xc);
            }
            ((AEGeometry *)child)->~AEGeometry();
            ::operator delete(child);
            ((AEGeometry *)child2)->~AEGeometry();
            ::operator delete(child2);
        }
    }

    if (collision != 0) {
        uint32_t count = **(uint32_t **)((char *)collision + 4);
        local_64 = 0.0f;
        local_60 = 0.0f;
        local_5c = 0.0f;
        void *volumes = ::operator new(0xc);
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
                *(Vector *)&local_64 *= (local_60);
                void *volume = ::operator new(0x2c);
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
                *(Vector *)&local_64 *= (radius);
                if (local_64 < 0.0f) {
                    *(Vector *)&local_64 *= (local_64);
                }
                void *volume = ::operator new(0x48);
                BoundingSphere_constructor(volume, local_64, (float)entry[3], (float)entry[2],
                                           radius, 0.0f, 0.0f, 0.0f);
                next = cursor + 5;
                *(void **)((char *)P(P(this, 0x130), 4) + i * 4) = volume;
            }
            cursor = next;
        }
        ArrayRelease_int(collision);
        ::operator delete((Array_int_destructor(collision), collision));
    }

    this->field_0x71 = 1;
    ((AEGeometry *)(P(this, 0x140)))->setRotation(0.0f, 0.0f, 0.0f);
    void *transform =
        ((PaintCanvas*)*canvasHolder)->TransformGetTransform(F<uint32_t>(P(this, 0x140), 0xc));
    this->collisionRadius = (int)(F<float>(transform, 0xe0) + 10.0f);
}

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

// Deleting destructor variant (D0): runs the base destructor body then frees.
// Emitted as a named function so it doesn't collide with the in-place ~PlayerStation() above.
void PlayerStation::deleting_dtor() {
    PlayerStation *self = this;
    ::operator delete(((PlayerStation *)self)->destructor_body());
}

// Complete-object destructor body (engine 0x1325ec calls this, then frees the
// returned pointer). Run the in-place destructor teardown and hand `this` back
// so the deleting variant can chain operator delete onto it.
void *PlayerStation::destructor_body() {
    this->~PlayerStation();
    return this;
}

// transformGet (GOT -> AbyssEngine::PaintCanvas::TransformGetTransform): fetch the
// Transform handle for sub-mesh `meshId` from the canvas. Returned as a 64-bit
// value so update() can carry it through the split 32-bit calling ABI.
long long PlayerStation::transformGet(void *canvas, int meshId) {
    return (long long)(uintptr_t)((PaintCanvas *)canvas)->TransformGetTransform((unsigned)meshId);
}

// transformUpdate (GOT -> AbyssEngine::Transform::Update): advance the Transform
// whose handle arrived split across (lo, hi) by `delta` ticks. deltaHigh is the
// sign-extension of delta; flag selects bounds recomputation.
void PlayerStation::transformUpdate(uint32_t lo, uint32_t hi, int delta, int deltaHigh, int flag) {
    (void)deltaHigh;
    AbyssEngine::Transform *transform =
        (AbyssEngine::Transform *)(uintptr_t)((uint64_t)lo | ((uint64_t)hi << 32));
    transform->Update((long long)delta, flag != 0);
}

void PlayerStation::translate(float x, float y, float z)
{
    return ((AEGeometry *)(P(this, 0x140)))->translate(x, y, z);
}
