#include "game/menu/ModStation.h"
#include <cstddef>
#include "game/core/Globals.h"
#include "engine/core/AERandom.h"
extern int g_android_current_achievements[3];
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
#include "game/ui/HangarWindow.h"
#include "game/ui/StatusWindow.h"
#include "game/ui/ScrollTouchBox.h"
#include "game/ui/MenuTouchWindow.h"
#include "game/world/SolarSystem.h"
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




void ModStation::autosave() {
    if (Globals::status->getPlayingTime() - 1LL < 0)
        return;
    RecordHandler *rh = new RecordHandler();
    rh->recordStoreWrite(0);
    rh->recordStoreWritePreview(0);
    delete rh;
    this->flag_0xb1 = 1;
    if ((int) (intptr_t) this->dlcMenu != 0) {
        this->dlcMenu->callDlcMenu();
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

    Item *e10 = Globals::status->getShip()->getFirstEquipmentOfSort(10);
    Item *e9 = Globals::status->getShip()->getFirstEquipmentOfSort(9);
    int shipIdx = Globals::status->getShip()->getIndex();
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




void ModStation::addAchievement(int medalId, int kind) {
    if (Globals::achievements->isEliteMedal(medalId) != 0)
        return;
    if ((unsigned) (kind - 1) < 2) {
        if (Globals::achievements->getValue(medalId, 3) != -1)
            *g_android_current_achievements = medalId * 3;
        if (kind == 1 && Globals::achievements->getValue(medalId, 2) != -1)
            g_android_current_achievements[1] = medalId * 3 + 1;
    }
    g_android_current_achievements[2] = (medalId * 3 + 3) - kind;
}

void ModStation::showMapWindow() {
    this->flag_0x64 = 0;
    this->flag_0x67 = 1;
}

void ModStation::setGameLoaded() {
    this->flag_0xb0 = 1;
    this->flag_0xb3 = 1;
}


void ModStation::OnSuspend() {
    this->activeTouch = 0;
    RecordHandler **holder = &Globals::recordHandler;
    RecordHandler *obj = *holder;
    if (obj != 0)
        obj->saveOptions();
}


void ModStation::OnResume() {
    FModSound **holder = &Globals::sound;
    FModSound *obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    (*holder)->setVolume(1, *reinterpret_cast<float *>(Globals::options));
}

void ModStation::checkPendingProducts() {
    int campaignMission = Globals::status->getCurrentCampaignMission();
    String message = *Globals::gameText->getText(213) + String("\n", false);
    Array<PendingProduct *> *products = (Array<PendingProduct *> *) (intptr_t) Globals::status->getPendingProducts();
    if (products != nullptr) {
        for (unsigned i = 0; i < products->size(); i++) {
            PendingProduct *product = (*products)[i];
            if (product == nullptr || product->stationIndex != Globals::status->getStation()->getIndex())
                continue;
            if (campaignMission == 146 && product->blueprintIndex == 210) {
                delete (*products)[i];
                (*products)[i] = nullptr;
                return;
            }
            Item *item = (*Globals::items)[product->blueprintIndex]->makeItem(product->quantity);
            Globals::status->getShip()->addCargo(item);
            message = message + String("\n", false) + String(item->getAmount()) + String("x ", false) +
                      *Globals::gameText->getText(item->getIndex() + 1274);
            delete (*products)[i];
            (*products)[i] = nullptr;
            this->flag_0x63 = 1;
        }
    }
    if (this->flag_0x63)
        this->choiceWindow->set(message);
}

static int *g_okp_stack = 0;

static inline void Status_checkForLevelUp_okp() { Globals::status->checkForLevelUp(); }

void leaveStation();

// Portrait part descriptors for the pirate-base greeting dialogues.
static int gPirateGreetingFaceA[10] = {2, 0, 0, 0, 0, 4, 2, 0, 2, 2};
static int gPirateGreetingFaceB[5];

void ModStation::OnKeyPress(long long, long long key) {
    if (!this->stationActive)
        return;
    Globals::status->checkForLevelUp();
    if (this->accumTime < 1000) {
        if (key == 0x10000)
            this->accumTime = 1001;
        return;
    }
    if (this->flag_0x63 || this->flag_0x69 || this->flag_0x65 || this->flag_0x66 || this->flag_0x67 ||
        this->flag_0x64 || this->flag_0x68)
        return;
    if (this->flag_0x6a) {
        if (key == 0x10000)
            checkMedals();
        return;
    }
    unsigned char menuOpen = this->flag_0x61;
    unsigned char dlcOpen = this->flag_0x62;
    if (menuOpen && !dlcOpen) {
        switch (key) {
            case 0x4000:
                this->buttonState[this->selectedButton] = 0;
                this->selectedButton = this->selectedButton > 0 ? this->selectedButton - 1 : 4;
                this->buttonState[this->selectedButton] = 1;
                return;
            case 0x8000:
                this->buttonState[this->selectedButton] = 0;
                this->selectedButton = this->selectedButton < 4 ? this->selectedButton + 1 : 0;
                this->buttonState[this->selectedButton] = 1;
                return;
            case 0x20000:
                return;
        }
        if (key == 0x10000) {
            switch (this->selectedButton) {
                case 0:
                    if (Globals::status->getCurrentCampaignMission() < 5 ||
                        Globals::status->getCurrentCampaignMission() == 48 ||
                        Globals::status->getCurrentCampaignMission() == 49 ||
                        Globals::status->getCurrentCampaignMission() == 56)
                        goto notAvailable;
                    if (Globals::status->getStation()->getIndex() == 4 &&
                        Globals::status->getStation()->stationHasPirateBase()) {
                        if (this->dialogueWindow == nullptr) {
                            this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(432),
                                                                      Globals::gameText->getText(1609),
                                                                      gPirateGreetingFaceA);
                            this->flag_0x69 = 1;
                            Agent *agent = new Agent(0, String("", false), 0, 0, 2, true, 0, 0, 0, 0);
                            Globals::sound->play(Globals::globals->getDialogueSoundId(432, agent), nullptr, nullptr,
                                                 0.0f);
                            delete agent;
                        }
                        return;
                    }
                    if (this->hangarWindow == nullptr) {
                        this->hangarWindow = new HangarWindow();
                        this->hangarWindow->initialize();
                    }
                    if (this->spaceLounge != nullptr && this->spaceLounge->hangarNeedsUpdate()) {
                        delete this->hangarWindow;
                        this->hangarWindow = nullptr;
                        this->hangarWindow = new HangarWindow();
                        this->hangarWindow->initialize();
                        this->spaceLounge->setHangarUpdate(false);
                    } else if (this->m_pDialogueWindow != nullptr &&
                               ((MissionsWindow *) this->m_pDialogueWindow)->hangarNeedsUpdate()) {
                        delete this->hangarWindow;
                        this->hangarWindow = nullptr;
                        this->hangarWindow = new HangarWindow();
                        this->hangarWindow->initialize();
                        ((MissionsWindow *) this->m_pDialogueWindow)->setHangarUpdate(false);
                    }
                    for (unsigned i = 0; i < this->hangarWindow->buttons->size(); i++) {
                        if (i < 10) {
                            Globals::sub_menu_buttons_x[i] =
                                (int) (*this->hangarWindow->buttons)[i]->getPosition().x;
                            Globals::sub_menu_buttons_y[i] =
                                (int) (*this->hangarWindow->buttons)[i]->getPosition().y;
                        }
                    }
                    Globals::sub_menu_button_count = this->hangarWindow->buttons->size();
                    this->field_0xd0 = 0;
                    {
                        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                        if (cargo != nullptr) {
                            for (unsigned i = 0; i < cargo->size(); i++) {
                                if ((*cargo)[i]->getIndex() >= 132 && (*cargo)[i]->getIndex() <= 153)
                                    this->field_0xd0 += (*cargo)[i]->getAmount();
                            }
                        }
                    }
                    Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 1.0f);
                    Globals::sound->stop(122);
                    Globals::sound->play(95, nullptr, nullptr, 0.0f);
                    this->savedPostEffect =
                        static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())->IsPostEffectActivated();
                    this->flag_0x66 = 1;
                    if (!Globals::hints[8]) {
                        Globals::hints[8] = 1;
                        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(622), false));
                    }
                    return;
                case 1:
                    if (Globals::status->getCurrentCampaignMission() < 13 ||
                        Globals::status->getCurrentCampaignMission() == 49)
                        goto notAvailable;
                    if (Globals::status->getStation()->getIndex() == 88 &&
                        Globals::status->getStation()->stationHasPirateBase()) {
                        if (this->dialogueWindow == nullptr) {
                            this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(433),
                                                                      Globals::gameText->getText(1597),
                                                                      gPirateGreetingFaceB);
                            this->flag_0x69 = 1;
                            Agent *agent = new Agent(0, String("", false), 0, 0, 0, true, 0, 0, 0, 0);
                            Globals::sound->play(Globals::globals->getDialogueSoundId(433, agent), nullptr, nullptr,
                                                 0.0f);
                            delete agent;
                        }
                        return;
                    }
                    if (this->spaceLounge != nullptr)
                        this->spaceLounge->init();
                    else
                        this->spaceLounge = new SpaceLounge();
                    Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 2.0f);
                    Globals::sound->stop(122);
                    Globals::sound->play(108, nullptr, nullptr, 0.0f);
                    this->flag_0x65 = 1;
                    if (!Globals::hints[13]) {
                        Globals::hints[13] = 1;
                        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(627), false));
                    }
                    return;
                case 2:
                    if (Globals::status->getCurrentCampaignMission() < 9 ||
                        Globals::status->getCurrentCampaignMission() == 48 ||
                        Globals::status->getCurrentCampaignMission() == 49)
                        goto notAvailable;
                    if (Globals::status->getShip()->getCurrentLoad() > Globals::status->getShip()->getMaxLoad()) {
                        this->choiceWindow->set(*Globals::gameText->getText(204));
                        this->flag_0x63 = 1;
                        return;
                    }
                    if (Globals::status->getCurrentCampaignMission() == 77 &&
                        Globals::status->getShip()->getIndex() != 37) {
                        this->choiceWindow->set(*Globals::gameText->getText(326));
                        this->flag_0x63 = 1;
                        return;
                    }
                    if (this->starMap != nullptr)
                        this->starMap->init(false, nullptr, false, -1);
                    else
                        this->starMap = new StarMap(false, nullptr, false, -1);
                    if (Globals::status->getShip()->hasEquipment(85, 1) ||
                        Globals::status->getShip()->hasJumpDriveIntegrated())
                        this->starMap->setJumpMapMode(false, true);
                    this->flag_0x67 = 1;
                    Globals::sound->stop(122);
                    Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.6f);
                    if (!Globals::hints[14] && Globals::status->getCurrentCampaignMission() >= 16) {
                        Globals::hints[14] = 1;
                        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(628), false));
                    } else if (!Globals::hints[15] && Globals::status->getCurrentCampaignMission() <= 15) {
                        Globals::hints[15] = 1;
                        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(631), false));
                    }
                    return;
                case 3:
                    if (Globals::status->getCurrentCampaignMission() <= 8)
                        goto notAvailable;
                    if (this->m_pDialogueWindow != nullptr)
                        ((MissionsWindow *) this->m_pDialogueWindow)->init();
                    else
                        this->m_pDialogueWindow = (DialogueWindow *) new MissionsWindow();
                    this->flag_0x64 = 1;
                    Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.6f);
                    if (!Globals::hints[19]) {
                        Globals::hints[19] = 1;
                        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(635), false));
                    }
                    return;
                case 4:
                    if (this->statusWindow == nullptr)
                        this->statusWindow = new StatusWindow();
                    this->statusWindow->reInit();
                    this->flag_0x68 = 1;
                    Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.6f);
                    if (!(Globals::iPad | Globals::hints[24])) {
                        Globals::hints[24] = 1;
                        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(640), false));
                    }
                    return;
                default:
                    return;
            }
        notAvailable:
            this->choiceWindow->set(*Globals::gameText->getText(528));
            this->flag_0x63 = 1;
            return;
        }
    }
    if (key == 0x40000 && !dlcOpen) {
        leaveStation();
        return;
    }
    if (key == 0x20000 && !dlcOpen)
        this->flag_0x61 = menuOpen ^ 1;
}

void ModStation::OnKeyRelease(long long, long long) {
}

int ModStation::ShowLoadingScreen() {
    return 1;
}

static int *g_msc_stack = 0;

static inline int Status_getStation_msc() { return (int)(Globals::status->getStation()); }


static inline int Status_getSystem_msc() { return (int)(Globals::status->getSystem()); }

const int *ModStation_msc_camCoordTable();

const int *ModStation_msc_camRotTable();

