#include "game/menu/ModStation.h"
#include <cstddef>
#include "game/core/Globals.h"
#include "engine/core/AERandom.h"
#include "game/core/RadioMessage.h"
#include "game/world/Galaxy.h"
#include "engine/core/NFC.h"
#include "game/world/Level.h"
#include "game/ship/Agent.h"
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
    char pad4c[0x54 - 0x4c];
    Array<String *> *productIds;
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



// Portrait part descriptors of the story characters (5 ints per face).
static int gStoryFaces[62][5] = {
    {0, 8, 8, 1, 8},
    {2, 1, 1, 1, 1},
    {0, 9, 9, 8, 9},
    {0, 2, 3, 3, 10},
    {10, 0, 0, 1, 0},
    {1, 1, 0, 4, 0},
    {0, 6, 8, 8, 7},
    {0, 4, 8, 8, 10},
    {0, 3, 5, 8, 6},
    {0, 7, 7, 8, 5},
    {0, 5, 4, 2, 7},
    {10, 1, 1, 2, 4},
    {2, 2, 2, 4, 2},
    {11, 1, 0, 0, 1},
    {11, 1, 0, 0, 2},
    {11, 1, 0, 0, 3},
    {0, 8, 10, 10, 4},
    {9, 0, 0, 0, 0},
    {7, 1, 1, 0, 0},
    {2, 0, 0, 1, 2},
    {0, 2, 10, 9, 2},
    {1, 2, 4, 3, 1},
    {1, 3, 1, 5, 0},
    {10, 3, 3, 4, 6},
    {0, 0, 5, 8, 6},
    {0, 8, 8, 8, 0},
    {0, 4, 8, 8, 4},
    {0, 0, 5, 8, 10},
    {0, 2, 10, 9, 2},
    {7, 0, 0, 2, 1},
    {0, 5, 4, 8, 3},
    {4, 1, 1, 1, 1},
    {0, 4, 8, 8, 4},
    {0, 2, 10, 9, 2},
    {2, 4, 1, 4, 1},
    {0, 8, 3, 8, 3},
    {2, 2, 1, 3, 2},
    {6, 0, 0, 2, 0},
    {10, 3, 3, 1, 4},
    {0, 0, 5, 4, 6},
    {1, 0, 3, 1, 2},
    {2, 4, 1, 2, 3},
    {1, 0, 1, 1, 3},
    {7, 0, 0, 0, 0},
    {2, 4, 2, 0, 0},
    {2, 0, 1, 1, 2},
    {2, 1, 0, 4, 0},
    {1, 0, 0, 3, 5},
    {0, 3, 3, 0, 5},
    {10, 0, 3, 3, 5},
    {0, 3, 6, 0, 8},
    {10, 3, 0, 3, 3},
    {0, 10, 7, 2, 1},
    {12, 0, 0, 0, 0},
    {0, 0, 10, 0, 7},
    {0, 6, 0, 7, 4},
    {2, 3, 1, 1, 4},
    {2, 1, 4, 2, 0},
    {0, 0, 4, 3, 5},
    {0, 10, 2, 9, 4},
    {2, 0, 0, 0, 0},
    {4, 2, 0, 2, 2}};
static int gPirateGreetingFaceB[5];
// Face of the story character speaking a given story dialogue.
static int *const gStoryFaceTable[63] = {
    gPirateGreetingFaceB, gStoryFaces[0], gStoryFaces[1], gStoryFaces[2], gStoryFaces[61], gStoryFaces[3],
    gStoryFaces[4], gStoryFaces[5], gStoryFaces[6], gStoryFaces[7], gStoryFaces[8], gStoryFaces[9],
    gStoryFaces[10], gStoryFaces[11], gStoryFaces[12], gStoryFaces[13], gStoryFaces[14], gStoryFaces[15],
    gStoryFaces[16], gStoryFaces[17], gStoryFaces[18], gStoryFaces[19], gStoryFaces[60], gStoryFaces[20],
    gStoryFaces[21], gStoryFaces[22], gStoryFaces[23], gStoryFaces[24], gStoryFaces[25], gStoryFaces[26],
    gStoryFaces[27], gStoryFaces[28], gStoryFaces[29], gStoryFaces[30], gStoryFaces[31], gStoryFaces[32],
    gStoryFaces[33], gStoryFaces[34], gStoryFaces[35], gStoryFaces[36], gStoryFaces[37], gStoryFaces[38],
    gStoryFaces[39], gStoryFaces[40], gStoryFaces[41], gStoryFaces[42], gStoryFaces[43], gStoryFaces[44],
    gStoryFaces[45], gStoryFaces[46], gStoryFaces[47], gStoryFaces[48], gStoryFaces[49], gStoryFaces[50],
    gStoryFaces[51], gStoryFaces[52], gStoryFaces[53], gStoryFaces[54], gStoryFaces[55], gStoryFaces[56],
    gStoryFaces[57], gStoryFaces[58], gStoryFaces[59]};




// Credit reward per medal grade (none, gold, silver, bronze).
static const int gMedalCreditReward[4] = {0, 5000, 2500, 1000};






static int **g_ou_appData = 0;


static int **g_ou_module = 0;




static int **g_ModStation_ric_chk = 0;

static const float gHangarCamPitch[10] = {-0.27f, -0.3f, -0.18f, -0.2f, 0.0f, 0.0f, 0.0f, -0.2f, -0.2f, 0.0f};
static const float gHangarCamYaw[10] = {-3.36f, -3.75f, -2.05f, 2.57f, 0.0f, 0.0f, 0.0f, 3.0f, 2.5f, 0.0f};







static int **g_ote_module = 0;
















void ChoiceWindow_setFee_frag(ChoiceWindow *cw, int credits, int templateId);

// Mission reward replacing an out-of-range reward, campaign missions 55..61.
static const int gCampaignRewardOverride[7] = {200000, 6666, 6666, 100000, 6666, 6666, 50000};












static int *g_oi_musicId = 0;






// Hangar camera key positions per station race (iPad layout).
static const int gHangarCamPosIPad[30] = {725, 899, -2271, 1630, 1199, -1933, -2934, 1027, -1890, 1759,
                                          909, -2232, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 1200, 800, -2978, 1759, 909, -2232, 0, 0, 0};
// Hangar camera key positions per station race (non-iPad layout).
static const int gHangarCamPos[30] = {1076, 900, -2273, 1787, 1086, -1632, -2247, 1010, -1845, 1800,
                                      800, -1778, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 1200, 800, -2678, 2200, 800, -1678, 0, 0, 0};
