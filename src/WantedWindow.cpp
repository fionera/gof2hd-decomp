#include "gof2/WantedWindow.h"
#include "gof2/Galaxy.h"
#include "gof2/ScrollTouchWindow.h"
#include "gof2/StarMap.h"
#include "gof2/Status.h"
#include "gof2/ApplicationManager.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/SolarSystem.h"   // defines the canonical global `struct RetStr` (12-byte String sret)
// Station.h, TouchButton.h and Wanted.h each redefine the identical (token-for-token,
// 12-byte aligned aggregate) global `struct RetStr` already provided by SolarSystem.h.
// Rename their duplicate definitions so they don't collide; the only RetStr-returning
// methods used here (getName/getText) have their results discarded, so the distinct-but-
// identical return type is harmless. (Those headers are out of this batch's edit scope.)
#define RetStr RetStr
#include "gof2/Station.h"
#undef RetStr
#define RetStr RetStr
#define B B_TouchButton
#define I I_TouchButton
#define P P_TouchButton
#include "gof2/TouchButton.h"
#undef RetStr
#undef B
#undef I
#undef P
#define RetStr RetStr
#include "gof2/Wanted.h"
#undef RetStr
#include "gof2/String.h"
#include "gof2/PaintCanvas.h"


extern "C" void WantedWindow_update_tail(void *starMap, int dt);
extern "C" void WantedWindow_render3D_tail(void *starMap);
extern "C" int __aeabi_idiv(int numerator, int denominator);
extern "C" void StarMap_OnTouchEnd_tail(void);
extern "C" int Wanted_getLastSeen(void *wanted);
extern "C" int Wanted_getTravelsTo(void *wanted);
extern "C" void *Mission_ctor(void *mission, int a, int b, int dest);
extern "C" void *StarMap_ctor(void *map, bool jumpMapMode, void *mission, bool flag, int idx);
extern "C" int Station_getSystem(void *station);
extern "C" void WantedWindow_draw_tail(void *starMap);
extern void DisableClip();  // AbyssEngine::PaintCanvas::DisableClip (free fn, not methodized)
extern "C" void String_cstr_ctor(String *s, const char *text, bool copy);
extern "C" void String_plus(String *out, String *a, String *b);
extern "C" void Array_Wanted_ctor(void *arr);
extern "C" void ArrayAdd_Wanted(void *wanted, void *arr);
extern "C" int SolarSystem_getRace(SolarSystem *system);
extern "C" int Wanted_getBoard(void *wanted);
extern "C" void Array_TouchButton_ctor(void *arr);
extern "C" void ArraySetLength_TouchButton(int length, void *arr);
extern "C" void TouchButton_ctor8(void *button, String *text, int type, int x, int y, int width, int icon, int mode);
extern "C" void ArrayReleaseClasses_ImagePart(void *arr);
extern "C" void *Array_ImagePart_dtor(void *arr);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *Array_Wanted_dtor(void *arr);
extern "C" void *ScrollTouchWindow_dtor(void *window);
extern "C" void *Wanted_getImageParts(void *wanted);
extern "C" int Wanted_getCurrentLocation(void *wanted);
extern "C" int Wanted_getReward(void *wanted);
extern "C" int Wanted_getIndex(void *wanted);
extern "C" void String_int_ctor(String *s, int value);
extern "C" void String_plusAssign(String *dst, String *src);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);

// ---- OnTouchMove_e21a0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_move_layout;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_a;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_force;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_b;


int WantedWindow::OnTouchMove(int x, int y) {
    WantedWindow *self = this;
    if (self->showingMap != 0) {
        ((StarMap *)(self->starMap))->OnTouchMove(x, y);
        return 0;
    }

    void **layoutHolder = g_WantedWindow_move_layout;
    void *layout = *layoutHolder;
    if (((F<int>(layout, 0xc) < y) &&
         (y < *g_WantedWindow_move_screen_h - F<int>(layout, 0x10)) &&
         (x < *g_WantedWindow_move_screen_w_a / 2)) ||
        (*g_WantedWindow_move_force != 0)) {
        int delta = y - self->lastDragY;
        self->dragDelta = delta;
        self->scrollDamping = 1.0f;
        self->scrollOffset += delta;
        self->lastDragY = y;
    }

    if (*g_WantedWindow_move_screen_w_b / 2 < x) {
        ((ScrollTouchWindow *)(self->scrollWindow))->OnTouchMove(x, y);
    }

    uint32_t i = 0;
    while (i < F<uint32_t>(self->buttons, 0x0)) {
        ((TouchButton *)(ArrayItem(self->buttons, i)))->OnTouchMove(x, y);
        ++i;
    }

    ((Layout *)(*layoutHolder))->OnTouchMove(x, y);
    if (self->detailButton != 0) {
        void *wanted = ArrayItem(self->wantedList, self->selectedWanted);
        if (((Wanted *)(wanted))->isActive() != 0) {
            ((TouchButton *)(self->detailButton))->OnTouchMove(x, y);
        }
    }

    uint32_t selected = ((WantedWindow *)(self))->getWantedAtPosition(x, y);
    int moved = y - self->touchStartY;
    if (moved < 0) {
        moved = -moved;
    }
    if (moved > 5) {
        selected = 0xffffffffu;
    }
    self->highlightedWanted = selected;
    return 0;
}

