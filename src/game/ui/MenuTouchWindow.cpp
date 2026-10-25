#include "engine/core/ArrayReleaseClasses.h"
#include "game/ui/MenuTouchWindow.h"
#include "engine/render/ImageFactory.h"
#include "game/menu/ModStation.h"
#include "engine/core/AbyssEngine.h"
#include "game/ui/TouchSlider.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Engine.h"
#include "game/mission/GameRecord.h"
#include "game/ui/Layout.h"
#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "engine/audio/FModSound.h"
#include "engine/core/ApplicationManager.h"
#include "game/ship/ShipDefTable.h"
#include "game/ship/Ship.h"
#include "game/mission/Item.h"
#include "game/mission/Mission.h"
#include "game/core/GameSettings.h"
#include "game/core/Globals.h"
#include "game/ui/ChoiceWindow.h"
#include "game/mission/Achievements.h"
#include "game/world/Galaxy.h"
#include "game/ui/MissionsWindow.h"
#include "game/ui/ScrollTouchWindow.h"
#include "engine/core/NFC.h"
#include "engine/core/GameData.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include <math.h>
#include <cstddef>

class GameText;
class FModSound;
class Achievements;
class Galaxy;
class ImageFactory;
class AppManager;
class ChoiceWindow;
class MissionsWindow;


struct MtwAppData {
    uint8_t pad_0x0[4];
    uint8_t screenshotPending;
    uint8_t screenshotResultFlag;
    uint8_t pad_0x6[2];
    int storeResultCode;
    uint8_t storeInitFlag;
    uint8_t rateRequested;
    uint8_t rated;
    uint8_t pad_0xf[0x2d];
    uint8_t dlcMessageAck;
    uint8_t dlcMenuRequestFlag;
    uint8_t pad_0x3e;
    uint8_t restoreRequested;
    uint8_t purchaseReadyFlag;
    uint8_t purchaseResultFlag;
    uint8_t purchaseErrorFlag;
    uint8_t pad_0x43[5];
    unsigned int purchaseCode;
    uint8_t dlcMenuAckFlag;
    uint8_t pad_0x4d[3];
    Array<AbyssEngine::String *> *prices;
    Array<AbyssEngine::String *> *productIds;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(MtwAppData, screenshotResultFlag) == 0x5, "appdata 0x5");
static_assert(offsetof(MtwAppData, storeResultCode) == 0x8, "appdata 0x8");
static_assert(offsetof(MtwAppData, storeInitFlag) == 0xc, "appdata 0xc");
static_assert(offsetof(MtwAppData, rateRequested) == 0xd, "appdata 0xd");
static_assert(offsetof(MtwAppData, rated) == 0xe, "appdata 0xe");
static_assert(offsetof(MtwAppData, dlcMessageAck) == 0x3c, "appdata 0x3c");
static_assert(offsetof(MtwAppData, dlcMenuRequestFlag) == 0x3d, "appdata 0x3d");
static_assert(offsetof(MtwAppData, restoreRequested) == 0x3f, "appdata 0x3f");
static_assert(offsetof(MtwAppData, purchaseReadyFlag) == 0x40, "appdata 0x40");
static_assert(offsetof(MtwAppData, purchaseResultFlag) == 0x41, "appdata 0x41");
static_assert(offsetof(MtwAppData, purchaseErrorFlag) == 0x42, "appdata 0x42");
static_assert(offsetof(MtwAppData, purchaseCode) == 0x48, "appdata 0x48");
static_assert(offsetof(MtwAppData, dlcMenuAckFlag) == 0x4c, "appdata 0x4c");
static_assert(offsetof(MtwAppData, screenshotPending) == 0x4, "appdata 0x4");
static_assert(offsetof(MtwAppData, prices) == 0x50, "appdata 0x50");
static_assert(offsetof(MtwAppData, productIds) == 0x54, "appdata 0x54");
#endif

static inline void *_mtw_GameText_getText(void *gt, int id) { return ((GameText*)gt)->getText(id); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_set(void *cw, void *s1, void *s2, bool b); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)



// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void *_mtw_Array_TB_dtor(void *arr) { ((Array<TouchButton*>*)arr)->~Array(); return arr; } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void *_mtw_Array_TS_dtor(void *arr) { ((Array<TouchSlider*>*)arr)->~Array(); return arr; } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void *_mtw_Array_Str_dtor(void *arr) { ((Array<String>*)arr)->~Array(); return arr; }

static inline void * _mtw_TouchButton_dtor(void *p) { ((TouchButton*)p)->~TouchButton(); return (void *)(p); }

