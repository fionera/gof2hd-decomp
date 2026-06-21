#include "game/weapons/ObjectGun.h"

// PlayerEgo.h references LevelScript by pointer only; it previously came from the
// (now-removed) global fwd.h, so forward-declare it here to keep that header usable.
class LevelScript;
class KIPlayer;

#include "game/world/Level.h"
#include "engine/math/Transform.h"
#include "game/mission/Explosion.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Gun.h"
#include "game/ship/Player.h"
#include "engine/render/AEGeometry.h"
#include "platform/libc.h"

// Byte-offset reader retained ONLY for foreign-class fields that are not modelled
// by name in their (out-of-batch) headers: a KIPlayer status byte, a PlayerEgo
// maneuver param accessed through a Player* local, the opaque matrix source, and
// the Gun barrel-mirror sub-bytes packed inside Gun::field_0xa4.
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }

// Small POD aggregate used only here: maps a weapon item index to its geometry mesh id.
struct MeshId {
    uint16_t id;
    uint16_t pad;
};

namespace AbyssEngine { namespace AEMath {
void MatrixMultiply(const Matrix &, const Matrix &);
} }

// Deletes every owned pointee in the array (nulling each slot as it goes), then frees
// the backing store. Out-of-line in the original as ArrayReleaseClasses<T>; the loop
// walks the full capacity, not just size. ObjectGun owns the Array<Explosion*> pool, so
// the original's ArraySetLength<Explosion*> / ArrayReleaseClasses<Explosion*> live here.
template<class T>
void ArrayReleaseClasses(Array<T> &a) {
    for (unsigned int i = 0; i < a.capacity_; i = i + 1) {
        if (a.data_[i] != 0) {
            delete a.data_[i];
        }
        a.data_[i] = 0;
    }
    if (a.data_) {
        ::operator delete[](a.data_);
    }
    a.data_ = 0;
}
template void ArraySetLength<Explosion*>(unsigned int, Array<Explosion*> &);
template void ArrayReleaseClasses<Explosion*>(Array<Explosion*> &);

// PaintCanvas transform/mesh helpers and the active canvas handle (engine globals).
extern "C" void *g_PaintCanvas;
void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
void TransformCreate(void *canvas, uint32_t *transform);
uint32_t TransformGetTransform(void *canvas, uint32_t transform);
void TransformSetLocal(void *canvas, uint32_t transform, Matrix *matrix);
void DrawTransform(void *canvas, uint32_t transform, int flags);
void MatrixRotateVector(void *out, const void *matrix, const void *vec);
void *CameraGetCurrent(void *canvas);
void *CameraGetLocal(void *canvas, void *camera);
void MatrixGetDir(Vector *out, const Matrix *matrix);
void MatrixGetUp(Vector *out, const Matrix *matrix);
void MatrixSetRotation(Matrix *dst, float x, float y, float z);
void MatrixSetTranslation(Matrix *dst, float x, float y, float z);
void MatrixSetScaling(Matrix *dst, float x, float y, float z);
void VectorNormalize(Vector *out, const Vector *in);
void VectorCross(Vector *out, const Vector *a, const Vector *b);

// Forwards the enemy list to the underlying weapon-targeting implementation.
extern "C" void ObjectGun_setEnemies_impl(void *items);

// Engine globals controlling per-weapon scaling and the geometry/mesh id tables.
// vtable slot 8: the base ObjectGun is not a rocket gun (RocketGun overrides this).
int ObjectGun::isRocketGun() { return 0; }
extern "C" void *g_ObjectGunScaleFlag;
extern "C" void *g_ObjectGunRenderScaleFlag;
extern "C" MeshId g_ObjectGunGeometryIds[];
extern "C" int g_ObjectGunPlayerGunIds[];
extern "C" uint32_t (*g_TransformGetObject)(void *canvas, uint32_t mesh);
extern "C" void (*g_TransformSetState)(uint32_t object, int state, int value);

