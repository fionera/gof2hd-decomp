#include "gof2/StatusWindow.h"
#include "gof2/Ship.h"
#include "gof2/BluePrint.h"
#include "gof2/PaintCanvas.h"
#include "gof2/Status.h"
#include "gof2/Achievements.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/Standing.h"
#include "gof2/String.h"
#include "gof2/TouchButton.h"
// Layout is the real gof2/Layout.h type (it self-compiles and co-includes with common.h now);
// StatusWindow drives it through its real methods (OnTouchMove/OnTouchEnd/draw*/helpPressed/...)
// and byte-offset field reads.


extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *p);
extern "C" void ArrayReleaseClasses_ImagePart(void *arr);
extern "C" void *Array_ImagePart_dtor(void *p);
extern "C" void ArrayReleaseClasses_String(void *arr);
extern "C" void *Array_String_dtor(void *p);
extern "C" __attribute__((visibility("hidden"))) void **g_SWm_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_SWm_height;
extern "C" __attribute__((visibility("hidden"))) int *g_SWm_force;
extern "C" __attribute__((visibility("hidden"))) unsigned char *g_SWm_btnFlag;
extern "C" __attribute__((visibility("hidden"))) void **g_SWm_ach;
extern "C" __attribute__((visibility("hidden"))) void **g_StatusWindow_layout;
extern "C" __attribute__((visibility("hidden"))) unsigned char *g_StatusWindow_btnFlag;
extern "C" __attribute__((visibility("hidden"))) void **g_StatusWindow_ach;

// ---- _StatusWindow_158164.cpp ----
// StatusWindow::~StatusWindow() -> returns this. Tears down the 4 owned Arrays.
StatusWindow *_ZN12StatusWindowD2Ev(StatusWindow *self)
{
    void *p = self->tabButtons;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->tabButtons;
        if (q != 0) ::operator delete(Array_TouchButton_dtor(q));
    }
    self->tabButtons = 0;
    p = self->medalButtons;
    if (p != 0) {
        ArrayReleaseClasses_TouchButton(p);
        void *q = self->medalButtons;
        if (q != 0) ::operator delete(Array_TouchButton_dtor(q));
    }
    self->medalButtons = 0;
    p = self->field_0xc;
    if (p != 0) {
        ArrayReleaseClasses_ImagePart(p);
        void *q = self->field_0xc;
        if (q != 0) ::operator delete(Array_ImagePart_dtor(q));
    }
    self->field_0xc = 0;
    p = self->detailLines;
    if (p != 0) {
        ArrayReleaseClasses_String(p);
        void *q = self->detailLines;
        if (q != 0) ::operator delete(Array_String_dtor(q));
    }
    self->detailLines = 0;
    return self;
}

// ---- OnTouchMove_15a0bc.cpp ----
// StatusWindow::OnTouchMove(int, int)
int StatusWindow::OnTouchMove(int param_1, int param_2) {
    StatusWindow *self = this;
    void **lh = g_SWm_layout;
    Layout *layout = (Layout *)*lh;
    if ((i32(layout, 0xc) < param_2 && param_2 < *g_SWm_height - i32(layout, 0x10)) || *g_SWm_force != 0) {
        int d = param_2 - i32(self, 0x3c);
        i32(self, 0x44) = d;
        u32(self, 0x48) = 0x3f800000u;
        i32(self, 0x38) += d;
        i32(self, 0x3c) = param_2;
        if (i32(self, 0x34) >= 0) {
            int e = i32(self, 0x50) - param_2;
            if (e < 0) e = -e;
            if (e > 3) {
                ((TouchButton *)(self->medalButtons->data[i32(self, 0x34)]))->setAlwaysPressed(0);
                i32(self, 0x34) = -1;
                layout = (Layout *)*lh;
            }
        }
    }
    ((Layout *)(layout))->OnTouchMove(param_1, param_2);
    if (*g_SWm_btnFlag == 0) {
        for (unsigned i = 0; i < self->tabButtons->size; ++i)
            ((TouchButton *)(self->tabButtons->data[i]))->OnTouchMove(param_1, param_2);
    }
    if (i32(self, 0x30) == 1) {
        void **holder = g_SWm_ach;
        int *medals = (int *)((Achievements *)(*holder))->getMedals();
        for (int i = 0; i < i32(self, 0x0); ++i) {
            if (medals[i] != 0 || ((Achievements *)(*holder))->isEliteMedal(i) != 0)
                ((TouchButton *)(self->medalButtons->data[i]))->OnTouchMove(param_1, param_2);
        }
    }
    return 0;
}

// ---- getRelativeScrollStartPos_1581d0.cpp ----
// StatusWindow::getRelativeScrollStartPos() -> 0 if scroll range (this+0x38) > 0,
// else -(float)range / (float)(this+0x58).
float StatusWindow::getRelativeScrollStartPos() {
    StatusWindow *self = this;
    int range = i32(self, 0x38);
    if (range > 0) return 0.0f;
    return -(float)range / (float)i32(self, 0x58);
}

