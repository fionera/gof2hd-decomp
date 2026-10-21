
#include "game/menu/SpaceLounge.h"
#include "engine/core/AERandom.h"
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
#include "game/ui/ScrollTouchBox.h"
#include "game/world/StarMap.h"
#include "game/ship/Agent.h"
#include "game/core/Globals.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
#include "game/mission/Mission.h"
#include "game/mission/Status.h"
#include "game/ui/TouchButton.h"

#include "engine/render/PaintCanvas.h"

static inline int pc_GetWidth(PaintCanvas *self) { return self->GetWidth(); }
static inline int pc_GetHeight(PaintCanvas *self) { return self->GetHeight(); }

static inline int &layoutMetric(Layout *layout, int off) {
    return *(int *) ((char *) layout + off);
}

namespace AbyssEngine {
    namespace AEMath {
        Vector operator+(const Vector &, const Vector &);

        Vector operator-(const Vector &, const Vector &);

        Vector operator*(const Vector &, float);

        void MatrixMultiply(Matrix &, const Matrix &);
    }
}

// Projection helper installed in SpaceLounge_screen_projector (maps a world Vector to a screen Vector).
typedef void (*ScreenProjectFn)(Vector *out, Vector *in);


void MatrixSetTranslation(void *matrix, float x, float y, float z); // lint: void_ptr (external symbol; mangling must match lib)

// lint: void_ptr (external symbol; mangling must match lib)

void MatrixSetRotation(void *matrix, float x, float y, float z); // lint: void_ptr (external symbol; mangling must match lib)

// lint: void_ptr (external symbol; mangling must match lib)

static Layout **SpaceLounge_touch_layout_slot;
static GameText **SpaceLounge_touch_help_text_slot;
static GameText **SpaceLounge_touch_list_help_text_slot;
static PaintCanvas **SpaceLounge_touch_camera_slot;
static int SpaceLounge_touch_race_vectors[64];

namespace AbyssEngine {
}

static int *SpaceLounge_getSoundId_missionText;
static GameText **SpaceLounge_getSoundId_offerText;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer2358910;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer5;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer7;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer4;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer6;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer0_11;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer0_else;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_chance;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_offer1;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_accepted;
static GameText **SpaceLounge_getSoundId_specialText;
static AbyssEngine::AERandom *SpaceLounge_getSoundId_specialRandom;

static inline void MatrixGetRight(void *out, void *matrix) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetRight(*(const AbyssEngine::AEMath::Matrix*)matrix); }

static inline void MatrixGetPosition(void *out, void *matrix) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetPosition(*(const AbyssEngine::AEMath::Matrix*)matrix); }

static inline void MatrixGetUp(void *out, void *matrix) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetUp(*(const AbyssEngine::AEMath::Matrix*)matrix); } // lint: void_ptr (external symbol; mangling must match lib)

static inline void MatrixGetLookAt(void *out, void *pos, void *target, void *up) { *(AbyssEngine::AEMath::Matrix*)out = AbyssEngine::AEMath::MatrixGetLookAt(*(const AbyssEngine::AEMath::Vector*)pos, *(const AbyssEngine::AEMath::Vector*)target, *(const AbyssEngine::AEMath::Vector*)up); } // lint: void_ptr (external symbol; mangling must match lib)

// lint: void_ptr (external symbol; mangling must match lib)

static inline void MatrixGetDir(void *out, void *matrix) { *(AbyssEngine::AEMath::Vector*)out = AbyssEngine::AEMath::MatrixGetDir(*(const AbyssEngine::AEMath::Matrix*)matrix); } // lint: void_ptr (external symbol; mangling must match lib)

static Level *SpaceLounge_screen_level_slot;
static PaintCanvas **SpaceLounge_screen_canvas_slot;
static ScreenProjectFn SpaceLounge_screen_projector;
static PaintCanvas **SpaceLounge_lounge_canvas_slot;
static Layout **SpaceLounge_lounge_layout_slot;
static ImageFactory **SpaceLounge_lounge_image_factory_slot;
static GameText **SpaceLounge_lounge_text_slot;
static String **SpaceLounge_lounge_font_slot;

void *Station_getAgents(void *station); // lint: void_ptr (external symbol; mangling must match lib)

static Layout **SpaceLounge_init_layout_slot;
static GameText **SpaceLounge_init_text_slot;
static PaintCanvas **SpaceLounge_init_camera_slot;

void ArrayRemove_AgentPtr(void *agent, void *array); // lint: void_ptr (external symbol; mangling must match lib)

static PaintCanvas **SpaceLounge_ctor_camera_slot;
static GameText **SpaceLounge_start_text_slot;
static Layout **SpaceLounge_draw_layout_slot;
static PaintCanvas **SpaceLounge_draw_canvas_slot;
static GameText **SpaceLounge_draw_text_slot;

static PaintCanvas **SpaceLounge_update_camera_slot_a;
static PaintCanvas **SpaceLounge_update_camera_slot_b;
static PaintCanvas **SpaceLounge_update_camera_slot_c;
static AbyssEngine::AERandom *SpaceLounge_update_random_slot;
static int *SpaceLounge_update_width_slot;
static int *SpaceLounge_update_height_slot;
static int *SpaceLounge_update_top_slot;

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

    switch (this->mode) {
        case 0:
            if (this->agents != 0) {
                float fx = (float) x;
                float fy = (float) y;
                this->selectedAgent = -1;
                this->hoverAgent = -1;
                for (int i = 0; i != (int) this->agents->size(); ++i) {
                    float *a = (float *) (*this->agentRects)[i * 2];
                    if (a[0] < fx) {
                        float *b = (float *) (*this->agentRects)[i * 2 + 1];
                        if (fx < b[0] && fy < a[1] && b[1] < fy) {
                            this->hoverAgent = i;
                        }
                    }
                }
            }
            break;
        case 3:
            (*this->buttons)[0]->OnTouchMove(x, y);
            break;
        case 2:
            for (unsigned i = 0; i < this->buttons->size(); ++i) {
                (*this->buttons)[i]->OnTouchMove(x, y);
            }
            break;
    }

    Globals::layout->OnTouchMove(x, y);
    if (this->listVisible != 0) {
        this->listWindow->OnTouchMove(x, y);
    } else {
        this->scrollWindow->OnTouchMove(x, y);
    }
    return 0;
}

