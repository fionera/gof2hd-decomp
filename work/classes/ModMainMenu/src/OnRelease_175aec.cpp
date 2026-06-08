#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseCanvas;
__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_releaseFontObj;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseReload;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseImageFactory;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_releaseSound;

extern "C" void *CutScene_dtor(void *self);
extern "C" void *MenuTouchWindow_dtor(void *self);
extern "C" void operator_delete(void *ptr);
extern "C" void PaintCanvas_ReleaseAllResources(void *canvas);
extern "C" int GameText_getLanguage();
extern "C" void Globals_loadFont(int obj, int language);
extern "C" void Layout_reload(void *layout);
extern "C" void ImageFactory_reload(void *factory);
extern "C" void Layout_initTip(void *layout);
extern "C" void ModMainMenu_releaseTail(void *sound);

extern "C" void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self)
{
    void *cutscene = self->f_1c;
    if (cutscene != 0)
        operator_delete(CutScene_dtor(cutscene));

    void *touchWindow = self->f_18;
    self->f_1c = 0;
    if (touchWindow != 0)
        operator_delete(MenuTouchWindow_dtor(touchWindow));

    self->f_18 = 0;
    PaintCanvas_ReleaseAllResources(*g_ModMainMenu_releaseCanvas);

    int fontObj = *g_ModMainMenu_releaseFontObj;
    Globals_loadFont(fontObj, GameText_getLanguage());

    void **reload = g_ModMainMenu_releaseReload;
    if (*reload != 0) {
        Layout_reload(*reload);
        ImageFactory_reload(*g_ModMainMenu_releaseImageFactory);
        Layout_initTip(*reload);
    }

    void *sound = *g_ModMainMenu_releaseSound;
    if (sound != 0)
        ModMainMenu_releaseTail(sound);
}