// ---- OnTouchEnd_15a1c4.cpp ----
extern "C" {


void String_fromC(void *s, const char *text, bool copy);
void String_fromText(void *s, void *text, bool copy);
void String_appendAssign(void *self, void *rhs);





void ArrayStr_ctor(void *self);
void ArrayStr_releaseClasses(void *self);
void *ArrayStr_dtor(void *self);

void Status_replaceHash(void *out, void *key, void *a, void *b);

void Globals_getLineArray(void *globals, void *font, void *text, void *outArr);

void StatusWindow_getMedalHintText(void *out, int medalIndex);

extern void *g_swe_layout;        // *(DAT_16a49c): Layout singleton
extern char *g_swe_dialogBlock;   // *(DAT_16a4a0): "is a dialog open" flag
extern void *g_swe_achievements;  // *(DAT_16a4a4): achievements root
extern void *g_swe_status;        // *(DAT_16a4a8): status (replaceHash key source)
extern void *g_swe_gameText;      // GameText id source
extern void *g_swe_globals;       // *(DAT_16a4b4): globals
extern void *g_swe_font;          // *(DAT_16a4b8): font
}

// StatusWindow::OnTouchEnd(int x, int y)
void StatusWindow::OnTouchEnd(int x, int y) {
    StatusWindow *self = this;

    // Release scroll inertia: snap velocity, clamp scroll position.
    int vy = i32(self, 0x44);
    int newOff = i32(self, 0x38) + vy;
    float vf = (float)vy;
    int absvy = vy < 0 ? -vy : vy;
    Layout *layout = (Layout *)*(void **)g_swe_layout;

    f32(self, 0x48) = 1.0f;                       // DAT_16a494 == 1.0
    self->isDragging = 0;
    i32(self, 0x38) = newOff;
    i32(self, 0x40) = newOff;
    f32(self, 0x4c) = (absvy > 3) ? vf : 0.0f;    // DAT_16a490 == 0.0

    if (((Layout *)(layout))->OnTouchEnd(x, y) != 0)
        goto done;

    // Tab buttons (only when no modal dialog is open).
    if (*g_swe_dialogBlock == 0) {
        void **tabs = (void **)pp(self, 0x4);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++) {
            if (((TouchButton *)(((void **)tabs[1])[i]))->OnTouchEnd(x, y) != 0) {
                u32(self, 0x30) = i;
                i32(self, 0x58) = *(int *)((char *)pp(self, 0x68) + i * 4);
                i32(self, 0x38) = 0;
            }
        }
    }

    // Medal grid (only on the achievements tab, index 1).
    if (i32(self, 0x30) == 1) {
        for (int i = 0; i < i32(self, 0x0); i++) {
            if (((TouchButton *)(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4)))->OnTouchEnd(x, y) != 0) {
                void *ach = *(void **)g_swe_achievements;
                int *medals = ((Achievements *)(ach))->getMedals();
                int elite = ((Achievements *)(ach))->isEliteMedal(i);
                if (elite != 0 || medals[i] != 0) {
                    if (i32(self, 0x34) >= 0) {
                        ((TouchButton *)(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i32(self, 0x34) * 4)))->setAlwaysPressed(false);
                    }
                    i32(self, 0x34) = i;

                    // Rebuild the detail-text line array at +0x10.
                    if (pp(self, 0x10) != 0) {
                        ArrayStr_releaseClasses(pp(self, 0x10));
                        if (pp(self, 0x10) != 0)
                            ::operator delete(ArrayStr_dtor(pp(self, 0x10)));
                    }
                    pp(self, 0x10) = 0;
                    void *lineArr = ::operator new(0xc);
                    ArrayStr_ctor(lineArr);
                    pp(self, 0x10) = lineArr;

                    int count = (elite == 0) ? medals[i32(self, 0x34)] : 1;

                    char hdr[0xc], hdrTxt[0xc], valStr[0xc], valTmp[0xc], full[0xc], hint[0xc];
                    void *key = *(void **)g_swe_status;
                    void *t = ((GameText *)(*(void **)g_swe_gameText))->getText(i32(self, 0x34) + 0x610);
                    String_fromText(hdr, t, false);
                    void *val = (void *)(__INTPTR_TYPE__)((Achievements *)(ach))->getValue(i32(self, 0x34), count);
                    String_fromText(valStr, val, false);
                    String_fromText(valTmp, valStr, false);
                    Status_replaceHash(full, key, hdr, valTmp);
                    ((String *)(valTmp))->dtor();
                    ((String *)(valStr))->dtor();
                    ((String *)(hdr))->dtor();

                    StatusWindow_getMedalHintText(hint, i32(self, 0x34));
                    String_appendAssign(full, hint);
                    ((String *)(hint))->dtor();

                    Globals_getLineArray(*(void **)g_swe_globals, *(void **)g_swe_font, full,
                                         (void *)(i32(self, 0x6c) - layout->field_0x4c * 2));
                    ((TouchButton *)(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4)))->setAlwaysPressed(true);
                    ((String *)(full))->dtor();
                    (void)hdrTxt;
                }
                break;
            }
        }
    }

    // Help button -> contextual help window for the current tab.
    if (((Layout *)(layout))->helpPressed() != 0) {
        if (i32(self, 0x30) == 1) {
            char title[0xc];
            void *t = ((GameText *)(*(void **)g_swe_gameText))->getText(0x287);
            String_fromText(title, t, false);
            ((Layout *)(layout))->initHelpWindow(title);
            ((String *)(title))->dtor();
        } else if (i32(self, 0x30) == 0) {
            char title[0xc];
            void *t = ((GameText *)(*(void **)g_swe_gameText))->getText(0x280);
            String_fromText(title, t, false);
            ((Layout *)(layout))->initHelpWindow(title);
            ((String *)(title))->dtor();
        }
    }

