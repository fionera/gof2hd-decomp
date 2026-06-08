#include "class.h"

// ImageFactory::loadImage(int row, int col, int frameBase) — looks up the image id for the
// [row][col] cell, creates the image2d in the global canvas, then builds an ImagePart whose
// position is taken from one of several layout tables selected by two orientation flags.
extern "C" void IF_PaintCanvas_Image2DCreate(unsigned canvas, unsigned short id, unsigned *out);
extern "C" void IF_ImagePart_ctor(void *self, unsigned id, int x, int y);

// id table: base[row*0x10 + col*4] -> image id (negative means "no image")
extern "C" __attribute__((visibility("hidden"))) int *g_IF_idTable;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_li_canvas; // canvas holder
extern "C" __attribute__((visibility("hidden"))) char *g_IF_flagA;        // first orientation flag
extern "C" __attribute__((visibility("hidden"))) char *g_IF_flagB;        // second orientation flag
extern "C" __attribute__((visibility("hidden"))) int g_IF_posTableA[];    // when flagA set
extern "C" __attribute__((visibility("hidden"))) int g_IF_posTableB[];    // when flagB set
extern "C" __attribute__((visibility("hidden"))) int *g_IF_posTableC;     // default base ptr
extern "C" __attribute__((visibility("hidden"))) char *g_IF_flagC;        // selects C vs D
extern "C" __attribute__((visibility("hidden"))) int *g_IF_posTableD;     // alt default base ptr

extern "C" void ImageFactory_loadImage(ImageFactory *self, int row, int col, int frameBase)
{
    int id = g_IF_idTable[row * 4 + col];   // 0x10-byte rows / 4-byte cells
    if (id < 0)
        return;

    unsigned local = 0;
    IF_PaintCanvas_Image2DCreate(*g_IF_li_canvas, (unsigned short)((short)id + (short)frameBase),
                                 &local);
    void *part = operator new(0x14);

    int *posBase;
    int rowCol = row * 0x20 + col * 8;
    if (*g_IF_flagA != 0) {
        posBase = g_IF_posTableA;       // base + rowCol applied below
    } else if (*g_IF_flagB != 0) {
        posBase = g_IF_posTableB;
    } else {
        posBase = g_IF_posTableC;
        if (*g_IF_flagC == 0)
            posBase = g_IF_posTableD;
    }

    int x = *(int *)((char *)posBase + rowCol);
    int y = *(int *)((char *)posBase + rowCol + 4);
    IF_ImagePart_ctor(part, local, x, y);
}
