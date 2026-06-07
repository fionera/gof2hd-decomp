#include "class.h"

struct Status;
struct Mission;
struct Level;
struct PlayerEgo;
struct TargetFollowCamera;
struct DialogueWindow;
__attribute__((visibility("hidden"))) extern Status **g_status;

extern "C" int LevelScript_startSequenceOver(Level *lvl);
extern "C" Mission *Status_getCurrentCampaignMission(Status *s);
extern "C" int DialogueWindow_hasBriefingDialogue(Mission *m);
extern "C" Mission *Status_getMission(Status *s);
extern "C" int Mission_isCampaignMission(Mission *m);
extern "C" int Mission_isEmpty(Mission *m);
extern "C" int Mission_getType(Mission *m);
extern "C" int Mission_isVisible(Mission *m);
extern "C" void *MGame_opnew(unsigned sz);
extern "C" void DialogueWindow_ctor(DialogueWindow *w, Mission *m, Level *lvl, int z);
extern "C" void PlayerEgo_setTurretMode(PlayerEgo *p, int on);
extern "C" void LevelScript_resetCamera(Level *lvl);
extern "C" void PlayerEgo_setFreeLookMode(PlayerEgo *p, int on);
extern "C" void TFC_enableFirstPersonCam(TargetFollowCamera *c, int on);
extern "C" void PlayerEgo_hideShipForFirstPersonCameraView(PlayerEgo *p, int on);
extern "C" void MGame_pauseSounds(MGame *self);

// MGame::dialogueEvent(): when a level sequence completes, decide whether to raise a
// briefing/mission dialogue, and if so set up the dialogue window + first-person view.
extern "C" void MGame_dialogueEvent(MGame *self) {
    if (LevelScript_startSequenceOver(F<Level *>(self, 0x7c)) == 0) return;
    Status **sp = g_status;
    if (DialogueWindow_hasBriefingDialogue(Status_getCurrentCampaignMission(*sp)) == 0) {
        if (Mission_isCampaignMission(Status_getMission(*sp)) != 0) return;
    }
    if (Mission_isEmpty(Status_getMission(*sp)) != 0) return;
    if (Mission_getType(Status_getMission(*sp)) == 8) return;
    if (Mission_getType(Status_getMission(*sp)) == 0xa6) return;
    if (Mission_getType(Status_getMission(*sp)) == 0) return;
    if (Mission_getType(Status_getMission(*sp)) == 0xb7) return;
    if (Mission_isVisible(Status_getMission(*sp)) == 0) return;
    if (Mission_isCampaignMission(Status_getMission(*sp)) == 0) {
        if (Mission_getType(Status_getMission(*sp)) == 0xb) return;
    }
    if (F<DialogueWindow *>(self, 0x8c) == 0) {
        DialogueWindow *w = (DialogueWindow *)MGame_opnew(0x74);
        DialogueWindow_ctor(w, Status_getMission(*sp), F<Level *>(self, 0x78), 0);
        F<DialogueWindow *>(self, 0x8c) = w;
    }
    PlayerEgo_setTurretMode(F<PlayerEgo *>(self, 0x58), 0);
    LevelScript_resetCamera(F<Level *>(self, 0x7c));
    PlayerEgo_setFreeLookMode(F<PlayerEgo *>(self, 0x58), 0);
    TFC_enableFirstPersonCam(F<TargetFollowCamera *>(self, 0xf4), 0);
    PlayerEgo_hideShipForFirstPersonCameraView(F<PlayerEgo *>(self, 0x58), 0);
    Level *cam = F<Level *>(self, 0x7c);
    F<uint8_t>(self, 0x111) = 1;
    F<int>((MGame *)cam, 0x8) = 0;
    F<int>((MGame *)cam, 0xc) = 0;
    F<uint8_t>(self, 0x5d) = 1;
    MGame_pauseSounds(self);
    F<uint8_t>(self, 0x5e) = 1;
}
