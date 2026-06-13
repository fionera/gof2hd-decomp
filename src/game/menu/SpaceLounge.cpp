#include <cstring>
#include "gof2/game/menu/SpaceLounge.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/externs.h"
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/game/core/CutScene.h"
#include "gof2/engine/math/EaseInOut.h"
#include "gof2/engine/math/EaseInOutMatrix.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/ui/ListItemWindow.h"
#include "gof2/game/ui/ScrollTouchWindow.h"
#include "gof2/game/world/StarMap.h"
#include "gof2/game/ship/Agent.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/core/String.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ui/TouchButton.h"

// Short byte-offset accessors (this TU still uses the offset idiom for not-yet-modeled
// fields). Guarded so it coexists with any header that defines the same block.
#ifndef GOF2_BIP_HELPERS
#define GOF2_BIP_HELPERS
static inline char*  B(void *p, int off) { return (char *)p + off; }
static inline int&   I(void *p, int off) { return *(int *)((char *)p + off); }
static inline void*& P(void *p, int off) { return *(void **)((char *)p + off); }
#endif

// This translation unit pulls in externs.h/fwd.h (which forward-declares the global
// `PaintCanvas` type) and additionally used `AbyssEngine::PaintCanvas` as a *namespace*
// of free thunks. Both clash with including the real PaintCanvas class header, so we
// declare the global PaintCanvas class (whose mangled `PaintCanvas::` symbols match the
// definitions in PaintCanvas.cpp) with exactly the methods this unit calls and cast the
// engine canvas pointer to it. GetWidth()/GetHeight() return void in the recovered class;
// the int-returning impl is exposed as pc_GetWidth/pc_GetHeight.
struct PaintCanvas {
    void SetColor(unsigned int color);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void FillRectangle(int x, int y, int w, int h);
    void DrawRectangle(int x, int y, int w, int h);
    int  GetTextWidth(unsigned int index, void *str);
    void DrawString(unsigned int index, void *str, int x, int y, bool b);
    unsigned int CameraGetCurrent();
    void *CameraGetLocal(unsigned int index);
    void CameraSetLocal(unsigned int index, const AbyssEngine::AEMath::Matrix &matrix);
    void *GetScreenPosition(AbyssEngine::AEMath::Vector *a, AbyssEngine::AEMath::Vector *b);
};
extern "C" int pc_GetWidth(PaintCanvas *self);
extern "C" int pc_GetHeight(PaintCanvas *self);

namespace AbyssEngine { namespace AEMath {
    Vector operator+(const Vector&, const Vector&);
    Vector operator-(const Vector&, const Vector&);
    Vector operator*(const Vector&, float);
    void MatrixMultiply(Matrix&, const Matrix&);
} }

