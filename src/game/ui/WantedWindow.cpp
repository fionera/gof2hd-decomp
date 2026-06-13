#include "gof2/game/ui/WantedWindow.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/ui/ScrollTouchWindow.h"
#include "gof2/game/world/StarMap.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/world/SolarSystem.h"   // defines the canonical global `struct RetStr` (12-byte String sret)
// Station.h, TouchButton.h and Wanted.h each redefine the identical (token-for-token,
// 12-byte aligned aggregate) global `struct RetStr` already provided by SolarSystem.h.
// Rename their duplicate definitions so they don't collide; the only RetStr-returning
// methods used here (getName/getText) have their results discarded, so the distinct-but-
// identical return type is harmless. (Those headers are out of this batch's edit scope.)
#define RetStr RetStr
#include "gof2/game/world/Station.h"
#undef RetStr
#define RetStr RetStr
#define B B_TouchButton
#define I I_TouchButton
#define P P_TouchButton
#include "gof2/game/ui/TouchButton.h"
#undef RetStr
#undef B
#undef I
#undef P
#define RetStr RetStr
#include "gof2/game/world/Wanted.h"
#undef RetStr
#include "gof2/game/core/String.h"
#include "gof2/engine/render/PaintCanvas.h"

extern "C" int __aeabi_idiv(int numerator, int denominator);
extern "C" void StarMap_OnTouchEnd_tail(void);
extern "C" void *Mission_ctor(void *mission, int a, int b, int dest);
extern "C" void *StarMap_ctor(void *map, bool jumpMapMode, void *mission, bool flag, int idx);
extern "C" int Station_getSystem(void *station);
extern void DisableClip();  // AbyssEngine::PaintCanvas::DisableClip (free fn, not methodized)
extern "C" void String_cstr_ctor(String *s, const char *text, bool copy);
extern "C" void String_plus(String *out, String *a, String *b);
extern "C" void *ScrollTouchWindow_dtor(void *window);
extern "C" void String_int_ctor(String *s, int value);
extern "C" void String_plusAssign(String *dst, String *src);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_move_layout;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_a;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_force;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_b;

int WantedWindow::OnTouchMove(int x, int y) {
    if (this->showingMap != 0) {
        ((StarMap *)(this->starMap))->OnTouchMove(x, y);
        return 0;
    }

    void **layoutHolder = g_WantedWindow_move_layout;
    void *layout = *layoutHolder;
    if (((F<int>(layout, 0xc) < y) &&
         (y < *g_WantedWindow_move_screen_h - F<int>(layout, 0x10)) &&
         (x < *g_WantedWindow_move_screen_w_a / 2)) ||
        (*g_WantedWindow_move_force != 0)) {
        int delta = y - this->lastDragY;
        this->dragDelta = delta;
        this->scrollDamping = 1.0f;
        this->scrollOffset += delta;
        this->lastDragY = y;
    }

    if (*g_WantedWindow_move_screen_w_b / 2 < x) {
        ((ScrollTouchWindow *)(this->scrollWindow))->OnTouchMove(x, y);
    }

    uint32_t i = 0;
    while (i < this->buttons->size()) {
        (*this->buttons)[i]->OnTouchMove(x, y);
        ++i;
    }

    ((Layout *)(*layoutHolder))->OnTouchMove(x, y);
    if (this->detailButton != 0) {
        Wanted *wanted = (*this->wantedList)[this->selectedWanted];
        if (((Wanted *)(wanted))->isActive() != 0) {
            ((TouchButton *)(this->detailButton))->OnTouchMove(x, y);
        }
    }

    uint32_t selected = ((WantedWindow *)(this))->getWantedAtPosition(x, y);
    int moved = y - this->touchStartY;
    if (moved < 0) {
        moved = -moved;
    }
    if (moved > 5) {
        selected = 0xffffffffu;
    }
    this->highlightedWanted = selected;
    return 0;
}

