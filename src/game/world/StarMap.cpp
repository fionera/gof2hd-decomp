#include "gof2/game/world/StarMap.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/engine/math/EaseInOut.h"
#include "gof2/engine/audio/FModSound.h"
// FileRead.h defines stub `struct Item/Station/SolarSystem/Agent/...` that collide with
// the real class headers pulled in below. StarMap only needs FileRead::loadStationsBinary()
// (its result is stored into an opaque void* field), so declare a minimal FileRead here
// instead of including the full header.
class FileRead {
public:
    void *loadStationsBinary();
};
#include "gof2/game/mission/Item.h"
// SystemPathFinder.h declares stub `struct SolarSystem`/`struct Status` that collide with
// the real class headers. StarMap only needs the SystemPathFinder class itself, so declare a
// minimal version here (its array params use the real SolarSystem, forward-declared below).
class SolarSystem;
// Level::setInitStreamOut() is a static method; declare it minimally to avoid pulling Level.h.
class Level {
public:
    static void setInitStreamOut();
};
class SystemPathFinder {
public:
    int getJumpDistance(Array<SolarSystem *> *systems, int from, int to);
    Array<int> *getSystemPath(Array<SolarSystem *> *systems, int from, int to);
};
#include "gof2/engine/math/Transform.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/game/mission/Achievements.h"
#include "gof2/engine/core/ApplicationManager.h"
// Engine.h re-declares __aeabi_memcpy with `unsigned long n`, conflicting with AEGeometry.h's
// `uint32_t n`. Rename Engine.h's decl during its include so both can coexist in this TU; the
// real builtin is resolved at link time and StarMap never calls __aeabi_memcpy directly.
#define __aeabi_memcpy __aeabi_memcpy_engine_unused
#include "gof2/engine/render/Engine.h"
#undef __aeabi_memcpy
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/mission/Mission.h"   // pulls in Station.h -> Agent.h, the canonical String
#include "gof2/game/world/Station.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/core/String.h"
#include "gof2/game/ui/TouchButton.h"

extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_render_canvas;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_render_geometry)(void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_alien_text;
extern "C" void *SystemPathFinder_dtor(void *finder);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_draw_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_text;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_font;
extern "C" float EaseInOut_GetValue(void *ease);
extern "C" float EaseInOut_GetMinValue(void *ease);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_status;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_store0_a;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_flag_a;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_jumpFlag_a;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_jumpCost_a;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_targetStation;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_flag_b;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_status2;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_jumpFlag_b;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_jumpCost_b;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_achievements;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_sound;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_modstation_flag;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_canvas;
extern "C" int Station_getSystem(void *station);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_sound;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_text;
extern "C" void *ChoiceWindow_ctor(void *window);
extern "C" int Station_getIndex(void *station);
void MatrixGetPosition(Vector *out, void *matrix);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_lights_engine;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_lights_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_sound;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_update_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_update_screenH;
void MatrixSetTranslation(void *matrix, float x, float y, float z);
void VectorNormalize(Vector *out, Vector *value);
extern "C" void EaseInOut_Update(void *ease, float dt);
extern "C" float EaseInOut_GetCurrentValue(void *ease);
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_ctor_vecCtor)(void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_galaxy;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_ctor_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_random;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_touch_screenH;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_sound;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_status;
extern "C" void FileRead_ctor(void *reader);
extern "C" void *FileRead_dtor(void *reader);
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_move_guard;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_move_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_move_canvas;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_drawKey_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_drawKey_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_drawKey_screenH;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_drawKey_drawImage)(uint32_t, uint32_t, int, int);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_text;
extern "C" __attribute__((visibility("hidden"))) String *(*g_StarMap_drawKey_getText)(void *, int);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_font;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_drawKey_drawString)(uint32_t, void *, String *, int, int, bool);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_system_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_random;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_engine;
extern "C" int Station_getTextureIndex(void *station);
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_info_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_info_screenH;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_info_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_font;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_text;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_info_isGerman;
extern "C" int Station_getTecLevel(void *station);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_init_canvas;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_init_imageCreate)(uint32_t, int, void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_text;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_font;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_init_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_init_screenH;
void MatrixSetRotation(void *matrix, float x, float y, float z, float w);
extern "C" void *EaseInOut_ctor(void *ease);
extern "C" void *SystemPathFinder_ctor(void *finder);

uint8_t StarMap::missionChanged()
{
    return this->missionChangedFlag;
}

void StarMap::render()
{
    uint32_t *canvasHolder = g_StarMap_render_canvas;
    ((PaintCanvas*)(long)(*canvasHolder))->SetColor((unsigned int)(0xffffffffu));
    void (*renderGeometry)(void *) = g_StarMap_render_geometry;
    renderGeometry(this->bgLayer0);
    renderGeometry(this->bgLayer1);
    renderGeometry(this->bgLayer2);
    renderGeometry(this->systemRoot);
    ((PaintCanvas*)(long)(*canvasHolder))->End3d();
    ((PaintCanvas*)(long)(*canvasHolder))->Begin3d();
    if (this->starSystemRoot != 0) {
        ((PaintCanvas*)(long)(*canvasHolder))->SetTexture((unsigned int)(this->planetTexture), (unsigned int)(0xffffffffu));
        ((PaintCanvas*)(long)(*canvasHolder))->SetBlendMode(2);
        ((AEGeometry *)(this->planetGeom))->render();
        ((AEGeometry *)(this->starSystemRoot))->render();
    }
    if (this->markerGeom != 0) {
        return this->render_tail();
    }
}

// render() tail (binary: indirect AEGeometry::render thunk @0x1abdd4 reached when the
// optional overlay mesh at +0xf8 is present): draws the highlighted jump-route /
// selected-system overlay geometry that sits on top of the system map.
void StarMap::render_tail()
{
    ((AEGeometry *)(this->markerGeom))->render();
}

// Background-render hook. The star map paints its whole scene (3D systems plus the
// 2D overlay) from render()/draw(); the dedicated "render background" entry point that
// the station shell dispatches per-frame is intentionally a no-op for this screen.
void StarMap::renderBG()
{
}

bool StarMap::isInPlanetMode()
{
    return this->mode == 3;
}

void StarMap::askForJumpIntoAlienWorld()
{
    void *window = (void *)this->choiceWindow;
    this->alienJumpPending = 1;
    String *text = (String *)((GameText *)(*g_StarMap_alien_text))->getText(0x1a6);
    ((ChoiceWindow *)(window))->set(*text, true);
    this->choiceVisible = 1;
}

void StarMap::setJumpMapMode(bool enabled, bool value)
{
    this->jumpMapModeB = (uint8_t)value;
    this->jumpMapModeA = (uint8_t)enabled;
}

struct SystemPathFinder;

void StarMap::setStart(int start, int target)
{
    this->routeStart = target;
    this->routeTarget = start;
    Array<int> *path = (Array<int> *)this->systemPath;
    if (path != 0) {
        delete path;
    }
    this->systemPath = (Array<int> *)0;
    this->systemPath =
        ((SystemPathFinder *)((SystemPathFinder *)this->pathFinder))->getSystemPath((Array<SolarSystem *> *)this->systems, start, this->targetSystem);
}

StarMap::~StarMap()
{
    void *p;

    {
        Array<Vector *> *positions = (Array<Vector *> *)this->systemPositions;
        if (positions != 0) {
            for (Vector *v : *positions) delete v;
            delete positions;
        }
        this->systemPositions = 0;
    }

    {
        Array<Vector *> *positions = (Array<Vector *> *)this->stationPositions;
        if (positions != 0) {
            for (Vector *v : *positions) delete v;
            delete positions;
        }
        this->stationPositions = 0;
    }

    p = this->planetGeom;
    if (p != 0) {
        [&]{ AEGeometry *g_=(AEGeometry*)(p); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
    }
    this->planetGeom = (AEGeometry *)0;

    p = this->bgLayer0;
    if (p != 0) {
        [&]{ AEGeometry *g_=(AEGeometry*)(p); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
    }
    this->bgLayer0 = (AEGeometry *)0;

    p = this->bgLayer1;
    if (p != 0) {
        [&]{ AEGeometry *g_=(AEGeometry*)(p); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
    }
    this->bgLayer1 = (AEGeometry *)0;

    p = this->bgLayer2;
    if (p != 0) {
        [&]{ AEGeometry *g_=(AEGeometry*)(p); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
    }
    this->bgLayer2 = (AEGeometry *)0;

    p = this->easeX;
    if (p != 0) {
        operator delete(p);
    }
    this->easeX = (AbyssEngine::EaseInOut *)0;

    p = this->easeY;
    if (p != 0) {
        operator delete(p);
    }
    this->easeY = (AbyssEngine::EaseInOut *)0;

    p = this->easeZ;
    if (p != 0) {
        operator delete(p);
    }
    this->easeZ = (AbyssEngine::EaseInOut *)0;

    p = this->markerGeom;
    if (p != 0) {
        [&]{ AEGeometry *g_=(AEGeometry*)(p); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
    }
    this->markerGeom = (AEGeometry *)0;

    p = this->pathFinder;
    if (p != 0) {
        operator delete(SystemPathFinder_dtor(p));
    }
    this->pathFinder = (SystemPathFinder *)0;
}

void StarMap::draw()
{
    String tmp;

    int mode = this->mode;
    int alpha = mode == 0 ? 0xff : 0;
    this->alpha = alpha;
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        float v = EaseInOut_GetValue(this->easeZ);
        float min = EaseInOut_GetMinValue(this->easeZ);
        float max = ((AbyssEngine::EaseInOut *)(this->easeZ))->GetMaxValue();
        float t = (v - min) / (max - min);
        if (this->transitionIn != 0) {
            t = 1.0f - t;
        }
        this->alpha = (int)(t * 255.0f);
    }

    uint32_t canvas = *g_StarMap_draw_canvas;
    if (mode != 3 || this->transitionIn != 0 || this->transitionOut != 0) {
        ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha));
        Array<Vector *> *positions = (Array<Vector *> *)this->systemPositions;
        for (uint32_t i = 0; i < positions->size(); i++) {
            this->scratchVector = *(const Vector *)(positions->data()[i]);
            void *routes = ((SolarSystem *)(((Array<void *> *)this->systems)->data()[i]))->getRoutes();
            if (routes != 0) {
                Array<int> *r = (Array<int> *)routes;
                for (uint32_t j = 0; j < r->size(); j++) {
                    uint32_t to = (uint32_t)r->data()[j];
                    if (to < positions->size()) {
                        this->scratchVector2 = *(const Vector *)(positions->data()[to]);
                        if (this->scratchVector.z >= 0.0f || this->scratchVector2.z >= 0.0f) {
                            ((PaintCanvas*)(long)(canvas))->DrawLine((int)this->scratchVector.x, (int)this->scratchVector.y, (int)this->scratchVector2.x, (int)this->scratchVector2.y);
                        }
                    }
                }
            }
        }
        for (uint32_t i = 0; i < ((Array<void *> *)this->systems)->size(); i++) {
            drawOnScreenInfo((int)i, false);
        }
    }

    if (this->selectedSystem >= 0 &&
        (this->autoMode == 0 || this->autoTimer > 3999)) {
        drawOnScreenInfo(this->selectedSystem, false);
    }

    if (this->starSystemRoot != 0 && this->stations != 0) {
        void *system = ((Array<void *> *)this->systems)->data()[this->selectedSystem];
        if (((SolarSystem *)(system))->hasNoOwner() == 0) {
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha ^ 0xff));
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->systemNameImage), field<int32_t>(*g_StarMap_draw_layout, 0x2c), field<int32_t>(*g_StarMap_draw_layout, 0xc) +
                                        field<int32_t>(*g_StarMap_draw_layout, 0x2c), (unsigned char)(0));
            ((SolarSystem *)(&tmp))->getName();
            ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(*g_StarMap_draw_font), (void *)(&tmp), ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned int)(this->systemNameImage)) +
                                       field<int32_t>(*g_StarMap_draw_layout, 0x2c) * 2, field<int32_t>(*g_StarMap_draw_layout, 0xc) +
                                       field<int32_t>(*g_StarMap_draw_layout, 0x2c) + 2, false);
        }
        Array<void *> *stations = (Array<void *> *)this->stations;
        for (uint32_t i = 0; i < stations->size(); i++) {
            if (i != (uint32_t)this->selectedStation) {
                drawOnScreenInfo((int)i, true);
            }
        }
        if (this->selectedStation >= 0) {
            drawOnScreenInfo(this->selectedStation, true);
        }
    }

    if (this->showKey != 0) {
        drawKey();
    }
    tmp.copy((String *)((GameText *)(*g_StarMap_draw_text))->getText(0x190), false);
    ((Layout *)(*g_StarMap_draw_layout))->drawHeader1(&tmp);
    ((Layout *)(*g_StarMap_draw_layout))->drawEmptyFooter(1);
    ((TouchButton *)(this->backButton))->draw();
    if (this->choiceVisible != 0) {
        ((ChoiceWindow *)(this->choiceWindow))->draw();
    }
}