// ---- update_e2028.cpp ----
void WantedWindow::update(int dt) {
    WantedWindow *self = this;
    if (self->showingMap != 0) {
        return WantedWindow_update_tail(self->starMap, dt);
    }

    ((TouchButton *)(*(void **)((char *)F<void *>(self->buttons, 0x4) + 4)))->setAlwaysPressed(true);
    ((ScrollTouchWindow *)(self->scrollWindow))->update(dt);

    if (self->dragging == 0) {
        float speed = self->scrollDamping;
        float old = self->scrollVelocity;
        float delta = speed * old;
        float mag = -(speed * old);
        if (0.0f < delta) {
            mag = delta;
        }
        self->scrollVelocity = delta;
        if (mag > 1.0f) {
            self->scrollOffset = (int)(delta + (float)self->scrollOffset);
        }
    }

    int scroll = self->scrollOffset;
    if (scroll > 0) {
        self->scrollDamping = 1.0f;
        self->scrollVelocity = (float)-scroll * 0.5f;
    }

    int limit = self->visibleHeight - self->contentHeight;
    if (limit <= -1) {
        if (scroll < limit) {
            self->scrollDamping = 1.0f;
            self->scrollVelocity = (float)(limit - scroll) * 0.5f;
        }
    } else {
        self->scrollOffset = 0;
        self->scrollVelocity = 0;
    }
}

// ---- OnTouchBegin_e2104.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_touch_layout;


int WantedWindow::OnTouchBegin(int x, int y) {
    WantedWindow *self = this;
    if (self->showingMap != 0) {
        ((StarMap *)(self->starMap))->OnTouchBegin(x, y);
        return 0;
    }

    self->lastDragY = y;
    self->touchStartY = y;
    self->dragDelta = 0;
    self->dragging = 1;
    ((ScrollTouchWindow *)(self->scrollWindow))->OnTouchBegin(x, y);

    uint32_t i = 0;
    while (i < F<uint32_t>(self->buttons, 0x0)) {
        ((TouchButton *)(ArrayItem(self->buttons, i)))->OnTouchBegin(x, y);
        ++i;
    }

    ((Layout *)(*g_WantedWindow_touch_layout))->OnTouchBegin(x, y);
    if (self->detailButton != 0) {
        void *wanted = ArrayItem(self->wantedList, self->selectedWanted);
        if (((Wanted *)(wanted))->isActive() != 0) {
            ((TouchButton *)(self->detailButton))->OnTouchBegin(x, y);
        }
    }

    self->highlightedWanted = ((WantedWindow *)(self))->getWantedAtPosition(x, y);
    return 0;
}

// ---- render3D_e182c.cpp ----
void WantedWindow::render3D() {
    WantedWindow *self = this;
    if (self->showingMap != 0) {
        return WantedWindow_render3D_tail(self->starMap);
    }
}

// ---- getRelativeScrollStartPos_e17a8.cpp ----
float WantedWindow::getRelativeScrollStartPos() {
    WantedWindow *self = this;
    int pos = self->scrollOffset;
    if (pos > 0) {
        union { uint32_t u; float f; } c;
        c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)pos / (float)self->contentHeight;
}

// ---- getWantedAtPosition_e1fc0.cpp ----
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_hit_layout;