static inline void * _mtw_ChoiceWindow_dtor(void *p) { ((ChoiceWindow*)p)->~ChoiceWindow(); return (void *)(p); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void * _mtw_ScrollTouchWindow_dtor(void *p) { ((ScrollTouchWindow*)p)->~ScrollTouchWindow(); return (void *)(p); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_Str_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void *_mtw_Array_StrArr_dtor(void *p) { ((Array<Array<String>*>*)p)->~Array(); return p; } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Array_StrArr_ctor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_Array_Str_ctor(void *p) { new(p) Array<String>(); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArraySetLength_StrArr(int n, void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArraySetLength_Str(int n, void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Globals_longToTimeStringNoSeconds(long long t, void *out); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_formatCredits(void *out); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_ctor7(void *btn, void *label, int a, int x, int y, char type); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_Status_resetGame() { Globals::status->resetGame(); }

// _mtw_Mission_ctor / _mtw_Status_setMission removed: status->setMission(new Mission())
// _mtw_Ship_makeShip / _mtw_Status_setShip removed: status->setShip(Globals::ships->data_[5]->makeShip(-1))
// _mtw_Ship_setRace removed: status->getShip()->setRace(0)
// _mtw_Item_makeItem / _mtw_makeItem2 removed: dead declarations (no call sites)

// _mtw_Status_setStation removed: status->setStation(station)

void _mtw_ChoiceWindow_OnTouchBegin(void *cw, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_Layout_OnTouchBegin(void *layout, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchBegin(void *btn, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchBeginXY(void *btn, int y, int x); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_TouchButton_setPosition removed: replaced with btn->setPosition(x, y)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchSlider_OnTouchBegin(void *sl, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ScrollTouchWindow_OnTouchBegin(void *w, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_MissionsWindow_OnTouchBegin(void *w, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_FModSound_play(void *snd, int id, void *pos, float v); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_idiv(int a, int b);


static inline void * _mtw_RecordHandler_dtor(void *rh) { ((RecordHandler*)rh)->~RecordHandler(); return (void *)(rh); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void * _mtw_GameRecord_dtor(void *gr) { ((GameRecord*)gr)->~GameRecord(); return (void *)(gr); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_GameRecord_load(void *gr) { ((GameRecord*)gr)->load(); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_set(void *cw, void *s, bool b); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void *_mtw_AppMgr_GetApplicationModule(void *app, int id) { return ((AbyssEngine::ApplicationManager*)app)->GetApplicationModule(id); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_TouchButton_getPosition removed: replaced with btn->getPosition() sret

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_TouchButton_setVisible removed: replaced with btn->setVisible(v)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_RecordHandler_readAllPreviewRecords(void *rh); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void *_mtw_AppMgr_GetEngine() { return GetEngine(); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_ChoiceWindow_set(cw, s) removed: replaced with cw->set(*s)

// _mtw_ChoiceWindow_update removed: replaced with cw->update(dt)

// _mtw_TouchButton_setYPosition removed: replaced with btn->setYPosition(y)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_TouchButton_isVisible removed: replaced with btn->isVisible()

void _mtw_ScrollTouchWindow_update(void *w); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_MissionsWindow_update(void *w); // lint: void_ptr (external symbol; param type is mangling-load-bearing)


// lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_DlcMenu_call removed: replaced with win->set(*s1, *s2)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_OnTouchMove(void *cw, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchMove(void *btn, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchMoveXY(void *btn, int y, int x); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchSlider_OnTouchMove(void *sl, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

float _mtw_TouchSlider_getValue(void *sl); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ScrollTouchWindow_OnTouchMove(void *w, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_MissionsWindow_OnTouchMove(void *w, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_FModSound_setVolume(void *snd, float v); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_Array_TB_ctor(void *a) { new(a) Array<TouchButton*>(); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_TouchButton_draw removed: replaced with btn->draw()

// _mtw_Layout_drawBox removed: replaced with layout->drawBox(mode, x, y, w, h, str)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ImageFactory_drawShip(void *imgF, unsigned int shipId, int x, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// g_android_link_game_gp is the Google-Play "link game" flag (JniBridge.cpp)
extern int g_android_link_game_gp;
extern int g_android_gp_is_linked;
extern int g_android_show_achievements;
extern int g_android_show_leaderboards;

// Globals::options is the raw OptionsRecord image; the menus access its fields by offset.
static inline bool &optionFlag(int offset) { return *reinterpret_cast<bool *>(&Globals::options[offset]); }

static inline float &optionFloat(int offset) { return *reinterpret_cast<float *>(&Globals::options[offset]); }

static inline int &optionInt(int offset) { return *reinterpret_cast<int *>(&Globals::options[offset]); }

void _mtw_ArrayReleaseClasses_TB(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArrayReleaseClasses_Str(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

template<class T>
static inline void freeObj(T **slot, void *(*dtor)(void *)) { // lint: void_ptr (dtor matches external void*(void*) deallocator ABI)
    // lint: void_ptr (dtor matches external void*(void*) deallocator ABI)
    T *o = *slot;
    if (o != 0) ::operator delete(dtor(o));
    *slot = 0;
}

void _mtw_ArrayReleaseClasses_Str(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArrayReleaseClasses_StrArr(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_String_op_plus(void *out, void *rhs); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_ctorFull(void *btn, void *label, int a, int x, int y, int w, // lint: void_ptr (external symbol; param type is mangling-load-bearing)
                               // lint: void_ptr (external symbol; param type is mangling-load-bearing)
                               char type, char id);

static const char gCrbDash[] = "";


typedef void (*RefreshFn)();


static const int g_mtw_upTextIds[16] = {};

#define MTW_ABS(v) ((v) > 0 ? (v) : -(v))

void _mtw_steerFromTouchId(void *self, int y, int x, void *touchId); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_OnTouchMove(void *layout, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_buildMenu(void *self, int menuType); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static const int MenuTouchWindow_languageIds[10] = {27, 28, 30, 29, 31, 32, 33, 35, 38, 39};
static const int MenuTouchWindow_languageTextIds[10] = {2, 1, 4, 3, 5, 7, 8, 14, 12, 13};
static const int MenuTouchWindow_controlImages[3][2] = {{9500, 9501}, {9504, 9505}, {9502, 9503}};

MenuTouchWindow::MenuTouchWindow(int menuType) {
    this->buttonWidth = Globals::layout->field_0x294_buttonWidth;
    this->buttonYBias = Globals::layout->field_0x298_buttonYBias;
    this->buttonRowGap = Globals::layout->field_0x29c_buttonRowGap;
    this->listRowGap = Globals::layout->field_0x2a0_listRowGap;
    this->metricA = Globals::layout->field_0x2a4_metricA;
    this->metricB = Globals::layout->field_0x2a8_metricB;
    this->metricC = Globals::layout->field_0x2ac_metricC;
    int mainButtonOffset = Globals::layout->field_0x2b0;
    this->backgroundEnabled = menuType;
    this->buttons = new Array<TouchButton *>();
    this->scrollEntries = new Array<TouchButton *>();
    this->scrollUpButton = 0;
    this->scrollExtraButton = 0;
    this->buttonsB8 = 0;
    this->buttonsB0 = 0;
    this->buttonsB4 = 0;
    this->cinematicSteerActive = 0;
    this->field_0x120 = -1;
    this->selectedRow = 0;
    this->field_0x1c4 = 0;
    this->heapBufA = 0;
    this->heapBufB = 0;
    this->field_0x1e0 = 0;
    this->scrollWindowA = 0;
    this->scrollWindowB = 0;
    this->scrollSlots = 0;
    this->field_0x230 = 0;
    this->field_0x234 = 0;
    this->cutsceneMode = 0;
    this->previewStrings0 = 0;
    this->previewStrings1 = 0;
    this->previewRecords = 0;
    this->loadPreviewRecords();

    bool showLanguages = true;
    switch (menuType) {
    case 0: {
        // Main menu.
        Globals::Canvas->Image2DCreate(7002, *(unsigned int *) &this->field_0x120);
        this->addButton(0, *Globals::gameText->getText(28), 0, this->buttons, mainButtonOffset);
        int row = 1;
        if (Globals::lastRecordWritten >= 0 && this->previewRecords != 0 &&
            Globals::lastRecordWritten < Globals::recordSlots && (*this->previewRecords)[Globals::lastRecordWritten] != 0) {
            this->addButton(11, *Globals::gameText->getText(41), 1, this->buttons, mainButtonOffset);
            row = 2;
        }
        this->addButton(1, *Globals::gameText->getText(29), row, this->buttons, mainButtonOffset);
        this->addButton(3, *Globals::gameText->getText(31), row + 1, this->buttons, mainButtonOffset);
        int next = row + 2;
        if (Globals::iPad) {
            this->addButton(25, *Globals::gameText->getText(0), row + 2, this->buttons, mainButtonOffset);
            next = row + 3;
        }
        this->addButton(4, *Globals::gameText->getText(43), next, this->buttons, mainButtonOffset);
        break;
    }
    case 2: {
        this->addButton(0, *Globals::gameText->getText(28), 0, this->buttons, 0);
        this->addButton(1, *Globals::gameText->getText(29), 1, this->buttons, 0);
        this->addButton(2, *Globals::gameText->getText(30), 2, this->buttons, 0);
        this->addButton(3, *Globals::gameText->getText(31), 3, this->buttons, 0);
        int row = 4;
        if (Globals::iPad) {
            this->addButton(25, *Globals::gameText->getText(0), 4, this->buttons, 0);
            row = 5;
        }
        this->addButton(4, *Globals::gameText->getText(43), row, this->buttons, 0);
        break;
    }
    case 1: {
        // In-game pause menu.
        Globals::Canvas->Image2DCreate(1332, *(unsigned int *) &this->field_0x120);
        this->addButton(3, *Globals::gameText->getText(31), 0, this->buttons, 0);
        int row = 1;
        if (Globals::status->getCurrentCampaignMission() >= 16 && !Globals::status->inAlienOrbit() &&
            Globals::status->getMission()->getType() != 183) {
            this->addButton(10, *Globals::gameText->getText(129), 1, this->buttons, 0);
            row = 2;
        }
        if (Globals::status->getCurrentCampaignMission() >= 2 && Globals::status->getMission()->getType() != 183) {
            this->addButton(12, *Globals::gameText->getText(166), row, this->buttons, 0);
            row++;
        }
        this->addButton(6, *Globals::gameText->getText(522), row, this->buttons, 0);
        this->addButton(19, *Globals::gameText->getText(59), row + 1, this->buttons, 0);
        int mission = Globals::status->getCurrentCampaignMission();
        if (mission == 154 || mission == 157 || mission == 158)
            this->addButton(18, *Globals::gameText->getText(395), row + 2, this->buttons, 0);
        TouchButton *quit = new TouchButton(*Globals::gameText->getText(61), 0,
                                            Globals::w - Globals::layout->field_0x2c_rowHeight,
                                            Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
        quit->id = 20;
        ArrayAdd(quit, *this->scrollEntries);
        TouchButton *save = new TouchButton(*Globals::gameText->getText(60), 0,
                                            Globals::w - Globals::layout->field_0x2c_rowHeight * 2 - quit->getWidth(),
                                            Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
        save->id = 21;
        ArrayAdd(save, *this->scrollEntries);
        save->setPosition(Globals::w - Globals::layout->field_0x2c_rowHeight,
                          Globals::h - Globals::layout->field_0x2c_rowHeight);
        showLanguages = false;
        break;
    }
    }
    if (showLanguages && Globals::layout->field_0x285) {
        TouchButton *dlc = new TouchButton(*Globals::gameText->getText(33), 0,
                                           Globals::w - Globals::layout->field_0x2c_rowHeight,
                                           Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
        dlc->id = 17;
        ArrayAdd(dlc, *this->scrollEntries);
    }
    TouchButton *back = new TouchButton(*Globals::gameText->getText(62), 0,
                                        Globals::w - Globals::layout->field_0x2c_rowHeight,
                                        Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
    back->id = 22;
    ArrayAdd(back, *this->scrollEntries);

    Vector anchor = this->buttons != 0
                        ? (*this->buttons)[this->buttons->size() - 1 >= 4 ? 4 : this->buttons->size() - 1]->getPosition()
                        : Vector();
    int leaderboardsX = (int) (anchor.x + (float) this->buttonWidth + (float) (this->buttonWidth / 3));
    TouchButton *leaderboards = new TouchButton(String("Leaderboards"), 17, leaderboardsX, (int) anchor.y, 68);
    leaderboards->id = 23;
    ArrayAdd(leaderboards, *this->scrollEntries);
    int achievementsX = (int) (anchor.x - (float) (this->buttonWidth / 3));
    TouchButton *achievements = new TouchButton(String("Achievements"), 17, achievementsX, (int) anchor.y, 68);
    achievements->id = 24;
    ArrayAdd(achievements, *this->scrollEntries);
    TouchButton *facebook = new TouchButton(*Globals::gameText->getText(5000), 0,
                                            Globals::w - Globals::layout->field_0x2c_rowHeight * 2 - back->getWidth(),
                                            Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
    facebook->id = 109;
    ArrayAdd(facebook, *this->scrollEntries);
    TouchButton *twitter = new TouchButton(*Globals::gameText->getText(5001), 0,
                                           Globals::w + Globals::layout->field_0x2c_rowHeight * -3 - back->getWidth() -
                                               facebook->getWidth(),
                                           Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
    twitter->id = 110;
    ArrayAdd(twitter, *this->scrollEntries);

    // Options.
    this->optionsButtons = new Array<TouchButton *>();
    this->addButton(8, *Globals::gameText->getText(489), 0, this->optionsButtons, 0);
    this->addButton(9, *Globals::gameText->getText(498), 1, this->optionsButtons, 0);
    if (showLanguages) {
        this->addButton(25, *Globals::gameText->getText(0), 2, this->optionsButtons, 0);
        // Language selection.
        this->buttonsB0 = new Array<TouchButton *>();
        int rowHeight = (int) (Globals::iPad ? (float) Globals::layout->field_0x34
                                             : (float) Globals::layout->field_0x34 * 0.9f) + 2;
        for (unsigned int i = 0; i < 10; i++) {
            // Phone: two columns; iPad: one centred column.
            int x = (int) ((float) ((i & 1) ? Globals::w - Globals::w / 4 : Globals::w / 4) +
                           (float) this->buttonWidth * -0.5f);
            int y = (rowHeight + Globals::layout->field_0x30_rowHeight) * (i >> 1) +
                    (Globals::h / 2 - this->buttonYBias - 12 - Globals::layout->field_0x29c_buttonRowGap);
            if (Globals::iPad) {
                x = Globals::w / 2 - this->buttonWidth / 2;
                y = (rowHeight + Globals::layout->field_0x30_rowHeight) * i +
                    (Globals::h / 2 - this->buttonYBias - Globals::layout->field_0x29c_buttonRowGap -
                     Globals::layout->field_0x30_rowHeight);
            }
            bool special = (i | 1) == 9;
            int spacing = Globals::Canvas->FontGetSpacing(
                special ? (unsigned int) Globals::fontLangSelect : (unsigned int) (uintptr_t) Globals::font);
            TouchButton *language = new TouchButton(
                *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y, this->buttonWidth, 17, 4,
                special ? (unsigned int) Globals::fontLangSelect : (unsigned int) (uintptr_t) Globals::font, spacing);
            language->id = MenuTouchWindow_languageIds[i];
            if (special)
                language->fontId = Globals::fontLangSelect;
            ArrayAdd(language, *this->buttonsB0);
        }
    }

    Globals::Canvas->Image2DCreate(1301, this->optionsImage_0x10c);
    Globals::Canvas->Image2DCreate(1302, this->optionsImage_0x110);
    Globals::Canvas->Image2DCreate(1303, this->optionsImage_0x114);
    Globals::Canvas->Image2DCreate(1304, this->optionsImage_0x118);
    this->heapBufA = new uint32_t[5];
    this->heapBufB = new uint32_t[5];
    Globals::Canvas->Image2DCreate(3002, this->heapBufA[0]);
    Globals::Canvas->Image2DCreate(3004, this->heapBufA[1]);
    Globals::Canvas->Image2DCreate(3011, this->heapBufA[2]);
    Globals::Canvas->Image2DCreate(3013, this->heapBufA[3]);
    Globals::Canvas->Image2DCreate(3015, this->heapBufA[4]);
    Globals::Canvas->Image2DCreate(3003, this->heapBufB[0]);
    if (Globals::iPad)
        Globals::Canvas->Image2DCreate(3014, this->heapBufB[1]);
    else
        Globals::Canvas->Image2DCreate(3005, this->heapBufB[1]);
    Globals::Canvas->Image2DCreate(3012, this->heapBufB[2]);
    Globals::Canvas->Image2DCreate(3014, this->heapBufB[3]);
    Globals::Canvas->Image2DCreate(3016, this->heapBufB[4]);
    if (Globals::iPad && !Globals::iPadAssetsWithLowerRes)
        Globals::Canvas->Image2DCreate(3101, this->borderImage_0x14c);
    Globals::Canvas->Image2DCreate(3000, this->scrollImage_0x13c);
    Globals::Canvas->Image2DCreate(3001, this->scrollImage_0x140);
    Globals::Canvas->Image2DCreate(3006, this->scrollImage_0x144);
    Globals::Canvas->Image2DCreate(3007, this->scrollImage_0x148);
    this->scrollbarHit = 0;
    this->contentHeightCache = Globals::Canvas->GetImage2DHeight(this->heapBufA[0]);
    this->slotImageWidth = Globals::Canvas->GetImage2DWidth(this->heapBufA[0]);
    this->slotX = Globals::layout->buttonInsetX + Globals::layout->field_0x2c_rowHeight;
    this->slotY = Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top;
    this->slotBorderWidth = Globals::iPad && !Globals::iPadAssetsWithLowerRes
                                ? 0
                                : Globals::Canvas->GetImage2DWidth(this->scrollImage_0x13c);
    this->slotMarkerWidth = Globals::Canvas->GetImage2DWidth(this->heapBufB[0]);
    this->slotMarkerHeight = Globals::Canvas->GetImage2DHeight(this->heapBufB[0]);
    this->slotListWidth = this->slotImageWidth + Globals::layout->field_0x2c_rowHeight * 2;
    this->slotTextWidth =
        Globals::w - this->slotListWidth - Globals::layout->buttonInsetX * 2 - this->slotBorderWidth;
    this->scrollSlots = new Array<TouchButton *>();
    ArraySetLength(5, *this->scrollSlots);
    this->slotSpacing = Globals::layout->field_0x2c_rowHeight;
    for (unsigned int i = 0; i < this->scrollSlots->size(); i++) {
        (*this->scrollSlots)[i] = new TouchButton(this->heapBufA[i], 16, this->slotX,
                                                  (this->slotSpacing + this->contentHeightCache) * i + this->slotY, 17);
        (*this->scrollSlots)[i]->id = 54 + i;
    }
    int windowHeight = Globals::h - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x20_top -
                       this->slotMarkerHeight - Globals::layout->field_0x2c_rowHeight -
                       Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24;
    if (Globals::iPad && !Globals::iPadAssetsWithLowerRes)
        windowHeight -= Globals::Canvas->GetImage2DHeight(this->borderImage_0x14c) * 2;
    this->scrollWindowB = new ScrollTouchWindow(
        Globals::w - Globals::layout->buttonInsetX - this->slotTextWidth,
        Globals::layout->field_0x2c_rowHeight +
            (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->slotMarkerHeight),
        this->slotTextWidth, windowHeight, false);
    this->scrollWindowB->setText(String(""), *Globals::gameText->getText(87 + this->field_0x1e0));

    int storeX = (int) (anchor.x + (float) this->buttonWidth + (float) (this->buttonWidth / 3));
    anchor = (*this->buttons)[0]->getPosition();
    int storeY = (int) (anchor.y - (float) (Globals::layout->field_0x2c_rowHeight * 3));
    TouchButton *store = new TouchButton(*Globals::gameText->getText(97), 17, storeX, storeY, 20);
    store->id = 53;
    ArrayAdd(store, *this->scrollEntries);
    if (!Globals::inAppPurchaseSupported)
        store->setVisible(false);
    Globals::Canvas->Image2DCreate(3010, this->image_0x124);
    Globals::Canvas->Image2DCreate(9023, this->image_0x128);

    if (Globals::iPad) {
        // Touch control preview.
        Globals::Canvas->Image2DCreate(1217, this->steerImageA);
        Globals::Canvas->Image2DCreate(1202, this->steerImageB);
        Globals::Canvas->Image2DCreate(1200, this->steerImageC);
        Globals::Canvas->Image2DCreate(1206, this->steerImageD);
        Globals::globals->setCoordsSteer(*(int *) &Globals::options[84],
                                         Globals::Canvas->GetImage2DWidth(this->steerImageA),
                                         Globals::Canvas->GetImage2DWidth(this->steerImageC),
                                         Globals::Canvas->GetImage2DWidth(this->steerImageB), this->steerCoord_0x2e,
                                         this->steerCoord_0x30, this->steerCoord_0x40, this->steerCoord_0x42,
                                         this->steerCoord_0x3c, this->steerCoord_0x3e, this->steerCoord_0x34,
                                         this->steerCoord_0x32, this->steerCoord_0x48, this->steerCoord_0x4a);
        Globals::Canvas->Image2DCreate(1222, this->fireImageA);
        Globals::Canvas->Image2DCreate(1706, this->fireImageB);
        Globals::Canvas->Image2DCreate(1210, this->fireImage_0x84);
        Globals::Canvas->Image2DCreate(1204, this->fireImage_0x80);
        Globals::Canvas->Image2DCreate(1214, this->fireImage_0x88);
        Globals::Canvas->Image2DCreate(1212, this->fireImage_0x8c);
        Globals::globals->setCoordsFire(*(int *) &Globals::options[88], Globals::Canvas->GetImage2DWidth(this->fireImageA),
                                        this->fireImageA, this->fireImageB, this->fireSize_0x74, this->fireCoord_0x60,
                                        this->fireCoord_0x62, this->fireCoord_0x64, this->fireCoord_0x66,
                                        this->fireCoord_0x68, this->fireCoord_0x6a, this->fireCoord_0x6c,
                                        this->fireCoord_0x6e, this->fireCoord_0x70, this->fireCoord_0x72,
                                        this->fireCoord_0x4c, this->fireCoord_0x4e);
        Globals::Canvas->Image2DCreate(1701, this->fireImage_0x50);
        Globals::Canvas->Image2DCreate(1702, this->fireImage_0x54);
        Globals::Canvas->Image2DCreate(1703, this->fireImage_0x58);
        Globals::Canvas->Image2DCreate(1704, this->fireImage_0x5c);
        this->cinematicBtnB = new TouchButton(
            *Globals::gameText->getText(497), 0, Globals::w / 2,
            Globals::layout->field_0x300 + Globals::layout->choiceWindowTitleHeight - Globals::layout->field_0x34 * 2 -
                Globals::layout->field_0x30_rowHeight * 2,
            (int) (Globals::iPadHD ? 281.25f : (Globals::iPadLarge ? 400.0f : 200.0f)), 68, 4);
        this->cinematicBtnA = new TouchButton(
            *Globals::gameText->getText(496), 0, Globals::w / 2,
            Globals::layout->field_0x300 + Globals::layout->choiceWindowTitleHeight - Globals::layout->field_0x34 -
                Globals::layout->field_0x30_rowHeight,
            (int) (Globals::iPadHD ? 281.25f : (Globals::iPadLarge ? 400.0f : 200.0f)), 68, 4);
        this->cinematicTextWindow =
            new ScrollTouchWindow(Globals::w / 2 - Globals::layout->field_0x304 / 2,
                                  Globals::layout->field_0x300 + Globals::layout->windowTopInset,
                                  Globals::layout->field_0x304, Globals::layout->choiceWindowTitleHeight, false);
        this->cinematicTextWindow->setText(String(""), *Globals::gameText->getText(495));
        this->cinematicTouchIdA = 0;
        this->cinematicTouchIdB = 0;
        float entryWidth;
        if (Globals::iPadHD) {
            this->listBottomY = 703;
            entryWidth = 175.78125f;
        } else {
            this->listBottomY = Globals::iPadLarge ? 1000 : 500;
            entryWidth = Globals::iPadLarge ? 250.0f : 125.0f;
        }
        this->listTopY = (Globals::w - this->listBottomY) / 2;
        this->listEntryWidth = (int) entryWidth;
    } else {
        this->listTopY = 0;
        this->listBottomY = Globals::w;
        this->listEntryWidth = (Globals::h - Globals::layout->field_0xc_leftMargin -
                                Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x20_top -
                                Globals::layout->field_0x24) / 2;
    }
    this->optionsWidth = this->listBottomY - Globals::layout->buttonInsetX * 2;
    this->listEntryHeight = 1 - Globals::layout->field_0x2c_rowHeight + this->optionsWidth / 2;
    this->field_0x15c = 1 - Globals::layout->field_0x2c_rowHeight + this->listEntryWidth / 2;
    this->sliderSpacing = Globals::Canvas->GetImage2DHeight(this->optionsImage_0x114);
    this->sliderRowHeight = this->sliderSpacing + 15;

    this->optBtnD0 = new TouchButton(
        *Globals::gameText->getText(492), 0,
        this->listTopY + this->listBottomY - Globals::layout->buttonInsetX - this->listEntryHeight / 2,
        Globals::layout->field_0x2c_rowHeight + (this->listEntryWidth + (Globals::layout->field_0xc_leftMargin +
                                                                         Globals::layout->field_0x20_top)) +
            this->field_0x15c / 2,
        this->listEntryHeight - 20, 68, 4);
    this->optBtnCC = new TouchButton(String(""), 13,
                                     this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight - 10,
                                     Globals::layout->field_0x2c_rowHeight +
                                         (this->listEntryWidth + (Globals::layout->field_0xc_leftMargin +
                                                                  Globals::layout->field_0x20_top)) +
                                         this->field_0x15c / 2,
                                     66);
    this->scrollExtraButton = new TouchButton(
        String(""), 13, this->listTopY + this->listBottomY - 25 - Globals::layout->buttonInsetX,
        Globals::layout->field_0x2c_rowHeight * 5 +
            (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x1c * 3 +
             this->listEntryWidth) +
            this->sliderRowHeight + this->sliderRowHeight / 2 +
            (Globals::layout->field_0x2fc + (this->field_0x15c + Globals::layout->field_0x2c_rowHeight) +
             this->sliderRowHeight) * 2,
        66);
    this->optBtnD0->setHalfTransparent(Globals::options[17]);
    this->optBtnCC->setAlwaysPressed(Globals::options[16]);
    this->scrollExtraButton->setAlwaysPressed(Globals::options[64]);
    if (Globals::iPad) {
        this->scrollUpButton = new TouchButton(
            *Globals::gameText->getText(494), 0,
            this->listTopY + this->listBottomY - Globals::layout->buttonInsetX - this->listEntryHeight / 2,
            Globals::layout->field_0x2c_rowHeight + (this->listEntryWidth + (Globals::layout->field_0xc_leftMargin +
                                                                             Globals::layout->field_0x20_top)) +
                this->field_0x15c / 2,
            this->listEntryHeight - 20, 68, 4);
    }

    this->buttonsB4 = new Array<TouchButton *>();
    this->addButton(16, *Globals::gameText->getText(519), 0, this->buttonsB4, 0);
    this->addButton(50, *Globals::gameText->getText(25), 1, this->buttonsB4, 0);

    // Control scheme selection.
    this->buttonsB8 = new Array<TouchButton *>();
    unsigned int pressedImage = 0xffffffffu;
    unsigned int image = 0xffffffffu;
    for (int i = 0; i < 3; i++) {
        Globals::Canvas->Image2DCreate(MenuTouchWindow_controlImages[i][0], image);
        Globals::Canvas->Image2DCreate(MenuTouchWindow_controlImages[i][1], pressedImage);
        int imageWidth = Globals::Canvas->GetImage2DWidth(image);
        int imageHeight = Globals::Canvas->GetImage2DHeight(image);
        int x;
        if (i == 0)
            x = 0 - imageWidth / 2 - imageWidth + Globals::w / 2;
        else
            x = (i == 1 ? 0 - imageWidth / 2 : imageWidth / 2) + Globals::w / 2;
        ArrayAdd(new TouchButton(image, pressedImage, 19, x, Globals::h / 2 - imageHeight / 2, 1), *this->buttonsB8);
    }

    // Volume / sensitivity sliders.
    int sliderTop = Globals::layout->field_0x2b4;
    int sliderTop2 = Globals::layout->field_0x2b8;
    this->sliders = new Array<TouchSlider *>();
    ArrayAdd(new TouchSlider(0,
                             Globals::w + 10 - Globals::layout->buttonInsetX - this->listEntryHeight,
                             Globals::layout->field_0xc_leftMargin - sliderTop + Globals::layout->field_0x20_top +
                                 this->listEntryWidth +
                                 (this->field_0x15c + Globals::layout->field_0x2c_rowHeight) * 2,
                             Globals::options[17] ? *(float *) &Globals::options[20] : *(float *) &Globals::options[24]),
             *this->sliders);
    ArrayAdd(new TouchSlider(1, Globals::layout->buttonInsetX + this->metricC,
                             Globals::layout->field_0xc_leftMargin + sliderTop2 + Globals::layout->field_0x20_top +
                                 Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight,
                             *(float *) &Globals::options[0]),
             *this->sliders);
    ArrayAdd(new TouchSlider(1, this->metricC + Globals::layout->buttonInsetX,
                             Globals::layout->field_0xc_leftMargin + sliderTop2 + Globals::layout->field_0x20_top +
                                 Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight * 3 +
                                 this->sliderSpacing,
                             *(float *) &Globals::options[4]),
             *this->sliders);
    ArrayAdd(new TouchSlider(1, this->metricC + Globals::layout->buttonInsetX,
                             Globals::layout->field_0xc_leftMargin + sliderTop2 + Globals::layout->field_0x20_top +
                                 Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight * 5 +
                                 this->sliderSpacing * 2,
                             *(float *) &Globals::options[8]),
             *this->sliders);
    ArrayAdd(new TouchSlider(2, this->metricC + Globals::layout->buttonInsetX,
                             (int) ((double) (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                              Globals::layout->field_0x1c * 3 +
                                              Globals::layout->field_0x2c_rowHeight * 6) +
                                    (double) this->sliderSpacing * 2.5 + (double) sliderTop),
                             *(float *) &Globals::options[36]),
             *this->sliders);
    ArrayAdd(new TouchSlider(2, this->metricC + Globals::layout->buttonInsetX,
                             (int) ((double) (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                              Globals::layout->field_0x1c * 3 +
                                              Globals::layout->field_0x2c_rowHeight * 8) +
                                    (double) this->sliderSpacing * 2.5 + (double) this->sliderRowHeight +
                                    (double) sliderTop),
                             *(float *) &Globals::options[40]),
             *this->sliders);
    ArrayAdd(new TouchSlider(2, Globals::layout->buttonInsetX + this->metricC,
                             Globals::layout->field_0xc_leftMargin + sliderTop + Globals::layout->field_0x20_top +
                                 Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight,
                             *(float *) &Globals::options[44]),
             *this->sliders);
    (*this->sliders)[6]->setPosition(Globals::w / 2 - (*this->sliders)[6]->getWidth() / 2,
                                     Globals::layout->field_0xc_leftMargin + sliderTop +
                                         Globals::layout->field_0x20_top + Globals::layout->field_0x1c +
                                         Globals::layout->field_0x2c_rowHeight);
    this->optBtnD4 = new TouchButton(String(""), 13, Globals::w - Globals::layout->buttonInsetX,
                                     Globals::layout->field_0x2c_rowHeight +
                                         (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                          Globals::layout->field_0x1c),
                                     18);
    this->optBtnD4->setAlwaysPressed(Globals::options[13]);
    (*this->sliders)[1]->setHalfTransparent(!Globals::options[13]);
    this->optBtnD8 = new TouchButton(String(""), 13, Globals::w - Globals::layout->buttonInsetX,
                                     Globals::layout->field_0x2c_rowHeight * 3 +
                                         (this->sliderSpacing + (Globals::layout->field_0xc_leftMargin +
                                                                 Globals::layout->field_0x20_top) +
                                          Globals::layout->field_0x1c),
                                     18);
    this->optBtnD8->setAlwaysPressed(Globals::options[12]);
    (*this->sliders)[2]->setHalfTransparent(!Globals::options[14]);
    this->optBtnDC = new TouchButton(String(""), 13, Globals::w - Globals::layout->buttonInsetX,
                                     Globals::layout->field_0x2c_rowHeight * 5 +
                                         (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                          this->sliderSpacing * 2 + Globals::layout->field_0x1c),
                                     18);
    this->optBtnDC->setAlwaysPressed(Globals::options[14]);
    (*this->sliders)[3]->setHalfTransparent(!Globals::options[14]);
    (*this->sliders)[5]->setFixedScale(1);
    (*this->sliders)[6]->setFixedScale(1);

    if (Globals::iPad) {
        (*this->sliders)[0]->setPosition(
            this->listTopY + Globals::layout->buttonInsetX + 10,
            (int) ((float) (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                            Globals::layout->field_0x1c + this->listEntryWidth +
                            Globals::layout->field_0x2c_rowHeight * 3 + this->field_0x15c * 2) -
                   (Globals::iPadHD ? 35.15625f : (Globals::iPadLarge ? 50.0f : 25.0f))));
        (*this->sliders)[1]->setPosition(
            Globals::layout->buttonInsetX + this->listTopY + this->metricC,
            (int) ((Globals::iPadHD ? 19.6875f : (Globals::iPadLarge ? 28.0f : 14.0f)) +
                   (float) (Globals::layout->field_0x2fc +
                            (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                             this->listEntryWidth + Globals::layout->field_0x2c_rowHeight * 4) +
                            (this->field_0x15c + Globals::layout->field_0x1c) * 2)));
        (*this->sliders)[2]->setPosition(
            this->listTopY + Globals::layout->buttonInsetX + this->metricC,
            (int) ((Globals::iPadHD ? 19.6875f : (Globals::iPadLarge ? 28.0f : 14.0f)) +
                   (float) (Globals::layout->field_0x2fc +
                            (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                             this->listEntryWidth + Globals::layout->field_0x2c_rowHeight * 3) +
                            this->sliderRowHeight +
                            (this->field_0x15c + Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight) *
                                2)));
        (*this->sliders)[3]->setPosition(
            this->listTopY + Globals::layout->buttonInsetX + this->metricC,
            (int) ((Globals::iPadHD ? 19.6875f : (Globals::iPadLarge ? 28.0f : 14.0f)) +
                   (float) (Globals::layout->field_0x2fc +
                            (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                             this->listEntryWidth + Globals::layout->field_0x2c_rowHeight * 6) +
                            (this->field_0x15c + Globals::layout->field_0x1c + this->sliderRowHeight) * 2)));
        if (Globals::iPadLargePossible) {
            (*this->sliders)[4]->setPosition(-100000, -100000);
            (*this->sliders)[5]->setPosition(
                Globals::layout->buttonInsetX + this->listTopY + this->metricC,
                (int) ((Globals::iPadHD ? 9.84375f : (Globals::iPadLarge ? 14.0f : 7.0f)) +
                       (float) ((Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                                 Globals::layout->field_0x1c * 3 + this->listEntryWidth +
                                 Globals::layout->field_0x2c_rowHeight * 7) +
                                (this->field_0x15c + Globals::layout->field_0x2fc + this->sliderRowHeight * 2) * 2)));
            (*this->sliders)[5]->setValue(*(float *) &Globals::options[68]);
        } else {
            (*this->sliders)[4]->setPosition(
                Globals::layout->buttonInsetX + this->listTopY + this->metricC,
                (int) ((Globals::iPadHD ? 9.84375f : (Globals::iPadLarge ? 14.0f : 7.0f)) +
                       (float) ((Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                                 Globals::layout->field_0x1c * 3 + this->listEntryWidth +
                                 Globals::layout->field_0x2c_rowHeight * 6) +
                                this->sliderRowHeight + (this->field_0x15c + Globals::layout->field_0x2fc) * 2)));
            (*this->sliders)[5]->setPosition(
                Globals::layout->buttonInsetX + this->listTopY + this->metricC,
                (int) ((Globals::iPadHD ? 9.84375f : (Globals::iPadLarge ? 14.0f : 7.0f)) +
                       (float) ((Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                                 Globals::layout->field_0x1c * 3 + this->listEntryWidth +
                                 Globals::layout->field_0x2c_rowHeight * 7) +
                                (this->field_0x15c + Globals::layout->field_0x2fc + this->sliderRowHeight * 2) * 2)));
        }
        Globals::Canvas->Image2DCreate(1700, this->scrollbarImageId);
        this->listX = Globals::iPadAssetsWithLowerRes ? 50 : (int) ((float) Globals::w * 0.2f);
        this->scrollWindowA = new ScrollTouchWindow(
            Globals::layout->buttonInsetX + Globals::w / 4,
            Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
            Globals::w / 2 - Globals::layout->buttonInsetX * 2,
            Globals::h - (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top) -
                Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
            false);
    } else {
        this->listX = 0;
        this->scrollWindowA = new ScrollTouchWindow(
            Globals::layout->buttonInsetX, Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
            Globals::w - Globals::layout->buttonInsetX * 2,
            Globals::h - (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top) -
                Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
            false);
    }

    // About text.
    String version = ((Engine *) Globals::appManager->GetEngine())->str_0x3c;
    if (version.length >= 3)
        version = String(" v") + version;
    else
        version = String("");
    String about = String("Galaxy on Fire 2 HD") + version + String("\n\n") + *Globals::gameText->getText(45) +
                   String("\n\n\n") + *Globals::gameText->getText(48);
    this->scrollWindowA->setText(String(""), about);
    this->scrollWindowA->setTextCentered(true);

    TouchButton *credits;
    if (!Globals::iPad || Globals::iPadAssetsWithLowerRes) {
        credits = new TouchButton(*Globals::gameText->getText(98), 0, Globals::w - Globals::layout->field_0x2c_rowHeight,
                                  Globals::h - Globals::layout->field_0x2c_rowHeight, 34);
    } else {
        credits = new TouchButton(*Globals::gameText->getText(98), 7,
                                  (int) ((float) (this->slotListWidth + Globals::layout->buttonInsetX) +
                                         (float) this->slotTextWidth * 0.5f),
                                  Globals::h + 5 - Globals::layout->field_0x10_rightMargin -
                                      Globals::layout->field_0x24 -
                                      Globals::Canvas->GetImage2DHeight(this->borderImage_0x14c),
                                  36);
    }
    credits->id = 52;
    ArrayAdd(credits, *this->scrollEntries);

    this->messageShowing = 0;
    this->genericConfirmA = 0;
    this->supernovaMessageShowing = 0;
    this->supernovaPurchaseDialogShowing = 0;
    this->choiceWindow = new ChoiceWindow();
    MtwAppData *appData = (MtwAppData *) Globals::appManager->GetApplicationData();
    if (!Globals::options[53] && ((uint8_t *) appData)[108]) {
        Globals::options[53] = 1;
        Globals::recordHandler->saveOptions();
        ((uint8_t *) appData)[108] = 0;
        this->choiceWindow->set(*Globals::gameText->getText(74));
        this->genericConfirmA = 1;
        this->messageShowing = 1;
    } else if (Globals::iPadLargePossible && !Globals::options[65]) {
        Globals::options[65] = 1;
        Globals::recordHandler->saveOptions();
        this->choiceWindow->set(*Globals::gameText->getText(123));
        this->genericConfirmA = 1;
        this->messageShowing = 1;
    }

    this->missionsWindow = 0;
    this->recordRows = 0;
    this->menuState = 0;
    this->quitConfirmShowing = 0;
    this->returnToMenuShowing = 0;
    this->leaderboardDialogShowing = 0;
    this->dlcMessageShowing = 0;
    this->saveDialogShowing = 0;
    this->returnToMenuSubFlag = 0;
    this->field_0x175 = 0;
    this->field_0x176 = 0;
    this->field_0x171 = 0;
    this->field_0x172 = 0;
    this->field_0x17b = 0;
    this->dlcResultDialogShowing = 0;
    this->dlcErrorDialogShowing = 0;
    this->loadFailedDialogShowing = 0;
    this->selectedRow = -1;
    this->okButton = 0;
    this->backButton = 0;
    this->upButtonPressed = 0;
    this->downButtonPressed = 0;
    this->fadeValue = 0.5f;
    this->screenshotState = -1;
    this->field_0x188 = 0;
    this->storeInitDialogShowing = 0;
    this->purchaseRestorePending = 0;
    this->scrollOffset = 0;
    this->field_0x1de = 0;
    this->field_0x1da = 0;
    this->listStateSuppress = 0;
    this->field_0x1dc = 0;
    this->field_0x1dd = 0;
    this->dragLastX = 0;
    this->field_0x210 = 0;
    this->dragVelocity = 0;
    this->inertiaDecay = 0.0f;
    this->inertiaVel = 0.0f;
    this->dragStartX = 0;
    this->dragging = 0;
}

void MenuTouchWindow::loadPreviewRecords() {
    this->scrollOffset = 0;

    this->dragLastX = 0;
    this->field_0x210 = 0;
    this->dragVelocity = 0;
    this->inertiaDecay = 0;
    this->inertiaVel = 0.0f;
    this->dragStartX = 0;
    this->dragging = 0;

    int *metrics = &Globals::h;
    Layout *layout = Globals::layout;
    int *rowObj = &Globals::recordSlots;

    this->contentHeight = (((metrics[0] - layout->field_0x10_rightMargin) - layout->field_0xc_leftMargin)
                           - layout->field_0x20_top) - layout->field_0x24;

    this->pageHeight = rowObj[0] * (layout->field_0x70_rowHeight + this->listRowGap);

    Array<GameRecord *> *rec = this->previewRecords;
    if (rec != 0) {
        delete rec;
        this->previewRecords = 0;
    }
    RecordHandler *rh = new RecordHandler();
    this->previewRecords = (Array<GameRecord *> *) rh->readAllPreviewRecords();
    delete rh;

}

void MenuTouchWindow::addButton(int id, AbyssEngine::String label, int row, Array<TouchButton *> *arr, int yOff) {
    TouchButton *btn = new TouchButton(label, 0, Globals::w / 2 - this->buttonWidth / 2,
                                       (yOff - this->buttonYBias) + Globals::h / 2 +
                                       (Globals::layout->field_0x30_rowHeight + this->buttonRowGap) * row,
                                       this->buttonWidth, 0x11, 0x04);
    btn->id = id;
    ArrayAdd<TouchButton *>(btn, *arr);

    int *posX = Globals::sub_menu_buttons_x;
    int *posY = Globals::sub_menu_buttons_y;
    for (uint32_t i = 0; i < arr->count; i++) {
        if (i < 10) {
            {
                Vector bpos = arr->data_[i]->getPosition();
                posX[i] = (int) bpos.x;
            }
            {
                Vector bpos = arr->data_[i]->getPosition();
                posY[i] = (int) bpos.y;
            }
        }
    }
    Globals::sub_menu_button_count = arr->count;
}

MenuTouchWindow::~MenuTouchWindow() {
    if (this->buttons != 0) {
        ArrayReleaseClasses(*this->buttons);
        delete this->buttons;
    }
    this->buttons = 0;
    if (this->optionsButtons != 0) {
        ArrayReleaseClasses(*this->optionsButtons);
        delete this->optionsButtons;
    }
    this->optionsButtons = 0;
    if (this->scrollEntries != 0) {
        ArrayReleaseClasses(*this->scrollEntries);
        delete this->scrollEntries;
    }
    this->scrollEntries = 0;
    if (this->buttonsB4 != 0) {
        ArrayReleaseClasses(*this->buttonsB4);
        delete this->buttonsB4;
    }
    this->buttonsB4 = 0;
    if (this->buttonsB8 != 0) {
        ArrayReleaseClasses(*this->buttonsB8);
        delete this->buttonsB8;
    }
    this->buttonsB8 = 0;
    if (this->buttonsB0 != 0) {
        ArrayReleaseClasses(*this->buttonsB0);
        delete this->buttonsB0;
    }
    this->buttonsB0 = 0;
    if (this->sliders != 0) {
        ArrayReleaseClasses(*this->sliders);
        delete this->sliders;
    }
    this->sliders = 0;
    if (this->scrollSlots != 0) {
        ArrayReleaseClasses(*this->scrollSlots);
        delete this->scrollSlots;
    }
    this->scrollSlots = 0;
    if (this->previewStrings0 != 0) {
        ArrayReleaseClasses(*this->previewStrings0);
        delete this->previewStrings0;
    }
    this->previewStrings0 = 0;
    if (this->previewStrings1 != 0) {
        ArrayReleaseClasses(*this->previewStrings1);
        delete this->previewStrings1;
    }
    this->previewStrings1 = 0;
    delete this->optBtnCC;
    this->optBtnCC = 0;
    delete this->okButton;
    this->okButton = 0;
    delete this->optBtnD0;
    this->optBtnD0 = 0;
    delete this->optBtnD4;
    this->optBtnD4 = 0;
    delete this->optBtnD8;
    this->optBtnD8 = 0;
    delete this->optBtnDC;
    this->optBtnDC = 0;
    delete this->choiceWindow;
    this->choiceWindow = 0;
    delete this->backButton;
    this->backButton = 0;
    delete this->scrollExtraButton;
    this->scrollExtraButton = 0;
    delete this->scrollWindowA;
    this->scrollWindowA = 0;
    delete this->scrollWindowB;
    this->scrollWindowB = 0;
    if (this->heapBufA != 0)
        ::operator delete[](this->heapBufA);
    this->heapBufA = 0;
    if (this->heapBufB != 0)
        ::operator delete[](this->heapBufB);
    this->heapBufB = 0;
}

bool MenuTouchWindow::isInMissionScreen() {
    return this->menuState == 9;
}

void MenuTouchWindow::setCutsceneMode(bool mode) {
    this->cutsceneMode = (uint8_t) mode;
    bool notMode = !mode;
    for (uint32_t i = 0; i < this->buttons->count; i++) {
        TouchButton *btn = this->buttons->data_[i];
        if (btn->id == 0x13) {
            btn->setVisible(notMode);
        }
    }
}

void MenuTouchWindow::createRecordButtons(bool inSaveMode) {
    if (this->recordRows != 0) {
        for (unsigned int i = 0; i < this->recordRows->size(); i++) {
            if ((*this->recordRows)[i] != 0) {
                ArrayReleaseClasses(*(*this->recordRows)[i]);
                delete (*this->recordRows)[i];
                (*this->recordRows)[i] = 0;
            }
        }
        ArrayReleaseClasses(*this->recordRows);
        delete this->recordRows;
        this->recordRows = 0;
    }
    this->recordRows = new Array<Array<String *> *>();
    ArraySetLength(Globals::recordSlots, *this->recordRows);

    for (int i = 0; i < Globals::recordSlots; i++) {
        (*this->recordRows)[i] = new Array<String *>();
        ArraySetLength(6, *(*this->recordRows)[i]);
        String unused;
        if (this->previewRecords != 0 && (*this->previewRecords)[i] != 0) {
            String time;
            Globals::globals->longToTimeStringNoSeconds((*this->previewRecords)[i]->playTime, time);
            (*(*this->recordRows)[i])[0] = new String(time);
            (*(*this->recordRows)[i])[1] = new String(reinterpret_cast<String &>((*this->previewRecords)[i]->pilotName));
            (*(*this->recordRows)[i])[2] = new String(i == 0 ? *Globals::gameText->getText(486) : String(""));
            (*(*this->recordRows)[i])[3] =
                new String(Globals::layout->formatCredits((*this->previewRecords)[i]->field_0x08));
            (*(*this->recordRows)[i])[4] =
                new String(*Globals::gameText->getText(321) + String(": ") +
                           String((int) (*this->previewRecords)[i]->killsText));
            (*(*this->recordRows)[i])[5] =
                new String((*this->previewRecords)[i]->rank <= 0.0f
                               ? *Globals::gameText->getText(519)
                               : (*this->previewRecords)[i]->rank <= 0.5f
                                     ? *Globals::gameText->getText(519)
                                     : (*this->previewRecords)[i]->rank <= 1.0f
                                           ? *Globals::gameText->getText(519)
                                           : *Globals::gameText->getText(25));
        } else {
            String time;
            Globals::globals->longToTimeStringNoSeconds(0, time);
            (*(*this->recordRows)[i])[0] = new String(time);
            (*(*this->recordRows)[i])[1] = new String(*Globals::gameText->getText(174));
            (*(*this->recordRows)[i])[2] = new String(i == 0 ? *Globals::gameText->getText(486) : String(""));
            (*(*this->recordRows)[i])[3] = new String("");
            (*(*this->recordRows)[i])[4] = new String("");
            (*(*this->recordRows)[i])[5] = new String("");
        }
    }

    if (this->okButton != 0) {
        delete this->okButton;
        this->okButton = 0;
    }
    int y = Globals::layout->field_0x108;
    if (Globals::iPad) {
        this->okButton = new TouchButton(*Globals::gameText->getText(inSaveMode ? 506 : 505), 7,
                                         Globals::w - this->listX - Globals::layout->buttonInsetX,
                                         Globals::layout->field_0xc_leftMargin + y + Globals::layout->field_0x20_top, 18);
    } else {
        this->okButton = new TouchButton(*Globals::gameText->getText(inSaveMode ? 506 : 505), 0,
                                         Globals::w - this->listX - Globals::layout->buttonInsetX,
                                         Globals::layout->field_0xc_leftMargin + y + Globals::layout->field_0x20_top,
                                         Globals::layout->field_0x2a4_metricA, 18, 4);
    }
    this->okButton->setPosition(Globals::w - this->listX - Globals::layout->buttonInsetX,
                                (Globals::layout->field_0xc_leftMargin + y + Globals::layout->field_0x20_top +
                                 this->scrollOffset) +
                                    (Globals::layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow);
    if (this->backButton != 0) {
        delete this->backButton;
        this->backButton = 0;
    }
    this->backButton = new TouchButton(*Globals::gameText->getText(65), 7,
                                       Globals::w - this->listX - Globals::layout->buttonInsetX,
                                       Globals::layout->field_0xc_leftMargin + y + Globals::layout->field_0x20_top, 18);
}

void MenuTouchWindow::saveGame(int slot) {
    RecordHandler *rh = new RecordHandler();
    rh->recordStoreWrite(slot);
    rh->recordStoreWritePreview(slot);

    int *arr = (int *) this->previewRecords->data_;
    GameRecord *rec = this->previewRecords->data_[slot];
    int *cell;
    if (rec == 0) {
        cell = arr + slot;
    } else {
        ::operator delete(_mtw_GameRecord_dtor(rec));
        cell = (int *) &this->previewRecords->data_[slot];
    }
    *cell = 0;

    GameRecord *preview = rh->recordStoreReadPreview(slot);
    this->previewRecords->data_[slot] = preview;
    delete rh;

    this->createRecordButtons(true);
    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    String *s = (String *) _mtw_GameText_getText(Globals::gameText, 0x32);
    cw->set(*s, false);
    this->messageShowing = 1;
    this->saveDialogShowing = 0;
}

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

// _mtw_Ship_addCargo removed: ship->addCargo(item)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void MenuTouchWindow::showSupernovaMessage() {
    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    GameText *gt = Globals::gameText;
    String *s1 = (String *) _mtw_GameText_getText(gt, 0x266);
    String *s2 = (String *) _mtw_GameText_getText(Globals::gameText, 0x267);
    cw->set(*s1, *s2, false);
    this->messageShowing = 1;
    this->supernovaMessageShowing = 1;
}

int MenuTouchWindow::loadGame(int slot) {
    RecordHandler *rh = new RecordHandler();
    GameRecord *rec = rh->readRecord(slot);

    if (rec != 0) {
        if (rec->versionMismatchFlag != 0 && Globals::options[55] == 0) {
            this->choiceWindow->set(*Globals::gameText->getText(102), false);
            this->messageShowing = 1;
            delete rec;
            delete rh;
            return 0;
        }
        if (rec->dlcRequiredFlag != 0 && Globals::options[53] == 0) {
            this->choiceWindow->set(*Globals::gameText->getText(101), false);
            this->messageShowing = 1;
            delete rec;
            delete rh;
            return 0;
        }
    } else {
        Globals::status->resetGame();
        this->choiceWindow->set(*Globals::gameText->getText(100), false);
        this->messageShowing = 1;
        this->loadFailedDialogShowing = 1;
        delete rh;
        return 0;
    }

    Globals::status->resetGame();
    rec->load();
    delete rh;
    delete rec;
    ModStation *ms = (ModStation *) Globals::appManager->GetApplicationModule(5);
    ms->setGameLoaded();
    Globals::switch_to_target_setting = 0;
    Globals::appManager->SetCurrentApplicationModule(5);
    return 1;
}

float MenuTouchWindow::getRelativeScrollStartPos() {
    int offset = this->scrollOffset;
    if (offset > 0)
        return 0.0f;
    return -(float) offset / (float) this->pageHeight;
}

float MenuTouchWindow::getRelativeScrollHeight() {
    int content = this->contentHeight;
    int page = this->pageHeight;
    if (content > page)
        return 0.0f;

    int pos = this->scrollOffset;
    if (pos >= 1) {
        return (float) (content - pos) / (float) page;
    }
    if (pos >= content - page) {
        return (float) content / (float) page;
    }
    return (float) (pos + page) / (float) page;
}

void MenuTouchWindow::drawLoadSaveMenu(bool param1) {
    int shipY = Globals::layout->field_0x10c_rowBaseY;
    Globals::Canvas->SetColor(0xffffffffu);
    int nameY = Globals::layout->field_0x110_strip58;
    int infoY = Globals::layout->field_0x114_strip5c;
    int width = Globals::w - Globals::layout->buttonInsetX * 2 - this->listX * 2;
    if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
        if (Globals::iPadHD) {
            nameY = 8;
            infoY = 5;
        } else {
            nameY = !Globals::iPadLarge ? 6 : 12;
            infoY = !Globals::iPadLarge ? 4 : 8;
        }
        // Side borders.
        int borderWidth = Globals::Canvas->GetImage2DWidth(this->scrollbarImageId);
        int borderHeight = Globals::Canvas->GetImage2DHeight(this->scrollbarImageId);
        int count = Globals::h / borderHeight;
        for (int i = 0, y = 0; i <= count; i++) {
            Globals::Canvas->DrawImage2D(this->scrollbarImageId,
                                         Globals::layout->buttonInsetX - borderWidth + this->listX, y, 1);
            Globals::Canvas->DrawImage2D(this->scrollbarImageId, Globals::layout->buttonInsetX + width + this->listX,
                                         y, 0);
            y += borderHeight;
        }
    }
    this->okButton->setPosition(Globals::w - this->listX - Globals::layout->buttonInsetX,
                                (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top) +
                                this->scrollOffset +
                                (Globals::layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow +
                                Globals::layout->field_0x108);

    for (int i = 0; i < Globals::recordSlots; i++) {
        int y = Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->scrollOffset + (Globals::layout->field_0x70_rowHeight + this->listRowGap) * i;
        if (y < 0 || y > Globals::h)
            continue;
        Globals::Canvas->SetColor(0xffffffffu);
        Globals::layout->drawBox(i == this->selectedRow ? 4 : 3, Globals::layout->buttonInsetX + this->listX, y,
                                 width - 3, Globals::layout->field_0x70_rowHeight, String(""));
        int nameLineY = y + nameY;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[0],
                                    this->listX + Globals::layout->buttonInsetX + Globals::layout->field_0x2c_rowHeight,
                                    nameLineY, false);
        GameRecord *record = (*this->previewRecords)[i];
        if (record != 0 && record->shipId < 64)
            Globals::imageFactory->drawShip(record->shipId,
                                            Globals::layout->buttonInsetX + this->listX +
                                            Globals::layout->field_0x2c_rowHeight + this->metricB,
                                            shipY + y);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[1],
                                    Globals::layout->buttonInsetX + this->listX +
                                    Globals::layout->field_0x2c_rowHeight * 2 + this->metricB +
                                    Globals::layout->field_0x2c4,
                                    nameLineY, false);
        Globals::Canvas->SetColor(0x777777ffu);
        int infoLineY = y + infoY;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[2],
                                    this->listX + Globals::layout->buttonInsetX + Globals::layout->field_0x2c_rowHeight,
                                    infoLineY + Globals::layout->field_0x70_rowHeight / 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[3],
                                    this->listX + Globals::layout->buttonInsetX +
                                    Globals::layout->field_0x2c_rowHeight * 2 + this->metricB +
                                    Globals::layout->field_0x2c4,
                                    infoLineY + Globals::layout->field_0x70_rowHeight / 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[4],
                                    Globals::layout->buttonInsetX + this->listX + Globals::layout->field_0x2c4 +
                                    (Globals::layout->field_0x2c_rowHeight + this->metricA) * 2,
                                    nameLineY, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[5],
                                    Globals::layout->buttonInsetX + this->listX + Globals::layout->field_0x2c4 +
                                    (Globals::layout->field_0x2c_rowHeight + this->metricA) * 2,
                                    infoLineY + Globals::layout->field_0x70_rowHeight / 2, false);
        if (i == this->selectedRow)
            this->okButton->draw();
    }
}

void MenuTouchWindow::draw() {
    Globals::is_menu_visible = 1;
    Globals::topMenuIndex = this->menuState;

    // Publish the visible sub-menu button positions (read by the platform layer).
    Array<TouchButton *> *menuButtons = this->buttons;
    if (this->menuState == 12)
        menuButtons = this->buttonsB4;
    if (this->menuState == 14)
        menuButtons = this->buttonsB0;
    for (unsigned int i = 0; i < menuButtons->size(); i++) {
        if (i < 10) {
            Globals::sub_menu_buttons_x[i] = (int) (*menuButtons)[i]->getPosition().x;
            Globals::sub_menu_buttons_y[i] = (int) (*menuButtons)[i]->getPosition().y;
        }
    }
    Globals::sub_menu_button_count = menuButtons->size();

    int arrowOffset = Globals::layout->field_0x118;
    int labelOffset = Globals::layout->field_0x11c;
    int sliderLabelOffset = Globals::layout->field_0x120;

    if (this->backgroundEnabled != 0 && this->menuState != 9 && this->menuState != 13)
        Globals::layout->drawBG();

    switch (this->menuState) {
        case 1:
        case 2:
            // Load / save game.
            Globals::layout->drawBG();
            Globals::layout->drawHeader(String(*Globals::gameText->getText(this->menuState == 2 ? 30 : 29)), false);
            this->drawLoadSaveMenu(false);
            break;
        case 0: {
            // Main menu.
            if (this->backgroundEnabled != 0) {
                Globals::layout->drawHeader(String(*Globals::gameText->getText(this->backgroundEnabled == 1 ? 40 : 171)),
                                            false);
            } else {
                Globals::Canvas->SetColor(0xffffffffu);
                if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
                    Globals::Canvas->DrawImage2D(this->field_0x120, Globals::w >> 1, 80, 17, 20);
                } else {
                    int logoTop = Globals::layout->field_0xc_leftMargin;
                    int logoWidth = Globals::Canvas->GetImage2DWidth(this->field_0x120);
                    int logoHeight = Globals::Canvas->GetImage2DHeight(this->field_0x120);
                    Globals::Canvas->DrawImage2D(this->field_0x120, Globals::w >> 1, logoTop / 4, logoWidth / 2,
                                                 logoHeight / 2, 17, 20, 0);
                }
            }
            for (unsigned int i = 0; i < this->buttons->size(); i++)
                (*this->buttons)[i]->draw();
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                int id = (int) (*this->scrollEntries)[i]->id;
                if (id == 53) {
                    (*this->scrollEntries)[i]->draw();
                    if (Globals::inAppPurchaseSupported && !optionFlag(59)) {
                        // Spinning "new" badge on the DLC button.
                        int badgeSize = Globals::Canvas->GetImage2DWidth(this->image_0x124);
                        int inset = (int) ((float) (*this->scrollEntries)[i]->getWidth() * 0.25f);
                        if (Globals::retinaDisplay)
                            inset -= 5;
                        else if (Globals::iPadHD)
                            inset -= 4;
                        long long now = (long long) Globals::appManager->GetCurrentTimeMillis();
                        Globals::Canvas->DrawRegion2D(this->image_0x124, 0, 0, badgeSize, badgeSize, (float) now * 0.004f,
                                                      (int) ((float) badgeSize * 0.5f), (int) ((float) badgeSize * 0.5f),
                                                      (int) ((*this->scrollEntries)[i]->getPosition().x - (float) inset),
                                                      (int) ((*this->scrollEntries)[i]->getPosition().y - (float) inset));
                    }
                } else if (id == 23 || id == 24) {
                    (*this->scrollEntries)[i]->draw();
                }
            }
            break;
        }
        case 3: {
            // Options.
            Layout *layout = Globals::layout;
            if (!Globals::iPad) {
                layout->drawHeader(String(*Globals::gameText->getText(31)), false);
                for (unsigned int i = 0; i < this->optionsButtons->size(); i++)
                    (*this->optionsButtons)[i]->draw();
                break;
            }
            // The iPad shows the control and sound options on one page.
            layout->drawBG();
            Globals::layout->drawHeader(String(*Globals::gameText->getText(498)), false);
            int edgeWidth = Globals::Canvas->GetImage2DWidth(this->scrollbarImageId);
            int edgeHeight = Globals::Canvas->GetImage2DHeight(this->scrollbarImageId);
            int edgeCount = Globals::h / edgeHeight;
            int edgeY = 0;
            for (int i = 0; i <= edgeCount; i++) {
                Globals::Canvas->DrawImage2D(this->scrollbarImageId,
                                             Globals::layout->buttonInsetX + this->listTopY - edgeWidth, edgeY, 1);
                Globals::Canvas->DrawImage2D(this->scrollbarImageId,
                                             Globals::layout->buttonInsetX + this->listTopY + this->listBottomY, edgeY,
                                             0);
                edgeY += edgeHeight;
            }

            // Controls.
            layout = Globals::layout;
            int y = layout->field_0xc_leftMargin + layout->field_0x20_top;
            layout->drawBox(0, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, layout->field_0x1c,
                            String(*Globals::gameText->getText(498)));
            layout = Globals::layout;
            y = layout->field_0x1c + y + layout->field_0x2c_rowHeight;
            layout->drawBox(5, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, this->listEntryWidth,
                            String(""));
            Globals::Canvas->SetColor(optionFlag(17) || this->upButtonPressed ? 0xe6ad4affu : 0x2f4455ffu);
            Globals::Canvas->DrawImage2D(this->optionsImage_0x110,
                                         Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight / 2,
                                         y + arrowOffset * 2, 17, 20);
            Globals::Canvas->SetColor(!optionFlag(17) || this->downButtonPressed ? 0xe6ad4affu : 0x2f4455ffu);
            Globals::Canvas->DrawImage2D(this->optionsImage_0x10c,
                                         this->listTopY + this->listBottomY - Globals::layout->buttonInsetX -
                                             this->listEntryHeight / 2,
                                         y + arrowOffset, 17, 20);
            Globals::Canvas->SetColor(optionFlag(17) ? 0xffffffffu : 0xffffff2fu);
            int labelY = y - labelOffset;
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(490),
                Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight / 2 -
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  *Globals::gameText->getText(490)) / 2,
                this->listEntryWidth + labelY, false);
            Globals::Canvas->SetColor(optionFlag(17) ? 0xffffff2fu : 0xffffffffu);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(491),
                this->listTopY + this->listBottomY - Globals::layout->buttonInsetX - this->listEntryHeight / 2 -
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  *Globals::gameText->getText(491)) / 2,
                this->listEntryWidth + labelY, false);
            Globals::Canvas->SetColor(0xffffffffu);

            layout = Globals::layout;
            y = y + this->listEntryWidth + layout->field_0x2c_rowHeight;
            layout->drawBox(5, layout->buttonInsetX + this->listTopY, y, this->listEntryHeight, this->sliderSpacing,
                            String(""));
            layout = Globals::layout;
            layout->drawBox(5,
                            this->listTopY + layout->buttonInsetX + this->listEntryHeight + layout->field_0x2c_rowHeight +
                                1,
                            y, this->listEntryHeight, this->sliderSpacing, String(""));
            layout = Globals::layout;
            layout->drawBox(5, layout->buttonInsetX + this->listTopY, y + this->sliderSpacing + layout->field_0x2c_rowHeight,
                            this->listEntryHeight, this->sliderSpacing, String(""));
            layout = Globals::layout;
            layout->drawBox(5,
                            this->listTopY + layout->buttonInsetX + this->listEntryHeight + layout->field_0x2c_rowHeight +
                                1,
                            y + layout->field_0x2c_rowHeight + this->sliderSpacing, this->listEntryHeight,
                            this->sliderSpacing, String(""));
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(500),
                                        this->listTopY + Globals::layout->buttonInsetX + 10,
                                        y + this->sliderSpacing / 2 -
                                            Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 +
                                            1,
                                        false);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font,
                *Globals::gameText->getText(499) + String(" [") +
                    String(String((int) ((*this->sliders)[0]->getValue() * 100.0f))) + String("]"),
                this->listTopY + Globals::layout->buttonInsetX + 10,
                Globals::layout->field_0x2c_rowHeight + y + this->sliderSpacing + 10, false);
            (*this->sliders)[0]->draw();
            this->optBtnCC->setPosition(Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight - 10,
                                        y + this->sliderSpacing / 2);
            this->optBtnCC->draw();
            this->optBtnD0->setPosition(this->listTopY + this->listBottomY - Globals::layout->buttonInsetX -
                                            this->listEntryHeight / 2,
                                        y + this->sliderSpacing / 2);
            this->optBtnD0->draw();
            layout = Globals::layout;
            this->scrollUpButton->setPosition(this->listBottomY + this->listTopY - layout->buttonInsetX -
                                                  this->listEntryHeight / 2,
                                              layout->field_0x2c_rowHeight + y + this->sliderSpacing +
                                                  this->sliderSpacing / 2);
            this->scrollUpButton->draw();

            // Sound.
            layout = Globals::layout;
            y = y + this->sliderSpacing * 2 + layout->field_0x2c_rowHeight + layout->field_0x2fc;
            int toggleY = y + 7;
            this->optBtnD4->setPosition(this->listTopY + this->listBottomY - 25 - layout->buttonInsetX,
                                        toggleY + layout->field_0x2c_rowHeight + layout->field_0x1c);
            layout = Globals::layout;
            this->optBtnD8->setPosition(this->listTopY + this->listBottomY - 25 - layout->buttonInsetX,
                                        toggleY + this->sliderRowHeight + layout->field_0x1c +
                                            layout->field_0x2c_rowHeight * 2);
            layout = Globals::layout;
            this->optBtnDC->setPosition(this->listTopY + this->listBottomY - 25 - layout->buttonInsetX,
                                        toggleY + this->sliderRowHeight * 2 + layout->field_0x1c +
                                            layout->field_0x2c_rowHeight * 3);
            layout = Globals::layout;
            layout->drawBox(0, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, layout->field_0x1c,
                            String(*Globals::gameText->getText(501)));
            for (int line = 0; line < 3; line++) {
                layout = Globals::layout;
                if (line == 0)
                    y = y + layout->field_0x1c + layout->field_0x2c_rowHeight;
                else
                    y = this->sliderRowHeight + y + layout->field_0x2c_rowHeight;
                layout->drawBox(5, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, this->sliderRowHeight,
                                String(""));
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                            *Globals::gameText->getText(34 + line),
                                            this->listTopY + Globals::layout->buttonInsetX + 10,
                                            y + this->sliderRowHeight / 2 -
                                                Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) /
                                                    2 +
                                                1,
                                            false);
            }

            // Graphics.
            y = y + this->sliderRowHeight + Globals::layout->field_0x2fc;
            if (!Globals::iPadLargePossible) {
                layout = Globals::layout;
                layout->drawBox(0, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, layout->field_0x1c,
                                String(*Globals::gameText->getText(502)));
                layout = Globals::layout;
                y = y + layout->field_0x1c + layout->field_0x2c_rowHeight;
                layout->drawBox(5, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, this->sliderRowHeight,
                                String(""));
            }
            if (Globals::iPadLargePossible) {
                layout = Globals::layout;
                layout->drawBox(0, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, layout->field_0x1c,
                                String(*Globals::gameText->getText(502)));
                layout = Globals::layout;
                y = y + layout->field_0x1c + layout->field_0x2c_rowHeight;
                layout->drawBox(5, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, this->sliderRowHeight,
                                String(""));
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(504),
                                            this->listTopY + Globals::layout->buttonInsetX + 10,
                                            y + this->sliderSpacing / 2 -
                                                Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) /
                                                    2 +
                                                1,
                                            false);
                int valueY = y - sliderLabelOffset;
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String("1024x768"),
                                            Globals::layout->buttonInsetX + this->listTopY + this->metricC,
                                            this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, String("1440x1080"),
                    this->listTopY + Globals::layout->buttonInsetX + this->metricC + (*this->sliders)[5]->getWidth() / 2 -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String("1440x1080")) / 2,
                    this->sliderRowHeight + y - sliderLabelOffset, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, String("2048x1536"),
                    Globals::layout->buttonInsetX + this->listTopY + this->metricC + (*this->sliders)[5]->getWidth() -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String("2048x1536")),
                    this->sliderRowHeight + y - sliderLabelOffset, false);
            } else {
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(503),
                                            this->listTopY + Globals::layout->buttonInsetX + 10,
                                            y + this->sliderSpacing / 2 -
                                                Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) /
                                                    2 +
                                                1,
                                            false);
                int valueY = y - sliderLabelOffset;
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(513),
                                            Globals::layout->buttonInsetX + this->listTopY + this->metricC,
                                            this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(514),
                    Globals::layout->buttonInsetX + this->listTopY + this->metricC + (*this->sliders)[4]->getWidth() / 2 -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                      *Globals::gameText->getText(514)) / 2,
                    this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(515),
                    Globals::layout->buttonInsetX + this->listTopY + this->metricC + (*this->sliders)[4]->getWidth() -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                      *Globals::gameText->getText(515)),
                    this->sliderRowHeight + valueY, false);
                layout = Globals::layout;
                y = y + this->sliderRowHeight + layout->field_0x2c_rowHeight;
                layout->drawBox(5, layout->buttonInsetX + this->listTopY, y, this->optionsWidth, this->sliderRowHeight,
                                String(""));
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(504),
                                            this->listTopY + Globals::layout->buttonInsetX + 10,
                                            y + this->sliderSpacing / 2 -
                                                Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) /
                                                    2 +
                                                1,
                                            false);
                valueY = y - sliderLabelOffset;
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(507),
                                            Globals::layout->buttonInsetX + this->listTopY + this->metricC,
                                            this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(508),
                    Globals::layout->buttonInsetX + this->listTopY + this->metricC + (*this->sliders)[5]->getWidth() / 2 -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                      *Globals::gameText->getText(508)) / 2,
                    this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(509),
                    Globals::layout->buttonInsetX + this->listTopY + this->metricC + (*this->sliders)[5]->getWidth() -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                      *Globals::gameText->getText(509)),
                    this->sliderRowHeight + valueY, false);
            }
            for (unsigned int i = 1; i < this->sliders->size() - 1; i++)
                (*this->sliders)[i]->draw();
            this->optBtnD4->draw();
            this->optBtnD8->draw();
            this->optBtnDC->draw();
            break;
        }
        case 4:
            // About.
            Globals::layout->drawBG();
            Globals::layout->drawHeader(String(*Globals::gameText->getText(43)), false);
            this->scrollWindowA->draw();
            Globals::layout->drawEmptyFooter(true);
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                long long id = (*this->scrollEntries)[i]->id;
                if (id == 106 || id == 109 || id == 110 || id == 22)
                    (*this->scrollEntries)[i]->draw();
            }
            break;
        case 7: {
            // Sound options.
            Globals::layout->drawBG();
            Globals::layout->drawHeader(String(*Globals::gameText->getText(489)), false);
            Layout *layout = Globals::layout;
            int y = layout->field_0x20_top + layout->field_0xc_leftMargin;
            layout->drawBox(0, layout->buttonInsetX, y, this->optionsWidth, layout->field_0x1c,
                            String(*Globals::gameText->getText(501)));
            layout = Globals::layout;
            y = layout->field_0x1c + y + layout->field_0x2c_rowHeight;
            layout->drawBox(5, layout->buttonInsetX, y, this->optionsWidth, this->sliderSpacing, String(""));
            for (int line = 0; line < 3; line++) {
                if (line != 0) {
                    layout = Globals::layout;
                    y = this->sliderSpacing + y + layout->field_0x2c_rowHeight * 2;
                    layout->drawBox(5, layout->buttonInsetX, y, this->optionsWidth, this->sliderSpacing, String(""));
                }
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(34 + line),
                    Globals::layout->buttonInsetX + 10,
                    y + this->sliderSpacing / 2 -
                        Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 + 1,
                    false);
            }
            layout = Globals::layout;
            y = this->sliderSpacing + y + layout->field_0x2c_rowHeight * 2;
            layout->drawBox(0, layout->buttonInsetX, y, this->optionsWidth, layout->field_0x1c,
                            String(*Globals::gameText->getText(502)));
            layout = Globals::layout;
            y = layout->field_0x1c + y + layout->field_0x2c_rowHeight;
            layout->drawBox(5, layout->buttonInsetX, y, this->optionsWidth, this->sliderRowHeight, String(""));
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(503),
                                        Globals::layout->buttonInsetX + 10,
                                        y + this->sliderSpacing / 2 -
                                            Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 +
                                            1,
                                        false);
            int valueY = y - sliderLabelOffset;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(513),
                                        Globals::layout->buttonInsetX + this->metricC, this->sliderRowHeight + valueY,
                                        false);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(514),
                Globals::layout->buttonInsetX + this->metricC + (*this->sliders)[4]->getWidth() / 2 -
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  *Globals::gameText->getText(514)) / 2,
                this->sliderRowHeight + valueY, false);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(515),
                Globals::layout->buttonInsetX + this->metricC + (*this->sliders)[4]->getWidth() -
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  *Globals::gameText->getText(515)),
                this->sliderRowHeight + valueY, false);
            if (Globals::layout->field_0x284_sliderSlot5Enabled) {
                layout = Globals::layout;
                y = y + this->sliderRowHeight + layout->field_0x2c_rowHeight;
                layout->drawBox(5, layout->buttonInsetX, y, this->optionsWidth, this->sliderRowHeight, String(""));
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(504),
                    Globals::layout->buttonInsetX + 10,
                    y + this->sliderSpacing / 2 -
                        Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 + 1,
                    false);
                valueY = y - sliderLabelOffset;
                Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(507),
                                            Globals::layout->buttonInsetX + this->metricC,
                                            this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(508),
                    Globals::layout->buttonInsetX + this->metricC + (*this->sliders)[5]->getWidth() / 2 -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                      *Globals::gameText->getText(508)) / 2,
                    this->sliderRowHeight + valueY, false);
                Globals::Canvas->DrawString(
                    (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(509),
                    Globals::layout->buttonInsetX + this->metricC + (*this->sliders)[5]->getWidth() -
                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                      *Globals::gameText->getText(509)),
                    this->sliderRowHeight + valueY, false);
            }
            for (unsigned int i = 1; i < this->sliders->size() - 1; i++) {
                if (i == 5 && !Globals::layout->field_0x284_sliderSlot5Enabled)
                    continue;
                (*this->sliders)[i]->draw();
            }
            this->optBtnD4->draw();
            this->optBtnD8->draw();
            this->optBtnDC->draw();
            break;
        }
        case 8: {
            // Control options.
            Globals::layout->drawBG();
            Globals::layout->drawHeader(String(*Globals::gameText->getText(498)), false);
            Layout *layout = Globals::layout;
            layout->drawBox(5, layout->buttonInsetX, layout->field_0x20_top + layout->field_0xc_leftMargin,
                            this->optionsWidth, this->listEntryWidth, String(""));
            Globals::Canvas->SetColor(optionFlag(17) || this->upButtonPressed ? 0xe6ad4affu : 0x2f4455ffu);
            layout = Globals::layout;
            Globals::Canvas->DrawImage2D(this->optionsImage_0x110, layout->buttonInsetX + this->listEntryHeight / 2,
                                         layout->field_0xc_leftMargin + arrowOffset * 2 + layout->field_0x20_top, 17,
                                         20);
            Globals::Canvas->SetColor(!optionFlag(17) || this->downButtonPressed ? 0xe6ad4affu : 0x2f4455ffu);
            layout = Globals::layout;
            Globals::Canvas->DrawImage2D(this->optionsImage_0x10c,
                                         Globals::w - layout->buttonInsetX - this->listEntryHeight / 2,
                                         arrowOffset + layout->field_0xc_leftMargin + layout->field_0x20_top, 17, 20);
            Globals::Canvas->SetColor(optionFlag(17) ? 0xffffffffu : 0xffffff2fu);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(490),
                Globals::layout->buttonInsetX + this->listEntryHeight / 2 -
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  *Globals::gameText->getText(490)) / 2,
                Globals::layout->field_0xc_leftMargin - labelOffset + Globals::layout->field_0x20_top +
                    this->listEntryWidth,
                false);
            Globals::Canvas->SetColor(optionFlag(17) ? 0xffffff2fu : 0xffffffffu);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(491),
                Globals::w - Globals::layout->buttonInsetX - this->listEntryHeight / 2 -
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  *Globals::gameText->getText(491)) / 2,
                Globals::layout->field_0xc_leftMargin - labelOffset + Globals::layout->field_0x20_top +
                    this->listEntryWidth,
                false);
            Globals::Canvas->SetColor(0xffffffffu);
            layout = Globals::layout;
            layout->drawBox(5, layout->buttonInsetX,
                            layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth +
                                layout->field_0x2c_rowHeight,
                            this->listEntryHeight, this->sliderSpacing, String(""));
            layout = Globals::layout;
            layout->drawBox(5,
                            layout->buttonInsetX + this->listEntryHeight + layout->field_0x2c_rowHeight + 1,
                            layout->field_0xc_leftMargin + layout->field_0x2c_rowHeight + layout->field_0x20_top +
                                this->listEntryWidth,
                            this->listEntryHeight, this->sliderSpacing, String(""));
            layout = Globals::layout;
            layout->drawBox(5,
                            layout->buttonInsetX + this->listEntryHeight + layout->field_0x2c_rowHeight + 1,
                            layout->field_0xc_leftMargin + layout->field_0x2c_rowHeight * 2 + layout->field_0x20_top +
                                this->listEntryWidth + this->sliderSpacing,
                            this->listEntryHeight, this->sliderSpacing, String(""));
            layout = Globals::layout;
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(500), layout->buttonInsetX + 10,
                layout->field_0xc_leftMargin + layout->field_0x20_top + this->listEntryWidth +
                    layout->field_0x2c_rowHeight + this->sliderSpacing / 2 -
                    Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 + 1,
                false);
            Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font,
                *Globals::gameText->getText(499) + String(" [") +
                    String(String((int) ((*this->sliders)[0]->getValue() * 100.0f))) + String("]"),
                Globals::layout->buttonInsetX + this->listEntryHeight + Globals::layout->field_0x2c_rowHeight + 10,
                this->listEntryWidth + Globals::layout->field_0xc_leftMargin +
                    Globals::layout->field_0x2c_rowHeight * 2 + Globals::layout->field_0x20_top + this->sliderSpacing +
                    10,
                false);
            (*this->sliders)[0]->draw();
            this->optBtnCC->draw();
            this->optBtnD0->draw();
            break;
        }
        case 9:
            this->missionsWindow->draw();
            break;
        case 11: {
            // Touch-control preview (iPad).
            if (!Globals::iPad || Globals::iPadAssetsWithLowerRes)
                break;
            int barWidth = Globals::Canvas->GetImage2DWidth(this->fireImage_0x54);
            int segmentHeight = Globals::Canvas->GetImage2DHeight(this->fireImage_0x50);
            int arrowHeight = Globals::Canvas->GetImage2DHeight(this->fireImage_0x5c);
            int steerX = this->steerCoord_0x40 - barWidth / 2 +
                         Globals::Canvas->GetImage2DWidth(this->steerImageA) / 2;
            int fireX = this->fireCoord_0x60 - barWidth / 2 + Globals::Canvas->GetImage2DWidth(this->fireSize_0x74) / 2;
            int steerY = this->steerCoord_0x42 + Globals::Canvas->GetImage2DHeight(this->steerImageA) / 2;
            int fireY = this->fireCoord_0x62 + Globals::Canvas->GetImage2DHeight(this->fireSize_0x74) / 2;
            int segmentY = (int) ((float) (Globals::Canvas->GetImage2DHeight(this->fireImage_0x54) + 100) -
                                  (Globals::iPadHD ? 91.40625f : (Globals::iPadLarge ? 130.0f : 65.0f)));
            for (int bar = 0; bar != 2; bar++) {
                int x = bar == 0 ? steerX : fireX;
                Globals::Canvas->DrawImage2D(
                    this->fireImage_0x54, x,
                    (int) (100.0f - (Globals::iPadHD ? 91.40625f : (Globals::iPadLarge ? 130.0f : 65.0f))));
                int segY = segmentY;
                for (int n = 0; n < 5; n++) {
                    Globals::Canvas->DrawImage2D(this->fireImage_0x50, x, segY);
                    segY += segmentHeight;
                }
            }
            Globals::Canvas->DrawImage2D(
                this->cinematicTouchIdA != 0 ? this->fireImage_0x58 : this->fireImage_0x5c, steerX + 3,
                (int) ((float) steerY - (Globals::iPadHD ? 239.0625f : (Globals::iPadLarge ? 340.0f : 170.0f))));
            Globals::Canvas->DrawImage2D(
                this->cinematicTouchIdB != 0 ? this->fireImage_0x58 : this->fireImage_0x5c, fireX + 3,
                (int) ((float) fireY - (Globals::iPadHD ? 225.0f : (Globals::iPadLarge ? 320.0f : 160.0f))));
            Globals::Canvas->DrawImage2D(
                this->cinematicTouchIdA != 0 ? this->fireImage_0x58 : this->fireImage_0x5c, steerX + 3,
                (int) ((Globals::iPadHD ? 239.0625f : (Globals::iPadLarge ? 340.0f : 170.0f)) + (float) steerY -
                       (float) arrowHeight),
                2);
            Globals::Canvas->DrawImage2D(
                this->cinematicTouchIdB != 0 ? this->fireImage_0x58 : this->fireImage_0x5c, fireX + 3,
                (int) ((Globals::iPadHD ? 225.0f : (Globals::iPadLarge ? 320.0f : 160.0f)) + (float) fireY -
                       (float) arrowHeight),
                2);
            Globals::Canvas->DrawImage2D(this->steerImageA, this->steerCoord_0x40, this->steerCoord_0x42);
            Globals::Canvas->DrawImage2D(this->steerImageD, this->steerCoord_0x3c, this->steerCoord_0x3e, 17, 68);
            Globals::Canvas->DrawImage2D(this->steerImageC, this->steerCoord_0x2e, this->steerCoord_0x30);
            Globals::Canvas->DrawImage2D(this->steerImageB, this->steerCoord_0x34, this->steerCoord_0x32);
            Globals::Canvas->DrawImage2D(this->fireSize_0x74, this->fireCoord_0x60, this->fireCoord_0x62);
            Globals::Canvas->DrawImage2D(this->fireImage_0x80, this->fireCoord_0x64, this->fireCoord_0x66, 17, 68);
            Globals::Canvas->DrawImage2D(this->fireImage_0x84, this->fireCoord_0x68, this->fireCoord_0x6a);
            Globals::Canvas->DrawImage2D(this->fireImage_0x88, this->fireCoord_0x6c, this->fireCoord_0x6e);
            Globals::Canvas->DrawImage2D(this->fireImage_0x8c, this->fireCoord_0x70, this->fireCoord_0x72);
            Layout *layout = Globals::layout;
            layout->drawBox(7, (Globals::w >> 1) - layout->field_0x304 / 2, layout->field_0x300, layout->field_0x304,
                            layout->choiceWindowTitleHeight, String(*Globals::gameText->getText(494)));
            this->cinematicTextWindow->draw();
            this->cinematicBtnA->draw();
            this->cinematicBtnB->draw();
            break;
        }
        case 12:
            // Difficulty.
            if (Globals::iPad && !Globals::iPadAssetsWithLowerRes)
                Globals::Canvas->DrawImage2D(this->field_0x120, Globals::w >> 1, 80, 17, 20);
            Globals::layout->drawHeader(String(*Globals::gameText->getText(517)), false);
            for (unsigned int i = 0; i < this->buttonsB4->size(); i++)
                (*this->buttonsB4)[i]->draw();
            break;
        case 13: {
            // Screenshot.
            Globals::Canvas->DrawImage2D(this->field_0x120, Globals::w - Globals::layout->field_0x2c_rowHeight,
                                         Globals::layout->field_0x2c_rowHeight, 17, 18);
            if (this->storeInitDialogShowing && this->field_0x1c4 && this->field_0x188) {
                ((MtwAppData *) Globals::appManager->GetApplicationData())->storeInitFlag = 1;
                ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->SaveImageToPhotosAlbum();
                this->field_0x1c4 = 0;
                this->field_0x188 = 0;
                this->storeInitDialogShowing = 0;
            } else if (this->screenshotState < 0) {
                if (!((MtwAppData *) Globals::appManager->GetApplicationData())->screenshotPending &&
                    !((MtwAppData *) Globals::appManager->GetApplicationData())->storeInitFlag)
                    Globals::layout->drawEmptyFooter(true);
            } else {
                if (this->field_0x188) {
                    // Caption for the shared screenshot: ship, station, system.
                    GameData *data = (GameData *) Globals::appManager->GetApplicationData();
                    data->field_0x18 = *Globals::gameText->getText(Globals::status->getShip()->getIndex() + 913);
                    if (Globals::status->inAlienOrbit()) {
                        data->field_0x24 = String("Void mothership");
                        data->field_0x30 = String("Void");
                    } else {
                        data->field_0x24 = Globals::status->getStation()->getName();
                        data->field_0x30 = ((SolarSystem *) Globals::status->getSystem())->getName();
                    }
                    ((MtwAppData *) data)->screenshotPending = 1;
                    this->field_0x188 = 0;
                }
                if (this->screenshotState == 1 && !this->field_0x1c4 &&
                    !((MtwAppData *) Globals::appManager->GetApplicationData())->screenshotPending &&
                    !((MtwAppData *) Globals::appManager->GetApplicationData())->storeInitFlag) {
                    ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->field_0xff = 1;
                    this->field_0x188 = 1;
                    this->field_0x1c4 = 1;
                }
            }
            this->screenshotState--;
            break;
        }
        case 14:
            // Language.
            if (Globals::iPad && !Globals::iPadAssetsWithLowerRes)
                Globals::Canvas->DrawImage2D(this->field_0x120, Globals::w >> 1, 80, 17, 20);
            Globals::layout->drawHeader(String(*Globals::gameText->getText(0)), false);
            for (unsigned int i = 0; i < this->buttonsB0->size(); i++) {
                // Language names are drawn in their own script: Arabic left-to-right switch.
                bool arabicButton = (*this->buttonsB0)[i]->id == 34 && GameText::getLanguage() != 9;
                bool latinButton = ((*this->buttonsB0)[i]->id == 33 || (*this->buttonsB0)[i]->id == 36 ||
                                    (*this->buttonsB0)[i]->id == 37) &&
                                   GameText::getLanguage() == 9;
                bool switchScript = latinButton || arabicButton;
                if (switchScript)
                    Globals::Canvas->field_0x1c = !arabicButton;
                (*this->buttonsB0)[i]->draw();
                if (switchScript)
                    Globals::Canvas->field_0x1c = arabicButton;
            }
            break;
        case 15: {
            // DLC store.
            Globals::layout->drawBG();
            Globals::layout->drawHeader(String(*Globals::gameText->getText(97)), false);
            Globals::Canvas->SetColor(0x255d8dffu);
            Globals::layout->drawEmptyFooter(true);
            Layout *layout = Globals::layout;
            int listHeight = Globals::h - layout->field_0xc_leftMargin - layout->field_0x10_rightMargin -
                             layout->field_0x20_top - layout->field_0x24;
            layout->drawBox(5, layout->buttonInsetX, layout->field_0x20_top + layout->field_0xc_leftMargin,
                            this->slotListWidth, listHeight, String(""));
            layout = Globals::layout;
            layout->drawBox(5, this->slotListWidth + layout->buttonInsetX + this->slotBorderWidth,
                            layout->field_0x20_top + layout->field_0xc_leftMargin, this->slotTextWidth, listHeight,
                            String(""));
            layout = Globals::layout;
            Globals::Canvas->EnableClip(0, layout->field_0xc_leftMargin + layout->field_0x20_top, Globals::w,
                                        Globals::h - (layout->field_0xc_leftMargin + layout->field_0x20_top) -
                                            layout->field_0x10_rightMargin - layout->field_0x24);
            this->contentHeightCache = Globals::Canvas->GetImage2DHeight(this->heapBufA[0]);
            MtwAppData *appData = (MtwAppData *) Globals::appManager->GetApplicationData();
            for (int i = 0; i != 5; i++) {
                if (i == this->field_0x1e0) {
                    if (!Globals::iPad && !Globals::iPadAssetsWithLowerRes)
                        Globals::Canvas->DrawImage2D(this->scrollImage_0x13c,
                                                     Globals::layout->field_0x2c_rowHeight + this->slotImageWidth +
                                                         this->slotX,
                                                     (this->slotSpacing + this->contentHeightCache) * i +
                                                         this->scrollOffset + this->slotY);
                    (*this->scrollSlots)[i]->setAlwaysPressed(true);
                } else {
                    (*this->scrollSlots)[i]->setAlwaysPressed(false);
                }
                layout = Globals::layout;
                (*this->scrollSlots)[i]->setYPosition((layout->field_0x2c_rowHeight + this->contentHeightCache) * i +
                                                      layout->field_0x20_top + layout->field_0xc_leftMargin +
                                                      this->scrollOffset);
                (*this->scrollSlots)[i]->draw();
                if ((i == 0 && optionFlag(53)) || (i == 1 && optionFlag(54)) || (i == 2 && optionFlag(55)) ||
                    (i == 3 && optionFlag(56)) || (i == 4 && ((optionFlag(53) && optionFlag(55)) || optionFlag(57)))) {
                    // Already owned: tick mark.
                    Globals::Canvas->DrawImage2D(
                        this->scrollImage_0x144,
                        (int) ((*this->scrollSlots)[i]->getPosition().x + (float) this->slotImageWidth),
                        (int) ((*this->scrollSlots)[i]->getPosition().y + (float) this->contentHeightCache), 17, 34);
                }
            }
            layout = Globals::layout;
            layout->drawBox(0, this->slotListWidth + layout->buttonInsetX + this->slotBorderWidth,
                            layout->field_0x20_top + layout->field_0xc_leftMargin,
                            this->slotTextWidth - layout->field_0x2c_rowHeight - this->slotMarkerWidth,
                            layout->field_0x1c, String(*Globals::gameText->getText(this->field_0x1e0 + 77)));
            layout = Globals::layout;
            Globals::Canvas->DrawImage2D(this->heapBufB[this->field_0x1e0], Globals::w - layout->buttonInsetX,
                                         layout->field_0x20_top + layout->field_0xc_leftMargin, 17, 18);
            layout = Globals::layout;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                        *Globals::gameText->getText(this->field_0x1e0 + 82),
                                        this->slotBorderWidth + layout->buttonInsetX + this->slotListWidth +
                                            layout->field_0x2c_rowHeight,
                                        layout->field_0xc_leftMargin + layout->field_0x2c_rowHeight +
                                            layout->field_0x20_top + layout->field_0x1c,
                                        false);
            Globals::Canvas->SetColor(0x7aa35bffu);
            if (appData->prices != 0 && appData->prices->size() != 0) {
                int priceIndex = Globals::globals->getInAppPurchaseArrayIndex(this->field_0x1e0, appData->productIds);
                if (priceIndex != -1) {
                    String *price = (*appData->prices)[priceIndex];
                    layout = Globals::layout;
                    Globals::Canvas->DrawString(
                        (unsigned int) (uintptr_t) Globals::font, *price,
                        this->slotListWidth + layout->buttonInsetX + this->slotBorderWidth +
                            layout->field_0x2c_rowHeight * 2 +
                            Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                          *Globals::gameText->getText(132) + String(":")),
                        Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                            Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight +
                            Globals::layout->field_0x4,
                        false);
                }
            }
            this->scrollWindowB->draw();
            int selected = this->field_0x1e0;
            bool owned = (selected == 0 && optionFlag(53)) || (selected == 1 && optionFlag(54)) ||
                         (selected == 2 && optionFlag(55)) || (selected == 3 && optionFlag(56)) ||
                         (selected == 4 && optionFlag(57)) || ((selected == 0 || selected == 2) && optionFlag(57)) ||
                         (selected == 4 && optionFlag(53) && optionFlag(55));
            Globals::Canvas->DisableClip();
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                TouchButton *button = (*this->scrollEntries)[i];
                if (button->id == 60) {
                    button->draw();
                } else if (button->id == 52) {
                    button->setVisible(!owned);
                    button->setText(*Globals::gameText->getText(98));
                    button->draw();
                }
            }
            break;
        }
        case 16:
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                long long id = (*this->scrollEntries)[i]->id;
                if (id >= 101 && id <= 105)
                    (*this->scrollEntries)[i]->draw();
            }
            break;
        case 17: {
            // Campaign selection.
            if (Globals::iPad && !Globals::iPadAssetsWithLowerRes)
                Globals::Canvas->DrawImage2D(this->field_0x120, Globals::w >> 1, 80, 17, 20);
            Globals::layout->drawHeader(String(*Globals::gameText->getText(103)), false);
            for (unsigned int i = 0; i < this->buttonsB8->size(); i++)
                (*this->buttonsB8)[i]->draw();
            Vector pos = (*this->buttonsB8)[0]->getPosition();
            Globals::Canvas->DrawImage2D(
                this->image_0x128,
                (int) (pos.x + (float) (*this->buttonsB8)[0]->getWidth() -
                       (float) (Globals::layout->field_0x2c_rowHeight * 20)),
                (int) (pos.y + (float) (Globals::layout->field_0x2c_rowHeight * 5)), 68);
            break;
        }
    }

    if (this->menuState != 11 && (this->backgroundEnabled != 0 || this->menuState != 0) && this->menuState != 4 &&
        this->menuState != 13 && this->menuState != 15)
        Globals::layout->drawEmptyFooter(true);
    if (Globals::layout->field_0x285 && (this->backgroundEnabled == 0 || this->backgroundEnabled == 2) &&
        this->menuState == 0) {
        for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
            if ((*this->scrollEntries)[i]->id == 17)
                (*this->scrollEntries)[i]->draw();
        }
    }
    if (this->messageShowing)
        this->choiceWindow->draw();
    if (Globals::layout->choiceWindowOpen)
        Globals::layout->drawHelpWindow();
}

