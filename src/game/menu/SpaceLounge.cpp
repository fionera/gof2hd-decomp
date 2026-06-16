#include "platform/libc.h"
#include "game/menu/SpaceLounge.h"
#include "game/world/SolarSystem.h"
#include "engine/render/AEGeometry.h"
#include "game/ui/ChoiceWindow.h"
#include "game/core/CutScene.h"
#include "engine/math/EaseInOut.h"
#include "engine/math/EaseInOutMatrix.h"
#include "game/world/Level.h"
#include "game/ship/KIPlayer.h"
#include "game/ui/ListItemWindow.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/world/StarMap.h"
#include "game/ship/Agent.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
#include "game/mission/Mission.h"
#include "game/mission/Status.h"
#include "game/ui/TouchButton.h"

// The engine paint canvas is reached here only through a small set of camera/draw
// thunks (whose mangled `PaintCanvas::` symbols are defined in PaintCanvas.cpp). We
// declare just those methods and cast the engine canvas pointer to them; the
// int-returning width/height impls are exposed as pc_GetWidth/pc_GetHeight.
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

// A handful of Layout sub-window metrics live at offsets the shared Layout header does
// not yet model (the lounge content-panel width/height at +0x68/+0x6c, the hover-box
// padding at +0x94 and the name baseline offset at +0x2c0). Read them in place until
// Layout grows the corresponding named members.
static inline int &layoutMetric(Layout *layout, int off)
{
    return *(int *)((char *)layout + off);
}

namespace AbyssEngine { namespace AEMath {
    Vector operator+(const Vector&, const Vector&);
    Vector operator-(const Vector&, const Vector&);
    Vector operator*(const Vector&, float);
    void MatrixMultiply(Matrix&, const Matrix&);
} }

extern "C" void *SpaceLounge_layout_move;
extern "C" void *SpaceLounge_layout_begin;
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
extern "C" void *SpaceLounge_init_layout_slot;
extern "C" void *SpaceLounge_init_text_slot;
extern "C" void *SpaceLounge_init_camera_slot;
extern "C" void ArrayRemove_AgentPtr(void *agent, void *array);
extern "C" void *SpaceLounge_ctor_camera_slot;
int SpaceLounge_getSoundId(SpaceLounge *self, void *agent);
extern "C" void *SpaceLounge_start_text_slot;
extern "C" void *SpaceLounge_draw_layout_slot;
extern "C" void *SpaceLounge_draw_canvas_slot;
extern "C" void *SpaceLounge_draw_text_slot;
float Sinf(float value);
extern "C" void *SpaceLounge_update_camera_slot_a;
extern "C" void *SpaceLounge_update_camera_slot_b;
extern "C" void *SpaceLounge_update_camera_slot_c;
extern "C" void *SpaceLounge_update_random_slot;
extern "C" int *SpaceLounge_update_width_slot;
extern "C" int *SpaceLounge_update_height_slot;
extern "C" int *SpaceLounge_update_top_slot;