uint32_t WantedWindow::getWantedAtPosition(int x, int y) {
    WantedWindow *self = this;
    if (x >= self->windowX + (self->windowWidth >> 1)) {
        return 0xffffffffu;
    }

    void *list = self->wantedList;
    void *layout = *g_WantedWindow_hit_layout;
    int numerator = y - self->windowY;
    numerator -= F<int>(layout, 0xc);
    numerator -= F<int>(layout, 0x20);
    numerator -= F<int>(layout, 0x5c);
    numerator -= self->scrollOffset;
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

void WantedWindow::OnTouchEnd(int x, int y) {
    WantedWindow *self = this;
    String help;

    if (self->showingMap != 0) {
        if ((((StarMap *)(self->starMap))->OnTouchEnd(x, y), true)) {
            uint32_t h;
            uint32_t w;
            uint32_t halfW = 0;
            if (*g_WantedWindow_end_fullscreen == 0) {
                h = *g_WantedWindow_end_window_h;
                w = *g_WantedWindow_end_window_w;
                self->windowX = 0;
                self->windowY = 0;
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
                self->windowX = (*g_WantedWindow_end_screen_w >> 1) - halfW;
                self->windowY = (*g_WantedWindow_end_screen_h >> 1) - (h >> 1);
            }
            self->windowWidth = w;
            self->windowHeight = h;
            self->showingMap = 0;
        }
    } else {
        int delta = self->dragDelta;
        int pos = self->scrollOffset + delta;
        float velocity = 0.0f;
        int mag = delta;
        if (mag < 0) {
            mag = -mag;
        }
        if (mag > 3) {
            velocity = (float)delta;
        }
        self->scrollDamping = 0.95f;
        self->dragging = 0;
        self->scrollOffset = pos;
        self->scrollOffsetSnapshot = pos;
        self->scrollVelocity = velocity;

        ((ScrollTouchWindow *)(self->scrollWindow))->OnTouchEnd(x, y);
        for (uint32_t i = 0; i < F<uint32_t>(self->buttons, 0x0); ++i) {
            if (((TouchButton *)(ArrayItem(self->buttons, i)))->OnTouchEnd(x, y) != 0) {
                self->field_0x0 = i;
            }
        }

        if (self->highlightedWanted >= 0) {
            int idx = ((WantedWindow *)(self))->getWantedAtPosition(x, y);
            self->selectedWanted = idx;
            self->highlightedWanted = idx;
            ((WantedWindow *)(self))->selectWanted(idx);
        }

        bool openMap = false;
        if (self->detailButton != 0) {
            void *wanted = ArrayItem(self->wantedList, self->selectedWanted);
            if (((Wanted *)(wanted))->isActive() != 0 &&
                ((TouchButton *)(self->detailButton))->OnTouchEnd(x, y) != 0) {
                openMap = true;
            }
        }

        if (openMap) {
            void **appHolder = g_WantedWindow_end_app;
            void *module = ((ApplicationManager *)(*appHolder))->GetApplicationModule(5);
            self->starMap = F<void *>(module, 0x10);
            void *wanted = ArrayItem(self->wantedList, self->selectedWanted);
            int lastSeen = Wanted_getLastSeen(wanted);
            void *station = (void *)(long)((Galaxy *)(*g_WantedWindow_end_galaxy))->getStation(lastSeen);
            void *oldMission = self->mission;
            if (oldMission != 0) {
                (*(void (**)(void *))(*(int *)oldMission + 4))(oldMission);
            }
            self->mission = 0;

            void *mission = ::operator new(0x78);
            Mission_ctor(mission, 0, 0, Wanted_getTravelsTo(wanted));
            self->mission = mission;

            void *map = self->starMap;
            if (map == 0) {
                map = ::operator new(0x1e8);
                StarMap_ctor(map, true, mission, false, -1);
                ((ApplicationManager *)(*appHolder))->GetApplicationModule(5);
                F<void *>(((ApplicationManager *)(*appHolder))->GetApplicationModule(5), 0x10) = map;
                map = F<void *>(((ApplicationManager *)(*appHolder))->GetApplicationModule(5), 0x10);
                self->starMap = map;
            } else {
                ((StarMap *)(map))->init(true, (Mission *)mission, false, -1);
            }

            int system = Station_getSystem(station);
            lastSeen = Wanted_getLastSeen(wanted);
            ((StarMap *)(map))->setStart(system, lastSeen);
            if (station != 0) {
                ((Station *)(station))->dtor(); ::operator delete(station);
            }
            self->showingMap = 1;
            ((Layout *)(*g_WantedWindow_end_layout_a))->resetWindowDimensions();
        } else {
            void **layoutHolder = g_WantedWindow_end_layout_b;
            void *layout = *layoutHolder;
            if (((Layout *)(layout))->OnTouchEnd(x, y) != 0) {
                ((Layout *)(layout))->resetWindowDimensions();
            } else if (((Layout *)(layout))->helpPressed() != 0) {
                ((String *)(&help))->ctor_copy((String *)((GameText *)(*g_WantedWindow_end_text))->getText(0x27b), false);
                ((Layout *)(layout))->initHelpWindow(&help);
                ((String *)(&help))->dtor();
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
    return ArrayItem(self->wantedList, idx);
}

void WantedWindow::draw() {
    WantedWindow *self = this;
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;
    String sa0;

    if (self->showingMap != 0) {
        WantedWindow_draw_tail(self->starMap);
        return;
    }

    void *layout = *g_WantedWindow_draw_layout;
    void *canvas = *g_WantedWindow_draw_canvas;
    void *font = *g_WantedWindow_draw_font;

    ((PaintCanvas *)canvas)->EnableClip(self->windowX,
                           self->windowY + F<int>(layout, 0xc) +
                               F<int>(layout, 0x20) + F<int>(layout, 0x5c),
                           self->windowWidth,
                           F<int>(layout, 0x2c) + self->visibleHeight);

    float relStart = ((WantedWindow *)(self))->getRelativeScrollStartPos();
    int visible = self->visibleHeight;
    float visf = (float)visible;
    float relHeight = ((WantedWindow *)(self))->getRelativeScrollHeight();
    int barSize = (int)(relHeight * visf);
    int barStart = (int)(relStart * visf);
    if (barSize >= 1 || barStart >= 0) {
        ((Layout *)(layout))->drawScrollBar(((self->windowX + (self->windowWidth >> 1)) -
                              F<int>(layout, 0x2c)) - F<int>(layout, 0x48), self->windowY + F<int>(layout, 0x2c) +
                                 F<int>(layout, 0xc) + F<int>(layout, 0x20) +
                                 F<int>(layout, 0x5c), visible, barStart, barSize);
    }

    int y = self->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
            F<int>(layout, 0x5c) + F<int>(layout, 0x2c) + self->scrollOffset;
    int inset = barSize < 1 ? 0 : F<int>(layout, 0x2c) + F<int>(layout, 0x48);

    for (uint32_t i = 0; i < F<uint32_t>(self->wantedList, 0x0); ++i) {
        int style = (i == self->selectedWanted || i == self->highlightedWanted) ? 4 : 3;
        String_cstr_ctor(&s40, g_WantedWindow_draw_empty_a, false);
        ((Layout *)(layout))->drawBox(style, F<int>(layout, 0x28) + self->windowX, y, (self->windowWidth >> 1) -
                           (inset + F<int>(layout, 0x28) + F<int>(layout, 0x2c)), F<int>(layout, 0x70), &s40, 0);
        ((String *)(&s40))->dtor();

        void *wanted = draw_wanted_at(self, i);
        ((PaintCanvas *)canvas)->SetColor(((Wanted *)(wanted))->isActive() ? 0xffffffffu : 0xff808080u);
        ((Wanted *)(&s4c))->getName();
        int textY = y + F<int>(layout, 0x70) / 2 -
                    ((PaintCanvas *)canvas)->GetTextHeight((unsigned int)(unsigned long)font) / 2;
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&s4c,
                               self->windowX + F<int>(layout, 0x28) +
                                   F<int>(layout, 0x44),
                               textY, false);
        ((String *)(&s4c))->dtor();

        int campaign = ((Status *)(*g_WantedWindow_draw_status))->getCurrentCampaignMission();
        if ((i == 0 && campaign == 0x80) ||
            (i == 1 && ((Status *)(*g_WantedWindow_draw_status))->getCurrentCampaignMission() == 0x82)) {
            ((Wanted *)(&s58))->getName();
            String_cstr_ctor(&s64, g_WantedWindow_draw_mark, false);
            String_plus(&s4c, &s58, &s64);
            int textW = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(unsigned long)font, (void *)&s4c);
            ((PaintCanvas *)canvas)->DrawImage2D(self->bgImage,
                                    self->windowX + F<int>(layout, 0x28) +
                                        F<int>(layout, 0x44) + textW,
                                    textY);
            ((String *)(&s4c))->dtor();
            ((String *)(&s64))->dtor();
            ((String *)(&s58))->dtor();
        }

        y += F<int>(layout, 0x34) + F<int>(layout, 0x70);
    }

    DisableClip();
    ((PaintCanvas *)canvas)->SetColor(0xffffffffu);
    ((String *)(&s70))->ctor_copy((String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc93), false);
    ((Layout *)layout)->drawHeader1(&s70);
    ((String *)(&s70))->dtor();

    for (uint32_t i = 0; i < F<uint32_t>(self->buttons, 0x0); ++i) {
        ((TouchButton *)(ArrayItem(self->buttons, i)))->draw();
    }

    ((String *)(&s7c))->ctor_copy((String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc95), false);
    ((Layout *)(layout))->drawBox(1, F<int>(layout, 0x28) + self->windowX, self->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20), (self->windowWidth >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)), F<int>(layout, 0x5c), &s7c, 0);
    ((String *)(&s7c))->dtor();

    String_cstr_ctor(&s88, g_WantedWindow_draw_empty_b, false);
    ((Layout *)(layout))->drawBox(5, F<int>(layout, 0x28) + self->windowX, self->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
                       F<int>(layout, 0x5c) + F<int>(layout, 0x2c), (self->windowWidth >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)), ((self->windowHeight -
                     (F<int>(layout, 0x20) + F<int>(layout, 0xc) +
                      F<int>(layout, 0x5c) + F<int>(layout, 0x2c) * 2)) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24), &s88, 0);
    ((String *)(&s88))->dtor();

    ((String *)(&s94))->ctor_copy((String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc95), false);
    ((Layout *)(layout))->drawBox(1, self->windowX + (self->windowWidth >> 1) +
                                  F<int>(layout, 0x2c), self->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20), ((self->windowWidth >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28), F<int>(layout, 0x5c), &s94, 0);
    ((String *)(&s94))->dtor();

    String_cstr_ctor(&sa0, g_WantedWindow_draw_empty_c, false);
    ((Layout *)(layout))->drawBox(5, self->windowX + (self->windowWidth >> 1) +
                                  F<int>(layout, 0x2c), self->windowY + F<int>(layout, 0x2c) + F<int>(layout, 0xc) +
                       F<int>(layout, 0x20) + F<int>(layout, 0x5c), ((self->windowWidth >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28), ((self->windowHeight -
                     (F<int>(layout, 0xc) + F<int>(layout, 0x2c) * 2 +
                      F<int>(layout, 0x20) + F<int>(layout, 0x5c))) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24), &sa0, 0);
    ((String *)(&sa0))->dtor();

    if (self->imageParts != 0) {
        int charX = self->windowX + (self->windowWidth >> 1) + F<int>(layout, 0x2c);
        int charY = F<int>(layout, 0x5c) + self->windowY + F<int>(layout, 0x2c) +
                    F<int>(layout, 0xc) + F<int>(layout, 0x20);
        ((ImageFactory *)(*g_WantedWindow_draw_factory))->drawChar((Arr *)self->imageParts, charX, charY, false);
        int textX = F<int>(layout, 0x2d4) + charX + F<int>(layout, 0x2c);
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)((char *)self + 0x54), textX, charY, false);

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_a, false);
        String_plus(&s58, &s64, (String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc93));
        String_plus(&s4c, &s58, (String *)((char *)self + 0x3c));
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&s4c, textX, charY + F<int>(layout, 0x4) * 2, false);
        ((String *)(&s4c))->dtor();
        ((String *)(&s58))->dtor();
        ((String *)(&s64))->dtor();

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_b, false);
        String_plus(&s58, &s64, (String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc93));
        String_plus(&s4c, &s58, (String *)((char *)self + 0x48));
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&s4c, textX, charY + F<int>(layout, 0x4) * 3, false);
        ((String *)(&s4c))->dtor();
        ((String *)(&s58))->dtor();
        ((String *)(&s64))->dtor();

        ((ScrollTouchWindow *)(self->scrollWindow))->draw();
    }

    if (self->detailButton != 0 &&
        ((Wanted *)(draw_wanted_at(self, self->selectedWanted)))->isActive() != 0) {
        ((TouchButton *)(self->detailButton))->draw();
    }

    ((Layout *)(layout))->drawFooter();
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