void SpaceLounge::OnRender3D() {
    if (this->mapVisible != 0) {
        this->starMap->render();
        return;
    }

    CutScene *cutscene = this->cutScene;
    if (cutscene == 0) {
        return;
    }

    if (this->listVisible != 0) {
        if (this->listWindow->shows3DShip() != 0) {
            return;
        }
        cutscene = this->cutScene;
    }

    cutscene->render3D();
}

void SpaceLounge::OnRenderBG() {
    if (this->cutScene != 0) {
        this->cutScene->renderBG();
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

bool SpaceLounge::checkLocationMode() {
    return false;
}

void SpaceLounge::draw3DShip() {
    if (this->listVisible != 0) {
        this->listWindow->render();
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
        Globals::layout->OnTouchBegin(x, y);
        this->scrollWindow->OnTouchBegin(x, y);
        return 0;
    }

    switch (this->mode) {
        case 0:
            if (this->introDone == 0) {
                return 0;
            }
            if (this->agents != 0) {
                float fx = (float) x;
                float fy = (float) y;
                for (int i = 0; i != (int) this->agents->size(); ++i) {
                    float *a = (float *) (*this->agentRects)[i * 2];
                    if (a[0] < fx) {
                        float *b = (float *) (*this->agentRects)[i * 2 + 1];
                        if (fx < b[0] && fy < a[1] && b[1] < fy) {
                            this->hoverAgent = i;
                        }
                    }
                }
            }
            break;
        case 3:
            (*this->buttons)[0]->OnTouchBegin(x, y);
            break;
        case 2:
            for (unsigned i = 0; i < this->buttons->size(); ++i) {
                (*this->buttons)[i]->OnTouchBegin(x, y);
            }
            break;
    }

    Globals::layout->OnTouchBegin(x, y);
    this->scrollWindow->OnTouchBegin(x, y);
    return 0;
}

int SpaceLounge::getSpecificSoundForRace(int soundId, int race, bool alternate) {
    // Generic chat sounds 759..806 map onto a per-race bank.
    switch (race) {
        case 0:
        case 5:
            if (alternate) {
                if (soundId >= 759 && soundId < 807) {
                    return soundId + 240;
                }
            } else if (soundId >= 759 && soundId < 807) {
                return soundId + 192;
            }
            break;
        case 1:
            if (soundId >= 759 && soundId < 807) {
                return soundId + 288;
            }
            break;
        case 2:
        case 3:
            if (soundId >= 759 && soundId < 807) {
                return soundId + 144;
            }
            break;
        case 4:
            if (soundId >= 759 && soundId < 807) {
                return soundId + 96;
            }
            break;
        case 6:
            return soundId;
        case 7:
            if (soundId >= 759 && soundId < 807) {
                return soundId + 48;
            }
            break;
    }
    return -1;
}

static inline Agent *selected_agent(SpaceLounge *self) {
    Array<Agent *> *agents = self->agents;
    return (*agents)[self->selectedAgent];
}

static inline bool hit_agent(SpaceLounge *self, int x, int y, int i) {
    Array<Vector *> *rects = self->agentRects;
    float *a = (float *) (*rects)[i * 2];
    if (!(a[0] < (float) x)) {
        return false;
    }
    float *b = (float *) (*rects)[i * 2 + 1];
    return (float) x < b[0] && (float) y < a[1] && b[1] < (float) y;
}

bool SpaceLounge::OnTouchEnd(int x, int y) {
    String helpBig;
    String helpSmall;
    char matrix[60];

    this->touchDown = 0;

    if (this->choiceVisible != 0 || this->chatActive != 0) {
        int result = this->choiceWindow->OnTouchEnd(x, y);
        if (result == 1) {
            this->chatActive = 0;
        } else if (result == 0) {
            ((SpaceLounge *) (this))->onKeyPress(0x10000);
        }
        return false;
    }

    if (this->mapVisible != 0) {
        if (this->starMap->OnTouchEnd(x, y) != 0) {
            this->cutScene->resetCamera();
            this->mapVisible = 0;
        }
        return false;
    }

    Layout **layoutSlot = SpaceLounge_touch_layout_slot;
    Layout *layout = *layoutSlot;
    if (layout->OnTouchEnd(x, y) != 0) {
        if (this->listVisible != 0) {
            layout->resetWindowDimensions();
            this->listVisible = 0;
        } else if (this->mode != 0) {
            if (this->selectedAgent >= 0) {
                Agent *agent = selected_agent(this);
                if (agent->isGenericAgent() != 0) {
                    agent->setEvent(1);
                }
            }
            this->mode = 0;
            this->singleOffer = 0;
        }
        return false;
    }

    if (this->listVisible != 0) {
        this->listWindow->OnTouchEnd(x, y);
        if (layout->helpPressed() != 0) {
            GameText **texts = SpaceLounge_touch_list_help_text_slot;
            String *text = (*texts)->getText(0x283);
            helpSmall.Set(text->data);
            layout->initHelpWindow(helpSmall);
        }
        return false;
    }

    switch (this->mode) {
        case 0:
            if (this->introDone == 0) {
                SolarSystem *system = (SolarSystem *) (long) Globals::status->getSystem();
                int race = system->getRace();
                int *v = &SpaceLounge_touch_race_vectors[race * 3];
                MatrixSetTranslation(matrix, (float) v[2], (float) v[0], (float) v[1]);
                MatrixSetRotation(matrix, 0.0f, 0.0f, 0.0f);
                PaintCanvas **cameraSlot = SpaceLounge_touch_camera_slot;
                PaintCanvas *camera = *cameraSlot;
                const AbyssEngine::AEMath::Matrix *current =
                        (const AbyssEngine::AEMath::Matrix *) (long) camera->CameraGetCurrent();
                camera->CameraSetLocal((unsigned int) (long) camera, *current);
                if (this->cameraEase != 0) {
                    this->cameraEase->SetRange(*(AbyssEngine::AEMath::Matrix *) matrix,
                                               *(AbyssEngine::AEMath::Matrix *) matrix);
                }
                this->introDone = 1;
                this->headBobPhase = 0;
                return false;
            }
            if (this->agents != 0) {
                this->hoverAgent = -1;
                this->selectedAgent = -1;
                unsigned count = (unsigned) this->agents->size();
                for (unsigned i = 0; i < count; ++i) {
                    if (hit_agent(this, x, y, i)) {
                        this->selectedAgent = i;
                        ((SpaceLounge *) (this))->onKeyPress(0x10000);
                        return false;
                    }
                }
            }
            break;
        case 1:
            ((SpaceLounge *) (this))->onKeyPress(0x10000);
            break;
        case 2: {
            Array<TouchButton *> *buttons = this->buttons;
            for (unsigned i = 0; i < buttons->size(); ++i) {
                TouchButton *button = (*buttons)[i];
                if (button->OnTouchEnd(x, y) != 0) {
                    Agent *agent = selected_agent(this);
                    if (i >= 5 && agent->isGenericAgent() != 0) {
                        agent->setEvent(1);
                    }
                }
            }
            break;
        }
        case 3:
            if ((*this->buttons)[0]->OnTouchEnd(x, y) != 0) {
                ((SpaceLounge *) (this))->onKeyPress(0x10000);
            }
            break;
    }

    this->scrollWindow->OnTouchEnd(x, y);
    if (layout->helpPressed() != 0) {
        GameText **texts = SpaceLounge_touch_help_text_slot;
        String *text = (*texts)->getText(0x273);
        helpBig.Set(text->data);
        layout->initHelpWindow(helpBig);
    }
    return false; // TODO(decomp): the original returns true when the window closes
}

int SpaceLounge::getSoundId(Agent *agent) {
    int race = agent->getRace();
    bool male = agent->isMale();
    int offer = agent->getOffer();
    int missionType;
    if (agent->getMission() == 0 || agent->getMission()->isEmpty()) {
        missionType = -1;
    } else {
        missionType = agent->getMission()->getType();
    }
    String missionText = Globals::globals->getAgentMissionText(agent);

    // Greeting by offer kind.
    bool canBeSpecial = true;
    int soundId;
    switch (offer) {
        case 0:
            if (missionType == 0 || missionType == 11) {
                soundId = Globals::rnd->nextInt(4) + 769;
            } else if (missionType == 12) {
                soundId = Globals::rnd->nextInt(4) + 762;
            } else if (Globals::rnd->nextInt(2) != 0) {
                soundId = Globals::rnd->nextInt(4) + 777;
            } else {
                soundId = Globals::rnd->nextInt(4) + 799;
            }
            break;
        case 1:
            soundId = Globals::rnd->nextInt(2) + 781;
            break;
        case 2:
        case 3:
        case 8:
        case 9:
        case 10:
            soundId = Globals::rnd->nextInt(2) + 759;
            break;
        case 4:
            soundId = Globals::rnd->nextInt(2) + 766;
            break;
        case 5:
            soundId = Globals::rnd->nextInt(4) + 795;
            break;
        case 6:
            soundId = Globals::rnd->nextInt(4) + 803;
            break;
        case 7:
            soundId = Globals::rnd->nextInt(4) + 773;
            break;
        default:
            soundId = -1;
            break;
    }
    if (offer != 1) {
        canBeSpecial = false;
        if (Globals::rnd->nextInt(100) < 30) {
            soundId = Globals::rnd->nextInt(2) + 781;
        }
    }
    if (agent->hasAcceptedOffer()) {
        soundId = Globals::rnd->nextInt(2) + 781;
    }
    if (canBeSpecial && (missionText.Compare(*Globals::gameText->getText(820)) == 0 ||
                         missionText.Compare(*Globals::gameText->getText(824)) == 0 ||
                         missionText.Compare(*Globals::gameText->getText(827)) == 0 ||
                         missionText.Compare(*Globals::gameText->getText(833)) == 0)) {
        soundId = Globals::rnd->nextInt(2) + 788;
    }

    if (race == 3) {
        // Midorian voices: only the type-2 face uses the race-3 bank.
        if (agent->getImageParts() == 0) {
            race = 3;
        } else {
            race = agent->getImageParts()[0] == 2 ? 3 : 0;
        }
    }
    return this->getSpecificSoundForRace(soundId, race, male);
}

static inline Agent *key_agent(SpaceLounge *self) {
    return (*self->agents)[self->selectedAgent];
}

void SpaceLounge::onKeyPress(int key) {
    char scratch[960];
    (void) scratch;

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
                current = (int) this->agents->size();
            }
            this->selectedAgent = current - 1;
        } else if (key == 0x10000 || key == 0x20000) {
            ((SpaceLounge *) (this))->startChat();
        }
        return;
    }

    if (mode == 1 || mode == 3) {
        if (key == 0x10000 || key == 0x20000) {
            this->chatAnswer = 0;
            Agent *agent = key_agent(this);
            if (agent->getOffer() == 1) {
                this->mode = 2;
            } else {
                this->mode = mode == 1 ? 2 : 0;
            }
        } else if (key == 0x40000) {
            if (this->chatScroll < 3) {
                if (mode == 1) {
                    this->chatScroll = 0;
                    this->mode = 0;
                }
            } else {
                this->chatScroll -= 0x28;
            }
        }
        return;
    }

    if (mode == 2) {
        Agent *agent = key_agent(this);
        if (key == 0x40000) {
            this->mode = 1;
        } else if (key == 0x10000 || key == 0x20000) {
            if (agent->getMission() == 0 && agent->isGenericAgent() != 0) {
                agent->setEvent(1);
            }
            this->mode = 0;
        } else if (key == 0x8000) {
            if (ScrollTouchBox *box = this->scrollWindow->scrollBox) {
                if (box->contentHeight - box->height > 0) {
                    box->scrollOffset -= box->height;
                    box->update(0);
                }
            }
        } else if (key == 0x4000) {
            if (ScrollTouchBox *box = this->scrollWindow->scrollBox) {
                if (box->contentHeight - box->height > 0) {
                    box->scrollOffset += box->height;
                    box->update(0);
                }
            }
        }
    }
}

