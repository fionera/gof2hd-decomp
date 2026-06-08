#include "class.h"

struct Level;
struct PlayerEgo;
struct Player;
struct Ship;
struct Station;
struct Item;
struct Engine;

extern "C" int Status_inAlienOrbit();                                    // 0x723d0
extern "C" void Status_getSystem();
extern "C" int SolarSystem_getTextureIndex(void *ss);
extern "C" void PaintCanvas_TextureCreate(unsigned a, unsigned *b, int c); // TextureCreate
extern "C" void PaintCanvas_ChangeCubeTexture(unsigned tex);
extern "C" void Globals_startNewSoundResourceList();
extern "C" void Globals_addSoundResourceToList(int list);
extern "C" int Status_getWingmen();
extern "C" int Status_getCurrentCampaignMission();                       // 0x71770
extern "C" int SolarSystem_currentOrbitHasWarpGate(void *ss);
extern "C" void Status_checkForLevelUp();
extern "C" void *operator_new(unsigned sz);                              // 0x6eb24
extern "C" void operator_delete(void *p);                                // 0x6eb48
extern "C" void Level_ctor(Level *l, int a);
extern "C" int Level_init(Level *l);
extern "C" void MGame_reset(MGame *self);                                // 0x178470
extern "C" void Player_setHitpoints(Player *p, int hp);
extern "C" void Player_setShieldHP(Player *p, int hp);
extern "C" void Player_setArmorHP(Player *p, int hp);
extern "C" void Player_setGammaHP(Player *p, int hp);
extern "C" void PlayerEgo_resetLastHP(PlayerEgo *p);
extern "C" Station *Status_getStation();                                 // 0x71c14
extern "C" int Station_getIndex(Station *s);                             // 0x71824
extern "C" unsigned ApplicationManager_GetCurrentTimeMillis();           // 0x713d4
extern "C" int Radar_hasScanner();
extern "C" int Status_getShip();                                         // 0x71a58? getShip
extern "C" int Ship_getFirstEquipmentOfSort(int ship);
extern "C" int Item_getAttribute(int item);
extern "C" void Hud_setTimeExtender(void *hud, int a, int b, int c);
extern "C" int Status_inBlackMarketSystem();
extern "C" int Level_getEnemies(Level *l);
extern "C" void Level_createRadioMessage(int level, int id);
extern "C" int Status_dlc1Won();
// Heavily-corrupt sub-blocks are delegated to documented helpers:
extern "C" void MGame_loadSoundResources(MGame *self);    // sound-resource list (62..145)
extern "C" void MGame_restorePlayerStats(MGame *self);    // HP/shield/armor restore (157..198)
extern "C" void MGame_setupWeaponsAndAudio(MGame *self);  // weapon + music init (315..end)

