#include "game/menu/ModStation.h"
#include <cstddef>
#include "game/core/Globals.h"
#include "engine/core/GameText.h"
#include "game/ship/Ship.h"
#include "engine/audio/FModSound.h"
#include "engine/render/PaintCanvas.h"
#include <new>
#include "game/ui/ChoiceWindow.h"
#include "game/core/CutScene.h"
#include "engine/math/EaseInOut.h"
#include "engine/math/EaseInOutMatrix.h"
#include "game/mission/Item.h"
#include "game/ui/MissionsWindow.h"
#include "game/world/NewsTicker.h"
#include "game/world/StarMap.h"
#include "game/mission/Achievements.h"
#include "engine/core/ApplicationManager.h"
#include "game/ui/DialogueWindow.h"
#include "engine/render/ImageFactory.h"
#include "game/core/Radio.h"
#include "game/ui/Layout.h"
#include "game/mission/RecordHandler.h"
#include "game/menu/SpaceLounge.h"
#include "game/mission/Generator.h"
#include "game/mission/Mission.h"
#include "game/mission/PendingProduct.h"
#include "game/world/Station.h"
#include "game/mission/Status.h"
#include "game/world/SolarSystem.h"
#include "game/ui/TouchButton.h"
#include "game/ui/MenuTouchWindow.h"
#include "game/world/Wanted.h"

using AbyssEngine::AEMath::VectorSignedToFloat;
using AbyssEngine::AEMath::Matrix;

struct HangarWindow {
    void OnTouchBegin(int touch, int coord);

    unsigned int OnTouchMove(int touch, int coord);
};

struct StatusWindow {
    int OnTouchBegin(int x, int y);

    int OnTouchMove(int x, int y);
};

struct HangarWindowHead {
    char shipChanged;
};

struct AppData {
    char pad00[0xd];
    char hideRadio;
    char pad0e[0x41 - 0x0e];
    char purchaseReady;
    char restoreNotice;
    char pad43[0x48 - 0x43];
    int purchaseId;
    char pad4c[0x58 - 0x4c];
    int *purchaseList;
    char pad5c[0x60 - 0x5c];
    Array<String *> *iapArray;
};

struct HintRecord {
    char flags[0x40];
};

struct TextRootRecord {
    char pad000[0x2ec];
    char noRoutesHintShown;
};

struct SettingsBlock {
    char pad00[0x2b];
    char wantedActivatedShown;
    char pad2c[0x38 - 0x2c];
    char gameWonShown;
    char dlc1WonShown;
};

#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(AppData, hideRadio) == 0x0d, "");
static_assert(offsetof(AppData, purchaseReady) == 0x41, "");
static_assert(offsetof(AppData, restoreNotice) == 0x42, "");
static_assert(offsetof(AppData, purchaseId) == 0x48, "");
static_assert(offsetof(AppData, purchaseList) == 0x58, "");
static_assert(offsetof(AppData, iapArray) == 0x60, "");
static_assert(offsetof(TextRootRecord, noRoutesHintShown) == 0x2ec, "");
static_assert(offsetof(SettingsBlock, wantedActivatedShown) == 0x2b, "");
static_assert(offsetof(SettingsBlock, gameWonShown) == 0x38, "");
static_assert(offsetof(SettingsBlock, dlc1WonShown) == 0x39, "");
#endif

static inline int FModSound_tryToStopMusicForBGMusic() { return (int)(Globals::sound->tryToStopMusicForBGMusic()); }

void AEMath_MatrixSetTranslation(Matrix *m, int x, int y, int z);

void AEMath_MatrixSetRotation(Matrix *m, Matrix *loc, int rx, int ry, int a4, int a5);

static inline int Station_getIndex(Station *st) { return st->getIndex(); }

class ScrollTouchBox;

unsigned char *ms_op_delete(unsigned char *p);

unsigned char *ArrayReleaseClasses_TouchButton(int *a);

unsigned char *Array_TB_dtor(int *p);

static inline unsigned char * HangarWindow_dtor(HangarWindow *p) { p->~HangarWindow(); return (unsigned char *)(p); }

static inline unsigned char *StarMap_dtor(StarMap *p) { p->~StarMap(); return (unsigned char*)p; }

static inline unsigned char * MissionsWindow_dtor(DialogueWindow * p) { ((MissionsWindow*)p)->~MissionsWindow(); return (unsigned char *)(p); }

static inline unsigned char * DialogueWindow_dtor(DialogueWindow * p) { p->~DialogueWindow(); return (unsigned char *)(p); }

static inline unsigned char * ChoiceWindow_dtor(ChoiceWindow * p) { p->~ChoiceWindow(); return (unsigned char *)(p); }

static inline unsigned char * StatusWindow_dtor(StatusWindow *p) { p->~StatusWindow(); return (unsigned char *)(p); }

static inline unsigned char * CutScene_dtor(CutScene * p) { p->~CutScene(); return (unsigned char *)(p); }

static inline unsigned char *Radio_dtor(Radio *p) { p->~Radio(); return (unsigned char*)p; }

static inline unsigned char *Array_RM_dtor(Array<Radio *> *p) { p->~Array(); return (unsigned char*)p; }

static inline unsigned char * ScrollTouchBox_dtor(ScrollTouchBox *p) { p->~ScrollTouchBox(); return (unsigned char *)(p); }

void ModStation::autosave() {
    if (Globals::status->getPlayingTime() - 1LL < 0)
        return;
    RecordHandler *rh = new RecordHandler();
    rh->recordStoreWrite(0);
    rh->recordStoreWritePreview(0);
    delete rh;
    reinterpret_cast<uint8_t*>(&this->cameraFlags)[1] = 1;
    if ((int) (intptr_t) this->dlcMenu != 0) {
    }
}

static Ship *(*g_ModStation_es_getShip)(Status *) = 0;

void ModStation::enterStation() {
    {
        void(Status:: * depart)(Station *) =
                reinterpret_cast<void (Status::*)(Station *)>(&Status::departStation);
        (Globals::status->*depart)(Globals::status->getStation());
    }
    ((Station *) (Globals::status->getStation()))->visit();
    Globals::achievements->applyNewMedals();

    Ship * (*getShip)(Status *) = g_ModStation_es_getShip;
    Item *e10 = ((Ship *) (getShip(Globals::status)))->getFirstEquipmentOfSort(10);
    Item *e9 = ((Ship *) (getShip(Globals::status)))->getFirstEquipmentOfSort(9);
    int shipIdx = ((Ship *) (getShip(Globals::status)))->getIndex();
    Globals::status->field_150 = shipIdx;

    int v;
    if (e10 == 0) {
        v = -1;
    } else {
        v = ((Item *) (e10))->getIndex();
    }
    Globals::status->field_154 = v;

    if (e9 == 0) {
        v = -1;
    } else {
        v = ((Item *) (e9))->getIndex();
    }
    Globals::status->field_158 = v;
    Globals::status->field_124 = 0;
    Globals::status->field_11c = 0;
}

static int *g_ModStation_ach_a = 0;

static int *g_ModStation_ach_b = 0;

static int *g_ModStation_ach_c = 0;

void ModStation::addAchievement(int medalId, int kind) {
    if (Globals::achievements->isEliteMedal(medalId) != 0)
        return;
    if ((unsigned) (kind - 1) < 2) {
        if (Globals::achievements->getValue(medalId, 3) != -1)
            *g_ModStation_ach_a = medalId * 3;
        if (kind == 1 && Globals::achievements->getValue(medalId, 2) != -1)
            g_ModStation_ach_b[1] = medalId * 3 + 1;
    }
    g_ModStation_ach_c[2] = (medalId * 3 + 3) - kind;
}

void ModStation::showMapWindow() {
    reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] = 0;
    reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] = 1;
}

void ModStation::setGameLoaded() {
    reinterpret_cast<uint8_t*>(&this->cameraFlags)[0] = 1;
    reinterpret_cast<uint8_t*>(&this->cameraFlags)[3] = 1;
}

static RecordHandler **g_ModStation_suspendObj = 0;

void ModStation::OnSuspend() {
    this->activeTouch = 0;
    RecordHandler **holder = g_ModStation_suspendObj;
    RecordHandler *obj = *holder;
    if (obj != 0)
        obj->saveOptions();
}

static FModSound **g_ModStation_resumeObj = 0;

static int **g_ModStation_resumeArg = 0;

void ModStation::OnResume() {
    FModSound **holder = g_ModStation_resumeObj;
    FModSound *obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = **g_ModStation_resumeArg;
    (*holder)->setVolume(1, (float) arg);
}

static int *g_cpp_stack = 0;

static int **g_cpp_status = 0;

static int *g_cpp_textId = 0;

static int **g_cpp_itemTable = 0;

static inline int Status_getCurrentCampaignMission_cpp() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline int GameText_getText_cpp(int id) { return (int)(Globals::gameText->getText(id)); }

int Status_getPendingProducts_cpp(int status);

static inline int Status_getStation_cpp() { return (int)(Globals::status->getStation()); }

static inline int Station_getIndex_cpp(Station * s) { return (int)(s->getIndex()); }

static inline int Status_getShip_cpp() { return (int)(Globals::status->getShip()); }

int Item_makeItem_cpp(int itemDesc);

void Ship_addCargo_cpp(int ship, Item *it);

int Item_getAmount_cpp();

static inline int Item_getIndex_cpp(Item * it) { return (int)(it->getIndex()); }
static inline unsigned char * PendingProduct_dtor_cpp(PendingProduct * p) { p->~PendingProduct(); return (unsigned char *)(p); }

void operator_delete_cpp(unsigned char *p);

void ChoiceWindow_setNotice_cpp(ChoiceWindow * cw);

static inline int GameText_getText_cppline(int id) { return (int)(Globals::gameText->getText(id)); }

void ModStation::checkPendingProducts() {
    int camp = Status_getCurrentCampaignMission_cpp();
    int textId = *(int *) g_cpp_textId;
    GameText_getText_cpp(textId);

    Array<PendingProduct *> *products =
            (Array<PendingProduct *> *) (intptr_t) Status_getPendingProducts_cpp(*(int *) g_cpp_status);
    if (products != 0) {
        Array<int> *itemTable = (Array<int> *) (intptr_t) * *g_cpp_itemTable;
        for (unsigned i = 0; i < products->size(); i = i + 1) {
            PendingProduct *pp = (*products)[i];
            if (pp != 0) {
                int dstStation = pp->stationIndex;
                Station *here = (Station *) Status_getStation_cpp();
                if (dstStation == Station_getIndex_cpp(here)) {
                    if (camp == 0x92 && pp->blueprintIndex == 0xd2) {
                        PendingProduct *o = (*products)[i];
                        if (o != 0)
                            operator_delete_cpp(PendingProduct_dtor_cpp(o));
                        (*products)[i] = 0;
                        goto done;
                    }
                    Item *it = (Item *) Item_makeItem_cpp(
                        (*itemTable)[pp->blueprintIndex]);
                    Ship_addCargo_cpp(Status_getShip_cpp(), it);
                    (void) Item_getAmount_cpp();
                    GameText_getText_cppline(Item_getIndex_cpp(it));

                    PendingProduct *o = (*products)[i];
                    if (o != 0)
                        operator_delete_cpp(PendingProduct_dtor_cpp(o));
                    (*products)[i] = 0;
                    reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
                }
            }
        }
    }

    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] != 0)
        ChoiceWindow_setNotice_cpp(this->choiceWindow);
done:;
}

static int *g_okp_stack = 0;

static inline void Status_checkForLevelUp_okp() { Globals::status->checkForLevelUp(); }

void leaveStation();

int GameText_getText_frag(int id);

void ChoiceWindow_set1_frag(ChoiceWindow *cw, int textStr);

int GameText_root_frag();

void ModStation::OnKeyPress(long long, long long key) {
    if (this->stationActive == 0)
        return;

    Status_checkForLevelUp_okp();

    int t34 = reinterpret_cast<uint32_t*>(&this->accumTime)[1];
    bool banner = 999 < (uint32_t) this->accumTime;
    if (!((-(int) banner - t34) < 0)) {
        if (key == 0x10000) {
            reinterpret_cast<uint32_t&>(this->accumTime) = 0x3e9;
            reinterpret_cast<uint32_t*>(&this->accumTime)[1] = 0;
        }
        return;
    }

    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] != 0 || reinterpret_cast<uint8_t*>(&this->modalFlags)[1] != 0 ||
        reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0 || reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0 ||
        reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0 || reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0 ||
        reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0)
        return;

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0) {
        if (key == 0x10000)
            leaveStation();
        return;
    }

    unsigned char held = reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[1];
    char sub = reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[2];
    if (held == 0) {
        if (key == 0x40000 && sub == 0)
            leaveStation();
        return;
    }

    if (sub != 0) {
        if (key == 0x20000)
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[1] = (unsigned char) (
                reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[1] ^ 1);
        return;
    }

    if (key == 0x4000) {
        int sel = this->selectedButton;
        int *btn = this->buttonState;
        btn[sel] = 0;
        sel = (0 < sel) ? (sel - 1) : 4;
        this->selectedButton = sel;
        btn[sel] = 1;
        return;
    }
    if (key == 0x8000) {
        int sel = this->selectedButton;
        int *btn = this->buttonState;
        btn[sel] = 0;
        sel = (sel < 4) ? (sel + 1) : 0;
        this->selectedButton = sel;
        btn[sel] = 1;
        return;
    }
    if (key == 0x20000)
        return;
    if (key != 0x10000)
        return;

    int which = this->selectedButton;
    if (which < 0 || which > 4) {
        ChoiceWindow_set1_frag(this->choiceWindow, GameText_getText_frag(GameText_root_frag()));
        return;
    }
    if (((which >= 0 && which <= 4) ? 1 : 0) == 0)
        ChoiceWindow_set1_frag(this->choiceWindow, GameText_getText_frag(GameText_root_frag()));
}

long long ModStation::OnKeyRelease(long long, long long key) {
    return key;
}

int ModStation::ShowLoadingScreen() {
    return 1;
}

static int *g_msc_stack = 0;

static inline int Status_getStation_msc() { return (int)(Globals::status->getStation()); }

static inline int Station_getIndex_msc(Station * s) { return (int)(s->getIndex()); }

static inline int Status_getSystem_msc() { return (int)(Globals::status->getSystem()); }

int SolarSystem_getRace_msc();

const int *ModStation_msc_camCoordTable();

const int *ModStation_msc_camRotTable();

