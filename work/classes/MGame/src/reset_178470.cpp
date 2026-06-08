#include "class.h"

struct Hud;
struct Radio;
struct Radar;
struct Level;
struct LevelScript;
struct TargetFollowCamera;
struct ChoiceWindow;
struct PlayerEgo;
struct Mission;
struct PaintCanvas;

extern "C" int Level_getPlayer(MGame *self);                              // 0x72034
extern "C" void *operator_new(unsigned sz);                              // 0x6eb24
extern "C" void operator_delete(void *p);                                // 0x6eb48
extern "C" void Hud_ctor(Hud *h);                                        // 0x78118
extern "C" void Radio_ctor(Radio *r);                                    // 0x75160
extern "C" void *Level_getMessages(Level *l);                            // 0x71ef0
extern "C" void Radio_setMessages(Radio *r, void *msgs);                 // 0x74488
extern "C" void PaintCanvas_CameraCreate(PaintCanvas *pc, unsigned *out); // 0x72058
extern "C" int Status_inAlienOrbit();                                    // 0x723d0
extern "C" int Status_getCurrentCampaignMission();                       // 0x71770
extern "C" void PaintCanvas_CameraSetPerspective(unsigned cam, float fov, float a, float b); // 0x72004
extern "C" void *TargetFollowCamera_dtor(TargetFollowCamera *c);         // 0x72064
extern "C" void TargetFollowCamera_ctor(TargetFollowCamera *c, int cam, int target,
                                        int a, int b, int d, int e, int f, int g); // 0x78124
extern "C" void PaintCanvas_CameraSetCurrent(PaintCanvas *pc, unsigned cam); // 0x6fd9c
extern "C" void PlayerEgo_setTargetFollowCamera(PlayerEgo *p, TargetFollowCamera *c); // 0x78130
extern "C" void TargetFollowCamera_resetShipHandling(TargetFollowCamera *c); // 0x72a18
extern "C" void Radar_ctor(Radar *r, Level *l);                          // 0x7813c
extern "C" Mission *Status_getMission();                                 // 0x72850
extern "C" int Mission_isCampaignMission(Mission *m);                    // 0x7372c
extern "C" void LevelScript_ctor(LevelScript *s, Hud *hud, Level *level,
                                 Radar *radar, TargetFollowCamera *cam);  // 0x78148
extern "C" void LevelScript_lookBehind(LevelScript *s, int a);           // 0x7246c (resetCamera shares 0x7246c? see disasm)
extern "C" void Level_initParticleSystems(Level *l);                     // 0x7204c
extern "C" void ChoiceWindow_ctor(ChoiceWindow *w);                      // 0x74584
extern "C" unsigned ApplicationManager_GetCurrentTimeMillis();           // 0x713d4

__attribute__((visibility("hidden"))) extern int g_resAspectA;    // @0x1886dc (DAT)
__attribute__((visibility("hidden"))) extern int g_resAspectB;    // @0x1886e0
__attribute__((visibility("hidden"))) extern int g_resAspectC;    // @0x1886e4
__attribute__((visibility("hidden"))) extern PaintCanvas **g_resCanvas; // @0x1884ea
__attribute__((visibility("hidden"))) extern int *g_resStatus;    // @0x1884fe (*piVar15)
__attribute__((visibility("hidden"))) extern int g_resInitB;      // @0x1886fc
__attribute__((visibility("hidden"))) extern int **g_resShipTune; // @0x188608 ([0][0x2f4])
__attribute__((visibility("hidden"))) extern uint8_t **g_resPauseSrc; // @0x188684

