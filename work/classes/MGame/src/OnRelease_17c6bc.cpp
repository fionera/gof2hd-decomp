#include "class.h"

struct Engine;
struct Level;
struct AEGeometry;
struct Hud;
struct LevelScript;
struct StarMap;

extern "C" int ApplicationManager_GetEngine();                            // 0x7258c
extern "C" void Engine_SetPostEffect(Engine *e, int id, int on);          // 0x707f8
extern "C" void FModSound_setDownPitch(int v);                            // 0x74f74
extern "C" void FModSound_disableReverb(Engine *e);                       // 0x75310
extern "C" void FModSound_stopAllSoundFXEvents(Engine *e);                // 0x7531c
extern "C" void *Level_dtor(Level *l);                                    // 0x71fd4
extern "C" void *AEGeometry_dtor(AEGeometry *g);                          // 0x71fc8
extern "C" void *Hud_dtor(Hud *h);                                        // 0x7846c
extern "C" void *LevelScript_dtor(LevelScript *s);                        // 0x78478
extern "C" void *Radar_dtor(void *r);                                     // 0x78484
extern "C" void *Radio_dtor(void *r);                                     // 0x75388
extern "C" int ApplicationManager_GetApplicationModule(int id);           // 0x75bd4
extern "C" void *StarMap_dtor(StarMap *m);                                // 0x75340
extern "C" void *MenuTouchWindow_dtor(void *w);                           // 0x7537c
extern "C" void *DialogueWindow_dtor(void *w);                            // 0x75010
extern "C" void *ChoiceWindow_dtor(void *w);                              // 0x74d88
extern "C" void *TargetFollowCamera_dtor(void *c);                        // 0x72064
extern "C" void *GameRecord_dtor(void *r);                                // 0x765ac
extern "C" void PaintCanvas_ReleaseAllResources(int x);                   // 0x6f964
extern "C" unsigned short GameText_getLanguage();                         // 0x6f544
extern "C" void Globals_loadFont(int font, int lang);                     // 0x71d04
extern "C" void Layout_reload(int layout);                                // 0x71d10
extern "C" void ImageFactory_reload(int factory);                         // 0x71d1c
extern "C" void Layout_initTip2(int layout);                              // 0x71d28
extern "C" void ArrayReleaseClasses_StringPtr(void *arr);                 // 0x6facc
extern "C" void *ArrayStringPtr_dtor(void *arr);                          // 0x6f64c
extern "C" void operator_delete(void *p);                                 // 0x6eb48
// Tail helper @0x1ac168 (re-enables low-pass / restores FMOD state).
extern "C" void FModSound_restoreState();

__attribute__((visibility("hidden"))) extern int g_relPostEffect;   // @0x18c8b8 (DAT)
__attribute__((visibility("hidden"))) extern int *g_relSoundFlag;   // @0x18c6da (*piVar4)
__attribute__((visibility("hidden"))) extern int *g_relAppMod;      // @0x18c792 (*puVar3)
__attribute__((visibility("hidden"))) extern int *g_relFont;        // @0x18c856 (**)
__attribute__((visibility("hidden"))) extern int *g_relLayout;      // @0x18c86a (**)
__attribute__((visibility("hidden"))) extern int **g_relImgFactory; // @0x18c878

