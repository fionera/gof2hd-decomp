#include "game/world/StarMap.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "game/core/Globals.h"
#include "game/world/SystemPathFinder.h"
#include "game/world/Level.h"
#include "engine/file/FileRead.h"
#include "game/ship/Ship.h"
#include "engine/core/AERandom.h"
#include "game/world/Galaxy.h"
#include "engine/render/AEGeometry.h"
#include "game/ui/ChoiceWindow.h"
#include "engine/math/EaseInOut.h"
#include "engine/audio/FModSound.h"

#include "game/mission/Item.h"

#include "engine/math/Transform.h"
#include "engine/render/PaintCanvas.h"

#include "game/mission/Achievements.h"

#include "engine/core/ApplicationManager.h"

#include "engine/render/Engine.h"
#include "engine/core/GameText.h"
#include "game/ui/Layout.h"
#include "game/mission/Mission.h"
#include "game/world/Station.h"
#include "game/world/SolarSystem.h"
#include "game/world/Wanted.h"
#include "game/mission/Status.h"
#include "game/core/String.h"
#include "game/ui/TouchButton.h"


static Status **g_StarMap_draw_status;
static Layout **g_StarMap_draw_layout;
static GameText **g_StarMap_draw_text;
static unsigned char **g_StarMap_draw_font;

float EaseInOut_GetValue(void *ease); // lint: void_ptr (external symbol; mangling must match lib)

float EaseInOut_GetMinValue(void *ease); // lint: void_ptr (external symbol; mangling must match lib)

static Layout **g_StarMap_end_layout;
static FModSound **g_StarMap_end_sound;
static GameText **g_StarMap_end_text;

int Station_getIndex(void *station); // lint: void_ptr (external symbol; mangling must match lib)

static inline void MatrixGetPosition(Vector *out, void *matrix) { *out = AbyssEngine::AEMath::MatrixGetPosition(*(const AbyssEngine::AEMath::Matrix*)matrix); } // lint: void_ptr (external symbol; mangling must match lib)

static FModSound **g_StarMap_update_sound;
static int *g_StarMap_update_screenW;
static int *g_StarMap_update_screenH;

void MatrixSetTranslation(void *matrix, float x, float y, float z); // lint: void_ptr (external symbol; mangling must match lib)

// lint: void_ptr (external symbol; mangling must match lib)

static inline void VectorNormalize(Vector *out, Vector *value) { *out = AbyssEngine::AEMath::VectorNormalize(*value); }

void EaseInOut_Update(void *ease, float dt); // lint: void_ptr (external symbol; mangling must match lib)

float EaseInOut_GetCurrentValue(void *ease); // lint: void_ptr (external symbol; mangling must match lib)


static Layout **g_StarMap_touch_layout;
static int *g_StarMap_touch_screenH;
static FModSound **g_StarMap_touch_sound;

void FileRead_ctor(void *reader); // lint: void_ptr (external symbol; mangling must match lib)

void *FileRead_dtor(void *reader); // lint: void_ptr (external symbol; mangling must match lib)

static Layout **g_StarMap_drawKey_layout;
static int *g_StarMap_drawKey_screenW;
static int *g_StarMap_drawKey_screenH;

static void (*g_StarMap_drawKey_drawImage)(uint32_t, uint32_t, int, int);

static GameText **g_StarMap_drawKey_text;

static String *(*g_StarMap_drawKey_getText)(GameText *, int);

static unsigned char **g_StarMap_drawKey_font;

static void (*g_StarMap_drawKey_drawString)(uint32_t, unsigned char *, String *, int, int, bool);

int Station_getTextureIndex(void *station); // lint: void_ptr (external symbol; mangling must match lib)

static int *g_StarMap_info_screenW;
static int *g_StarMap_info_screenH;
static unsigned char **g_StarMap_info_font;
static Layout **g_StarMap_info_layout;
static GameText **g_StarMap_info_text;
static uint8_t *g_StarMap_info_isGerman;

int Station_getTecLevel(void *station); // lint: void_ptr (external symbol; mangling must match lib)

static void (*g_StarMap_init_imageCreate)(uint32_t, int, uint32_t *);

static Layout **g_StarMap_init_layout;
static GameText **g_StarMap_init_text;
static unsigned char **g_StarMap_init_font;
static int *g_StarMap_init_screenW;
static int *g_StarMap_init_screenH;

void MatrixSetRotation(void *matrix, float x, float y, float z, float w); // lint: void_ptr (external symbol; mangling must match lib)

// lint: void_ptr (external symbol; mangling must match lib)

void *SystemPathFinder_ctor(void *finder); // lint: void_ptr (external symbol; mangling must match lib)


StarMap::StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4)
        : scratchVector(), scratchVector2() {
    this->touchStartX = 0.0f;
    this->touchStartY = 0.0f;
    this->field_0x14c = 0.0f;
    this->touchDeltaX = 0.0f;
    this->touchDeltaY = 0.0f;
    this->field_0x158 = 0.0f;
    this->lastTouchX = 0.0f;
    this->lastTouchY = 0.0f;
    this->field_0x164 = 0;
    this->yaw = 0.0f;
    this->pitch = 0.0f;
    this->field_0x190 = 0;

    this->mode = 0;
    this->hitRadius = Globals::layout->field_0x88;
    this->selectedSystem = -1;
    this->starSystemRoot = (AEGeometry *) 0;
    this->stations = (Array<Station *> *) 0;
    this->systemPositions = 0;
    this->stationPositions = 0;
    this->suppressNextClose = 0;
    this->field_0x01 = 0;
    this->alienJumpPending = 0;
    this->easeX = (AbyssEngine::EaseInOut *) 0;
    this->easeY = (AbyssEngine::EaseInOut *) 0;
    this->easeZ = (AbyssEngine::EaseInOut *) 0;
    this->momentumFactor = 0.0f;
    this->velocityX = 0.0f;
    this->velocityY = 0.0f;
    this->dragging = 0;

    this->iconBuffer = new int[5];
    this->systems = (Array<SolarSystem *> *) Globals::galaxy->getSystems();
    this->field_0x10 = 500;
    this->field_0x14 = 500;

    this->systemRoot = new AEGeometry(Globals::Canvas);
    this->systemGeoms = new Array<AEGeometry *>();
    this->systemPositions = new Array<Vector *>();
    ArraySetLength(34, *this->systemGeoms);
    ArraySetLength(34, *this->systemPositions);

    for (uint32_t i = 0; i < this->systemGeoms->size(); i++) {
        int image = (*this->systems)[i]->getTextureIndex() + 18070;
        if (Globals::status->getCurrentCampaignMission() > 157 && i == 27) {
            image = 18075;  // the supernova system's star after the campaign
        }
        (*this->systemGeoms)[i] = new AEGeometry((uint16_t) image, Globals::Canvas, false);

        float scale;
        if ((*this->systems)[i]->getTextureIndex() == 15 && Globals::status->getCurrentCampaignMission() <= 157) {
            scale = Globals::status->getCurrentCampaignMission() > 105 ? 0.025f : 0.018f;
        } else {
            scale = 0.012f;
        }
        (*this->systemGeoms)[i]->setScaling(scale, scale, scale);

        (*this->systemGeoms)[i]->setPosition(
                (float) ((int) ((float) (100 - (*this->systems)[i]->getX()) / 100.0f * 14000.0f) - 10000),
                (float) ((int) ((float) (100 - (*this->systems)[i]->getY()) / 100.0f * 13000.0f) - 9000),
                (float) ((int) ((float) (100 - (*this->systems)[i]->getZ()) / 100.0f * 6000.0f) + 1000));
        this->systemRoot->addChild((*this->systemGeoms)[i]->transform);
        (*this->systemPositions)[i] = new Vector();
    }

    Globals::rnd->reset();
    this->markerGeom = (AEGeometry *) 0;
    this->choiceVisible = 0;
    if (Globals::status->getCurrentCampaignMission() > 31 && Globals::status->field_7c >= 0) {
        this->markerGeom = new AEGeometry((uint16_t) 0x4262, Globals::Canvas, false);
        this->markerGeom->setPosition((*this->systemGeoms)[Globals::status->field_7c]->getPosition());
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->markerGeom->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 2, 0);
        this->markerGeom->setRotation(0.0f, 3.14159274f, 0.0f);
    }

    init(jumpMapMode, mission, param3, param4);
}

