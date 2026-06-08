#include "class.h"

struct Wanted;
struct Agent;
struct DialogueWindow;
struct Achievements;

__attribute__((visibility("hidden"))) extern int  *g_ch_stack;      // [DAT_000eaf3c]
__attribute__((visibility("hidden"))) extern int  *g_ch_hintRec;    // [DAT_000eaf40] hint-shown flags record
__attribute__((visibility("hidden"))) extern int **g_ch_status;     // [DAT_000eafb4] etc.
__attribute__((visibility("hidden"))) extern Achievements **g_ch_ach;// [DAT_000eaf54]

extern "C" {
int  Status_getCurrentCampaignMission_ch();
int  Wanted_isTerminated_ch(Wanted *w);
int  Achievements_gotAllMedals_ch(Achievements *a);
int  Achievements_gotAllGoldMedals_ch();
int  Achievements_gotAllSupernovaMedals_ch(Achievements *a);
int  Status_isBlueprintUnlocked_ch(int status, int bp);
void Status_unlockBluePrint_ch(int status, int bp);
int  Status_hardCoreMode_ch();
void ModStation_autosave_ch();
// Pops the CBS choice-window hint for the given wanted index (builds the "criminal terminated"
// message with name/ship substitution — a long String-temporary chain the decompiler mangled).
void ModStation_ch_showWantedHint(ModStation *self, int wantedFieldOff);
// Pops a simple text-only choice-window hint (medals / blueprint unlock / hardcore reward).
void ModStation_ch_showTextHint(ModStation *self, int textIdSlot);
// Spawns the "new wingman available" / "wingman left" dialogue window with its sound.
void ModStation_ch_showWingmanDialogue(ModStation *self, int kind);
}

// ModStation::checkHints() — once per docked frame, shows the highest-priority contextual hint
// (wanted criminal terminated, all medals earned, blueprint unlocked, wingman events).
extern "C" void ModStation_checkHints(ModStation *self)
{
    char *s = (char *)self;
    if (C(s, 0x60) != 0)   // a window is already busy
        return;

    int hintRec = *g_ch_hintRec;

    // intro CBS hint (after mission 0x12).
    if (C(s, 0x6a) == 0 && C(s, 0x63) == 0 && *(char *)(hintRec + 0x34) == 0 &&
        0x12 < Status_getCurrentCampaignMission_ch()) {
        ModStation_ch_showWantedHint(self, -1);
        *(char *)(hintRec + 0x34) = 1;
        C(s, 0x63) = 1;
        C(s, 0xdc) = 1;
    }

    int **status = g_ch_status;
    // four "storyline wanted terminated" hints (slots 0x18/0x30/0x48/0x60).
    static const int wantedOff[4] = {0x18, 0x30, 0x48, 0x60};
    static const int wantedFlag[4] = {0x33, 0x34, 0x35, 0x36};
    if (C(s, 0x69) == 0) {
        for (int k = 0; k < 4; k = k + 1) {
            if (C(s, 0x6a) != 0) return;
            if (C(s, 0x63) == 0 && *(char *)(hintRec + wantedFlag[k]) == 0) {
                Wanted *w = *(Wanted **)(*(int *)(*(int *)*status + 4) + wantedOff[k]);
                if (Wanted_isTerminated_ch(w) != 0) {
                    ModStation_ch_showWantedHint(self, wantedOff[k]);
                    C(s, 0x63) = 1;
                    *(char *)(hintRec + wantedFlag[k]) = 1;
                }
            }
        }
    }

    // achievement-completion hints.
    if (C(s, 0x6a) == 0) {
        if (C(s, 0x63) == 0 && *(char *)(hintRec + 0x1a) == 0 &&
            Achievements_gotAllMedals_ch(*g_ch_ach) != 0) {
            ModStation_ch_showTextHint(self, 0x1a);
            *(char *)(hintRec + 0x1a) = 1;
            C(s, 0x63) = 1;
        }
        if (C(s, 0x6a) == 0 && C(s, 0x63) == 0 && *(char *)(hintRec + 0x1b) == 0 &&
            Achievements_gotAllGoldMedals_ch() != 0) {
            ModStation_ch_showTextHint(self, 0x1b);
            *(char *)(hintRec + 0x1b) = 1;
            C(s, 0x63) = 1;
        }
        // supernova blueprint unlock.
        if (C(s, 0x6a) == 0 && C(s, 0x63) == 0) {
            int statPtr = (int)*status;
            if (Status_isBlueprintUnlocked_ch(statPtr, 0xe8) == 0 &&
                Achievements_gotAllGoldMedals_ch() != 0 &&
                Achievements_gotAllSupernovaMedals_ch(*g_ch_ach) != 0) {
                ModStation_ch_showTextHint(self, 0x3b);
                Status_unlockBluePrint_ch(statPtr, 0xe8);
                ModStation_autosave_ch();
                C(s, 0x63) = 1;
            }
        }
        // hardcore-unlock hint.
        if (C(s, 0x6a) == 0 && C(s, 0x63) == 0 && *(char *)(hintRec + 0x3a) == 0 &&
            0xa1 < Status_getCurrentCampaignMission_ch()) {
            bool ok = (Achievements_gotAllGoldMedals_ch() != 0 &&
                       Achievements_gotAllSupernovaMedals_ch(*g_ch_ach) != 0 &&
                       Status_hardCoreMode_ch() == 0) ||
                      Status_hardCoreMode_ch() != 0;
            if (ok) {
                *(char *)(hintRec + 0x3a) = 1;
                ModStation_ch_showTextHint(self, 0x3c);
                ModStation_autosave_ch();
                C(s, 0x63) = 1;
            }
        }
    }

    // wingman recruit / dismissal dialogue.
    if (C(s, 0x69) == 0 && C(s, 0x6a) == 0 && C(s, 0x63) == 0)
        ModStation_ch_showWingmanDialogue(self, 0);
}