void StarMap::depart(bool jump)
{
    int selected = this->selectedStation;
    if (selected < 0) {
        return;
    }

    void **statusHolder = g_StarMap_depart_status;
    void *status = *statusHolder;
    if (this->jumpMapModeA != 0) {
        Array<void *> *stations = (Array<void *> *)this->stations;
        ((Status *)(status))->departStation((Station *)stations->data()[selected]);
        *g_StarMap_depart_store0_a = 0;
        Level::setInitStreamOut();
        int used = ((Status *)(status))->getJumpgateUsed();
        if (jump) {
            used = this->jumpMapModeB;
        }
        if (jump && used != 0) {
            int toSystem = Station_getSystem(stations->data()[selected]);
            int current = ((Status *)(status))->getSystem();
            *g_StarMap_depart_jumpFlag_a = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                *g_StarMap_depart_jumpCost_a = this->jumpCost;
            }
        } else {
            *g_StarMap_depart_flag_a = 0;
        }
    } else {
        if (((Status *)(status))->getCurrentCampaignMission() == 3) {
            goto cleanup;
        }

        int *statusWords = (int *)status;
        statusWords[0x5c / 4] = -1;
        statusWords[0x60 / 4] = -1;
        statusWords[0x64 / 4] = -1;
        statusWords[0x68 / 4] = -1;
        ((Status *)(status))->departStation((Station *)((Status *)(status))->getStation());

        Array<void *> *stations = (Array<void *> *)this->stations;
        void *target = stations->data()[this->selectedStation];
        if (((Station *)(target))->equals((Station *)((Status *)(status))->getStation()) == 0) {
            *g_StarMap_depart_targetStation = (int)(intptr_t)target;
        }

        if (jump) {
            void *ship = ((Status *)(status))->getShip();
            if (((Ship *)(ship))->hasVolatileGoods() != 0) {
                goto no_jump;
            }
            if (((Ship *)(((Status *)(status))->getShip()))->hasJumpDriveIntegrated() == 0 && this->jumpMapModeB == 0) {
                goto no_jump;
            }
            int toSystem = Station_getSystem(*g_StarMap_depart_status2);
            int current = ((Status *)(status))->getSystem();
            *g_StarMap_depart_jumpFlag_b = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                *g_StarMap_depart_jumpCost_b = this->jumpCost;
            }
        } else {
no_jump:
            *g_StarMap_depart_flag_b = 0;
        }
        ((Achievements *)(*g_StarMap_depart_achievements))->resetNewMedals();
    }

cleanup:
    {
        Array<Station *> *stations = (Array<Station *> *)this->stations;
        for (uint32_t i = 0; i < stations->size(); i++) {
            if (i != (uint32_t)this->selectedStation) {
                Station *station = (*stations)[i];
                if (station != 0) {
                    station->dtor();
                    operator delete(station);
                }
                (*stations)[i] = 0;
            }
        }
        if (stations != 0) {
            delete stations;
        }
        this->stations = (Array<Station *> *)0;
    }
    ((FModSound *)(*g_StarMap_depart_sound))->stop(0x66);
    *g_StarMap_depart_modstation_flag = 1;
    return this->depart_tail(*g_StarMap_depart_canvas, 2);
}

// depart() tail (binary: module-switch trampoline @0x1ab908): the player has committed
// to leaving the current station / jumping, so the running star-map module hands control
// back to the application, switching it to the in-flight module (`moduleId` == 2).
void StarMap::depart_tail(void *app, int moduleId)
{
    ((ApplicationManager *)app)->SetCurrentApplicationModule((unsigned)moduleId);
}