static const float gHangarCamYawIPad[10] = {-3.36f, -3.75f, -2.05f, 2.57f, 0.0f, 0.0f, 0.0f, 3.0f, 2.35f, 0.0f};
// Station background image resource per race.
static const unsigned gStationImageByRace[10] = {1190, 1187, 1189, 1188, 0xffffffff, 0xffffffff,
                                                 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};





void ChoiceWindow_setNotice_frag(ChoiceWindow *cw, int textStr);

void ChoiceWindow_set6_frag(ChoiceWindow *cw, int a, int b, int yesNo, int c, int d, int e);

int Status_holder_frag();

static inline int Status_getCredits_frag() { return (int)(Globals::status->getCredits()); }

ModStation::ModStation() {
    this->idleBox = nullptr;
    this->field_0x108 = 0;
    this->field_0x10c = 0;
    this->lightDirection.x = 0.0f;
    this->lightDirection.y = 0.0f;
    this->lightDirection.z = 0.0f;
    this->dt = 0;
    this->flag_0xb3 = 0;
    this->flag_0xb0 = 0;
    this->flag_0xb1 = 0;
    this->stationActive = 0;
    this->flag_0x69 = 0;
    this->state = 100;
    this->starMap = nullptr;
    this->cutScene = nullptr;
    this->newsTicker = nullptr;
    this->cameraTween = nullptr;
    this->dlcMenu = nullptr;
    this->activeMission = 0;
    this->radioMessages = nullptr;
    this->flag_0xdc = 0;
    this->flag_0xdd = 0;
    this->flag_0x6e = 0;
    this->pendingHangarClose = 0;
    this->flag_0x148 = 0;
    this->choiceWindow = nullptr;
    this->spaceLounge = nullptr;
    this->hangarWindow = nullptr;
    this->statusWindow = nullptr;
    this->m_pDialogueWindow = nullptr;
    this->dialogueWindow = nullptr;
    this->medalChoiceWindow = nullptr;
    this->buttonRow = nullptr;
    this->buttonLaunch = nullptr;
    this->buttonCredits = nullptr;
    this->scrollBox = nullptr;
    this->dialogueMission = nullptr;

    Matrix nearKey;
    int race;
    if (Globals::status->getStation()->getIndex() == 101) {
        race = 8;
    } else if (Globals::status->getStation()->getIndex() == 100) {
        race = 7;
    } else {
        race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
    }

    this->camKeyX = (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3];
    this->camKeyY = (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3 + 1];
    this->camKeyZ = (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3 + 2];
    MatrixSetTranslation(nearKey, this->camKeyX, this->camKeyY, this->camKeyZ);
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    MatrixSetRotation(nearKey, gHangarCamPitch[race], yawTable[race], 0.0f, (AbyssEngine::AEMath::RotationOrder) 2);
    Matrix farKey;
    MatrixSetTranslation(farKey, (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3],
                         (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3 + 1],
                         (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3 + 2]);
    MatrixSetRotation(farKey, gHangarCamPitch[race], (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
    this->cameraTween = new AbyssEngine::EaseInOutMatrix(nearKey, farKey, 3000);
    Globals::Canvas->CameraSetLocal(Globals::Canvas->CameraGetCurrent(), nearKey);

    this->easeX = new AbyssEngine::EaseInOut(this->camKeyX, this->camKeyX);
    this->easeY = new AbyssEngine::EaseInOut(this->camKeyY, this->camKeyY);
    this->easeZ = new AbyssEngine::EaseInOut(this->camKeyZ, this->camKeyZ);
}

ModStation::~ModStation() {
    this->OnRelease();
}

int ModStation::OnInitialize() {
    this->dialogueMission = nullptr;
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
                                                                  gStoryFaces[60]);
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
                                               gStoryFaces[61], 4, 0, 108, 0);
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
            this->flag_0x148 = 0;
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

void ModStation::OnTouchBegin(int x, int y, void *touch) { // lint: void_ptr
    // lint: void_ptr
    if (this->activeTouch != 0)
        return;
    this->activeTouch = touch;
    this->flag_0x118 = 1;
    int flag = this->flag_0x60;
    this->touchX = x;
    this->touchY = y;
    if (flag != 0) {
        if (((Radio *) (intptr_t) this->activeMission)->lastMessageShown() != 0) {
            (unsigned char &) this->stationActive = 0;
            Globals::status->nextCampaignMission(true);
            Globals::switch_to_target_setting = 0;
            Globals::appManager->SetCurrentApplicationModule(5);
            this->flag_0x60 = 0;
        }
        return;
    }
    if (Globals::layout->choiceWindowOpen != 0) {
        Globals::layout->OnTouchBegin(x, y);
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
    Globals::layout->OnTouchBegin(x, y);
    if (((NewsTicker *) (this->newsTicker))->OnTouchBegin(x, y) != 0)
        return;
    if (y <= Globals::layout->field_0xc_leftMargin)
        return;
    if (Globals::h - Globals::layout->field_0x10_rightMargin <= y)
        return;
    if (x <= this->infoBoxW)
        return;
    this->field_0xfc = x;
    this->camScrollPos = x;
    this->camScrollVel = 0;
    this->flag_0x100 = 1;
}

void ModStation::OnTouchMove(int x, int y, void *touch) { // lint: void_ptr
    // lint: void_ptr
    if (this->activeTouch != touch)
        return;
    this->touchX = x;
    this->touchY = y;
    if (this->flag_0x60 != 0)
        return;

    if (Globals::layout->choiceWindowOpen != 0) {
        Globals::layout->OnTouchMove(x, y);
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
        ((MenuTouchWindow *) (this->dlcMenu))->OnTouchMove(x, y, touch);
        return;
    }
    if (this->flag_0x61 == 0)
        return;
    ((TouchButton *) (this->buttonLaunch))->OnTouchMove(x, y);
    ((TouchButton *) (this->buttonCredits))->OnTouchMove(x, y);
    for (int i = 0; i != 5; i++)
        (((TouchButton **) (intptr_t) this->buttonRow[1])[i])->OnTouchMove(x, y);
    Globals::layout->OnTouchMove(x, y);
    if (((NewsTicker *) (this->newsTicker))->OnTouchMove(x, y) != 0)
        return;
    if (y <= Globals::layout->field_0xc_leftMargin)
        return;
    if (Globals::h - Globals::layout->field_0x10_rightMargin <= y)
        return;
    if (x <= this->infoBoxW)
        return;
    int delta = x - this->camScrollPos;
    this->camScrollVel = delta;
    this->field_0xf4 = 0x3f800000;
    this->camAngle = this->camAngle + delta;
    this->camScrollPos = x;
}

void ModStation::OnTouchEnd(int x, int y, void *touch) { // lint: void_ptr
    if (this->activeTouch != touch) {
        return;
    }
    this->activeTouch = nullptr;
    this->flag_0x118 = 0;
    if (this->flag_0x60) {
        return;
    }
    if (Globals::layout->choiceWindowOpen) {
        if (Globals::layout->OnTouchEnd(x, y)) {
            Globals::layout->choiceWindowOpen = 0;
        }
        return;
    }

    if (this->flag_0x69) {
        if (this->dialogueWindow->OnTouchEnd(x, y)) {
            if (this->dialogueMission == nullptr) {
                // Greeting dialogue closed.
                this->flag_0x69 = 0;
                delete this->dialogueWindow;
                this->dialogueWindow = nullptr;
                if (this->flag_0x101) {
                    return;
                }
                if (Globals::status->getStation()->getIndex() != 4 &&
                    Globals::status->getStation()->getIndex() != 88 &&
                    Globals::status->getStation()->stationHasPirateBase()) {
                    // Pirates send the player away.
                    this->flag_0xb2 = 0;
                    Globals::status->departStation(Globals::status->getStation());
                    Globals::achievements->resetNewMedals();
                    Globals::switch_to_target_setting = Globals::status->getCurrentCampaignMission() == 16 ? -1 : 1;
                    Globals::appManager->SetCurrentApplicationModule(2);
                    this->stationActive = 0;
                }
                return;
            }

            // Mission delivered: settle the cargo.
            int type = this->dialogueMission->getType();
            bool campaignMission = this->dialogueMission->isCampaignMission();
            if (type == 8) {
                if (!campaignMission) {
                    Globals::status->getShip()->removeCargo(this->dialogueMission->getProductionGoodIndex(),
                                                            this->dialogueMission->getProductionGoodAmount());
                    if (this->hangarWindow != nullptr) {
                        this->hangarWindow->initialize();
                    }
                }
            } else if (!campaignMission && this->dialogueMission->getType() == 11) {
                Globals::status->setPassengers(0);
                if (!this->dialogueMission->isCampaignMission()) {
                    Globals::status->field_b8 += this->dialogueMission->getProductionGoodAmount();
                }
                Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                if (cargo != nullptr) {
                    for (unsigned i = 0; i < cargo->size(); i++) {
                        if ((*cargo)[i]->isUnsaleable() &&
                            ((*cargo)[i]->getIndex() == 116 || (*cargo)[i]->getIndex() == 117)) {
                            Globals::status->getShip()->removeCargo((*cargo)[i]);
                            if (this->hangarWindow != nullptr) {
                                this->hangarWindow->initialize();
                            }
                            break;
                        }
                    }
                }
            } else if (this->dialogueMission->getType() == 3 || this->dialogueMission->getType() == 5 || this->dialogueMission->getType() == 11) {
            } else if (this->dialogueMission->getType() == 0) {
                Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                if (cargo != nullptr) {
                    for (unsigned i = 0; i < cargo->size(); i++) {
                        if ((*cargo)[i]->isUnsaleable() &&
                            ((*cargo)[i]->getIndex() == 116 || (*cargo)[i]->getIndex() == 117)) {
                            Globals::status->getShip()->removeCargo((*cargo)[i]);
                            if (this->hangarWindow != nullptr) {
                                this->hangarWindow->initialize();
                            }
                            break;
                        }
                    }
                }
                Globals::status->field_9c += this->dialogueMission->getProductionGoodAmount();
            } else if (this->dialogueMission->getType() == 14) {
                Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                if (cargo != nullptr) {
                    for (unsigned i = 0; i < cargo->size(); i++) {
                        if ((*cargo)[i]->getIndex() == 115) {
                            Globals::status->getShip()->removeCargo((*cargo)[i]);
                            if (this->hangarWindow != nullptr) {
                                this->hangarWindow->initialize();
                            }
                            break;
                        }
                    }
                }
            }

            if (this->dialogueMission->isCampaignMission()) {
                if (Globals::status->getCurrentCampaignMission() == 43) {
                    // Valkyrie intro: radio chatter over the hangar.
                    Globals::status->removeMission(this->dialogueMission);
                    Globals::status->setMission(Mission::empty);
                    this->flag_0x69 = 0;
                    Globals::sound->stop(Globals::sound->currentMusicEvent);
                    Globals::sound->play(144, nullptr, nullptr, 0.0f);
                    this->activeMission = (int) (intptr_t) new Radio();
                    Array<RadioMessage *> *messages = new Array<RadioMessage *>();
                    this->radioMessages = reinterpret_cast<Array<Radio *> *>(messages);
                    ArraySetLength<RadioMessage *>(4, *messages);
                    (*reinterpret_cast<Array<RadioMessage *> *>(this->radioMessages))[0] =
                        new RadioMessage(2071, 1, 5, 4000);
                    (*reinterpret_cast<Array<RadioMessage *> *>(this->radioMessages))[1] =
                        new RadioMessage(2072, 1, 6, 0);
                    (*reinterpret_cast<Array<RadioMessage *> *>(this->radioMessages))[2] =
                        new RadioMessage(2073, 1, 6, 1);
                    (*reinterpret_cast<Array<RadioMessage *> *>(this->radioMessages))[3] =
                        new RadioMessage(2074, 1, 6, 2);
                    ((Radio *) (intptr_t) this->activeMission)
                        ->setMessages(reinterpret_cast<Array<RadioMessage *> *>(this->radioMessages));
                    Globals::Canvas->Image2DCreate(7002, reinterpret_cast<unsigned &>(this->field_0x5c));
                    Vector &textPos = reinterpret_cast<Vector &>(this->idleBox);
                    textPos = Vector{50.0f,
                                     (float) (Globals::h + Globals::Canvas->GetImage2DHeight(this->field_0x5c) +
                                              Globals::h / 2 + 10),
                                     0.0f};
                    this->scrollBox = new ScrollTouchBox((int) textPos.x, (int) textPos.y,
                                                         (int) ((float) Globals::w - (textPos.x + textPos.x)), 5000);
                    this->scrollBox->setText(String(*Globals::gameText->getText(48), false));
                    this->scrollBox->setTextCentered(true);
                    this->field_0xb4 = -12000;
                    this->field_0xb8 = 0;
                    this->flag_0x60 = 1;
                    return;
                }
                if (Globals::status->getCurrentCampaignMission() != 148) {
                    Globals::status->nextCampaignMission(true);
                }
                int next = Globals::status->getCurrentCampaignMission();
                if (next == 48 || next == 49 || next == 55 || next == 56 || next == 58) {
                    this->cutScene->replacePlayerShip(Globals::status->getShip()->getIndex(),
                                                      Globals::status->getShip()->getRace());
                } else if (next == 45) {
                    Globals::status->removeMission(this->dialogueMission);
                    this->dialogueMission = nullptr;
                    Globals::status->setMission(Mission::empty);
                    this->selectedButton = 1;
                    this->flag_0x69 = 0;
                    OnKeyPress(0x10000, 0);
                    return;
                } else if (next == 18) {
                    Globals::status->removeMission(this->dialogueMission);
                    this->flag_0x69 = 0;
                    this->dialogueMission = nullptr;
                    return;
                } else if (next == 9 || next == 44 || next == 75 || next == 76 || next == 83) {
                    Globals::status->removeMission(this->dialogueMission);
                    this->dialogueMission = nullptr;
                    Globals::status->setMission(Mission::empty);
                    this->flag_0x69 = 0;
                    Globals::switch_to_target_setting = 0;
                    Globals::appManager->SetCurrentApplicationModule(5);
                    return;
                } else {
                        if (next == 77 && Globals::status->getStation()->getIndex() == 100) {
                            Globals::status->getStation()->addShip((*Globals::ships)[37]->makeShip(0));
                        } else if (next == 78) {
                            Globals::status->field_5c = -1;
                            Globals::status->field_60 = -1;
                            Globals::status->field_64 = -1;
                            Globals::status->field_68 = -1;
                            Globals::achievements->resetNewMedals();
                            Globals::status->departStation(Globals::status->getStation());
                            Globals::switch_to_target_setting = 1;
                            Globals::appManager->SetCurrentApplicationModule(2);
                            this->stationActive = 0;
                        } else if (next == 84 && Globals::status->getStation()->getIndex() == 100) {
                            Station *station = Globals::status->getStation();
                            if (!station->hasShip(38)) {
                                station->addShip((*Globals::ships)[38]->makeShip(-1));
                            }
                            if (!station->hasShip(40)) {
                                station->addShip((*Globals::ships)[40]->makeShip(-1));
                            }
                            Globals::status->getShip()->addCargo((*Globals::items)[137]->makeItem(1));
                        } else if (next == 160) {
                            Level::initStreamOutPosition = 0;
                            Globals::switch_to_target_setting = 1;
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
                            this->applicationManager->SetCurrentApplicationModule(2);
                            return;
                        } else if (next == 99 || next == 119) {
                            Level::initStreamOutPosition = 1;
                            Globals::switch_to_target_setting = 1;
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
                            Globals::appManager->SetCurrentApplicationModule(2);
                            this->stationActive = 0;
                            return;
                        } else if (next == 104) {
                            delete this->hangarWindow;
                            this->flag_0x66 = 0;
                            this->hangarWindow = nullptr;
                        } else if (next == 109) {
                            Level::initStreamOutPosition = 1;
                            Globals::switch_to_target_setting = 1;
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(114));
                            Globals::appManager->SetCurrentApplicationModule(2);
                            this->stationActive = 0;
                            return;
                        } else if (next == 128) {
                            Globals::status->activateNewWanted();
                            if (!Globals::hints[42]) {
                                this->flag_0x6e = 1;
                            }
                        } else if (next == 133) {
                            Level::initStreamOutPosition = 1;
                            Globals::switch_to_target_setting = 1;
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(120));
                            Globals::appManager->SetCurrentApplicationModule(2);
                            this->stationActive = 0;
                            return;
                        } else if (next == 144) {
                            Level::initStreamOutPosition = 0;
                            Globals::switch_to_target_setting = 1;
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(112));
                            this->applicationManager->SetCurrentApplicationModule(2);
                            this->stationActive = 0;
                            return;
                        } else if (next == 89) {
                            Level::initStreamOutPosition = 1;
                            Globals::switch_to_target_setting = 1;
                            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(109));
                            Globals::appManager->SetCurrentApplicationModule(2);
                            this->stationActive = 0;
                            return;
                        }
                }
            } else {
                Globals::status->incMissionCount();
                Globals::layout->showMissionRewardMessage(this->dialogueMission->getReward() + this->dialogueMission->getBonus(), false);
            }
            unsigned reward = this->dialogueMission->getReward() + this->dialogueMission->getBonus();
            if (reward > 1000000) {
                unsigned slot = Globals::status->getCurrentCampaignMission() - 55;
                reward = slot <= 6 ? gCampaignRewardOverride[slot] : 6666;
            }
            Globals::status->changeCredits(reward);
            Globals::status->removeMission(this->dialogueMission);
            this->dialogueMission = nullptr;
            if (this->spaceLounge != nullptr) {
                this->spaceLounge->refresh();
            }
            this->flag_0x69 = 0;
        }
        if (this->flag_0x65) {
            return;
        }
    }

    if (this->flag_0x63) {
        switch (this->choiceWindow->OnTouchEnd(x, y)) {
            case 0:
                if (this->flag_0xb2) {
                    // Leave the station.
                    this->flag_0xb2 = 0;
                    if (Globals::status->getCurrentCampaignMission() == 24 &&
                        Globals::status->getStation()->getIndex() == 10) {
                        for (unsigned i = 0; i < Globals::status->getStation()->getAgents()->size(); i++) {
                            Agent *agent = (*Globals::status->getStation()->getAgents())[i];
                            if (agent->getOffer() == 2 && agent->getSellItemIndex() == 68) {
                                agent->setEvent(1);
                                agent->setOfferAccepted(true);
                            }
                        }
                    }
                    if (Globals::status->getCurrentCampaignMission() == 48) {
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(58));
                        Level::initStreamOutPosition = 1;
                    } else {
                        Globals::status->departStation(Globals::status->getStation());
                    }
                    Globals::status->field_5c = -1;
                    Globals::status->field_60 = -1;
                    Globals::status->field_64 = -1;
                    Globals::status->field_68 = -1;
                    Globals::achievements->resetNewMedals();
                    Globals::switch_to_target_setting = 1;
                    Globals::appManager->SetCurrentApplicationModule(2);
                    this->stationActive = 0;
                    return;
                }
                if (this->flag_0x6b) {
                    // Landing fee.
                    if (Globals::status->getCredits() < this->field_0xd4) {
                        String text(*Globals::gameText->getText(203), false);
                        text = Globals::status->replaceHash(
                            String(text, false),
                            Globals::layout->formatCredits(this->field_0xd4 - Globals::status->getCredits()),
                            String("#C"));
                        this->choiceWindow->set(text, false);
                        this->flag_0xb2 = 1;
                        this->flag_0x6b = 1;
                        this->flag_0x63 = 1;
                        return;
                    }
                    Globals::status->changeCredits(-this->field_0xd4);
                    this->flag_0x6b = 0;
                    this->flag_0xd8 = 1;
                    Globals::status->getStation()->setAttackedFriends(false);
                    this->flag_0x101 = 1;
                    enterStation();
                    autosave();
                }
                if (this->flag_0xdc) {
                    this->flag_0xdc = 0;
                    this->flag_0x63 = 0;
                    reinterpret_cast<uint8_t *>(Globals::appManager->GetApplicationData())[13] = 1;
                    NFC::rateGame();
                    return;
                }
                if (this->flag_0x6c) {
                    // Sold the Kaamo artefacts.
                    Globals::status->changeCredits(-30000000);
                    Globals::status->getShip()->removeCargo(109, 50);
                    Globals::status->field_114 = 3;
                    Globals::recordHandler->saveOptions();
                    this->choiceWindow->set(*Globals::gameText->getText(485));
                    Globals::status->getStation()->setItems(nullptr, false);
                    Globals::status->voidStation->setItems(nullptr, false);
                    this->flag_0x6c = 0;
                    return;
                }
                if (this->flag_0x6d) {
                    // Emergency jump to the next system.
                    if (Globals::status->getCredits() < 25000) {
                        String text(*Globals::gameText->getText(203), false);
                        text = Globals::status->replaceHash(
                            String(text, false), Globals::layout->formatCredits(25000 - Globals::status->getCredits()),
                            String("#C"));
                        this->choiceWindow->set(text, false);
                        this->flag_0x6d = 0;
                        return;
                    }
                    Globals::status->changeCredits(-25000);
                    this->flag_0x63 = 0;
                    this->flag_0x6d = 0;
                    Globals::status->setStation((Station *) (intptr_t) Globals::galaxy->getStation(70));
                    Globals::switch_to_target_setting = 0;
                    Globals::appManager->SetCurrentApplicationModule(5);
                }
                if (this->pendingHangarClose && this->flag_0x66) {
                    this->pendingHangarClose = 0;
                    this->flag_0x66 = 0;
                }
                this->flag_0x63 = 0;
                return;
            case 1:
                this->flag_0xb2 = 0;
                if (!this->flag_0x6b) {
                    this->flag_0xdc = 0;
                    this->flag_0x63 = 0;
                    return;
                }
                this->flag_0xb2 = 0;
                Globals::status->departStation(Globals::status->getStation());
                Globals::achievements->resetNewMedals();
                Globals::switch_to_target_setting = 1;
                Globals::appManager->SetCurrentApplicationModule(2);
                this->stationActive = 0;
                return;
            default:
                return;
        }
    }

    if (this->flag_0x6a) {
        if (!this->medalChoiceWindow->OnTouchEnd(x, y)) {
            checkMedals();
        }
        return;
    }

    if (this->flag_0x66) {
        if (!this->hangarWindow->OnTouchEnd(x, y)) {
            return;
        }
        int minedOre = 0;
        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
        if (cargo != nullptr) {
            for (unsigned i = 0; i < cargo->size(); i++) {
                if ((*cargo)[i]->getIndex() >= 132 && (*cargo)[i]->getIndex() <= 153) {
                    minedOre += (*cargo)[i]->getAmount();
                }
            }
        }
        if (minedOre > this->field_0xd0) {
            Globals::status->field_a8 += minedOre - this->field_0xd0;
        }
        this->hangarWindow->setSellMode(false);
        resetIdleCamForHangar();
        this->flag_0x66 = 0;
        if (this->hangarWindow->field_0x0) {
            this->hangarWindow->field_0x0 = 0;
            this->cutScene->replacePlayerShip(Globals::status->getShip()->getIndex(),
                                              Globals::status->getShip()->getRace());
        }
        if (this->cutScene != nullptr) {
            this->cutScene->checkForTurret();
        }
        Globals::sound->stop(95);
        Globals::sound->play(122, nullptr, nullptr, 0.0f);
        Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.0f);
        Array<TouchButton *> *row = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
        for (unsigned i = 0; i < reinterpret_cast<Array<TouchButton *> *>(this->buttonRow)->size(); i++) {
            if (i < 10) {
                Globals::sub_menu_buttons_x[i] = (int) (*row)[i]->getPosition().x;
                Globals::sub_menu_buttons_y[i] =
                    (int) (*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[i]->getPosition().y;
                row = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
            }
        }
        Globals::sub_menu_button_count = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow)->size();
        return;
    }

    if (this->flag_0x67) {
        if (!this->starMap->OnTouchEnd(x, y)) {
            return;
        }
        this->flag_0x67 = 0;
        resetLight();
        Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.0f);
        return;
    }

    if (this->flag_0x65) {
        if (!this->spaceLounge->OnTouchEnd(x, y)) {
            return;
        }
        this->flag_0x65 = 0;
        resetIdleCamForHangar();
        resetLight();
        Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.0f);
        Globals::sound->stop(108);
        Globals::sound->play(122, nullptr, nullptr, 0.0f);
        this->cutScene->checkForTurret();
        if (this->spaceLounge != nullptr && this->spaceLounge->hangarNeedsUpdate()) {
            delete this->hangarWindow;
            this->hangarWindow = nullptr;
        }
        return;
    }

    if (this->flag_0x68) {
        if (!this->statusWindow->OnTouchEnd(x, y)) {
            return;
        }
        this->flag_0x68 = 0;
        Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.0f);
        return;
    }

    if (this->flag_0x64) {
        if (!((MissionsWindow *) this->m_pDialogueWindow)->OnTouchEnd(x, y)) {
            return;
        }
        this->flag_0x64 = 0;
        Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 0.0f);
        resetLight();
        return;
    }

    if (this->flag_0x62) {
        if (!this->dlcMenu->OnTouchEnd(x, y, touch)) {
            return;
        }
        this->flag_0x62 = 0;
        Array<TouchButton *> *row = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
        for (unsigned i = 0; i < reinterpret_cast<Array<TouchButton *> *>(this->buttonRow)->size(); i++) {
            if (i < 10) {
                Globals::sub_menu_buttons_x[i] = (int) (*row)[i]->getPosition().x;
                Globals::sub_menu_buttons_y[i] =
                    (int) (*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[i]->getPosition().y;
                row = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow);
            }
        }
        Globals::sub_menu_button_count = reinterpret_cast<Array<TouchButton *> *>(this->buttonRow)->size();
        return;
    }

    if (!this->flag_0x61) {
        return;
    }
    if (this->buttonLaunch->OnTouchEnd(x, y)) {
        leaveStation();
        return;
    }
    if (this->buttonCredits->OnTouchEnd(x, y)) {
        // Credits button: open the credit shop.
        Globals::options[78] = 1;
        Globals::recordHandler->saveOptions();
        if (this->hangarWindow == nullptr) {
            this->hangarWindow = new HangarWindow();
        }
        this->hangarWindow->initialize();
        this->pendingHangarClose = 1;
        this->flag_0x66 = 1;
        this->hangarWindow->showCreditsBuyWindow();
    }
    this->selectedButton = -1;
    for (int i = 0; i < 5; i++) {
        if ((*reinterpret_cast<Array<TouchButton *> *>(this->buttonRow))[i]->OnTouchEnd(x, y)) {
            this->selectedButton = i;
            OnKeyPress(0x10000, 0);
            return;
        }
    }
    if (Globals::layout->OnTouchEnd(x, y)) {
        // Menu button.
        if (this->dlcMenu == nullptr) {
            this->dlcMenu = new MenuTouchWindow(2);
        }
        Globals::status->checkForLevelUp();
        this->flag_0x62 = 1;
        for (unsigned i = 0; i < this->dlcMenu->buttons->size(); i++) {
            if (i < 10) {
                Globals::sub_menu_buttons_x[i] = (int) (*this->dlcMenu->buttons)[i]->getPosition().x;
                Globals::sub_menu_buttons_y[i] = (int) (*this->dlcMenu->buttons)[i]->getPosition().y;
            }
        }
        Globals::sub_menu_button_count = this->dlcMenu->buttons->size();
        return;
    }
    if (Globals::layout->helpPressed()) {
        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(644), false));
    }
    if (this->newsTicker->OnTouchEnd(x, y)) {
        return;
    }
    // Release of the hangar rotation drag: keep spinning with inertia.
    int velocity = this->camScrollVel;
    reinterpret_cast<float &>(this->field_0xf4) = 0.9f;
    this->flag_0x100 = 0;
    this->camAngle += velocity;
    this->field_0xec = this->camAngle;
    reinterpret_cast<float &>(this->field_0xf8) = (velocity < 0 ? -velocity : velocity) > 3 ? (float) velocity : 0.0f;
}

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
    this->flag_0x63 = 1;
    this->flag_0xb2 = 1;
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

