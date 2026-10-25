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
#include "game/ship/Agent.h"
#include "game/mission/PendingProduct.h"


static Status **g_StarMap_draw_status;
static Layout **g_StarMap_draw_layout;
static GameText **g_StarMap_draw_text;
static unsigned char **g_StarMap_draw_font;



static Layout **g_StarMap_end_layout;
static FModSound **g_StarMap_end_sound;
static GameText **g_StarMap_end_text;

int Station_getIndex(void *station); // lint: void_ptr (external symbol; mangling must match lib)

static inline Vector StarMap_MatrixGetPosition(const AbyssEngine::AEMath::Matrix &matrix) {
    return AbyssEngine::AEMath::MatrixGetPosition(matrix);
}

static bool StarMap_GetScreenPosition(PaintCanvas *canvas, const Vector &position, Vector *screenPosition) {
    if (canvas == 0)
        return false;
    return canvas->GetScreenPosition(position, *screenPosition) != 0;
}


void MatrixSetTranslation(void *matrix, float x, float y, float z); // lint: void_ptr (external symbol; mangling must match lib)

// lint: void_ptr (external symbol; mangling must match lib)

static inline void VectorNormalize(Vector *out, Vector *value) { *out = AbyssEngine::AEMath::VectorNormalize(*value); }




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

    this->hitRadius = Globals::layout->field_0x88;
    this->mode = 0;
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

        if ((*this->systems)[i]->getTextureIndex() == 15 && Globals::status->getCurrentCampaignMission() <= 157) {
            if (Globals::status->getCurrentCampaignMission() <= 105)
                (*this->systemGeoms)[i]->setScaling(0.018f, 0.018f, 0.018f);
            else
                (*this->systemGeoms)[i]->setScaling(0.025f, 0.025f, 0.025f);
        } else {
            (*this->systemGeoms)[i]->setScaling(0.012f, 0.012f, 0.012f);
        }

        (*this->systemGeoms)[i]->setPosition(
                (float) ((int) ((float) (100 - (*this->systems)[i]->getX()) / 100.0f * 14000.0f) - 10000),
                (float) ((int) ((float) (100 - (*this->systems)[i]->getY()) / 100.0f * 13000.0f) - 9000),
                (float) ((int) ((float) (100 - (*this->systems)[i]->getZ()) / 100.0f * 6000.0f) + 1000));
        this->systemRoot->addChild((*this->systemGeoms)[i]->transform);
        (*this->systemPositions)[i] = new Vector();
    }

    Globals::rnd->reset();
    this->choiceVisible = 0;
    this->markerGeom = (AEGeometry *) 0;
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
#define STARMAP_ABS(v) ((v) > 0 ? (v) : -(v))
#define STARMAP_MIN(a, b) ((a) < (b) ? (a) : (b))
#define STARMAP_MAX(a, b) ((a) > (b) ? (a) : (b))

static const int StarMap_planetTextures[19] = {10031, 10032, 10033, 10034, 10035, 10036, 10037, 10038, 10039, 10040,
                                               10041, 10032, 10039, 10035, 10031, 11741, 10038, 10033, 10037};
static const int StarMap_keyTextIds[6] = {400, 401, 547, 556, 555, 274};
static const int StarMap_stationScales[28] = {320, 192, 256, 256, 192, 256, 192, 192, 320, 256, 192, 192, 320, 256,
                                              320, 256, 256, 256, 320, 192, 320, 256, 256, 0, 256, 256, 192, 256};
static const int StarMap_raceImages[10] = {1190, 1187, 1189, 1188, -1, -1, -1, -1, -1, -1};
static const int StarMap_securityColors[4][3] = {{255, 42, 0}, {255, 108, 0}, {237, 237, 0}, {0, 237, 0}};