// MGame::OnRelease(): tear down the whole game session and reload UI resources.
extern "C" void MGame_OnRelease(MGame *self) {
    Engine *eng = (Engine *)ApplicationManager_GetEngine();
    Engine_SetPostEffect(eng, g_relPostEffect, 0);

    int *soundFlag = g_relSoundFlag;
    if (*soundFlag != 0) {
        FModSound_setDownPitch(0);
        FModSound_disableReverb((Engine *)*soundFlag);
        FModSound_stopAllSoundFXEvents((Engine *)*soundFlag);
    }

    if (F<Level *>(self, 0x78) != 0)
        operator_delete(Level_dtor(F<Level *>(self, 0x78)));

    F<int>(self, 0x48) = 0; F<int>(self, 0x4c) = 0;
    F<int>(self, 0x14) = 0;
    F<int>(self, 0x78) = 0;
    F<uint8_t>(self, 0x54) = 0;
    F<int>(self, 0x70) = 0;
    F<int>(self, 0x40) = 0;
    F<uint16_t>(self, 0x60) = 0;
    F<int>(self, 0x58) = 0; F<int>(self, 0x5c) = 0;
    // NEON-clear 0x30 and 0x20 groups.
    for (unsigned i = 0; i < 4; i++) F<int>(self, 0x30 + i * 4) = 0;
    for (unsigned i = 0; i < 4; i++) F<int>(self, 0x20 + i * 4) = 0;

    if (F<int>(self, 0x114) != 0)
        operator_delete(AEGeometry_dtor(F<AEGeometry *>(self, 0x114)));
    F<int>(self, 0x114) = 0;

    if (F<int>(self, 0x74) != 0)
        operator_delete(Hud_dtor(F<Hud *>(self, 0x74)));
    F<int>(self, 0x74) = 0;

    if (F<int>(self, 0x7c) != 0)
        operator_delete(LevelScript_dtor(F<LevelScript *>(self, 0x7c)));
    F<int>(self, 0x7c) = 0;

    if (F<int>(self, 0x80) != 0)
        operator_delete(Radar_dtor(F<void *>(self, 0x80)));
    F<int>(self, 0x80) = 0;

    if (F<int>(self, 0x84) != 0)
        operator_delete(Radio_dtor(F<void *>(self, 0x84)));
    F<int>(self, 0x84) = 0;

    int *appMod = g_relAppMod;
    int m1 = ApplicationManager_GetApplicationModule(*appMod);
    if (*(int *)((char *)m1 + 0x10) != 0) {
        int m2 = ApplicationManager_GetApplicationModule(*appMod);
        StarMap *sm = *(StarMap **)((char *)m2 + 0x10);
        if (sm != 0)
            operator_delete(StarMap_dtor(sm));
    }
    int m3 = ApplicationManager_GetApplicationModule(*appMod);
    *(int *)((char *)m3 + 0x10) = 0;

    if (F<int>(self, 0x88) != 0)
        operator_delete(MenuTouchWindow_dtor(F<void *>(self, 0x88)));
    F<int>(self, 0x88) = 0;

    if (F<int>(self, 0x8c) != 0)
        operator_delete(DialogueWindow_dtor(F<void *>(self, 0x8c)));
    F<int>(self, 0x8c) = 0;

    if (F<int>(self, 0x90) != 0)
        operator_delete(StarMap_dtor(F<StarMap *>(self, 0x90)));
    F<int>(self, 0x90) = 0;

    if (F<int>(self, 0x94) != 0)
        operator_delete(ChoiceWindow_dtor(F<void *>(self, 0x94)));
    F<int>(self, 0x94) = 0;

    F<int>(self, 0xd8) = 0; F<int>(self, 0xdc) = 0;
    F<uint16_t>(self, 0xd5) = 0;
    F<int>(self, 0xc5) = 0;
    F<int>(self, 0xcb) = 0;

    if (F<int>(self, 0xf4) != 0)
        operator_delete(TargetFollowCamera_dtor(F<void *>(self, 0xf4)));
    F<int>(self, 0xf4) = 0;

    if (F<int>(self, 0x1e8) != 0)
        operator_delete(GameRecord_dtor(F<void *>(self, 0x1e8)));
    F<int>(self, 0x1e8) = 0;

    // GetApplicationData()->[0] : release-all-resources target.
    {
        int *p = (int *)0; (void)p;
    }
    PaintCanvas_ReleaseAllResources(0);

    int font = **(int **)&g_relFont;
    int lang = GameText_getLanguage();
    Globals_loadFont(font, lang);

    int *layout = g_relLayout;
    if (*layout != 0) {
        Layout_reload(*layout);
        ImageFactory_reload(**g_relImgFactory);
        Layout_initTip2(*layout);
    }

    ArrayReleaseClasses_StringPtr(F<void *>(self, 0x1ec));
    if (F<int>(self, 0x1ec) != 0)
        operator_delete(ArrayStringPtr_dtor(F<void *>(self, 0x1ec)));
    F<int>(self, 0x1ec) = 0;

    if (*soundFlag != 0)
        FModSound_restoreState();
}