int WantedWindow::init() {
    WantedWindow *self = this;
    v4si zero = {0, 0, 0, 0};
    self->field_0x91 = zero;
    // Original cleared a 16-byte vector starting at +0x84 (the scroll-state ints).
    self->scrollOffset = 0;
    self->lastDragY = 0;
    self->scrollOffsetSnapshot = 0;
    self->dragDelta = 0;

    void *wantedList = ::operator new(0x0c);
    Array_Wanted_ctor(wantedList);
    self->wantedList = wantedList;

    void *status = *g_WantedWindow_init_status;
    void *allWanted = ((Status *)status)->getWanted();
    void **layoutHolder = g_WantedWindow_init_layout;
    void *layout = *layoutHolder;
    int wBase = F<int>(layout, 0x28);
    int wLimit = F<int>(layout, 0xcc);
    int wExtra = F<int>(layout, 0x4c);
    int activeWidth = wLimit - wBase - wExtra;

    for (uint32_t i = 0; i < F<uint32_t>(allWanted, 0x0); ++i) {
        int race = SolarSystem_getRace((SolarSystem *)(long)((Status *)(status))->getSystem());
        void *wanted = ArrayItem(allWanted, i);
        if (race == Wanted_getBoard(wanted)) {
            race = SolarSystem_getRace((SolarSystem *)(long)((Status *)(status))->getSystem());
            if (race != 0 || ((Status *)(status))->getCurrentCampaignMission() < 0x80) {
                race = SolarSystem_getRace((SolarSystem *)(long)((Status *)(status))->getSystem());
                if (race == 0 && ((Status *)(status))->getCurrentCampaignMission() >= 0xa2) {
                    ArrayAdd_Wanted(wanted, self->wantedList);
                }
            } else {
                ArrayAdd_Wanted(wanted, self->wantedList);
            }
        }
    }

    uint32_t h;
    uint32_t w;
    uint32_t halfW = 0;
    if (*g_WantedWindow_init_fullscreen == 0) {
        h = *g_WantedWindow_init_window_h;
        w = *g_WantedWindow_init_window_w;
        self->windowX = 0;
        self->windowY = 0;
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
        self->windowX = (*g_WantedWindow_init_screen_w >> 1) - halfW;
        self->windowY = (*g_WantedWindow_init_screen_h >> 1) - (h >> 1);
    }

    self->windowWidth = w;
    self->windowHeight = h;
    self->selectedWanted = 0;

    uint32_t selected = 0;
    for (;;) {
        void *arr = self->wantedList;
        uint32_t count = F<uint32_t>(arr, 0x0);
        if (selected >= count) {
            selected = self->selectedWanted;
            if (selected == count - 1) {
                if (((Wanted *)(ArrayItem(arr, selected)))->isActive() == 0) {
                    selected = 0;
                    self->selectedWanted = 0;
                } else {
                    selected = self->selectedWanted;
                }
            }
            break;
        }
        if (((Wanted *)(ArrayItem(arr, selected)))->isActive() != 0) {
            self->selectedWanted = selected;
            arr = self->wantedList;
            count = F<uint32_t>(arr, 0x0);
            if (selected == count - 1) {
                if (((Wanted *)(ArrayItem(arr, selected)))->isActive() == 0) {
                    selected = 0;
                    self->selectedWanted = 0;
                } else {
                    selected = self->selectedWanted;
                }
            }
            break;
        }
        ++selected;
    }

    self->highlightedWanted = selected;
    ((WantedWindow *)(self))->selectWanted(selected);

    void *buttons = ::operator new(0x0c);
    Array_TouchButton_ctor(buttons);
    self->buttons = buttons;
    ArraySetLength_TouchButton(2, buttons);

    void **textHolder = g_WantedWindow_init_text;
    void *button = ::operator new(0xc8);
    String *text = (String *)((GameText *)(*textHolder))->getText(0xc93);
    int x = self->windowX;
    int bw = self->windowWidth;
    int help = ((Layout *)(layout))->getHelpButtonOffset();
    // ctor6 header order is (x, y, text, p4, p5, p6); decomp call passed (text, kind, x, y, flags).
    ((TouchButton *)(button))->ctor6(x + bw - help, self->windowY, text, 3, 0x12, 0);
    *(void **)((char *)F<void *>(buttons, 0x4) + 4) = button;

    button = ::operator new(0xc8);
    text = (String *)((GameText *)(*textHolder))->getText(0x81);
    x = self->windowX;
    bw = self->windowWidth;
    help = ((Layout *)(layout))->getHelpButtonOffset();
    int width = ((TouchButton *)(*(void **)((char *)F<void *>(buttons, 0x4) + 4)))->getWidth();
    ((TouchButton *)(button))->ctor6(x + bw - help - width + F<int>(layout, 0x38), self->windowY, text, 3, 0x12, 0);
    *(void **)F<void *>(buttons, 0x4) = button;

    ((TouchButton *)(*(void **)((char *)F<void *>(buttons, 0x4) + 4)))->setAlwaysPressed(true);
    ((Layout *)(layout))->setWindowDimensions(self->windowX, self->windowY, self->windowWidth, self->windowHeight);

    layout = *layoutHolder;
    self->contentHeight = (F<int>(layout, 0x34) + F<int>(layout, 0x70)) *
                         F<int>(self->wantedList, 0x0);
    self->visibleHeight =
        (((((self->windowHeight - F<int>(layout, 0x10)) - F<int>(layout, 0xc)) -
           F<int>(layout, 0x20)) - F<int>(layout, 0x24)) - F<int>(layout, 0x5c)) +
        F<int>(layout, 0x2c);

    if (self->detailButton != 0) {
        ((TouchButton *)(self->detailButton))->dtor(); ::operator delete(self->detailButton);
    }
    self->starMap = 0;
    self->detailButton = 0;
    self->showingMap = 0;

    button = ::operator new(0xc8);
    text = (String *)((GameText *)(*textHolder))->getText(0x1a8);
    layout = *layoutHolder;
    TouchButton_ctor8(button, text, 0,
                      self->windowX + (self->windowWidth >> 1) + F<int>(layout, 0x2c),
                      (((self->windowY - F<int>(layout, 0x2c)) + self->windowHeight) -
                       F<int>(layout, 0x10)) - F<int>(layout, 0x24),
                      activeWidth, 0x21, 4);
    self->field_0x15 = 0;
    self->detailButton = button;
    self->field_0x0 = 1;
    return 1;
}