ModStation::ModStation() {
    this->dt = 0;
    reinterpret_cast<uint8_t*>(&this->cameraTween)[3] = 0;
    reinterpret_cast<uint16_t&>(this->cameraTween) = 0;
    this->stationActive = 0;
    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
    this->state = 100;
    this->starMap = 0;
    this->cutScene = 0;
    this->newsTicker = 0;
    reinterpret_cast<uint32_t&>(this->cameraTween) = 0;
    this->dlcMenu = 0;
    this->activeMission = 0;
    this->radioMessages = 0;
    reinterpret_cast<uint8_t*>(&this->screenFlags)[2] = 0;
    this->pendingHangarClose = 0;
    this->m_pDialogueWindow = 0;
    this->choiceWindow = 0;

    Station *st = (Station *) Status_getStation_msc();
    int race;
    if (Station_getIndex_msc(st) == 0x65) {
        race = 8;
    } else {
        st = (Station *) Status_getStation_msc();
        if (Station_getIndex_msc(st) == 100) {
            race = 7;
        } else {
            Status_getSystem_msc();
            race = SolarSystem_getRace_msc();
        }
    }

    AbyssEngine::EaseInOutMatrix *cam;
    {
        const int *coord = ModStation_msc_camCoordTable();
        const int *rot = ModStation_msc_camRotTable();

        int ix = race * 3, iy = race * 3 + 1, iz = race * 3 + 2;
        float kx = VectorSignedToFloat(coord[ix], 0);
        float ky = VectorSignedToFloat(coord[iy], 0);
        float kz = VectorSignedToFloat(coord[iz], 0);
        float yaw = VectorSignedToFloat(rot[race], 0);

        this->camCoordX = kx;
        this->camCoordY = ky;
        this->camCoordZ = kz;

        Matrix nearKey, farKey;
        MatrixSetTranslation(nearKey, kx, ky, kz);
        MatrixSetRotation(nearKey, 0.0f, yaw, 0.0f);

        MatrixSetTranslation(farKey, kx, ky, kz);
        MatrixSetRotation(farKey, 0.0f, yaw, 0.0f);

        cam = new AbyssEngine::EaseInOutMatrix(nearKey, farKey, 3000);
    }
    this->cameraTween = cam;

    unsigned camHandle = *(unsigned *) Globals::Canvas;
    AbyssEngine::Matrix *cur = (AbyssEngine::Matrix *) Globals::Canvas->CameraGetCurrent();
    Globals::Canvas->CameraSetLocal(camHandle, *cur);

    this->easeX = new AbyssEngine::EaseInOut();
    this->easeY = new AbyssEngine::EaseInOut();
    this->easeZ = new AbyssEngine::EaseInOut();
}

ModStation::~ModStation() {
    this->OnRelease();
}

void ModStation::leaveStation() {
    leaveStation();
}

static int *g_ModStation_cm_credit1 = 0;

static int *g_ModStation_cm_credit2 = 0;

static int **g_ModStation_cm_lead = 0;

void ModStation::checkMedals() {
    Globals::globals->reportLeaderboards();

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0 || reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] != 0) {
        int idx = this->medalIndex + 1;
        this->medalIndex = idx;
        if (idx >= this->medalCount) {
            reinterpret_cast<uint8_t*>(&this->modalFlags)[2] = 0;
            return;
        }
        Array<int *> *medalArr = *(Array<int *> **) &this->medalArray;
        int *medal = (*medalArr)[idx];
        ((ChoiceWindow *) (this->medalChoiceWindow))->setMedal(medal[0], medal[1]);
        int delta = *g_ModStation_cm_credit2;
        if (Globals::status->hardCoreMode() == 0)
            Globals::status->changeCredits(delta);
        int *p = (*medalArr)[this->medalIndex];
        this->addAchievement(p[0], p[1]);
        return;
    }

    int delta = *g_ModStation_cm_credit1;
    if (Globals::status->getCurrentCampaignMission() == 1)
        ((ModStation *) ((ModStation *) 1))->addAchievement(0, 1);
    this->medalArray = 0;
    int *medals = Globals::achievements->getNewMedals();
    int count = 0;
    this->medalCount = 0;
    for (int i = 0; i != 0x2d; i++) {
        if (medals[i] >= 1) {
            count++;
            this->medalCount = count;
        }
    }
    if (count < 1)
        return;

    Array<int *> *arr = new Array<int *>();
    *(Array<int *> **) &this->medalArray = arr;
    ArraySetLength(this->medalCount, *arr);
    int j = 0;
    this->medalCount = 0;
    for (int i = 0; i != 0x2d; i++) {
        if (medals[i] >= 1) {
            int *cell = new int[2];
            (*arr)[j] = cell;
            cell[0] = i;
            (*arr)[this->medalCount][1] = medals[i];
            j = this->medalCount + 1;
            this->medalCount = j;
        }
    }
    this->medalIndex = 0;
    reinterpret_cast<uint8_t*>(&this->modalFlags)[2] = 1;
    ChoiceWindow *cw = new ChoiceWindow();
    this->medalChoiceWindow = cw;
    int *medal = (*arr)[0];
    cw->setMedal(medal[0], medal[1]);
    if (Globals::status->hardCoreMode() == 0)
        Globals::status->changeCredits(delta);
    int *p = (*arr)[0];
    this->addAchievement(p[0], p[1]);
}

void ModStation::OnRender3D() {
    if (this->stationActive == 0)
        return;
    Globals::Canvas->ClearBuffer((unsigned int) (long) Globals::Canvas);

    uint8_t *p65 = &reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1];
    if (this->cutScene == 0 || reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0 || reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0 ||
        reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0 || reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0 || *p65 != 0 ||
        reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[2] != 0) {
        if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0) {
            this->starMap->renderBG();
        } else if (*p65 != 0) {
            ((SpaceLounge *) (this->spaceLounge))->OnRenderBG();
        }
    } else {
        ((CutScene *) (this->cutScene))->renderBG();
    }
    Globals::Canvas->Begin3d();
    if (*p65 != 0)
        ((SpaceLounge *) (this->spaceLounge))->OnRender3D();
    else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0)
        this->starMap->render();
    else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0)
        ((MissionsWindow *) (this->m_pDialogueWindow))->render3D();
    else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] == 0 && this->cutScene != 0)
        ((CutScene *) (this->cutScene))->render3D();
    ((PaintCanvas *) Globals::Canvas)->End3d();
}

static int **g_rl_engineHolder = 0;

static int g_rl_lightFlag = 0;

int ApplicationManager_GetEngine_rl();

static inline int Status_getSystem_rl() { return (int)(Globals::status->getSystem()); }

int SolarSystem_getRace_rl();

int rl_engineFromHolder(int holder);

void Engine_resetStationLight(int engine, int race);

void Engine_LightEnable_rl(int engine, int flag);

void ModStation::resetLight() {
    int holder = *(int *) g_rl_engineHolder;
    rl_engineFromHolder(holder);
    rl_engineFromHolder(holder);
    rl_engineFromHolder(holder);

    Status_getSystem_rl();
    int race = SolarSystem_getRace_rl();
    int engine;
    switch (race) {
        case 1:
        case 2:
        case 3:
        case 8:
            engine = ApplicationManager_GetEngine_rl();
            break;
        default:
            engine = ApplicationManager_GetEngine_rl();
            break;
    }
    Engine_resetStationLight(engine, race);
    engine = ApplicationManager_GetEngine_rl();

    engine = ApplicationManager_GetEngine_rl();
    Engine_LightEnable_rl(engine, g_rl_lightFlag);
}

static int *g_ou_stack = 0;

static int **g_ou_sound = 0;

static int **g_ou_layout = 0;

static int **g_ou_status = 0;

static int **g_ou_achievements = 0;

static char **g_ou_spaceLoungeFlag = 0;

static int **g_ou_appData = 0;

static int **g_ou_textRoot = 0;

static int **g_ou_module = 0;

int ApplicationManager_GetElapsedTimeMillis_ou();

int ApplicationManager_GetApplicationData_ou();

void ApplicationManager_SetCurrentApplicationModule_ou(int module);

void FModSound_updateAll_ou(int sound);

void FModSound_play_ou(int sound, int id, const float *p, float vol);

static inline void FModSound_stop_ou(int sound) { Globals::sound->stop(sound); }

void FModSound_setParamValue_ou(int sound, int a, int b, float v);

void Layout_update_ou(int layout);

void Layout_formatCredits_ou(ModStation * outStr);

static inline void Status_incPlayingTime_ou(long long delta) { Globals::status->incPlayingTime(delta); }

static inline int Status_getCredits_ou() { return (int)(Globals::status->getCredits()); }

static inline void Status_changeCredits_ou(int status) { Globals::status->changeCredits(status); }

static inline int Status_getCurrentCampaignMission_ou() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline void Status_nextCampaignMission_ou(int status) { Globals::status->nextCampaignMission(status); }

static inline void Status_setCurrentCampaignMission_ou(int status) { Globals::status->setCurrentCampaignMission(status); }

static inline int Status_getStation_ou() { return (int)(Globals::status->getStation()); }

static inline int Station_getIndex_ou(Station * s) { return (int)(s->getIndex()); }

static inline int Status_getSystem_ou() { return (int)(Globals::status->getSystem()); }

int SolarSystem_getIndex_ou();

static inline int Status_getCampaignMission_ou() { return (int)(Globals::status->getCampaignMission()); }

static inline int Status_missionCompleted_ou(int status, int a, long long b) { return (int)(Globals::status->missionCompleted(status, a, b)); }

int Status_missionFailed_ou(int status, int a, int b);

void Status_removeMission_ou(int status);

int Mission_getType_ou();

int Mission_getStatusValue_ou();

void Mission_setStatusValue_ou(int v);

static inline void Mission_ctor_ou(Mission *m, int a, int b, int c) { new ((void*)m) Mission(a, b, c); }

void Mission_setCampaignMission_ou(Mission * mission);

static inline void Achievements_updateCredits_ou(Achievements *ach, int credits) { ach->updateCredits(credits); }

int Globals_getInAppPurchaseArrayIndex_ou(int globals, Array<String *> *arr);

void TouchButton_setText_ou(int btn);

void ChoiceWindow_set_ou(int cw, int text, int flag);

void ChoiceWindow_setNotice_ou(int cw, int text);

void ChoiceWindow_update_ou(int cw);

static inline int GameText_getText_ou(int id) { return (int)(Globals::gameText->getText(id)); }

void CutScene_process_ou(int cs);

void CutScene_update_ou(int cs);

static inline void CutScene_ctor_ou(CutScene *cs, int kind) { new ((void*)cs) CutScene(kind); }

int CutScene_initialize_ou(CutScene * cs);

void StarMap_update_ou(int sm, int dt);

void MissionsWindow_update_ou(int w);

void HangarWindow_update_ou(int w);

void HangarWindow_hideMessage_ou();

void StatusWindow_update_ou(int w);

static inline void MenuTouchWindow_update_ou(MenuTouchWindow *w, int dt) { w->update(dt); }

static inline void MenuTouchWindow_dtor_ou(MenuTouchWindow *w) { w->~MenuTouchWindow(); }

int SpaceLounge_introFinished_ou();

static inline void SpaceLounge_ctor_ou(SpaceLounge * l) { new ((void*)l) SpaceLounge(); }

static inline void SpaceLounge_init_ou(SpaceLounge * l) { l->init(); }

void SpaceLounge_update_ou(int l);

void SpaceLounge_setHangarUpdate_ou(SpaceLounge * l);

int Radio_lastMessageShown_ou();

float EaseInOut_advance_ou(AbyssEngine::EaseInOut *e, int elapsed);

unsigned ModStation_ou_cameraHandle();

void ModStation_ou_setCameraLocal(unsigned h, const Matrix &m);

static inline void AEGeometry_rotate_ou(AEGeometry *geom, float x, float y, float z) { geom->rotate(x, y, z); }

void Engine_setHangarLightIntensity_ou(float v);

