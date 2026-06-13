#include "gof2/game/weapons/ObjectGun.h"
#include "gof2/game/world/Level.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/mission/Explosion.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/ship/Player.h"
#include "gof2/engine/render/AEGeometry.h"

// Byte-offset reader retained ONLY for foreign-class fields that are not yet
// modelled in their (out-of-batch) header (currently a few Player fields).
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }

namespace AbyssEngine { namespace AEMath {
void MatrixMultiply(Matrix &, const Matrix &);
Vector operator-(const Vector &);
} }

ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self);
extern "C" void ObjectGun_delete(ObjectGun *self);
void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
extern "C" void ObjectGun_setEnemies_impl(void *items);
void TransformCreate(void *canvas, uint32_t *transform);
extern "C" void Explosion_ctor(Explosion *self, int type);
uint32_t TransformGetTransform(void *canvas, uint32_t transform);
// __aeabi_memcpy is declared by gof2/AEGeometry.h (returns void*)
void MatrixRotateVector(void *out, const void *matrix, const void *vec);
void *CameraGetCurrent(void *canvas);
void *CameraGetLocal(void *canvas, void *camera);
void MatrixGetDir(Vector *out, const Matrix *matrix);
void MatrixGetUp(Vector *out, const Matrix *matrix);
void MatrixSetRotation(Matrix *dst, float x, float y, float z);
void MatrixSetTranslation(Matrix *dst, float x, float y, float z);
void VectorNormalize(Vector *out, const Vector *in);
extern "C" void MatrixGetTranslation(Vector *out, const Matrix *matrix);
void VectorCross(Vector *out, const Vector *a, const Vector *b);
extern "C" void MatrixSetScaling(Matrix *dst, float x, float y, float z);
void TransformSetLocal(void *canvas, uint32_t transform, Matrix *matrix);
void DrawTransform(void *canvas, uint32_t transform, int flags);

void _ZN9ObjectGunD0Ev(ObjectGun *self)
{
    return ObjectGun_delete(_ZN9ObjectGunD1Ev(self));
}

void ObjectGun::setScaling(int x, int y)
{
    this->scaleX = (float)x;
    this->scaleY = (float)y;
}

__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;

ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self)
{
    self->vtable = (char *)ObjectGun_vtable + 8;

    AEGeometry *geometry = self->geometry;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }

    Array<Explosion*> *explosions = self->explosions;
    self->geometry = 0;
    if (explosions != 0) {
        for (Explosion *e : *explosions)
            delete e;
        explosions->clear();
        delete self->explosions;
        self->explosions = 0;
    }

    ::operator delete[](self->explosionReady);
    self->explosionReady = 0;
    return self;
}

// ObjectGun::~ObjectGun() -- real destructor.
//   Resets the vtable slot, then releases the gun geometry, the per-explosion
//   array (releasing each element first), and the ready-flag buffer.
ObjectGun::~ObjectGun()
{
    this->vtable = (char *)ObjectGun_vtable + 8;

    AEGeometry *geometry = this->geometry;
    if (geometry != 0) {
        ((AEGeometry *)geometry)->~AEGeometry();
        ::operator delete(geometry);
    }

    Array<Explosion*> *explosions = this->explosions;
    this->geometry = 0;
    if (explosions != 0) {
        for (Explosion *e : *explosions)
            delete e;
        explosions->clear();
        delete this->explosions;
        this->explosions = 0;
    }

    ::operator delete[](this->explosionReady);
    this->explosionReady = 0;
}

__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;

void ObjectGun::replaceGun(int mesh)
{
    void **canvas = (void **)g_PaintCanvas;
    TransformRemoveMesh(*canvas, this->transform, this->meshId);
    this->meshId = mesh;
    return TransformAddMesh(*canvas, this->transform, (uint16_t)mesh, 0);
}

void ObjectGun::setEnemies(Array<Player*> *enemies)
{
    // Forwards the enemy list to the underlying implementation.
    return ObjectGun_setEnemies_impl((void *)enemies->data());
}