void SpaceLounge::updateScreenPositions() {
    using namespace AbyssEngine::AEMath;
    if (this->agents == 0) {
        return;
    }

    Array<KIPlayer *> *figures = this->cutScene->level->getEnemies();
    Vector offset = Vector();
    Vector pos = Vector();
    Vector screen = Vector();
    PaintCanvas *canvas = Globals::Canvas;
    offset = 150.0f * MatrixGetRight(*(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent()));

    for (unsigned int i = 0; i < this->agents->size(); ++i) {
        // Name-plate anchors: left/right of the figure, the right one raised above its head.
        pos = (*figures)[i]->getPosition();
        screen = pos - offset;
        Globals::Canvas->GetScreenPosition(screen, *(*this->agentRects)[i * 2]);
        screen = pos + offset;
        screen.y += 450.0f;
        Globals::Canvas->GetScreenPosition(screen, *(*this->agentRects)[i * 2 + 1]);

        PaintCanvas *c = Globals::Canvas;
        Matrix look = *(const Matrix *) c->CameraGetLocal(c->CameraGetCurrent());
        look = MatrixGetLookAt(MatrixGetPosition(look), pos, MatrixGetUp(look));
        (*figures)[i + this->agents->size()]->geometry->setMatrix(look);

        this->silhouettePos = pos;
        this->silhouettePos.x -= MatrixGetDir(look).x * 100.0f;
        this->silhouettePos.z -= MatrixGetDir(look).z * 100.0f;
        (*figures)[i + this->agents->size()]->setPosition(this->silhouettePos);

        if (((SolarSystem *) Globals::status->getSystem())->getRace() == 0) {
            Matrix turn;
            MatrixSetRotation(turn, 0.0f, 3.14159274f, 0.0f);
            look *= turn;
        }
        (*figures)[i]->geometry->setMatrix(look);
        (*figures)[i]->setPosition(pos);
    }
}