void ModStation::OnUpdate() {
    int *status = *(int **) g_ou_status;
    int *sound = *(int **) g_ou_sound;

    int elapsed;
    int e = ApplicationManager_GetElapsedTimeMillis_ou();
    if (e < 0x97 && ApplicationManager_GetElapsedTimeMillis_ou() < 0) {
        elapsed = 0;
    } else {
        if (ApplicationManager_GetElapsedTimeMillis_ou() < 0x97)
            elapsed = ApplicationManager_GetElapsedTimeMillis_ou();
        else
            elapsed = 0x96;
    }

    this->dt = elapsed;
    this->accumTime += (long long) elapsed;

    FModSound_updateAll_ou(*sound);
    int *layout = *(int **) g_ou_layout;
    Layout_update_ou(*layout);
    if (reinterpret_cast<uint8_t*>(&this->accumTime)[2] == 0)
        Status_incPlayingTime_ou((long long) (unsigned) **(int **) g_ou_status);

    int creditsBtn = this->activeMission;
    Status_getCredits_ou();
    Layout_formatCredits_ou(this);
    TouchButton_setText_ou(creditsBtn);

    Achievements_updateCredits_ou((Achievements *) **(int **) g_ou_achievements, Status_getCredits_ou());

    if (this->stationActive != 0) {
        char *flag = *g_ou_spaceLoungeFlag;
        if (*flag != 0) {
            reinterpret_cast<uint8_t*>(&this->buttonCredits)[0] = 1;
            *flag = 0;
            if (this->spaceLounge == 0) {
                SpaceLounge *sl = (SpaceLounge *) ::operator new(0x10c);
                SpaceLounge_ctor_ou(sl);
                this->spaceLounge = sl;
            } else {
                SpaceLounge_init_ou(this->spaceLounge);
            }
            FModSound_setParamValue_ou(*sound, 0, *sound, 0.0f);
            FModSound_stop_ou(*sound);
            FModSound_play_ou(*sound, 0x6c, 0, 0.0f);
            reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] = 1;
            goto epilogue;
        }
    }

    {
        AppData *appData = (AppData *) (intptr_t) ApplicationManager_GetApplicationData_ou();
        if (appData->restoreNotice != 0) {
            ChoiceWindow_setNotice_ou((int) (intptr_t) this->choiceWindow, GameText_getText_ou(**g_ou_textRoot));
            reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 1;
            appData->restoreNotice = 0;
        }
        if (appData->purchaseReady != 0 && appData->purchaseList != 0 &&
            *appData->purchaseList != 0 &&
            (unsigned) (appData->purchaseId - 0x32) < 5) {
            int idx = Globals::globals->getInAppPurchaseArrayIndex(appData->purchaseId,
                                                                   appData->iapArray);
            if (-1 < (intptr_t)(*appData->iapArray)[idx]) {
                Status_changeCredits_ou(*status);
                this->autosave();
                ChoiceWindow_setNotice_ou((int) (intptr_t) this->choiceWindow, GameText_getText_ou(**g_ou_textRoot));
                reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 1;
                appData->purchaseReady = 0;
                if ((int) (intptr_t) this->hangarWindow != 0)
                    HangarWindow_hideMessage_ou();
                if ((int) (intptr_t) this->dlcMenu != 0) {
                    MenuTouchWindow_dtor_ou(this->dlcMenu);
                    ::operator delete(this->dlcMenu);
                }
                this->dlcMenu = 0;
            }
        }
    }

    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] == 0 && reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] == 0 &&
        reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] == 0 && reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] == 0 &&
        reinterpret_cast<uint8_t*>(&this->modalFlags)[0] == 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] == 0) {
        if (this->newsTicker != nullptr)
            this->newsTicker->update(0);
        if ((int) (intptr_t) this->cutScene != 0)
            CutScene_process_ou((int) (intptr_t) this->cutScene);
        {
            float bx = this->camCoordX;
            float by = this->camCoordY;
            float bz = this->camCoordZ;

            float dx = EaseInOut_advance_ou(this->easeX, elapsed);
            float dy = EaseInOut_advance_ou(this->easeY, elapsed);
            float dz = EaseInOut_advance_ou(this->easeZ, elapsed);
            Matrix cam;
            MatrixSetTranslation(cam, bx + dx, by + dy, bz + dz);
            ModStation_ou_setCameraLocal(ModStation_ou_cameraHandle(), cam);
        }
    }

    if (this->accumTime < (long long) ((int) this->accumTime < 1000 ? 1 : 0))
        goto epilogue;

    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] != 0) {
        int t = (int) (intptr_t) this->spaceLounge;
        if (t < 1 + 0 && 0 < this->dt + t) {
            if (this->cutScene != 0) {
                ::operator delete(this->cutScene);
            }
            this->cutScene = 0;
            CutScene *cs = (CutScene *) ::operator new(0xa0);
            CutScene_ctor_ou(cs, 2);
            this->cutScene = cs;
            CutScene_initialize_ou(cs);
            cs->rotationSpeed = 0;
            t = (int) (intptr_t) this->spaceLounge;
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] = 1;
        }
        this->spaceLounge = (SpaceLounge *) (intptr_t)(t + this->dt);
        CutScene_update_ou((int) (intptr_t) this->cutScene);

        if (Radio_lastMessageShown_ou() != 0) {
            float scroll = reinterpret_cast<float&>(this->touchX);
            float target = reinterpret_cast<float&>(this->touchY);

            float step = VectorSignedToFloat(elapsed, 0) * 0.004f;
            if (scroll < target) {
                scroll += (target - scroll) * step;
                if (scroll > target) scroll = target;
            } else if (scroll > target) {
                scroll -= (scroll - target) * step;
                if (scroll < target) scroll = target;
            }
            reinterpret_cast<float&>(this->touchX) = scroll;
        }

        if ((int) (intptr_t) this->spaceLounge >= 0 /* cutscene countdown elapsed */) {
            this->stationActive = 0;
            Status_nextCampaignMission_ou(*status);
            int nextModule = **(int **) g_ou_appData;
            ApplicationManager_SetCurrentApplicationModule_ou(nextModule);
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] = 0;
            goto epilogue;
        }
    } else if (reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0) {
        this->dialogueWindow->update(0);
    } else if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] != 0) {
        StarMap_update_ou((int) (intptr_t) this->starMap, this->dt);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0) {
        HangarWindow_update_ou((int) (intptr_t) this->hangarWindow);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
        MissionsWindow_update_ou((int) (intptr_t) this->m_pDialogueWindow);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0) {
        SpaceLounge_update_ou((int) (intptr_t) this->spaceLounge);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0) {
        StatusWindow_update_ou((int) (intptr_t) this->statusWindow);
    }

    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[3] != 0)
        ChoiceWindow_update_ou((int) (intptr_t) this->choiceWindow);
    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[2] != 0)
        MenuTouchWindow_update_ou(this->dlcMenu, this->dt);

    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[3] == 0 && reinterpret_cast<uint8_t*>(&this->modalFlags)[1] == 0 &&
        reinterpret_cast<uint8_t*>(&this->screenFlags)[0] == 0) {
        if (reinterpret_cast<uint8_t*>(&this->buttonCredits)[2] == 0) {
            this->checkPendingProducts();
            reinterpret_cast<uint8_t*>(&this->buttonCredits)[2] = 1;
        }
        if (reinterpret_cast<uint8_t*>(&this->buttonCredits)[1] == 0) {
            this->checkMedals();
            reinterpret_cast<uint8_t*>(&this->buttonCredits)[1] = 1;
        }

        if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 && reinterpret_cast<uint8_t*>(&this->screenFlags)[2] != 0) {
            Status *rec = (Status *) (intptr_t) * (int *) g_ou_status;
            if (rec->byte_0x2a == 0) {
                ChoiceWindow_setNotice_ou((int) (intptr_t) this->choiceWindow, GameText_getText_ou(**g_ou_textRoot));
                reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 1;
                rec->byte_0x2a = 1;
                reinterpret_cast<uint8_t*>(&this->screenFlags)[2] = 0;
            }
        }

        bool special = false;
        int intro = (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0) ? SpaceLounge_introFinished_ou() : 0;
        int completed = Status_missionCompleted_ou(*status, 1, (long long) intro);
        Mission *m = (Mission *) completed;
        int camp = Status_getCurrentCampaignMission_ou();
        if (m == 0 && camp == 0x74) {
            if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0 && SpaceLounge_introFinished_ou() != 0) {
                Status_getSystem_ou();
                if (SolarSystem_getIndex_ou() == 0x12) {
                    Status_getCampaignMission_ou();
                    int mask = Mission_getStatusValue_ou();
                    Station *st = (Station *) Status_getStation_ou();
                    int sidx = Station_getIndex_ou(st);
                    special = true;
                    if ((1 << ((sidx - 0x5a) & 0xff) & mask) == 0) {
                        int v = Status_getCampaignMission_ou();
                        Status_getCampaignMission_ou();
                        Mission_getStatusValue_ou();
                        Status_getStation_ou();
                        Mission_setStatusValue_ou(v);
                    }
                }
            }
        }

        int campNow = Status_getCurrentCampaignMission_ou();
        bool generic = campNow < 0x94 || Status_getCurrentCampaignMission_ou() > 0x97;
        if (!generic) {
            Station *st = (Station *) Status_getStation_ou();
            int sidx = Station_getIndex_ou(st);
            int bit = (sidx == 0x42) ? 2 : (sidx == 0x37) ? 1 : (sidx == 9) ? 4 : 0;
            bool introOk = reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0 && SpaceLounge_introFinished_ou() != 0;
            if (m == 0) {
                if (introOk && 0x93 < Status_getCurrentCampaignMission_ou() &&
                    Status_getCurrentCampaignMission_ou() < 0x97 &&
                    (sidx == 9 || sidx == 0x37 || sidx == 0x42)) {
                    Status_getCampaignMission_ou();
                    if ((Mission_getStatusValue_ou() & bit) == 0) {
                        int v = Status_getCampaignMission_ou();
                        Status_getCampaignMission_ou();
                        Mission_getStatusValue_ou();
                        Mission_setStatusValue_ou(v);
                        Status_getStation_ou();
                        DialogueWindow *dw = new DialogueWindow();
                        this->dialogueWindow = dw;
                        Mission *nm = (Mission *) ::operator new(0x78);
                        Mission_ctor_ou(nm, 0xa0, 0, -1);
                        Mission_setCampaignMission_ou(nm);
                        this->dialogueWindow->set(nm, 1, -1);
                        reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 1;
                        goto afterDialogue;
                    }
                }
                generic = true;
            } else if (introOk && 0x93 < Status_getCurrentCampaignMission_ou() &&
                       sidx == 0x60 && Status_getCurrentCampaignMission_ou() <= 0x96) {
                Status_setCurrentCampaignMission_ou(*status);
                DialogueWindow *dw = new DialogueWindow();
                this->activeMission = (int) (intptr_t) m;
                this->dialogueWindow = dw;
                this->dialogueWindow->set(m, 1, -1);
                reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 1;
                goto afterDialogue;
            } else {
                generic = true;
            }
        }
        if (generic) {
            if (special) {
                Station *st = (Station *) Status_getStation_ou();
                int sidx = Station_getIndex_ou(st);
                int slot = (sidx == 0x5e) ? 3 : (sidx - 0x5a);
                FModSound_play_ou(*sound, slot + 0x619, 0, 0.0f);
                int kind = (slot == 1) ? 0x39 : 0x3a;
                if (slot == 0 || slot == 2) kind = 0;
                DialogueWindow *dw = new DialogueWindow(
                    (String *) (intptr_t) GameText_getText_ou(**g_ou_textRoot),
                    (String *) (intptr_t) GameText_getText_ou(**g_ou_textRoot),
                    (int *) (long) kind);
                reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 1;
                this->dialogueWindow = dw;
            } else if (m == 0) {
                Mission *fm = (Mission *) Status_missionFailed_ou(*status, 1, 0);
                if (fm != 0) {
                    DialogueWindow *dw = new DialogueWindow(fm, (Level *) 0, 2);
                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 1;
                    this->dialogueWindow = dw;
                    Status_removeMission_ou(*status);
                    if (Mission_getType_ou() == 0xd) {
                        ((Status *) (intptr_t) * status)->field_0xf0 = 0;
                        this->autosave();
                    }
                }
            } else {
                this->activeMission = (int) (intptr_t) m;
                DialogueWindow *dw = new DialogueWindow(m, (Level *) 0, 1);
                reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 1;
                this->dialogueWindow = dw;
                if (Mission_getType_ou() == 0xd) {
                    ((Status *) (intptr_t) * status)->field_0xf0 = 0;
                    this->autosave();
                }
                if (this->spaceLounge != 0)
                    SpaceLounge_setHangarUpdate_ou(this->spaceLounge);
            }
        }
    afterDialogue:
        this->checkHints();
    }

    if (reinterpret_cast<uint8_t*>(&this->dragFlags)[0] == 0) {
        float cur = reinterpret_cast<float&>(this->scrollFlags);
        float tgt = reinterpret_cast<float&>(this->scrollTarget);
        float step = VectorSignedToFloat(elapsed, 0) * 0.002f;
        if (cur < tgt) {
            cur += step;
            if (cur > tgt) cur = tgt;
        } else if (cur > tgt) {
            cur -= step;
            if (cur < tgt) cur = tgt;
        }
        if (cur < 0.0f) cur = 0.0f;
        if (cur > 1.0f) cur = 1.0f;
        reinterpret_cast<float&>(this->scrollFlags) = cur;
        Engine_setHangarLightIntensity_ou(cur);
    }
    if ((int) (intptr_t) this->cutScene != 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] == 0) {
        AEGeometry *geom = this->hangarGeom;
        if (geom != nullptr)
            AEGeometry_rotate_ou(geom, 0.0f, 0.1f, 0.0f);
    }

epilogue:;
}

static int **g_ModStation_ric_chk = 0;

static int *g_ModStation_ric_rotX = 0;

static int *g_ModStation_ric_rotY = 0;

void ModStation::resetIdleCamForHangar() {
    char matrix[60];

    if (this->cutScene != 0)
        ((CutScene *) (this->cutScene))->resetCamera();

    if (this->easeX != 0)
        ((AbyssEngine::EaseInOut *) (this->easeX))->SetRange(
            this->camKeyX, this->camKeyX);
    else
        this->easeX = new AbyssEngine::EaseInOut(this->camKeyX,
                                                 this->camKeyX);

    if (this->easeY != 0)
        ((AbyssEngine::EaseInOut *) (this->easeY))->SetRange(
            this->camKeyY, this->camKeyY);
    else
        this->easeY = new AbyssEngine::EaseInOut(this->camKeyY,
                                                 this->camKeyY);

    if (this->easeZ != 0)
        ((AbyssEngine::EaseInOut *) (this->easeZ))->SetRange(
            this->camKeyZ, this->camKeyZ);
    else
        this->easeZ = new AbyssEngine::EaseInOut(this->camKeyZ,
                                                 this->camKeyZ);

    Globals::Canvas->CameraGetCurrent();
    Matrix *loc = (Matrix *) Globals::Canvas->CameraGetLocal((unsigned int) (long) Globals::Canvas);

    AEMath_MatrixSetTranslation((Matrix *) matrix, this->camKeyX, this->camKeyY,
                                this->camKeyZ);

    int race;
    Station *st = Globals::status->getStation();
    if (Station_getIndex(st) == 0x65) {
        race = 8;
    } else {
        st = Globals::status->getStation();
        if (Station_getIndex(st) == 100) {
            race = 7;
        } else {
            race = ((SolarSystem *) (long) Globals::status->getSystem())->getRace();
        }
    }

    Globals::Canvas->CameraGetCurrent();
    Matrix *loc2 = (Matrix *) Globals::Canvas->CameraGetLocal((unsigned int) (long) Globals::Canvas);
    AEMath_MatrixSetRotation((Matrix *) matrix, loc2, g_ModStation_ric_rotX[race], g_ModStation_ric_rotY[race], 0, 2);
    (void) loc;
}

static int *g_ch_stack = 0;

static int *g_ch_hintRec = 0;

static int **g_ch_status = 0;

static Achievements **g_ch_ach = 0;

static inline int Status_getCurrentCampaignMission_ch() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline int Wanted_isTerminated_ch(Wanted * w) { return (int)(w->isTerminated()); }
static inline int Achievements_gotAllMedals_ch(Achievements * a) { return (int)(a->gotAllMedals()); }

static inline int Achievements_gotAllGoldMedals_ch() { return (int)(Globals::achievements->gotAllGoldMedals()); }

static inline int Achievements_gotAllSupernovaMedals_ch(Achievements * a) { return (int)(a->gotAllSupernovaMedals()); }

static inline int Status_isBlueprintUnlocked_ch(Status *status, int bp) { return (int)(status->isBlueprintUnlocked(bp)); }

static inline void Status_unlockBluePrint_ch(Status *status, int bp) { status->unlockBluePrint(bp); }

static inline int Status_hardCoreMode_ch() { return (int)(Globals::status->hardCoreMode()); }

static inline int GameText_getText_ch(int id) { return (int)(Globals::gameText->getText(id)); }

int GameText_text_ch(int slot);

void ChoiceWindow_set6_ch(ChoiceWindow *cw, int a, int b, int c, int d, int e);

void ChoiceWindow_set1_ch(ChoiceWindow *cw, int textStr);

void DialogueWindow_initWingman_ch(DialogueWindow *dw, int kind);

void FModSound_playWingmanRecruit_ch();

void ModStation::checkHints() {
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] != 0)
        return;

    HintRecord *hintRec = (HintRecord *) (intptr_t) * g_ch_hintRec;

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 &&
        hintRec->flags[0x34] == 0 &&
        0x12 < Status_getCurrentCampaignMission_ch()) {
        ChoiceWindow_set6_ch(this->choiceWindow,
                             GameText_getText_ch(0x3e), GameText_getText_ch(0x49),
                             GameText_getText_ch(0x7e), GameText_getText_ch(0x7f),
                             GameText_getText_ch(0x20c));
        hintRec->flags[0x34] = 1;
        reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
        reinterpret_cast<uint8_t*>(&this->hintFlags)[0] = 1;
    }

    int **status = g_ch_status;

    static const int wantedOff[4] = {0x18, 0x30, 0x48, 0x60};
    static const int wantedFlag[4] = {0x33, 0x34, 0x35, 0x36};
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] == 0) {
        for (int k = 0; k < 4; k = k + 1) {
            if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0) return;
            if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 && hintRec->flags[wantedFlag[k]] == 0) {
                Status *st = (Status *) (intptr_t) * status;
                Wanted *w = (*st->wanted)[wantedOff[k] / 4];
                if (Wanted_isTerminated_ch(w) != 0) {
                    ChoiceWindow_set1_ch(this->choiceWindow, GameText_getText_ch(wantedOff[k]));
                    reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
                    hintRec->flags[wantedFlag[k]] = 1;
                }
            }
        }
    }

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0) {
        if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 && hintRec->flags[0x1a] == 0 &&
            Achievements_gotAllMedals_ch(*g_ch_ach) != 0) {
            ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x1a));
            hintRec->flags[0x1a] = 1;
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
        }
        if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 &&
            hintRec->flags[0x1b] == 0 &&
            Achievements_gotAllGoldMedals_ch() != 0) {
            ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x1b));
            hintRec->flags[0x1b] = 1;
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
        }

        if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0) {
            Status *statPtr = (Status *) *status;
            if (Status_isBlueprintUnlocked_ch(statPtr, 0xe8) == 0 &&
                Achievements_gotAllGoldMedals_ch() != 0 &&
                Achievements_gotAllSupernovaMedals_ch(*g_ch_ach) != 0) {
                ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x3b));
                Status_unlockBluePrint_ch(statPtr, 0xe8);
                this->autosave();
                reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
            }
        }

        if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 &&
            hintRec->flags[0x3a] == 0 &&
            0xa1 < Status_getCurrentCampaignMission_ch()) {
            bool ok = (Achievements_gotAllGoldMedals_ch() != 0 &&
                       Achievements_gotAllSupernovaMedals_ch(*g_ch_ach) != 0 &&
                       Status_hardCoreMode_ch() == 0) ||
                      Status_hardCoreMode_ch() != 0;
            if (ok) {
                hintRec->flags[0x3a] = 1;
                ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x3c));
                this->autosave();
                reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
            }
        }
    }

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] == 0 && reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 &&
        reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0) {
        delete this->dialogueWindow;
        this->dialogueWindow = nullptr;
        DialogueWindow *dw = (DialogueWindow *) ::operator new(0x74);
        DialogueWindow_initWingman_ch(dw, 0);
        this->dialogueWindow = dw;
        FModSound_playWingmanRecruit_ch();
        reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 1;
        reinterpret_cast<uint8_t*>(&this->alarmFlags)[3] = 1;
    }
}

