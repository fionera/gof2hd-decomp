#include "game/ui/MenuTouchWindow.h"
#include "game/menu/ModStation.h"
#include "engine/core/AbyssEngine.h"
#include "game/ui/TouchSlider.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Engine.h"
#include "game/mission/GameRecord.h"
#include "game/ui/Layout.h"
#include "game/mission/Status.h"
#include "game/mission/RecordHandler.h"
#include "game/ship/ShipDefTable.h"
#include "game/core/GameSettings.h"
#include <math.h>
#include <cstddef>

class GameText;
class FModSound;
class Achievements;
class Galaxy;
class ImageFactory;
class AppManager;
class Mission;
class Item;
class Ship;
class ChoiceWindow;
class MissionsWindow;

static PaintCanvas *g_PaintCanvas = nullptr;

struct MtwAppData {
    uint8_t pad_0x0[5];
    uint8_t screenshotResultFlag;
    uint8_t pad_0x6[2];
    int storeResultCode;
    uint8_t storeInitFlag;
    uint8_t pad_0xd[0x30];
    uint8_t dlcMenuRequestFlag;
    uint8_t pad_0x3e[2];
    uint8_t purchaseReadyFlag;
    uint8_t purchaseResultFlag;
    uint8_t purchaseErrorFlag;
    uint8_t pad_0x43[5];
    unsigned int purchaseCode;
    uint8_t dlcMenuAckFlag;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(MtwAppData, screenshotResultFlag) == 0x5, "appdata 0x5");
static_assert(offsetof(MtwAppData, storeResultCode) == 0x8, "appdata 0x8");
static_assert(offsetof(MtwAppData, storeInitFlag) == 0xc, "appdata 0xc");
static_assert(offsetof(MtwAppData, dlcMenuRequestFlag) == 0x3d, "appdata 0x3d");
static_assert(offsetof(MtwAppData, purchaseReadyFlag) == 0x40, "appdata 0x40");
static_assert(offsetof(MtwAppData, purchaseResultFlag) == 0x41, "appdata 0x41");
static_assert(offsetof(MtwAppData, purchaseErrorFlag) == 0x42, "appdata 0x42");
static_assert(offsetof(MtwAppData, purchaseCode) == 0x48, "appdata 0x48");
static_assert(offsetof(MtwAppData, dlcMenuAckFlag) == 0x4c, "appdata 0x4c");
#endif