extern "C" void *SpaceLounge_layout_move;
extern "C" void *SpaceLounge_layout_begin;
// Dropped-self Status singleton accessors: the decompiler emitted these calls
// with no receiver argument (the Status* singleton is loaded inside the thunk).
// The singleton is `*g_status`; call the real methods through it.
extern "C" __attribute__((visibility("hidden"))) Status **g_status;
void MatrixSetTranslation(void *matrix, float x, float y, float z);
void MatrixSetRotation(void *matrix, float x, float y, float z);
extern "C" void *SpaceLounge_touch_layout_slot;
extern "C" void *SpaceLounge_touch_help_text_slot;
extern "C" void *SpaceLounge_touch_list_help_text_slot;
extern "C" void *SpaceLounge_touch_camera_slot;
extern "C" int SpaceLounge_touch_race_vectors[];
void Globals_getAgentMissionText(void *out, int textId, void *agent);
namespace AbyssEngine { namespace AERandom { int nextInt(void *random, int limit); } }
int SpaceLounge_getSpecificSoundForRace(int, unsigned soundId, int race, bool alternate);
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
void MatrixGetRight(void *out, void *matrix);
void MatrixGetPosition(void *out, void *matrix);
void MatrixGetUp(void *out, void *matrix);
void MatrixGetLookAt(void *out, void *pos, void *target, void *up);
void MatrixGetDir(void *out, void *matrix);
extern "C" void *SpaceLounge_screen_level_slot;
extern "C" void *SpaceLounge_screen_canvas_slot;
extern "C" void *SpaceLounge_screen_projector;
extern "C" void *SpaceLounge_lounge_canvas_slot;
extern "C" void *SpaceLounge_lounge_layout_slot;
extern "C" void *SpaceLounge_lounge_image_factory_slot;
extern "C" void *SpaceLounge_lounge_text_slot;
extern "C" void *SpaceLounge_lounge_font_slot;
extern "C" void *Station_getAgents(void *station);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void ChoiceWindow_ctor(void *choice);
extern "C" void *ScrollTouchWindow_ctor(void *scroll, int x, int y, int w, int h, bool vertical);
extern "C" void *SpaceLounge_init_layout_slot;
extern "C" void *SpaceLounge_init_text_slot;
extern "C" void *SpaceLounge_init_camera_slot;
extern "C" void CutScene_ctor(void *cutscene, int id);
extern "C" void ArrayRemove_AgentPtr(void *agent, void *array);
extern "C" void EaseInOutMatrix_ctor(void *ease, void *from, void *to, int duration);
extern "C" void *SpaceLounge_ctor_camera_slot;
int SpaceLounge_getSoundId(SpaceLounge *self, void *agent);
extern "C" void *SpaceLounge_start_text_slot;
extern "C" void *CutScene_dtor(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void *SpaceLounge_draw_layout_slot;
extern "C" void *SpaceLounge_draw_canvas_slot;
extern "C" void *SpaceLounge_draw_text_slot;
float Sinf(float value);
extern "C" void EaseInOut_ctor(void *ease, float start, float end);
extern "C" float EaseInOut_GetValue(void *ease);
extern "C" void *SpaceLounge_update_camera_slot_a;
extern "C" void *SpaceLounge_update_camera_slot_b;
extern "C" void *SpaceLounge_update_camera_slot_c;
extern "C" void *SpaceLounge_update_random_slot;
extern "C" int *SpaceLounge_update_width_slot;
extern "C" int *SpaceLounge_update_height_slot;
extern "C" int *SpaceLounge_update_top_slot;

int SpaceLounge::OnTouchMove(int x, int y) {
    I(this, 0xb4) = x;
    I(this, 0xb8) = y;

    if (UC(this, 0x1b) != 0 || UC(this, 0x19) != 0) {
        ((ChoiceWindow *)(P(this, 0x8)))->OnTouchMove(x, y);
        return 0;
    }

    if (UC(this, 0x34) != 0) {
        ((StarMap *)(P(this, 0x4)))->OnTouchMove(x, y);
        return 0;
    }

    int mode = I(this, 0x14);
    if (mode == 0) {
        Array<Agent *> *agents = (Array<Agent *> *)P(this, 0x24);
        if (agents != 0) {
            Array<Vector *> *rects = (Array<Vector *> *)P(this, 0x40);
            float fy = (float)y;
            float fx = (float)x;
            I(this, 0x88) = -1;
            I(this, 0x20) = -1;
            int count = (int)agents->size();
            for (int i = 0; count != i; ++i) {
                float *a = (float *)(*rects)[i * 2];
                if (a[0] < fx) {
                    float *b = (float *)(*rects)[i * 2 + 1];
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        I(this, 0x88) = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        Array<TouchButton *> *buttons = (Array<TouchButton *> *)P(this, 0x5c);
        (*buttons)[0]->OnTouchMove(x, y);
    } else if (mode == 2) {
        Array<TouchButton *> *buttons = (Array<TouchButton *> *)P(this, 0x5c);
        for (unsigned i = 0; i < buttons->size(); ++i) {
            (*buttons)[i]->OnTouchMove(x, y);
        }
    }

    ((Layout *)(*(void **)SpaceLounge_layout_move))->OnTouchMove(x, y);
    if (UC(this, 0x1c) != 0) {
        ((ListItemWindow *)(P(this, 0xc)))->OnTouchMove(x, y);
    } else {
        ((ScrollTouchWindow *)(P(this, 0x60)))->OnTouchMove(x, y);
    }
    return 0;
}

void SpaceLounge::OnRender3D() {
    if (UC(this, 0x34) != 0) {
        return this->OnRender3D_map_tail(P(this, 0x4));
    }

    void *cutscene = P(this, 0x44);
    if (cutscene == 0) {
        return;
    }

    if (UC(this, 0x1c) != 0) {
        if (((ListItemWindow *)(P(this, 0xc)))->shows3DShip() != 0) {
            return;
        }
        cutscene = P(this, 0x44);
    }

    return this->OnRender3D_cutscene_tail(cutscene);
}

void SpaceLounge::OnRenderBG() {
    if (I(this, 0x44) != 0) {
        return this->OnRenderBG_tail(P(this, 0x44));
    }
}

unsigned char SpaceLounge::introFinished() {
    return UC(this, 0xbd);
}

bool SpaceLounge::listMode() {
    return UC(this, 0x34) == 0;
}

unsigned char SpaceLounge::mapMode() {
    return UC(this, 0x34);
}

unsigned char SpaceLounge::hangarNeedsUpdate() {
    return UC(this, 0x35);
}

void SpaceLounge::draw3DShip() {
    if (UC(this, 0x1c) != 0) {
        return this->draw3DShip_tail(P(this, 0xc));
    }
}

void SpaceLounge::setHangarUpdate(bool value) {
    BL(this, 0x35) = value;
}

int SpaceLounge::OnTouchBegin(int x, int y) {
    UC(this, 0xb2) = 1;
    I(this, 0xb4) = x;
    I(this, 0xb8) = y;
    I(this, 0x88) = -1;

    if (UC(this, 0x1b) != 0 || UC(this, 0x19) != 0) {
        ((ChoiceWindow *)(P(this, 0x8)))->OnTouchBegin(x, y);
        return 0;
    }

    if (UC(this, 0x34) != 0) {
        ((StarMap *)(P(this, 0x4)))->OnTouchBegin(x, y);
        return 0;
    }

    if (UC(this, 0x1c) != 0) {
        ((ListItemWindow *)(P(this, 0xc)))->OnTouchBegin(x, y);
        ((Layout *)(*(void **)SpaceLounge_layout_begin))->OnTouchBegin(x, y);
        ((ScrollTouchWindow *)(P(this, 0x60)))->OnTouchBegin(x, y);
        return 0;
    }

    int mode = I(this, 0x14);
    if (mode == 0) {
        if (UC(this, 0xbd) == 0) {
            return 0;
        }
        Array<Agent *> *agents = (Array<Agent *> *)P(this, 0x24);
        if (agents != 0) {
            Array<Vector *> *rects = (Array<Vector *> *)P(this, 0x40);
            float fy = (float)y;
            float fx = (float)x;
            int count = (int)agents->size();
            for (int i = 0; count != i; ++i) {
                float *a = (float *)(*rects)[i * 2];
                if (a[0] < fx) {
                    float *b = (float *)(*rects)[i * 2 + 1];
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        I(this, 0x88) = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        Array<TouchButton *> *buttons = (Array<TouchButton *> *)P(this, 0x5c);
        (*buttons)[0]->OnTouchBegin(x, y);
    } else if (mode == 2) {
        Array<TouchButton *> *buttons = (Array<TouchButton *> *)P(this, 0x5c);
        for (unsigned i = 0; i < buttons->size(); ++i) {
            (*buttons)[i]->OnTouchBegin(x, y);
        }
    }

    ((Layout *)(*(void **)SpaceLounge_layout_begin))->OnTouchBegin(x, y);
    ((ScrollTouchWindow *)(P(this, 0x60)))->OnTouchBegin(x, y);
    return 0;
}

int SpaceLounge_getSpecificSoundForRace(int, unsigned soundId, int race, bool alternate)
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

static inline void *selected_agent(SpaceLounge *self)
{
    Array<Agent *> *agents = (Array<Agent *> *)P(self, 0x24);
    return (*agents)[I(self, 0x20)];
}

static inline bool hit_agent(SpaceLounge *self, int x, int y, int i)
{
    Array<Vector *> *rects = (Array<Vector *> *)P(self, 0x40);
    float *a = (float *)(*rects)[i * 2];
    if (!(a[0] < (float)x)) {
        return false;
    }
    float *b = (float *)(*rects)[i * 2 + 1];
    return (float)x < b[0] && (float)y < a[1] && b[1] < (float)y;
}

void SpaceLounge::OnTouchEnd(int x, int y) {
    String helpBig;
    String helpSmall;
    char matrix[60];

    UC(this, 0xb2) = 0;

    if (UC(this, 0x1b) != 0 || UC(this, 0x19) != 0) {
        int result = ((ChoiceWindow *)(P(this, 0x8)))->touch_end(x, y);
        if (result == 1) {
            UC(this, 0x19) = 0;
        } else if (result == 0) {
            ((SpaceLounge *)(this))->onKeyPress(0x10000);
        }
        return;
    }

    if (UC(this, 0x34) != 0) {
        if (((StarMap *)(P(this, 0x4)))->touch_end(x, y) != 0) {
            ((CutScene *)(P(this, 0x44)))->resetCamera();
            UC(this, 0x34) = 0;
        }
        return;
    }

    void *layoutSlot = *(void **)&SpaceLounge_touch_layout_slot;
    void *layout = *(void **)layoutSlot;
    if (((Layout *)(layout))->touch_end(x, y) != 0) {
        if (UC(this, 0x1c) != 0) {
            ((Layout *)(layout))->resetWindowDimensions();
            UC(this, 0x1c) = 0;
        } else if (I(this, 0x14) != 0) {
            if (I(this, 0x20) >= 0) {
                void *agent = selected_agent(this);
                if (((Agent *)(agent))->isGenericAgent() != 0) {
                    ((Agent *)(agent))->setEvent(1);
                }
            }
            I(this, 0x14) = 0;
            UC(this, 0x36) = 0;
        }
        return;
    }

    if (UC(this, 0x1c) != 0) {
        ((ListItemWindow *)(P(this, 0xc)))->touch_end(x, y);
        if (((Layout *)(layout))->helpPressed() != 0) {
            void *texts = *(void **)&SpaceLounge_touch_list_help_text_slot;
            void *text = ((GameText *)(*(void **)texts))->getText(0x283);
            helpSmall.ctor_copy((String *)text, false);
            ((Layout *)(layout))->initHelpWindow(&helpSmall);
        }
        return;
    }

    switch (I(this, 0x14)) {
    case 0:
        if (UC(this, 0xbd) == 0) {
            void *system = (void *)(*g_status)->getSystem();
            int race = ((SolarSystem *)(system))->getRace();
            int *v = &SpaceLounge_touch_race_vectors[race * 3];
            MatrixSetTranslation(matrix, (float)v[2], (float)v[0], (float)v[1]);
            MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
            void *cameraSlot = *(void **)&SpaceLounge_touch_camera_slot;
            void *camera = *(void **)cameraSlot;
            void *current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
            ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)current);
            if (P(this, 0x48) != 0) {
                ((AbyssEngine::EaseInOutMatrix *)(P(this, 0x48)))->SetRange(*(AEMath::Matrix *)matrix, *(AEMath::Matrix *)matrix);
            }
            UC(this, 0xbd) = 1;
            I(this, 0x104) = 0;
            return;
        }
        if (P(this, 0x24) != 0) {
            I(this, 0x88) = -1;
            I(this, 0x20) = -1;
            unsigned count = (unsigned)((Array<Agent *> *)P(this, 0x24))->size();
            for (unsigned i = 0; i < count; ++i) {
                if (hit_agent(this, x, y, i)) {
                    I(this, 0x20) = i;
                    ((SpaceLounge *)(this))->onKeyPress(0x10000);
                    return;
                }
            }
        }
        break;
    case 1:
        ((SpaceLounge *)(this))->onKeyPress(0x10000);
        break;
    case 2: {
        Array<TouchButton *> *buttons = (Array<TouchButton *> *)P(this, 0x5c);
        for (unsigned i = 0; i < buttons->size(); ++i) {
            TouchButton *button = (*buttons)[i];
            if (button->touch_end(x, y) != 0) {
                void *agent = selected_agent(this);
                if (i >= 5 && ((Agent *)(agent))->isGenericAgent() != 0) {
                    ((Agent *)(agent))->setEvent(1);
                }
            }
        }
        break;
    }
    case 3:
        if ((*((Array<TouchButton *> *)P(this, 0x5c)))[0]->touch_end(x, y) != 0) {
            ((SpaceLounge *)(this))->onKeyPress(0x10000);
        }
        break;
    }

    ((ScrollTouchWindow *)(P(this, 0x60)))->touch_end(x, y);
    if (((Layout *)(layout))->helpPressed() != 0) {
        void *texts = *(void **)&SpaceLounge_touch_help_text_slot;
        void *text = ((GameText *)(*(void **)texts))->getText(0x273);
        helpBig.ctor_copy((String *)text, false);
        ((Layout *)(layout))->initHelpWindow(&helpBig);
    }
}

static inline int random_from(void *slot, int limit)
{
    return AbyssEngine::AERandom::nextInt(*(void **)slot, limit);
}

int SpaceLounge_getSoundId(SpaceLounge *, void *agent)
{
    String missionText;

    int race = ((Agent *)(agent))->getRace();
    bool male = ((Agent *)(agent))->isMale();
    int offer = ((Agent *)(agent))->getOffer();
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
            missionType = ((Mission *)(mission))->getType();
        }
    }

    Globals_getAgentMissionText(&missionText, *SpaceLounge_getSoundId_missionText, agent);

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
            int first = AbyssEngine::AERandom::nextInt(random, 2);
            if (first == 0) {
                soundId = AbyssEngine::AERandom::nextInt(random, 4) + 0x31f;
            } else {
                soundId = AbyssEngine::AERandom::nextInt(random, 4) + 0x309;
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
        if (missionText.Compare_str((String *)text) != 0) {
            text = ((GameText *)(*(void **)texts))->getText(0x338);
            if (missionText.Compare_str((String *)text) != 0) {
                text = ((GameText *)(*(void **)texts))->getText(0x33b);
                if (missionText.Compare_str((String *)text) != 0) {
                    text = ((GameText *)(*(void **)texts))->getText(0x341);
                    dummy = missionText.Compare_str((String *)text);
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
    return result;
}

static inline void *key_agent(SpaceLounge *self)
{
    return (*((Array<Agent *> *)P(self, 0x24)))[I(self, 0x20)];
}

void SpaceLounge::onKeyPress(int key) {
    char scratch[960];
    (void)scratch;

    if (UC(this, 0x1b) != 0) {
        if (key == 0x1000) {
            ((ChoiceWindow *)(P(this, 0x8)))->left();
        } else if (key == 0x2000) {
            ((ChoiceWindow *)(P(this, 0x8)))->right();
        } else if (key == 0x10000) {
            UC(this, 0x1b) = 0;
        }
        return;
    }

    int mode = I(this, 0x14);
    if (mode == 0) {
        if (key == 0x1000 || key == 0x8000) {
            unsigned next = I(this, 0x20) + 1;
            if (P(this, 0x24) != 0 && next >= ((Array<Agent *> *)P(this, 0x24))->size()) {
                next = 0;
            }
            I(this, 0x20) = next;
        } else if (key == 0x2000 || key == 0x4000) {
            int current = I(this, 0x20);
            if (current < 1 && P(this, 0x24) != 0) {
                current = (int)((Array<Agent *> *)P(this, 0x24))->size();
            }
            I(this, 0x20) = current - 1;
        } else if (key == 0x10000 || key == 0x20000) {
            ((SpaceLounge *)(this))->startChat();
        }
        return;
    }

    if (mode == 1 || mode == 3) {
        if (key == 0x10000 || key == 0x20000) {
            I(this, 0x30) = 0;
            void *agent = key_agent(this);
            if (((Agent *)(agent))->getOffer() == 1) {
                I(this, 0x14) = 2;
            } else {
                I(this, 0x14) = mode == 1 ? 2 : 0;
            }
        } else if (key == 0x40000) {
            if (I(this, 0x2c) < 3) {
                if (mode == 1) {
                    I(this, 0x2c) = 0;
                    I(this, 0x14) = 0;
                }
            } else {
                I(this, 0x2c) -= 0x28;
            }
        }
        return;
    }

    if (mode == 2) {
        void *agent = key_agent(this);
        if (key == 0x40000) {
            I(this, 0x14) = 1;
        } else if (key == 0x10000 || key == 0x20000) {
            if (((Agent *)(agent))->getMission() == 0 && ((Agent *)(agent))->isGenericAgent() != 0) {
                ((Agent *)(agent))->setEvent(1);
            }
            I(this, 0x14) = 0;
        } else if (key == 0x8000) {
            ((ScrollTouchWindow *)(P(this, 0x60)))->scroll(1);
        } else if (key == 0x4000) {
            ((ScrollTouchWindow *)(P(this, 0x60)))->scroll(-1);
        }
    }
}

void SpaceLounge::updateScreenPositions() {
    char camera[60];
    char look[60];
    char pos[12];
    char up[12];
    char target[12];
    char halfRight[12];
    char screen[12];

    if (P(this, 0x24) == 0) {
        return;
    }

    void *level = *(void **)&SpaceLounge_screen_level_slot;
    void *enemies = (void *)(intptr_t)((Level *)(level))->getEnemies();
    void *canvasSlot = *(void **)&SpaceLounge_screen_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *project = *(void **)&SpaceLounge_screen_projector;

    void *current = (void *)(long)((PaintCanvas *)canvas)->CameraGetCurrent();
    void *local = ((PaintCanvas *)canvas)->CameraGetLocal((unsigned int)(long)canvas);
    MatrixGetRight(pos, local);
    *(Vector*)(halfRight) = *(const Vector*)(pos) * (0.5f);

    unsigned count = (unsigned)((Array<Agent *> *)P(this, 0x24))->size();
    for (unsigned i = 0; i < count; ++i) {
        void *enemy = ((void **)P(enemies, 0x4))[i];
        void (**vt)(void) = *(void (***)(void))enemy;
        ((void (*)(void *, void *))vt[0x28 / 4])(enemy, target);

        ((void (*)(void *, void *))project)(screen, target);
        *(Vector*)(pos) = *(const Vector*)(target) - *(const Vector*)(halfRight);
        ((void (*)(void *, void *))project)(screen, pos);
        ((PaintCanvas *)canvas)->GetScreenPosition((AbyssEngine::AEMath::Vector *)canvas, (AbyssEngine::AEMath::Vector *)screen);

        *(Vector*)(pos) = *(const Vector*)(target) + *(const Vector*)(halfRight);
        ((void (*)(void *, void *))project)(screen, pos);
        ((PaintCanvas *)canvas)->GetScreenPosition((AbyssEngine::AEMath::Vector *)canvas, (AbyssEngine::AEMath::Vector *)screen);

        current = (void *)(long)((PaintCanvas *)canvas)->CameraGetCurrent();
        local = ((PaintCanvas *)canvas)->CameraGetLocal((unsigned int)(long)canvas);
        *(Matrix*)(camera) = *(const Matrix*)(local);
        MatrixGetPosition(pos, camera);
        MatrixGetUp(up, camera);
        MatrixGetLookAt(look, pos, target, up);
        *(Matrix*)(camera) = *(const Matrix*)(look);

        void *mapped = ((void **)P(enemies, 0x4))[count + i];
        ((AEGeometry *)P(mapped, 0x8))->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(camera));

        ((void (*)(void *, void *))project)(B(this, 0x4c), target);
        MatrixGetDir(pos, look);
        F(this, 0x4c) -= (*(float *)pos) * 20.0f;
        MatrixGetDir(pos, look);
        F(this, 0x54) -= (*(float *)(pos + 8)) * 20.0f;

        ((void (*)(void *, void *))(*(void ***)mapped)[0x44 / 4])(mapped, B(this, 0x4c));

        if (((SolarSystem *)((void *)(*g_status)->getSystem()))->getRace() == 0) {
            MatrixSetRotation(look, 0.0f, 0.0f, 0.0f);
            AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)(camera),*(const Matrix*)(look));
        }

        ((AEGeometry *)P(enemy, 0x8))->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(camera));
        ((void (*)(void *, void *))vt[0x44 / 4])(enemy, target);
    }
}

static inline void *button_at(SpaceLounge *self, unsigned i)
{
    return (*((Array<TouchButton *> *)P(self, 0x5c)))[i];
}

void SpaceLounge::drawLounge() {
    String s0, s1, s2, s3, s4, s5, s6;

    void *canvasSlot = *(void **)&SpaceLounge_lounge_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *layoutSlot = *(void **)&SpaceLounge_lounge_layout_slot;
    void *layout = *(void **)layoutSlot;
    void *factorySlot = *(void **)&SpaceLounge_lounge_image_factory_slot;
    void *factory = *(void **)factorySlot;

    if (I(this, 0x14) == 0) {
        int hover = I(this, 0x88);
        if (hover >= 0) {
            void *agent = (*((Array<Agent *> *)P(this, 0x24)))[hover];
            Array<Vector *> *rects = (Array<Vector *> *)P(this, 0x40);
            float *left = (float *)(*rects)[hover * 2];
            float *right = (float *)(*rects)[hover * 2 + 1];
            int pad = I(layout, 0x94);
            int cx = (int)(left[0] + (right[0] - left[0]) * 0.5f);
            int y = (int)(right[1] - (float)(pad * 2));

            ((PaintCanvas *)canvas)->SetColor((unsigned int)(long)canvas);
            if (((Agent *)(agent))->isKnown() != 0 || ((Agent *)(agent))->isStoryAgent() != 0) {
                s0 = ((Agent *)(agent))->getName();
            } else {
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                void *text = ((GameText *)(*(void **)texts))->getText(((Agent *)(agent))->getRace() + 0x196);
                s0.ctor_copy((String *)text, false);
            }

            void *mission = ((Agent *)(agent))->getMission();
            if (mission != 0) {
                void *texts = *(void **)&SpaceLounge_lounge_text_slot;
                void *text = ((GameText *)(*(void **)texts))->getText(((Mission *)(mission))->getType() + 0x162);
                s1.ctor_copy((String *)text, false);
            } else {
                int offer = ((Agent *)(agent))->getOffer();
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
                    s1.ctor_copy((String *)((GameText *)(*(void **)texts))->getText(id), false);
                } else {
                    s1 = "";
                }
            }

            void *font = *(void **)*(void **)&SpaceLounge_lounge_font_slot;
            int textWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(long)canvas, font);
            int boxX = cx - pad;
            int boxY = y - pad;
            int width = pad * 2 + textWidth;
            s2 = "";
            ((Layout *)(layout))->drawBox6(2, boxX, boxY, width, I(layout, 0x30), &s2);
            ((PaintCanvas *)canvas)->DrawRectangle(boxX, boxY, width, I(layout, 0x30));

            s3 = ((Agent *)(agent))->isKnown() == 0 ? "?" : "";
            s4 = s3 + s0;
            ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, &s4, cx, y + I(layout, 0x2c0), false);

            if (((Agent *)(agent))->isKnown() != 0) {
                s5 = " ";
                s6 = s5 + s0;
                int nameWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(long)canvas, font);
                ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, &s1, cx + nameWidth, y + I(layout, 0x2c0), false);
            }
        }
        return;
    }

    ((PaintCanvas *)canvas)->SetColor((unsigned int)(long)canvas);
    s0 = "";
    ((Layout *)(layout))->drawBox6(2, I(this, 0x70), I(this, 0x74), I(layout, 0x68), I(layout, 0x6c), &s0);
    ((PaintCanvas *)canvas)->DrawRectangle(I(this, 0x70), I(this, 0x74), I(layout, 0x68), I(layout, 0x6c));
    ((ImageFactory *)(factory))->drawChar((*((Array<Array<ImagePart *> *> *)P(this, 0x38)))[I(this, 0x20)], I(layout, 0x4c) + I(this, 0x70), I(layout, 0x4c) + I(this, 0x74), false);
    ((ScrollTouchWindow *)(P(this, 0x60)))->draw();

    if ((I(this, 0x14) & 0xfffffffe) != 2) {
        return;
    }

    ((TouchButton *)(button_at(this, 0)))->setTextColor(-1);
    int offer = ((Agent *)((*((Array<Agent *> *)P(this, 0x24)))[I(this, 0x20)]))->getOffer();
    if (I(this, 0x14) == 2) {
        ((TouchButton *)(button_at(this, 0)))->setPosition2(I(this, 0x84), I(this, 0x80));
        ((TouchButton *)(button_at(this, 1)))->setPosition3(I(this, 0x6c) + I(this, 0x84), I(this, 0x80), 0x12);
        I(this, 0x68) = 0;
        if (offer < 11 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            I(this, 0x68) = 3;
        } else if (offer == 1) {
            I(this, 0x68) = 1;
            ((TouchButton *)(button_at(this, 0)))->setPosition2(I(this, 0x84), I(this, 0x7c));
        } else {
            I(this, 0x68) = 2;
            ((TouchButton *)(button_at(this, 0)))->setPosition2(I(this, 0x84), I(this, 0x7c));
            ((TouchButton *)(button_at(this, 1)))->setPosition3(I(this, 0x6c) + I(this, 0x84), I(this, 0x7c), 0x12);
        }
    } else {
        I(this, 0x68) = 1;
        ((TouchButton *)(button_at(this, 0)))->setTextColor(-1);
        ((TouchButton *)(button_at(this, 0)))->setPosition2(I(this, 0x84), I(this, 0x7c));
    }

    for (unsigned i = 0; i < ((Array<TouchButton *> *)P(this, 0x5c))->size(); ++i) {
        ((TouchButton *)(button_at(this, i)))->setVisible(false);
    }

    int buttonHeight = I(layout, 0x2d8);
    if (I(this, 0x68) > 2) {
        int needed = I(layout, 0x30) * (I(this, 0x68) - 1) + I(layout, 0x34) * (I(this, 0x68) - 2);
        if (needed > buttonHeight) {
            buttonHeight = needed;
        }
    }
    int panelHeight = I(layout, 0x4c) * 2 + buttonHeight;
    s0 = "";
    ((Layout *)(layout))->drawBox6(2, I(this, 0x70), I(this, 0x78), I(layout, 0x68), panelHeight, &s0);
    ((PaintCanvas *)canvas)->DrawRectangle(I(this, 0x70), I(this, 0x78), I(layout, 0x68), panelHeight);
    ((ImageFactory *)(factory))->drawChar((Array<ImagePart *> *)P(this, 0x3c), I(layout, 0x4c) + I(this, 0x70), I(this, 0x78) + I(layout, 0x4c), true);

    ((TouchButton *)(button_at(this, 0)))->setVisible(true);
    if (!(offer == 1 || UC(this, 0x36) != 0 || I(this, 0x14) == 3)) {
        ((TouchButton *)(button_at(this, 1)))->setVisible(true);
        ((TouchButton *)(button_at(this, 0)))->draw();
        ((TouchButton *)(button_at(this, 1)))->draw();
        if (offer <= 10 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            ((TouchButton *)(button_at(this, 2)))->setVisible(true);
        }
    }
    ((TouchButton *)(button_at(this, 0)))->draw();
}