static Layout **g_ModStation_tm_layout = 0;

static int **g_ModStation_tm_screenH = 0;

void ModStation::OnTouchMove(int x, int y, void *touch) {
    // lint: void_ptr
    if (this->activeTouch != touch)
        return;
    this->touchX = x;
    this->touchY = y;
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] != 0)
        return;

    Layout **layoutHolder = g_ModStation_tm_layout;
    Layout *layoutObj = *layoutHolder;
    if (layoutObj->choiceWindowOpen != 0) {
        layoutObj->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] != 0) {
        ((DialogueWindow *) (this->dialogueWindow))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] != 0) {
        ((ChoiceWindow *) (this->choiceWindow))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0) {
        ((ChoiceWindow *) (this->medalChoiceWindow))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
        ((HangarWindow *) (this->hangarWindow))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0) {
        this->starMap->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0) {
        ((SpaceLounge *) (this->spaceLounge))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0) {
        ((StatusWindow *) (this->statusWindow))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0) {
        ((MissionsWindow *) (this->m_pDialogueWindow))->OnTouchMove(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[2] != 0) {
        ((MenuTouchWindow *) (this->dlcMenu))->OnTouchMove(x, y, nullptr);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[1] == 0)
        return;
    ((TouchButton *) (this->buttonLaunch))->OnTouchMove(x, y);
    ((TouchButton *) (this->buttonCredits))->OnTouchMove(x, y);
    for (int i = 0; i != 5; i++)
        (((TouchButton **) (intptr_t) this->buttonRow[1])[i])->OnTouchMove(x, y);
    layoutObj->OnTouchMove(x, y);
    if (((NewsTicker *) (this->newsTicker))->OnTouchMove(x, y) != 0)
        return;
    Layout *layout = layoutObj;
    if (y <= layout->field_0xc_leftMargin)
        return;
    if (*(int *) *g_ModStation_tm_screenH - layout->field_0x10_rightMargin <= y)
        return;
    if (x <= (int) (intptr_t) this->hangarGeom)
        return;
    int delta = x - this->camScrollPos;
    this->camScrollVel = delta;
    this->field_0xf4 = 0x3f800000;
    this->camAngle = this->camAngle + delta;
    this->camScrollPos = x;
}

static FModSound **g_ModStation_or_sound = 0;

static Layout **g_ModStation_or_reload = 0;

static ImageFactory **g_ModStation_or_imgfac = 0;

void ModStation::OnRelease() {
    FModSound **soundHolder = g_ModStation_or_sound;
    if (*soundHolder != 0) {
        (*soundHolder)->disableReverb();
        (*soundHolder)->stopAllSoundFXEvents();
    }
    Globals::Canvas->FogEnable(0, AbyssEngine::FogMode_dummy);

    if (this->buttonRow != 0) {
        ArrayReleaseClasses_TouchButton(this->buttonRow);
        if (this->buttonRow != 0)
            ms_op_delete(Array_TB_dtor(this->buttonRow));
    }
    this->buttonRow = 0;

    if (this->hangarWindow != 0)
        ms_op_delete(HangarWindow_dtor(this->hangarWindow));
    this->hangarWindow = 0;

    if (this->starMap != 0)
        ms_op_delete(StarMap_dtor(this->starMap));
    this->starMap = 0;

    delete (SpaceLounge *) this->spaceLounge;
    this->spaceLounge = 0;

    if (this->m_pDialogueWindow != 0)
        ms_op_delete(MissionsWindow_dtor(this->m_pDialogueWindow));
    this->m_pDialogueWindow = 0;

    if (this->dialogueWindow != 0)
        ms_op_delete(DialogueWindow_dtor(this->dialogueWindow));
    this->dialogueWindow = 0;

    if (this->medalChoiceWindow != 0)
        ms_op_delete(ChoiceWindow_dtor(this->medalChoiceWindow));
    this->medalChoiceWindow = 0;

    if (this->statusWindow != 0)
        ms_op_delete(StatusWindow_dtor(this->statusWindow));
    this->statusWindow = 0;

    if (this->cutScene != 0)
        ms_op_delete(CutScene_dtor(this->cutScene));
    this->cutScene = 0;

    delete (MenuTouchWindow *) this->dlcMenu;
    this->dlcMenu = 0;

    if (this->activeMission != 0)
        ms_op_delete(Radio_dtor((Radio *) (intptr_t) this->activeMission));
    this->activeMission = 0;

    if (this->radioMessages != 0)
        ms_op_delete(Array_RM_dtor(this->radioMessages));
    this->radioMessages = 0;

    if (this->newsTicker != 0)
        ms_op_delete((unsigned char *) (this->newsTicker->~NewsTicker(), this->newsTicker));
    this->newsTicker = 0;

    if (this->choiceWindow != 0)
        ms_op_delete(ChoiceWindow_dtor(this->choiceWindow));
    this->choiceWindow = 0;

    Globals::Canvas->ReleaseAllResources();
    Globals::globals->loadFont(GameText::getLanguage());

    Layout **reloadHolder = g_ModStation_or_reload;
    if (*reloadHolder != 0) {
        (*reloadHolder)->reload();
        (*g_ModStation_or_imgfac)->reload();
        (*reloadHolder)->initTip();
    }

    delete this->cameraTween;
    this->cameraTween = 0;

    delete (AbyssEngine::EaseInOut *) this->easeX;
    this->easeX = 0;

    delete (AbyssEngine::EaseInOut *) this->easeY;
    this->easeY = 0;

    delete (AbyssEngine::EaseInOut *) this->easeZ;
    this->easeZ = 0;

    if (this->activeMission != 0)
        ms_op_delete(Radio_dtor((Radio *) (intptr_t) this->activeMission));
    this->activeMission = 0;

    if (this->scrollBox != 0)
        ms_op_delete(ScrollTouchBox_dtor(this->scrollBox));
    this->scrollBox = 0;

    reinterpret_cast<uint16_t&>(this->cameraFlags) = 0;
    if (*soundHolder != 0)
        (*g_ModStation_or_sound)->freeAllEvents();
}

static int *g_ote_stack = 0;

static int **g_ote_helpLayout = 0;

static int **g_ote_status = 0;

static int **g_ote_achievements = 0;

static int **g_ote_module = 0;

static int **g_ote_canvas = 0;

static int **g_ote_sound = 0;

static int **g_ote_textRoot = 0;

static int **g_ote_galaxy = 0;

static int **g_ote_itemTable = 0;

static int **g_ote_shipTable = 0;

void ApplicationManager_SetCurrentApplicationModule_ote(int module);

int ApplicationManager_GetApplicationData_ote();

static inline int Status_getStation_ote() { return (int)(Globals::status->getStation()); }

static inline int Station_getIndex_ote(Station * s) { return (int)(s->getIndex()); }

int Station_stationHasPirateBase_ote();

int Station_hasShip_ote(Station * station);

static inline int Station_getAgents_ote(Station * s) { return (int)(s->getAgents()); }
void Station_addShip_ote(Ship * s);
void Station_departStation_ote(Station * s);

static inline void Station_setAttackedFriends_ote(Station *s, int flag) { s->setAttackedFriends(flag); }

static inline void Station_setItems_ote(Station *s, Array<Item *> *arr, int flag) { s->setItems(arr, flag); }

static inline int Status_getShip_ote() { return (int)(Globals::status->getShip()); }

static inline int Status_getCredits_ote() { return (int)(Globals::status->getCredits()); }

static inline void Status_changeCredits_ote(int status) { Globals::status->changeCredits(status); }

static inline int Status_getCurrentCampaignMission_ote() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline void Status_nextCampaignMission_ote(int status) { Globals::status->nextCampaignMission(status); }

static inline void Status_departStation_ote(Station * s) { Globals::status->departStation(s); }
static inline void Status_setStation_ote(Station * s) { Globals::status->setStation(s); }

void Status_setMission_ote(int status);

void Status_removeMission_ote(int status);

void Status_incMissionCount_ote(int status);

void Status_setPassengers_ote(int status, int n);

static inline void Status_checkForLevelUp_ote() { Globals::status->checkForLevelUp(); }

static inline void Status_activateNewWanted_ote() { Globals::status->activateNewWanted(); }

int Ship_getCargo_ote();

int Ship_getIndex_ote();

static inline void Ship_getRace_ote(Ship * s) { s->getRace(); }

void Ship_removeCargo_ote(int ship, int item);

void Ship_removeCargo1_ote(Item * it);
void Ship_addCargo_ote(Item * it);

void Ship_makeShip_ote(int desc);

static inline int Item_isUnsaleable_ote(Item * it) { return (int)(it->isUnsaleable()); }
static inline int Item_getIndex_ote(Item * it) { return (int)(it->getIndex()); }

int Item_getAmount_ote();

void Item_makeItem_ote(int desc);

int Mission_getType_ote();

static inline int Mission_isCampaignMission_ote(Mission * m) { return (int)(m->isCampaignMission()); }
static inline int Mission_getProductionGoodIndex_ote(Mission * m) { return (int)(m->getProductionGoodIndex()); }

int Mission_getProductionGoodAmount_ote();

int Mission_getReward_ote();

int Mission_getBonus_ote();

static inline int Agent_getOffer_ote(Agent * a) { return (int)(a->getOffer()); }
static inline int Agent_getSellItemIndex_ote(Agent * a) { return (int)(a->getSellItemIndex()); }

static inline void Agent_setEvent_ote(Agent *a, int e) { a->setEvent(e); }

static inline void Agent_setOfferAccepted_ote(Agent *a, int flag) { a->setOfferAccepted(flag); }

static inline void Achievements_resetNewMedals_ote(Achievements * ach) { ach->resetNewMedals(); }

static inline int GameText_getText_ote(int id) { return (int)(Globals::gameText->getText(id)); }

void Galaxy_getStation_ote(int index);

void Layout_OnTouchEnd_ote(Layout *l, int p1, int p2);

int Layout_OnTouchEndR_ote(Layout *l, int p1, int p2);

static inline int Layout_helpPressed_ote(Layout * l) { return (int)(l->helpPressed()); }

void Layout_initHelpWindow_ote(int l, int textStr);

void Layout_showMissionRewardMessage_ote(int l, int flag);

int ChoiceWindow_OnTouchEnd_ote(int cw, int p1);

void ChoiceWindow_set_ote(int cw, int textStr, int flag);

void ChoiceWindow_setNotice_ote(int cw, int textStr);

int HangarWindow_OnTouchEnd_ote(HangarWindow *w, int p1, int p2);

static inline void HangarWindow_ctor_ote(HangarWindow *w) { new ((void*)w) HangarWindow(); }

static inline void HangarWindow_dtor_ote(HangarWindow *w) { w->~HangarWindow(); }

void HangarWindow_initialize_ote();

void HangarWindow_showCreditsBuyWindow_ote(HangarWindow *w);

void HangarWindow_setSellMode_ote(int w);

int StarMap_OnTouchEnd_ote(int sm, int p1);

int SpaceLounge_OnTouchEnd_ote(int l, int p1, int p2);

int SpaceLounge_hangarNeedsUpdate_ote();

void SpaceLounge_refresh_ote();

int StatusWindow_OnTouchEnd_ote(int w, int p1, int p2);

int MissionsWindow_OnTouchEnd_ote(int w, int p1);

int MenuTouchWindow_OnTouchEnd_ote(MenuTouchWindow *w, int p1, int p2, void *p3); // lint: void_ptr

static inline void MenuTouchWindow_ctor_ote(MenuTouchWindow *w, int kind) { new ((void*)w) MenuTouchWindow(kind); }

int TouchButton_OnTouchEnd_ote(int btn, int p1);

void FModSound_play_ote(int sound, int id, const float *p, float vol);

static inline void FModSound_stop_ote(int sound) { Globals::sound->stop(sound); }

void FModSound_setParamValue_ote(int sound, int a, int b, float v);

static inline void RecordHandler_saveOptions_ote(RecordHandler * rh) { rh->saveOptions(); }

void CutScene_replacePlayerShip_ote(int cs, int shipIndex);

void CutScene_checkForTurret_ote(int cs);

void ChoiceWindow_setFee_frag(ChoiceWindow *cw, int credits, int templateId);

int *ModStation_ote_buttonRow(ModStation * self);
int TouchButton_getX_ote(TouchButton * btn);
int TouchButton_getY_ote(TouchButton * btn);

static inline void Radio_ctor_ote(Radio * radio) { new ((void*)radio) Radio(); }

void Radio_addMessage_ote(Radio *radio, int textId);

void ScrollTouchBox_initRadio_ote(ScrollTouchBox * box, Radio * radio);