static inline float absf_end(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::OnTouchEnd(int x, int y)
{
    String help;

    if (this->choiceVisible != 0) {
        int result = ((ChoiceWindow *)(this->choiceWindow))->OnTouchEnd(x, y);
        if (result == 1) {
            this->choiceVisible = 0;
            this->alienJumpPending = 0;
            return;
        }
        if (result != 0) {
            return;
        }
        this->choiceVisible = 0;
        if (this->suppressNextClose != 0) {
            this->suppressNextClose = 0;
            return;
        }
        if (this->mode == 3) {
            void *station = ((Array<void *> *)this->stations)->data()[this->selectedStation];
            if (Station_getIndex(station) == Station_getIndex(((Status *)(*g_StarMap_end_status))->getStation())) {
                this->suppressNextClose = 0;
                return;
            }
        }
        if (this->alienJumpPending != 0 && this->mode == 0 &&
            this->pad_0xa8_a == 0 && this->autoMode == 0) {
            this->alienJumpPending = 0;
            this->exitRequested = 1;
            ((PaintCanvas*)(long)(*g_StarMap_end_canvas))->CameraSetCurrent((unsigned int)(this->prevCamera));
            return;
        }
        if (this->pad_0xa8_a == 0 && this->mode == 3) {
            if (this->jumpMapModeB == 0 ||
                this->jumpCost <= this->cargoAmount) {
                if (this->jumpMapModeA == 0) {
                    depart(true);
                    return;
                }
            } else if (this->jumpCost == 1 && this->jumpMapModeA == 0) {
                depart(false);
                return;
            }
            this->exitRequested = 1;
            ((PaintCanvas*)(long)(*g_StarMap_end_canvas))->CameraSetCurrent((unsigned int)(this->prevCamera));
        }
        return;
    }

    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return;
    }
    void *layout = *g_StarMap_end_layout;
    if (*(uint8_t *)layout == 0 && ((Layout *)(layout))->OnTouchEnd(x, y) != 0) {
        if (this->mode == 3 && this->isGalaxyMode != 0) {
            this->transitionOut = 1;
            this->momentumFactor = 0.0f;
            this->velocityX = 0.0f;
            this->velocityY = 0.0f;
            ((AbyssEngine::EaseInOut *)(this->easeX))->SetRange(this->scratchVector.x, this->scratchVector.x);
            ((AbyssEngine::EaseInOut *)(this->easeY))->SetRange(this->scratchVector.y, this->scratchVector.y);
            ((AbyssEngine::EaseInOut *)(this->easeZ))->SetRange(this->scratchVector.z, this->scratchVector.z);
            ((FModSound *)(*g_StarMap_end_sound))->play(0x6b, 0, 0, 0.0f);
        } else {
            ((PaintCanvas*)(long)(*g_StarMap_end_canvas))->CameraSetCurrent((unsigned int)(this->prevCamera));
            ((FModSound *)(*g_StarMap_end_sound))->stop(0x66);
        }
        return;
    }
    if (this->pad_0xa8_a != 0 && this->pathAnim != 0) {
        return;
    }
    if (((TouchButton *)(this->backButton))->OnTouchEnd(x, y) != 0) {
        this->showKey ^= 1;
    }
    if (this->dragging != 0) {
        float dx = this->touchDeltaX;
        float dy = this->touchDeltaY;
        this->momentumFactor = 0.9f;
        this->dragging = 0;
        this->velocityX = absf_end(dx) > 3.0f ? dx : 0.0f;
        this->velocityY = absf_end(dy) > 3.0f ? dy : 0.0f;
        if (this->mode == 0) {
            if (this->selectedSystem >= 0 &&
                this->autoMode == 0 &&
                this->pad_0xa8_a == 0 &&
                this->lastSelectedSystem == this->selectedSystem) {
                if (this->jumpMapModeB == 0 &&
                    ((SolarSystem *)(long)(((Status *)(*g_StarMap_end_status))->getSystem()))->systemIsInSystemRoutes(((Status *)(*g_StarMap_end_status))->getSystem()) == 0) {
                    ((ChoiceWindow *)(this->choiceWindow))->set(*(String *)((GameText *)(*g_StarMap_end_text))->getText(0x1a4), false);
                    this->choiceVisible = 1;
                    return;
                }
                ((FModSound *)(*g_StarMap_end_sound))->play(0x6a, 0, 0, 0.0f);
                initStarSystem();
                this->transitionIn = 1;
                Vector p;
                ((AEGeometry *)(&p))->getPosition();
                this->scratchVector = *(const Vector *)(&p);
                this->scratchVector.z += 20.0f;
                MatrixGetPosition(&p, ((PaintCanvas*)(long)(*g_StarMap_end_canvas))->CameraGetLocal(((PaintCanvas*)(long)(*g_StarMap_end_canvas))->CameraGetCurrent()));
                this->scratchVector2 = *(const Vector *)(&p);
                ((AbyssEngine::EaseInOut *)(this->easeX))->SetRange(this->scratchVector2.x, this->scratchVector.x);
                ((AbyssEngine::EaseInOut *)(this->easeY))->SetRange(this->scratchVector2.y, this->scratchVector.y);
                ((AbyssEngine::EaseInOut *)(this->easeZ))->SetRange(this->scratchVector2.z, this->scratchVector.z);
                this->momentumFactor = 0.0f;
                this->velocityX = 0.0f;
                this->velocityY = 0.0f;
            } else if (this->selectedSystem >= 0) {
                this->lastSelectedSystem = -1;
                this->pathAnim = 1;
            }
        } else if (this->selectedStation >= 0) {
            if (this->lastSelectedStation == this->selectedStation) {
                if (this->choiceWindow == 0) {
                    this->choiceWindow = (ChoiceWindow *)ChoiceWindow_ctor(operator new(0x5c));
                }
                ((ChoiceWindow *)(this->choiceWindow))->set(*(String *)((GameText *)(*g_StarMap_end_text))->getText(0x1a3), true);
                this->choiceVisible = 1;
            } else {
                ((FModSound *)(*g_StarMap_end_sound))->play(0x69, 0, 0, 0.0f);
                this->stationCenterAnim = 1;
            }
        }
    }
    if (((Layout *)(layout))->helpPressed() != 0) {
        OnTouchEnd_tail();
    }
}

// OnTouchEnd() tail: the help-window branch reached when the shared map layout reports its
// help button was pressed. Opens the localized help overlay (text id 0x1a5) and releases the
// temporary String. Split out as its own fragment in the binary (tail of OnTouchEnd).
void StarMap::OnTouchEnd_tail()
{
    String help;
    void *layout = *g_StarMap_end_layout;
    help.copy((String *)((GameText *)(*g_StarMap_end_text))->getText(0x1a5), false);
    ((Layout *)(layout))->initHelpWindow(&help);
}

// StarMap::touch_end(x, y): touch-release handler used while the star map is embedded inside
// another screen (e.g. the space lounge map overlay). Unlike OnTouchEnd() — which drives the
// full standalone map screen — this thin variant just feeds the release to the shared map
// layout and reports whether its back/OK button was pressed, so the host screen knows to leave
// map mode and restore its own camera. Returns nonzero when the map was dismissed.
int StarMap::touch_end(int x, int y)
{
    void *layout = *g_StarMap_end_layout;
    if (((Layout *)(layout))->OnTouchEnd(x, y) != 0) {
        return 1;
    }
    OnTouchEnd(x, y);
    return 0;
}

void StarMap::initLights()
{
    void *engine = ((ApplicationManager *)(*g_StarMap_lights_engine))->GetEngine();
    ((Engine *)(engine))->LightSetMaterialColorAmbient(0.5f, 0.5f, 0.5f);
    ((Engine *)(engine))->LightEnable(true);
}