void MenuTouchWindow::update(int dt) {
    if (this->menuState == 1 || this->menuState == 2 || this->menuState == 15) {
        // Inertial scrolling with spring-back at both ends.
        if (!this->dragging) {
            this->inertiaVel = this->inertiaDecay * this->inertiaVel;
            if (MTW_ABS(this->inertiaVel) > 1.0f)
                this->scrollOffset = (int) (this->inertiaVel + (float) this->scrollOffset);
        }
        if (this->scrollOffset >= 1) {
            this->inertiaDecay = 1.0f;
            this->inertiaVel = (float) -this->scrollOffset * 0.5f;
        }
        int limit = this->contentHeight - this->pageHeight;
        if (limit <= -1) {
            if (limit > this->scrollOffset) {
                this->inertiaDecay = 1.0f;
                this->inertiaVel = (float) (limit - this->scrollOffset) * 0.5f;
            }
        } else {
            this->scrollOffset = 0;
            this->inertiaVel = 0.0f;
        }
    }

    MtwAppData *appData = (MtwAppData *) Globals::appManager->GetApplicationData();
    if ((this->dlcMessageShowing || this->purchaseRestorePending) && appData->purchaseReadyFlag) {
        // Store purchase finished: open the supernova/DLC list.
        this->contentHeight = Globals::h - Globals::layout->field_0x10_rightMargin -
                              Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x20_top -
                              Globals::layout->field_0x24;
        this->contentHeightCache = Globals::Canvas->GetImage2DHeight(this->heapBufA[0]);
        this->pageHeight = (this->contentHeightCache + Globals::layout->field_0x2c_rowHeight) * 5;
        this->menuState = 15;
        this->messageShowing = 0;
        this->dlcMessageShowing = 0;
        appData->purchaseReadyFlag = 0;
        Globals::options[59] = 1;
        Globals::recordHandler->saveOptions();
    }
    if ((this->dlcMessageShowing || this->purchaseRestorePending) && appData->purchaseErrorFlag) {
        this->choiceWindow->set(*Globals::gameText->getText(100));
        this->messageShowing = 1;
        this->dlcErrorDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->field_0x17b = 0;
        appData->purchaseErrorFlag = 0;
        this->purchaseRestorePending = 0;
    }
    if (appData->purchaseCode <= 4 && appData->purchaseResultFlag) {
        switch (appData->purchaseCode) {
        case 0:
            Globals::options[53] = 1;
            Globals::status->setSystemVisibility(25, true);
            this->choiceWindow->set(*Globals::gameText->getText(92));
            break;
        case 1:
            Globals::status->field_114 = 3;
            Globals::options[54] = 1;
            this->choiceWindow->set(*Globals::gameText->getText(93));
            break;
        case 2:
            Globals::options[55] = 1;
            Globals::status->setSystemVisibility(25, true);
            this->choiceWindow->set(*Globals::gameText->getText(94));
            break;
        case 3:
            Globals::options[56] = 1;
            this->choiceWindow->set(*Globals::gameText->getText(95));
            break;
        case 4:
            Globals::options[57] = 1;
            Globals::options[53] = 1;
            Globals::status->setSystemVisibility(25, true);
            Globals::options[55] = 1;
            this->choiceWindow->set(*Globals::gameText->getText(96));
            break;
        }
        Globals::recordHandler->saveOptions();
        this->messageShowing = 1;
        this->dlcMessageShowing = 0;
        this->dlcResultDialogShowing = 1;
        appData->purchaseResultFlag = 0;
    } else if (this->purchaseRestorePending && appData->purchaseResultFlag) {
        Globals::recordHandler->saveOptions();
        if (this->purchaseRestorePending) {
            this->choiceWindow->set(*Globals::gameText->getText(76));
            this->purchaseRestorePending = 0;
        }
        this->messageShowing = 1;
        this->dlcMessageShowing = 0;
        this->dlcResultDialogShowing = 1;
        appData->purchaseResultFlag = 0;
    }
    if (Globals::isTelekomCustomer && (uint8_t) (*(volatile uint8_t *) &this->messageShowing | Globals::options[53]) == 0) {
        this->choiceWindow->set(*Globals::gameText->getText(3234));
        this->messageShowing = 1;
        this->supernovaPurchaseDialogShowing = 1;
        Globals::options[53] = 1;
        Globals::status->setSystemVisibility(25, true);
        Globals::recordHandler->saveOptions();
    }

    switch (this->menuState) {
    case 0:
        if (this->backgroundEnabled == 1) {
            // Centre the main menu buttons vertically.
            int top = Globals::h / 2 - (this->buttons->size() * Globals::layout->field_0x30_rowHeight +
                                        this->buttonRowGap * (this->buttons->size() - 1)) / 2;
            for (unsigned int i = 0; i < this->buttons->size(); i++) {
                (*this->buttons)[i]->setYPosition((Globals::layout->field_0x30_rowHeight + this->buttonRowGap) * i + top);
                if ((*this->buttons)[i]->id == 18 && (*this->buttons)[i]->isVisible() && this->cutsceneMode)
                    (*this->buttons)[i]->setYPosition((int) (*this->buttons)[i - 1]->getPosition().y);
            }
        }
        break;
    case 3: {
        int top = Globals::h / 2 - (this->optionsButtons->size() * Globals::layout->field_0x30_rowHeight +
                                    (this->buttons->size() - 1) * this->buttonRowGap) / 2;
        for (unsigned int i = 0; i < this->optionsButtons->size(); i++)
            (*this->optionsButtons)[i]->setYPosition((Globals::layout->field_0x30_rowHeight + this->buttonRowGap) * i +
                                                     top);
        break;
    }
    case 4:
        this->scrollWindowA->update(dt);
        break;
    case 9:
        if (this->missionsWindow != 0)
            this->missionsWindow->update(dt);
        break;
    case 15:
        this->scrollWindowB->update(dt);
        break;
    }
    if (this->messageShowing)
        this->choiceWindow->update(dt);

    if (this->menuState == 13) {
        if (((MtwAppData *) Globals::appManager->GetApplicationData())->storeInitFlag) {
            int state = ((Engine *) Globals::appManager->GetEngine())->field_0x100;
            if (state == 1) {
                this->choiceWindow->set(*Globals::gameText->getText(55));
                this->messageShowing = 1;
                this->storeInitDialogShowing = 0;
                ((MtwAppData *) Globals::appManager->GetApplicationData())->storeInitFlag = 0;
            } else if (state == 2) {
                this->choiceWindow->set(*Globals::gameText->getText(56));
                this->messageShowing = 1;
                this->storeInitDialogShowing = 0;
                ((MtwAppData *) Globals::appManager->GetApplicationData())->storeInitFlag = 0;
            }
        }
        if (((MtwAppData *) Globals::appManager->GetApplicationData())->screenshotResultFlag) {
            int result = ((MtwAppData *) Globals::appManager->GetApplicationData())->storeResultCode;
            if (result == 1) {
                this->choiceWindow->set(*Globals::gameText->getText(57));
                this->messageShowing = 1;
                this->screenshotState = -1;
                ((MtwAppData *) Globals::appManager->GetApplicationData())->screenshotResultFlag = 0;
            } else if (result == 2) {
                this->choiceWindow->set(*Globals::gameText->getText(58));
                this->messageShowing = 1;
                this->screenshotState = -1;
                ((MtwAppData *) Globals::appManager->GetApplicationData())->screenshotResultFlag = 0;
            }
        }
    }
}

