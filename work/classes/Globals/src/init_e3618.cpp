#include "class.h"

extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void Mission_ctor(void *m);
extern "C" void Galaxy_ctor(void *g);
extern "C" void Achievements_ctor(void *a);
extern "C" void Status_ctor(void *s);
extern "C" void ImageFactory_ctor(void *f);
extern "C" void FileRead_ctor(void *f);
extern "C" int FileRead_loadItemsBinary();
extern "C" int FileRead_loadShipsBinary();
extern "C" void *FileRead_dtor(void *f);
extern "C" void ApplicationManager_VibrateEnable(void *am, int on);
extern "C" void AERandom_ctor(void *r);
extern "C" void Generator_ctor(void *g);
extern "C" void RecordHandler_ctor(void *rh);
extern "C" void Status_resetGame();
extern "C" void RecordHandler_loadOptions(void *rh);
extern "C" void FModSound_ctor(void *s);
extern "C" void FModSound_init(void *s, void *engine);
extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void Layout_ctor(void *l);
extern "C" void Layout_reload();
extern "C" void ParticleSettingsRef_initialize();
extern "C" void ArrayInt_ctor(void *a);

// Hidden PC-relative globals.
extern int **const gI_mission __attribute__((visibility("hidden")));     // DAT_000f38c8
extern void **const gI_settings __attribute__((visibility("hidden")));   // DAT_000f38cc (puVar14)
extern int **const gI_flagFFFF __attribute__((visibility("hidden")));    // DAT_000f38d0 (puVar6)
extern int **const gI_uVar7slot __attribute__((visibility("hidden")));   // DAT_000f38d4 (puVar10)
extern char **const gI_langFlag __attribute__((visibility("hidden")));   // DAT_000f38d8 (pcVar11)
extern char **const gI_zeroByte __attribute__((visibility("hidden")));   // DAT_000f38dc (puVar12)
extern void ***const gI_galaxy __attribute__((visibility("hidden")));    // DAT_000f38e0
extern void ***const gI_achieve __attribute__((visibility("hidden")));   // DAT_000f38e4
extern void ***const gI_status __attribute__((visibility("hidden")));    // DAT_000f38e8
extern void ***const gI_imgFac __attribute__((visibility("hidden")));    // DAT_000f38ec
extern int ***const gI_items __attribute__((visibility("hidden")));      // DAT_000f38f0
extern int ***const gI_ships __attribute__((visibility("hidden")));      // DAT_000f38f4
extern int **const gI_engineSlot __attribute__((visibility("hidden")));  // DAT_000f38f8
extern void ***const gI_appMgr __attribute__((visibility("hidden")));    // DAT_000f38fc
extern void ***const gI_ctxSlot __attribute__((visibility("hidden")));   // DAT_000f3900
extern void ***const gI_random __attribute__((visibility("hidden")));    // DAT_000f3904
extern void ***const gI_generator __attribute__((visibility("hidden"))); // DAT_000f3908
extern void ***const gI_recHandler __attribute__((visibility("hidden")));// DAT_000f390c
extern void ***const gI_fmod __attribute__((visibility("hidden")));      // DAT_000f3910
extern void **const gI_setMusVol __attribute__((visibility("hidden")));  // DAT_000f3914 (fn ptr)
extern void **const gI_setSfxVol __attribute__((visibility("hidden")));  // DAT_000f3918 (fn ptr)
extern int ***const gI_g381c __attribute__((visibility("hidden")));      // DAT_000f391c
extern char **const gI_g381a __attribute__((visibility("hidden")));      // DAT_000f3920 (puVar12)
extern int ***const gI_g381e __attribute__((visibility("hidden")));      // DAT_000f3924 (puVar6 second)
extern int **const gI_g3822 __attribute__((visibility("hidden")));       // DAT_000f3928 (puVar10)
extern char ***const gI_g3824 __attribute__((visibility("hidden")));     // DAT_000f392c (puVar17)
extern char **const gI_g383a __attribute__((visibility("hidden")));      // DAT_000f3930 (puVar16)
extern void ***const gI_layout __attribute__((visibility("hidden")));    // DAT_000f3934

typedef void (*VolFn)(void *snd, int channel, int value);