void *_mtw_GameText_getText(void *gt, int id); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_set(void *cw, void *s1, void *s2, bool b);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_render3D_inner(void *obj); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_Layout_OnTouchEnd(void *layout, int y, int x);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_ChoiceWindow_OnTouchEnd(void *cw, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_FModSound_resumeAll(void *snd); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_FModSound_stopAll();

void _mtw_AppMgr_SetCurrentApplicationModule(void *app, int id);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_AppMgr_Quit(void *app); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Globals_reportLeaderboards();

int _mtw_onTouchEnd_listState(void *self, int y, int x, int state);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_optionsState(void *self, int y, int x);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_scrollState(void *self, int y, int x, int which);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_missionsState(void *self, int y, int x);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_cinematicState(void *self, int y, int x);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_genericButtons(void *self, int y, int x, int fieldOff);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_TB_dtor(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_TS_dtor(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_Str_dtor(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_TouchButton_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_ChoiceWindow_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_ScrollTouchWindow_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_Str_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_StrArr_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Array_StrArr_ctor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Array_Str_ctor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArraySetLength_StrArr(int n, void *arr);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArraySetLength_Str(int n, void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Globals_longToTimeStringNoSeconds(long long t, void *out);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_formatCredits(void *out); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_ctor7(void *btn, void *label, int a, int x, int y, char type);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_resetGame();

void _mtw_Status_nextCampaignMission(bool a);

void _mtw_Mission_ctor(void *m); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_setMission(void *status); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Ship_makeShip(int shipDef); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_setShip(void *status); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Ship_setRace(void *ship, int race); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Item_makeItem(int itemDef, int qty); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_makeItem2(int itemDef); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_setCredits(void *status); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Galaxy_getStation(void *galaxy, int idx);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_setStation(void *status); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_setSystemVisibility(void *status, int sys, bool vis);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Achievements_setMedal(void *ach, int id, int n);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_RecordHandler_saveOptions(void *rh); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Status_setKills(void *status, int count);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_OnTouchBegin(void *cw, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_Layout_OnTouchBegin(void *layout, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchBegin(void *btn, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchBeginXY(void *btn, int y, int x);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

float _mtw_TouchButton_setPosition(void *btn, int x, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchSlider_OnTouchBegin(void *sl, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ScrollTouchWindow_OnTouchBegin(void *w, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_MissionsWindow_OnTouchBegin(void *w, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_FModSound_play(void *snd, int id, void *pos, float v);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_idiv(int a, int b);

void _mtw_RecordHandler_ctor(void *rh); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_RecordHandler_readRecord(void *rh); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_RecordHandler_dtor(void *rh); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_GameRecord_dtor(void *gr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_GameRecord_load(void *gr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_set(void *cw, void *s, bool b);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_AppMgr_GetApplicationModule(void *app, int id);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_getPosition(void *out, void *btn);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_setVisible(void *btn, bool v);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Array_GameRecord_dtor(void *p); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_RecordHandler_readAllPreviewRecords(void *rh);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_RecordHandler_recordStoreWrite(void *rh, int slot);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_RecordHandler_recordStoreWritePreview(void *rh, int slot);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_RecordHandler_recordStoreReadPreview(void *rh);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_createRecordButtons(void *self, bool b);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_AppMgr_GetApplicationData(); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_AppMgr_GetEngine(); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_set(void *cw, void *s); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_update(void *cw); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_setYPosition(void *btn, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_TouchButton_isVisible(void *btn); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ScrollTouchWindow_update(void *w); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_MissionsWindow_update(void *w); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_startSupernovaChallenge_impl(void *self);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_GetApplicationData(void *app); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_DlcMenu_call(void *win, void *s1, void *s2);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_drawBG();

void _mtw_ChoiceWindow_OnTouchMove(void *cw, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchMove(void *btn, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_OnTouchMoveXY(void *btn, int y, int x);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchSlider_OnTouchMove(void *sl, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

float _mtw_TouchSlider_getValue(void *sl); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ScrollTouchWindow_OnTouchMove(void *w, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_MissionsWindow_OnTouchMove(void *w, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_FModSound_tryToStopMusicForBGMusic();

void _mtw_FModSound_setVolume(void *snd, float v);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Array_TB_ctor(void *a); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_loadPreviewRecords(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_draw(void *btn); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_drawBox(void *layout, int mode, int x, int y, int w, int h, void *str);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ImageFactory_drawShip(void *imgF, unsigned int shipId, int x, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Ship_addCargo(void *ship, void *item);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

float _mtw_FModSound_stop(void *snd); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static GameText *const *const gSupernovaGameText = nullptr;

void MenuTouchWindow::showSupernovaMessage() {
    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    GameText *gt = *gSupernovaGameText;
    String *s1 = (String *) _mtw_GameText_getText(gt, 0x266);
    String *s2 = (String *) _mtw_GameText_getText(*gSupernovaGameText, 0x267);
    _mtw_ChoiceWindow_set(cw, s1, s2, false);
    this->messageShowing = 1;
    this->supernovaMessageShowing = 1;
}

bool MenuTouchWindow::isInMissionScreen() {
    return this->menuState == 9;
}

uint8_t MenuTouchWindow::isShowingMessage() {
    return this->messageShowing;
}

bool MenuTouchWindow::isMakingScreenshot() {
    return this->screenshotState < 0x80000000u;
}

void MenuTouchWindow::hideMessage() {
    this->messageShowing = 0;
}

void MenuTouchWindow::render3D() {
    if (this->menuState != 9)
        return;
    MissionsWindow *obj = (MissionsWindow *) this->missionsWindow;
    if (obj == 0)
        return;
    return _mtw_render3D_inner(obj);
}

bool MenuTouchWindow::inCinematicMode() {
    return this->menuState == 0xd;
}

float MenuTouchWindow::getRelativeScrollStartPos() {
    int offset = this->scrollOffset;
    if (offset > 0)
        return 0.0f;
    return -(float) offset / (float) this->pageHeight;
}

static uint8_t *const *const gEndLayoutGuard = nullptr;
static int *const *const gEndStatusObj = nullptr;
static AppManager *const *const gEndAppHolder = nullptr;
static FModSound *const *const gEndFmod = nullptr;
static AppManager *const *const gEndQuitApp = nullptr;
static int *const *const gEndModuleId = nullptr;
static int *const *const gEndPendingFlag = nullptr;

int MenuTouchWindow::OnTouchEnd(int y, int x, void *touchId) {
    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->menuState == 0xb && touchId != 0) {
        if (this->cinematicTouchIdA == touchId)
            this->cinematicTouchIdA = 0;
        if (this->cinematicTouchIdB == touchId)
            this->cinematicTouchIdB = 0;
        this->cinematicTouchState = 0;
    }

    this->pendingActivate = 0;
    this->dragging = 0;

    uint8_t *layoutGuard = (uint8_t *) *gEndLayoutGuard;
    if (*layoutGuard != 0) {
        if (_mtw_Layout_OnTouchEnd(layoutGuard, y, x) != 0)
            *layoutGuard = 0;
        return 0;
    }

    int state = this->menuState;

    if (state == 0) {
        if (this->messageShowing != 0) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            if (this->returnToMenuShowing != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    this->returnToMenuShowing = 0;
                    this->messageShowing = 0;
                } else if (r == 0) {
                    _mtw_AppMgr_Quit(*gEndQuitApp);
                }
                return 0;
            }
            if (this->genericConfirmB != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r != 1) {
                    if (r != 0) return 0;
                    **gEndPendingFlag = 1;
                }
                this->messageShowing = 0;
                this->genericConfirmB = 0;
                return 0;
            }
            if (this->quitConfirmShowing != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    this->quitConfirmShowing = 0;
                    this->messageShowing = 0;
                    this->returnToMenuSubFlag = 0;
                    return 0;
                }
                if (r != 0) return 0;
                _mtw_FModSound_resumeAll(*gEndFmod);
                _mtw_FModSound_stopAll();
                **gEndModuleId = 2;
                _mtw_AppMgr_SetCurrentApplicationModule(*gEndAppHolder,
                                                        **gEndModuleId);
                this->quitConfirmShowing = 0;
                this->messageShowing = 0;
                return 0;
            }
            if (this->leaderboardDialogShowing != 0) {
                unsigned int r = (unsigned int) _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r < 2) _mtw_Globals_reportLeaderboards();
                this->leaderboardDialogShowing = 0;
                this->messageShowing = 0;
            }
            if (this->dlcMessageShowing != 0) {
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                this->messageShowing = 0;
                return 0;
            }
            if (this->dlcErrorDialogShowing != 0 || this->dlcResultDialogShowing != 0) {
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                this->messageShowing = 0;
                return 0;
            }
            if (this->loadFailedDialogShowing != 0) {
                this->loadFailedDialogShowing = 0;
            } else if (this->genericConfirmA != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                this->messageShowing = 0;
                this->genericConfirmA = 0;
                return 0;
            } else if (this->supernovaMessageShowing != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                this->supernovaMessageShowing = 0;
            } else if (this->supernovaPurchaseDialogShowing != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                this->supernovaPurchaseDialogShowing = 0;
            } else {
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
            }
            this->messageShowing = 0;
            return 0;
        }
        return _mtw_onTouchEnd_genericButtons(this, y, x, 0x4), 0;
    }

    switch (state) {
        case 1:
        case 2:
            if (this->listStateSuppress != 0) return 0;
            _mtw_onTouchEnd_listState(this, y, x, state);
            break;
        case 3:
            _mtw_onTouchEnd_genericButtons(this, y, x, 0xac);
            break;
        case 4:
            _mtw_onTouchEnd_scrollState(this, y, x, 0xf0);
            break;
        case 7:
        case 8:
            _mtw_onTouchEnd_optionsState(this, y, x);
            break;
        case 9:
            _mtw_onTouchEnd_missionsState(this, y, x);
            break;
        case 0xb:
            _mtw_onTouchEnd_cinematicState(this, y, x);
            break;
        case 0xc:
            _mtw_onTouchEnd_genericButtons(this, y, x, 0xb4);
            break;
        case 0xd:
            _mtw_onTouchEnd_genericButtons(this, y, x, 0x4);
            break;
        case 0xe:
            _mtw_onTouchEnd_genericButtons(this, y, x, 0xb0);
            break;
        case 0xf:
        case 0x10:
            _mtw_onTouchEnd_scrollState(this, y, x, 0xf4);
            break;
        case 0x11:
            _mtw_onTouchEnd_genericButtons(this, y, x, 0xb8);
            break;
        default:
            _mtw_onTouchEnd_genericButtons(this, y, x, 0x4);
            break;
    }
    return 0;
}

void _mtw_ArrayReleaseClasses_TB(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArrayReleaseClasses_Str(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

template<class T>
static inline void freeObj(T **slot, void *(*dtor)(void *)) {
    // lint: void_ptr (dtor matches external void*(void*) deallocator ABI)
    T *o = *slot;
    if (o != 0) ::operator delete(dtor(o));
    *slot = 0;
}

MenuTouchWindow::~MenuTouchWindow() {
    {
        Array<TouchButton *> *a = this->buttons;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->buttons;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttons = 0;
    }
    {
        Array<TouchButton *> *a = this->optionsButtons;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->optionsButtons;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->optionsButtons = 0;
    }
    {
        Array<TouchButton *> *a = this->scrollEntries;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->scrollEntries;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->scrollEntries = 0;
    }
    {
        Array<TouchButton *> *a = this->buttonsB4;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->buttonsB4;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttonsB4 = 0;
    }
    {
        Array<TouchButton *> *a = this->buttonsB8;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->buttonsB8;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttonsB8 = 0;
    }
    {
        Array<TouchButton *> *a = this->buttonsB0;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->buttonsB0;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttonsB0 = 0;
    }

    {
        Array<TouchSlider *> *a = (Array<TouchSlider *> *) this->sliders;
        if (a != 0) {
            ArrayReleaseClasses<TouchSlider *>(*a);
            Array<TouchSlider *> *b = this->sliders;
            if (b != 0) ::operator delete(_mtw_Array_TS_dtor(b));
        }
        this->sliders = 0;
    }

    {
        Array<TouchButton *> *a = this->scrollSlots;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            Array<TouchButton *> *b = this->scrollSlots;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->scrollSlots = 0;
    }

    {
        Array<AbyssEngine::String *> **slots[2] = {&this->previewStrings0, &this->previewStrings1};
        for (int k = 0; k < 2; k++) {
            Array<AbyssEngine::String *> *a = *slots[k];
            if (a != 0) {
                _mtw_ArrayReleaseClasses_Str(a);
                Array<AbyssEngine::String *> *b = *slots[k];
                if (b != 0) ::operator delete(_mtw_Array_Str_dtor(b));
            }
            *slots[k] = 0;
        }
    }

    freeObj(&this->optBtnCC, _mtw_TouchButton_dtor);
    freeObj(&this->okButton, _mtw_TouchButton_dtor);
    freeObj(&this->optBtnD0, _mtw_TouchButton_dtor);
    freeObj(&this->optBtnD4, _mtw_TouchButton_dtor);
    freeObj(&this->optBtnD8, _mtw_TouchButton_dtor);
    freeObj(&this->optBtnDC, _mtw_TouchButton_dtor);

    freeObj(&this->choiceWindow, _mtw_ChoiceWindow_dtor);
    freeObj(&this->backButton, _mtw_TouchButton_dtor);
    freeObj(&this->scrollExtraButton, _mtw_TouchButton_dtor);

    freeObj(&this->scrollWindowA, _mtw_ScrollTouchWindow_dtor);
    freeObj(&this->scrollWindowB, _mtw_ScrollTouchWindow_dtor);

    if (this->heapBufA != 0) ::operator delete[](this->heapBufA);
    this->heapBufA = 0;
    if (this->heapBufB != 0) ::operator delete[](this->heapBufB);
    this->heapBufB = 0;
}

void _mtw_ArrayReleaseClasses_Str(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArrayReleaseClasses_StrArr(void *arr);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_String_op_plus(void *out, void *rhs);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_ctorFull(void *btn, void *label, int a, int x, int y, int w,
                               // lint: void_ptr (external symbol; param type is mangling-load-bearing)
                               char type, char id);

static int *const *const gCrbRowCount = nullptr;
static long *const *const gCrbTimeVal = nullptr;
static GameText *const *const *const gCrbGameText = nullptr;
static const char gCrbDash[] = "";
static Layout *const *const gCrbLayout = nullptr;
static int *const *const gCrbScreenW = nullptr;
static uint8_t *const *const gCrbBackFlag = nullptr;

void MenuTouchWindow::createRecordButtons(bool inSaveMode) {
    Array<Array<String *> *> *rows = this->recordRows;
    if (rows != 0) {
        for (uint32_t i = 0; i < rows->count; i++) {
            Array<String *> *row = rows->data_[i];
            if (row != 0) {
                _mtw_ArrayReleaseClasses_Str(row);
                Array<String *> *r2 = this->recordRows->data_[i];
                if (r2 != 0) ::operator delete(_mtw_Array_Str_dtor(r2));
                this->recordRows->data_[i] = 0;
                rows = this->recordRows;
            }
        }
        _mtw_ArrayReleaseClasses_StrArr(rows);
        if (this->recordRows != 0) ::operator delete(_mtw_Array_StrArr_dtor(this->recordRows));
        this->recordRows = 0;
    }

    Array<Array<String *> *> *outer = (Array<Array<String *> *> *) ::operator new(0xc);
    _mtw_Array_StrArr_ctor(outer);
    this->recordRows = outer;
    int rowCount = **gCrbRowCount;
    _mtw_ArraySetLength_StrArr(rowCount, outer);

    long *timeHolder = *gCrbTimeVal;
    GameText *const *gtHolder = *gCrbGameText;

    for (int i = 0; i < rowCount; i++) {
        Array<String *> *row = (Array<String *> *) ::operator new(0xc);
        _mtw_Array_Str_ctor(row);
        this->recordRows->data_[i] = row;
        _mtw_ArraySetLength_Str(6, this->recordRows->data_[i]);

        String s48;
        {
            if (s48.data) delete[] s48.data;
            s48.data = nullptr;
            s48.length = 0;
        }
        String s54;

        Array<GameRecord *> *rec = (Array<GameRecord *> *) this->previewRecords;
        bool empty = (rec == 0) || (rec->data_[i] == 0);

        if (empty) {
            {
                if (s54.data) delete[] s54.data;
                s54.data = nullptr;
                s54.length = 0;
            }
            _mtw_Globals_longToTimeStringNoSeconds((long long) (int) (long) timeHolder, 0);
            String *e;
            String **rowData = this->recordRows->data_[i]->data_;

            e = (String *) ::operator new(0xc);
            e->Set((s54).data);
            rowData[0] = e;
            e = (String *) ::operator new(0xc);
            e->Set(((String *) _mtw_GameText_getText(*gtHolder, 0xae))->data);
            rowData = this->recordRows->data_[i]->data_;
            rowData[1] = e;

            e = (String *) ::operator new(0xc);
            if (i == 0) e->Set(((String *) _mtw_GameText_getText(*gtHolder, 0x1e6))->data);
            else e->ctor_char(gCrbDash, false);
            rowData = this->recordRows->data_[i]->data_;
            rowData[2] = e;

            e = (String *) ::operator new(0xc);
            e->ctor_char(gCrbDash, false);
            rowData = this->recordRows->data_[i]->data_;
            rowData[3] = e;

            e = (String *) ::operator new(0xc);
            e->ctor_char(gCrbDash, false);
            rowData = this->recordRows->data_[i]->data_;
            rowData[4] = e;

            e = (String *) ::operator new(0xc);
            e->ctor_char(gCrbDash, false);
            rowData = this->recordRows->data_[i]->data_;
            rowData[5] = e;
        } else {
            {
                if (s54.data) delete[] s54.data;
                s54.data = nullptr;
                s54.length = 0;
            }
            GameRecord *slot = rec->data_[i];
            _mtw_Globals_longToTimeStringNoSeconds((long long) (int) (long) slot, slot->playTimeObj);
            String *e;
            String **rowData;

            e = (String *) ::operator new(0xc);
            e->Set((s54).data);
            rowData = this->recordRows->data_[i]->data_;
            rowData[0] = e;

            e = (String *) ::operator new(0xc);
            e->Set(((String *) &slot->pilotName)->data);
            rowData = this->recordRows->data_[i]->data_;
            rowData[1] = e;

            e = (String *) ::operator new(0xc);
            if (i == 0) e->Set(((String *) _mtw_GameText_getText(*gtHolder, 0x1e6))->data);
            else e->ctor_char(gCrbDash, false);
            rowData = this->recordRows->data_[i]->data_;
            rowData[2] = e;

            String *credits = (String *) ::operator new(0xc);
            _mtw_Layout_formatCredits(credits);
            rowData = this->recordRows->data_[i]->data_;
            rowData[3] = credits;

            String *combined = (String *) ::operator new(0xc);
            String *label = (String *) _mtw_GameText_getText(*gtHolder, 0x141);
            String s6c;
            s6c.ctor_char(gCrbDash, false);
            String s60;
            _mtw_String_op_plus(&s60, label);
            String s78;
            s78.Set(((String *) &slot->killsText)->data);
            _mtw_String_op_plus(combined, &s60);
            rowData = this->recordRows->data_[i]->data_;
            rowData[4] = combined;

            e = (String *) ::operator new(0xc);
            float rank = slot->rank;
            String *rankTxt;
            if (rank <= 0.0f) rankTxt = (String *) _mtw_GameText_getText(*gtHolder, 0x207);
            else if (rank <= 0.5f) rankTxt = (String *) _mtw_GameText_getText(*gtHolder, 0x207);
            else if (rank <= 1.0f) rankTxt = (String *) _mtw_GameText_getText(*gtHolder, 0x207);
            else rankTxt = (String *) _mtw_GameText_getText(*gtHolder, 0x19);
            e->Set(((String *) rankTxt)->data);
            rowData = this->recordRows->data_[i]->data_;
            rowData[5] = e;
        }
    }

    if (this->okButton != 0) {
        ::operator delete(_mtw_TouchButton_dtor(this->okButton));
        this->okButton = 0;
    }
    Layout *layout = (Layout *) *gCrbLayout;
    char backForm = **gCrbBackFlag;
    int extra = layout->field_0x108;
    TouchButton *okBtn = (TouchButton *) ::operator new(0xc8);
    int screenW = **gCrbScreenW;
    int x = (screenW - this->listX) - layout->buttonInsetX;
    int y = layout->field_0x20_top + layout->field_0xc_leftMargin + extra;
    String *okLabel = (String *) _mtw_GameText_getText(*gtHolder, inSaveMode ? 0x1fa : 0x1f9);
    if (backForm)
        _mtw_TouchButton_ctor7(okBtn, okLabel, 7, x, y, 0x12);
    else
        _mtw_TouchButton_ctorFull(okBtn, okLabel, 0, x, y, layout->field_0x2a4_metricA, 0x12, 0x04);
    this->okButton = okBtn;

    _mtw_TouchButton_setPosition(okBtn,
                                 (screenW - this->listX) - layout->buttonInsetX,
                                 (layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow
                                 + layout->field_0xc_leftMargin + extra + layout->field_0x20_top +
                                 this->scrollOffset);

    if (this->backButton != 0) {
        ::operator delete(_mtw_TouchButton_dtor(this->backButton));
        this->backButton = 0;
    }
    TouchButton *backBtn = (TouchButton *) ::operator new(0xc8);
    String *backLabel = (String *) _mtw_GameText_getText(*gtHolder, 0x41);
    _mtw_TouchButton_ctor7(backBtn, backLabel, 7,
                           (screenW - this->listX) - layout->buttonInsetX,
                           layout->field_0xc_leftMargin + extra + layout->field_0x20_top, 0x12);
    this->backButton = backBtn;
}

void *_mtw_Status_getShip(void *status); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Item_make(int itemDef); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Item_makeQty(int itemDef, int qty); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Ship_setItem(void *ship, void *item, int slot);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static Status *const *const gValStatus = nullptr;
static int *const *const gValShipTable = nullptr;
static Galaxy *const *const gValGalaxy = nullptr;
static OptionsRecord *const *const gValOptA = nullptr;
static Achievements *const *const gValAch = nullptr;
static OptionsRecord *const *const gValOptB = nullptr;
static RecordHandler *const *const gValOptHolder = nullptr;
static FModSound *const *const gValFmod = nullptr;
static ModuleTransitionThunk *const gValTransition = nullptr;

void MenuTouchWindow::startValkyrie() {
    Status *const *statusHolder = gValStatus;
    _mtw_Status_resetGame();
    for (int i = 0x2d; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool) (unsigned char) (long) *statusHolder);

    Status *status = *statusHolder;
    Mission *mission = (Mission *) ::operator new(0x78);
    _mtw_Mission_ctor(mission);
    _mtw_Status_setMission(status);

    ShipDefTable *row = *(ShipDefTable **) (*(int *) (*gValShipTable) + 4);
    status = *statusHolder;
    _mtw_Ship_makeShip(row->itemDef_secondary);
    _mtw_Status_setShip(status);

    Status *ship = *statusHolder;
    int race = (int) (long) _mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    Item *it;
    Ship *sh;
    it = (Item *) _mtw_Item_make(row->itemDef_primary);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = (Item *) _mtw_Item_make(row->itemDef_secondary);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = (Item *) _mtw_Item_makeQty(row->itemDef_missile, 0xa);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0x144);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0xcc);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = (Item *) _mtw_Item_makeQty(row->itemDef_equip0x158, 0xa);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 2);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0x154);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 3);

    _mtw_Status_setCredits(*statusHolder);
    Status *station = *statusHolder;
    _mtw_Galaxy_getStation(*gValGalaxy, 0x5b);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    OptionsRecord *opt = (OptionsRecord *) *gValOptA;
    Achievements *ach = *gValAch;
    OptionsRecord *optB = (OptionsRecord *) *gValOptB;
    ((Status *) *statusHolder)->field_8c = 0x1a0a;
    opt->flag_word_0x8 = 0x101;
    opt->flag_word_0xd = 0x101;
    opt->flag_word_0x1c = 0x101;
    opt->flag_dword_0x20 = 0x1010101;
    opt->flag_0x17 = 1;
    opt->flag_0xa = 1;
    opt->flag_0x15 = 1;
    opt->flag_0x13 = 1;
    opt->flag_0xf = 1;
    opt->flag_0x1e = 1;
    opt->flag_0x24 = 1;
    opt->flag_0x36 = 0;
    reinterpret_cast<int32_t &>(opt->field_0x32[0]) = 0;
    opt->flag_0x38 = 1;
    optB->flag_0x34 = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*gValAch, 0x1e, 1);
    _mtw_RecordHandler_saveOptions(*gValOptHolder);
    _mtw_Status_setKills(*statusHolder, 0xc5);
    _mtw_FModSound_stop(*gValFmod);

    ModuleTransitionThunk *thunk = gValTransition;
    optB->fadeValue = this->fadeValue;
    thunk->transitionFn(reinterpret_cast<void *&>(thunk->transitionFn), 5); // lint: void_ptr (thunk slot reused as opaque context)
}

static Layout *const *const gBgLayout = nullptr;
static int *const *const gBgScreenW = nullptr;
static int *const *const gBgScrollDiv = nullptr;
static int *const *const gBgListPosX = nullptr;
static FModSound *const *const gBgFmod = nullptr;
static uint8_t *const *const gBgFlagA = nullptr;
static uint8_t *const *const gBgFlagB = nullptr;
static uint8_t *const *const gBgFlagC = nullptr;
static uint8_t *const *const gBgFlagD = nullptr;
static GameSettings *const *const gBgObjA = nullptr;
static GameSettings *const *const gBgObjB = nullptr;
static int *const *const gBgScreenH2 = nullptr;
static int *const *const gBgScrollImg = nullptr;
static int *const *const gBgScreenW2 = nullptr;

int MenuTouchWindow::OnTouchBegin(int y, int x, void *touchId) {
    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->messageShowing != 0) {
        _mtw_ChoiceWindow_OnTouchBegin(this->choiceWindow, y);
        return 0;
    }

    Layout *layout = (Layout *) *gBgLayout;
    if (layout->choiceWindowOpen != 0) {
        _mtw_Layout_OnTouchBegin(layout, y);
        return 0;
    }

    int state = this->menuState;
    switch (state) {
        case 1:
        case 2: {
            this->dragStartX = x;
            this->dragLastX = x;
            this->dragVelocity = 0;
            this->dragging = 1;
            int oldRow = this->selectedRow;
            int leftMargin = layout->field_0xc_leftMargin;
            if (leftMargin < x && x < **gBgScreenW - layout->field_0x10_rightMargin) {
                int rowH = layout->field_0x70_rowHeight;
                int gap = this->listRowGap;
                int top = layout->field_0x20_top;
                int off = this->scrollOffset;
                int row = _mtw_idiv(x + ((-top - leftMargin) - off), 1);
                if (row < **gBgScrollDiv) {
                    int *posX = (int *) *gBgListPosX;
                    this->selectedRow = row;
                    float v = _mtw_TouchButton_setPosition(this->okButton,
                                                           (*posX - this->listX) - layout->buttonInsetX,
                                                           row * (gap + rowH) + top + leftMargin + off +
                                                           layout->field_0x108);
                    _mtw_FModSound_play(*gBgFmod, 0x7c, 0, v);
                }
            }
            if (oldRow == this->selectedRow)
                _mtw_TouchButton_OnTouchBegin(this->okButton, y);
        }
        break;
        case 3: {
            if (**gBgFlagA == 0) {
                Array<TouchButton *> *arr = (Array<TouchButton *> *) this->optionsButtons;
                for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                    _mtw_TouchButton_OnTouchBegin(arr->data_[i], y);
                break;
            }
            _mtw_TouchButton_OnTouchBegin(this->scrollUpButton, y);
            this->upButtonPressed = 0;
            int b28 = layout->buttonInsetX;
            int top = b28 + this->listTopY;
            if (top < y && y < this->listEntryHeight + top &&
                layout->field_0xc_leftMargin + b28 < x &&
                x < layout->field_0xc_leftMargin + layout->field_0x20_top + this->listEntryWidth) {
                this->upButtonPressed = 1;
                _mtw_FModSound_play(*gBgFmod, 0x7c, 0, 0);
                b28 = layout->buttonInsetX;
                top = b28 + this->listTopY;
            }
            int bottom = top + this->listEntryHeight;
            if (bottom < y && y < (this->listTopY - b28) + this->listBottomY &&
                b28 + layout->field_0xc_leftMargin < x &&
                x < layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth) {
                this->downButtonPressed = 1;
                _mtw_FModSound_play(*gBgFmod, 0x7c, 0, 0);
            }
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnCC, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD0, y, x);
            _mtw_TouchSlider_OnTouchBegin(((Array<TouchSlider *> *) this->sliders)->data_[0], y);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD4, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD8, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnDC, y, x);
            Array<TouchSlider *> *arr = (Array<TouchSlider *> *) this->sliders;
            for (unsigned int i = 1; i < *(unsigned int *) arr; i++)
                _mtw_TouchSlider_OnTouchBegin(arr->data_[i], y);
            if (**gBgFlagB != 0 && this->scrollExtraButton != 0)
                _mtw_TouchButton_OnTouchBegin(this->scrollExtraButton, y);
        }
        break;
        case 4: {
            _mtw_ScrollTouchWindow_OnTouchBegin(this->scrollWindowA, y);
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) arr->data_[i];
                unsigned int t = e[0];
                if (((unsigned int) (t - 0x6a) < 5 && ((1u << ((t - 0x6a) & 0xff)) & 0x19u) != 0)
                    || (e[1] == 0 && t == 0x16))
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
        }
        break;
        case 6:
        case 0xd:
            break;
        case 7: {
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD4, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD8, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnDC, y, x);
            Array<TouchSlider *> *arr = (Array<TouchSlider *> *) this->sliders;
            unsigned int n = *(unsigned int *) arr;
            for (unsigned int i = 1; i < n; i++) {
                if (i == 5 && ((Layout *) *gBgLayout)->field_0x284_sliderSlot5Enabled == 0) continue;
                _mtw_TouchSlider_OnTouchBegin(arr->data_[i], y);
            }
        }
        break;
        case 8: {
            this->upButtonPressed = 0;
            int b28 = layout->buttonInsetX;
            int top = b28 + this->listTopY;
            if (top < y && y < this->listEntryHeight + top &&
                layout->field_0xc_leftMargin + b28 < x &&
                x < layout->field_0xc_leftMargin + layout->field_0x20_top + this->listEntryWidth) {
                this->upButtonPressed = 1;
                _mtw_FModSound_play(*gBgFmod, 0x7c, 0, 0);
                layout = (Layout *) *gBgLayout;
                b28 = layout->buttonInsetX;
                top = b28 + this->listTopY;
            }
            int bottom = top + this->listEntryHeight;
            if (bottom < y && y < (this->listTopY - b28) + this->listBottomY &&
                b28 + layout->field_0xc_leftMargin < x &&
                x < layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth) {
                this->downButtonPressed = 1;
                _mtw_FModSound_play(*gBgFmod, 0x7c, 0, 0);
            }
            _mtw_TouchButton_OnTouchBegin(this->optBtnCC, y);
            _mtw_TouchButton_OnTouchBegin(this->optBtnD0, y);
            _mtw_TouchSlider_OnTouchBegin(((Array<TouchSlider *> *) this->sliders)->data_[0], y);
        }
        break;
        case 9:
            _mtw_MissionsWindow_OnTouchBegin(this->missionsWindow, y);
            break;
        case 0xb: {
            if (**gBgFlagC != 0 && **gBgFlagD == 0) {
                _mtw_TouchButton_OnTouchBegin(this->cinematicBtnA, y);
                _mtw_TouchButton_OnTouchBegin(this->cinematicBtnB, y);
                GameSettings *steerCtl = *gBgObjA;
                GameSettings *fireCtl = *gBgObjB;
                if (touchId != 0 &&
                    (this->cinematicTouchIdA != 0 || y > 0xd1 || this->cinematicTouchIdB == touchId ||
                     x <= steerCtl->steerAnchorX - 0x14 ||
                     steerCtl->steerAnchorX + 300 <= x)) {
                    if (this->cinematicTouchIdB == touchId || touchId == 0 ||
                        this->cinematicTouchIdB != 0 || y <= **gBgScreenH2 - 0xdc ||
                        x <= fireCtl->fireAnchorX - 0x14 ||
                        fireCtl->fireAnchorX + 0xe6 <= x) {
                        this->cinematicTouchState = 0;
                    } else {
                        this->cinematicTouchIdB = touchId;
                        this->cinematicTouchState = 0x100;
                        this->cinematicAnchorB = x;
                    }
                } else {
                    this->cinematicTouchIdA = touchId;
                    this->cinematicTouchState = 1;
                    this->cinematicAnchorA = x;
                }
            }
        }
        break;
        case 0xc: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttonsB4;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(arr->data_[i], y);
        }
        break;
        case 0xe: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttonsB0;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(arr->data_[i], y);
        }
        break;
        case 0x10: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) arr->data_[i];
                if ((unsigned int) (e[0] - 0x65) < 5 && e[1] == 0)
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
        }
            [[fallthrough]];
        case 0xf: {
            _mtw_ScrollTouchWindow_OnTouchBegin(this->scrollWindowB, y);
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) arr->data_[i];
                if ((e[0] | 8) == 0x3c && e[1] == 0)
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
            Array<TouchButton *> *slots = (Array<TouchButton *> *) this->scrollSlots;
            for (unsigned int i = 0; i < *(unsigned int *) slots; i++)
                _mtw_TouchButton_OnTouchBegin(slots->data_[i], y);

            int *img = *gBgScrollImg;
            int bound = **gBgScreenW2;
            int b28 = ((Layout *) *gBgLayout)->buttonInsetX;
            int iw = g_PaintCanvas->GetImage2DWidth((unsigned int) (long) img);
            unsigned char hit;
            if ((bound - b28) - iw < y) {
                int lc = ((Layout *) *gBgLayout)->field_0xc_leftMargin;
                int tp = ((Layout *) *gBgLayout)->field_0x20_top;
                int ih = g_PaintCanvas->GetImage2DHeight((unsigned int) (long) img);
                hit = (x < ih + tp + lc) ? 1 : 0;
            } else hit = 0;
            this->scrollbarHit = hit;

            b28 = ((Layout *) *gBgLayout)->buttonInsetX;
            iw = g_PaintCanvas->GetImage2DWidth((unsigned int) (long) img);
            if (y < iw + b28) {
                this->dragStartX = x;
                this->dragLastX = x;
                this->dragVelocity = 0;
                this->dragging = 1;
            }
        }
        break;
        case 0x11: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttonsB8;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(arr->data_[i], y);
        }
        break;
        default: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttons;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(arr->data_[i], y);
            Array<TouchButton *> *arr2 = (Array<TouchButton *> *) this->scrollEntries;
            unsigned int n = *(unsigned int *) arr2;
            for (unsigned int i = 0; i < n; i++) {
                int *e = (int *) arr2->data_[i];
                if ((unsigned int) (e[0] - 0x17) < 2) _mtw_TouchButton_OnTouchBegin(e, y);
            }
            for (unsigned int i = 0; i < n; i++) {
                unsigned int *e = (unsigned int *) arr2->data_[i];
                unsigned int t = e[0], id = e[1];
                bool go;
                if (t == 5 && id == 0) go = true;
                else {
                    unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                    go = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
                }
                if (go) {
                    _mtw_TouchButton_OnTouchBegin(e, y);
                    arr2 = (Array<TouchButton *> *) this->scrollEntries;
                    n = *(unsigned int *) arr2;
                }
            }
        }
        break;
    }

    int r = _mtw_Layout_OnTouchBegin(*gBgLayout, y);
    if (r != 0 && this->menuState == 0xd)
        this->pendingActivate = 1;
    return 0;
}

static Status *const *const gLoadStatusFlags = nullptr;
static GameText *const *const gLoadGameText = nullptr;
static AppManager *const *const *const gLoadAppHolder = nullptr;
static int *const *const gLoadResetCell = nullptr;

int MenuTouchWindow::loadGame(int slot) {
    RecordHandler *rh = (RecordHandler *) ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    GameRecord *rec = (GameRecord *) _mtw_RecordHandler_readRecord(rh);

    if (rec == 0) {
        _mtw_Status_resetGame();
        ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
        String *s = (String *) _mtw_GameText_getText(*gLoadGameText, 0x64);
        _mtw_ChoiceWindow_set(cw, s, false);
        this->loadFailedDialogShowing = 1;
        this->messageShowing = 1;
        ::operator delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    Status *flags = (Status *) *gLoadStatusFlags;
    GameRecord *record = (GameRecord *) rec;
    bool versionOk = (record->versionMismatchFlag == 0) || (reinterpret_cast<uint8_t *>(&flags->systemVisibilities)[3] != 0);
    if (versionOk) {
        bool dlcOk = (record->dlcRequiredFlag == 0) || (reinterpret_cast<uint8_t *>(&flags->systemVisibilities)[1] != 0);
        if (dlcOk) {
            _mtw_Status_resetGame();
            _mtw_GameRecord_load(rec);
            ::operator delete(_mtw_RecordHandler_dtor(rh));
            ::operator delete(_mtw_GameRecord_dtor(rec));
            AppManager *const *app = *gLoadAppHolder;
            ModStation *ms = (ModStation *) _mtw_AppMgr_GetApplicationModule(*app, 5);
            ((ModStation *) ms)->setGameLoaded();
            **gLoadResetCell = 0;
            _mtw_AppMgr_SetCurrentApplicationModule(*app, 5);
            return 1;
        }

        ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
        String *s = (String *) _mtw_GameText_getText(*gLoadGameText, 0x65);
        _mtw_ChoiceWindow_set(cw, s, false);
        this->messageShowing = 1;
        ::operator delete(_mtw_GameRecord_dtor(rec));
        ::operator delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    String *s = (String *) _mtw_GameText_getText(*gLoadGameText, 0x66);
    _mtw_ChoiceWindow_set(cw, s, false);
    this->messageShowing = 1;
    ::operator delete(_mtw_GameRecord_dtor(rec));
    ::operator delete(_mtw_RecordHandler_dtor(rh));
    return 0;
}

void _mtw_TouchButton_ctor(void *btn, void *label, int a, int x, int y, int w,
                           // lint: void_ptr (external symbol; param type is mangling-load-bearing)
                           char type, char id);

static int *const *const gAddBtnScreenW = nullptr;
static Layout *const *const gAddBtnLayout = nullptr;
static int *const *const gAddBtnScreenH = nullptr;
static int *const *const gAddBtnPosX = nullptr;
static int *const *const gAddBtnPosY = nullptr;
static uint32_t *const *const gAddBtnCount = nullptr;

void MenuTouchWindow::addButton(int id, AbyssEngine::String label, int row, Array<TouchButton *> *arr, int yOff) {
    TouchButton *btn = (TouchButton *) ::operator new(0xc8);

    int btnW = this->buttonWidth;
    int screenW = **gAddBtnScreenW;
    int screenH = **gAddBtnScreenH;
    int rowH = ((Layout *) *gAddBtnLayout)->field_0x30_rowHeight;

    int x = screenW / 2 - btnW / 2;
    int y = (rowH + this->buttonRowGap) * row + (yOff - this->buttonYBias) + screenH / 2;

    _mtw_TouchButton_ctor(btn, &label, 0, x, y, btnW, 0x11, 0x04);
    ((TouchButton *) btn)->field_0x0 = id;
    ((TouchButton *) btn)->field_0x4 = id >> 31;
    ArrayAdd<TouchButton *>((TouchButton *) btn, *arr);

    int *posX = (int *) *gAddBtnPosX;
    int *posY = (int *) *gAddBtnPosY;
    for (uint32_t i = 0; i < arr->count; i++) {
        if (i < 10) {
            char buf[12];
            TouchButton *b = arr->data_[i];
            _mtw_TouchButton_getPosition(buf, b);
            posX[i] = (int) *(float *) (buf + 0);
            b = arr->data_[i];
            _mtw_TouchButton_getPosition(buf, b);
            posY[i] = (int) *(float *) (buf + 4);
        }
    }
    **gAddBtnCount = arr->count;
}

void MenuTouchWindow::setCutsceneMode(bool mode) {
    this->cutsceneMode = (uint8_t) mode;
    Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttons;
    for (uint32_t i = 0; i < *(uint32_t *) arr; i++) {
        TouchButton *btn = (TouchButton *) arr->data_[i];
        if (btn->field_0x0 == 0x13 && btn->field_0x4 == 0) {
            _mtw_TouchButton_setVisible(btn, (bool) ((uint8_t) mode ^ 1));
        }
        arr = (Array<TouchButton *> *) this->buttons;
    }
}

static int *const *const gPrevScreenW = nullptr;
static Layout *const *const gPrevLayout = nullptr;
static int *const *const gPrevRowCnt = nullptr;

typedef void (*RefreshFn)();

static RefreshThunk *const gPrevRefreshThunk = nullptr;

void MenuTouchWindow::loadPreviewRecords() {
    this->scrollOffset = 0;

    this->dragLastX = 0;
    this->dragVelocity = 0;
    this->inertiaDecay = 0;
    this->inertiaVel = 0.0f;
    this->dragStartX = 0;
    this->dragging = 0;

    int *metrics = (int *) *gPrevScreenW;
    Layout *layout = (Layout *) *gPrevLayout;
    int *rowObj = (int *) *gPrevRowCnt;

    this->contentHeight = (((metrics[0] - layout->field_0xc_leftMargin) - layout->windowTopInset)
                           - layout->field_0x20_top) - layout->field_0x24;

    this->pageHeight = rowObj[0] * (layout->field_0x70_rowHeight + this->listRowGap);

    Array<GameRecord *> *rec = this->previewRecords;
    if (rec != 0) {
        ::operator delete(_mtw_Array_GameRecord_dtor(rec));
        this->previewRecords = 0;
    }
    RecordHandler *rh = (RecordHandler *) ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    this->previewRecords = (Array<GameRecord *> *) _mtw_RecordHandler_readAllPreviewRecords(rh);
    _mtw_RecordHandler_dtor(rh);

    gPrevRefreshThunk->refreshFn();
}

static GameText *const *const gSaveGameText = nullptr;

void MenuTouchWindow::saveGame(int slot) {
    RecordHandler *rh = (RecordHandler *) ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    _mtw_RecordHandler_recordStoreWrite(rh, slot);
    _mtw_RecordHandler_recordStoreWritePreview(rh, slot);

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

    GameRecord *preview = (GameRecord *) _mtw_RecordHandler_recordStoreReadPreview(rh);
    this->previewRecords->data_[slot] = preview;
    ::operator delete(_mtw_RecordHandler_dtor(rh));

    _mtw_createRecordButtons(this, true);
    ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
    String *s = (String *) _mtw_GameText_getText(*gSaveGameText, 0x32);
    _mtw_ChoiceWindow_set(cw, s, false);
    this->saveDialogShowing = 0;
    this->messageShowing = 1;
}

static uint8_t *const *const gUpStatusGuard = nullptr;
static Layout *const *const gUpLayout = nullptr;
static int *const *const gUpScreenW = nullptr;
static PaintCanvas *const *const gUpCanvas = nullptr;
static OptionsRecord *const *const gUpOptObj = nullptr;
static RecordHandler *const *const gUpRecHandler = nullptr;
static GameText *const *const gUpGameText = nullptr;
static Status *const *const gUpStatusObj = nullptr;
static OptionsRecord *const *const gUpDlcFlags = nullptr;
static int *const *const gUpScreenH = nullptr;
static Layout *const *const gUpListLayout = nullptr;

static const int g_mtw_upTextIds[16] = {};

void MenuTouchWindow::update(int dt) {
    unsigned int st = this->menuState;
    if (st < 0x10 && ((1u << (st & 0xff)) & 0x8006u) != 0) {
        if (this->dragging == 0) {
            float v = *(float *) &this->inertiaDecay * this->inertiaVel;
            this->inertiaVel = v;
            if (fabsf(v) > 1.0f) {
                this->scrollOffset = (int) (v + (float) this->scrollOffset);
            }
        }
        int off = this->scrollOffset;
        if (off > 0) {
            this->inertiaDecay = 0x3f800000;
            this->inertiaVel = (float) (-off) * 0.5f;
        }
        int overshoot = this->contentHeight - this->pageHeight;
        if (overshoot < 0) {
            if (off < overshoot) {
                this->inertiaDecay = 0x3f800000;
                this->inertiaVel = (float) (overshoot - off) * 0.5f;
            }
        } else {
            this->inertiaVel = 0;
            this->scrollOffset = 0;
        }
    }

    MtwAppData *appData = (MtwAppData *) _mtw_AppMgr_GetApplicationData();
    unsigned char busy = this->dlcMessageShowing;

    if (busy != 0 || this->purchaseRestorePending != 0) {
        if (appData->purchaseReadyFlag != 0) {
            Layout *layout = (Layout *) *gUpLayout;
            PaintCanvas *canvas = *gUpCanvas;
            this->contentHeight = (((**gUpScreenW - layout->field_0x10_rightMargin)
                                    - layout->field_0xc_leftMargin) - layout->field_0x20_top) - layout->field_0x24;
            int ih = ((PaintCanvas *) canvas)->GetImage2DHeight(this->scrollbarImageId);
            this->contentHeightCache = ih;
            OptionsRecord *optObj = (OptionsRecord *) *gUpOptObj;
            int rowH = layout->field_0x2c_rowHeight;
            this->menuState = 0xf;
            this->messageShowing = 0;
            this->dlcMessageShowing = 0;
            this->pageHeight = (ih + rowH) * 5;
            appData->purchaseReadyFlag = 0;
            optObj->flag_0x3b = 1;
            _mtw_RecordHandler_saveOptions(*gUpRecHandler);
            busy = this->dlcMessageShowing;
        }
        if (busy != 0) {
            if (appData->purchaseErrorFlag != 0) {
                ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
                String *s = (String *) _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[0]);
                _mtw_ChoiceWindow_set(cw, s);
                this->dlcErrorDialogShowing = 1;
                this->messageShowing = 1;
                this->dlcMessageShowing = 0;
                appData->purchaseErrorFlag = 0;
                this->purchaseRestorePending = 0;
            }
        }
    }

    unsigned int code = appData->purchaseCode;
    bool handled = false;
    if (code <= 4 && appData->purchaseResultFlag != 0) {
        ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
        switch (code) {
            case 0: {
                Status *status = (Status *) *gUpStatusObj;
                reinterpret_cast<uint8_t *>(&status->systemVisibilities)[1] = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[1]));
            }
            break;
            case 1: {
                reinterpret_cast<int32_t &>(((Status *) *gUpStatusObj)->field_120) = 3;
                ((OptionsRecord *) *gUpOptObj)->flag_0x36 = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[2]));
            }
            break;
            case 2: {
                Status *status = (Status *) *gUpStatusObj;
                reinterpret_cast<uint8_t *>(&status->systemVisibilities)[3] = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[3]));
            }
            break;
            case 3: {
                ((OptionsRecord *) *gUpOptObj)->flag_0x38 = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[4]));
            }
            break;
            case 4: {
                OptionsRecord *flags = (OptionsRecord *) *gUpOptObj;
                Status *status = (Status *) *gUpStatusObj;
                flags->flag_0x35 = 1;
                flags->flag_0x39 = 1;
                flags->flag_0x37 = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[5]));
            }
            break;
        }
        _mtw_RecordHandler_saveOptions(*gUpRecHandler);
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        appData->purchaseResultFlag = 0;
        handled = true;
    } else if (this->purchaseRestorePending != 0 && appData->purchaseResultFlag != 0) {
        _mtw_RecordHandler_saveOptions(*gUpRecHandler);
        if (this->purchaseRestorePending != 0) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[6]));
            this->purchaseRestorePending = 0;
        }
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        appData->purchaseResultFlag = 0;
        handled = true;
    }
    (void) handled;

    if (**gUpStatusGuard != 0) {
        OptionsRecord *flags = (OptionsRecord *) *gUpDlcFlags;
        if (this->messageShowing == 0 && flags->flag_0x35 == 0) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, g_mtw_upTextIds[7]));
            this->supernovaPurchaseDialogShowing = 1;
            this->messageShowing = 1;
            flags->flag_0x35 = 1;
            _mtw_Status_setSystemVisibility(*gUpStatusObj, 0x19, true);
            _mtw_RecordHandler_saveOptions(*gUpRecHandler);
        }
    }

    int state = this->menuState;
    if (state == 0) {
        if (this->backgroundEnabled == 1) {
            int rowGap = this->buttonRowGap;
            Layout *layout = (Layout *) *gUpListLayout;
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttons;
            unsigned int n = *(unsigned int *) arr;
            int total = n * layout->field_0x30_rowHeight;
            int screenH = **gUpScreenH;
            for (unsigned int i = 0; i < n; i++) {
                _mtw_TouchButton_setYPosition(arr->data_[i],
                                              (layout->field_0x30_rowHeight + this->buttonRowGap) * i +
                                              (screenH / 2 - (int) ((unsigned int) (rowGap * (n - 1) + total) >> 1)));
                TouchButton *b = (TouchButton *) this->buttons->data_[i];
                if (b->field_0x0 == 0x12 && b->field_0x4 == 0 && _mtw_TouchButton_isVisible(b) != 0 &&
                    this->cutsceneMode != 0) {
                    b = (TouchButton *) this->buttons->data_[i];
                    char pos[12];
                    _mtw_TouchButton_getPosition(pos, b);
                    _mtw_TouchButton_setYPosition(b, (int) *(float *) (pos + 4));
                }
                arr = (Array<TouchButton *> *) this->buttons;
                n = *(unsigned int *) arr;
            }
        }
    } else if (state == 3) {
        int rowGap = this->buttonRowGap;
        Layout *layout = (Layout *) *gUpListLayout;
        Array<TouchButton *> *arr = (Array<TouchButton *> *) this->optionsButtons;
        unsigned int n = *(unsigned int *) arr;
        int firstCount = *(int *) *(void **) this->buttons;
        // lint: void_ptr (decompiled artifact: reads Array header word as a pointer)
        int total = n * layout->field_0x30_rowHeight;
        int screenH = **gUpScreenH;
        for (unsigned int i = 0; i < n; i++) {
            _mtw_TouchButton_setYPosition(arr->data_[i],
                                          (layout->field_0x30_rowHeight + this->buttonRowGap) * i +
                                          (screenH / 2 - (int) (
                                               (unsigned int) ((firstCount - 1) * rowGap + total) >> 1)));
            arr = (Array<TouchButton *> *) this->optionsButtons;
            n = *(unsigned int *) arr;
        }
    } else if (state == 4) {
        _mtw_ScrollTouchWindow_update(this->scrollWindowA);
    } else if (state == 9) {
        if (this->missionsWindow != 0) _mtw_MissionsWindow_update(this->missionsWindow);
    } else if (state == 0xf) {
        _mtw_ScrollTouchWindow_update(this->scrollWindowB);
    }

    if (this->messageShowing != 0)
        _mtw_ChoiceWindow_update(this->choiceWindow);

    if (state == 0xd) {
        appData = (MtwAppData *) _mtw_AppMgr_GetApplicationData();
        if (appData->storeInitFlag != 0) {
            Engine *eng = (Engine *) _mtw_AppMgr_GetEngine();
            int r = eng->field_0x100;
            if (r == 2 || r == 1) {
                ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
                int id = (r == 2) ? g_mtw_upTextIds[8] : g_mtw_upTextIds[9];
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, id));
                this->storeInitDialogShowing = 0;
                this->messageShowing = 1;
                appData = (MtwAppData *) _mtw_AppMgr_GetApplicationData();
                appData->storeInitFlag = 0;
            }
        }
    }

    appData = (MtwAppData *) _mtw_AppMgr_GetApplicationData();
    if (appData->screenshotResultFlag != 0) {
        appData = (MtwAppData *) _mtw_AppMgr_GetApplicationData();
        int r = appData->storeResultCode;
        if (r == 2 || r == 1) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            int id = (r == 2) ? g_mtw_upTextIds[10] : g_mtw_upTextIds[11];
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*gUpGameText, id));
            this->screenshotState = -1;
            this->messageShowing = 1;
            appData = (MtwAppData *) _mtw_AppMgr_GetApplicationData();
            appData->screenshotResultFlag = 0;
        }
    }
}