bool MenuTouchWindow::OnTouchBegin(int x, int y, void *touchId) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->messageShowing) {
        this->choiceWindow->OnTouchBegin(x, y);
        return false;
    }
    if (Globals::layout->choiceWindowOpen) {
        Globals::layout->OnTouchBegin(x, y);
        return false;
    }
    switch (this->menuState) {
    case 1:
    case 2: {
        // Load / save list.
        this->dragLastX = y;
        this->dragStartX = y;
        this->dragVelocity = 0;
        this->dragging = 1;
        int previous = this->selectedRow;
        if (y > Globals::layout->field_0xc_leftMargin && y < Globals::h - Globals::layout->field_0x10_rightMargin) {
            int row = (y - Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x20_top - this->scrollOffset) /
                      (Globals::layout->field_0x70_rowHeight + this->listRowGap);
            if (row < Globals::recordSlots) {
                this->selectedRow = row;
                this->okButton->setPosition(
                    Globals::w - this->listX - Globals::layout->buttonInsetX,
                    row * (Globals::layout->field_0x70_rowHeight + this->listRowGap) +
                        (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin + this->scrollOffset) +
                        Globals::layout->field_0x108);
                Globals::sound->play(124, 0, 0, 0.0f);
            }
        }
        if (previous == this->selectedRow)
            this->okButton->OnTouchBegin(x, y);
        break;
    }
    case 3:
        // Options.
        if (Globals::iPad) {
            this->scrollUpButton->OnTouchBegin(x, y);
                this->upButtonPressed = 0;
                this->downButtonPressed = 0;
                if (x > this->listTopY + Globals::layout->buttonInsetX &&
                    x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                    y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth) {
                    this->upButtonPressed = 1;
                    Globals::sound->play(124, 0, 0, 0.0f);
                }
                if (x > this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight &&
                    x < this->listTopY - Globals::layout->buttonInsetX + this->listBottomY &&
                    y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth) {
                    this->downButtonPressed = 1;
                    Globals::sound->play(124, 0, 0, 0.0f);
                }
            this->optBtnCC->OnTouchBegin(x, y);
            this->optBtnD0->OnTouchBegin(x, y);
            (*this->sliders)[0]->OnTouchBegin(x, y);
            this->optBtnD4->OnTouchBegin(x, y);
            this->optBtnD8->OnTouchBegin(x, y);
            this->optBtnDC->OnTouchBegin(x, y);
            for (unsigned int i = 1; i < this->sliders->size(); i++)
                (*this->sliders)[i]->OnTouchBegin(x, y);
            if (Globals::iPadLargePossible && this->scrollExtraButton != 0)
                this->scrollExtraButton->OnTouchBegin(x, y);
        } else {
            for (unsigned int i = 0; i < this->optionsButtons->size(); i++)
                (*this->optionsButtons)[i]->OnTouchBegin(x, y);
        }
        break;
    case 4:
        this->scrollWindowA->OnTouchBegin(x, y);
        for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
            long long id = (*this->scrollEntries)[i]->id;
            if (id == 106 || id == 109 || id == 110 || id == 22)
                (*this->scrollEntries)[i]->OnTouchBegin(x, y);
        }
        break;
    case 16:
        for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
            long long id = (*this->scrollEntries)[i]->id;
            if (id >= 101 && id <= 105)
                (*this->scrollEntries)[i]->OnTouchBegin(x, y);
        }
        // fall through
    case 15:
        this->scrollWindowB->OnTouchBegin(x, y);
        for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
            long long id = (*this->scrollEntries)[i]->id;
            if (id - 52 == 0 || id - 60 == 0)
                (*this->scrollEntries)[i]->OnTouchBegin(x, y);
        }
        for (unsigned int i = 0; i < this->scrollSlots->size(); i++)
            (*this->scrollSlots)[i]->OnTouchBegin(x, y);
        if (x > Globals::w - Globals::layout->buttonInsetX -
                    Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
            y < ((*(decltype(Globals::layout) volatile *) &Globals::layout)->field_0x20_top +
                  (*(decltype(Globals::layout) volatile *) &Globals::layout)->field_0xc_leftMargin) +
                    Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]))
            this->scrollbarHit = 1;
        else
            this->scrollbarHit = 0;
        if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
            this->dragLastX = y;
            this->dragStartX = y;
            this->dragVelocity = 0;
            this->dragging = 1;
        }
        break;
    case 9:
        this->missionsWindow->OnTouchBegin(x, y);
        break;
    case 11:
        // Touch control preview: grab the steering pad (left) or the fire buttons (right).
        if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
            this->cinematicBtnA->OnTouchBegin(x, y);
            this->cinematicBtnB->OnTouchBegin(x, y);
            auto touchA = this->cinematicTouchIdA;
            auto touchB = this->cinematicTouchIdB;
            if (touchA == 0 && touchId != 0 && touchB != touchId && x <= 209 &&
                y > *(int *) &Globals::options[84] - 20 && y < *(int *) &Globals::options[84] + 300) {
                this->cinematicTouchState = 1;
                this->cinematicTouchIdA = touchId;
                this->cinematicAnchorA = y;
            } else if (touchId != 0 && this->cinematicTouchIdA != touchId) {
                if (touchB == 0 && x > Globals::w - 220 &&
                    y > *(int *) &Globals::options[88] - 20 && y < *(int *) &Globals::options[88] + 230) {
                    this->cinematicTouchIdB = touchId;
                    this->cinematicTouchState = 0x100;
                    this->cinematicAnchorB = y;
                } else {
                    this->cinematicTouchState = 0;
                }
            } else {
                this->cinematicTouchState = 0;
            }
        }
        break;
    case 8:
            this->upButtonPressed = 0;
            this->downButtonPressed = 0;
            if (x > this->listTopY + Globals::layout->buttonInsetX &&
                x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth) {
                this->upButtonPressed = 1;
                Globals::sound->play(124, 0, 0, 0.0f);
            }
            if (x > this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight &&
                x < this->listTopY - Globals::layout->buttonInsetX + this->listBottomY &&
                y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth) {
                this->downButtonPressed = 1;
                Globals::sound->play(124, 0, 0, 0.0f);
            }
        this->optBtnCC->OnTouchBegin(x, y);
        this->optBtnD0->OnTouchBegin(x, y);
        (*this->sliders)[0]->OnTouchBegin(x, y);
        break;
    case 12:
        for (unsigned int i = 0; i < this->buttonsB4->size(); i++)
            (*this->buttonsB4)[i]->OnTouchBegin(x, y);
        break;
    case 14:
        for (unsigned int i = 0; i < this->buttonsB0->size(); i++)
            (*this->buttonsB0)[i]->OnTouchBegin(x, y);
        break;
    case 6:
    case 13:
        break;
    case 7:
        this->optBtnD4->OnTouchBegin(x, y);
        this->optBtnD8->OnTouchBegin(x, y);
        this->optBtnDC->OnTouchBegin(x, y);
        for (unsigned int i = 1; i < this->sliders->size(); i++) {
            if ((i != 5) | (Globals::layout->field_0x284_sliderSlot5Enabled != 0)) {
                (*this->sliders)[i]->OnTouchBegin(x, y);
            }
        }
        break;
    case 17:
        for (unsigned int i = 0; i < this->buttonsB8->size(); i++)
            (*this->buttonsB8)[i]->OnTouchBegin(x, y);
        break;
    default:
        for (unsigned int i = 0; i < this->buttons->size(); i++)
            (*this->buttons)[i]->OnTouchBegin(x, y);
        for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
            int id = (int) (*this->scrollEntries)[i]->id;
            if (id == 23 || id == 24)
                (*this->scrollEntries)[i]->OnTouchBegin(x, y);
        }
        for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
            long long id = (*this->scrollEntries)[i]->id;
            if (id == 5 || id == 17 || id == 100 || id == 53)
                (*this->scrollEntries)[i]->OnTouchBegin(x, y);
        }
        break;
    }
    if (Globals::layout->OnTouchBegin(x, y) && this->menuState == 13)
        this->pendingActivate = 1;
    return false;
}