// ---- _WantedWindow_e0ec4.cpp ----
__attribute__((visibility("hidden"))) extern void (*g_WantedWindow_string_dtor)(void *);

WantedWindow *_ZN12WantedWindowD2Ev(WantedWindow *self)
{
    void *p = self->imageParts;
    if (p != 0) {
        ArrayReleaseClasses_ImagePart(p);
        p = self->imageParts;
        if (p != 0) {
            ::operator delete(Array_ImagePart_dtor(p));
        }
    }
    self->imageParts = 0;

    p = self->buttons;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        p = self->buttons;
        if (p != 0) {
            ::operator delete(Array_TouchButton_dtor(p));
        }
    }
    self->buttons = 0;

    p = self->detailButton;
    if (p != 0) {
        ((TouchButton *)(p))->dtor(); ::operator delete(p);
    }
    self->detailButton = 0;

    p = self->wantedList;
    if (p != 0) {
        ::operator delete(Array_Wanted_dtor(p));
    }
    self->wantedList = 0;

    p = self->mission;
    if (p != 0) {
        (*(void (**)(void *))(*(int *)p + 4))(p);
    }
    self->mission = 0;

    p = self->scrollWindow;
    if (p != 0) {
        ::operator delete(ScrollTouchWindow_dtor(p));
    }
    self->scrollWindow = 0;

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