// Legend entries measured for the key box width (also the drawKey texts).
static const int StarMap_keyTextIds[6] = {400, 401, 547, 556, 555, 274};

int StarMap::init(bool jumpMapMode, Mission *mission, bool param3, int param4) {
    using namespace AbyssEngine::AEMath;
    Globals::Canvas->FogEnable(false, AbyssEngine::FogMode_1);
    this->pad_0xa8_a = jumpMapMode;
    this->autoMode = param3;
    this->pulseSystem = param4;

    Globals::Canvas->Image2DCreate(0x4a1, this->raceImageNeutral);
    Globals::Canvas->Image2DCreate(0x49c, this->raceImageA);
    Globals::Canvas->Image2DCreate(0x49f, this->raceImageB);
    Globals::Canvas->Image2DCreate(0x49e, this->raceImageDefault);
    Globals::Canvas->Image2DCreate(0x452, this->keyImageRetreat);
    Globals::Canvas->Image2DCreate(0x4a2, this->keyImageDiscovered);
    Globals::Canvas->Image2DCreate(0x453, this->keyImageCurrent);
    Globals::Canvas->Image2DCreate(0x455, this->keyImageMission);
    Globals::Canvas->Image2DCreate(0x454, this->keyImageWanted);
    Globals::Canvas->Image2DCreate(0x48c, this->selectIcon);
    Globals::Canvas->Image2DCreate(0x48a, this->cursorIcon);
    Globals::Canvas->Image2DCreate(0x4fd, this->currentMarkerIcon);
    Globals::Canvas->Image2DCreate(0x545, this->image_0x134);

    this->iconWidth = Globals::Canvas->GetImage2DWidth(this->selectIcon);
    this->missionIconWidth = Globals::Canvas->GetImage2DWidth(this->keyImageMission);
    this->jumpCost = 0;
    this->noRoute = 0;
    this->planetGeom = (AEGeometry *) 0;
    this->spin = 0.0f;
    this->centeredStation = -1;
    this->lastSelectedSystem = -1;
    this->lastSelectedStation = -1;
    this->routeStart = -1;
    this->routeTarget = -1;
    this->field_0xe8 = 0;
    this->field_0xec = 0;
    this->cameraBaseX = 0.0f;
    this->cameraBaseZ = 0.0f;
    this->field_0x01 = 0;
    this->dragging = 0;
    this->transitionIn = 0;
    this->transitionOut = 0;
    this->pathAnim = 0;
    this->stationCenterAnim = 0;
    this->panX = 0;
    this->panY = 0;

    this->easeX = new AbyssEngine::EaseInOut();
    this->easeY = new AbyssEngine::EaseInOut();
    this->easeZ = new AbyssEngine::EaseInOut();
    this->dragScale = Globals::layout->field_0x90;
    this->prevCamera = Globals::Canvas->CameraGetCurrent();
    Globals::Canvas->CameraCreate(this->camera);
    Globals::Canvas->CameraSetPerspective(this->camera, 1.15040f, 200.0f, 64000.0f);
    Matrix view;
    MatrixSetTranslation(view, 0.0f, 0.0f, -2500.0f);
    MatrixSetRotation(view, 0.0f, 3.14159274f, 0.0f);
    MatrixSetTranslation(view, (float) ((int) this->cameraBaseX * 20), (float) ((int) this->cameraBaseZ * 20), 0.0f);
    Globals::Canvas->CameraSetLocal(this->camera, view);
    Globals::Canvas->CameraSetCurrent(this->camera);

    int campaign = Globals::status->getCurrentCampaignMission();
    this->isGalaxyMode = campaign > 15;
    this->mode = campaign > 15 ? 0 : 3;
    this->selectedSystem = ((SolarSystem *) Globals::status->getSystem())->getIndex();

    if (param3) {
        this->scratchVector = (*this->systemGeoms)[param4]->getPosition();
    } else if (jumpMapMode && this->isGalaxyMode) {
        // Centre on the mission's target system (or the remembered one).
        Station *target = (Station *) Globals::galaxy->getStation(mission->getTargetStation());
        if (target != 0 && target->getSystem() >= 0) {
            this->scratchVector = (*this->systemGeoms)[target->getSystem()]->getPosition();
        } else if (Globals::status->field_7c >= 0) {
            this->scratchVector = (*this->systemGeoms)[Globals::status->field_7c]->getPosition();
        }
        if (target != Globals::status->playerStation && target != 0) {
            delete target;
        }
    } else {
        this->scratchVector = (*this->systemGeoms)[this->selectedSystem]->getPosition();
    }

    PaintCanvas *canvas = Globals::Canvas;
    Matrix local = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
    MatrixSetTranslation(local, this->scratchVector.x, this->scratchVector.y, 0.0f);
    Globals::Canvas->CameraSetLocal(this->camera, local);
    this->cameraBaseX = this->scratchVector.x / 20.0f;
    this->cameraBaseZ = this->scratchVector.y / 20.0f;

    if (this->mode == 3) {
        this->selectedStation = -1;
        this->initStarSystem();
        this->scratchVector = (*this->systemGeoms)[this->selectedSystem]->getPosition();
        this->scratchVector.z += -500.0f;
        PaintCanvas *c = Globals::Canvas;
        Matrix cam = *(const Matrix *) c->CameraGetLocal(c->CameraGetCurrent());
        cam.m[11] = this->scratchVector.z;
        Globals::Canvas->CameraSetLocal(this->camera, cam);
    } else {
        this->lastSelectedSystem = this->selectedSystem;
        if (this->stations != 0) {
            ArrayReleaseClasses(*this->stations);
            delete this->stations;
            this->stations = (Array<Station *> *) 0;
        }
        this->stations = new Array<Station *>();
        FileRead *reader = new FileRead();
        this->stations = reader->loadStationsBinary((*this->systems)[this->selectedSystem]);
        delete reader;
    }
    if (param3) {
        this->lastSelectedSystem = -1;
        this->selectedSystem = -1;
    }

    this->showKey = 0;
    this->jumpMapModeA = 0;
    this->jumpMapModeB = 0;
    this->exitRequested = 0;
    this->backButton = new TouchButton(*Globals::gameText->getText(400), 0,
                                       Globals::w - Globals::layout->field_0x2c_rowHeight,
                                       Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
    this->choiceWindow = new ChoiceWindow();
    this->systemPath = (Array<int> *) 0;
    this->pathFinder = new SystemPathFinder();

    if (jumpMapMode && this->mode == 0) {
        this->targetSystem = -1;
        this->selectedSystem = -1;
        if (!mission->isEmpty() && (mission->isVisible() || mission->getType() == 14) &&
            (this->mode == 3 || ((SolarSystem *) Globals::status->getSystem())->getRoutes() != 0)) {
            for (uint32_t i = 0; i < this->systems->size(); i++) {
                if ((*this->systems)[i]->getStations() == 0)
                    continue;
                for (uint32_t j = 0; j < ((Array<int> *) (*this->systems)[i]->getStations())->size(); j++) {
                    if (mission->getTargetStation() == (*(Array<int> *) (*this->systems)[i]->getStations())[j]) {
                        this->targetSystem = i;
                        if (this->mode == 3) {
                            this->lastSelectedStation = j;
                            this->selectedStation = j;
                            this->stationCenterAnim = 1;
                        }
                        break;
                    }
                }
            }
            if (this->targetSystem == -1 && mission->getTargetStation() == -1 && Globals::status->field_7c >= 0) {
                this->targetSystem = Globals::status->field_7c;
            }
            if (this->targetSystem >= 0) {
                this->systemPath = this->pathFinder->getSystemPath(
                        this->systems, ((SolarSystem *) Globals::status->getSystem())->getIndex(), this->targetSystem);
            }
        } else if (!mission->isEmpty() && (mission->isVisible() || mission->getType() == 14) &&
                   ((SolarSystem *) Globals::status->getSystem())->getRoutes() == 0) {
            for (uint32_t i = 0; i < this->systems->size(); i++) {
                if ((*this->systems)[i]->getStations() == 0)
                    continue;
                for (uint32_t j = 0; j < ((Array<int> *) (*this->systems)[i]->getStations())->size(); j++) {
                    if (mission->getTargetStation() == (*(Array<int> *) (*this->systems)[i]->getStations())[j]) {
                        this->targetSystem = i;
                        break;
                    }
                }
            }
        }
        this->pathAnim = 1;
        this->momentumFactor = 0.9f;
        this->selectedSystem = this->targetSystem;
    }

    this->keyBoxWidth = 0;
    for (int i = 0; i < 6; i++) {
        int w = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                              *Globals::gameText->getText(StarMap_keyTextIds[i]));
        if (w > this->keyBoxWidth) {
            this->keyBoxWidth = w;
        }
    }
    this->keyBoxWidth += Globals::layout->field_0x8c;
    this->keyBoxHeight = Globals::layout->field_0x4 * 5 + Globals::layout->field_0x2c_rowHeight * 2;
    this->autoTimer = 0;
    Item *energy = Globals::status->getShip()->getCargo(122);
    this->cargoAmount = energy != 0 ? energy->getAmount() : 0;

    this->bgLayer0 = new AEGeometry((uint16_t) 0x41d2, Globals::Canvas, false);
    this->bgLayer0->setRotation(0.0f, 3.14159274f, 0.0f);
    this->bgLayer0->setPosition(-3000.0f, -2500.0f, 0.0f);
    this->bgLayer1 = new AEGeometry((uint16_t) 0x41d3, Globals::Canvas, false);
    this->bgLayer1->setRotation(0.0f, 3.14159274f, 0.0f);
    this->bgLayer1->setPosition(-3000.0f, -2500.0f, 0.0f);
    this->bgLayer2 = new AEGeometry((uint16_t) 0x41d4, Globals::Canvas, false);
    this->bgLayer2->setRotation(0.0f, 3.14159274f, 0.0f);
    this->bgLayer2->setPosition(-3000.0f, -2500.0f, 0.0f);
    return 0;
}