int SpaceLounge::init() {
    char matrix[60];

    UC(this, 0xb2) = 0;
    UC(this, 0xb0) = 0;
    UC(this, 0xbc) = 0;
    P(this, 0x24) = Station_getAgents((*g_status)->getStation());

    if (P(this, 0x8) != 0) {
        ::operator delete(ChoiceWindow_dtor(P(this, 0x8)));
        P(this, 0x8) = 0;
    }
    void *choice = ::operator new(0x5c);
    ChoiceWindow_ctor(choice);
    P(this, 0x8) = choice;
    UC(this, 0x35) = 0;

    if (P(this, 0x44) != 0) {
        ((CutScene *)(P(this, 0x44)))->resetCamera();
    }

    UC(this, 0x18) = 0;
    P(this, 0x58) = ::operator new[](0x15);
    for (int i = 0; i != 0x15; ++i) {
        ((char *)P(this, 0x58))[i] = 0;
    }

    void *layoutSlot = *(void **)&SpaceLounge_init_layout_slot;
    void *layout = *(void **)layoutSlot;
    int panelW = I(layout, 0x68);
    I(this, 0x70) = panelW / 2;
    I(this, 0x74) = I(layout, 0x20) + I(layout, 0xc);
    I(this, 0x6c) = panelW - I(layout, 0x4c) * 3 - I(layout, 0x2d4);

    void *scroll = ::operator new(0x24);
    ScrollTouchWindow_ctor(scroll, I(this, 0x70) + I(layout, 0x4c) * 2 + I(layout, 0x2d4),
                           I(this, 0x74), I(this, 0x6c), I(layout, 0x6c), false);
    P(this, 0x60) = scroll;

    I(this, 0x14) = 0;
    I(this, 0x78) = I(this, 0x74) + I(layout, 0x6c) + I(layout, 0x2c);
    I(this, 0x64) = (I(layout, 0x34) + I(layout, 0x30)) * 5;

    if (P(this, 0x5c) != 0) {
        Array<TouchButton *> *oldButtons = (Array<TouchButton *> *)P(this, 0x5c);
        oldButtons->clear();
        delete oldButtons;
    }
    Array<TouchButton *> *buttons = new Array<TouchButton *>();
    P(this, 0x5c) = buttons;
    buttons->resize(5);

    void *textsSlot = *(void **)&SpaceLounge_init_text_slot;
    void *text = ((GameText *)(*(void **)textsSlot))->getText(0);
    int baseY = I(this, 0x78) + I(layout, 0x4c);
    I(this, 0x84) = I(this, 0x70) + I(layout, 0x68) - I(layout, 0x4c) - I(this, 0x6c);
    I(this, 0x7c) = (baseY + I(layout, 0x2d8) / 2) - I(layout, 0x30) / 2;
    I(this, 0x80) = baseY;

    for (unsigned i = 0; i < 5; ++i) {
        TouchButton *button = (TouchButton *)::operator new(200);
        new (button) TouchButton((String *)text, 0, I(this, 0x84), baseY + (int)i * (I(layout, 0x30) + I(layout, 0x34)), I(this, 0x6c), 0x11, 4);
        (*buttons)[i] = button;
        button->setTextColor(-1);
    }

    I(this, 0x88) = -1;
    MatrixSetTranslation(matrix, 0.0f, 0.0f, 0.0f);
    MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
    void *cameraSlot = *(void **)&SpaceLounge_init_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
    ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)current);
    if (P(this, 0x48) != 0) {
        ((AbyssEngine::EaseInOutMatrix *)(P(this, 0x48)))->SetRange(*(AEMath::Matrix *)matrix, *(AEMath::Matrix *)matrix);
    }
    UC(this, 0xbd) = 1;
    I(this, 0x104) = 0;
    ((SpaceLounge *)(this))->updateScreenPositions();
    return 0;
}