static inline float absf_update(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::update(int dt)
{
    Matrix matrix;
    Vector tmp;

    this->lastDt = dt;
    if (this->mode == 0 || this->transitionIn != 0 || this->transitionOut != 0) {
        Array<Vector *> *positions = (Array<Vector *> *)this->systemPositions;
        for (uint32_t i = 0; i < positions->size(); i++) {
            ((AEGeometry *)(&tmp))->getPosition();
            int visible = (int)(long)((PaintCanvas*)(long)(*g_StarMap_update_canvas))->GetScreenPosition((AbyssEngine::AEMath::Vector *)(&tmp), (AbyssEngine::AEMath::Vector *)(positions->data()[i]));
            positions->data()[i]->z = visible != 0 ? 1.0f : -1.0f;
        }
    }
    if (this->mode == 3 || this->transitionIn != 0 || this->transitionOut != 0) {
        Array<Vector *> *positions = (Array<Vector *> *)this->stationPositions;
        if (positions != 0) {
            for (uint32_t i = 0; i < positions->size(); i++) {
                ((AEGeometry *)(&tmp))->getPosition();
                int visible = (int)(long)((PaintCanvas*)(long)(*g_StarMap_update_canvas))->GetScreenPosition((AbyssEngine::AEMath::Vector *)(&tmp), (AbyssEngine::AEMath::Vector *)(positions->data()[i]));
                positions->data()[i]->z = visible != 0 ? 1.0f : -1.0f;
            }
        }
    }

    if (this->markerGeom != 0) {
        float v = EaseInOut_GetValue(this->easeZ);
        float min = EaseInOut_GetMinValue(this->easeZ);
        float max = ((AbyssEngine::EaseInOut *)(this->easeZ))->GetMaxValue();
        float t = (v - min) / (max - min);
        if (this->transitionOut != 0) {
            t = 1.0f - t;
        } else if (this->transitionIn == 0 && this->mode == 0) {
            t = 1.0f;
        }
        float scale = (float)(0.5 + (double)t * 0.5);
        ((AEGeometry *)(this->markerGeom))->setScaling(scale);
        if (this->starSystemRoot != 0 && this->centeredStation >= 0) {
            ((AEGeometry *)(&tmp))->getPosition();
            ((AEGeometry *)(this->markerGeom))->setPosition(tmp);
        }
        void *canvas = *g_StarMap_update_canvas;
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        MatrixGetPosition(&tmp, ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent()));
        this->scratchVector = *(const Vector *)(&tmp);
        ((AEGeometry *)(&tmp))->getPosition();
        this->scratchVector -= *(const Vector *)(&tmp);
        VectorNormalize(&tmp, &this->scratchVector);
        this->scratchVector = *(const Vector *)(&tmp);
        this->scratchVector.x += 0.5f;
        Vector worldUp = {0.0f, 1.0f, 0.0f};
        ((AEGeometry *)(this->markerGeom))->setDirection(this->scratchVector, worldUp);
        ((AbyssEngine::Transform *)(((PaintCanvas*)(long)(canvas))->TransformGetTransform(0)))->Update(dt, false);
    }

    if (this->transitionIn != 0 || this->transitionOut != 0) {
        float step = (float)(dt * 15);
        EaseInOut_Update(this->easeX, step);
        EaseInOut_Update(this->easeY, step);
        EaseInOut_Update(this->easeZ, step);
        tmp.x = EaseInOut_GetCurrentValue(this->easeX);
        tmp.y = EaseInOut_GetCurrentValue(this->easeY);
        tmp.z = EaseInOut_GetCurrentValue(this->easeZ);
        this->scratchVector = *(const Vector *)(&tmp);
        void *canvas = *g_StarMap_update_canvas;
        __builtin_memcpy(&matrix, ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent()), 0x3c);
        MatrixSetTranslation(&matrix, this->scratchVector.x, this->scratchVector.y, this->scratchVector.z);
        ((PaintCanvas*)(long)(canvas))->CameraSetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent(), *(const AbyssEngine::AEMath::Matrix *)(&matrix));
        if (absf_update(this->scratchVector.x - ((AbyssEngine::EaseInOut *)(this->easeX))->GetMaxValue()) <= 1.0f &&
            absf_update(this->scratchVector.y - ((AbyssEngine::EaseInOut *)(this->easeY))->GetMaxValue()) <= 1.0f &&
            absf_update(this->scratchVector.z - ((AbyssEngine::EaseInOut *)(this->easeZ))->GetMaxValue()) <= 1.0f) {
            if (this->transitionIn == 0) {
                {
                    Array<AEGeometry *> *geoms = (Array<AEGeometry *> *)this->stationGeoms;
                    if (geoms != 0) {
                        for (AEGeometry *g : *geoms) delete g;
                        delete geoms;
                    }
                    this->stationGeoms = 0;
                }
                {
                    Array<AEGeometry *> *rings = (Array<AEGeometry *> *)this->ringGeoms;
                    if (rings != 0) {
                        for (AEGeometry *g : *rings) delete g;
                        delete rings;
                    }
                    this->ringGeoms = 0;
                }
                if (this->stationAngles != 0) {
                    operator delete(this->stationAngles);
                }
                this->stationAngles = (int *)0;
                if (this->stationDistances != 0) {
                    operator delete(this->stationDistances);
                }
                this->stationDistances = (int *)0;
                {
                    Array<uint8_t> *used = (Array<uint8_t> *)this->usedFlags;
                    if (used != 0) {
                        used->clear();
                        delete used;
                    }
                    this->usedFlags = 0;
                }
                if (this->starSystemRoot != 0) {
                    [&]{ AEGeometry *g_=(AEGeometry*)(this->starSystemRoot); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
                }
                this->mode = 0;
                this->starSystemRoot = (AEGeometry *)0;
                ((AEGeometry *)(((Array<void *> *)this->systemGeoms)->data()[this->selectedSystem]))->setVisible(true);
            } else {
                this->mode = 3;
            }
            this->transitionIn = 0;
            this->transitionOut = 0;
        }
        return;
    }

    ((FModSound *)(*g_StarMap_update_sound))->setParamValue(0, 0x66, 0.0f);
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
            int target = 0x8000 - ((int *)this->stationAngles)[this->selectedStation];
            int diff = (int)this->yaw - target;
            this->yaw += (float)(diff < 0 ? -diff : diff) * (diff < 0 ? 0.25f : -0.25f);
            if (absf_update((float)diff) < 11.0f) {
                this->stationCenterAnim = 0;
            }
        }
        float speed = absf_update(this->velocityX + this->velocityY);
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        this->spin += speed;
        if (this->stationGeoms != 0) {
            Array<void *> *geoms = (Array<void *> *)this->stationGeoms;
            for (uint32_t i = 0; i < geoms->size(); i++) {
                if (geoms->data()[i] != 0) {
                    ((AEGeometry *)(geoms->data()[i]))->rotate((float)dt * 0.001f, 0.0f, 0.001f);
                }
            }
        }
        this->yaw = (float)((int)this->yaw & 0xffff);
        if (this->pitch < -90.0f) {
            this->pitch = -90.0f;
        }
        if (this->pitch > 90.0f) {
            this->pitch = 90.0f;
        }
        ((AEGeometry *)(this->starSystemRoot))->setRotation(this->pitch, 0.0f, this->yaw);
        return;
    }

    if (this->mode == 0) {
        if (this->pulseSystem >= 0 && this->autoMode != 0 &&
            this->autoTimer < 4000) {
            this->autoTimer += dt;
            if (this->autoTimer > 3999) {
                OnTouchBegin(*g_StarMap_update_screenW >> 1, *g_StarMap_update_screenH >> 1);
            }
            float scale = ((float)this->autoTimer / 4000.0f) * 1.5f;
            ((AEGeometry *)(((Array<void *> *)this->systemGeoms)->data()[this->pulseSystem]))->setScaling(scale);
        }
        if (this->dragging == 0) {
            float vx = this->momentumFactor * this->velocityX;
            float vy = this->momentumFactor * this->velocityY;
            this->velocityX = vx;
            this->velocityY = vy;
            if (absf_update(vx) > 0.5f) {
                this->panX = (int)((float)this->panX + vx);
            }
            if (absf_update(vy) > 0.5f) {
                this->panY = (int)((float)this->panY + vy);
            }
        }
        void *canvas = *g_StarMap_update_canvas;
        __builtin_memcpy(&matrix, ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent()), 0x3c);
        MatrixSetTranslation(&matrix,
                             (this->cameraBaseX + (float)this->panX) * 20.0f,
                             0.0f,
                             (this->cameraBaseZ + (float)this->panY) * 20.0f);
        ((PaintCanvas*)(long)(canvas))->CameraSetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent(), *(const AbyssEngine::AEMath::Matrix *)(&matrix));
        if (this->pathAnim != 0) {
            Array<uint8_t> *vis = (Array<uint8_t> *)((Status *)(*g_StarMap_update_status))->getSystemVisibilities();
            uint32_t selected = this->selectedSystem;
            if (vis != 0 && selected < vis->size() && vis->data()[selected] != 0) {
                this->scratchVector = *(const Vector *)(((Array<Vector *> *)this->systemPositions)->data()[selected]);
                float targetX = (float)(*g_StarMap_update_screenW >> 1);
                float targetY = (float)(*g_StarMap_update_screenH >> 1);
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

StarMap::StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4)
{
    void (*vecCtor)(void *) = g_StarMap_ctor_vecCtor;
    Vector zero = {0.0f, 0.0f, 0.0f};
    this->scratchVector = zero;
    this->scratchVector2.y = 0.0f;
    this->scratchVector2.z = 0.0f;
    vecCtor(&this->field_0xac);
    vecCtor(&this->field_0xbc);
    vecCtor(&this->field_0xcc);
    this->touchDeltaX = 0.0f; this->touchDeltaY = 0.0f; this->field_0x158 = 0.0f;
    this->touchStartX = 0.0f; this->touchStartY = 0.0f; this->field_0x14c = 0.0f;
    this->field_0x164 = 0;
    this->yaw = 0.0f;
    this->pitch = 0.0f;
    this->field_0x190 = 0;

    this->mode = 0;
    this->hitRadius = field<int32_t>(*g_StarMap_ctor_status, 0x88);
    this->selectedSystem = -1;
    this->starSystemRoot = (AEGeometry *)0;
    this->stations = (Array<Station *> *)0;
    this->systemPositions = 0;
    this->stationPositions = 0;
    this->suppressNextClose = 0;
    this->field_0x01 = 0;
    this->alienJumpPending = 0;
    this->easeX = (AbyssEngine::EaseInOut *)0;
    this->easeY = (AbyssEngine::EaseInOut *)0;
    this->easeZ = (AbyssEngine::EaseInOut *)0;
    this->momentumFactor = 0.0f;
    this->velocityX = 0.0f;
    this->velocityY = 0.0f;

    this->iconBuffer = (int *)operator new(0x14);
    this->systems = (Array<SolarSystem *> *)((Galaxy *)(*g_StarMap_ctor_galaxy))->getSystems();
    this->field_0x10 = 500;
    this->field_0x14 = 500;

    void *root = operator new(0xc0);
    new ((void*)root) AEGeometry((PaintCanvas*)(long)(*g_StarMap_ctor_canvas));
    this->systemRoot = (AEGeometry *)root;

    Array<AEGeometry *> *systemsGeom = new Array<AEGeometry *>();
    this->systemGeoms = systemsGeom;
    Array<Vector *> *systemPositions = new Array<Vector *>();
    this->systemPositions = systemPositions;
    systemsGeom->resize(0x22);
    systemPositions->resize(0x22);

    Array<void *> *systems = (Array<void *> *)this->systems;
    Array<void *> *geoms = (Array<void *> *)this->systemGeoms;
    Array<Vector *> *positions = (Array<Vector *> *)this->systemPositions;
    for (uint32_t i = 0; i < geoms->size(); i++) {
        void *sys = systems->data()[i];
        int tex = ((SolarSystem *)(sys))->getTextureIndex();
        uint16_t image = (uint16_t)(tex + 0x4696);
        if (i == 0x1b && ((Status *)(*g_StarMap_ctor_status))->getCurrentCampaignMission() > 0x9d) {
            image = 0x469b;
        }
        void *geom = operator new(0xc0);
        new ((void*)geom) AEGeometry((uint16_t)image, (PaintCanvas*)(long)(*g_StarMap_ctor_canvas), false);
        geoms->data()[i] = geom;
        ((AEGeometry *)(geom))->setScaling(1.0f);
        float px = (float)((int)(((100 - ((SolarSystem *)(sys))->getX()) / 100.0f) * 20000.0f) - 10000);
        float py = (float)((int)(((100 - ((SolarSystem *)(sys))->getY()) / 100.0f) * 18000.0f) - 9000);
        float pz = (float)((int)(((100 - ((SolarSystem *)(sys))->getZ()) / 100.0f) * 9000.0f) + 1000);
        Vector posVec = {px, py, pz};
        ((AEGeometry *)(geom))->setPosition(posVec);
        ((AEGeometry *)(root))->addChild(field<uint32_t>(geom, 0xc));
        Vector *pos = (Vector *)operator new(0xc);
        *pos = zero;
        positions->data()[i] = pos;
    }

    ((AbyssEngine::AERandom *)(*g_StarMap_ctor_random))->reset();
    this->markerGeom = (AEGeometry *)0;
    this->choiceVisible = 0;
    if (((Status *)(*g_StarMap_ctor_status))->getCurrentCampaignMission() > 0x1f &&
        field<int32_t>(*g_StarMap_ctor_status, 0x7c) >= 0) {
        void *marker = operator new(0xc0);
        new ((void*)marker) AEGeometry((uint16_t)0x4262, (PaintCanvas*)(long)(*g_StarMap_ctor_canvas), false);
        this->markerGeom = (AEGeometry *)marker;
        Vector p;
        ((AEGeometry *)(&p))->getPosition();
        ((AEGeometry *)(marker))->setPosition(p);
        ((AbyssEngine::Transform *)(((PaintCanvas*)(long)(*g_StarMap_ctor_canvas))->TransformGetTransform(0)))->SetAnimationState((AbyssEngine::AnimationMode)2, 0);
        ((AEGeometry *)(marker))->setRotation(0.0f, 0.0f, 0.0f);
    }

    init(jumpMapMode, mission, param3, param4);
}

static inline float absf_local(float v)
{
    return v < 0.0f ? -v : v;
}

uint32_t StarMap::OnTouchBegin(int x, int y)
{
    if (this->choiceVisible != 0) {
        ((ChoiceWindow *)(this->choiceWindow))->OnTouchBegin(x, y);
        return 0;
    }
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return 0;
    }

    void *layout = *g_StarMap_touch_layout;
    ((Layout *)(layout))->OnTouchBegin(x, y);
    if ((this->pad_0xa8_a != 0 && this->pathAnim != 0) ||
        this->stationCenterAnim != 0) {
        return 0;
    }
    ((TouchButton *)(this->backButton))->OnTouchBegin(x, y);
    if (field<int32_t>(layout, 0xc) >= y || y >= *g_StarMap_touch_screenH - field<int32_t>(layout, 0x10)) {
        return 0;
    }
    if (this->autoMode != 0 && this->autoTimer < 4000) {
        return 0;
    }

    void *sound = *g_StarMap_touch_sound;
    ((FModSound *)(sound))->stop(0x66);
    ((FModSound *)(sound))->play(0x66, 0, 0, 0.0f);

    float fx = (float)x;
    float fy = (float)y;
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
        this->jumpCost = 0;
        this->selectedSystem = -1;
        void *status = *g_StarMap_touch_status;
        Array<void *> *systems = (Array<void *> *)this->systemGeoms;
        for (uint32_t i = 0; i < systems->size(); i++) {
            Array<uint8_t> *vis = (Array<uint8_t> *)((Status *)(status))->getSystemVisibilities();
            if (vis != 0 && i < vis->size() && vis->data()[i] != 0) {
                this->scratchVector = *(const Vector *)(((Array<Vector *> *)this->systemPositions)->data()[i]);
                if (this->scratchVector.z > 0.0f &&
                    absf_local(this->scratchVector.x - fx) < (float)this->hitRadius &&
                    absf_local(this->scratchVector.y - fy) < (float)this->hitRadius) {
                    this->selectedSystem = (int)i;
                    Array<Station *> *stations = (Array<Station *> *)this->stations;
                    if (stations != 0) {
                        for (Station *s : *stations) delete s;
                        stations->clear();
                        delete stations;
                        this->stations = (Array<Station *> *)0;
                    }
                    this->stations = new Array<Station *>();
                    void *reader = operator new(1);
                    FileRead_ctor(reader);
                    this->stations =
                        (Array<Station *> *)((FileRead *)(reader))->loadStationsBinary();
                    operator delete(FileRead_dtor(reader));
                    if (oldSystem != this->selectedSystem) {
                        ((FModSound *)(sound))->play(0x67, 0, 0, 0.0f);
                    }
                    int current = ((Status *)(status))->getSystem();
                    int dist = ((SystemPathFinder *)(this->pathFinder))->getJumpDistance((Array<SolarSystem *> *)this->systems, current, this->selectedSystem);
                    this->jumpCost = dist;
                    if (dist == 0 && current != this->selectedSystem) {
                        this->jumpCost = 4;
                        if (((SolarSystem *)(((Array<void *> *)this->systems)->data()[i]))->getRoutes() == 0) {
                            this->noRoute = 1;
                        }
                    }
                    if (((Status *)(status))->hardCoreMode() != 0) {
                        this->jumpCost <<= 1;
                    }
                    return 0;
                }
            }
        }
    } else if (this->mode == 3) {
        Array<Vector *> *stations = (Array<Vector *> *)this->stationPositions;
        for (uint32_t i = 0; i < stations->size(); i++) {
            this->scratchVector = *(const Vector *)(stations->data()[i]);
            if (this->scratchVector.z > 0.0f &&
                absf_local(this->scratchVector.x - fx) < (float)this->hitRadius &&
                absf_local(this->scratchVector.y - fy) < (float)this->hitRadius) {
                this->selectedStation = (int)i;
                ((FModSound *)(sound))->play(0x68, 0, 0, 0.0f);
                return 0;
            }
        }
    }
    return 0;
}

