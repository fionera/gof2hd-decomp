#include "gof2/ObjectGun.h"
#include "gof2/Level.h"
#include "gof2/Transform.h"
#include "gof2/Explosion.h"
#include "gof2/PlayerEgo.h"
#include "gof2/Gun.h"
#include "gof2/Player.h"
#include "gof2/AEGeometry.h"

// Byte-offset reader retained ONLY for foreign-class fields that are not yet
// modelled in their (out-of-batch) header (currently a few Player fields).
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }


ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self);
extern "C" void ObjectGun_delete(ObjectGun *self);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *self);
extern "C" void ArrayReleaseClasses_Explosion(Array<Explosion*> *self);
extern "C" void *Array_Explosion_dtor(Array<Explosion*> *self);
extern "C" void operator_delete_array(void *self);
void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
extern "C" void ObjectGun_setEnemies_impl(void *items);
extern "C" void Matrix_ctor(Matrix *self);
void TransformCreate(void *canvas, uint32_t *transform);
extern "C" void *operator_new(uint32_t size);
extern "C" void *operator_new_array(uint32_t size);
extern "C" void Array_Explosion_ctor(Array<Explosion*> *self);
extern "C" void ArraySetLength_Explosion(uint32_t length, Array<Explosion*> *self);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setWeaponIndex(Explosion *self, int weapon);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
uint32_t TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Player_getPosition(Vector *out, Player *self);
// __aeabi_memcpy is declared by gof2/AEGeometry.h (returns void*)
void MatrixRotateVector(void *out, const void *matrix, const void *vec);
extern "C" void Vector_add_assign(Vector *self, const Vector *rhs);
void *CameraGetCurrent(void *canvas);
void *CameraGetLocal(void *canvas, void *camera);
extern "C" void Matrix_copy(Matrix *dst, const Matrix *src);
void MatrixGetDir(Vector *out, const Matrix *matrix);
void MatrixGetUp(Vector *out, const Matrix *matrix);
extern "C" void Matrix_multiply(Matrix *out, const Matrix *rhs);
extern "C" Vector *Matrix_getVector(void *self);
extern "C" void Matrix_multiply_out(Matrix *out, const Vector *a, const Vector *b);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const Matrix *matrix);
extern "C" void Explosion_update(Explosion *self, int dt, TargetFollowCamera *camera);
void MatrixSetRotation(Matrix *dst, float x, float y, float z);
extern "C" void Matrix_multiply_inplace(Matrix *self, const Matrix *rhs);
void MatrixSetTranslation(Matrix *dst, float x, float y, float z);
void VectorNormalize(Vector *out, const Vector *in);
extern "C" void Vector_assign(Vector *dst, const Vector *src);
extern "C" void Vector_negate(Vector *out, const Vector *in);
extern "C" void Vector_scale(Vector *self, float scale);
extern "C" void MatrixGetTranslation(Vector *out, const Matrix *matrix);
extern "C" void Vector_add_scaled(Vector *out, const Vector *dir, float scale);
void VectorCross(Vector *out, const Vector *a, const Vector *b);
extern "C" void MatrixSetScaling(Matrix *dst, float x, float y, float z);
void TransformSetLocal(void *canvas, uint32_t transform, Matrix *matrix);
void DrawTransform(void *canvas, uint32_t transform, int flags);

// ---- _ObjectGun_15f94c.cpp ----
void _ZN9ObjectGunD0Ev(ObjectGun *self)
{
    return ObjectGun_delete(_ZN9ObjectGunD1Ev(self));
}

// ---- setScaling_16035a.cpp ----
void ObjectGun::setScaling(int x, int y)
{
    this->field_0x3c = (float)x;
    this->field_0x40 = (float)y;
}

// ---- _ObjectGun_15f904.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;


ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self)
{
    self->field_0x0 = (char *)ObjectGun_vtable + 8;

    AEGeometry *geometry = self->field_0x18;
    if (geometry != 0)
        operator_delete(AEGeometry_dtor(geometry));

    Array<Explosion*> *explosions = self->field_0x2c;
    self->field_0x18 = 0;
    if (explosions != 0) {
        ArrayReleaseClasses_Explosion(explosions);
        explosions = self->field_0x2c;
        if (explosions != 0)
            operator_delete(Array_Explosion_dtor(explosions));
        self->field_0x2c = 0;
    }

    operator_delete_array(self->field_0x30);
    self->field_0x30 = 0;
    return self;
}