static inline TouchButton *button_at(SpaceLounge *self, unsigned i) {
    return (*self->buttons)[i];
}

void SpaceLounge::drawLounge() {
    if (this->mode != 0) {
        // Conversation: portrait panel + answer buttons.
        Globals::Canvas->SetColor(0xffffffffu);
        Layout *layout = Globals::layout;
        layout->drawBox(2, this->panelX, this->panelY, layout->field_0x68, layout->field_0x6c, String(""));
        Globals::Canvas->SetColor(0x193043ffu);
        Globals::Canvas->DrawRectangle(this->panelX, this->panelY, Globals::layout->field_0x68,
                                       Globals::layout->field_0x6c);
        Globals::Canvas->SetColor(0xffffffffu);
        Globals::imageFactory->drawChar((*this->silhouetteGrid)[this->selectedAgent],
                                        Globals::layout->field_0x4c + this->panelX,
                                        Globals::layout->field_0x4c + this->panelY, false);
        this->scrollWindow->draw();
        if ((this->mode & ~1) != 2) {
            return;
        }

        (*this->buttons)[0]->setTextColor(0xed00ff);
        int offer = (*this->agents)[this->selectedAgent]->getOffer();
        if (this->mode == 2) {
            (*this->buttons)[0]->setPosition(this->buttonX, this->buttonY0);
            (*this->buttons)[1]->setPosition(this->panelWidth + this->buttonX, this->buttonY0, 18);
            this->visibleButtonCount = 0;
            if (offer <= 10 && ((1 << offer) & 0x60c) != 0) {
                this->visibleButtonCount = 3;
            } else if (offer == 0 && (*this->agents)[this->selectedAgent]->getMission() != 0 &&
                       (*this->agents)[this->selectedAgent]->getMission()->isOutsideMission() &&
                       (*this->agents)[this->selectedAgent]->getMission()->getType() != 12) {
                this->visibleButtonCount = this->singleOffer == 0 ? 4 : 1;
                if (this->singleOffer != 0) {
                    (*this->buttons)[0]->setTextColor(-1);
                    (*this->buttons)[0]->setPosition(this->buttonX, this->buttonY1);
                }
            } else if (offer == 1) {
                this->visibleButtonCount = 1;
                (*this->buttons)[0]->setPosition(this->buttonX, this->buttonY1);
            } else {
                this->visibleButtonCount = 2;
                (*this->buttons)[0]->setPosition(this->buttonX, this->buttonY1);
                (*this->buttons)[1]->setPosition(this->panelWidth + this->buttonX, this->buttonY1, 18);
            }
        } else {
            this->visibleButtonCount = 1;
            (*this->buttons)[0]->setTextColor(-1);
            (*this->buttons)[0]->setPosition(this->buttonX, this->buttonY1);
        }

        for (unsigned int i = 0; i < this->buttons->size(); i++) {
            (*this->buttons)[i]->setVisible(false);
        }
        layout = Globals::layout;
        int inset = layout->field_0x4c * 2;
        int boxHeight = layout->field_0x2d8;
        if (this->visibleButtonCount > 2) {
            int stacked = layout->field_0x30_rowHeight * (this->visibleButtonCount - 1) +
                          layout->field_0x34 * (this->visibleButtonCount - 2);
            boxHeight = boxHeight > stacked ? boxHeight : stacked;
        }
        layout->drawBox(2, this->panelX, this->buttonPanelY, layout->field_0x68, inset + boxHeight, String(""));
        Globals::Canvas->SetColor(0x193043ffu);
        Globals::Canvas->DrawRectangle(this->panelX, this->buttonPanelY, Globals::layout->field_0x68, inset + boxHeight);
        Globals::Canvas->SetColor(0xffffffffu);
        Globals::imageFactory->drawChar(this->agentImageParts, Globals::layout->field_0x4c + this->panelX,
                                        this->buttonPanelY + Globals::layout->field_0x4c, true);

        if (offer == 1 || this->singleOffer != 0 || this->mode == 3) {
            (*this->buttons)[0]->setVisible(true);
            (*this->buttons)[0]->draw();
            return;
        }
        (*this->buttons)[0]->setVisible(true);
        (*this->buttons)[1]->setVisible(true);
        (*this->buttons)[0]->draw();
        (*this->buttons)[1]->draw();
        if (offer > 10) {
            return;
        }
        if (((1 << offer) & 0x60c) != 0) {
            (*this->buttons)[2]->setVisible(true);
            (*this->buttons)[2]->draw();
        } else if (offer == 0 && (*this->agents)[this->selectedAgent]->getMission()->isOutsideMission() &&
                   (*this->agents)[this->selectedAgent]->getMission()->getType() != 12) {
            (*this->buttons)[3]->setVisible(true);
            (*this->buttons)[4]->setVisible(true);
            (*this->buttons)[3]->draw();
            (*this->buttons)[4]->draw();
        }
        return;
    }

    // Lounge overview: name plate above the hovered agent.
    if (this->hoverAgent < 0) {
        return;
    }
    Agent *agent = (*this->agents)[this->hoverAgent];
    Vector *left = (*this->agentRects)[this->hoverAgent * 2];
    Vector *right = (*this->agentRects)[this->hoverAgent * 2 + 1];
    int pad = Globals::layout->field_0x94;
    Globals::Canvas->SetColor(0xffffffffu);
    int centerX = (int) (left->x + (right->x - left->x) * 0.5f);
    int top = (int) (right->y - (float) (pad * 2));

    String name = agent->isKnown() || agent->isStoryAgent()
                          ? agent->getName()
                          : String(*Globals::gameText->getText(agent->getRace() + 406), false);
    String role;
    if (agent->getMission() != 0) {
        role = String(*Globals::gameText->getText(agent->getMission()->getType() + 354), false);
    } else if (agent->getOffer() == 6) {
        role = String(*Globals::gameText->getText(306), false);
    } else if (agent->getOffer() == 2) {
        role = String(*Globals::gameText->getText(305), false);
    } else if (agent->getOffer() == 7) {
        role = String(*Globals::gameText->getText(884), false);
    } else {
        role = String("");
    }

    int width = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, name);
    if (agent->isKnown()) {
        width += Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, role + String("  "));
    }
    Layout *layout = Globals::layout;
    int x = centerX;
    if (x - pad < layout->buttonInsetX) {
        x = layout->buttonInsetX + pad;
    } else if (pad + x + width > Globals::w - layout->buttonInsetX) {
        x = Globals::w - layout->buttonInsetX - (width + pad * 2);
    }
    int lineHeight = layout->field_0x30_rowHeight;
    layout->drawBox(2, x - pad, top - pad, pad * 2 + width, lineHeight, String(""));
    Globals::Canvas->SetColor(0x193043ffu);
    Globals::Canvas->DrawRectangle(x - pad, top - pad, pad * 2 + width, Globals::layout->field_0x30_rowHeight);
    Globals::Canvas->SetColor(0xffffffffu);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                name + (agent->isKnown() ? String("  ") : String("")), x,
                                top + Globals::layout->field_0x2c0, false);
    if (agent->isKnown()) {
        Globals::Canvas->SetColor(0xff8000ffu);
        Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, role,
                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, name + String("  ")) + x,
                top + Globals::layout->field_0x2c0, false);
    }
}