void StarMap::OnTouchMove(int x, int y)
{
    Matrix matrix;

    if (this->choiceVisible != 0) {
        ((ChoiceWindow *)(this->choiceWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->transitionIn != 0 || this->transitionOut != 0) {
        return;
    }
    void *layout = *g_StarMap_move_layout;
    ((Layout *)(layout))->OnTouchMove(x, y);
    if ((this->pad_0xa8_a != 0 && this->pathAnim != 0) ||
        this->stationCenterAnim != 0) {
        return;
    }
    ((TouchButton *)(this->backButton))->OnTouchMove(x, y);
    if (this->dragging == 0) {
        return;
    }

    float fx = (float)x;
    float fy = (float)y;
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
        float speed = absf_local(dx + dy);
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        float targetX = (float)this->panX;
        float targetY = (float)this->panY;
        this->panX = (int)(dx + targetX);
        this->panY = (int)(dy + targetY);
        this->spin = this->spin + speed;
        if (absf_local(this->touchStartX - fx) > 3.0f ||
            absf_local(this->touchStartY - fy) > 3.0f) {
            this->lastSelectedSystem = -1;
            this->selectedSystem = -1;
            this->jumpCost = 0;
        }
        void *canvas = *g_StarMap_move_canvas;
        ((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
        __builtin_memcpy(&matrix, ((PaintCanvas*)(long)(canvas))->CameraGetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent()), 0x3c);
        ((PaintCanvas*)(long)(canvas))->CameraSetLocal(((PaintCanvas*)(long)(canvas))->CameraGetCurrent(), *(const AbyssEngine::AEMath::Matrix *)(&matrix));
        return;
    }

    if (absf_local(this->touchStartX - fx) <= 3.0f ||
        absf_local(this->touchStartY - fy) <= 3.0f) {
        this->lastSelectedStation = -1;
        this->selectedStation = -1;
    }

    float rotZ = dx * 16.0f;
    float rotX = dy * 16.0f;
    float pitch = this->pitch + rotX;
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }
    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    float yaw = (float)((int)(this->yaw + rotZ) & 0xffff);
    float absZ = absf_local(rotZ);
    float absX = absf_local(rotX);
    this->touchDeltaX = rotZ;
    this->touchDeltaY = rotX;
    this->velocityX = absZ > 3.0f ? rotZ : 0.0f;
    this->velocityY = absX > 3.0f ? rotX : 0.0f;
    this->yaw = yaw;
    this->pitch = pitch;
    ((AEGeometry *)(this->starSystemRoot))->setRotation(this->velocityY, yaw, this->velocityX);
}

void StarMap::drawKey()
{
    uint32_t *canvasHolder = g_StarMap_drawKey_canvas;
    int imageWidth = ((PaintCanvas*)(long)(*canvasHolder))->GetImage2DWidth((unsigned int)(this->keyImageDiscovered));
    void *layout = *g_StarMap_drawKey_layout;
    int screenW = *g_StarMap_drawKey_screenW;
    int screenH = *g_StarMap_drawKey_screenH;
    int boxW = this->keyBoxWidth;
    int boxH = this->keyBoxHeight;
    int marginY = field<int32_t>(layout, 4);
    int padY = field<int32_t>(layout, 8);
    int rightPad = field<int32_t>(layout, 0x10);
    int lineH = field<int32_t>(layout, 0x2c);

    String empty;
    int x = screenH - boxW;
    ((Layout *)(layout))->drawBox(7, x, ((screenW - rightPad) - boxH) - padY, boxW, padY + boxH, &empty, 0);
    int drawX = x + lineH;
    int textX = imageWidth + lineH + drawX;
    int y = ((screenW - lineH) - rightPad) - marginY;

    void (*drawImage)(uint32_t, uint32_t, int, int) = g_StarMap_drawKey_drawImage;
    drawImage(*canvasHolder, this->keyImageRetreat, drawX, y);

    void **textHolder = g_StarMap_drawKey_text;
    String *(*getText)(void *, int) = g_StarMap_drawKey_getText;
    void **fontHolder = g_StarMap_drawKey_font;
    void (*drawString)(uint32_t, void *, String *, int, int, bool) = g_StarMap_drawKey_drawString;

    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x112), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, this->keyImageDiscovered, drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x191), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, this->keyImageCurrent, drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x223), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, this->keyImageMission, drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x22c), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, this->keyImageWanted, drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x22b), textX, y, false);
}