done:
    
}

// ---- OnTouchBegin_15a010.cpp ----
// *g_layout -> Layout pointer (double load). *g_btnFlag -> byte flag. g_ach -> Achievements**.

// StatusWindow::OnTouchBegin(int, int)
int StatusWindow::OnTouchBegin(int param_1, int param_2) {
    StatusWindow *self = this;
    void **lh = g_StatusWindow_layout;
    i32(self, 0x50) = param_2;
    i32(self, 0x3c) = param_2;
    i32(self, 0x44) = 0;
    self->isDragging = 1;
    ((Layout *)(*lh))->OnTouchBegin(param_1, param_2);
    if (*g_StatusWindow_btnFlag == 0) {
        for (unsigned i = 0; i < self->tabButtons->size; ++i)
            ((TouchButton *)(self->tabButtons->data[i]))->OnTouchBegin(param_1, param_2);
    }
    if (i32(self, 0x30) == 1) {
        void **holder = g_StatusWindow_ach;
        int *medals = (int *)((Achievements *)(*holder))->getMedals();
        for (int i = 0; i < i32(self, 0x0); ++i) {
            if (medals[i] != 0 || ((Achievements *)(*holder))->isEliteMedal(i) != 0)
                ((TouchButton *)(self->medalButtons->data[i]))->OnTouchBegin(param_1, param_2);
        }
    }
    return 0;
}

// ---- getRelativeScrollHeight_158200.cpp ----
// StatusWindow::getRelativeScrollHeight() -> ratio of visible content to total, clamped by range.
float StatusWindow::getRelativeScrollHeight() {
    StatusWindow *self = this;
    int a = i32(self, 0x58);
    int b = i32(self, 0x5c);
    if (b > a) return 0.0f;
    int range = i32(self, 0x38);
    int num;
    if (range >= 1) {
        num = b - range;
    } else if (range < b - a) {
        num = range + a;
    } else {
        num = b;
    }
    return (float)num / (float)a;
}

// ---- update_158250.cpp ----
// StatusWindow::update(int) -- scroll inertia + selected-tab button highlight.
void StatusWindow::update() {
    StatusWindow *self = this;
    // Velocity integration while not being dragged (+0x54 == drag flag).
    if (self->isDragging == 0) {
        float v = f32(self, 0x48) * f32(self, 0x4c);
        f32(self, 0x4c) = v;
        // If |v| >= 1.0 keep scrolling: advance the integer scroll offset (+0x38).
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            f32(self, 0x38) = (float)(int)(v + (float)i32(self, 0x38));
            i32(self, 0x38) = (int)(v + (float)i32(self, 0x38));
        }
    }

    int off = i32(self, 0x38);
    if (off > 0) {
        // Overscrolled past the top: spring back.
        f32(self, 0x48) = 1.0f;
        f32(self, 0x4c) = (float)(-off) * 0.5f;
    }

    int bottom = i32(self, 0x5c) - i32(self, 0x58);
    if (bottom < 0) {
        if (off < bottom) {
            // Overscrolled past the bottom: spring back.
            f32(self, 0x48) = 1.0f;
            f32(self, 0x4c) = (float)(bottom - off) * 0.5f;
        }
    } else {
        // Content fits: clamp to top.
        f32(self, 0x4c) = 0.0f;
        i32(self, 0x38) = 0;
    }

    // Highlight the button matching the active tab index (+0x30).
    void **arr = (void **)pp(self, 0x4);
    for (unsigned int idx = 0; idx < *(unsigned int *)arr; idx++) {
        void *btn = ((void **)arr[1])[idx];
        ((TouchButton *)(btn))->setAlwaysPressed(idx == u32(self, 0x30));
    }
}

// ---- getMedalHintText_159ba4.cpp ----
// Stack String slots are 0xc bytes; we drive them via the engine's String entry points.
extern "C" {

void String_default(void *s);                              // String::String()
void String_fromC(void *s, const char *text, bool copy);   // String::String(char*, bool)
// String::~String()
void String_concat(void *out, void *lhs, void *rhs);       // out = lhs + rhs (operator+)
void String_appendAssign(void *self, void *rhs);           // self += rhs (operator+=)




extern void *g_swh_achievements; // *(DAT_169f68): achievements root
extern void *g_swh_status;       // various *(DAT...): status/campaign singletons share this base
extern void *g_swh_gameText;     // GameText id source
}