StarMap::~StarMap() {
    if (this->systemPositions != 0) {
        ArrayReleaseClasses(*this->systemPositions);
        delete this->systemPositions;
    }
    this->systemPositions = 0;

    if (this->stationPositions != 0) {
        ArrayReleaseClasses(*this->stationPositions);
        delete this->stationPositions;
    }
    this->stationPositions = 0;

    delete this->planetGeom;
    this->planetGeom = (AEGeometry *) 0;

    delete this->bgLayer0;
    this->bgLayer0 = (AEGeometry *) 0;

    delete this->bgLayer1;
    this->bgLayer1 = (AEGeometry *) 0;

    delete this->bgLayer2;
    this->bgLayer2 = (AEGeometry *) 0;

    delete this->easeX;
    this->easeX = (AbyssEngine::EaseInOut *) 0;

    delete this->easeY;
    this->easeY = (AbyssEngine::EaseInOut *) 0;

    delete this->easeZ;
    this->easeZ = (AbyssEngine::EaseInOut *) 0;

    delete this->markerGeom;
    this->markerGeom = (AEGeometry *) 0;

    delete this->pathFinder;
    this->pathFinder = (SystemPathFinder *) 0;
}

void StarMap::initStarSystem() {
    SolarSystem *system = (*this->systems)[this->selectedSystem];
    uint32_t count = ((Array<int> *) system->getStations())->size();

    Array<Station *> *stations = new Array<Station *>();
    this->stations = stations;
    ArraySetLength(count, *stations);
    FileRead *reader = (FileRead *) operator new(1);
    FileRead_ctor(reader);
    this->stations = (Array<Station *> *) reader->loadStationsBinary();
    operator delete(FileRead_dtor(reader));

    this->stationAngles = new int[count];
    this->stationDistances = new int[count];
    this->centeredStation = -1;
    Globals::rnd->setSeed((long long) system->getIndex() * 1000);

    Array<AEGeometry *> *stationGeoms = new Array<AEGeometry *>();
    this->stationGeoms = stationGeoms;
    ArraySetLength(count + 1, *stationGeoms);

    AEGeometry *root = new AEGeometry(Globals::Canvas);
    this->starSystemRoot = root;

    Array<bool> *used = new Array<bool>();
    this->usedFlags = used;
    ArraySetLength(stationGeoms->size(), *used);
    for (uint32_t i = 0; i < used->size(); i++) {
        (*used)[i] = false;
    }

    for (uint32_t i = 1; i < stationGeoms->size(); i++) {
        uint32_t stationIndex = i - 1;
        int tex = Station_getTextureIndex((*this->stations)[stationIndex]);
        AEGeometry *geom = new AEGeometry((uint16_t)(tex + 0x4704), Globals::Canvas, false);
        (*stationGeoms)[i] = geom;
        this->stationAngles[stationIndex] =
                Globals::rnd->nextInt(used->size()) *
                (0x10000 / (int) used->size());
        int dist = (i == 1) ? 0x1900 : this->stationDistances[i - 2];
        dist += Globals::rnd->nextInt(0x15e0) + 0x640;
        this->stationDistances[stationIndex] = dist;
        Vector pos = {0.0f, 0.0f, (float) dist};
        geom->translate(pos);
        float scale = (float) (tex << 4) * 0.001f;
        geom->setScaling(scale);
        root->addChild(geom->transform);
        if (this->markerGeom != 0 && this->selectedSystem == Globals::status->field_7c &&
            Station_getIndex((*this->stations)[stationIndex]) == Globals::status->field_80) {
            this->centeredStation = i;
        }
    }

    (*stationGeoms)[1]->setVisible(false);

    Array<AEGeometry *> *rings = new Array<AEGeometry *>();
    this->ringGeoms = rings;
    ArraySetLength(count, *rings);
    for (uint32_t i = 0; i < rings->size(); i++) {
        AEGeometry *ring = new AEGeometry((uint16_t) 0x1a7b, Globals::Canvas, false);
        (*rings)[i] = ring;
        root->addChild(ring->transform);
        float scale = (float) (this->stationDistances[i] << 1) * 0.001f;
        ring->setScaling(scale);
    }

    Vector selected;
    ((AEGeometry *) (&selected))->getPosition();
    root->setPosition(selected);
    root->setScaling(0.0078125f);
    root->setRotation(0.0f, 0.0f, 0.0f);
    this->yaw = 4096.0f;
    this->pitch = 0.0f;
    this->selectedStation = -1;
    Globals::Canvas->Image2DCreate((unsigned short) ((uint16_t)(0x4500 + system->getRace())), this->systemNameImage);

    if (this->stationPositions != 0) {
        ArrayReleaseClasses(*this->stationPositions);
        delete this->stationPositions;
        this->stationPositions = 0;
    }
    Array<Vector *> *stationPositions = new Array<Vector *>();
    this->stationPositions = stationPositions;
    ArraySetLength(this->stations->size(), *stationPositions);
    for (uint32_t i = 0; i < stationPositions->size(); i++) {
        Vector *p = new Vector{0.0f, 0.0f, 0.0f};
        (*stationPositions)[i] = p;
    }

    gEngine->LightSetLightDirection(0.0f, 0.0f, 1.0f, 1);
    if (this->planetGeom != 0) {
        delete this->planetGeom;
    }
    this->planetTexture = 0xffffffffu;
    this->planetGeom = (AEGeometry *) 0;
    uint16_t texture = (uint16_t)(0x2700 + system->getTextureIndex());
    if (this->selectedSystem == 0x1b) {
        texture = 0x2734;
    }
    Globals::Canvas->TextureCreate((unsigned short) (texture), nullptr, nullptr, this->planetTexture, false);
    AEGeometry *planet = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
    this->planetGeom = planet;
    planet->setPosition(selected);
    planet->setRotation(0.0f, 0.0f, 0.0f);
    planet->setScaling(1.0f);
    (*this->systemGeoms)[this->selectedSystem]->setVisible(false);
}

void StarMap::initLights() {
    Engine *engine = (Engine *) Globals::appManager->GetEngine();
    engine->LightSetMaterialColorAmbient(0.5f, 0.5f, 0.5f);
    Globals::Canvas->FogEnable(false, AbyssEngine::FogMode_1);
}

void StarMap::setStart(int start, int target) {
    this->routeStart = target;
    this->routeTarget = start;
    delete this->systemPath;
    this->systemPath = nullptr;
    this->systemPath =
            this->pathFinder->getSystemPath(this->systems, start, this->targetSystem);
}

