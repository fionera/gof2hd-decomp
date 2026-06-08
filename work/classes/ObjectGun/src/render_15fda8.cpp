#include "class.h"

__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;
__attribute__((visibility("hidden"))) extern "C" void *g_ObjectGunRenderScaleFlag;

extern "C" void Gun_render(Gun *self);
extern "C" void Explosion_render(Explosion *self);
extern "C" void *CameraGetCurrent(void *canvas);
extern "C" void *CameraGetLocal(void *canvas, void *camera);
extern "C" void Matrix_copy(Matrix *dst, const Matrix *src);
extern "C" void MatrixSetRotation(Matrix *dst, float x, float y, float z);
extern "C" void Matrix_multiply_inplace(Matrix *self, const Matrix *rhs);
extern "C" void MatrixSetTranslation(Matrix *dst, float x, float y, float z);
extern "C" void VectorNormalize(Vector *out, const Vector *in);
extern "C" void Vector_assign(Vector *dst, const Vector *src);
extern "C" void Vector_negate(Vector *out, const Vector *in);
extern "C" void MatrixGetDir(Vector *out, const Matrix *matrix);
extern "C" void Vector_scale(Vector *self, float scale);
extern "C" void MatrixGetTranslation(Vector *out, const Matrix *matrix);
extern "C" void Vector_add_scaled(Vector *out, const Vector *dir, float scale);
extern "C" void VectorCross(Vector *out, const Vector *a, const Vector *b);
extern "C" void MatrixSetScaling(Matrix *dst, float x, float y, float z);
extern "C" void TransformSetLocal(void *canvas, uint32_t transform, Matrix *matrix);
extern "C" void DrawTransform(void *canvas, uint32_t transform, int flags);
extern "C" void AEGeometry_render(AEGeometry *self);
extern "C" Player *Level_getPlayer(Level *self);
extern "C" int Gun_isPlayerGun(Gun *self);