// StatusWindow::getMedalHintText(String *out, int medalIndex)
void StatusWindow_getMedalHintText(void *outStr, int medalIndex)
{

    int *medals = ((Achievements *)(*(void **)g_swh_achievements))->getMedals();
    int state = medals[medalIndex];

    String_default(outStr);

    // Only "in-progress" (state == 2) medals get a detailed checklist.
    if (state != 0) {
        char tmpA[0xc];  // intro/prefix line
        char tmpB[0xc];  // concatenation scratch

        // Helper macro-ish: append GameText(id) prefixed by 'lead'.
        // Each branch builds one header then iterates over a sub-goal list.
        if (medalIndex == 2 && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = ((GameText *)(*(void **)g_swh_gameText))->getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            ((String *)(tmpB))->dtor();
            ((String *)(tmpA))->dtor();

            void *base = *(void **)g_swh_status;
            unsigned int *list = *(unsigned int **)((char *)base + 0x94);
            for (unsigned int i = 0; i < *list; i++) {
                if (*(char *)(list[1] + i) == 0) {
                    String_fromC(tmpA, "\n", false);
                    void *t = ((GameText *)(*(void **)g_swh_gameText))->getText(0x594 + (int)i);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    ((String *)(tmpB))->dtor();
                    ((String *)(tmpA))->dtor();
                }
            }
        } else if (medalIndex == 3 && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = ((GameText *)(*(void **)g_swh_gameText))->getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            ((String *)(tmpB))->dtor();
            ((String *)(tmpA))->dtor();

            void *base = *(void **)g_swh_status;
            unsigned int *list = *(unsigned int **)((char *)base + 0x98);
            for (unsigned int i = 0; i < *list; i++) {
                if (*(char *)(list[1] + i) == 0) {
                    String_fromC(tmpA, "\n", false);
                    void *t = ((GameText *)(*(void **)g_swh_gameText))->getText(0x59f + (int)i);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    ((String *)(tmpB))->dtor();
                    ((String *)(tmpA))->dtor();
                }
            }
        } else if (medalIndex == 9 && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = ((GameText *)(*(void **)g_swh_gameText))->getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            ((String *)(tmpB))->dtor();
            ((String *)(tmpA))->dtor();

            void *base = *(void **)g_swh_status;
            unsigned int *list = *(unsigned int **)((char *)base + 0xac);
            for (unsigned int i = 0; i < *list; i++) {
                if (*(char *)(list[1] + i) == 0) {
                    String_fromC(tmpA, "\n", false);
                    void *t = ((GameText *)(*(void **)g_swh_gameText))->getText(0x57e + (int)i);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    ((String *)(tmpB))->dtor();
                    ((String *)(tmpA))->dtor();
                }
            }
        } else if (medalIndex == 0xd && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = ((GameText *)(*(void **)g_swh_gameText))->getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            ((String *)(tmpB))->dtor();
            ((String *)(tmpA))->dtor();

            void *root = *(void **)g_swh_status;
            for (unsigned int i = 0; i < 0xd; i++) {
                void *bp = *(void **)(*(int *)(*(int *)((char *)root + 0x18) + 4) + i * 4);
                if (*(char *)((char *)bp + 8) == 0) {
                    String_fromC(tmpA, "\n", false);
                    int idx = ((BluePrint *)(bp))->getIndex();
                    void *t = ((GameText *)(*(void **)g_swh_gameText))->getText(idx + 0x4fa);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    ((String *)(tmpB))->dtor();
                    ((String *)(tmpA))->dtor();
                }
            }
        } else if (medalIndex == 0xe && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = ((GameText *)(*(void **)g_swh_gameText))->getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            ((String *)(tmpB))->dtor();
            ((String *)(tmpA))->dtor();

            void *root = *(void **)g_swh_status;
            for (unsigned int i = 0; i < 0xd; i++) {
                void *bp = *(void **)(*(int *)(*(int *)((char *)root + 0x18) + 4) + i * 4);
                if (*(int *)((char *)bp + 0xc) == 0) {
                    String_fromC(tmpA, "\n", false);
                    int idx = ((BluePrint *)(bp))->getIndex();
                    void *t = ((GameText *)(*(void **)g_swh_gameText))->getText(idx + 0x4fa);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    ((String *)(tmpB))->dtor();
                    ((String *)(tmpA))->dtor();
                }
            }
        }
    }

    
}

// ---- reInit_158044.cpp ----
extern "C" {
void *Achievements_get();            // *(DAT_168150): achievements singleton (via getter at DAT_168154)
int Achievements_isUnlocked(void *ach, int index);

extern void *g_sw_imageFactory;   // **(DAT_168148): image factory root
extern int   g_sw_charDef;        // DAT_16814c: character definition arg
extern void *g_sw_achievements;   // *(DAT_168150): achievements accessor base
extern void *g_sw_canvas;         // *(DAT_168160): paint canvas singleton
}

