#include "class.h"

// Two singleton holders (single pc-rel deref each).
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_sound;   // *g -> flag
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_canvas;  // *g -> canvas
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_lang;    // *g -> obj (font lang)
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_reload;  // *g -> reload flag obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_imgfac;  // *g -> image factory

extern "C" void FModSound_disableReverb();
extern "C" void FModSound_stopAllSoundFXEvents();
extern "C" void PaintCanvas_FogEnable(void *c, int a, int b);
extern "C" void PaintCanvas_ReleaseAllResources();
extern "C" int GameText_getLanguage();
extern "C" void Globals_loadFont(int obj, int lang);
extern "C" void Layout_reload(void *p);
extern "C" void ImageFactory_reload(void *p);
extern "C" void Layout_initTip(void *p);

extern "C" void *ms_op_delete(void *p);
extern "C" void *ArrayReleaseClasses_TouchButton(void *a);

extern "C" void *Array_TB_dtor(void *p);
extern "C" void *HangarWindow_dtor(void *p);
extern "C" void *StarMap_dtor(void *p);
extern "C" void *SpaceLounge_dtor(void *p);
extern "C" void *MissionsWindow_dtor(void *p);
extern "C" void *DialogueWindow_dtor(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void *StatusWindow_dtor(void *p);
extern "C" void *CutScene_dtor(void *p);
extern "C" void *MenuTouchWindow_dtor(void *p);
extern "C" void *Radio_dtor(void *p);
extern "C" void *Array_RM_dtor(void *p);
extern "C" void *NewsTicker_dtor(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void *ScrollTouchBox_dtor(void *p);

// Tail veneer at 0x1ac168.
extern "C" void ModStation_or_tail();

// ModStation::OnRelease()
extern "C" void ModStation_OnRelease(ModStation *self)
{
    void **soundHolder = g_ModStation_or_sound;
    if (*soundHolder != 0) {
        FModSound_disableReverb();
        FModSound_stopAllSoundFXEvents();
    }
    PaintCanvas_FogEnable(*g_ModStation_or_canvas, 0, 1);

    if (P(self, 0x8c) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x8c));
        if (P(self, 0x8c) != 0)
            ms_op_delete(Array_TB_dtor(P(self, 0x8c)));
    }
    P(self, 0x8c) = 0;

    if (P(self, 0x78) != 0)
        ms_op_delete(HangarWindow_dtor(P(self, 0x78)));
    P(self, 0x78) = 0;

    if (P(self, 0x10) != 0)
        ms_op_delete(StarMap_dtor(P(self, 0x10)));
    P(self, 0x10) = 0;

    if (P(self, 0x74) != 0)
        ms_op_delete(SpaceLounge_dtor(P(self, 0x74)));
    P(self, 0x74) = 0;

    if (P(self, 0x80) != 0)
        ms_op_delete(MissionsWindow_dtor(P(self, 0x80)));
    P(self, 0x80) = 0;

    if (P(self, 0x84) != 0)
        ms_op_delete(DialogueWindow_dtor(P(self, 0x84)));
    P(self, 0x84) = 0;

    if (P(self, 0x88) != 0)
        ms_op_delete(ChoiceWindow_dtor(P(self, 0x88)));
    P(self, 0x88) = 0;

    if (P(self, 0x7c) != 0)
        ms_op_delete(StatusWindow_dtor(P(self, 0x7c)));
    P(self, 0x7c) = 0;

    if (P(self, 0x14) != 0)
        ms_op_delete(CutScene_dtor(P(self, 0x14)));
    P(self, 0x14) = 0;

    if (P(self, 0x50) != 0)
        ms_op_delete(MenuTouchWindow_dtor(P(self, 0x50)));
    P(self, 0x50) = 0;

    if (P(self, 0x54) != 0)
        ms_op_delete(Radio_dtor(P(self, 0x54)));
    P(self, 0x54) = 0;

    if (P(self, 0x58) != 0)
        ms_op_delete(Array_RM_dtor(P(self, 0x58)));
    P(self, 0x58) = 0;

    if (P(self, 0x1c) != 0)
        ms_op_delete(NewsTicker_dtor(P(self, 0x1c)));
    P(self, 0x1c) = 0;

    if (P(self, 0x70) != 0)
        ms_op_delete(ChoiceWindow_dtor(P(self, 0x70)));
    P(self, 0x70) = 0;

    PaintCanvas_ReleaseAllResources();
    int langObj = *(int *)*g_ModStation_or_lang;
    Globals_loadFont(langObj, GameText_getLanguage());

    void **reloadHolder = g_ModStation_or_reload;
    if (*reloadHolder != 0) {
        Layout_reload(*reloadHolder);
        ImageFactory_reload(*g_ModStation_or_imgfac);
        Layout_initTip(*reloadHolder);
    }

    if (P(self, 0x20) != 0)
        ms_op_delete(EaseInOutMatrix_dtor(P(self, 0x20)));
    P(self, 0x20) = 0;

    if (P(self, 0x13c) != 0)
        ms_op_delete(P(self, 0x13c));
    P(self, 0x13c) = 0;

    if (P(self, 0x140) != 0)
        ms_op_delete(P(self, 0x140));
    P(self, 0x140) = 0;

    if (P(self, 0x144) != 0)
        ms_op_delete(P(self, 0x144));
    P(self, 0x144) = 0;

    if (P(self, 0x54) != 0)
        ms_op_delete(Radio_dtor(P(self, 0x54)));
    P(self, 0x54) = 0;

    if (P(self, 0x98) != 0)
        ms_op_delete(ScrollTouchBox_dtor(P(self, 0x98)));
    P(self, 0x98) = 0;

    US(self, 0xb0) = 0;
    if (*soundHolder != 0)
        ModStation_or_tail();
}
