#include "class.h"

extern "C" int ChoiceWindow_touch_end(void *choice, int x, int y);
extern "C" void SpaceLounge_onKeyPress(SpaceLounge *self, int key);
extern "C" int StarMap_touch_end(void *map, int x, int y);
extern "C" void CutScene_resetCamera(void *cutscene);
extern "C" int Layout_touch_end(void *layout, int x, int y);
extern "C" void Layout_resetWindowDimensions(void *layout);
extern "C" int Agent_isGenericAgent(void *agent);
extern "C" void Agent_setEvent(void *agent, int event);
extern "C" int ListItemWindow_touch_end(void *list, int x, int y);
extern "C" int TouchButton_touch_end(void *button, int x, int y);
extern "C" int Layout_helpPressed(void *layout);
extern "C" void *GameText_getText(void *texts, int textId);
extern "C" void String_ctor_copy(void *dst, void *src, bool copy);
extern "C" void String_dtor(void *s);
extern "C" void Layout_initHelpWindow(void *layout, void *text);
extern "C" void ScrollTouchWindow_touch_end(void *scroll, int x, int y);
extern "C" void *Status_getSystem();
extern "C" int SolarSystem_getRace(void *system);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
extern "C" void EaseInOutMatrix_SetRange(void *ease, void *from, void *to);

extern "C" void *SpaceLounge_touch_layout_slot;
extern "C" void *SpaceLounge_touch_help_text_slot;
extern "C" void *SpaceLounge_touch_list_help_text_slot;
extern "C" void *SpaceLounge_touch_camera_slot;
extern "C" int SpaceLounge_touch_race_vectors[];

static inline void *selected_agent(SpaceLounge *self)
{
    void *agents = P(self, 0x24);
    return ((void **)P(agents, 0x4))[I(self, 0x20)];
}

static inline bool hit_agent(SpaceLounge *self, int x, int y, int i)
{
    char *data = (char *)P(P(self, 0x40), 0x4);
    float *a = *(float **)(data + i * 8);
    if (!(a[0] < (float)x)) {
        return false;
    }
    float *b = *(float **)(data + i * 8 + 4);
    return (float)x < b[0] && (float)y < a[1] && b[1] < (float)y;
}

extern "C" void SpaceLounge_OnTouchEnd(SpaceLounge *self, int x, int y)
{
    char helpBig[924];
    char helpSmall[12];
    char matrix[60];

    UC(self, 0xb2) = 0;

    if (UC(self, 0x1b) != 0 || UC(self, 0x19) != 0) {
        int result = ChoiceWindow_touch_end(P(self, 0x8), x, y);
        if (result == 1) {
            UC(self, 0x19) = 0;
        } else if (result == 0) {
            SpaceLounge_onKeyPress(self, 0x10000);
        }
        return;
    }

    if (UC(self, 0x34) != 0) {
        if (StarMap_touch_end(P(self, 0x4), x, y) != 0) {
            CutScene_resetCamera(P(self, 0x44));
            UC(self, 0x34) = 0;
        }
        return;
    }

    void *layoutSlot = *(void **)&SpaceLounge_touch_layout_slot;
    void *layout = *(void **)layoutSlot;
    if (Layout_touch_end(layout, x, y) != 0) {
        if (UC(self, 0x1c) != 0) {
            Layout_resetWindowDimensions(layout);
            UC(self, 0x1c) = 0;
        } else if (I(self, 0x14) != 0) {
            if (I(self, 0x20) >= 0) {
                void *agent = selected_agent(self);
                if (Agent_isGenericAgent(agent) != 0) {
                    Agent_setEvent(agent, 1);
                }
            }
            I(self, 0x14) = 0;
            UC(self, 0x36) = 0;
        }
        return;
    }

    if (UC(self, 0x1c) != 0) {
        ListItemWindow_touch_end(P(self, 0xc), x, y);
        if (Layout_helpPressed(layout) != 0) {
            void *texts = *(void **)&SpaceLounge_touch_list_help_text_slot;
            void *text = GameText_getText(*(void **)texts, 0x283);
            String_ctor_copy(helpSmall, text, false);
            Layout_initHelpWindow(layout, helpSmall);
            String_dtor(helpSmall);
        }
        return;
    }

    switch (I(self, 0x14)) {
    case 0:
        if (UC(self, 0xbd) == 0) {
            void *system = Status_getSystem();
            int race = SolarSystem_getRace(system);
            int *v = &SpaceLounge_touch_race_vectors[race * 3];
            MatrixSetTranslation(matrix, (float)v[2], (float)v[0], (float)v[1]);
            MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
            void *cameraSlot = *(void **)&SpaceLounge_touch_camera_slot;
            void *camera = *(void **)cameraSlot;
            void *current = PaintCanvas_CameraGetCurrent(camera);
            PaintCanvas_CameraSetLocal(camera, current);
            if (P(self, 0x48) != 0) {
                EaseInOutMatrix_SetRange(P(self, 0x48), matrix, matrix);
            }
            UC(self, 0xbd) = 1;
            I(self, 0x104) = 0;
            return;
        }
        if (P(self, 0x24) != 0) {
            I(self, 0x88) = -1;
            I(self, 0x20) = -1;
            unsigned count = U(P(self, 0x24), 0x0);
            for (unsigned i = 0; i < count; ++i) {
                if (hit_agent(self, x, y, i)) {
                    I(self, 0x20) = i;
                    SpaceLounge_onKeyPress(self, 0x10000);
                    return;
                }
            }
        }
        break;
    case 1:
        SpaceLounge_onKeyPress(self, 0x10000);
        break;
    case 2: {
        unsigned count = U(P(self, 0x5c), 0x0);
        for (unsigned i = 0; i < count; ++i) {
            void *button = ((void **)P(P(self, 0x5c), 0x4))[i];
            if (TouchButton_touch_end(button, x, y) != 0) {
                void *agent = selected_agent(self);
                if (i >= 5 && Agent_isGenericAgent(agent) != 0) {
                    Agent_setEvent(agent, 1);
                }
            }
        }
        break;
    }
    case 3:
        if (TouchButton_touch_end(*(void **)P(P(self, 0x5c), 0x4), x, y) != 0) {
            SpaceLounge_onKeyPress(self, 0x10000);
        }
        break;
    }

    ScrollTouchWindow_touch_end(P(self, 0x60), x, y);
    if (Layout_helpPressed(layout) != 0) {
        void *texts = *(void **)&SpaceLounge_touch_help_text_slot;
        void *text = GameText_getText(*(void **)texts, 0x273);
        String_ctor_copy(helpBig, text, false);
        Layout_initHelpWindow(layout, helpBig);
        String_dtor(helpBig);
    }
}