// StatusWindow::reInit() -- rebuild the four medal/rank image tiles from achievement state.
void StatusWindow::reInit() {
    StatusWindow *self = this;
    pp(self, 0xc) = ((ImageFactory *)(*(void **)g_sw_imageFactory))->loadChar(&g_sw_charDef);

    void *ach = *(void **)g_sw_achievements;
    int a0 = Achievements_isUnlocked(Achievements_get(), 0);
    int a1 = Achievements_isUnlocked(Achievements_get(), 1);
    int a2 = Achievements_isUnlocked(Achievements_get(), 2);
    int a3 = Achievements_isUnlocked(Achievements_get(), 3);
    (void)ach;

    void *canvas = *(void **)g_sw_canvas;

    int id = 0x493;
    if (a1) id = 0x494;
    if (a0) id = 0x495;
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)id, (unsigned int *)((char *)self + 0x14));

    id = 0x492;
    if (a0) id = 0x496;
    if (a1) id = 0x497;
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)id, (unsigned int *)((char *)self + 0x18));

    id = 0x490;
    if (a3) id = 0x498;
    if (a2) id = 0x499;
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)id, (unsigned int *)((char *)self + 0x1c));

    id = 0x491;
    if (a2) id = 0x49a;
    if (a3) id = 0x49b;
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)id, (unsigned int *)((char *)self + 0x20));

    i32(self, 0x60) = ((PaintCanvas *)canvas)->GetImage2DWidth(0);
    i32(self, 0x64) = ((PaintCanvas *)canvas)->GetImage2DHeight(0);
}

// ---- draw_15831c.cpp ----
// StatusWindow::draw() -- renders either the player-stats tab or the achievements/medal tab.
// This is a very large UI routine; the control flow, field offsets and the substantive engine
// calls are reproduced faithfully. Repetitive DrawString blocks share the local helpers below.

extern "C" {

// String slots are 0xc bytes, driven via engine entry points.
void String_default(void *s);
void String_fromC(void *s, const char *text, bool copy);
void String_fromInt(void *s, int value);
void String_fromText(void *s, void *text, bool copy);
void String_concatText(void *out, void *lhs);          // operator+(String, text)
void String_concatInt(void *out, void *lhs, int *v);   // operator+(String, int)
void String_subString(void *self, void *other);

int   GameText_getLanguage();

void Layout_drawHeader(void *layout, void *title);
void Layout_formatCredits(void *out, int credits);





void  Globals_longToTimeStringNoSeconds(void *globals, void *out, unsigned long long t);
void  Globals_drawLines(void *globals, void *font, void *arr, int y, char clip);


int   __aeabi_idiv(int a, int b);
int   __aeabi_uidiv(unsigned a, unsigned b);

extern void *g_swd_screen;     // *(DAT_168464): screen-state base (stack-guard source)
extern void *g_swd_canvas;     // *(DAT_168468): paint canvas
extern int  *g_swd_dimW;       // *(DAT_16846c): screen width
extern int  *g_swd_dimH;       // *(DAT_168470): screen height
extern void *g_swd_layout;     // *(DAT_168474): Layout singleton
extern void *g_swd_imageFactory; // *(DAT_168544): image factory
extern void *g_swd_font;       // *(DAT_1685a4): default font
extern void *g_swd_status;     // *(DAT_16855c): status singleton
extern void *g_swd_globals;    // *(DAT_16868c): globals
extern char *g_swd_landscape;  // *(DAT_168478): landscape-layout flag
extern int  *g_swd_textId;     // *(DAT_1684ac): rolling GameText id cursor
extern void *g_swd_gameText;   // *(DAT_1684ac): GameText singleton (receiver for getText)
}