void ModStation::OnTouchEnd(int x, int y, void *touch) {
    // lint: void_ptr
    int *status = *(int **) g_ote_status;

    if (this->activeTouch != touch)
        return;
    int starMapOpen = reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0];
    reinterpret_cast<uint8_t*>(&this->scrollFlags)[0] = 0;
    this->activeTouch = 0;
    if (starMapOpen != 0)
        return;

    int *help = *(int **) g_ote_helpLayout;
    if (((Layout *) *help)->choiceWindowOpen != 0) {
        if (Layout_OnTouchEndR_ote((Layout *) *help, x, y) != 0)
            ((Layout *) *help)->choiceWindowOpen = 0;
        return;
    }

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] != 0) {
        if (this->dialogueWindow->OnTouchEnd(x, x) != 0) {
            if (this->activeMission != 0) {
                {
                    Mission *mission = (Mission *) this->activeMission;

                    if (mission != 0) {
                        int type = Mission_getType_ote();
                        int campaign = Mission_isCampaignMission_ote(mission);
                        if (type == 8) {
                            if (campaign == 0) {
                                int ship = Status_getShip_ote();
                                int good = Mission_getProductionGoodIndex_ote(mission);
                                Mission_getProductionGoodAmount_ote();
                                Ship_removeCargo_ote(ship, good);
                                if ((int) (intptr_t) this->hangarWindow != 0)
                                    HangarWindow_initialize_ote();
                            }
                        } else if (campaign == 0 && Mission_getType_ote() == 0xb) {
                            Status_setPassengers_ote(*status, 0);
                            if (Mission_isCampaignMission_ote(mission) == 0)
                                ((Status *) (intptr_t) * status)->field_b8 += Mission_getProductionGoodAmount_ote();
                            Status_getShip_ote();
                            Array<Item *> *cargo = (Array<Item *> *) (intptr_t) Ship_getCargo_ote();
                            if (cargo != 0) {
                                for (unsigned i = 0; i < cargo->size(); i = i + 1) {
                                    if (Item_isUnsaleable_ote((*cargo)[i]) != 0 &&
                                        (Item_getIndex_ote((*cargo)[i]) == 0x74 ||
                                         Item_getIndex_ote((*cargo)[i]) == 0x75)) {
                                        Ship_removeCargo1_ote((Item *) Status_getShip_ote());
                                        if ((int) (intptr_t) this->hangarWindow != 0)
                                            HangarWindow_initialize_ote();
                                        break;
                                    }
                                }
                            }
                        } else if (Mission_getType_ote() != 3 && Mission_getType_ote() != 5 &&
                                   Mission_getType_ote() != 0xb) {
                            if (Mission_getType_ote() == 0) {
                                Status_getShip_ote();
                                Array<Item *> *cargo = (Array<Item *> *) (intptr_t) Ship_getCargo_ote();
                                if (cargo != 0) {
                                    for (unsigned i = 0; i < cargo->size(); i = i + 1) {
                                        if (Item_isUnsaleable_ote((*cargo)[i]) != 0 &&
                                            (Item_getIndex_ote((*cargo)[i]) == 0x74 ||
                                             Item_getIndex_ote((*cargo)[i]) == 0x75)) {
                                            Ship_removeCargo1_ote((Item *) Status_getShip_ote());
                                            if ((int) (intptr_t) this->hangarWindow != 0)
                                                HangarWindow_initialize_ote();
                                            break;
                                        }
                                    }
                                }
                                ((Status *) (intptr_t) * status)->field_9c += Mission_getProductionGoodAmount_ote();
                            } else if (Mission_getType_ote() == 0xe) {
                                Status_getShip_ote();
                                Array<Item *> *cargo = (Array<Item *> *) (intptr_t) Ship_getCargo_ote();
                                if (cargo != 0) {
                                    for (unsigned i = 0; i < cargo->size(); i = i + 1) {
                                        if (Item_getIndex_ote((*cargo)[i]) == 0x73) {
                                            Ship_removeCargo1_ote((Item *) Status_getShip_ote());
                                            if ((int) (intptr_t) this->hangarWindow != 0)
                                                HangarWindow_initialize_ote();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (Mission_isCampaignMission_ote(mission) == 0) {
                        Status_incMissionCount_ote(*status);
                        int reward = Mission_getReward_ote();
                        int bonus = Mission_getBonus_ote();
                        Layout_showMissionRewardMessage_ote(*(int *) *(int **) g_ote_helpLayout,
                                                            (char) (bonus + reward));
                        {
                            Mission_getReward_ote();
                            int bonus = Mission_getBonus_ote();
                            if ((unsigned) bonus >= 0)
                                Status_getCurrentCampaignMission_ote();
                            Status_changeCredits_ote(*status);
                            Status_removeMission_ote(*status);
                            this->activeMission = 0;
                            if ((int) (intptr_t) this->spaceLounge != 0)
                                SpaceLounge_refresh_ote();
                            reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                            this->checkHints();
                        }
                        return;
                    }

                    int camp = Status_getCurrentCampaignMission_ote();
                    if (camp == 0x2b) {
                        Status_removeMission_ote(*status);
                        Status_setMission_ote(*status);
                        reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                        int snd = **(int **) g_ote_sound;
                        FModSound_stop_ote(snd);
                        FModSound_play_ote(snd, 0x90, 0, 0.0f);
                        {
                            Radio *radio = (Radio *) ::operator new(0x40);
                            Radio_ctor_ote(radio);
                            for (int id = 0x817; id <= 0x81a; ++id)
                                Radio_addMessage_ote(radio, id);
                            this->activeMission = (int) (intptr_t) radio;

                            ScrollTouchBox *box = (ScrollTouchBox *) ::operator new(0x80);
                            ScrollTouchBox_initRadio_ote(box, radio);
                            this->idleBox = box;
                        }
                        reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] = 1;
                        return;
                    }

                    if (Status_getCurrentCampaignMission_ote() != 0x94)
                        Status_nextCampaignMission_ote(*status);

                    int cm = Status_getCurrentCampaignMission_ote();
                    unsigned d = (unsigned) (cm - 0x2c);
                    bool handled = false;
                    if (d < 0xf) {
                        if ((1 << (d & 0xff) & 0x5830) != 0) {
                            int cs = (int) (intptr_t) this->cutScene;
                            Status_getShip_ote();
                            int shipIndex = Ship_getIndex_ote();
                            Ship *sh = (Ship *) Status_getShip_ote();
                            Ship_getRace_ote(sh);
                            CutScene_replacePlayerShip_ote(cs, shipIndex);
                            {
                                Mission_getReward_ote();
                                int bonus = Mission_getBonus_ote();
                                if ((unsigned) bonus >= 0)
                                    Status_getCurrentCampaignMission_ote();
                                Status_changeCredits_ote(*status);
                                Status_removeMission_ote(*status);
                                this->activeMission = 0;
                                if ((int) (intptr_t) this->spaceLounge != 0)
                                    SpaceLounge_refresh_ote();
                                reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                this->checkHints();
                            }
                            return;
                        }
                        if (d == 1) {
                            Status_removeMission_ote(*status);
                            this->activeMission = 0;
                            Status_setMission_ote(*status);
                            this->selectedButton = 1;
                            reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;

                            {
                                int module = **(int **) g_ote_module;
                                if (module != 0)
                                    ((void (*)(int, int)) module)(module, 0x10000);
                            }
                            return;
                        }
                        if (d != 0)
                            handled = false;
                        else
                            handled = true;
                    }

                    if (!handled) {
                        if ((unsigned) (cm - 0x4b) > 8 || (1 << ((cm - 0x4b) & 0xff) & 0x103) == 0) {
                            if (cm == 0x12) {
                                Status_removeMission_ote(*status);
                                reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                this->activeMission = 0;
                                return;
                            }

                            Station *here = (Station *) Status_getStation_ote();
                            int hereIdx = Station_getIndex_ote(here);

                            if (cm == 0x4d && hereIdx == 100) {
                                Ship *sh = (Ship *) Status_getStation_ote();
                                Ship_makeShip_ote((*(Array<int> *) (intptr_t) * *g_ote_shipTable)[(0x94) / 4]);
                                Station_addShip_ote(sh);
                                {
                                    Mission_getReward_ote();
                                    int bonus = Mission_getBonus_ote();
                                    if ((unsigned) bonus >= 0)
                                        Status_getCurrentCampaignMission_ote();
                                    Status_changeCredits_ote(*status);
                                    Status_removeMission_ote(*status);
                                    this->activeMission = 0;
                                    if ((int) (intptr_t) this->spaceLounge != 0)
                                        SpaceLounge_refresh_ote();
                                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                    this->checkHints();
                                }
                                return;
                            }
                            if (cm == 0x4e) {
                                ((Status *) (intptr_t) * status)->field_5c = -1;
                                ((Status *) (intptr_t) * status)->field_60 = -1;
                                ((Status *) (intptr_t) * status)->field_64 = -1;
                                ((Status *) (intptr_t) * status)->field_68 = -1;
                                Achievements_resetNewMedals_ote((Achievements *) **(int **) g_ote_achievements);
                                Station *home = (Station *) *status;
                                Status_getStation_ote();
                                Status_departStation_ote(home);
                                **(int **) g_ote_module = 1;
                                ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                                this->stationActive = 0;
                                {
                                    Mission_getReward_ote();
                                    int bonus = Mission_getBonus_ote();
                                    if ((unsigned) bonus >= 0)
                                        Status_getCurrentCampaignMission_ote();
                                    Status_changeCredits_ote(*status);
                                    Status_removeMission_ote(*status);
                                    this->activeMission = 0;
                                    if ((int) (intptr_t) this->spaceLounge != 0)
                                        SpaceLounge_refresh_ote();
                                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                    this->checkHints();
                                }
                                return;
                            }
                            Station *st = (Station *) Status_getStation_ote();
                            if (cm == 0x54 && Station_getIndex_ote(st) == 100) {
                                Ship *sh = (Ship *) Status_getStation_ote();
                                if (Station_hasShip_ote((Station *) sh) == 0) {
                                    Ship_makeShip_ote((*(Array<int> *) (intptr_t) * *g_ote_shipTable)[(0x98) / 4]);
                                    Station_addShip_ote(sh);
                                }
                                if (Station_hasShip_ote((Station *) sh) == 0) {
                                    Ship_makeShip_ote((*(Array<int> *) (intptr_t) * *g_ote_shipTable)[(0xa0) / 4]);
                                    Station_addShip_ote(sh);
                                }
                                Item_makeItem_ote((*(Array<int> *) (intptr_t) * *g_ote_itemTable)[(0x224) / 4]);
                                Ship_addCargo_ote((Item *) Status_getShip_ote());
                                {
                                    Mission_getReward_ote();
                                    int bonus = Mission_getBonus_ote();
                                    if ((unsigned) bonus >= 0)
                                        Status_getCurrentCampaignMission_ote();
                                    Status_changeCredits_ote(*status);
                                    Status_removeMission_ote(*status);
                                    this->activeMission = 0;
                                    if ((int) (intptr_t) this->spaceLounge != 0)
                                        SpaceLounge_refresh_ote();
                                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                    this->checkHints();
                                }
                                return;
                            }

                            if (cm == 0xa0 || cm == 0x90 || cm == 99 || cm == 0x77 || cm == 0x85 || cm == 0x6d ||
                                cm == 0x59) {
                                Station *home = (Station *) *status;
                                **(char **) g_ote_module = 1;
                                Galaxy_getStation_ote(**(int **) g_ote_galaxy);
                                Status_departStation_ote(home);
                                ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                                this->stationActive = 0;
                                return;
                            }
                            if (cm == 0x68) {
                                if (this->hangarWindow != 0) {
                                    HangarWindow_dtor_ote((HangarWindow *) this->hangarWindow);
                                    ::operator delete(this->hangarWindow);
                                }
                                reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] = 0;
                                this->hangarWindow = 0;
                                {
                                    Mission_getReward_ote();
                                    int bonus = Mission_getBonus_ote();
                                    if ((unsigned) bonus >= 0)
                                        Status_getCurrentCampaignMission_ote();
                                    Status_changeCredits_ote(*status);
                                    Status_removeMission_ote(*status);
                                    this->activeMission = 0;
                                    if ((int) (intptr_t) this->spaceLounge != 0)
                                        SpaceLounge_refresh_ote();
                                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                    this->checkHints();
                                }
                                return;
                            }
                            if (cm == 0x80) {
                                Status_activateNewWanted_ote();
                                if (((Status *) (intptr_t) * (int *) g_ote_status)->byte_0x2a == 0)
                                    reinterpret_cast<uint8_t*>(&this->screenFlags)[2] = 1;
                                {
                                    Mission_getReward_ote();
                                    int bonus = Mission_getBonus_ote();
                                    if ((unsigned) bonus >= 0)
                                        Status_getCurrentCampaignMission_ote();
                                    Status_changeCredits_ote(*status);
                                    Status_removeMission_ote(*status);
                                    this->activeMission = 0;
                                    if ((int) (intptr_t) this->spaceLounge != 0)
                                        SpaceLounge_refresh_ote();
                                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                    this->checkHints();
                                }
                                return;
                            }

                            {
                                Mission_getReward_ote();
                                int bonus = Mission_getBonus_ote();
                                if ((unsigned) bonus >= 0)
                                    Status_getCurrentCampaignMission_ote();
                                Status_changeCredits_ote(*status);
                                Status_removeMission_ote(*status);
                                this->activeMission = 0;
                                if ((int) (intptr_t) this->spaceLounge != 0)
                                    SpaceLounge_refresh_ote();
                                reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                                this->checkHints();
                            }
                            return;
                        }
                    }

                    Status_removeMission_ote(*status);
                    this->activeMission = 0;
                    Status_setMission_ote(*status);
                    reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
                    ((Status *) (intptr_t) * status)->wanted = 0; /* cutscene slot (+0x0) */
                    {
                        int module = **(int **) g_ote_module;
                        if (module != 0)
                            ((void (*)(int, int)) module)(module, 5);
                    }
                }
                return;
            }

            DialogueWindow *dw = this->dialogueWindow;
            reinterpret_cast<uint8_t*>(&this->modalFlags)[1] = 0;
            if (dw != 0) {
                delete dw;
            }
            char justEntered = reinterpret_cast<uint8_t*>(&this->choiceWindow)[1];
            this->dialogueWindow = 0;
            if (justEntered == 0) {
                Station *st = (Station *) Status_getStation_ote();
                if (Station_getIndex_ote(st) != 4) {
                    st = (Station *) Status_getStation_ote();
                    if (Station_getIndex_ote(st) != 0x58) {
                        Status_getStation_ote();
                        if (Station_stationHasPirateBase_ote() != 0) {
                            reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
                            Station *home = (Station *) **(int **) g_ote_status;
                            Status_getStation_ote();
                            Status_departStation_ote(home);
                            Achievements_resetNewMedals_ote((Achievements *) **(int **) g_ote_achievements);
                            int mod = (Status_getCurrentCampaignMission_ote() == 0x10) ? -1 : 1;
                            **(int **) g_ote_module = mod;
                            ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                            this->stationActive = 0;
                        }
                    }
                }
            }
            return;
        }
        if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0)
            return;
    }

    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[3] != 0) {
        int r = ChoiceWindow_OnTouchEnd_ote((int) (intptr_t) this->choiceWindow, x);
        if (r == 1) {
            char departed = reinterpret_cast<uint8_t*>(&this->modalFlags)[3];
            reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
            if (departed == 0) {
                reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                reinterpret_cast<uint8_t*>(&this->scrollBox)[0] = 0;
            } else {
                reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
                Station *home = (Station *) **(int **) g_ote_status;
                Status_getStation_ote();
                Status_departStation_ote(home);
                Achievements_resetNewMedals_ote((Achievements *) **(int **) g_ote_achievements);
                **(int **) g_ote_module = 1;
                ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                this->stationActive = 0;
            }
        } else if (r == 0) {
            {
                (void) x;
                (void) y;

                if (reinterpret_cast<uint8_t*>(&this->departPending)[2] != 0) {
                    reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
                    int camp = Status_getCurrentCampaignMission_ote();
                    if (camp == 0x18) {
                        Station *st = (Station *) Status_getStation_ote();
                        if (Station_getIndex_ote(st) == 10) {
                            unsigned i = 0;
                            for (;;) {
                                Station *st1 = (Station *) Status_getStation_ote();
                                Array<Agent *> *ag = (Array<Agent *> *) (intptr_t) Station_getAgents_ote(st1);
                                if (ag->size() <= i) break;
                                Station *st2 = (Station *) Status_getStation_ote();
                                Array<Agent *> *agents = (Array<Agent *> *) (intptr_t) Station_getAgents_ote(st2);
                                Agent *a = (*agents)[i];
                                if (Agent_getOffer_ote(a) == 2 && Agent_getSellItemIndex_ote(a) == 0x44) {
                                    Agent_setEvent_ote(a, 1);
                                    Agent_setOfferAccepted_ote(a, 1);
                                }
                                i = i + 1;
                            }
                        }
                    }
                    Station *home = (Station *) *status;
                    if (Status_getCurrentCampaignMission_ote() == 0x30) {
                        Galaxy_getStation_ote(**(int **) g_ote_galaxy);
                        Status_departStation_ote(home);
                        **(char **) g_ote_galaxy = 1;
                    } else {
                        Status_getStation_ote();
                        Status_departStation_ote(home);
                    }
                    ((Status *) (intptr_t) * status)->field_5c = -1;
                    ((Status *) (intptr_t) * status)->field_60 = -1;
                    ((Status *) (intptr_t) * status)->field_64 = -1;
                    ((Status *) (intptr_t) * status)->field_68 = -1;
                    Achievements_resetNewMedals_ote((Achievements *) **(int **) g_ote_achievements);
                    **(int **) g_ote_module = 1;
                    ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                    this->stationActive = 0;
                    return;
                }

                if (reinterpret_cast<uint8_t*>(&this->modalFlags)[3] != 0) {
                    int credits = Status_getCredits_ote();
                    if ((int) (intptr_t) this->buttonCredits <= credits) {
                        Status_changeCredits_ote(*status);
                        reinterpret_cast<uint8_t*>(&this->modalFlags)[3] = 0;
                        reinterpret_cast<uint8_t*>(&this->buttonCredits)[0] = 1;
                        Station *st = (Station *) Status_getStation_ote();
                        Station_setAttackedFriends_ote(st, 0);
                        reinterpret_cast<uint8_t*>(&this->choiceWindow)[1] = 1;
                        this->enterStation();
                        this->autosave();
                        {
                            if (reinterpret_cast<uint8_t*>(&this->scrollBox)[0] != 0) {
                                reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                                reinterpret_cast<uint8_t*>(&this->scrollBox)[0] = 0;
                                AppData *appData = (AppData *) (intptr_t) ApplicationManager_GetApplicationData_ote();
                                appData->hideRadio = 1;
                                return;
                            }
                            if (reinterpret_cast<uint8_t*>(&this->screenFlags)[0] == 0) {
                                if (reinterpret_cast<uint8_t*>(&this->screenFlags)[1] != 0) {
                                    int feeCredits = Status_getCredits_ote();
                                    if (feeCredits < 25000) {
                                        ChoiceWindow_setFee_frag(this->choiceWindow, feeCredits, 1);
                                        reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 0;
                                        return;
                                    }
                                    Status_changeCredits_ote(*status);
                                    reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                                    reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 0;
                                    Station *home = (Station *) *status;
                                    Galaxy_getStation_ote(**(int **) g_ote_galaxy);
                                    Status_setStation_ote(home);
                                    **(int **) g_ote_module = 0;
                                    ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                                }
                                if (this->pendingHangarClose != 0 && reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
                                    this->pendingHangarClose = 0;
                                    reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] = 0;
                                }
                                reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                            } else {
                                Status_changeCredits_ote(*status);
                                Ship_removeCargo_ote(Status_getShip_ote(), 0x6d);
                                ((Status *) (intptr_t) * status)->field_114 = 3;
                                RecordHandler_saveOptions_ote((RecordHandler *) **(int **) g_ote_status);
                                ChoiceWindow_setNotice_ote((int) (intptr_t) this->choiceWindow,
                                                           GameText_getText_ote(**g_ote_textRoot));
                                Station *st2 = (Station *) Status_getStation_ote();
                                Station_setItems_ote(st2, 0, 0);
                                Station_setItems_ote(((Status *) (intptr_t) * status)->voidStation, 0, 0);
                                reinterpret_cast<uint8_t*>(&this->screenFlags)[0] = 0;
                            }
                        }
                        return;
                    }
                    ChoiceWindow_setFee_frag(this->choiceWindow, credits, 0);
                    reinterpret_cast<uint8_t*>(&this->modalFlags)[3] = 1;
                    reinterpret_cast<uint8_t*>(&this->departPending)[2] = 1;
                    reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 1;
                    return;
                }

                {
                    if (reinterpret_cast<uint8_t*>(&this->scrollBox)[0] != 0) {
                        reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                        reinterpret_cast<uint8_t*>(&this->scrollBox)[0] = 0;
                        AppData *appData = (AppData *) (intptr_t) ApplicationManager_GetApplicationData_ote();
                        appData->hideRadio = 1;
                        return;
                    }
                    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[0] == 0) {
                        if (reinterpret_cast<uint8_t*>(&this->screenFlags)[1] != 0) {
                            int credits = Status_getCredits_ote();
                            if (credits < 25000) {
                                ChoiceWindow_setFee_frag(this->choiceWindow, credits, 1);
                                reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 0;
                                return;
                            }
                            Status_changeCredits_ote(*status);
                            reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                            reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 0;
                            Station *home = (Station *) *status;
                            Galaxy_getStation_ote(**(int **) g_ote_galaxy);
                            Status_setStation_ote(home);
                            **(int **) g_ote_module = 0;
                            ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                        }
                        if (this->pendingHangarClose != 0 && reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
                            this->pendingHangarClose = 0;
                            reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] = 0;
                        }
                        reinterpret_cast<uint8_t*>(&this->screenFlags)[3] = 0;
                    } else {
                        Status_changeCredits_ote(*status);
                        Ship_removeCargo_ote(Status_getShip_ote(), 0x6d);
                        ((Status *) (intptr_t) * status)->field_114 = 3;
                        RecordHandler_saveOptions_ote((RecordHandler *) **(int **) g_ote_status);
                        ChoiceWindow_setNotice_ote((int) (intptr_t) this->choiceWindow,
                                                   GameText_getText_ote(**g_ote_textRoot));
                        Station *st = (Station *) Status_getStation_ote();
                        Station_setItems_ote(st, 0, 0);
                        Station_setItems_ote(((Status *) (intptr_t) * status)->voidStation, 0, 0);
                        reinterpret_cast<uint8_t*>(&this->screenFlags)[0] = 0;
                    }
                }
            }
        }
        return;
    }

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0) {
        if (StatusWindow_OnTouchEnd_ote((int) (intptr_t) this->statusWindow, x, y) != 0) {
            reinterpret_cast<uint8_t*>(&this->modalFlags)[0] = 0;
            this->resetLight();
        }
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0) {
        if (StarMap_OnTouchEnd_ote((int) (intptr_t) this->starMap, x) != 0) {
            reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] = 0;
            this->resetLight();
        }
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
        if (HangarWindow_OnTouchEnd_ote((HangarWindow *) this->hangarWindow, x, y) != 0) {
            int *st = *(int **) g_ote_status;

            Status_getShip_ote();
            Array<Item *> *cargo = (Array<Item *> *) (intptr_t) Ship_getCargo_ote();
            int sold = 0;
            if (cargo != 0) {
                for (unsigned i = 0; i < cargo->size(); i = i + 1) {
                    int idx = Item_getIndex_ote((*cargo)[i]);
                    if (0x83 < idx && Item_getIndex_ote((*cargo)[i]) < 0x9a)
                        sold += Item_getAmount_ote();
                }
            }
            if ((int) (intptr_t) this->buttonLaunch < sold)
                ((Status *) (intptr_t) * st)->field_a8 += sold - (int) (intptr_t) this->buttonLaunch;
            HangarWindow_setSellMode_ote((int) (intptr_t) this->hangarWindow);
            this->resetIdleCamForHangar();
            HangarWindowHead *hw = (HangarWindowHead *) this->hangarWindow;
            reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] = 0;
            if (hw->shipChanged != 0) {
                hw->shipChanged = 0;
                int cs = (int) (intptr_t) this->cutScene;
                Status_getShip_ote();
                int shipIndex = Ship_getIndex_ote();
                Ship *sh = (Ship *) Status_getShip_ote();
                Ship_getRace_ote(sh);
                CutScene_replacePlayerShip_ote(cs, shipIndex);
            }
            if ((int) (intptr_t) this->cutScene != 0)
                CutScene_checkForTurret_ote((int) (intptr_t) this->cutScene);
            int snd = **(int **) g_ote_sound;
            FModSound_stop_ote(snd);
            FModSound_play_ote(snd, 0x7a, 0, 0.0f);
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
            {
                int *row = (int *) ModStation_ote_buttonRow(this);
                if (row != nullptr) {
                    int *btns = (int *) row[1];
                    int n = row[0];
                    int *xs = this->buttonCacheX;
                    int *ys = this->buttonCacheY;
                    for (int i = 0; i < n && i < 5; ++i) {
                        TouchButton *b = (TouchButton *) (intptr_t) btns[i];
                        xs[i] = TouchButton_getX_ote(b);
                        ys[i] = TouchButton_getY_ote(b);
                    }
                }
            }
        }
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0) {
        if (SpaceLounge_OnTouchEnd_ote((int) (intptr_t) this->spaceLounge, x, y) != 0) {
            reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] = 0;
            this->resetIdleCamForHangar();
            this->resetLight();
            int snd = **(int **) g_ote_sound;
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
            FModSound_stop_ote(snd);
            FModSound_play_ote(snd, 0x7a, 0, 0.0f);
            CutScene_checkForTurret_ote((int) (intptr_t) this->cutScene);
            if ((int) (intptr_t) this->spaceLounge != 0 && SpaceLounge_hangarNeedsUpdate_ote() != 0) {
                if (this->hangarWindow != 0) {
                    HangarWindow_dtor_ote((HangarWindow *) this->hangarWindow);
                    ::operator delete(this->hangarWindow);
                }
                this->hangarWindow = 0;
            }
        }
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0) {
        if (MissionsWindow_OnTouchEnd_ote((int) (intptr_t) this->m_pDialogueWindow, x) != 0) {
            reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] = 0;
            int snd = **(int **) g_ote_sound;
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
        }
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[2] != 0) {
        if (MenuTouchWindow_OnTouchEnd_ote((MenuTouchWindow *) this->dlcMenu,
                                           x, y, touch) != 0) {
            reinterpret_cast<uint8_t*>(&this->screenFlags)[2] = 0;
            {
                int *row = (int *) ModStation_ote_buttonRow(this);
                if (row != nullptr) {
                    int *btns = (int *) row[1];
                    int n = row[0];
                    int *xs = this->buttonCacheX;
                    int *ys = this->buttonCacheY;
                    for (int i = 0; i < n && i < 5; ++i) {
                        TouchButton *b = (TouchButton *) (intptr_t) btns[i];
                        xs[i] = TouchButton_getX_ote(b);
                        ys[i] = TouchButton_getY_ote(b);
                    }
                }
            }
        }
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->screenFlags)[1] != 0) {
        {
            int *help = *(int **) g_ote_helpLayout;

            if (TouchButton_OnTouchEnd_ote((int) (intptr_t) this->dlcMenu, x) != 0)
                return;

            if (TouchButton_OnTouchEnd_ote(this->activeMission, x) != 0) {
                RecordHandler *rh = (RecordHandler *) **(int **) g_ote_status;
                reinterpret_cast<uint8_t *>(&((Status *) (intptr_t) * (int *) g_ote_status)->field_4c)[2] = 1;
                RecordHandler_saveOptions_ote(rh);
                if (this->hangarWindow == 0) {
                    HangarWindow *hw = (HangarWindow *) ::operator new(0x134);
                    HangarWindow_ctor_ote(hw);
                    this->hangarWindow = hw;
                }
                HangarWindow_initialize_ote();
                this->pendingHangarClose = 1;
                reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] = 1;
                HangarWindow_showCreditsBuyWindow_ote((HangarWindow *) this->hangarWindow);
            }

            this->selectedButton = -1;
            for (unsigned i = 0; i < 5; i = i + 1) {
                if (TouchButton_OnTouchEnd_ote(this->buttonRow[i], x) != 0) {
                    this->selectedButton = (int) i;

                    {
                        int module = **(int **) g_ote_module;
                        if (module != 0)
                            ((void (*)(int, int)) module)(module, 0x10000);
                    }
                    return;
                }
            }

            if (Layout_OnTouchEndR_ote((Layout *) *help, x, y) != 0) {
                if (this->dlcMenu == 0) {
                    MenuTouchWindow *w = (MenuTouchWindow *) ::operator new(0x240);
                    MenuTouchWindow_ctor_ote(w, 2);
                    this->dlcMenu = w;
                }
                Status_checkForLevelUp_ote();
                reinterpret_cast<uint8_t*>(&this->screenFlags)[2] = 1;
                {
                    int *row = (int *) ModStation_ote_buttonRow(this);
                    if (row != nullptr) {
                        int *btns = (int *) row[1];
                        int n = row[0];
                        int *xs = this->buttonCacheX;
                        int *ys = this->buttonCacheY;
                        for (int i = 0; i < n && i < 5; ++i) {
                            TouchButton *b = (TouchButton *) (intptr_t) btns[i];
                            xs[i] = TouchButton_getX_ote(b);
                            ys[i] = TouchButton_getY_ote(b);
                        }
                    }
                }
                return;
            }

            if (Layout_helpPressed_ote((Layout *) *help) != 0)
                Layout_initHelpWindow_ote(*help, GameText_getText_ote(**g_ote_textRoot));

            if (this->newsTicker->OnTouchEnd(x, 0) == 0) {
                float dx = VectorSignedToFloat(this->idleDeltaX, 0);
                float dy = VectorSignedToFloat(this->idleDeltaY, 0);
                this->camCoordX += dx * 0.01f;
                this->camCoordY += dy * 0.01f;
            }
        }
    }
}