// MGame::reset(): rebuild all per-session subsystems and reset transient state.
extern "C" void MGame_reset(MGame *self) {
    F<int>(self, 0xbc) = 0; F<int>(self, 0xc0) = 0;
    F<int>(self, 0x98) = 0; F<int>(self, 0x9c) = 0;
    F<int>(self, 0xa0) = 0;
    F<int>(self, 0x154) = 0; F<int>(self, 0x158) = 0;
    F<int>(self, 0x138) = 0;
    F<uint8_t>(self, 0x15c) = 0;
    F<int>(self, 0x118) = g_resAspectC;
    F<int>(self, 0x11c) = g_resAspectA;
    F<int>(self, 0x120) = g_resAspectB;
    F<int>(self, 0x124) = 0; F<int>(self, 0x128) = 0;
    F<int>(self, 300) = 0;   // 0x12c
    F<int>(self, 0x130) = 0; F<int>(self, 0x134) = 0;

    F<int>(self, 0x58) = Level_getPlayer(self);

    Hud *hud = (Hud *)operator_new(0x53c);
    Hud_ctor(hud);
    F<Hud *>(self, 0x74) = hud;

    Radio *radio = (Radio *)operator_new(0x48);
    Radio_ctor(radio);
    F<Radio *>(self, 0x84) = radio;
    Radio_setMessages(radio, Level_getMessages(F<Level *>(self, 0x78)));

    PaintCanvas *pc = *g_resCanvas;
    PaintCanvas_CameraCreate(pc, (unsigned *)((char *)self + 0xf0));
    unsigned cam = *(unsigned *)g_resCanvas;
    int *status = g_resStatus;

    float fov = *(float *)&g_resAspectC;  // DAT_001886e8 default
    if (Status_inAlienOrbit() != 0) {
        int cm = Status_getCurrentCampaignMission();
        fov = (cm < 0x50) ? *(float *)&g_resInitB : *(float *)&g_resAspectC;
    }
    PaintCanvas_CameraSetPerspective(cam, fov, 0, 0);

    if (F<TargetFollowCamera *>(self, 0xf4) != 0) {
        operator_delete(TargetFollowCamera_dtor(F<TargetFollowCamera *>(self, 0xf4)));
        F<int>(self, 0xf4) = 0;
    }
    TargetFollowCamera *tfc = (TargetFollowCamera *)operator_new(0x178);
    TargetFollowCamera_ctor(tfc, F<int>(self, 0xf0),
                            *(int *)((char *)F<int>(self, 0x58) + 8), 0, 0, 0, 0, 0, 0);
    F<TargetFollowCamera *>(self, 0xf4) = tfc;
    PaintCanvas_CameraSetCurrent(pc, F<unsigned>(self, 0xf0));
    PlayerEgo_setTargetFollowCamera(F<PlayerEgo *>(self, 0x58), F<TargetFollowCamera *>(self, 0xf4));
    TargetFollowCamera_resetShipHandling(F<TargetFollowCamera *>(self, 0xf4));

    Radar *radar = (Radar *)operator_new(0x248);
    Radar_ctor(radar, F<Level *>(self, 0x78));
    F<Radar *>(self, 0x80) = radar;

    if (Status_getMission() != 0)
        F<uint8_t>(self, 0x61) = (uint8_t)Mission_isCampaignMission(Status_getMission());

    LevelScript *script = (LevelScript *)operator_new(0xe8);
    LevelScript_ctor(script, F<Hud *>(self, 0x74), F<Level *>(self, 0x78),
                     F<Radar *>(self, 0x80), F<TargetFollowCamera *>(self, 0xf4));
    F<LevelScript *>(self, 0x7c) = script;
    LevelScript_lookBehind(script, F<int>(self, 0x78));
    Level_initParticleSystems(F<Level *>(self, 0x78));

    ChoiceWindow *cw = (ChoiceWindow *)operator_new(0x5c);
    ChoiceWindow_ctor(cw);
    F<ChoiceWindow *>(self, 0x94) = cw;

    F<int>(self, 0x48) = 0; F<int>(self, 0x4c) = 0;
    F<int>(self, 0x14) = 0;
    F<uint8_t>(self, 0x18) = 0;
    F<uint16_t>(self, 0x5c) = 0;
    F<uint16_t>(self, 0x5f) = 0;
    F<uint16_t>(self, 0xc4) = 0;
    F<uint8_t>(self, 0xc6) = 0;
    F<uint16_t>(self, 0xc8) = 0;
    F<uint8_t>(self, 0xe0) = 0;
    F<uint8_t>(self, 0x15e) = 0;
    F<uint8_t>(self, 0x110) = 0;
    F<int>(self, 0x168) = 0x3f800000;
    F<int>(self, 0x1b0) = 0;
    F<int>(self, 0x1b4) = g_resInitB;
    F<int>(self, 0x100) = 0;
    F<int>(self, 0x104) = 0; F<int>(self, 0x108) = 0; F<int>(self, 0x10c) = 0;
    F<uint16_t>(self, 0x1b8) = 0;
    F<int>(self, 0x1cc) = (*g_resShipTune)[0x2f4 / 4];
    F<int>(self, 0x1d0) = 0;
    F<int>(self, 0x1e8) = 0;
    F<int>(self, 0x178) = 0;
    F<uint8_t>(self, 0x17c) = 0;
    F<int>(self, 0x180) = 0; F<int>(self, 0x184) = 0;

    unsigned t = ApplicationManager_GetCurrentTimeMillis();
    F<uint16_t>(self, 0x111) = 0x101;
    F<uint8_t>(self, 0xcf) = 0;
    F<int>(self, 0xd0) = 0;
    F<uint8_t>(self, 0xd4) = 0;
    F<uint16_t>(self, 0x1e4) = 0;
    F<uint8_t>(self, 0x1e6) = 0;
    F<unsigned>(self, 0x20) = t & 0xffff;
    F<unsigned>(self, 0x24) = 0;
    F<unsigned>(self, 0x28) = t & 0xffff;
    F<unsigned>(self, 0x2c) = 0;
    F<uint8_t>(self, 0x1dd) = **g_resPauseSrc;

    int s = *status;
    *(int *)((char *)s + 0x184) = 0;
    *(int *)((char *)s + 0x188) = 1;
    *(int *)((char *)s + 0x18c) = 1;
}