// ---- replaceGun_160374.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;

void ObjectGun::replaceGun(int mesh)
{
    void **canvas = (void **)g_PaintCanvas;
    TransformRemoveMesh(*canvas, this->field_0x10, this->field_0x28);
    this->field_0x28 = mesh;
    return TransformAddMesh(*canvas, this->field_0x10, (uint16_t)mesh, 0);
}

// ---- setEnemies_15f95c.cpp ----
void ObjectGun::setEnemies(Array<Player*> *enemies)
{
    // Forwards the enemy list to the underlying implementation.
    return ObjectGun_setEnemies_impl((void *)enemies->data());
}

// ---- ObjectGun_15f700.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunScaleFlag;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" int g_ObjectGunPlayerGunIds[];


ObjectGun::ObjectGun(int, Gun *gun, int mesh, uint32_t, Level *level)
{
    ObjectGun *self = this;
    // zero the embedded "up" vector (0x60..0x68) and "dir" vector (0x50..0x58)
    self->field_0x60 = 0.0f;
    self->field_0x64 = 0.0f;
    self->field_0x68 = 0.0f;
    self->field_0x50 = 0.0f;
    self->field_0x54 = 0.0f;
    self->field_0x58 = 0.0f;
    self->field_0x70 = 0.0f;
    self->field_0x0 = (char *)ObjectGun_vtable + 8;
    Matrix_ctor((Matrix *)((char *)self + 0x74));

    void **canvas = (void **)g_PaintCanvas;
    self->field_0x24 = 0;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    self->field_0x4 = -1;
    self->field_0x8 = gun;
    self->field_0xc = level;
    self->field_0x14 = -1;
    TransformCreate(*canvas, &self->field_0x10);
    self->field_0x28 = mesh;
    TransformAddMesh(*canvas, self->field_0x10, (uint16_t)mesh, 0);
    self->field_0x20 = 0;
    self->field_0x3c = 1.0f;
    self->field_0x40 = 1.0f;
    self->field_0x44 = 1.0f;
    self->field_0x48 = 0.0f;

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
    self->field_0x4c = (uint8_t)visible;

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
    self->field_0x3c = 0.6f;
    self->field_0x40 = 0.6f;
    self->field_0x44 = 0.6f;

check_special_type:
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    goto after_special_type;

scale_to_medium:
    self->field_0x3c = 0.7f;
    self->field_0x40 = 0.7f;
    self->field_0x44 = 0.7f;
    goto after_special_type;

make_explosions:
    {
        Array<Explosion*> *explosions = new Array<Explosion*>();
        self->field_0x2c = explosions;
        ArraySetLength_Explosion(gun->count, explosions);
        explosions = self->field_0x2c;
        self->field_0x30 = (uint8_t *)operator_new_array((uint32_t)explosions->size());

        for (uint32_t i = 0; i < explosions->size(); ++i) {
            Explosion *explosion = (Explosion *)operator_new(0x68);
            int explosionType = 10;
            int weapon = gun->itemIndex;
            if (weapon == 0xb1)
                explosionType = 9;
            if (weapon == 0xb0)
                explosionType = 8;
            Explosion_ctor(explosion, explosionType);
            self->field_0x2c->data()[i] = explosion;
            Explosion_setWeaponIndex(self->field_0x2c->data()[i],
                                     gun->itemIndex);
            self->field_0x30[i] = 1;
            explosions = self->field_0x2c;
        }
    }

after_special_type:

    AEGeometry *geometry;
    if (gun->field_0xa8 == 0) {
        if (((Gun *)(gun))->isPlayerGun() == 0 ||
            g_ObjectGunPlayerGunIds[gun->itemIndex] < 0) {
            geometry = 0;
            self->field_0x1c = 0;
            goto done;
        }
        int createGeometry = gun->weaponType - 0xb;
        if (createGeometry != 0)
            createGeometry = 1;
        self->field_0x1c = createGeometry;
        if (createGeometry == 0) {
            geometry = 0;
            goto done;
        }
    } else {
        self->field_0x1c = 1;
    }

    geometry = (AEGeometry *)operator_new(0xc0);
    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[gun->itemIndex].id, *canvas, false);