void StarMap::init(bool jumpMapMode, Mission *mission, bool param3, int param4) {
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
        if (target == Globals::status->playerStation)
            target = 0;
        else if (target != 0)
            delete target;
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
        this->selectedSystem = -1;
        this->lastSelectedSystem = -1;
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
        this->selectedSystem = -1;
        this->targetSystem = -1;
        if (!mission->isEmpty() && (mission->isVisible() || mission->getType() == 14) &&
            (this->mode == 3 || ((SolarSystem *) Globals::status->getSystem())->getRoutes() != 0)) {
            for (uint32_t i = 0; i < this->systems->size(); i++) {
                if ((*this->systems)[i]->getStations() == 0)
                    continue;
                for (uint32_t j = 0; j < ((Array<int> *) (*this->systems)[i]->getStations())->size(); j++) {
                    if (mission->getTargetStation() == (*(Array<int> *) (*this->systems)[i]->getStations())[j]) {
                        this->targetSystem = i;
                        if (this->mode == 3) {
                            this->selectedStation = j;
                            this->lastSelectedStation = j;
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
        this->selectedSystem = this->targetSystem;
        this->momentumFactor = 0.9f;
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
    unsigned int count = *(*this->systems)[this->selectedSystem]->getStations();
    this->stations = new Array<Station *>();
    ArraySetLength(count, *this->stations);
    FileRead *reader = new FileRead();
    this->stations = reader->loadStationsBinary((*this->systems)[this->selectedSystem]);
    delete reader;

    this->stationAngles = new int[count];
    this->stationDistances = new int[count];
    this->centeredStation = -1;
    Globals::rnd->setSeed((long long) ((*this->systems)[this->selectedSystem]->getIndex() * 1000));

    this->stationGeoms = new Array<AEGeometry *>();
    ArraySetLength(count + 1, *this->stationGeoms);
    this->starSystemRoot = new AEGeometry(Globals::Canvas);
    this->usedFlags = new Array<bool>();
    ArraySetLength(this->stationGeoms->size(), *this->usedFlags);
    for (unsigned int i = 0; i < this->usedFlags->size(); i++)
        (*this->usedFlags)[i] = false;

    // Stations orbit the sun on random, non-overlapping angles.
    for (unsigned int i = 0; i < this->stationGeoms->size(); i++) {
        if (i != 0) {
            AEGeometry *geom = new AEGeometry((uint16_t) ((*this->stations)[i - 1]->getTextureIndex() + 18180),
                                              Globals::Canvas, false);
            (*this->stationGeoms)[i] = geom;
            Matrix rotation;
            bool found = false;
            int angle = 0;
            while (!found) {
                int slot = Globals::rnd->nextInt(this->usedFlags->size());
                if (!(*this->usedFlags)[slot]) {
                    (*this->usedFlags)[slot] = true;
                    found = true;
                    angle = slot * (0x10000 / this->usedFlags->size());
                }
            }
            this->stationAngles[i - 1] = angle;
            MatrixSetRotation(rotation, 0.0f, (float) angle * (1.0f / 65536.0f) * 6.2831855f, 0.0f);
            int distance = i == 1 ? 6400 : this->stationDistances[i - 2];
            distance = distance + Globals::rnd->nextInt(5600) + 1600;
            this->stationDistances[i - 1] = distance;
            Vector offset = {0.0f, 0.0f, (float) distance};
            Vector position = {0.0f, 0.0f, 0.0f};
            position = MatrixTransformVector(rotation, offset);
            (*this->stationGeoms)[i]->translate(position);
            int texture = (*this->stations)[i - 1]->getTextureIndex();
            float scale = (float) (StarMap_stationScales[texture] << 4) * (1.0f / 65536.0f);
            (*this->stationGeoms)[i]->setScaling(scale, scale, scale);
            Vector direction = (*this->stationGeoms)[i]->getPosition();
            direction = VectorNormalize(direction);
            this->starSystemRoot->addChild((*this->stationGeoms)[i]->transform);
            if (texture == 9 || texture == 16 || texture == 21 ||
                Globals::status->orbitHasPlanetRing((*this->stations)[i - 1]->getIndex())) {
                unsigned int ring = 0xffffffffu;
                Globals::Canvas->TransformCreate(ring);
                Globals::Canvas->TransformAddMesh(ring, 16853, false);
                Globals::Canvas->TransformSetLocal(ring, (*this->stationGeoms)[i]->getMatrix());
                this->starSystemRoot->addChild(ring);
            }
            if (this->markerGeom != 0 && this->selectedSystem == Globals::status->field_7c &&
                (*this->stations)[i - 1]->getIndex() == Globals::status->field_80) {
                this->centeredStation = i;
            }
        }
        if ((*this->stationGeoms)[i] != 0)
            this->starSystemRoot->addChild((*this->stationGeoms)[i]->transform);
    }
    (*this->stationGeoms)[1]->setVisible(false);

    // Orbit rings.
    this->ringGeoms = new Array<AEGeometry *>();
    ArraySetLength(count, *this->ringGeoms);
    for (unsigned int i = 0; i < this->ringGeoms->size(); i++) {
        (*this->ringGeoms)[i] = new AEGeometry(6779, Globals::Canvas, false);
        this->starSystemRoot->addChild((*this->ringGeoms)[i]->transform);
        float distance = (float) this->stationDistances[i];
        (*this->ringGeoms)[i]->setRotation(0.0f, (float) Globals::rnd->nextInt(3141) / 1000.0f, 0.0f);
        float scale = (float) ((int) distance * 2) * (1.0f / 65536.0f);
        (*this->ringGeoms)[i]->setScaling(scale, scale, scale);
    }

    this->starSystemRoot->setPosition((*this->systemGeoms)[this->selectedSystem]->getPosition());
    this->starSystemRoot->setScaling(0.0078125f, 0.0078125f, 0.0078125f);
    this->starSystemRoot->setRotation(0.0f, 90.0f, 0.0f);
    this->yaw = 4096.0f;
    this->pitch = -4096.0f;
    this->starSystemRoot->setRotation(-0.3926991f, 0.3926991f, -0.09817477f);
    this->selectedStation = -1;
    Globals::Canvas->Image2DCreate(
        (uint16_t) StarMap_raceImages[(*this->systems)[this->selectedSystem]->getRace()], this->systemNameImage);

    if (this->stationPositions != 0) {
        ArrayReleaseClasses(*this->stationPositions);
        delete this->stationPositions;
    }
    this->stationPositions = 0;
    this->stationPositions = new Array<Vector *>();
    ArraySetLength(this->stations->size(), *this->stationPositions);
    for (unsigned int i = 0; i < this->stationPositions->size(); i++) {
        Vector *position = new Vector();
        position->x = 0.0f;
        position->y = 0.0f;
        position->z = 0.0f;
        (*this->stationPositions)[i] = position;
    }

    ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(0.0f, -1.0f, -1.0f, 0x4000);
    if (this->planetGeom != 0)
        delete this->planetGeom;
    this->planetGeom = 0;
    this->planetTexture = 0xffffffffu;
    int planetTexture = StarMap_planetTextures[(*this->systems)[this->selectedSystem]->getTextureIndex()];
    if (this->selectedSystem == 27)
        planetTexture = 10036;
    Globals::Canvas->TextureCreate((uint16_t) planetTexture, this->planetTexture, false);
    this->planetGeom = new AEGeometry(6768, Globals::Canvas, false);
    this->planetGeom->setPosition((*this->systemGeoms)[this->selectedSystem]->getPosition());
    this->planetGeom->setRotation(0.0f, 0.0f, 0.0f);
    if ((*this->systems)[this->selectedSystem]->getTextureIndex() == 15) {
        // The void planet grows during the campaign.
        if (Globals::status->getCurrentCampaignMission() <= 105)
            this->planetGeom->setScaling(0.007f, 0.007f, 0.007f);
        else if (Globals::status->getCurrentCampaignMission() <= 157)
            this->planetGeom->setScaling(0.01f, 0.01f, 0.01f);
        else
            this->planetGeom->setScaling(0.004f, 0.004f, 0.004f);
    } else {
        this->planetGeom->setScaling(0.004f, 0.004f, 0.004f);
    }
    (*this->systemGeoms)[this->selectedSystem]->setVisible(false);

    Vector sun = this->planetGeom->getPosition();
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightPosition(sun.x, sun.y, sun.z, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorAmbient(0.2f, 0.2f, 0.2f, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(2.0f, 2.0f, 2.0f, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorAmbient(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorDiffuse(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorSpecular(0.0f, 0.0f, 0.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorShininess(10.0f);
    ((Engine *) Globals::appManager->GetEngine())->lightCount = 1;
    Globals::rnd->setSeed(Globals::status->getPlayingTime());
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
    int boxW = this->keyBoxWidth;
    int screenW = Globals::w;
    int indent = layout->field_0x2c_rowHeight;
    int bottomMargin = layout->field_0x10_rightMargin;
    int boxH = this->keyBoxHeight;
    int padY = layout->windowTopInset;
    int lineSpacing = layout->field_0x4;

    int x = screenW - boxW;
    int iconX = x + indent;
    int textX = iconWidth + indent + iconX;
    int y = screenH - indent - bottomMargin - lineSpacing;
    layout->drawBox(7, x, screenH - bottomMargin - boxH - padY, boxW, padY + boxH, String(""));

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
    if (stationMode)
        this->scratchVector = Vector(*(*this->stationPositions)[index]);
    else
        this->scratchVector = Vector(*(*this->systemPositions)[index]);
    if (this->scratchVector.x < -50.0f || this->scratchVector.x > (float) (Globals::w + 50) ||
        this->scratchVector.y < -50.0f || this->scratchVector.y > (float) (Globals::h + 50))
        return;

    Globals::Canvas->SetColor(0xffffffffu);
    for (int i = 0; i != 5; i++)
        this->iconBuffer[i] = -1;

    // Key icons: discovered, wanted/campaign target, freelance target, jump gate, pending product.
    if (stationMode ? (int) (*this->stations)[index]->isDiscovered() : (*this->systems)[index]->isFullyDiscovered())
        this->iconBuffer[0] = this->keyImageDiscovered;
    Mission *campaign = (Mission *) Globals::status->getCampaignMission();
    Mission *freelance = Globals::status->getFreelanceMission();
    if (Globals::status->getCurrentCampaignMission() == 52 &&
        (stationMode ? (*this->stations)[index]->getIndex() == 74
                     : (int) (*this->systems)[index]->getStationEnumIndex(74) >= 0))
        this->iconBuffer[1] = this->keyImageWanted;
    if (Globals::status->getCurrentCampaignMission() == 116 && stationMode &&
        (*this->stations)[index]->getIndex() >= 90 && (*this->stations)[index]->getIndex() <= 94 &&
        !(((Mission *) Globals::status->getCampaignMission())->getStatusValue() &
          (1 << ((*this->stations)[index]->getIndex() - 90)))) {
        this->iconBuffer[1] = this->keyImageWanted;
    } else if (Globals::status->getCurrentCampaignMission() == 120 &&
               (stationMode ? (*this->stations)[index]->getIndex() == 93
                            : (int) (*this->systems)[index]->getStationEnumIndex(93) >= 0)) {
        this->iconBuffer[1] = this->keyImageWanted;
    } else if (Globals::status->getCurrentCampaignMission() == 125 && stationMode &&
               Globals::status->isFreighterMissionStation((*this->stations)[index]->getIndex()) &&
               !(((Mission *) Globals::status->getCampaignMission())->getStatusValue() &
                 (1 << Globals::status->getFreighterMissionStationBit((*this->stations)[index]->getIndex())))) {
        this->iconBuffer[1] = this->keyImageWanted;
    } else if (Globals::status->getCurrentCampaignMission() == 125 && !stationMode &&
               Globals::status->isFreighterMissionStation((*this->systems)[index]->getWarpGateIndex()) &&
               !(((Mission *) Globals::status->getCampaignMission())->getStatusValue() &
                 (1 << Globals::status->getFreighterMissionStationBit((*this->systems)[index]->getWarpGateIndex())))) {
        this->iconBuffer[1] = this->keyImageWanted;
    }

    if (campaign != 0) {
        bool targeted = false;
        if (campaign->getType() == 163 && Globals::status->field_90 != 0) {
            for (unsigned int i = 0; i < Globals::status->field_90->size(); i++) {
                if (stationMode ? (*Globals::status->field_90)[i] == (*this->stations)[index]->getIndex()
                                : (int) (*this->systems)[index]->getStationEnumIndex((*Globals::status->field_90)[i]) > -1) {
                    targeted = true;
                    break;
                }
            }
        }
        if (!campaign->isEmpty() && Globals::status->getCurrentCampaignMission() != 52 &&
            Globals::status->getCurrentCampaignMission() != 120 && Globals::status->getCurrentCampaignMission() != 128 &&
            Globals::status->getCurrentCampaignMission() != 130 &&
            !(Globals::status->getCurrentCampaignMission() >= 148 && Globals::status->getCurrentCampaignMission() < 152) &&
            !(Globals::status->getCurrentCampaignMission() == 59 &&
              (stationMode ? (*this->stations)[index]->getIndex() == 101 : (*this->systems)[index]->getIndex() == 23))) {
            bool const target = stationMode ? (*this->stations)[index]->getIndex() == campaign->getTargetStation()
                                            : (int) (*this->systems)[index]->getStationEnumIndex(campaign->getTargetStation()) > -1;
            if (targeted || target) {
                this->iconBuffer[1] = this->keyImageWanted;
            } else if (Globals::status->getCurrentCampaignMission() >= 33 &&
                       (stationMode ? (*this->stations)[index]->getIndex() == Globals::status->field_80
                                    : (int) (*this->systems)[index]->getStationEnumIndex(Globals::status->field_80) >= 0) &&
                       campaign->getTargetStation() == -1) {
                this->iconBuffer[1] = this->keyImageWanted;
            }
        }
    }

    if (freelance != 0 && !freelance->isEmpty() &&
        (stationMode ? (*this->stations)[index]->getIndex() == (freelance->getType() == 14
                                                                    ? freelance->getAgent()->getStation()
                                                                    : freelance->getTargetStation())
                     : (int) (*this->systems)[index]->getStationEnumIndex(
                           freelance->getType() == 14 && Globals::status->getShip()->hasCargo(115, 1)
                               ? freelance->getAgent()->getStation()
                               : freelance->getTargetStation()) >= 0))
        this->iconBuffer[2] = this->keyImageMission;
    if (stationMode && this->selectedSystem >= 0 &&
        (*this->systems)[this->selectedSystem]->getWarpGateIndex() == (*this->stations)[index]->getIndex())
        this->iconBuffer[3] = this->keyImageCurrent;
    Array<PendingProduct *> *products = (Array<PendingProduct *> *) Globals::status->getPendingProducts();
    if (products != 0) {
        for (unsigned int i = 0; i < products->size(); i++) {
            if ((*products)[i] != 0 &&
                (stationMode ? (*products)[i]->stationIndex == (*this->stations)[index]->getIndex()
                             : (int) (*this->systems)[index]->getStationEnumIndex((*products)[i]->stationIndex) >= 0)) {
                this->iconBuffer[4] = this->keyImageRetreat;
                break;
            }
        }
    }

    int textY = (int) (this->scratchVector.y + (float) (this->iconWidth >> 1) - 3.0f);
    if (stationMode) {
        int textX = (int) (this->scratchVector.x -
                           (float) (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, (*this->stations)[index]->getName()) / 2));
        if (Globals::status->getStation()->getIndex() == (*this->stations)[index]->getIndex()) {
            Globals::Canvas->SetColor(255, 255, 255,
                                      (unsigned char) ((float) (255 - this->alpha) * Globals::layout->getPulseValue(0.005f)));
            Globals::Canvas->DrawImage2D(this->currentMarkerIcon, (int) this->scratchVector.x, (int) this->scratchVector.y,
                                         17, 68);
        }
        if (this->selectedStation == index) {
            Globals::Canvas->SetColor(255, 128, 0, this->alpha ^ 255);
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, (*this->stations)[index]->getName(), textX, textY, false);
            Globals::Canvas->SetColor(255, 255, 255, this->alpha ^ 255);
            if ((*this->stations)[index]->getTecLevel() >= 1) {
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                            *Globals::gameText->getText(133) + String(": ") +
                                            String(String((*this->stations)[index]->getTecLevel()), false),
                                            textX, textY + Globals::layout->field_0x4, false);
            }
            Globals::Canvas->DrawImage2D(this->selectIcon, (int) this->scratchVector.x, (int) this->scratchVector.y, 17,
                                         68);
        } else {
            Globals::Canvas->SetColor(255, 255, 255, this->alpha ^ 255);
            Globals::Canvas->DrawImage2D(this->cursorIcon, (int) this->scratchVector.x, (int) this->scratchVector.y, 17,
                                         68);
            if (this->selectedStation >= 0)
                Globals::Canvas->SetColor(255, 255, 255, STARMAP_MIN(255 - this->alpha, 64));
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, (*this->stations)[index]->getName(), textX, textY, false);
        }
    } else {
        int textX = (int) (this->scratchVector.x -
                           (float) (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, (*this->systems)[index]->getName()) / 2));
        if (this->pad_0xa8_a)
            (*this->systems)[index]->getIndex();
        if (((SolarSystem *) Globals::status->getSystem())->getIndex() == (*this->systems)[index]->getIndex()) {
            Globals::Canvas->SetColor(255, 255, 255,
                                      (unsigned char) ((float) this->alpha * Globals::layout->getPulseValue(0.005f)));
            Globals::Canvas->DrawImage2D(this->currentMarkerIcon, (int) this->scratchVector.x, (int) this->scratchVector.y,
                                         17, 68);
        }
        Globals::Canvas->SetColor(255, 255, 255, this->alpha);
        if (this->selectedSystem == index)
            Globals::Canvas->SetColor(255, 128, 0, this->alpha);
        else if (this->selectedSystem >= 0)
            Globals::Canvas->SetColor(255, 255, 255, STARMAP_MIN(this->alpha, 64));
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, (*this->systems)[index]->getName(), textX, textY, false);
        Globals::Canvas->SetColor(255, 255, 255,
                                  this->selectedSystem < 0 || this->selectedSystem == index
                                      ? this->alpha
                                      : STARMAP_MIN(this->alpha, 64));
        if (!(*this->systems)[index]->hasNoOwner()) {
            int race = (*this->systems)[index]->getRace();
            uint32_t raceImage = race == 0   ? this->raceImageNeutral
                                 : race == 1 ? this->raceImageA
                                 : race == 2 ? this->raceImageB
                                             : this->raceImageDefault;
            Globals::Canvas->DrawImage2D(raceImage, textX, textY - 3, 17, 18);
        }
        if (index == 26 && Globals::status->field_114 == 3)
            Globals::Canvas->DrawImage2D(this->image_0x134, textX,
                                         (int) ((float) textY + (float) Globals::layout->field_0x2c_rowHeight * 0.5f),
                                         17, 18);
        int selected = this->selectedSystem;
        Globals::Canvas->SetColor(255, 255, 255, this->alpha);
        if (selected == index) {
            Globals::Canvas->DrawImage2D(this->selectIcon, (int) this->scratchVector.x, (int) this->scratchVector.y, 17,
                                         68);
            int lineOffset = Globals::layout->field_0x4;
            if (!(*this->systems)[index]->hasNoOwner()) {
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(406 + (*this->systems)[this->selectedSystem]->getRace()),
                    textX, textY + Globals::layout->field_0x4, false);
                lineOffset = Globals::layout->field_0x4 * 2;
            }
            int security = (*this->systems)[this->selectedSystem]->getSecurityLevel();
            if (index == 26 && Globals::status->field_114 > 1)
                security = 3;
            Globals::Canvas->SetColor(StarMap_securityColors[security][0], StarMap_securityColors[security][1],
                                      StarMap_securityColors[security][2], this->alpha);
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(402 + security), textX,
                                        textY + lineOffset, false);
            Globals::Canvas->SetColor(255, 255, 255, this->alpha);
        } else {
            Globals::Canvas->DrawImage2D(this->cursorIcon, (int) this->scratchVector.x, (int) this->scratchVector.y, 17,
                                         68);
            if (this->selectedSystem >= 0)
                Globals::Canvas->SetColor(255, 255, 255, STARMAP_MIN(this->alpha, 64));
        }
    }

    int iconX = (int) (this->scratchVector.x + (float) (this->iconWidth >> 1) - 7.0f);
    int iconY = (int) (this->scratchVector.y - (float) (this->iconWidth >> 1) + 10.0f);
    for (int i = 0; i != 5; i++) {
        if (this->iconBuffer[i] == -1)
            continue;
        if (i == 0) {
            Globals::Canvas->DrawImage2D(this->iconBuffer[i], iconX - (Globals::iPadHD ? 12 : 18),
                                         iconY + this->iconWidth + (Globals::iPadHD ? -24 : -35));
        } else {
            Globals::Canvas->DrawImage2D(this->iconBuffer[i], iconX, iconY);
            iconY += this->missionIconWidth;
        }
    }
}

// Security level colours (RGB) for the system info line.

void StarMap::draw() {
    this->alpha = this->mode == 0 ? 255 : 0;
    if (this->transitionIn || this->transitionOut) {
        float t = (this->easeZ->GetValue() - this->easeZ->GetMinValue()) /
                  (this->easeZ->GetMaxValue() - this->easeZ->GetMinValue());
        this->alpha = (int) ((this->transitionIn ? 1.0f - t : t) * 255.0f);
    }

    if (this->mode != 3 || this->transitionIn || this->transitionOut) {
        // Galaxy: faint route network.
        Globals::Canvas->SetColor(255, 255, 255, (uint8_t) (this->alpha < 34 ? this->alpha : 34));
        for (uint32_t i = 0; i < this->systemGeoms->size(); i++) {
            Array<bool> *visible = Globals::status->getSystemVisibilities();
            if (visible == 0 || visible->size() <= i || !(*visible)[i])
                continue;
            if (this->autoMode != 0 && this->pulseSystem == (int) i && this->autoTimer < 4000)
                continue;
            Array<int> *routes = (Array<int> *) (*this->systems)[i]->getRoutes();
            if (routes == 0)
                continue;
            this->scratchVector = Vector(*(*this->systemPositions)[i]);
            for (uint32_t j = 0; j < routes->size(); j++) {
                uint32_t to = (*routes)[j];
                if (visible->size() <= to || !(*visible)[to] || to >= this->systemPositions->size())
                    continue;
                if (this->autoMode != 0 && this->pulseSystem == (int) to && this->autoTimer < 4000)
                    continue;
                this->scratchVector2 = Vector(*(*this->systemPositions)[to]);
                if (this->scratchVector.z < 0.0f && this->scratchVector2.z < 0.0f)
                    continue;
                Globals::Canvas->DrawLine((int) this->scratchVector.x, (int) this->scratchVector.y,
                                          (int) this->scratchVector2.x, (int) this->scratchVector2.y);
            }
        }

        // Routes out of the current system pulse.
        Globals::Canvas->SetColor(255, 255, 255, (uint8_t) this->alpha);
        this->scratchVector2 = Vector(*(*this->systemPositions)[((SolarSystem *) Globals::status->getSystem())->getIndex()]);
        Array<int> *routes = (Array<int> *) ((SolarSystem *) Globals::status->getSystem())->getRoutes();
        for (uint32_t i = 0; i < this->systems->size(); i++) {
            Array<bool> *visible = Globals::status->getSystemVisibilities();
            if (visible == 0 || i >= this->systemPositions->size() || visible->size() <= i || !(*visible)[i])
                continue;
            this->scratchVector = Vector(*(*this->systemPositions)[i]);
            bool hidden = this->scratchVector.z < 0.0f && this->scratchVector2.z < 0.0f;
            if (routes != 0 && !hidden && this->jumpMapModeB == 0) {
                for (uint32_t j = 0; j < routes->size(); j++) {
                    if ((uint32_t) (*routes)[j] == i) {
                        float pulse = (float) (101 - Globals::status->getPlayingTime() / 10 % 99) / 100.0f;
                        int fade = (int) (pulse * 255.0f);
                        Globals::Canvas->SetColor(255, 255, 255, (uint8_t) (this->alpha < fade ? this->alpha : fade));
                        Globals::Canvas->DrawLine(
                                (int) this->scratchVector2.x, (int) this->scratchVector2.y,
                                (int) (this->scratchVector.x + pulse * (this->scratchVector2.x - this->scratchVector.x)),
                                (int) (this->scratchVector.y + pulse * (this->scratchVector2.y - this->scratchVector.y)));
                        break;
                    }
                }
            }
            if (!this->transitionIn && !this->transitionOut) {
                this->drawOnScreenInfo(i, false);
            }
        }

        // Animated planned route.
        if (this->mode == 0 && !this->transitionIn && !this->transitionOut && this->systemPath != 0) {
            float pulse = (float) (101 - Globals::status->getPlayingTime() / 10 % 99) / 100.0f;
            int step = this->field_0xe8;
            if (pulse > *(float *) &this->field_0xec) {
                step++;
                this->field_0xe8 = step;
                step = (uint32_t) step < this->systemPath->size() ? step : 0;
                this->field_0xe8 = step;
            }
            *(float *) &this->field_0xec = pulse;
            if ((uint32_t) step == this->systemPath->size() - 1) {
                Globals::Canvas->SetColor(255, 255, 0, (uint8_t) (int) (pulse * 255.0f));
            } else {
                Globals::Canvas->SetColor(0xffff00ffu);
            }
            for (int i = 0; i < this->field_0xe8; i++) {
                this->scratchVector2 = Vector(*(*this->systemPositions)[(*this->systemPath)[i]]);
                this->scratchVector = Vector(*(*this->systemPositions)[(*this->systemPath)[i + 1]]);
                Globals::Canvas->DrawLine((int) this->scratchVector2.x, (int) this->scratchVector2.y,
                                          (int) this->scratchVector.x, (int) this->scratchVector.y);
            }
            if ((uint32_t) this->field_0xe8 < this->systemPath->size() - 1) {
                this->scratchVector2 = Vector(*(*this->systemPositions)[(*this->systemPath)[this->field_0xe8]]);
                this->scratchVector = Vector(*(*this->systemPositions)[(*this->systemPath)[this->field_0xe8 + 1]]);
                Globals::Canvas->DrawLine(
                        (int) this->scratchVector2.x, (int) this->scratchVector2.y,
                        (int) (this->scratchVector.x + pulse * (this->scratchVector2.x - this->scratchVector.x)),
                        (int) (this->scratchVector.y + pulse * (this->scratchVector2.y - this->scratchVector.y)));
            }
        }
    }

    if (this->selectedSystem >= 0 && (this->autoMode == 0 || this->autoTimer >= 4000)) {
        this->drawOnScreenInfo(this->selectedSystem, false);
    }

    if (this->starSystemRoot != 0 && this->stations != 0) {
        SolarSystem *system = (*this->systems)[this->selectedSystem];
        if (!system->hasNoOwner()) {
            Globals::Canvas->SetColor(255, 255, 255, (uint8_t) (this->alpha ^ 255));
            Globals::Canvas->DrawImage2D(this->systemNameImage, Globals::layout->field_0x2c_rowHeight,
                                Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x2c_rowHeight);
            int indent = Globals::layout->field_0x2c_rowHeight;
            int nameWidth = Globals::Canvas->GetImage2DWidth(this->systemNameImage);
            Globals::Canvas->SetColor(255, 128, 0, (uint8_t) (this->alpha ^ 255));
            int textX = nameWidth + indent * 2;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, (*this->systems)[this->selectedSystem]->getName(),
                               textX, Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x2c_rowHeight + 2,
                               false);
            Globals::Canvas->SetColor(255, 255, 255, (uint8_t) (this->alpha ^ 255));
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                               *Globals::gameText->getText((*this->systems)[this->selectedSystem]->getRace() + 406), textX,
                               Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x2c_rowHeight +
                               Globals::layout->field_0x4 + 2,
                               false);
            int security = (*this->systems)[this->selectedSystem]->getSecurityLevel();
            Globals::Canvas->SetColor((uint8_t) StarMap_securityColors[security][0], (uint8_t) StarMap_securityColors[security][1],
                             (uint8_t) StarMap_securityColors[security][2], (uint8_t) (this->alpha ^ 255));
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(security + 402),
                               textX,
                               Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x2c_rowHeight +
                               Globals::layout->field_0x4 * 2 + 2,
                               false);
        }
        for (uint32_t i = 0; i < this->stations->size(); i++) {
            if ((int) i != this->selectedStation) {
                this->drawOnScreenInfo(i, true);
            }
        }
        if (this->selectedStation > -1) {
            this->drawOnScreenInfo(this->selectedStation, true);
        }
    }

    if (this->showKey) {
        this->drawKey();
    }
    Globals::layout->drawHeader(String(*Globals::gameText->getText(177), false));
    Globals::layout->drawEmptyFooter(true);

    if ((Globals::status->getShip()->hasJumpDriveIntegrated() || this->jumpMapModeB != 0) && this->jumpCost >= 1 &&
        this->selectedSystem >= 0 && this->selectedSystem != ((SolarSystem *) Globals::status->getSystem())->getIndex()) {
        // Energy cells needed / carried.
        String line = *Globals::gameText->getText(578) + String(" ") +
                      String(String(this->jumpCost) + String(" / ") + String(this->cargoAmount), false);
        if (this->jumpCost > this->cargoAmount) {
            Globals::Canvas->SetColor(0xff2a00ffu);
        }
        Layout *layout = Globals::layout;
        int left = layout->windowX;
        int width = layout->windowWidth;
        unsigned int font = (unsigned int) (uintptr_t) Globals::font;
        int textWidth = Globals::Canvas->GetTextWidth(font, line);
        Globals::Canvas->DrawString(font, line,
                                    left + width / 2 - textWidth / 2,
                                    Globals::layout->windowY + Globals::layout->windowHeight -
                                    Globals::layout->footerTextInset,
                                    false);
        Globals::Canvas->SetColor(0xffffffffu);
    }
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

void StarMap::update(int dt) {
    this->lastDt = dt;
    if (this->mode == 0 || this->transitionIn || this->transitionOut) {
        for (unsigned int i = 0; i < this->systemPositions->size(); i++) {
            bool onScreen = StarMap_GetScreenPosition(Globals::Canvas, (*this->systemGeoms)[i]->getPosition(),
                                                       (*this->systemPositions)[i]);
            (*this->systemPositions)[i]->z = onScreen ? 1.0f : -1.0f;
        }
    }
    if (this->mode == 3 || this->transitionIn || this->transitionOut) {
        for (unsigned int i = 0; i < this->stationPositions->size(); i++) {
            this->scratchVector2 = StarMap_MatrixGetPosition(this->starSystemRoot->getMatrix() *
                                                     (*this->stationGeoms)[i + 1]->getMatrix());
            bool onScreen = StarMap_GetScreenPosition(Globals::Canvas, this->scratchVector2,
                                                       (*this->stationPositions)[i]);
            (*this->stationPositions)[i]->z = onScreen ? 1.0f : -1.0f;
        }
    }

    if (this->markerGeom != 0) {
        float t = (this->easeZ->GetValue() - this->easeZ->GetMinValue()) /
                  (this->easeZ->GetMaxValue() - this->easeZ->GetMinValue());
        if (this->transitionOut)
            t = 1.0f - t;
        else if (!this->transitionIn && this->mode == 0)
            t = 0.0f;
        float scale = (float) ((double) t * -0.014 + 0.02);
        this->markerGeom->setScaling(scale, scale, scale);
        if (this->starSystemRoot != 0 && this->centeredStation >= 0) {
            Vector system = (*this->systemGeoms)[Globals::status->field_7c]->getPosition();
            Matrix m = this->starSystemRoot->getMatrix() *
                (*this->stationGeoms)[this->centeredStation]->getMatrix();
            Vector station = StarMap_MatrixGetPosition(m);
            this->markerGeom->setPosition(system + t * (station - system));
        }
        this->scratchVector = StarMap_MatrixGetPosition(
            *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));
        this->scratchVector -= this->markerGeom->getPosition();
        this->scratchVector = VectorNormalize(this->scratchVector);
        this->scratchVector.x += 0.5f;
        this->markerGeom->setDirection(this->scratchVector, Vector{0.0f, 1.0f, 0.0f});
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->markerGeom->transform))->Update(dt, false);
    }

    if (this->transitionIn || this->transitionOut) {
        float step = (float) (dt * 15);
        this->easeX->Increase(step);
        this->easeY->Increase(step);
        this->easeZ->Increase(step);
        this->scratchVector = Vector{this->easeX->GetValue(), this->easeY->GetValue(), this->easeZ->GetValue()};
        Matrix camera = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
        MatrixSetTranslation(camera, this->scratchVector.x, this->scratchVector.y, this->scratchVector.z);
        Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(), camera);
        if (STARMAP_ABS(this->scratchVector.x - this->easeX->GetMaxValue()) <= 1.0f &&
            STARMAP_ABS(this->scratchVector.y - this->easeY->GetMaxValue()) <= 1.0f &&
            STARMAP_ABS(this->scratchVector.z - this->easeZ->GetMaxValue()) <= 1.0f) {
            if (this->transitionIn) {
                this->mode = 3;
            } else {
                // Zoomed back out of the star system.
                ArrayReleaseClasses(*this->stationGeoms);
                delete this->stationGeoms;
                this->stationGeoms = 0;
                ArrayReleaseClasses(*this->ringGeoms);
                delete this->ringGeoms;
                this->ringGeoms = 0;
                delete[] this->stationAngles;
                this->stationAngles = 0;
                delete[] this->stationDistances;
                this->stationDistances = 0;
                ArrayRelease(*this->usedFlags);
                delete this->usedFlags;
                this->usedFlags = 0;
                delete this->starSystemRoot;
                this->starSystemRoot = 0;
                this->mode = 0;
                (*this->systemGeoms)[this->selectedSystem]->setVisible(true);
            }
            this->transitionIn = 0;
            this->transitionOut = 0;
        }
        return;
    }

    Globals::sound->setParamValue(0, 102, this->spin);
    switch (this->mode) {
    case 0: {
        if (this->pulseSystem >= 0 && this->autoMode && this->autoTimer < 4000) {
            this->autoTimer += dt;
            if (this->autoTimer >= 4000)
                this->OnTouchBegin(Globals::w >> 1, Globals::h >> 1);
            float scale = (float) ((double) ((float) this->autoTimer / 4000.0f) * 0.002);
            (*this->systemGeoms)[this->pulseSystem]->setScaling(scale, scale, scale);
        }
        if (!this->dragging) {
            this->velocityX = this->momentumFactor * this->velocityX;
            this->velocityY = this->momentumFactor * this->velocityY;
            if (STARMAP_ABS(this->velocityX) > 0.5f)
                this->panX = (int) (this->velocityX + (float) this->panX);
            if (STARMAP_ABS(this->velocityY) > 0.5f)
                this->panY = (int) (this->velocityY + (float) this->panY);
        }
        // Spring back when panned past the map edges.
        int x = (int) (this->cameraBaseX + (float) this->panX);
        int y = (int) (this->cameraBaseZ + (float) this->panY);
        if (x > 120)
            this->velocityX = (float) (120 - x) * 0.1f;
        else if (x < -500)
            this->velocityX = (float) (-500 - x) * 0.1f;
        if (y > 140)
            this->velocityY = (float) (140 - y) * 0.1f;
        else if (y < -400)
            this->velocityY = (float) (-400 - y) * 0.1f;
        Matrix camera = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
        MatrixSetTranslation(camera, (this->cameraBaseX + (float) this->panX) * 20.0f,
                             (this->cameraBaseZ + (float) this->panY) * 20.0f, 0.0f);
        Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(), camera);
        this->spin += STARMAP_MIN(STARMAP_ABS(this->velocityX + this->velocityY), 10.0f);
        if (this->spin > 100.0f)
            this->spin = 0.0f;
        if (this->pathAnim) {
            // Scroll the selected system to the screen centre.
            Array<bool> *visible = Globals::status->getSystemVisibilities();
            if (visible != 0 && this->selectedSystem >= 0 && this->selectedSystem < visible->size() &&
                (*visible)[this->selectedSystem]) {
                this->scratchVector = *(*this->systemPositions)[this->selectedSystem];
                float centerY = (float) (Globals::h >> 1);
                float centerX = (float) (Globals::w >> 1);
                float dx = this->scratchVector.x - centerX;
                float dy = this->scratchVector.y - centerY;
                float vx = 0.0f;
                float vy = 0.0f;
                if (this->scratchVector.x < centerX)
                    vx = dx / -30.0f;
                else if (this->scratchVector.x > centerX)
                    vx = dx / -30.0f;
                if (this->scratchVector.y > centerY)
                    vy = dy / -30.0f;
                else if (this->scratchVector.y < centerY)
                    vy = dy / -30.0f;
                if (STARMAP_ABS((int) vx) > STARMAP_ABS((int) dx))
                    vx = vx * 0.5f;
                if (STARMAP_ABS((int) vy) > STARMAP_ABS((int) dy))
                    vy = vy * 0.5f;
                this->velocityX = vx;
                this->velocityY = vy;
                if (STARMAP_ABS(vx) <= 2.0f && STARMAP_ABS(vy) <= 2.0f) {
                    this->pathAnim = 0;
                    this->lastSelectedSystem = this->selectedSystem;
                }
            }
        }
        break;
    }
    case 3: {
        if (this->stationCenterAnim) {
            // Rotate the selected station to the front.
            int target = 0x8000 - this->stationAngles[this->selectedStation];
            int pitch = (int) this->pitch;
            int yaw = (int) this->yaw;
            int pitchDiff = STARMAP_ABS(pitch + 3096);
            int yawDiff = yaw - target;
            if (yawDiff < 0)
                yawDiff = -yawDiff;
            float yawStep;
            if (target > yaw)
                yawStep = (float) (target - yaw) * 0.25f;
            else
                yawStep = (float) (yaw - target) * 0.25f * -1.0f;
            this->yaw += yawStep;
            if (yawDiff <= 10)
                this->stationCenterAnim = 0;
            float pitchStep;
            if (pitch <= -3096)
                pitchStep = (float) (-3096 - pitch) * 0.25f;
            else
                pitchStep = (float) (pitch + 3096) * -0.25f;
            this->pitch += pitchStep;
            if (yawDiff <= dt * 4 && pitchDiff <= dt * 4) {
                this->lastSelectedStation = this->selectedStation;
                this->stationCenterAnim = 0;
            }
        } else if (!this->dragging) {
            this->velocityX = this->momentumFactor * this->velocityX;
            this->velocityY = this->momentumFactor * this->velocityY;
            if (STARMAP_ABS(this->velocityX) > 0.5f)
                this->yaw += this->velocityX;
            if (STARMAP_ABS(this->velocityY) > 0.5f)
                this->pitch += this->velocityY;
        }
        this->spin += STARMAP_MIN(STARMAP_ABS(this->velocityX + this->velocityY) * 0.01f, 10.0f);
        if (this->spin > 100.0f)
            this->spin = 0.0f;
        if (this->stationGeoms != 0) {
            for (unsigned int i = 0; i < this->stationGeoms->size(); i++) {
                if ((*this->stationGeoms)[i] != 0)
                    (*this->stationGeoms)[i]->rotate(0.0f, (float) dt * 0.0002f, 0.0f);
            }
        }
        this->yaw = (float) ((int) this->yaw % 0x10000);
        this->pitch = STARMAP_MIN(STARMAP_MAX(this->pitch, -8192.0f), 8192.0f);
        this->starSystemRoot->setRotation(this->pitch * (1.0f / 65536.0f) * 6.2831855f,
                                          this->yaw * (1.0f / 65536.0f) * 6.2831855f, -0.09817477f);
        break;
    }
    }

}