static inline Explosion *render_explosion_at(ObjectGun *self, uint32_t index)
{
    return F<Explosion **>(self->f_2c, 0x4)[index];
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

    Gun *gun = this->f_8;
    Gun_render(gun);

    if (gun->f_5c == 0x19) {
        for (uint32_t i = 0; i < gun->f_8; ++i) {
            if (gun->f_40[i] != 0) {
                Explosion_render(render_explosion_at(this, i));
                gun = this->f_8;
            }
        }
    }

    if (gun->f_4c != 0) {
        identity(&cameraLocal);
        if (gun->f_5c == 8) {
            void **canvas = (void **)g_PaintCanvas;
            void *paint = *canvas;
            void *camera = CameraGetCurrent(paint);
            Matrix_copy(&cameraLocal, (Matrix *)CameraGetLocal(paint, camera));
            if (this->f_4c != 0) {
                identity(&rotate);
                MatrixSetRotation(&scaleMatrix, this->f_48, 0.0f, 0.0f);
                Matrix_copy(&rotate, &scaleMatrix);
                Matrix_multiply_inplace(&cameraLocal, &rotate);
            }
        }

        uint32_t inactive = 0;
        int offset = 0;
        for (uint32_t i = 0; i < gun->f_8; ++i) {
            Vector *position = (Vector *)(gun->f_c + offset);
            if (position->x != -1000.0f) {
                MatrixSetTranslation(&local, position->x, position->y, position->z);
                VectorNormalize((Vector *)&local, (Vector *)(gun->f_18 + offset));
                Vector_assign((Vector *)((char *)this + 0x50), (Vector *)&local);

                if (this->f_4c != 0) {
                    Vector_negate(&muzzle, (Vector *)((char *)this + 0x50));
                    if ((uint32_t)(gun->f_58 - 0xb4) > 2) {
                        void **canvas = (void **)g_PaintCanvas;
                        void *paint = *canvas;
                        void *camera = CameraGetCurrent(paint);
                        Matrix_copy(&cameraLocal, (Matrix *)CameraGetLocal(paint, camera));
                        MatrixGetDir(&dir, &cameraLocal);
                        Vector_assign(&muzzle, &dir);
                        gun = this->f_8;
                    }

                    this->f_74 = cameraLocal.words[0];
                    this->f_78 = cameraLocal.words[1];
                    this->f_84 = cameraLocal.words[4];
                    this->f_88 = cameraLocal.words[5];
                    this->f_94 = cameraLocal.words[10];
                    this->f_98 = cameraLocal.words[11];
                    this->f_7c = -muzzle.x;
                    this->f_8c = -muzzle.y;
                    this->f_9c = -muzzle.z;

                    int scale = gun->f_3c[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (F<uint8_t>(gun, 0x4d) != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                    }
                } else if (this->f_24 == 0) {
                    Vector *base;
                    if (this->f_c == 0 || Level_getPlayer(this->f_c) == 0 ||
                        Gun_isPlayerGun(gun) == 0) {
                        up.x = 0.0f;
                        up.y = 1.0f;
                        up.z = 0.0f;
                        base = &up;
                    } else {
                        base = (Vector *)(gun->f_24 + offset);
                    }
                    Vector_assign((Vector *)((char *)this + 0x5c), base);
                    VectorCross(&side, (Vector *)((char *)this + 0x5c), (Vector *)((char *)this + 0x50));
                    VectorNormalize((Vector *)((char *)this + 0x68), &side);
                    VectorCross(&side, (Vector *)((char *)this + 0x68), (Vector *)((char *)this + 0x50));
                    VectorNormalize((Vector *)((char *)this + 0x5c), &side);

                    this->f_74 = this->f_68;
                    this->f_78 = this->f_5c;
                    this->f_7c = this->f_50;
                    this->f_84 = this->f_6c;
                    this->f_88 = this->f_60;
                    this->f_8c = this->f_54;
                    this->f_94 = this->f_70;
                    this->f_98 = this->f_64;
                    this->f_9c = this->f_58;

                    int scale = gun->f_3c[i];
                    if (scale < 1000) {
                        float fscale = (float)scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (F<uint8_t>(gun, 0x4d) != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                        typedef int (*DoneFn)(ObjectGun *);
                        if (((DoneFn *)this->f_0)[8](this) == 0)
                            F<uint8_t>(gun, 0x4d) = 0;
                    }
                } else {
                    Player *player = Level_getPlayer(this->f_c);
                    identity(&local);
                    MatrixSetRotation(&scaleMatrix, player->f_7c, player->f_80, 0.0f);
                    Matrix_multiply_inplace((Matrix *)((char *)player + 0x40), &local);
                    Matrix_copy((Matrix *)((char *)this + 0x74), (Matrix *)((char *)player + 0x40));
                    MatrixSetTranslation(&scaleMatrix, position->x, position->y, position->z);
                    MatrixGetDir(&dir, &scaleMatrix);
                    VectorNormalize(&muzzle, &dir);
                    Vector_scale(&muzzle, gun->f_50);
                    Vector_assign((Vector *)(gun->f_18 + offset), &muzzle);
                    player->f_7c = 0;
                    player->f_80 = 0;
                    MatrixSetRotation(&scaleMatrix, this->f_20, 0.0f, 0.0f);
                    TransformSetLocal(*(void **)g_PaintCanvas, this->f_14, &scaleMatrix);
                }

                if (*(uint8_t *)g_ObjectGunRenderScaleFlag != 0)
                    MatrixSetScaling(&local, this->f_3c, this->f_40,
                                     this->f_44);

                if (F<int>(this->f_8, 0x5c) == 0xb) {
                    Vector *spin = F<Vector **>(F<Array *>(this->f_8, 0xac), 0x4)[i];
                    if (spin != 0 && this->f_34 > 0) {
                        MatrixSetRotation(&local, spin->x, spin->y, spin->z);
                        float step = (float)this->f_34 * 0.02f;
                        float neg = -step;
                        spin->x += spin->x < 0.0f ? neg : step;
                        spin->y += spin->y < 0.0f ? neg : step;
                        spin->z += spin->z < 0.0f ? neg : step;
                    }
                    MatrixSetScaling(&local, this->f_3c, this->f_40,
                                     this->f_44);
                }

                void **canvas = (void **)g_PaintCanvas;
                TransformSetLocal(*canvas, this->f_10, (Matrix *)((char *)this + 0x74));
                DrawTransform(*canvas, this->f_10, 0);
            } else {
                ++inactive;
            }
            offset += 0xc;
            gun = this->f_8;
        }
        if (gun->f_14 <= (int)inactive)
            gun->f_4c = 0;
    }

    if (F<uint8_t>(this, 0x1d) != 0 && this->f_18 != 0)
        AEGeometry_render(this->f_18);
    this->f_34 = 0;
}
