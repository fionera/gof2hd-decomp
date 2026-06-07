#include "class.h"

extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_draw_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_text;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_font;

extern "C" float EaseInOut_GetValue(void *ease);
extern "C" float EaseInOut_GetMinValue(void *ease);
extern "C" float EaseInOut_GetMaxValue(void *ease);
extern "C" void PaintCanvas_SetColor(uint32_t canvas, uint32_t color);
extern "C" void PaintCanvas_SetColorRGBA(uint32_t canvas, int r, int g, int b, int a);
extern "C" void PaintCanvas_DrawLine(uint32_t canvas, int x0, int y0, int x1, int y1);
extern "C" void PaintCanvas_DrawImage2D(uint32_t canvas, uint32_t image, int x, int y, int anchor);
extern "C" void PaintCanvas_DrawString(uint32_t canvas, void *font, String *text, int x, int y, bool flag);
extern "C" int PaintCanvas_GetImage2DWidth(uint32_t canvas, uint32_t image);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void Layout_drawHeader(void *layout, String *text);
extern "C" void Layout_drawEmptyFooter(void *layout);
extern "C" void TouchButton_draw(void *button);
extern "C" void ChoiceWindow_draw(void *choice);
extern "C" void String_ctor_copy(String *self, String *src, bool copy);
extern "C" void String_ctor_int(String *self, int value);
extern "C" void String_ctor_char(String *self, const char *value, bool copy);
extern "C" void String_dtor(String *self);
extern "C" void String_add(String *out, String *a, String *b);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void *Status_getSystem(void *status);
extern "C" long long Status_getPlayingTime(void *status);
extern "C" void *Status_getShip(void *status);
extern "C" int Ship_hasJumpDriveIntegrated(void *ship);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" void *SolarSystem_getRoutes(void *system);
extern "C" int SolarSystem_hasNoOwner(void *system);
extern "C" void SolarSystem_getName(String *out, void *system);
extern "C" int SolarSystem_getRace(void *system);
extern "C" int SolarSystem_getSecurityLevel(void *system);

void StarMap::draw()
{
    String tmp;

    int mode = field<int32_t>(this, 4);
    int alpha = mode == 0 ? 0xff : 0;
    field<int32_t>(this, 0x1a4) = alpha;
    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        float v = EaseInOut_GetValue(ptr_field(this, 0x184));
        float min = EaseInOut_GetMinValue(ptr_field(this, 0x184));
        float max = EaseInOut_GetMaxValue(ptr_field(this, 0x184));
        float t = (v - min) / (max - min);
        if (field<uint8_t>(this, 0x138) != 0) {
            t = 1.0f - t;
        }
        field<int32_t>(this, 0x1a4) = (int)(t * 255.0f);
    }

    uint32_t canvas = *g_StarMap_draw_canvas;
    if (mode != 3 || field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
        Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x194);
        for (uint32_t i = 0; i < positions->length; i++) {
            Vector_assign((Vector *)((char *)this + 0x78), positions->data[i]);
            void *routes = SolarSystem_getRoutes(((Array<void *> *)ptr_field(this, 0x54))->data[i]);
            if (routes != 0) {
                Array<int> *r = (Array<int> *)routes;
                for (uint32_t j = 0; j < r->length; j++) {
                    uint32_t to = (uint32_t)r->data[j];
                    if (to < positions->length) {
                        Vector_assign((Vector *)((char *)this + 0x84), positions->data[to]);
                        if (field<float>(this, 0x80) >= 0.0f || field<float>(this, 0x8c) >= 0.0f) {
                            PaintCanvas_DrawLine(canvas, (int)field<float>(this, 0x78),
                                                 (int)field<float>(this, 0x7c),
                                                 (int)field<float>(this, 0x84),
                                                 (int)field<float>(this, 0x88));
                        }
                    }
                }
            }
        }
        for (uint32_t i = 0; i < ((Array<void *> *)ptr_field(this, 0x54))->length; i++) {
            drawOnScreenInfo((int)i, false);
        }
    }

    if (field<int32_t>(this, 0x60) >= 0 &&
        (field<uint8_t>(this, 0x118) == 0 || field<int32_t>(this, 0x11c) > 3999)) {
        drawOnScreenInfo(field<int32_t>(this, 0x60), false);
    }

    if (ptr_field(this, 0xa4) != 0 && ptr_field(this, 0x58) != 0) {
        void *system = ((Array<void *> *)ptr_field(this, 0x54))->data[field<int32_t>(this, 0x60)];
        if (SolarSystem_hasNoOwner(system) == 0) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4) ^ 0xff);
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x34),
                                    field<int32_t>(*g_StarMap_draw_layout, 0x2c),
                                    field<int32_t>(*g_StarMap_draw_layout, 0xc) +
                                        field<int32_t>(*g_StarMap_draw_layout, 0x2c),
                                    0);
            SolarSystem_getName(&tmp, system);
            PaintCanvas_DrawString(canvas, *g_StarMap_draw_font, &tmp,
                                   PaintCanvas_GetImage2DWidth(canvas, field<uint32_t>(this, 0x34)) +
                                       field<int32_t>(*g_StarMap_draw_layout, 0x2c) * 2,
                                   field<int32_t>(*g_StarMap_draw_layout, 0xc) +
                                       field<int32_t>(*g_StarMap_draw_layout, 0x2c) + 2,
                                   false);
            String_dtor(&tmp);
        }
        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        for (uint32_t i = 0; i < stations->length; i++) {
            if (i != (uint32_t)field<int32_t>(this, 0x64)) {
                drawOnScreenInfo((int)i, true);
            }
        }
        if (field<int32_t>(this, 0x64) >= 0) {
            drawOnScreenInfo(field<int32_t>(this, 0x64), true);
        }
    }

    if (field<uint8_t>(this, 0x108) != 0) {
        drawKey();
    }
    String_ctor_copy(&tmp, GameText_getText(*g_StarMap_draw_text, 0x190), false);
    Layout_drawHeader(*g_StarMap_draw_layout, &tmp);
    String_dtor(&tmp);
    Layout_drawEmptyFooter(*g_StarMap_draw_layout);
    TouchButton_draw(ptr_field(this, 0x4c));
    if (field<uint8_t>(this, 0xa9) != 0) {
        ChoiceWindow_draw(ptr_field(this, 0x5c));
    }
}
