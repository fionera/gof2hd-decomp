#include "gof2/WantedWindow.h"


extern "C" void StarMap_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
extern "C" void TouchButton_OnTouchMove(void *self, int x, int y);
extern "C" void Layout_OnTouchMove(void *self, int x, int y);
extern "C" int Wanted_isActive(void *self);
extern "C" uint32_t WantedWindow_getWantedAtPosition(WantedWindow *self, int x, int y);
extern "C" void WantedWindow_update_tail(void *starMap, int dt);
extern "C" void TouchButton_setAlwaysPressed(void *button, bool pressed);
extern "C" void ScrollTouchWindow_update(void *window, int dt);
extern "C" void StarMap_OnTouchBegin(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);
extern "C" void TouchButton_OnTouchBegin(void *self, int x, int y);
extern "C" void Layout_OnTouchBegin(void *self, int x, int y);
extern "C" void WantedWindow_render3D_tail(void *starMap);
extern "C" int __aeabi_idiv(int numerator, int denominator);
extern "C" void StarMap_OnTouchEnd_tail(void);
extern "C" int StarMap_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" void WantedWindow_selectWanted(WantedWindow *self, int idx);
extern "C" int Wanted_isActive(void *wanted);
extern "C" int Wanted_getLastSeen(void *wanted);
extern "C" int Wanted_getTravelsTo(void *wanted);
extern "C" void *ApplicationManager_GetApplicationModule(void *app, int id);
extern "C" void *Galaxy_getStation(void *galaxy, int id);
extern "C" void *Mission_ctor(void *mission, int a, int b, int dest);
extern "C" void *StarMap_ctor(void *map, bool jumpMapMode, void *mission, bool flag, int idx);
extern "C" void StarMap_init(void *map, bool jumpMapMode, void *mission, bool flag, int idx);
extern "C" int Station_getSystem(void *station);
extern "C" void StarMap_setStart(void *map, int system, int lastSeen);
extern "C" void *Station_dtor(void *station);
extern "C" int Layout_OnTouchEnd(void *layout, int x, int y);
extern "C" void Layout_resetWindowDimensions(void *layout);
extern "C" int Layout_helpPressed(void *layout);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void String_copy_ctor(String *dst, String *src, bool copy);
extern "C" void Layout_initHelpWindow(void *layout, String *text);
extern "C" void String_dtor(String *s);
extern "C" void WantedWindow_draw_tail(void *starMap);
extern "C" float WantedWindow_getRelativeScrollStartPos(WantedWindow *self);
extern "C" float WantedWindow_getRelativeScrollHeight(WantedWindow *self);
extern "C" void PaintCanvas_EnableClip(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_DisableClip(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, unsigned int color);
extern "C" int PaintCanvas_GetTextHeight(void *canvas, unsigned int font);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *font, String *text);
extern "C" void PaintCanvas_DrawString(void *canvas, void *font, String *text, int x, int y, bool flag);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void Layout_drawScrollBar(void *layout, int x, int y, int h, int start, int size);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, String *text);
extern "C" void Layout_drawHeader(void *layout, String *text);
extern "C" void Layout_drawFooter(void *layout);
extern "C" void TouchButton_draw(void *button);
extern "C" void Wanted_getName(String *out, void *wanted);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void String_cstr_ctor(String *s, const char *text, bool copy);
extern "C" void String_copy_ctor(String *s, String *src, bool copy);
extern "C" void String_plus(String *out, String *a, String *b);
extern "C" void ImageFactory_drawChar(void *factory, void *parts, int x, int y, bool flag);
extern "C" void ScrollTouchWindow_draw(void *scroll);
extern "C" void Array_Wanted_ctor(void *arr);
extern "C" void ArrayAdd_Wanted(void *wanted, void *arr);
extern "C" void *Status_getWanted(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getRace(void *system);
extern "C" int Wanted_getBoard(void *wanted);
extern "C" void Array_TouchButton_ctor(void *arr);
extern "C" void ArraySetLength_TouchButton(int length, void *arr);
extern "C" int Layout_getHelpButtonOffset(void *layout);
extern "C" void TouchButton_ctor6(void *button, String *text, int type, int x, int y, int icon);
extern "C" int TouchButton_getWidth(void *button);
extern "C" void Layout_setWindowDimensions(void *layout, int x, int y, int w, int h);
extern "C" void *TouchButton_dtor(void *button);
extern "C" void TouchButton_ctor8(void *button, String *text, int type, int x, int y, int width, int icon, int mode);
extern "C" void ArrayReleaseClasses_ImagePart(void *arr);
extern "C" void *Array_ImagePart_dtor(void *arr);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *Array_Wanted_dtor(void *arr);
extern "C" void *ScrollTouchWindow_dtor(void *window);
extern "C" void String_ctor(void *self);
extern "C" void PaintCanvas_Image2DCreate(void *canvas, int image, void *out);
extern "C" int WantedWindow_init(WantedWindow *self);
extern "C" void *ImageFactory_loadChar(void *factory, void *parts);
extern "C" void *Wanted_getImageParts(void *wanted);
extern "C" int Wanted_isTerminated(void *wanted);
extern "C" int Wanted_getCurrentLocation(void *wanted);
extern "C" int Wanted_getReward(void *wanted);
extern "C" int Wanted_getIndex(void *wanted);
extern "C" void Station_getName(String *out, void *station);
extern "C" void SolarSystem_getName(String *out, int system);
extern "C" void String_int_ctor(String *s, int value);
extern "C" void String_assign(String *dst, String *src);
extern "C" void String_plusAssign(String *dst, String *src);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void ScrollTouchWindow_setText(void *self, String *a, String *b);

// ---- OnTouchMove_e21a0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_move_layout;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_a;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_force;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_b;


extern "C" int WantedWindow_OnTouchMove(WantedWindow *self, int x, int y)
{
    if (self->field_0x14 != 0) {
        StarMap_OnTouchMove(self->field_0x4, x, y);
        return 0;
    }

    void **layoutHolder = g_WantedWindow_move_layout;
    void *layout = *layoutHolder;
    if (((F<int>(layout, 0xc) < y) &&
         (y < *g_WantedWindow_move_screen_h - F<int>(layout, 0x10)) &&
         (x < *g_WantedWindow_move_screen_w_a / 2)) ||
        (*g_WantedWindow_move_force != 0)) {
        int delta = y - self->field_0x88;
        self->field_0x90 = delta;
        self->field_0x94 = 1.0f;
        self->field_0x84 += delta;
        self->field_0x88 = y;
    }

    if (*g_WantedWindow_move_screen_w_b / 2 < x) {
        ScrollTouchWindow_OnTouchMove(self->field_0x2c, x, y);
    }

    uint32_t i = 0;
    while (i < F<uint32_t>(self->field_0xc, 0x0)) {
        TouchButton_OnTouchMove(ArrayItem(self->field_0xc, i), x, y);
        ++i;
    }

    Layout_OnTouchMove(*layoutHolder, x, y);
    if (self->field_0x18 != 0) {
        void *wanted = ArrayItem(self->field_0x38, self->field_0x30);
        if (Wanted_isActive(wanted) != 0) {
            TouchButton_OnTouchMove(self->field_0x18, x, y);
        }
    }

    uint32_t selected = WantedWindow_getWantedAtPosition(self, x, y);
    int moved = y - self->field_0x9c;
    if (moved < 0) {
        moved = -moved;
    }
    if (moved > 5) {
        selected = 0xffffffffu;
    }
    self->field_0x34 = selected;
    return 0;
}

// ---- update_e2028.cpp ----
extern "C" void WantedWindow_update(WantedWindow *self, int dt)
{
    if (self->field_0x14 != 0) {
        return WantedWindow_update_tail(self->field_0x4, dt);
    }

    TouchButton_setAlwaysPressed(*(void **)((char *)F<void *>(self->field_0xc, 0x4) + 4), true);
    ScrollTouchWindow_update(self->field_0x2c, dt);

    if (self->field_0xa0 == 0) {
        float speed = self->field_0x94;
        float old = self->field_0x98;
        float delta = speed * old;
        float mag = -(speed * old);
        if (0.0f < delta) {
            mag = delta;
        }
        self->field_0x98 = delta;
        if (mag > 1.0f) {
            self->field_0x84 = (int)(delta + (float)self->field_0x84);
        }
    }

    int scroll = self->field_0x84;
    if (scroll > 0) {
        self->field_0x94 = 1.0f;
        self->field_0x98 = (float)-scroll * 0.5f;
    }

    int limit = self->field_0xa8 - self->field_0xa4;
    if (limit <= -1) {
        if (scroll < limit) {
            self->field_0x94 = 1.0f;
            self->field_0x98 = (float)(limit - scroll) * 0.5f;
        }
    } else {
        self->field_0x84 = 0;
        self->field_0x98 = 0;
    }
}

// ---- OnTouchBegin_e2104.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_touch_layout;


extern "C" int WantedWindow_OnTouchBegin(WantedWindow *self, int x, int y)
{
    if (self->field_0x14 != 0) {
        StarMap_OnTouchBegin(self->field_0x4, x, y);
        return 0;
    }

    self->field_0x88 = y;
    self->field_0x9c = y;
    self->field_0x90 = 0;
    self->field_0xa0 = 1;
    ScrollTouchWindow_OnTouchBegin(self->field_0x2c, x, y);

    uint32_t i = 0;
    while (i < F<uint32_t>(self->field_0xc, 0x0)) {
        TouchButton_OnTouchBegin(ArrayItem(self->field_0xc, i), x, y);
        ++i;
    }

    Layout_OnTouchBegin(*g_WantedWindow_touch_layout, x, y);
    if (self->field_0x18 != 0) {
        void *wanted = ArrayItem(self->field_0x38, self->field_0x30);
        if (Wanted_isActive(wanted) != 0) {
            TouchButton_OnTouchBegin(self->field_0x18, x, y);
        }
    }

    self->field_0x34 = WantedWindow_getWantedAtPosition(self, x, y);
    return 0;
}

// ---- render3D_e182c.cpp ----
extern "C" void WantedWindow_render3D(WantedWindow *self)
{
    if (self->field_0x14 != 0) {
        return WantedWindow_render3D_tail(self->field_0x4);
    }
}

// ---- getRelativeScrollStartPos_e17a8.cpp ----
extern "C" float WantedWindow_getRelativeScrollStartPos(WantedWindow *self)
{
    int pos = self->field_0x84;
    if (pos > 0) {
        union { uint32_t u; float f; } c;
        c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)pos / (float)self->field_0xa4;
}

// ---- getWantedAtPosition_e1fc0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_hit_layout;

extern "C" uint32_t WantedWindow_getWantedAtPosition(WantedWindow *self, int x, int y)
{
    if (x >= self->field_0x1c + (self->field_0x24 >> 1)) {
        return 0xffffffffu;
    }

    void *list = self->field_0x38;
    void *layout = *g_WantedWindow_hit_layout;
    int numerator = y - self->field_0x20;
    numerator -= F<int>(layout, 0xc);
    numerator -= F<int>(layout, 0x20);
    numerator -= F<int>(layout, 0x5c);
    numerator -= self->field_0x84;
    int idx = __aeabi_idiv(numerator, F<int>(layout, 0x70) + F<int>(layout, 0x34));
    if ((uint32_t)idx > (uint32_t)(F<int>(list, 0x0) - 1)) {
        return 0xffffffffu;
    }
    return (uint32_t)idx;
}

// ---- OnTouchEnd_e22ac.cpp ----
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_end_fullscreen;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_end_tablet;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_end_small;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_screen_w;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_window_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_window_w;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_app;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_galaxy;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_layout_a;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_layout_b;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_text;

extern "C" void WantedWindow_OnTouchEnd(WantedWindow *self, int x, int y)
{
    String help;

    if (self->field_0x14 != 0) {
        if (StarMap_OnTouchEnd(self->field_0x4, x, y) != 0) {
            uint32_t h;
            uint32_t w;
            uint32_t halfW = 0;
            if (*g_WantedWindow_end_fullscreen == 0) {
                h = *g_WantedWindow_end_window_h;
                w = *g_WantedWindow_end_window_w;
                self->field_0x1c = 0;
                self->field_0x20 = 0;
            } else {
                if (*g_WantedWindow_end_tablet == 0) {
                    h = 1000;
                    w = 0x514;
                    if (*g_WantedWindow_end_small == 0) {
                        h = 500;
                        w = 0x28a;
                    }
                    halfW = w >> 1;
                } else {
                    h = 0x2bf;
                    halfW = 0x1c9;
                    w = 0x392;
                }
                self->field_0x1c = (*g_WantedWindow_end_screen_w >> 1) - halfW;
                self->field_0x20 = (*g_WantedWindow_end_screen_h >> 1) - (h >> 1);
            }
            self->field_0x24 = w;
            self->field_0x28 = h;
            self->field_0x14 = 0;
        }
    } else {
        int delta = self->field_0x90;
        int pos = self->field_0x84 + delta;
        float velocity = 0.0f;
        int mag = delta;
        if (mag < 0) {
            mag = -mag;
        }
        if (mag > 3) {
            velocity = (float)delta;
        }
        self->field_0x94 = 0.95f;
        self->field_0xa0 = 0;
        self->field_0x84 = pos;
        self->field_0x8c = pos;
        self->field_0x98 = velocity;

        ScrollTouchWindow_OnTouchEnd(self->field_0x2c, x, y);
        for (uint32_t i = 0; i < F<uint32_t>(self->field_0xc, 0x0); ++i) {
            if (TouchButton_OnTouchEnd(ArrayItem(self->field_0xc, i), x, y) != 0) {
                self->field_0x0 = i;
            }
        }

        if (self->field_0x34 >= 0) {
            int idx = WantedWindow_getWantedAtPosition(self, x, y);
            self->field_0x30 = idx;
            self->field_0x34 = idx;
            WantedWindow_selectWanted(self, idx);
        }

        bool openMap = false;
        if (self->field_0x18 != 0) {
            void *wanted = ArrayItem(self->field_0x38, self->field_0x30);
            if (Wanted_isActive(wanted) != 0 &&
                TouchButton_OnTouchEnd(self->field_0x18, x, y) != 0) {
                openMap = true;
            }
        }

        if (openMap) {
            void **appHolder = g_WantedWindow_end_app;
            void *module = ApplicationManager_GetApplicationModule(*appHolder, 5);
            self->field_0x4 = F<void *>(module, 0x10);
            void *wanted = ArrayItem(self->field_0x38, self->field_0x30);
            int lastSeen = Wanted_getLastSeen(wanted);
            void *station = Galaxy_getStation(*g_WantedWindow_end_galaxy, lastSeen);
            void *oldMission = self->field_0xb0;
            if (oldMission != 0) {
                (*(void (**)(void *))(*(int *)oldMission + 4))(oldMission);
            }
            self->field_0xb0 = 0;

            void *mission = operator_new(0x78);
            Mission_ctor(mission, 0, 0, Wanted_getTravelsTo(wanted));
            self->field_0xb0 = mission;

            void *map = self->field_0x4;
            if (map == 0) {
                map = operator_new(0x1e8);
                StarMap_ctor(map, true, mission, false, -1);
                ApplicationManager_GetApplicationModule(*appHolder, 5);
                F<void *>(ApplicationManager_GetApplicationModule(*appHolder, 5), 0x10) = map;
                map = F<void *>(ApplicationManager_GetApplicationModule(*appHolder, 5), 0x10);
                self->field_0x4 = map;
            } else {
                StarMap_init(map, true, mission, false, -1);
            }

            int system = Station_getSystem(station);
            lastSeen = Wanted_getLastSeen(wanted);
            StarMap_setStart(map, system, lastSeen);
            if (station != 0) {
                operator_delete(Station_dtor(station));
            }
            self->field_0x14 = 1;
            Layout_resetWindowDimensions(*g_WantedWindow_end_layout_a);
        } else {
            void **layoutHolder = g_WantedWindow_end_layout_b;
            void *layout = *layoutHolder;
            if (Layout_OnTouchEnd(layout, x, y) != 0) {
                Layout_resetWindowDimensions(layout);
            } else if (Layout_helpPressed(layout) != 0) {
                String_copy_ctor(&help, GameText_getText(*g_WantedWindow_end_text, 0x27b), false);
                Layout_initHelpWindow(layout, &help);
                String_dtor(&help);
            }
        }
    }

}

// ---- draw_e1838.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_layout;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_canvas;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_font;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_status;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_text;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_draw_factory;
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_empty_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_mark[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_empty_b[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_empty_c[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_label_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_draw_label_b[];

static inline void *draw_wanted_at(WantedWindow *self, int idx)
{
    return ArrayItem(self->field_0x38, idx);
}

extern "C" void WantedWindow_draw(WantedWindow *self)
{
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;
    String sa0;

    if (self->field_0x14 != 0) {
        WantedWindow_draw_tail(self->field_0x4);
        return;
    }

    void *layout = *g_WantedWindow_draw_layout;
    void *canvas = *g_WantedWindow_draw_canvas;
    void *font = *g_WantedWindow_draw_font;

    PaintCanvas_EnableClip(canvas, self->field_0x1c,
                           self->field_0x20 + F<int>(layout, 0xc) +
                               F<int>(layout, 0x20) + F<int>(layout, 0x5c),
                           self->field_0x24,
                           F<int>(layout, 0x2c) + self->field_0xa8);

    float relStart = WantedWindow_getRelativeScrollStartPos(self);
    int visible = self->field_0xa8;
    float visf = (float)visible;
    float relHeight = WantedWindow_getRelativeScrollHeight(self);
    int barSize = (int)(relHeight * visf);
    int barStart = (int)(relStart * visf);
    if (barSize >= 1 || barStart >= 0) {
        Layout_drawScrollBar(layout,
                             ((self->field_0x1c + (self->field_0x24 >> 1)) -
                              F<int>(layout, 0x2c)) - F<int>(layout, 0x48),
                             self->field_0x20 + F<int>(layout, 0x2c) +
                                 F<int>(layout, 0xc) + F<int>(layout, 0x20) +
                                 F<int>(layout, 0x5c),
                             visible, barStart, barSize);
    }

    int y = self->field_0x20 + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
            F<int>(layout, 0x5c) + F<int>(layout, 0x2c) + self->field_0x84;
    int inset = barSize < 1 ? 0 : F<int>(layout, 0x2c) + F<int>(layout, 0x48);

    for (uint32_t i = 0; i < F<uint32_t>(self->field_0x38, 0x0); ++i) {
        int style = (i == self->field_0x30 || i == self->field_0x34) ? 4 : 3;
        String_cstr_ctor(&s40, g_WantedWindow_draw_empty_a, false);
        Layout_drawBox(layout, style, F<int>(layout, 0x28) + self->field_0x1c, y,
                       (self->field_0x24 >> 1) -
                           (inset + F<int>(layout, 0x28) + F<int>(layout, 0x2c)),
                       F<int>(layout, 0x70), &s40);
        String_dtor(&s40);

        void *wanted = draw_wanted_at(self, i);
        PaintCanvas_SetColor(canvas, Wanted_isActive(wanted) ? 0xffffffffu : 0xff808080u);
        Wanted_getName(&s4c, wanted);
        int textY = y + F<int>(layout, 0x70) / 2 -
                    PaintCanvas_GetTextHeight(canvas, (unsigned int)(unsigned long)font) / 2;
        PaintCanvas_DrawString(canvas, font, &s4c,
                               self->field_0x1c + F<int>(layout, 0x28) +
                                   F<int>(layout, 0x44),
                               textY, false);
        String_dtor(&s4c);

        int campaign = Status_getCurrentCampaignMission(*g_WantedWindow_draw_status);
        if ((i == 0 && campaign == 0x80) ||
            (i == 1 && Status_getCurrentCampaignMission(*g_WantedWindow_draw_status) == 0x82)) {
            Wanted_getName(&s58, wanted);
            String_cstr_ctor(&s64, g_WantedWindow_draw_mark, false);
            String_plus(&s4c, &s58, &s64);
            int textW = PaintCanvas_GetTextWidth(canvas, font, &s4c);
            PaintCanvas_DrawImage2D(canvas, self->field_0xac,
                                    self->field_0x1c + F<int>(layout, 0x28) +
                                        F<int>(layout, 0x44) + textW,
                                    textY);
            String_dtor(&s4c);
            String_dtor(&s64);
            String_dtor(&s58);
        }

        y += F<int>(layout, 0x34) + F<int>(layout, 0x70);
    }

    PaintCanvas_DisableClip(canvas);
    PaintCanvas_SetColor(canvas, 0xffffffffu);
    String_copy_ctor(&s70, GameText_getText(*g_WantedWindow_draw_text, 0xc93), false);
    Layout_drawHeader(layout, &s70);
    String_dtor(&s70);

    for (uint32_t i = 0; i < F<uint32_t>(self->field_0xc, 0x0); ++i) {
        TouchButton_draw(ArrayItem(self->field_0xc, i));
    }

    String_copy_ctor(&s7c, GameText_getText(*g_WantedWindow_draw_text, 0xc95), false);
    Layout_drawBox(layout, 1, F<int>(layout, 0x28) + self->field_0x1c,
                   self->field_0x20 + F<int>(layout, 0xc) + F<int>(layout, 0x20),
                   (self->field_0x24 >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)),
                   F<int>(layout, 0x5c), &s7c);
    String_dtor(&s7c);

    String_cstr_ctor(&s88, g_WantedWindow_draw_empty_b, false);
    Layout_drawBox(layout, 5, F<int>(layout, 0x28) + self->field_0x1c,
                   self->field_0x20 + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
                       F<int>(layout, 0x5c) + F<int>(layout, 0x2c),
                   (self->field_0x24 >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)),
                   ((self->field_0x28 -
                     (F<int>(layout, 0x20) + F<int>(layout, 0xc) +
                      F<int>(layout, 0x5c) + F<int>(layout, 0x2c) * 2)) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24),
                   &s88);
    String_dtor(&s88);

    String_copy_ctor(&s94, GameText_getText(*g_WantedWindow_draw_text, 0xc95), false);
    Layout_drawBox(layout, 1, self->field_0x1c + (self->field_0x24 >> 1) +
                                  F<int>(layout, 0x2c),
                   self->field_0x20 + F<int>(layout, 0xc) + F<int>(layout, 0x20),
                   ((self->field_0x24 >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28),
                   F<int>(layout, 0x5c), &s94);
    String_dtor(&s94);

    String_cstr_ctor(&sa0, g_WantedWindow_draw_empty_c, false);
    Layout_drawBox(layout, 5, self->field_0x1c + (self->field_0x24 >> 1) +
                                  F<int>(layout, 0x2c),
                   self->field_0x20 + F<int>(layout, 0x2c) + F<int>(layout, 0xc) +
                       F<int>(layout, 0x20) + F<int>(layout, 0x5c),
                   ((self->field_0x24 >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28),
                   ((self->field_0x28 -
                     (F<int>(layout, 0xc) + F<int>(layout, 0x2c) * 2 +
                      F<int>(layout, 0x20) + F<int>(layout, 0x5c))) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24),
                   &sa0);
    String_dtor(&sa0);

    if (self->field_0x8 != 0) {
        int charX = self->field_0x1c + (self->field_0x24 >> 1) + F<int>(layout, 0x2c);
        int charY = F<int>(layout, 0x5c) + self->field_0x20 + F<int>(layout, 0x2c) +
                    F<int>(layout, 0xc) + F<int>(layout, 0x20);
        ImageFactory_drawChar(*g_WantedWindow_draw_factory, self->field_0x8, charX, charY, false);
        int textX = F<int>(layout, 0x2d4) + charX + F<int>(layout, 0x2c);
        PaintCanvas_DrawString(canvas, font, (String *)((char *)self + 0x54), textX, charY, false);

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_a, false);
        String_plus(&s58, &s64, GameText_getText(*g_WantedWindow_draw_text, 0xc93));
        String_plus(&s4c, &s58, (String *)((char *)self + 0x3c));
        PaintCanvas_DrawString(canvas, font, &s4c, textX, charY + F<int>(layout, 0x4) * 2, false);
        String_dtor(&s4c);
        String_dtor(&s58);
        String_dtor(&s64);

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_b, false);
        String_plus(&s58, &s64, GameText_getText(*g_WantedWindow_draw_text, 0xc93));
        String_plus(&s4c, &s58, (String *)((char *)self + 0x48));
        PaintCanvas_DrawString(canvas, font, &s4c, textX, charY + F<int>(layout, 0x4) * 3, false);
        String_dtor(&s4c);
        String_dtor(&s58);
        String_dtor(&s64);

        ScrollTouchWindow_draw(self->field_0x2c);
    }

    if (self->field_0x18 != 0 &&
        Wanted_isActive(draw_wanted_at(self, self->field_0x30)) != 0) {
        TouchButton_draw(self->field_0x18);
    }

    Layout_drawFooter(layout);
}

// ---- init_e0b38.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_init_status;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_init_layout;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_init_fullscreen;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_init_tablet;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_init_small;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_screen_w;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_window_w;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_window_h;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_init_text;

extern "C" int WantedWindow_init(WantedWindow *self)
{
    v4si zero = {0, 0, 0, 0};
    self->field_0x91 = zero;
    // Original cleared a 16-byte vector starting at +0x84 (the scroll-state ints).
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x8c = 0;
    self->field_0x90 = 0;

    void *wantedList = operator_new(0x0c);
    Array_Wanted_ctor(wantedList);
    self->field_0x38 = wantedList;

    void *status = *g_WantedWindow_init_status;
    void *allWanted = Status_getWanted(status);
    void **layoutHolder = g_WantedWindow_init_layout;
    void *layout = *layoutHolder;
    int wBase = F<int>(layout, 0x28);
    int wLimit = F<int>(layout, 0xcc);
    int wExtra = F<int>(layout, 0x4c);
    int activeWidth = wLimit - wBase - wExtra;

    for (uint32_t i = 0; i < F<uint32_t>(allWanted, 0x0); ++i) {
        int race = SolarSystem_getRace(Status_getSystem(status));
        void *wanted = ArrayItem(allWanted, i);
        if (race == Wanted_getBoard(wanted)) {
            race = SolarSystem_getRace(Status_getSystem(status));
            if (race != 0 || Status_getCurrentCampaignMission(status) < 0x80) {
                race = SolarSystem_getRace(Status_getSystem(status));
                if (race == 0 && Status_getCurrentCampaignMission(status) >= 0xa2) {
                    ArrayAdd_Wanted(wanted, self->field_0x38);
                }
            } else {
                ArrayAdd_Wanted(wanted, self->field_0x38);
            }
        }
    }

    uint32_t h;
    uint32_t w;
    uint32_t halfW = 0;
    if (*g_WantedWindow_init_fullscreen == 0) {
        h = *g_WantedWindow_init_window_h;
        w = *g_WantedWindow_init_window_w;
        self->field_0x1c = 0;
        self->field_0x20 = 0;
    } else {
        if (*g_WantedWindow_init_tablet == 0) {
            h = 1000;
            w = 0x514;
            if (*g_WantedWindow_init_small == 0) {
                h = 500;
                w = 0x28a;
            }
            halfW = w >> 1;
        } else {
            h = 0x2bf;
            halfW = 0x1c9;
            w = 0x392;
        }
        self->field_0x1c = (*g_WantedWindow_init_screen_w >> 1) - halfW;
        self->field_0x20 = (*g_WantedWindow_init_screen_h >> 1) - (h >> 1);
    }

    self->field_0x24 = w;
    self->field_0x28 = h;
    self->field_0x30 = 0;

    uint32_t selected = 0;
    for (;;) {
        void *arr = self->field_0x38;
        uint32_t count = F<uint32_t>(arr, 0x0);
        if (selected >= count) {
            selected = self->field_0x30;
            if (selected == count - 1) {
                if (Wanted_isActive(ArrayItem(arr, selected)) == 0) {
                    selected = 0;
                    self->field_0x30 = 0;
                } else {
                    selected = self->field_0x30;
                }
            }
            break;
        }
        if (Wanted_isActive(ArrayItem(arr, selected)) != 0) {
            self->field_0x30 = selected;
            arr = self->field_0x38;
            count = F<uint32_t>(arr, 0x0);
            if (selected == count - 1) {
                if (Wanted_isActive(ArrayItem(arr, selected)) == 0) {
                    selected = 0;
                    self->field_0x30 = 0;
                } else {
                    selected = self->field_0x30;
                }
            }
            break;
        }
        ++selected;
    }

    self->field_0x34 = selected;
    WantedWindow_selectWanted(self, selected);

    void *buttons = operator_new(0x0c);
    Array_TouchButton_ctor(buttons);
    self->field_0xc = buttons;
    ArraySetLength_TouchButton(2, buttons);

    void **textHolder = g_WantedWindow_init_text;
    void *button = operator_new(0xc8);
    String *text = GameText_getText(*textHolder, 0xc93);
    int x = self->field_0x1c;
    int bw = self->field_0x24;
    int help = Layout_getHelpButtonOffset(layout);
    TouchButton_ctor6(button, text, 3, x + bw - help, self->field_0x20, 0x12);
    *(void **)((char *)F<void *>(buttons, 0x4) + 4) = button;

    button = operator_new(0xc8);
    text = GameText_getText(*textHolder, 0x81);
    x = self->field_0x1c;
    bw = self->field_0x24;
    help = Layout_getHelpButtonOffset(layout);
    int width = TouchButton_getWidth(*(void **)((char *)F<void *>(buttons, 0x4) + 4));
    TouchButton_ctor6(button, text, 3, x + bw - help - width + F<int>(layout, 0x38),
                      self->field_0x20, 0x12);
    *(void **)F<void *>(buttons, 0x4) = button;

    TouchButton_setAlwaysPressed(*(void **)((char *)F<void *>(buttons, 0x4) + 4), true);
    Layout_setWindowDimensions(layout, self->field_0x1c, self->field_0x20,
                               self->field_0x24, self->field_0x28);

    layout = *layoutHolder;
    self->field_0xa4 = (F<int>(layout, 0x34) + F<int>(layout, 0x70)) *
                         F<int>(self->field_0x38, 0x0);
    self->field_0xa8 =
        (((((self->field_0x28 - F<int>(layout, 0x10)) - F<int>(layout, 0xc)) -
           F<int>(layout, 0x20)) - F<int>(layout, 0x24)) - F<int>(layout, 0x5c)) +
        F<int>(layout, 0x2c);

    if (self->field_0x18 != 0) {
        operator_delete(TouchButton_dtor(self->field_0x18));
    }
    self->field_0x4 = 0;
    self->field_0x18 = 0;
    self->field_0x14 = 0;

    button = operator_new(0xc8);
    text = GameText_getText(*textHolder, 0x1a8);
    layout = *layoutHolder;
    TouchButton_ctor8(button, text, 0,
                      self->field_0x1c + (self->field_0x24 >> 1) + F<int>(layout, 0x2c),
                      (((self->field_0x20 - F<int>(layout, 0x2c)) + self->field_0x28) -
                       F<int>(layout, 0x10)) - F<int>(layout, 0x24),
                      activeWidth, 0x21, 4);
    self->field_0x15 = 0;
    self->field_0x18 = button;
    self->field_0x0 = 1;
    return 1;
}

// ---- _WantedWindow_e0ec4.cpp ----
__attribute__((visibility("hidden"))) extern void (*g_WantedWindow_string_dtor)(void *);

extern "C" WantedWindow *_ZN12WantedWindowD2Ev(WantedWindow *self)
{
    void *p = self->field_0x8;
    if (p != 0) {
        ArrayReleaseClasses_ImagePart(p);
        p = self->field_0x8;
        if (p != 0) {
            operator_delete(Array_ImagePart_dtor(p));
        }
    }
    self->field_0x8 = 0;

    p = self->field_0xc;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        p = self->field_0xc;
        if (p != 0) {
            operator_delete(Array_TouchButton_dtor(p));
        }
    }
    self->field_0xc = 0;

    p = self->field_0x18;
    if (p != 0) {
        operator_delete(TouchButton_dtor(p));
    }
    self->field_0x18 = 0;

    p = self->field_0x38;
    if (p != 0) {
        operator_delete(Array_Wanted_dtor(p));
    }
    self->field_0x38 = 0;

    p = self->field_0xb0;
    if (p != 0) {
        (*(void (**)(void *))(*(int *)p + 4))(p);
    }
    self->field_0xb0 = 0;

    p = self->field_0x2c;
    if (p != 0) {
        operator_delete(ScrollTouchWindow_dtor(p));
    }
    self->field_0x2c = 0;

    void (*stringDtor)(void *) = g_WantedWindow_string_dtor;
    stringDtor((char *)self + 0x78);
    stringDtor((char *)self + 0x6c);
    stringDtor((char *)self + 0x60);
    stringDtor((char *)self + 0x54);
    stringDtor((char *)self + 0x48);
    stringDtor((char *)self + 0x3c);
    return self;
}

// ---- WantedWindow_e0a44.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_ctor_canvas;
__attribute__((visibility("hidden"))) extern unsigned int **g_WantedWindow_ctor_font;

extern "C" WantedWindow *_ZN12WantedWindowC2Ev(WantedWindow *self)
{
    String_ctor((char *)self + 0x3c);
    String_ctor((char *)self + 0x48);
    String_ctor((char *)self + 0x54);
    String_ctor((char *)self + 0x60);
    String_ctor((char *)self + 0x6c);
    String_ctor((char *)self + 0x78);

    void **canvasHolder = g_WantedWindow_ctor_canvas;
    self->field_0x18 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    unsigned int *fontHolder = *g_WantedWindow_ctor_font;
    int h = PaintCanvas_GetTextHeight(*canvasHolder, *fontHolder);
    self->field_0x38 = 0;
    self->field_0xb0 = 0;
    self->field_0x2c = 0;
    self->field_0xc = 0;
    self->field_0x10 = h / 2 - 1;
    PaintCanvas_Image2DCreate(*canvasHolder, 0x454, (char *)self + 0xac);
    WantedWindow_init(self);
    return self;
}

// ---- getRelativeScrollHeight_e17d8.cpp ----
extern "C" float WantedWindow_getRelativeScrollHeight(WantedWindow *self)
{
    long long pair = self->field_0xa4;
    int content = (int)pair;
    int visible = (int)((unsigned long long)pair >> 32);
    if (content < visible) {
        union { uint32_t u; float f; } c;
        c.u = 0x4605e009u;
        return c.f;
    }
    int scroll = self->field_0x84;
    int num;
    if (scroll >= 1) {
        num = visible - scroll;
    } else if (scroll >= visible - content) {
        num = visible;
    } else {
        num = scroll + content;
    }
    return (float)num / (float)content;
}

// ---- selectWanted_e1034.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_select_stack;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_factory;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_galaxy;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_text_a;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_layout;
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_b[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_c[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_d[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_from[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_to[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_at[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_reward[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_space[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_b[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_c[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_d[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_empty[];

static inline void *wanted_at(WantedWindow *self, int idx)
{
    return ArrayItem(self->field_0x38, idx);
}

static inline void append_label(String *dst, const char *prefix, String *value)
{
    String a;
    String b;
    String_cstr_ctor(&a, prefix, false);
    String_plus(&b, &a, value);
    String_plusAssign(dst, &b);
    String_dtor(&b);
    String_dtor(&a);
}

extern "C" void WantedWindow_selectWanted(WantedWindow *self, int idx)
{
    String s34;
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;

    if (self->field_0x8 != 0) {
        operator_delete(Array_ImagePart_dtor(self->field_0x8));
    }
    self->field_0x8 = 0;

    if (self->field_0x2c != 0) {
        operator_delete(ScrollTouchWindow_dtor(self->field_0x2c));
    }
    self->field_0x2c = 0;

    void *wanted = wanted_at(self, idx);
    self->field_0x8 = ImageFactory_loadChar(*g_WantedWindow_select_factory,
                                                  Wanted_getImageParts(wanted));

    Wanted_getName(&s34, wanted);
    String_assign((String *)((char *)self + 0x54), &s34);
    String_dtor(&s34);

    if (Wanted_isActive(wanted) != 0) {
        void *galaxy = *g_WantedWindow_select_galaxy;
        void *last = Galaxy_getStation(galaxy, Wanted_getLastSeen(wanted));
        void *travel = Galaxy_getStation(galaxy, Wanted_getTravelsTo(wanted));
        void *current = Galaxy_getStation(galaxy, Wanted_getCurrentLocation(wanted));

        Station_getName(&s58, last);
        String_cstr_ctor(&s64, g_WantedWindow_s_from, false);
        String_plus(&s4c, &s58, &s64);
        SolarSystem_getName(&s70, Station_getSystem(last));
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        String_assign((String *)((char *)self + 0x3c), &s34);
        String_dtor(&s34);
        String_dtor(&s7c);
        String_dtor(&s40);
        String_dtor(&s70);
        String_dtor(&s4c);
        String_dtor(&s64);
        String_dtor(&s58);

        Station_getName(&s58, travel);
        String_cstr_ctor(&s64, g_WantedWindow_s_to, false);
        String_plus(&s4c, &s58, &s64);
        SolarSystem_getName(&s70, Station_getSystem(travel));
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        String_assign((String *)((char *)self + 0x48), &s34);
        String_dtor(&s34);
        String_dtor(&s7c);
        String_dtor(&s40);
        String_dtor(&s70);
        String_dtor(&s4c);
        String_dtor(&s64);
        String_dtor(&s58);

        Station_getName(&s58, current);
        String_cstr_ctor(&s64, g_WantedWindow_s_at, false);
        String_plus(&s4c, &s58, &s64);
        SolarSystem_getName(&s70, Station_getSystem(current));
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        String_assign((String *)((char *)self + 0x6c), &s34);
        String_dtor(&s34);
        String_dtor(&s7c);
        String_dtor(&s40);
        String_dtor(&s70);
        String_dtor(&s4c);
        String_dtor(&s64);
        String_dtor(&s58);

        String_int_ctor(&s40, Wanted_getReward(wanted));
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        String_assign((String *)((char *)self + 0x78), &s34);
        String_dtor(&s34);
        String_dtor(&s4c);
        String_dtor(&s40);

        if (last != 0) {
            operator_delete(Station_dtor(last));
        }
        if (travel != 0) {
            operator_delete(Station_dtor(travel));
        }
        if (current != 0) {
            operator_delete(Station_dtor(current));
        }
    } else if (Wanted_isTerminated(wanted) != 0) {
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_a, false);
        String_assign((String *)((char *)self + 0x3c), &s34);
        String_dtor(&s34);
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_b, false);
        String_assign((String *)((char *)self + 0x48), &s34);
        String_dtor(&s34);
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_c, false);
        String_assign((String *)((char *)self + 0x6c), &s34);
        String_dtor(&s34);
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_d, false);
        String_assign((String *)((char *)self + 0x78), &s34);
        String_dtor(&s34);
    } else {
        String *text = GameText_getText(*g_WantedWindow_select_text_a, 0xc9d);
        String_assign((String *)((char *)self + 0x3c), text);
        text = GameText_getText(*g_WantedWindow_select_text_a, 0xc9d);
        String_assign((String *)((char *)self + 0x48), text);
        String_int_ctor(&s40, Wanted_getReward(wanted));
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        String_assign((String *)((char *)self + 0x78), &s34);
        String_dtor(&s34);
        String_dtor(&s4c);
        String_dtor(&s40);
        text = GameText_getText(*g_WantedWindow_select_text_a, 0xc9d);
        String_assign((String *)((char *)self + 0x6c), text);
    }

    Wanted_isTerminated(wanted);
    String_assign((String *)((char *)self + 0x60),
                  GameText_getText(*g_WantedWindow_select_text_a, 0xc9d));

    void *layout = *g_WantedWindow_select_layout;
    int y = self->field_0x20;
    int h = self->field_0x28;
    int top = y + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
              F<int>(layout, 0x5c) + F<int>(layout, 0x2c);
    int height = (((((y - F<int>(layout, 0x2c)) + h) - top) -
                   F<int>(layout, 0x10)) - F<int>(layout, 0x2d8)) -
                 F<int>(layout, 0x24);
    if (Wanted_isActive(wanted) != 0) {
        height = (height - F<int>(layout, 0x4c)) - F<int>(layout, 0x30);
    }

    void *scroll = operator_new(0x24);
    int pad = F<int>(layout, 0x2c);
    ScrollTouchWindow_ctor(scroll, self->field_0x1c + (self->field_0x24 >> 1) + pad,
                           F<int>(layout, 0x2d8) + pad + top,
                           ((self->field_0x24 >> 1) - pad) - F<int>(layout, 0x28),
                           height, false);
    self->field_0x2c = scroll;

    String_cstr_ctor(&s34, g_WantedWindow_s_empty, false);
    String_assign(&s34, GameText_getText(*g_WantedWindow_select_text_a, 0xc9d));
    append_label(&s34, g_WantedWindow_s_line_a,
                 (String *)((char *)self + 0x3c));
    append_label(&s34, g_WantedWindow_s_line_b,
                 (String *)((char *)self + 0x48));
    append_label(&s34, g_WantedWindow_s_line_c,
                 (String *)((char *)self + 0x6c));
    Wanted_getIndex(wanted);
    append_label(&s34, g_WantedWindow_s_line_d,
                 (String *)((char *)self + 0x78));

    String_cstr_ctor(&s88, g_WantedWindow_s_empty, false);
    String_copy_ctor(&s94, &s34, false);
    ScrollTouchWindow_setText(self->field_0x2c, &s88, &s94);
    String_dtor(&s94);
    String_dtor(&s88);
    String_dtor(&s34);
}