static Layout **g_ModStation_tb_layout = 0;

static int **g_ModStation_tb_screenH = 0;

static int **g_ModStation_tb_clear = 0;

void ModStation::OnTouchBegin(int x, int y, void *touch) {
    // lint: void_ptr
    if (this->activeTouch != 0)
        return;
    reinterpret_cast<uint8_t*>(&this->scrollFlags)[0] = 1;
    this->activeTouch = touch;
    int flag = this->m_nStarMapWindowOpen;
    this->touchX = x;
    this->touchY = y;
    if (flag != 0) {
        if (((Radio *) (intptr_t) this->activeMission)->lastMessageShown() != 0) {
            (unsigned char &) this->stationActive = 0;
            Globals::status->nextCampaignMission(true);
            unsigned int mod = *(unsigned int *) Globals::appManager;
            **g_ModStation_tb_clear = 0;

            Globals::appManager->SetCurrentApplicationModule(mod);
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[0] = 0;
        }
        return;
    }
    Layout **layoutHolder = g_ModStation_tb_layout;
    Layout *layoutObj = *layoutHolder;
    if (layoutObj->choiceWindowOpen != 0) {
        layoutObj->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] != 0) {
        ((DialogueWindow *) (this->dialogueWindow))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] != 0) {
        ((ChoiceWindow *) (this->choiceWindow))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0) {
        ((ChoiceWindow *) (this->medalChoiceWindow))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
        ((HangarWindow *) (this->hangarWindow))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0) {
        ((SpaceLounge *) (this->spaceLounge))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0) {
        this->starMap->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0) {
        ((StatusWindow *) (this->statusWindow))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0) {
        ((MissionsWindow *) (this->m_pDialogueWindow))->OnTouchBegin(x, y);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[2] != 0) {
        ((MenuTouchWindow *) (this->dlcMenu))->OnTouchBegin(x, y, touch);
        return;
    }
    if (reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[1] == 0)
        return;
    ((TouchButton *) (this->buttonLaunch))->OnTouchBegin(x, y);
    ((TouchButton *) (this->buttonCredits))->OnTouchBegin(x, y);
    for (int i = 0; i != 5; i++)
        (((TouchButton **) (intptr_t) this->buttonRow[1])[i])->OnTouchBegin(x, y);
    layoutObj->OnTouchBegin(x, y);
    if (((NewsTicker *) (this->newsTicker))->OnTouchBegin(x, y) != 0)
        return;
    Layout *layout = layoutObj;
    if (y <= layout->field_0xc_leftMargin)
        return;
    if (*(int *) *g_ModStation_tb_screenH - layout->field_0x10_rightMargin <= y)
        return;
    if (x <= (int) (intptr_t) this->hangarGeom)
        return;
    this->field_0xfc = x;
    this->camScrollPos = x;
    this->camScrollVel = 0;
    reinterpret_cast<uint8_t*>(&this->dragFlags)[0] = 1;
}

void ModStation::OnTouchBegin(int x, int y) {}
void ModStation::OnTouchEnd(int x, int y) {}
void ModStation::OnTouchMove(int x, int y) {}

static int *g_r2d_stack = 0;

static int **g_r2d_helpLayout = 0;

void HangarWindow_render_r2d(ModStation * self);

void HangarWindow_render3D_r2d();

void SpaceLounge_draw_r2d(ModStation * self);

void SpaceLounge_draw3DShip_r2d();

void MissionsWindow_draw_r2d(MissionsWindow * w);

void StarMap_draw_r2d(ModStation * self);
void StatusWindow_draw_r2d(ModStation * self);
void MenuTouchWindow_draw_r2d(ModStation * self);
void Radio_draw_r2d(ModStation * self);

void CutScene_render2D_r2d();

void ChoiceWindow_draw_r2d(ModStation * self);
void Layout_drawHelpWindow_r2d(Layout * l);

Layout *ModStation_r2d_layout();

void Layout_drawCredits_r2d(Layout * layout);

