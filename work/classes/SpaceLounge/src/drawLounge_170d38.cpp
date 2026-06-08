#include "class.h"

extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *text);
extern "C" void PaintCanvas_DrawRectangle(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_DrawString(void *canvas, void *font, void *text, int x, int y);
extern "C" void Layout_drawBox(void *layout, int style, int x, int y, int w, int h, void *title);
extern "C" void *String_ctor_cstr(void *dst, const char *src, bool copy);
extern "C" void String_ctor_copy(void *dst, void *src, bool copy);
extern "C" void String_dtor(void *s);
extern "C" void String_add(void *dst, void *a, void *b);
extern "C" void *GameText_getText(void *texts, int textId);
extern "C" void Agent_getName(void *agent, void *out);
extern "C" int Agent_isKnown(void *agent);
extern "C" int Agent_isStoryAgent(void *agent);
extern "C" int Agent_getRace(void *agent);
extern "C" void *Agent_getMission(void *agent);
extern "C" int Agent_getOffer(void *agent);
extern "C" int Mission_getType(void *mission);
extern "C" int Mission_isOutsideMission(void *mission);
extern "C" void ImageFactory_drawChar(void *factory, void *parts, int x, int y, bool mirror);
extern "C" void ScrollTouchWindow_draw(void *scroll);
extern "C" void TouchButton_setTextColor(void *button, int color);
extern "C" void TouchButton_setPosition(void *button, int x, int y);
extern "C" void TouchButton_setPosition3(void *button, int x, int y, int align);
extern "C" void TouchButton_setVisible(void *button, bool visible);
extern "C" void TouchButton_draw(void *button);

extern "C" void *SpaceLounge_lounge_canvas_slot;
extern "C" void *SpaceLounge_lounge_layout_slot;
extern "C" void *SpaceLounge_lounge_image_factory_slot;
extern "C" void *SpaceLounge_lounge_text_slot;
extern "C" void *SpaceLounge_lounge_font_slot;

static inline void *button_at(SpaceLounge *self, unsigned i)
{
    return ((void **)P(P(self, 0x5c), 0x4))[i];
}

extern "C" void SpaceLounge_drawLounge(SpaceLounge *self)
{
    char s0[12], s1[12], s2[12], s3[12], s4[12], s5[12], s6[12];

    void *canvasSlot = *(void **)&SpaceLounge_lounge_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *layoutSlot = *(void **)&SpaceLounge_lounge_layout_slot;
    void *layout = *(void **)layoutSlot;
    void *factorySlot = *(void **)&SpaceLounge_lounge_image_factory_slot;
    void *factory = *(void **)factorySlot;

    if (I(self, 0x14) == 0) {
        int hover = I(self, 0x88);
        if (hover >= 0) {
            void *agent = ((void **)P(P(self, 0x24), 0x4))[hover];
            char *rects = (char *)P(P(self, 0x40), 0x4);
            float *left = *(float **)(rects + hover * 8);
            float *right = *(float **)(rects + hover * 8 + 4);
            int pad = I(layout, 0x94);
            int cx = (int)(left[0] + (right[0] - left[0]) * 0.5f);
            int y = (int)(right[1] - (float)(pad * 2));

            PaintCanvas_SetColor(canvas, -1);
            if (Agent_isKnown(agent) != 0 || Agent_isStoryAgent(agent) != 0) {
                Agent_getName(agent, s0);
            } else {
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                void *text = GameText_getText(*(void **)texts, Agent_getRace(agent) + 0x196);
                String_ctor_copy(s0, text, false);
            }

            void *mission = Agent_getMission(agent);
            if (mission != 0) {
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                void *text = GameText_getText(*(void **)texts, Mission_getType(mission) + 0x162);
                String_ctor_copy(s1, text, false);
            } else {
                int offer = Agent_getOffer(agent);
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                int id = 0;
                if (offer == 6) {
                    id = 0x132;
                } else if (offer == 2) {
                    id = 0x131;
                } else if (offer == 7) {
                    id = 0x130;
                }
                if (id != 0) {
                    String_ctor_copy(s1, GameText_getText(*(void **)texts, id), false);
                } else {
                    String_ctor_cstr(s1, "", false);
                }
            }

            void *font = *(void **)*(void **)&SpaceLounge_lounge_font_slot;
            int textWidth = PaintCanvas_GetTextWidth(canvas, font);
            int boxX = cx - pad;
            int boxY = y - pad;
            int width = pad * 2 + textWidth;
            String_ctor_cstr(s2, "", false);
            Layout_drawBox(layout, 2, boxX, boxY, width, I(layout, 0x30), s2);
            String_dtor(s2);
            PaintCanvas_DrawRectangle(canvas, boxX, boxY, width, I(layout, 0x30));

            String_ctor_cstr(s3, Agent_isKnown(agent) == 0 ? "?" : "", false);
            String_add(s4, s3, s0);
            PaintCanvas_DrawString(canvas, font, s4, cx, y + I(layout, 0x2c0));
            String_dtor(s4);
            String_dtor(s3);

            if (Agent_isKnown(agent) != 0) {
                String_ctor_cstr(s5, " ", false);
                String_add(s6, s5, s0);
                int nameWidth = PaintCanvas_GetTextWidth(canvas, font);
                PaintCanvas_DrawString(canvas, font, s1, cx + nameWidth, y + I(layout, 0x2c0));
                String_dtor(s6);
                String_dtor(s5);
            }

            String_dtor(s1);
            String_dtor(s0);
        }
        return;
    }

    PaintCanvas_SetColor(canvas, -1);
    String_ctor_cstr(s0, "", false);
    Layout_drawBox(layout, 2, I(self, 0x70), I(self, 0x74), I(layout, 0x68), I(layout, 0x6c), s0);
    String_dtor(s0);
    PaintCanvas_DrawRectangle(canvas, I(self, 0x70), I(self, 0x74), I(layout, 0x68), I(layout, 0x6c));
    ImageFactory_drawChar(factory,
                          ((void **)P(P(self, 0x38), 0x4))[I(self, 0x20)],
                          I(layout, 0x4c) + I(self, 0x70),
                          I(layout, 0x4c) + I(self, 0x74),
                          false);
    ScrollTouchWindow_draw(P(self, 0x60));

    if ((I(self, 0x14) & 0xfffffffe) != 2) {
        return;
    }

    TouchButton_setTextColor(button_at(self, 0), -1);
    int offer = Agent_getOffer(((void **)P(P(self, 0x24), 0x4))[I(self, 0x20)]);
    if (I(self, 0x14) == 2) {
        TouchButton_setPosition(button_at(self, 0), I(self, 0x84), I(self, 0x80));
        TouchButton_setPosition3(button_at(self, 1), I(self, 0x6c) + I(self, 0x84), I(self, 0x80), 0x12);
        I(self, 0x68) = 0;
        if (offer < 11 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            I(self, 0x68) = 3;
        } else if (offer == 1) {
            I(self, 0x68) = 1;
            TouchButton_setPosition(button_at(self, 0), I(self, 0x84), I(self, 0x7c));
        } else {
            I(self, 0x68) = 2;
            TouchButton_setPosition(button_at(self, 0), I(self, 0x84), I(self, 0x7c));
            TouchButton_setPosition3(button_at(self, 1), I(self, 0x6c) + I(self, 0x84), I(self, 0x7c), 0x12);
        }
    } else {
        I(self, 0x68) = 1;
        TouchButton_setTextColor(button_at(self, 0), -1);
        TouchButton_setPosition(button_at(self, 0), I(self, 0x84), I(self, 0x7c));
    }

    for (unsigned i = 0; i < U(P(self, 0x5c), 0x0); ++i) {
        TouchButton_setVisible(button_at(self, i), false);
    }

    int buttonHeight = I(layout, 0x2d8);
    if (I(self, 0x68) > 2) {
        int needed = I(layout, 0x30) * (I(self, 0x68) - 1) + I(layout, 0x34) * (I(self, 0x68) - 2);
        if (needed > buttonHeight) {
            buttonHeight = needed;
        }
    }
    int panelHeight = I(layout, 0x4c) * 2 + buttonHeight;
    String_ctor_cstr(s0, "", false);
    Layout_drawBox(layout, 2, I(self, 0x70), I(self, 0x78), I(layout, 0x68), panelHeight, s0);
    String_dtor(s0);
    PaintCanvas_DrawRectangle(canvas, I(self, 0x70), I(self, 0x78), I(layout, 0x68), panelHeight);
    ImageFactory_drawChar(factory, P(self, 0x3c),
                          I(layout, 0x4c) + I(self, 0x70),
                          I(self, 0x78) + I(layout, 0x4c),
                          true);

    TouchButton_setVisible(button_at(self, 0), true);
    if (!(offer == 1 || UC(self, 0x36) != 0 || I(self, 0x14) == 3)) {
        TouchButton_setVisible(button_at(self, 1), true);
        TouchButton_draw(button_at(self, 0));
        TouchButton_draw(button_at(self, 1));
        if (offer <= 10 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            TouchButton_setVisible(button_at(self, 2), true);
        }
    }
    TouchButton_draw(button_at(self, 0));
}
