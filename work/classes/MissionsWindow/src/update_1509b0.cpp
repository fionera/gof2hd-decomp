#include "class.h"

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// Two mode-specific tail calls reached via GOT thunks.
void MissionsWindow_acceptAction(void *self);   // DAT_1ac514 thunk (mode == 1)
void MissionsWindow_cancelAction(void *self);   // DAT_1ac284 thunk (flag @0x22)

void ScrollTouchWindow_update(void *win, int dt);
void ScrollTouchWindow_setText(void *win, void *a, void *b);

void *Status_getCampaignMission();
int   Status_gameWon();
int   Status_getCurrentCampaignMission();

int   Mission_getType(void *mission);
int   Mission_getProductionGoodAmount(void *mission);
int   Mission_getStatusValue(void *mission);

void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_fromInt(void *s, int value);
void  String_dtor(void *s);
void  String_assign(void *dst, void *src);

void *GameText_getText(int id);

void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

void  TouchButton_setAlwaysPressed(void *btn, bool pressed);

extern void *g_mw_status;        // *(DAT_160bbc): Status singleton
extern void *g_mw_campaign;      // *(DAT_160bc0): campaign-state record
extern int  *g_mw_textBase;      // *(DAT_160bc8): GameText id base for mission titles
extern int  *g_mw_titleTable;    // *(DAT_160bcc): per-mission title id table
extern void *g_mw_hashSource;    // *(DAT_160bbc): replaceHash key source (== status)
}

// MissionsWindow::update(int dt)
extern "C" void MissionsWindow_update(void *self, int dt)
{
    void *volatile cookie = __stack_chk_guard;

    // Mode 1: confirm/accept and bail out.
    if (i32(self, 0x40) == 1) {
        MissionsWindow_acceptAction(self);
        return;
    }
    // Cancel flag set: cancel and bail out.
    if (u8(self, 0x22) != 0) {
        MissionsWindow_cancelAction(self);
        return;
    }

    ScrollTouchWindow_update(pp(self, 0x0), dt);
    ScrollTouchWindow_update(pp(self, 0x4), dt);

    void *status = *(void **)g_mw_status;
    int type = Mission_getType(Status_getCampaignMission());
    bool relevant = (type == 0xa7);
    if (!relevant) {
        if (Mission_getType(Status_getCampaignMission()) == 0xae)
            relevant = true;
    }

    if (relevant) {
        void *camp = *(void **)g_mw_campaign;
        bool show = (Status_gameWon() == 0) ||
                    (*(char *)((char *)camp + 0x37) != 0 || *(char *)((char *)camp + 0x35) != 0);
        if (show) {
            char text[0xc];
            String_fromC(text, "", false);
            if (Status_getCurrentCampaignMission() < 0xa4) {
                int base = *g_mw_textBase;
                void *titleTxt = GameText_getText(g_mw_titleTable[Status_getCurrentCampaignMission()]);
                (void)base;
                String_assign(text, titleTxt);

                void *key = *(void **)g_mw_hashSource;
                char hdr[0xc], amount[0xc], suffix[0xc], merged[0xc];
                String_fromText(hdr, text, false);
                int need = Mission_getProductionGoodAmount(Status_getCampaignMission());
                int have = Mission_getStatusValue(Status_getCampaignMission());
                String_fromInt(amount, need - have);
                String_fromC(suffix, "", false);
                Status_replaceHash(merged, key, hdr, amount, suffix);
                String_assign(text, merged);
                String_dtor(merged); String_dtor(suffix); String_dtor(amount); String_dtor(hdr);

                void *win = pp(self, 0x0);
                char a[0xc], b[0xc];
                String_fromC(a, "", false);
                String_fromText(b, text, false);
                ScrollTouchWindow_setText(win, a, b);
                String_dtor(b);
                String_dtor(a);
            }
            String_dtor(text);
        }
    }

    // Highlight the selected tab.
    void **tabs = (void **)pp(self, 0x14);
    for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
        TouchButton_setAlwaysPressed(((void **)tabs[1])[i], i == u32(self, 0x40));

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