// Lounge camera fly-in per lounge race: start/end translations and Y rotations.
static const int SpaceLounge_cameraStartPos[4 * 3] = {
        -1315, 1160, -2728, 1484, 2980, -1626, -2472, 734, -3000, -2598, 400, 1813};
static const int SpaceLounge_cameraEndPos[4 * 3] = {
        -1800, 620, 1805, 607, 610, 2362, 136, 444, 1122, 2222, 450, 1227};
static const float SpaceLounge_cameraStartRot[4] = {0.03f, 0.03f, 0.21f, 1.87f};
static const float SpaceLounge_cameraEndRot[4] = {0.04f, 0.43f, 0.6f, 0.46f};

// Character descriptor for the anonymous silhouette (all parts default).
static int SpaceLounge_silhouetteChar[8];

int SpaceLounge::init() {
    this->touchDown = 0;
    this->initialized = 0;
    this->cameraAnimating = 0;
    this->agents = Globals::status->getStation()->getAgents();

    if (this->silhouetteGrid == 0 && this->agents != 0 && this->agentRects == 0) {
        this->silhouetteGrid = new Array<Array<ImagePart *> *>();
        ArraySetLength(this->agents->size(), *this->silhouetteGrid);
        this->agentRects = new Array<Vector *>();
        ArraySetLength(this->agents->size() * 2, *this->agentRects);
        for (unsigned int i = 0; i < this->agents->size(); i++) {
            (*this->silhouetteGrid)[i] = Globals::imageFactory->loadChar((*this->agents)[i]->getImageParts());
            (*this->agentRects)[i] = new Vector();
            (*this->agentRects)[this->agents->size() + i] = new Vector();
        }
        this->agentImageParts = new Array<ImagePart *>();
        this->agentImageParts = Globals::imageFactory->loadChar(SpaceLounge_silhouetteChar);
    }

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
    this->agentVisited = new char[21];
    for (int i = 0; i != 21; ++i) {
        this->agentVisited[i] = 0;
    }

    // Agent list panel + buttons.
    Layout *layout = Globals::layout;
    this->panelX = Globals::w / 2 - layout->field_0x68 / 2;
    this->panelY = layout->field_0x20_top + layout->field_0xc_leftMargin;
    this->panelWidth = layout->field_0x68 - layout->field_0x4c * 3 - layout->field_0x2d4;
    this->scrollWindow = new ScrollTouchWindow(this->panelX + layout->field_0x4c * 2 + layout->field_0x2d4, this->panelY,
                                               this->panelWidth, layout->field_0x6c, false);
    this->mode = 0;
    this->buttonPanelY = this->panelY + Globals::layout->field_0x6c + Globals::layout->field_0x2c_rowHeight;
    this->buttonsHeight = (Globals::layout->field_0x34 + Globals::layout->field_0x30_rowHeight) * 5;

    if (this->buttons != 0) {
        ArrayReleaseClasses(*this->buttons);
        delete this->buttons;
    }
    this->buttons = 0;
    this->buttons = new Array<TouchButton *>();
    ArraySetLength(5, *this->buttons);

    layout = Globals::layout;
    this->buttonX = this->panelX + layout->field_0x68 - layout->field_0x4c - this->panelWidth;
    int y = this->buttonPanelY + layout->field_0x4c;
    int rowHeight = layout->field_0x30_rowHeight;
    int rowGap = layout->field_0x34;
    (*this->buttons)[0] = new TouchButton(*Globals::gameText->getText(860), 0, this->buttonX, y,
                                          this->panelWidth / 2 - Globals::layout->field_0x4c / 2, 17, 4);
    (*this->buttons)[0]->setTextColor(0xed00ff);
    (*this->buttons)[1] = new TouchButton(*Globals::gameText->getText(861), 0,
                                          this->panelX + Globals::layout->field_0x68 - Globals::layout->field_0x4c, y,
                                          this->panelWidth / 2 - Globals::layout->field_0x4c, 18, 4);
    (*this->buttons)[1]->setTextColor(0xff2a00ff);
    int y2 = y + rowHeight + rowGap;
    (*this->buttons)[2] = new TouchButton(*Globals::gameText->getText(776), 0, this->buttonX, y2, this->panelWidth, 17, 4);
    (*this->buttons)[3] = new TouchButton(*Globals::gameText->getText(804), 0, this->buttonX, y2, this->panelWidth, 17, 4);
    (*this->buttons)[4] = new TouchButton(*Globals::gameText->getText(807), 0, this->buttonX,
                                          y2 + Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34,
                                          this->panelWidth, 17, 4);
    this->hoverAgent = -1;
    this->buttonY1 = y + Globals::layout->field_0x2d8 / 2 - Globals::layout->field_0x30_rowHeight / 2;
    this->buttonY0 = y;

    // Camera at the lounge's resting view.
    int race = ((SolarSystem *) Globals::status->getSystem())->getRace();
    Matrix rest;
    AbyssEngine::AEMath::MatrixSetTranslation(rest, (float) SpaceLounge_cameraEndPos[race * 3],
                                              (float) SpaceLounge_cameraEndPos[race * 3 + 1],
                                              (float) SpaceLounge_cameraEndPos[race * 3 + 2]);
    AbyssEngine::AEMath::MatrixSetRotation(rest, 0.0f, SpaceLounge_cameraEndRot[race], 0.0f);
    PaintCanvas *canvas = Globals::Canvas;
    canvas->CameraSetLocal(canvas->CameraGetCurrent(), rest);
    if (this->cameraEase != 0) {
        this->cameraEase->SetRange(rest, rest);
    }
    this->introDone = 1;
    this->headBobPhase = 0.0f;
    if (this->cutScene != 0) {
        this->cutScene->level->getStarSystem()->initLight();
    }
    ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(0.5f, 0.5f, 0.5f, 0x4000);

    if (Globals::iPad) {
        // iPad: place the agent panel next to the selected agent.
        float margin = Globals::iPadHD ? 14.0f : (Globals::iPadLarge ? 20.0f : 10.0f);
        if (this->agentRects != 0 && this->selectedAgent >= 0 &&
            (unsigned int) (this->selectedAgent * 2) < this->agentRects->size()) {
            Vector *rect = (*this->agentRects)[this->selectedAgent * 2];
            layout = Globals::layout;
            float x;
            if (rect->x - margin - (float) layout->field_0x68 < (float) layout->buttonInsetX) {
                x = this->field_0x98 + margin;
                float right = (float) (Globals::w - layout->buttonInsetX);
                if (x >= right) {
                    x = right;
                }
            } else {
                x = this->field_0x8c - margin - (float) layout->field_0x68;
                if (x < (float) layout->buttonInsetX) {
                    x = (float) layout->buttonInsetX;
                }
            }
            this->panelX = (int) x;
            int panelHeight = layout->field_0x6c;
            float top = this->field_0x9c - (float) (panelHeight / 3);
            float bottom = (float) (Globals::h - layout->field_0x10_rightMargin - layout->field_0x24 - panelHeight);
            float minTop = (float) (layout->field_0x20_top + layout->field_0xc_leftMargin);
            if (top < bottom) {
                bottom = top;
            }
            if (bottom < minTop) {
                bottom = minTop;
            }
            this->panelY = (int) bottom;
            delete this->scrollWindow;
            this->scrollWindow = 0;
            this->scrollWindow = new ScrollTouchWindow(this->panelX + layout->field_0x4c * 2 + layout->field_0x2d4,
                                                       this->panelY, this->panelWidth, layout->field_0x6c, false);
            layout = Globals::layout;
            this->buttonPanelY = this->panelY + layout->field_0x6c + layout->field_0x2c_rowHeight;
            this->buttonX = this->panelX + layout->field_0x68 - layout->field_0x4c - this->panelWidth;
            int y0 = layout->field_0x4c + this->buttonPanelY;
            float extra = Globals::iPadHD ? 15.46875f : (Globals::iPadLarge ? 22.0f : 11.0f);
            int spacing = (int) (extra + (float) layout->field_0x34);
            this->buttonY1 = y0 - layout->field_0x30_rowHeight / 2 + layout->field_0x2d8 / 2;
            this->buttonY0 = y0;
            int y1 = layout->field_0x30_rowHeight + y0 + spacing;
            (*this->buttons)[2]->setPosition(this->buttonX, y1);
            (*this->buttons)[3]->setPosition(this->buttonX, y1);
            (*this->buttons)[4]->setPosition(this->buttonX, y1 + spacing + Globals::layout->field_0x30_rowHeight);
        }
    }

    this->field_0xb1 = ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->IsPostEffectActivated();
    if (Globals::status->getCurrentCampaignMission() == 24 && Globals::status->getStation()->getIndex() == 10) {
        // Campaign: the arms dealer opens the conversation by himself.
        for (unsigned int i = 0; i < this->agents->size(); i++) {
            Agent *agent = (*this->agents)[i];
            if (agent->getOffer() == 2 && agent->getSellItemIndex() == 68 && !agent->isKnown()) {
                this->selectedAgent = i;
                this->startChat();
                break;
            }
        }
    }
    this->initialized = 1;
    return 0;
}