SpaceLounge *_ZN11SpaceLoungeC2Ev(SpaceLounge *self)
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
    ((String *)B(self, 0xa4))->ctor();
    ((Matrix*)(B(self, 0xc8)))->initIdentity();

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

    Array<Agent *> *agents = (Array<Agent *> *)P(self, 0x24);
    if (agents != 0) {
        for (unsigned i = 0; i < agents->size(); ++i) {
            void *agent = (*agents)[i];
            int offer = ((Agent *)(agent))->getOffer();
            if ((offer == 6 || offer == 0) && ((Agent *)(agent))->getMission() != 0) {
                void *mission = ((Agent *)(agent))->getMission();
                if (((Mission *)(mission))->getType() == 0xc && ((Agent *)(agent))->hasAcceptedOffer() != 0) {
                    ArrayRemove_AgentPtr(agent, P(self, 0x24));
                    ((SpaceLounge *)(self))->init();
                    break;
                }
            }
            agents = (Array<Agent *> *)P(self, 0x24);
        }
    }

    void *cutscene = P(self, 0x44);
    if (cutscene == 0) {
        cutscene = ::operator new(0xa0);
        CutScene_ctor(cutscene, 4);
        P(self, 0x44) = cutscene;
    }
    while (((CutScene *)(cutscene))->isInitialized() == 0) {
        ((CutScene *)(cutscene))->initialize();
        cutscene = P(self, 0x44);
    }

    int race = ((SolarSystem *)((void *)(*g_status)->getSystem()))->getRace();
    MatrixSetTranslation(from, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(from, 0.0f, 0.0f, 0.0f);
    MatrixSetTranslation(to, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(to, 0.0f, 0.0f, 0.0f);
    void *ease = ::operator new(0xf4);
    EaseInOutMatrix_ctor(ease, from, to, 3000);
    P(self, 0x48) = ease;

    void *cameraSlot = *(void **)&SpaceLounge_ctor_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
    ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)current);
    UC(self, 0xb0) = 1;
    UC(self, 0xbd) = 0;
    return self;
}