void ModStation::OnKeyPress(long long key, long long) {
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
        if (key == 0x4000) {
            this->buttonState[this->selectedButton] = 0;
            this->selectedButton = this->selectedButton > 0 ? this->selectedButton - 1 : 4;
            this->buttonState[this->selectedButton] = 1;
            return;
        } else if (key == 0x8000) {
            this->buttonState[this->selectedButton] = 0;
            this->selectedButton = this->selectedButton < 4 ? this->selectedButton + 1 : 0;
            this->buttonState[this->selectedButton] = 1;
            return;
        } else if (key == 0x20000) {
            return;
        } else if (key == 0x10000) {
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
                                                                      gStoryFaces[60]);
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

void ModStation::OnUpdate() {
    int delta = ((int) this->applicationManager->GetElapsedTimeMillis() > 150
                     ? 150
                     : (int) this->applicationManager->GetElapsedTimeMillis()) < 0
                    ? 0
                    : ((int) this->applicationManager->GetElapsedTimeMillis() > 150
                           ? 150
                           : (int) this->applicationManager->GetElapsedTimeMillis());
    this->dt = delta;
    this->accumTime += delta;
    Globals::sound->updateAll(nullptr, nullptr, nullptr, nullptr);
    Globals::layout->update(this->dt);
    if (!this->flag_0x62) {
        Globals::status->incPlayingTime(this->dt);
    }
    this->buttonCredits->setText(Globals::layout->formatCredits(Globals::status->getCredits()));
    Globals::achievements->updateCredits(Globals::status->getCredits());

    if (this->stationActive && Globals::enterSpaceLounge) {
        // Straight into the space lounge.
        this->flag_0xd8 = 1;
        Globals::enterSpaceLounge = 0;
        if (this->spaceLounge != nullptr) {
            this->spaceLounge->init();
        } else {
            this->spaceLounge = new SpaceLounge();
        }
        Globals::sound->setParamValue(0, Globals::sound->currentMusicEvent, 2.0f);
        Globals::sound->stop(122);
        Globals::sound->play(108, nullptr, nullptr, 0.0f);
        this->flag_0x65 = 1;
        return;
    }

    AppData *appData = (AppData *) Globals::appManager->GetApplicationData();
    if (appData->restoreNotice) {
        this->choiceWindow->set(*Globals::gameText->getText(100));
        this->flag_0x63 = 1;
        appData->restoreNotice = 0;
    }
    if (appData->purchaseReady && appData->purchaseList != nullptr && *appData->purchaseList != 0 &&
        (unsigned) (appData->purchaseId - 50) <= 4) {
        // In-app purchase completed: hand out the credits.
        int credits = (*reinterpret_cast<Array<int> *>(appData->iapArray))[
            Globals::globals->getInAppPurchaseArrayIndex(appData->purchaseId, appData->productIds)];
        if (credits >= 0) {
            Globals::status->changeCredits(credits);
            autosave();
            this->choiceWindow->set(*Globals::gameText->getText(125));
            this->flag_0x63 = 1;
            appData->purchaseReady = 0;
            if (this->hangarWindow != nullptr) {
                this->hangarWindow->hideMessage();
            }
            delete this->dlcMenu;
            this->dlcMenu = nullptr;
        }
    }

    if (!this->flag_0x64 && !this->flag_0x65 && !this->flag_0x66 && !this->flag_0x67 && !this->flag_0x68 &&
        !this->flag_0x60) {
        if (this->newsTicker != nullptr) {
            this->newsTicker->update(this->dt);
        }
        if (this->cutScene != nullptr) {
            this->cutScene->process(0);
        }
        // Idle camera drift around the hangar.
        uint8_t *flipAxis = &this->_pad_0x12d[0];
        int step = this->dt;
        int backStep = -this->dt;
        this->easeX->Increase((float) (flipAxis[1] ? backStep : step));
        float camX = this->easeX->GetValue();
        if ((camX - this->easeX->GetMaxValue() > 0.0f ? camX - this->easeX->GetMaxValue()
                                                      : -(camX - this->easeX->GetMaxValue())) <= 20.0f) {
            bool back = flipAxis[1];
            flipAxis[1] = !back;
            int offset = Globals::rnd->nextInt(131);
            this->easeX->SetRange(camX, this->camKeyX + (float) (back ? offset + 18 : -18 - offset));
        }
        this->easeY->Increase((float) (flipAxis[0] ? backStep : step));
        float camY = this->easeY->GetValue();
        if ((camY - this->easeY->GetMaxValue() > 0.0f ? camY - this->easeY->GetMaxValue()
                                                      : -(camY - this->easeY->GetMaxValue())) <= 20.0f) {
            bool back = flipAxis[0];
            flipAxis[0] = !back;
            int offset = Globals::rnd->nextInt(120);
            this->easeY->SetRange(camY, this->camKeyY + (float) (back ? offset + 30 : -30 - offset));
        }
        this->easeZ->Increase((float) (flipAxis[2] ? backStep : step));
        float camZ = this->easeZ->GetValue();
        if ((camZ - this->easeZ->GetMaxValue() > 0.0f ? camZ - this->easeZ->GetMaxValue()
                                                      : -(camZ - this->easeZ->GetMaxValue())) <= 20.0f) {
            bool back = flipAxis[2];
            flipAxis[2] = !back;
            int offset = Globals::rnd->nextInt(100);
            this->easeZ->SetRange(camZ, this->camKeyZ + (float) (back ? offset + 50 : -50 - offset));
        }
        AbyssEngine::AEMath::MatrixSetTranslation(
            *reinterpret_cast<AbyssEngine::AEMath::Matrix *>(
                Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent())),
            camX, camY, camZ);
    }

    if (this->accumTime < 1000) {
        return;
    }

    if (this->flag_0x60) {
        // Valkyrie intro sequence.
        if (this->field_0xb4 < -6000 && this->field_0xb4 + this->dt > -6001) {
            delete this->cutScene;
            this->cutScene = nullptr;
            this->cutScene = new CutScene(2);
            this->cutScene->initialize();
            this->cutScene->rotationSpeed = 3e-05f;
            this->flag_0x60 = 1;
        }
        this->field_0xb4 += this->dt;
        this->cutScene->update(this->dt);
        if (((Radio *) (intptr_t) this->activeMission)->lastMessageShown()) {
            Vector &textPos = reinterpret_cast<Vector &>(this->idleBox);
            float textY = textPos.y;
            if (textY > (float) (Globals::h / 2)) {
                float scrolled = textY + (float) this->dt * -0.03f;
                if (scrolled <= (float) (Globals::h / 2)) {
                    textPos.y = scrolled;
                    this->field_0xb8 = 4000;
                    textY = scrolled;
                }
            }
            if (this->field_0xb8 > 0) {
                this->field_0xb8 -= this->dt;
            } else {
                textY += (float) this->dt * -0.03f;
                textPos.y = textY;
                this->scrollBox->setPosition(
                    (int) textPos.x,
                    (int) (textY + (float) Globals::Canvas->GetImage2DHeight(this->field_0x5c) +
                           (float) (Globals::h / 2)));
            }
        }
        if (this->field_0xb4 >= 136001) {
            this->stationActive = 0;
            Globals::status->nextCampaignMission(true);
            Globals::switch_to_target_setting = 0;
            Globals::appManager->SetCurrentApplicationModule(5);
            this->flag_0x60 = 0;
        }
        return;
    }

    if (this->flag_0x69) {
        this->dialogueWindow->update(this->dt);
    } else if (this->flag_0x67) {
        this->starMap->update(this->dt);
    } else if (this->flag_0x66) {
        this->hangarWindow->update(this->dt);
    } else if (this->flag_0x64) {
        ((MissionsWindow *) this->m_pDialogueWindow)->update(this->dt);
    } else if (this->flag_0x65) {
        this->spaceLounge->update(this->dt);
    } else if (this->flag_0x68) {
        this->statusWindow->update(this->dt);
    }
    if (this->flag_0x63) {
        this->choiceWindow->update(this->dt);
    }
    if (this->flag_0x62) {
        this->dlcMenu->update(this->dt);
    }

    if (!this->flag_0x63 && !this->flag_0x69 && !this->flag_0xb0) {
        if (!this->flag_0xda) {
            checkPendingProducts();
            this->flag_0xda = 1;
        }
        if (!this->flag_0xd9) {
            checkMedals();
            this->flag_0xd9 = 1;
        }
        if (!this->flag_0x6a && this->flag_0x6e && !Globals::hints[42]) {
            this->choiceWindow->set(*Globals::gameText->getText(601));
            this->flag_0x63 = 1;
            Globals::hints[42] = 1;
            this->flag_0x6e = 0;
        }

        bool reachedBeacon = false;
        Mission *completed = Globals::status->missionCompleted(
            true, this->flag_0x65 ? this->spaceLounge->introFinished() : false, 0);
        if (completed == nullptr && Globals::status->getCurrentCampaignMission() == 116 && this->flag_0x65 &&
            this->spaceLounge->introFinished() &&
            ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 18 &&
            !(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() &
              (1 << (Globals::status->getStation()->getIndex() - 90)))) {
            // Mission 116: one of the beacon stations visited.
            ((Mission *) (intptr_t) Globals::status->getCampaignMission())
                ->setStatusValue(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() |
                                 (1 << (Globals::status->getStation()->getIndex() - 90)));
            reachedBeacon = true;
        }

        if (Globals::status->getCurrentCampaignMission() >= 148 &&
            Globals::status->getCurrentCampaignMission() <= 151) {
            int stationIndex = Globals::status->getStation()->getIndex();
            int visitBit;
            switch (stationIndex) {
            case 66:
                visitBit = 2;
                break;
            case 9:
                visitBit = 4;
                break;
            case 55:
                visitBit = 1;
                break;
            default:
                visitBit = 0;
                break;
            }
            if (completed != nullptr) {
                if (this->flag_0x65 && this->spaceLounge->introFinished() &&
                    Globals::status->getCurrentCampaignMission() >= 148 && stationIndex == 96 &&
                    Globals::status->getCurrentCampaignMission() <= 150) {
                    Globals::status->setCurrentCampaignMission(151);
                    this->dialogueWindow = new DialogueWindow();
                    this->dialogueMission = completed;
                    this->dialogueWindow->set(completed, 1, 151);
                    this->flag_0x69 = 1;
                    checkHints();
                    goto afterMissions;
                }
            } else if (this->flag_0x65 && this->spaceLounge->introFinished() &&
                       Globals::status->getCurrentCampaignMission() >= 148 &&
                       Globals::status->getCurrentCampaignMission() <= 150 &&
                       (stationIndex == 9 || stationIndex == 55 || stationIndex == 66) &&
                       !(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() & visitBit)) {
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())
                    ->setStatusValue(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() |
                                     visitBit);
                int here = Globals::status->getStation()->getIndex();
                int textId = here == 66 ? 149 : 148;
                if (here == 9) {
                    textId = 150;
                }
                this->dialogueWindow = new DialogueWindow();
                Mission *briefing = new Mission(160, 0, -1);
                briefing->setCampaignMission(true);
                this->dialogueWindow->set(briefing, 1, textId);
                this->flag_0x69 = 1;
                checkHints();
                goto afterMissions;
            }
        }

        if (reachedBeacon) {
            int beaconStation = Globals::status->getStation()->getIndex();
            int beacon = beaconStation == 94 ? 3 : beaconStation - 90;
            Globals::sound->play(1561 + beacon, nullptr, nullptr, 0.0f);
            bool firstVoice = beacon == 0 || beacon == 2;
            int textId = 2716 + beacon;
            int face = firstVoice ? 0 : (textId == 2717 ? 57 : 58);
            this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(textId),
                                                      Globals::gameText->getText(firstVoice ? 1597 : 1516),
                                                      gStoryFaceTable[face]);
            this->flag_0x69 = 1;
        } else if (completed != nullptr) {
            this->dialogueMission = completed;
            this->dialogueWindow = new DialogueWindow(completed, nullptr, 1);
            this->flag_0x69 = 1;
            if (completed->getType() == 13) {
                reinterpret_cast<uint16_t &>(Globals::status->field_0xf0) = 0;
                autosave();
            }
            if (this->spaceLounge != nullptr) {
                this->spaceLounge->setHangarUpdate(true);
            }
        } else {
            Mission *failed = (Mission *) (intptr_t) Globals::status->missionFailed(true, 0);
            if (failed != nullptr) {
                this->dialogueWindow = new DialogueWindow(failed, nullptr, 2);
                this->flag_0x69 = 1;
                Globals::status->removeMission(failed);
                if (failed->getType() == 13) {
                    reinterpret_cast<uint16_t &>(Globals::status->field_0xf0) = 0;
                    autosave();
                }
            }
        }
        checkHints();
    }