static inline float absf_local(float v) {
    return v > 0.0f ? v : -v;
}

bool StarMap::OnTouchBegin(int x, int y) {
    if (this->choiceVisible != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
        return false;
    }
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return false;
    }

    Globals::layout->OnTouchBegin(x, y);
    if ((this->pad_0xa8_a != 0 && this->pathAnim != 0) || this->stationCenterAnim != 0) {
        return false;
    }
    this->backButton->OnTouchBegin(x, y);
    if (Globals::layout->field_0xc_leftMargin >= y || y >= Globals::h - Globals::layout->field_0x10_rightMargin) {
        return false;
    }
    if (this->autoMode != 0 && this->autoTimer < 4000) {
        return false;
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
                return false;
            }
        }
    } else if (this->mode == 3) {
        for (uint32_t i = 0; i < this->stationPositions->size(); i++) {
            this->scratchVector = *(*this->stationPositions)[i];
            if (this->scratchVector.z > 0.0f && absf_local(this->scratchVector.x - fx) < (float) this->hitRadius &&
                absf_local(this->scratchVector.y - fy) < (float) this->hitRadius) {
                this->selectedStation = i;
                Globals::sound->play(0x68, 0, 0, 0.0f);
                return false;
            }
        }
    }
    return false;
}

bool StarMap::OnTouchMove(int x, int y) {
    if (this->choiceVisible != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return false;
    }
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return false;
    }
    Globals::layout->OnTouchMove(x, y);
    if ((this->pad_0xa8_a != 0 && this->pathAnim != 0) || this->stationCenterAnim != 0) {
        return false;
    }
    this->backButton->OnTouchMove(x, y);
    if (this->dragging == 0) {
        return false;
    }

    float fx = (float) x;
    float fy = (float) y;
    float lastX = this->lastTouchX;
    float lastY = this->lastTouchY;
    float scale = this->dragScale;
    this->momentumFactor = 1.0f;
    this->lastTouchX = fx;
    this->lastTouchY = fy;
    float dx = (fx - lastX) * scale;
    float dy = scale * (fy - lastY);
    this->touchDeltaX = dx;
    this->touchDeltaY = dy;

    if (this->mode == 0) {
        // Galaxy map: pan the camera.
        float speed = absf_local(dx + dy);
        if (!(speed < 10.0f)) {
            speed = 10.0f;
        }
        this->panX = (int) (dx + (float) this->panX);
        this->panY = (int) (dy + (float) this->panY);
        this->spin = speed + this->spin;
        if (this->spin > 100.0f) {
            this->spin = 0.0f;
        }
        if (absf_local(this->touchStartX - fx) > 3.0f || absf_local(this->touchStartY - fy) > 3.0f) {
            this->selectedSystem = -1;
            this->lastSelectedSystem = -1;
            this->jumpCost = 0;
        }
        PaintCanvas *canvas = Globals::Canvas;
        Matrix local = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
        canvas = Globals::Canvas;
        canvas->CameraSetLocal(canvas->CameraGetCurrent(), local);
        return false;
    }

    // Star system view: rotate the system (angles in 1/65536 turns).
    if (absf_local(this->touchStartX - fx) > 3.0f || absf_local(this->touchStartY - fy) > 3.0f) {
        this->selectedStation = -1;
        this->lastSelectedStation = -1;
    }
    float rotY = dx * 50.0f;
    float rotX = dy * -50.0f;
    float p = rotX + this->pitch;
    // Clamp to [-8192, 8192]; this four-select shape is what the original -Oz build emitted.
    float clamped = p > -8192.0f ? 8192.0f : -8192.0f;
    clamped = p > 8192.0f ? 8192.0f : clamped;
    float pitch = p > -8192.0f ? p : clamped;
    pitch = p > 8192.0f ? clamped : pitch;
    float yaw = (float) ((int) (rotY + this->yaw) % 65536);
    this->touchDeltaX = rotY;
    this->touchDeltaY = rotX;
    this->velocityX = absf_local(rotY) > 150.0f ? rotY : 0.0f;
    this->velocityY = absf_local(rotX) > 150.0f ? rotX : 0.0f;
    this->yaw = yaw;
    this->pitch = pitch;
    this->starSystemRoot->setRotation(pitch / 65536.0f * 6.28318548f, yaw / 65536.0f * 6.28318548f, -0.0981747732f);
    return false;
}

