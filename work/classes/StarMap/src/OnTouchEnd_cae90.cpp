#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_sound;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_text;

extern "C" int ChoiceWindow_OnTouchEnd(void *window, int x, int y);
extern "C" int Layout_OnTouchEnd(void *layout, int x, int y);
extern "C" int TouchButton_OnTouchEnd(void *button, int x, int y);
extern "C" int Layout_helpPressed(void *layout);
extern "C" void Layout_initHelpWindow(void *layout, String *text);
extern "C" void PaintCanvas_CameraSetCurrent(void *canvas, uint32_t camera);
extern "C" void FModSound_stop(void *sound, int id);
extern "C" void FModSound_play(void *sound, int id, void *pos, float volume);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void ChoiceWindow_set(void *window, String *text, bool yesNo);
extern "C" void *ChoiceWindow_ctor(void *window);
extern "C" void String_ctor_copy(String *self, String *src, bool copy);
extern "C" void String_dtor(String *self);
extern "C" int Station_getIndex(void *station);
extern "C" void *Status_getStation(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" void *Status_getShip(void *status);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int SolarSystem_systemIsInSystemRoutes(void *system, int index);
extern "C" int Ship_hasVolatileGoods(void *ship);
extern "C" void AEGeometry_getPosition(Vector *out, void *geometry);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void MatrixGetPosition(Vector *out, void *matrix);
extern "C" void EaseInOut_SetRange(void *ease, float from, float to);

static inline float absf_end(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::OnTouchEnd(int x, int y)
{
    String help;

    if (field<uint8_t>(this, 0xa9) != 0) {
        int result = ChoiceWindow_OnTouchEnd(ptr_field(this, 0x5c), x, y);
        if (result == 1) {
            field<uint8_t>(this, 0xa9) = 0;
            field<uint8_t>(this, 0x120) = 0;
            return;
        }
        if (result != 0) {
            return;
        }
        field<uint8_t>(this, 0xa9) = 0;
        if (field<uint8_t>(this, 0x1e4) != 0) {
            field<uint8_t>(this, 0x1e4) = 0;
            return;
        }
        if (field<int32_t>(this, 4) == 3) {
            void *station = ((Array<void *> *)ptr_field(this, 0x58))->data[field<int32_t>(this, 0x64)];
            if (Station_getIndex(station) == Station_getIndex(Status_getStation(*g_StarMap_end_status))) {
                field<uint8_t>(this, 0x1e4) = 0;
                return;
            }
        }
        if (field<uint8_t>(this, 0x120) != 0 && field<int32_t>(this, 4) == 0 &&
            field<uint8_t>(this, 0xa8) == 0 && field<uint8_t>(this, 0x118) == 0) {
            field<uint8_t>(this, 0x120) = 0;
            field<uint8_t>(this, 0) = 1;
            PaintCanvas_CameraSetCurrent(*g_StarMap_end_canvas, field<uint32_t>(this, 0x74));
            return;
        }
        if (field<uint8_t>(this, 0xa8) == 0 && field<int32_t>(this, 4) == 3) {
            if (field<uint8_t>(this, 0xab) == 0 ||
                field<int32_t>(this, 0x1d0) <= field<int32_t>(this, 0x1d8)) {
                if (field<uint8_t>(this, 0xaa) == 0) {
                    depart(true);
                    return;
                }
            } else if (field<int32_t>(this, 0x1d0) == 1 && field<uint8_t>(this, 0xaa) == 0) {
                depart(false);
                return;
            }
            field<uint8_t>(this, 0) = 1;
            PaintCanvas_CameraSetCurrent(*g_StarMap_end_canvas, field<uint32_t>(this, 0x74));
        }
        return;
    }

    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        return;
    }
    void *layout = *g_StarMap_end_layout;
    if (*(uint8_t *)layout == 0 && Layout_OnTouchEnd(layout, x, y) != 0) {
        if (field<int32_t>(this, 4) == 3 && field<uint8_t>(this, 0xf4) != 0) {
            field<uint8_t>(this, 0x139) = 1;
            field<int32_t>(this, 0x168) = 0;
            field<int32_t>(this, 0x16c) = 0;
            field<int32_t>(this, 0x170) = 0;
            EaseInOut_SetRange(ptr_field(this, 0x17c), field<float>(this, 0x78), field<float>(this, 0x78));
            EaseInOut_SetRange(ptr_field(this, 0x180), field<float>(this, 0x7c), field<float>(this, 0x7c));
            EaseInOut_SetRange(ptr_field(this, 0x184), field<float>(this, 0x80), field<float>(this, 0x80));
            FModSound_play(*g_StarMap_end_sound, 0x6b, 0, 0.0f);
        } else {
            PaintCanvas_CameraSetCurrent(*g_StarMap_end_canvas, field<uint32_t>(this, 0x74));
            FModSound_stop(*g_StarMap_end_sound, 0x66);
        }
        return;
    }
    if (field<uint8_t>(this, 0xa8) != 0 && field<uint8_t>(this, 0x13a) != 0) {
        return;
    }
    if (TouchButton_OnTouchEnd(ptr_field(this, 0x4c), x, y) != 0) {
        field<uint8_t>(this, 0x108) ^= 1;
    }
    if (field<uint8_t>(this, 0x174) != 0) {
        float dx = field<float>(this, 0x150);
        float dy = field<float>(this, 0x154);
        field<int32_t>(this, 0x168) = 0x3f666666;
        field<uint8_t>(this, 0x174) = 0;
        field<float>(this, 0x16c) = absf_end(dx) > 3.0f ? dx : 0.0f;
        field<float>(this, 0x170) = absf_end(dy) > 3.0f ? dy : 0.0f;
        if (field<int32_t>(this, 4) == 0) {
            if (field<int32_t>(this, 0x60) >= 0 &&
                field<uint8_t>(this, 0x118) == 0 &&
                field<uint8_t>(this, 0xa8) == 0 &&
                field<int32_t>(this, 0x19c) == field<int32_t>(this, 0x60)) {
                if (field<uint8_t>(this, 0xab) == 0 &&
                    SolarSystem_systemIsInSystemRoutes(Status_getSystem(*g_StarMap_end_status),
                                                       SolarSystem_getIndex(Status_getSystem(*g_StarMap_end_status))) == 0) {
                    ChoiceWindow_set(ptr_field(this, 0x5c), GameText_getText(*g_StarMap_end_text, 0x1a4), false);
                    field<uint8_t>(this, 0xa9) = 1;
                    return;
                }
                FModSound_play(*g_StarMap_end_sound, 0x6a, 0, 0.0f);
                initStarSystem();
                field<uint8_t>(this, 0x138) = 1;
                Vector p;
                AEGeometry_getPosition(&p, ptr_field(this, 0xa4));
                Vector_assign((Vector *)((char *)this + 0x78), &p);
                field<float>(this, 0x80) += 20.0f;
                MatrixGetPosition(&p, PaintCanvas_CameraGetLocal(*g_StarMap_end_canvas));
                Vector_assign((Vector *)((char *)this + 0x84), &p);
                EaseInOut_SetRange(ptr_field(this, 0x17c), field<float>(this, 0x84), field<float>(this, 0x78));
                EaseInOut_SetRange(ptr_field(this, 0x180), field<float>(this, 0x88), field<float>(this, 0x7c));
                EaseInOut_SetRange(ptr_field(this, 0x184), field<float>(this, 0x8c), field<float>(this, 0x80));
                field<int32_t>(this, 0x168) = 0;
                field<int32_t>(this, 0x16c) = 0;
                field<int32_t>(this, 0x170) = 0;
            } else if (field<int32_t>(this, 0x60) >= 0) {
                field<int32_t>(this, 0x19c) = -1;
                field<uint8_t>(this, 0x13a) = 1;
            }
        } else if (field<int32_t>(this, 0x64) >= 0) {
            if (field<int32_t>(this, 0x1a0) == field<int32_t>(this, 0x64)) {
                if (ptr_field(this, 0x5c) == 0) {
                    ptr_field(this, 0x5c) = ChoiceWindow_ctor(operator new(0x5c));
                }
                ChoiceWindow_set(ptr_field(this, 0x5c), GameText_getText(*g_StarMap_end_text, 0x1a3), true);
                field<uint8_t>(this, 0xa9) = 1;
            } else {
                FModSound_play(*g_StarMap_end_sound, 0x69, 0, 0.0f);
                field<uint8_t>(this, 0x13b) = 1;
            }
        }
    }
    if (Layout_helpPressed(layout) != 0) {
        String_ctor_copy(&help, GameText_getText(*g_StarMap_end_text, 0x1a5), false);
        Layout_initHelpWindow(layout, &help);
        String_dtor(&help);
    }
}