void WantedWindow::update(int dt) {
    if (this->showingMap != 0) {
        return this->update_tail(this->starMap, dt);
    }

    (*this->buttons)[1]->setAlwaysPressed(true);
    ((ScrollTouchWindow *)(this->scrollWindow))->update(dt);

    if (this->dragging == 0) {
        float speed = this->scrollDamping;
        float old = this->scrollVelocity;
        float delta = speed * old;
        float mag = -(speed * old);
        if (0.0f < delta) {
            mag = delta;
        }
        this->scrollVelocity = delta;
        if (mag > 1.0f) {
            this->scrollOffset = (int)(delta + (float)this->scrollOffset);
        }
    }

    int scroll = this->scrollOffset;
    if (scroll > 0) {
        this->scrollDamping = 1.0f;
        this->scrollVelocity = (float)-scroll * 0.5f;
    }

    int limit = this->visibleHeight - this->contentHeight;
    if (limit <= -1) {
        if (scroll < limit) {
            this->scrollDamping = 1.0f;
            this->scrollVelocity = (float)(limit - scroll) * 0.5f;
        }
    } else {
        this->scrollOffset = 0;
        this->scrollVelocity = 0;
    }
}

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_touch_layout;

int WantedWindow::OnTouchBegin(int x, int y) {
    if (this->showingMap != 0) {
        ((StarMap *)(this->starMap))->OnTouchBegin(x, y);
        return 0;
    }

    this->lastDragY = y;
    this->touchStartY = y;
    this->dragDelta = 0;
    this->dragging = 1;
    ((ScrollTouchWindow *)(this->scrollWindow))->OnTouchBegin(x, y);

    uint32_t i = 0;
    while (i < this->buttons->size()) {
        (*this->buttons)[i]->OnTouchBegin(x, y);
        ++i;
    }

    ((Layout *)(*g_WantedWindow_touch_layout))->OnTouchBegin(x, y);
    if (this->detailButton != 0) {
        Wanted *wanted = (*this->wantedList)[this->selectedWanted];
        if (((Wanted *)(wanted))->isActive() != 0) {
            ((TouchButton *)(this->detailButton))->OnTouchBegin(x, y);
        }
    }

    this->highlightedWanted = ((WantedWindow *)(this))->getWantedAtPosition(x, y);
    return 0;
}

void WantedWindow::render3D() {
    if (this->showingMap != 0) {
        return this->render3D_tail(this->starMap);
    }
}

float WantedWindow::getRelativeScrollStartPos() {
    int pos = this->scrollOffset;
    if (pos > 0) {
        union { uint32_t u; float f; } c;
        c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)pos / (float)this->contentHeight;
}

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_hit_layout;

