#include "class.h"

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// Two mode-specific draw tail calls (mode==1 / flag@0x22).
void MissionsWindow_drawWanted(void *self);   // DAT_1ac504 thunk
void MissionsWindow_drawStarMap(void *self);  // DAT_1ac274 thunk

void  PaintCanvas_SetColor(void *canvas);
void  PaintCanvas_DrawString(void *canvas, void *font, void *text, int x, int y);

void  Layout_drawHeader(void *layout, void *title);
void  Layout_drawBox(void *layout, int style, int x, int y, int w, int h, void *label,
                     void *canvas, void *color);
void  Layout_drawFooter(void *layout);

void *GameText_getText(int id);
void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_dtor(void *s);

int   Status_wantedBoardAccessible();
void *Status_getFreelanceMission();

int   Mission_isEmpty(void *mission);
void *Mission_getAgent(void *mission);
int   Mission_getType(void *mission);

void  Agent_getName(void *agent, void *out);
void  Agent_getStationName(void *agent, void *out);
void *Agent_getMission(void *agent);

void  TouchButton_draw(void *btn);
void  ScrollTouchWindow_draw(void *win);
void  ChoiceWindow_draw(void *win);
void  ImageFactory_drawChar(void *factory, void *arr, int x, int y, bool b);

extern void *g_mwd_canvas;    // *(DAT_16059c): paint canvas
extern void *g_mwd_layout;    // *(DAT_1605a0): Layout singleton
extern int  *g_mwd_textId;    // *(DAT_1605a4): GameText id base
extern void *g_mwd_color;     // *(DAT_1605a8): box colour
extern void *g_mwd_imageFactory; // *(DAT_1609a0): image factory
extern void *g_mwd_font;      // *(DAT_1609a4): default font
}

// MissionsWindow::draw()
extern "C" void MissionsWindow_draw(void *self)
{
    char *s = (char *)self;
    void *volatile cookie = __stack_chk_guard;

    if (i32(self, 0x40) == 1) { MissionsWindow_drawWanted(self);  return; }
    if (u8(self, 0x22) != 0)  { MissionsWindow_drawStarMap(self); return; }

    void *canvas = *(void **)g_mwd_canvas;
    void *layout = *(void **)g_mwd_layout;
    void *color = *(void **)g_mwd_color;
    void *font = *(void **)g_mwd_font;
    int titleId = *g_mwd_textId;

    PaintCanvas_SetColor(canvas);

    char header[0xc];
    void *ht = GameText_getText(titleId);
    String_fromText(header, ht, false);
    Layout_drawHeader(layout, header);
    String_dtor(header);

    if (Status_wantedBoardAccessible() != 0) {
        void **tabs = (void **)pp(self, 0x14);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
            TouchButton_draw(((void **)tabs[1])[i]);
    }

    int ox = i32(self, 0x30), oy = i32(self, 0x34);
    int ow = i32(self, 0x38), oh = i32(self, 0x3c);

    // Campaign-mission title box + body box.
    {
        char box[0xc];
        void *t = GameText_getText(titleId);
        String_fromText(box, t, false);
        int c = *(int *)((char *)layout + 0xc), p20 = *(int *)((char *)layout + 0x20);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        Layout_drawBox(layout, 1, p28 + ox, oy + c + p20, (ow >> 1) - (p2c + p28),
                       *(int *)((char *)layout + 0x5c), box, canvas, color);
        String_dtor(box);
    }
    {
        char box[0xc];
        String_fromC(box, "", false);
        int c = *(int *)((char *)layout + 0xc), p10 = *(int *)((char *)layout + 0x10);
        int p20 = *(int *)((char *)layout + 0x20), p24 = *(int *)((char *)layout + 0x24);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        int p5c = *(int *)((char *)layout + 0x5c);
        Layout_drawBox(layout, 5, p28 + ox, oy + c + p20 + p5c + p2c, (ow >> 1) - (p2c + p28),
                       ((oh - (p20 + c + p5c + p2c * 2)) - p10) - p24, box, canvas, color);
        String_dtor(box);
    }

    ScrollTouchWindow_draw(pp(self, 0x0));
    if (pp(self, 0x24) != 0) TouchButton_draw(pp(self, 0x24));

    // Freelance-mission title + body box (right column).
    {
        char box[0xc];
        void *t = GameText_getText(titleId);
        String_fromText(box, t, false);
        int c = *(int *)((char *)layout + 0xc), p20 = *(int *)((char *)layout + 0x20);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        Layout_drawBox(layout, 1, ox + (ow >> 1) + p2c, oy + c + p20,
                       ((ow >> 1) - p2c) - p28, *(int *)((char *)layout + 0x5c), box, canvas, color);
        String_dtor(box);
    }
    {
        char box[0xc];
        String_fromC(box, "", false);
        int c = *(int *)((char *)layout + 0xc), p10 = *(int *)((char *)layout + 0x10);
        int p20 = *(int *)((char *)layout + 0x20), p24 = *(int *)((char *)layout + 0x24);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        int p5c = *(int *)((char *)layout + 0x5c);
        Layout_drawBox(layout, 5, ox + (ow >> 1) + p2c, oy + p2c + c + p20 + p5c,
                       ((ow >> 1) - p2c) - p28, ((oh - (c + p2c * 2 + p20 + p5c)) - p10) - p24,
                       box, canvas, color);
        String_dtor(box);
    }

    // Active freelance mission details.
    void *fm = Status_getFreelanceMission();
    if (fm != 0 && Mission_isEmpty(fm) == 0 && pp(self, 0x18) != 0) {
        ImageFactory_drawChar(*(void **)g_mwd_imageFactory, pp(self, 0x18),
                              ox + (ow >> 1) + *(int *)((char *)layout + 0x2c),
                              *(int *)((char *)layout + 0x2c) + oy + *(int *)((char *)layout + 0xc) +
                                  *(int *)((char *)layout + 0x20) + *(int *)((char *)layout + 0x5c),
                              false);

        int detailX = ox + (ow >> 1) + *(int *)((char *)layout + 0x2d4) +
                      *(int *)((char *)layout + 0x2c) * 2;
        int detailY = oy + *(int *)((char *)layout + 0xc) + *(int *)((char *)layout + 0x20) +
                      *(int *)((char *)layout + 0x2c) + *(int *)((char *)layout + 0x5c);

        char nameStr[0xc];
        Agent_getName(Mission_getAgent(fm), nameStr);
        PaintCanvas_DrawString(canvas, font, nameStr, detailX, detailY);
        String_dtor(nameStr);

        char stationStr[0xc];
        Agent_getStationName(Mission_getAgent(fm), stationStr);
        PaintCanvas_DrawString(canvas, font, stationStr, detailX, detailY);
        String_dtor(stationStr);

        void *typeTxt = GameText_getText(Mission_getType(Agent_getMission(Mission_getAgent(fm))) + 0x162);
        PaintCanvas_DrawString(canvas, font, typeTxt, detailX, detailY);
    }

    ScrollTouchWindow_draw(pp(self, 0x4));
    if (pp(self, 0x2c) != 0) TouchButton_draw(pp(self, 0x2c));
    if (pp(self, 0x28) != 0) TouchButton_draw(pp(self, 0x28));

    Layout_drawFooter(layout);
    if (u8(self, 0x21) != 0 || u8(self, 0x20) != 0)
        ChoiceWindow_draw(pp(self, 0xc));

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
