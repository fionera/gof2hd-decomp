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
