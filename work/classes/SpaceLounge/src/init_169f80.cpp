#include "class.h"

extern "C" void *Status_getStation();
extern "C" void *Station_getAgents(void *station);
extern "C" void *operator_new(unsigned int size);
extern "C" void *operator_new_arr(unsigned int size);
extern "C" void operator_delete(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void ChoiceWindow_ctor(void *choice);
extern "C" void CutScene_resetCamera(void *cutscene);
extern "C" void *ScrollTouchWindow_ctor(void *scroll, int x, int y, int w, int h, bool vertical);
extern "C" void Array_TouchButton_ctor(void *array);
extern "C" void ArraySetLength_TouchButtonPtr(void *array, unsigned count);
extern "C" void ArrayRelease_TouchButtonPtr(void *array);
extern "C" void *Array_TouchButtonPtr_dtor(void *array);
extern "C" void *TouchButton_ctor(void *button, void *text, int icon, int x, int y, int w, int align, int flags);
extern "C" void TouchButton_setTextColor(void *button, int color);
extern "C" void *GameText_getText(void *texts, int textId);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z);
extern "C" void EaseInOutMatrix_SetRange(void *ease, void *from, void *to);
extern "C" void SpaceLounge_updateScreenPositions(SpaceLounge *self);

extern "C" void *SpaceLounge_init_layout_slot;
extern "C" void *SpaceLounge_init_text_slot;
extern "C" void *SpaceLounge_init_camera_slot;

extern "C" int SpaceLounge_init(SpaceLounge *self)
{
    char matrix[60];

    UC(self, 0xb2) = 0;
    UC(self, 0xb0) = 0;
    UC(self, 0xbc) = 0;
    P(self, 0x24) = Station_getAgents(Status_getStation());

    if (P(self, 0x8) != 0) {
        operator_delete(ChoiceWindow_dtor(P(self, 0x8)));
        P(self, 0x8) = 0;
    }
    void *choice = operator_new(0x5c);
    ChoiceWindow_ctor(choice);
    P(self, 0x8) = choice;
    UC(self, 0x35) = 0;

    if (P(self, 0x44) != 0) {
        CutScene_resetCamera(P(self, 0x44));
    }

    UC(self, 0x18) = 0;
    P(self, 0x58) = operator_new_arr(0x15);
    for (int i = 0; i != 0x15; ++i) {
        ((char *)P(self, 0x58))[i] = 0;
    }

    void *layoutSlot = *(void **)&SpaceLounge_init_layout_slot;
    void *layout = *(void **)layoutSlot;
    int panelW = I(layout, 0x68);
    I(self, 0x70) = panelW / 2;
    I(self, 0x74) = I(layout, 0x20) + I(layout, 0xc);
    I(self, 0x6c) = panelW - I(layout, 0x4c) * 3 - I(layout, 0x2d4);

    void *scroll = operator_new(0x24);
    ScrollTouchWindow_ctor(scroll, I(self, 0x70) + I(layout, 0x4c) * 2 + I(layout, 0x2d4),
                           I(self, 0x74), I(self, 0x6c), I(layout, 0x6c), false);
    P(self, 0x60) = scroll;

    I(self, 0x14) = 0;
    I(self, 0x78) = I(self, 0x74) + I(layout, 0x6c) + I(layout, 0x2c);
    I(self, 0x64) = (I(layout, 0x34) + I(layout, 0x30)) * 5;

    if (P(self, 0x5c) != 0) {
        ArrayRelease_TouchButtonPtr(P(self, 0x5c));
        if (P(self, 0x5c) != 0) {
            operator_delete(Array_TouchButtonPtr_dtor(P(self, 0x5c)));
        }
    }
    void *buttons = operator_new(0xc);
    Array_TouchButton_ctor(buttons);
    P(self, 0x5c) = buttons;
    ArraySetLength_TouchButtonPtr(buttons, 5);

    void *textsSlot = *(void **)&SpaceLounge_init_text_slot;
    void *text = GameText_getText(*(void **)textsSlot, 0);
    int baseY = I(self, 0x78) + I(layout, 0x4c);
    I(self, 0x84) = I(self, 0x70) + I(layout, 0x68) - I(layout, 0x4c) - I(self, 0x6c);
    I(self, 0x7c) = (baseY + I(layout, 0x2d8) / 2) - I(layout, 0x30) / 2;
    I(self, 0x80) = baseY;

    for (unsigned i = 0; i < 5; ++i) {
        void *button = operator_new(200);
        TouchButton_ctor(button, text, 0, I(self, 0x84), baseY + (int)i * (I(layout, 0x30) + I(layout, 0x34)),
                         I(self, 0x6c), 0x11, 4);
        ((void **)P(buttons, 0x4))[i] = button;
        TouchButton_setTextColor(button, -1);
    }

    I(self, 0x88) = -1;
    MatrixSetTranslation(matrix, 0.0f, 0.0f, 0.0f);
    MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
    void *cameraSlot = *(void **)&SpaceLounge_init_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = PaintCanvas_CameraGetCurrent(camera);
    PaintCanvas_CameraSetLocal(camera, current);
    if (P(self, 0x48) != 0) {
        EaseInOutMatrix_SetRange(P(self, 0x48), matrix, matrix);
    }
    UC(self, 0xbd) = 1;
    I(self, 0x104) = 0;
    SpaceLounge_updateScreenPositions(self);
    return 0;
}
