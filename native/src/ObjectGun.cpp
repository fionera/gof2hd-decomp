#include "ObjectGun.h"


extern "C" ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self);
extern "C" void ObjectGun_delete(ObjectGun *self);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *self);
extern "C" void ArrayReleaseClasses_Explosion(Array *self);
extern "C" void *Array_Explosion_dtor(Array *self);
extern "C" void operator_delete_array(void *self);
extern "C" void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
extern "C" void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
extern "C" void ObjectGun_setEnemies_impl(void *items);
extern "C" void Matrix_ctor(Matrix *self);
extern "C" void TransformCreate(void *canvas, uint32_t *transform);
extern "C" void *operator_new(uint32_t size);
extern "C" void *operator_new_array(uint32_t size);
extern "C" void Array_Explosion_ctor(Array *self);
extern "C" void ArraySetLength_Explosion(uint32_t length, Array *self);
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setWeaponIndex(Explosion *self, int weapon);
extern "C" int Gun_isPlayerGun(Gun *self);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
extern "C" uint32_t TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Transform_Update(uint64_t transform, int64_t dt, int flags);
extern "C" void Gun_update(Gun *self, int dt);
extern "C" Player *Player_getKIPlayer(Player *self);
extern "C" Player *Level_getPlayer(Level *self);
extern "C" void PlayerEgo_getPosition(Vector *out, Player *self);
extern "C" void Player_getPosition(Vector *out, Player *self);
extern "C" void __aeabi_memcpy(void *dst, const void *src, uint32_t size);
extern "C" void MatrixRotateVector(void *out, const void *matrix, const void *vec);
extern "C" void Vector_add_assign(Vector *self, const Vector *rhs);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void *CameraGetCurrent(void *canvas);
extern "C" void *CameraGetLocal(void *canvas, void *camera);
extern "C" void Matrix_copy(Matrix *dst, const Matrix *src);
extern "C" void MatrixGetDir(Vector *out, const Matrix *matrix);
extern "C" void MatrixGetUp(Vector *out, const Matrix *matrix);
extern "C" void AEGeometry_setDirection(AEGeometry *self, const Vector *dir, const Vector *up);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void Matrix_multiply(Matrix *out, const Matrix *rhs);
extern "C" Vector *Matrix_getVector(void *self);
extern "C" void Matrix_multiply_out(Matrix *out, const Vector *a, const Vector *b);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const Matrix *matrix);
extern "C" void AEGeometry_translate(AEGeometry *self, const Vector *offset);
extern "C" void Explosion_start(Explosion *self, const Vector *position, const Vector *zero);
extern "C" void Explosion_update(Explosion *self, int dt, TargetFollowCamera *camera);
extern "C" int Explosion_isPlaying(Explosion *self);
extern "C" void Explosion_reset(Explosion *self);
extern "C" void Gun_render(Gun *self);
extern "C" void Explosion_render(Explosion *self);
extern "C" void MatrixSetRotation(Matrix *dst, float x, float y, float z);
extern "C" void Matrix_multiply_inplace(Matrix *self, const Matrix *rhs);
extern "C" void MatrixSetTranslation(Matrix *dst, float x, float y, float z);
extern "C" void VectorNormalize(Vector *out, const Vector *in);
extern "C" void Vector_assign(Vector *dst, const Vector *src);
extern "C" void Vector_negate(Vector *out, const Vector *in);
extern "C" void Vector_scale(Vector *self, float scale);
extern "C" void MatrixGetTranslation(Vector *out, const Matrix *matrix);
extern "C" void Vector_add_scaled(Vector *out, const Vector *dir, float scale);
extern "C" void VectorCross(Vector *out, const Vector *a, const Vector *b);
extern "C" void MatrixSetScaling(Matrix *dst, float x, float y, float z);
extern "C" void TransformSetLocal(void *canvas, uint32_t transform, Matrix *matrix);
extern "C" void DrawTransform(void *canvas, uint32_t transform, int flags);
extern "C" void AEGeometry_render(AEGeometry *self);

// ---- _ObjectGun_15f94c.cpp ----
extern "C" void _ZN9ObjectGunD0Ev(ObjectGun *self)
{
    return ObjectGun_delete(_ZN9ObjectGunD1Ev(self));
}