void StarMap::initStarSystem()
{
    void *system = ((Array<void *> *)this->systems)->data()[this->selectedSystem];
    Array<void *> *stationIds = (Array<void *> *)((SolarSystem *)(system))->getStations();
    uint32_t count = stationIds->size();

    Array<Station *> *stations = new Array<Station *>();
    this->stations = (Array<Station *> *)stations;
    stations->resize(count);
    void *reader = operator new(1);
    FileRead_ctor(reader);
    this->stations = (Array<Station *> *)((FileRead *)(reader))->loadStationsBinary();
    operator delete(FileRead_dtor(reader));

    uint32_t bytes = count > 0x3fffffffu ? 0xffffffffu : count * 4;
    this->stationAngles = (int *)operator new(bytes);
    this->stationDistances = (int *)operator new(bytes);
    this->centeredStation = -1;
    ((AbyssEngine::AERandom *)(*g_StarMap_system_random))->setSeed((long long)((SolarSystem *)(system))->getIndex() * 1000);

    Array<AEGeometry *> *stationGeoms = new Array<AEGeometry *>();
    this->stationGeoms = stationGeoms;
    stationGeoms->resize(count + 1);

    void *root = operator new(0xc0);
    new ((void*)root) AEGeometry((PaintCanvas*)(long)(*g_StarMap_system_canvas));
    this->starSystemRoot = (AEGeometry *)root;

    Array<uint8_t> *used = new Array<uint8_t>();
    this->usedFlags = used;
    used->resize(stationGeoms->size());
    for (uint32_t i = 0; i < used->size(); i++) {
        (*used)[i] = 0;
    }

    Array<void *> *stationArr = (Array<void *> *)this->stations;
    Array<void *> *geomArr = (Array<void *> *)this->stationGeoms;
    for (uint32_t i = 1; i < geomArr->size(); i++) {
        uint32_t stationIndex = i - 1;
        int tex = Station_getTextureIndex(stationArr->data()[stationIndex]);
        void *geom = operator new(0xc0);
        new ((void*)geom) AEGeometry((uint16_t)(tex + 0x4704), (PaintCanvas*)(long)(*g_StarMap_system_canvas), false);
        geomArr->data()[i] = geom;
        ((int *)this->stationAngles)[stationIndex] =
            ((AbyssEngine::AERandom *)(*g_StarMap_system_random))->nextInt(used->size()) *
            (0x10000 / (int)used->size());
        int dist = (i == 1) ? 0x1900 : ((int *)this->stationDistances)[i - 2];
        dist += ((AbyssEngine::AERandom *)(*g_StarMap_system_random))->nextInt(0x15e0) + 0x640;
        ((int *)this->stationDistances)[stationIndex] = dist;
        Vector pos = {0.0f, 0.0f, (float)dist};
        ((AEGeometry *)(geom))->translate(pos);
        float scale = (float)(tex << 4) * 0.001f;
        ((AEGeometry *)(geom))->setScaling(scale);
        ((AEGeometry *)(root))->addChild(field<uint32_t>(geom, 0xc));
        if (this->markerGeom != 0 && this->selectedSystem == field<int32_t>(*g_StarMap_system_status, 0x7c) &&
            Station_getIndex(stationArr->data()[stationIndex]) == field<int32_t>(*g_StarMap_system_status, 0x80)) {
            this->centeredStation = i;
        }
    }

    ((AEGeometry *)(((Array<void *> *)this->stationGeoms)->data()[1]))->setVisible(false);

    Array<AEGeometry *> *rings = new Array<AEGeometry *>();
    this->ringGeoms = rings;
    rings->resize(count);
    for (uint32_t i = 0; i < rings->size(); i++) {
        void *ring = operator new(0xc0);
        new ((void*)ring) AEGeometry((uint16_t)0x1a7b, (PaintCanvas*)(long)(*g_StarMap_system_canvas), false);
        (*rings)[i] = (AEGeometry *)ring;
        ((AEGeometry *)(root))->addChild(field<uint32_t>(ring, 0xc));
        float scale = (float)(((int *)this->stationDistances)[i] << 1) * 0.001f;
        ((AEGeometry *)(ring))->setScaling(scale);
    }

    Vector selected;
    ((AEGeometry *)(&selected))->getPosition();
    ((AEGeometry *)(root))->setPosition(selected);
    ((AEGeometry *)(root))->setScaling(0.0078125f);
    ((AEGeometry *)(root))->setRotation(0.0f, 0.0f, 0.0f);
    this->yaw = 4096.0f;
    this->pitch = 0.0f;
    this->selectedStation = -1;
    ((PaintCanvas*)(long)(*g_StarMap_system_canvas))->Image2DCreate((unsigned short)((uint16_t)(0x4500 + ((SolarSystem *)(system))->getRace())), &this->systemNameImage);

    {
        Array<Vector *> *oldPositions = (Array<Vector *> *)this->stationPositions;
        if (oldPositions != 0) {
            for (Vector *v : *oldPositions) delete v;
            delete oldPositions;
        }
        this->stationPositions = 0;
    }
    Array<Vector *> *stationPositions = new Array<Vector *>();
    this->stationPositions = stationPositions;
    stationPositions->resize(((Array<void *> *)this->stations)->size());
    for (uint32_t i = 0; i < stationPositions->size(); i++) {
        Vector *p = (Vector *)operator new(0xc);
        p->x = 0.0f;
        p->y = 0.0f;
        p->z = 0.0f;
        (*stationPositions)[i] = p;
    }

    ((Engine *)(*g_StarMap_system_engine))->LightSetLightDirection(0.0f, 0.0f, 1.0f, 1);
    if (this->planetGeom != 0) {
        [&]{ AEGeometry *g_=(AEGeometry*)(this->planetGeom); if(g_){ g_->~AEGeometry(); ::operator delete(g_);} }();
    }
    this->planetTexture = 0xffffffffu;
    this->planetGeom = (AEGeometry *)0;
    uint16_t texture = (uint16_t)(0x2700 + ((SolarSystem *)(system))->getTextureIndex());
    if (this->selectedSystem == 0x1b) {
        texture = 0x2734;
    }
    ((PaintCanvas*)(long)(*g_StarMap_system_canvas))->TextureCreate((unsigned short)(texture), (void *)0, (void *)0, &this->planetTexture, false);
    void *planet = operator new(0xc0);
    new ((void*)planet) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)(long)(*g_StarMap_system_canvas), false);
    this->planetGeom = (AEGeometry *)planet;
    ((AEGeometry *)(planet))->setPosition(selected);
    ((AEGeometry *)(planet))->setRotation(0.0f, 0.0f, 0.0f);
    ((AEGeometry *)(planet))->setScaling(1.0f);
    ((AEGeometry *)(((Array<void *> *)this->systemGeoms)->data()[this->selectedSystem]))->setVisible(false);
}