__attribute__((visibility("hidden"))) extern int *g_initGuard;   // @0x187c60 (stack guard [0])
__attribute__((visibility("hidden"))) extern int *g_initStatus;  // @0x187e46 (*piVar17)
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// MGame::OnInitialize(): bootstrap a flight session (level, player, sounds, state).
extern "C" void MGame_OnInitialize(MGame *self) {
    int *guard = g_initGuard;
    int g0 = *guard;

    F<uint8_t>(self, 0x1ec) = 0;   // this[2].field_4C low byte
    Level *level = F<Level *>(self, 0x78);
    F<int>(self, 0xc) = 100;

    if (level == 0) {
        // Create skybox cube texture from the current system / alien orbit.
        unsigned texSel;
        if (Status_inAlienOrbit() == 0) {
            Status_getSystem();
            int ti = SolarSystem_getTextureIndex(0);
            texSel = (ti + 0x2efe) & 0xffff;
        } else {
            texSel = 0x2f08;
        }
        PaintCanvas_TextureCreate(F<unsigned>(self, 0x4), &texSel, 0);
        PaintCanvas_ChangeCubeTexture(F<unsigned>(self, 0x4));

        // Build the per-session sound-resource list (long sequential block).
        MGame_loadSoundResources(self);

        Status_checkForLevelUp();
        level = (Level *)operator_new(0x2a0);
        Level_ctor(level, 3);
        F<Level *>(self, 0x78) = level;
    }

    if (Level_init(level) == 0) {
        F<int>(self, 0xc) = 100;
        if (*guard != g0) __stack_chk_fail();
        return;
    }

    MGame_reset(self);

    int *status = g_initStatus;
    MGame_restorePlayerStats(self);
    PlayerEgo_resetLastHP(F<PlayerEgo *>(self, 0x58));

    // Per-mission HP cap and alien-orbit station bookkeeping.
    if (Status_getCurrentCampaignMission() != 0x5f) {
        // (Ship max-HP restore delegated within restorePlayerStats variant)
    }
    if (Status_inAlienOrbit() == 0)
        *(int *)((char *)*status + 0x84) = Station_getIndex(Status_getStation());

    unsigned t = ApplicationManager_GetCurrentTimeMillis();
    F<int>(self, 0x1ac) = 0;   // this[1].field_A4
    F<int>(self, 0x1b4) = 0;   // this[1].field_AC
    F<unsigned>(self, 0x20) = t & 0xffff;
    F<unsigned>(self, 0x24) = 0;
    F<unsigned>(self, 0x28) = t & 0xffff;
    F<unsigned>(self, 0x2c) = 0;

    if (Radar_hasScanner() != 0)
        *(int *)((char *)*status + 0x11c) = 0;
    *(int *)((char *)*status + 300) = 0;
    *(int *)((char *)*status + 0x134) = 0;
    *(int *)((char *)*status + 0x13c) = 0;
    *(int *)((char *)*status + 0x144) = 0;

    int eq = Ship_getFirstEquipmentOfSort(Status_getShip());
    if (eq != 0) {
        F<int>(self, 0x1b4) = Item_getAttribute(eq);
        F<int>(self, 0x1b0) = Item_getAttribute(eq);
        Hud_setTimeExtender(F<void *>(self, 0x74), 1, 0, 1);
    }

    // Alien-orbit / black-market radio messages.
    if (Status_dlc1Won() != 0 && Status_inAlienOrbit() != 0 &&
        Status_getCurrentCampaignMission() < 0x93) {
        if (F<int>(self, 0x58) != 0 && F<int>(self, 0x84) != 0)
            *(int *)((char *)F<int>(self, 0x58) + 0x18) = F<int>(self, 0x84);
        Level_createRadioMessage(F<int>(self, 0x78), 8);
    }

    if (Status_inBlackMarketSystem() == 0) {
        *(uint16_t *)((char *)*status + 0x110) = 0;
    } else {
        if (F<int>(self, 0x58) != 0 && F<int>(self, 0x84) != 0)
            *(int *)((char *)F<int>(self, 0x58) + 0x18) = F<int>(self, 0x84);
        if (*(uint8_t *)((char *)*status + 0x110) == 0) {
            int id;
            int lvl;
            if (*(uint8_t *)((char *)*status + 0x111) == 0) {
                int enemies = Level_getEnemies(F<Level *>(self, 0x78));
                if (enemies != 0) {
                    int n = *(int *)enemies;
                    for (int i = 0; i < n; i++) {
                        int e = *(int *)(*(int *)((char *)enemies + 4) + i * 4);
                        if (*(int *)((char *)e + 0x28) == 8)
                            *(uint8_t *)((char *)e + 0x25) = 0;
                    }
                }
                lvl = F<int>(self, 0x78);
                id = 9;
            } else {
                lvl = F<int>(self, 0x78);
                id = 0xd;
            }
            Level_createRadioMessage(lvl, id);
        } else {
            int enemies = Level_getEnemies(F<Level *>(self, 0x78));
            if (enemies != 0) {
                int n = *(int *)enemies;
                for (int i = 0; i < n; i++) {
                    int e = *(int *)(*(int *)((char *)enemies + 4) + i * 4);
                    if (*(int *)((char *)e + 0x28) == 8)
                        *(uint8_t *)((char *)e + 0x25) = 0;
                }
            }
        }
    }

    // Weapon selection, particle effects, audio and music init.
    MGame_setupWeaponsAndAudio(self);

    if (*guard != g0)
        __stack_chk_fail();
}
