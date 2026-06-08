#include "class.h"

__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" MeshId g_ObjectGunGeometryIds[];
__attribute__((visibility("hidden"))) extern "C" uint32_t (*g_TransformGetObject)(void *canvas, uint32_t mesh);
__attribute__((visibility("hidden"))) extern "C" void (*g_TransformSetState)(uint32_t object, int state, int value);

extern "C" uint32_t TransformGetTransform(void *canvas, uint32_t transform);
extern "C" void Transform_Update(uint64_t transform, int64_t dt, int flags);
extern "C" void Gun_update(Gun *self, int dt);
extern "C" int Gun_isPlayerGun(Gun *self);
extern "C" Player *Player_getKIPlayer(Player *self);
extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
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