ObjectGun::ObjectGun(int /*unused*/, Gun *gun, int mesh, uint32_t /*param*/, Level *level)
{
    this->dir.x = 0.0f;
    this->dir.y = 0.0f;
    this->dir.z = 0.0f;
    this->up.y = 0.0f;
    this->up.z = 0.0f;
    this->side.x = 0.0f;
    this->side.z = 0.0f;
    this->orientation = AbyssEngine::AEMath::Matrix();

    void **canvas = (void **)g_PaintCanvas;
    this->useEgoOrientation = 0;
    this->explosions = nullptr;
    this->explosionReady = nullptr;
    this->unusedSlot = -1;
    this->gun = gun;
    this->level = level;
    this->secondaryTransform = -1;
    TransformCreate(*canvas, &this->transform);
    this->meshId = mesh;
    TransformAddMesh(*canvas, this->transform, (uint16_t)mesh, 0);
    this->rollAngle = 0.0f;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->scaleZ = 1.0f;
    this->spinAngle = 0.0f;

    int type = gun->weaponType;
    this->visible = (type <= 8 && ((1u << type) & 0x10aU) != 0) ? 1 : 0;

    if (*(uint8_t *)g_ObjectGunScaleFlag != 0) {
        if (type == 1 || type == 3 || type == 8) {
            this->scaleX = 0.6f;
            this->scaleY = 0.6f;
            this->scaleZ = 0.6f;
        }
    }
    if (type == 0xb) {
        this->scaleX = 0.7f;
        this->scaleY = 0.7f;
        this->scaleZ = 0.7f;
    } else if (type == 0x19) {
        this->explosions = new Array<Explosion*>();
        this->explosions->resize(gun->count);
        this->explosionReady = new uint8_t[this->explosions->size()];

        for (uint32_t i = 0; i < this->explosions->size(); ++i) {
            int explosionType = 10;
            if (gun->itemIndex == 0xb1)
                explosionType = 9;
            if (gun->itemIndex == 0xb0)
                explosionType = 8;
            Explosion *explosion = new Explosion(explosionType);
            this->explosions->data()[i] = explosion;
            explosion->setWeaponIndex(gun->itemIndex);
            this->explosionReady[i] = 1;
        }
    }

    AEGeometry *geometry = nullptr;
    bool createGeometry = true;
    if (gun->field_0xa8 == 0) {
        if (gun->isPlayerGun() == 0 || g_ObjectGunPlayerGunIds[gun->itemIndex] < 0) {
            this->hasGeometry = 0;
            createGeometry = false;
        } else {
            this->hasGeometry = (gun->weaponType != ITEM_SORT_MINE) ? 1 : 0;
            createGeometry = this->hasGeometry != 0;
        }
    } else {
        this->hasGeometry = 1;
    }

    if (createGeometry)
        geometry = new AEGeometry((uint16_t)g_ObjectGunGeometryIds[gun->itemIndex].id,
                                  (PaintCanvas *)*canvas, false);

    this->wasFiring = 0;
    this->geometry = geometry;
    this->deltaTime = 0;
}

// Resets the vtable slot, then releases the gun geometry, the per-explosion array
// (releasing each element first), and the ready-flag buffer.
ObjectGun::~ObjectGun()
{
    delete this->geometry;
    this->geometry = nullptr;

    if (this->explosions != nullptr) {
        for (Explosion *e : *this->explosions)
            delete e;
        this->explosions->clear();
        delete this->explosions;
        this->explosions = nullptr;
    }

    delete[] this->explosionReady;
    this->explosionReady = nullptr;
}

// Stores x/y into scaleX/scaleY; the trailing z component is accepted but unused
// by this build (scaleZ is left untouched).
void ObjectGun::setScaling(int x, int y, int /*z*/)
{
    this->scaleX = (float)x;
    this->scaleY = (float)y;
}

// Swaps the mesh shown by the primary transform: removes the old mesh, records the
// new one, and re-adds it. The trailing argument is accepted but ignored.
void ObjectGun::replaceGun(unsigned int mesh, int /*unused*/)
{
    void **canvas = (void **)g_PaintCanvas;
    TransformRemoveMesh(*canvas, this->transform, this->meshId);
    this->meshId = (int)mesh;
    TransformAddMesh(*canvas, this->transform, (uint16_t)mesh, 0);
}

void ObjectGun::setEnemies(Array<Player*> *enemies)
{
    ObjectGun_setEnemies_impl((void *)enemies->data());
}

// Targeting hooks overridden by the KI-driven guns; the base ObjectGun keeps no
// enemy state and applies no camera translation, so both are no-ops here.
void ObjectGun::setEnemy(Player * /*enemy*/)
{
}

void ObjectGun::translate(const Vector & /*v*/)
{
}

