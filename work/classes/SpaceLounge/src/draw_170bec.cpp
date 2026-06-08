#include "class.h"

extern "C" int ListItemWindow_shows3DShip(void *list);
extern "C" void ListItemWindow_draw_call(void *list);
extern "C" void SpaceLounge_draw_cutscene_tail();
extern "C" void SpaceLounge_draw_map_tail(void *map);
extern "C" void PaintCanvas_SetColor4(void *canvas, int a, int r, int g, int b);
extern "C" int PaintCanvas_GetWidth(void *canvas);
extern "C" int PaintCanvas_GetHeight(void *canvas);
extern "C" void PaintCanvas_FillRectangle(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void *GameText_getText(void *texts, int textId);
extern "C" void String_ctor_copy(void *dst, void *src, bool copy);
extern "C" void String_dtor(void *s);
extern "C" void Layout_drawHeader_call(void *layout, void *title);
extern "C" void SpaceLounge_drawLounge(SpaceLounge *self);
extern "C" void Layout_drawFooterNoBackButton_call(void *layout);
extern "C" void Layout_drawFooter_call(void *layout);
extern "C" void ChoiceWindow_draw_call(void *choice);

extern "C" void *SpaceLounge_draw_layout_slot;
extern "C" void *SpaceLounge_draw_canvas_slot;
extern "C" void *SpaceLounge_draw_text_slot;

extern "C" void SpaceLounge_draw(SpaceLounge *self)
{
    char title[12];

    if (UC(self, 0x1c) != 0) {
        if (ListItemWindow_shows3DShip(P(self, 0xc)) != 0) {
            void *canvasSlot = *(void **)&SpaceLounge_draw_canvas_slot;
            void *canvas = *(void **)canvasSlot;
            PaintCanvas_SetColor4(canvas, 0xff, 0, 0, 0);
            int width = PaintCanvas_GetWidth(canvas);
            int height = PaintCanvas_GetHeight(canvas);
            PaintCanvas_FillRectangle(canvas, 0, 0, width, height);
            PaintCanvas_SetColor(canvas, -1);
        }
        ListItemWindow_draw_call(P(self, 0xc));
        return SpaceLounge_draw_cutscene_tail();
    }

    if (UC(self, 0x34) != 0) {
        return SpaceLounge_draw_map_tail(P(self, 0x4));
    }

    void *layoutSlot = *(void **)&SpaceLounge_draw_layout_slot;
    void *layout = *(void **)layoutSlot;
    void *textsSlot = *(void **)&SpaceLounge_draw_text_slot;
    void *text = GameText_getText(*(void **)textsSlot, 0x18e);
    String_ctor_copy(title, text, false);
    Layout_drawHeader_call(layout, title);
    String_dtor(title);

    SpaceLounge_drawLounge(self);

    layout = *(void **)layoutSlot;
    if ((I(self, 0x14) & 0xfffffffe) == 2) {
        Layout_drawFooterNoBackButton_call(layout);
    } else {
        Layout_drawFooter_call(layout);
    }

    if (UC(self, 0x19) != 0 || UC(self, 0x1a) != 0 || UC(self, 0x1b) != 0) {
        ChoiceWindow_draw_call(P(self, 0x8));
    }
}