afterMissions:

    if (!this->flag_0x100) {
        // Hangar rotation inertia.
        float factor = (float) this->dt * 0.05f;
        factor = factor < 1.0f ? factor : 1.0f;
        float &spin = reinterpret_cast<float &>(this->field_0xf8);
        spin = spin * (reinterpret_cast<float &>(this->field_0xf4) * factor);
        if ((spin > 0.0f ? spin : -spin) > 1.0f) {
            this->camAngle = (int) (spin + (float) this->camAngle);
        }
    }
    if (this->cutScene != nullptr && !this->flag_0x60) {
        this->cutScene->cameraRotX = (float) this->camAngle / 120.0f;
    }

    if (this->flag_0x118) {
        // Debug camera controls by touching the screen edges.
        if (this->touchY < Globals::layout->field_0xc_leftMargin) {
            if (this->touchX < 100) {
                this->cutScene->rootGeom->translate(0.0f, -10.0f, 0.0f);
                this->cutScene->vec8.y += -10.0f;
            } else if (this->touchX < 200) {
                this->cutScene->rootGeom->translate(0.0f, 10.0f, 0.0f);
                this->cutScene->vec8.y += 10.0f;
            } else if (this->touchX < 300) {
                this->cutScene->rootGeom->rotate(0.01f, 0.0f, 0.0f);
                this->cutScene->vec8w += 0.01f;
            } else if (this->touchX < 400) {
                this->cutScene->rootGeom->rotate(-0.01f, 0.0f, 0.0f);
                this->cutScene->vec8w += -0.01f;
            }
        } else if (this->touchY < Globals::h - 2 * Globals::layout->field_0x10_rightMargin) {
            if (this->touchX < 70) {
                this->cutScene->rootGeom->rotate(0.0f, 0.01f, 0.0f);
                reinterpret_cast<float &>(this->cutScene->shipPosY) += 0.01f;
            } else if (this->touchX > Globals::w - 70) {
                this->cutScene->rootGeom->rotate(0.0f, -0.01f, 0.0f);
                reinterpret_cast<float &>(this->cutScene->shipPosY) += -0.01f;
            } else if (this->touchX > 100 && this->touchX < Globals::w - 100) {
                if (this->touchY < Globals::h / 2) {
                    this->cutScene->rootGeom->moveForward((float) -this->dt);
                    this->cutScene->vec8.z -= (float) this->dt;
                } else {
                    this->cutScene->rootGeom->moveForward((float) this->dt);
                    this->cutScene->vec8.z += (float) this->dt;
                }
            }
        } else if (this->touchX < 70) {
            this->cutScene->rootGeom->translate((float) -this->dt, 0.0f, 0.0f);
            this->cutScene->vec8.x -= (float) this->dt;
        } else if (this->touchX > Globals::w - 70) {
            this->cutScene->rootGeom->translate((float) this->dt, 0.0f, 0.0f);
            this->cutScene->vec8.x += (float) this->dt;
        }
    }
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