SpaceLounge::SpaceLounge()
        : silhouettePos(), field_0x8c(0), field_0x90(0), field_0x94(0), field_0x98(0), field_0x9c(0),
          field_0xa0(0) {
    this->selectedAgent = 0;
    this->agents = 0;
    this->agentTexts = 0;
    this->chatScroll = 0;
    this->silhouetteGrid = 0;
    this->agentImageParts = 0;
    this->agentRects = 0;
    this->cutScene = 0;
    this->initialized = 0;
    this->starMap = 0;
    this->choiceWindow = 0;
    this->listWindow = 0;
    this->mapVisible = 0;
    this->hangarUpdate = 0;
    this->singleOffer = 0;
    this->agentVisited = 0;
    this->buttons = 0;
    this->headEase = 0;
    this->listVisible = 0;
    this->mode = 0;
    this->field_0x18 = 0;
    this->chatActive = 0;
    this->popupActive = 0;
    this->choiceVisible = 0;
    this->cameraEase = 0;

    this->init();

    // A finished "rescue" mission agent leaves the lounge: drop it and rebuild.
    if (this->agents != 0) {
        for (unsigned i = 0; i < this->agents->size(); ++i) {
            Agent *agent = (*this->agents)[i];
            if ((agent->getOffer() == 6 || agent->getOffer() == 0) && agent->getMission() != 0 &&
                agent->getMission()->getType() == 12 && agent->hasAcceptedOffer()) {
                ArrayRemove((*this->agents)[i], *this->agents);
                this->init();
                break;
            }
        }
    }

    if (this->cutScene == 0) {
        this->cutScene = new CutScene(4);
    }
    while (!this->cutScene->isInitialized()) {
        this->cutScene->initialize();
    }

    // Turn every agent (and its seat partner) towards the camera.
    Array<KIPlayer *> *figures = this->cutScene->level->getEnemies();
    if (this->agents != 0) {
        for (unsigned i = 0; i < this->agents->size(); ++i) {
            Vector pos = (*figures)[i]->getPosition();
            PaintCanvas *canvas = Globals::Canvas;
            Matrix look = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
            look = AbyssEngine::AEMath::MatrixGetLookAt(AbyssEngine::AEMath::MatrixGetPosition(look), pos,
                                                        AbyssEngine::AEMath::MatrixGetUp(look));
            (*figures)[i]->geometry->setMatrix(look);
            (*figures)[i]->setPosition(pos);
            (*figures)[i + this->agents->size()]->geometry->setMatrix(look);
            pos.x += AbyssEngine::AEMath::MatrixGetDir(look).x * -100.0f;
            pos.z += AbyssEngine::AEMath::MatrixGetDir(look).z * -100.0f;
            (*figures)[i + this->agents->size()]->setPosition(pos);
        }
    }

    Matrix from;
    int race = ((SolarSystem *) Globals::status->getSystem())->getRace();
    AbyssEngine::AEMath::MatrixSetTranslation(from, (float) SpaceLounge_cameraStartPos[race * 3],
                                              (float) SpaceLounge_cameraStartPos[race * 3 + 1],
                                              (float) SpaceLounge_cameraStartPos[race * 3 + 2]);
    AbyssEngine::AEMath::MatrixSetRotation(from, 0.0f, SpaceLounge_cameraStartRot[race], 0.0f);
    Matrix to;
    AbyssEngine::AEMath::MatrixSetTranslation(to, (float) SpaceLounge_cameraEndPos[race * 3],
                                              (float) SpaceLounge_cameraEndPos[race * 3 + 1],
                                              (float) SpaceLounge_cameraEndPos[race * 3 + 2]);
    AbyssEngine::AEMath::MatrixSetRotation(to, 0.0f, SpaceLounge_cameraEndRot[race], 0.0f);
    this->cameraEase = new AbyssEngine::EaseInOutMatrix(from, to, 3000);

    PaintCanvas *canvas = Globals::Canvas;
    canvas->CameraSetLocal(canvas->CameraGetCurrent(), from);
    this->introDone = 0;
    this->initialized = 1;
}