ModStation::ModStation() {
    this->dt = 0;
    reinterpret_cast<uint8_t*>(&this->cameraTween)[3] = 0;
    reinterpret_cast<uint16_t&>(this->cameraTween) = 0;
    this->stationActive = 0;
    this->flag_0x69 = 0;
    this->state = 100;
    this->starMap = 0;
    this->cutScene = 0;
    this->newsTicker = 0;
    reinterpret_cast<uint32_t&>(this->cameraTween) = 0;
    this->dlcMenu = 0;
    this->activeMission = 0;
    this->radioMessages = 0;
    this->flag_0x6e = 0;
    this->pendingHangarClose = 0;
    this->m_pDialogueWindow = 0;
    this->choiceWindow = 0;

    Station *st = (Station *) Status_getStation_msc();
    int race;
    if (st->getIndex() == 0x65) {
        race = 8;
    } else {
        st = (Station *) Status_getStation_msc();
        if (st->getIndex() == 100) {
            race = 7;
        } else {
            race = ((SolarSystem *) (long) Status_getSystem_msc())->getRace();
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
    if (Globals::status->getShip()->getCurrentLoad() > Globals::status->getShip()->getMaxLoad()) {
        this->choiceWindow->set(*Globals::gameText->getText(204));
        this->flag_0x63 = 1;
        return;
    }
    if (Globals::status->getCurrentCampaignMission() == 6 ||
        (Globals::status->getCurrentCampaignMission() == 7 &&
         (Globals::status->getShip()->getFirePower() == 0.0f ||
          Globals::status->getShip()->getCombinedHP() == Globals::status->getShip()->getBaseHP()))) {
        if (Globals::status->getShip()->hasCargoType(0) || Globals::status->getShip()->hasCargoType(3))
            this->choiceWindow->set(*Globals::gameText->getText(530));
        else
            this->choiceWindow->set(*Globals::gameText->getText(529));
        this->flag_0x63 = 1;
        return;
    }
    if (Globals::status->getCurrentCampaignMission() == 20 &&
        Globals::status->getStation()->getIndex() ==
        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
        this->choiceWindow->set(*Globals::gameText->getText(531));
        this->flag_0x63 = 1;
        return;
    }
    if (Globals::status->getCurrentCampaignMission() == 21 &&
        Globals::status->getStation()->getIndex() ==
        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
        if (!Globals::status->getShip()->hasEquipment(41, 1) && !Globals::status->getShip()->hasEquipment(42, 1) &&
            !Globals::status->getShip()->hasEquipment(43, 1)) {
            this->choiceWindow->set(*Globals::gameText->getText(531));
            this->flag_0x63 = 1;
            return;
        }
    } else if (Globals::status->getCurrentCampaignMission() == 77 && Globals::status->getShip()->getIndex() != 37) {
        this->choiceWindow->set(*Globals::gameText->getText(326));
        this->flag_0x63 = 1;
        return;
    }
    if (this->flag_0x63 || this->flag_0x69 || this->flag_0x6a)
        return;
    this->choiceWindow->set(*Globals::gameText->getText(397), true);
    this->choiceWindow->left();
    this->flag_0xb2 = 1;
    this->flag_0x63 = 1;
}

// Credit reward per medal grade (none, gold, silver, bronze).
static const int gMedalCreditReward[4] = {0, 5000, 2500, 1000};

void ModStation::checkMedals() {
    Globals::globals->reportLeaderboards();
    if (!this->flag_0x6a && !this->flag_0x63) {
        if (Globals::status->getCurrentCampaignMission() == 1)
            addAchievement(0, 1);
        this->medalArray = nullptr;
        int *medals = Globals::achievements->getNewMedals();
        int count = 0;
        this->medalCount = 0;
        for (int i = 0; i < 45; i++) {
            if (medals[i] >= 1)
                this->medalCount = ++count;
        }
        if (count > 0) {
            this->medalArray = new Array<int *>();
            ArraySetLength(this->medalCount, *this->medalArray);
            this->medalCount = 0;
            for (int i = 0; i < 45; i++) {
                if (medals[i] >= 1) {
                    (*this->medalArray)[this->medalCount] = new int[2];
                    (*this->medalArray)[this->medalCount][0] = i;
                    (*this->medalArray)[this->medalCount][1] = medals[i];
                    this->medalCount++;
                }
            }
            this->flag_0x6a = 1;
            this->medalIndex = 0;
            this->medalChoiceWindow = new ChoiceWindow();
            this->medalChoiceWindow->setMedal((*this->medalArray)[0][0], (*this->medalArray)[0][1]);
            if (!Globals::status->hardCoreMode())
                Globals::status->changeCredits(gMedalCreditReward[(*this->medalArray)[0][1]]);
            addAchievement((*this->medalArray)[0][0], (*this->medalArray)[0][1]);
        }
    } else {
        this->medalIndex++;
        if (this->medalIndex >= this->medalCount) {
            this->flag_0x6a = 0;
            return;
        }
        this->medalChoiceWindow->setMedal((*this->medalArray)[this->medalIndex][0],
                                          (*this->medalArray)[this->medalIndex][1]);
        if (!Globals::status->hardCoreMode())
            Globals::status->changeCredits(gMedalCreditReward[(*this->medalArray)[this->medalIndex][1]]);
        addAchievement((*this->medalArray)[this->medalIndex][0], (*this->medalArray)[this->medalIndex][1]);
    }
}

void ModStation::OnRender3D() {
    if (this->stationActive == 0)
        return;
    Globals::Canvas->ClearBuffer(0);

    uint8_t *p65 = &this->flag_0x65;
    if (this->cutScene == 0 || this->flag_0x66 != 0 || this->flag_0x64 != 0 ||
        this->flag_0x68 != 0 || this->flag_0x67 != 0 || *p65 != 0 ||
        this->flag_0x62 != 0) {
        if (this->flag_0x67 != 0) {
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
    else if (this->flag_0x67 != 0)
        this->starMap->render();
    else if (this->flag_0x64 != 0)
        ((MissionsWindow *) (this->m_pDialogueWindow))->render3D();
    else if (this->flag_0x66 == 0 && this->cutScene != 0)
        ((CutScene *) (this->cutScene))->render3D();
    ((PaintCanvas *) Globals::Canvas)->End3d();
}

void ModStation::resetLight() {
    static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetMaterialColorAmbient(0.7f, 0.7f, 0.7f);
    static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightDirection(-this->lightDirection.x, -this->lightDirection.y,
                                                             -this->lightDirection.z, 0x4000);
    static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorDiffuse(1.0f, 1.0f, 1.0f, 0x4000);
    switch (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()) {
        case 1:
            static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorAmbient(0.25f, 0.55f, 0.25f, 0x4000);
            break;
        case 3:
            static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorAmbient(0.45f, 0.25f, 0.25f, 0x4000);
            break;
        case 2:
            static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorAmbient(0.15f, 0.15f, 0.35f, 0x4000);
            break;
        case 8:
            static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorAmbient(0.83f, 0.28f, 0.12f, 0x4000);
            break;
        default:
            static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorAmbient(0.25f, 0.25f, 0.25f, 0x4000);
            break;
    }
    static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetLightColorSpecular(0.5f, 0.5f, 0.5f, 0x4000);
    static_cast<AbyssEngine::Engine *>(Globals::appManager->GetEngine())
        ->LightSetMaterialColorShininess(96.0f);
}

static int *g_ou_stack = 0;






static int **g_ou_appData = 0;


static int **g_ou_module = 0;

static inline int ApplicationManager_GetElapsedTimeMillis_ou() { return (int) Globals::appManager->GetElapsedTimeMillis(); }

static inline int ApplicationManager_GetApplicationData_ou() { return (int)(intptr_t) Globals::appManager->GetApplicationData(); }

static inline void ApplicationManager_SetCurrentApplicationModule_ou(int module) { Globals::appManager->SetCurrentApplicationModule((unsigned int)module); }

static inline void FModSound_stop_ou(int sound) { Globals::sound->stop(sound); }

static inline void Layout_update_ou(int layout) { Globals::layout->update(layout); }

void Layout_formatCredits_ou(ModStation * outStr);

static inline void Status_incPlayingTime_ou(long long delta) { Globals::status->incPlayingTime(delta); }

static inline int Status_getCredits_ou() { return (int)(Globals::status->getCredits()); }

static inline void Status_changeCredits_ou(int status) { Globals::status->changeCredits(status); }

static inline int Status_getCurrentCampaignMission_ou() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline void Status_nextCampaignMission_ou(int status) { Globals::status->nextCampaignMission(status); }

static inline void Status_setCurrentCampaignMission_ou(int status) { Globals::status->setCurrentCampaignMission(status); }

static inline int Status_getStation_ou() { return (int)(Globals::status->getStation()); }


static inline int Status_getSystem_ou() { return (int)(Globals::status->getSystem()); }

int SolarSystem_getIndex_ou();

static inline int Status_getCampaignMission_ou() { return (int)(Globals::status->getCampaignMission()); }

static inline int Status_missionCompleted_ou(int status, int a, long long b) { return (int)(Globals::status->missionCompleted(status, a, b)); }

int Status_missionFailed_ou(int status, int a, int b);

void Status_removeMission_ou(int status);

int Mission_getType_ou();

int Mission_getStatusValue_ou();

void Mission_setStatusValue_ou(int v);


void Mission_setCampaignMission_ou(Mission * mission);

static inline void Achievements_updateCredits_ou(Achievements *ach, int credits) { ach->updateCredits(credits); }

int Globals_getInAppPurchaseArrayIndex_ou(int globals, Array<String *> *arr);

void TouchButton_setText_ou(int btn);

void ChoiceWindow_set_ou(int cw, int text, int flag);

void ChoiceWindow_setNotice_ou(int cw, int text);

static inline int GameText_getText_ou(int id) { return (int)(Globals::gameText->getText(id)); }

static inline void CutScene_ctor_ou(CutScene *cs, int kind) { new ((void*)cs) CutScene(kind); }

int CutScene_initialize_ou(CutScene * cs);

static inline void MenuTouchWindow_update_ou(MenuTouchWindow *w, int dt) { w->update(dt); }

static inline void MenuTouchWindow_dtor_ou(MenuTouchWindow *w) { w->~MenuTouchWindow(); }

static inline void SpaceLounge_ctor_ou(SpaceLounge * l) { new ((void*)l) SpaceLounge(); }

static inline void SpaceLounge_init_ou(SpaceLounge * l) { l->init(); }

static inline float EaseInOut_advance_ou(AbyssEngine::EaseInOut *e, int elapsed) { e->Increase((float)elapsed); return e->GetValue(); }

unsigned ModStation_ou_cameraHandle();

void ModStation_ou_setCameraLocal(unsigned h, const Matrix &m);

static inline void AEGeometry_rotate_ou(AEGeometry *geom, float x, float y, float z) { geom->rotate(x, y, z); }

void Engine_setHangarLightIntensity_ou(float v);

void ModStation::OnUpdate() {
    int *status = (int *) Globals::status;
    int *sound = (int *) Globals::sound;

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

    Globals::sound->updateAll(nullptr, nullptr, nullptr, nullptr);
    int *layout = (int *) Globals::layout;
    Layout_update_ou(*layout);
    if (reinterpret_cast<uint8_t*>(&this->accumTime)[2] == 0)
        Status_incPlayingTime_ou((long long) (unsigned) *(int *) &Globals::status);

    int creditsBtn = this->activeMission;
    Status_getCredits_ou();
    Layout_formatCredits_ou(this);
    TouchButton_setText_ou(creditsBtn);

    Achievements_updateCredits_ou((Achievements *) *(int *) &Globals::achievements, Status_getCredits_ou());

    if (this->stationActive != 0) {
        char *flag = (char *) &Globals::enterSpaceLounge;
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
            Globals::sound->setParamValue(0, *sound, 0.0f);
            FModSound_stop_ou(*sound);
            Globals::sound->play(0x6c, nullptr, nullptr, 0.0f);
            this->flag_0x65 = 1;
            goto epilogue;
        }
    }

    {
        AppData *appData = (AppData *) (intptr_t) ApplicationManager_GetApplicationData_ou();
        if (appData->restoreNotice != 0) {
            ChoiceWindow_setNotice_ou((int) (intptr_t) this->choiceWindow, GameText_getText_ou(*(int *) &Globals::gameText));
            this->flag_0x6f = 1;
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
                ChoiceWindow_setNotice_ou((int) (intptr_t) this->choiceWindow, GameText_getText_ou(*(int *) &Globals::gameText));
                this->flag_0x6f = 1;
                appData->purchaseReady = 0;
                if ((int) (intptr_t) this->hangarWindow != 0)
                    this->hangarWindow->hideMessage();
                if ((int) (intptr_t) this->dlcMenu != 0) {
                    MenuTouchWindow_dtor_ou(this->dlcMenu);
                    ::operator delete(this->dlcMenu);
                }
                this->dlcMenu = 0;
            }
        }
    }

    if (this->flag_0x64 == 0 && this->flag_0x65 == 0 &&
        this->flag_0x66 == 0 && this->flag_0x67 == 0 &&
        this->flag_0x68 == 0 && this->flag_0x60 == 0) {
        if (this->newsTicker != nullptr)
            this->newsTicker->update(0);
        if ((int) (intptr_t) this->cutScene != 0)
            this->cutScene->process(0);
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

    if (this->flag_0x60 != 0) {
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
            this->flag_0x60 = 1;
        }
        this->spaceLounge = (SpaceLounge *) (intptr_t)(t + this->dt);
        this->cutScene->update(this->dt);

        if (((Radio *)(intptr_t)this->activeMission)->lastMessageShown() != 0) {
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
            this->flag_0x60 = 0;
            goto epilogue;
        }
    } else if (this->flag_0x68 != 0) {
        this->dialogueWindow->update(0);
    } else if (this->flag_0x69 != 0) {
        this->starMap->update(this->dt);
    } else if (this->flag_0x67 != 0) {
        this->hangarWindow->update(this->dt);
    } else if (this->flag_0x66 != 0) {
        ((MissionsWindow *) this->m_pDialogueWindow)->update(this->dt);
    } else if (this->flag_0x64 != 0) {
        this->spaceLounge->update(this->dt);
    } else if (this->flag_0x65 != 0) {
        this->statusWindow->update(this->dt);
    }

    if (this->flag_0x6f != 0)
        this->choiceWindow->update(this->dt);
    if (this->flag_0x6e != 0)
        MenuTouchWindow_update_ou(this->dlcMenu, this->dt);

    if (this->flag_0x6f == 0 && this->flag_0x69 == 0 &&
        this->flag_0x6c == 0) {
        if (reinterpret_cast<uint8_t*>(&this->buttonCredits)[2] == 0) {
            this->checkPendingProducts();
            reinterpret_cast<uint8_t*>(&this->buttonCredits)[2] = 1;
        }
        if (reinterpret_cast<uint8_t*>(&this->buttonCredits)[1] == 0) {
            this->checkMedals();
            reinterpret_cast<uint8_t*>(&this->buttonCredits)[1] = 1;
        }

        if (this->flag_0x6a == 0 && this->flag_0x6e != 0) {
            Status *rec = (Status *) (intptr_t) *(int *) &Globals::status;
            if (rec->byte_0x2a == 0) {
                ChoiceWindow_setNotice_ou((int) (intptr_t) this->choiceWindow, GameText_getText_ou(*(int *) &Globals::gameText));
                this->flag_0x6f = 1;
                rec->byte_0x2a = 1;
                this->flag_0x6e = 0;
            }
        }

        bool special = false;
        int intro = (this->flag_0x65 != 0) ? this->spaceLounge->introFinished() : 0;
        int completed = Status_missionCompleted_ou(*status, 1, (long long) intro);
        Mission *m = (Mission *) completed;
        int camp = Status_getCurrentCampaignMission_ou();
        if (m == 0 && camp == 0x74) {
            if (this->flag_0x65 != 0 && this->spaceLounge->introFinished() != 0) {
                Status_getSystem_ou();
                if (SolarSystem_getIndex_ou() == 0x12) {
                    Status_getCampaignMission_ou();
                    int mask = Mission_getStatusValue_ou();
                    Station *st = (Station *) Status_getStation_ou();
                    int sidx = st->getIndex();
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
            int sidx = st->getIndex();
            int bit = (sidx == 0x42) ? 2 : (sidx == 0x37) ? 1 : (sidx == 9) ? 4 : 0;
            bool introOk = this->flag_0x65 != 0 && this->spaceLounge->introFinished() != 0;
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
                        new ((void*)nm) Mission(0xa0, 0, -1);
                        Mission_setCampaignMission_ou(nm);
                        this->dialogueWindow->set(nm, 1, -1);
                        this->flag_0x69 = 1;
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
                this->flag_0x69 = 1;
                goto afterDialogue;
            } else {
                generic = true;
            }
        }
        if (generic) {
            if (special) {
                Station *st = (Station *) Status_getStation_ou();
                int sidx = st->getIndex();
                int slot = (sidx == 0x5e) ? 3 : (sidx - 0x5a);
                Globals::sound->play(slot + 0x619, nullptr, nullptr, 0.0f);
                int kind = (slot == 1) ? 0x39 : 0x3a;
                if (slot == 0 || slot == 2) kind = 0;
                DialogueWindow *dw = new DialogueWindow(
                    (String *) (intptr_t) GameText_getText_ou(*(int *) &Globals::gameText),
                    (String *) (intptr_t) GameText_getText_ou(*(int *) &Globals::gameText),
                    (int *) (long) kind);
                this->flag_0x69 = 1;
                this->dialogueWindow = dw;
            } else if (m == 0) {
                Mission *fm = (Mission *) Status_missionFailed_ou(*status, 1, 0);
                if (fm != 0) {
                    DialogueWindow *dw = new DialogueWindow(fm, (Level *) 0, 2);
                    this->flag_0x69 = 1;
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
                this->flag_0x69 = 1;
                this->dialogueWindow = dw;
                if (Mission_getType_ou() == 0xd) {
                    ((Status *) (intptr_t) * status)->field_0xf0 = 0;
                    this->autosave();
                }
                if (this->spaceLounge != 0)
                    this->spaceLounge->setHangarUpdate(true);
            }
        }
    afterDialogue:
        this->checkHints();
    }

    if (this->flag_0x100 == 0) {
        float cur = reinterpret_cast<float&>(this->flag_0x118);
        float tgt = this->lightDirection.x;
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
        reinterpret_cast<float&>(this->flag_0x118) = cur;
        Engine_setHangarLightIntensity_ou(cur);
    }
    if ((int) (intptr_t) this->cutScene != 0 && this->flag_0x60 == 0) {
        AEGeometry *geom = (AEGeometry *) (intptr_t) this->infoBoxW;
        if (geom != nullptr)
            AEGeometry_rotate_ou(geom, 0.0f, 0.1f, 0.0f);
    }

epilogue:;
}

static int **g_ModStation_ric_chk = 0;

static const float gHangarCamPitch[10] = {-0.27f, -0.3f, -0.18f, -0.2f, 0.0f, 0.0f, 0.0f, -0.2f, -0.2f, 0.0f};
static const float gHangarCamYaw[10] = {-3.36f, -3.75f, -2.05f, 2.57f, 0.0f, 0.0f, 0.0f, 3.0f, 2.5f, 0.0f};

void ModStation::resetIdleCamForHangar() {
    if (this->cutScene != nullptr)
        this->cutScene->resetCamera();
    if (this->easeX != nullptr)
        this->easeX->SetRange(this->camKeyX, this->camKeyX);
    else
        this->easeX = new AbyssEngine::EaseInOut(this->camKeyX, this->camKeyX);
    if (this->easeY != nullptr)
        this->easeY->SetRange(this->camKeyY, this->camKeyY);
    else
        this->easeY = new AbyssEngine::EaseInOut(this->camKeyY, this->camKeyY);
    if (this->easeZ != nullptr)
        this->easeZ->SetRange(this->camKeyZ, this->camKeyZ);
    else
        this->easeZ = new AbyssEngine::EaseInOut(this->camKeyZ, this->camKeyZ);

    AbyssEngine::AEMath::MatrixSetTranslation(
        *reinterpret_cast<Matrix *>(Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent())),
        this->camKeyX, this->camKeyY, this->camKeyZ);
    int race;
    if (Globals::status->getStation()->getIndex() == 101)
        race = 8;
    else if (Globals::status->getStation()->getIndex() == 100)
        race = 7;
    else
        race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
    AbyssEngine::AEMath::MatrixSetRotation(
        *reinterpret_cast<Matrix *>(Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent())),
        gHangarCamPitch[race], gHangarCamYaw[race], 0.0f, (AbyssEngine::AEMath::RotationOrder) 2);
}

static int *g_ch_stack = 0;




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

static inline void FModSound_playWingmanRecruit_ch() {
    Agent *agent = new Agent(0, AbyssEngine::String(""), 0, 0, Globals::status->field_0x2c, true, 0, 0, 0, 0);
    int soundId = Globals::globals->getDialogueSoundId(0x139, agent);
    Globals::sound->play(soundId, nullptr, nullptr, 0.0f);
    delete agent;
}

void ModStation::checkHints() {
    if (this->flag_0x60)
        return;
    if (!this->flag_0x6a && !(this->flag_0x63 | Globals::options[52]) &&
        Globals::status->getCurrentCampaignMission() >= 19) {
        this->choiceWindow->set(*Globals::gameText->getText(62), *Globals::gameText->getText(73), true,
                                *Globals::gameText->getText(126), *Globals::gameText->getText(127),
                                *Globals::gameText->getText(524), -1, -1);
        Globals::options[52] = 1;
        this->flag_0x63 = 1;
        this->flag_0xdc = 1;
    }

    // Bounty hunter hints: one per terminated wanted pilot (4 targets).
    if (!this->flag_0x69) {
        if (this->flag_0x6a)
            return;
        if (!(this->flag_0x63 | Globals::hints[51]) && (*Globals::status->wanted)[6]->isTerminated()) {
            String text(*Globals::gameText->getText(3232), false);
            text = Globals::status->replaceHash(
                String(text, false),
                String(*Globals::gameText->getText((*Globals::status->wanted)[6]->getShip() + 913), false),
                String("#SHIP_NAME", false));
            text = Globals::status->replaceHash(String(text, false), (*Globals::status->wanted)[6]->getName(),
                                                String("#WANTED_NAME", false));
            this->choiceWindow->set(text);
            this->flag_0x63 = 1;
            Globals::hints[51] = 1;
        }
    }
    if (!this->flag_0x69) {
        if (this->flag_0x6a)
            return;
        if (!(this->flag_0x63 | Globals::hints[52]) && (*Globals::status->wanted)[12]->isTerminated()) {
            String text(*Globals::gameText->getText(3232), false);
            text = Globals::status->replaceHash(
                String(text, false),
                String(*Globals::gameText->getText((*Globals::status->wanted)[12]->getShip() + 913), false),
                String("#SHIP_NAME", false));
            text = Globals::status->replaceHash(String(text, false), (*Globals::status->wanted)[12]->getName(),
                                                String("#WANTED_NAME", false));
            this->choiceWindow->set(text);
            this->flag_0x63 = 1;
            Globals::hints[52] = 1;
        }
    }
    if (!this->flag_0x69) {
        if (this->flag_0x6a)
            return;
        if (!(this->flag_0x63 | Globals::hints[53]) && (*Globals::status->wanted)[18]->isTerminated()) {
            String text(*Globals::gameText->getText(3232), false);
            text = Globals::status->replaceHash(
                String(text, false),
                String(*Globals::gameText->getText((*Globals::status->wanted)[18]->getShip() + 913), false),
                String("#SHIP_NAME", false));
            text = Globals::status->replaceHash(String(text, false), (*Globals::status->wanted)[18]->getName(),
                                                String("#WANTED_NAME", false));
            this->choiceWindow->set(text);
            this->flag_0x63 = 1;
            Globals::hints[53] = 1;
        }
    }
    if (!this->flag_0x69) {
        if (this->flag_0x6a)
            return;
        if (!(this->flag_0x63 | Globals::hints[54]) && (*Globals::status->wanted)[24]->isTerminated()) {
            String text(*Globals::gameText->getText(3232), false);
            text = Globals::status->replaceHash(
                String(text, false),
                String(*Globals::gameText->getText((*Globals::status->wanted)[24]->getShip() + 913), false),
                String("#SHIP_NAME", false));
            text = Globals::status->replaceHash(String(text, false), (*Globals::status->wanted)[24]->getName(),
                                                String("#WANTED_NAME", false));
            this->choiceWindow->set(text);
            this->flag_0x63 = 1;
            Globals::hints[54] = 1;
        }
    }

    if (!this->flag_0x6a) {
        if (!(this->flag_0x63 | Globals::hints[26]) && Globals::achievements->gotAllMedals()) {
            this->choiceWindow->set(*Globals::gameText->getText(649));
            Globals::hints[26] = 1;
            this->flag_0x63 = 1;
        }
        if (this->flag_0x6a)
            goto wingmen;
        if (!(this->flag_0x63 | Globals::hints[27]) && Globals::achievements->gotAllGoldMedals()) {
            this->choiceWindow->set(*Globals::gameText->getText(650));
            Globals::hints[27] = 1;
            this->flag_0x63 = 1;
        }
        if (this->flag_0x6a)
            goto wingmen;
        if (!this->flag_0x63) {
            if (!Globals::status->isBlueprintUnlocked(232) && Globals::achievements->gotAllGoldMedals() &&
                Globals::achievements->gotAllSupernovaMedals()) {
                this->choiceWindow->set(*Globals::gameText->getText(651));
                Globals::status->unlockBluePrint(232);
                autosave();
                this->flag_0x63 = 1;
            }
            if (this->flag_0x6a)
                goto wingmen;
        }
        if (!(this->flag_0x63 | Globals::hints[58]) && Globals::status->getCurrentCampaignMission() >= 162 &&
            ((Globals::achievements->gotAllGoldMedals() && Globals::achievements->gotAllSupernovaMedals() &&
              !Globals::status->hardCoreMode()) ||
             Globals::status->hardCoreMode())) {
            Globals::hints[58] = 1;
            this->choiceWindow->set(*Globals::gameText->getText(3233));
            autosave();
            this->flag_0x63 = 1;
        }
    }

wingmen:
    if (this->flag_0x69 || this->flag_0x6a || this->flag_0x63)
        return;
    if (Globals::status->wingmen != nullptr && !this->flag_0xdb && (int) Globals::status->fadeValue <= 0 &&
        !Globals::layout->rewardMessageActive) {
        delete this->dialogueWindow;
        this->dialogueWindow = nullptr;
        this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(313), (*Globals::status->wingmen)[0],
                                                  reinterpret_cast<int *&>(Globals::status->_byte_0x28));
        Agent *agent = new Agent(0, String("", false), 0, 0, Globals::status->field_0x2c, true, 0, 0, 0, 0);
        Globals::sound->play(Globals::globals->getDialogueSoundId(313, agent), nullptr, nullptr, 0.0f);
        delete agent;
        this->flag_0xdb = 1;
        this->flag_0x69 = 1;
        Globals::status->setWingmen(nullptr);
        return;
    }
    // Pending pirate reward (the freighter rescue pays 20000 credits).
    if (reinterpret_cast<uint8_t *>(&Globals::status->field_f8)[1] && !Globals::layout->rewardMessageActive) {
        delete this->dialogueWindow;
        this->dialogueWindow = nullptr;
        this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(442), Globals::gameText->getText(1662),
                                                  gPirateGreetingFaceA);
        Globals::sound->play(589, nullptr, nullptr, 0.0f);
        this->flag_0x69 = 1;
        reinterpret_cast<uint8_t *>(&Globals::status->field_f8)[1] = 0;
        Globals::status->changeCredits(20000);
    }
}



