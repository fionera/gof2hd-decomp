#include "game/menu/ModStation.h"
#include <cstddef>
#include "game/core/Globals.h"
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
    int firePower;
    if (Globals::status->getCurrentCampaignMission() == 6 ||
        (Globals::status->getCurrentCampaignMission() == 7 &&
         ((firePower = Globals::status->getShip()->getFirePower(), reinterpret_cast<float &>(firePower) == 0.0f) ||
          Globals::status->getShip()->getCombinedHP() == Globals::status->getShip()->getBaseHP()))) {
        // Ship::firePower holds a float; Ship::getFirePower() is declared int in our headers.
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
    if (this->flag_0x60 != 0)
        return;

    HintRecord *hintRec = (HintRecord *) (intptr_t) (int) (intptr_t) Globals::hints;

    if (this->flag_0x6a == 0 && this->flag_0x63 == 0 &&
        hintRec->flags[0x34] == 0 &&
        0x12 < Status_getCurrentCampaignMission_ch()) {
        ChoiceWindow_set6_ch(this->choiceWindow,
                             GameText_getText_ch(0x3e), GameText_getText_ch(0x49),
                             GameText_getText_ch(0x7e), GameText_getText_ch(0x7f),
                             GameText_getText_ch(0x20c));
        hintRec->flags[0x34] = 1;
        this->flag_0x63 = 1;
        this->flag_0xdc = 1;
    }

    int **status = (int **) &Globals::status;

    static const int wantedOff[4] = {0x18, 0x30, 0x48, 0x60};
    static const int wantedFlag[4] = {0x33, 0x34, 0x35, 0x36};
    if (this->flag_0x69 == 0) {
        for (int k = 0; k < 4; k = k + 1) {
            if (this->flag_0x6a != 0) return;
            if (this->flag_0x63 == 0 && hintRec->flags[wantedFlag[k]] == 0) {
                Status *st = (Status *) (intptr_t) * status;
                Wanted *w = (*st->wanted)[wantedOff[k] / 4];
                if (Wanted_isTerminated_ch(w) != 0) {
                    ChoiceWindow_set1_ch(this->choiceWindow, GameText_getText_ch(wantedOff[k]));
                    this->flag_0x63 = 1;
                    hintRec->flags[wantedFlag[k]] = 1;
                }
            }
        }
    }

    if (this->flag_0x6a == 0) {
        if (this->flag_0x63 == 0 && hintRec->flags[0x1a] == 0 &&
            Achievements_gotAllMedals_ch(*&Globals::achievements) != 0) {
            ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x1a));
            hintRec->flags[0x1a] = 1;
            this->flag_0x63 = 1;
        }
        if (this->flag_0x6a == 0 && this->flag_0x63 == 0 &&
            hintRec->flags[0x1b] == 0 &&
            Achievements_gotAllGoldMedals_ch() != 0) {
            ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x1b));
            hintRec->flags[0x1b] = 1;
            this->flag_0x63 = 1;
        }

        if (this->flag_0x6a == 0 && this->flag_0x63 == 0) {
            Status *statPtr = (Status *) *status;
            if (Status_isBlueprintUnlocked_ch(statPtr, 0xe8) == 0 &&
                Achievements_gotAllGoldMedals_ch() != 0 &&
                Achievements_gotAllSupernovaMedals_ch(*&Globals::achievements) != 0) {
                ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x3b));
                Status_unlockBluePrint_ch(statPtr, 0xe8);
                this->autosave();
                this->flag_0x63 = 1;
            }
        }

        if (this->flag_0x6a == 0 && this->flag_0x63 == 0 &&
            hintRec->flags[0x3a] == 0 &&
            0xa1 < Status_getCurrentCampaignMission_ch()) {
            bool ok = (Achievements_gotAllGoldMedals_ch() != 0 &&
                       Achievements_gotAllSupernovaMedals_ch(*&Globals::achievements) != 0 &&
                       Status_hardCoreMode_ch() == 0) ||
                      Status_hardCoreMode_ch() != 0;
            if (ok) {
                hintRec->flags[0x3a] = 1;
                ChoiceWindow_set1_ch(this->choiceWindow, GameText_text_ch(0x3c));
                this->autosave();
                this->flag_0x63 = 1;
            }
        }
    }

    if (this->flag_0x69 == 0 && this->flag_0x6a == 0 &&
        this->flag_0x63 == 0) {
        delete this->dialogueWindow;
        this->dialogueWindow = nullptr;
        DialogueWindow *dw = (DialogueWindow *) ::operator new(0x74);
        DialogueWindow_initWingman_ch(dw, 0);
        this->dialogueWindow = dw;
        FModSound_playWingmanRecruit_ch();
        this->flag_0x69 = 1;
        this->flag_0xdb = 1;
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

int ModStation::OnInitialize() {
    int *status = (int *) Globals::status;

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
        if (this->flag_0xb1 == 0 && this->flag_0xb0 == 0 &&
            this->flag_0x101 != 0) {
            bool skip = false;
            if (Status_getCurrentCampaignMission_oi() == 0x4d) {
                Station *st = (Station *) Status_getStation_oi();
                if (st->getIndex() == 0x65)
                    skip = true;
            }
            if (!skip)
                this->autosave();
        }

        int *shipTbl = (int *) Globals::ships;

        if (Status_getCurrentCampaignMission_oi() == 1) {
            Ship *ship = (Ship *) *shipTbl;
            Ship_makeShip_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::ships)[0]);
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
            if (st->getIndex() == 100) showShop = true;
            else {
                st = (Station *) Status_getStation_oi();
                if (st->getIndex() == 0x65) showShop = true;
            }
        }
        if (showShop)
            TouchButton_setHalfTransparent_oi(((int *) (intptr_t) this->buttonRow[1])[(4) / 4]);

        int race;
        Station *st = (Station *) Status_getStation_oi();
        if (st->getIndex() == 0x65) {
            race = 8;
        } else {
            st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 100) {
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
        if (this->flag_0x101 != 0)
            this->enterStation();

        if (Status_getCurrentCampaignMission_oi() == 0x4d) {
            Station *st2 = (Station *) Status_getStation_oi();
            if (st2->getIndex() == 100) {
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

        if (this->flag_0x69 == 0 && this->flag_0x6a == 0 &&
            this->flag_0x63 == 0 &&
            ((TextRootRecord *) (intptr_t) *(int *) &Globals::gameText)->noRoutesHintShown == 0) {
            Station *st2 = (Station *) Status_getStation_oi();
            if (st2->getIndex() != 0x65) {
                Status_getShip_oi();
                int hasJump = Ship_hasJumpDrive_oi();
                int hasFuel = Ship_hasCargo_oi(Status_getShip_oi(), 0x55);
                Status_getSystem_oi();
                int routes = SolarSystem_getRoutes_oi();
                if (routes == 0 && hasJump == 0 && hasFuel == 0 &&
                    0x10 < Status_getCurrentCampaignMission_oi()) {
                    ChoiceWindow_set_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText), 1);
                    this->flag_0x6d = 1;
                    this->flag_0x63 = 1;
                }
            }
        }

        ((Status *) (intptr_t) *(int *) &Globals::status)->field_f8 = 1;
        this->flag_0x118 = 0;
        this->activeTouch = 0;
        this->flag_0xb3 = 0;
        this->loadTick = 0;
        reinterpret_cast<uint8_t*>(&this->field_0x148)[0] = 0;
        next = 1;
    } else if (state == 0x28) {
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 0x78) {
                int camp = Status_getCurrentCampaignMission_oi();
                if (camp == 0x62 || Status_getCurrentCampaignMission_oi() == 100)
                    this->flag_0xd8 = 1;
            }
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 0x3a && Status_getCurrentCampaignMission_oi() == 0x8a)
                this->flag_0xd8 = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 0x7e && Status_getCurrentCampaignMission_oi() == 0x78)
                this->flag_0xd8 = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 0x4e && Status_getCurrentCampaignMission_oi() == 0x8d)
                this->flag_0xd8 = 1;
        }
        if (Status_inSupernovaSystem_oi() != 0)
            this->flag_0xd8 = 1;
        if (Globals::iPad != 0)
            this->flag_0xd8 = 1;
            this->flag_0xd9 = 1;

        if (this->flag_0xb0 == 0) {
            bool gate = false;
            Station *st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 4) gate = true;
            else {
                st = (Station *) Status_getStation_oi();
                if (st->getIndex() == 0x58) gate = true;
                else {
                    Status_getStation_oi();
                    if (Station_stationHasPirateBase_oi() != 0) {
                        if (this->dialogueWindow == 0) {
                            ChoiceWindow *cw = this->choiceWindow;
                            ChoiceWindow_set6_oiImpl(cw,
                                                     GameText_getText_oiImpl(0x3e), GameText_getText_oiImpl(0x49),
                                                     GameText_getText_oiImpl(0x7e), GameText_getText_oiImpl(0x7f),
                                                     GameText_getText_oiImpl(0x20c), GameText_getText_oiImpl(0x20d));
                            this->flag_0x6d = 1;
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
                if (s1->getIndex() != 0x6c) {
                    Station *s2 = (Station *) Status_getStation_oi();
                    if (s2->getIndex() != 100) {
                        Station *s3 = (Station *) Status_getStation_oi();
                        if (s3->getIndex() != 0x65 && this->flag_0x6a == 0 &&
                            this->flag_0x63 == 0 &&
                            this->flag_0xd8 == 0 /* RAWREAD: alarmActive (+0xd8) */ &&
                            ((Status *) (intptr_t) *(int *) &Globals::status)->field_0x108 == 0) {
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
                                    this->flag_0x6d = 1;
                                }
                            }
                        }
                    }
                }
            }
        afterGate:;
        }

        Station *here = (Station *) Status_getStation_oi();
        int hereIdx = here->getIndex();
        Status_getCampaignMission_oi();

        if (Status_getCurrentCampaignMission_oi() == 0x14 &&
            Mission_getTargetStation_oi() == hereIdx) {
            Status_getStation_oi();
            Array<Item *> *items = (Array<Item *> *) (intptr_t) Station_getItems_oi();
            if (items != 0) {
                for (unsigned i = 0; i < items->size(); i = i + 1) {
                    if ((*items)[i]->getIndex() == 0x29)
                        Item_setPrice_oi((int) (intptr_t)(*items)[i]);
                }
            }
            Item *it = (Item *) Status_getStation_oi();
            Item_makeItemDescAmt_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::items)[(0xa4) / 4], 10);
            Station_addItem_oi(it);
        }
        if (Status_getCurrentCampaignMission_oi() == 0x1b &&
            Mission_getTargetStation_oi() == hereIdx) {
            Ship_removeCargo_oi(Status_getShip_oi());
        }

        if (Status_getCurrentCampaignMission_oi() == 0x2b) {
            if (hereIdx == 10) this->flag_0xd9 = 1;
        } else if (hereIdx == 10 && Status_getCurrentCampaignMission_oi() == 0x2c) {
            this->flag_0xd9 = 1;
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
                        Item_makeItemDesc_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::items)[(0x290) / 4]);
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
                int cargo = (int) (intptr_t) ((Ship *) (long) Status_getShip_oi())->getCargo();
                amt = (cargo == 0) ? 0 : Item_getAmount_oi();
                if (amt < 6 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                    Item *it = (Item *) Status_getStation_oi();
                    Item_makeItemDesc_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::items)[(0x1e8) / 4]);
                    Station_addItem_oi(it);
                }
            }
        }

        Station *st10 = (Station *) Status_getStation_oi();
        if (st10->getIndex() == 10 && Achievements_gotAllGoldMedals_oi() != 0) {
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
                Ship_makeShip_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::ships)[(0x98) / 4]);
                Station_addShip_oi(st);
            }
            if (Station_hasShip_oi((Station *) st) == 0) {
                Ship_makeShip_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::ships)[(0xa0) / 4]);
                Station_addShip_oi(st);
            }
        }

        Station *st6c = (Station *) Status_getStation_oi();
        bool atReward = st6c->getIndex() == 0x6c;
        int rewardState = atReward ? ((Status *) (intptr_t) *(int *) &Globals::status)->field_114 : 0;
        if (atReward && rewardState == 1) {
            if (this->dialogueWindow != 0) {
            }
            this->dialogueWindow = 0;
            ChoiceWindow_set1_oiImpl(this->choiceWindow, GameText_getText_oiImpl(0x6c));
            this->flag_0x6d = 1;
        } else {
            Station *st = (Station *) Status_getStation_oi();
            if (st->getIndex() == 0x6c && ((Status *) (intptr_t) *(int *) &Globals::status)->field_114 == 2) {
                if (Status_getCredits_oi() < 100000 ||
                    Ship_hasCargo_oi(Status_getShip_oi(), 0x6d) == 0) {
                    ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText));
                } else {
                    ChoiceWindow_set_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText), 1);
                    this->flag_0x6c = 1;
                }
                this->flag_0x63 = 1;
            }
        }

        if (Status_getFreelanceMission_oi() != 0 && Mission_getType_oi() == 0xe) {
            Station *st = (Station *) Status_getStation_oi();
            if (Mission_getTargetStation_oi() == st->getIndex() &&
                Station_hasItem_oi(Status_getStation_oi()) == 0) {
                Item_makeItemDesc_oi((*(Array<int> *) (intptr_t) *(int *) &Globals::items)[(0x1cc) / 4]);
                Item *it = (Item *) Status_getStation_oi();
                Station_addItem_oi(it);
            }
        }

        SettingsBlock *settings = (SettingsBlock *) (intptr_t) (int) (intptr_t) Globals::options;
        int rec = *(int *) &Globals::gameText;
        (void) rec;
        if (Status_getCurrentCampaignMission_oi() == 0xa2 && (unsigned char) settings->wantedActivatedShown == 0) {
            ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText));
            this->flag_0x63 = 1;
            settings->wantedActivatedShown = 1;
        }
        SettingsBlock *settings2 = (SettingsBlock *) (intptr_t) (int) (intptr_t) Globals::options;
        if (Status_gameWon_oi() != 0 && ((Status *) (intptr_t) *(int *) &Globals::status)->byte_0x35 == 0 &&
            settings->gameWonShown == 0) {
            ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText));
            this->flag_0x63 = 1;
            settings->gameWonShown = 1;
        }
        if (Status_dlc1Won_oi() != 0 && ((Status *) (intptr_t) *(int *) &Globals::status)->byte_0x37 == 0 &&
            settings->dlc1WonShown == 0) {
            ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText));
            this->flag_0x63 = 1;
            settings->dlc1WonShown = 1;
        }
        (void) settings2;

        int wanted = Status_activateNewWanted_oi();
        if (wanted > 0 && this->flag_0x63 == 0) {
            if (wanted == 1)
                ChoiceWindow_setNotice_oi(this->choiceWindow, GameText_getText_oi(*(int *) &Globals::gameText));
            else {
                ChoiceWindow_set1_oiImpl(this->choiceWindow, GameText_getText_oiImpl(0x2c5));
                this->flag_0x6d = 1;
            }
            this->flag_0x63 = 1;
        }
        next = 0x14;
    } else if (state == 0x3c) {
        this->flag_0x100 = 0;
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
        this->flag_0xb0 =
                this->flag_0xb3;
        NewsTicker_build_oiImpl(this);
        next = 0x28;
    } else if (state == 0x50) {
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
        this->savedPostEffect = 0;
        this->pendingHangarClose = 0;
        this->flag_0x63 = 0;
        DlcMenu_build_oiImpl(this);
        next = 0x3c;
    } else if (state == 100) {
        int *sound = (int *) Globals::sound;
        reinterpret_cast<Globals *>(*sound)->startNewSoundResourceList();
        static const int snd[] = {
            0x5f, 0x7a, 0x6c, 0x60, 0x61, 0x62, 99, 0x65, 100,
            0x66, 0x68, 0x69, 0x6a, 0x6b, 0x67, 0x7e
        };
        for (unsigned i = 0; i < sizeof(snd) / sizeof(snd[0]); i = i + 1)
            ((Globals *) (*sound))->addSoundResourceToList(snd[i]);

        this->introCountdown = 0x32;
        int *st = (int *) Globals::status;
        this->flag_0x101 = 1;
        Status_getStation_oi();
        Station_getName_oi();

        Status *stRec = (Status *) (intptr_t) * st;
        long long enterTime = (long long) (uint32_t) stRec->field_70 |
                              ((long long) stRec->field_74 << 32);
        long long now = (long long) (unsigned) Status_getPlayingTime_lo_oi();
        if (now - enterTime >= 0x7531) {
            Station *here = (Station *) Status_getStation_oi();
            if (here->getIndex() != 0x6c) {
                Generator *g = (Generator *) ::operator new(1);
                Generator_ctor_oi(g);
                Status_getStation_oi();
                Generator_computerTradeGoods_oi(g, (Station *) g);
            }
        }
        next = 0x50;
    } else {
        int *sound = (int *) Globals::sound;
        Globals::sound->play(0x7a, nullptr, nullptr, 0.0f);
        FModSound_enableReverb_oi(*sound);
        FModSound_setDownPitch_oi(*sound);

        int *music = g_oi_musicId;
        if (*music != -1)
            Globals::globals->playMusicAndFadeOutCurrent(*(int *) &Globals::sound);
        *music = -1;
        this->stationActive = 1;
        this->state = 100;

        Array<TouchButton *> *btns = (Array<TouchButton *> *) (intptr_t) this->buttonRow;
        if (btns != 0) {
            for (unsigned i = 0; i < btns->size(); i = i + 1) {
                (void) i;
            }
        }
        return this->state;
    }

    this->state = next;
    return next;
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