void SpaceLounge::startChat() {
    String title;
    String body;
    String left;
    String right;

    if (P(this, 0x24) == 0 || I(this, 0x20) < 0) {
        return;
    }

    void *agent = (*((Array<Agent *> *)P(this, 0x24)))[I(this, 0x20)];
    int offer = ((Agent *)(agent))->getOffer();
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

    title.ctor_copy((String *)((GameText *)(*(void **)texts))->getText(titleId), false);
    body.ctor_copy((String *)((GameText *)(*(void **)texts))->getText(bodyId), false);
    left.ctor_copy((String *)((GameText *)(*(void **)texts))->getText(0x10), false);
    right.ctor_copy((String *)((GameText *)(*(void **)texts))->getText(0x11), false);

    ((ChoiceWindow *)(P(this, 0x8)))->setText(title, body);
    ((ChoiceWindow *)(P(this, 0x8)))->setButtonText(left, right);

    if (((Agent *)(agent))->isKnown() == 0 && ((Agent *)(agent))->isStoryAgent() == 0) {
        ((Agent *)(agent))->setKnown(true);
    }
    SpaceLounge_getSoundId(this, agent);

    UC(this, 0x1b) = 1;
    UC(this, 0x19) = 1;
    I(this, 0x14) = 1;

    if (offer == 1) {
        I(this, 0x14) = 3;
    } else if (((Agent *)(agent))->isGenericAgent() != 0) {
        ((Agent *)(agent))->setEvent(1);
    }
}