void ModStation::OnRender2D() {
    Globals::Canvas->Begin2d();
    Globals::Canvas->SetColor(this->fadeColor);

    if (this->stationActive == 0) {
        return;
    }

    if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
        HangarWindow_render_r2d(this);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0) {
        SpaceLounge_draw_r2d(this);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[0] != 0) {
        MissionsWindow_draw_r2d((MissionsWindow *) this->m_pDialogueWindow);
    } else if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[3] != 0) {
        StarMap_draw_r2d(this);
    } else if (reinterpret_cast<uint8_t*>(&this->modalFlags)[0] != 0) {
        StatusWindow_draw_r2d(this);
    } else if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0) {
        MenuTouchWindow_draw_r2d(this);
    } else if (reinterpret_cast<uint8_t*>(&this->screenFlags)[0] != 0) {
        Radio_draw_r2d(this);
        CutScene_render2D_r2d();
    } else {
        Layout *layout = (Layout *) this->buttonState;
        if (layout == nullptr) layout = ModStation_r2d_layout();
        if (layout != nullptr)
            Layout_drawCredits_r2d(layout);
    }

    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[2] != 0 || reinterpret_cast<uint8_t*>(&this->modalFlags)[3] != 0)
        ChoiceWindow_draw_r2d(this);
    if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] != 0) {
        if (this->dialogueWindow != 0)
            this->dialogueWindow->draw();
    }

    Layout **help = (Layout **) g_r2d_helpLayout;
    if ((*help)->choiceWindowOpen != 0)
        Layout_drawHelpWindow_r2d((Layout *) *help);

    Globals::Canvas->End2d();

    if ((*help)->choiceWindowOpen == 0) {
        if (reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] != 0) {
            HangarWindow_render3D_r2d();
        }
        if ((*help)->choiceWindowOpen == 0 && reinterpret_cast<uint8_t*>(&this->subWindowFlags)[1] != 0)
            SpaceLounge_draw3DShip_r2d();
    }

    Globals::Canvas->SwapBuffer();
}

static int *g_oi_stack = 0;

static int **g_oi_status = 0;

static int **g_oi_itemTable = 0;

static int **g_oi_shipTable = 0;

static int **g_oi_canvas = 0;

static int **g_oi_settings = 0;

static int **g_oi_sound = 0;

static int **g_oi_textRoot = 0;

static char **g_oi_demoFlag = 0;

static int *g_oi_musicId = 0;

static inline int Status_getCurrentCampaignMission_oi() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline int Status_getStation_oi() { return (int)(Globals::status->getStation()); }

static inline int Station_getIndex_oi(Station * s) { return (int)(s->getIndex()); }

static inline int Status_getSystem_oi() { return (int)(Globals::status->getSystem()); }

int SolarSystem_getRace_oi();

int SolarSystem_getRoutes_oi();

static inline int Status_inAlienOrbit_oi() { return (int)(Globals::status->inAlienOrbit()); }

static inline int Status_inSupernovaSystem_oi() { return (int)(Globals::status->inSupernovaSystem()); }

static inline int Status_inBlackMarketSystem_oi() { return (int)(Globals::status->inBlackMarketSystem()); }

static inline int Status_getShip_oi() { return (int)(Globals::status->getShip()); }

static inline int Status_getCredits_oi() { return (int)(Globals::status->getCredits()); }

static inline int Status_getLevel_oi() { return (int)(Globals::status->getLevel()); }

static inline int Status_hardCoreMode_oi() { return (int)(Globals::status->hardCoreMode()); }

static inline int Status_getStanding_oi() { return (int)(Globals::status->getStanding()); }

static inline int Status_gameWon_oi() { return (int)(Globals::status->gameWon()); }

static inline int Status_dlc1Won_oi() { return (int)(Globals::status->dlc1Won()); }

int Status_getPlayingTime_lo_oi();

static inline int Status_activateNewWanted_oi() { return (int)(Globals::status->activateNewWanted()); }

static inline int Status_getFreelanceMission_oi() { return (int)(Globals::status->getFreelanceMission()); }

static inline int Status_getCampaignMission_oi() { return (int)(Globals::status->getCampaignMission()); }

static inline void Status_setShip_oi(Ship * s) { Globals::status->setShip(s); }

int Ship_getIndex_oi();

int Ship_hasJumpDrive_oi();

int Ship_hasCargo_oi(int ship, int item);

int Ship_hasEquipment_oi(int ship, int item);

int Ship_getCargo_oi(int ship);

void Ship_makeShip_oi(int desc);

void Ship_setRace_oi(int race);

void Ship_setEquipment_oi(Item *eq, int slot);

void Ship_setPrice_oi(int shipPtr);

void Ship_removeCargo_oi(int ship);

void Ship_addCargo_oi(int ship, Item *it);

int Item_makeItem_oi();

int Item_makeItemDesc_oi(int desc);

int Item_makeItemDescAmt_oi(int desc, int amt);

void Item_setUnsaleable_oi(int flag);

static inline int Item_getIndex_oi(Item * it) { return (int)(it->getIndex()); }

int Item_getAmount_oi();

void Item_setPrice_oi(int itemPtr);

int Station_getShips_oi();

int Station_getItems_oi();

int Station_getName_oi();

int Station_hasItem_oi(int station);

int Station_hasShip_oi(Station * station);

int Station_hasAttackedFriends_oi();

int Station_stationHasPirateBase_oi();

void Station_addItem_oi(Item * it);
void Station_addShip_oi(Ship * s);
void Station_removeShip_oi(Ship * s);

void Station_setShips_oi(Station *station, int list);

int Standing_isEnemy_oi(int standing);

int Standing_getStanding_oi(int standing);

int Mission_getTargetStation_oi();

int Mission_getType_oi();

static inline int Achievements_gotAllGoldMedals_oi() { return (int)(Globals::achievements->gotAllGoldMedals()); }

static inline int GameText_getText_oi(int id) { return (int)(Globals::gameText->getText(id)); }

void ChoiceWindow_set_oi(ChoiceWindow *cw, int text, int flag);

void ChoiceWindow_setNotice_oi(ChoiceWindow *cw, int text);

int CutScene_initialize_oi(CutScene * cs);

static inline void CutScene_ctor_oi(CutScene *cs, int kind) { new ((void*)cs) CutScene(kind); }

void CutScene_replacePlayerShip_oi(int cs, int shipIndex);

void TouchButton_setHalfTransparent_oi(int flag);

void Globals_playMusicAndFadeOutCurrent_oi(int id);

void FModSound_play_oi(int sound, int id, const float *pos, float vol);

static inline void FModSound_enableReverb_oi(int sound) { Globals::sound->enableReverb(sound); }

static inline void FModSound_setDownPitch_oi(int sound) { Globals::sound->setDownPitch(sound); }

static inline void Generator_ctor_oi(Generator * g) { new ((void*)g) Generator(); }
static inline void Generator_dtor_oi(Generator *g) { g->~Generator(); }
static inline void Generator_computerTradeGoods_oi(Generator * g, Station * s) { g->computerTradeGoods(s); }
int Generator_getShipBuyList_oi(Station * s);

const int *ModStation_msc_camCoordTable();

const int *ModStation_msc_camRotTable();

unsigned ModStation_oi_cameraHandle();

void ModStation_oi_setCameraLocal(unsigned h, const Matrix &m);

int GameText_getText_oiImpl(int id);

void ChoiceWindow_setFee_oiImpl(ChoiceWindow *cw, int credits, int templateId);

void ChoiceWindow_set6_oiImpl(ChoiceWindow *cw, int a, int b, int c, int d, int e, int f);

void ChoiceWindow_set1_oiImpl(ChoiceWindow *cw, int textStr);

void NewsTicker_build_oiImpl(ModStation * self);
void DlcMenu_build_oiImpl(ModStation * self);