// lint: void_ptr (external symbol; param type is mangling-load-bearing)



bool MenuTouchWindow::OnTouchMove(int x, int y, void *touchId) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->messageShowing) {
        this->choiceWindow->OnTouchMove(x, y);
        return false;
    }
    if (Globals::layout->choiceWindowOpen) {
        Globals::layout->OnTouchMove(x, y);
        return false;
    }
    switch (this->menuState) {
        case 1:
        case 2:
            // Load / save list.
            if (y > Globals::layout->field_0xc_leftMargin && y < Globals::h - Globals::layout->field_0x10_rightMargin) {
                int delta = y - this->dragLastX;
                this->dragLastX = y;
                this->dragVelocity = delta;
                this->inertiaDecay = 1.0f;
                this->scrollOffset += delta;
            }
            this->okButton->OnTouchMove(x, y);
            break;
        case 3:
            // Options.
            if (Globals::iPad) {
                this->scrollUpButton->OnTouchMove(x, y);
                this->upButtonPressed = 0;
                this->downButtonPressed = 0;
                if (x > this->listTopY + Globals::layout->buttonInsetX &&
                    x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                    y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth)
                    this->upButtonPressed = 1;
                if (x > this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight &&
                    x < this->listTopY - Globals::layout->buttonInsetX + this->listBottomY &&
                    y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth)
                    this->downButtonPressed = 1;
                this->optBtnCC->OnTouchMove(x, y);
                this->optBtnD0->OnTouchMove(x, y);
                (*this->sliders)[0]->OnTouchMove(x, y);
                this->optBtnD4->OnTouchMove(x, y);
                this->optBtnD8->OnTouchMove(x, y);
                this->optBtnDC->OnTouchMove(x, y);
                if (!Globals::sound->tryToStopMusicForBGMusic())
                    Globals::sound->setVolume(1, (*this->sliders)[1]->getValue());
                Globals::sound->setVolume(2, (*this->sliders)[2]->getValue());
                Globals::sound->setVolume(3, (*this->sliders)[3]->getValue());
                for (unsigned int i = 1; i < this->sliders->size(); i++)
                    (*this->sliders)[i]->OnTouchMove(x, y);
                if (Globals::iPadLargePossible && this->scrollExtraButton != 0)
                    this->scrollExtraButton->OnTouchMove(x, y);
            } else {
                for (unsigned int i = 0; i < this->optionsButtons->size(); i++)
                    (*this->optionsButtons)[i]->OnTouchMove(x, y);
            }
            break;
        case 4:
            this->scrollWindowA->OnTouchMove(x, y);
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                long long id = (*this->scrollEntries)[i]->id;
                if (id == 106 || id == 109 || id == 110 || id == 22)
                    (*this->scrollEntries)[i]->OnTouchMove(x, y);
            }
            break;
        case 6:
        case 13:
            break;
        case 7:
            this->optBtnD4->OnTouchMove(x, y);
            this->optBtnD8->OnTouchMove(x, y);
            this->optBtnDC->OnTouchMove(x, y);
            if (!Globals::sound->tryToStopMusicForBGMusic())
                Globals::sound->setVolume(1, (*this->sliders)[1]->getValue());
            Globals::sound->setVolume(2, (*this->sliders)[2]->getValue());
            Globals::sound->setVolume(3, (*this->sliders)[3]->getValue());
            for (unsigned int i = 1; i < this->sliders->size(); i++) {
                if (i != 5 || Globals::layout->field_0x284_sliderSlot5Enabled)
                    (*this->sliders)[i]->OnTouchMove(x, y);
            }
            break;
        case 8:
            this->upButtonPressed = 0;
            this->downButtonPressed = 0;
            if (x > this->listTopY + Globals::layout->buttonInsetX &&
                x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth)
                this->upButtonPressed = 1;
            if (x > this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight &&
                x < this->listTopY - Globals::layout->buttonInsetX + this->listBottomY &&
                y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth)
                this->downButtonPressed = 1;
            this->optBtnCC->OnTouchMove(x, y);
            this->optBtnD0->OnTouchMove(x, y);
            (*this->sliders)[0]->OnTouchMove(x, y);
            break;
        case 9:
            this->missionsWindow->OnTouchMove(x, y);
            break;
        case 11:
            // Touch control preview: drag the steering pad / fire buttons.
            if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
                this->cinematicBtnA->OnTouchMove(x, y);
                this->cinematicBtnB->OnTouchMove(x, y);
                if (touchId != 0) {
                    if (this->cinematicTouchIdA == touchId) {
                        Globals::globals->setCoordsSteer(
                            *(int *) &Globals::options[84] + y - this->cinematicAnchorA,
                            Globals::Canvas->GetImage2DWidth(this->steerImageA),
                            Globals::Canvas->GetImage2DWidth(this->steerImageC),
                            Globals::Canvas->GetImage2DWidth(this->steerImageB), this->steerCoord_0x2e,
                            this->steerCoord_0x30, this->steerCoord_0x40, this->steerCoord_0x42, this->steerCoord_0x3c,
                            this->steerCoord_0x3e, this->steerCoord_0x34, this->steerCoord_0x32, this->steerCoord_0x48,
                            this->steerCoord_0x4a);
                        this->cinematicAnchorA = y;
                    } else if (this->cinematicTouchIdB == touchId) {
                        Globals::globals->setCoordsFire(
                            *(int *) &Globals::options[88] + y - this->cinematicAnchorB,
                            Globals::Canvas->GetImage2DWidth(this->fireImageA), this->fireImageA, this->fireImageB,
                            this->fireSize_0x74, this->fireCoord_0x60, this->fireCoord_0x62, this->fireCoord_0x64,
                            this->fireCoord_0x66, this->fireCoord_0x68, this->fireCoord_0x6a, this->fireCoord_0x6c,
                            this->fireCoord_0x6e, this->fireCoord_0x70, this->fireCoord_0x72, this->fireCoord_0x4c,
                            this->fireCoord_0x4e);
                        this->cinematicAnchorB = y;
                    }
                }
            }
            break;
        case 12:
            for (unsigned int i = 0; i < this->buttonsB4->size(); i++)
                (*this->buttonsB4)[i]->OnTouchMove(x, y);
            break;
        case 14:
            for (unsigned int i = 0; i < this->buttonsB0->size(); i++)
                (*this->buttonsB0)[i]->OnTouchMove(x, y);
            break;
        case 16:
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                long long id = (*this->scrollEntries)[i]->id;
                if (id >= 101 && id <= 105)
                    (*this->scrollEntries)[i]->OnTouchMove(x, y);
            }
            // fall through
        case 15:
            this->scrollWindowB->OnTouchMove(x, y);
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                long long id = (*this->scrollEntries)[i]->id;
                if (id == 52 || id == 60)
                    (*this->scrollEntries)[i]->OnTouchMove(x, y);
            }
            for (unsigned int i = 0; i < this->scrollSlots->size(); i++)
                (*this->scrollSlots)[i]->OnTouchMove(x, y);
            if (y > Globals::layout->field_0xc_leftMargin && y < Globals::h - Globals::layout->field_0x10_rightMargin &&
                x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
                int delta = y - this->dragLastX;
                this->dragLastX = y;
                this->dragVelocity = delta;
                this->inertiaDecay = 1.0f;
                this->scrollOffset += delta;
            }
            break;
        case 17:
            for (unsigned int i = 0; i < this->buttonsB8->size(); i++)
                (*this->buttonsB8)[i]->OnTouchMove(x, y);
            break;
        default:
            for (unsigned int i = 0; i < this->buttons->size(); i++)
                (*this->buttons)[i]->OnTouchMove(x, y);
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                int id = (int) (*this->scrollEntries)[i]->id;
                if (id == 23 || id == 24)
                    (*this->scrollEntries)[i]->OnTouchBegin(x, y);
            }
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                long long id = (*this->scrollEntries)[i]->id;
                if (id == 5 || id == 17 || id == 100 || id == 53)
                    (*this->scrollEntries)[i]->OnTouchMove(x, y);
            }
            break;
    }
    Globals::layout->OnTouchMove(x, y);
    return false;
}