// StatusWindow::draw()
void StatusWindow::draw() {
    StatusWindow *self = this;

    void *canvas = *(void **)g_swd_canvas;
    Layout *layout = (Layout *)*(void **)g_swd_layout;
    void *font = *(void **)g_swd_font;
    char *land = (char *)*(void **)((char *)&g_swd_landscape);
    int screenW = *g_swd_dimW;
    int screenH = *g_swd_dimH;

    // --- background + scrollbar ---
    ((PaintCanvas *)canvas)->SetColor(0u);
    ((PaintCanvas *)canvas)->FillRectangle(0, 0, screenW, screenH);
    ((PaintCanvas *)canvas)->SetColor(0u);
    ((Layout *)(layout))->drawBG();

    float relStart = ((StatusWindow *)(self))->getRelativeScrollStartPos();
    int contentH = i32(self, 0x5c);
    float ch = (float)contentH;
    float relH = ((StatusWindow *)(self))->getRelativeScrollHeight();
    int barH = (int)(relH * ch);
    if (barH > 0 || (int)(relStart * ch) > 0) {
        ((Layout *)(layout))->drawScrollBar((screenW - layout->field_0x48) - layout->field_0x28, layout->field_0x20 + layout->field_0xc, contentH, (int)(relStart * ch), barH);
    }

    int top = layout->field_0x20 + layout->field_0xc;
    int colW;
    if (*land == 0) {
        colW = screenW;
        top += i32(self, 0x38);
    } else {
        colW = screenW >> 1;
    }
    if (barH > 0)
        colW = (colW - layout->field_0x48) - layout->field_0x2c;
    i32(self, 0x6c) = colW + layout->field_0x28 * -2;

    char creditStr[0xc];
    String_default(creditStr);
    char sep[0xc];
    if (GameText_getLanguage() == 9)
        String_fromC(sep, "\xa1", false);
    else
        String_fromC(sep, ":", false);

    int tab = i32(self, 0x30);
    char drewStats = 0;

    // ===== player-stats tab (index 0 / landscape) =====
    if (tab == 0 || *land != 0) {
        int boxW = i32(self, 0x6c);
        int x0 = layout->field_0x28;
        int pad = layout->field_0x2c;
        char lbl[0xc];

        void *t = ((GameText *)(*(void **)g_swd_gameText))->getText(*g_swd_textId);
        String_fromText(lbl, t, false);
        ((Layout *)(layout))->drawBox(0, x0, top, boxW, layout->field_0x1c, lbl, 0);
        ((String *)(lbl))->dtor();

        int y = layout->field_0x1c + top + pad;

        // Credits panel.
        String_fromC(lbl, "", false);
        ((Layout *)(layout))->drawBox(5, x0, y, (boxW >> 1) - pad, layout->field_0x2d8, lbl, 0);
        ((String *)(lbl))->dtor();
        ((ImageFactory *)(*(void **)g_swd_imageFactory))->drawChar((Arr *)pp(self, 0xc), layout->field_0x4c + x0, y, false);
        char credTmp[0xc];
        Layout_formatCredits(credTmp, ((Status *)(*(void **)g_swd_status))->getCredits());
        ((String *)(creditStr))->assign((String *)credTmp);
        ((String *)(credTmp))->dtor();
        int tw = ((PaintCanvas *)canvas)->GetTextWidth((unsigned)(uintptr_t)font, creditStr);
        ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y, false);

        // Level line.
        char lvlPrefix[0xc], lvlText[0xc], lvlFull[0xc];
        void *lt = ((GameText *)(*(void **)g_swd_gameText))->getText(*g_swd_textId);
        String_fromC(lvlPrefix, " ", false);
        String_concatText(lvlText, lt);
        int lvl = ((Status *)(*(void **)g_swd_status))->getLevel();
        String_concatInt(lvlFull, lvlText, &lvl);
        ((String *)(creditStr))->assign((String *)lvlFull);
        ((String *)(lvlFull))->dtor(); ((String *)(lvlText))->dtor(); ((String *)(lvlPrefix))->dtor();
        tw = ((PaintCanvas *)canvas)->GetTextWidth((unsigned)(uintptr_t)font, creditStr);
        ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y, false);

        // Playing-time line.
        char timeStr[0xc];
        Globals_longToTimeStringNoSeconds(*(void **)g_swd_globals, timeStr, ((Status *)(*(void **)g_swd_status))->getPlayingTime());
        tw = ((PaintCanvas *)canvas)->GetTextWidth((unsigned)(uintptr_t)font, creditStr);
        ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y, false);

        // Ship picture panel.
        String_fromC(lbl, "", false);
        ((Layout *)(layout))->drawBox(5, (boxW >> 1) + x0 + pad, y, (boxW >> 1) - pad, layout->field_0x2d8, lbl, 0);
        ((String *)(lbl))->dtor();
        ((ImageFactory *)(*(void **)g_swd_imageFactory))->drawShip(((Ship *)(((Status *)(*(void **)g_swd_status))->getShip()))->getIndex(), x0 + (boxW >> 1) + pad * 2, y);
        void *shipNameTxt = ((GameText *)(*(void **)g_swd_gameText))->getText(((Ship *)(((Status *)(*(void **)g_swd_status))->getShip()))->getIndex());
        ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, shipNameTxt,
                               x0 + (boxW >> 1) + pad * 3 + layout->field_0x2cc, y, false);

        // Fire-power line.
        char fpStr[0xc], fpPre[0xc], fpFull[0xc];
        int firePow = ((Ship *)(((Status *)(*(void **)g_swd_status))->getShip()))->getFirePower();
        String_fromInt(fpStr, (int)((float)firePow * 1.0f));
        int fp2 = ((Ship *)(((Status *)(*(void **)g_swd_status))->getShip()))->getFirePower();
        String_fromC(fpPre, "%", false);
        String_concatInt(fpFull, fpPre, &fp2);
        String_concatText(fpStr, fpFull);
        ((String *)(creditStr))->assign((String *)fpStr);
        ((String *)(fpFull))->dtor(); ((String *)(fpPre))->dtor(); ((String *)(fpStr))->dtor();
        tw = ((PaintCanvas *)canvas)->GetTextWidth((unsigned)(uintptr_t)font, creditStr);
        ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, creditStr, ((y + x0) - pad) - tw, y, false);

        // Combined-HP line.
        char hpStr[0xc];
        String_fromInt(hpStr, ((Ship *)(((Status *)(*(void **)g_swd_status))->getShip()))->getCombinedHP());
        tw = ((PaintCanvas *)canvas)->GetTextWidth((unsigned)(uintptr_t)font, hpStr);
        ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, hpStr, ((y + x0) - pad) - tw, y, false);
        ((String *)(hpStr))->dtor();

        // Standing emblem panel + bars.
        void *standing = (void *)(intptr_t)((Status *)(*(void **)g_swd_status))->getStanding();
        float rate = ((Standing *)(standing))->getStandingRate(0);
        ((PaintCanvas *)canvas)->DrawImage2D((unsigned)i32(self, 0x24), x0 + (boxW >> 2), y, (unsigned char)'\x11');
        ((PaintCanvas *)canvas)->DrawRegion2D((unsigned)i32(self, 0x28), i32(self, 0x70), 0,
                                 (int)-(rate * (float)i32(self, 0x70)), i32(self, 0x74),
                                 -(rate * (float)i32(self, 0x70)), 0, 0, 0, x0 + (boxW >> 2));
        ((PaintCanvas *)canvas)->DrawImage2D((unsigned)i32(self, 0x2c), x0, y, (unsigned char)'\x11');

        // Career-stat rows from the Status singleton.
        Status *st = (Status *)(*(void **)g_swd_status);
        char rowStr[0xc];
        int rowX = layout->field_0x4c + x0;
        for (unsigned r = 0; r < 6; r++) {
            int rowVal;
            switch (r) {
                case 0:  rowVal = st->getMissionCount();    break;
                case 1:  rowVal = st->getKills();           break;
                case 2:  rowVal = st->getCapturedCrates();  break;
                case 3:  rowVal = st->getStationsVisited(); break;
                case 4:  rowVal = st->getJumpgateUsed();    break;
                default: rowVal = st->getGoodsProduced();   break;
            }
            void *labelTxt = ((GameText *)(*(void **)g_swd_gameText))->getText(*g_swd_textId);
            ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, labelTxt, rowX, y, false);
            String_fromInt(rowStr, rowVal);
            tw = ((PaintCanvas *)canvas)->GetTextWidth((unsigned)(uintptr_t)font, rowStr);
            ((PaintCanvas *)canvas)->DrawString((unsigned)(uintptr_t)font, rowStr, ((y + x0) - pad) - tw, y, false);
            ((String *)(rowStr))->dtor();
        }

        drewStats = *land;
        tab = i32(self, 0x30);
    }

    // ===== achievements / medal tab (index 1) =====
    if (drewStats != 0 || tab == 1) {
        int boxW = i32(self, 0x6c);
        int third = __aeabi_idiv(boxW, 3);
        int x0 = layout->field_0x28;
        int rowH = i32(self, 0x78);
        int gridX0 = drewStats ? (boxW + (third >> 1) + x0) : (x0 + (third >> 1));
        int gridY0 = layout->field_0xc + (rowH >> 1) + layout->field_0x2c;

        if (drewStats != 0) {
            char hdr[0xc];
            void *t = ((GameText *)(*(void **)g_swd_gameText))->getText(*g_swd_textId);
            String_fromText(hdr, t, false);
            ((Layout *)(layout))->drawBox(0, boxW + x0 * 2, top, x0 + boxW, layout->field_0x1c, hdr, 0);
            ((String *)(hdr))->dtor();
            gridY0 += layout->field_0x1c + layout->field_0x2c;
        }

        for (int i = 0; i < i32(self, 0x0); i++) {
            int col = (int)__aeabi_uidiv((unsigned)i, 3);
            int by = col * rowH + gridY0 + i32(self, 0x38);
            ((TouchButton *)(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4)))->setPosition((i - col * 3) * third + gridX0, by, 0x44);
            if (by >= 0 && by <= screenH)
                ((TouchButton *)(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4)))->draw();
        }

        // Selected-medal detail panel.
        if (i32(self, 0x34) >= 0) {
            ((PaintCanvas *)canvas)->SetColor(0u);
            int lines = *(int *)pp(self, 0x10);
            int lineH = layout->field_0x4;
            char lbl[0xc];
            String_fromC(lbl, "", false);
            ((Layout *)(layout))->drawBox(2, layout->field_0x28, (((screenH - layout->field_0x10) -
                             layout->field_0x24) - lineH * lines) +
                               layout->field_0x4c * -2, i32(self, 0x6c), layout->field_0x4c * 2 + lineH * lines, lbl, 0);
            ((String *)(lbl))->dtor();

            String_fromC(lbl, "", false);
            ((Layout *)(layout))->drawBox(5, layout->field_0x28, (((screenH - layout->field_0x10) -
                             layout->field_0x24) - lineH * lines) +
                               layout->field_0x4c * -2, i32(self, 0x6c), layout->field_0x4c * 2 + lineH * lines, lbl, 0);
            ((String *)(lbl))->dtor();

            Globals_drawLines(*(void **)g_swd_globals, font, pp(self, 0x10),
                              layout->field_0x4c + layout->field_0x28,
                              (char)screenH);
        }
    }

    // --- header / footer / tab buttons ---
    char header[0xc];
    void *ht = ((GameText *)(*(void **)g_swd_gameText))->getText(*g_swd_textId);
    String_fromText(header, ht, false);
    Layout_drawHeader(layout, header);
    ((String *)(header))->dtor();
    ((Layout *)(layout))->drawFooter();

    if (*land == 0) {
        void **tabs = (void **)pp(self, 0x4);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
            ((TouchButton *)(((void **)tabs[1])[i]))->draw();
    }

    ((String *)(sep))->dtor();
    ((String *)(creditStr))->dtor();

    
}