// ---- setScaling_16035a.cpp ----
void ObjectGun::setScaling(int x, int y)
{
    F<float>(this, 0x3c) = (float)x;
    F<float>(this, 0x40) = (float)y;
}

// ---- _ObjectGun_15f904.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;


extern "C" ObjectGun *_ZN9ObjectGunD1Ev(ObjectGun *self)
{
    F<void *>(self, 0x0) = (char *)ObjectGun_vtable + 8;

    AEGeometry *geometry = F<AEGeometry *>(self, 0x18);
    if (geometry != 0)
        operator_delete(AEGeometry_dtor(geometry));

    Array *explosions = F<Array *>(self, 0x2c);
    F<AEGeometry *>(self, 0x18) = 0;
    if (explosions != 0) {
        ArrayReleaseClasses_Explosion(explosions);
        explosions = F<Array *>(self, 0x2c);
        if (explosions != 0)
            operator_delete(Array_Explosion_dtor(explosions));
        F<Array *>(self, 0x2c) = 0;
    }

    operator_delete_array(F<void *>(self, 0x30));
    F<void *>(self, 0x30) = 0;
    return self;
}

// ---- replaceGun_160374.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;

void ObjectGun::replaceGun(int mesh)
{
    void **canvas = (void **)g_PaintCanvas;
    TransformRemoveMesh(*canvas, F<uint32_t>(this, 0x10), F<uint16_t>(this, 0x28));
    F<int>(this, 0x28) = mesh;
    return TransformAddMesh(*canvas, F<uint32_t>(this, 0x10), (uint16_t)mesh, 0);
}

// ---- setEnemies_15f95c.cpp ----
void ObjectGun::setEnemies(Array *enemies)
{
    return ObjectGun_setEnemies_impl(F<void *>(enemies, 0x8));
}

// ---- ObjectGun_15f700.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *ObjectGun_vtable;
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunScaleFlag;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" int g_ObjectGunPlayerGunIds[];


static const Vec4 kZeroVec = {0.0f, 0.0f, 0.0f, 0.0f};
static const Vec4 kDefaultScale = {1.0f, 1.0f, 1.0f, 0.0f};

ObjectGun::ObjectGun(int, Gun *gun, int mesh, uint32_t, Level *level)
{
    ObjectGun *self = this;
    F<Vec4>(self, 0x60) = kZeroVec;
    F<Vec4>(self, 0x50) = kZeroVec;
    F<uint32_t>(self, 0x70) = 0;
    F<void *>(self, 0x0) = (char *)ObjectGun_vtable + 8;
    Matrix_ctor((Matrix *)((char *)self + 0x74));

    void **canvas = (void **)g_PaintCanvas;
    F<uint8_t>(self, 0x24) = 0;
    F<Array *>(self, 0x2c) = 0;
    F<void *>(self, 0x30) = 0;
    F<int>(self, 0x4) = -1;
    F<Gun *>(self, 0x8) = gun;
    F<Level *>(self, 0xc) = level;
    F<int>(self, 0x14) = -1;
    TransformCreate(*canvas, &F<uint32_t>(self, 0x10));
    F<int>(self, 0x28) = mesh;
    TransformAddMesh(*canvas, F<uint32_t>(self, 0x10), (uint16_t)mesh, 0);
    F<uint32_t>(self, 0x20) = 0;
    F<Vec4>(self, 0x3c) = kDefaultScale;

    uint32_t type = F<uint32_t>(gun, 0x5c);
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
    F<uint8_t>(self, 0x4c) = (uint8_t)visible;

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
    F<float>(self, 0x3c) = 0.6f;
    F<float>(self, 0x40) = 0.6f;
    F<float>(self, 0x44) = 0.6f;

check_special_type:
    if (type == 0x19)
        goto make_explosions;
    if (type == 0xb)
        goto scale_to_medium;
    goto after_special_type;

scale_to_medium:
    F<float>(self, 0x3c) = 0.7f;
    F<float>(self, 0x40) = 0.7f;
    F<float>(self, 0x44) = 0.7f;
    goto after_special_type;

make_explosions:
    {
        Array *explosions = (Array *)operator_new(0xc);
        Array_Explosion_ctor(explosions);
        F<Array *>(self, 0x2c) = explosions;
        ArraySetLength_Explosion(F<uint32_t>(gun, 0x8), explosions);
        explosions = F<Array *>(self, 0x2c);
        F<void *>(self, 0x30) = operator_new_array(F<uint32_t>(explosions, 0x0));

        for (uint32_t i = 0; i < F<uint32_t>(explosions, 0x0); ++i) {
            Explosion *explosion = (Explosion *)operator_new(0x68);
            int explosionType = 10;
            int weapon = F<int>(gun, 0x58);
            if (weapon == 0xb1)
                explosionType = 9;
            if (weapon == 0xb0)
                explosionType = 8;
            Explosion_ctor(explosion, explosionType);
            F<Explosion **>(F<Array *>(self, 0x2c), 0x4)[i] = explosion;
            Explosion_setWeaponIndex(F<Explosion **>(F<Array *>(self, 0x2c), 0x4)[i],
                                     F<int>(gun, 0x58));
            F<uint8_t *>(self, 0x30)[i] = 1;
            explosions = F<Array *>(self, 0x2c);
        }
    }

after_special_type:

    AEGeometry *geometry;
    if (F<uint8_t>(gun, 0xa8) == 0) {
        if (Gun_isPlayerGun(gun) == 0 ||
            g_ObjectGunPlayerGunIds[F<int>(gun, 0x58)] < 0) {
            geometry = 0;
            F<uint8_t>(self, 0x1c) = 0;
            goto done;
        }
        int createGeometry = F<int>(gun, 0x5c) - 0xb;
        if (createGeometry != 0)
            createGeometry = 1;
        F<uint8_t>(self, 0x1c) = createGeometry;
        if (createGeometry == 0) {
            geometry = 0;
            goto done;
        }
    } else {
        F<uint8_t>(self, 0x1c) = 1;
    }

    geometry = (AEGeometry *)operator_new(0xc0);
    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[F<int>(gun, 0x58)].id, *canvas, false);