done:
    self->field_0x1d = 0;
    self->field_0x18 = geometry;
    self->field_0x34 = 0;
}

// ---- update_15f964.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" uint32_t (*g_TransformGetObject)(void *canvas, uint32_t mesh);
__attribute__((visibility("hidden"))) extern "C" void (*g_TransformSetState)(uint32_t object, int state, int value);


static inline Explosion *explosion_at(ObjectGun *self, uint32_t index)
{
    return self->field_0x2c->data()[index];
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
    uint32_t transform = TransformGetTransform(*canvas, this->field_0x10);
    ((AbyssEngine::Transform *)((uint64_t)transform))->Update((int64_t)dt, 0);

    this->field_0x34 = dt;
    Gun *gun = this->field_0x8;
    ((Gun *)(gun))->update(dt);

    if (this->field_0x1c == 0) {
        if (((Gun *)(gun))->isPlayerGun() == 0) {
            Player *owner = gun->field_0x4;
            if (owner != 0) {
                KIPlayer *ki = ((Player *)(owner))->getKIPlayer();
                if (ki != 0 && F<uint8_t>(((Player *)(owner))->getKIPlayer(), 0x3f) != 0) {
                    this->field_0x1c = 1;
                    AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
                    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[this->field_0x8->itemIndex].id,
                                    *canvas, false);
                    this->field_0x18 = geometry;
                }
            }
        }
        if (this->field_0x1c == 0)
            goto after_geometry;
    }

    gun = this->field_0x8;
    if (gun->delayActive == 0) {
        uint32_t object = g_TransformGetObject(*canvas, this->field_0x18->transform);
        g_TransformSetState(object, 0, 0);
        object = g_TransformGetObject(*canvas, this->field_0x18->transform);
        g_TransformSetState(object, 3, 0);
        object = g_TransformGetObject(*canvas, this->field_0x18->transform);
        g_TransformSetState(object, 1, 0);
        goto after_geometry;
    }

    {
        Player *player = (Player *)(uint64_t)((Level *)(this->field_0xc))->getPlayer();
        if (((Gun *)(gun))->isPlayerGun() != 0) {
            ((PlayerEgo *)(&position))->getPosition();
        } else {
            Player_getPosition(&position, gun->field_0x4);
        }

        gun = this->field_0x8;
        void *matrixSource = player;
        if (((Gun *)(gun))->isPlayerGun() == 0)
            matrixSource = (char *)gun + 0x4;
        __aeabi_memcpy(&playerMatrix, (char *)F<void *>(matrixSource, 0x0) + 0x4, 0x3c);

        offsets.x = gun->offsetX;
        offsets.y = gun->offsetY;
        offsets.z = gun->offsetZ + 8.0f;
        MatrixRotateVector(&workMatrix, &playerMatrix, &offsets);
        Vector_add_assign(&position, (Vector *)&workMatrix);
        ((AEGeometry *)(this->field_0x18))->setPosition(position);

        transform = TransformGetTransform(*canvas, this->field_0x18->transform);
        ((AbyssEngine::Transform *)((uint64_t)transform))->Update((int64_t)dt, 0);

        if (this->field_0x8->weaponType != 8) {
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            Matrix_copy(&cameraMatrix, (Matrix *)CameraGetLocal(paint, camera));
            MatrixGetDir(&dir, &cameraMatrix);
            MatrixGetUp(&up, &cameraMatrix);
            ((AEGeometry *)(this->field_0x18))->setDirection(dir, up);
            goto after_geometry;
        }

        Matrix *m0 = &((AEGeometry *)(this->field_0x18))->getMatrix();
        Matrix *m1 = &((AEGeometry *)(this->field_0x18))->getMatrix();
        Matrix_multiply(&workMatrix, m0);

        gun = this->field_0x8;
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
                offsets.x = left - gun->offsetX;
                if (gun->field_0xa6 != 0)
                    offsets.x = gun->offsetX + right;
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
        AEGeometry_setMatrix(this->field_0x18, &workMatrix);
        ((AEGeometry *)(this->field_0x18))->translate(*(Vector *)&cameraMatrix);
        (void)m1;
    }