// Globals::init(ApplicationManager* app, Engine* engine)
// self in r0, app in r1.
extern "C" int Globals_init(Globals *self, void *app)
{
    int *missionSlot = *gI_mission;
    if (*missionSlot == 0) {
        void *m = operator_new(0x78);
        Mission_ctor(m);
        *missionSlot = (int)m;
    }

    int *settings = (int *)*gI_settings;
    int *flagFFFF = (int *)*gI_flagFFFF;
    int *uVar7slot = (int *)*gI_uVar7slot;
    char *langFlag = *gI_langFlag;
    char *zeroByte = *gI_zeroByte;

    char *s = (char *)settings;
    *(unsigned char *)(s + 0x11) = 1;
    *(unsigned char *)(s + 0x30) = 1;
    *(float *)(s + 0x00) = 0.5f;
    *(float *)(s + 0x04) = 0.5f;
    *(float *)(s + 0x08) = 0.5f;
    *(float *)(s + 0x24) = 0.5f;
    *(float *)(s + 0x28) = 1.0f;
    *zeroByte = 0;
    char lang = *langFlag;
    *(short *)(s + 0x0f) = 0x101;
    *flagFFFF = -1;
    *uVar7slot = (lang == 0) ? 6 : 0xc;

    void *galaxy = operator_new(8);
    Galaxy_ctor(galaxy);
    **gI_galaxy = galaxy;
    void *ach = operator_new(0x28);
    Achievements_ctor(ach);
    **gI_achieve = ach;
    void *status = operator_new(0x1f0);
    Status_ctor(status);
    **gI_status = status;
    void *imgFac = operator_new(0xc);
    ImageFactory_ctor(imgFac);
    **gI_imgFac = imgFac;

    void *fr = operator_new(1);
    FileRead_ctor(fr);
    **gI_items = (int *)(long)FileRead_loadItemsBinary();
    **gI_ships = (int *)(long)FileRead_loadShipsBinary();
    operator_delete(FileRead_dtor(fr));

    int *engineSlot = *gI_engineSlot;
    if (*engineSlot == 0) {
        *engineSlot = *(int *)app;
    }
    **gI_appMgr = app;
    ApplicationManager_VibrateEnable(app, 0);

    void *rng = operator_new(8);
    AERandom_ctor(rng);
    **gI_ctxSlot = self;
    **gI_random = (void *)rng;

    void *gen = operator_new(1);
    Generator_ctor(gen);
    **gI_generator = gen;

    void *rh = operator_new(0x2c);
    RecordHandler_ctor(rh);
    void **rhSlotP = *gI_recHandler;
    *rhSlotP = rh;
    Status_resetGame();
    RecordHandler_loadOptions(*rhSlotP);

    void *fmod = operator_new(0x243c);
    FModSound_ctor(fmod);
    void **fmodSlotP = *gI_fmod;
    *fmodSlotP = fmod;
    FModSound_init(fmod, 0);

    VolFn setMus = (VolFn)*gI_setMusVol;
    setMus(*fmodSlotP, 1, *(unsigned char *)(s + 0xd));
    setMus(*fmodSlotP, 2, *(unsigned char *)(s + 0xc));
    setMus(*fmodSlotP, 3, *(unsigned char *)(s + 0xe));
    VolFn setSfx = (VolFn)*gI_setSfxVol;
    setSfx(*fmodSlotP, 1, settings[0]);
    setSfx(*fmodSlotP, 2, settings[1]);
    setSfx(*fmodSlotP, 3, settings[2]);

    if (FModSound_tryToStopMusicForBGMusic() != 0) {
        *(unsigned char *)(s + 0xd) = 0;
    }

    // Camera/transform-style zeroing of a vector-triple struct (puVar6 second object).
    **gI_g381c = 0;
    **gI_g381a = 1;
    int *obj = (int *)*gI_g381e;
    obj[0] = 0; obj[1] = 0; obj[2] = 0; obj[3] = 0;
    obj[4] = 0; obj[5] = 0; obj[6] = 0; obj[7] = 0;
    obj[8] = 0; obj[9] = 0; obj[10] = 0; obj[11] = 0;
    *(int *)((char *)obj + 0x2b) = 0;
    *(int *)((char *)obj + 0x2f) = 0;
    *(int *)((char *)obj + 0x33) = 0;
    *(int *)((char *)obj + 0x37) = 0;
    *(char *)*(int **)gI_g3822 = 0;
    **gI_g3824 = 0;
    **gI_g383a = 0;

    void *layout = operator_new(0x414);
    Layout_ctor(layout);
    **gI_layout = layout;
    Layout_reload();
    ParticleSettingsRef_initialize();

    void *arr = operator_new(0xc);
    ArrayInt_ctor(arr);
    F<void *>(self, 4) = arr;
    return (int)(long)arr;
}