void ObjectGun::update(int dt)
{
    Matrix playerMatrix;
    Matrix workMatrix;
    Matrix cameraMatrix;
    Vector position;
    Vector dir;
    Vector up;
    Vector offsets;
    Vector zero = {0.0f, 0.0f, 0.0f};

    void **canvas = (void **)g_PaintCanvas;
    uint32_t transform = TransformGetTransform(*canvas, this->transform);
    ((AbyssEngine::Transform *)((uint64_t)transform))->Update((int64_t)dt, 0);

    this->deltaTime = dt;
    Gun *gun = this->gun;
    gun->update(dt);

    if (this->hasGeometry == 0) {
        if (gun->isPlayerGun() == 0) {
            Player *owner = gun->owner;
            if (owner != nullptr) {
                KIPlayer *ki = owner->getKIPlayer();
                // RAWREAD: original-ABI byte 0x3f status flag; KIPlayer header is recompiled 64-bit so named offsets don't align.
                if (ki != nullptr && F<uint8_t>(ki, 0x3f) != 0) {
                    this->hasGeometry = 1;
                    this->geometry = new AEGeometry(
                        (uint16_t)g_ObjectGunGeometryIds[this->gun->itemIndex].id,
                        (PaintCanvas *)*canvas, false);
                }
            }
        }
        if (this->hasGeometry == 0)
            goto after_geometry;
    }

    gun = this->gun;
    if (gun->delayActive == 0) {
        g_TransformSetState(g_TransformGetObject(*canvas, this->geometry->transform), 0, 0);
        g_TransformSetState(g_TransformGetObject(*canvas, this->geometry->transform), 3, 0);
        g_TransformSetState(g_TransformGetObject(*canvas, this->geometry->transform), 1, 0);
        goto after_geometry;
    }

    {
        Player *player = (Player *)(uint64_t)this->level->getPlayer();
        if (gun->isPlayerGun() != 0) {
            position = ((PlayerEgo *)player)->getPosition();
        } else {
            gun->owner->getPosition(&position);
        }

        gun = this->gun;
        void *matrixSource = player;
        if (gun->isPlayerGun() == 0)
            matrixSource = (char *)&gun->owner;
        // RAWREAD: matrixSource is an opaque void* (PlayerEgo or &owner); deref of slot 0 then +4 reaches the matrix payload.
        memcpy(&playerMatrix, (char *)F<void *>(matrixSource, 0x0) + 0x4, 0x3c);

        offsets.x = gun->offset.x;
        offsets.y = gun->offset.y;
        offsets.z = gun->offset.z + 8.0f;
        MatrixRotateVector(&workMatrix, &playerMatrix, &offsets);
        position += *(const Vector *)&workMatrix;
        this->geometry->setPosition(position);

        transform = TransformGetTransform(*canvas, this->geometry->transform);
        ((AbyssEngine::Transform *)((uint64_t)transform))->Update((int64_t)dt, 0);

        if (this->gun->weaponType != ITEM_SORT_TURRET) {
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            cameraMatrix = *(const Matrix *)CameraGetLocal(paint, camera);
            MatrixGetDir(&dir, &cameraMatrix);
            MatrixGetUp(&up, &cameraMatrix);
            this->geometry->setDirection(dir, up);
            goto after_geometry;
        }

        AbyssEngine::AEMath::MatrixMultiply(workMatrix, this->geometry->getMatrix());

        gun = this->gun;
        int weapon = gun->itemIndex;
        offsets.x = 0.0f;
        offsets.y = 0.0f;
        offsets.z = -3.5f;
        if (weapon == 0xb5) {
            offsets.z = -1.5f;
        } else if (weapon == 0x31) {
            offsets.z = 7.0f;
        } else if (weapon == 0xb4) {
            offsets.z = -6.0f;
        } else if (weapon == 0x30) {
            offsets.z = 15.0f;
        } else if (weapon == 0xe0) {
            offsets.z = -13.0f;
        } else if (weapon == 0xb6) {
            offsets.z = 13.0f;
        }

        if (gun->field_0xa4 != 0) {
            // RAWREAD: 0xa5..0xa7 are barrel-mirror sub-bytes packed inside Gun::field_0xa4 (int); no separate named members.
            if (weapon == 0xb5) {
                offsets.x = F<uint8_t>(gun, 0xa6) != 0 ? -1.5f : 1.5f;
            } else {
                float left = weapon == 0x30 ? 20.0f : 15.0f;
                float right = weapon == 0x30 ? -20.0f : -15.0f;
                offsets.x = left - gun->offset.x;
                if (F<uint8_t>(gun, 0xa6) != 0)
                    offsets.x = gun->offset.x + right;
            }
        }
        if (F<uint8_t>(gun, 0xa5) != 0) {
            if (weapon == 0xe0) {
                offsets.y = F<uint8_t>(gun, 0xa7) == 0 ? 13.0f : -13.0f;
            } else if (weapon == 0xb5) {
                offsets.y = F<uint8_t>(gun, 0xa7) == 0 ? 2.5f : -2.5f;
            }
        }
        MatrixRotateVector(&cameraMatrix, &workMatrix, &offsets);
        this->geometry->setMatrix(workMatrix);
        this->geometry->translate(*(Vector *)&cameraMatrix);
    }

after_geometry:
    gun = this->gun;
    this->wasFiring = gun->delayActive;
    if (gun->weaponType == ITEM_SORT_SCATTER_GUN) {
        int positionOffset = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                Explosion *explosion = this->explosions->data()[i];
                if (this->explosionReady[i] != 0) {
                    explosion->start(*(Vector *)((char *)gun->hitPositions + positionOffset), zero);
                    this->explosionReady[i] = 0;
                }
                explosion->update(dt, static_cast<TargetFollowCamera *>(nullptr));
                if (explosion->isPlaying() == 0) {
                    gun = this->gun;
                    gun->hitFlags[i] = 0;
                    gun->ignited = 0;
                    this->explosionReady[i] = 1;
                    explosion->reset();
                }
            }
            positionOffset += 0xc;
            gun = this->gun;
        }
    }
}