static inline float absf_end(float v) {
    return v > 0.0f ? v : -v;
}

// Race of each ship hull (indexed by Ship::getIndex()); a campaign mission needs a race-1 hull.
static const int StarMap_shipRaces[64] = {
        3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0, 2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
        8, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 2, 1, 3, 3, 3, 3, 1, 1, 0, 8, 1, 1, 0, 3, 2, 0, 0, 8, 1, 3, 1};

bool StarMap::OnTouchEnd(int x, int y) {
    if (this->choiceVisible != 0) {
        int answer = this->choiceWindow->OnTouchEnd(x, y);
        if (answer == 0) {
            this->choiceVisible = 0;
            if (this->suppressNextClose != 0 ||
                (this->mode == 3 && (*this->stations)[this->selectedStation]->getIndex() ==
                                       Globals::status->getStation()->getIndex())) {
                this->suppressNextClose = 0;
                return false;
            }
            if (this->alienJumpPending != 0 && this->mode == 0 && this->pad_0xa8_a == 0 && this->autoMode == 0) {
                // Confirmed jump into the alien world.
                Level::programmedStation = Globals::status->playerStation;
                this->exitRequested = 1;
                this->alienJumpPending = 0;
                Level::doInstantJump = 1;
                Level::energyCellsForNextJump = this->jumpCost;
                Globals::Canvas->CameraSetCurrent(this->prevCamera);
                return true;
            }
            if (this->pad_0xa8_a != 0 || this->mode != 3) {
                return false;
            }
            if (this->jumpMapModeB != 0 && this->cargoAmount < this->jumpCost) {
                if (this->jumpCost != 1) {
                    return false;
                }
                if (this->jumpMapModeA == 0) {
                    this->depart(false);
                    return false;
                }
                Level::programmedStation = (*this->stations)[this->selectedStation];
                Level::doInstantJump = 0;
                this->exitRequested = 1;
                Globals::Canvas->CameraSetCurrent(this->prevCamera);
                return true;
            }
            if (this->jumpMapModeA == 0) {
                this->depart(true);
                return false;
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
            return true;
        } else if (answer == 1) {
            this->alienJumpPending = 0;
            this->choiceVisible = 0;
        }
        return false;
    }

    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return false;
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
            return false;
        }
        Globals::Canvas->CameraSetCurrent(this->prevCamera);
        Globals::sound->stop(0x66);
        return true;
    }

    if (this->pad_0xa8_a != 0 && this->pathAnim != 0) {
        return false;
    }
    if (this->backButton->OnTouchEnd(x, y)) {
        this->showKey = this->showKey ^ 1;
    }

    if (this->dragging != 0) {
        this->momentumFactor = 0.9f;
        this->velocityX = absf_end(this->touchDeltaX) > 3.0f ? this->touchDeltaX : 0.0f;
        this->velocityY = absf_end(this->touchDeltaY) > 3.0f ? this->touchDeltaY : 0.0f;

        this->dragging = 0;
        if (this->mode == 0) {
            if (this->selectedSystem >= 0) {
                if (this->autoMode != 0 || this->pad_0xa8_a != 0 || this->lastSelectedSystem != this->selectedSystem) {
                    this->pathAnim = 1;
                    this->lastSelectedSystem = -1;
                } else {
                    if (this->jumpMapModeB == 0 &&
                        !((SolarSystem *) Globals::status->getSystem())
                                 ->systemIsInSystemRoutes(
                                         (*this->systems)[this->selectedSystem]->getIndex())) {
                        this->choiceWindow->set(*Globals::gameText->getText(420), false);
                        this->choiceVisible = 1;
                        return false;
                    } else if (this->jumpMapModeB != 0 && Globals::status->getShip()->hasVolatileGoods() &&
                               !((SolarSystem *) Globals::status->getSystem())
                                        ->systemIsInSystemRoutes(
                                                (*this->systems)[this->selectedSystem]->getIndex())) {
                        this->choiceWindow->set(*Globals::gameText->getText(612), false);
                    } else {
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
                        return false;
                    }
                    this->choiceVisible = 1;
                    return false;
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
                        if (STARMAP_AT_CAMPAIGN_TARGET(24) &&
                            (!Globals::status->getShip()->getFirstEquipmentOfSort(13) ||
                             !Globals::status->getShip()->getFirstEquipmentOfSort(17))) {
                            this->choiceWindow->set(*Globals::gameText->getText(532));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(135) &&
                                   !Globals::status->getShip()->getFirstEquipmentOfSort(19)) {
                            this->choiceWindow->set(*Globals::gameText->getText(3213));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(91) &&
                                   Globals::status->getShip()->getMaxPassengers() <= 9) {
                            this->choiceWindow->set(*Globals::gameText->getText(3214));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(94) &&
                                   Globals::status->getShip()->getMaxPassengers() == 0) {
                            this->choiceWindow->set(*Globals::gameText->getText(3214));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(105) &&
                                   !Globals::status->getShip()->hasEquipment(206, 1)) {
                            this->choiceWindow->set(*Globals::gameText->getText(3217));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(139) && Globals::status->getShip()->getIndex() != 42 &&
                                   (StarMap_shipRaces[Globals::status->getShip()->getIndex()] != 1 ||
                                    !Globals::status->getShip()->hasEquipment(190, 1))) {
                            this->choiceWindow->set(*Globals::gameText->getText(3215));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(142) &&
                                   (!Globals::status->getShip()->getFirstEquipmentOfSort(33) ||
                                    !Globals::status->getShip()->hasEquipment(197, 15) ||
                                    !Globals::status->getShip()->getFirstEquipmentOfSort(35))) {
                            this->choiceWindow->set(*Globals::gameText->getText(3216));
                            this->suppressNextClose = 1;
                        } else if (STARMAP_AT_CAMPAIGN_TARGET(142) && Globals::status->getShip()->getFreeSpace() == 0) {
                            this->choiceWindow->set(*Globals::gameText->getText(3218));
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
#undef STARMAP_AT_CAMPAIGN_TARGET
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
    return false;
}