after_geometry:
    gun = this->field_0x8;
    this->field_0x1d = gun->delayActive;
    if (gun->weaponType == 0x19) {
        int positionOffset = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                if (this->field_0x30[i] != 0) {
                    ((Explosion *)(explosion_at(this, i)))->start((Vector *)((char *)(__INTPTR_TYPE__)gun->field_0x30 + positionOffset), &zero);
                    this->field_0x30[i] = 0;
                }
                Explosion *explosion = explosion_at(this, i);
                Explosion_update(explosion, dt, 0);
                if (((Explosion *)(explosion))->isPlaying() == 0) {
                    gun = this->field_0x8;
                    gun->hitFlags[i] = 0;
                    gun->field_0x88 = 0;
                    this->field_0x30[i] = 1;
                    ((Explosion *)(explosion_at(this, i)))->reset();
                }
            }
            positionOffset += 0xc;
            gun = this->field_0x8;
        }
    }
}

// ---- render_15fda8.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunRenderScaleFlag;


static inline Explosion *render_explosion_at(ObjectGun *self, uint32_t index)
{
    return self->field_0x2c->data()[index];
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

    Gun *gun = this->field_0x8;
    ((Gun *)(gun))->render();

    if (gun->weaponType == 0x19) {
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                ((Explosion *)(render_explosion_at(this, i)))->render();
                gun = this->field_0x8;
            }
        }
    }

    if (gun->field_0x4c != 0) {
        identity(&cameraLocal);
        if (gun->weaponType == 8) {
            void **canvas = (void **)g_PaintCanvas;
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            Matrix_copy(&cameraLocal, (Matrix *)CameraGetLocal(paint, camera));
            if (this->field_0x4c != 0) {
                identity(&rotate);
                MatrixSetRotation(&scaleMatrix, this->field_0x48, 0.0f, 0.0f);
                Matrix_copy(&rotate, &scaleMatrix);
                Matrix_multiply_inplace(&cameraLocal, &rotate);
            }
        }

        uint32_t inactive = 0;
        int offset = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            Vector *position = (Vector *)((char *)gun->positions + offset);
            if (position->x != -1000.0f) {
                MatrixSetTranslation(&local, position->x, position->y, position->z);
                VectorNormalize((Vector *)&local, (Vector *)((char *)gun->velocities + offset));
                Vector_assign((Vector *)((char *)this + 0x50), (Vector *)&local);

                if (this->field_0x4c != 0) {
                    Vector_negate(&muzzle, (Vector *)((char *)this + 0x50));
                    if ((uint32_t)(gun->itemIndex - 0xb4) > 2) {
                        void **canvas = (void **)g_PaintCanvas;
                        void *paint = *canvas;
                        void *camera = CameraGetCurrent(paint);
                        Matrix_copy(&cameraLocal, (Matrix *)CameraGetLocal(paint, camera));
                        MatrixGetDir(&dir, &cameraLocal);
                        Vector_assign(&muzzle, &dir);
                        gun = this->field_0x8;
                    }

                    this->field_0x74 = cameraLocal.m[0];
                    this->field_0x78 = cameraLocal.m[1];
                    this->field_0x84 = cameraLocal.m[4];
                    this->field_0x88 = cameraLocal.m[5];
                    this->field_0x94 = cameraLocal.m[10];
                    this->field_0x98 = cameraLocal.m[11];
                    this->field_0x7c = -muzzle.x;
                    this->field_0x8c = -muzzle.y;
                    this->field_0x9c = -muzzle.z;

                    int scale = ((int *)gun->lifetimes)[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (gun->field_0x4d != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                    }
                } else if (this->field_0x24 == 0) {
                    Vector *base;
                    if (this->field_0xc == 0 || ((Level *)(this->field_0xc))->getPlayer() == 0 ||
                        ((Gun *)(gun))->isPlayerGun() == 0) {
                        up.x = 0.0f;
                        up.y = 1.0f;
                        up.z = 0.0f;
                        base = &up;
                    } else {
                        base = (Vector *)((char *)gun->field_0x24 + offset);
                    }
                    Vector_assign((Vector *)((char *)this + 0x5c), base);
                    VectorCross(&side, (Vector *)((char *)this + 0x5c), (Vector *)((char *)this + 0x50));
                    VectorNormalize((Vector *)((char *)this + 0x68), &side);
                    VectorCross(&side, (Vector *)((char *)this + 0x68), (Vector *)((char *)this + 0x50));
                    VectorNormalize((Vector *)((char *)this + 0x5c), &side);

                    this->field_0x74 = this->field_0x68;
                    this->field_0x78 = this->field_0x5c;
                    this->field_0x7c = this->field_0x50;
                    this->field_0x84 = this->field_0x6c;
                    this->field_0x88 = this->field_0x60;
                    this->field_0x8c = this->field_0x54;
                    this->field_0x94 = this->field_0x70;
                    this->field_0x98 = this->field_0x64;
                    this->field_0x9c = this->field_0x58;

                    int scale = ((int *)gun->lifetimes)[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (gun->field_0x4d != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                        typedef int (*DoneFn)(ObjectGun *);
                        if (((DoneFn *)this->field_0x0)[8](this) == 0)
                            gun->field_0x4d = 0;
                    }
                } else {
                    Player *player = (Player *)(uint64_t)((Level *)(this->field_0xc))->getPlayer();
                    identity(&local);
                    // Player::field_0x7c / field_0x80 are not yet modelled in Player.h
                    // (out-of-batch header) -> byte-offset accessed via F<float>.
                    MatrixSetRotation(&scaleMatrix, F<float>(player, 0x7c), F<float>(player, 0x80), 0.0f);
                    Matrix_multiply_inplace((Matrix *)((char *)player + 0x40), &local);
                    Matrix_copy((Matrix *)((char *)this + 0x74), (Matrix *)((char *)player + 0x40));
                    MatrixSetTranslation(&scaleMatrix, position->x, position->y, position->z);
                    MatrixGetDir(&dir, &scaleMatrix);
                    VectorNormalize(&muzzle, &dir);
                    Vector_scale(&muzzle, gun->field_0x50);
                    Vector_assign((Vector *)((char *)gun->velocities + offset), &muzzle);
                    F<float>(player, 0x7c) = 0.0f;
                    F<float>(player, 0x80) = 0.0f;
                    MatrixSetRotation(&scaleMatrix, this->field_0x20, 0.0f, 0.0f);
                    TransformSetLocal(*(void **)g_PaintCanvas, this->field_0x14, &scaleMatrix);
                }

                if (*(uint8_t *)g_ObjectGunRenderScaleFlag != 0)
                    MatrixSetScaling(&local, this->field_0x3c, this->field_0x40,
                                     this->field_0x44);

                if (this->field_0x8->weaponType == 0xb) {
                    Array<Vector*> *spins = (Array<Vector*> *)this->field_0x8->field_0xac;
                    Vector *spin = spins->data()[i];
                    if (spin != 0 && this->field_0x34 > 0) {
                        MatrixSetRotation(&local, spin->x, spin->y, spin->z);
                        float step = (float)this->field_0x34 * 0.02f;
                        float neg = -step;
                        spin->x += spin->x < 0.0f ? neg : step;
                        spin->y += spin->y < 0.0f ? neg : step;
                        spin->z += spin->z < 0.0f ? neg : step;
                    }
                    MatrixSetScaling(&local, this->field_0x3c, this->field_0x40,
                                     this->field_0x44);
                }

                void **canvas = (void **)g_PaintCanvas;
                TransformSetLocal(*canvas, this->field_0x10, (Matrix *)((char *)this + 0x74));
                DrawTransform(*canvas, this->field_0x10, 0);
            } else {
                ++inactive;
            }
            offset += 0xc;
            gun = this->field_0x8;
        }
        if (gun->field_0x14 <= (int)inactive)
            gun->field_0x4c = 0;
    }

    if (this->field_0x1d != 0 && this->field_0x18 != 0)
        ((AEGeometry *)(this->field_0x18))->render();
    this->field_0x34 = 0;
}