void MenuTouchWindow::startSupernovaChallenge() {
    _mtw_startSupernovaChallenge_impl(this);
}

static AppManager *const *const gAppHolder = nullptr;
static GameText *const *const gDlcGameText = nullptr;

void MenuTouchWindow::callDlcMenu() {
    AppManager *const *holder = gAppHolder;

    MtwAppData *ad = (MtwAppData *) _mtw_GetApplicationData(*holder);
    ad->dlcMenuAckFlag = 0;
    ad = (MtwAppData *) _mtw_GetApplicationData(*holder);
    GameText *const *gt = gDlcGameText;
    ad->dlcMenuRequestFlag = 1;
    ChoiceWindow *win = (ChoiceWindow *) this->choiceWindow;
    this->messageShowing = 1;
    this->dlcMessageShowing = 1;
    String *s1 = (String *) _mtw_GameText_getText(*gt, 0x47);
    String *s2 = (String *) _mtw_GameText_getText(*gt, 0x1a9);
    return _mtw_DlcMenu_call(win, s1, s2);
}

void _mtw_draw_mainMenu(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_loadSaveList(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_optionsTabs(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_scrollA(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_audioOptions(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_keyConfig(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_missions(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_cinematic(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_buttonsB4(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_store(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_buttonsB0(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_scrollB(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_draw_buttonsB8(void *self); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static int *const *const gDrawDrawingFlag = nullptr;
static int *const *const gDrawStatePub = nullptr;
static int *const *const gDrawPosX = nullptr;
static int *const *const gDrawPosY = nullptr;
static unsigned int *const *const gDrawCountObj = nullptr;
static int *const *const gDrawColorSrc = nullptr;

void MenuTouchWindow::draw() {
    **gDrawDrawingFlag = 1;
    int state = this->menuState;
    **gDrawStatePub = state;

    Array<TouchButton *> *btnArr = this->buttons;
    if (state == 0xc) btnArr = this->buttonsB4;
    if (state == 0xe) btnArr = this->buttonsB0;
    Array<TouchButton *> *arr = btnArr;

    int *posX = *gDrawPosX;
    int *posY = *gDrawPosY;
    for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
        if (i < 10) {
            char buf[12];
            _mtw_TouchButton_getPosition(buf, arr->data_[i]);
            posX[i] = (int) *(float *) (buf + 0);
            _mtw_TouchButton_getPosition(buf, arr->data_[i]);
            posY[i] = (int) *(float *) (buf + 4);
        }
    }
    **gDrawCountObj = *(unsigned int *) arr;

    if (this->backgroundEnabled != 0) {
        unsigned int s = this->menuState;
        if ((s | 4) != 0xd) {
            _mtw_Layout_drawBG();
        }
    }

    switch (this->menuState) {
        case 0: _mtw_draw_mainMenu(this);
            break;
        case 1:
        case 2: _mtw_draw_loadSaveList(this);
            break;
        case 3: _mtw_draw_optionsTabs(this);
            break;
        case 4: _mtw_draw_scrollA(this);
            break;
        case 7: _mtw_draw_audioOptions(this);
            break;
        case 8: _mtw_draw_keyConfig(this);
            break;
        case 9: _mtw_draw_missions(this);
            break;
        case 0xb: _mtw_draw_cinematic(this);
            break;
        case 0xc: _mtw_draw_buttonsB4(this);
            break;
        case 0xd: _mtw_draw_store(this);
            break;
        case 0xe: _mtw_draw_buttonsB0(this);
            break;
        case 0xf:
        case 0x10: _mtw_draw_scrollB(this);
            break;
        case 0x11: _mtw_draw_buttonsB8(this);
            break;
        default: break;
    }
}

float MenuTouchWindow::getRelativeScrollHeight() {
    int content = this->contentHeight;
    int page = this->pageHeight;
    if (page < content)
        return 0.0f;
    int off = this->scrollOffset;
    int numer;
    if (off >= 1) {
        numer = content - off;
    } else {
        if (content - page <= off) {
            return (float) content / (float) page;
        }
        numer = off + page;
    }
    return (float) numer / (float) page;
}

void _mtw_steerFromTouchId(void *self, int y, int x, void *touchId);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_OnTouchMove(void *layout, int y);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static Layout *const *const gMvLayout = nullptr;
static int *const *const gMvScreenW = nullptr;
static uint8_t *const *const gMvFlagA = nullptr;
static uint8_t *const *const gMvFlagB = nullptr;
static FModSound *const *const gMvFmod = nullptr;

static int *const *const gMvScrollBound = nullptr;

int MenuTouchWindow::OnTouchMove(int y, int x, void *touchId) {
    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->messageShowing != 0) {
        _mtw_ChoiceWindow_OnTouchMove(this->choiceWindow, y);
        return 0;
    }
    Layout *layout = (Layout *) *gMvLayout;
    if (layout->choiceWindowOpen != 0) {
        _mtw_Layout_OnTouchMove(layout, y);
        return 0;
    }

    int state = this->menuState;
    switch (state - 1) {
        case 0:
        case 1:
            if (layout->field_0xc_leftMargin < x &&
                x < **gMvScreenW - layout->field_0x10_rightMargin) {
                int dx = x - this->dragLastX;
                this->dragLastX = x;
                this->dragVelocity = dx;
                this->inertiaDecay = 0x3f800000;
                this->scrollOffset = this->scrollOffset + dx;
            }
            _mtw_TouchButton_OnTouchMove(this->okButton, y);
            break;
        case 2: {
            if (**gMvFlagA == 0) {
                Array<TouchButton *> *arr = (Array<TouchButton *> *) this->optionsButtons;
                for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                    _mtw_TouchButton_OnTouchMove(arr->data_[i], y);
            } else {
                _mtw_TouchButton_OnTouchMove(this->okButton, y);
                this->upButtonPressed = 0;
                int base = layout->buttonInsetX;
                int top = base + this->listTopY;
                int bottom = this->listEntryHeight + top;
                if (top < y && y < bottom &&
                    layout->field_0xc_leftMargin + base < x &&
                    x < layout->field_0xc_leftMargin + layout->field_0x20_top + this->listEntryWidth)
                    this->upButtonPressed = 1;
                if (bottom < y && y < (this->listTopY - base) + this->listBottomY &&
                    base + layout->field_0xc_leftMargin < x &&
                    x < layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth)
                    this->downButtonPressed = 1;
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnCC, y, x);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnD0, y, x);
                _mtw_TouchSlider_OnTouchMove(((Array<TouchSlider *> *) this->sliders)->data_[0], y);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnD4, y, x);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnD8, y, x);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnDC, y, x);
                {
                    FModSound *fmod = *gMvFmod;
                    TouchSlider **sl = ((Array<TouchSlider *> *) this->sliders)->data_;
                    if (_mtw_FModSound_tryToStopMusicForBGMusic() == 0)
                        _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(sl[1]));
                    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(sl[2]));
                    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(sl[3]));
                }
                Array<TouchSlider *> *arr = (Array<TouchSlider *> *) this->sliders;
                for (unsigned int i = 1; i < *(unsigned int *) arr; i++)
                    _mtw_TouchSlider_OnTouchMove(arr->data_[i], y);
                if (**gMvFlagB != 0 && this->okButton != 0)
                    _mtw_TouchButton_OnTouchMove(this->okButton, y);
            }
        }
        break;
        case 3: {
            _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowA, y);
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) arr->data_[i];
                unsigned int t = e[0];
                if (((unsigned int) (t - 0x6a) < 5 && ((1u << ((t - 0x6a) & 0xff)) & 0x19u) != 0)
                    || (e[1] == 0 && t == 0x16))
                    _mtw_TouchButton_OnTouchMove(e, y);
            }
        }
        break;
        case 5:
        case 0xc:
            break;
        case 6:
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnD4, y, x);
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnD8, y, x);
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnDC, y, x);
            {
                FModSound *fmod = *gMvFmod;
                TouchSlider **sl = ((Array<TouchSlider *> *) this->sliders)->data_;
                if (_mtw_FModSound_tryToStopMusicForBGMusic() == 0)
                    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(sl[1]));
                _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(sl[2]));
                _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(sl[3]));
            }
            {
                Array<TouchSlider *> *arr = (Array<TouchSlider *> *) this->sliders;
                for (unsigned int i = 1; i < *(unsigned int *) arr; i++) {
                    if (i == 5 && ((Layout *) *gMvLayout)->field_0x284_sliderSlot5Enabled == 0) continue;
                    _mtw_TouchSlider_OnTouchMove(arr->data_[i], y);
                }
            }
            break;
        case 7: {
            this->upButtonPressed = 0;
            int base = layout->buttonInsetX;
            int top = base + this->listTopY;
            int bottom = this->listEntryHeight + top;
            if (top < y && y < bottom &&
                layout->field_0xc_leftMargin + base < x &&
                x < layout->field_0xc_leftMargin + layout->field_0x20_top + this->listEntryWidth)
                this->upButtonPressed = 1;
            if (bottom < y && y < (this->listTopY - base) + this->listBottomY &&
                base + layout->field_0xc_leftMargin < x &&
                x < layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth)
                this->downButtonPressed = 1;
            _mtw_TouchButton_OnTouchMove(this->optBtnCC, y);
            _mtw_TouchButton_OnTouchMove(this->optBtnD0, y);
            _mtw_TouchSlider_OnTouchMove(((Array<TouchSlider *> *) this->sliders)->data_[0], y);
        }
        break;
        case 8:
            _mtw_MissionsWindow_OnTouchMove(this->missionsWindow, y);
            break;
        case 0xa:
            _mtw_steerFromTouchId(this, y, x, touchId);
            break;
        case 0xb: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttonsB4;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(arr->data_[i], y);
        }
        break;
        case 0xd: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttonsB0;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(arr->data_[i], y);
        }
        break;
        case 0xe: {
            Layout *slayout = (Layout *) *gMvLayout;

            _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowB, y);

            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) arr->data_[i];
                if ((e[0] | 8) == 0x3c && e[1] == 0)
                    _mtw_TouchButton_OnTouchMove(e, y);
            }

            Array<TouchButton *> *slots = (Array<TouchButton *> *) this->scrollSlots;
            for (unsigned int i = 0; i < *(unsigned int *) slots; i++)
                _mtw_TouchButton_OnTouchMove(slots->data_[i], y);

            if (slayout->field_0xc_leftMargin < x &&
                x < **gMvScrollBound - slayout->field_0x10_rightMargin) {
                int b28 = slayout->buttonInsetX;
                int iw = g_PaintCanvas->GetImage2DWidth(*(unsigned int *) this->heapBufA);
                if (y < iw + b28) {
                    int dx = x - this->dragLastX;
                    this->dragLastX = x;
                    this->dragVelocity = dx;
                    this->inertiaDecay = 0x3f800000;
                    this->scrollOffset = this->scrollOffset + dx;
                }
            }
            break;
        }
        case 0xf: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) arr->data_[i];
                if ((unsigned int) (e[0] - 0x65) < 5)
                    _mtw_TouchButton_OnTouchMove(e, y);
            }
            {
                Layout *slayout = (Layout *) *gMvLayout;

                _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowB, y);

                Array<TouchButton *> *arr2 = (Array<TouchButton *> *) this->scrollEntries;
                for (unsigned int i = 0; i < *(unsigned int *) arr2; i++) {
                    unsigned int *e = (unsigned int *) arr2->data_[i];
                    if ((e[0] | 8) == 0x3c && e[1] == 0)
                        _mtw_TouchButton_OnTouchMove(e, y);
                }

                Array<TouchButton *> *slots = (Array<TouchButton *> *) this->scrollSlots;
                for (unsigned int i = 0; i < *(unsigned int *) slots; i++)
                    _mtw_TouchButton_OnTouchMove(slots->data_[i], y);

                if (slayout->field_0xc_leftMargin < x &&
                    x < **gMvScrollBound - slayout->field_0x10_rightMargin) {
                    int b28 = slayout->buttonInsetX;
                    int iw = g_PaintCanvas->GetImage2DWidth(*(unsigned int *) this->heapBufA);
                    if (y < iw + b28) {
                        int dx = x - this->dragLastX;
                        this->dragLastX = x;
                        this->dragVelocity = dx;
                        this->inertiaDecay = 0x3f800000;
                        this->scrollOffset = this->scrollOffset + dx;
                    }
                }
            }
        }
        break;
        case 0x10: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttonsB8;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(arr->data_[i], y);
        }
        break;
        default: {
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttons;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(arr->data_[i], y);
            Array<TouchButton *> *arr2 = (Array<TouchButton *> *) this->scrollEntries;
            unsigned int n = *(unsigned int *) arr2;
            for (unsigned int i = 0; i < n; i++) {
                int *e = (int *) arr2->data_[i];
                if ((unsigned int) (e[0] - 0x17) < 2)
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
            for (unsigned int i = 0; i < n; i++) {
                unsigned int *e = (unsigned int *) arr2->data_[i];
                unsigned int t = e[0], id = e[1];
                bool hit;
                if (t == 5 && id == 0) hit = true;
                else {
                    unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                    hit = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
                }
                if (hit) {
                    _mtw_TouchButton_OnTouchMove(e, y);
                    arr2 = (Array<TouchButton *> *) this->scrollEntries;
                    n = *(unsigned int *) arr2;
                }
            }
        }
        break;
    }

    _mtw_Layout_OnTouchMove(*gMvLayout, y);
    return 0;
}