__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunScaleFlag;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" int g_ObjectGunPlayerGunIds[];

ObjectGun::ObjectGun(int, Gun *gun, int mesh, uint32_t, Level *level)
{
    ObjectGun *self = this;
    // zero the embedded "up" vector (0x5c..0x64), "dir" vector (0x50..0x58) and
    // the side vector's z (0x70)
    self->up.y = 0.0f;
    self->up.z = 0.0f;
    self->side.x = 0.0f;
    self->dir.x = 0.0f;
    self->dir.y = 0.0f;
    self->dir.z = 0.0f;
    self->side.z = 0.0f;
    self->vtable = (char *)ObjectGun_vtable + 8;
    self->orientation.initIdentity();

    void **canvas = (void **)g_PaintCanvas;
    self->useEgoOrientation = 0;
    self->explosions = 0;
    self->explosionReady = 0;
    self->field_0x4 = -1;
    self->gun = gun;
    self->level = level;
    self->secondaryTransform = -1;
    TransformCreate(*canvas, &self->transform);
    self->meshId = mesh;
    TransformAddMesh(*canvas, self->transform, (uint16_t)mesh, 0);
    self->rollAngle = 0;
    self->scaleX = 1.0f;
    self->scaleY = 1.0f;
    self->scaleZ = 1.0f;
    self->spinAngle = 0.0f;

    uint32_t type = gun->weaponType;
    uint32_t visible = 1;
    if (type > 8)
        goto visible_zero;
    {
        uint32_t mask = visible << type;
        if ((mask & 0x10aU) == 0)
            visible = 0;
    }
    goto visible_done;
visible_zero:
    visible = 0;
visible_done:
    self->visible = (uint8_t)visible;

    if (*(uint8_t *)g_ObjectGunScaleFlag == 0)
        goto check_special_type;
    if (type == 1)
        goto scale_to_small;
    if (type == 3)
        goto scale_to_small;
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    if (type == 8)
        goto scale_to_small;
    goto after_special_type;

scale_to_small:
    self->scaleX = 0.6f;
    self->scaleY = 0.6f;
    self->scaleZ = 0.6f;

check_special_type:
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    goto after_special_type;

scale_to_medium:
    self->scaleX = 0.7f;
    self->scaleY = 0.7f;
    self->scaleZ = 0.7f;
    goto after_special_type;

make_explosions:
    {
        Array<Explosion*> *explosions = new Array<Explosion*>();
        self->explosions = explosions;
        explosions->resize(gun->count);
        explosions = self->explosions;
        self->explosionReady = (uint8_t *)::operator new[]((uint32_t)explosions->size());

        for (uint32_t i = 0; i < explosions->size(); ++i) {
            Explosion *explosion = (Explosion *)::operator new(0x68);
            int explosionType = 10;
            int weapon = gun->itemIndex;
            if (weapon == 0xb1)
                explosionType = 9;
            if (weapon == 0xb0)
                explosionType = 8;
            Explosion_ctor(explosion, explosionType);
            self->explosions->data()[i] = explosion;
            ((Explosion *)(self->explosions->data()[i]))->setWeaponIndex(gun->itemIndex);
            self->explosionReady[i] = 1;
            explosions = self->explosions;
        }
    }

after_special_type:

    AEGeometry *geometry;
    if (gun->field_0xa8 == 0) {
        if (((Gun *)(gun))->isPlayerGun() == 0 ||
            g_ObjectGunPlayerGunIds[gun->itemIndex] < 0) {
            geometry = 0;
            self->hasGeometry = 0;
            goto done;
        }
        int createGeometry = gun->weaponType - 0xb;
        if (createGeometry != 0)
            createGeometry = 1;
        self->hasGeometry = createGeometry;
        if (createGeometry == 0) {
            geometry = 0;
            goto done;
        }
    } else {
        self->hasGeometry = 1;
    }

    geometry = (AEGeometry *)::operator new(0xc0);
    new ((void *)geometry) AEGeometry((uint16_t)g_ObjectGunGeometryIds[gun->itemIndex].id, (PaintCanvas *)*canvas, false);

done:
    self->wasFiring = 0;
    self->geometry = geometry;
    self->deltaTime = 0;
}

