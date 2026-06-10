#include "gof2/SpaceLounge.h"
#include "gof2/Agent.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/Mission.h"
#include "gof2/String.h"
// gof2/TouchButton.h and gof2/Station.h (pulled in via Mission.h above) both define
// `struct RetStr` unconditionally with identical layout, which is a C++ redefinition error.
// We need TouchButton.h for the TouchButton struct/methods used below, so include it with
// its RetStr renamed out of the way (it is never referenced in this translation unit; the
// canonical RetStr is the one already provided by Station.h).
#define RetStr RetStr
#include "gof2/TouchButton.h"
#undef RetStr


extern "C" void ChoiceWindow_OnTouchMove(void *choice, int x, int y);
extern "C" void StarMap_OnTouchMove(void *map, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *scroll, int x, int y);
extern "C" void ListItemWindow_OnTouchMove(void *list, int x, int y);
extern "C" void *SpaceLounge_layout_move;
extern "C" void SpaceLounge_OnRender3D_map_tail(void *map);
extern "C" int ListItemWindow_shows3DShip(void *item);
extern "C" void SpaceLounge_OnRender3D_cutscene_tail(void *cutscene);
extern "C" void SpaceLounge_OnRenderBG_tail();
extern "C" void SpaceLounge_draw3DShip_tail(void *ship);
extern "C" void ChoiceWindow_OnTouchBegin(void *choice, int x, int y);
extern "C" void StarMap_OnTouchBegin(void *map, int x, int y);
extern "C" void ListItemWindow_OnTouchBegin(void *list, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchBegin(void *scroll, int x, int y);
extern "C" void *SpaceLounge_layout_begin;
extern "C" int ChoiceWindow_touch_end(void *choice, int x, int y);
extern "C" int StarMap_touch_end(void *map, int x, int y);
extern "C" void CutScene_resetCamera(void *cutscene);
extern "C" int Layout_touch_end(void *layout, int x, int y);
extern "C" void Agent_setEvent(void *agent, int event);
extern "C" int ListItemWindow_touch_end(void *list, int x, int y);
extern "C" int TouchButton_touch_end(void *button, int x, int y);
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
extern "C" int Agent_getRace(void *agent);
extern "C" int Agent_getOffer(void *agent);
extern "C" int Mission_getType(void *mission);
extern "C" void Globals_getAgentMissionText(void *out, int textId, void *agent);
extern "C" int AERandom_nextInt(void *random, int limit);
extern "C" int String_Compare(void *lhs, void *rhs);
extern "C" int SpaceLounge_getSpecificSoundForRace(int, unsigned soundId, int race, bool alternate);
extern "C" int *SpaceLounge_getSoundId_missionText;
extern "C" void *SpaceLounge_getSoundId_offerText;
extern "C" void *SpaceLounge_getSoundId_offer2358910;
extern "C" void *SpaceLounge_getSoundId_offer5;
extern "C" void *SpaceLounge_getSoundId_offer7;
extern "C" void *SpaceLounge_getSoundId_offer4;
extern "C" void *SpaceLounge_getSoundId_offer6;
extern "C" void *SpaceLounge_getSoundId_offer0_11;
extern "C" void *SpaceLounge_getSoundId_offer0_else;
extern "C" void *SpaceLounge_getSoundId_chance;
extern "C" void *SpaceLounge_getSoundId_offer1;
extern "C" void *SpaceLounge_getSoundId_accepted;
extern "C" void *SpaceLounge_getSoundId_specialText;
extern "C" void *SpaceLounge_getSoundId_specialRandom;
extern "C" void ChoiceWindow_left(void *choice);
extern "C" void ChoiceWindow_right(void *choice);
extern "C" void ScrollTouchWindow_scroll(void *scroll, int amount);
extern "C" void *Level_getEnemies(void *level);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void PaintCanvas_GetScreenPosition(void *canvas, void *vec);
extern "C" void MatrixGetRight(void *out, void *matrix);
extern "C" void MatrixGetPosition(void *out, void *matrix);
extern "C" void MatrixGetUp(void *out, void *matrix);
extern "C" void MatrixGetLookAt(void *out, void *pos, void *target, void *up);
extern "C" void MatrixGetDir(void *out, void *matrix);
extern "C" void Vector_mul(void *out, void *vec, float scale);
extern "C" void Vector_assign(void *dst, void *src);
extern "C" void Vector_sub(void *out, void *a, void *b);
extern "C" void Vector_add(void *out, void *a, void *b);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void Matrix_mul_assign(void *dst, void *rhs);
extern "C" void AEGeometry_setMatrix(void *matrix);
extern "C" void *SpaceLounge_screen_level_slot;
extern "C" void *SpaceLounge_screen_canvas_slot;
extern "C" void *SpaceLounge_screen_projector;
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *text);
extern "C" void PaintCanvas_DrawRectangle(void *canvas, int x, int y, int w, int h);
extern "C" void PaintCanvas_DrawString(void *canvas, void *font, void *text, int x, int y);
extern "C" void *String_ctor_cstr(void *dst, const char *src, bool copy);
extern "C" void String_add(void *dst, void *a, void *b);
extern "C" int Mission_isOutsideMission(void *mission);
extern "C" void ScrollTouchWindow_draw(void *scroll);
extern "C" void TouchButton_setPosition3(void *button, int x, int y, int align);
extern "C" void *SpaceLounge_lounge_canvas_slot;
extern "C" void *SpaceLounge_lounge_layout_slot;
extern "C" void *SpaceLounge_lounge_image_factory_slot;
extern "C" void *SpaceLounge_lounge_text_slot;
extern "C" void *SpaceLounge_lounge_font_slot;
extern "C" void *Status_getStation();
extern "C" void *Station_getAgents(void *station);
extern "C" void *operator_new(unsigned int size);
extern "C" void *operator_new_arr(unsigned int size);
extern "C" void operator_delete(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void ChoiceWindow_ctor(void *choice);
extern "C" void *ScrollTouchWindow_ctor(void *scroll, int x, int y, int w, int h, bool vertical);
extern "C" void Array_TouchButton_ctor(void *array);
extern "C" void ArraySetLength_TouchButtonPtr(void *array, unsigned count);
extern "C" void ArrayRelease_TouchButtonPtr(void *array);
extern "C" void *Array_TouchButtonPtr_dtor(void *array);
extern "C" void *TouchButton_ctor(void *button, void *text, int icon, int x, int y, int w, int align, int flags);
extern "C" void *SpaceLounge_init_layout_slot;
extern "C" void *SpaceLounge_init_text_slot;
extern "C" void *SpaceLounge_init_camera_slot;
extern "C" void String_ctor_default(void *s);
extern "C" void Matrix_ctor(void *m);
extern "C" void CutScene_ctor(void *cutscene, int id);
extern "C" int CutScene_isInitialized(void *cutscene);
extern "C" void CutScene_initialize(void *cutscene);
extern "C" void ArrayRemove_AgentPtr(void *agent, void *array);
extern "C" void EaseInOutMatrix_ctor(void *ease, void *from, void *to, int duration);
extern "C" void *SpaceLounge_ctor_camera_slot;
extern "C" void Agent_setKnown(void *agent, bool known);
extern "C" void ChoiceWindow_setText(void *choice, void *title, void *body);
extern "C" void ChoiceWindow_setButtonText(void *choice, void *left, void *right);
extern "C" int SpaceLounge_getSoundId(SpaceLounge *self, void *agent);
extern "C" void *SpaceLounge_start_text_slot;
extern "C" void *CutScene_dtor(void *p);
extern "C" void *Array_StringPtr_dtor(void *p);
extern "C" void *Array_TouchButtonPtr_dtor(void *p);
extern "C" void *Array_ImagePartPtr_dtor(void *p);
extern "C" void *Array_ArrayImagePartPtr_dtor(void *p);
extern "C" void *Array_VectorPtr_dtor(void *p);
extern "C" void ArrayRelease_StringPtr(void *p);
extern "C" void ArrayRelease_TouchButtonPtr(void *p);
extern "C" void ArrayRelease_ImagePartPtr(void *p);
extern "C" void ArrayRelease_ArrayImagePartPtr(void *p);
extern "C" void ArrayRelease_VectorPtr(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void operator_delete_arr(void *p);
extern "C" int ListItemWindow_shows3DShip(void *list);
extern "C" void ListItemWindow_draw_call(void *list);
extern "C" void SpaceLounge_draw_cutscene_tail();
extern "C" void SpaceLounge_draw_map_tail(void *map);
extern "C" void PaintCanvas_SetColor4(void *canvas, int a, int r, int g, int b);
extern "C" int PaintCanvas_GetWidth(void *canvas);
extern "C" int PaintCanvas_GetHeight(void *canvas);
extern "C" void PaintCanvas_FillRectangle(void *canvas, int x, int y, int w, int h);
extern "C" void Layout_drawHeader_call(void *layout, void *title);
extern "C" void Layout_drawFooterNoBackButton_call(void *layout);
extern "C" void Layout_drawFooter_call(void *layout);
extern "C" void ChoiceWindow_draw_call(void *choice);
extern "C" void *SpaceLounge_draw_layout_slot;
extern "C" void *SpaceLounge_draw_canvas_slot;
extern "C" void *SpaceLounge_draw_text_slot;
extern "C" void SpaceLounge_update_map_tail(void *map, int dt);
extern "C" void SpaceLounge_update_ship_tail(void *list, int dt);
extern "C" void EaseInOutMatrix_Increase(void *ease, float dt);
extern "C" void EaseInOutMatrix_GetMaxValue(void *out, void *ease);
extern "C" void EaseInOutMatrix_GetValue(void *out, void *ease);
extern "C" int Matrix_equal(void *a, void *b);
extern "C" float Sinf(float value);
extern "C" void EaseInOut_ctor(void *ease, float start, float end);
extern "C" float EaseInOut_GetValue(void *ease);
extern "C" float EaseInOut_GetMaxValue(void *ease);
extern "C" void EaseInOut_SetRange(void *ease, float start, float end);
extern "C" void EaseInOut_Increase(void *ease, float value);
extern "C" void Matrix_mul_assign(void *lhs, void *rhs);
extern "C" void ScrollTouchWindow_update(void *scroll, int dt);
extern "C" void CutScene_update(void *cutscene);
extern "C" void *SpaceLounge_update_camera_slot_a;
extern "C" void *SpaceLounge_update_camera_slot_b;
extern "C" void *SpaceLounge_update_camera_slot_c;
extern "C" void *SpaceLounge_update_random_slot;
extern "C" int *SpaceLounge_update_width_slot;
extern "C" int *SpaceLounge_update_height_slot;
extern "C" int *SpaceLounge_update_top_slot;

// ---- OnTouchMove_171928.cpp ----
int SpaceLounge::OnTouchMove(int x, int y) {
    SpaceLounge *self = this;
    I(self, 0xb4) = x;
    I(self, 0xb8) = y;

    if (UC(self, 0x1b) != 0 || UC(self, 0x19) != 0) {
        ChoiceWindow_OnTouchMove(P(self, 0x8), x, y);
        return 0;
    }

    if (UC(self, 0x34) != 0) {
        StarMap_OnTouchMove(P(self, 0x4), x, y);
        return 0;
    }

    int mode = I(self, 0x14);
    if (mode == 0) {
        void *items = P(self, 0x24);
        if (items != 0) {
            float fy = (float)y;
            float fx = (float)x;
            I(self, 0x88) = -1;
            I(self, 0x20) = -1;
            int i = 0;
            int count = I(items, 0x0);
            for (; count != i; ++i) {
                char *data = (char *)P(P(self, 0x40), 0x4);
                float *a = *(float **)(data + i * 8);
                if (a[0] < fx) {
                    float *b = *(float **)(data + i * 8 + 4);
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        I(self, 0x88) = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        void *buttons = P(self, 0x5c);
        ((TouchButton *)(*(void **)P(buttons, 0x4)))->OnTouchMove(x, y);
    } else if (mode == 2) {
        unsigned i = 0;
        for (; i < U(P(self, 0x5c), 0x0); ++i) {
            void **data = (void **)P(P(self, 0x5c), 0x4);
            ((TouchButton *)(data[i]))->OnTouchMove(x, y);
        }
    }

    ((Layout *)(*(void **)SpaceLounge_layout_move))->OnTouchMove(x, y);
    if (UC(self, 0x1c) != 0) {
        ListItemWindow_OnTouchMove(P(self, 0xc), x, y);
    } else {
        ScrollTouchWindow_OnTouchMove(P(self, 0x60), x, y);
    }
    return 0;
}

// ---- OnRender3D_173760.cpp ----
void SpaceLounge::OnRender3D() {
    SpaceLounge *self = this;
    if (UC(self, 0x34) != 0) {
        return SpaceLounge_OnRender3D_map_tail(P(self, 0x4));
    }

    void *cutscene = P(self, 0x44);
    if (cutscene == 0) {
        return;
    }

    if (UC(self, 0x1c) != 0) {
        if (ListItemWindow_shows3DShip(P(self, 0xc)) != 0) {
            return;
        }
        cutscene = P(self, 0x44);
    }

    return SpaceLounge_OnRender3D_cutscene_tail(cutscene);
}

// ---- OnRenderBG_173792.cpp ----
void SpaceLounge::OnRenderBG() {
    SpaceLounge *self = this;
    if (I(self, 0x44) != 0) {
        return SpaceLounge_OnRenderBG_tail();
    }
}

// ---- introFinished_16a894.cpp ----
unsigned char SpaceLounge::introFinished() {
    SpaceLounge *self = this;
    return UC(self, 0xbd);
}

// ---- listMode_1714ea.cpp ----
bool SpaceLounge::listMode() {
    SpaceLounge *self = this;
    return UC(self, 0x34) == 0;
}

// ---- mapMode_1714e4.cpp ----
unsigned char SpaceLounge::mapMode() {
    SpaceLounge *self = this;
    return UC(self, 0x34);
}

// ---- hangarNeedsUpdate_1717f0.cpp ----
unsigned char SpaceLounge::hangarNeedsUpdate() {
    SpaceLounge *self = this;
    return UC(self, 0x35);
}

// ---- draw3DShip_1714d4.cpp ----
void SpaceLounge::draw3DShip() {
    SpaceLounge *self = this;
    if (UC(self, 0x1c) != 0) {
        return SpaceLounge_draw3DShip_tail(P(self, 0xc));
    }
}

// ---- setHangarUpdate_16aa74.cpp ----
void SpaceLounge::setHangarUpdate(bool value) {
    SpaceLounge *self = this;
    BL(self, 0x35) = value;
}

// ---- OnTouchBegin_1717f8.cpp ----
int SpaceLounge::OnTouchBegin(int x, int y) {
    SpaceLounge *self = this;
    UC(self, 0xb2) = 1;
    I(self, 0xb4) = x;
    I(self, 0xb8) = y;
    I(self, 0x88) = -1;

    if (UC(self, 0x1b) != 0 || UC(self, 0x19) != 0) {
        ChoiceWindow_OnTouchBegin(P(self, 0x8), x, y);
        return 0;
    }

    if (UC(self, 0x34) != 0) {
        StarMap_OnTouchBegin(P(self, 0x4), x, y);
        return 0;
    }

    if (UC(self, 0x1c) != 0) {
        ListItemWindow_OnTouchBegin(P(self, 0xc), x, y);
        ((Layout *)(*(void **)SpaceLounge_layout_begin))->OnTouchBegin(x, y);
        ScrollTouchWindow_OnTouchBegin(P(self, 0x60), x, y);
        return 0;
    }

    int mode = I(self, 0x14);
    if (mode == 0) {
        if (UC(self, 0xbd) == 0) {
            return 0;
        }
        void *items = P(self, 0x24);
        if (items != 0) {
            int i = 0;
            float fy = (float)y;
            float fx = (float)x;
            int count = I(items, 0x0);
            for (; count != i; ++i) {
                char *data = (char *)P(P(self, 0x40), 0x4);
                float *a = *(float **)(data + i * 8);
                if (a[0] < fx) {
                    float *b = *(float **)(data + i * 8 + 4);
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        I(self, 0x88) = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        void *buttons = P(self, 0x5c);
        ((TouchButton *)(*(void **)P(buttons, 0x4)))->OnTouchBegin(x, y);
    } else if (mode == 2) {
        unsigned i = 0;
        for (; i < U(P(self, 0x5c), 0x0); ++i) {
            void **data = (void **)P(P(self, 0x5c), 0x4);
            ((TouchButton *)(data[i]))->OnTouchBegin(x, y);
        }
    }

    ((Layout *)(*(void **)SpaceLounge_layout_begin))->OnTouchBegin(x, y);
    ScrollTouchWindow_OnTouchBegin(P(self, 0x60), x, y);
    return 0;
}

// ---- getSpecificSoundForRace_17177c.cpp ----
extern "C" int SpaceLounge_getSpecificSoundForRace(int, unsigned soundId, int race, bool alternate)
{
    unsigned delta;

    switch (race) {
    case 0:
    case 5:
        delta = soundId - 0x2f7;
        if (alternate) {
            if ((unsigned)delta < 0x30) {
                return soundId + 0xf0;
            }
        } else if ((unsigned)delta < 0x30) {
            return soundId + 0xc0;
        }
        break;
    case 1:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x120;
        }
        break;
    case 2:
    case 3:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x90;
        }
        break;
    case 4:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x60;
        }
        break;
    case 6:
        goto done;
    case 7:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x30;
        }
        break;
    }

    soundId = -1;
done:
    return soundId;
}

// ---- OnTouchEnd_171a48.cpp ----
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

void SpaceLounge::OnTouchEnd(int x, int y) {
    SpaceLounge *self = this;
    char helpBig[924];
    char helpSmall[12];
    char matrix[60];

    UC(self, 0xb2) = 0;

    if (UC(self, 0x1b) != 0 || UC(self, 0x19) != 0) {
        int result = ChoiceWindow_touch_end(P(self, 0x8), x, y);
        if (result == 1) {
            UC(self, 0x19) = 0;
        } else if (result == 0) {
            ((SpaceLounge *)(self))->onKeyPress(0x10000);
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
            ((Layout *)(layout))->resetWindowDimensions();
            UC(self, 0x1c) = 0;
        } else if (I(self, 0x14) != 0) {
            if (I(self, 0x20) >= 0) {
                void *agent = selected_agent(self);
                if (((Agent *)(agent))->isGenericAgent() != 0) {
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
        if (((Layout *)(layout))->helpPressed() != 0) {
            void *texts = *(void **)&SpaceLounge_touch_list_help_text_slot;
            void *text = ((GameText *)(*(void **)texts))->getText(0x283);
            ((String *)(helpSmall))->ctor_copy((String *)text, false);
            ((Layout *)(layout))->initHelpWindow(helpSmall);
            ((String *)(helpSmall))->dtor();
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
                    ((SpaceLounge *)(self))->onKeyPress(0x10000);
                    return;
                }
            }
        }
        break;
    case 1:
        ((SpaceLounge *)(self))->onKeyPress(0x10000);
        break;
    case 2: {
        unsigned count = U(P(self, 0x5c), 0x0);
        for (unsigned i = 0; i < count; ++i) {
            void *button = ((void **)P(P(self, 0x5c), 0x4))[i];
            if (TouchButton_touch_end(button, x, y) != 0) {
                void *agent = selected_agent(self);
                if (i >= 5 && ((Agent *)(agent))->isGenericAgent() != 0) {
                    Agent_setEvent(agent, 1);
                }
            }
        }
        break;
    }
    case 3:
        if (TouchButton_touch_end(*(void **)P(P(self, 0x5c), 0x4), x, y) != 0) {
            ((SpaceLounge *)(self))->onKeyPress(0x10000);
        }
        break;
    }

    ScrollTouchWindow_touch_end(P(self, 0x60), x, y);
    if (((Layout *)(layout))->helpPressed() != 0) {
        void *texts = *(void **)&SpaceLounge_touch_help_text_slot;
        void *text = ((GameText *)(*(void **)texts))->getText(0x273);
        ((String *)(helpBig))->ctor_copy((String *)text, false);
        ((Layout *)(layout))->initHelpWindow(helpBig);
        ((String *)(helpBig))->dtor();
    }
}

// ---- getSoundId_1714f8.cpp ----
static inline int random_from(void *slot, int limit)
{
    return AERandom_nextInt(*(void **)slot, limit);
}

extern "C" int SpaceLounge_getSoundId(SpaceLounge *, void *agent)
{
    char missionText[12];

    int race = Agent_getRace(agent);
    bool male = ((Agent *)(agent))->isMale();
    int offer = Agent_getOffer(agent);
    void *mission = ((Agent *)(agent))->getMission();
    int missionType;
    if (mission == 0) {
        missionType = -1;
    } else {
        mission = ((Agent *)(agent))->getMission();
        if (((Mission *)(mission))->isEmpty() != 0) {
            missionType = -1;
        } else {
            mission = ((Agent *)(agent))->getMission();
            missionType = Mission_getType(mission);
        }
    }

    Globals_getAgentMissionText(missionText, *SpaceLounge_getSoundId_missionText, agent);

    bool checkSpecialText = true;
    int soundId;
    switch (offer) {
    case 0:
        if (missionType == 0 || missionType == 0xb) {
            soundId = random_from(&SpaceLounge_getSoundId_offer0_11, 4) + 0x301;
        } else if (missionType == 0xc) {
            soundId = random_from(&SpaceLounge_getSoundId_offer0_else, 4) + 0x2fa;
        } else {
            void *random = *(void **)&SpaceLounge_getSoundId_offer0_else;
            int first = AERandom_nextInt(random, 2);
            if (first == 0) {
                soundId = AERandom_nextInt(random, 4) + 0x31f;
            } else {
                soundId = AERandom_nextInt(random, 4) + 0x309;
            }
        }
        break;
    case 1:
        soundId = random_from(&SpaceLounge_getSoundId_offer1, 2) + 0x30d;
        break;
    case 2:
    case 3:
    case 8:
    case 9:
    case 10:
        soundId = random_from(&SpaceLounge_getSoundId_offer2358910, 2) + 0x2f7;
        break;
    case 4:
        soundId = random_from(&SpaceLounge_getSoundId_offer4, 2) + 0x2fe;
        break;
    case 5:
        soundId = random_from(&SpaceLounge_getSoundId_offer5, 4) + 0x31b;
        break;
    case 6:
        soundId = random_from(&SpaceLounge_getSoundId_offer6, 4) + 0x323;
        break;
    case 7:
        soundId = random_from(&SpaceLounge_getSoundId_offer7, 4) + 0x305;
        break;
    default:
        soundId = -1;
        break;
    }

    if (offer != 1) {
        checkSpecialText = false;
        if (random_from(&SpaceLounge_getSoundId_chance, 100) < 30) {
            soundId = random_from(&SpaceLounge_getSoundId_offer1, 2) + 0x30d;
            checkSpecialText = true;
        }
    }

    int dummy = 0;
    if (((Agent *)(agent))->hasAcceptedOffer() != 0) {
        dummy = random_from(&SpaceLounge_getSoundId_accepted, 2);
        soundId = dummy + 0x30d;
    }

    if (checkSpecialText) {
        void *texts = *(void **)&SpaceLounge_getSoundId_specialText;
        void *text = ((GameText *)(*(void **)texts))->getText(0x334);
        if (String_Compare(missionText, text) != 0) {
            text = ((GameText *)(*(void **)texts))->getText(0x338);
            if (String_Compare(missionText, text) != 0) {
                text = ((GameText *)(*(void **)texts))->getText(0x33b);
                if (String_Compare(missionText, text) != 0) {
                    text = ((GameText *)(*(void **)texts))->getText(0x341);
                    dummy = String_Compare(missionText, text);
                    if (dummy != 0) {
                        goto special_done;
                    }
                }
            }
        }
        dummy = random_from(&SpaceLounge_getSoundId_specialRandom, 2);
        soundId = dummy + 0x314;
    }

special_done:
    if (race == 3) {
        void *parts = ((Agent *)(agent))->getImageParts();
        if (parts == 0) {
            race = 3;
            dummy = 0;
        } else {
            parts = ((Agent *)(agent))->getImageParts();
            dummy = I(parts, 0x0);
            race = 0;
            if (dummy == 2) {
                race = 3;
            }
        }
    }

    int result = SpaceLounge_getSpecificSoundForRace(dummy, soundId, race, male);
    ((String *)(missionText))->dtor();
    return result;
}

// ---- onKeyPress_16e2f0.cpp ----
static inline void *key_agent(SpaceLounge *self)
{
    return ((void **)P(P(self, 0x24), 0x4))[I(self, 0x20)];
}

void SpaceLounge::onKeyPress(int key) {
    SpaceLounge *self = this;
    char scratch[960];
    (void)scratch;

    if (UC(self, 0x1b) != 0) {
        if (key == 0x1000) {
            ChoiceWindow_left(P(self, 0x8));
        } else if (key == 0x2000) {
            ChoiceWindow_right(P(self, 0x8));
        } else if (key == 0x10000) {
            UC(self, 0x1b) = 0;
        }
        return;
    }

    int mode = I(self, 0x14);
    if (mode == 0) {
        if (key == 0x1000 || key == 0x8000) {
            unsigned next = I(self, 0x20) + 1;
            if (P(self, 0x24) != 0 && next >= U(P(self, 0x24), 0x0)) {
                next = 0;
            }
            I(self, 0x20) = next;
        } else if (key == 0x2000 || key == 0x4000) {
            int current = I(self, 0x20);
            if (current < 1 && P(self, 0x24) != 0) {
                current = U(P(self, 0x24), 0x0);
            }
            I(self, 0x20) = current - 1;
        } else if (key == 0x10000 || key == 0x20000) {
            ((SpaceLounge *)(self))->startChat();
        }
        return;
    }

    if (mode == 1 || mode == 3) {
        if (key == 0x10000 || key == 0x20000) {
            I(self, 0x30) = 0;
            void *agent = key_agent(self);
            if (Agent_getOffer(agent) == 1) {
                I(self, 0x14) = 2;
            } else {
                I(self, 0x14) = mode == 1 ? 2 : 0;
            }
        } else if (key == 0x40000) {
            if (I(self, 0x2c) < 3) {
                if (mode == 1) {
                    I(self, 0x2c) = 0;
                    I(self, 0x14) = 0;
                }
            } else {
                I(self, 0x2c) -= 0x28;
            }
        }
        return;
    }

    if (mode == 2) {
        void *agent = key_agent(self);
        if (key == 0x40000) {
            I(self, 0x14) = 1;
        } else if (key == 0x10000 || key == 0x20000) {
            if (((Agent *)(agent))->getMission() == 0 && ((Agent *)(agent))->isGenericAgent() != 0) {
                Agent_setEvent(agent, 1);
            }
            I(self, 0x14) = 0;
        } else if (key == 0x8000) {
            ScrollTouchWindow_scroll(P(self, 0x60), 1);
        } else if (key == 0x4000) {
            ScrollTouchWindow_scroll(P(self, 0x60), -1);
        }
    }
}

// ---- updateScreenPositions_170478.cpp ----
void SpaceLounge::updateScreenPositions() {
    SpaceLounge *self = this;
    char camera[60];
    char look[60];
    char pos[12];
    char up[12];
    char target[12];
    char halfRight[12];
    char screen[12];

    if (P(self, 0x24) == 0) {
        return;
    }

    void *level = *(void **)&SpaceLounge_screen_level_slot;
    void *enemies = Level_getEnemies(level);
    void *canvasSlot = *(void **)&SpaceLounge_screen_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *project = *(void **)&SpaceLounge_screen_projector;

    void *current = PaintCanvas_CameraGetCurrent(canvas);
    void *local = PaintCanvas_CameraGetLocal(canvas);
    MatrixGetRight(pos, local);
    Vector_mul(halfRight, pos, 0.5f);

    unsigned count = U(P(self, 0x24), 0x0);
    for (unsigned i = 0; i < count; ++i) {
        void *enemy = ((void **)P(enemies, 0x4))[i];
        void (**vt)(void) = *(void (***)(void))enemy;
        ((void (*)(void *, void *))vt[0x28 / 4])(enemy, target);

        ((void (*)(void *, void *))project)(screen, target);
        Vector_sub(pos, target, halfRight);
        ((void (*)(void *, void *))project)(screen, pos);
        PaintCanvas_GetScreenPosition(canvas, screen);

        Vector_add(pos, target, halfRight);
        ((void (*)(void *, void *))project)(screen, pos);
        PaintCanvas_GetScreenPosition(canvas, screen);

        current = PaintCanvas_CameraGetCurrent(canvas);
        local = PaintCanvas_CameraGetLocal(canvas);
        Matrix_assign(camera, local);
        MatrixGetPosition(pos, camera);
        MatrixGetUp(up, camera);
        MatrixGetLookAt(look, pos, target, up);
        Matrix_assign(camera, look);

        void *mapped = ((void **)P(enemies, 0x4))[U(P(self, 0x24), 0x0) + i];
        AEGeometry_setMatrix(P(mapped, 0x8));

        ((void (*)(void *, void *))project)(B(self, 0x4c), target);
        MatrixGetDir(pos, look);
        F(self, 0x4c) -= (*(float *)pos) * 20.0f;
        MatrixGetDir(pos, look);
        F(self, 0x54) -= (*(float *)(pos + 8)) * 20.0f;

        ((void (*)(void *, void *))(*(void ***)mapped)[0x44 / 4])(mapped, B(self, 0x4c));

        if (SolarSystem_getRace(Status_getSystem()) == 0) {
            MatrixSetRotation(look, 0.0f, 0.0f, 0.0f);
            Matrix_mul_assign(camera, look);
        }

        AEGeometry_setMatrix(P(enemy, 0x8));
        ((void (*)(void *, void *))vt[0x44 / 4])(enemy, target);
    }
}

// ---- drawLounge_170d38.cpp ----
static inline void *button_at(SpaceLounge *self, unsigned i)
{
    return ((void **)P(P(self, 0x5c), 0x4))[i];
}

void SpaceLounge::drawLounge() {
    SpaceLounge *self = this;
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
            if (((Agent *)(agent))->isKnown() != 0 || ((Agent *)(agent))->isStoryAgent() != 0) {
                ((Agent *)(agent))->getName();
            } else {
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                void *text = ((GameText *)(*(void **)texts))->getText(Agent_getRace(agent) + 0x196);
                ((String *)(s0))->ctor_copy((String *)text, false);
            }

            void *mission = ((Agent *)(agent))->getMission();
            if (mission != 0) {
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                void *text = ((GameText *)(*(void **)texts))->getText(Mission_getType(mission) + 0x162);
                ((String *)(s1))->ctor_copy((String *)text, false);
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
                    ((String *)(s1))->ctor_copy((String *)((GameText *)(*(void **)texts))->getText(id), false);
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
            ((Layout *)(layout))->drawBox6(2, boxX, boxY, width, I(layout, 0x30), s2);
            ((String *)(s2))->dtor();
            PaintCanvas_DrawRectangle(canvas, boxX, boxY, width, I(layout, 0x30));

            String_ctor_cstr(s3, ((Agent *)(agent))->isKnown() == 0 ? "?" : "", false);
            String_add(s4, s3, s0);
            PaintCanvas_DrawString(canvas, font, s4, cx, y + I(layout, 0x2c0));
            ((String *)(s4))->dtor();
            ((String *)(s3))->dtor();

            if (((Agent *)(agent))->isKnown() != 0) {
                String_ctor_cstr(s5, " ", false);
                String_add(s6, s5, s0);
                int nameWidth = PaintCanvas_GetTextWidth(canvas, font);
                PaintCanvas_DrawString(canvas, font, s1, cx + nameWidth, y + I(layout, 0x2c0));
                ((String *)(s6))->dtor();
                ((String *)(s5))->dtor();
            }

            ((String *)(s1))->dtor();
            ((String *)(s0))->dtor();
        }
        return;
    }

    PaintCanvas_SetColor(canvas, -1);
    String_ctor_cstr(s0, "", false);
    ((Layout *)(layout))->drawBox6(2, I(self, 0x70), I(self, 0x74), I(layout, 0x68), I(layout, 0x6c), s0);
    ((String *)(s0))->dtor();
    PaintCanvas_DrawRectangle(canvas, I(self, 0x70), I(self, 0x74), I(layout, 0x68), I(layout, 0x6c));
    ((ImageFactory *)(factory))->drawChar((Arr *)((void **)P(P(self, 0x38), 0x4))[I(self, 0x20)], I(layout, 0x4c) + I(self, 0x70), I(layout, 0x4c) + I(self, 0x74), false);
    ScrollTouchWindow_draw(P(self, 0x60));

    if ((I(self, 0x14) & 0xfffffffe) != 2) {
        return;
    }

    ((TouchButton *)(button_at(self, 0)))->setTextColor(-1);
    int offer = Agent_getOffer(((void **)P(P(self, 0x24), 0x4))[I(self, 0x20)]);
    if (I(self, 0x14) == 2) {
        ((TouchButton *)(button_at(self, 0)))->setPosition2(I(self, 0x84), I(self, 0x80));
        TouchButton_setPosition3(button_at(self, 1), I(self, 0x6c) + I(self, 0x84), I(self, 0x80), 0x12);
        I(self, 0x68) = 0;
        if (offer < 11 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            I(self, 0x68) = 3;
        } else if (offer == 1) {
            I(self, 0x68) = 1;
            ((TouchButton *)(button_at(self, 0)))->setPosition2(I(self, 0x84), I(self, 0x7c));
        } else {
            I(self, 0x68) = 2;
            ((TouchButton *)(button_at(self, 0)))->setPosition2(I(self, 0x84), I(self, 0x7c));
            TouchButton_setPosition3(button_at(self, 1), I(self, 0x6c) + I(self, 0x84), I(self, 0x7c), 0x12);
        }
    } else {
        I(self, 0x68) = 1;
        ((TouchButton *)(button_at(self, 0)))->setTextColor(-1);
        ((TouchButton *)(button_at(self, 0)))->setPosition2(I(self, 0x84), I(self, 0x7c));
    }

    for (unsigned i = 0; i < U(P(self, 0x5c), 0x0); ++i) {
        ((TouchButton *)(button_at(self, i)))->setVisible(false);
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
    ((Layout *)(layout))->drawBox6(2, I(self, 0x70), I(self, 0x78), I(layout, 0x68), panelHeight, s0);
    ((String *)(s0))->dtor();
    PaintCanvas_DrawRectangle(canvas, I(self, 0x70), I(self, 0x78), I(layout, 0x68), panelHeight);
    ((ImageFactory *)(factory))->drawChar((Arr *)P(self, 0x3c), I(layout, 0x4c) + I(self, 0x70), I(self, 0x78) + I(layout, 0x4c), true);

    ((TouchButton *)(button_at(self, 0)))->setVisible(true);
    if (!(offer == 1 || UC(self, 0x36) != 0 || I(self, 0x14) == 3)) {
        ((TouchButton *)(button_at(self, 1)))->setVisible(true);
        ((TouchButton *)(button_at(self, 0)))->draw();
        ((TouchButton *)(button_at(self, 1)))->draw();
        if (offer <= 10 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            ((TouchButton *)(button_at(self, 2)))->setVisible(true);
        }
    }
    ((TouchButton *)(button_at(self, 0)))->draw();
}

// ---- init_169f80.cpp ----
int SpaceLounge::init() {
    SpaceLounge *self = this;
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
    void *text = ((GameText *)(*(void **)textsSlot))->getText(0);
    int baseY = I(self, 0x78) + I(layout, 0x4c);
    I(self, 0x84) = I(self, 0x70) + I(layout, 0x68) - I(layout, 0x4c) - I(self, 0x6c);
    I(self, 0x7c) = (baseY + I(layout, 0x2d8) / 2) - I(layout, 0x30) / 2;
    I(self, 0x80) = baseY;

    for (unsigned i = 0; i < 5; ++i) {
        void *button = operator_new(200);
        TouchButton_ctor(button, text, 0, I(self, 0x84), baseY + (int)i * (I(layout, 0x30) + I(layout, 0x34)),
                         I(self, 0x6c), 0x11, 4);
        ((void **)P(buttons, 0x4))[i] = button;
        ((TouchButton *)(button))->setTextColor(-1);
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
    ((SpaceLounge *)(self))->updateScreenPositions();
    return 0;
}

// ---- SpaceLounge_169ad0.cpp ----
extern "C" SpaceLounge *_ZN11SpaceLoungeC2Ev(SpaceLounge *self)
{
    char from[60];
    char to[60];

    I(self, 0x8c) = 0;
    I(self, 0x90) = 0;
    I(self, 0x94) = 0;
    I(self, 0x98) = 0;
    I(self, 0x4c) = 0;
    I(self, 0x50) = 0;
    I(self, 0x54) = 0;
    I(self, 0x9c) = 0;
    I(self, 0xa0) = 0;
    String_ctor_default(B(self, 0xa4));
    Matrix_ctor(B(self, 0xc8));

    UC(self, 0xb0) = 0;
    P(self, 0x4) = 0;
    P(self, 0x8) = 0;
    P(self, 0xc) = 0;
    I(self, 0x34) = 0;
    UC(self, 0x36) = 0;
    P(self, 0x58) = 0;
    P(self, 0x5c) = 0;
    P(self, 0xc0) = 0;
    UC(self, 0x1c) = 0;
    I(self, 0x14) = 0;
    I(self, 0x18) = 0;
    P(self, 0x48) = 0;
    I(self, 0x20) = 0;
    P(self, 0x24) = 0;
    P(self, 0x28) = 0;
    P(self, 0x2c) = 0;
    P(self, 0x38) = 0;
    P(self, 0x3c) = 0;
    P(self, 0x40) = 0;
    P(self, 0x44) = 0;

    ((SpaceLounge *)(self))->init();

    void *agents = P(self, 0x24);
    if (agents != 0) {
        for (unsigned i = 0; i < U(agents, 0x0); ++i) {
            void *agent = ((void **)P(agents, 0x4))[i];
            int offer = Agent_getOffer(agent);
            if ((offer == 6 || offer == 0) && ((Agent *)(agent))->getMission() != 0) {
                void *mission = ((Agent *)(agent))->getMission();
                if (Mission_getType(mission) == 0xc && ((Agent *)(agent))->hasAcceptedOffer() != 0) {
                    ArrayRemove_AgentPtr(agent, P(self, 0x24));
                    ((SpaceLounge *)(self))->init();
                    break;
                }
            }
            agents = P(self, 0x24);
        }
    }

    void *cutscene = P(self, 0x44);
    if (cutscene == 0) {
        cutscene = operator_new(0xa0);
        CutScene_ctor(cutscene, 4);
        P(self, 0x44) = cutscene;
    }
    while (CutScene_isInitialized(cutscene) == 0) {
        CutScene_initialize(cutscene);
        cutscene = P(self, 0x44);
    }

    int race = SolarSystem_getRace(Status_getSystem());
    MatrixSetTranslation(from, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(from, 0.0f, 0.0f, 0.0f);
    MatrixSetTranslation(to, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(to, 0.0f, 0.0f, 0.0f);
    void *ease = operator_new(0xf4);
    EaseInOutMatrix_ctor(ease, from, to, 3000);
    P(self, 0x48) = ease;

    void *cameraSlot = *(void **)&SpaceLounge_ctor_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = PaintCanvas_CameraGetCurrent(camera);
    PaintCanvas_CameraSetLocal(camera, current);
    UC(self, 0xb0) = 1;
    UC(self, 0xbd) = 0;
    return self;
}

// ---- startChat_16aa7c.cpp ----
void SpaceLounge::startChat() {
    SpaceLounge *self = this;
    char title[12];
    char body[12];
    char left[12];
    char right[12];
    char scratch[1900];
    (void)scratch;

    if (P(self, 0x24) == 0 || I(self, 0x20) < 0) {
        return;
    }

    void *agent = ((void **)P(P(self, 0x24), 0x4))[I(self, 0x20)];
    int offer = Agent_getOffer(agent);
    void *mission = ((Agent *)(agent))->getMission();
    void *texts = *(void **)&SpaceLounge_start_text_slot;

    int titleId = 0x100;
    int bodyId = 0x101;
    if (mission != 0) {
        titleId = 0x120;
        bodyId = 0x121;
    } else if (offer == 1) {
        titleId = 0x130;
        bodyId = 0x131;
    } else if (offer == 6) {
        titleId = 0x132;
        bodyId = 0x133;
    }

    ((String *)(title))->ctor_copy((String *)((GameText *)(*(void **)texts))->getText(titleId), false);
    ((String *)(body))->ctor_copy((String *)((GameText *)(*(void **)texts))->getText(bodyId), false);
    ((String *)(left))->ctor_copy((String *)((GameText *)(*(void **)texts))->getText(0x10), false);
    ((String *)(right))->ctor_copy((String *)((GameText *)(*(void **)texts))->getText(0x11), false);

    ChoiceWindow_setText(P(self, 0x8), title, body);
    ChoiceWindow_setButtonText(P(self, 0x8), left, right);

    if (((Agent *)(agent))->isKnown() == 0 && ((Agent *)(agent))->isStoryAgent() == 0) {
        Agent_setKnown(agent, true);
    }
    SpaceLounge_getSoundId(self, agent);

    UC(self, 0x1b) = 1;
    UC(self, 0x19) = 1;
    I(self, 0x14) = 1;

    if (offer == 1) {
        I(self, 0x14) = 3;
    } else if (((Agent *)(agent))->isGenericAgent() != 0) {
        Agent_setEvent(agent, 1);
    }

    ((String *)(right))->dtor();
    ((String *)(left))->dtor();
    ((String *)(body))->dtor();
    ((String *)(title))->dtor();
}

// ---- _SpaceLounge_16a89a.cpp ----
extern "C" void *_ZN11SpaceLoungeD2Ev(SpaceLounge *self)
{
    void *p = P(self, 0x8);
    if (p != 0) {
        operator_delete(ChoiceWindow_dtor(p));
    }
    P(self, 0x8) = 0;

    p = P(self, 0x44);
    if (p != 0) {
        operator_delete(CutScene_dtor(p));
    }
    P(self, 0x44) = 0;

    operator_delete_arr(P(self, 0x58));
    P(self, 0x58) = 0;

    p = P(self, 0x28);
    if (p != 0) {
        ArrayRelease_StringPtr(p);
        p = P(self, 0x28);
        if (p != 0) {
            operator_delete(Array_StringPtr_dtor(p));
        }
    }
    P(self, 0x28) = 0;

    p = P(self, 0x5c);
    if (p != 0) {
        ArrayRelease_TouchButtonPtr(p);
        p = P(self, 0x5c);
        if (p != 0) {
            operator_delete(Array_TouchButtonPtr_dtor(p));
        }
    }
    P(self, 0x5c) = 0;

    if (I(self, 0x3c) != 0) {
        return self;
    }
    I(self, 0x3c) = 0;

    p = P(self, 0x38);
    if (p != 0) {
        unsigned i = 0;
        for (; i < U(p, 0x0); ++i) {
            void **data = (void **)P(p, 0x4);
            void *inner = data[i];
            ArrayRelease_ImagePartPtr(inner);
            inner = ((void **)P(P(self, 0x38), 0x4))[i];
            if (inner != 0) {
                operator_delete(Array_ImagePartPtr_dtor(inner));
                ((void **)P(P(self, 0x38), 0x4))[i] = 0;
            } else {
                data[i] = 0;
            }
            p = P(self, 0x38);
        }
        ArrayRelease_ArrayImagePartPtr(p);
        p = P(self, 0x38);
        if (p != 0) {
            operator_delete(Array_ArrayImagePartPtr_dtor(p));
        }
    }
    P(self, 0x38) = 0;

    p = P(self, 0x40);
    if (p != 0) {
        ArrayRelease_VectorPtr(p);
        p = P(self, 0x40);
        if (p != 0) {
            operator_delete(Array_VectorPtr_dtor(p));
        }
    }
    P(self, 0x40) = 0;

    p = P(self, 0x48);
    if (p != 0) {
        operator_delete(EaseInOutMatrix_dtor(p));
    }
    P(self, 0x48) = 0;

    p = P(self, 0xc0);
    if (p != 0) {
        operator_delete(p);
    }
    P(self, 0xc0) = 0;
    ((String *)(B(self, 0xa4)))->dtor();
    return self;
}

// ---- draw_170bec.cpp ----
void SpaceLounge::draw() {
    SpaceLounge *self = this;
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
    void *text = ((GameText *)(*(void **)textsSlot))->getText(0x18e);
    ((String *)(title))->ctor_copy((String *)text, false);
    Layout_drawHeader_call(layout, title);
    ((String *)(title))->dtor();

    ((SpaceLounge *)(self))->drawLounge();

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

// ---- update_170738.cpp ----
void SpaceLounge::update(int dt) {
    SpaceLounge *self = this;
    char maxMatrix[60];
    char valueMatrix[60];

    if (dt > 50) {
        dt = 50;
    }
    if (UC(self, 0xb0) == 0) {
        return;
    }

    if (UC(self, 0x34) != 0) {
        return SpaceLounge_update_map_tail(P(self, 0x4), dt);
    }
    if (UC(self, 0x1c) != 0) {
        return SpaceLounge_update_ship_tail(P(self, 0xc), dt);
    }

    if (UC(self, 0xbc) == 0) {
        EaseInOutMatrix_Increase(P(self, 0x48), (float)dt);
    }

    void *cameraSlot;
    void *camera;
    void *current;
    if (UC(self, 0xbd) == 0) {
        EaseInOutMatrix_GetMaxValue(maxMatrix, P(self, 0x48));
        EaseInOutMatrix_GetValue(valueMatrix, P(self, 0x48));
        if (Matrix_equal(maxMatrix, valueMatrix) == 0) {
            UC(self, 0xbc) = 0;
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_c;
            camera = *(void **)cameraSlot;
            current = PaintCanvas_CameraGetCurrent(camera);
            EaseInOutMatrix_GetValue(valueMatrix, P(self, 0x48));
            PaintCanvas_CameraSetLocal(camera, valueMatrix);
        } else {
            goto idle_camera;
        }
    } else {
idle_camera:
        float step = (float)dt * 0.001f;
        if (step > 1.0f) {
            step = 1.0f;
        }
        if (step < 0.0f) {
            step = 0.0f;
        }
        UC(self, 0xbd) = 1;
        F(self, 0x104) = F(self, 0x104) + step;
        float wave = Sinf(F(self, 0x104));
        float maxRot = (float)I(self, 0x108) * 0.5f;

        if (UC(self, 0xbc) == 0) {
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_a;
            camera = *(void **)cameraSlot;
            current = PaintCanvas_CameraGetLocal(camera);
            (void)current;
            UC(self, 0xbc) = 1;
            int amount = AERandom_nextInt(*(void **)&SpaceLounge_update_random_slot, 10);
            UC(self, 0xc4) = 0;
            if (P(self, 0xc0) == 0) {
                void *ease = operator_new(0x10);
                EaseInOut_ctor(ease, 0.0f, (float)amount);
                P(self, 0xc0) = ease;
            } else {
                EaseInOut_SetRange(P(self, 0xc0), 0.0f, (float)amount);
            }
            I(self, 0x108) = 2;
        } else {
            float value = EaseInOut_GetValue(P(self, 0xc0));
            float maxValue = EaseInOut_GetMaxValue(P(self, 0xc0));
            float distance = value - maxValue;
            if (distance < 0.0f) {
                distance = -distance;
            }
            int amount = I(self, 0x108);
            if (distance < 0.25f) {
                void *random = *(void **)&SpaceLounge_update_random_slot;
                amount = AERandom_nextInt(random, 10);
                float next = (float)(5 - amount);
                UC(self, 0xc4) = value > next;
                EaseInOut_SetRange(P(self, 0xc0), value, next);
                I(self, 0x108) = AERandom_nextInt(random, 4) + 1;
                amount = I(self, 0x108);
            }
            if (UC(self, 0xc4) != 0) {
                amount = -amount;
            }
            EaseInOut_Increase(P(self, 0xc0), (float)(dt * amount));
        }

        float value = EaseInOut_GetValue(P(self, 0xc0));
        MatrixSetRotation(valueMatrix, value / 90.0f, 0.0f, value);
        MatrixSetTranslation(maxMatrix, 0.0f, 0.0f, wave * maxRot);
        Matrix_mul_assign(valueMatrix, B(self, 0xc8));
        cameraSlot = *(void **)&SpaceLounge_update_camera_slot_b;
        camera = *(void **)cameraSlot;
        current = PaintCanvas_CameraGetCurrent(camera);
        PaintCanvas_CameraSetLocal(camera, valueMatrix);
    }

    ((SpaceLounge *)(self))->updateScreenPositions();
    if (I(self, 0x14) != 0) {
        ScrollTouchWindow_update(P(self, 0x60), dt);
    }
    CutScene_update(P(self, 0x44));

    if (UC(self, 0xb2) != 0) {
        int x = I(self, 0xb4);
        int y = I(self, 0xb8);
        int top = *(int *)SpaceLounge_update_top_slot;
        int height = *(int *)SpaceLounge_update_height_slot;
        int width = *(int *)SpaceLounge_update_width_slot;
        void *cutscene = P(self, 0x44);
        if (y < top) {
            if (x < 150) {
                F(cutscene, 0xc) -= 10.0f;
            } else if (x > width - 150) {
                F(cutscene, 0xc) += 10.0f;
            }
        } else if (y < height - top * 2) {
            if (x < 70) {
                F(cutscene, 0x4) -= 7.5f;
            } else if (x > width - 70) {
                F(cutscene, 0x4) += 7.5f;
            } else if (x > 100 && x < width - 100) {
                if (y < height / 2) {
                    F(cutscene, 0x10) -= (float)dt;
                } else {
                    F(cutscene, 0x10) += (float)dt;
                }
            }
        } else {
            if (x < 70) {
                F(cutscene, 0x8) -= (float)dt;
            } else if (x > width - 70) {
                F(cutscene, 0x8) += (float)dt;
            }
        }
    }
}
