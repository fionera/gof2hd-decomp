
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
#include "game/menu/ModStation.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/Engine.h"
#include "game/mission/BluePrint.h"
#include "game/mission/Achievements.h"
#include "game/world/Station.h"
#include "game/world/Galaxy.h"
#include "game/ship/Ship.h"
#include "game/mission/Item.h"
#include "game/ui/ListItem.h"
#include "game/mission/Generator.h"
#include "engine/audio/FModSound.h"
#include "game/world/Standing.h"

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

static Level *SpaceLounge_screen_level_slot;
static PaintCanvas **SpaceLounge_screen_canvas_slot;
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

static const int SpaceLounge_cameraPositions[12] = {-1800, 620, 1805, 607, 610, 2362, 136, 444, 1122, 2222, 450, 1227};
static const float SpaceLounge_cameraRotations[4] = {0.04f, 0.43f, 0.6f, 0.46f};

static inline Agent *key_agent(SpaceLounge *self) {
    return (*self->agents)[self->selectedAgent];
}

static inline TouchButton *button_at(SpaceLounge *self, unsigned i) {
    return (*self->buttons)[i];
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

#define SPACELOUNGE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SPACELOUNGE_MAX(a, b) ((a) < (b) ? (b) : (a))
#define SPACELOUNGE_ABS(x) ((x) > 0.0f ? (x) : -(x))

SpaceLounge::SpaceLounge()
        : silhouettePos(), agentTopLeft(), agentBottomRight() {
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
    bool found = false;
    int foundIndex = 0;
    if (this->agents != 0) {
        for (unsigned i = 0; i < this->agents->size(); ++i) {
            Agent *agent = (*this->agents)[i];
            if ((agent->getOffer() == 6 || agent->getOffer() == 0) && agent->getMission() != 0 &&
                agent->getMission()->getType() == 12 && agent->hasAcceptedOffer()) {
                found = true;
                foundIndex = i;
                break;
            }
        }
    }
    if (found) {
        ArrayRemove((*this->agents)[foundIndex], *this->agents);
        this->init();
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
    PaintCanvas *canvas;
    this->cameraEase = new AbyssEngine::EaseInOutMatrix(from, to, 3000);

    canvas = Globals::Canvas;
    canvas->CameraSetLocal(canvas->CameraGetCurrent(), from);
    this->introDone = 0;
    this->initialized = 1;
}

void SpaceLounge::init() {
    this->initialized = 0;
    this->touchDown = 0;
    this->cameraAnimating = 0;
    this->agents = Globals::status->getStation()->getAgents();

    if (this->silhouetteGrid == 0 && this->agentRects == 0 && this->agents != 0) {
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
    char *visited = new char[21];
    this->agentVisited = visited;
    int i = 0;
    for (; i != 21; ++i) {
        visited[i] = 0;
    }

    // Agent list panel + buttons.
    this->panelX = Globals::w / 2 - Globals::layout->field_0x68 / 2;
    this->panelY = Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top;
    this->panelWidth = Globals::layout->field_0x68 - Globals::layout->field_0x4c * 3 - Globals::layout->field_0x2d4;
    this->scrollWindow = new ScrollTouchWindow(this->panelX + Globals::layout->field_0x4c * 2 + Globals::layout->field_0x2d4, this->panelY,
                                               this->panelWidth, Globals::layout->field_0x6c, false);
    this->buttonPanelY = this->panelY + Globals::layout->field_0x6c + Globals::layout->field_0x2c_rowHeight;
    this->mode = 0;
    this->buttonsHeight = (Globals::layout->field_0x34 + Globals::layout->field_0x30_rowHeight) * 5;

    if (this->buttons != 0) {
        ArrayReleaseClasses(*this->buttons);
        delete this->buttons;
    }
    this->buttons = 0;
    this->buttons = new Array<TouchButton *>();
    ArraySetLength(5, *this->buttons);

    this->buttonX = this->panelX + Globals::layout->field_0x68 - Globals::layout->field_0x4c - this->panelWidth;
    int y = this->buttonPanelY + Globals::layout->field_0x4c;
    int rowHeight = Globals::layout->field_0x30_rowHeight;
    int rowGap = Globals::layout->field_0x34;
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
    this->buttonY1 = y + Globals::layout->field_0x2d8 / 2 - Globals::layout->field_0x30_rowHeight / 2;
    this->buttonY0 = y;
    this->hoverAgent = -1;

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
    this->headBobPhase = 0.0f;
    this->introDone = 1;
    if (this->cutScene != 0) {
        this->cutScene->level->getStarSystem()->initLight();
    }
    ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(0.5f, 0.5f, 0.5f, 0x4000);

    if (Globals::iPad) {
        // iPad: place the agent panel next to the selected agent.
        float margin = Globals::iPadHD ? 14.0f : (Globals::iPadLarge ? 20.0f : 10.0f);
        if (this->agentRects != 0) {
            unsigned int agentIndex = (unsigned int) (this->selectedAgent * 2);
            if (this->selectedAgent >= 0 && agentIndex < this->agentRects->size()) {
                Vector *rect = (*this->agentRects)[agentIndex];
                float x;
                if (rect->x - margin - (float) Globals::layout->field_0x68 < (float) Globals::layout->buttonInsetX) {
                    x = SPACELOUNGE_MIN(this->agentBottomRight.x + margin,
                                         (float) (Globals::w - Globals::layout->buttonInsetX));
                } else {
                    x = SPACELOUNGE_MAX(this->agentTopLeft.x - margin - (float) Globals::layout->field_0x68,
                                         (float) Globals::layout->buttonInsetX);
                }
                this->panelX = (int) x;
                float bottom = (float) (Globals::h - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 - Globals::layout->field_0x6c);
                float top = this->agentBottomRight.y - (float) (Globals::layout->field_0x6c / 3);
                float minTop = (float) (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top);
                if (top < bottom) {
                    bottom = top;
                }
                if (bottom < minTop) {
                    bottom = minTop;
                }
                this->panelY = (int) bottom;
                delete this->scrollWindow;
                this->scrollWindow = 0;
                this->scrollWindow = new ScrollTouchWindow(this->panelX + Globals::layout->field_0x4c * 2 + Globals::layout->field_0x2d4,
                                                           this->panelY, this->panelWidth, Globals::layout->field_0x6c, false);
                this->buttonPanelY = this->panelY + Globals::layout->field_0x6c + Globals::layout->field_0x2c_rowHeight;
                this->buttonX = this->panelX + Globals::layout->field_0x68 - Globals::layout->field_0x4c - this->panelWidth;
                int y0 = Globals::layout->field_0x4c + this->buttonPanelY;
                float gap = (float) Globals::layout->field_0x34;
                float extra = Globals::iPadHD ? 15.46875f : (Globals::iPadLarge ? 22.0f : 11.0f);
                int spacing = (int) (extra + gap);
                this->buttonY1 = y0 - Globals::layout->field_0x30_rowHeight / 2 + Globals::layout->field_0x2d8 / 2;
                this->buttonY0 = y0;
                int y1 = Globals::layout->field_0x30_rowHeight + y0 + spacing;
                (*this->buttons)[2]->setPosition(this->buttonX, y1);
                (*this->buttons)[3]->setPosition(this->buttonX, y1);
                (*this->buttons)[4]->setPosition(this->buttonX, y1 + spacing + Globals::layout->field_0x30_rowHeight);
            }
        }
    }

    this->postEffectWasActive = ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->IsPostEffectActivated();
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
}

bool SpaceLounge::introFinished() {
    return this->introDone;
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

void SpaceLounge::setHangarUpdate(bool value) {
    this->hangarUpdate = value;
}

// Mod sale offers: {mod, bonus percentage shown in the pitch}.
static const int SpaceLounge_modBonuses[4][2] = {{0, 40}, {1, 30}, {2, 1}, {3, 20}};

void SpaceLounge::startChat() {
    Agent *agent = (*this->agents)[this->selectedAgent];
    if (Globals::iPad) {
        // iPad: move the chat panel next to the agent.
        int margin = (int) (Globals::iPadHD ? 14.0625f : (Globals::iPadLarge ? 20.0f : 10.0f));
        float x;
        if ((*this->agentRects)[this->selectedAgent * 2]->x - (float) margin - (float) Globals::layout->field_0x68 <
            (float) Globals::layout->buttonInsetX) {
            // Right of the agent, limited to the screen edge (the limit is evaluated first).
            x = (float) (Globals::w - Globals::layout->buttonInsetX) >
                        (*this->agentRects)[this->selectedAgent * 2 + 1]->x + (float) margin
                    ? (*this->agentRects)[this->selectedAgent * 2 + 1]->x + (float) margin
                    : (float) (Globals::w - Globals::layout->buttonInsetX);
        } else {
            x = (*this->agentRects)[this->selectedAgent * 2]->x - (float) margin - (float) Globals::layout->field_0x68;
        }
        this->panelX = (int) x;
        float bottom = (float) (Globals::h - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
                                Globals::layout->field_0x6c);
        float top = (*this->agentRects)[this->selectedAgent * 2 + 1]->y - (float) (Globals::layout->field_0x6c / 3);
        float minTop = (float) (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top);
        float chosen = top < bottom ? top : bottom;
        float resultTop = top;
        float resultBottom = bottom;
        if (chosen < minTop) {
            resultTop = minTop;
            resultBottom = resultTop;
        }
        if (top < bottom)
            resultTop = resultBottom;
        this->panelY = (int) resultTop;
        delete this->scrollWindow;
        this->scrollWindow = 0;
        this->scrollWindow = new ScrollTouchWindow(
            this->panelX + Globals::layout->field_0x4c * 2 + Globals::layout->field_0x2d4, this->panelY,
            this->panelWidth, Globals::layout->field_0x6c, false);
        this->buttonPanelY = this->panelY + Globals::layout->field_0x6c + Globals::layout->field_0x2c_rowHeight;
        this->buttonX = this->panelX + Globals::layout->field_0x68 - Globals::layout->field_0x4c - this->panelWidth;
        int y0 = Globals::layout->field_0x4c + this->buttonPanelY;
        const float gap = (float) Globals::layout->field_0x34;
        float extra = Globals::iPadHD ? 0.0f : (Globals::iPadLarge ? 0.0f : 11.0f);
        int spacing = (int) (gap + extra);
        this->buttonY1 = y0 - Globals::layout->field_0x30_rowHeight / 2 + Globals::layout->field_0x2d8 / 2;
        this->buttonY0 = y0;
        int y1 = y0 + Globals::layout->field_0x30_rowHeight + spacing;
        (*this->buttons)[2]->setPosition(this->buttonX, y1);
        (*this->buttons)[3]->setPosition(this->buttonX, y1);
        (*this->buttons)[4]->setPosition(this->buttonX, y1 + spacing + Globals::layout->field_0x30_rowHeight);
        {
            Vector *rect = (*this->agentRects)[this->selectedAgent * 2];
            float x = rect->x;
            float y = rect->y;
            float z = rect->z;
            Vector corner = {x, y, z};
            this->agentTopLeft = corner;
        }
        {
            Vector *rect = (*this->agentRects)[this->selectedAgent * 2 + 1];
            float x = rect->x;
            float y = rect->y;
            float z = rect->z;
            Vector corner = {x, y, z};
            this->agentBottomRight = corner;
        }
    }

    int offer = agent->getOffer();
    if (agent->isGenericAgent()) {
        if (agent->isKnown() && agent->getOffer() != 7) {
            if (agent->hasAcceptedOffer()) {
                String message = *Globals::gameText->getText(
                    agent->getOffer() == 5
                        ? 857
                        : (agent->getOffer() != 6 && (agent->getMission() == 0 || agent->getMission()->getType() != 12) ? 858
                                                                                                                       : 859));
                this->scrollWindow->setText(agent->getName(), message);
                this->chatScroll = 0;
                this->mode = 3;
                int sound = this->getSoundId(agent);
                if (sound >= 0)
                    Globals::sound->play(sound, 0, 0, 0.0f);
                return;
            }
            if (agent->getOffer() == 0 && agent->getMission() != 0 && agent->getMission()->getType() != 8 &&
                agent->getMission()->getType() != 12) {
                // Standing bonus on the reward.
                float bonus = ((Standing *) (intptr_t) Globals::status->getStanding())->getMissionBonus(agent->getRace());
                int bonusCredits = (int) (bonus * (float) agent->getMission()->getReward());
                int rest = bonusCredits % 50;
                int rounded = rest + bonusCredits;
                if (rounded % 50 != 0)
                    rounded = bonusCredits - rest;
                agent->getMission()->setBonus(rounded);
                this->title = bonus > 0.0f ? String(" ") +
                                                 Globals::status->replaceHash(*Globals::gameText->getText(767),
                                                                              String(String((int) (bonus * 100.0f)), false),
                                                                              String("#P")) +
                                                 String(" ")
                                           : String("");
                String message = Globals::globals->getAgentMissionText(agent);
                message = Globals::status->replaceHash(
                    message,
                    String(Globals::layout->formatCredits(agent->getMission()->getReward() + agent->getMission()->getBonus()),
                           false) +
                        this->title,
                    String("#C"));
                this->scrollWindow->setText(agent->getName(), message);
            } else {
                String message = Globals::globals->getAgentMissionText(agent);
                this->scrollWindow->setText(agent->getName(), message);
            }
        } else {
            // First talk with a generic agent.
            Globals::status->field_d0++;
            bool smallTalk = offer != 1 && offer != 7;
            bool missionTalk = offer == 0 || offer == 5;
            String intro = smallTalk ? *Globals::gameText->getText(750 + Globals::rnd->nextInt(6)) : String("");
            String greeting = smallTalk ? *Globals::gameText->getText(756 + Globals::rnd->nextInt(2)) : String("");
            String hint = missionTalk ? *Globals::gameText->getText(758 + Globals::rnd->nextInt(6)) : String("");
            String body("");
            greeting = Globals::status->replaceHash(greeting, String(agent->getName(), false), String("#N"));
            Generator *generator = new Generator();
            switch (agent->getOffer()) {
            case 7: {
                // Standing rehabilitation.
                int race = agent->getRace();
                int standing = ((Standing *) (intptr_t) Globals::status->getStanding())
                                   ->getStanding(race != 2 && race != 3 ? 0 : 1);
                if (((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(race)) {
                    int costs = (int) ((float) (standing < 0 ? -standing : standing) / 100.0f * 16000.0f);
                    int textId = race == 2 ? 878 : race == 3 ? 880 : race == 0 ? 881 : 882;
                    body = *Globals::gameText->getText(textId);
                    body = Globals::status->replaceHash(body, String(Globals::layout->formatCredits(costs), false), String("#C"));
                    agent->setCosts(costs);
                    break;
                }
                body = *Globals::gameText->getText(883);
                this->scrollWindow->setText(agent->getName(), body);
                this->chatScroll = 0;
                this->mode = 3;
                return;
            }
            case 5:
                // Freelance job.
                agent->setMission(generator->createMission(agent, Globals::galaxy->getSystems()));
                agent->field_0x28 = 777 + Globals::rnd->nextInt(2);
                body += *Globals::gameText->getText(agent->field_0x28);
                body = Globals::status->replaceHash(
                    body, String(String(agent->getMission()->getProductionGoodAmount()), false), String("#Q"));
                body = Globals::status->replaceHash(
                    body, *Globals::gameText->getText(1274 + agent->getMission()->getProductionGoodIndex()), String("#P"));
                body = Globals::status->replaceHash(
                    body, String(Globals::layout->formatCredits(agent->getMission()->getReward()), false), String("#C"));
                break;
            case 1: {
                // Ore rumours.
                int ore;
                do {
                    ore = Globals::rnd->nextInt(21);
                } while (this->agentVisited[ore]);
                this->agentVisited[ore] = 1;
                if (agent->getRace() != 0 && ore == 16)
                    ore = 4;
                agent->field_0x28 = !agent->isMale() && ore == 13 ? 824 : 820 + ore;
                body = *Globals::gameText->getText(agent->field_0x28);
                body = Globals::status->replaceHash(body, String(Globals::globals->getRandomPlanetName(), false), String("#S"));
                body = Globals::status->replaceHash(body, String(agent->getName(), false), String("#N"));
                body = Globals::status->replaceHash(body, *Globals::gameText->getText(1428 + Globals::rnd->nextInt(10)),
                                                    String("#ORE"));
                break;
            }
            case 0: {
                Mission *mission = agent->getMission();
                body += *Globals::gameText->getText(786 + mission->getType());
                if (mission->getType() == 5 || mission->getType() == 3)
                    body += String(" ") + *Globals::gameText->getText(802);
                if (mission->getType() == 15)
                    body = Globals::status->replaceHash(
                        body, *Globals::gameText->getText(1274 + mission->getProductionGoodIndex()), String("#P"));
                else
                    body = Globals::status->replaceHash(
                        body, *Globals::gameText->getText(813 + mission->getProductionGoodIndex()), String("#P"));
                body = Globals::status->replaceHash(body, String(String(mission->getProductionGoodAmount()), false),
                                                    String("#Q"));
                if (mission->getType() == 14)
                    body = Globals::status->replaceHash(body, String(mission->getTargetSystemName(), false), String("#S"));
                else
                    body = Globals::status->replaceHash(body, String(mission->getTargetStationName(), false), String("#S"));
                body = Globals::status->replaceHash(body, String(mission->getTargetName(), false), String("#N"));
                agent->field_0x28 = 764 + Globals::rnd->nextInt(3);
                body += String("\n") + *Globals::gameText->getText(agent->field_0x28);
                float bonus = ((Standing *) (intptr_t) Globals::status->getStanding())->getMissionBonus(agent->getRace());
                this->title = bonus > 0.0f ? String(" ") +
                                                 Globals::status->replaceHash(*Globals::gameText->getText(767),
                                                                              String(String((int) (bonus * 100.0f)), false),
                                                                              String("#P")) +
                                                 String(" ")
                                           : String("");
                agent->setMissionString(body);
                int bonusCredits = (int) (bonus * (float) agent->getMission()->getReward());
                int rest = bonusCredits % 50;
                agent->getMission()->setBonus((bonusCredits + rest) % 50 != 0 ? bonusCredits - rest : bonusCredits + rest);
                body = Globals::status->replaceHash(
                    body, String(Globals::layout->formatCredits(mission->getReward() + mission->getBonus()), false) + this->title,
                    String("#C"));
                break;
            }
            case 2:
                // Goods.
                agent->field_0x28 = 768 + Globals::rnd->nextInt(5);
                agent->field_0x2c = 773 + Globals::rnd->nextInt(2);
                if (Globals::status->getCurrentCampaignMission() == 24 && Globals::status->getStation()->getIndex() == 10 &&
                    agent->getSellItemIndex() == 68) {
                    agent->field_0x28 = 193;
                    agent->field_0x2c = 194;
                }
                body += *Globals::gameText->getText(agent->field_0x28);
                body += String("\n") + *Globals::gameText->getText(agent->field_0x2c);
                body = Globals::status->replaceHash(body, String(String(agent->getSellItemQuantity()), false), String("#Q"));
                body = Globals::status->replaceHash(body, *Globals::gameText->getText(1274 + agent->getSellItemIndex()),
                                                    String("#P"));
                body = Globals::status->replaceHash(
                    body, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false), String("#C"));
                if (agent->getSellItemQuantity() > 1) {
                    body += String(" ") + *Globals::gameText->getText(775);
                    body = Globals::status->replaceHash(
                        body,
                        String(Globals::layout->formatCredits(agent->getSellItemPrice() / agent->getSellItemQuantity()), false),
                        String("#C"));
                }
                break;
            case 6:
                // Wingmen.
                body += *Globals::gameText->getText((Globals::achievements->gotAllMedals() ? 782 : 779) +
                                                    agent->getWingmanFriendsCount());
                body = Globals::status->replaceHash(body, String(Globals::layout->formatCredits(agent->getCosts()), false),
                                                    String("#C"));
                if (agent->getWingmanFriendsCount() > 0)
                    body = Globals::status->replaceHash(body, String(agent->getWingmanName(1), false), String("#W"));
                break;
            }
            delete generator;
            if (agent->getOffer() != 0 || agent->getMission()->getType() == 8)
                agent->setMissionString(body);
            if (agent->getOffer() == 1) {
                this->scrollWindow->setText(agent->getName(), body);
                agent->setEvent(1);
            } else if (agent->getOffer() == 7) {
                this->scrollWindow->setText(agent->getName(), body);
            } else if (agent->getMission() != 0 && agent->getMission()->getType() == 12) {
                body = *Globals::gameText->getText(798);
                body = Globals::status->replaceHash(
                    body,
                    String(Globals::layout->formatCredits(agent->getMission()->getReward() + agent->getMission()->getBonus()),
                           false),
                    String("#C"));
                agent->setMissionString(body);
                String closing = String("\n") + *Globals::gameText->getText(841 + Globals::rnd->nextInt(3));
                this->scrollWindow->setText(agent->getName(), body + closing);
            } else {
                body = String("\n") + body;
                String closing = String("\n") + *Globals::gameText->getText(841 + Globals::rnd->nextInt(3));
                this->scrollWindow->setText(agent->getName(),
                                            intro + String(" ") + greeting + String(" ") + hint + body + closing);
            }
        }
    } else {
        // Story agents.
        String message;
        if (agent->getEvent() > 0) {
            message = *Globals::gameText->getText(858);
        } else if (agent->hasAcceptedOffer()) {
            message = *Globals::gameText->getText(858);
            int sound = this->getSoundId(agent);
            if (sound >= 0)
                Globals::sound->play(sound, 0, 0, 0.0f);
        } else {
            Globals::status->field_d0++;
            if (agent->getOffer() == 8) {
                // Ship mod.
                float factor = Globals::options[56] ? 1.0f : 2.0f;
                if (Globals::status->getShip()->hasModInstalled(agent->getSellModIndex())) {
                    message = *Globals::gameText->getText(858);
                    Globals::sound->play(this->getSpecificSoundForRace(781 + Globals::rnd->nextInt(2), agent->getRace(),
                                                                       agent->isMale()),
                                         0, 0, 0.0f);
                    Globals::status->field_d0--;
                    this->scrollWindow->setText(agent->getName(), message);
                    this->chatScroll = 0;
                    this->mode = 3;
                    return;
                }
                agent->field_0x28 = 874 + Globals::rnd->nextInt(2);
                int shipPrice = Globals::status->getShip()->getPrice();
                agent->setSellItemPrice(shipPrice * agent->getModPricePercentage() / 100);
                message = *Globals::gameText->getText(agent->field_0x28);
                message += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                message += String(" ") + *Globals::gameText->getText(879);
                message = Globals::status->replaceHash(
                    message, *Globals::gameText->getText(913 + Globals::status->getShip()->getIndex()),
                    String("#SHIP_NAME"));
                message = Globals::status->replaceHash(
                    message, String(String(SpaceLounge_modBonuses[agent->getSellModIndex()][1]), false), String("#N"));
                message = Globals::status->replaceHash(
                    message, String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())), false),
                    String("#C"));
            } else if (agent->getOffer() == 9) {
                // Premium item.
                float factor = Globals::options[56] ? 1.0f : 2.0f;
                if (agent->field_0x28 == -1)
                    agent->field_0x28 = 874 + Globals::rnd->nextInt(2);
                message = *Globals::gameText->getText(agent->field_0x28);
                message += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                int detailText = agent->field_0x2c;
                if (detailText == -1)
                    detailText = 773 + Globals::rnd->nextInt(2);
                agent->field_0x2c = detailText;
                message += String("\n") + *Globals::gameText->getText(detailText);
                message = Globals::status->replaceHash(message, String(String(agent->getSellItemQuantity()), false),
                                                       String("#Q"));
                message = Globals::status->replaceHash(
                    message, *Globals::gameText->getText(1274 + agent->getSellItemIndex()), String("#P"));
                message = Globals::status->replaceHash(
                    message, String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())), false),
                    String("#C"));
                if (agent->getSellItemQuantity() > 1) {
                    message += String(" ") + *Globals::gameText->getText(775);
                    message = Globals::status->replaceHash(
                        message,
                        String(Globals::layout->formatCredits(
                                   (int) (factor * (float) (agent->getSellItemPrice() / agent->getSellItemQuantity()))),
                               false),
                        String("#C"));
                }
            } else if (agent->getOffer() == 10) {
                // Ship.
                if (Globals::status->field_114 < 3) {
                    message = *Globals::gameText->getText(750 + Globals::rnd->nextInt(6));
                    Globals::sound->play(this->getSpecificSoundForRace(781 + Globals::rnd->nextInt(2), agent->getRace(),
                                                                       agent->isMale()),
                                         0, 0, 0.0f);
                    Globals::status->field_d0--;
                    this->scrollWindow->setText(agent->getName(), message);
                    this->chatScroll = 0;
                    this->mode = 3;
                    return;
                }
                float factor = Globals::options[56] ? 1.0f : 2.0f;
                if (agent->field_0x28 == -1)
                    agent->field_0x28 = 874 + Globals::rnd->nextInt(2);
                message = *Globals::gameText->getText(agent->field_0x28);
                message += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                if (agent->field_0x2c == -1)
                    agent->field_0x2c = 773 + Globals::rnd->nextInt(2);
                int detailText = agent->field_0x2c;
                message += String("\n") + *Globals::gameText->getText(detailText);
                message = Globals::status->replaceHash(message, String(String(1), false), String("#Q"));
                message = Globals::status->replaceHash(
                    message, *Globals::gameText->getText(913 + agent->getSellItemIndex()), String("#P"));
                message = Globals::status->replaceHash(
                    message, String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())), false),
                    String("#C"));
            } else if (agent->getOffer() == 4) {
                // System coordinates.
                agent->field_0x28 = 874 + Globals::rnd->nextInt(2);
                message = *Globals::gameText->getText(agent->field_0x28);
                message += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                message += String(" ") + *Globals::gameText->getText(877);
                SolarSystem *system = (SolarSystem *) Globals::galaxy->getSystem(agent->getSellSystemIndex());
                message = Globals::status->replaceHash(message, String(system->getName(), false), String("#S"));
                message = Globals::status->replaceHash(
                    message, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false), String("#C"));
            } else {
                // Blueprint.
                agent->field_0x28 = 874 + Globals::rnd->nextInt(2);
                message = *Globals::gameText->getText(agent->field_0x28);
                message += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                message += String(" ") + *Globals::gameText->getText(876);
                message = Globals::status->replaceHash(
                    message, *Globals::gameText->getText(1274 + (*Globals::items)[agent->getSellBlueprintIndex()]->getIndex()),
                    String("#N"));
                message = Globals::status->replaceHash(
                    message, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false), String("#C"));
            }
            message += String("\n") + *Globals::gameText->getText(841 + Globals::rnd->nextInt(3));
            this->scrollWindow->setText(agent->getName(), message);
            (*this->agents)[this->selectedAgent]->setMissionString(message);
            this->chatScroll = 0;
            this->mode = 2;
            int sound = this->getSoundId(agent);
            if (sound >= 0)
                Globals::sound->play(sound, 0, 0, 0.0f);
            return;
        }
        this->scrollWindow->setText(agent->getName(), message);
        this->chatScroll = 0;
        this->mode = 3;
        return;
    }

    agent->setStationName(Globals::status->getStation()->getName());
    agent->setSystemName(((SolarSystem *) Globals::status->getSystem())->getName());
    this->mode = 1;
    this->chatScroll = 0;
    this->onKeyPress(0x10000);
    int sound = this->getSoundId(agent);
    if (sound >= 0)
        Globals::sound->play(sound, 0, 0, 0.0f);
}