void SpaceLounge::startChat() {
    String title;
    String body;
    String left;
    String right;

    if (this->agents == 0 || this->selectedAgent < 0) {
        return;
    }

    Agent *agent = (*this->agents)[this->selectedAgent];
    int offer = agent->getOffer();
    Mission *mission = agent->getMission();
    GameText **texts = SpaceLounge_start_text_slot;

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

    title.Set((*texts)->getText(titleId)->data);
    body.Set((*texts)->getText(bodyId)->data);
    left.Set((*texts)->getText(0x10)->data);
    right.Set((*texts)->getText(0x11)->data);

    this->choiceWindow->set(title, body, true);
    if (this->choiceWindow->leftButton != nullptr)
        this->choiceWindow->leftButton->setText(left);
    if (this->choiceWindow->rightButton != nullptr)
        this->choiceWindow->rightButton->setText(right);

    if (agent->isKnown() == 0 && agent->isStoryAgent() == 0) {
        if (agent->eventCount <= 0)
            agent->eventCount = 1;
    }
    this->getSoundId(agent);

    this->choiceVisible = 1;
    this->chatActive = 1;
    this->mode = 1;

    if (offer == 1) {
        this->mode = 3;
    } else if (agent->isGenericAgent() != 0) {
        agent->setEvent(1);
    }
}