// The original inlined this matrix-identity helper into every caller (no standalone
// symbol), so force it inline here too rather than letting -Oz emit it out-of-line.
__attribute__((always_inline)) static inline void identity(Matrix *m)
{
    for (uint32_t i = 0; i < 15; ++i)
        m->m[i] = 0.0f;
    m->m[0] = 1.0f;
    m->m[5] = 1.0f;
    m->m[14] = 1.0f;
}

void ObjectGun::render()
{
    Matrix local;
    Matrix cameraLocal;
    Matrix rotate;
    Matrix scaleMatrix;
    Vector muzzle;
    Vector dir;
    Vector up;
    Vector side;

    Gun *gun = this->gun;
    gun->render();

    if (gun->weaponType == ITEM_SORT_SCATTER_GUN) {
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                this->explosions->data()[i]->render();
                gun = this->gun;
            }
        }
    }

    if (gun->active != 0) {
        identity(&cameraLocal);
        if (gun->weaponType == ITEM_SORT_TURRET) {
            void **canvas = (void **)g_PaintCanvas;
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            cameraLocal = *(const Matrix *)CameraGetLocal(paint, camera);
            if (this->visible != 0) {
                identity(&rotate);
                MatrixSetRotation(&scaleMatrix, this->spinAngle, 0.0f, 0.0f);
                rotate = scaleMatrix;
                AbyssEngine::AEMath::MatrixMultiply(cameraLocal, rotate);
            }
        }

        uint32_t inactive = 0;
        int offset = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            Vector *gunPos = (Vector *)((char *)gun->positions + offset);
            if (gunPos->x != -1000.0f) {
                MatrixSetTranslation(&local, gunPos->x, gunPos->y, gunPos->z);
                ::VectorNormalize((Vector *)&local, (Vector *)((char *)gun->velocities + offset));
                this->dir = *(const Vector *)&local;

                if (this->visible != 0) {
                    muzzle.x = -this->dir.x;
                    muzzle.y = -this->dir.y;
                    muzzle.z = -this->dir.z;
                    if ((uint32_t)(gun->itemIndex - 0xb4) > 2) {
                        void **canvas = (void **)g_PaintCanvas;
                        void *paint = *canvas;
                        void *camera = CameraGetCurrent(paint);
                        cameraLocal = *(const Matrix *)CameraGetLocal(paint, camera);
                        MatrixGetDir(&dir, &cameraLocal);
                        muzzle = dir;
                        gun = this->gun;
                    }

                    this->orientation.m[0] = cameraLocal.m[0];
                    this->orientation.m[1] = cameraLocal.m[1];
                    this->orientation.m[4] = cameraLocal.m[4];
                    this->orientation.m[5] = cameraLocal.m[5];
                    this->orientation.m[8] = cameraLocal.m[10];
                    this->orientation.m[9] = cameraLocal.m[11];
                    this->orientation.m[2] = -muzzle.x;
                    this->orientation.m[6] = -muzzle.y;
                    this->orientation.m[10] = -muzzle.z;

                    int scale = ((int *)gun->lifetimes)[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (gun->hitSmall != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                    }
                } else if (this->useEgoOrientation == 0) {
                    Vector base;
                    if (this->level == nullptr || this->level->getPlayer() == 0 ||
                        gun->isPlayerGun() == 0) {
                        base.x = 0.0f;
                        base.y = 1.0f;
                        base.z = 0.0f;
                    } else {
                        base = *(const Vector *)((char *)gun->upVectors + offset);
                    }
                    this->up = base;
                    VectorCross(&side, &this->up, &this->dir);
                    ::VectorNormalize(&this->side, &side);
                    VectorCross(&side, &this->side, &this->dir);
                    ::VectorNormalize(&this->up, &side);

                    this->orientation.m[0] = this->side.x;
                    this->orientation.m[1] = this->up.x;
                    this->orientation.m[2] = this->dir.x;
                    this->orientation.m[4] = this->side.y;
                    this->orientation.m[5] = this->up.y;
                    this->orientation.m[6] = this->dir.y;
                    this->orientation.m[8] = this->side.z;
                    this->orientation.m[9] = this->up.z;
                    this->orientation.m[10] = this->dir.z;

                    int scale = ((int *)gun->lifetimes)[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (gun->hitSmall != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                        if (this->isRocketGun() == 0)
                            gun->hitSmall = 0;
                    }
                } else {
                    Player *player = (Player *)(uint64_t)this->level->getPlayer();
                    identity(&local);
                    // RAWREAD: PlayerEgo::maneuverParam / field_0x80 (orig ABI 0x7c/0x80) and the orig-ABI 0x40 matrix,
                    // reached through a Player* local that is really a PlayerEgo*; recompiled 64-bit header offsets don't align.
                    MatrixSetRotation(&scaleMatrix, F<float>(player, 0x7c), F<float>(player, 0x80), 0.0f);
                    AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)((char *)player + 0x40), local);
                    this->orientation = *(const Matrix *)((char *)player + 0x40);
                    MatrixSetTranslation(&scaleMatrix, gunPos->x, gunPos->y, gunPos->z);
                    MatrixGetDir(&dir, &scaleMatrix);
                    ::VectorNormalize(&muzzle, &dir);
                    muzzle *= gun->field_0x50;
                    *(Vector *)((char *)gun->velocities + offset) = muzzle;
                    F<float>(player, 0x7c) = 0.0f;
                    F<float>(player, 0x80) = 0.0f;
                    MatrixSetRotation(&scaleMatrix, this->rollAngle, 0.0f, 0.0f);
                    TransformSetLocal(*(void **)g_PaintCanvas, this->secondaryTransform, &scaleMatrix);
                }

                if (*(uint8_t *)g_ObjectGunRenderScaleFlag != 0)
                    MatrixSetScaling(&local, this->scaleX, this->scaleY, this->scaleZ);

                if (this->gun->weaponType == ITEM_SORT_MINE) {
                    Array<Vector*> *spins = (Array<Vector*> *)this->gun->wobbleOffsets;
                    Vector *spin = spins->data()[i];
                    if (spin != nullptr && this->deltaTime > 0) {
                        MatrixSetRotation(&local, spin->x, spin->y, spin->z);
                        float step = (float)this->deltaTime * 0.02f;
                        float neg = -step;
                        spin->x += spin->x < 0.0f ? neg : step;
                        spin->y += spin->y < 0.0f ? neg : step;
                        spin->z += spin->z < 0.0f ? neg : step;
                    }
                    MatrixSetScaling(&local, this->scaleX, this->scaleY, this->scaleZ);
                }

                void **canvas = (void **)g_PaintCanvas;
                TransformSetLocal(*canvas, this->transform, &this->orientation);
                DrawTransform(*canvas, this->transform, 0);
            } else {
                ++inactive;
            }
            offset += 0xc;
            gun = this->gun;
        }
        if (gun->directionCount <= (int)inactive)
            gun->active = 0;
    }

    if (this->wasFiring != 0 && this->geometry != nullptr)
        this->geometry->render();
    this->deltaTime = 0;
}
