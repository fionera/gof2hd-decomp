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
    return F<Explosion **>(self->f_2c, 0x4)[index];
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
    uint32_t transform = TransformGetTransform(*canvas, this->f_10);
    Transform_Update((uint64_t)transform, (int64_t)dt, 0);

    this->f_34 = dt;
    Gun *gun = this->f_8;
    Gun_update(gun, dt);

    if (this->f_1c == 0) {
        if (Gun_isPlayerGun(gun) == 0) {
            Player *owner = gun->f_4;
            if (owner != 0) {
                Player *ki = Player_getKIPlayer(owner);
                if (ki != 0 && F<uint8_t>(Player_getKIPlayer(owner), 0x3f) != 0) {
                    this->f_1c = 1;
                    AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
                    AEGeometry_ctor(geometry, g_ObjectGunGeometryIds[F<int>(this->f_8, 0x58)].id,
                                    *canvas, false);
                    this->f_18 = geometry;
                }
            }
        }
        if (this->f_1c == 0)
            goto after_geometry;
    }

    gun = this->f_8;
    if (F<uint8_t>(gun, 0xa9) == 0) {
        uint32_t object = g_TransformGetObject(*canvas, F<uint32_t>(this->f_18, 0xc));
        g_TransformSetState(object, 0, 0);
        object = g_TransformGetObject(*canvas, F<uint32_t>(this->f_18, 0xc));
        g_TransformSetState(object, 3, 0);
        object = g_TransformGetObject(*canvas, F<uint32_t>(this->f_18, 0xc));
        g_TransformSetState(object, 1, 0);
        goto after_geometry;
    }

    {
        Player *player = Level_getPlayer(this->f_c);
        if (Gun_isPlayerGun(gun) != 0) {
            PlayerEgo_getPosition(&position, player);
        } else {
            Player_getPosition(&position, gun->f_4);
        }

        gun = this->f_8;
        void *matrixSource = player;
        if (Gun_isPlayerGun(gun) == 0)
            matrixSource = (char *)gun + 0x4;
        __aeabi_memcpy(&playerMatrix, (char *)matrixSource->f_0 + 0x4, 0x3c);

        offsets.x = gun->f_7c;
        offsets.y = gun->f_80;
        offsets.z = gun->f_84 + 8.0f;
        MatrixRotateVector(&workMatrix, &playerMatrix, &offsets);
        Vector_add_assign(&position, (Vector *)&workMatrix);
        AEGeometry_setPosition(this->f_18, &position);

        transform = TransformGetTransform(*canvas, F<uint32_t>(this->f_18, 0xc));
        Transform_Update((uint64_t)transform, (int64_t)dt, 0);

        if (F<int>(this->f_8, 0x5c) != 8) {
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            Matrix_copy(&cameraMatrix, (Matrix *)CameraGetLocal(paint, camera));
            MatrixGetDir(&dir, &cameraMatrix);
            MatrixGetUp(&up, &cameraMatrix);
            AEGeometry_setDirection(this->f_18, &dir, &up);
            goto after_geometry;
        }

        Matrix *m0 = AEGeometry_getMatrix(this->f_18);
        Matrix *m1 = AEGeometry_getMatrix(this->f_18);
        Matrix_multiply(&workMatrix, m0);

        gun = this->f_8;
        int weapon = gun->f_58;
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

        if (gun->f_a4 != 0) {
            if (weapon == 0xb5) {
                offsets.x = F<uint8_t>(gun, 0xa6) != 0 ? -1.5f : 1.5f;
            } else {
                float left = weapon == 0x30 ? 20.0f : 15.0f;
                float right = weapon == 0x30 ? -20.0f : -15.0f;
                offsets.x = left - gun->f_7c;
                if (F<uint8_t>(gun, 0xa6) != 0)
                    offsets.x = gun->f_7c + right;
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
        AEGeometry_setMatrix(this->f_18, &workMatrix);
        AEGeometry_translate(this->f_18, (Vector *)&cameraMatrix);
        (void)m1;
    }

after_geometry:
    gun = this->f_8;
    F<uint8_t>(this, 0x1d) = F<uint8_t>(gun, 0xa9);
    if (gun->f_5c == 0x19) {
        int positionOffset = 0;
        for (uint32_t i = 0; i < gun->f_8; ++i) {
            if (gun->f_40[i] != 0) {
                if (this->f_30[i] != 0) {
                    Explosion_start(explosion_at(this, i),
                                    (Vector *)(gun->f_30 + positionOffset),
                                    &zero);
                    this->f_30[i] = 0;
                }
                Explosion *explosion = explosion_at(this, i);
                Explosion_update(explosion, dt, 0);
                if (Explosion_isPlaying(explosion) == 0) {
                    gun = this->f_8;
                    gun->f_40[i] = 0;
                    gun->f_88 = 0;
                    this->f_30[i] = 1;
                    Explosion_reset(explosion_at(this, i));
                }
            }
            positionOffset += 0xc;
            gun = this->f_8;
        }
    }
}