__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" uint32_t (*g_TransformGetObject)(void *canvas, uint32_t mesh);
__attribute__((visibility("hidden"))) extern "C" void (*g_TransformSetState)(uint32_t object, int state, int value);

static inline Explosion *explosion_at(ObjectGun *self, uint32_t index)
{
    return self->explosions->data()[index];
}

void ObjectGun::update(int dt)
{
    Matrix playerMatrix;
    Matrix workMatrix;
    Matrix cameraMatrix;
    Vector position;
    Vector rotated;
    Vector dir;
    Vector up;
    Vector offsets;
    Vector zero = {0.0f, 0.0f, 0.0f};

    void **canvas = (void **)g_PaintCanvas;
    uint32_t transform = TransformGetTransform(*canvas, this->transform);
    ((AbyssEngine::Transform *)((uint64_t)transform))->Update((int64_t)dt, 0);

    this->deltaTime = dt;
    Gun *gun = this->gun;
    ((Gun *)(gun))->update(dt);

    if (this->hasGeometry == 0) {
        if (((Gun *)(gun))->isPlayerGun() == 0) {
            Player *owner = gun->field_0x4;
            if (owner != 0) {
                KIPlayer *ki = ((Player *)(owner))->getKIPlayer();
                if (ki != 0 && F<uint8_t>(((Player *)(owner))->getKIPlayer(), 0x3f) != 0) {
                    this->hasGeometry = 1;
                    AEGeometry *geometry = (AEGeometry *)::operator new(0xc0);
                    new ((void *)geometry) AEGeometry((uint16_t)g_ObjectGunGeometryIds[this->gun->itemIndex].id,
                                    (PaintCanvas *)*canvas, false);
                    this->geometry = geometry;
                }
            }
        }
        if (this->hasGeometry == 0)
            goto after_geometry;
    }

    gun = this->gun;
    if (gun->delayActive == 0) {
        uint32_t object = g_TransformGetObject(*canvas, this->geometry->transform);
        g_TransformSetState(object, 0, 0);
        object = g_TransformGetObject(*canvas, this->geometry->transform);
        g_TransformSetState(object, 3, 0);
        object = g_TransformGetObject(*canvas, this->geometry->transform);
        g_TransformSetState(object, 1, 0);
        goto after_geometry;
    }

    {
        Player *player = (Player *)(uint64_t)((Level *)(this->level))->getPlayer();
        if (((Gun *)(gun))->isPlayerGun() != 0) {
            position = ((PlayerEgo *)(player))->getPosition();
        } else {
            ((Player *)(gun->field_0x4))->getPosition(&position);
        }

        gun = this->gun;
        void *matrixSource = player;
        if (((Gun *)(gun))->isPlayerGun() == 0)
            matrixSource = (char *)gun + 0x4;
        __aeabi_memcpy(&playerMatrix, (char *)F<void *>(matrixSource, 0x0) + 0x4, 0x3c);

        offsets.x = gun->offset.x;
        offsets.y = gun->offset.y;
        offsets.z = gun->offset.z + 8.0f;
        MatrixRotateVector(&workMatrix, &playerMatrix, &offsets);
        *(Vector *)&position += *(const Vector *)((Vector *)&workMatrix);
        ((AEGeometry *)(this->geometry))->setPosition(position);

        transform = TransformGetTransform(*canvas, this->geometry->transform);
        ((AbyssEngine::Transform *)((uint64_t)transform))->Update((int64_t)dt, 0);

        if (this->gun->weaponType != 8) {
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            *(Matrix *)&cameraMatrix = *(const Matrix *)((Matrix *)CameraGetLocal(paint, camera));
            MatrixGetDir(&dir, &cameraMatrix);
            MatrixGetUp(&up, &cameraMatrix);
            ((AEGeometry *)(this->geometry))->setDirection(dir, up);
            goto after_geometry;
        }

        Matrix *m0 = &((AEGeometry *)(this->geometry))->getMatrix();
        Matrix *m1 = &((AEGeometry *)(this->geometry))->getMatrix();
        AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)&workMatrix, *(const Matrix *)m0);

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
            if (weapon == 0xb5) {
                offsets.x = gun->field_0xa6 != 0 ? -1.5f : 1.5f;
            } else {
                float left = weapon == 0x30 ? 20.0f : 15.0f;
                float right = weapon == 0x30 ? -20.0f : -15.0f;
                offsets.x = left - gun->offset.x;
                if (gun->field_0xa6 != 0)
                    offsets.x = gun->offset.x + right;
            }
        }
        if (gun->field_0xa5 != 0) {
            if (weapon == 0xe0) {
                offsets.y = gun->field_0xa7 == 0 ? 13.0f : -13.0f;
            } else if (weapon == 0xb5) {
                offsets.y = gun->field_0xa7 == 0 ? 2.5f : -2.5f;
            }
        }
        MatrixRotateVector(&cameraMatrix, &workMatrix, &offsets);
        ((AEGeometry *)this->geometry)->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(&workMatrix));
        ((AEGeometry *)(this->geometry))->translate(*(Vector *)&cameraMatrix);
        (void)m1;
    }

