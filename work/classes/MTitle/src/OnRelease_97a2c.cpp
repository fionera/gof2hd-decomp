#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_MTitle_or_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_font;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_reload;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_imgfac;

extern "C" void PaintCanvas_ReleaseAllResources(void *canvas);
extern "C" int GameText_getLanguage();
extern "C" void Globals_loadFont(void *obj, int lang);
extern "C" void Layout_reload(void *p);
extern "C" void ImageFactory_reload(void *p);
extern "C" void MTitle_or_tail(void *p);

void MTitle::OnRelease()
{
    PaintCanvas_ReleaseAllResources(*g_MTitle_or_canvas);

    void *fontObj = *g_MTitle_or_font;
    Globals_loadFont(fontObj, GameText_getLanguage());

    void **reload = g_MTitle_or_reload;
    if (*reload != 0) {
        Layout_reload(*reload);
        ImageFactory_reload(*g_MTitle_or_imgfac);
        MTitle_or_tail(*reload);
    }
}