void _mtw_buildMenu(void *self, int menuType); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static Layout *const *const gCtorLayout = nullptr;

MenuTouchWindow::MenuTouchWindow(int menuType) {
    Layout *layout = (Layout *) *gCtorLayout;
    this->buttonWidth = layout->field_0x294_buttonWidth;
    this->buttonYBias = layout->field_0x298_buttonYBias;
    this->buttonRowGap = layout->field_0x29c_buttonRowGap;
    this->listRowGap = layout->field_0x2a0_listRowGap;
    this->metricA = layout->field_0x2a4_metricA;
    this->metricB = layout->field_0x2a8_metricB;
    this->metricC = layout->field_0x2ac_metricC;

    this->backgroundEnabled = menuType;

    Array<TouchButton *> *arr1 = (Array<TouchButton *> *) ::operator new(0xc);
    _mtw_Array_TB_ctor(arr1);
    this->buttons = arr1;
    Array<TouchButton *> *arr2 = (Array<TouchButton *> *) ::operator new(0xc);
    _mtw_Array_TB_ctor(arr2);
    this->scrollEntries = arr2;

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

    _mtw_loadPreviewRecords(this);

    _mtw_buildMenu(this, menuType);
}

void MenuTouchWindow::setSkipButtonVisible(bool visible) {
    Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttons;
    if (arr != 0) {
        for (uint32_t i = 0; i < *(uint32_t *) arr; i++) {
            TouchButton *btn = (TouchButton *) arr->data_[i];
            if (btn != 0 && btn->field_0x0 == 0x12 && btn->field_0x4 == 0) {
                _mtw_TouchButton_setVisible(btn, visible);
                arr = (Array<TouchButton *> *) this->buttons;
            }
        }
    }
}