WantedWindow *_ZN12WantedWindowC2Ev(WantedWindow *self)
{
    ((String *)((char *)self + 0x3c))->ctor();
    ((String *)((char *)self + 0x48))->ctor();
    ((String *)((char *)self + 0x54))->ctor();
    ((String *)((char *)self + 0x60))->ctor();
    ((String *)((char *)self + 0x6c))->ctor();
    ((String *)((char *)self + 0x78))->ctor();

    void **canvasHolder = g_WantedWindow_ctor_canvas;
    self->detailButton = 0;
    self->starMap = 0;
    self->imageParts = 0;
    unsigned int *fontHolder = *g_WantedWindow_ctor_font;
    int h = ((PaintCanvas *)*canvasHolder)->GetTextHeight(*fontHolder);
    self->wantedList = 0;
    self->mission = 0;
    self->scrollWindow = 0;
    self->buttons = 0;
    self->halfTextHeight = h / 2 - 1;
    ((PaintCanvas *)*canvasHolder)->Image2DCreate(0x454, (unsigned int *)((char *)self + 0xac));
    ((WantedWindow *)(self))->init();
    return self;
}

// ---- getRelativeScrollHeight_e17d8.cpp ----
float WantedWindow::getRelativeScrollHeight() {
    WantedWindow *self = this;
    long long pair = self->contentHeight;
    int content = (int)pair;
    int visible = (int)((unsigned long long)pair >> 32);
    if (content < visible) {
        union { uint32_t u; float f; } c;
        c.u = 0x4605e009u;
        return c.f;
    }
    int scroll = self->scrollOffset;
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
    return ArrayItem(self->wantedList, idx);
}