done:
    F<uint8_t>(self, 0x1d) = 0;
    F<AEGeometry *>(self, 0x18) = geometry;
    F<uint32_t>(self, 0x34) = 0;
}

// ---- update_15f964.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" uint32_t (*g_TransformGetObject)(void *canvas, uint32_t mesh);
__attribute__((visibility("hidden"))) extern "C" void (*g_TransformSetState)(uint32_t object, int state, int value);


static inline Explosion *explosion_at(ObjectGun *self, uint32_t index)
{
    return F<Explosion **>(F<Array *>(self, 0x2c), 0x4)[index];
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
    uint32_t transform = TransformGetTransform(*canvas, F<uint32_t>(this, 0x10));
    Transform_Update((uint64_t)transform, (int64_t)dt, 0);

    F<int>(this, 0x34) = dt;
    Gun *gun = F<Gun *>(this, 0x8);
    Gun_update(gun, dt);

    if (F<uint8_t>(this, 0x1c) == 0) {
        if (Gun_isPlayerGun(gun) == 0) {
            Player *owner = F<Player *>(gun, 0x4);
            if (owner != 0) {
                Player *ki = Player_getKIPlayer(owner);
                if (ki != 0 && F<uint8_t>(Player_getKIPlayer(owner), 0x3f) != 0) {
                    F<uint8_t>(this, 0x1c) = 1;
                    AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
                    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[F<int>(F<Gun *>(this, 0x8), 0x58)].id,
                                    *canvas, false);
                    F<AEGeometry *>(this, 0x18) = geometry;
                }
            }
        }
        if (F<uint8_t>(this, 0x1c) == 0)
            goto after_geometry;
    }

    gun = F<Gun *>(this, 0x8);
    if (F<uint8_t>(gun, 0xa9) == 0) {
        uint32_t object = g_TransformGetObject(*canvas, F<uint32_t>(F<AEGeometry *>(this, 0x18), 0xc));
        g_TransformSetState(object, 0, 0);
        object = g_TransformGetObject(*canvas, F<uint32_t>(F<AEGeometry *>(this, 0x18), 0xc));
        g_TransformSetState(object, 3, 0);
        object = g_TransformGetObject(*canvas, F<uint32_t>(F<AEGeometry *>(this, 0x18), 0xc));
        g_TransformSetState(object, 1, 0);
        goto after_geometry;
    }

    {
        Player *player = Level_getPlayer(F<Level *>(this, 0xc));
        if (Gun_isPlayerGun(gun) != 0) {
            PlayerEgo_getPosition(&position, player);
        } else {
            Player_getPosition(&position, F<Player *>(gun, 0x4));
        }

        gun = F<Gun *>(this, 0x8);
        void *matrixSource = player;
        if (Gun_isPlayerGun(gun) == 0)
            matrixSource = (char *)gun + 0x4;
        __aeabi_memcpy(&playerMatrix, (char *)F<void *>(matrixSource, 0x0) + 0x4, 0x3c);

        offsets.x = F<float>(gun, 0x7c);
        offsets.y = F<float>(gun, 0x80);
        offsets.z = F<float>(gun, 0x84) + 8.0f;
        MatrixRotateVector(&workMatrix, &playerMatrix, &offsets);
        Vector_add_assign(&position, (Vector *)&workMatrix);
        AEGeometry_setPosition(F<AEGeometry *>(this, 0x18), &position);

        transform = TransformGetTransform(*canvas, F<uint32_t>(F<AEGeometry *>(this, 0x18), 0xc));
        Transform_Update((uint64_t)transform, (int64_t)dt, 0);

        if (F<int>(F<Gun *>(this, 0x8), 0x5c) != 8) {
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            Matrix_copy(&cameraMatrix, (Matrix *)CameraGetLocal(paint, camera));
            MatrixGetDir(&dir, &cameraMatrix);
            MatrixGetUp(&up, &cameraMatrix);
            AEGeometry_setDirection(F<AEGeometry *>(this, 0x18), &dir, &up);
            goto after_geometry;
        }

        Matrix *m0 = AEGeometry_getMatrix(F<AEGeometry *>(this, 0x18));
        Matrix *m1 = AEGeometry_getMatrix(F<AEGeometry *>(this, 0x18));
        Matrix_multiply(&workMatrix, m0);

        gun = F<Gun *>(this, 0x8);
        int weapon = F<int>(gun, 0x58);
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

        if (F<uint8_t>(gun, 0xa4) != 0) {
            if (weapon == 0xb5) {
                offsets.x = F<uint8_t>(gun, 0xa6) != 0 ? -1.5f : 1.5f;
            } else {
                float left = weapon == 0x30 ? 20.0f : 15.0f;
                float right = weapon == 0x30 ? -20.0f : -15.0f;
                offsets.x = left - F<float>(gun, 0x7c);
                if (F<uint8_t>(gun, 0xa6) != 0)
                    offsets.x = F<float>(gun, 0x7c) + right;
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
        AEGeometry_setMatrix(F<AEGeometry *>(this, 0x18), &workMatrix);
        AEGeometry_translate(F<AEGeometry *>(this, 0x18), (Vector *)&cameraMatrix);
        (void)m1;
    }

after_geometry:
    gun = F<Gun *>(this, 0x8);
    F<uint8_t>(this, 0x1d) = F<uint8_t>(gun, 0xa9);
    if (F<int>(gun, 0x5c) == 0x19) {
        int positionOffset = 0;
        for (uint32_t i = 0; i < F<uint32_t>(gun, 0x8); ++i) {
            if (F<uint8_t *>(gun, 0x40)[i] != 0) {
                if (F<uint8_t *>(this, 0x30)[i] != 0) {
                    Explosion_start(explosion_at(this, i),
                                    (Vector *)(F<char *>(gun, 0x30) + positionOffset),
                                    &zero);
                    F<uint8_t *>(this, 0x30)[i] = 0;
                }
                Explosion *explosion = explosion_at(this, i);
                Explosion_update(explosion, dt, 0);
                if (Explosion_isPlaying(explosion) == 0) {
                    gun = F<Gun *>(this, 0x8);
                    F<uint8_t *>(gun, 0x40)[i] = 0;
                    F<uint8_t>(gun, 0x88) = 0;
                    F<uint8_t *>(this, 0x30)[i] = 1;
                    Explosion_reset(explosion_at(this, i));
                }
            }
            positionOffset += 0xc;
            gun = F<Gun *>(this, 0x8);
        }
    }
}

// ---- render_15fda8.cpp ----
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunRenderScaleFlag;


static inline Explosion *render_explosion_at(ObjectGun *self, uint32_t index)
{
    return F<Explosion **>(F<Array *>(self, 0x2c), 0x4)[index];
}

static inline void identity(Matrix *m)
{
    for (uint32_t i = 0; i < 15; ++i)
        m->words[i] = 0;
    m->words[0] = 0x3f800000;
    m->words[5] = 0x3f800000;
    m->words[14] = 0x3f800000;
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

    Gun *gun = F<Gun *>(this, 0x8);
    Gun_render(gun);

    if (F<int>(gun, 0x5c) == 0x19) {
        for (uint32_t i = 0; i < F<uint32_t>(gun, 0x8); ++i) {
            if (F<uint8_t *>(gun, 0x40)[i] != 0) {
                Explosion_render(render_explosion_at(this, i));
                gun = F<Gun *>(this, 0x8);
            }
        }
    }

    if (F<uint8_t>(gun, 0x4c) != 0) {
        identity(&cameraLocal);
        if (F<int>(gun, 0x5c) == 8) {
            void **canvas = (void **)g_PaintCanvas;
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            Matrix_copy(&cameraLocal, (Matrix *)CameraGetLocal(paint, camera));
            if (F<uint8_t>(this, 0x4c) != 0) {
                identity(&rotate);
                MatrixSetRotation(&scaleMatrix, F<float>(this, 0x48), 0.0f, 0.0f);
                Matrix_copy(&rotate, &scaleMatrix);
                Matrix_multiply_inplace(&cameraLocal, &rotate);
            }
        }

        uint32_t inactive = 0;
        int offset = 0;
        for (uint32_t i = 0; i < F<uint32_t>(gun, 0x8); ++i) {
            Vector *position = (Vector *)(F<char *>(gun, 0xc) + offset);
            if (position->x != -1000.0f) {
                MatrixSetTranslation(&local, position->x, position->y, position->z);
                VectorNormalize((Vector *)&local, (Vector *)(F<char *>(gun, 0x18) + offset));
                Vector_assign((Vector *)((char *)this + 0x50), (Vector *)&local);

                if (F<uint8_t>(this, 0x4c) != 0) {
                    Vector_negate(&muzzle, (Vector *)((char *)this + 0x50));
                    if ((uint32_t)(F<int>(gun, 0x58) - 0xb4) > 2) {
                        void **canvas = (void **)g_PaintCanvas;
                        void *paint = *canvas;
                        void *camera = CameraGetCurrent(paint);
                        Matrix_copy(&cameraLocal, (Matrix *)CameraGetLocal(paint, camera));
                        MatrixGetDir(&dir, &cameraLocal);
                        Vector_assign(&muzzle, &dir);
                        gun = F<Gun *>(this, 0x8);
                    }

                    F<uint32_t>(this, 0x74) = cameraLocal.words[0];
                    F<uint32_t>(this, 0x78) = cameraLocal.words[1];
                    F<uint32_t>(this, 0x84) = cameraLocal.words[4];
                    F<uint32_t>(this, 0x88) = cameraLocal.words[5];
                    F<uint32_t>(this, 0x94) = cameraLocal.words[10];
                    F<uint32_t>(this, 0x98) = cameraLocal.words[11];
                    F<float>(this, 0x7c) = -muzzle.x;
                    F<float>(this, 0x8c) = -muzzle.y;
                    F<float>(this, 0x9c) = -muzzle.z;

                    int scale = F<int *>(gun, 0x3c)[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (F<uint8_t>(gun, 0x4d) != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                    }
                } else if (F<uint8_t>(this, 0x24) == 0) {
                    Vector *base;
                    if (F<Level *>(this, 0xc) == 0 || Level_getPlayer(F<Level *>(this, 0xc)) == 0 ||
                        Gun_isPlayerGun(gun) == 0) {
                        up.x = 0.0f;
                        up.y = 1.0f;
                        up.z = 0.0f;
                        base = &up;
                    } else {
                        base = (Vector *)(F<char *>(gun, 0x24) + offset);
                    }
                    Vector_assign((Vector *)((char *)this + 0x5c), base);
                    VectorCross(&side, (Vector *)((char *)this + 0x5c), (Vector *)((char *)this + 0x50));
                    VectorNormalize((Vector *)((char *)this + 0x68), &side);
                    VectorCross(&side, (Vector *)((char *)this + 0x68), (Vector *)((char *)this + 0x50));
                    VectorNormalize((Vector *)((char *)this + 0x5c), &side);

                    F<uint32_t>(this, 0x74) = F<uint32_t>(this, 0x68);
                    F<uint32_t>(this, 0x78) = F<uint32_t>(this, 0x5c);
                    F<uint32_t>(this, 0x7c) = F<uint32_t>(this, 0x50);
                    F<uint32_t>(this, 0x84) = F<uint32_t>(this, 0x6c);
                    F<uint32_t>(this, 0x88) = F<uint32_t>(this, 0x60);
                    F<uint32_t>(this, 0x8c) = F<uint32_t>(this, 0x54);
                    F<uint32_t>(this, 0x94) = F<uint32_t>(this, 0x70);
                    F<uint32_t>(this, 0x98) = F<uint32_t>(this, 0x64);
                    F<uint32_t>(this, 0x9c) = F<uint32_t>(this, 0x58);

                    int scale = F<int *>(gun, 0x3c)[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (F<uint8_t>(gun, 0x4d) != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                        typedef int (*DoneFn)(ObjectGun *);
                        if (((DoneFn *)F<void *>(this, 0x0))[8](this) == 0)
                            F<uint8_t>(gun, 0x4d) = 0;
                    }
                } else {
                    Player *player = Level_getPlayer(F<Level *>(this, 0xc));
                    identity(&local);
                    MatrixSetRotation(&scaleMatrix, F<float>(player, 0x7c), F<float>(player, 0x80), 0.0f);
                    Matrix_multiply_inplace((Matrix *)((char *)player + 0x40), &local);
                    Matrix_copy((Matrix *)((char *)this + 0x74), (Matrix *)((char *)player + 0x40));
                    MatrixSetTranslation(&scaleMatrix, position->x, position->y, position->z);
                    MatrixGetDir(&dir, &scaleMatrix);
                    VectorNormalize(&muzzle, &dir);
                    Vector_scale(&muzzle, F<float>(gun, 0x50));
                    Vector_assign((Vector *)(F<char *>(gun, 0x18) + offset), &muzzle);
                    F<uint32_t>(player, 0x7c) = 0;
                    F<uint32_t>(player, 0x80) = 0;
                    MatrixSetRotation(&scaleMatrix, F<float>(this, 0x20), 0.0f, 0.0f);
                    TransformSetLocal(*(void **)g_PaintCanvas, F<uint32_t>(this, 0x14), &scaleMatrix);
                }

                if (*(uint8_t *)g_ObjectGunRenderScaleFlag != 0)
                    MatrixSetScaling(&local, F<float>(this, 0x3c), F<float>(this, 0x40),
                                     F<float>(this, 0x44));

                if (F<int>(F<Gun *>(this, 0x8), 0x5c) == 0xb) {
                    Vector *spin = F<Vector **>(F<Array *>(F<Gun *>(this, 0x8), 0xac), 0x4)[i];
                    if (spin != 0 && F<int>(this, 0x34) > 0) {
                        MatrixSetRotation(&local, spin->x, spin->y, spin->z);
                        float step = (float)F<int>(this, 0x34) * 0.02f;
                        float neg = -step;
                        spin->x += spin->x < 0.0f ? neg : step;
                        spin->y += spin->y < 0.0f ? neg : step;
                        spin->z += spin->z < 0.0f ? neg : step;
                    }
                    MatrixSetScaling(&local, F<float>(this, 0x3c), F<float>(this, 0x40),
                                     F<float>(this, 0x44));
                }

                void **canvas = (void **)g_PaintCanvas;
                TransformSetLocal(*canvas, F<uint32_t>(this, 0x10), (Matrix *)((char *)this + 0x74));
                DrawTransform(*canvas, F<uint32_t>(this, 0x10), 0);
            } else {
                ++inactive;
            }
            offset += 0xc;
            gun = F<Gun *>(this, 0x8);
        }
        if (F<int>(gun, 0x14) <= (int)inactive)
            F<uint8_t>(gun, 0x4c) = 0;
    }

    if (F<uint8_t>(this, 0x1d) != 0 && F<AEGeometry *>(this, 0x18) != 0)
        AEGeometry_render(F<AEGeometry *>(this, 0x18));
    F<uint32_t>(this, 0x34) = 0;
}