void StarMap::depart(bool jump) {
    int selected = this->selectedStation;
    if (selected < 0) {
        return;
    }

    if (this->jumpMapModeA != 0) {
        Globals::status->departStation((*this->stations)[selected]);
        Level::programmedStation = 0;
        Level::setInitStreamOut();
        Globals::status->jumpgateUsed();
        if (!jump || this->jumpMapModeB == 0) {
            Level::doInstantJump = 0;
        } else {
            int toSystem = (*this->stations)[this->selectedStation]->getSystem();
            int current = ((SolarSystem *) (long) Globals::status->getSystem())->getIndex();
            Level::doInstantJump = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                Level::energyCellsForNextJump = this->jumpCost;
            }
        }
    } else if (Globals::status->getCurrentCampaignMission() != 3) {
        Globals::status->field_5c = -1;
        Globals::status->field_60 = -1;
        Globals::status->field_64 = -1;
        Globals::status->field_68 = -1;
        Globals::status->departStation((Station *) Globals::status->getStation());

        if ((*this->stations)[this->selectedStation]->equals((Station *) Globals::status->getStation()) == 0) {
            Level::programmedStation = (*this->stations)[this->selectedStation];
        }

        if (!jump || ((Ship *) Globals::status->getShip())->hasVolatileGoods() ||
            (!((Ship *) Globals::status->getShip())->hasJumpDriveIntegrated() && this->jumpMapModeB == 0)) {
            Level::doInstantJump = 0;
        } else {
            int toSystem = Level::programmedStation->getSystem();
            int current = ((SolarSystem *) (long) Globals::status->getSystem())->getIndex();
            Level::doInstantJump = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                Level::energyCellsForNextJump = this->jumpCost;
            }
        }
        Globals::achievements->resetNewMedals();
    }

    for (uint32_t i = 0; i < this->stations->size(); i++) {
        if (i != (uint32_t) this->selectedStation) {
            delete (*this->stations)[i];
            (*this->stations)[i] = 0;
        }
    }
    delete this->stations;
    this->stations = (Array<Station *> *) 0;
    Globals::sound->stop(0x66);
    Globals::switch_to_target_setting = 1;

    Globals::appManager->SetCurrentApplicationModule(2);
}

bool StarMap::isInPlanetMode() {
    return this->mode == 3;
}

void StarMap::setJumpMapMode(bool enabled, bool value) {
    this->jumpMapModeA = (uint8_t) enabled;
    this->jumpMapModeB = (uint8_t) value;
}

void StarMap::renderBG() {
}

void StarMap::render() {
    Globals::Canvas->SetColor((unsigned int) (0xffffffffu));
    this->bgLayer0->render();
    this->bgLayer1->render();
    this->bgLayer2->render();
    this->systemRoot->render();
    Globals::Canvas->End3d();
    Globals::Canvas->Begin3d();
    if (this->starSystemRoot != 0) {
        Globals::Canvas->SetTexture((unsigned int) (this->planetTexture), (unsigned int) (0xffffffffu));
        Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);
        this->planetGeom->render();
        this->starSystemRoot->render();
    }
    if (this->markerGeom != 0) {
        this->markerGeom->render();
    }
}

void StarMap::drawKey() {
    int iconWidth = Globals::Canvas->GetImage2DWidth(this->keyImageDiscovered);
    Layout *layout = Globals::layout;
    int screenH = Globals::h;
    int lineSpacing = layout->field_0x4;
    int boxW = this->keyBoxWidth;
    int boxH = this->keyBoxHeight;
    int screenW = Globals::w;
    int padY = layout->windowTopInset;
    int bottomMargin = layout->field_0x10_rightMargin;
    int indent = layout->field_0x2c_rowHeight;

    int x = screenW - boxW;
    layout->drawBox(7, x, screenH - bottomMargin - boxH - padY, boxW, padY + boxH, String(""));

    int iconX = x + indent;
    int textX = iconWidth + indent + iconX;
    int y = screenH - indent - bottomMargin - lineSpacing;

    Globals::Canvas->DrawImage2D(this->keyImageRetreat, iconX, y);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(274), textX, y,
                                false);
    y -= Globals::layout->field_0x4;
    Globals::Canvas->DrawImage2D(this->keyImageDiscovered, iconX, y);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(401), textX, y,
                                false);
    y -= Globals::layout->field_0x4;
    Globals::Canvas->DrawImage2D(this->keyImageCurrent, iconX, y);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(547), textX, y,
                                false);
    y -= Globals::layout->field_0x4;
    Globals::Canvas->DrawImage2D(this->keyImageMission, iconX, y);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(556), textX, y,
                                false);
    y -= Globals::layout->field_0x4;
    Globals::Canvas->DrawImage2D(this->keyImageWanted, iconX, y);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(555), textX, y,
                                false);
}

void StarMap::drawOnScreenInfo(int index, bool stationMode) {
    String name;
    String line;
    String value;

    Array<Vector *> *positions =
            stationMode ? this->stationPositions : this->systemPositions;
    this->scratchVector = *(*positions)[index];
    float x = this->scratchVector.x;
    float y = this->scratchVector.y;
    if (x < 0.0f || x > (float) (*g_StarMap_info_screenW + 0x32) ||
        y < 0.0f || y > (float) (*g_StarMap_info_screenH + 0x32)) {
        return;
    }

    PaintCanvas *canvas = Globals::Canvas;
    canvas->SetColor((unsigned int) (0xffffffffu));
    int *icons = this->iconBuffer;
    for (int i = 0; i != 5; i++) {
        icons[i] = -1;
    }

    if (stationMode) {
        Station *station = (*this->stations)[index];
        if (station->isDiscovered() != 0) {
            icons[0] = this->keyImageDiscovered;
        }
        int current = Station_getIndex(Globals::status->getStation());
        if (current == Station_getIndex(station)) {
            icons[3] = this->keyImageCurrent;
        }
        ((Station *) (&name))->getName();
        int textW = ((PaintCanvas *) (long) (canvas))->GetTextWidth((unsigned int) (long) (*g_StarMap_info_font), name);
        int drawX = (int) (x - (float) (textW / 2));
        int drawY = (int) (y + (float) (this->iconWidth >> 1) - 3.0f);
        if (this->selectedStation == index) {
            ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0x80),
                                                        (unsigned char) (0), (unsigned char) (this->alpha));
            ((PaintCanvas *) (long) (canvas))->DrawString((unsigned int) (long) (*g_StarMap_info_font), name, drawX,
                                                          drawY, false);
            ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0xff),
                                                        (unsigned char) (0xff), (unsigned char) (this->alpha));
            if (Station_getTecLevel(station) > 0) {
                line.copy((String *) ((GameText *) (*g_StarMap_info_text))->getText(0x200), false);
                value.Set((long long) (Station_getTecLevel(station)));
                name = line + value;
                ((PaintCanvas *) (long) (canvas))->DrawString((unsigned int) (long) (*g_StarMap_info_font), name, drawX,
                                                              drawY + ((Layout *) *g_StarMap_info_layout)->field_0x4,
                                                              false);
            }
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->selectIcon), (int) x, (int) y,
                                                           (unsigned char) (0x11));
        } else {
            ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0xff),
                                                        (unsigned char) (0xff), (unsigned char) (this->alpha));
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->cursorIcon), (int) x, (int) y,
                                                           (unsigned char) (0x11));
            ((PaintCanvas *) (long) (canvas))->DrawString((unsigned int) (long) (*g_StarMap_info_font), name, drawX,
                                                          drawY, false);
        }
    } else {
        SolarSystem *system = (*this->systems)[index];
        if (system->isFullyDiscovered() != 0) {
            icons[0] = this->keyImageDiscovered;
        }
        if (Globals::status->getCurrentCampaignMission() == 0x34 &&
            system->getStationEnumIndex(0x4a) >= 0) {
            icons[1] = this->keyImageWanted;
        }
        Mission *mission = (Mission *) (long) Globals::status->getCampaignMission();
        if (mission != 0 && mission->isEmpty() == 0) {
            int target = mission->getTargetStation();
            if (system->getStationEnumIndex(target) >= 0) {
                icons[2] = this->keyImageMission;
            }
        }
        Mission *freelance = (Mission *) Globals::status->getFreelanceMission();
        if (freelance != 0 && freelance->isEmpty() == 0) {
            int target = freelance->getTargetStation();
            if (system->getStationEnumIndex(target) >= 0) {
                icons[2] = this->keyImageMission;
            }
        }
        ((SolarSystem *) (&name))->getName();
        int textW = ((PaintCanvas *) (long) (canvas))->GetTextWidth((unsigned int) (long) (*g_StarMap_info_font), name);
        int drawX = (int) (x - (float) (textW / 2));
        int drawY = (int) (y + (float) (this->iconWidth >> 1) - 3.0f);
        int currentSystem = Globals::status->getSystem();
        if (currentSystem == system->getIndex()) {
            ((Layout *) (*g_StarMap_info_layout))->getPulseValue((float) this->alpha);
            ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0xff),
                                                        (unsigned char) (0xff), (unsigned char) (this->alpha));
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->currentMarkerIcon), (int) x, (int) y,
                                                           (unsigned char) (0x11));
        }
        if (this->selectedSystem == index) {
            ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0x80),
                                                        (unsigned char) (0), (unsigned char) (this->alpha));
        } else if (this->selectedSystem >= 0) {
            ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0xff),
                                                        (unsigned char) (0xff), (unsigned char) (this->alpha));
        }
        ((PaintCanvas *) (long) (canvas))->DrawString((unsigned int) (long) (*g_StarMap_info_font), name, drawX, drawY,
                                                      false);
        ((PaintCanvas *) (long) (canvas))->SetColor((unsigned char) (0xff), (unsigned char) (0xff),
                                                    (unsigned char) (0xff), (unsigned char) (this->alpha));
        if (system->hasNoOwner() == 0) {
            uint32_t image = this->raceImageDefault;
            int race = system->getRace();
            if (race == 2) {
                image = this->raceImageB;
            } else if (race == 1) {
                image = this->raceImageA;
            } else if (race == 0) {
                image = this->raceImageNeutral;
            }
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (image), drawX, drawY + 0xfd,
                                                           (unsigned char) (0x11));
        }
        if (this->selectedSystem == index) {
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->selectIcon), (int) x, (int) y,
                                                           (unsigned char) (0x11));
        } else {
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) (this->cursorIcon), (int) x, (int) y,
                                                           (unsigned char) (0x11));
        }
    }

    float iconY = (float) (int) ((this->scratchVector.y - (float) (this->iconWidth >> 1)) + 10.0f);
    float iconX = (float) (int) (this->scratchVector.x + (float) (this->iconWidth >> 1) - 7.0f);
    for (int i = 0; i != 5; i++) {
        int image = icons[i];
        if (image != -1) {
            int dx = (i == 0 && *g_StarMap_info_isGerman != 0) ? 0xc : 0x12;
            ((PaintCanvas *) (long) (canvas))->DrawImage2D((unsigned int) ((uint32_t) image), (int) iconX - dx,
                                                           (int) iconY, (unsigned char) (0));
        }
    }
}