void ModStation::checkHints() {
    if (this->flag_0x60)
        return;
    if (!this->flag_0x6a && !(this->flag_0x63 | Globals::options[52]) &&
        Globals::status->getCurrentCampaignMission() >= 19) {
        this->choiceWindow->set(*Globals::gameText->getText(62), *Globals::gameText->getText(73), true,
                                *Globals::gameText->getText(126), *Globals::gameText->getText(127),
                                *Globals::gameText->getText(524), -1, -1);
        Globals::options[52] = 1;
        this->flag_0xdc = 1;
        this->flag_0x63 = 1;
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
                                                  gStoryFaces[60]);
        Globals::sound->play(589, nullptr, nullptr, 0.0f);
        this->flag_0x69 = 1;
        reinterpret_cast<uint8_t *>(&Globals::status->field_f8)[1] = 0;
        Globals::status->changeCredits(20000);
    }
}

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
        bool fading = true;
        float fade = 0.0f;
        if (time > 130000) {
            fade = (float) (time - 130000) / 6000.0f;
        } else if (time < -6000) {
            fade = (float) (time + 12000) / 6000.0f;
        } else if (time < 0) {
            fade = (float) (time + 6000) / -6000.0f + 1.0f;
        } else {
            fading = false;
        }
        if (fading) {
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

void ModStation::setGameLoaded() {
    this->flag_0xb0 = 1;
    this->flag_0xb3 = 1;
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

void ModStation::showDlcMenu() {
    MenuTouchWindow *menu = this->dlcMenu;
    if (menu == nullptr) {
        menu = new MenuTouchWindow(2);
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

void ModStation::OnResume() {
    FModSound **holder = &Globals::sound;
    FModSound *obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    (*holder)->setVolume(1, *reinterpret_cast<float *>(Globals::options));
}

void ModStation::OnSuspend() {
    this->activeTouch = 0;
    RecordHandler **holder = &Globals::recordHandler;
    RecordHandler *obj = *holder;
    if (obj != 0)
        obj->saveOptions();
}

void ModStation::OnKeyRelease(long long, long long) {
}

void ModStation::OnTouchBegin(int x, int y) {}

void ModStation::OnTouchMove(int x, int y) {}

void ModStation::OnTouchEnd(int x, int y) {}

int ModStation::ShowLoadingScreen() {
    return 1;
}