void SpaceLounge::refresh() {
}

int SpaceLounge::onKeyPress(int key) {
    if (this->choiceVisible) {
        if (key == 0x10000)
            this->choiceVisible = 0;
        else if (key == 0x2000)
            this->choiceWindow->right();
        else if (key == 0x1000)
            this->choiceWindow->left();
        return 1;
    }

    switch (this->mode) {
    case 0:
        // Agent selection.
        if (key == 0x4000 || key == 0x2000) {
            this->selectedAgent--;
            if (this->selectedAgent < 0)
                this->selectedAgent = this->agents->size() - 1;
        } else if (key == 0x10000 || key == 0x20000) {
            this->startChat();
        } else if (key == 0x8000 || key == 0x1000) {
            this->selectedAgent++;
            if ((unsigned) this->selectedAgent >= this->agents->size())
                this->selectedAgent = 0;
        } else if (key == 0x40000) {
            return 0;
        }
        break;
    case 1:
    case 3:
        if (key == 0x40000) {
            if (this->chatScroll >= 3) {
                this->chatScroll -= 40;
            } else if (this->mode == 1) {
                this->mode = 0;
                this->chatScroll = 0;
            }
        } else if (key == 0x10000 || key == 0x20000) {
            this->chatAnswer = 0;
            Agent *agent = key_agent(this);
            if (agent->getOffer() == 1) {
                this->mode = 2;
            } else {
                this->mode = this->mode == 1 ? 2 : 0;
                if (this->mode == 0 && this->field_0x18) {
                    // Show the system whose coordinates were just bought.
                    this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
                    if (this->starMap != 0) {
                        this->starMap->init(false, 0, true, agent->getSellSystemIndex());
                    } else {
                        StarMap *map = new StarMap(false, 0, true, agent->getSellSystemIndex());
                        ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap = map;
                        this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
                    }
                    this->mapVisible = 1;
                    this->field_0x18 = 0;
                }
            }
        }
        break;
    case 2: {
        int answers = 3;
        if (key_agent(this)->getMission() != 0 && key_agent(this)->getMission()->isOutsideMission())
            answers = 5;
        else if (key_agent(this)->getOffer() == 2 || key_agent(this)->getOffer() == 3 ||
                 key_agent(this)->getOffer() == 8 || key_agent(this)->getOffer() == 9 ||
                 key_agent(this)->getOffer() == 10)
            answers = 4;

        if (this->listVisible) {
            if (key == 0x40000) {
                this->listVisible = 0;
                ((Engine *) Globals::appManager->GetEngine())->SetPostEffect(0x1400000, this->postEffectWasActive);
            }
            break;
        }

        if (this->chatActive) {
            if (key == 0x1000) {
                this->choiceWindow->left();
            } else if (key == 0x2000) {
                this->choiceWindow->right();
            } else if (key == 0x10000) {
                // The player confirmed the deal.
                {
                String message = *Globals::gameText->getText(850 + Globals::rnd->nextInt(3));
                Agent *agent = key_agent(this);
                switch (agent->getOffer()) {
                case 0:
                case 5:
                    if (Globals::status->hardCoreMode()) {
                        int fee = (agent->getMission()->getReward() + agent->getMission()->getBonus()) / 10;
                        if (fee > Globals::status->getCredits()) {
                            String text = *Globals::gameText->getText(203);
                            text = Globals::status->replaceHash(
                                text,
                                String(Globals::layout->formatCredits(fee - Globals::status->getCredits()), false),
                                String("#C"));
                            this->choiceWindow->set(text, false);
                            this->choiceVisible = 1;
                            this->chatActive = 0;
                            return 0;
                        }
                        Globals::status->changeCredits(-fee);
                    }
                    if (Globals::status->getFreelanceMission() != 0 &&
                        !Globals::status->getFreelanceMission()->isEmpty()) {
                        // Drop the previous freelance job.
                        if (Globals::status->getFreelanceMission()->getType() == 0 ||
                            Globals::status->getFreelanceMission()->getType() == 3 ||
                            Globals::status->getFreelanceMission()->getType() == 5) {
                            Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                            if (cargo != 0) {
                                for (unsigned int i = 0; i < cargo->size(); i++) {
                                    if ((*cargo)[i]->isUnsaleable() && (*cargo)[i]->getIndex() == 116) {
                                        Globals::status->getShip()->removeCargo((*cargo)[i]);
                                        this->hangarUpdate = 1;
                                        break;
                                    }
                                }
                            }
                        } else if (Globals::status->getFreelanceMission()->getType() == 11) {
                            Globals::status->setPassengers(0);
                        }
                        if (!Globals::status->getFreelanceMission()->getAgent()->isGenericAgent())
                            Globals::status->getFreelanceMission()->getAgent()->setOfferAccepted(false);
                        Globals::status->setFreelanceMission(Mission::empty);
                    }
                    if (agent->getMission()->getType() == 0) {
                        Item *goods = (*Globals::items)[116]->makeItem(agent->getMission()->getProductionGoodAmount());
                        goods->setUnsaleable(true);
                        Globals::status->getShip()->addCargo(goods);
                    } else if (agent->getMission()->getType() == 11) {
                        Globals::status->setPassengers(agent->getMission()->getProductionGoodAmount());
                    }
                    if (agent->getMission()->getType() == 12)
                        message = *Globals::gameText->getText(856);
                    else
                        message += String(" ") + *Globals::gameText->getText(853 + Globals::rnd->nextInt(3));
                    if (agent->getMission()->isOutsideMission()) {
                        if (!agent->wasAskedForDifficulty)
                            Globals::status->field_e8++;
                        if (!agent->wasAskedForLocation)
                            Globals::status->field_ec++;
                    }
                    Globals::status->setFreelanceMission(agent->getMission());
                    Globals::status->getFreelanceMission()->setAgent(agent);
                    this->init();
                    this->hangarUpdate = 1;
                    break;
                case 2: {
                    Globals::status->changeCredits(-agent->getSellItemPrice());
                    Item *item = (*Globals::items)[agent->getSellItemIndex()]->makeItem(agent->getSellItemQuantity());
                    bool merged = false;
                    if (item->getType() == 1) {
                        Array<Item *> *equipment = Globals::status->getShip()->getEquipment();
                        if (equipment != 0) {
                            for (unsigned int i = 0; i < equipment->size(); i++) {
                                if ((*equipment)[i] != 0 && (*equipment)[i]->getIndex() == item->getIndex()) {
                                    (*equipment)[i]->changeAmount(item->getAmount());
                                    merged = true;
                                }
                            }
                        }
                    }
                    if (!merged)
                        Globals::status->getShip()->addCargo(item);
                    if (agent->getSellItemIndex() >= 132 && agent->getSellItemIndex() <= 153)
                        (*Globals::status->field_ac)[agent->getSellItemIndex() - 132] = true;
                    this->hangarUpdate = 1;
                    break;
                }
                case 3: {
                    Globals::status->changeCredits(-agent->getSellItemPrice());
                    for (unsigned int i = 0; i < ((Array<BluePrint *> *) Globals::status->getBluePrints())->size(); i++) {
                        if ((*(Array<BluePrint *> *) Globals::status->getBluePrints())[i]->getIndex() ==
                            agent->getSellBlueprintIndex()) {
                            (*(Array<BluePrint *> *) Globals::status->getBluePrints())[i]->unlock();
                            break;
                        }
                    }
                    Array<Agent *> *stationAgents = (Array<Agent *> *) Globals::status->getAgents();
                    if ((unsigned) agent->getIndex() < stationAgents->size())
                        (*stationAgents)[agent->getIndex()]->setOfferAccepted(true);
                    this->hangarUpdate = 1;
                    break;
                }
                case 4: {
                    Globals::status->changeCredits(-agent->getSellItemPrice());
                    Globals::status->setSystemVisibility(agent->getSellSystemIndex(), true);
                    this->hangarUpdate = 1;
                    this->field_0x18 = 1;
                    Array<Agent *> *stationAgents = (Array<Agent *> *) Globals::status->getAgents();
                    if ((unsigned) agent->getIndex() < stationAgents->size())
                        (*stationAgents)[agent->getIndex()]->setOfferAccepted(true);
                    break;
                }
                case 6: {
                    // Hire the wingmen.
                    Globals::status->field_d4 += agent->getWingmanFriendsCount() + 1;
                    Globals::status->setWingmen(agent->getWingmanNames());
                    Globals::status->field_0x2c = agent->getRace();
                    Globals::status->fadeValue = 600000;
                    int *parts = new int[5];
                    for (unsigned int i = 0; i < 5; i++)
                        parts[i] = agent->getImageParts()[i];
                    reinterpret_cast<int *&>(Globals::status->_byte_0x28) = parts;
                    if (Globals::achievements->gotAllMedals())
                        Globals::status->changeCredits(agent->getCosts());
                    else
                        Globals::status->changeCredits(-agent->getCosts());
                    break;
                }
                case 7:
                    ((Standing *) (intptr_t) Globals::status->getStanding())->rehabilitate(agent->getRace());
                    Globals::status->changeCredits(-agent->getCosts());
                    break;
                case 8:
                    Globals::status->changeCredits(Globals::options[56] ? -agent->getSellItemPrice() * 1.0f
                                                                           : -agent->getSellItemPrice() * 2.0f);
                    Globals::status->getShip()->addMod(agent->getSellModIndex());
                    this->hangarUpdate = 1;
                    break;
                case 9: {
                    Globals::status->changeCredits(Globals::options[56] ? -agent->getSellItemPrice() * 1.0f
                                                                           : -agent->getSellItemPrice() * 2.0f);
                    Item *item = (*Globals::items)[agent->getSellItemIndex()]->makeItem(agent->getSellItemQuantity());
                    bool merged = false;
                    if (item->getType() == 1) {
                        Array<Item *> *equipment = Globals::status->getShip()->getEquipment();
                        if (equipment != 0) {
                            for (unsigned int i = 0; i < equipment->size(); i++) {
                                if ((*equipment)[i] != 0 && (*equipment)[i]->getIndex() == item->getIndex()) {
                                    (*equipment)[i]->changeAmount(item->getAmount());
                                    merged = true;
                                }
                            }
                        }
                    }
                    if (!merged)
                        Globals::status->getShip()->addCargo(item);
                    this->hangarUpdate = 1;
                    break;
                }
                case 10:
                    Globals::status->changeCredits(Globals::options[56] ? -agent->getSellItemPrice() * 1.0f
                                                                           : -agent->getSellItemPrice() * 2.0f);
                    Globals::status->voidStation->addShip((*Globals::ships)[agent->getSellItemIndex()]->makeShip(-1));
                    Globals::status->getStation()->addShip((*Globals::ships)[agent->getSellItemIndex()]->makeShip(-1));
                    this->hangarUpdate = 1;
                    break;
                }
                if (agent->getOffer() != 8)
                    agent->setOfferAccepted(true);
                this->scrollWindow->setText(agent->getName(), message);
                this->mode = 3;
                this->chatScroll = 0;
                }
                this->chatActive = 0;
            }
            break;
        }

        if (key == 0x4000) {
            this->chatAnswer--;
            if (this->chatAnswer < 0)
                this->chatAnswer = answers - 1;
        }
        if (key == 0x8000) {
            this->chatAnswer++;
            if (this->chatAnswer >= answers)
                this->chatAnswer = 0;
        }
        if (key == 0x10000 || key == 0x20000) {
            Agent *agent = key_agent(this);
            switch (this->chatAnswer) {
            case 0: {
                // Accept the offer.
                int offer = agent->getOffer();
                String message("");
                bool fits = true;
                switch (offer) {
                case 7:
                    if (Globals::status->getCredits() < agent->getCosts()) {
                        this->choiceWindow->set(
                            Globals::status->replaceHash(
                                *Globals::gameText->getText(203),
                                String(Globals::layout->formatCredits(agent->getCosts() - Globals::status->getCredits()),
                                       false),
                                String("#C")),
                            false);
                        goto showChoice;
                    }
                    message = *Globals::gameText->getText(885);
                    message = Globals::status->replaceHash(
                        message, String(Globals::layout->formatCredits(agent->getCosts()), false), String("#C"));
                    break;
                case 0:
                case 5:
                    if (agent->getMission()->getType() == 0) {
                        int amount = agent->getMission()->getProductionGoodAmount();
                        if (!Globals::status->getShip()->spaceAvailable(amount)) {
                            message = *Globals::gameText->getText(337);
                            message = Globals::status->replaceHash(message, String(String(amount), false), String("#Q"));
                            fits = false;
                            break;
                        }
                    } else if (agent->getMission()->getType() == 11) {
                        int amount = agent->getMission()->getProductionGoodAmount();
                        if (amount > Globals::status->getShip()->getMaxPassengers()) {
                            message = *Globals::gameText->getText(338);
                            message = Globals::status->replaceHash(message, String(String(amount), false), String("#Q"));
                            fits = false;
                            break;
                        }
                    }
                    if (agent->isGenericAgent()) {
                        message = *Globals::gameText->getText(865);
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits(agent->getMission()->getReward() +
                                                                  agent->getMission()->getBonus()),
                                   false),
                            String("#C"));
                        message = Globals::status->replaceHash(
                            message, *Globals::gameText->getText(354 + agent->getMission()->getType()), String("#M"));
                    } else {
                        message = *Globals::gameText->getText(863);
                    }
                    {
                        Mission *freelance = Globals::status->getFreelanceMission();
                        if (freelance != 0 && !freelance->isEmpty())
                            message += String(" ") + *Globals::gameText->getText(864);
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 8:
                case 9:
                case 10: {
                    float premium = Globals::options[56] ? 1.0f : 2.0f;
                    float factor = offer >= 8 && offer <= 10 ? premium : 1.0f;
                    int credits = Globals::status->getCredits();
                    if (factor * (float) agent->getSellItemPrice() > (float) credits) {
                        this->choiceWindow->set(
                            Globals::status->replaceHash(
                                *Globals::gameText->getText(203),
                                String(Globals::layout->formatCredits(
                                           (int) (factor * (float) agent->getSellItemPrice() -
                                                  (float) Globals::status->getCredits())),
                                       false),
                                String("#C")),
                            false);
                        goto showChoice;
                    }
                    switch (offer) {
                    case 8:
                        message = *Globals::gameText->getText(871);
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())),
                                   false),
                            String("#C"));
                        break;
                    case 10:
                        message = *Globals::gameText->getText(873);
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())),
                                   false),
                            String("#C"));
                        break;
                    case 3:
                        message = *Globals::gameText->getText(869);
                        message = Globals::status->replaceHash(
                            message,
                            *Globals::gameText->getText(1274 +
                                                        (*Globals::items)[agent->getSellBlueprintIndex()]->getIndex()),
                            String("#P"));
                        message = Globals::status->replaceHash(
                            message, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                            String("#C"));
                        break;
                    case 4: {
                        message = *Globals::gameText->getText(870);
                        SolarSystem *system =
                            (SolarSystem *) Globals::galaxy->getSystem(agent->getSellSystemIndex());
                        message = Globals::status->replaceHash(
                            message, String(system->getName(), false), String("#S"));
                        message = Globals::status->replaceHash(
                            message, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                            String("#C"));
                        break;
                    }
                    case 2:
                    case 9:
                        message = *Globals::gameText->getText(868);
                        message = Globals::status->replaceHash(message, String(String(agent->getSellItemQuantity()), false),
                                                               String("#Q"));
                        message = Globals::status->replaceHash(
                            message, *Globals::gameText->getText(1274 + agent->getSellItemIndex()), String("#P"));
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())),
                                   false),
                            String("#C"));
                        break;
                    }
                    break;
                }
                case 6:
                    // Hire a wingman.
                    if (!Globals::achievements->gotAllMedals() && Globals::status->getCredits() < agent->getCosts()) {
                        this->choiceWindow->set(
                            Globals::status->replaceHash(
                                *Globals::gameText->getText(203),
                                String(Globals::layout->formatCredits(agent->getCosts() - Globals::status->getCredits()),
                                       false),
                                String("#C")),
                            false);
                        goto showChoice;
                    }
                    if (Globals::status->getWingmen() != 0) {
                        this->choiceWindow->set(*Globals::gameText->getText(785), false);
                        goto showChoice;
                    }
                    message = *Globals::gameText->getText(Globals::achievements->gotAllMedals() ? 867 : 866);
                    message = Globals::status->replaceHash(
                        message, String(String(agent->getWingmanFriendsCount() + 1), false), String("#Q"));
                    message = Globals::status->replaceHash(
                        message, String(Globals::layout->formatCredits(agent->getCosts()), false), String("#C"));
                    break;
                case 1:
                    this->mode = 0;
                    return 1;
                default:
                    break;
                showChoice:
                    this->choiceVisible = 1;
                    goto done;
                }
                if (fits) {
                    this->choiceWindow->set(message, true);
                    this->chatActive = 1;
                } else {
                    this->choiceWindow->set(message, false);
                    this->choiceVisible = 1;
                }
                break;
            }
            case 1:
                // Decline.
                this->scrollWindow->setText(agent->getName(),
                                            *Globals::gameText->getText(845 + Globals::rnd->nextInt(5)));
                this->mode = 3;
                this->chatScroll = 0;
                Globals::status->field_e0++;
                break;
            case 2:
                // Repeat the offer.
                this->mode = 2;
                this->scrollWindow->setText(agent->getName(), Globals::globals->getAgentMissionText(agent));
                this->chatScroll = 0;
                Globals::status->field_e4++;
                break;
            case 3:
                if (agent->getOffer() == 2 || key_agent(this)->getOffer() == 3) {
                    // Show the offered item.
                    if (this->listWindow == 0)
                        this->listWindow = new ListItemWindow();
                    if (key_agent(this)->getOffer() == 2) {
                        this->listWindow->set(new ListItem((*Globals::items)[key_agent(this)->getSellItemIndex()]), 0,
                                              0, 0, 0, false);
                    } else {
                        for (unsigned int b = 0; b < ((Array<BluePrint *> *) Globals::status->getBluePrints())->size();
                             b++) {
                            if ((*(Array<BluePrint *> *) Globals::status->getBluePrints())[b]->getIndex() ==
                                key_agent(this)->getSellBlueprintIndex()) {
                                this->listWindow->set(
                                    new ListItem(
                                        (*Globals::items)[(*(Array<BluePrint *> *) Globals::status->getBluePrints())[b]
                                                              ->getIndex()]),
                                    0, 0, 0, 0, false);
                                break;
                            }
                        }
                    }
                    this->listVisible = 1;
                } else {
                    // Where is the target?
                    key_agent(this)->wasAskedForLocation = 1;
                    if (key_agent(this)->getMission()->getTargetStation() == Globals::status->getStation()->getIndex()) {
                        this->scrollWindow->setText(agent->getName(), *Globals::gameText->getText(805));
                        this->mode = 2;
                        this->chatScroll = 0;
                    } else if (((SolarSystem *) Globals::status->getSystem())
                                   ->stationIsInSystem(key_agent(this)->getMission()->getTargetStation())) {
                        this->scrollWindow->setText(agent->getName(), *Globals::gameText->getText(806));
                        this->mode = 2;
                        this->chatScroll = 0;
                    } else {
                        this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
                        if (this->starMap != 0) {
                            this->starMap->init(true, key_agent(this)->getMission(), false, -1);
                        } else {
                            StarMap *map = new StarMap(true, key_agent(this)->getMission(), false, -1);
                            ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap = map;
                            this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
                        }
                        this->mapVisible = 1;
                    }
                }
                break;
            case 4: {
                // How difficult is it?
                agent->wasAskedForDifficulty = 1;
                int difficulty = agent->getMission()->getDifficulty();
                this->scrollWindow->setText(
                    agent->getName(), *Globals::gameText->getText(808 + (int) ((float) difficulty / 10.0f * 5.0f)));
                this->mode = 2;
                this->chatScroll = 0;
                break;
            }
            }
            if (key_agent(this)->isGenericAgent())
                key_agent(this)->setEvent(1);
        }
        break;
    }
    }