void StarMap::drawOnScreenInfo(int index, bool stationMode)
{
    String name;
    String line;
    String value;

    Array<Vector *> *positions =
        stationMode ? (Array<Vector *> *)this->stationPositions : (Array<Vector *> *)this->systemPositions;
    this->scratchVector = *(const Vector *)(positions->data()[index]);
    float x = this->scratchVector.x;
    float y = this->scratchVector.y;
    if (x < 0.0f || x > (float)(*g_StarMap_info_screenW + 0x32) ||
        y < 0.0f || y > (float)(*g_StarMap_info_screenH + 0x32)) {
        return;
    }

    uint32_t canvas = *g_StarMap_info_canvas;
    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned int)(0xffffffffu));
    int *icons = (int *)this->iconBuffer;
    for (int i = 0; i != 5; i++) {
        icons[i] = -1;
    }

    if (stationMode) {
        void *station = ((Array<void *> *)this->stations)->data()[index];
        if (((Station *)(station))->isDiscovered() != 0) {
            icons[0] = this->keyImageDiscovered;
        }
        int current = Station_getIndex(((Status *)(*g_StarMap_info_status))->getStation());
        if (current == Station_getIndex(station)) {
            icons[3] = this->keyImageCurrent;
        }
        ((Station *)(&name))->getName();
        int textW = ((PaintCanvas*)(long)(canvas))->GetTextWidth((unsigned int)(long)(*g_StarMap_info_font), (void *)&name);
        int drawX = (int)(x - (float)(textW / 2));
        int drawY = (int)(y + (float)(this->iconWidth >> 1) - 3.0f);
        if (this->selectedStation == index) {
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0x80), (unsigned char)(0), (unsigned char)(this->alpha));
            ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(*g_StarMap_info_font), (void *)(&name), drawX, drawY, false);
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha));
            if (Station_getTecLevel(station) > 0) {
                line.copy((String *)((GameText *)(*g_StarMap_info_text))->getText(0x200), false);
                value.ctor_int(Station_getTecLevel(station));
                name = line + value;
                ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(*g_StarMap_info_font), (void *)(&name), drawX, drawY + field<int32_t>(*g_StarMap_info_layout, 4), false);
            }
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->selectIcon), (int)x, (int)y, (unsigned char)(0x11));
        } else {
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha));
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->cursorIcon), (int)x, (int)y, (unsigned char)(0x11));
            ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(*g_StarMap_info_font), (void *)(&name), drawX, drawY, false);
        }
    } else {
        void *system = ((Array<void *> *)this->systems)->data()[index];
        if (((SolarSystem *)(system))->isFullyDiscovered() != 0) {
            icons[0] = this->keyImageDiscovered;
        }
        if (((Status *)(*g_StarMap_info_status))->getCurrentCampaignMission() == 0x34 &&
            ((SolarSystem *)(system))->getStationEnumIndex(0x4a) >= 0) {
            icons[1] = this->keyImageWanted;
        }
        void *mission = (void *)(long)((Status *)(*g_StarMap_info_status))->getCampaignMission();
        if (mission != 0 && ((Mission *)(mission))->isEmpty() == 0) {
            int target = ((Mission *)(mission))->getTargetStation();
            if (((SolarSystem *)(system))->getStationEnumIndex(target) >= 0) {
                icons[2] = this->keyImageMission;
            }
        }
        void *freelance = ((Status *)(*g_StarMap_info_status))->getFreelanceMission();
        if (freelance != 0 && ((Mission *)(freelance))->isEmpty() == 0) {
            int target = ((Mission *)(freelance))->getTargetStation();
            if (((SolarSystem *)(system))->getStationEnumIndex(target) >= 0) {
                icons[2] = this->keyImageMission;
            }
        }
        ((SolarSystem *)(&name))->getName();
        int textW = ((PaintCanvas*)(long)(canvas))->GetTextWidth((unsigned int)(long)(*g_StarMap_info_font), (void *)&name);
        int drawX = (int)(x - (float)(textW / 2));
        int drawY = (int)(y + (float)(this->iconWidth >> 1) - 3.0f);
        int currentSystem = ((Status *)(*g_StarMap_info_status))->getSystem();
        if (currentSystem == ((SolarSystem *)(system))->getIndex()) {
            ((Layout *)(*g_StarMap_info_layout))->getPulseValue((float)this->alpha);
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha));
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->currentMarkerIcon), (int)x, (int)y, (unsigned char)(0x11));
        }
        if (this->selectedSystem == index) {
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0x80), (unsigned char)(0), (unsigned char)(this->alpha));
        } else if (this->selectedSystem >= 0) {
            ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha));
        }
        ((PaintCanvas*)(long)(canvas))->DrawString((unsigned int)(long)(*g_StarMap_info_font), (void *)(&name), drawX, drawY, false);
        ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(this->alpha));
        if (((SolarSystem *)(system))->hasNoOwner() == 0) {
            uint32_t image = this->raceImageDefault;
            int race = ((SolarSystem *)(system))->getRace();
            if (race == 2) {
                image = this->raceImageB;
            } else if (race == 1) {
                image = this->raceImageA;
            } else if (race == 0) {
                image = this->raceImageNeutral;
            }
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(image), drawX, drawY + 0xfd, (unsigned char)(0x11));
        }
        if (this->selectedSystem == index) {
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->selectIcon), (int)x, (int)y, (unsigned char)(0x11));
        } else {
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)(this->cursorIcon), (int)x, (int)y, (unsigned char)(0x11));
        }
    }

    float iconY = (float)(int)((this->scratchVector.y - (float)(this->iconWidth >> 1)) + 10.0f);
    float iconX = (float)(int)(this->scratchVector.x + (float)(this->iconWidth >> 1) - 7.0f);
    for (int i = 0; i != 5; i++) {
        int image = icons[i];
        if (image != -1) {
            int dx = (i == 0 && *g_StarMap_info_isGerman != 0) ? 0xc : 0x12;
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned int)((uint32_t)image), (int)iconX - dx, (int)iconY, (unsigned char)(0));
        }
    }
}

