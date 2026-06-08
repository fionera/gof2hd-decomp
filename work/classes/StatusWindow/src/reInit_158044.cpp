#include "class.h"

extern "C" {
void *ImageFactory_loadChar(void *factory, int *def);
void *Achievements_get();            // *(DAT_168150): achievements singleton (via getter at DAT_168154)
int Achievements_isUnlocked(void *ach, int index);
void PaintCanvas_setImage2D(void *canvas, int imageId, void *out);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);

extern void *g_sw_imageFactory;   // **(DAT_168148): image factory root
extern int   g_sw_charDef;        // DAT_16814c: character definition arg
extern void *g_sw_achievements;   // *(DAT_168150): achievements accessor base
extern void *g_sw_canvas;         // *(DAT_168160): paint canvas singleton
}

// StatusWindow::reInit() -- rebuild the four medal/rank image tiles from achievement state.
extern "C" void StatusWindow_reInit(StatusWindow *self)
{
    pp(self, 0xc) = ImageFactory_loadChar(*(void **)g_sw_imageFactory, &g_sw_charDef);

    void *ach = *(void **)g_sw_achievements;
    int a0 = Achievements_isUnlocked(Achievements_get(), 0);
    int a1 = Achievements_isUnlocked(Achievements_get(), 1);
    int a2 = Achievements_isUnlocked(Achievements_get(), 2);
    int a3 = Achievements_isUnlocked(Achievements_get(), 3);
    (void)ach;

    void *canvas = *(void **)g_sw_canvas;

    int id = 0x493;
    if (a1) id = 0x494;
    if (a0) id = 0x495;
    PaintCanvas_setImage2D(canvas, id, (char *)self + 0x14);

    id = 0x492;
    if (a0) id = 0x496;
    if (a1) id = 0x497;
    PaintCanvas_setImage2D(canvas, id, (char *)self + 0x18);

    id = 0x490;
    if (a3) id = 0x498;
    if (a2) id = 0x499;
    PaintCanvas_setImage2D(canvas, id, (char *)self + 0x1c);

    id = 0x491;
    if (a2) id = 0x49a;
    if (a3) id = 0x49b;
    PaintCanvas_setImage2D(canvas, id, (char *)self + 0x20);

    i32(self, 0x60) = PaintCanvas_GetImage2DWidth(canvas);
    i32(self, 0x64) = PaintCanvas_GetImage2DHeight(canvas);
}