void ModStation::OnTouchMove(int x, int y, void *touch) { // lint: void_ptr
    // lint: void_ptr
    if (this->activeTouch != touch)
        return;
    this->touchX = x;
    this->touchY = y;
    if (this->flag_0x60 != 0)
        return;

    Layout **layoutHolder = &Globals::layout;
    Layout *layoutObj = *layoutHolder;
    if (layoutObj->choiceWindowOpen != 0) {
        layoutObj->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x69 != 0) {
        ((DialogueWindow *) (this->dialogueWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x63 != 0) {
        ((ChoiceWindow *) (this->choiceWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x6a != 0) {
        ((ChoiceWindow *) (this->medalChoiceWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x66 != 0) {
        ((HangarWindow *) (this->hangarWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x67 != 0) {
        this->starMap->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x65 != 0) {
        ((SpaceLounge *) (this->spaceLounge))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x68 != 0) {
        ((StatusWindow *) (this->statusWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x64 != 0) {
        ((MissionsWindow *) (this->m_pDialogueWindow))->OnTouchMove(x, y);
        return;
    }
    if (this->flag_0x62 != 0) {
        ((MenuTouchWindow *) (this->dlcMenu))->OnTouchMove(x, y, nullptr);
        return;
    }
    if (this->flag_0x61 == 0)
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
    if (*(int *) &Globals::h - layout->field_0x10_rightMargin <= y)
        return;
    if (x <= this->infoBoxW)
        return;
    int delta = x - this->camScrollPos;
    this->camScrollVel = delta;
    this->field_0xf4 = 0x3f800000;
    this->camAngle = this->camAngle + delta;
    this->camScrollPos = x;
}




void ModStation::OnRelease() {
    FModSound **soundHolder = &Globals::sound;
    if (*soundHolder != 0) {
        (*soundHolder)->disableReverb();
        (*soundHolder)->stopAllSoundFXEvents();
    }
    Globals::Canvas->FogEnable(0, AbyssEngine::FogMode_1);

    if (this->buttonRow != 0) {
        ArrayReleaseClasses(
                *reinterpret_cast<Array<TouchButton *> *>(this->buttonRow));
        if (this->buttonRow != 0)
            delete reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
    }
    this->buttonRow = 0;

    delete this->hangarWindow;
    this->hangarWindow = 0;

    delete this->starMap;
    this->starMap = 0;

    delete (SpaceLounge *) this->spaceLounge;
    this->spaceLounge = 0;

    delete (MissionsWindow *) this->m_pDialogueWindow;
    this->m_pDialogueWindow = 0;

    delete this->dialogueWindow;
    this->dialogueWindow = 0;

    delete this->medalChoiceWindow;
    this->medalChoiceWindow = 0;

    delete this->statusWindow;
    this->statusWindow = 0;

    delete this->cutScene;
    this->cutScene = 0;

    delete (MenuTouchWindow *) this->dlcMenu;
    this->dlcMenu = 0;

    delete (Radio *) (intptr_t) this->activeMission;
    this->activeMission = 0;

    delete reinterpret_cast<Array<RadioMessage *> *>(this->radioMessages);
    this->radioMessages = 0;

    delete this->newsTicker;
    this->newsTicker = 0;

    delete this->choiceWindow;
    this->choiceWindow = 0;

    Globals::Canvas->ReleaseAllResources();
    Globals::globals->loadFont(GameText::getLanguage());

    Layout **reloadHolder = &Globals::layout;
    if (*reloadHolder != 0) {
        (*reloadHolder)->reload();
        (*&Globals::imageFactory)->reload();
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

    delete (Radio *) (intptr_t) this->activeMission;
    this->activeMission = 0;

    delete this->scrollBox;
    this->scrollBox = 0;

    this->flag_0xb0 = 0;
    this->flag_0xb1 = 0;
    if (*soundHolder != 0)
        (*&Globals::sound)->freeAllEvents();
}

static int *g_ote_stack = 0;




static int **g_ote_module = 0;







static inline void ApplicationManager_SetCurrentApplicationModule_ote(int module) { Globals::appManager->SetCurrentApplicationModule((unsigned int)module); }

static inline int ApplicationManager_GetApplicationData_ote() { return (int)(intptr_t) Globals::appManager->GetApplicationData(); }

static inline int Status_getStation_ote() { return (int)(Globals::status->getStation()); }


int Station_stationHasPirateBase_ote();

int Station_hasShip_ote(Station * station);

void Station_addShip_ote(Ship * s);
void Station_departStation_ote(Station * s);



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

void Ship_removeCargo1_ote(Item * it);
void Ship_addCargo_ote(Item * it);

void Ship_makeShip_ote(int desc);


int Item_getAmount_ote();


int Mission_getType_ote();


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

static inline void Layout_showMissionRewardMessage_ote(int l, int flag) { Globals::layout->showMissionRewardMessage(l, flag); }

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


void SpaceLounge_refresh_ote();

int StatusWindow_OnTouchEnd_ote(int w, int p1, int p2);

int MissionsWindow_OnTouchEnd_ote(int w, int p1);

int MenuTouchWindow_OnTouchEnd_ote(MenuTouchWindow *w, int p1, int p2, void *p3); // lint: void_ptr

static inline void MenuTouchWindow_ctor_ote(MenuTouchWindow *w, int kind) { new ((void*)w) MenuTouchWindow(kind); } // lint: void_ptr

int TouchButton_OnTouchEnd_ote(int btn, int p1);

static inline void FModSound_stop_ote(int sound) { Globals::sound->stop(sound); }

static inline void RecordHandler_saveOptions_ote(RecordHandler * rh) { rh->saveOptions(); }

void CutScene_replacePlayerShip_ote(int cs, int shipIndex);


void ChoiceWindow_setFee_frag(ChoiceWindow *cw, int credits, int templateId);

int *ModStation_ote_buttonRow(ModStation * self);
int TouchButton_getX_ote(TouchButton * btn);
int TouchButton_getY_ote(TouchButton * btn);

static inline void Radio_ctor_ote(Radio * radio) { new ((void*)radio) Radio(); }

void Radio_addMessage_ote(Radio *radio, int textId);

void ScrollTouchBox_initRadio_ote(ScrollTouchBox * box, Radio * radio);

void ModStation::OnTouchEnd(int x, int y, void *touch) { // lint: void_ptr
    // lint: void_ptr
    int *status = (int *) Globals::status;

    if (this->activeTouch != touch)
        return;
    int starMapOpen = this->flag_0x60;
    this->flag_0x118 = 0;
    this->activeTouch = 0;
    if (starMapOpen != 0)
        return;

    int *help = (int *) Globals::layout;
    if (((Layout *) *help)->choiceWindowOpen != 0) {
        if (Layout_OnTouchEndR_ote((Layout *) *help, x, y) != 0)
            ((Layout *) *help)->choiceWindowOpen = 0;
        return;
    }

    if (this->flag_0x69 != 0) {
        if (this->dialogueWindow->OnTouchEnd(x, x) != 0) {
            if (this->activeMission != 0) {
                {
                    Mission *mission = (Mission *) this->activeMission;

                    if (mission != 0) {
                        int type = Mission_getType_ote();
                        int campaign = mission->isCampaignMission();
                        if (type == 8) {
                            if (campaign == 0) {
                                int ship = Status_getShip_ote();
                                int good = mission->getProductionGoodIndex();
                                Mission_getProductionGoodAmount_ote();
                                ((Ship *) (long) ship)->removeCargo(good);
                                if ((int) (intptr_t) this->hangarWindow != 0)
                                    HangarWindow_initialize_ote();
                            }
                        } else if (campaign == 0 && Mission_getType_ote() == 0xb) {
                            Status_setPassengers_ote(*status, 0);
                            if (mission->isCampaignMission() == 0)
                                ((Status *) (intptr_t) * status)->field_b8 += Mission_getProductionGoodAmount_ote();
                            Status_getShip_ote();
                            Array<Item *> *cargo = (Array<Item *> *) (intptr_t) Ship_getCargo_ote();
                            if (cargo != 0) {
                                for (unsigned i = 0; i < cargo->size(); i = i + 1) {
                                    if ((*cargo)[i]->isUnsaleable() != 0 &&
                                        ((*cargo)[i]->getIndex() == 0x74 ||
                                         (*cargo)[i]->getIndex() == 0x75)) {
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
                                        if ((*cargo)[i]->isUnsaleable() != 0 &&
                                            ((*cargo)[i]->getIndex() == 0x74 ||
                                             (*cargo)[i]->getIndex() == 0x75)) {
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
                                        if ((*cargo)[i]->getIndex() == 0x73) {
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

                    if (mission->isCampaignMission() == 0) {
                        Status_incMissionCount_ote(*status);
                        int reward = Mission_getReward_ote();
                        int bonus = Mission_getBonus_ote();
                        Layout_showMissionRewardMessage_ote(*(int *) (int *) Globals::layout,
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
                            this->flag_0x69 = 0;
                            this->checkHints();
                        }
                        return;
                    }

                    int camp = Status_getCurrentCampaignMission_ote();
                    if (camp == 0x2b) {
                        Status_removeMission_ote(*status);
                        Status_setMission_ote(*status);
                        this->flag_0x69 = 0;
                        int snd = *(int *) &Globals::sound;
                        FModSound_stop_ote(snd);
                        Globals::sound->play(0x90, nullptr, nullptr, 0.0f);
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
                        this->flag_0x60 = 1;
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
                            sh->getRace();
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
                                this->flag_0x69 = 0;
                                this->checkHints();
                            }
                            return;
                        }
                        if (d == 1) {
                            Status_removeMission_ote(*status);
                            this->activeMission = 0;
                            Status_setMission_ote(*status);
                            this->selectedButton = 1;
                            this->flag_0x69 = 0;

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
                                this->flag_0x69 = 0;
                                this->activeMission = 0;
                                return;
                            }

                            Station *here = (Station *) Status_getStation_ote();
                            int hereIdx = here->getIndex();

                            if (cm == 0x4d && hereIdx == 100) {
                                Ship *sh = (Ship *) Status_getStation_ote();
                                Ship_makeShip_ote((*(Array<int> *) (intptr_t) *(int *) &Globals::ships)[(0x94) / 4]);
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
                                    this->flag_0x69 = 0;
                                    this->checkHints();
                                }
                                return;
                            }
                            if (cm == 0x4e) {
                                ((Status *) (intptr_t) * status)->field_5c = -1;
                                ((Status *) (intptr_t) * status)->field_60 = -1;
                                ((Status *) (intptr_t) * status)->field_64 = -1;
                                ((Status *) (intptr_t) * status)->field_68 = -1;
                                Achievements_resetNewMedals_ote((Achievements *) *(int *) &Globals::achievements);
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
                                    this->flag_0x69 = 0;
                                    this->checkHints();
                                }
                                return;
                            }
                            Station *st = (Station *) Status_getStation_ote();
                            if (cm == 0x54 && st->getIndex() == 100) {
                                Ship *sh = (Ship *) Status_getStation_ote();
                                if (Station_hasShip_ote((Station *) sh) == 0) {
                                    Ship_makeShip_ote((*(Array<int> *) (intptr_t) *(int *) &Globals::ships)[(0x98) / 4]);
                                    Station_addShip_ote(sh);
                                }
                                if (Station_hasShip_ote((Station *) sh) == 0) {
                                    Ship_makeShip_ote((*(Array<int> *) (intptr_t) *(int *) &Globals::ships)[(0xa0) / 4]);
                                    Station_addShip_ote(sh);
                                }
                                ((Item*)(long)(*(Array<int> *) (intptr_t) *(int *) &Globals::items)[(0x224) / 4])->makeItem();
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
                                    this->flag_0x69 = 0;
                                    this->checkHints();
                                }
                                return;
                            }

                            if (cm == 0xa0 || cm == 0x90 || cm == 99 || cm == 0x77 || cm == 0x85 || cm == 0x6d ||
                                cm == 0x59) {
                                Station *home = (Station *) *status;
                                **(char **) g_ote_module = 1;
                                Galaxy_getStation_ote(*(int *) &Globals::galaxy);
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
                                this->flag_0x66 = 0;
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
                                    this->flag_0x69 = 0;
                                    this->checkHints();
                                }
                                return;
                            }
                            if (cm == 0x80) {
                                Status_activateNewWanted_ote();
                                if (((Status *) (intptr_t) *(int *) &Globals::status)->byte_0x2a == 0)
                                    this->flag_0x6e = 1;
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
                                    this->flag_0x69 = 0;
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
                                this->flag_0x69 = 0;
                                this->checkHints();
                            }
                            return;
                        }
                    }

                    Status_removeMission_ote(*status);
                    this->activeMission = 0;
                    Status_setMission_ote(*status);
                    this->flag_0x69 = 0;
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
            this->flag_0x69 = 0;
            if (dw != 0) {
                delete dw;
            }
            char justEntered = reinterpret_cast<uint8_t*>(&this->choiceWindow)[1];
            this->dialogueWindow = 0;
            if (justEntered == 0) {
                Station *st = (Station *) Status_getStation_ote();
                if (st->getIndex() != 4) {
                    st = (Station *) Status_getStation_ote();
                    if (st->getIndex() != 0x58) {
                        Status_getStation_ote();
                        if (Station_stationHasPirateBase_ote() != 0) {
                            reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
                            Station *home = (Station *) *(int *) &Globals::status;
                            Status_getStation_ote();
                            Status_departStation_ote(home);
                            Achievements_resetNewMedals_ote((Achievements *) *(int *) &Globals::achievements);
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
        if (this->flag_0x65 != 0)
            return;
    }

    if (this->flag_0x6f != 0) {
        int r = ChoiceWindow_OnTouchEnd_ote((int) (intptr_t) this->choiceWindow, x);
        if (r == 1) {
            char departed = this->flag_0x6b;
            reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
            if (departed == 0) {
                this->flag_0x6f = 0;
                reinterpret_cast<uint8_t*>(&this->scrollBox)[0] = 0;
            } else {
                reinterpret_cast<uint8_t*>(&this->departPending)[2] = 0;
                Station *home = (Station *) *(int *) &Globals::status;
                Status_getStation_ote();
                Status_departStation_ote(home);
                Achievements_resetNewMedals_ote((Achievements *) *(int *) &Globals::achievements);
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
                        if (st->getIndex() == 10) {
                            unsigned i = 0;
                            for (;;) {
                                Station *st1 = (Station *) Status_getStation_ote();
                                Array<Agent *> *ag = st1->getAgents();
                                if (ag->size() <= i) break;
                                Station *st2 = (Station *) Status_getStation_ote();
                                Array<Agent *> *agents = st2->getAgents();
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
                        Galaxy_getStation_ote(*(int *) &Globals::galaxy);
                        Status_departStation_ote(home);
                        *(char *) Globals::galaxy = 1;
                    } else {
                        Status_getStation_ote();
                        Status_departStation_ote(home);
                    }
                    ((Status *) (intptr_t) * status)->field_5c = -1;
                    ((Status *) (intptr_t) * status)->field_60 = -1;
                    ((Status *) (intptr_t) * status)->field_64 = -1;
                    ((Status *) (intptr_t) * status)->field_68 = -1;
                    Achievements_resetNewMedals_ote((Achievements *) *(int *) &Globals::achievements);
                    **(int **) g_ote_module = 1;
                    ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                    this->stationActive = 0;
                    return;
                }

                if (this->flag_0x6b != 0) {
                    int credits = Status_getCredits_ote();
                    if ((int) (intptr_t) this->buttonCredits <= credits) {
                        Status_changeCredits_ote(*status);
                        this->flag_0x6b = 0;
                        reinterpret_cast<uint8_t*>(&this->buttonCredits)[0] = 1;
                        Station *st = (Station *) Status_getStation_ote();
                        st->setAttackedFriends(0);
                        reinterpret_cast<uint8_t*>(&this->choiceWindow)[1] = 1;
                        this->enterStation();
                        this->autosave();
                        {
                            if (reinterpret_cast<uint8_t*>(&this->scrollBox)[0] != 0) {
                                this->flag_0x6f = 0;
                                reinterpret_cast<uint8_t*>(&this->scrollBox)[0] = 0;
                                AppData *appData = (AppData *) (intptr_t) ApplicationManager_GetApplicationData_ote();
                                appData->hideRadio = 1;
                                return;
                            }
                            if (this->flag_0x6c == 0) {
                                if (this->flag_0x6d != 0) {
                                    int feeCredits = Status_getCredits_ote();
                                    if (feeCredits < 25000) {
                                        ChoiceWindow_setFee_frag(this->choiceWindow, feeCredits, 1);
                                        this->flag_0x6d = 0;
                                        return;
                                    }
                                    Status_changeCredits_ote(*status);
                                    this->flag_0x6f = 0;
                                    this->flag_0x6d = 0;
                                    Station *home = (Station *) *status;
                                    Galaxy_getStation_ote(*(int *) &Globals::galaxy);
                                    Status_setStation_ote(home);
                                    **(int **) g_ote_module = 0;
                                    ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                                }
                                if (this->pendingHangarClose != 0 && this->flag_0x66 != 0) {
                                    this->pendingHangarClose = 0;
                                    this->flag_0x66 = 0;
                                }
                                this->flag_0x6f = 0;
                            } else {
                                Status_changeCredits_ote(*status);
                                ((Ship *) (long) Status_getShip_ote())->removeCargo(0x6d);
                                ((Status *) (intptr_t) * status)->field_114 = 3;
                                RecordHandler_saveOptions_ote((RecordHandler *) *(int *) &Globals::status);
                                ChoiceWindow_setNotice_ote((int) (intptr_t) this->choiceWindow,
                                                           GameText_getText_ote(*(int *) &Globals::gameText));
                                Station *st2 = (Station *) Status_getStation_ote();
                                st2->setItems(0, (bool)0);
                                ((Status *) (intptr_t) * status)->voidStation->setItems(0, (bool)0);
                                this->flag_0x6c = 0;
                            }
                        }
                        return;
                    }
                    ChoiceWindow_setFee_frag(this->choiceWindow, credits, 0);
                    this->flag_0x6b = 1;
                    reinterpret_cast<uint8_t*>(&this->departPending)[2] = 1;
                    this->flag_0x6f = 1;
                    return;
                }

                {
                    if (reinterpret_cast<uint8_t*>(&this->scrollBox)[0] != 0) {
                        this->flag_0x6f = 0;
                        reinterpret_cast<uint8_t*>(&this->scrollBox)[0] = 0;
                        AppData *appData = (AppData *) (intptr_t) ApplicationManager_GetApplicationData_ote();
                        appData->hideRadio = 1;
                        return;
                    }
                    if (this->flag_0x6c == 0) {
                        if (this->flag_0x6d != 0) {
                            int credits = Status_getCredits_ote();
                            if (credits < 25000) {
                                ChoiceWindow_setFee_frag(this->choiceWindow, credits, 1);
                                this->flag_0x6d = 0;
                                return;
                            }
                            Status_changeCredits_ote(*status);
                            this->flag_0x6f = 0;
                            this->flag_0x6d = 0;
                            Station *home = (Station *) *status;
                            Galaxy_getStation_ote(*(int *) &Globals::galaxy);
                            Status_setStation_ote(home);
                            **(int **) g_ote_module = 0;
                            ApplicationManager_SetCurrentApplicationModule_ote(**(int **) g_ote_module);
                        }
                        if (this->pendingHangarClose != 0 && this->flag_0x66 != 0) {
                            this->pendingHangarClose = 0;
                            this->flag_0x66 = 0;
                        }
                        this->flag_0x6f = 0;
                    } else {
                        Status_changeCredits_ote(*status);
                        ((Ship *) (long) Status_getShip_ote())->removeCargo(0x6d);
                        ((Status *) (intptr_t) * status)->field_114 = 3;
                        RecordHandler_saveOptions_ote((RecordHandler *) *(int *) &Globals::status);
                        ChoiceWindow_setNotice_ote((int) (intptr_t) this->choiceWindow,
                                                   GameText_getText_ote(*(int *) &Globals::gameText));
                        Station *st = (Station *) Status_getStation_ote();
                        st->setItems(0, (bool)0);
                        ((Status *) (intptr_t) * status)->voidStation->setItems(0, (bool)0);
                        this->flag_0x6c = 0;
                    }
                }
            }
        }
        return;
    }

    if (this->flag_0x68 != 0) {
        if (StatusWindow_OnTouchEnd_ote((int) (intptr_t) this->statusWindow, x, y) != 0) {
            this->flag_0x68 = 0;
            this->resetLight();
        }
        return;
    }
    if (this->flag_0x67 != 0) {
        if (StarMap_OnTouchEnd_ote((int) (intptr_t) this->starMap, x) != 0) {
            this->flag_0x67 = 0;
            this->resetLight();
        }
        return;
    }
    if (this->flag_0x66 != 0) {
        if (HangarWindow_OnTouchEnd_ote((HangarWindow *) this->hangarWindow, x, y) != 0) {
            int *st = (int *) Globals::status;

            Status_getShip_ote();
            Array<Item *> *cargo = (Array<Item *> *) (intptr_t) Ship_getCargo_ote();
            int sold = 0;
            if (cargo != 0) {
                for (unsigned i = 0; i < cargo->size(); i = i + 1) {
                    int idx = (*cargo)[i]->getIndex();
                    if (0x83 < idx && (*cargo)[i]->getIndex() < 0x9a)
                        sold += Item_getAmount_ote();
                }
            }
            if ((int) (intptr_t) this->buttonLaunch < sold)
                ((Status *) (intptr_t) * st)->field_a8 += sold - (int) (intptr_t) this->buttonLaunch;
            HangarWindow_setSellMode_ote((int) (intptr_t) this->hangarWindow);
            this->resetIdleCamForHangar();
            HangarWindowHead *hw = (HangarWindowHead *) this->hangarWindow;
            this->flag_0x66 = 0;
            if (hw->shipChanged != 0) {
                hw->shipChanged = 0;
                int cs = (int) (intptr_t) this->cutScene;
                Status_getShip_ote();
                int shipIndex = Ship_getIndex_ote();
                Ship *sh = (Ship *) Status_getShip_ote();
                sh->getRace();
                CutScene_replacePlayerShip_ote(cs, shipIndex);
            }
            if ((int) (intptr_t) this->cutScene != 0)
                this->cutScene->checkForTurret();
            int snd = *(int *) &Globals::sound;
            FModSound_stop_ote(snd);
            Globals::sound->play(0x7a, nullptr, nullptr, 0.0f);
            Globals::sound->setParamValue(0, snd, 0.0f);
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
    if (this->flag_0x64 != 0) {
        if (SpaceLounge_OnTouchEnd_ote((int) (intptr_t) this->spaceLounge, x, y) != 0) {
            this->flag_0x65 = 0;
            this->resetIdleCamForHangar();
            this->resetLight();
            int snd = *(int *) &Globals::sound;
            Globals::sound->setParamValue(0, snd, 0.0f);
            FModSound_stop_ote(snd);
            Globals::sound->play(0x7a, nullptr, nullptr, 0.0f);
            this->cutScene->checkForTurret();
            if ((int) (intptr_t) this->spaceLounge != 0 && this->spaceLounge->hangarNeedsUpdate() != 0) {
                if (this->hangarWindow != 0) {
                    HangarWindow_dtor_ote((HangarWindow *) this->hangarWindow);
                    ::operator delete(this->hangarWindow);
                }
                this->hangarWindow = 0;
            }
        }
        return;
    }
    if (this->flag_0x65 != 0) {
        if (MissionsWindow_OnTouchEnd_ote((int) (intptr_t) this->m_pDialogueWindow, x) != 0) {
            this->flag_0x64 = 0;
            int snd = *(int *) &Globals::sound;
            Globals::sound->setParamValue(0, snd, 0.0f);
        }
        return;
    }
    if (this->flag_0x6e != 0) {
        if (MenuTouchWindow_OnTouchEnd_ote((MenuTouchWindow *) this->dlcMenu,
                                           x, y, touch) != 0) {
            this->flag_0x6e = 0;
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
    if (this->flag_0x6d != 0) {
        {
            int *help = (int *) Globals::layout;

            if (TouchButton_OnTouchEnd_ote((int) (intptr_t) this->dlcMenu, x) != 0)
                return;

            if (TouchButton_OnTouchEnd_ote(this->activeMission, x) != 0) {
                RecordHandler *rh = (RecordHandler *) *(int *) &Globals::status;
                reinterpret_cast<uint8_t *>(&((Status *) (intptr_t) *(int *) &Globals::status)->field_4c)[2] = 1;
                RecordHandler_saveOptions_ote(rh);
                if (this->hangarWindow == 0) {
                    HangarWindow *hw = (HangarWindow *) ::operator new(0x134);
                    HangarWindow_ctor_ote(hw);
                    this->hangarWindow = hw;
                }
                HangarWindow_initialize_ote();
                this->pendingHangarClose = 1;
                this->flag_0x66 = 1;
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
                this->flag_0x6e = 1;
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
                Layout_initHelpWindow_ote(*help, GameText_getText_ote(*(int *) &Globals::gameText));

            if (this->newsTicker->OnTouchEnd(x, 0) == 0) {
                float dx = VectorSignedToFloat(this->idleDeltaX, 0);
                float dy = VectorSignedToFloat(this->idleDeltaY, 0);
                this->camCoordX += dx * 0.01f;
                this->camCoordY += dy * 0.01f;
            }
        }
    }
}



static int **g_ModStation_tb_clear = 0;

void ModStation::OnTouchBegin(int x, int y, void *touch) { // lint: void_ptr
    // lint: void_ptr
    if (this->activeTouch != 0)
        return;
    this->flag_0x118 = 1;
    this->activeTouch = touch;
    int flag = this->flag_0x60;
    this->touchX = x;
    this->touchY = y;
    if (flag != 0) {
        if (((Radio *) (intptr_t) this->activeMission)->lastMessageShown() != 0) {
            (unsigned char &) this->stationActive = 0;
            Globals::status->nextCampaignMission(true);
            unsigned int mod = *(unsigned int *) Globals::appManager;
            **g_ModStation_tb_clear = 0;

            Globals::appManager->SetCurrentApplicationModule(mod);
            this->flag_0x60 = 0;
        }
        return;
    }
    Layout **layoutHolder = &Globals::layout;
    Layout *layoutObj = *layoutHolder;
    if (layoutObj->choiceWindowOpen != 0) {
        layoutObj->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x69 != 0) {
        ((DialogueWindow *) (this->dialogueWindow))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x63 != 0) {
        ((ChoiceWindow *) (this->choiceWindow))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x6a != 0) {
        ((ChoiceWindow *) (this->medalChoiceWindow))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x66 != 0) {
        ((HangarWindow *) (this->hangarWindow))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x65 != 0) {
        ((SpaceLounge *) (this->spaceLounge))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x67 != 0) {
        this->starMap->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x68 != 0) {
        ((StatusWindow *) (this->statusWindow))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x64 != 0) {
        ((MissionsWindow *) (this->m_pDialogueWindow))->OnTouchBegin(x, y);
        return;
    }
    if (this->flag_0x62 != 0) {
        ((MenuTouchWindow *) (this->dlcMenu))->OnTouchBegin(x, y, touch);
        return;
    }
    if (this->flag_0x61 == 0)
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
    if (*(int *) &Globals::h - layout->field_0x10_rightMargin <= y)
        return;
    if (x <= this->infoBoxW)
        return;
    this->field_0xfc = x;
    this->camScrollPos = x;
    this->camScrollVel = 0;
    this->flag_0x100 = 1;
}

void ModStation::OnTouchBegin(int x, int y) {}
void ModStation::OnTouchEnd(int x, int y) {}
void ModStation::OnTouchMove(int x, int y) {}

void ModStation::OnRender2D() {
    this->paintCanvas->Begin2d();
    this->paintCanvas->SetColor(0xffffffff);
    if (!this->stationActive)
        return;

    if (this->flag_0x66) {
        this->hangarWindow->render();
    } else if (this->flag_0x65) {
        this->spaceLounge->draw();
    } else if (this->flag_0x64) {
        ((MissionsWindow *) this->m_pDialogueWindow)->draw();
    } else if (this->flag_0x67) {
        this->starMap->draw();
    } else if (this->flag_0x68) {
        this->statusWindow->draw();
    } else if (this->flag_0x62) {
        this->dlcMenu->draw();
    } else if (this->flag_0x60) {
        Radio *radio = (Radio *) (intptr_t) this->activeMission;
        radio->draw((long long) this->field_0xb4, nullptr, nullptr);
        if (((Radio *) (intptr_t) this->activeMission)->lastMessageShown()) {
            Globals::Canvas->DrawImage2D(this->field_0x5c, 0, (int) reinterpret_cast<float &>(this->field_0x108), 20, 68);
            this->scrollBox->draw();
        }
        int time = this->field_0xb4;
        if (time > 130000 || time < -6000 || time < 0) {
            float fade;
            if (time > 130000)
                fade = (float) (time - 130000) / 6000.0f;
            else if (time < -6000)
                fade = (float) (time + 12000) / 6000.0f;
            else
                fade = (float) (time + 6000) / -6000.0f + 1.0f;
            this->paintCanvas->SetColor((int) (fade * 255.0f));
            Globals::Canvas->FillRectangle(0, 0, Globals::w, Globals::h);
        }
        if (this->field_0xb4 > -6000)
            this->cutScene->render2D();
    } else {
        Globals::layout->drawFooterStation();
        {
            String stationName = Globals::status->getStation()->getName();
            String title(stationName, false);
            Globals::layout->drawHeader(title + (Globals::status->getStation()->getIndex() == 101
                                                     ? String("", false)
                                                     : String(" ", false) + *Globals::gameText->getText(136)));
        }
        if (this->flag_0x61) {
            Globals::layout->drawBox(2, this->infoBoxX, this->infoBoxY, this->infoBoxW, this->infoBoxH, String("", false));
            if (!((SolarSystem *) (intptr_t) Globals::status->getSystem())->hasNoOwner())
                this->paintCanvas->DrawImage2D(this->departPending, Globals::layout->buttonInsetX,
                                               Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                               Globals::layout->field_0x2c_rowHeight * 2);
            String systemTitle = String(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getName(), false) +
                                 String(" ", false) + *Globals::gameText->getText(137);
            unsigned int &font = reinterpret_cast<unsigned int &>(Globals::font);
            if (Globals::Canvas->GetTextWidth(font, systemTitle) >
                this->infoBoxW - Globals::layout->buttonInsetX * 2 - Globals::layout->field_0x4c -
                this->paintCanvas->GetImage2DWidth(this->departPending))
                systemTitle = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getName();
            this->paintCanvas->DrawString(font, systemTitle,
                                          Globals::layout->buttonInsetX +
                                          this->paintCanvas->GetImage2DWidth(this->departPending) +
                                          Globals::layout->field_0x4c,
                                          Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                          Globals::layout->field_0x2c_rowHeight * 2,
                                          false);
            this->paintCanvas->DrawString(font,
                                          *Globals::gameText->getText(133) + String(": ", false) +
                                          String(String(Globals::status->getStation()->getTecLevel()), false),
                                          Globals::layout->buttonInsetX +
                                          this->paintCanvas->GetImage2DWidth(this->departPending) +
                                          Globals::layout->field_0x4c,
                                          Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                          Globals::layout->field_0x4 + Globals::layout->field_0x2c_rowHeight * 4,
                                          false);
            if (Globals::layout->field_0x286 &&
                !((SolarSystem *) (intptr_t) Globals::status->getSystem())->hasNoOwner())
                this->paintCanvas->DrawString(
                    font,
                    *Globals::gameText->getText(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() + 406),
                    Globals::layout->buttonInsetX + this->paintCanvas->GetImage2DWidth(this->departPending) +
                    Globals::layout->field_0x4c,
                    Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                    Globals::layout->field_0x4 * 2 + Globals::layout->field_0x2c_rowHeight * 6,
                    false);
            Array<TouchButton *> *buttons = (Array<TouchButton *> *) (intptr_t) this->buttonRow;
            for (unsigned i = 0; i < 5; i++)
                (*buttons)[i]->draw();
            this->buttonLaunch->draw();
            this->buttonCredits->draw();
            if (Globals::status->getStation()->getIndex() != 101 && Globals::status->getStation()->getIndex() != 108 &&
                ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() != 25 &&
                this->newsTicker != nullptr)
                this->newsTicker->draw();
            Globals::layout->drawMissionRewardMessage(true);
        }
        if (this->accumTime < 3000) {
            this->paintCanvas->DrawString(reinterpret_cast<unsigned int &>(Globals::font),
                                          *Globals::gameText->getText(this->flag_0xb0 ? 54 : 50),
                                          Globals::layout->buttonInsetX + this->infoBoxW,
                                          Globals::layout->headerTitleY, false);
        } else {
            this->flag_0xb0 = 0;
        }
    }

    if (this->flag_0x6a)
        this->medalChoiceWindow->draw();
    else if (this->flag_0x63)
        this->choiceWindow->draw();
    if (this->flag_0x69)
        this->dialogueWindow->draw();
    if (Globals::layout->choiceWindowOpen)
        Globals::layout->drawHelpWindow();
    this->paintCanvas->End2d();
    if (!Globals::layout->choiceWindowOpen) {
        if (this->flag_0x66)
            this->hangarWindow->render3D();
        if (!Globals::layout->choiceWindowOpen && this->flag_0x65)
            this->spaceLounge->draw3DShip();
    }
    this->paintCanvas->SwapBuffer();
}

static int *g_oi_stack = 0;









static int *g_oi_musicId = 0;

static inline int Status_getCurrentCampaignMission_oi() { return (int)(Globals::status->getCurrentCampaignMission()); }

static inline int Status_getStation_oi() { return (int)(Globals::status->getStation()); }


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

void Ship_makeShip_oi(int desc);

void Ship_setRace_oi(int race);

void Ship_setEquipment_oi(Item *eq, int slot);

void Ship_setPrice_oi(int shipPtr);

void Ship_removeCargo_oi(int ship);


int Item_makeItem_oi();

int Item_makeItemDesc_oi(int desc);

int Item_makeItemDescAmt_oi(int desc, int amt);

void Item_setUnsaleable_oi(int flag);


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

static inline void FModSound_enableReverb_oi(int sound) { Globals::sound->enableReverb(sound); }

static inline void FModSound_setDownPitch_oi(int sound) { Globals::sound->setDownPitch(sound); }

static inline void Generator_ctor_oi(Generator * g) { new ((void*)g) Generator(); }
static inline void Generator_dtor_oi(Generator *g) { g->~Generator(); }
static inline void Generator_computerTradeGoods_oi(Generator * g, Station * s) { g->computerTradeGoods(s); }
static inline int Generator_getShipBuyList_oi(Station * s) { return (int)(Globals::generator->getShipBuyList(s)); }

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

// Hangar camera key positions per station race (non-iPad layout).
static const int gHangarCamPos[30] = {1076, 900, -2273, 1787, 1086, -1632, -2247, 1010, -1845, 1800,
                                      800, -1778, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 1200, 800, -2678, 2200, 800, -1678, 0, 0, 0};
static const float gHangarCamYawIPad[10] = {-3.36f, -3.75f, -2.05f, 2.57f, 0.0f, 0.0f, 0.0f, 3.0f, 2.35f, 0.0f};
// Station background image resource per race.
static const unsigned gStationImageByRace[10] = {1190, 1187, 1189, 1188, 0xffffffff, 0xffffffff,
                                                 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};

int ModStation::OnInitialize() {
    this->introTimer = 0;
    if (this->cutScene == nullptr) {
        this->cutScene = new CutScene(23);
        this->cutScene->initialize();
    }

    switch (this->state) {
        case 100: {
            Globals::globals->startNewSoundResourceList();
            Globals::globals->addSoundResourceToList(95);
            Globals::globals->addSoundResourceToList(122);
            Globals::globals->addSoundResourceToList(108);
            Globals::globals->addSoundResourceToList(96);
            Globals::globals->addSoundResourceToList(97);
            Globals::globals->addSoundResourceToList(98);
            Globals::globals->addSoundResourceToList(99);
            Globals::globals->addSoundResourceToList(101);
            Globals::globals->addSoundResourceToList(100);
            Globals::globals->addSoundResourceToList(102);
            Globals::globals->addSoundResourceToList(104);
            Globals::globals->addSoundResourceToList(105);
            Globals::globals->addSoundResourceToList(106);
            Globals::globals->addSoundResourceToList(107);
            Globals::globals->addSoundResourceToList(103);
            Globals::globals->addSoundResourceToList(126);
            this->introCountdown = 50;
            this->flag_0x101 = 1;
            this->stationName = Globals::status->getStation()->getName();
            if (Globals::status->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_70) > 30000 &&
                Globals::status->getStation()->getIndex() != 108) {
                // Restock the trade goods after a while away.
                Generator *generator = new Generator();
                generator->computerTradeGoods(Globals::status->getStation());
            }
            this->state = 80;
            break;
        }
        case 80: {
            this->flag_0x62 = 0;
            this->flag_0x63 = 0;
            this->flag_0x64 = 0;
            this->flag_0x65 = 0;
            this->flag_0x6e = 0;
            this->flag_0x6c = 0;
            this->flag_0x6d = 0;
            this->flag_0x68 = 0;
            this->flag_0x69 = 0;
            this->flag_0x6a = 0;
            this->flag_0x6b = 0;
            this->flag_0x66 = 0;
            this->flag_0x67 = 0;
            this->flag_0x60 = 0;
            this->flag_0x61 = 1;
            this->flag_0xdc = 0;
            this->flag_0xde = 0;
            this->flag_0xdf = 0;
            this->savedPostEffect = false;
            this->pendingHangarClose = 0;
            this->flag_0x63 = 0;

            this->infoBoxW = Globals::layout->field_0xcc;
            this->infoBoxH = Globals::h - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x10_rightMargin;
            this->infoBoxY = Globals::layout->field_0xc_leftMargin;
            this->infoBoxX = 0;
            this->buttonLaunch = new TouchButton(String(""), 12, Globals::w, Globals::h, 34);
            this->buttonCredits = new TouchButton(Globals::layout->formatCredits(Globals::status->getCredits()), 11,
                                                  Globals::w - this->buttonLaunch->getWidth(), Globals::h, 34);
            Array<TouchButton *> *row = new Array<TouchButton *>();
            this->buttonRow = reinterpret_cast<int *>(row);
            ArraySetLength<TouchButton *>(5, *row);

            Layout *layout = Globals::layout;
            int y = layout->field_0xcc - layout->buttonInsetX - layout->field_0x4c;
            int x = Globals::h - layout->field_0x10_rightMargin - layout->field_0x24 - 5 * layout->field_0x30_rowHeight -
                    4 * layout->field_0x34;
            (*row)[0] = new TouchButton(*Globals::gameText->getText(167), 0, Globals::layout->buttonInsetX, x, y, 17, 1);
            (*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[1] =
                new TouchButton(*Globals::gameText->getText(398), 0, Globals::layout->buttonInsetX,
                                x + Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34, y, 17, 1);
            (*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[2] =
                new TouchButton(*Globals::gameText->getText(177), 0, Globals::layout->buttonInsetX,
                                x + 2 * (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34), y, 17,
                                1);
            (*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[3] =
                new TouchButton(*Globals::gameText->getText(129), 0, Globals::layout->buttonInsetX,
                                x + 3 * (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34), y, 17,
                                1);
            (*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[4] =
                new TouchButton(*Globals::gameText->getText(169), 0, Globals::layout->buttonInsetX,
                                x + 4 * (Globals::layout->field_0x30_rowHeight + Globals::layout->field_0x34), y, 17,
                                1);
            this->state = 60;
            break;
        }
        case 60: {
            this->flag_0x100 = 0;
            this->camScrollPos = 0;
            this->field_0xe0 = 0;
            this->camScrollVel = 0;
            this->field_0xf4 = 0;
            this->field_0xf8 = 0;
            this->field_0xfc = 0;
            this->camAngle = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 2 ? -200 : 270;
            if (this->cutScene != nullptr) {
                this->cutScene->cameraRotX = (float) this->camAngle / 120.0f;
            }
            this->field_0xec = 0;
            this->flag_0xb0 = this->flag_0xb3;
            this->newsTicker = new NewsTicker(
                this->infoBoxW,
                Globals::h - Globals::layout->field_0x10_rightMargin -
                Globals::Canvas->GetTextHeight((unsigned) (intptr_t) Globals::font),
                Globals::w - this->infoBoxW, ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace(),
                Globals::status->getCurrentCampaignMission());
            this->accumTime = 0;
            this->flag_0xb1 = 0;
            this->flag_0xb2 = 0;
            this->field_0xd0 = 0;
            this->field_0xd4 = 0;
            this->flag_0xd8 = 0;
            this->flag_0xd9 = 0;
            this->flag_0xda = 0;
            this->flag_0xdb = 0;
            this->choiceWindow = new ChoiceWindow();
            Globals::Canvas->Image2DCreate(
                gStationImageByRace[((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()],
                reinterpret_cast<unsigned &>(this->departPending));
            this->state = 40;
            break;
        }
        case 40: {
            if (!Globals::status->inAlienOrbit() && Globals::status->getStation()->getIndex() == 120 &&
                (Globals::status->getCurrentCampaignMission() == 98 ||
                 Globals::status->getCurrentCampaignMission() == 100)) {
                this->flag_0xd8 = 1;
            }
            if (!Globals::status->inAlienOrbit() && Globals::status->getStation()->getIndex() == 58 &&
                Globals::status->getCurrentCampaignMission() == 138) {
                this->flag_0xd8 = 1;
            }
            if (!Globals::status->inAlienOrbit() && Globals::status->getStation()->getIndex() == 126 &&
                Globals::status->getCurrentCampaignMission() == 120) {
                this->flag_0xd8 = 1;
            }
            if (!Globals::status->inAlienOrbit() && Globals::status->getStation()->getIndex() == 78 &&
                Globals::status->getCurrentCampaignMission() == 141) {
                this->flag_0xd8 = 1;
            }
            if (Globals::status->inSupernovaSystem()) {
                this->flag_0xd8 = 1;
            }
            if (Globals::enterSpaceLounge) {
                this->flag_0xd8 = 1;
                this->flag_0xd9 = 1;
            }

            if (!this->flag_0xb0) {
                if (Globals::status->getStation()->getIndex() != 4 && Globals::status->getStation()->getIndex() != 88 &&
                    Globals::status->getStation()->stationHasPirateBase()) {
                    // Pirate base: the pirates greet the player.
                    if (this->dialogueWindow == nullptr) {
                        this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(434),
                                                                  Globals::gameText->getText(1609),
                                                                  gPirateGreetingFaceA);
                        Agent *agent = new Agent(0, String(""), 0, 0, 2, true, 0, 0, 0, 0);
                        Globals::sound->play(Globals::globals->getDialogueSoundId(434, agent), nullptr, nullptr, 0.0f);
                        delete agent;
                        this->flag_0x101 = 0;
                        this->flag_0x69 = 1;
                    }
                } else if (Globals::status->getCurrentCampaignMission() != 48 &&
                           !Globals::status->inBlackMarketSystem() &&
                           Globals::status->getStation()->getIndex() != 108 &&
                           Globals::status->getStation()->getIndex() != 100 &&
                           Globals::status->getStation()->getIndex() != 101 && !this->flag_0x6a &&
                           !this->flag_0x63 && !this->flag_0xd8 && !Globals::status->field_0x108 &&
                           (Globals::status->getStation()->hasAttackedFriends() ||
                            ((Standing *) (intptr_t) Globals::status->getStanding())
                                ->isEnemy(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()))) {
                    // Hostile station: pay a landing fee.
                    String text;
                    if (((Standing *) (intptr_t) Globals::status->getStanding())
                            ->isEnemy(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace())) {
                        Standing *standing = (Standing *) (intptr_t) Globals::status->getStanding();
                        int faction = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() > 1 ? 1 : 0;
                        int value = standing->getStanding(faction) >= 1 ? standing->getStanding(faction)
                                                                         : -standing->getStanding(faction);
                        this->field_0xd4 = (int) ((float) value / 100.0f * 2800.0f);
                        this->field_0xd4 = Globals::rnd->nextInt(200) + this->field_0xd4 - 100;
                        if (Globals::status->hardCoreMode()) {
                            this->field_0xd4 *= 10;
                        }
                        text = *Globals::gameText->getText(205);
                    } else {
                        this->field_0xd4 = Globals::status->getLevel() * 150 + 1000;
                        if (Globals::status->hardCoreMode()) {
                            this->field_0xd4 *= 10;
                        }
                        text = *Globals::gameText->getText(206);
                    }
                    text = Globals::status->replaceHash(String(text, false),
                                                        Globals::layout->formatCredits(this->field_0xd4),
                                                        String("#C"));
                    this->choiceWindow->set(text, true);
                    this->flag_0x63 = 1;
                    this->flag_0x6b = 1;
                    this->flag_0x101 = 0;
                }
            }

            int stationIndex = Globals::status->getStation()->getIndex();
            Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
            if (Globals::status->getCurrentCampaignMission() == 20 && campaign->getTargetStation() == stationIndex) {
                Array<Item *> *items = Globals::status->getStation()->getItems();
                if (items != nullptr) {
                    for (unsigned i = 0; i < items->size(); i++) {
                        if ((*items)[i]->getIndex() == 41) {
                            (*items)[i]->setPrice(0);
                        }
                    }
                }
                Globals::status->getStation()->addItem((*Globals::items)[41]->makeItem(10, 0));
            }
            if (Globals::status->getCurrentCampaignMission() == 27 && campaign->getTargetStation() == stationIndex) {
                Globals::status->getShip()->removeCargo(131);
            }
            if ((Globals::status->getCurrentCampaignMission() == 43 && stationIndex == 10) ||
                (Globals::status->getCurrentCampaignMission() == 44 && stationIndex == 10)) {
                this->flag_0xd9 = 1;
            }
            if (Globals::status->gameWon() && stationIndex == 10 &&
                !Globals::status->getShip()->hasEquipment(85, 1) && !Globals::status->getShip()->hasCargo(85, 1) &&
                !Globals::status->getStation()->hasItem(85) && !Globals::status->getStation()->hasItem(164)) {
                Globals::status->getStation()->addItem((*Globals::items)[164]->makeItem(50));
            }
            bool atLab = stationIndex == 100;
            if (stationIndex == 100 || stationIndex == 101 || stationIndex == 10) {
                // Keep some fuel for sale where the story needs it.
                Item *fuel = Globals::status->getShip()->getCargo(122);
                int fuelAmount = fuel != nullptr ? fuel->getAmount() : 0;
                if (fuelAmount <= 5 && !Globals::status->getStation()->hasItem(122)) {
                    Globals::status->getStation()->addItem((*Globals::items)[122]->makeItem(10));
                }
            }
            if (Globals::status->getStation()->getIndex() == 10 && Globals::achievements->gotAllGoldMedals()) {
                Array<Ship *> *ships = Globals::status->getStation()->getShips();
                bool hasReward = false;
                if (ships != nullptr) {
                    for (unsigned i = 0; i < ships->size(); i++) {
                        if ((*ships)[i]->getIndex() == 8) {
                            hasReward = true;
                            break;
                        }
                    }
                }
                if (!hasReward) {
                    Generator *generator = new Generator();
                    Globals::status->getStation()->setShips(generator->getShipBuyList(Globals::status->getStation()),
                                                            false);
                    delete generator;
                }
            }
            if (atLab && (Globals::status->dlc1Won() || Globals::status->getCurrentCampaignMission() == 80 ||
                          Globals::status->getCurrentCampaignMission() == 81 ||
                          Globals::status->getCurrentCampaignMission() == 82 ||
                          Globals::status->getCurrentCampaignMission() == 83 ||
                          Globals::status->getCurrentCampaignMission() == 84)) {
                Station *station = Globals::status->getStation();
                if (station->hasShip(37)) {
                    Array<Ship *> *ships = station->getShips();
                    Ship *prototype = nullptr;
                    for (unsigned i = 0; i < ships->size(); i++) {
                        if ((*ships)[i]->getIndex() == 37) {
                            prototype = (*ships)[i];
                            break;
                        }
                    }
                    station->removeShip(prototype);
                }
                if (!station->hasShip(38)) {
                    station->addShip((*Globals::ships)[38]->makeShip(-1));
                }
                if (!station->hasShip(40)) {
                    station->addShip((*Globals::ships)[40]->makeShip(-1));
                }
            }
            if (Globals::status->getStation()->getIndex() == 108 && Globals::status->field_114 == 1) {
                delete this->dialogueWindow;
                this->dialogueWindow = nullptr;
                Mission *mission = new Mission(-1, String(*Globals::gameText->getText(1601), false),
                                               gPirateGreetingFaceA + 5, 4, 0, 108, 0);
                mission->setWon(true);
                Globals::status->field_114 = 2;
                this->dialogueWindow = new DialogueWindow(mission, nullptr, 0);
                this->flag_0x69 = 1;
            } else if (Globals::status->getStation()->getIndex() == 108 && Globals::status->field_114 == 2) {
                if (Globals::status->getCredits() > 30000000 && Globals::status->getShip()->hasCargo(109, 50)) {
                    this->choiceWindow->set(*Globals::gameText->getText(477), true);
                    this->flag_0x6c = 1;
                } else {
                    this->choiceWindow->set(*Globals::gameText->getText(476));
                }
                this->flag_0x63 = 1;
            }
            Mission *freelance = Globals::status->getFreelanceMission();
            if (freelance != nullptr && freelance->getType() == 14 &&
                freelance->getTargetStation() == Globals::status->getStation()->getIndex() &&
                !Globals::status->getStation()->hasItem(115)) {
                Item *item = (*Globals::items)[115]->makeItem(1);
                Globals::status->getStation()->addItem(item);
            }
            if (Globals::status->getCurrentCampaignMission() == 162 && !Globals::hints[43]) {
                this->choiceWindow->set(*Globals::gameText->getText(613));
                this->flag_0x63 = 1;
                Globals::hints[43] = 1;
            }
            if (Globals::status->gameWon() && !(Globals::options[53] || Globals::hints[56])) {
                this->choiceWindow->set(*Globals::gameText->getText(110));
                this->flag_0x63 = 1;
                Globals::hints[56] = 1;
            }
            if (Globals::status->dlc1Won() && !(Globals::options[55] || Globals::hints[57])) {
                this->choiceWindow->set(*Globals::gameText->getText(111));
                this->flag_0x63 = 1;
                Globals::hints[57] = 1;
            }
            int newWanted = Globals::status->activateNewWanted();
            if (newWanted >= 1 && !this->flag_0x63) {
                if (newWanted == 1) {
                    this->choiceWindow->set(*Globals::gameText->getText(3230));
                } else {
                    String text(*Globals::gameText->getText(3231), false);
                    text = Globals::status->replaceHash(String(text, false), String(String(newWanted), false),
                                                        String("#N"));
                    this->choiceWindow->set(text);
                }
                this->flag_0x63 = 1;
            }
            this->state = 20;
            break;
        }
        case 20: {
            if (!this->flag_0xb1 && !this->flag_0xb0 && this->flag_0x101 &&
                !(Globals::status->getCurrentCampaignMission() == 77 &&
                  Globals::status->getStation()->getIndex() == 101)) {
                autosave();
            }
            if (Globals::status->getCurrentCampaignMission() == 1) {
                // Tutorial: the player starts in the Khador ship.
                Globals::status->setShip((*Globals::ships)[0]->makeShip(-1));
                Globals::status->getShip()->setRace(8);
                Item *primary = (*Globals::items)[90]->makeItem();
                primary->setUnsaleable(true);
                Globals::status->getShip()->setEquipment(primary, 0);
                Item *secondary = (*Globals::items)[81]->makeItem();
                Globals::status->getShip()->setEquipment(secondary, 1);
                secondary->setUnsaleable(true);
                this->cutScene->replacePlayerShip(Globals::status->getShip()->getIndex(), 3);
            }
            Array<TouchButton *> &row = *reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
            int mission = Globals::status->getCurrentCampaignMission();
            bool mission15 = false;
            if (mission <= 4) {
                row[0]->setHalfTransparent(true);
                row[2]->setHalfTransparent(true);
                row[3]->setHalfTransparent(true);
            } else if (mission < 9 || mission == 15) {
                mission15 = mission == 15;
                row[2]->setHalfTransparent(true);
                row[3]->setHalfTransparent(true);
            }
            if (mission < 12 || mission15 || Globals::status->getStation()->getIndex() == 100 ||
                Globals::status->getStation()->getIndex() == 101) {
                row[1]->setHalfTransparent(true);
            }

            Vector lightDir = AbyssEngine::AEMath::MatrixGetDir(*reinterpret_cast<AbyssEngine::AEMath::Matrix *>(
                Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent())));
            lightDir.y += -0.3f;
            lightDir.x += -0.2f;
            this->lightDirection = lightDir;
            resetLight();
            if (this->flag_0x101) {
                enterStation();
            }
            if (Globals::status->getCurrentCampaignMission() == 77 &&
                Globals::status->getStation()->getIndex() == 100) {
                Array<Ship *> *ships = Globals::status->getStation()->getShips();
                if (ships != nullptr) {
                    for (unsigned i = 0; i < ships->size(); i++) {
                        if ((*ships)[i]->getIndex() == 37) {
                            (*ships)[i]->setPrice(0);
                        }
                    }
                }
            }

            int race;
            if (Globals::status->getStation()->getIndex() == 101) {
                race = 8;
            } else if (Globals::status->getStation()->getIndex() == 100) {
                race = 7;
            } else {
                race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            }
            AbyssEngine::AEMath::Matrix camera;
            float camX = (float) gHangarCamPos[race * 3];
            float camY = (float) gHangarCamPos[race * 3 + 1];
            float camZ = (float) gHangarCamPos[race * 3 + 2];
            this->camKeyX = camX;
            this->camKeyY = camY;
            this->camKeyZ = camZ;
            if (this->easeX != nullptr) {
                this->easeX->SetRange(this->camKeyX, this->camKeyX);
            } else {
                this->easeX = new AbyssEngine::EaseInOut(this->camKeyX, this->camKeyX);
            }
            if (this->easeY != nullptr) {
                this->easeY->SetRange(this->camKeyY, this->camKeyY);
            } else {
                this->easeY = new AbyssEngine::EaseInOut(this->camKeyY, this->camKeyY);
            }
            if (this->easeZ != nullptr) {
                this->easeZ->SetRange(this->camKeyZ, this->camKeyZ);
            } else {
                this->easeZ = new AbyssEngine::EaseInOut(this->camKeyZ, this->camKeyZ);
            }
            if (this->cutScene != nullptr) {
                AbyssEngine::AEMath::MatrixSetTranslation(camera, camX, camY, camZ);
                float pitch = gHangarCamPitch[race];
                AbyssEngine::AEMath::MatrixSetRotation(camera, pitch,
                                                       (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race],
                                                       -0.03f, (AbyssEngine::AEMath::RotationOrder) 2);
                Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(), camera);
                this->cutScene->vec8 = Vector{camX, camY, camZ};
                reinterpret_cast<Vector &>(this->cutScene->vec8w) =
                    Vector{pitch, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], -0.03f};
                this->cutScene->rootGeom->setMatrix(camera);

                // Random offset of the idle camera.
                uint8_t *flipAxis = &this->_pad_0x12d[0];
                flipAxis[0] = Globals::rnd->nextInt(20) < 10;
                flipAxis[1] = Globals::rnd->nextInt(20) > 10;
                flipAxis[2] = Globals::rnd->nextInt(20) < 10;
                int offset = Globals::rnd->nextInt(150);
                this->cutScene->vec8.x += (float) (flipAxis[1] ? -offset : offset);
                offset = Globals::rnd->nextInt(150);
                this->cutScene->vec8.y += (float) (flipAxis[0] ? -offset : offset);
                offset = Globals::rnd->nextInt(150);
                this->cutScene->vec8.z += (float) (flipAxis[2] ? -offset : offset);
                AbyssEngine::AEMath::MatrixSetTranslation(camera, this->cutScene->vec8.x, this->cutScene->vec8.y,
                                                          this->cutScene->vec8.z);
                if (this->cameraTween != nullptr) {
                    this->cameraTween->SetRange(camera, camera);
                } else {
                    this->cameraTween = new AbyssEngine::EaseInOutMatrix(camera, camera, 3000);
                }
            }

            if (!this->flag_0x69 && !this->flag_0x6a && !this->flag_0x63 &&
                !Globals::layout->rewardMessageActive && Globals::status->getStation()->getIndex() != 101) {
                bool hasJumpDrive = Globals::status->getShip()->hasJumpDrive();
                bool hasFuel = Globals::status->getShip()->hasCargo(85, 1);
                if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRoutes() == nullptr &&
                    !(hasJumpDrive || hasFuel) && Globals::status->getCurrentCampaignMission() >= 17) {
                    this->choiceWindow->set(*Globals::gameText->getText(335), true);
                    this->flag_0x6d = 1;
                    this->flag_0x63 = 1;
                }
            }
            reinterpret_cast<uint8_t &>(Globals::status->field_f8) = 1;
            this->flag_0x118 = 0;
            this->activeTouch = nullptr;
            this->flag_0xb3 = 0;
            this->loadTick = 0;
            reinterpret_cast<uint8_t &>(this->field_0x148) = 0;
            this->state = 1;
            break;
        }
        default: {
            Globals::sound->play(122, nullptr, nullptr, 0.0f);
            Globals::sound->enableReverb(0);
            Globals::sound->setDownPitch(false);
            if (Globals::switch_to_target_setting != -1) {
                Globals::globals->playMusicAndFadeOutCurrent(Globals::switch_to_target_setting);
            }
            Globals::switch_to_target_setting = -1;
            this->stationActive = 1;
            this->state = 100;
            Array<TouchButton *> *row = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
            if (row != nullptr) {
                // Remember the sub menu button positions for the touch overlay.
                for (unsigned i = 0; i < row->size(); i++) {
                    if (i < 10) {
                        Globals::sub_menu_buttons_x[i] = (int) (*row)[i]->getPosition().x;
                        Globals::sub_menu_buttons_y[i] = (int) (*row)[i]->getPosition().y;
                        row = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
                    }
                }
                Globals::sub_menu_button_count = row->size();
            }
            return 0;
        }
    }
    return this->state;
}

static int *g_dlc_stack = 0;




static inline void MenuTouchWindow_ctor_dlc(MenuTouchWindow *w, int kind) { new ((void*)w) MenuTouchWindow(kind); }

void ModStation::showDlcMenu() {
    MenuTouchWindow *menu = this->dlcMenu;
    if (menu == nullptr) {
        menu = (MenuTouchWindow *) ::operator new(0x240);
        MenuTouchWindow_ctor_dlc(menu, 2);
        this->dlcMenu = menu;
    }
    this->flag_0x62 = 1;
    for (unsigned i = 0; i < menu->buttons->size(); i++) {
        if (i < 10) {
            Globals::sub_menu_buttons_x[i] = (int) (*menu->buttons)[i]->getPosition().x;
            Globals::sub_menu_buttons_y[i] = (int) (*this->dlcMenu->buttons)[i]->getPosition().y;
            menu = this->dlcMenu;
        }
    }
    Globals::sub_menu_button_count = menu->buttons->size();
    this->flag_0x66 = 0;
    menu->callDlcMenu();
}

void ModStation::showCBSMessage() {
    String code("Code", false);
    String purchase("In-App Purchase", false);
    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    String *title = Globals::gameText->getText(390);
    cw->set(
            *title,
            String(
                    "Zum Weiterspielen musst Du Galaxy on Fire 2 freischalten. Moechtest Du die "
                    "Vollversion mit einen COMPUTERBILD SPIELE Freischalt-Code oder via "
                    "In-App-Purchase freischalten?",
                    false),
            true, code, purchase, code, -1, -1);

    this->flag_0x63 = 1;
    this->flag_0xdd = 1;
}

void ChoiceWindow_setNotice_frag(ChoiceWindow *cw, int textStr);

void ChoiceWindow_set6_frag(ChoiceWindow *cw, int a, int b, int yesNo, int c, int d, int e);

int Status_holder_frag();

static inline int Status_getCredits_frag() { return (int)(Globals::status->getCredits()); }