int StarMap::init(bool jumpMapMode, Mission *mission, bool param3, int param4)
{
    Matrix matrix;
    Vector pos;

    uint32_t canvas = *g_StarMap_init_canvas;
    ((PaintCanvas*)(long)(canvas))->FogEnable(0, (int)(true));
    this->autoMode = (uint8_t)param3;
    this->pad_0xa8_a = (uint8_t)jumpMapMode;
    this->pulseSystem = param4;

    void (*imageCreate)(uint32_t, int, void *) = g_StarMap_init_imageCreate;
    imageCreate(canvas, 0x4a1, (void *)&this->raceImageNeutral);
    imageCreate(canvas, 0x49c, (void *)&this->raceImageA);
    imageCreate(canvas, 0x49f, (void *)&this->raceImageB);
    imageCreate(canvas, 0x49e, (void *)&this->raceImageDefault);
    imageCreate(canvas, 0x452, (void *)&this->keyImageRetreat);
    imageCreate(canvas, 0x4a2, (void *)&this->keyImageDiscovered);
    imageCreate(canvas, 0x453, (void *)&this->keyImageCurrent);
    imageCreate(canvas, 0x455, (void *)&this->keyImageMission);
    imageCreate(canvas, 0x454, (void *)&this->keyImageWanted);
    imageCreate(canvas, 0x48c, (void *)&this->selectIcon);
    imageCreate(canvas, 0x48a, (void *)&this->cursorIcon);
    imageCreate(canvas, 0x4fd, (void *)&this->currentMarkerIcon);
    imageCreate(canvas, 0x545, (void *)&this->image_0x134);

    this->iconWidth = ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned int)(this->selectIcon));
    this->missionIconWidth = ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned int)(this->keyImageMission));
    this->jumpCost = 0;
    this->noRoute = 0;
    this->planetGeom = (AEGeometry *)0;
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

    this->easeX = (AbyssEngine::EaseInOut *)EaseInOut_ctor(operator new(0x10));
    this->easeY = (AbyssEngine::EaseInOut *)EaseInOut_ctor(operator new(0x10));
    this->easeZ = (AbyssEngine::EaseInOut *)EaseInOut_ctor(operator new(0x10));
    this->dragScale = field<int32_t>(*g_StarMap_init_layout, 0x90);
    this->prevCamera = (uint32_t)((PaintCanvas*)(long)(canvas))->CameraGetCurrent();
    ((PaintCanvas*)(long)(canvas))->CameraCreate(&this->camera);
    ((PaintCanvas*)(long)(canvas))->CameraSetPerspective(this->camera, 60.0f, 1.0f, 10000.0f);
    MatrixSetTranslation(&matrix, 0.0f, 0.0f, 0.0f);
    MatrixSetRotation(&matrix, 0.0f, 0.0f, 0.0f, 0.0f);
    ((PaintCanvas*)(long)(canvas))->CameraSetLocal(this->camera, *(const AbyssEngine::AEMath::Matrix *)(&matrix));
    ((PaintCanvas*)(long)(canvas))->CameraSetCurrent((unsigned int)(this->camera));

    void *status = *g_StarMap_init_status;
    int campaign = ((Status *)(status))->getCurrentCampaignMission();
    this->isGalaxyMode = campaign > 0xf;
    this->mode = campaign > 0xf ? 0 : 3;
    this->selectedSystem = ((Status *)(status))->getSystem();

    if (param3 != 0) {
        ((AEGeometry *)(&pos))->getPosition();
        this->scratchVector = *(const Vector *)(&pos);
    } else if (!jumpMapMode || this->isGalaxyMode == 0) {
        ((AEGeometry *)(&pos))->getPosition();
        this->scratchVector = *(const Vector *)(&pos);
    }

    this->cameraBaseX = this->scratchVector.x / 20.0f;
    this->cameraBaseZ = this->scratchVector.y / 20.0f;
    if (this->mode == 3) {
        this->selectedStation = -1;
        initStarSystem();
    } else {
        this->lastSelectedSystem = this->selectedSystem;
        Array<Station *> *stations = (Array<Station *> *)this->stations;
        if (stations != 0) {
            for (Station *s : *stations) delete s;
            stations->clear();
            delete stations;
            this->stations = (Array<Station *> *)0;
        }
        this->stations = new Array<Station *>();
        void *reader = operator new(1);
        FileRead_ctor(reader);
        this->stations =
            (Array<Station *> *)((FileRead *)(reader))->loadStationsBinary();
        operator delete(FileRead_dtor(reader));
    }
    if (param3 != 0) {
        this->lastSelectedSystem = -1;
        this->selectedSystem = -1;
    }

    this->showKey = 0;
    this->jumpMapModeA = 0;
    this->jumpMapModeB = 0;
    this->exitRequested = 0;
    void *button = operator new(0xc8);
    String *back = (String *)((GameText *)(*g_StarMap_init_text))->getText(0x190);
    new ((button)) TouchButton(back, 0, *g_StarMap_init_screenW - field<int32_t>(*g_StarMap_init_layout, 0x2c), *g_StarMap_init_screenH - field<int32_t>(*g_StarMap_init_layout, 0x2c), 0x22);
    this->backButton = (TouchButton *)button;
    this->systemPath = (Array<int> *)0;
    this->choiceWindow = (ChoiceWindow *)ChoiceWindow_ctor(operator new(0x5c));
    this->pathFinder = (SystemPathFinder *)SystemPathFinder_ctor(operator new(1));

    if (jumpMapMode && this->mode == 0 && mission != 0 && ((Mission *)(mission))->isEmpty() == 0 &&
        (((Mission *)(mission))->isVisible() != 0 || ((Mission *)(mission))->getType() == 0xe)) {
        this->targetSystem = -1;
        this->selectedSystem = -1;
        int target = ((Mission *)(mission))->getTargetStation();
        Array<void *> *systems = (Array<void *> *)this->systems;
        for (uint32_t i = 0; i < systems->size(); i++) {
            if (target >= 0) {
                this->targetSystem = (int)i;
                break;
            }
        }
        if (this->targetSystem >= 0) {
            int current = ((Status *)(status))->getSystem();
            this->systemPath =
                ((SystemPathFinder *)(this->pathFinder))->getSystemPath((Array<SolarSystem *> *)this->systems, current, this->targetSystem);
            this->pathAnim = 1;
            this->momentumFactor = 1.0f;
            this->selectedSystem = this->targetSystem;
        }
    }

    this->keyBoxWidth = 0;
    for (int i = 0; i < 6; i++) {
        int width = ((PaintCanvas*)(long)(canvas))->GetTextWidth((unsigned int)(long)(*g_StarMap_init_font), (void *)((GameText *)(*g_StarMap_init_text))->getText(0x112 + i));
        if (this->keyBoxWidth < width) {
            this->keyBoxWidth = width;
        }
    }
    this->keyBoxWidth += field<int32_t>(*g_StarMap_init_layout, 0x8c);
    this->keyBoxHeight =
        field<int32_t>(*g_StarMap_init_layout, 4) * 5 + field<int32_t>(*g_StarMap_init_layout, 0x2c) * 2;
    this->autoTimer = 0;
    void *cargo = (void *)((Ship *)(((Status *)(status))->getShip()))->getCargo();
    this->cargoAmount = cargo != 0 ? ((Item *)(cargo))->getAmount() : 0;

    this->bgLayer0 = (AEGeometry *)(void*)new ((void*)operator new(0xc0)) AEGeometry((uint16_t)0x41d2, (PaintCanvas*)(long)(canvas), false);
    ((AEGeometry *)(this->bgLayer0))->setRotation(0.0f, 0.0f, 0.0f);
    ((AEGeometry *)(this->bgLayer0))->setPosition((Vector){0.0f, 0.0f, 0.0f});
    this->bgLayer1 = (AEGeometry *)(void*)new ((void*)operator new(0xc0)) AEGeometry((uint16_t)0x41d3, (PaintCanvas*)(long)(canvas), false);
    ((AEGeometry *)(this->bgLayer1))->setRotation(0.0f, 0.0f, 0.0f);
    ((AEGeometry *)(this->bgLayer1))->setPosition((Vector){0.0f, 0.0f, 0.0f});
    this->bgLayer2 = (AEGeometry *)(void*)new ((void*)operator new(0xc0)) AEGeometry((uint16_t)0x41d4, (PaintCanvas*)(long)(canvas), false);
    ((AEGeometry *)(this->bgLayer2))->setRotation(0.0f, 0.0f, 0.0f);
    ((AEGeometry *)(this->bgLayer2))->setPosition((Vector){0.0f, 0.0f, 0.0f});
    return 0;
}