static Layout *const *const gDlsLayout = nullptr;
static PaintCanvas *const *const gDlsCanvas = nullptr;
static int *const *const gDlsScreenW = nullptr;
static uint8_t *const *const gDlsScrollOn = nullptr;
static uint8_t *const *const gDlsScrollNo = nullptr;
static uint8_t *const *const gDlsFlagA = nullptr;
static int *const *const gDlsFlagB = nullptr;
static uint8_t *const *const gDlsTileCnt = nullptr;
static int *const *const gDlsRowCount = nullptr;
static int *const *const gDlsRowMax = nullptr;
static const char gDlsBoxStr[] = "";
static int *const *const *const gDlsFont = nullptr;
static ImageFactory *const *const gDlsImgFact = nullptr;

void MenuTouchWindow::drawLoadSaveMenu(bool param1) {
    (void) param1;
    Layout *layout = (Layout *) *gDlsLayout;
    PaintCanvas *canvas = *gDlsCanvas;

    int rowBaseY = layout->field_0x10c_rowBaseY;
    ((PaintCanvas *) canvas)->SetColor(0xffffffff);

    int scrollOff = this->listX;
    int margin = layout->buttonInsetX;
    int strip58 = layout->field_0x110_strip58;
    int strip5c = layout->field_0x114_strip5c;
    int screenBound = **gDlsScreenW;
    int inner = screenBound + margin * -2 + scrollOff * -2;

    if (**gDlsScrollOn != 0 && **gDlsScrollNo == 0) {
        strip5c = 8;
        if (**gDlsFlagA == 0) {
            strip58 = 0xc;
            if (**gDlsTileCnt == 0) {
                strip5c = 4;
                strip58 = 6;
            }
        } else {
            strip58 = 8;
            strip5c = 5;
        }
        int iw = ((PaintCanvas *) canvas)->GetImage2DWidth(this->scrollbarImageId);
        int ih = ((PaintCanvas *) canvas)->GetImage2DHeight(this->scrollbarImageId);
        int count = _mtw_idiv((int) (long) *gDlsTileCnt, 1);
        int yy = 0;
        for (int k = 0; k <= count; k++) {
            ((PaintCanvas *) canvas)->DrawImage2D(this->scrollbarImageId,
                                                  (layout->buttonInsetX - iw) + this->listX, yy, (unsigned char) 1);
            ((PaintCanvas *) canvas)->DrawImage2D(this->scrollbarImageId,
                                                  layout->buttonInsetX + inner + this->listX, yy, (unsigned char) 0);
            yy += ih;
        }
        scrollOff = this->listX;
        margin = layout->buttonInsetX;
    }

    _mtw_TouchButton_setPosition(this->okButton,
                                 (screenBound - scrollOff) - margin,
                                 (layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow
                                 + this->scrollOffset
                                 + layout->field_0x20_top + layout->field_0xc_leftMargin + layout->field_0x108);

    int rowCount = **gDlsRowCount;
    int rowMax = **gDlsRowMax;

    for (int i = 0; i < rowCount; i++) {
        int rowY = (layout->field_0x70_rowHeight + this->listRowGap) * i + this->scrollOffset
                   + layout->field_0x20_top + layout->field_0xc_leftMargin;
        if (rowY < 0 || rowY > rowMax) continue;

        ((PaintCanvas *) canvas)->SetColor(0xffffffff);
        int boxX = layout->buttonInsetX + this->listX;
        String box;
        box.ctor_char(gDlsBoxStr, false);
        int mode = (i == this->selectedRow) ? 4 : 3;
        _mtw_Layout_drawBox(layout, mode, boxX, rowY, inner - 3, layout->field_0x70_rowHeight, &box);

        int *font = **gDlsFont;
        int yName = strip58 + rowY;
        String **cols = this->recordRows->data_[i]->data_;

        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[0],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c_rowHeight,
                                             (char) yName, false);

        GameRecord *slot = ((Array<GameRecord *> *) this->previewRecords)->data_[i];
        if (slot != 0) {
            unsigned int shipId = slot->shipId;
            if (shipId < 0x40) {
                _mtw_ImageFactory_drawShip(*gDlsImgFact, shipId,
                                           layout->field_0x2c_rowHeight + layout->buttonInsetX + this->listX +
                                           this->metricB,
                                           rowBaseY + rowY);
            }
        }

        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[1],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c_rowHeight * 2 +
                                             this->metricB + layout->field_0x2c4,
                                             yName, (bool) 0);

        ((PaintCanvas *) canvas)->SetColor(0x777777ff);
        int rowY2 = rowY + strip5c;
        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[2],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c_rowHeight,
                                             rowY2 + layout->field_0x70_rowHeight / 2, (bool) 0);

        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[3],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c_rowHeight * 2 +
                                             this->metricB + layout->field_0x2c4,
                                             rowY2 + layout->field_0x70_rowHeight / 2, (bool) 0);

        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[4],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c4 +
                                             (layout->field_0x2c_rowHeight + this->metricA) * 2,
                                             yName, (bool) 0);

        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[5],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c4 +
                                             (layout->field_0x2c_rowHeight + this->metricA) * 2,
                                             rowY2 + layout->field_0x70_rowHeight / 2, (bool) 0);

        if (i == this->selectedRow)
            _mtw_TouchButton_draw(this->okButton);
    }
}