void StarMap::draw() {
    String tmp;

    int mode = this->mode;
    int alpha = mode == 0 ? 0xff : 0;
    this->alpha = alpha;
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        float v = EaseInOut_GetValue(this->easeZ);
        float min = EaseInOut_GetMinValue(this->easeZ);
        float max = this->easeZ->GetMaxValue();
        float t = (v - min) / (max - min);
        if (this->transitionIn != 0) {
            t = 1.0f - t;
        }
        this->alpha = (int) (t * 255.0f);
    }

    if (mode != 3 || this->transitionIn != 0 || this->transitionOut != 0) {
        Globals::Canvas->SetColor((unsigned char) (0xff), (unsigned char) (0xff), (unsigned char) (0xff),
                                  (unsigned char) (this->alpha));
        Array<Vector *> *positions = this->systemPositions;
        for (uint32_t i = 0; i < positions->size(); i++) {
            this->scratchVector = *(*positions)[i];
            Array<int> *routes = (Array<int> *) (*this->systems)[i]->getRoutes();
            if (routes != 0) {
                for (uint32_t j = 0; j < routes->size(); j++) {
                    uint32_t to = (uint32_t) (*routes)[j];
                    if (to < positions->size()) {
                        this->scratchVector2 = *(*positions)[to];
                        if (this->scratchVector.z >= 0.0f || this->scratchVector2.z >= 0.0f) {
                            Globals::Canvas->DrawLine((int) this->scratchVector.x, (int) this->scratchVector.y,
                                                      (int) this->scratchVector2.x, (int) this->scratchVector2.y);
                        }
                    }
                }
            }
        }
        for (uint32_t i = 0; i < this->systems->size(); i++) {
            drawOnScreenInfo((int) i, false);
        }
    }

    if (this->selectedSystem >= 0 &&
        (this->autoMode == 0 || this->autoTimer > 3999)) {
        drawOnScreenInfo(this->selectedSystem, false);
    }

    if (this->starSystemRoot != 0 && this->stations != 0) {
        SolarSystem *system = (*this->systems)[this->selectedSystem];
        if (system->hasNoOwner() == 0) {
            Globals::Canvas->SetColor((unsigned char) (0xff), (unsigned char) (0xff), (unsigned char) (0xff),
                                      (unsigned char) (this->alpha ^ 0xff));
            Layout *drawLayout = (Layout *) *g_StarMap_draw_layout;
            Globals::Canvas->DrawImage2D((unsigned int) (this->systemNameImage), drawLayout->field_0x2c_rowHeight,
                                         drawLayout->field_0xc_leftMargin +
                                         drawLayout->field_0x2c_rowHeight, (unsigned char) (0));
            ((SolarSystem *) (&tmp))->getName();
            Globals::Canvas->DrawString((unsigned int) (long) (*g_StarMap_draw_font), tmp,
                                        Globals::Canvas->GetImage2DWidth((unsigned int) (this->systemNameImage)) +
                                        drawLayout->field_0x2c_rowHeight * 2,
                                        drawLayout->field_0xc_leftMargin +
                                        drawLayout->field_0x2c_rowHeight + 2, false);
        }
        for (uint32_t i = 0; i < this->stations->size(); i++) {
            if (i != (uint32_t) this->selectedStation) {
                drawOnScreenInfo((int) i, true);
            }
        }
        if (this->selectedStation >= 0) {
            drawOnScreenInfo(this->selectedStation, true);
        }
    }

    if (this->showKey != 0) {
        drawKey();
    }
    tmp.copy((String *) ((GameText *) (*g_StarMap_draw_text))->getText(0x190), false);
    ((Layout *) (*g_StarMap_draw_layout))->drawHeader(tmp);
    ((Layout *) (*g_StarMap_draw_layout))->drawEmptyFooter(1);
    this->backButton->draw();
    if (this->choiceVisible != 0) {
        this->choiceWindow->draw();
    }
}

uint8_t StarMap::missionChanged() {
    return this->missionChangedFlag;
}

void StarMap::askForJumpIntoAlienWorld() {
    ChoiceWindow *window = this->choiceWindow;
    this->alienJumpPending = 1;
    String *text = Globals::gameText->getText(0x1a6);
    window->set(*text, true);
    this->choiceVisible = 1;
}

static inline float absf_update(float v) {
    return v < 0.0f ? -v : v;
}