uint32_t WantedWindow::getWantedAtPosition(int x, int y) {
    if (x >= this->windowX + (this->windowWidth >> 1)) {
        return 0xffffffffu;
    }

    Array<Wanted*> *list = this->wantedList;
    void *layout = *g_WantedWindow_hit_layout;
    int numerator = y - this->windowY;
    numerator -= F<int>(layout, 0xc);
    numerator -= F<int>(layout, 0x20);
    numerator -= F<int>(layout, 0x5c);
    numerator -= this->scrollOffset;
    int idx = __aeabi_idiv(numerator, F<int>(layout, 0x70) + F<int>(layout, 0x34));
    if ((uint32_t)idx > (uint32_t)((int)list->size() - 1)) {
        return 0xffffffffu;
    }
    return (uint32_t)idx;
}

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
    String help;

    if (this->showingMap != 0) {
        if ((((StarMap *)(this->starMap))->OnTouchEnd(x, y), true)) {
            uint32_t h;
            uint32_t w;
            uint32_t halfW = 0;
            if (*g_WantedWindow_end_fullscreen == 0) {
                h = *g_WantedWindow_end_window_h;
                w = *g_WantedWindow_end_window_w;
                this->windowX = 0;
                this->windowY = 0;
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
                this->windowX = (*g_WantedWindow_end_screen_w >> 1) - halfW;
                this->windowY = (*g_WantedWindow_end_screen_h >> 1) - (h >> 1);
            }
            this->windowWidth = w;
            this->windowHeight = h;
            this->showingMap = 0;
        }
    } else {
        int delta = this->dragDelta;
        int pos = this->scrollOffset + delta;
        float velocity = 0.0f;
        int mag = delta;
        if (mag < 0) {
            mag = -mag;
        }
        if (mag > 3) {
            velocity = (float)delta;
        }
        this->scrollDamping = 0.95f;
        this->dragging = 0;
        this->scrollOffset = pos;
        this->scrollOffsetSnapshot = pos;
        this->scrollVelocity = velocity;

        ((ScrollTouchWindow *)(this->scrollWindow))->OnTouchEnd(x, y);
        for (uint32_t i = 0; i < this->buttons->size(); ++i) {
            if ((*this->buttons)[i]->OnTouchEnd(x, y) != 0) {
                this->field_0x0 = i;
            }
        }

        if (this->highlightedWanted >= 0) {
            int idx = ((WantedWindow *)(this))->getWantedAtPosition(x, y);
            this->selectedWanted = idx;
            this->highlightedWanted = idx;
            ((WantedWindow *)(this))->selectWanted(idx);
        }

        bool openMap = false;
        if (this->detailButton != 0) {
            Wanted *wanted = (*this->wantedList)[this->selectedWanted];
            if (((Wanted *)(wanted))->isActive() != 0 &&
                ((TouchButton *)(this->detailButton))->OnTouchEnd(x, y) != 0) {
                openMap = true;
            }
        }

        if (openMap) {
            void **appHolder = g_WantedWindow_end_app;
            void *module = ((ApplicationManager *)(*appHolder))->GetApplicationModule(5);
            this->starMap = F<void *>(module, 0x10);
            Wanted *wanted = (*this->wantedList)[this->selectedWanted];
            int lastSeen = ((Wanted *)(wanted))->getLastSeen();
            void *station = (void *)(long)((Galaxy *)(*g_WantedWindow_end_galaxy))->getStation(lastSeen);
            void *oldMission = this->mission;
            if (oldMission != 0) {
                (*(void (**)(void *))(*(int *)oldMission + 4))(oldMission);
            }
            this->mission = 0;

            void *mission = ::operator new(0x78);
            Mission_ctor(mission, 0, 0, ((Wanted *)(wanted))->getTravelsTo());
            this->mission = mission;

            void *map = this->starMap;
            if (map == 0) {
                map = ::operator new(0x1e8);
                StarMap_ctor(map, true, mission, false, -1);
                ((ApplicationManager *)(*appHolder))->GetApplicationModule(5);
                F<void *>(((ApplicationManager *)(*appHolder))->GetApplicationModule(5), 0x10) = map;
                map = F<void *>(((ApplicationManager *)(*appHolder))->GetApplicationModule(5), 0x10);
                this->starMap = map;
            } else {
                ((StarMap *)(map))->init(true, (Mission *)mission, false, -1);
            }

            int system = Station_getSystem(station);
            lastSeen = ((Wanted *)(wanted))->getLastSeen();
            ((StarMap *)(map))->setStart(system, lastSeen);
            if (station != 0) {
                ((Station *)(station))->dtor(); ::operator delete(station);
            }
            this->showingMap = 1;
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

static inline Wanted *draw_wanted_at(WantedWindow *self, int idx)
{
    return (*self->wantedList)[idx];
}

void WantedWindow::draw() {
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;
    String sa0;

    if (this->showingMap != 0) {
        this->draw_tail(this->starMap);
        return;
    }

    void *layout = *g_WantedWindow_draw_layout;
    void *canvas = *g_WantedWindow_draw_canvas;
    void *font = *g_WantedWindow_draw_font;

    ((PaintCanvas *)canvas)->EnableClip(this->windowX,
                           this->windowY + F<int>(layout, 0xc) +
                               F<int>(layout, 0x20) + F<int>(layout, 0x5c),
                           this->windowWidth,
                           F<int>(layout, 0x2c) + this->visibleHeight);

    float relStart = ((WantedWindow *)(this))->getRelativeScrollStartPos();
    int visible = this->visibleHeight;
    float visf = (float)visible;
    float relHeight = ((WantedWindow *)(this))->getRelativeScrollHeight();
    int barSize = (int)(relHeight * visf);
    int barStart = (int)(relStart * visf);
    if (barSize >= 1 || barStart >= 0) {
        ((Layout *)(layout))->drawScrollBar(((this->windowX + (this->windowWidth >> 1)) -
                              F<int>(layout, 0x2c)) - F<int>(layout, 0x48), this->windowY + F<int>(layout, 0x2c) +
                                 F<int>(layout, 0xc) + F<int>(layout, 0x20) +
                                 F<int>(layout, 0x5c), visible, barStart, barSize);
    }

    int y = this->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
            F<int>(layout, 0x5c) + F<int>(layout, 0x2c) + this->scrollOffset;
    int inset = barSize < 1 ? 0 : F<int>(layout, 0x2c) + F<int>(layout, 0x48);

    for (uint32_t i = 0; i < this->wantedList->size(); ++i) {
        int style = (i == this->selectedWanted || i == this->highlightedWanted) ? 4 : 3;
        String_cstr_ctor(&s40, g_WantedWindow_draw_empty_a, false);
        ((Layout *)(layout))->drawBox(style, F<int>(layout, 0x28) + this->windowX, y, (this->windowWidth >> 1) -
                           (inset + F<int>(layout, 0x28) + F<int>(layout, 0x2c)), F<int>(layout, 0x70), &s40, 0);
        ((String *)(&s40))->dtor();

        Wanted *wanted = draw_wanted_at(this, i);
        ((PaintCanvas *)canvas)->SetColor(((Wanted *)(wanted))->isActive() ? 0xffffffffu : 0xff808080u);
        ((Wanted *)(&s4c))->getName();
        int textY = y + F<int>(layout, 0x70) / 2 -
                    ((PaintCanvas *)canvas)->GetTextHeight((unsigned int)(unsigned long)font) / 2;
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&s4c,
                               this->windowX + F<int>(layout, 0x28) +
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
            ((PaintCanvas *)canvas)->DrawImage2D(this->bgImage,
                                    this->windowX + F<int>(layout, 0x28) +
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

    for (uint32_t i = 0; i < this->buttons->size(); ++i) {
        (*this->buttons)[i]->draw();
    }

    ((String *)(&s7c))->ctor_copy((String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc95), false);
    ((Layout *)(layout))->drawBox(1, F<int>(layout, 0x28) + this->windowX, this->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20), (this->windowWidth >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)), F<int>(layout, 0x5c), &s7c, 0);
    ((String *)(&s7c))->dtor();

    String_cstr_ctor(&s88, g_WantedWindow_draw_empty_b, false);
    ((Layout *)(layout))->drawBox(5, F<int>(layout, 0x28) + this->windowX, this->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20) +
                       F<int>(layout, 0x5c) + F<int>(layout, 0x2c), (this->windowWidth >> 1) - (F<int>(layout, 0x2c) + F<int>(layout, 0x28)), ((this->windowHeight -
                     (F<int>(layout, 0x20) + F<int>(layout, 0xc) +
                      F<int>(layout, 0x5c) + F<int>(layout, 0x2c) * 2)) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24), &s88, 0);
    ((String *)(&s88))->dtor();

    ((String *)(&s94))->ctor_copy((String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc95), false);
    ((Layout *)(layout))->drawBox(1, this->windowX + (this->windowWidth >> 1) +
                                  F<int>(layout, 0x2c), this->windowY + F<int>(layout, 0xc) + F<int>(layout, 0x20), ((this->windowWidth >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28), F<int>(layout, 0x5c), &s94, 0);
    ((String *)(&s94))->dtor();

    String_cstr_ctor(&sa0, g_WantedWindow_draw_empty_c, false);
    ((Layout *)(layout))->drawBox(5, this->windowX + (this->windowWidth >> 1) +
                                  F<int>(layout, 0x2c), this->windowY + F<int>(layout, 0x2c) + F<int>(layout, 0xc) +
                       F<int>(layout, 0x20) + F<int>(layout, 0x5c), ((this->windowWidth >> 1) - F<int>(layout, 0x2c)) - F<int>(layout, 0x28), ((this->windowHeight -
                     (F<int>(layout, 0xc) + F<int>(layout, 0x2c) * 2 +
                      F<int>(layout, 0x20) + F<int>(layout, 0x5c))) -
                    F<int>(layout, 0x10)) -
                       F<int>(layout, 0x24), &sa0, 0);
    ((String *)(&sa0))->dtor();

    if (this->imageParts != 0) {
        int charX = this->windowX + (this->windowWidth >> 1) + F<int>(layout, 0x2c);
        int charY = F<int>(layout, 0x5c) + this->windowY + F<int>(layout, 0x2c) +
                    F<int>(layout, 0xc) + F<int>(layout, 0x20);
        ((ImageFactory *)(*g_WantedWindow_draw_factory))->drawChar((Array<ImagePart *> *)this->imageParts, charX, charY, false);
        int textX = F<int>(layout, 0x2d4) + charX + F<int>(layout, 0x2c);
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)((char *)this + 0x54), textX, charY, false);

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_a, false);
        String_plus(&s58, &s64, (String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc93));
        String_plus(&s4c, &s58, (String *)((char *)this + 0x3c));
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&s4c, textX, charY + F<int>(layout, 0x4) * 2, false);
        ((String *)(&s4c))->dtor();
        ((String *)(&s58))->dtor();
        ((String *)(&s64))->dtor();

        String_cstr_ctor(&s64, g_WantedWindow_draw_label_b, false);
        String_plus(&s58, &s64, (String *)((GameText *)(*g_WantedWindow_draw_text))->getText(0xc93));
        String_plus(&s4c, &s58, (String *)((char *)this + 0x48));
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(unsigned long)font, (void *)&s4c, textX, charY + F<int>(layout, 0x4) * 3, false);
        ((String *)(&s4c))->dtor();
        ((String *)(&s58))->dtor();
        ((String *)(&s64))->dtor();

        ((ScrollTouchWindow *)(this->scrollWindow))->draw();
    }

    if (this->detailButton != 0 &&
        ((Wanted *)(draw_wanted_at(this, this->selectedWanted)))->isActive() != 0) {
        ((TouchButton *)(this->detailButton))->draw();
    }

    ((Layout *)(layout))->drawFooter();
}

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
    v4si zero = {0, 0, 0, 0};
    this->field_0x91 = zero;
    // Original cleared a 16-byte vector starting at +0x84 (the scroll-state ints).
    this->scrollOffset = 0;
    this->lastDragY = 0;
    this->scrollOffsetSnapshot = 0;
    this->dragDelta = 0;

    this->wantedList = new Array<Wanted*>();

    void *status = *g_WantedWindow_init_status;
    Array<Wanted*> *allWanted = ((Status *)status)->getWanted();
    void **layoutHolder = g_WantedWindow_init_layout;
    void *layout = *layoutHolder;
    int wBase = F<int>(layout, 0x28);
    int wLimit = F<int>(layout, 0xcc);
    int wExtra = F<int>(layout, 0x4c);
    int activeWidth = wLimit - wBase - wExtra;

    for (uint32_t i = 0; i < allWanted->size(); ++i) {
        int race = ((SolarSystem *)((SolarSystem *)(long)((Status *)(status))->getSystem()))->getRace();
        Wanted *wanted = (*allWanted)[i];
        if (race == ((Wanted *)(wanted))->getBoard()) {
            race = ((SolarSystem *)((SolarSystem *)(long)((Status *)(status))->getSystem()))->getRace();
            if (race != 0 || ((Status *)(status))->getCurrentCampaignMission() < 0x80) {
                race = ((SolarSystem *)((SolarSystem *)(long)((Status *)(status))->getSystem()))->getRace();
                if (race == 0 && ((Status *)(status))->getCurrentCampaignMission() >= 0xa2) {
                    this->wantedList->push_back(wanted);
                }
            } else {
                this->wantedList->push_back(wanted);
            }
        }
    }

    uint32_t h;
    uint32_t w;
    uint32_t halfW = 0;
    if (*g_WantedWindow_init_fullscreen == 0) {
        h = *g_WantedWindow_init_window_h;
        w = *g_WantedWindow_init_window_w;
        this->windowX = 0;
        this->windowY = 0;
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
        this->windowX = (*g_WantedWindow_init_screen_w >> 1) - halfW;
        this->windowY = (*g_WantedWindow_init_screen_h >> 1) - (h >> 1);
    }

    this->windowWidth = w;
    this->windowHeight = h;
    this->selectedWanted = 0;

    uint32_t selected = 0;
    for (;;) {
        Array<Wanted*> *arr = this->wantedList;
        uint32_t count = arr->size();
        if (selected >= count) {
            selected = this->selectedWanted;
            if (selected == count - 1) {
                if (((Wanted *)((*arr)[selected]))->isActive() == 0) {
                    selected = 0;
                    this->selectedWanted = 0;
                } else {
                    selected = this->selectedWanted;
                }
            }
            break;
        }
        if (((Wanted *)((*arr)[selected]))->isActive() != 0) {
            this->selectedWanted = selected;
            arr = this->wantedList;
            count = arr->size();
            if (selected == count - 1) {
                if (((Wanted *)((*arr)[selected]))->isActive() == 0) {
                    selected = 0;
                    this->selectedWanted = 0;
                } else {
                    selected = this->selectedWanted;
                }
            }
            break;
        }
        ++selected;
    }

    this->highlightedWanted = selected;
    ((WantedWindow *)(this))->selectWanted(selected);

    Array<TouchButton*> *buttons = new Array<TouchButton*>();
    this->buttons = buttons;
    buttons->resize(2);

    void **textHolder = g_WantedWindow_init_text;
    TouchButton *button = (TouchButton *)::operator new(0xc8);
    String *text = (String *)((GameText *)(*textHolder))->getText(0xc93);
    int x = this->windowX;
    int bw = this->windowWidth;
    int help = ((Layout *)(layout))->getHelpButtonOffset();
    // ctor6 header order is (x, y, text, p4, p5, p6); decomp call passed (text, kind, x, y, flags).
    ((TouchButton *)(button))->ctor6(x + bw - help, this->windowY, text, 3, 0x12, 0);
    (*buttons)[1] = button;

    button = (TouchButton *)::operator new(0xc8);
    text = (String *)((GameText *)(*textHolder))->getText(0x81);
    x = this->windowX;
    bw = this->windowWidth;
    help = ((Layout *)(layout))->getHelpButtonOffset();
    int width = ((TouchButton *)((*buttons)[1]))->getWidth();
    ((TouchButton *)(button))->ctor6(x + bw - help - width + F<int>(layout, 0x38), this->windowY, text, 3, 0x12, 0);
    (*buttons)[0] = button;

    ((TouchButton *)((*buttons)[1]))->setAlwaysPressed(true);
    ((Layout *)(layout))->setWindowDimensions(this->windowX, this->windowY, this->windowWidth, this->windowHeight);

    layout = *layoutHolder;
    this->contentHeight = (F<int>(layout, 0x34) + F<int>(layout, 0x70)) *
                         (int)this->wantedList->size();
    this->visibleHeight =
        (((((this->windowHeight - F<int>(layout, 0x10)) - F<int>(layout, 0xc)) -
           F<int>(layout, 0x20)) - F<int>(layout, 0x24)) - F<int>(layout, 0x5c)) +
        F<int>(layout, 0x2c);

    if (this->detailButton != 0) {
        ((TouchButton *)(this->detailButton))->dtor(); ::operator delete(this->detailButton);
    }
    this->starMap = 0;
    this->detailButton = 0;
    this->showingMap = 0;

    button = (TouchButton *)::operator new(0xc8);
    text = (String *)((GameText *)(*textHolder))->getText(0x1a8);
    layout = *layoutHolder;
    ((TouchButton *)(button))->ctor8(text, 0, this->windowX + (this->windowWidth >> 1) + F<int>(layout, 0x2c), (((this->windowY - F<int>(layout, 0x2c)) + this->windowHeight) -
                       F<int>(layout, 0x10)) - F<int>(layout, 0x24), activeWidth, 0x21, 4);
    this->field_0x15 = 0;
    this->detailButton = button;
    this->field_0x0 = 1;
    return 1;
}