SpaceLounge::~SpaceLounge() {
    delete this->choiceWindow;
    this->choiceWindow = 0;

    delete this->cutScene;
    this->cutScene = 0;

    delete[] this->agentVisited;
    this->agentVisited = 0;

    if (this->agentTexts != 0) {
        ArrayReleaseClasses(*this->agentTexts);
        delete this->agentTexts;
    }
    this->agentTexts = 0;

    if (this->buttons != 0) {
        ArrayReleaseClasses(*this->buttons);
        delete this->buttons;
    }
    this->buttons = 0;

    if (this->agentImageParts != 0) {
        ArrayReleaseClasses(*this->agentImageParts);
        delete this->agentImageParts;
    }
    this->agentImageParts = 0;

    if (this->silhouetteGrid != 0) {
        for (unsigned i = 0; i < this->silhouetteGrid->size(); ++i) {
            ArrayReleaseClasses(*(*this->silhouetteGrid)[i]);
            delete (*this->silhouetteGrid)[i];
            (*this->silhouetteGrid)[i] = 0;
        }
        ArrayReleaseClasses(*this->silhouetteGrid);
        delete this->silhouetteGrid;
    }
    this->silhouetteGrid = 0;

    if (this->agentRects != 0) {
        ArrayReleaseClasses(*this->agentRects);
        delete this->agentRects;
    }
    this->agentRects = 0;

    delete this->cameraEase;
    this->cameraEase = 0;

    delete this->headEase;
    this->headEase = 0;
}

void SpaceLounge::draw() {
    if (this->listVisible != 0) {
        if (this->listWindow->shows3DShip()) {
            Globals::Canvas->SetColor(0, 0, 0, 255);
            Globals::Canvas->FillRectangle(0, 0, Globals::Canvas->GetWidth(), Globals::Canvas->GetHeight());
            Globals::Canvas->SetColor(0xffffffffu);
        }
        this->listWindow->draw();
        Globals::layout->drawFooter();
        return;
    }

    if (this->mapVisible != 0) {
        this->starMap->draw();
        return;
    }

    Globals::layout->drawHeader(String(*Globals::gameText->getText(398), false));
    this->drawLounge();

    if ((this->mode & ~1) == 2) {
        Globals::layout->drawFooterNoBackButton();
    } else {
        Globals::layout->drawFooter();
    }

    if (this->chatActive != 0 || this->popupActive != 0 || this->choiceVisible != 0) {
        this->choiceWindow->draw();
    }
}

#define SPACELOUNGE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SPACELOUNGE_MAX(a, b) ((a) < (b) ? (b) : (a))
#define SPACELOUNGE_ABS(x) ((x) > 0.0f ? (x) : -(x))

void SpaceLounge::update(int dt) {
    using namespace AbyssEngine::AEMath;
    if (dt >= 50) {
        dt = 50;
    }
    if (this->initialized == 0) {
        return;
    }
    if (this->mapVisible != 0) {
        this->starMap->update(dt);
        return;
    }
    if (this->listVisible != 0) {
        this->listWindow->update(dt);
        return;
    }

    if (this->cameraAnimating == 0) {
        this->cameraEase->Increase((float) dt);
    }
    if (this->introDone != 0 || this->cameraEase->GetMaxValue() == this->cameraEase->GetValue()) {
        // Idle: gentle head bob of the camera.
        float step = SPACELOUNGE_MAX(SPACELOUNGE_MIN((float) dt * 0.0025f, 0.12f), 0.05f);
        this->introDone = 1;
        this->headBobPhase = step + this->headBobPhase;
        float wave = Sinf(this->headBobPhase);
        float amplitude = (float) this->headBobSteps * 0.85f;
        amplitude = amplitude < 3.5f ? amplitude : 3.5f;
        if (this->cameraAnimating != 0) {
            float turn = this->headEase->GetValue();
            if (SPACELOUNGE_ABS(turn - this->headEase->GetMaxValue()) < 0.25f) {
                float target = (float) (5 - Globals::rnd->nextInt(10));
                this->headBobReverse = turn > target;
                this->headEase->SetRange(turn, target);
                this->headBobSteps = Globals::rnd->nextInt(4) + 1;
            }
            int steps = this->headBobSteps;
            this->headEase->Increase((float) ((this->headBobReverse ? -steps : steps) * dt));
        } else {
            PaintCanvas *canvas = Globals::Canvas;
            this->baseMatrix = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
            this->cameraAnimating = 1;
            Globals::rnd->nextInt(10);
            this->headBobReverse = 0;
            if (this->headEase != 0) {
                this->headEase->SetRange(0.0f, 5.0f);
            } else {
                this->headEase = new AbyssEngine::EaseInOut(0.0f, 5.0f);
            }
            this->headBobSteps = 2;
        }
        float bob = wave * amplitude;
        Matrix view = MatrixSetRotation(view, 0.0f, this->headEase->GetValue() / 35.0f, 0.0f);
        MatrixSetTranslation(view, 0.0f, bob, 0.0f);
        view *= this->baseMatrix;
        PaintCanvas *canvas = Globals::Canvas;
        canvas->CameraSetLocal(canvas->CameraGetCurrent(), view);
    } else {
        this->cameraAnimating = 0;
        PaintCanvas *canvas = Globals::Canvas;
        canvas->CameraSetLocal(canvas->CameraGetCurrent(), this->cameraEase->GetValue());
    }

    this->updateScreenPositions();
    if (this->mode != 0) {
        this->scrollWindow->update(dt);
    }
    this->cutScene->update();

    if (this->touchDown != 0) {
        // Debug-style free camera while a finger rests on the screen edges.
        Layout *layout = Globals::layout;
        if (this->touchY < layout->field_0xc_leftMargin) {
            if (this->touchX <= 149) {
                this->cutScene->vec8.y += -10.0f;
            } else if (this->touchX > Globals::w - 150) {
                this->cutScene->vec8.y += 10.0f;
            }
        } else if (this->touchY < Globals::h - layout->field_0x10_rightMargin * 2) {
            if (this->touchX <= 69) {
                this->cutScene->cameraRotX += 0.01f;
            } else if (this->touchX > Globals::w - 70) {
                this->cutScene->cameraRotX += -0.01f;
            } else if (this->touchX >= 101 && this->touchX < Globals::w - 100) {
                if (this->touchY < Globals::h / 2) {
                    this->cutScene->vec8.z -= (float) dt;
                } else {
                    this->cutScene->vec8.z += (float) dt;
                }
            }
        } else {
            if (this->touchX <= 69) {
                this->cutScene->vec8.x -= (float) dt;
            } else if (this->touchX > Globals::w - 70) {
                this->cutScene->vec8.x += (float) dt;
            }
        }
    }
}

void SpaceLounge::refresh() {
}