int SpaceLounge::OnTouchMove(int x, int y) {
    this->touchX = x;
    this->touchY = y;

    if (this->choiceVisible != 0 || this->chatActive != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return 0;
    }

    if (this->mapVisible != 0) {
        this->starMap->OnTouchMove(x, y);
        return 0;
    }

    int mode = this->mode;
    if (mode == 0) {
        Array<Agent *> *agents = this->agents;
        if (agents != 0) {
            Array<Vector *> *rects = this->agentRects;
            float fy = (float)y;
            float fx = (float)x;
            this->hoverAgent = -1;
            this->selectedAgent = -1;
            int count = (int)agents->size();
            for (int i = 0; count != i; ++i) {
                float *a = (float *)(*rects)[i * 2];
                if (a[0] < fx) {
                    float *b = (float *)(*rects)[i * 2 + 1];
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        this->hoverAgent = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        Array<TouchButton *> *buttons = this->buttons;
        (*buttons)[0]->OnTouchMove(x, y);
    } else if (mode == 2) {
        Array<TouchButton *> *buttons = this->buttons;
        for (unsigned i = 0; i < buttons->size(); ++i) {
            (*buttons)[i]->OnTouchMove(x, y);
        }
    }

    ((Layout *)(*(void **)SpaceLounge_layout_move))->OnTouchMove(x, y);
    if (this->listVisible != 0) {
        this->listWindow->OnTouchMove(x, y);
    } else {
        this->scrollWindow->OnTouchMove(x, y);
    }
    return 0;
}

void SpaceLounge::OnRender3D() {
    if (this->mapVisible != 0) {
        return this->OnRender3D_map_tail(this->starMap);
    }

    void *cutscene = this->cutScene;
    if (cutscene == 0) {
        return;
    }

    if (this->listVisible != 0) {
        if (this->listWindow->shows3DShip() != 0) {
            return;
        }
        cutscene = this->cutScene;
    }

    return this->OnRender3D_cutscene_tail(cutscene);
}

void SpaceLounge::OnRenderBG() {
    if (this->cutScene != 0) {
        return this->OnRenderBG_tail(this->cutScene);
    }
}

unsigned char SpaceLounge::introFinished() {
    return this->introDone;
}

bool SpaceLounge::listMode() {
    return this->mapVisible == 0;
}

unsigned char SpaceLounge::mapMode() {
    return this->mapVisible;
}

unsigned char SpaceLounge::hangarNeedsUpdate() {
    return this->hangarUpdate;
}

void SpaceLounge::draw3DShip() {
    if (this->listVisible != 0) {
        return this->draw3DShip_tail(this->listWindow);
    }
}

void SpaceLounge::setHangarUpdate(bool value) {
    this->hangarUpdate = value;
}

int SpaceLounge::OnTouchBegin(int x, int y) {
    this->touchDown = 1;
    this->touchX = x;
    this->touchY = y;
    this->hoverAgent = -1;

    if (this->choiceVisible != 0 || this->chatActive != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
        return 0;
    }

    if (this->mapVisible != 0) {
        this->starMap->OnTouchBegin(x, y);
        return 0;
    }

    if (this->listVisible != 0) {
        this->listWindow->OnTouchBegin(x, y);
        ((Layout *)(*(void **)SpaceLounge_layout_begin))->OnTouchBegin(x, y);
        this->scrollWindow->OnTouchBegin(x, y);
        return 0;
    }

    int mode = this->mode;
    if (mode == 0) {
        if (this->introDone == 0) {
            return 0;
        }
        Array<Agent *> *agents = this->agents;
        if (agents != 0) {
            Array<Vector *> *rects = this->agentRects;
            float fy = (float)y;
            float fx = (float)x;
            int count = (int)agents->size();
            for (int i = 0; count != i; ++i) {
                float *a = (float *)(*rects)[i * 2];
                if (a[0] < fx) {
                    float *b = (float *)(*rects)[i * 2 + 1];
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        this->hoverAgent = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        Array<TouchButton *> *buttons = this->buttons;
        (*buttons)[0]->OnTouchBegin(x, y);
    } else if (mode == 2) {
        Array<TouchButton *> *buttons = this->buttons;
        for (unsigned i = 0; i < buttons->size(); ++i) {
            (*buttons)[i]->OnTouchBegin(x, y);
        }
    }

    ((Layout *)(*(void **)SpaceLounge_layout_begin))->OnTouchBegin(x, y);
    this->scrollWindow->OnTouchBegin(x, y);
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
    Array<Agent *> *agents = self->agents;
    return (*agents)[self->selectedAgent];
}

static inline bool hit_agent(SpaceLounge *self, int x, int y, int i)
{
    Array<Vector *> *rects = self->agentRects;
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

    this->touchDown = 0;

    if (this->choiceVisible != 0 || this->chatActive != 0) {
        int result = this->choiceWindow->touch_end(x, y);
        if (result == 1) {
            this->chatActive = 0;
        } else if (result == 0) {
            ((SpaceLounge *)(this))->onKeyPress(0x10000);
        }
        return;
    }

    if (this->mapVisible != 0) {
        if (this->starMap->touch_end(x, y) != 0) {
            this->cutScene->resetCamera();
            this->mapVisible = 0;
        }
        return;
    }

    void *layoutSlot = *(void **)&SpaceLounge_touch_layout_slot;
    void *layout = *(void **)layoutSlot;
    if (((Layout *)(layout))->touch_end(x, y) != 0) {
        if (this->listVisible != 0) {
            ((Layout *)(layout))->resetWindowDimensions();
            this->listVisible = 0;
        } else if (this->mode != 0) {
            if (this->selectedAgent >= 0) {
                void *agent = selected_agent(this);
                if (((Agent *)(agent))->isGenericAgent() != 0) {
                    ((Agent *)(agent))->setEvent(1);
                }
            }
            this->mode = 0;
            this->field_0x36 = 0;
        }
        return;
    }

    if (this->listVisible != 0) {
        this->listWindow->touch_end(x, y);
        if (((Layout *)(layout))->helpPressed() != 0) {
            void *texts = *(void **)&SpaceLounge_touch_list_help_text_slot;
            void *text = ((GameText *)(*(void **)texts))->getText(0x283);
            helpSmall.ctor_copy((String *)text, false);
            ((Layout *)(layout))->initHelpWindow(&helpSmall);
        }
        return;
    }

    switch (this->mode) {
    case 0:
        if (this->introDone == 0) {
            void *system = (void *)(long)gStatus->getSystem();
            int race = ((SolarSystem *)(system))->getRace();
            int *v = &SpaceLounge_touch_race_vectors[race * 3];
            MatrixSetTranslation(matrix, (float)v[2], (float)v[0], (float)v[1]);
            MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
            void *cameraSlot = *(void **)&SpaceLounge_touch_camera_slot;
            void *camera = *(void **)cameraSlot;
            void *current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
            ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)current);
            if (this->cameraEase != 0) {
                this->cameraEase->SetRange(*(AEMath::Matrix *)matrix, *(AEMath::Matrix *)matrix);
            }
            this->introDone = 1;
            this->field_0x104 = 0;
            return;
        }
        if (this->agents != 0) {
            this->hoverAgent = -1;
            this->selectedAgent = -1;
            unsigned count = (unsigned)this->agents->size();
            for (unsigned i = 0; i < count; ++i) {
                if (hit_agent(this, x, y, i)) {
                    this->selectedAgent = i;
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
        Array<TouchButton *> *buttons = this->buttons;
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
        if ((*this->buttons)[0]->touch_end(x, y) != 0) {
            ((SpaceLounge *)(this))->onKeyPress(0x10000);
        }
        break;
    }

    this->scrollWindow->touch_end(x, y);
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
        int *parts = ((Agent *)(agent))->getImageParts();
        if (parts == 0) {
            race = 3;
            dummy = 0;
        } else {
            parts = ((Agent *)(agent))->getImageParts();
            dummy = parts[0];
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
    return (*self->agents)[self->selectedAgent];
}

void SpaceLounge::onKeyPress(int key) {
    char scratch[960];
    (void)scratch;

    if (this->choiceVisible != 0) {
        if (key == 0x1000) {
            this->choiceWindow->left();
        } else if (key == 0x2000) {
            this->choiceWindow->right();
        } else if (key == 0x10000) {
            this->choiceVisible = 0;
        }
        return;
    }

    int mode = this->mode;
    if (mode == 0) {
        if (key == 0x1000 || key == 0x8000) {
            unsigned next = this->selectedAgent + 1;
            if (this->agents != 0 && next >= this->agents->size()) {
                next = 0;
            }
            this->selectedAgent = next;
        } else if (key == 0x2000 || key == 0x4000) {
            int current = this->selectedAgent;
            if (current < 1 && this->agents != 0) {
                current = (int)this->agents->size();
            }
            this->selectedAgent = current - 1;
        } else if (key == 0x10000 || key == 0x20000) {
            ((SpaceLounge *)(this))->startChat();
        }
        return;
    }

    if (mode == 1 || mode == 3) {
        if (key == 0x10000 || key == 0x20000) {
            this->field_0x30 = 0;
            void *agent = key_agent(this);
            if (((Agent *)(agent))->getOffer() == 1) {
                this->mode = 2;
            } else {
                this->mode = mode == 1 ? 2 : 0;
            }
        } else if (key == 0x40000) {
            if (this->field_0x2c < 3) {
                if (mode == 1) {
                    this->field_0x2c = 0;
                    this->mode = 0;
                }
            } else {
                this->field_0x2c -= 0x28;
            }
        }
        return;
    }

    if (mode == 2) {
        void *agent = key_agent(this);
        if (key == 0x40000) {
            this->mode = 1;
        } else if (key == 0x10000 || key == 0x20000) {
            if (((Agent *)(agent))->getMission() == 0 && ((Agent *)(agent))->isGenericAgent() != 0) {
                ((Agent *)(agent))->setEvent(1);
            }
            this->mode = 0;
        } else if (key == 0x8000) {
            this->scrollWindow->scroll(1);
        } else if (key == 0x4000) {
            this->scrollWindow->scroll(-1);
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

    if (this->agents == 0) {
        return;
    }

    void *level = *(void **)&SpaceLounge_screen_level_slot;
    Array<KIPlayer *> *enemies = ((Level *)(level))->getEnemies();
    void *canvasSlot = *(void **)&SpaceLounge_screen_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *project = *(void **)&SpaceLounge_screen_projector;

    void *current = (void *)(long)((PaintCanvas *)canvas)->CameraGetCurrent();
    void *local = ((PaintCanvas *)canvas)->CameraGetLocal((unsigned int)(long)canvas);
    MatrixGetRight(pos, local);
    *(Vector*)(halfRight) = *(const Vector*)(pos) * (0.5f);

    unsigned count = (unsigned)this->agents->size();
    for (unsigned i = 0; i < count; ++i) {
        KIPlayer *enemy = (*enemies)[i];
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

        KIPlayer *mapped = (*enemies)[count + i];
        mapped->parentGeometry->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(camera));

        ((void (*)(void *, void *))project)(&this->silhouettePos, target);
        MatrixGetDir(pos, look);
        this->silhouettePos.x -= (*(float *)pos) * 20.0f;
        MatrixGetDir(pos, look);
        this->silhouettePos.z -= (*(float *)(pos + 8)) * 20.0f;

        ((void (*)(void *, void *))(*(void ***)mapped)[0x44 / 4])(mapped, &this->silhouettePos);

        if (((SolarSystem *)((void *)(long)gStatus->getSystem()))->getRace() == 0) {
            MatrixSetRotation(look, 0.0f, 0.0f, 0.0f);
            AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)(camera),*(const Matrix*)(look));
        }

        enemy->parentGeometry->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(camera));
        ((void (*)(void *, void *))vt[0x44 / 4])(enemy, target);
    }
}

static inline void *button_at(SpaceLounge *self, unsigned i)
{
    return (*self->buttons)[i];
}

void SpaceLounge::drawLounge() {
    String s0, s1, s2, s3, s4, s5, s6;

    void *canvasSlot = *(void **)&SpaceLounge_lounge_canvas_slot;
    void *canvas = *(void **)canvasSlot;
    void *layoutSlot = *(void **)&SpaceLounge_lounge_layout_slot;
    Layout *layout = (Layout *)*(void **)layoutSlot;
    void *factorySlot = *(void **)&SpaceLounge_lounge_image_factory_slot;
    void *factory = *(void **)factorySlot;

    if (this->mode == 0) {
        int hover = this->hoverAgent;
        if (hover >= 0) {
            void *agent = (*this->agents)[hover];
            Array<Vector *> *rects = this->agentRects;
            float *left = (float *)(*rects)[hover * 2];
            float *right = (float *)(*rects)[hover * 2 + 1];
            int pad = layoutMetric(layout, 0x94);
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
            layout->drawBox6(2, boxX, boxY, width, layout->field_0x30, &s2);
            ((PaintCanvas *)canvas)->DrawRectangle(boxX, boxY, width, layout->field_0x30);

            s3 = ((Agent *)(agent))->isKnown() == 0 ? "?" : "";
            s4 = s3 + s0;
            ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, &s4, cx, y + layoutMetric(layout, 0x2c0), false);

            if (((Agent *)(agent))->isKnown() != 0) {
                s5 = " ";
                s6 = s5 + s0;
                int nameWidth = ((PaintCanvas *)canvas)->GetTextWidth((unsigned int)(long)canvas, font);
                ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, &s1, cx + nameWidth, y + layoutMetric(layout, 0x2c0), false);
            }
        }
        return;
    }

    ((PaintCanvas *)canvas)->SetColor((unsigned int)(long)canvas);
    s0 = "";
    layout->drawBox6(2, this->panelX, this->panelY, layoutMetric(layout, 0x68), layoutMetric(layout, 0x6c), &s0);
    ((PaintCanvas *)canvas)->DrawRectangle(this->panelX, this->panelY, layoutMetric(layout, 0x68), layoutMetric(layout, 0x6c));
    ((ImageFactory *)(factory))->drawChar((*this->silhouetteGrid)[this->selectedAgent], layout->field_0x4c + this->panelX, layout->field_0x4c + this->panelY, false);
    this->scrollWindow->draw();

    if ((this->mode & 0xfffffffe) != 2) {
        return;
    }

    ((TouchButton *)(button_at(this, 0)))->setTextColor(-1);
    int offer = ((Agent *)((*this->agents)[this->selectedAgent]))->getOffer();
    if (this->mode == 2) {
        ((TouchButton *)(button_at(this, 0)))->setPosition2(this->buttonX, this->buttonY0);
        ((TouchButton *)(button_at(this, 1)))->setPosition3(this->panelWidth + this->buttonX, this->buttonY0, 0x12);
        this->field_0x68 = 0;
        if (offer < 11 && ((1 << (offer & 0xff)) & 0x60c) != 0) {
            this->field_0x68 = 3;
        } else if (offer == 1) {
            this->field_0x68 = 1;
            ((TouchButton *)(button_at(this, 0)))->setPosition2(this->buttonX, this->buttonY1);
        } else {
            this->field_0x68 = 2;
            ((TouchButton *)(button_at(this, 0)))->setPosition2(this->buttonX, this->buttonY1);
            ((TouchButton *)(button_at(this, 1)))->setPosition3(this->panelWidth + this->buttonX, this->buttonY1, 0x12);
        }
    } else {
        this->field_0x68 = 1;
        ((TouchButton *)(button_at(this, 0)))->setTextColor(-1);
        ((TouchButton *)(button_at(this, 0)))->setPosition2(this->buttonX, this->buttonY1);
    }

    for (unsigned i = 0; i < this->buttons->size(); ++i) {
        ((TouchButton *)(button_at(this, i)))->setVisible(false);
    }

    int buttonHeight = layout->field_0x2d8;
    if (this->field_0x68 > 2) {
        int needed = layout->field_0x30 * (this->field_0x68 - 1) + layout->field_0x34 * (this->field_0x68 - 2);
        if (needed > buttonHeight) {
            buttonHeight = needed;
        }
    }
    int panelHeight = layout->field_0x4c * 2 + buttonHeight;
    s0 = "";
    layout->drawBox6(2, this->panelX, this->buttonPanelY, layoutMetric(layout, 0x68), panelHeight, &s0);
    ((PaintCanvas *)canvas)->DrawRectangle(this->panelX, this->buttonPanelY, layoutMetric(layout, 0x68), panelHeight);
    ((ImageFactory *)(factory))->drawChar(this->agentImageParts, layout->field_0x4c + this->panelX, this->buttonPanelY + layout->field_0x4c, true);

    ((TouchButton *)(button_at(this, 0)))->setVisible(true);
    if (!(offer == 1 || this->field_0x36 != 0 || this->mode == 3)) {
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

    this->touchDown = 0;
    this->initialized = 0;
    this->field_0xbc = 0;
    this->agents = (Array<Agent *> *)Station_getAgents(gStatus->getStation());

    if (this->choiceWindow != 0) {
        delete this->choiceWindow;
        this->choiceWindow = 0;
    }
    this->choiceWindow = new ChoiceWindow();
    this->hangarUpdate = 0;

    if (this->cutScene != 0) {
        this->cutScene->resetCamera();
    }

    this->field_0x18 = 0;
    this->agentVisited = (char *)::operator new[](0x15);
    for (int i = 0; i != 0x15; ++i) {
        this->agentVisited[i] = 0;
    }

    void *layoutSlot = *(void **)&SpaceLounge_init_layout_slot;
    Layout *layout = (Layout *)*(void **)layoutSlot;
    int panelW = layoutMetric(layout, 0x68);
    this->panelX = panelW / 2;
    this->panelY = layout->field_0x20 + layout->field_0xc;
    this->panelWidth = panelW - layout->field_0x4c * 3 - layout->field_0x2d4;

    this->scrollWindow = new ScrollTouchWindow(
        this->panelX + layout->field_0x4c * 2 + layout->field_0x2d4,
        this->panelY, this->panelWidth, layoutMetric(layout, 0x6c), false);

    this->mode = 0;
    this->buttonPanelY = this->panelY + layoutMetric(layout, 0x6c) + layout->field_0x2c;
    this->field_0x64 = (layout->field_0x34 + layout->field_0x30) * 5;

    if (this->buttons != 0) {
        Array<TouchButton *> *oldButtons = this->buttons;
        oldButtons->clear();
        delete oldButtons;
    }
    Array<TouchButton *> *buttons = new Array<TouchButton *>();
    this->buttons = buttons;
    buttons->resize(5);

    void *textsSlot = *(void **)&SpaceLounge_init_text_slot;
    void *text = ((GameText *)(*(void **)textsSlot))->getText(0);
    int baseY = this->buttonPanelY + layout->field_0x4c;
    this->buttonX = this->panelX + layoutMetric(layout, 0x68) - layout->field_0x4c - this->panelWidth;
    this->buttonY1 = (baseY + layout->field_0x2d8 / 2) - layout->field_0x30 / 2;
    this->buttonY0 = baseY;

    for (unsigned i = 0; i < 5; ++i) {
        TouchButton *button = new TouchButton((String *)text, 0, this->buttonX,
            baseY + (int)i * (layout->field_0x30 + layout->field_0x34), this->panelWidth, 0x11, 4);
        (*buttons)[i] = button;
        button->setTextColor(-1);
    }

    this->hoverAgent = -1;
    MatrixSetTranslation(matrix, 0.0f, 0.0f, 0.0f);
    MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
    void *cameraSlot = *(void **)&SpaceLounge_init_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
    ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)current);
    if (this->cameraEase != 0) {
        this->cameraEase->SetRange(*(AEMath::Matrix *)matrix, *(AEMath::Matrix *)matrix);
    }
    this->introDone = 1;
    this->field_0x104 = 0;
    ((SpaceLounge *)(this))->updateScreenPositions();
    return 0;
}

SpaceLounge::SpaceLounge()
{
    char from[60];
    char to[60];

    this->field_0x8c = 0;
    this->field_0x90 = 0;
    this->field_0x94 = 0;
    this->field_0x98 = 0;
    this->silhouettePos.x = 0;
    this->silhouettePos.y = 0;
    this->silhouettePos.z = 0;
    this->field_0x9c = 0;
    this->field_0xa0 = 0;
    this->baseMatrix.initIdentity();

    this->initialized = 0;
    this->starMap = 0;
    this->choiceWindow = 0;
    this->listWindow = 0;
    this->mapVisible = 0;
    this->field_0x36 = 0;
    this->agentVisited = 0;
    this->buttons = 0;
    this->headEase = 0;
    this->listVisible = 0;
    this->mode = 0;
    this->field_0x18 = 0;
    this->cameraEase = 0;
    this->selectedAgent = 0;
    this->agents = 0;
    this->agentTexts = 0;
    this->field_0x2c = 0;
    this->silhouetteGrid = 0;
    this->agentImageParts = 0;
    this->agentRects = 0;
    this->cutScene = 0;

    this->init();

    Array<Agent *> *agents = this->agents;
    if (agents != 0) {
        for (unsigned i = 0; i < agents->size(); ++i) {
            void *agent = (*agents)[i];
            int offer = ((Agent *)(agent))->getOffer();
            if ((offer == 6 || offer == 0) && ((Agent *)(agent))->getMission() != 0) {
                void *mission = ((Agent *)(agent))->getMission();
                if (((Mission *)(mission))->getType() == 0xc && ((Agent *)(agent))->hasAcceptedOffer() != 0) {
                    ArrayRemove_AgentPtr(agent, this->agents);
                    this->init();
                    break;
                }
            }
            agents = this->agents;
        }
    }

    if (this->cutScene == 0) {
        this->cutScene = new CutScene(4);
    }
    while (this->cutScene->isInitialized() == 0) {
        this->cutScene->initialize();
    }

    int race = ((SolarSystem *)((void *)(long)gStatus->getSystem()))->getRace();
    MatrixSetTranslation(from, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(from, 0.0f, 0.0f, 0.0f);
    MatrixSetTranslation(to, (float)race, 0.0f, 0.0f);
    MatrixSetRotation(to, 0.0f, 0.0f, 0.0f);
    this->cameraEase = new AbyssEngine::EaseInOutMatrix(*(Matrix *)from, *(Matrix *)to, 3000);

    void *cameraSlot = *(void **)&SpaceLounge_ctor_camera_slot;
    void *camera = *(void **)cameraSlot;
    void *current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
    ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)current);
    this->initialized = 1;
    this->introDone = 0;
}

void SpaceLounge::startChat() {
    String title;
    String body;
    String left;
    String right;

    if (this->agents == 0 || this->selectedAgent < 0) {
        return;
    }

    void *agent = (*this->agents)[this->selectedAgent];
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

    this->choiceWindow->setText(title, body);
    this->choiceWindow->setButtonText(left, right);

    if (((Agent *)(agent))->isKnown() == 0 && ((Agent *)(agent))->isStoryAgent() == 0) {
        ((Agent *)(agent))->setKnown(true);
    }
    SpaceLounge_getSoundId(this, agent);

    this->choiceVisible = 1;
    this->chatActive = 1;
    this->mode = 1;

    if (offer == 1) {
        this->mode = 3;
    } else if (((Agent *)(agent))->isGenericAgent() != 0) {
        ((Agent *)(agent))->setEvent(1);
    }
}

// SpaceLounge::~SpaceLounge() — tears down the owned ChoiceWindow / CutScene, the
// agent-text and button arrays, the per-system ImagePart grid and silhouette-vector
// array, the camera-ease matrix, and the cached canvas buffer + title String.
SpaceLounge::~SpaceLounge()
{
    delete this->choiceWindow;
    this->choiceWindow = 0;

    delete this->cutScene;
    this->cutScene = 0;

    delete[] this->agentVisited;
    this->agentVisited = 0;

    {
        Array<String *> *strings = this->agentTexts;
        if (strings != 0) {
            strings->clear();
            delete strings;
        }
    }
    this->agentTexts = 0;

    {
        Array<TouchButton *> *buttons = this->buttons;
        if (buttons != 0) {
            buttons->clear();
            delete buttons;
        }
    }
    this->buttons = 0;

    if (this->agentImageParts != 0) {
        return;
    }
    this->agentImageParts = 0;

    {
        Array<Array<ImagePart *> *> *grid = this->silhouetteGrid;
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
    this->silhouetteGrid = 0;

    {
        Array<Vector *> *rects = this->agentRects;
        if (rects != 0) {
            rects->clear();
            delete rects;
        }
    }
    this->agentRects = 0;

    delete this->cameraEase;
    this->cameraEase = 0;

    delete this->headEase;
    this->headEase = 0;
}

void SpaceLounge::draw() {
    String title;

    if (this->listVisible != 0) {
        if (this->listWindow->shows3DShip() != 0) {
            void *canvasSlot = *(void **)&SpaceLounge_draw_canvas_slot;
            void *canvas = *(void **)canvasSlot;
            ((PaintCanvas *)canvas)->SetColor((unsigned char)(long)canvas, 0, 0, 0);
            int width = pc_GetWidth((PaintCanvas *)canvas);
            int height = pc_GetHeight((PaintCanvas *)canvas);
            (void)height;
            ((PaintCanvas *)canvas)->FillRectangle((int)(long)canvas, 0, 0, width);
            ((PaintCanvas *)canvas)->SetColor((unsigned int)(long)canvas);
        }
        this->listWindow->draw();
        return this->draw_cutscene_tail();
    }

    if (this->mapVisible != 0) {
        return this->draw_map_tail(this->starMap);
    }

    void *layoutSlot = *(void **)&SpaceLounge_draw_layout_slot;
    void *layout = *(void **)layoutSlot;
    void *textsSlot = *(void **)&SpaceLounge_draw_text_slot;
    void *text = ((GameText *)(*(void **)textsSlot))->getText(0x18e);
    title.ctor_copy((String *)text, false);
    ((Layout *)(layout))->drawHeader_call(&title);

    ((SpaceLounge *)(this))->drawLounge();

    layout = *(void **)layoutSlot;
    if ((this->mode & 0xfffffffe) == 2) {
        ((Layout *)layout)->drawFooterNoBackButton();
    } else {
        ((Layout *)layout)->drawFooter();
    }

    if (this->chatActive != 0 || this->field_0x1a != 0 || this->choiceVisible != 0) {
        this->choiceWindow->draw();
    }
}

void SpaceLounge::update(int dt) {
    char maxMatrix[60];
    char valueMatrix[60];

    if (dt > 50) {
        dt = 50;
    }
    if (this->initialized == 0) {
        return;
    }

    if (this->mapVisible != 0) {
        return this->update_map_tail(this->starMap, dt);
    }
    if (this->listVisible != 0) {
        return this->update_ship_tail(this->listWindow, dt);
    }

    if (this->field_0xbc == 0) {
        this->cameraEase->Increase((float)dt);
    }

    void *cameraSlot;
    void *camera;
    void *current;
    if (this->introDone == 0) {
        ((AbyssEngine::EaseInOutMatrix *)(maxMatrix))->GetMaxValue();
        ((AbyssEngine::EaseInOutMatrix *)(valueMatrix))->GetValue();
        if ((memcmp((maxMatrix),(valueMatrix),sizeof(float)*16)==0) == 0) {
            this->field_0xbc = 0;
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
        this->introDone = 1;
        this->field_0x104 = this->field_0x104 + step;
        float wave = Sinf(this->field_0x104);
        float maxRot = (float)this->field_0x108 * 0.5f;

        if (this->field_0xbc == 0) {
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_a;
            camera = *(void **)cameraSlot;
            current = ((PaintCanvas *)camera)->CameraGetLocal((unsigned int)(long)camera);
            (void)current;
            this->field_0xbc = 1;
            int amount = AbyssEngine::AERandom::nextInt(*(void **)&SpaceLounge_update_random_slot, 10);
            this->field_0xc4 = 0;
            if (this->headEase == 0) {
                this->headEase = new AbyssEngine::EaseInOut(0.0f, (float)amount);
            } else {
                this->headEase->SetRange(0.0f, (float)amount);
            }
            this->field_0x108 = 2;
        } else {
            float value = this->headEase->GetValue();
            float maxValue = this->headEase->GetMaxValue();
            float distance = value - maxValue;
            if (distance < 0.0f) {
                distance = -distance;
            }
            int amount = this->field_0x108;
            if (distance < 0.25f) {
                void *random = *(void **)&SpaceLounge_update_random_slot;
                amount = AbyssEngine::AERandom::nextInt(random, 10);
                float next = (float)(5 - amount);
                this->field_0xc4 = value > next;
                this->headEase->SetRange(value, next);
                this->field_0x108 = AbyssEngine::AERandom::nextInt(random, 4) + 1;
                amount = this->field_0x108;
            }
            if (this->field_0xc4 != 0) {
                amount = -amount;
            }
            this->headEase->Increase((float)(dt * amount));
        }

        float value = this->headEase->GetValue();
        MatrixSetRotation(valueMatrix, value / 90.0f, 0.0f, value);
        MatrixSetTranslation(maxMatrix, 0.0f, 0.0f, wave * maxRot);
        AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)(valueMatrix),this->baseMatrix);
        cameraSlot = *(void **)&SpaceLounge_update_camera_slot_b;
        camera = *(void **)cameraSlot;
        current = (void *)(long)((PaintCanvas *)camera)->CameraGetCurrent();
        ((PaintCanvas *)camera)->CameraSetLocal((unsigned int)(long)camera, *(const AbyssEngine::AEMath::Matrix *)valueMatrix);
    }

    ((SpaceLounge *)(this))->updateScreenPositions();
    if (this->mode != 0) {
        this->scrollWindow->update(dt);
    }
    this->cutScene->update();

    if (this->touchDown != 0) {
        int x = this->touchX;
        int y = this->touchY;
        int top = *(int *)SpaceLounge_update_top_slot;
        int height = *(int *)SpaceLounge_update_height_slot;
        int width = *(int *)SpaceLounge_update_width_slot;
        CutScene *cutscene = this->cutScene;
        if (y < top) {
            if (x < 150) {
                cutscene->vec8.y -= 10.0f;
            } else if (x > width - 150) {
                cutscene->vec8.y += 10.0f;
            }
        } else if (y < height - top * 2) {
            if (x < 70) {
                cutscene->cameraRotX -= 7.5f;
            } else if (x > width - 70) {
                cutscene->cameraRotX += 7.5f;
            } else if (x > 100 && x < width - 100) {
                if (y < height / 2) {
                    cutscene->vec8.z -= (float)dt;
                } else {
                    cutscene->vec8.z += (float)dt;
                }
            }
        } else {
            if (x < 70) {
                cutscene->vec8.x -= (float)dt;
            } else if (x > width - 70) {
                cutscene->vec8.x += (float)dt;
            }
        }
    }
}

// Mode-dependent tail handlers: each forwards rendering/updating to whichever
// sub-screen (StarMap / CutScene / ListItemWindow / Layout) is live for the current
// mode. The receiver is supplied by the caller, except draw_cutscene_tail which reads
// the active draw-layout from its global slot.

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
    // When a cutscene is active the lounge draws the cutscene's background layer.
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