void StarMap::update(int dt) {
    Matrix matrix;
    Vector tmp;

    this->lastDt = dt;
    if (this->mode == 0 || this->transitionIn != 0 || this->transitionOut != 0) {
        Array<Vector *> *positions = this->systemPositions;
        for (uint32_t i = 0; i < positions->size(); i++) {
            ((AEGeometry *) (&tmp))->getPosition();
            int visible = Globals::Canvas->GetScreenPosition(tmp, *(*positions)[i]);
            (*positions)[i]->z = visible != 0 ? 1.0f : -1.0f;
        }
    }
    if (this->mode == 3 || this->transitionIn != 0 || this->transitionOut != 0) {
        Array<Vector *> *positions = this->stationPositions;
        if (positions != 0) {
            for (uint32_t i = 0; i < positions->size(); i++) {
                ((AEGeometry *) (&tmp))->getPosition();
                int visible = Globals::Canvas->GetScreenPosition(tmp, *(*positions)[i]);
                (*positions)[i]->z = visible != 0 ? 1.0f : -1.0f;
            }
        }
    }

    if (this->markerGeom != 0) {
        float v = EaseInOut_GetValue(this->easeZ);
        float min = EaseInOut_GetMinValue(this->easeZ);
        float max = this->easeZ->GetMaxValue();
        float t = (v - min) / (max - min);
        if (this->transitionOut != 0) {
            t = 1.0f - t;
        } else if (this->transitionIn == 0 && this->mode == 0) {
            t = 1.0f;
        }
        float scale = (float) (0.5 + (double) t * 0.5);
        this->markerGeom->setScaling(scale);
        if (this->starSystemRoot != 0 && this->centeredStation >= 0) {
            ((AEGeometry *) (&tmp))->getPosition();
            this->markerGeom->setPosition(tmp);
        }
        PaintCanvas *canvas = Globals::Canvas;
        ((PaintCanvas *) (long) (canvas))->CameraGetCurrent();
        MatrixGetPosition(
            &tmp, ((PaintCanvas *) (long) (canvas))->CameraGetLocal(
                ((PaintCanvas *) (long) (canvas))->CameraGetCurrent()));
        this->scratchVector = tmp;
        ((AEGeometry *) (&tmp))->getPosition();
        this->scratchVector -= tmp;
        VectorNormalize(&tmp, &this->scratchVector);
        this->scratchVector = tmp;
        this->scratchVector.x += 0.5f;
        Vector worldUp = {0.0f, 1.0f, 0.0f};
        this->markerGeom->setDirection(this->scratchVector, worldUp);
        ((AbyssEngine::Transform *) (((PaintCanvas *) (long) (canvas))->TransformGetTransform(0)))->Update(dt, false);
    }

    if (this->transitionIn != 0 || this->transitionOut != 0) {
        float step = (float) (dt * 15);
        EaseInOut_Update(this->easeX, step);
        EaseInOut_Update(this->easeY, step);
        EaseInOut_Update(this->easeZ, step);
        tmp.x = EaseInOut_GetCurrentValue(this->easeX);
        tmp.y = EaseInOut_GetCurrentValue(this->easeY);
        tmp.z = EaseInOut_GetCurrentValue(this->easeZ);
        this->scratchVector = tmp;
        PaintCanvas *canvas = Globals::Canvas;
        __builtin_memcpy(
            &matrix, ((PaintCanvas *) (long) (canvas))->CameraGetLocal(
                ((PaintCanvas *) (long) (canvas))->CameraGetCurrent()), 0x3c);
        MatrixSetTranslation(&matrix, this->scratchVector.x, this->scratchVector.y, this->scratchVector.z);
        ((PaintCanvas *) (long) (canvas))->CameraSetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent(),
                                                          *(const AbyssEngine::AEMath::Matrix *) (&matrix));
        if (absf_update(this->scratchVector.x - this->easeX->GetMaxValue()) <= 1.0f &&
            absf_update(this->scratchVector.y - this->easeY->GetMaxValue()) <= 1.0f &&
            absf_update(this->scratchVector.z - this->easeZ->GetMaxValue()) <= 1.0f) {
            if (this->transitionIn == 0) {
                if (this->stationGeoms != 0) {
                    ArrayReleaseClasses(*this->stationGeoms);
                    delete this->stationGeoms;
                    this->stationGeoms = 0;
                }
                if (this->ringGeoms != 0) {
                    ArrayReleaseClasses(*this->ringGeoms);
                    delete this->ringGeoms;
                    this->ringGeoms = 0;
                }
                delete[] this->stationAngles;
                this->stationAngles = (int *) 0;
                delete[] this->stationDistances;
                this->stationDistances = (int *) 0;
                if (this->usedFlags != 0) {
                    delete this->usedFlags;
                    this->usedFlags = 0;
                }
                delete this->starSystemRoot;
                this->mode = 0;
                this->starSystemRoot = (AEGeometry *) 0;
                (*this->systemGeoms)[this->selectedSystem]->setVisible(true);
            } else {
                this->mode = 3;
            }
            this->transitionIn = 0;
            this->transitionOut = 0;
        }
        return;
    }

    ((FModSound *) (*g_StarMap_update_sound))->setParamValue(0, 0x66, 0.0f);
    if (this->mode == 3) {
        if (this->stationCenterAnim == 0 && this->dragging == 0) {
            float vx = this->momentumFactor * this->velocityX;
            float vy = this->momentumFactor * this->velocityY;
            this->velocityX = vx;
            this->velocityY = vy;
            if (absf_update(vx) > 0.5f) {
                this->yaw += vx;
            }
            if (absf_update(vy) > 0.5f) {
                this->pitch += vy;
            }
        } else if (this->stationCenterAnim != 0 && this->selectedStation >= 0) {
            int target = 0x8000 - ((int *) this->stationAngles)[this->selectedStation];
            int diff = (int) this->yaw - target;
            this->yaw += (float) (diff < 0 ? -diff : diff) * (diff < 0 ? 0.25f : -0.25f);
            if (absf_update((float) diff) < 11.0f) {
                this->stationCenterAnim = 0;
            }
        }
        float speed = absf_update(this->velocityX + this->velocityY);
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        this->spin += speed;
        if (this->stationGeoms != 0) {
            Array<AEGeometry *> *geoms = this->stationGeoms;
            for (uint32_t i = 0; i < geoms->size(); i++) {
                if ((*geoms)[i] != 0) {
                    (*geoms)[i]->rotate((float) dt * 0.001f, 0.0f, 0.001f);
                }
            }
        }
        this->yaw = (float) ((int) this->yaw & 0xffff);
        if (this->pitch < -90.0f) {
            this->pitch = -90.0f;
        }
        if (this->pitch > 90.0f) {
            this->pitch = 90.0f;
        }
        this->starSystemRoot->setRotation(this->pitch, 0.0f, this->yaw);
        return;
    }

    if (this->mode == 0) {
        if (this->pulseSystem >= 0 && this->autoMode != 0 &&
            this->autoTimer < 4000) {
            this->autoTimer += dt;
            if (this->autoTimer > 3999) {
                OnTouchBegin(*g_StarMap_update_screenW >> 1, *g_StarMap_update_screenH >> 1);
            }
            float scale = ((float) this->autoTimer / 4000.0f) * 1.5f;
            (*this->systemGeoms)[this->pulseSystem]->setScaling(scale);
        }
        if (this->dragging == 0) {
            float vx = this->momentumFactor * this->velocityX;
            float vy = this->momentumFactor * this->velocityY;
            this->velocityX = vx;
            this->velocityY = vy;
            if (absf_update(vx) > 0.5f) {
                this->panX = (int) ((float) this->panX + vx);
            }
            if (absf_update(vy) > 0.5f) {
                this->panY = (int) ((float) this->panY + vy);
            }
        }
        PaintCanvas *canvas = Globals::Canvas;
        __builtin_memcpy(
            &matrix, ((PaintCanvas *) (long) (canvas))->CameraGetLocal(
                ((PaintCanvas *) (long) (canvas))->CameraGetCurrent()), 0x3c);
        MatrixSetTranslation(&matrix,
                             (this->cameraBaseX + (float) this->panX) * 20.0f,
                             0.0f,
                             (this->cameraBaseZ + (float) this->panY) * 20.0f);
        ((PaintCanvas *) (long) (canvas))->CameraSetLocal(((PaintCanvas *) (long) (canvas))->CameraGetCurrent(),
                                                          *(const AbyssEngine::AEMath::Matrix *) (&matrix));
        if (this->pathAnim != 0) {
            Array<uint8_t> *vis = (Array<uint8_t> *) Globals::status->getSystemVisibilities();
            uint32_t selected = this->selectedSystem;
            if (vis != 0 && selected < vis->size() && (*vis)[selected] != 0) {
                this->scratchVector = *(*this->systemPositions)[selected];
                float targetX = (float) (*g_StarMap_update_screenW >> 1);
                float targetY = (float) (*g_StarMap_update_screenH >> 1);
                float sx = (this->scratchVector.x - targetX) / -30.0f;
                float sy = (this->scratchVector.y - targetY) / -30.0f;
                this->velocityX = sx;
                this->velocityY = sy;
                if (absf_update(sx) <= 2.0f && absf_update(sy) <= 2.0f) {
                    this->pathAnim = 0;
                    this->lastSelectedSystem = this->selectedSystem;
                }
            }
        }
    }
}

static inline float absf_local(float v) {
    return v > 0.0f ? v : -v;
}

