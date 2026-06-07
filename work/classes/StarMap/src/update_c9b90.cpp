#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_sound;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_update_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_update_screenH;

extern "C" void AEGeometry_getPosition(Vector *out, void *geometry);
extern "C" void *AEGeometry_getMatrix(void *geometry);
extern "C" void AEGeometry_setScaling(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setPosition(void *geometry, Vector *pos);
extern "C" void AEGeometry_setDirection(void *geometry, Vector *dir);
extern "C" void AEGeometry_setRotation(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_rotate(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setVisible(void *geometry, bool visible);
extern "C" void *AEGeometry_dtor(void *geometry);
extern "C" int PaintCanvas_GetScreenPosition(void *canvas, Vector *world, Vector *screen);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas);
extern "C" void Transform_Update(void *transform, int dt);
extern "C" void MatrixGetPosition(Vector *out, void *matrix);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void Vector_sub_assign(Vector *dst, Vector *src);
extern "C" void VectorNormalize(Vector *out, Vector *value);
extern "C" float EaseInOut_GetValue(void *ease);
extern "C" float EaseInOut_GetMinValue(void *ease);
extern "C" float EaseInOut_GetMaxValue(void *ease);
extern "C" void EaseInOut_Update(void *ease, float dt);
extern "C" float EaseInOut_GetCurrentValue(void *ease);
extern "C" void FModSound_setParamValue(void *sound, int event, int id, float value);
extern "C" void ArrayReleaseClasses_AEGeometry(void *arr);
extern "C" void *Array_AEGeometry_dtor(void *arr);
extern "C" void ArrayRelease_bool(void *arr);
extern "C" void *Array_bool_dtor(void *arr);
extern "C" void *Status_getSystem(void *status);
extern "C" uint8_t *Status_getSystemVisibilities(void *status);

static inline float absf_update(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::update(int dt)
{
    Matrix matrix;
    Vector tmp;

    field<int32_t>(this, 0x18) = dt;
    if (field<int32_t>(this, 4) == 0 || field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x194);
        for (uint32_t i = 0; i < positions->length; i++) {
            AEGeometry_getPosition(&tmp, ((Array<void *> *)ptr_field(this, 0x68))->data[i]);
            int visible = PaintCanvas_GetScreenPosition(*g_StarMap_update_canvas, &tmp, positions->data[i]);
            positions->data[i]->z = visible != 0 ? 1.0f : -1.0f;
        }
    }
    if (field<int32_t>(this, 4) == 3 || field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x198);
        if (positions != 0) {
            for (uint32_t i = 0; i < positions->length; i++) {
                AEGeometry_getPosition(&tmp, ((Array<void *> *)ptr_field(this, 0x90))->data[i + 1]);
                int visible = PaintCanvas_GetScreenPosition(*g_StarMap_update_canvas, &tmp, positions->data[i]);
                positions->data[i]->z = visible != 0 ? 1.0f : -1.0f;
            }
        }
    }

    if (ptr_field(this, 0xf8) != 0) {
        float v = EaseInOut_GetValue(ptr_field(this, 0x184));
        float min = EaseInOut_GetMinValue(ptr_field(this, 0x184));
        float max = EaseInOut_GetMaxValue(ptr_field(this, 0x184));
        float t = (v - min) / (max - min);
        if (field<uint8_t>(this, 0x139) != 0) {
            t = 1.0f - t;
        } else if (field<uint8_t>(this, 0x138) == 0 && field<int32_t>(this, 4) == 0) {
            t = 1.0f;
        }
        float scale = (float)(0.5 + (double)t * 0.5);
        AEGeometry_setScaling(ptr_field(this, 0xf8), scale, scale, scale);
        if (ptr_field(this, 0xa4) != 0 && field<int32_t>(this, 0x1c4) >= 0) {
            AEGeometry_getPosition(&tmp,
                                   ((Array<void *> *)ptr_field(this, 0x68))->data[field<int32_t>(*g_StarMap_update_status, 0x7c)]);
            AEGeometry_setPosition(ptr_field(this, 0xf8), &tmp);
        }
        void *canvas = *g_StarMap_update_canvas;
        PaintCanvas_CameraGetCurrent(canvas);
        MatrixGetPosition(&tmp, PaintCanvas_CameraGetLocal(canvas));
        Vector_assign((Vector *)((char *)this + 0x78), &tmp);
        AEGeometry_getPosition(&tmp, ptr_field(this, 0xf8));
        Vector_sub_assign((Vector *)((char *)this + 0x78), &tmp);
        VectorNormalize(&tmp, (Vector *)((char *)this + 0x78));
        Vector_assign((Vector *)((char *)this + 0x78), &tmp);
        field<float>(this, 0x78) += 0.5f;
        AEGeometry_setDirection(ptr_field(this, 0xf8), (Vector *)((char *)this + 0x78));
        Transform_Update(PaintCanvas_TransformGetTransform(canvas), dt);
    }

    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        float step = (float)(dt * 15);
        EaseInOut_Update(ptr_field(this, 0x17c), step);
        EaseInOut_Update(ptr_field(this, 0x180), step);
        EaseInOut_Update(ptr_field(this, 0x184), step);
        tmp.x = EaseInOut_GetCurrentValue(ptr_field(this, 0x17c));
        tmp.y = EaseInOut_GetCurrentValue(ptr_field(this, 0x180));
        tmp.z = EaseInOut_GetCurrentValue(ptr_field(this, 0x184));
        Vector_assign((Vector *)((char *)this + 0x78), &tmp);
        void *canvas = *g_StarMap_update_canvas;
        __builtin_memcpy(&matrix, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        MatrixSetTranslation(&matrix, field<float>(this, 0x78), field<float>(this, 0x7c), field<float>(this, 0x80));
        PaintCanvas_CameraSetLocal(canvas, &matrix);
        if (absf_update(field<float>(this, 0x78) - EaseInOut_GetMaxValue(ptr_field(this, 0x17c))) <= 1.0f &&
            absf_update(field<float>(this, 0x7c) - EaseInOut_GetMaxValue(ptr_field(this, 0x180))) <= 1.0f &&
            absf_update(field<float>(this, 0x80) - EaseInOut_GetMaxValue(ptr_field(this, 0x184))) <= 1.0f) {
            if (field<uint8_t>(this, 0x138) == 0) {
                if (ptr_field(this, 0x90) != 0) {
                    ArrayReleaseClasses_AEGeometry(ptr_field(this, 0x90));
                    operator delete(Array_AEGeometry_dtor(ptr_field(this, 0x90)));
                }
                ptr_field(this, 0x90) = 0;
                if (ptr_field(this, 0x94) != 0) {
                    ArrayReleaseClasses_AEGeometry(ptr_field(this, 0x94));
                    operator delete(Array_AEGeometry_dtor(ptr_field(this, 0x94)));
                }
                ptr_field(this, 0x94) = 0;
                if (ptr_field(this, 0x98) != 0) {
                    operator delete(ptr_field(this, 0x98));
                }
                ptr_field(this, 0x98) = 0;
                if (ptr_field(this, 0x9c) != 0) {
                    operator delete(ptr_field(this, 0x9c));
                }
                ptr_field(this, 0x9c) = 0;
                if (ptr_field(this, 0x100) != 0) {
                    ArrayRelease_bool(ptr_field(this, 0x100));
                    operator delete(Array_bool_dtor(ptr_field(this, 0x100)));
                }
                ptr_field(this, 0x100) = 0;
                if (ptr_field(this, 0xa4) != 0) {
                    operator delete(AEGeometry_dtor(ptr_field(this, 0xa4)));
                }
                field<int32_t>(this, 4) = 0;
                ptr_field(this, 0xa4) = 0;
                AEGeometry_setVisible(((Array<void *> *)ptr_field(this, 0x68))->data[field<int32_t>(this, 0x60)], true);
            } else {
                field<int32_t>(this, 4) = 3;
            }
            field<uint16_t>(this, 0x138) = 0;
        }
        return;
    }

    FModSound_setParamValue(*g_StarMap_update_sound, 0, 0x66, 0.0f);
    if (field<int32_t>(this, 4) == 3) {
        if (field<uint8_t>(this, 0x13b) == 0 && field<uint8_t>(this, 0x174) == 0) {
            float vx = field<float>(this, 0x168) * field<float>(this, 0x16c);
            float vy = field<float>(this, 0x168) * field<float>(this, 0x170);
            field<float>(this, 0x16c) = vx;
            field<float>(this, 0x170) = vy;
            if (absf_update(vx) > 0.5f) {
                field<float>(this, 0x188) += vx;
            }
            if (absf_update(vy) > 0.5f) {
                field<float>(this, 0x18c) += vy;
            }
        } else if (field<uint8_t>(this, 0x13b) != 0 && field<int32_t>(this, 0x64) >= 0) {
            int target = 0x8000 - ((int *)ptr_field(this, 0x98))[field<int32_t>(this, 0x64)];
            int diff = (int)field<float>(this, 0x188) - target;
            field<float>(this, 0x188) += (float)(diff < 0 ? -diff : diff) * (diff < 0 ? 0.25f : -0.25f);
            if (absf_update((float)diff) < 11.0f) {
                field<uint8_t>(this, 0x13b) = 0;
            }
        }
        float speed = absf_update(field<float>(this, 0x16c) + field<float>(this, 0x170));
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        field<float>(this, 0x1c0) += speed;
        if (ptr_field(this, 0x90) != 0) {
            Array<void *> *geoms = (Array<void *> *)ptr_field(this, 0x90);
            for (uint32_t i = 0; i < geoms->length; i++) {
                if (geoms->data[i] != 0) {
                    AEGeometry_rotate(geoms->data[i], (float)dt * 0.001f, 0.0f, 0.001f);
                }
            }
        }
        field<float>(this, 0x188) = (float)((int)field<float>(this, 0x188) & 0xffff);
        if (field<float>(this, 0x18c) < -90.0f) {
            field<float>(this, 0x18c) = -90.0f;
        }
        if (field<float>(this, 0x18c) > 90.0f) {
            field<float>(this, 0x18c) = 90.0f;
        }
        AEGeometry_setRotation(ptr_field(this, 0xa4), field<float>(this, 0x18c), 0.0f, field<float>(this, 0x188));
        return;
    }

    if (field<int32_t>(this, 4) == 0) {
        if (field<int32_t>(this, 0x114) >= 0 && field<uint8_t>(this, 0x118) != 0 &&
            field<int32_t>(this, 0x11c) < 4000) {
            field<int32_t>(this, 0x11c) += dt;
            if (field<int32_t>(this, 0x11c) > 3999) {
                OnTouchBegin(*g_StarMap_update_screenW >> 1, *g_StarMap_update_screenH >> 1);
            }
            float scale = ((float)field<int32_t>(this, 0x11c) / 4000.0f) * 1.5f;
            AEGeometry_setScaling(((Array<void *> *)ptr_field(this, 0x68))->data[field<int32_t>(this, 0x114)],
                                  scale, scale, scale);
        }
        if (field<uint8_t>(this, 0x174) == 0) {
            float vx = field<float>(this, 0x168) * field<float>(this, 0x16c);
            float vy = field<float>(this, 0x168) * field<float>(this, 0x170);
            field<float>(this, 0x16c) = vx;
            field<float>(this, 0x170) = vy;
            if (absf_update(vx) > 0.5f) {
                field<int32_t>(this, 0x13c) = (int)((float)field<int32_t>(this, 0x13c) + vx);
            }
            if (absf_update(vy) > 0.5f) {
                field<int32_t>(this, 0x140) = (int)((float)field<int32_t>(this, 0x140) + vy);
            }
        }
        void *canvas = *g_StarMap_update_canvas;
        __builtin_memcpy(&matrix, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        MatrixSetTranslation(&matrix,
                             (field<float>(this, 8) + (float)field<int32_t>(this, 0x13c)) * 20.0f,
                             0.0f,
                             (field<float>(this, 0xc) + (float)field<int32_t>(this, 0x140)) * 20.0f);
        PaintCanvas_CameraSetLocal(canvas, &matrix);
        if (field<uint8_t>(this, 0x13a) != 0) {
            Array<uint8_t> *vis = (Array<uint8_t> *)Status_getSystemVisibilities(*g_StarMap_update_status);
            uint32_t selected = field<uint32_t>(this, 0x60);
            if (vis != 0 && selected < vis->length && vis->data[selected] != 0) {
                Vector_assign((Vector *)((char *)this + 0x78),
                              ((Array<Vector *> *)ptr_field(this, 0x194))->data[selected]);
                float targetX = (float)(*g_StarMap_update_screenW >> 1);
                float targetY = (float)(*g_StarMap_update_screenH >> 1);
                float sx = (field<float>(this, 0x78) - targetX) / -30.0f;
                float sy = (field<float>(this, 0x7c) - targetY) / -30.0f;
                field<float>(this, 0x16c) = sx;
                field<float>(this, 0x170) = sy;
                if (absf_update(sx) <= 2.0f && absf_update(sy) <= 2.0f) {
                    field<uint8_t>(this, 0x13a) = 0;
                    field<int32_t>(this, 0x19c) = field<int32_t>(this, 0x60);
                }
            }
        }
    }
}