// SpaceLounge::~SpaceLounge() — tears down the owned ChoiceWindow / CutScene, the
// agent-text and button arrays, the per-system ImagePart grid and silhouette-vector
// array, the camera-ease matrix, and the cached canvas buffer + title String.
SpaceLounge::~SpaceLounge()
{
    void *p = P(this, 0x8);
    if (p != 0) {
        ::operator delete(ChoiceWindow_dtor(p));
    }
    P(this, 0x8) = 0;

    p = P(this, 0x44);
    if (p != 0) {
        ::operator delete(CutScene_dtor(p));
    }
    P(this, 0x44) = 0;

    ::operator delete[](P(this, 0x58));
    P(this, 0x58) = 0;

    {
        Array<String *> *strings = (Array<String *> *)P(this, 0x28);
        if (strings != 0) {
            strings->clear();
            delete strings;
        }
    }
    P(this, 0x28) = 0;

    {
        Array<TouchButton *> *buttons = (Array<TouchButton *> *)P(this, 0x5c);
        if (buttons != 0) {
            buttons->clear();
            delete buttons;
        }
    }
    P(this, 0x5c) = 0;

    if (I(this, 0x3c) != 0) {
        return;
    }
    I(this, 0x3c) = 0;

    {
        Array<Array<ImagePart *> *> *grid = (Array<Array<ImagePart *> *> *)P(this, 0x38);
        if (grid != 0) {
            for (unsigned i = 0; i < grid->size(); ++i) {
                Array<ImagePart *> *inner = (*grid)[i];
                if (inner != 0) {
                    inner->clear();
                    delete inner;
                }
                (*grid)[i] = 0;
            }
            grid->clear();
            delete grid;
        }
    }
    P(this, 0x38) = 0;

    {
        Array<Vector *> *rects = (Array<Vector *> *)P(this, 0x40);
        if (rects != 0) {
            rects->clear();
            delete rects;
        }
    }
    P(this, 0x40) = 0;

    p = P(this, 0x48);
    if (p != 0) {
        ::operator delete(EaseInOutMatrix_dtor(p));
    }
    P(this, 0x48) = 0;

    p = P(this, 0xc0);
    if (p != 0) {
        ::operator delete(p);
    }
    P(this, 0xc0) = 0;
    ((String *)(B(this, 0xa4)))->dtor();
}