done:
    return 1;
}

void SpaceLounge::updateScreenPositions() {
    using namespace AbyssEngine::AEMath;
    if (this->agents == 0) {
        return;
    }

    Array<KIPlayer *> *figures = this->cutScene->level->getEnemies();
    Vector screen = {0.0f, 0.0f, 0.0f};
    Vector pos = {0.0f, 0.0f, 0.0f};
    Vector offset = {0.0f, 0.0f, 0.0f};
    offset = 150.0f * MatrixGetRight(*(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));

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
        float bob = wave * amplitude;
        if (this->cameraAnimating == 0) {
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
        } else {
            float turn = this->headEase->GetValue();
            if (SPACELOUNGE_ABS(turn - this->headEase->GetMaxValue()) < 0.25f) {
                float target = (float) (5 - Globals::rnd->nextInt(10));
                this->headBobReverse = turn > target;
                this->headEase->SetRange(turn, target);
                this->headBobSteps = Globals::rnd->nextInt(4) + 1;
            }
            int steps = this->headBobSteps;
            this->headEase->Increase((float) ((this->headBobReverse ? -steps : steps) * dt));
        }
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

void SpaceLounge::drawLounge() {
    if (this->mode == 0) {
        // Lounge overview: name plate above the hovered agent.
        if (this->hoverAgent < 0) {
            return;
        }
        Agent *agent = (*this->agents)[this->hoverAgent];
        Vector topLeft = *(*this->agentRects)[this->hoverAgent * 2];
        Vector bottomRight = *(*this->agentRects)[this->hoverAgent * 2 + 1];
        int pad = Globals::layout->field_0x94;
        Globals::Canvas->SetColor(0xffffffffu);
        int centerX = (int) (topLeft.x + (bottomRight.x - topLeft.x) * 0.5f);
        int top = (int) (bottomRight.y - (float) (pad + pad));

        String name = agent->isKnown() || agent->isStoryAgent()
                              ? agent->getName()
                              : String(*Globals::gameText->getText(agent->getRace() + 406), false);
        String role = agent->getMission() != 0
                          ? String(*Globals::gameText->getText(agent->getMission()->getType() + 354), false)
                          : agent->getOffer() == 6
                                ? String(*Globals::gameText->getText(306), false)
                                : agent->getOffer() == 2
                                      ? String(*Globals::gameText->getText(305), false)
                                      : agent->getOffer() == 7 ? String(*Globals::gameText->getText(884), false)
                                                               : String("");

        int width = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, name);
        if (agent->isKnown()) {
            width += Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, role + String("  "));
        }
        Layout *layout = Globals::layout;
        int x = centerX;
        if (x - pad < layout->buttonInsetX) {
            x = layout->buttonInsetX + pad;
        } else if (pad + x + width > Globals::w - layout->buttonInsetX) {
            x = Globals::w - layout->buttonInsetX - (width + pad + pad);
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
        return;
    }

    // Conversation: portrait panel + answer buttons.
    Globals::Canvas->SetColor(0xffffffffu);
    Layout *layout = Globals::layout;
    layout->drawBox(2, this->panelX, this->panelY, layout->field_0x68, layout->field_0x6c, String(""));
    Globals::Canvas->SetColor(0x193043ffu);
    Globals::Canvas->DrawRectangle(this->panelX, this->panelY, Globals::layout->field_0x68,
                                   Globals::layout->field_0x6c);
    Globals::Canvas->SetColor(0xffffffffu);
    Globals::imageFactory->drawChar((*this->silhouetteGrid)[this->selectedAgent],
                                    this->panelX + Globals::layout->field_0x4c,
                                    this->panelY + Globals::layout->field_0x4c, false);
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
        if (offer == 2 || offer == 3 || offer == 9 || offer == 10) {
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
        int stacked = layout->field_0x34 * (this->visibleButtonCount - 2) +
                      layout->field_0x30_rowHeight * (this->visibleButtonCount - 1);
        boxHeight = boxHeight > stacked ? boxHeight : stacked;
    }
    layout->drawBox(2, this->panelX, this->buttonPanelY, layout->field_0x68, inset + boxHeight, String(""));
    Globals::Canvas->SetColor(0x193043ffu);
    Globals::Canvas->DrawRectangle(this->panelX, this->buttonPanelY, Globals::layout->field_0x68, inset + boxHeight);
    Globals::Canvas->SetColor(0xffffffffu);
    Globals::imageFactory->drawChar(this->agentImageParts, this->panelX + Globals::layout->field_0x4c,
                                    this->buttonPanelY + Globals::layout->field_0x4c, true);

    if (this->singleOffer != 0 || offer == 1 || this->mode == 3) {
        (*this->buttons)[0]->setVisible(true);
        (*this->buttons)[0]->draw();
    } else {
        (*this->buttons)[0]->setVisible(true);
        (*this->buttons)[1]->setVisible(true);
        (*this->buttons)[0]->draw();
        (*this->buttons)[1]->draw();
        if (offer == 2 || offer == 3 || offer == 9 || offer == 10) {
            (*this->buttons)[2]->setVisible(true);
            (*this->buttons)[2]->draw();
        } else if (offer == 0 && (*this->agents)[this->selectedAgent]->getMission()->isOutsideMission() &&
                   (*this->agents)[this->selectedAgent]->getMission()->getType() != 12) {
            (*this->buttons)[3]->setVisible(true);
            (*this->buttons)[4]->setVisible(true);
            (*this->buttons)[3]->draw();
            (*this->buttons)[4]->draw();
        }
    }
}

