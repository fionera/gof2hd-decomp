#include "class.h"

extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_drawKey_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_drawKey_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_drawKey_screenH;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_drawKey_drawImage)(uint32_t, uint32_t, int, int);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_text;
extern "C" __attribute__((visibility("hidden"))) String *(*g_StarMap_drawKey_getText)(void *, int);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_font;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_drawKey_drawString)(uint32_t, void *, String *, int, int, bool);

extern "C" int PaintCanvas_GetImage2DWidth(uint32_t canvas, uint32_t image);
extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_dtor(String *self);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, String *label);

void StarMap::drawKey()
{
    uint32_t *canvasHolder = g_StarMap_drawKey_canvas;
    int imageWidth = PaintCanvas_GetImage2DWidth(*canvasHolder, field<uint32_t>(this, 0x30));
    void *layout = *g_StarMap_drawKey_layout;
    int screenW = *g_StarMap_drawKey_screenW;
    int screenH = *g_StarMap_drawKey_screenH;
    int boxW = field<int32_t>(this, 0x10c);
    int boxH = field<int32_t>(this, 0x110);
    int marginY = field<int32_t>(layout, 4);
    int padY = field<int32_t>(layout, 8);
    int rightPad = field<int32_t>(layout, 0x10);
    int lineH = field<int32_t>(layout, 0x2c);

    String empty;
    String_ctor_char(&empty, "", false);
    int x = screenH - boxW;
    Layout_drawBox(layout, 7, x, ((screenW - rightPad) - boxH) - padY, boxW, padY + boxH, &empty);
    int drawX = x + lineH;
    int textX = imageWidth + lineH + drawX;
    int y = ((screenW - lineH) - rightPad) - marginY;
    String_dtor(&empty);

    void (*drawImage)(uint32_t, uint32_t, int, int) = g_StarMap_drawKey_drawImage;
    drawImage(*canvasHolder, field<uint32_t>(this, 0x20), drawX, y);

    void **textHolder = g_StarMap_drawKey_text;
    String *(*getText)(void *, int) = g_StarMap_drawKey_getText;
    void **fontHolder = g_StarMap_drawKey_font;
    void (*drawString)(uint32_t, void *, String *, int, int, bool) = g_StarMap_drawKey_drawString;

    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x112), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x30), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x191), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x2c), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x223), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x28), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x22c), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x24), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x22b), textX, y, false);
}