// ---- StatusWindow_157d80.cpp ----
extern "C" {

void ArrayTB_ctor(void *self);                       // Array<TouchButton*>::Array()
void ArrayTB_setLength(int n, void *self);           // ArraySetLength<TouchButton*>


void TouchButton_ctor_tab(void *self, void *text, int kind, int x, int y, char flags);
void TouchButton_ctor_medal(void *self, int index, int medal, void *text, int x, int y, char flags);



int  __aeabi_idiv(int a, int b);

extern void *g_sw_gameTextDef;   // *(DAT_168024): GameText id source
extern void *g_sw_layout;        // *(DAT_16802c): Layout singleton
extern void *g_sw_layoutW;       // *(DAT_168028): layout width source
extern unsigned char *g_sw_tabIndex; // *(DAT_168030): default tab index byte
extern void *g_sw_achievements;  // *(DAT_168034): achievements root
extern void *g_sw_canvas;        // *(DAT_168038): paint canvas
extern int   g_sw_screenH;       // *(DAT_168040): screen height source
}


// StatusWindow::StatusWindow() -- build the tab bar and the medal grid, then lay out scrolling.
StatusWindow * StatusWindow::ctor() {
    StatusWindow *self = this;
    // --- two-tab button bar at +0x04 ---
    void *tabs = ::operator new(0xc);
    ArrayTB_ctor(tabs);
    pp(self, 0x4) = tabs;
    ArrayTB_setLength(2, tabs);

    Layout *layout = (Layout *)*(void **)g_sw_layout;
    int layoutW = *(int *)*(void **)g_sw_layoutW;
    int textId = *(int *)*(void **)g_sw_gameTextDef;

    void *b0 = ::operator new(200);
    void *t0 = ((GameText *)(*(void **)g_sw_gameTextDef))->getText(textId);
    int helpOff = layout->getHelpButtonOffset();
    TouchButton_ctor_tab(b0, t0, 3, layoutW - helpOff, 0, 0x12);
    *(void **)(*(int *)(self->tabButtons + 4) + 4) = b0;

    void *b1 = ::operator new(200);
    void *t1 = ((GameText *)(*(void **)g_sw_gameTextDef))->getText(textId);
    int helpOff2 = layout->getHelpButtonOffset();
    int w1 = ((TouchButton *)(b1))->getWidth();
    TouchButton_ctor_tab(b1, t1, 3,
                         ((layoutW - helpOff2) - w1) + layout->field_0x38, 0, 0x12);
    *(void **)(self->tabButtons + 4) = b1;

    unsigned int defTab = *g_sw_tabIndex;
    u32(self, 0x30) = defTab;
    ((TouchButton *)(*(void **)(*(int *)(self->tabButtons + 4) + defTab * 4)))->setAlwaysPressed(true);

    i32(self, 0x10) = 0;
    i32(self, 0x78) = layout->field_0x84;
    i32(self, 0x34) = -1;

    // --- 45 medal buttons at +0x08 ---
    void *medals = ::operator new(0xc);
    ArrayTB_ctor(medals);
    i32(self, 0x0) = 0x2d;
    pp(self, 0x8) = medals;
    ArrayTB_setLength(0x2d, medals);

    int *medalIds = ((Achievements *)(*(void **)g_sw_achievements))->getMedals();
    for (int i = 0; i < i32(self, 0x0); i++) {
        void *btn = ::operator new(200);
        int medal = medalIds[i];
        void *txt = ((GameText *)(*(void **)g_sw_gameTextDef))->getText(textId);
        TouchButton_ctor_medal(btn, i, medal, txt, 0, 0, 'D');
        *(void **)(*(int *)(self->medalButtons + 4) + i * 4) = btn;
    }

    ((StatusWindow *)(self))->reInit();

    void *canvas = *(void **)g_sw_canvas;
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)0x48e, (unsigned int *)((char *)self + 0x24));
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)0x48f, (unsigned int *)((char *)self + 0x28));
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)0x48d, (unsigned int *)((char *)self + 0x2c));
    i32(self, 0x70) = ((PaintCanvas *)canvas)->GetImage2DWidth(0) / 2;
    int img3h = ((PaintCanvas *)canvas)->GetImage2DHeight(0);

    // Zero the inertia/colour state blocks (+0x45..+0x54, +0x38..+0x44).
    for (int o = 0x45; o < 0x55; o++) *((unsigned char *)self + o) = 0;
    for (int o = 0x38; o < 0x48; o += 4) i32(self, o) = 0;
    i32(self, 0x74) = img3h;

    // Precompute per-tab scroll content heights.
    int *heights = (int *)::operator new[](0xc);
    pp(self, 0x68) = heights;
    int row = ((layout->field_0x1c * 3 + layout->field_0x2d8) +
               layout->field_0x2c * 8) + i32(self, 0x64) +
               layout->field_0x4 * 7;
    heights[0] = row;

    int lineH;
    if (*g_sw_tabIndex == 0) {
        lineH = i32(self, 0x78) * __aeabi_idiv(i32(self, 0x0), 3);
    } else {
        lineH = __aeabi_idiv(i32(self, 0x0), 3) * i32(self, 0x78) +
                layout->field_0x1c + layout->field_0x2c;
    }
    heights[1] = lineH + 10;

    i32(self, 0x58) = heights[u32(self, 0x30)];
    i32(self, 0x5c) =
        (((g_sw_screenH - layout->field_0x10) - layout->field_0xc) -
         layout->field_0x20) - layout->field_0x24;
    return self;
}