void SpaceLounge::draw3DShip() {
    if (this->listVisible != 0) {
        this->listWindow->render();
    }
}

bool SpaceLounge::checkLocationMode() {
    return false;
}

unsigned char SpaceLounge::mapMode() {
    return this->mapVisible;
}

bool SpaceLounge::listMode() {
    return this->mapVisible == 0;
}

int SpaceLounge::getSoundId(Agent *agent) {
    int race = agent->getRace();
    bool male = agent->isMale();
    int offer = agent->getOffer();
    int missionType = (agent->getMission() != 0 && !agent->getMission()->isEmpty())
                              ? agent->getMission()->getType()
                              : -1;
    String missionText = Globals::globals->getAgentMissionText(agent);

    // Greeting by offer kind.
    bool canBeSpecial = true;
    int soundId;
    switch (offer) {
        case 5:
            soundId = Globals::rnd->nextInt(4) + 795;
            break;
        case 2:
        case 3:
        case 8:
        case 9:
        case 10:
            soundId = Globals::rnd->nextInt(2) + 759;
            break;
        case 7:
            soundId = Globals::rnd->nextInt(4) + 773;
            break;
        case 4:
            soundId = Globals::rnd->nextInt(2) + 766;
            break;
        case 6:
            soundId = Globals::rnd->nextInt(4) + 803;
            break;
        case 0:
            switch (missionType) {
                case 12:
                    soundId = Globals::rnd->nextInt(4) + 762;
                    break;
                case 0:
                case 11:
                    soundId = Globals::rnd->nextInt(4) + 769;
                    break;
                default:
                    if (Globals::rnd->nextInt(2) != 0) {
                        soundId = Globals::rnd->nextInt(4) + 777;
                    } else {
                        soundId = Globals::rnd->nextInt(4) + 799;
                    }
                    break;
            }
            break;
        case 1:
            break;
        default:
            soundId = -1;
            break;
    }
    if (offer == 1 || Globals::rnd->nextInt(100) < 30) {
        soundId = Globals::rnd->nextInt(2) + 781;
    }
    if (offer != 1) {
        canBeSpecial = false;
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

int SpaceLounge::getSpecificSoundForRace(int soundId, int race, bool alternate) {
    short result = soundId;
    switch (race) {
        case 0:
        case 5: {
            unsigned int delta = soundId - 759;
            if (alternate) {
                if (delta < 48) return soundId + 240;
            } else if (delta < 48) {
                return soundId + 192;
            }
            result = -1;
            goto done;
        }
        case 1: {
            unsigned int delta = soundId - 759;
            if (delta < 48) return soundId + 288;
            result = -1;
            goto done;
        }
        case 2:
        case 3: {
            unsigned int delta = soundId - 759;
            if (delta < 48) return soundId + 144;
            result = -1;
            goto done;
        }
        case 4: {
            unsigned int delta = soundId - 759;
            if (delta < 48) return soundId + 96;
            result = -1;
            goto done;
        }
        case 6:
            goto done;
        case 7: {
            unsigned int delta = soundId - 759;
            if (delta < 48) return soundId + 48;
            result = -1;
            goto done;
        }
    }
    result = -1;
done:
    return result;
}

unsigned char SpaceLounge::hangarNeedsUpdate() {
    return this->hangarUpdate;
}

bool SpaceLounge::OnTouchBegin(int x, int y) {
    this->touchDown = 1;
    this->touchX = x;
    this->touchY = y;
    this->hoverAgent = -1;

    if (this->choiceVisible != 0 || this->chatActive != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
        return false;
    }

    if (this->mapVisible != 0) {
        this->starMap->OnTouchBegin(x, y);
        return false;
    }

    if (this->listVisible != 0) {
        this->listWindow->OnTouchBegin(x, y);
        Globals::layout->OnTouchBegin(x, y);
        this->scrollWindow->OnTouchBegin(x, y);
        return false;
    }

    switch (this->mode) {
        case 0:
            if (this->introDone == 0) {
                return false;
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
    return false;
}

bool SpaceLounge::OnTouchMove(int x, int y) {
    this->touchX = x;
    this->touchY = y;

    if (this->choiceVisible != 0 || this->chatActive != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return false;
    }

    if (this->mapVisible != 0) {
        this->starMap->OnTouchMove(x, y);
        return false;
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
    return false;
}

bool SpaceLounge::OnTouchEnd(int x, int y) {
    this->touchDown = 0;
    if (this->choiceVisible || this->chatActive) {
        int choice = this->choiceWindow->OnTouchEnd(x, y);
        if (choice == 0)
            this->onKeyPress(0x10000);
        else if (choice == 1)
            this->chatActive = 0;
        return false;
    }
    if (this->mapVisible) {
        if (this->starMap->OnTouchEnd(x, y)) {
            this->cutScene->resetCamera();
            this->mapVisible = 0;
        }
        return false;
    }
    if (Globals::layout->OnTouchEnd(x, y)) {
        // Back button.
        if (this->listVisible) {
            Globals::layout->resetWindowDimensions();
            this->listVisible = 0;
            return false;
        }
        if (this->mode == 0)
            return true;
        if (this->selectedAgent >= 0 && (*this->agents)[this->selectedAgent]->isGenericAgent())
            (*this->agents)[this->selectedAgent]->setEvent(1);
        this->singleOffer = 0;
        this->mode = 0;
        return false;
    }
    if (this->listVisible) {
        this->listWindow->OnTouchEnd(x, y);
        if (Globals::layout->helpPressed())
            Globals::layout->initHelpWindow(*Globals::gameText->getText(643));
        return false;
    }

    switch (this->mode) {
    case 0:
        if (!this->introDone) {
            // Skip the camera fly-in.
            int race = ((SolarSystem *) Globals::status->getSystem())->getRace();
            Matrix camera;
            MatrixSetTranslation(camera, (float) SpaceLounge_cameraPositions[race * 3],
                                 (float) SpaceLounge_cameraPositions[race * 3 + 1],
                                 (float) SpaceLounge_cameraPositions[race * 3 + 2]);
            MatrixSetRotation(camera, 0.0f, SpaceLounge_cameraRotations[race], 0.0f);
            Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(), camera);
            if (this->cameraEase != 0)
                this->cameraEase->SetRange(camera, camera);
            this->headBobPhase = 0;
            this->introDone = 1;
            return false;
        }
        if (this->agents != 0) {
            this->selectedAgent = -1;
            this->hoverAgent = -1;
            for (unsigned int i = 0; i < this->agents->size(); i++) {
                Vector *topLeft = (*this->agentRects)[i * 2];
                Vector *bottomRight = (*this->agentRects)[i * 2 + 1];
                if (topLeft->x < (float) x && bottomRight->x > (float) x && topLeft->y > (float) y &&
                    bottomRight->y < (float) y) {
                    this->selectedAgent = i;
                    this->onKeyPress(0x10000);
                    return false;
                }
            }
        }
        break;
    case 1:
        this->onKeyPress(0x10000);
        break;
    case 2:
        for (int i = 0; i < this->buttons->size(); i++) {
            if (!(*this->buttons)[i]->OnTouchEnd(x, y))
                continue;
            Agent *agent = (*this->agents)[this->selectedAgent];
            agent->getRace();
            switch (i) {
            case 0: {
                // Accept the offer.
                int offer = agent->getOffer();
                String message("");
                bool fits = true;
                switch (offer) {
                case 7:
                    if (Globals::status->getCredits() < agent->getCosts()) {
                        this->choiceWindow->set(Globals::status->replaceHash(*Globals::gameText->getText(203),
                                                                         String(Globals::layout->formatCredits(agent->getCosts() - Globals::status->getCredits()), false),
                                                                         String("#C")),
                                           false);
                    this->choiceVisible = 1;
                    return false;
                    }
                    message = *Globals::gameText->getText(885);
                    message = Globals::status->replaceHash(
                        message, String(Globals::layout->formatCredits(agent->getCosts()), false), String("#C"));
                    break;
                case 0:
                case 5:
                    if (this->singleOffer) {
                        this->singleOffer = 0;
                        this->mode = 2;
                        String text = Globals::globals->getAgentMissionText((*this->agents)[this->selectedAgent]);
                        if (agent->getMission() != 0 && !agent->getMission()->isEmpty()) {
                            text = Globals::status->replaceHash(
                                text,
                                String(Globals::layout->formatCredits(agent->getMission()->getReward() +
                                                                      agent->getMission()->getBonus()),
                                       false) + this->title,
                                String("#C"));
                        }
                        this->scrollWindow->setText(agent->getName(), text);
                        this->chatScroll = 0;
                        return false;
                    }
                    if (agent->getMission()->getType() == 0) {
                        int amount = agent->getMission()->getProductionGoodAmount();
                        if (!Globals::status->getShip()->spaceAvailable(amount)) {
                            message = *Globals::gameText->getText(337);
                            message = Globals::status->replaceHash(message, String(String(amount), false), String("#Q"));
                            fits = false;
                            break;
                        }
                    } else if (agent->getMission()->getType() == 11) {
                        int amount = agent->getMission()->getProductionGoodAmount();
                        if (amount > Globals::status->getShip()->getMaxPassengers()) {
                            message = *Globals::gameText->getText(338);
                            message = Globals::status->replaceHash(message, String(String(amount), false), String("#Q"));
                            fits = false;
                            break;
                        }
                    }
                    if (agent->isGenericAgent()) {
                        message = *Globals::gameText->getText(865);
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits(agent->getMission()->getReward() +
                                                                  agent->getMission()->getBonus()),
                                   false),
                            String("#C"));
                        message = Globals::status->replaceHash(
                            message, *Globals::gameText->getText(354 + agent->getMission()->getType()), String("#M"));
                        if (Globals::status->hardCoreMode())
                            message += String("\n") + *Globals::gameText->getText(27) + String(": ") +
                                       String(Globals::layout->formatCredits((agent->getMission()->getReward() +
                                                                              agent->getMission()->getBonus()) /
                                                                             10),
                                              false);
                    } else {
                        message = *Globals::gameText->getText(863);
                    }
                    {
                        Mission *freelance = Globals::status->getFreelanceMission();
                        if (freelance != 0 && !freelance->isEmpty())
                            message += String(" ") + *Globals::gameText->getText(864);
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 8:
                case 9:
                case 10: {
                    float premium = Globals::options[56] ? 1.0f : 2.0f;
                    float factor = offer >= 8 && offer <= 10 ? premium : 1.0f;
                    int credits = Globals::status->getCredits();
                    if (factor * (float) agent->getSellItemPrice() > (float) credits) {
                        this->choiceWindow->set(Globals::status->replaceHash(*Globals::gameText->getText(203),
                                                                         String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice() - (float) Globals::status->getCredits())), false),
                                                                         String("#C")),
                                           false);
                    this->choiceVisible = 1;
                    return false;
                    }
                    switch (offer) {
                    case 8:
                        message = *Globals::gameText->getText(871);
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())),
                                   false),
                            String("#C"));
                        break;
                    case 10:
                        message = *Globals::gameText->getText(873);
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())),
                                   false),
                            String("#C"));
                        break;
                    case 3:
                        message = *Globals::gameText->getText(869);
                        message = Globals::status->replaceHash(
                            message,
                            *Globals::gameText->getText(1274 +
                                                        (*Globals::items)[agent->getSellBlueprintIndex()]->getIndex()),
                            String("#P"));
                        message = Globals::status->replaceHash(
                            message, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                            String("#C"));
                        break;
                    case 4: {
                        message = *Globals::gameText->getText(870);
                        SolarSystem *system = (SolarSystem *) Globals::galaxy->getSystem(agent->getSellSystemIndex());
                        message = Globals::status->replaceHash(
                            message, String(system->getName(), false), String("#S"));
                        message = Globals::status->replaceHash(
                            message, String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                            String("#C"));
                        break;
                    }
                    case 2:
                    case 9:
                        message = *Globals::gameText->getText(868);
                        message = Globals::status->replaceHash(message, String(String(agent->getSellItemQuantity()), false),
                                                               String("#Q"));
                        message = Globals::status->replaceHash(
                            message, *Globals::gameText->getText(1274 + agent->getSellItemIndex()), String("#P"));
                        message = Globals::status->replaceHash(
                            message,
                            String(Globals::layout->formatCredits((int) (factor * (float) agent->getSellItemPrice())),
                                   false),
                            String("#C"));
                        break;
                    }
                    break;
                }
                case 6:
                    // Hire a wingman.
                    if (Globals::status->getWingmen() != 0) {
                        this->choiceWindow->set(*Globals::gameText->getText(785), false);
                        this->choiceVisible = 1;
                        return false;
                    }
                    if (Globals::status->getCredits() < agent->getCosts()) {
                        this->choiceWindow->set(Globals::status->replaceHash(*Globals::gameText->getText(203),
                                                                         String(Globals::layout->formatCredits(agent->getCosts() - Globals::status->getCredits()), false),
                                                                         String("#C")),
                                           false);
                    this->choiceVisible = 1;
                    return false;
                    }
                    message = *Globals::gameText->getText(Globals::achievements->gotAllMedals() ? 867 : 866);
                    message = Globals::status->replaceHash(
                        message, String(String(agent->getWingmanFriendsCount() + 1), false), String("#Q"));
                    message = Globals::status->replaceHash(
                        message, String(Globals::layout->formatCredits(agent->getCosts()), false), String("#C"));
                    break;
                case 1:
                    this->mode = 0;
                    return false;
                default:
                    break;
                }
                if (fits) {
                    this->choiceWindow->set(message, true);
                    this->chatActive = 1;
                } else {
                    this->choiceWindow->set(message, false);
                    this->choiceVisible = 1;
                }
                break;
            }
            case 1:
                // Decline.
                this->scrollWindow->setText(agent->getName(),
                                            *Globals::gameText->getText(845 + Globals::rnd->nextInt(5)));
                this->mode = 0;
                this->chatScroll = 0;
                Globals::status->field_e0++;
                break;
            case 2: {
                // Show the offered item.
                if (this->listWindow == 0)
                    this->listWindow = new ListItemWindow();
                if ((*this->agents)[this->selectedAgent]->getOffer() == 2 ||
                    (*this->agents)[this->selectedAgent]->getOffer() == 9) {
                    this->listWindow->set(
                        new ListItem((*Globals::items)[(*this->agents)[this->selectedAgent]->getSellItemIndex()]), 0, 0,
                        0, 0, false);
                } else if ((*this->agents)[this->selectedAgent]->getOffer() == 10) {
                    this->postEffectWasActive = ((Engine *) Globals::appManager->GetEngine())->IsPostEffectActivated();
                    ((Engine *) Globals::appManager->GetEngine())->SetPostEffect(0x1400000, false);
                    this->listWindow->set(
                        new ListItem((*Globals::ships)[(*this->agents)[this->selectedAgent]->getSellItemIndex()]), 0, 0,
                        0, 0, false);
                } else {
                    for (unsigned int b = 0; b < ((Array<BluePrint *> *) Globals::status->getBluePrints())->size(); b++) {
                        if ((*(Array<BluePrint *> *) Globals::status->getBluePrints())[b]->getIndex() ==
                            (*this->agents)[this->selectedAgent]->getSellBlueprintIndex()) {
                            this->listWindow->set(
                                new ListItem((*Globals::items)[(*(Array<BluePrint *> *) Globals::status->getBluePrints())[b]
                                                                   ->getIndex()]),
                                0, 0, 0, 0, false);
                            break;
                        }
                    }
                }
                this->listVisible = 1;
                break;
            }
            case 3: {
                // Where is the target?
                (*this->agents)[this->selectedAgent]->wasAskedForLocation = 1;
                int target = (*this->agents)[this->selectedAgent]->getMission()->getTargetStation();
                if (target == Globals::status->getStation()->getIndex()) {
                    this->scrollWindow->setText(agent->getName(), *Globals::gameText->getText(805));
                    this->mode = 2;
                    this->chatScroll = 0;
                } else if (((SolarSystem *) Globals::status->getSystem())
                               ->stationIsInSystem((*this->agents)[this->selectedAgent]->getMission()->getTargetStation())) {
                    this->scrollWindow->setText(agent->getName(), *Globals::gameText->getText(806));
                    this->mode = 2;
                    this->chatScroll = 0;
                } else {
                    this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
                    if (this->starMap != 0) {
                        this->starMap->init(true, (*this->agents)[this->selectedAgent]->getMission(), false, -1);
                        this->mapVisible = 1;
                    } else {
                        StarMap *map = new StarMap(true, (*this->agents)[this->selectedAgent]->getMission(), false, -1);
                        ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap = map;
                        this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
                        this->mapVisible = 1;
                    }
                }
                break;
            }
            case 4: {
                // How difficult is it?
                (*this->agents)[this->selectedAgent]->wasAskedForDifficulty = 1;
                int difficulty = (*this->agents)[this->selectedAgent]->getMission()->getDifficulty();
                this->scrollWindow->setText(
                    agent->getName(),
                    *Globals::gameText->getText(808 + (int) ((float) difficulty / 10.0f * 5.0f)));
                this->mode = 2;
                this->singleOffer = 1;
                this->chatScroll = 0;
                agent->getRace();
                break;
            }
            }
            if ((*this->agents)[this->selectedAgent]->isGenericAgent())
                (*this->agents)[this->selectedAgent]->setEvent(1);
        }
        break;
    case 3:
        if ((*this->buttons)[0]->OnTouchEnd(x, y))
            this->onKeyPress(0x10000);
        break;
    }
    this->scrollWindow->OnTouchEnd(x, y);
    if (Globals::layout->helpPressed())
        Globals::layout->initHelpWindow(*Globals::gameText->getText(627));
    return false;
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