int MenuTouchWindow::OnTouchEnd(int x, int y, void *touchId) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    this->pendingActivate = 0;
    this->dragging = 0;

    if (Globals::layout->choiceWindowOpen) {
        if (Globals::layout->OnTouchEnd(x, y))
            Globals::layout->choiceWindowOpen = 0;
        return 0;
    }

    switch (this->menuState) {
        case 17:
            // Campaign selection.
            if (this->field_0x175) {
                if (this->choiceWindow->OnTouchEnd(x, y) == 0)
                    this->menuState = 12;
                this->field_0x175 = 0;
                this->messageShowing = 0;
                return 0;
            }
            for (unsigned int i = 0; i < this->buttonsB8->size(); i++) {
                if (!(*this->buttonsB8)[i]->OnTouchEnd(x, y))
                    continue;
                this->field_0x234 = i;
                if (i == 1) {
                    if (!optionFlag(53)) {
                        NFC::iap_buy_dlc_valkyrie();
                        return 0;
                    }
                } else if (i == 2) {
                    if (!optionFlag(55)) {
                        NFC::iap_buy_dlc_supernova();
                        return 0;
                    }
                }
                this->menuState = 12;
                break;
            }
            break;
        case 15: {
            // DLC store.
            if (this->dlcMessageShowing) {
                if (this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    ((MtwAppData *) Globals::appManager->GetApplicationData())->dlcMessageAck = 1;
                    this->dlcMessageShowing = 0;
                    this->messageShowing = 0;
                }
                return 0;
            }
            if (this->dlcErrorDialogShowing || this->dlcResultDialogShowing) {
                this->messageShowing = 0;
                this->dlcResultDialogShowing = 0;
                this->dlcErrorDialogShowing = 0;
                return 0;
            }
            if (this->messageShowing) {
                if (this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    this->messageShowing = 0;
                    ((MtwAppData *) Globals::appManager->GetApplicationData())->purchaseReadyFlag = 0;
                }
                return 0;
            }
            this->scrollWindowB->OnTouchEnd(x, y);
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                TouchButton *button = (*this->scrollEntries)[i];
                if (button->id == 52) {
                    if (button->OnTouchEnd(x, y)) {
                        switch (this->field_0x1e0) {
                            case 0:
                                if (!optionFlag(53))
                                    NFC::iap_buy_dlc_valkyrie();
                                break;
                            case 1:
                                if (!optionFlag(54))
                                    NFC::iap_buy_dlc_kaamo_club();
                                break;
                            case 2:
                                if (!optionFlag(55))
                                    NFC::iap_buy_dlc_supernova();
                                break;
                            case 3:
                                if (!optionFlag(56))
                                    NFC::iap_buy_dlc_vip();
                                break;
                            case 4:
                                if (!optionFlag(57))
                                    NFC::iap_buy_dlc_full_package();
                                break;
                        }
                        ((MtwAppData *) Globals::appManager->GetApplicationData())->purchaseCode = this->field_0x1e0;
                        this->messageShowing = 0;
                    }
                } else if (button->id == 60) {
                    if (button->OnTouchEnd(x, y)) {
                        ((MtwAppData *) Globals::appManager->GetApplicationData())->purchaseCode = -1;
                        ((MtwAppData *) Globals::appManager->GetApplicationData())->restoreRequested = 1;
                        this->messageShowing = 0;
                        this->purchaseRestorePending = 1;
                    }
                }
            }
            int previous = this->field_0x1e0;
            for (unsigned int i = 0; i < this->scrollSlots->size(); i++) {
                if (!(*this->scrollSlots)[i]->OnTouchEnd(x, y))
                    continue;
                this->field_0x1e0 = i;
                if (previous == (int) i) {
                    // Tapping the selected DLC again buys it.
                    switch (i) {
                        case 0:
                            NFC::iap_buy_dlc_valkyrie();
                            break;
                        case 1:
                            NFC::iap_buy_dlc_kaamo_club();
                            break;
                        case 2:
                            NFC::iap_buy_dlc_supernova();
                            break;
                        case 3:
                            NFC::iap_buy_dlc_vip();
                            break;
                        case 4:
                            NFC::iap_buy_dlc_full_package();
                            break;
                    }
                } else {
                    this->scrollWindowB->setText(String(""), *Globals::gameText->getText(this->field_0x1e0 + 87));
                }
                break;
            }
            if (this->scrollbarHit &&
                x > Globals::w - Globals::layout->buttonInsetX -
                        Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
                y < Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]) +
                        (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin)) {
                // Scrollbar release: nothing to do in this build.
            }
            break;
        }
        case 14:
            // Language selection.
            for (unsigned int i = 0; i < this->buttonsB0->size(); i++) {
                if (!(*this->buttonsB0)[i]->OnTouchEnd(x, y))
                    continue;
                short previousLanguage = GameText::getLanguage();
                int language;
                switch ((*this->buttonsB0)[i]->id) {
                    case 27: language = 0; break;
                    case 28: language = 1; break;
                    case 29: language = 4; break;
                    case 30: language = 2; break;
                    case 31: language = 3; break;
                    case 32: language = 6; break;
                    case 33: language = 5; break;
                    case 34: language = 9; break;
                    case 35: language = 12; break;
                    case 36: language = 11; break;
                    case 37: language = 10; break;
                    case 38: language = 14; break;
                    case 39: language = 15; break;
                    default: continue;
                }
                Globals::globals->loadFont(language);
                Globals::gameText->setLanguage((short) language, 3401);
                if (previousLanguage == 1 || language == 1) {
                    // Switching to/from the language with its own voice bank reloads the sound system.
                    delete Globals::sound;
                    Globals::sound = 0;
                    Globals::sound = new FModSound();
                    Globals::sound->init();
                    Globals::switch_to_target_setting =
                        Globals::appManager->GetCurrentApplicationModule() == 5 ? 1 : 2;
                }
                Globals::recordHandler->saveOptions();
                Globals::achievements->resetNewMedals();
                Globals::layout->reload();
                if (Globals::appManager->GetCurrentApplicationModule() == 5)
                    Globals::status->field_0x108 = 1;
                Globals::appManager->SetCurrentApplicationModule(Globals::appManager->GetCurrentApplicationModule());
            }
            break;
        case 13:
            if (this->messageShowing && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                this->messageShowing = 0;
                return 0;
            }
            break;
        case 12:
            // Difficulty selection for a new game.
            if (this->returnToMenuSubFlag) {
                int r = this->choiceWindow->OnTouchEnd(x, y);
                if (r == 0) {
                    if (this->field_0x234 == 1)
                        this->startValkyrie();
                    else if (this->field_0x234 == 2)
                        this->startSupernova();
                    else
                        this->startGOF2();
                } else if (r == 1) {
                    this->returnToMenuSubFlag = 0;
                    this->messageShowing = 0;
                }
                return 0;
            }
            for (unsigned int i = 0; i < this->buttonsB4->size(); i++) {
                if (!(*this->buttonsB4)[i]->OnTouchEnd(x, y))
                    continue;
                float difficulty = i == 0 ? 0.5f : 1.5f;
                this->fadeValue = difficulty;
                optionFloat(44) = difficulty;
                if (i != 0) {
                    this->choiceWindow->set(*Globals::gameText->getText(26), true);
                    this->returnToMenuSubFlag = 1;
                    this->messageShowing = 1;
                    return 0;
                }
                if (Globals::status->getPlayingTime() > 0) {
                    this->choiceWindow->set(*Globals::gameText->getText(52), true);
                    this->returnToMenuSubFlag = 1;
                    this->messageShowing = 1;
                } else {
                    if (this->field_0x234 == 1)
                        this->startValkyrie();
                    else if (this->field_0x234 == 2)
                        this->startSupernova();
                    else
                        this->startGOF2();
                    return 0;
                }
            }
            break;
        case 11:
            // Touch-control preview (iPad).
            if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
                if (touchId != 0) {
                    if (this->cinematicTouchIdA == touchId)
                        this->cinematicTouchIdA = 0;
                    if (this->cinematicTouchIdB == touchId)
                        this->cinematicTouchIdB = 0;
                }
                this->cinematicTouchState = 0;
                if (this->cinematicBtnA->OnTouchEnd(x, y))
                    this->menuState = 3;
                if (this->cinematicBtnB->OnTouchEnd(x, y)) {
                    float controlSize = optionFloat(68);
                    optionInt(84) = controlSize <= 0.0f ? 415 : (controlSize >= 1.0f ? 830 : 583);
                    optionInt(88) = controlSize <= 0.0f ? 365 : (controlSize >= 1.0f ? 730 : 513);
                    Globals::globals->setCoordsFire(optionInt(88), Globals::Canvas->GetImage2DWidth(this->fireImageA),
                                                    this->fireImageA, this->fireImageB, this->fireSize_0x74,
                                                    this->fireCoord_0x60, this->fireCoord_0x62, this->fireCoord_0x64,
                                                    this->fireCoord_0x66, this->fireCoord_0x68, this->fireCoord_0x6a,
                                                    this->fireCoord_0x6c, this->fireCoord_0x6e, this->fireCoord_0x70,
                                                    this->fireCoord_0x72, this->fireCoord_0x4c, this->fireCoord_0x4e);
                    Globals::globals->setCoordsSteer(optionInt(84), Globals::Canvas->GetImage2DWidth(this->steerImageA),
                                                     Globals::Canvas->GetImage2DWidth(this->steerImageC),
                                                     Globals::Canvas->GetImage2DWidth(this->steerImageB),
                                                     this->steerCoord_0x2e, this->steerCoord_0x30,
                                                     this->steerCoord_0x40, this->steerCoord_0x42,
                                                     this->steerCoord_0x3c, this->steerCoord_0x3e,
                                                     this->steerCoord_0x34, this->steerCoord_0x32,
                                                     this->steerCoord_0x48, this->steerCoord_0x4a);
                }
            }
            break;
        case 10:
            // Cargo overview message.
            if (this->messageShowing && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                this->menuState = 0;
                this->messageShowing = 0;
            }
            break;
        case 9:
            // Missions.
            if (this->missionsWindow->OnTouchEnd(x, y))
                this->menuState = 0;
            break;
        case 8:
            // Control options.
            if (this->messageShowing && this->field_0x176) {
                if (this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    double z = ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[2];
                    double tilt = ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[0];
                    if (z > 0.0) {
                        if (tilt > 1.0)
                            ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[0] = 1.0;
                        tilt = 1.0 - ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[0] + 1.0;
                    }
                    optionFloat(28) = (float) tilt;
                    optionFloat(32) =
                        (float) ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[2];
                    this->field_0x176 = 0;
                    this->messageShowing = 0;
                }
            }
            if (optionFlag(17))
                optionFloat(20) = (*this->sliders)[0]->getValue();
            else
                optionFloat(24) = (*this->sliders)[0]->getValue();
            if (this->upButtonPressed && x > this->listTopY + Globals::layout->buttonInsetX &&
                x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                y > Globals::layout->buttonInsetX + Globals::layout->field_0xc_leftMargin &&
                y < Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin + this->listEntryWidth) {
                optionFlag(17) = 1;
                this->optBtnD0->setHalfTransparent(true);
                (*this->sliders)[0]->setValue(optionFloat(20));
                Globals::sound->play(123, 0, 0, 0.0f);
            }
            if (this->downButtonPressed &&
                x > this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight &&
                x < this->listTopY - Globals::layout->buttonInsetX + this->listBottomY &&
                y > Globals::layout->buttonInsetX + Globals::layout->field_0xc_leftMargin &&
                y < Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin + this->listEntryWidth) {
                optionFlag(17) = 0;
                this->optBtnD0->setHalfTransparent(false);
                (*this->sliders)[0]->setValue(optionFloat(24));
                Globals::sound->play(123, 0, 0, 0.0f);
            }
            this->upButtonPressed = 0;
            this->downButtonPressed = 0;
            if (this->optBtnCC->OnTouchEnd(x, y)) {
                bool on = !optionFlag(16);
                optionFlag(16) = on;
                this->optBtnCC->setAlwaysPressed(on);
            }
            if (this->optBtnD0->OnTouchEnd(x, y)) {
                this->choiceWindow->set(*Globals::gameText->getText(493));
                this->field_0x176 = 1;
                this->messageShowing = 1;
            }
            (*this->sliders)[0]->OnTouchEnd(x, y);
            break;
        case 7:
            // Sound options.
            if (this->messageShowing && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                this->messageShowing = 0;
                break;
            }
            if (this->optBtnD4->OnTouchEnd(x, y)) {
                bool on = !optionFlag(13);
                optionFlag(13) = on;
                this->optBtnD4->setAlwaysPressed(on);
                (*this->sliders)[1]->setHalfTransparent(!optionFlag(13));
                Globals::sound->enableCategory(1, optionFlag(13));
            }
            if (this->optBtnD8->OnTouchEnd(x, y)) {
                bool on = !optionFlag(12);
                optionFlag(12) = on;
                this->optBtnD8->setAlwaysPressed(on);
                (*this->sliders)[2]->setHalfTransparent(!optionFlag(12));
                Globals::sound->enableCategory(2, optionFlag(12));
                Globals::sound->play(126, 0, 0, 0.0f);
            }
            if (this->optBtnDC->OnTouchEnd(x, y)) {
                bool on = !optionFlag(14);
                optionFlag(14) = on;
                this->optBtnDC->setAlwaysPressed(on);
                (*this->sliders)[3]->setHalfTransparent(!optionFlag(14));
                Globals::sound->enableCategory(3, optionFlag(14));
            }
            optionFloat(0) = (*this->sliders)[1]->getValue();
            optionFloat(4) = (*this->sliders)[2]->getValue();
            optionFloat(8) = (*this->sliders)[3]->getValue();
            optionFloat(36) = (*this->sliders)[4]->getValue();
            optionFloat(40) = (*this->sliders)[5]->getValue();
            for (unsigned int i = 1; i < this->sliders->size(); i++) {
                if (i == 5 && !Globals::layout->field_0x284_sliderSlot5Enabled)
                    continue;
                if (!(*this->sliders)[i]->OnTouchEnd(x, y))
                    continue;
                if (i == 2) {
                    Globals::sound->play(126, 0, 0, 0.0f);
                } else if (i == 5) {
                    float quality = (*this->sliders)[5]->getValue();
                    this->choiceWindow->set(
                        *Globals::gameText->getText(quality <= 0.33f ? 510 : (quality <= 0.66f ? 511 : 512)));
                    this->messageShowing = 1;
                }
            }
            break;
        case 4:
            // About.
            this->scrollWindowA->OnTouchEnd(x, y);
            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                TouchButton *button = (*this->scrollEntries)[i];
                switch (button->id) {
                    case 22:
                        if (button->OnTouchEnd(x, y)) {
                            ((MtwAppData *) Globals::appManager->GetApplicationData())->rateRequested = 1;
                            NFC::rateGame();
                        }
                        break;
                    case 106:
                        if (button->OnTouchEnd(x, y)) {
                            NFC::rateGame();
                            ((MtwAppData *) Globals::appManager->GetApplicationData())->rated = 1;
                        }
                        break;
                    case 109:
                        if (button->OnTouchEnd(x, y))
                            NFC::openPrivacyPolicy();
                        break;
                    case 110:
                        if (button->OnTouchEnd(x, y))
                            NFC::openTermsOfService();
                        break;
                }
            }
            break;
        case 3:
            // Options.
            if (Globals::iPad) {
                // The iPad layout shows the control and the sound options on one page.
                if (this->scrollUpButton->OnTouchEnd(x, y))
                    this->menuState = 11;

                if (this->messageShowing && this->field_0x176) {
                    if (this->choiceWindow->OnTouchEnd(x, y) == 0) {
                        double z = ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[2];
                        double tilt = ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[0];
                        if (z > 0.0) {
                            if (tilt > 1.0)
                                ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[0] = 1.0;
                            tilt = 1.0 - ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[0] + 1.0;
                        }
                        optionFloat(28) = (float) tilt;
                        optionFloat(32) =
                            (float) ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->GetAccelValue()[2];
                        this->field_0x176 = 0;
                        this->messageShowing = 0;
                    }
                }
                if (optionFlag(17))
                    optionFloat(20) = (*this->sliders)[0]->getValue();
                else
                    optionFloat(24) = (*this->sliders)[0]->getValue();
                if (this->upButtonPressed && x > this->listTopY + Globals::layout->buttonInsetX &&
                    x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                    y > Globals::layout->buttonInsetX + Globals::layout->field_0xc_leftMargin &&
                    y < Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin + this->listEntryWidth) {
                    optionFlag(17) = 1;
                    this->optBtnD0->setHalfTransparent(true);
                    (*this->sliders)[0]->setValue(optionFloat(20));
                    Globals::sound->play(123, 0, 0, 0.0f);
                }
                if (this->downButtonPressed &&
                    x > this->listTopY + Globals::layout->buttonInsetX + this->listEntryHeight &&
                    x < this->listTopY - Globals::layout->buttonInsetX + this->listBottomY &&
                    y > Globals::layout->buttonInsetX + Globals::layout->field_0xc_leftMargin &&
                    y < Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin + this->listEntryWidth) {
                    optionFlag(17) = 0;
                    this->optBtnD0->setHalfTransparent(false);
                    (*this->sliders)[0]->setValue(optionFloat(24));
                    Globals::sound->play(123, 0, 0, 0.0f);
                }
                this->upButtonPressed = 0;
                this->downButtonPressed = 0;
                if (this->optBtnCC->OnTouchEnd(x, y)) {
                    bool on = !optionFlag(16);
                    optionFlag(16) = on;
                    this->optBtnCC->setAlwaysPressed(on);
                }
                if (this->optBtnD0->OnTouchEnd(x, y)) {
                    this->choiceWindow->set(*Globals::gameText->getText(493));
                    this->field_0x176 = 1;
                    this->messageShowing = 1;
                }
                (*this->sliders)[0]->OnTouchEnd(x, y);

                if (this->messageShowing && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    this->messageShowing = 0;
                    break;
                }
                if (this->optBtnD4->OnTouchEnd(x, y)) {
                    bool on = !optionFlag(13);
                    optionFlag(13) = on;
                    this->optBtnD4->setAlwaysPressed(on);
                    (*this->sliders)[1]->setHalfTransparent(!optionFlag(13));
                    Globals::sound->enableCategory(1, optionFlag(13));
                }
                if (this->optBtnD8->OnTouchEnd(x, y)) {
                    bool on = !optionFlag(12);
                    optionFlag(12) = on;
                    this->optBtnD8->setAlwaysPressed(on);
                    (*this->sliders)[2]->setHalfTransparent(!optionFlag(12));
                    Globals::sound->enableCategory(2, optionFlag(12));
                    Globals::sound->play(126, 0, 0, 0.0f);
                }
                if (this->optBtnDC->OnTouchEnd(x, y)) {
                    bool on = !optionFlag(14);
                    optionFlag(14) = on;
                    this->optBtnDC->setAlwaysPressed(on);
                    (*this->sliders)[3]->setHalfTransparent(!optionFlag(14));
                    Globals::sound->enableCategory(3, optionFlag(14));
                }
                optionFloat(0) = (*this->sliders)[1]->getValue();
                optionFloat(4) = (*this->sliders)[2]->getValue();
                optionFloat(8) = (*this->sliders)[3]->getValue();
                optionFloat(36) = (*this->sliders)[4]->getValue();
                optionFloat(40) = (*this->sliders)[5]->getValue();
                for (unsigned int i = 1; i < this->sliders->size(); i++) {
                    if (!(*this->sliders)[i]->OnTouchEnd(x, y))
                        continue;
                    switch (i) {
                        case 2:
                            Globals::sound->play(126, 0, 0, 0.0f);
                            break;
                        case 5: {
                            float quality = (*this->sliders)[5]->getValue();
                            if (Globals::iPadLargePossible) {
                                this->choiceWindow->set(*Globals::gameText->getText(122));
                                this->messageShowing = 1;
                                optionFloat(68) = quality <= 0.33f ? 0.0f : (quality <= 0.66f ? 0.5f : 1.0f);
                                Globals::recordHandler->saveOptions();
                            } else {
                                this->choiceWindow->set(*Globals::gameText->getText(
                                    quality <= 0.33f ? 510 : (quality <= 0.66f ? 511 : 512)));
                                this->messageShowing = 1;
                            }
                            break;
                        }
                    }
                }
                if (Globals::iPadLargePossible && this->scrollExtraButton != 0 &&
                    this->scrollExtraButton->OnTouchEnd(x, y)) {
                    optionFlag(64) = !optionFlag(64);
                    Globals::recordHandler->saveOptions();
                    this->scrollExtraButton->setAlwaysPressed(optionFlag(64));
                    this->choiceWindow->set(*Globals::gameText->getText(122));
                    this->messageShowing = 1;
                }
                break;
            }
            for (unsigned int i = 0; i < this->optionsButtons->size(); i++) {
                if (!(*this->optionsButtons)[i]->OnTouchEnd(x, y))
                    continue;
                switch ((*this->optionsButtons)[i]->id) {
                    case 4:
                        this->menuState = 4;
                        break;
                    case 7:
                        this->menuState = 6;
                        break;
                    case 8:
                        this->menuState = 7;
                        break;
                    case 9:
                        this->menuState = 8;
                        break;
                    case 25:
                        this->menuState = 14;
                        break;
                }
            }
            break;
        case 2:
            // Save game.
            if (this->messageShowing) {
                if (this->listStateSuppress)
                    return 0;
                if (this->field_0x1da)
                    return 0;
                bool saveDlg = this->saveDialogShowing;
                int r = this->choiceWindow->OnTouchEnd(x, y);
                if (!saveDlg) {
                    if (r != 0)
                        return 0;
                } else {
                    if (r != 1) {
                        if (r == 0) {
                            this->saveGame(this->selectedRow);
                            this->saveDialogShowing = 0;
                        }
                        return 0;
                    }
                    this->saveDialogShowing = 0;
                }
                this->messageShowing = 0;
                return 0;
            }
            if (this->okButton->OnTouchEnd(x, y)) {
                if (this->selectedRow == 0) {
                    this->choiceWindow->set(+*Globals::gameText->getText(487));
                    this->messageShowing = 1;
                    return 0;
                }
                if ((*this->previewRecords)[this->selectedRow] == 0) {
                    Globals::sound->play(123, 0, 0, 0.0f);
                    this->saveGame(this->selectedRow);
                    return 0;
                }
                this->choiceWindow->set(*Globals::gameText->getText(49), true);
                this->saveDialogShowing = 1;
                this->messageShowing = 1;
                return 0;
            }
            break;
        case 1:
            // Load game.
            if (this->messageShowing) {
                if (this->listStateSuppress)
                    return 0;
                if (this->field_0x1da ||
                    (this->choiceWindow->OnTouchEnd(x, y) == 0 && this->choiceWindow->hasChoice() &&
                     this->loadGame(this->selectedRow) == 0))
                    return 0;
                this->messageShowing = 0;
            }
            if (this->okButton->OnTouchEnd(x, y)) {
                if ((*this->previewRecords)[this->selectedRow] == 0)
                    return 0;
                if (Globals::status->getPlayingTime() > 0) {
                    this->choiceWindow->set(*Globals::gameText->getText(51), true);
                    this->messageShowing = 1;
                    return 0;
                }
                Globals::sound->play(123, 0, 0, 0.0f);
                this->loadGame(this->selectedRow);
                return 0;
            }
            break;
        case 0: {
            // Main menu.
            if (this->messageShowing) {
                if (this->returnToMenuShowing) {
                    int r = this->choiceWindow->OnTouchEnd(x, y);
                    if (r == 0) {
                        Globals::appManager->Quit();
                    } else if (r == 1) {
                        this->returnToMenuShowing = 0;
                        this->messageShowing = 0;
                    }
                    return 0;
                }
                if (this->genericConfirmB) {
                    int r = this->choiceWindow->OnTouchEnd(x, y);
                    if (r == 0) {
                        g_android_link_game_gp = 1;
                        this->messageShowing = 0;
                        this->genericConfirmB = 0;
                    } else if (r == 1) {
                        this->messageShowing = 0;
                        this->genericConfirmB = 0;
                    }
                    return 0;
                }
                if (this->quitConfirmShowing) {
                    int r = this->choiceWindow->OnTouchEnd(x, y);
                    if (r == 0) {
                        Globals::sound->resumeAll();
                        Globals::sound->stopAll();
                        Globals::switch_to_target_setting = 2;
                        Globals::appManager->SetCurrentApplicationModule(1);
                        this->quitConfirmShowing = 0;
                        this->messageShowing = 0;
                    } else if (r == 1) {
                        this->quitConfirmShowing = 0;
                        this->messageShowing = 0;
                        this->returnToMenuSubFlag = 0;
                    }
                    return 0;
                }
                if (this->leaderboardDialogShowing) {
                    int r = this->choiceWindow->OnTouchEnd(x, y);
                    if (r == 1 || r == 0)
                        Globals::globals->reportLeaderboards();
                    this->leaderboardDialogShowing = 0;
                    this->messageShowing = 0;
                }
                if (this->dlcMessageShowing) {
                    if (this->choiceWindow->OnTouchEnd(x, y) == 0) {
                        ((MtwAppData *) Globals::appManager->GetApplicationData())->dlcMessageAck = 1;
                        this->dlcMessageShowing = 0;
                        this->messageShowing = 0;
                    }
                    return 0;
                }
                if (this->dlcErrorDialogShowing || this->dlcResultDialogShowing) {
                    this->messageShowing = 0;
                    this->dlcResultDialogShowing = 0;
                    this->dlcErrorDialogShowing = 0;
                    return 0;
                }
                if (this->loadFailedDialogShowing) {
                    this->loadFailedDialogShowing = 0;
                    this->messageShowing = 0;
                    return 0;
                }
                if (this->genericConfirmA && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    this->messageShowing = 0;
                    this->genericConfirmA = 0;
                    return 0;
                }
                if (this->supernovaMessageShowing && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    this->supernovaMessageShowing = 0;
                    this->messageShowing = 0;
                    return 0;
                }
                if (this->supernovaPurchaseDialogShowing && this->choiceWindow->OnTouchEnd(x, y) == 0) {
                    this->supernovaPurchaseDialogShowing = 0;
                    this->messageShowing = 0;
                    return 0;
                }
            }

            for (unsigned int i = 0; i < this->buttons->size(); i++) {
                if (!(*this->buttons)[i]->OnTouchEnd(x, y))
                    continue;
                switch ((*this->buttons)[i]->id) {
                    case 0:
                        this->menuState = 17;
                        break;
                    case 1:
                        this->loadPreviewRecords();
                        this->selectedRow = 0;
                        this->field_0x1dc = 0;
                        this->createRecordButtons(false);
                        this->menuState = 1;
                        break;
                    case 2:
                        this->loadPreviewRecords();
                        this->selectedRow = 0;
                        this->field_0x1dc = 0;
                        this->createRecordButtons(true);
                        this->menuState = 2;
                        break;
                    case 3:
                        this->menuState = 3;
                        break;
                    case 4:
                        this->menuState = 4;
                        break;
                    case 6:
                        this->choiceWindow->set(*Globals::gameText->getText(523), true);
                        this->quitConfirmShowing = 1;
                        this->messageShowing = 1;
                        break;
                    case 10:
                        this->menuState = 9;
                        if (this->missionsWindow != 0)
                            this->missionsWindow->init();
                        else
                            this->missionsWindow = new MissionsWindow();
                        break;
                    case 11:
                        this->loadGame(Globals::lastRecordWritten);
                        break;
                    case 12: {
                        // Cargo overview.
                        this->menuState = 10;
                        String cargoText;
                        String amountText;
                        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                        if (cargo != 0) {
                            int lastType = (*cargo)[0]->getType();
                            for (unsigned int j = 0; j < cargo->size(); j++) {
                                amountText = (*cargo)[j]->getAmount() >= 2
                                                 ? String(" (") + String((*cargo)[j]->getAmount()) + String(")")
                                                 : String("");
                                cargoText += *Globals::gameText->getText((*cargo)[j]->getIndex() + 1274) +
                                             String(amountText) + String("\n");
                                if ((*cargo)[j]->getType() != lastType) {
                                    cargoText += String("\n");
                                    lastType = (*cargo)[j]->getType();
                                }
                            }
                        } else {
                            cargoText = String(*Globals::gameText->getText(286));
                        }
                        String load = String(Globals::status->getShip()->getCurrentLoad()) + String(" / ") +
                                      String(Globals::status->getShip()->getMaxLoad()) + String("t");
                        this->choiceWindow->set(*Globals::gameText->getText(166) + String(" ") + String(load),
                                                cargoText, false);
                        this->messageShowing = 1;
                        break;
                    }
                    case 18:
                        this->cinematicSteerActive = 1;
                        return 1;
                    case 19:
                        this->menuState = 13;
                        break;
                    case 25:
                        this->menuState = 14;
                        break;
                    case 107:
                        this->menuState = 18;
                        break;
                }
            }

            for (unsigned int i = 0; i < this->scrollEntries->size(); i++) {
                TouchButton *button = (*this->scrollEntries)[i];
                int id = (int) button->id;
                if (id == 23) {
                    if (button->OnTouchEnd(x, y)) {
                        if (g_android_gp_is_linked) {
                            g_android_show_leaderboards = 1;
                        } else {
                            this->choiceWindow->set(String("No link with Google+"),
                                                    *Globals::gameText->getText(3398), true);
                            this->genericConfirmB = 1;
                            this->messageShowing = 1;
                        }
                    }
                } else if (id == 24) {
                    if (button->OnTouchEnd(x, y)) {
                        if (g_android_gp_is_linked) {
                            g_android_show_achievements = 1;
                        } else {
                            this->choiceWindow->set(String("No link with Google+"),
                                                    *Globals::gameText->getText(3398), true);
                            this->genericConfirmB = 1;
                            this->messageShowing = 1;
                        }
                    }
                }
                Globals::globals->reportLeaderboards();
                if (id == 53 && (*this->scrollEntries)[i]->OnTouchEnd(x, y)) {
                    MtwAppData *appData = (MtwAppData *) Globals::appManager->GetApplicationData();
                    appData->dlcMenuAckFlag = 0;
                    ((MtwAppData *) Globals::appManager->GetApplicationData())->dlcMenuRequestFlag = 1;
                    this->messageShowing = 1;
                    this->dlcMessageShowing = 1;
                    this->choiceWindow->set(*Globals::gameText->getText(71), *Globals::gameText->getText(425));
                }
                if (id == 17 && (*this->scrollEntries)[i]->OnTouchEnd(x, y)) {
                    this->choiceWindow->set(*Globals::gameText->getText(53), true);
                    this->returnToMenuShowing = 1;
                    this->messageShowing = 1;
                    return 0;
                }
            }
            break;
        }
    }

    if (this->menuState == 1 || this->menuState == 2) {
        // Release the record list with inertia.
        int velocity = this->dragVelocity;
        this->inertiaDecay = 0.9f;
        this->dragging = 0;
        this->scrollOffset += velocity;
        this->field_0x210 = this->scrollOffset;
        this->inertiaVel = (velocity < 0 ? -velocity : velocity) > 3 ? (float) velocity : 0.0f;
    }

    if (Globals::layout->OnTouchEnd(x, y)) {
        // Back button.
        switch (this->menuState) {
            case 0:
                return 1;
            case 3:
                Globals::recordHandler->saveOptions();
                this->menuState = 0;
                break;
            case 6:
            case 7:
            case 8:
            case 14:
                this->menuState = Globals::iPad ? 0 : 3;
                break;
            case 11:
                break;
            case 12:
                this->menuState = 17;
                break;
            default:
                this->menuState = 0;
                break;
        }
    }
    return 0;
}