uint32_t StarMap::OnTouchBegin(int x, int y) {
    if (this->choiceVisible != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
        return 0;
    }
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return 0;
    }

    Globals::layout->OnTouchBegin(x, y);
    if ((this->pad_0xa8_a != 0 && this->pathAnim != 0) || this->stationCenterAnim != 0) {
        return 0;
    }
    this->backButton->OnTouchBegin(x, y);
    if (Globals::layout->field_0xc_leftMargin >= y || y >= Globals::h - Globals::layout->field_0x10_rightMargin) {
        return 0;
    }
    if (this->autoMode != 0 && this->autoTimer < 4000) {
        return 0;
    }

    Globals::sound->stop(0x66);
    Globals::sound->play(0x66, 0, 0, 0.0f);

    float fx = (float) x;
    float fy = (float) y;
    this->lastTouchX = fx;
    this->lastTouchY = fy;
    this->touchStartX = fx;
    this->touchStartY = fy;
    this->touchDeltaX = 0.0f;
    this->touchDeltaY = 0.0f;
    this->dragging = 1;
    this->pathAnim = 0;

    int oldSystem = this->selectedSystem;
    if (this->mode == 0) {
        this->selectedSystem = -1;
        this->jumpCost = 0;
        for (uint32_t i = 0; i < this->systemGeoms->size(); i++) {
            Array<bool> *visible = Globals::status->getSystemVisibilities();
            if (visible == 0 || visible->size() <= i || !(*visible)[i])
                continue;
            this->scratchVector = *(*this->systemPositions)[i];
            if (this->scratchVector.z > 0.0f && absf_local(this->scratchVector.x - fx) < (float) this->hitRadius &&
                absf_local(this->scratchVector.y - fy) < (float) this->hitRadius) {
                this->selectedSystem = i;
                if (this->stations != 0) {
                    ArrayReleaseClasses(*this->stations);
                    delete this->stations;
                    this->stations = (Array<Station *> *) 0;
                }
                this->stations = new Array<Station *>();
                FileRead *reader = new FileRead();
                this->stations = reader->loadStationsBinary((*this->systems)[this->selectedSystem]);
                delete reader;
                if (oldSystem != this->selectedSystem) {
                    Globals::sound->play(0x67, 0, 0, 0.0f);
                }
                this->jumpCost = this->pathFinder->getJumpDistance(
                        this->systems, ((SolarSystem *) Globals::status->getSystem())->getIndex(), this->selectedSystem);
                if (this->jumpCost == 0 &&
                    ((SolarSystem *) Globals::status->getSystem())->getIndex() != this->selectedSystem) {
                    this->jumpCost = 4;
                    if ((*this->systems)[this->selectedSystem]->getRoutes() == 0) {
                        this->noRoute = 1;
                    }
                }
                if (Globals::status->hardCoreMode()) {
                    this->jumpCost <<= 1;
                }
                return 0;
            }
        }
    } else if (this->mode == 3) {
        for (uint32_t i = 0; i < this->stationPositions->size(); i++) {
            this->scratchVector = *(*this->stationPositions)[i];
            if (this->scratchVector.z > 0.0f && absf_local(this->scratchVector.x - fx) < (float) this->hitRadius &&
                absf_local(this->scratchVector.y - fy) < (float) this->hitRadius) {
                this->selectedStation = i;
                Globals::sound->play(0x68, 0, 0, 0.0f);
                return 0;
            }
        }
    }
    return 0;
}

int StarMap::OnTouchMove(int x, int y) {
    if (this->choiceVisible != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return 0;
    }
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return 0;
    }
    Globals::layout->OnTouchMove(x, y);
    if ((this->pad_0xa8_a != 0 && this->pathAnim != 0) || this->stationCenterAnim != 0) {
        return 0;
    }
    this->backButton->OnTouchMove(x, y);
    if (this->dragging == 0) {
        return 0;
    }

    float fx = (float) x;
    float fy = (float) y;
    float lastX = this->lastTouchX;
    float lastY = this->lastTouchY;
    this->momentumFactor = 1.0f;
    this->lastTouchX = fx;
    this->lastTouchY = fy;
    float dx = (fx - lastX) * this->dragScale;
    float dy = this->dragScale * (fy - lastY);
    this->touchDeltaX = dx;
    this->touchDeltaY = dy;

    if (this->mode == 0) {
        // Galaxy map: pan the camera.
        float speed = absf_local(dx + dy);
        if (speed >= 10.0f) {
            speed = 10.0f;
        }
        this->panX = (int) (dx + (float) this->panX);
        this->panY = (int) (dy + (float) this->panY);
        this->spin = speed + this->spin;
        if (this->spin > 100.0f) {
            this->spin = 0.0f;
        }
        if (absf_local(this->touchStartX - fx) > 3.0f || absf_local(this->touchStartY - fy) > 3.0f) {
            this->lastSelectedSystem = -1;
            this->selectedSystem = -1;
            this->jumpCost = 0;
        }
        PaintCanvas *canvas = Globals::Canvas;
        Matrix local = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
        canvas = Globals::Canvas;
        canvas->CameraSetLocal(canvas->CameraGetCurrent(), local);
        return 0;
    }

    // Star system view: rotate the system (angles in 1/65536 turns).
    if (absf_local(this->touchStartX - fx) > 3.0f || absf_local(this->touchStartY - fy) > 3.0f) {
        this->selectedStation = -1;
        this->lastSelectedStation = -1;
    }
    float rotY = dx * 50.0f;
    float rotX = dy * -50.0f;
    float pitch = rotX + this->pitch;
    if (pitch > 8192.0f) {
        pitch = 8192.0f;
    } else if (pitch < -8192.0f) {
        pitch = -8192.0f;
    }
    float yaw = (float) ((int) (rotY + this->yaw) % 65536);
    this->touchDeltaX = rotY;
    this->touchDeltaY = rotX;
    this->velocityX = absf_local(rotY) > 150.0f ? rotY : 0.0f;
    this->velocityY = absf_local(rotX) > 150.0f ? rotX : 0.0f;
    this->yaw = yaw;
    this->pitch = pitch;
    this->starSystemRoot->setRotation(pitch / 65536.0f * 6.28318548f, yaw / 65536.0f * 6.28318548f, -0.0981747732f);
    return 0;
}

static inline float absf_end(float v) {
    return v > 0.0f ? v : -v;
}

// Race of each ship hull (indexed by Ship::getIndex()); a campaign mission needs a race-1 hull.
static const int StarMap_shipRaces[64] = {
        3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0, 2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
        8, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 2, 1, 3, 3, 3, 3, 1, 1, 0, 8, 1, 1, 0, 3, 2, 0, 0, 8, 1, 3, 1};

