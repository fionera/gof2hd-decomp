#include "class.h"

extern "C" int ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchEnd(void *self, int x, int y);
extern "C" void ChoiceWindow_set(void *self, String *text, bool flag);
extern "C" String *GameText_getText(void *self, int id);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" int FModSound_stop(void *self, int sound);
extern "C" void FModSound_play(void *self, int sound, void *pos, int volume, int extra);
extern "C" int DialogueWindow_previousPage(DialogueWindow *self);
extern "C" int DialogueWindow_nextPage(DialogueWindow *self);

__attribute__((visibility("hidden"))) extern void **g_dw_statusTouchEnd;
__attribute__((visibility("hidden"))) extern void **g_dw_soundChoice;
__attribute__((visibility("hidden"))) extern void **g_dw_soundVoice;
__attribute__((visibility("hidden"))) extern void **g_dw_soundPrev;
__attribute__((visibility("hidden"))) extern void **g_dw_soundNext;
__attribute__((visibility("hidden"))) extern void **g_dw_gameTextTouchEnd;

extern "C" int DialogueWindow_OnTouchEnd(DialogueWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x54) != 0) {
        int r = ChoiceWindow_OnTouchEnd(F<void *>(self, 0x50), x, y);
        if (r == 1) {
            goto choice_close;
        }
        if (r != 0) goto choice_return_zero;
        F<uint8_t>(self, 0x54) = 0;
        if (Status_getCurrentCampaignMission(*g_dw_statusTouchEnd) == 0x0f) {
            void **sound = g_dw_soundChoice;
            FModSound_play(*sound, 0xa2, 0, 0, 0);
            FModSound_stop(*sound, F<int>(*sound, 0));
            FModSound_play(*sound, 0x88, 0, 0, 0);
        }
        if (F<int>(self, 0x64) != -1) {
            FModSound_stop(*g_dw_soundVoice, F<int>(self, 0x64));
        }
        return 1;

choice_close:
        F<uint8_t>(self, 0x54) = 0;
choice_return_zero:
        return 0;
    }

    ScrollTouchWindow_OnTouchEnd(F<void *>(self, 0x40), x, y);
    if (TouchButton_OnTouchEnd(F<void *>(self, 0x0), x, y) != 0) {
        FModSound_stop(*g_dw_soundPrev, F<int>(self, 0x64));
        DialogueWindow_previousPage(self);
    }
    if (TouchButton_OnTouchEnd(F<void *>(self, 0x4), x, y) != 0) {
        FModSound_stop(*g_dw_soundNext, F<int>(self, 0x64));
        if (DialogueWindow_nextPage(self) == 0) {
            return 1;
        }
    }
    if (TouchButton_OnTouchEnd(F<void *>(self, 0x8), x, y) != 0) {
        void *choice = F<void *>(self, 0x50);
        String *text = GameText_getText(*g_dw_gameTextTouchEnd, 0x18c);
        ChoiceWindow_set(choice, text, true);
        F<uint8_t>(self, 0x54) = 1;
    }
    return 0;
}