// _mtw_Status_getShip / _mtw_Item_make / _mtw_Item_makeQty / _mtw_Ship_setItem removed:
// real calls are Status::getShip(), Item::makeItem()/makeItem(int) on Globals::items
// prototypes, and Ship::setEquipment(Item*, int) (proven from 0x12ca80 disasm).

void MenuTouchWindow::startValkyrie() {
    Status *const *statusHolder = &Globals::status;
    Globals::status->resetGame();
    for (int i = 0; i < 0x2d; ++i)
        (*statusHolder)->nextCampaignMission(false);

    Status *status = *statusHolder;
    Mission *mission = new Mission();
    status->setMission(mission);

    status = *statusHolder;
    status->setShip(Globals::ships->data_[5]->makeShip(-1));

    (*statusHolder)->getShip()->setRace(0);

    Item *it;
    it = Globals::items->data_[2]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[5]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[36]->makeItem(10);
    (*statusHolder)->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[81]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[51]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[86]->makeItem(10);
    (*statusHolder)->getShip()->setEquipment(it, 2);
    it = Globals::items->data_[85]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 3);

    (*statusHolder)->setCredits(0);
    Status *stationTarget = *statusHolder;
    Station *station = reinterpret_cast<Station *>(Globals::galaxy->getStation(0x5b));
    stationTarget->setStation(station);
    (*statusHolder)->setSystemVisibility(6, true);
    (*statusHolder)->setSystemVisibility(0x19, true);
    (*statusHolder)->setCredits(0);

    OptionsRecord *optB = (OptionsRecord *) Globals::options;
    (*statusHolder)->field_84 = 0x1a0a;
    unsigned char *hints = Globals::hints;
    hints[8] = 1;
    hints[9] = 1;
    hints[13] = 1;
    hints[14] = 1;
    hints[28] = 1;
    hints[29] = 1;
    hints[32] = 1;
    hints[33] = 1;
    hints[34] = 1;
    hints[35] = 1;
    hints[23] = 1;
    hints[10] = 1;
    hints[21] = 1;
    hints[19] = 1;
    hints[15] = 1;
    hints[30] = 1;
    hints[36] = 1;
    hints[54] = 0;
    hints[50] = 0;
    hints[51] = 0;
    hints[52] = 0;
    hints[53] = 0;
    hints[56] = 1;
    optB->flag_0x34 = 1;

    Globals::achievements->setMedal(0x17, 3);
    Globals::achievements->setMedal(0x1e, 1);
    Globals::recordHandler->saveOptions();
    (*statusHolder)->setKills(0xc5);
    Globals::sound->stop(Globals::sound->currentMusicEvent);

    optB->fadeValue = this->fadeValue;
    Globals::appManager->SetCurrentApplicationModule(5);
}