static inline void append_label(String *dst, const char *prefix, String *value)
{
    String a;
    String b;
    String_cstr_ctor(&a, prefix, false);
    String_plus(&b, &a, value);
    String_plusAssign(dst, &b);
    ((String *)(&b))->dtor();
    ((String *)(&a))->dtor();
}

void WantedWindow::selectWanted(int idx) {
    WantedWindow *self = this;
    String s34;
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;

    if (self->imageParts != 0) {
        ::operator delete(Array_ImagePart_dtor(self->imageParts));
    }
    self->imageParts = 0;

    if (self->scrollWindow != 0) {
        ::operator delete(ScrollTouchWindow_dtor(self->scrollWindow));
    }
    self->scrollWindow = 0;

    void *wanted = wanted_at(self, idx);
    self->imageParts = ((ImageFactory *)(*g_WantedWindow_select_factory))->loadChar((int *)Wanted_getImageParts(wanted));

    ((Wanted *)(&s34))->getName();
    ((String *)((String *)((char *)self + 0x54)))->assign(&s34);
    ((String *)(&s34))->dtor();

    if (((Wanted *)(wanted))->isActive() != 0) {
        void *galaxy = *g_WantedWindow_select_galaxy;
        void *last = (void *)(long)((Galaxy *)(galaxy))->getStation(Wanted_getLastSeen(wanted));
        void *travel = (void *)(long)((Galaxy *)(galaxy))->getStation(Wanted_getTravelsTo(wanted));
        void *current = (void *)(long)((Galaxy *)(galaxy))->getStation(Wanted_getCurrentLocation(wanted));

        ((Station *)(&s58))->getName();
        String_cstr_ctor(&s64, g_WantedWindow_s_from, false);
        String_plus(&s4c, &s58, &s64);
        ((SolarSystem *)(&s70))->getName();
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        ((String *)((String *)((char *)self + 0x3c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s7c))->dtor();
        ((String *)(&s40))->dtor();
        ((String *)(&s70))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s64))->dtor();
        ((String *)(&s58))->dtor();

        ((Station *)(&s58))->getName();
        String_cstr_ctor(&s64, g_WantedWindow_s_to, false);
        String_plus(&s4c, &s58, &s64);
        ((SolarSystem *)(&s70))->getName();
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        ((String *)((String *)((char *)self + 0x48)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s7c))->dtor();
        ((String *)(&s40))->dtor();
        ((String *)(&s70))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s64))->dtor();
        ((String *)(&s58))->dtor();

        ((Station *)(&s58))->getName();
        String_cstr_ctor(&s64, g_WantedWindow_s_at, false);
        String_plus(&s4c, &s58, &s64);
        ((SolarSystem *)(&s70))->getName();
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        ((String *)((String *)((char *)self + 0x6c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s7c))->dtor();
        ((String *)(&s40))->dtor();
        ((String *)(&s70))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s64))->dtor();
        ((String *)(&s58))->dtor();

        String_int_ctor(&s40, Wanted_getReward(wanted));
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        ((String *)((String *)((char *)self + 0x78)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s40))->dtor();

        if (last != 0) {
            ((Station *)(last))->dtor(); ::operator delete(last);
        }
        if (travel != 0) {
            ((Station *)(travel))->dtor(); ::operator delete(travel);
        }
        if (current != 0) {
            ((Station *)(current))->dtor(); ::operator delete(current);
        }
    } else if (((Wanted *)(wanted))->isTerminated() != 0) {
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_a, false);
        ((String *)((String *)((char *)self + 0x3c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_b, false);
        ((String *)((String *)((char *)self + 0x48)))->assign(&s34);
        ((String *)(&s34))->dtor();
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_c, false);
        ((String *)((String *)((char *)self + 0x6c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_d, false);
        ((String *)((String *)((char *)self + 0x78)))->assign(&s34);
        ((String *)(&s34))->dtor();
    } else {
        String *text = (String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d);
        ((String *)((String *)((char *)self + 0x3c)))->assign(text);
        text = (String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d);
        ((String *)((String *)((char *)self + 0x48)))->assign(text);
        String_int_ctor(&s40, Wanted_getReward(wanted));
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        ((String *)((String *)((char *)self + 0x78)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s40))->dtor();
        text = (String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d);
        ((String *)((String *)((char *)self + 0x6c)))->assign(text);
    }

    ((Wanted *)(wanted))->isTerminated();
    ((String *)((String *)((char *)self + 0x60)))->assign((String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d));

    void *layout = *g_WantedWindow_select_layout;
    int y = self->windowY;
    int h = self->windowHeight;
    int top = y + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
              F<int>(layout, 0x5c) + F<int>(layout, 0x2c);
    int height = (((((y - F<int>(layout, 0x2c)) + h) - top) -
                   F<int>(layout, 0x10)) - F<int>(layout, 0x2d8)) -
                 F<int>(layout, 0x24);
    if (((Wanted *)(wanted))->isActive() != 0) {
        height = (height - F<int>(layout, 0x4c)) - F<int>(layout, 0x30);
    }

    void *scroll = ::operator new(0x24);
    int pad = F<int>(layout, 0x2c);
    ScrollTouchWindow_ctor(scroll, self->windowX + (self->windowWidth >> 1) + pad,
                           F<int>(layout, 0x2d8) + pad + top,
                           ((self->windowWidth >> 1) - pad) - F<int>(layout, 0x28),
                           height, false);
    self->scrollWindow = scroll;

    String_cstr_ctor(&s34, g_WantedWindow_s_empty, false);
    ((String *)(&s34))->assign((String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d));
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
    ((String *)(&s94))->ctor_copy((String *)(&s34), false);
    ((ScrollTouchWindow *)(self->scrollWindow))->setText(s88, s94);
    ((String *)(&s94))->dtor();
    ((String *)(&s88))->dtor();
    ((String *)(&s34))->dtor();
}