void *_mtw_Status_getShip(void *status); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Item_make(int itemDef); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_Item_makeQty(int itemDef, int qty); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Ship_setItem(void *ship, void *item, int slot);

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static Status *const *const gSnStatus = nullptr;
static int *const *const gSnShipTable = nullptr;
static Galaxy *const *const gSnGalaxy = nullptr;
static OptionsRecord *const *const gSnOptA = nullptr;
static Achievements *const *const gSnAch = nullptr;
static OptionsRecord *const *const gSnOptB = nullptr;
static RecordHandler *const *const gSnOptHolder = nullptr;
static FModSound *const *const gSnFmod = nullptr;
static ModuleTransitionThunk *const gSnTransition = nullptr;

void MenuTouchWindow::startSupernova() {
    Status *const *statusHolder = gSnStatus;
    _mtw_Status_resetGame();
    for (int i = 0x54; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool) (unsigned char) (long) *statusHolder);

    _mtw_Status_setMission(*statusHolder);
    Status *status = *statusHolder;
    ShipDefTable *row = *(ShipDefTable **) (*(int *) (*gSnShipTable) + 4);
    _mtw_Ship_makeShip(row->shipDefId_0x78);
    _mtw_Status_setShip(status);

    Status *ship = *statusHolder;
    int race = (int) (long) _mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    Item *it;
    Ship *sh;
    it = (Item *) _mtw_Item_make(row->itemDef_0x2c0);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = (Item *) _mtw_Item_make(row->itemDef_0x50);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = (Item *) _mtw_Item_makeQty(row->itemDef_missile, 0x14);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = (Item *) _mtw_Item_makeQty(row->itemDef_0xb0, 0x14);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0x144);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0xcc);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = (Item *) _mtw_Item_make(row->itemDef_0x110);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 2);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0x158);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 3);
    it = (Item *) _mtw_Item_make(row->itemDef_equip0x154);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 4);
    it = (Item *) _mtw_Item_make(row->itemDef_0xe0);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 5);
    it = (Item *) _mtw_Item_makeQty(row->itemDef_cargo0x1e8, 8);
    sh = (Ship *) _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_addCargo(sh, it);

    _mtw_Status_setCredits(*statusHolder);
    Status *station = *statusHolder;
    _mtw_Galaxy_getStation(*gSnGalaxy, 0x46);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    OptionsRecord *opt = (OptionsRecord *) *gSnOptA;
    OptionsRecord *optB = (OptionsRecord *) *gSnOptB;
    ((Status *) *statusHolder)->field_8c = 0x1a0a;
    opt->flag_dword_0x20 = 0x1010101;
    opt->flag_0x17 = 1;
    opt->flag_0x36 = 0;
    reinterpret_cast<int32_t &>(opt->field_0x32[0]) = 0;
    opt->flag_word_0x8 = 0x101;
    opt->flag_0xa = 1;
    opt->flag_0x15 = 1;
    opt->flag_0x13 = 1;
    opt->flag_word_0xd = 0x101;
    opt->flag_0xf = 1;
    opt->flag_0x26 = 1;
    opt->flag_0x31 = 1;
    opt->flag_0x24 = 1;
    opt->flag_word_0x1c = 0x101;
    opt->flag_0x1e = 1;
    reinterpret_cast<uint16_t &>(opt->flag_0x38) = 0x101;
    Achievements *ach = *gSnAch;
    optB->flag_0x34 = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*gSnAch, 0x1e, 1);
    _mtw_Status_setKills(*statusHolder, 0x182);
    _mtw_RecordHandler_saveOptions(*gSnOptHolder);
    _mtw_FModSound_stop(*gSnFmod);

    ModuleTransitionThunk *thunk = gSnTransition;
    optB->fadeValue = this->fadeValue;
    thunk->transitionFn(reinterpret_cast<void *&>(thunk->transitionFn), 5); // lint: void_ptr (thunk slot reused as opaque context)
}

static FModSound *const *const gGof2Fmod = nullptr;
static Status *const *const gGof2StatusObj = nullptr;
static ModuleTransitionThunk *const *const gGof2Transition = nullptr;

void MenuTouchWindow::startGOF2() {
    _mtw_Status_resetGame();
    FModSound *snd = *gGof2Fmod;

    ((Status *) *gGof2StatusObj)->fadeValue = this->fadeValue;
    float v = _mtw_FModSound_stop(snd);
    _mtw_FModSound_play(snd, 0x8f, 0, v);
    ModuleTransitionThunk *thunk = *gGof2Transition;
    reinterpret_cast<void (*)(void *, int, int)>(thunk->transitionFn)(reinterpret_cast<void *&>(thunk->transitionFn), 2, 0); // lint: void_ptr (thunk slot cast to 3-arg variant; opaque context)
}