void MenuTouchWindow::startSupernova() {
    Status *const *statusHolder = &Globals::status;
    Globals::status->resetGame();
    for (int i = 0; i < 0x54; ++i)
        (*statusHolder)->nextCampaignMission(false);

    (*statusHolder)->setMission(Mission::empty);
    Status *status = *statusHolder;
    status->setShip(Globals::ships->data_[30]->makeShip(-1));

    (*statusHolder)->getShip()->setRace(3);

    Item *it;
    it = Globals::items->data_[176]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[20]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[36]->makeItem(20);
    (*statusHolder)->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[44]->makeItem(20);
    (*statusHolder)->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[81]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[51]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[68]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 2);
    it = Globals::items->data_[86]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 3);
    it = Globals::items->data_[85]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 4);
    it = Globals::items->data_[56]->makeItem();
    (*statusHolder)->getShip()->setEquipment(it, 5);
    it = Globals::items->data_[122]->makeItem(8);
    (*statusHolder)->getShip()->addCargo(it);

    (*statusHolder)->setCredits(0);
    Status *stationTarget = *statusHolder;
    Station *station = reinterpret_cast<Station *>(Globals::galaxy->getStation(0x46));
    stationTarget->setStation(station);
    (*statusHolder)->setSystemVisibility(6, true);
    (*statusHolder)->setSystemVisibility(0x19, true);
    (*statusHolder)->setCredits(0);

    (*statusHolder)->field_84 = 0x1a0a;
    unsigned char *hints = Globals::hints;
    hints[8] = 1;
    hints[9] = 1;
    hints[32] = 1;
    hints[33] = 1;
    hints[34] = 1;
    hints[35] = 1;
    hints[23] = 1;
    hints[54] = 0;
    hints[50] = 0;
    hints[51] = 0;
    hints[52] = 0;
    hints[53] = 0;
    hints[10] = 1;
    hints[21] = 1;
    hints[19] = 1;
    hints[13] = 1;
    hints[14] = 1;
    hints[15] = 1;
    hints[38] = 1;
    hints[49] = 1;
    hints[36] = 1;
    hints[28] = 1;
    hints[29] = 1;
    hints[30] = 1;
    hints[56] = 1;
    hints[57] = 1;
    Globals::options[52] = 1;

    Globals::achievements->setMedal(0x17, 3);
    Globals::achievements->setMedal(0x1e, 1);
    (*statusHolder)->setKills(0x182);
    Globals::recordHandler->saveOptions();
    Globals::sound->stop(Globals::sound->currentMusicEvent);

    ((OptionsRecord *) Globals::options)->fadeValue = this->fadeValue;
    Globals::appManager->SetCurrentApplicationModule(5);
}

void MenuTouchWindow::startGOF2() {
    Globals::status->resetGame();
    ((OptionsRecord *) Globals::options)->fadeValue = this->fadeValue;
    Globals::sound->stop(Globals::sound->currentMusicEvent);
    Globals::sound->play(0x8f, nullptr, nullptr, 0.0f);
    Globals::appManager->SetCurrentApplicationModule(2);
}

void MenuTouchWindow::render3D() {
    if (this->menuState != 9)
        return;
    MissionsWindow *obj = (MissionsWindow *) this->missionsWindow;
    if (obj == 0)
        return;
    return obj->render3D();
}

uint8_t MenuTouchWindow::isShowingMessage() {
    return this->messageShowing;
}

bool MenuTouchWindow::inCinematicMode() {
    return this->menuState == 0xd;
}

bool MenuTouchWindow::isMakingScreenshot() {
    return this->screenshotState < 0x80000000u;
}

void MenuTouchWindow::hideMessage() {
    this->messageShowing = 0;
}

void MenuTouchWindow::setSkipButtonVisible(bool visible) {
    Array<TouchButton *> *arr = this->buttons;
    if (arr != 0) {
        for (uint32_t i = 0; i < this->buttons->count; i++) {
            TouchButton *btn = this->buttons->data_[i];
            if (btn != 0 && btn->id == 0x12) {
                btn->setVisible(visible);
            }
        }
    }
}

void MenuTouchWindow::callDlcMenu() {
    AppManager *const *holder = (AppManager *const *) &Globals::appManager;

    MtwAppData *ad = (MtwAppData *) ((AbyssEngine::ApplicationManager*)*holder)->GetApplicationData();
    ad->dlcMenuAckFlag = 0;
    ad = (MtwAppData *) ((AbyssEngine::ApplicationManager*)*holder)->GetApplicationData();
    GameText *const *gt = &Globals::gameText;
    ad->dlcMenuRequestFlag = 1;
    ChoiceWindow *win = (ChoiceWindow *) this->choiceWindow;
    this->messageShowing = 1;
    this->dlcMessageShowing = 1;
    String *s1 = (String *) _mtw_GameText_getText(*gt, 0x47);
    String *s2 = (String *) _mtw_GameText_getText(*gt, 0x1a9);
    win->set(*s1, *s2);
}

void MenuTouchWindow::startSupernovaChallenge() {
    Globals::status->resetGame();
    Globals::status->setCurrentCampaignMission(152);
    Globals::status->setShip(Globals::ships->data_[46]->makeShip(-1));

    Item *it = Globals::items->data_[231]->makeItem();
    Globals::status->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[230]->makeItem();
    Globals::status->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[231]->makeItem();
    Globals::status->getShip()->setEquipment(it, 2);
    it = Globals::items->data_[73]->makeItem();
    Globals::status->getShip()->setEquipment(it, 0);
    it = Globals::items->data_[54]->makeItem();
    Globals::status->getShip()->setEquipment(it, 1);
    it = Globals::items->data_[59]->makeItem();
    Globals::status->getShip()->setEquipment(it, 2);
    it = Globals::items->data_[83]->makeItem();
    Globals::status->getShip()->setEquipment(it, 3);
    it = Globals::items->data_[206]->makeItem();
    Globals::status->getShip()->setEquipment(it, 4);
    it = Globals::items->data_[75]->makeItem();
    Globals::status->getShip()->setEquipment(it, 5);
    it = Globals::items->data_[76]->makeItem();
    Globals::status->getShip()->setEquipment(it, 6);
    it = Globals::items->data_[216]->makeItem(5);
    Globals::status->getShip()->setEquipment(it, 0);

    Station *station = reinterpret_cast<Station *>(Globals::galaxy->getStation(111));
    Globals::status->setStation(station);
    Mission *mission = new Mission(183, String("Client"), Globals::imageFactory->createChar(true, 0), 0, 0,
                                   station->getIndex(), 1);
    Globals::status->setMission(mission);
    Globals::status->setFreelanceMission(mission);

    unsigned char *hints = Globals::hints;
    hints[23] = 1;
    hints[8] = 1;
    hints[9] = 1;
    hints[10] = 1;
    hints[28] = 1;
    hints[21] = 1;
    hints[19] = 1;
    hints[13] = 1;
    hints[14] = 1;
    hints[15] = 1;
    hints[41] = 1;
    hints[34] = 1;
    hints[32] = 1;
    hints[33] = 1;
    Globals::status->setKills(500);
    Globals::status->setKills(220);
    Globals::sound->stop(Globals::sound->currentMusicEvent);
    Globals::switch_to_target_setting = -1;
    Globals::enterSpaceLounge = 0;
    Globals::globals->playMusicAndFadeOutCurrent(1);
    Globals::status->field_5c = -1;
    Globals::status->field_60 = -1;
    Globals::status->field_64 = -1;
    Globals::appManager->SetCurrentApplicationModule(2);
}