__attribute__((visibility("hidden"))) extern void (*g_WantedWindow_string_dtor)(void *);

WantedWindow *_ZN12WantedWindowD2Ev(WantedWindow *self)
{
    if (self->imageParts != 0) {
        for (ImagePart *part : *self->imageParts) delete part;
        self->imageParts->clear();
        delete self->imageParts;
    }
    self->imageParts = 0;

    if (self->buttons != 0) {
        for (TouchButton *btn : *self->buttons) delete btn;
        self->buttons->clear();
        delete self->buttons;
    }
    self->buttons = 0;

    void *p = self->detailButton;
    if (p != 0) {
        ((TouchButton *)(p))->dtor(); ::operator delete(p);
    }
    self->detailButton = 0;

    delete self->wantedList;
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

// Real special members. The shipped binary emitted the base ctor/dtor as the
// mangled free functions above (_ZN12WantedWindowC2Ev / _ZN12WantedWindowD2Ev);
// these forward to that already-recovered logic so call sites can use the natural
// C++ special members while the byte-exact field handling lives in one place.
WantedWindow::WantedWindow() {
    _ZN12WantedWindowC2Ev(this);
}

WantedWindow::~WantedWindow() {
    _ZN12WantedWindowD2Ev(this);
}

float WantedWindow::getRelativeScrollHeight() {
    long long pair = this->contentHeight;
    int content = (int)pair;
    int visible = (int)((unsigned long long)pair >> 32);
    if (content < visible) {
        union { uint32_t u; float f; } c;
        c.u = 0x4605e009u;
        return c.f;
    }
    int scroll = this->scrollOffset;
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

static inline Wanted *wanted_at(WantedWindow *self, int idx)
{
    return (*self->wantedList)[idx];
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
    String s34;
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;

    if (this->imageParts != 0) {
        delete this->imageParts;
    }
    this->imageParts = 0;

    if (this->scrollWindow != 0) {
        ::operator delete(ScrollTouchWindow_dtor(this->scrollWindow));
    }
    this->scrollWindow = 0;

    Wanted *wanted = wanted_at(this, idx);
    this->imageParts = ((ImageFactory *)(*g_WantedWindow_select_factory))->loadChar((int *)((Wanted *)(wanted))->getImageParts());

    ((Wanted *)(&s34))->getName();
    ((String *)((String *)((char *)this + 0x54)))->assign(&s34);
    ((String *)(&s34))->dtor();

    if (((Wanted *)(wanted))->isActive() != 0) {
        void *galaxy = *g_WantedWindow_select_galaxy;
        void *last = (void *)(long)((Galaxy *)(galaxy))->getStation(((Wanted *)(wanted))->getLastSeen());
        void *travel = (void *)(long)((Galaxy *)(galaxy))->getStation(((Wanted *)(wanted))->getTravelsTo());
        void *current = (void *)(long)((Galaxy *)(galaxy))->getStation(((Wanted *)(wanted))->getCurrentLocation());

        ((Station *)(&s58))->getName();
        String_cstr_ctor(&s64, g_WantedWindow_s_from, false);
        String_plus(&s4c, &s58, &s64);
        ((SolarSystem *)(&s70))->getName();
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        ((String *)((String *)((char *)this + 0x3c)))->assign(&s34);
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
        ((String *)((String *)((char *)this + 0x48)))->assign(&s34);
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
        ((String *)((String *)((char *)this + 0x6c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s7c))->dtor();
        ((String *)(&s40))->dtor();
        ((String *)(&s70))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s64))->dtor();
        ((String *)(&s58))->dtor();

        String_int_ctor(&s40, ((Wanted *)(wanted))->getReward());
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        ((String *)((String *)((char *)this + 0x78)))->assign(&s34);
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
        ((String *)((String *)((char *)this + 0x3c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_b, false);
        ((String *)((String *)((char *)this + 0x48)))->assign(&s34);
        ((String *)(&s34))->dtor();
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_c, false);
        ((String *)((String *)((char *)this + 0x6c)))->assign(&s34);
        ((String *)(&s34))->dtor();
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_d, false);
        ((String *)((String *)((char *)this + 0x78)))->assign(&s34);
        ((String *)(&s34))->dtor();
    } else {
        String *text = (String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d);
        ((String *)((String *)((char *)this + 0x3c)))->assign(text);
        text = (String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d);
        ((String *)((String *)((char *)this + 0x48)))->assign(text);
        String_int_ctor(&s40, ((Wanted *)(wanted))->getReward());
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        ((String *)((String *)((char *)this + 0x78)))->assign(&s34);
        ((String *)(&s34))->dtor();
        ((String *)(&s4c))->dtor();
        ((String *)(&s40))->dtor();
        text = (String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d);
        ((String *)((String *)((char *)this + 0x6c)))->assign(text);
    }

    ((Wanted *)(wanted))->isTerminated();
    ((String *)((String *)((char *)this + 0x60)))->assign((String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d));

    void *layout = *g_WantedWindow_select_layout;
    int y = this->windowY;
    int h = this->windowHeight;
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
    ScrollTouchWindow_ctor(scroll, this->windowX + (this->windowWidth >> 1) + pad,
                           F<int>(layout, 0x2d8) + pad + top,
                           ((this->windowWidth >> 1) - pad) - F<int>(layout, 0x28),
                           height, false);
    this->scrollWindow = scroll;

    String_cstr_ctor(&s34, g_WantedWindow_s_empty, false);
    ((String *)(&s34))->assign((String *)((GameText *)(*g_WantedWindow_select_text_a))->getText(0xc9d));
    append_label(&s34, g_WantedWindow_s_line_a,
                 (String *)((char *)this + 0x3c));
    append_label(&s34, g_WantedWindow_s_line_b,
                 (String *)((char *)this + 0x48));
    append_label(&s34, g_WantedWindow_s_line_c,
                 (String *)((char *)this + 0x6c));
    ((Wanted *)(wanted))->getIndex();
    append_label(&s34, g_WantedWindow_s_line_d,
                 (String *)((char *)this + 0x78));

    String_cstr_ctor(&s88, g_WantedWindow_s_empty, false);
    ((String *)(&s94))->ctor_copy((String *)(&s34), false);
    ((ScrollTouchWindow *)(this->scrollWindow))->setText(s88, s94);
    ((String *)(&s94))->dtor();
    ((String *)(&s88))->dtor();
    ((String *)(&s34))->dtor();
}

// ---- star-map tail forwarders ----
// When showingMap is set, update()/render3D()/draw() tail-call straight into the
// embedded StarMap object instead of doing the bounty-board work.
void WantedWindow::update_tail(void *starMap, int dt) {
    ((StarMap *)starMap)->update(dt);
}

void WantedWindow::render3D_tail(void *starMap) {
    ((StarMap *)starMap)->render();
}

void WantedWindow::draw_tail(void *starMap) {
    ((StarMap *)starMap)->draw();
}
