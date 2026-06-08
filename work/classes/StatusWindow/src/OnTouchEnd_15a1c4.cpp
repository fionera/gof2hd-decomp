#include "class.h"

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void *operator_new(uint32_t size);
void operator_delete(void *p) noexcept;

void String_fromC(void *s, const char *text, bool copy);
void String_fromText(void *s, void *text, bool copy);
void String_dtor(void *s);
void String_appendAssign(void *self, void *rhs);

int  Layout_OnTouchEnd(void *layout, int x, int y);
int  Layout_helpPressed(void *layout);
void Layout_initHelpWindow(void *layout, void *title);

int  TouchButton_OnTouchEnd(void *btn, int x);
void TouchButton_setAlwaysPressed(void *btn, bool pressed);

int  *Achievements_getMedals(void *ach);
int  Achievements_isEliteMedal(void *ach, int index);
void *Achievements_getValue(void *ach, int medal, int count);

void *GameText_getText(int id);

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
extern "C" void StatusWindow_OnTouchEnd(StatusWindow *self, int x, int y)
{
    void *volatile cookie = __stack_chk_guard;

    // Release scroll inertia: snap velocity, clamp scroll position.
    int vy = i32(self, 0x44);
    int newOff = i32(self, 0x38) + vy;
    float vf = (float)vy;
    int absvy = vy < 0 ? -vy : vy;
    void *layout = *(void **)g_swe_layout;

    f32(self, 0x48) = 1.0f;                       // DAT_16a494 == 1.0
    F<unsigned char>(self, 0x54) = 0;
    i32(self, 0x38) = newOff;
    i32(self, 0x40) = newOff;
    f32(self, 0x4c) = (absvy > 3) ? vf : 0.0f;    // DAT_16a490 == 0.0

    if (Layout_OnTouchEnd(layout, x, y) != 0)
        goto done;

    // Tab buttons (only when no modal dialog is open).
    if (*g_swe_dialogBlock == 0) {
        void **tabs = (void **)pp(self, 0x4);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++) {
            if (TouchButton_OnTouchEnd(((void **)tabs[1])[i], x) != 0) {
                u32(self, 0x30) = i;
                i32(self, 0x58) = *(int *)((char *)pp(self, 0x68) + i * 4);
                i32(self, 0x38) = 0;
            }
        }
    }

    // Medal grid (only on the achievements tab, index 1).
    if (i32(self, 0x30) == 1) {
        for (int i = 0; i < i32(self, 0x0); i++) {
            if (TouchButton_OnTouchEnd(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4), x) != 0) {
                void *ach = *(void **)g_swe_achievements;
                int *medals = Achievements_getMedals(ach);
                int elite = Achievements_isEliteMedal(ach, i);
                if (elite != 0 || medals[i] != 0) {
                    if (i32(self, 0x34) >= 0) {
                        TouchButton_setAlwaysPressed(
                            *(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i32(self, 0x34) * 4), false);
                    }
                    i32(self, 0x34) = i;

                    // Rebuild the detail-text line array at +0x10.
                    if (pp(self, 0x10) != 0) {
                        ArrayStr_releaseClasses(pp(self, 0x10));
                        if (pp(self, 0x10) != 0)
                            operator_delete(ArrayStr_dtor(pp(self, 0x10)));
                    }
                    pp(self, 0x10) = 0;
                    void *lineArr = operator_new(0xc);
                    ArrayStr_ctor(lineArr);
                    pp(self, 0x10) = lineArr;

                    int count = (elite == 0) ? medals[i32(self, 0x34)] : 1;

                    char hdr[0xc], hdrTxt[0xc], valStr[0xc], valTmp[0xc], full[0xc], hint[0xc];
                    void *key = *(void **)g_swe_status;
                    void *t = GameText_getText(i32(self, 0x34) + 0x610);
                    String_fromText(hdr, t, false);
                    void *val = Achievements_getValue(ach, i32(self, 0x34), count);
                    String_fromText(valStr, val, false);
                    String_fromText(valTmp, valStr, false);
                    Status_replaceHash(full, key, hdr, valTmp);
                    String_dtor(valTmp);
                    String_dtor(valStr);
                    String_dtor(hdr);

                    StatusWindow_getMedalHintText(hint, i32(self, 0x34));
                    String_appendAssign(full, hint);
                    String_dtor(hint);

                    Globals_getLineArray(*(void **)g_swe_globals, *(void **)g_swe_font, full,
                                         (void *)(i32(self, 0x6c) - *(int *)((char *)layout + 0x4c) * 2));
                    TouchButton_setAlwaysPressed(*(void **)(*(int *)((char *)pp(self, 0x8) + 4) + i * 4), true);
                    String_dtor(full);
                    (void)hdrTxt;
                }
                break;
            }
        }
    }

    // Help button -> contextual help window for the current tab.
    if (Layout_helpPressed(layout) != 0) {
        if (i32(self, 0x30) == 1) {
            char title[0xc];
            void *t = GameText_getText(0x287);
            String_fromText(title, t, false);
            Layout_initHelpWindow(layout, title);
            String_dtor(title);
        } else if (i32(self, 0x30) == 0) {
            char title[0xc];
            void *t = GameText_getText(0x280);
            String_fromText(title, t, false);
            Layout_initHelpWindow(layout, title);
            String_dtor(title);
        }
    }

done:
    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