after_geometry:
    gun = this->gun;
    this->wasFiring = gun->delayActive;
    if (gun->weaponType == 0x19) {
        int positionOffset = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                if (this->explosionReady[i] != 0) {
                    ((Explosion *)(explosion_at(this, i)))->start((Vector *)((char *)(__INTPTR_TYPE__)gun->hitPositions + positionOffset), &zero);
                    this->explosionReady[i] = 0;
                }
                Explosion *explosion = explosion_at(this, i);
                ((Explosion *)(explosion))->update(dt, 0);
                if (((Explosion *)(explosion))->isPlaying() == 0) {
                    gun = this->gun;
                    gun->hitFlags[i] = 0;
                    gun->field_0x88 = 0;
                    this->explosionReady[i] = 1;
                    ((Explosion *)(explosion_at(this, i)))->reset();
                }
            }
            positionOffset += 0xc;
            gun = this->gun;
        }
    }
}

__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunRenderScaleFlag;

static inline Explosion *render_explosion_at(ObjectGun *self, uint32_t index)
{
    return self->explosions->data()[index];
}

static inline void identity(Matrix *m)
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
    ((Gun *)(gun))->render();

    if (gun->weaponType == 0x19) {
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                ((Explosion *)(render_explosion_at(this, i)))->render();
                gun = this->gun;
            }
        }
    }

    if (gun->field_0x4c != 0) {
        identity(&cameraLocal);
        if (gun->weaponType == 8) {
            void **canvas = (void **)g_PaintCanvas;
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            *(Matrix *)&cameraLocal = *(const Matrix *)((Matrix *)CameraGetLocal(paint, camera));
            if (this->visible != 0) {
                identity(&rotate);
                MatrixSetRotation(&scaleMatrix, this->spinAngle, 0.0f, 0.0f);
                *(Matrix *)&rotate = *(const Matrix *)&scaleMatrix;
                AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)&cameraLocal, *(const Matrix *)&rotate);
            }
        }

        uint32_t inactive = 0;
        int offset = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            Vector *position = (Vector *)((char *)gun->positions + offset);
            if (position->x != -1000.0f) {
                MatrixSetTranslation(&local, position->x, position->y, position->z);
                VectorNormalize((Vector *)&local, (Vector *)((char *)gun->velocities + offset));
                this->dir = *(const Vector *)((Vector *)&local);

                if (this->visible != 0) {
                    *(Vector *)&muzzle = -this->dir;
                    if ((uint32_t)(gun->itemIndex - 0xb4) > 2) {
                        void **canvas = (void **)g_PaintCanvas;
                        void *paint = *canvas;
                        void *camera = CameraGetCurrent(paint);
                        *(Matrix *)&cameraLocal = *(const Matrix *)((Matrix *)CameraGetLocal(paint, camera));
                        MatrixGetDir(&dir, &cameraLocal);
                        *(Vector *)&muzzle = *(const Vector *)&dir;
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
                    } else if (gun->field_0x4d != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                    }
                } else if (this->useEgoOrientation == 0) {
                    Vector *base;
                    if (this->level == 0 || ((Level *)(this->level))->getPlayer() == 0 ||
                        ((Gun *)(gun))->isPlayerGun() == 0) {
                        up.x = 0.0f;
                        up.y = 1.0f;
                        up.z = 0.0f;
                        base = &up;
                    } else {
                        base = (Vector *)((char *)gun->field_0x24 + offset);
                    }
                    this->up = *(const Vector *)base;
                    VectorCross(&side, &this->up, &this->dir);
                    VectorNormalize(&this->side, &side);
                    VectorCross(&side, &this->side, &this->dir);
                    VectorNormalize(&this->up, &side);

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
                    } else if (gun->field_0x4d != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                        typedef int (*DoneFn)(ObjectGun *);
                        if (((DoneFn *)this->vtable)[8](this) == 0)
                            gun->field_0x4d = 0;
                    }
                } else {
                    Player *player = (Player *)(uint64_t)((Level *)(this->level))->getPlayer();
                    identity(&local);
                    // Player::field_0x7c / field_0x80 are not yet modelled in Player.h
                    // (out-of-batch header) -> byte-offset accessed via F<float>.
                    MatrixSetRotation(&scaleMatrix, F<float>(player, 0x7c), F<float>(player, 0x80), 0.0f);
                    AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)((char *)player + 0x40), *(const Matrix *)&local);
                    this->orientation = *(const Matrix *)((Matrix *)((char *)player + 0x40));
                    MatrixSetTranslation(&scaleMatrix, position->x, position->y, position->z);
                    MatrixGetDir(&dir, &scaleMatrix);
                    VectorNormalize(&muzzle, &dir);
                    *(Vector *)&muzzle *= (gun->field_0x50);
                    *(Vector *)((char *)gun->velocities + offset) = *(const Vector *)&muzzle;
                    F<float>(player, 0x7c) = 0.0f;
                    F<float>(player, 0x80) = 0.0f;
                    MatrixSetRotation(&scaleMatrix, this->rollAngle, 0.0f, 0.0f);
                    TransformSetLocal(*(void **)g_PaintCanvas, this->secondaryTransform, &scaleMatrix);
                }

                if (*(uint8_t *)g_ObjectGunRenderScaleFlag != 0)
                    MatrixSetScaling(&local, this->scaleX, this->scaleY,
                                     this->scaleZ);

                if (this->gun->weaponType == 0xb) {
                    Array<Vector*> *spins = (Array<Vector*> *)this->gun->field_0xac;
                    Vector *spin = spins->data()[i];
                    if (spin != 0 && this->deltaTime > 0) {
                        MatrixSetRotation(&local, spin->x, spin->y, spin->z);
                        float step = (float)this->deltaTime * 0.02f;
                        float neg = -step;
                        spin->x += spin->x < 0.0f ? neg : step;
                        spin->y += spin->y < 0.0f ? neg : step;
                        spin->z += spin->z < 0.0f ? neg : step;
                    }
                    MatrixSetScaling(&local, this->scaleX, this->scaleY,
                                     this->scaleZ);
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
        if (gun->field_0x14 <= (int)inactive)
            gun->field_0x4c = 0;
    }

    if (this->wasFiring != 0 && this->geometry != 0)
        ((AEGeometry *)(this->geometry))->render();
    this->deltaTime = 0;
}
