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

static inline void *_mtw_GameText_getText(void *gt, int id) { return ((GameText*)gt)->getText(id); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ChoiceWindow_set(void *cw, void *s1, void *s2, bool b); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_render3D_inner(void *obj); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline int _mtw_Layout_OnTouchEnd(void *layout, int y, int x) { return ((Layout*)layout)->OnTouchEnd(x, y); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_ChoiceWindow_OnTouchEnd(void *cw, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_FModSound_resumeAll(void *snd) { ((FModSound*)snd)->resumeAll(); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_FModSound_stopAll() { Globals::sound->stopAll(); }

static inline void _mtw_AppMgr_SetCurrentApplicationModule(void *app, int id) { ((AbyssEngine::ApplicationManager*)app)->SetCurrentApplicationModule(id); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

static inline void _mtw_AppMgr_Quit(void *app) { ((AbyssEngine::ApplicationManager*)app)->Quit(); } // lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_listState(void *self, int y, int x, int state); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_optionsState(void *self, int y, int x); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_scrollState(void *self, int y, int x, int which); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_missionsState(void *self, int y, int x); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_cinematicState(void *self, int y, int x); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mtw_onTouchEnd_genericButtons(void *self, int y, int x, int fieldOff); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

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

// g_android_link_game_gp is the Google-Play "link game" flag (JniBridge.cpp)
extern int g_android_link_game_gp;

int MenuTouchWindow::OnTouchEnd(int y, int x, void *touchId) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
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

    uint8_t *layoutGuard = (uint8_t *) Globals::layout;
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
                    _mtw_AppMgr_Quit(Globals::appManager);
                }
                return 0;
            }
            if (this->genericConfirmB != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r != 1) {
                    if (r != 0) return 0;
                    g_android_link_game_gp = 1;
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
                _mtw_FModSound_resumeAll(Globals::sound);
                _mtw_FModSound_stopAll();
                Globals::switch_to_target_setting = 2;
                _mtw_AppMgr_SetCurrentApplicationModule(Globals::appManager,
                                                        Globals::switch_to_target_setting);
                this->quitConfirmShowing = 0;
                this->messageShowing = 0;
                return 0;
            }
            if (this->leaderboardDialogShowing != 0) {
                unsigned int r = (unsigned int) _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r < 2) Globals::globals->reportLeaderboards();
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
static inline void freeObj(T **slot, void *(*dtor)(void *)) { // lint: void_ptr (dtor matches external void*(void*) deallocator ABI)
    // lint: void_ptr (dtor matches external void*(void*) deallocator ABI)
    T *o = *slot;
    if (o != 0) ::operator delete(dtor(o));
    *slot = 0;
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

void _mtw_ArrayReleaseClasses_Str(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_ArrayReleaseClasses_StrArr(void *arr); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void *_mtw_String_op_plus(void *out, void *rhs); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_TouchButton_ctorFull(void *btn, void *label, int a, int x, int y, int w, // lint: void_ptr (external symbol; param type is mangling-load-bearing)
                               // lint: void_ptr (external symbol; param type is mangling-load-bearing)
                               char type, char id);

static const char gCrbDash[] = "";

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
            float rank = (*this->previewRecords)[i]->rank;
            (*(*this->recordRows)[i])[5] = new String(rank <= 0.0f   ? *Globals::gameText->getText(519)
                                                      : rank <= 0.5f ? *Globals::gameText->getText(519)
                                                      : rank <= 1.0f ? *Globals::gameText->getText(519)
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
                                (Globals::layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow +
                                (Globals::layout->field_0xc_leftMargin + y + Globals::layout->field_0x20_top +
                                 this->scrollOffset));
    if (this->backButton != 0) {
        delete this->backButton;
        this->backButton = 0;
    }
    this->backButton = new TouchButton(*Globals::gameText->getText(65), 7,
                                       Globals::w - this->listX - Globals::layout->buttonInsetX,
                                       Globals::layout->field_0xc_leftMargin + y + Globals::layout->field_0x20_top, 18);
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


int MenuTouchWindow::OnTouchBegin(int x, int y, void *touchId) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->messageShowing) {
        this->choiceWindow->OnTouchBegin(x, y);
        return 0;
    }
    if (Globals::layout->choiceWindowOpen) {
        Globals::layout->OnTouchBegin(x, y);
        return 0;
    }
    switch (this->menuState) {
    case 1:
    case 2: {
        // Load / save list.
        this->dragStartX = y;
        this->dragLastX = y;
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
                if (x > Globals::layout->buttonInsetX + this->listTopY &&
                    x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                    y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth) {
                    this->upButtonPressed = 1;
                    Globals::sound->play(124, 0, 0, 0.0f);
                }
                if (x > Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
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
    case 6:
    case 13:
        break;
    case 7:
        this->optBtnD4->OnTouchBegin(x, y);
        this->optBtnD8->OnTouchBegin(x, y);
        this->optBtnDC->OnTouchBegin(x, y);
        for (unsigned int i = 1; i < this->sliders->size(); i++) {
            if (i != 5 || Globals::layout->field_0x284_sliderSlot5Enabled)
                (*this->sliders)[i]->OnTouchBegin(x, y);
        }
        break;
    case 8:
            this->upButtonPressed = 0;
            this->downButtonPressed = 0;
            if (x > Globals::layout->buttonInsetX + this->listTopY &&
                x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth) {
                this->upButtonPressed = 1;
                Globals::sound->play(124, 0, 0, 0.0f);
            }
            if (x > Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
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
    case 9:
        this->missionsWindow->OnTouchBegin(x, y);
        break;
    case 11:
        // Touch control preview: grab the steering pad (left) or the fire buttons (right).
        if (Globals::iPad && !Globals::iPadAssetsWithLowerRes) {
            this->cinematicBtnA->OnTouchBegin(x, y);
            this->cinematicBtnB->OnTouchBegin(x, y);
            if (touchId != 0 && this->cinematicTouchIdA == 0 && x <= 209 && this->cinematicTouchIdB != touchId &&
                y > *(int *) &Globals::options[84] - 20 && y < *(int *) &Globals::options[84] + 300) {
                this->cinematicTouchIdA = touchId;
                this->cinematicTouchState = 1;
                this->cinematicAnchorA = y;
            } else if (this->cinematicTouchIdA != touchId && touchId != 0 && this->cinematicTouchIdB == 0 &&
                       x > Globals::w - 220 && y > *(int *) &Globals::options[88] - 20 &&
                       y < *(int *) &Globals::options[88] + 230) {
                this->cinematicTouchIdB = touchId;
                this->cinematicTouchState = 0x100;
                this->cinematicAnchorB = y;
            } else {
                this->cinematicTouchState = 0;
            }
        }
        break;
    case 12:
        for (unsigned int i = 0; i < this->buttonsB4->size(); i++)
            (*this->buttonsB4)[i]->OnTouchBegin(x, y);
        break;
    case 14:
        for (unsigned int i = 0; i < this->buttonsB0->size(); i++)
            (*this->buttonsB0)[i]->OnTouchBegin(x, y);
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
            if (id == 52 || id == 60)
                (*this->scrollEntries)[i]->OnTouchBegin(x, y);
        }
        for (unsigned int i = 0; i < this->scrollSlots->size(); i++)
            (*this->scrollSlots)[i]->OnTouchBegin(x, y);
        this->scrollbarHit =
            x > Globals::w - Globals::layout->buttonInsetX -
                    Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
            y < Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]) +
                    (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin);
        if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
            this->dragStartX = y;
            this->dragLastX = y;
            this->dragVelocity = 0;
            this->dragging = 1;
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
    return 0;
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


typedef void (*RefreshFn)();


void MenuTouchWindow::loadPreviewRecords() {
    this->scrollOffset = 0;

    this->dragLastX = 0;
    memset(this->pad_0x210, 0, sizeof(this->pad_0x210));
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


static const int g_mtw_upTextIds[16] = {};

void MenuTouchWindow::update(int dt) {
    unsigned int st = this->menuState;
    if (st < 0x10 && ((1u << (st & 0xff)) & 0x8006u) != 0) {
        if (this->dragging == 0) {
            float v = this->inertiaDecay * this->inertiaVel;
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

    MtwAppData *appData = (MtwAppData *) Globals::appManager->GetApplicationData();
    unsigned char busy = this->dlcMessageShowing;

    if (busy != 0 || this->purchaseRestorePending != 0) {
        if (appData->purchaseReadyFlag != 0) {
            Layout *layout = Globals::layout;
            PaintCanvas *canvas = Globals::Canvas;
            this->contentHeight = (((Globals::h - layout->field_0x10_rightMargin)
                                    - layout->field_0xc_leftMargin) - layout->field_0x20_top) - layout->field_0x24;
            int ih = ((PaintCanvas *) canvas)->GetImage2DHeight(this->scrollbarImageId);
            this->contentHeightCache = ih;
            OptionsRecord *optObj = (OptionsRecord *) Globals::options;
            int rowH = layout->field_0x2c_rowHeight;
            this->menuState = 0xf;
            this->messageShowing = 0;
            this->dlcMessageShowing = 0;
            this->pageHeight = (ih + rowH) * 5;
            appData->purchaseReadyFlag = 0;
            optObj->flag_0x3b = 1;
            Globals::recordHandler->saveOptions();
            busy = this->dlcMessageShowing;
        }
        if (busy != 0) {
            if (appData->purchaseErrorFlag != 0) {
                ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
                String *s = (String *) _mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[0]);
                cw->set(*s);
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
                Status *status = Globals::status;
                reinterpret_cast<uint8_t *>(&status->systemVisibilities)[1] = 1;
                status->setSystemVisibility(0x19, true);
                cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[1]));
            }
            break;
            case 1: {
                reinterpret_cast<int32_t &>((Globals::status)->field_120) = 3;
                ((OptionsRecord *) Globals::options)->flag_0x36 = 1;
                cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[2]));
            }
            break;
            case 2: {
                Status *status = Globals::status;
                reinterpret_cast<uint8_t *>(&status->systemVisibilities)[3] = 1;
                status->setSystemVisibility(0x19, true);
                cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[3]));
            }
            break;
            case 3: {
                ((OptionsRecord *) Globals::options)->flag_0x38 = 1;
                cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[4]));
            }
            break;
            case 4: {
                OptionsRecord *flags = (OptionsRecord *) Globals::options;
                Status *status = Globals::status;
                flags->flag_0x35 = 1;
                flags->flag_0x39 = 1;
                flags->flag_0x37 = 1;
                status->setSystemVisibility(0x19, true);
                cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[5]));
            }
            break;
        }
        Globals::recordHandler->saveOptions();
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        appData->purchaseResultFlag = 0;
        handled = true;
    } else if (this->purchaseRestorePending != 0 && appData->purchaseResultFlag != 0) {
        Globals::recordHandler->saveOptions();
        if (this->purchaseRestorePending != 0) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[6]));
            this->purchaseRestorePending = 0;
        }
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        appData->purchaseResultFlag = 0;
        handled = true;
    }
    (void) handled;

    if (Globals::isTelekomCustomer != 0) {
        OptionsRecord *flags = (OptionsRecord *) Globals::options;
        if (this->messageShowing == 0 && flags->flag_0x35 == 0) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, g_mtw_upTextIds[7]));
            this->supernovaPurchaseDialogShowing = 1;
            this->messageShowing = 1;
            flags->flag_0x35 = 1;
            Globals::status->setSystemVisibility(0x19, true);
            Globals::recordHandler->saveOptions();
        }
    }

    int state = this->menuState;
    if (state == 0) {
        if (this->backgroundEnabled == 1) {
            int rowGap = this->buttonRowGap;
            Layout *layout = Globals::layout;
            Array<TouchButton *> *arr = (Array<TouchButton *> *) this->buttons;
            unsigned int n = *(unsigned int *) arr;
            int total = n * layout->field_0x30_rowHeight;
            int screenH = Globals::h;
            for (unsigned int i = 0; i < n; i++) {
                arr->data_[i]->setYPosition((layout->field_0x30_rowHeight + this->buttonRowGap) * i +
                                            (screenH / 2 - (int) ((unsigned int) (rowGap * (n - 1) + total) >> 1)));
                TouchButton *b = (TouchButton *) this->buttons->data_[i];
                if (b->id == 0x12 && b->isVisible() &&
                    this->cutsceneMode != 0) {
                    b = (TouchButton *) this->buttons->data_[i];
                    Vector bpos = b->getPosition();
                    b->setYPosition((int) bpos.y);
                }
                arr = (Array<TouchButton *> *) this->buttons;
                n = *(unsigned int *) arr;
            }
        }
    } else if (state == 3) {
        int rowGap = this->buttonRowGap;
        Layout *layout = Globals::layout;
        Array<TouchButton *> *arr = (Array<TouchButton *> *) this->optionsButtons;
        unsigned int n = *(unsigned int *) arr;
        int firstCount = *(int *) *(void **) this->buttons; // lint: void_ptr (decompiled artifact: reads Array header word as a pointer)
        // lint: void_ptr (decompiled artifact: reads Array header word as a pointer)
        int total = n * layout->field_0x30_rowHeight;
        int screenH = Globals::h;
        for (unsigned int i = 0; i < n; i++) {
            arr->data_[i]->setYPosition((layout->field_0x30_rowHeight + this->buttonRowGap) * i +
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
        this->choiceWindow->update(dt);

    if (state == 0xd) {
        appData = (MtwAppData *) Globals::appManager->GetApplicationData();
        if (appData->storeInitFlag != 0) {
            Engine *eng = (Engine *) _mtw_AppMgr_GetEngine();
            int r = eng->field_0x100;
            if (r == 2 || r == 1) {
                ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
                int id = (r == 2) ? g_mtw_upTextIds[8] : g_mtw_upTextIds[9];
                cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, id));
                this->storeInitDialogShowing = 0;
                this->messageShowing = 1;
                appData = (MtwAppData *) Globals::appManager->GetApplicationData();
                appData->storeInitFlag = 0;
            }
        }
    }

    appData = (MtwAppData *) Globals::appManager->GetApplicationData();
    if (appData->screenshotResultFlag != 0) {
        appData = (MtwAppData *) Globals::appManager->GetApplicationData();
        int r = appData->storeResultCode;
        if (r == 2 || r == 1) {
            ChoiceWindow *cw = (ChoiceWindow *) this->choiceWindow;
            int id = (r == 2) ? g_mtw_upTextIds[10] : g_mtw_upTextIds[11];
            cw->set(*(String*)_mtw_GameText_getText(Globals::gameText, id));
            this->screenshotState = -1;
            this->messageShowing = 1;
            appData = (MtwAppData *) Globals::appManager->GetApplicationData();
            appData->screenshotResultFlag = 0;
        }
    }
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

void MenuTouchWindow::draw() {
    Globals::is_menu_visible = 1;
    int state = this->menuState;
    Globals::topMenuIndex = state;

    Array<TouchButton *> *btnArr = this->buttons;
    if (state == 0xc) btnArr = this->buttonsB4;
    if (state == 0xe) btnArr = this->buttonsB0;
    Array<TouchButton *> *arr = btnArr;

    int *posX = Globals::sub_menu_buttons_x;
    int *posY = Globals::sub_menu_buttons_y;
    for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
        if (i < 10) {
            Vector dpos = arr->data_[i]->getPosition();
            posX[i] = (int) dpos.x;
            dpos = arr->data_[i]->getPosition();
            posY[i] = (int) dpos.y;
        }
    }
    Globals::sub_menu_button_count = *(unsigned int *) arr;

    if (this->backgroundEnabled != 0) {
        unsigned int s = this->menuState;
        if ((s | 4) != 0xd) {
            Globals::layout->drawBG();
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

void _mtw_steerFromTouchId(void *self, int y, int x, void *touchId); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

void _mtw_Layout_OnTouchMove(void *layout, int y); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)



int MenuTouchWindow::OnTouchMove(int x, int y, void *touchId) { // lint: void_ptr (exported method signature; void* is mangling-load-bearing)
    if (this->messageShowing) {
        this->choiceWindow->OnTouchMove(x, y);
        return 0;
    }
    if (!Globals::layout->choiceWindowOpen) {
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
                if (x > Globals::layout->buttonInsetX + this->listTopY &&
                    x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                    y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth)
                    this->upButtonPressed = 1;
                if (x > Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
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
            if (x > Globals::layout->buttonInsetX + this->listTopY &&
                x < Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
                y > Globals::layout->field_0xc_leftMargin + Globals::layout->buttonInsetX &&
                y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->listEntryWidth)
                this->upButtonPressed = 1;
            if (x > Globals::layout->buttonInsetX + this->listTopY + this->listEntryHeight &&
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
    }
    Globals::layout->OnTouchMove(x, y);
    return 0;
}

void _mtw_buildMenu(void *self, int menuType); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

MenuTouchWindow::MenuTouchWindow(int menuType) {
    Layout *layout = Globals::layout;
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

    this->loadPreviewRecords();

    _mtw_buildMenu(this, menuType);
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
        } else if (Globals::iPadLarge) {
            nameY = 12;
            infoY = 8;
        } else {
            nameY = 6;
            infoY = 4;
        }
        // Side borders.
        int borderWidth = Globals::Canvas->GetImage2DWidth(this->scrollbarImageId);
        int borderHeight = Globals::Canvas->GetImage2DHeight(this->scrollbarImageId);
        int count = Globals::h / borderHeight;
        int y = 0;
        for (int i = 0; i <= count; i++) {
            Globals::Canvas->DrawImage2D(this->scrollbarImageId,
                                         Globals::layout->buttonInsetX - borderWidth + this->listX, y, 1);
            Globals::Canvas->DrawImage2D(this->scrollbarImageId, Globals::layout->buttonInsetX + width + this->listX,
                                         y, 0);
            y += borderHeight;
        }
    }
    this->okButton->setPosition(Globals::w - this->listX - Globals::layout->buttonInsetX,
                                (Globals::layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow +
                                this->scrollOffset +
                                (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin) +
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
                                    Globals::layout->buttonInsetX + this->listX + Globals::layout->field_0x2c_rowHeight,
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
                                    Globals::layout->buttonInsetX + this->listX + Globals::layout->field_0x2c_rowHeight,
                                    infoLineY + Globals::layout->field_0x70_rowHeight / 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *(*(*this->recordRows)[i])[3],
                                    Globals::layout->buttonInsetX + this->listX +
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
    hints[8] = 1;
    hints[9] = 1;
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

    ModuleTransitionThunk *thunk = (ModuleTransitionThunk *) Globals::appManager;
    ((OptionsRecord *) Globals::options)->fadeValue = this->fadeValue;
    thunk->transitionFn(reinterpret_cast<void *&>(thunk->transitionFn), 5); // lint: void_ptr (thunk slot reused as opaque context)
}


void MenuTouchWindow::startGOF2() {
    Globals::status->resetGame();
    ((OptionsRecord *) Globals::options)->fadeValue = this->fadeValue;
    Globals::sound->stop(Globals::sound->currentMusicEvent);
    Globals::sound->play(0x8f, nullptr, nullptr, 0.0f);
    Globals::appManager->SetCurrentApplicationModule(2);
}