int StarMap::OnTouchEnd(int x, int y) {
    if (this->choiceVisible != 0) {
        int answer = this->choiceWindow->OnTouchEnd(x, y);
        if (answer == 1) {
            this->choiceVisible = 0;
            this->alienJumpPending = 0;
            return 0;
        }
        if (answer != 0) {
            return 0;
        }
        this->choiceVisible = 0;
        if (this->suppressNextClose != 0 ||
            (this->mode == 3 && (*this->stations)[this->selectedStation]->getIndex() ==
                                Globals::status->getStation()->getIndex())) {
            this->suppressNextClose = 0;
            return 0;
        }
        if (this->alienJumpPending != 0 && this->mode == 0 && this->pad_0xa8_a == 0 && this->autoMode == 0) {
            // Confirmed jump into the alien world.
            Level::programmedStation = Globals::status->playerStation;
            this->alienJumpPending = 0;
            this->exitRequested = 1;
            Level::doInstantJump = 1;
            Level::energyCellsForNextJump = this->jumpCost;
            Globals::Canvas->CameraSetCurrent(this->prevCamera);
            return 1;
        }
        if (this->pad_0xa8_a != 0 || this->mode != 3) {
            return 0;
        }
        if (this->jumpMapModeB != 0 && this->cargoAmount < this->jumpCost) {
            if (this->jumpCost != 1) {
                return 0;
            }
            if (this->jumpMapModeA == 0) {
                this->depart(false);
                return 0;
            }
            Level::doInstantJump = 0;
            Level::programmedStation = (*this->stations)[this->selectedStation];
            this->exitRequested = 1;
            Globals::Canvas->CameraSetCurrent(this->prevCamera);
            return 1;
        }
        if (this->jumpMapModeA == 0) {
            this->depart(true);
            return 0;
        }
        Level::programmedStation = (*this->stations)[this->selectedStation];
        if (this->jumpMapModeB == 0) {
            Level::doInstantJump = 0;
        } else {
            int toSystem = (*this->stations)[this->selectedStation]->getSystem();
            int current = ((SolarSystem *) Globals::status->getSystem())->getIndex();
            Level::doInstantJump = toSystem != current;
            if (toSystem != current) {
                Level::energyCellsForNextJump = this->jumpCost;
            }
        }
        this->exitRequested = 1;
        Globals::Canvas->CameraSetCurrent(this->prevCamera);
        return 1;
    }

    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return 0;
    }

    if (Globals::layout->choiceWindowOpen == 0 && Globals::layout->OnTouchEnd(x, y) != 0) {
        // Back: leave the star system view, or leave the map.
        if (this->mode == 3 && this->isGalaxyMode) {
            this->transitionOut = 1;
            this->momentumFactor = 0.0f;
            this->velocityX = 0.0f;
            this->velocityY = 0.0f;
            this->easeX->SetRange(this->easeX->GetMaxValue(), this->easeX->GetMinValue());
            this->easeY->SetRange(this->easeY->GetMaxValue(), this->easeY->GetMinValue());
            this->easeZ->SetRange(this->easeZ->GetMaxValue(), this->easeZ->GetMinValue());
            Globals::sound->play(0x6b, 0, 0, 0.0f);
            return 0;
        }
        Globals::Canvas->CameraSetCurrent(this->prevCamera);
        Globals::sound->stop(0x66);
        return 1;
    }

    if (this->pad_0xa8_a != 0 && this->pathAnim != 0) {
        return 0;
    }
    if (this->backButton->OnTouchEnd(x, y)) {
        this->showKey = this->showKey ^ 1;
    }

    if (this->dragging != 0) {
        this->momentumFactor = 0.9f;
        this->dragging = 0;
        this->velocityX = absf_end(this->touchDeltaX) > 3.0f ? this->touchDeltaX : 0.0f;
        this->velocityY = absf_end(this->touchDeltaY) > 3.0f ? this->touchDeltaY : 0.0f;

        if (this->mode == 0) {
            if (this->selectedSystem >= 0) {
                if (this->autoMode != 0 || this->pad_0xa8_a != 0 || this->lastSelectedSystem != this->selectedSystem) {
                    this->lastSelectedSystem = -1;
                    this->pathAnim = 1;
                } else {
                    if (this->jumpMapModeB == 0 &&
                        !((SolarSystem *) Globals::status->getSystem())
                                 ->systemIsInSystemRoutes((*this->systems)[this->selectedSystem]->getIndex())) {
                        this->choiceWindow->set(*Globals::gameText->getText(420), false);
                        this->choiceVisible = 1;
                        return 0;
                    }
                    if (this->jumpMapModeB != 0 && Globals::status->getShip()->hasVolatileGoods() &&
                        !((SolarSystem *) Globals::status->getSystem())
                                 ->systemIsInSystemRoutes((*this->systems)[this->selectedSystem]->getIndex())) {
                        this->choiceWindow->set(*Globals::gameText->getText(612), false);
                        this->choiceVisible = 1;
                        return 0;
                    }
                    // Zoom into the selected system.
                    Globals::sound->play(0x6a, 0, 0, 0.0f);
                    this->initStarSystem();
                    this->transitionIn = 1;
                    this->scratchVector = (*this->systemGeoms)[this->selectedSystem]->getPosition();
                    this->scratchVector.z += -500.0f;
                    PaintCanvas *canvas = Globals::Canvas;
                    this->scratchVector2 = AbyssEngine::AEMath::MatrixGetPosition(
                            *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent()));
                    this->easeX->SetRange(this->scratchVector2.x, this->scratchVector.x);
                    this->easeY->SetRange(this->scratchVector2.y, this->scratchVector.y);
                    this->easeZ->SetRange(this->scratchVector2.z, this->scratchVector.z);
                    this->momentumFactor = 0.0f;
                    this->velocityX = 0.0f;
                    this->velocityY = 0.0f;
                    return 0;
                }
            }
        } else {
            if (this->selectedStation >= 0) {
                if (this->lastSelectedStation != this->selectedStation) {
                    Globals::sound->play(0x69, 0, 0, 0.0f);
                    this->stationCenterAnim = 1;
                } else if (this->pad_0xa8_a == 0) {
                    if ((*this->stations)[this->selectedStation]->getIndex() ==
                        Globals::status->getStation()->getIndex()) {
                        if (this->choiceWindow == 0) {
                            this->choiceWindow = new ChoiceWindow();
                        }
                        this->choiceWindow->set(*Globals::gameText->getText(419));
                        this->choiceVisible = 1;
                    } else {
                        if (this->choiceWindow == 0) {
                            this->choiceWindow = new ChoiceWindow();
                        }
                        // Campaign missions refuse to leave without the required gear.
#define STARMAP_AT_CAMPAIGN_TARGET(n)                                                                      \
    (Globals::status->getCurrentCampaignMission() == (n) &&                                                \
     (*this->stations)[this->selectedStation]->getIndex() ==                                               \
             ((Mission *) Globals::status->getCampaignMission())->getTargetStation())
                        int refusal = -1;
                        if (STARMAP_AT_CAMPAIGN_TARGET(24) &&
                            (!Globals::status->getShip()->getFirstEquipmentOfSort(13) ||
                             !Globals::status->getShip()->getFirstEquipmentOfSort(17))) {
                            refusal = 532;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(135) &&
                                   !Globals::status->getShip()->getFirstEquipmentOfSort(19)) {
                            refusal = 3213;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(91) &&
                                   Globals::status->getShip()->getMaxPassengers() <= 9) {
                            refusal = 3214;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(94) &&
                                   Globals::status->getShip()->getMaxPassengers() == 0) {
                            refusal = 3214;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(105) &&
                                   !Globals::status->getShip()->hasEquipment(206, 1)) {
                            refusal = 3217;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(139) && Globals::status->getShip()->getIndex() != 42 &&
                                   (StarMap_shipRaces[Globals::status->getShip()->getIndex()] != 1 ||
                                    !Globals::status->getShip()->hasEquipment(190, 1))) {
                            refusal = 3215;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(142) &&
                                   (!Globals::status->getShip()->getFirstEquipmentOfSort(33) ||
                                    !Globals::status->getShip()->hasEquipment(197, 15) ||
                                    !Globals::status->getShip()->getFirstEquipmentOfSort(35))) {
                            refusal = 3216;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(142) && Globals::status->getShip()->getFreeSpace() == 0) {
                            refusal = 3218;
                        }
#undef STARMAP_AT_CAMPAIGN_TARGET
                        if (refusal >= 0) {
                            this->choiceWindow->set(*Globals::gameText->getText(refusal));
                            this->suppressNextClose = 1;
                        } else if (this->jumpMapModeB != 0 && this->cargoAmount < this->jumpCost &&
                                   !Globals::status->getShip()->hasVolatileGoods()) {
                            if (this->jumpCost == 1) {
                                this->choiceWindow->set(*Globals::gameText->getText(582), true);
                            } else {
                                this->choiceWindow->set(*Globals::gameText->getText(579));
                            }
                        } else if (this->jumpMapModeB != 0 && this->noRoute != 0 &&
                                   this->cargoAmount < this->jumpCost * 2 &&
                                   !Globals::status->getShip()->hasVolatileGoods()) {
                            this->choiceWindow->set(*Globals::gameText->getText(581), true);
                        } else {
                            this->choiceWindow->set(*Globals::gameText->getText(574) + String(": ") +
                                                            (*this->stations)[this->selectedStation]->getName() +
                                                            String("\n") + *Globals::gameText->getText(421),
                                                    true);
                        }
                        this->choiceVisible = 1;
                    }
                }
            }
            this->velocityX = absf_end(this->touchDeltaX) > 150.0f ? this->touchDeltaX : 0.0f;
            this->velocityY = absf_end(this->touchDeltaY) > 150.0f ? this->touchDeltaY : 0.0f;
        }
    }

    if (Globals::layout->helpPressed()) {
        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(this->mode == 0 ? 628 : 631), false));
    }
    return 0;
}