void ModStation::OnInitialize() {
    int *status = *(int **) g_oi_status;

    this->introTimer = 0;

    if ((int) (intptr_t) this->cutScene == 0) {
        CutScene *cs = (CutScene *) ::operator new(0xa0);
        CutScene_ctor_oi(cs, 0x17);
        this->cutScene = cs;
        CutScene_initialize_oi(cs);
    }

    int state = this->state;
    int next;

    if (state == 0x14) {
        if (reinterpret_cast<uint8_t*>(&this->cameraFlags)[1] == 0 && reinterpret_cast<uint8_t*>(&this->cameraFlags)[0] == 0 &&
            reinterpret_cast<uint8_t*>(&this->dragFlags)[1] != 0) {
            bool skip = false;
            if (Status_getCurrentCampaignMission_oi() == 0x4d) {
                Station *st = (Station *) Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x65)
                    skip = true;
            }
            if (!skip)
                this->autosave();
        }

        int *shipTbl = *(int **) g_oi_shipTable;

        if (Status_getCurrentCampaignMission_oi() == 1) {
            Ship *ship = (Ship *) *shipTbl;
            Ship_makeShip_oi((*(Array<int> *) (intptr_t) * *g_oi_shipTable)[0]);
            Status_setShip_oi(ship);
            Ship_setRace_oi(Ship_getIndex_oi());
            int eq = Item_makeItem_oi();
            Item_setUnsaleable_oi(eq);
            Ship_setEquipment_oi((Item *) *shipTbl, eq);
            int eq2 = Item_makeItem_oi();
            Ship_setEquipment_oi((Item *) *shipTbl, eq2);
            Item_setUnsaleable_oi(eq2);
            CutScene_replacePlayerShip_oi((int) (intptr_t) this->cutScene, Ship_getIndex_oi());
        }

        int camp = Status_getCurrentCampaignMission_oi();
        bool special = false;
        if (camp < 5) {
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[0]);
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[(8) / 4]);
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[(0xc) / 4]);
        } else if (camp < 9 || camp == 0xf) {
            special = camp == 0xf;
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[(8) / 4]);
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[(0xc) / 4]);
        }
        bool showShop = camp < 0xc || special;
        if (!showShop) {
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 100) showShop = true;
            else {
                st = (Station *) Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x65) showShop = true;
            }
        }
        if (showShop)
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[(4) / 4]);

        int race;
        Station *st = (Station *) Status_getStation_oi();
        if (Station_getIndex_oi(st) == 0x65) {
            race = 8;
        } else {
            st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 100) {
                race = 7;
            } else {
                Status_getSystem_oi();
                race = SolarSystem_getRace_oi();
            }
        }
        {
            const int *coord = ModStation_msc_camCoordTable();
            const int *rot = ModStation_msc_camRotTable();
            float kx = VectorSignedToFloat(coord[race * 3 + 0], 0);
            float ky = VectorSignedToFloat(coord[race * 3 + 1], 0);
            float kz = VectorSignedToFloat(coord[race * 3 + 2], 0);
            float yaw = VectorSignedToFloat(rot[race], 0);
            this->camCoordX = kx;
            this->camCoordY = ky;
            this->camCoordZ = kz;
            Matrix key;
            MatrixSetTranslation(key, kx, ky, kz);
            MatrixSetRotation(key, 0.0f, yaw, 0.0f);
            ModStation_oi_setCameraLocal(ModStation_oi_cameraHandle(), key);
        }
        this->resetLight();
        if (reinterpret_cast<uint8_t*>(&this->dragFlags)[1] != 0)
            this->enterStation();

        if (Status_getCurrentCampaignMission_oi() == 0x4d) {
            Station *st2 = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st2) == 100) {
                Status_getStation_oi();
                Array<Ship *> *ships = (Array<Ship *> *) (intptr_t) Station_getShips_oi();
                if (ships != 0) {
                    for (unsigned i = 0; i < ships->size(); i = i + 1) {
                        if (Ship_getIndex_oi() == 0x25)
                            Ship_setPrice_oi((int) (intptr_t)(*ships)[i]);
                    }
                }
            }
        }

        if (reinterpret_cast<uint8_t*>(&this->modalFlags)[1] == 0 && reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 &&
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 &&
            ((TextRootRecord *) (intptr_t) * *g_oi_textRoot)->noRoutesHintShown == 0) {
            Station *st2 = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st2) != 0x65) {
                Status_getShip_oi();
                int hasJump = Ship_hasJumpDrive_oi();
                int hasFuel = Ship_hasCargo_oi(Status_getShip_oi(), 0x55);
                Status_getSystem_oi();
                int routes = SolarSystem_getRoutes_oi();
                if (routes == 0 && hasJump == 0 && hasFuel == 0 &&
                    0x10 < Status_getCurrentCampaignMission_oi()) {
                    ChoiceWindow_set_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot), 1);
                    reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 1;
                    reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
                }
            }
        }

        ((Status *) (intptr_t) * (int *) g_oi_status)->field_f8 = 1;
        reinterpret_cast<uint8_t*>(&this->scrollFlags)[0] = 0;
        this->activeTouch = 0;
        reinterpret_cast<uint8_t*>(&this->cameraFlags)[3] = 0;
        this->loadTick = 0;
        reinterpret_cast<uint8_t*>(&this->field_0x148)[0] = 0;
        next = 1;
    } else if (state == 0x28) {
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x78) {
                int camp = Status_getCurrentCampaignMission_oi();
                if (camp == 0x62 || Status_getCurrentCampaignMission_oi() == 100)
                    reinterpret_cast<uint8_t*>(&this->alarmFlags)[0] = 1;
            }
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x3a && Status_getCurrentCampaignMission_oi() == 0x8a)
                reinterpret_cast<uint8_t*>(&this->alarmFlags)[0] = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x7e && Status_getCurrentCampaignMission_oi() == 0x78)
                reinterpret_cast<uint8_t*>(&this->alarmFlags)[0] = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x4e && Status_getCurrentCampaignMission_oi() == 0x8d)
                reinterpret_cast<uint8_t*>(&this->alarmFlags)[0] = 1;
        }
        if (Status_inSupernovaSystem_oi() != 0)
            reinterpret_cast<uint8_t*>(&this->alarmFlags)[0] = 1;
        if (**g_oi_demoFlag != 0)
            reinterpret_cast<uint16_t&>(this->alarmFlags) = 0x101;

        if (reinterpret_cast<uint8_t*>(&this->cameraFlags)[0] == 0) {
            bool gate = false;
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 4) gate = true;
            else {
                st = (Station *) Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x58) gate = true;
                else {
                    Status_getStation_oi();
                    if (Station_stationHasPirateBase_oi() != 0) {
                        if (this->dialogueWindow == 0) {
                            ChoiceWindow *cw = this->choiceWindow;
                            ChoiceWindow_set6_oiImpl(cw,
                                                     GameText_getText_oiImpl(0x3e), GameText_getText_oiImpl(0x49),
                                                     GameText_getText_oiImpl(0x7e), GameText_getText_oiImpl(0x7f),
                                                     GameText_getText_oiImpl(0x20c), GameText_getText_oiImpl(0x20d));
                            reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 1;
                        }
                        goto afterGate;
                    }
                    gate = true;
                }
            }
            (void) gate;
            if (Status_getCurrentCampaignMission_oi() != 0x30 &&
                Status_inBlackMarketSystem_oi() == 0) {
                Station *s1 = (Station *) Status_getStation_oi();
                if (Station_getIndex_oi(s1) != 0x6c) {
                    Station *s2 = (Station *) Status_getStation_oi();
                    if (Station_getIndex_oi(s2) != 100) {
                        Station *s3 = (Station *) Status_getStation_oi();
                        if (Station_getIndex_oi(s3) != 0x65 && reinterpret_cast<uint8_t*>(&this->modalFlags)[2] == 0 &&
                            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0 &&
                            reinterpret_cast<uint8_t*>(&this->alarmFlags)[0] == 0 /* RAWREAD: alarmActive (+0xd8) */ &&
                            ((Status *) (intptr_t) * (int *) g_oi_status)->field_0x108 == 0) {
                            Status_getStation_oi();
                            bool enemy = false;
                            if (Station_hasAttackedFriends_oi() != 0) {
                                enemy = true;
                            } else {
                                int st0 = Status_getStanding_oi();
                                Status_getSystem_oi();
                                SolarSystem_getRace_oi();
                                if (Standing_isEnemy_oi(st0) != 0)
                                    enemy = true;
                            }
                            if (enemy) {
                                int standing = Status_getStanding_oi();
                                int credits = Status_getCredits_oi();
                                if (standing < 0) {
                                    ChoiceWindow_setFee_oiImpl(this->choiceWindow, credits, 0);
                                    reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 1;
                                }
                            }
                        }
                    }
                }
            }
        afterGate:;
        }

        Station *here = (Station *) Status_getStation_oi();
        int hereIdx = Station_getIndex_oi(here);
        Status_getCampaignMission_oi();

        if (Status_getCurrentCampaignMission_oi() == 0x14 &&
            Mission_getTargetStation_oi() == hereIdx) {
            Status_getStation_oi();
            Array<Item *> *items = (Array<Item *> *) (intptr_t) Station_getItems_oi();
            if (items != 0) {
                for (unsigned i = 0; i < items->size(); i = i + 1) {
                    if (Item_getIndex_oi((*items)[i]) == 0x29)
                        Item_setPrice_oi((int) (intptr_t)(*items)[i]);
                }
            }
            Item *it = (Item *) Status_getStation_oi();
            Item_makeItemDescAmt_oi((*(Array<int> *) (intptr_t) * *g_oi_itemTable)[(0xa4) / 4], 10);
            Station_addItem_oi(it);
        }
        if (Status_getCurrentCampaignMission_oi() == 0x1b &&
            Mission_getTargetStation_oi() == hereIdx) {
            Ship_removeCargo_oi(Status_getShip_oi());
        }

        if (Status_getCurrentCampaignMission_oi() == 0x2b) {
            if (hereIdx == 10) reinterpret_cast<uint8_t*>(&this->alarmFlags)[1] = 1;
        } else if (hereIdx == 10 && Status_getCurrentCampaignMission_oi() == 0x2c) {
            reinterpret_cast<uint8_t*>(&this->alarmFlags)[1] = 1;
        }

        bool isS100;
        if (hereIdx == 10 && Status_gameWon_oi() != 0) {
            int ship = Status_getShip_oi();
            if (Ship_hasEquipment_oi(ship, 0x55) == 0) {
                ship = Status_getShip_oi();
                if (Ship_hasCargo_oi(ship, 0x55) == 0) {
                    int st0 = Status_getStation_oi();
                    if (Station_hasItem_oi(st0) == 0 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                        Item *it = (Item *) Status_getStation_oi();
                        Item_makeItemDesc_oi((*(Array<int> *) (intptr_t) * *g_oi_itemTable)[(0x290) / 4]);
                        Station_addItem_oi(it);
                    }
                }
            }
            isS100 = false;
            goto fuelTop;
        } else {
            isS100 = hereIdx == 100;
            if ((unsigned) (hereIdx - 100) < 2 || hereIdx == 10) {
            fuelTop:
                int amt;
                int cargo = Ship_getCargo_oi(Status_getShip_oi());
                amt = (cargo == 0) ? 0 : Item_getAmount_oi();
                if (amt < 6 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                    Item *it = (Item *) Status_getStation_oi();
                    Item_makeItemDesc_oi((*(Array<int> *) (intptr_t) * *g_oi_itemTable)[(0x1e8) / 4]);
                    Station_addItem_oi(it);
                }
            }
        }

        Station *st10 = (Station *) Status_getStation_oi();
        if (Station_getIndex_oi(st10) == 10 && Achievements_gotAllGoldMedals_oi() != 0) {
            Status_getStation_oi();
            Array<Ship *> *ships = (Array<Ship *> *) (intptr_t) Station_getShips_oi();
            bool needNew = ships == 0;
            if (ships != 0) {
                unsigned i = 0;
                bool found = false;
                while (i < ships->size()) {
                    if (Ship_getIndex_oi() == 8) {
                        found = true;
                        break;
                    }
                    i = i + 1;
                }
                needNew = !found;
            }
            if (needNew) {
                Generator *g = (Generator *) ::operator new(1);
                Generator_ctor_oi(g);
                Station *station = (Station *) (intptr_t) Status_getStation_oi();
                Status_getStation_oi();
                int list = Generator_getShipBuyList_oi((Station *) g);
                Station_setShips_oi(station, list);
                Generator_dtor_oi(g);
                ::operator delete(g);
            }
        }

        if (isS100 &&
            (Status_dlc1Won_oi() != 0 ||
             Status_getCurrentCampaignMission_oi() == 0x50 ||
             Status_getCurrentCampaignMission_oi() == 0x51 ||
             Status_getCurrentCampaignMission_oi() == 0x52 ||
             Status_getCurrentCampaignMission_oi() == 0x53 ||
             Status_getCurrentCampaignMission_oi() == 0x54)) {
            Ship *st = (Ship *) Status_getStation_oi();
            if (Station_hasShip_oi((Station *) st) != 0) {
                Array<Ship *> *ships = (Array<Ship *> *) (intptr_t) Station_getShips_oi();
                unsigned i = 0;
                while (i < ships->size() && Ship_getIndex_oi() != 0x25) i = i + 1;
                Station_removeShip_oi(st);
            }
            if (Station_hasShip_oi((Station *) st) == 0) {
                Ship_makeShip_oi((*(Array<int> *) (intptr_t) * *g_oi_shipTable)[(0x98) / 4]);
                Station_addShip_oi(st);
            }
            if (Station_hasShip_oi((Station *) st) == 0) {
                Ship_makeShip_oi((*(Array<int> *) (intptr_t) * *g_oi_shipTable)[(0xa0) / 4]);
                Station_addShip_oi(st);
            }
        }

        Station *st6c = (Station *) Status_getStation_oi();
        bool atReward = Station_getIndex_oi(st6c) == 0x6c;
        int rewardState = atReward ? ((Status *) (intptr_t) * (int *) g_oi_status)->field_114 : 0;
        if (atReward && rewardState == 1) {
            if (this->dialogueWindow != 0) {
            }
            this->dialogueWindow = 0;
            ChoiceWindow_set1_oiImpl(this->choiceWindow, GameText_getText_oiImpl(0x6c));
            reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 1;
        } else {
            Station *st = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x6c && ((Status *) (intptr_t) * (int *) g_oi_status)->field_114 == 2) {
                if (Status_getCredits_oi() < 100000 ||
                    Ship_hasCargo_oi(Status_getShip_oi(), 0x6d) == 0) {
                    ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot));
                } else {
                    ChoiceWindow_set_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot), 1);
                    reinterpret_cast<uint8_t*>(&this->screenFlags)[0] = 1;
                }
                reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
            }
        }

        if (Status_getFreelanceMission_oi() != 0 && Mission_getType_oi() == 0xe) {
            Station *st = (Station *) Status_getStation_oi();
            if (Mission_getTargetStation_oi() == Station_getIndex_oi(st) &&
                Station_hasItem_oi(Status_getStation_oi()) == 0) {
                Item_makeItemDesc_oi((*(Array<int> *) (intptr_t) * *g_oi_itemTable)[(0x1cc) / 4]);
                Item *it = (Item *) Status_getStation_oi();
                Station_addItem_oi(it);
            }
        }

        SettingsBlock *settings = (SettingsBlock *) (intptr_t) * (int *) g_oi_settings;
        int rec = *(int *) g_oi_textRoot;
        (void) rec;
        if (Status_getCurrentCampaignMission_oi() == 0xa2 && (unsigned char) settings->wantedActivatedShown == 0) {
            ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot));
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
            settings->wantedActivatedShown = 1;
        }
        SettingsBlock *settings2 = (SettingsBlock *) (intptr_t) * (int *) g_oi_settings;
        if (Status_gameWon_oi() != 0 && ((Status *) (intptr_t) * (int *) g_oi_status)->byte_0x35 == 0 &&
            settings->gameWonShown == 0) {
            ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot));
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
            settings->gameWonShown = 1;
        }
        if (Status_dlc1Won_oi() != 0 && ((Status *) (intptr_t) * (int *) g_oi_status)->byte_0x37 == 0 &&
            settings->dlc1WonShown == 0) {
            ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot));
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
            settings->dlc1WonShown = 1;
        }
        (void) settings2;

        int wanted = Status_activateNewWanted_oi();
        if (wanted > 0 && reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] == 0) {
            if (wanted == 1)
                ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(**g_oi_textRoot));
            else {
                ChoiceWindow_set1_oiImpl(this->choiceWindow, GameText_getText_oiImpl(0x2c5));
                reinterpret_cast<uint8_t*>(&this->screenFlags)[1] = 1;
            }
            reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 1;
        }
        next = 0x14;
    } else if (state == 0x3c) {
        reinterpret_cast<uint8_t*>(&this->dragFlags)[0] = 0;
        this->camScrollPos = 0;
        this->field_0xe0 = 0;
        this->camScrollVel = 0;
        this->field_0xf4 = 0;
        this->field_0xf8 = 0;
        this->field_0xfc = 0;
        Status_getSystem_oi();
        int angle = (SolarSystem_getRace_oi() == 2) ? (int) 0xffffff38 : 0x10e;
        this->camAngle = angle;
        if (this->cutScene != 0)
            this->cutScene->cameraRotX = (float) angle / 120.0f;
        this->field_0xec = 0;
        reinterpret_cast<uint8_t*>(&this->cameraFlags)[0] =
                reinterpret_cast<uint8_t*>(&this->cameraFlags)[3];
        NewsTicker_build_oiImpl(this);
        next = 0x28;
    } else if (state == 0x50) {
        reinterpret_cast<uint16_t*>(&this->m_nStarMapWindowOpen)[1] = 0;
        reinterpret_cast<uint16_t*>(&this->subWindowFlags)[0] = 0;
        reinterpret_cast<uint8_t*>(&this->screenFlags)[2] = 0;
        reinterpret_cast<uint16_t&>(this->screenFlags) = 0;
        this->modalFlags = 0;
        reinterpret_cast<uint16_t*>(&this->subWindowFlags)[1] = 0;
        reinterpret_cast<uint16_t&>(this->m_nStarMapWindowOpen) = 0x100;
        reinterpret_cast<uint8_t*>(&this->hintFlags)[0] = 0;
        reinterpret_cast<uint16_t*>(&this->hintFlags)[1] = 0;
        reinterpret_cast<uint8_t*>(&this->field_0x12c)[0] = 0;
        this->pendingHangarClose = 0;
        reinterpret_cast<uint8_t*>(&this->m_nStarMapWindowOpen)[3] = 0;
        DlcMenu_build_oiImpl(this);
        next = 0x3c;
    } else if (state == 100) {
        int *sound = *(int **) g_oi_sound;
        reinterpret_cast<Globals *>(*sound)->startNewSoundResourceList();
        static const int snd[] = {
            0x5f, 0x7a, 0x6c, 0x60, 0x61, 0x62, 99, 0x65, 100,
            0x66, 0x68, 0x69, 0x6a, 0x6b, 0x67, 0x7e
        };
        for (unsigned i = 0; i < sizeof(snd) / sizeof(snd[0]); i = i + 1)
            ((Globals *) (*sound))->addSoundResourceToList(snd[i]);

        this->introCountdown = 0x32;
        int *st = *(int **) g_oi_status;
        reinterpret_cast<uint8_t*>(&this->dragFlags)[1] = 1;
        Status_getStation_oi();
        Station_getName_oi();

        Status *stRec = (Status *) (intptr_t) * st;
        long long enterTime = (long long) (uint32_t) stRec->field_70 |
                              ((long long) stRec->field_74 << 32);
        long long now = (long long) (unsigned) Status_getPlayingTime_lo_oi();
        if (now - enterTime >= 0x7531) {
            Station *here = (Station *) Status_getStation_oi();
            if (Station_getIndex_oi(here) != 0x6c) {
                Generator *g = (Generator *) ::operator new(1);
                Generator_ctor_oi(g);
                Status_getStation_oi();
                Generator_computerTradeGoods_oi(g, (Station *) g);
            }
        }
        next = 0x50;
    } else {
        int *sound = *(int **) g_oi_sound;
        FModSound_play_oi(*sound, 0x7a, 0, 0.0f);
        FModSound_enableReverb_oi(*sound);
        FModSound_setDownPitch_oi(*sound);

        int *music = g_oi_musicId;
        if (*music != -1)
            Globals::globals->playMusicAndFadeOutCurrent(**(int **) g_oi_sound);
        *music = -1;
        this->stationActive = 1;
        this->state = 100;

        Array<TouchButton *> *btns = (Array<TouchButton *> *) (intptr_t) this->buttonRow;
        if (btns != 0) {
            for (unsigned i = 0; i < btns->size(); i = i + 1) {
                (void) i;
            }
        }
        return;
    }

    this->state = next;
}

static int *g_dlc_stack = 0;

static int *g_dlc_btnX = 0;

static int *g_dlc_btnY = 0;

static int **g_dlc_btnCount = 0;

static inline void MenuTouchWindow_ctor_dlc(MenuTouchWindow *w, int kind) { new ((void*)w) MenuTouchWindow(kind); }

static inline void MenuTouchWindow_callDlcMenu_dlc(MenuTouchWindow * w) { w->callDlcMenu(); }

void TouchButton_getPosition_dlc(float *dst, MenuTouchWindow *win, unsigned idx);

void ModStation::showDlcMenu() {
    MenuTouchWindow *win = (MenuTouchWindow *) this->dlcMenu;
    if (win == 0) {
        win = (MenuTouchWindow *) ::operator new(0x240);
        MenuTouchWindow_ctor_dlc(win, 2);
        this->dlcMenu = win;
    }
    reinterpret_cast<uint8_t*>(&this->modalFlags)[2] = 1;

    int *bx = g_dlc_btnX;
    int *by = g_dlc_btnY;
    for (unsigned i = 0; i < win->buttons->size(); i = i + 1) {
        if (i < 10) {
            float pos[3];
            TouchButton_getPosition_dlc(pos, win, i);
            bx[i] = (int) pos[0];
            TouchButton_getPosition_dlc(pos, win, i);
            by[i] = (int) pos[1];
            win = (MenuTouchWindow *) this->dlcMenu;
        }
    }
    **g_dlc_btnCount = win->buttons->size();
    reinterpret_cast<uint8_t*>(&this->subWindowFlags)[2] = 0;
    MenuTouchWindow_callDlcMenu_dlc(win);
}

static int *g_cbs_stack = 0;

static int **g_cbs_textId = 0;

static inline int GameText_getText_cbs(int id) { return (int)(Globals::gameText->getText(id)); }

void ChoiceWindow_set_cbs(ChoiceWindow *cw, String *title, String *ok, int modal,
                          String *a, String *b, String *c, int d, int e);

void ModStation::showCBSMessage() {
    String emptyA, emptyB, ok;

    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    String *title = (String *) GameText_getText_cbs(**g_cbs_textId);
    ChoiceWindow_set_cbs(cw, title, &ok, 1, &emptyA, &emptyB, &emptyA, -1, -1);

    reinterpret_cast<uint8_t*>(&this->hintFlags)[1] = 1;
    reinterpret_cast<uint8_t*>(&this->modalFlags)[3] = 1;
}

int GameText_getText_frag(int id);

void ChoiceWindow_set1_frag(ChoiceWindow *cw, int textStr);

void ChoiceWindow_setNotice_frag(ChoiceWindow *cw, int textStr);

void ChoiceWindow_set6_frag(ChoiceWindow *cw, int a, int b, int yesNo, int c, int d, int e);

int Status_holder_frag();

int GameText_root_frag();

static inline int Status_getCredits_frag() { return (int)(Globals::status->getCredits()); }