void SpaceLounge::draw() {
    String title;

    if (UC(this, 0x1c) != 0) {
        if (((ListItemWindow *)(P(this, 0xc)))->shows3DShip() != 0) {
            void *canvasSlot = *(void **)&SpaceLounge_draw_canvas_slot;
            void *canvas = *(void **)canvasSlot;
            ((PaintCanvas *)canvas)->SetColor((unsigned char)(long)canvas, 0, 0, 0);
            int width = pc_GetWidth((PaintCanvas *)canvas);
            int height = pc_GetHeight((PaintCanvas *)canvas);
            (void)height;
            ((PaintCanvas *)canvas)->FillRectangle((int)(long)canvas, 0, 0, width);
            ((PaintCanvas *)canvas)->SetColor((unsigned int)(long)canvas);
        }
        ((ListItemWindow *)(P(this, 0xc)))->draw();
        return this->draw_cutscene_tail();
    }

    if (UC(this, 0x34) != 0) {
        return this->draw_map_tail(P(this, 0x4));
    }

    void *layoutSlot = *(void **)&SpaceLounge_draw_layout_slot;
    void *layout = *(void **)layoutSlot;
    void *textsSlot = *(void **)&SpaceLounge_draw_text_slot;
    void *text = ((GameText *)(*(void **)textsSlot))->getText(0x18e);
    title.ctor_copy((String *)text, false);
    ((Layout *)(layout))->drawHeader_call(&title);

    ((SpaceLounge *)(this))->drawLounge();

    layout = *(void **)layoutSlot;
    if ((I(this, 0x14) & 0xfffffffe) == 2) {
        ((Layout *)layout)->drawFooterNoBackButton();
    } else {
        ((Layout *)layout)->drawFooter();
    }

    if (UC(this, 0x19) != 0 || UC(this, 0x1a) != 0 || UC(this, 0x1b) != 0) {
        ((ChoiceWindow *)(P(this, 0x8)))->draw();
    }
}

void SpaceLounge::update(int dt) {
    char maxMatrix[60];
    char valueMatrix[60];

    if (dt > 50) {
        dt = 50;
    }
    if (UC(this, 0xb0) == 0) {
        return;
    }

    if (UC(this, 0x34) != 0) {
        return this->update_map_tail(P(this, 0x4), dt);
    }
    if (UC(this, 0x1c) != 0) {
        return this->update_ship_tail(P(this, 0xc), dt);
    }

    if (UC(this, 0xbc) == 0) {
        ((AbyssEngine::EaseInOutMatrix *)(P(this, 0x48)))->Increase((float)dt);
    }

    void *cameraSlot;
    void *camera;
    void *current;
    if (UC(this, 0xbd) == 0) {
        ((AbyssEngine::EaseInOutMatrix *)(maxMatrix))->GetMaxValue();
        ((AbyssEngine::EaseInOutMatrix *)(valueMatrix))->GetValue();
        if ((memcmp((maxMatrix),(valueMatrix),sizeof(float)*16)==0) == 0) {
            UC(this, 0xbc) = 0;
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_c;
            camera = *(void **)cameraSlot;
            current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
            ((AbyssEngine::EaseInOutMatrix *)(valueMatrix))->GetValue();
            ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)valueMatrix);
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
        UC(this, 0xbd) = 1;
        F(this, 0x104) = F(this, 0x104) + step;
        float wave = Sinf(F(this, 0x104));
        float maxRot = (float)I(this, 0x108) * 0.5f;

        if (UC(this, 0xbc) == 0) {
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_a;
            camera = *(void **)cameraSlot;
            current = ((PaintCanvas *)camera)->CameraGetLocal((unsigned int)(long)camera);
            (void)current;
            UC(this, 0xbc) = 1;
            int amount = AbyssEngine::AERandom::nextInt(*(void **)&SpaceLounge_update_random_slot, 10);
            UC(this, 0xc4) = 0;
            if (P(this, 0xc0) == 0) {
                void *ease = ::operator new(0x10);
                EaseInOut_ctor(ease, 0.0f, (float)amount);
                P(this, 0xc0) = ease;
            } else {
                ((AbyssEngine::EaseInOut *)(P(this, 0xc0)))->SetRange(0.0f, (float)amount);
            }
            I(this, 0x108) = 2;
        } else {
            float value = EaseInOut_GetValue(P(this, 0xc0));
            float maxValue = ((AbyssEngine::EaseInOut *)(P(this, 0xc0)))->GetMaxValue();
            float distance = value - maxValue;
            if (distance < 0.0f) {
                distance = -distance;
            }
            int amount = I(this, 0x108);
            if (distance < 0.25f) {
                void *random = *(void **)&SpaceLounge_update_random_slot;
                amount = AbyssEngine::AERandom::nextInt(random, 10);
                float next = (float)(5 - amount);
                UC(this, 0xc4) = value > next;
                ((AbyssEngine::EaseInOut *)(P(this, 0xc0)))->SetRange(value, next);
                I(this, 0x108) = AbyssEngine::AERandom::nextInt(random, 4) + 1;
                amount = I(this, 0x108);
            }
            if (UC(this, 0xc4) != 0) {
                amount = -amount;
            }
            ((AbyssEngine::EaseInOut *)(P(this, 0xc0)))->Increase((float)(dt * amount));
        }

        float value = EaseInOut_GetValue(P(this, 0xc0));
        MatrixSetRotation(valueMatrix, value / 90.0f, 0.0f, value);
        MatrixSetTranslation(maxMatrix, 0.0f, 0.0f, wave * maxRot);
        AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)(valueMatrix),*(const Matrix*)(B(this, 0xc8)));
        cameraSlot = *(void **)&SpaceLounge_update_camera_slot_b;
        camera = *(void **)cameraSlot;
        current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
        ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)valueMatrix);
    }

    ((SpaceLounge *)(this))->updateScreenPositions();
    if (I(this, 0x14) != 0) {
        ((ScrollTouchWindow *)(P(this, 0x60)))->update(dt);
    }
    ((CutScene *)(P(this, 0x44)))->update();

    if (UC(this, 0xb2) != 0) {
        int x = I(this, 0xb4);
        int y = I(this, 0xb8);
        int top = *(int *)SpaceLounge_update_top_slot;
        int height = *(int *)SpaceLounge_update_height_slot;
        int width = *(int *)SpaceLounge_update_width_slot;
        void *cutscene = P(this, 0x44);
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

// ---------------------------------------------------------------------------
// Tail-call fragments (resolved inherited / sub-screen methods)
//
// The decompiler split SpaceLounge::OnRender3D / OnRenderBG / draw3DShip / draw /
// update at their mode branches: each branch ended in a tail-call through a long-
// branch veneer into the appropriate sub-screen object's method. Following the
// veneers to their final entry points resolves them as:
//   OnRender3D_map_tail      -> StarMap::render()        (_ZN7StarMap6renderEv,   veneer 0x19c258 -> 0x6567c)
//   OnRender3D_cutscene_tail -> CutScene::render3D()     (_ZN8CutScene8render3DEv, veneer 0x19c6a8 -> 0x65694)
//   OnRenderBG_tail          -> CutScene::renderBG()     (_ZN8CutScene8renderBGEv, veneer 0x19c6b8 -> 0x656a0)
//   draw3DShip_tail          -> ListItemWindow::render() (_ZN14ListItemWindow6renderEv, veneer 0x19c4b8 -> 0x66fd8)
//   draw_map_tail            -> StarMap::draw()          (_ZN7StarMap4drawEv,     veneer 0x19c268 -> 0x655bc)
//   draw_cutscene_tail       -> Layout::drawFooter()     (_ZN6Layout10drawFooterEv, veneer 0x19c698 -> 0x65b8c, draw-layout slot)
//   update_map_tail          -> StarMap::update(dt)      (_ZN7StarMap6updateEi,   veneer 0x19c278 -> 0x654c0)
//   update_ship_tail         -> ListItemWindow::update(dt) (_ZN14ListItemWindow6updateEi, veneer 0x19c4a8 -> 0x66f54)
// These are now real SpaceLounge member methods that forward to the live sub-screen;
// the receiver is supplied by the caller (or, for draw_cutscene_tail, taken from the
// draw-layout global slot). No extern "C" veneers remain.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Recovered tail-call fragments as real SpaceLounge members. Each mirrors the
// corresponding extern "C" veneer below; the lounge forwards rendering/updating
// to whichever sub-screen (StarMap / CutScene / ListItemWindow / Layout) is live
// for the current mode. The receiver is supplied by the caller, except
// draw_cutscene_tail which reads the active draw-layout from the global slot.
// ---------------------------------------------------------------------------

void SpaceLounge::OnRender3D_map_tail(void *map)
{
    ((StarMap *)map)->render();
}

void SpaceLounge::OnRender3D_cutscene_tail(void *cutscene)
{
    ((CutScene *)cutscene)->render3D();
}

void SpaceLounge::OnRenderBG_tail(void *cutscene)
{
    // OnRenderBG: when a cutscene is active (field 0x44 != 0) the lounge draws the
    // cutscene's background layer. Sibling of OnRender3D_cutscene_tail.
    ((CutScene *)cutscene)->renderBG();
}

void SpaceLounge::draw3DShip_tail(void *ship)
{
    ((ListItemWindow *)ship)->render();
}

void SpaceLounge::draw_map_tail(void *map)
{
    ((StarMap *)map)->draw();
}

void SpaceLounge::draw_cutscene_tail()
{
    // Tail of draw() in 3D-ship mode: after the ListItemWindow has been drawn,
    // render the footer of the currently active draw layout.
    void *layout = *(void **)(*(void **)&SpaceLounge_draw_layout_slot);
    ((Layout *)layout)->drawFooter();
}

void SpaceLounge::update_map_tail(void *map, int dt)
{
    ((StarMap *)map)->update(dt);
}

void SpaceLounge::update_ship_tail(void *list, int dt)
{
    ((ListItemWindow *)list)->update(dt);
}

void *SpaceLounge::dtor()
{
    this->~SpaceLounge();
    return this;
}

// SpaceLounge::refresh -- called from ModStation after a mission reward is credited
// and the mission removed, to refresh the lounge's offer list. In this build the body
// is empty: the agent/offer list is rebuilt on the next init()/re-entry of the lounge,
// so refresh() has no work to do here (the binary's SpaceLounge::refresh is a no-op stub).
void SpaceLounge::refresh()
{
}
