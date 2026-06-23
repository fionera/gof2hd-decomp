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

static PaintCanvas *g_PaintCanvas = nullptr;

// Minimal byte-faithful model of the untyped handle returned by
// _mtw_AppMgr_GetApplicationData(). Only the fields this translation unit
// touches are named; the rest is padding to keep offsets exact.
struct MtwAppData {
    uint8_t pad_0x0[5];
    uint8_t screenshotResultFlag;   // 0x5
    uint8_t pad_0x6[2];
    int storeResultCode;            // 0x8
    uint8_t storeInitFlag;          // 0xc
    uint8_t pad_0xd[0x30];
    uint8_t dlcMenuRequestFlag;     // 0x3d
    uint8_t pad_0x3e[2];
    uint8_t purchaseReadyFlag;      // 0x40
    uint8_t purchaseResultFlag;     // 0x41
    uint8_t purchaseErrorFlag;      // 0x42
    uint8_t pad_0x43[5];
    unsigned int purchaseCode;      // 0x48
    uint8_t dlcMenuAckFlag;         // 0x4c
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

void *_mtw_GameText_getText(void *gt, int id);

void _mtw_ChoiceWindow_set(void *cw, void *s1, void *s2, bool b);

void _mtw_render3D_inner(void *obj);

int _mtw_Layout_OnTouchEnd(void *layout, int y, int x);

int _mtw_ChoiceWindow_OnTouchEnd(void *cw, int y);

void _mtw_FModSound_resumeAll(void *snd);

void _mtw_FModSound_stopAll();

void _mtw_AppMgr_SetCurrentApplicationModule(void *app, int id);

void _mtw_AppMgr_Quit(void *app);

void _mtw_Globals_reportLeaderboards();

int _mtw_onTouchEnd_listState(void *self, int y, int x, int state);

int _mtw_onTouchEnd_optionsState(void *self, int y, int x);

int _mtw_onTouchEnd_scrollState(void *self, int y, int x, int which);

int _mtw_onTouchEnd_missionsState(void *self, int y, int x);

int _mtw_onTouchEnd_cinematicState(void *self, int y, int x);

int _mtw_onTouchEnd_genericButtons(void *self, int y, int x, int fieldOff);

void *_mtw_Array_TB_dtor(void *arr);

void *_mtw_Array_TS_dtor(void *arr);

void *_mtw_Array_Str_dtor(void *arr);

void *_mtw_TouchButton_dtor(void *p);

void *_mtw_ChoiceWindow_dtor(void *p);

void *_mtw_ScrollTouchWindow_dtor(void *p);

void *_mtw_Array_Str_dtor(void *p);

void *_mtw_Array_StrArr_dtor(void *p);

void _mtw_Array_StrArr_ctor(void *p);

void _mtw_Array_Str_ctor(void *p);

void _mtw_ArraySetLength_StrArr(int n, void *arr);

void _mtw_ArraySetLength_Str(int n, void *arr);

void _mtw_Globals_longToTimeStringNoSeconds(long long t, void *out);

void _mtw_Layout_formatCredits(void *out);

void _mtw_TouchButton_ctor7(void *btn, void *label, int a, int x, int y, char type);

void _mtw_Status_resetGame();

void _mtw_Status_nextCampaignMission(bool a);

void _mtw_Mission_ctor(void *m);

void _mtw_Status_setMission(void *status);

void *_mtw_Ship_makeShip(int shipDef);

void _mtw_Status_setShip(void *status);

void _mtw_Ship_setRace(void *ship, int race);

void *_mtw_Item_makeItem(int itemDef, int qty);

void *_mtw_makeItem2(int itemDef);

void _mtw_Status_setCredits(void *status);

void *_mtw_Galaxy_getStation(void *galaxy, int idx);

void _mtw_Status_setStation(void *status);

void _mtw_Status_setSystemVisibility(void *status, int sys, bool vis);

void _mtw_Achievements_setMedal(void *ach, int id, int n);

void _mtw_RecordHandler_saveOptions(void *rh);

void _mtw_Status_setKills(void *status, int count);

void _mtw_ChoiceWindow_OnTouchBegin(void *cw, int y);

int _mtw_Layout_OnTouchBegin(void *layout, int y);

void _mtw_TouchButton_OnTouchBegin(void *btn, int y);

void _mtw_TouchButton_OnTouchBeginXY(void *btn, int y, int x);

float _mtw_TouchButton_setPosition(void *btn, int x, int y);

void _mtw_TouchSlider_OnTouchBegin(void *sl, int y);

void _mtw_ScrollTouchWindow_OnTouchBegin(void *w, int y);

void _mtw_MissionsWindow_OnTouchBegin(void *w, int y);

void _mtw_FModSound_play(void *snd, int id, void *pos, float v);

int _mtw_idiv(int a, int b);

void _mtw_RecordHandler_ctor(void *rh);

void *_mtw_RecordHandler_readRecord(void *rh);

void *_mtw_RecordHandler_dtor(void *rh);

void *_mtw_GameRecord_dtor(void *gr);

void _mtw_GameRecord_load(void *gr);

void _mtw_ChoiceWindow_set(void *cw, void *s, bool b);

void *_mtw_AppMgr_GetApplicationModule(void *app, int id);

void _mtw_TouchButton_getPosition(void *out, void *btn);

void _mtw_TouchButton_setVisible(void *btn, bool v);

void *_mtw_Array_GameRecord_dtor(void *p);

void *_mtw_RecordHandler_readAllPreviewRecords(void *rh);

void _mtw_RecordHandler_recordStoreWrite(void *rh, int slot);

void _mtw_RecordHandler_recordStoreWritePreview(void *rh, int slot);

void *_mtw_RecordHandler_recordStoreReadPreview(void *rh);

void _mtw_createRecordButtons(void *self, bool b);

void *_mtw_AppMgr_GetApplicationData();

void *_mtw_AppMgr_GetEngine();

void _mtw_ChoiceWindow_set(void *cw, void *s);

void _mtw_ChoiceWindow_update(void *cw);

void _mtw_TouchButton_setYPosition(void *btn, int y);

int _mtw_TouchButton_isVisible(void *btn);

void _mtw_ScrollTouchWindow_update(void *w);

void _mtw_MissionsWindow_update(void *w);

void _mtw_startSupernovaChallenge_impl(void *self);

void *_mtw_GetApplicationData(void *app);

void _mtw_DlcMenu_call(void *win, void *s1, void *s2);

void _mtw_Layout_drawBG();

void _mtw_ChoiceWindow_OnTouchMove(void *cw, int y);

void _mtw_TouchButton_OnTouchMove(void *btn, int y);

void _mtw_TouchButton_OnTouchMoveXY(void *btn, int y, int x);

void _mtw_TouchSlider_OnTouchMove(void *sl, int y);

float _mtw_TouchSlider_getValue(void *sl);

void _mtw_ScrollTouchWindow_OnTouchMove(void *w, int y);

void _mtw_MissionsWindow_OnTouchMove(void *w, int y);

int _mtw_FModSound_tryToStopMusicForBGMusic();

void _mtw_FModSound_setVolume(void *snd, float v);

void _mtw_Array_TB_ctor(void *a);

void _mtw_loadPreviewRecords(void *self);

void _mtw_TouchButton_draw(void *btn);

void _mtw_Layout_drawBox(void *layout, int mode, int x, int y, int w, int h, void *str);

void _mtw_ImageFactory_drawShip(void *imgF, unsigned int shipId, int x, int y);

void _mtw_Ship_addCargo(void *ship, void *item);

float _mtw_FModSound_stop(void *snd);

static void *const gSupernovaGameText = nullptr;

void MenuTouchWindow::showSupernovaMessage() {
    void *cw = this->choiceWindow;
    void *gt = *(void **) gSupernovaGameText;
    void *s1 = _mtw_GameText_getText(gt, 0x266);
    void *s2 = _mtw_GameText_getText(*(void **) gSupernovaGameText, 0x267);
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
    void *obj = this->missionsWindow;
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

static void *const gEndLayoutGuard = nullptr;
static void *const gEndStatusObj = nullptr;
static void *const gEndAppHolder = nullptr;
static void *const gEndFmod = nullptr;
static void *const gEndQuitApp = nullptr;
static void *const gEndModuleId = nullptr;
static void *const gEndPendingFlag = nullptr;

int MenuTouchWindow::OnTouchEnd(int y, int x, void *touchId) {
    if (this->menuState == 0xb && touchId != 0) {
        if (this->cinematicTouchIdA == touchId)
            this->cinematicTouchIdA = 0;
        if (this->cinematicTouchIdB == touchId)
            this->cinematicTouchIdB = 0;
        this->cinematicTouchState = 0;
    }

    this->pendingActivate = 0;
    this->dragging = 0;

    uint8_t *layoutGuard = (uint8_t *) *(void **) gEndLayoutGuard;
    if (*layoutGuard != 0) {
        if (_mtw_Layout_OnTouchEnd(layoutGuard, y, x) != 0)
            *layoutGuard = 0;
        return 0;
    }

    int state = this->menuState;

    if (state == 0) {
        if (this->messageShowing != 0) {
            void *cw = this->choiceWindow;
            if (this->returnToMenuShowing != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    this->returnToMenuShowing = 0;
                    this->messageShowing = 0;
                } else if (r == 0) {
                    _mtw_AppMgr_Quit(*(void **) gEndQuitApp);
                }
                return 0;
            }
            if (this->genericConfirmB != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r != 1) {
                    if (r != 0) return 0;
                    *(int *) *(void **) gEndPendingFlag = 1;
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
                _mtw_FModSound_resumeAll(*(void **) gEndFmod);
                _mtw_FModSound_stopAll();
                *(int *) *(void **) gEndModuleId = 2;
                _mtw_AppMgr_SetCurrentApplicationModule(*(void **) gEndAppHolder,
                                                        *(int *) *(void **) gEndModuleId);
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

void _mtw_ArrayReleaseClasses_TB(void *arr);

void _mtw_ArrayReleaseClasses_Str(void *arr);

static inline void freeObj(void **slot, void *(*dtor)(void *)) {
    void *o = *slot;
    if (o != 0) ::operator delete(dtor(o));
    *slot = 0;
}

MenuTouchWindow::~MenuTouchWindow() {
    {
        void *a = this->buttons;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->buttons;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttons = 0;
    }
    {
        void *a = this->optionsButtons;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->optionsButtons;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->optionsButtons = 0;
    }
    {
        void *a = this->scrollEntries;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->scrollEntries;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->scrollEntries = 0;
    }
    {
        void *a = this->buttonsB4;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->buttonsB4;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttonsB4 = 0;
    }
    {
        void *a = this->buttonsB8;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->buttonsB8;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttonsB8 = 0;
    }
    {
        void *a = this->buttonsB0;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->buttonsB0;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->buttonsB0 = 0;
    }

    {
        Array<TouchSlider *> *a = (Array<TouchSlider *> *) this->sliders;
        if (a != 0) {
            ArrayReleaseClasses<TouchSlider *>(*a);
            void *b = this->sliders;
            if (b != 0) ::operator delete(_mtw_Array_TS_dtor(b));
        }
        this->sliders = 0;
    }

    {
        void *a = this->scrollSlots;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TB(a);
            void *b = this->scrollSlots;
            if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
        }
        this->scrollSlots = 0;
    }

    {
        void **slots[2] = {(void **) &this->previewStrings0, (void **) &this->previewStrings1};
        for (int k = 0; k < 2; k++) {
            void *a = *slots[k];
            if (a != 0) {
                _mtw_ArrayReleaseClasses_Str(a);
                void *b = *slots[k];
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

void _mtw_ArrayReleaseClasses_Str(void *arr);

void _mtw_ArrayReleaseClasses_StrArr(void *arr);

void *_mtw_String_op_plus(void *out, void *rhs);

void _mtw_TouchButton_ctorFull(void *btn, void *label, int a, int x, int y, int w,
                                          char type, char id);

static void *const gCrbRowCount = nullptr;
static void *const gCrbTimeVal = nullptr;
static void *const gCrbGameText = nullptr;
static const char gCrbDash[] = "";
static void *const gCrbLayout = nullptr;
static void *const gCrbScreenW = nullptr;
static void *const gCrbBackFlag = nullptr;

void MenuTouchWindow::createRecordButtons(bool inSaveMode) {
    Array<void *> *rows = this->recordRows;
    if (rows != 0) {
        for (uint32_t i = 0; i < rows->size_; i++) {
            void *row = rows->data_[i];
            if (row != 0) {
                _mtw_ArrayReleaseClasses_Str(row);
                void *r2 = this->recordRows->data_[i];
                if (r2 != 0) ::operator delete(_mtw_Array_Str_dtor(r2));
                this->recordRows->data_[i] = 0;
                rows = this->recordRows;
            }
        }
        _mtw_ArrayReleaseClasses_StrArr(rows);
        if (this->recordRows != 0) ::operator delete(_mtw_Array_StrArr_dtor(this->recordRows));
        this->recordRows = 0;
    }

    void *outer = ::operator new(0xc);
    _mtw_Array_StrArr_ctor(outer);
    this->recordRows = (Array<void *> *) outer;
    int rowCount = *(int *) *(void **) gCrbRowCount;
    _mtw_ArraySetLength_StrArr(rowCount, outer);

    void *timeHolder = *(void **) gCrbTimeVal;
    void *gtHolder = *(void **) gCrbGameText;

    for (int i = 0; i < rowCount; i++) {
        void *row = ::operator new(0xc);
        _mtw_Array_Str_ctor(row);
        this->recordRows->data_[i] = row;
        _mtw_ArraySetLength_Str(6, this->recordRows->data_[i]);

        String s48;
        { if (s48.data) delete[] s48.data; s48.data = nullptr; s48.length = 0; }
        String s54;

        Array<GameRecord *> *rec = (Array<GameRecord *> *) this->previewRecords;
        bool empty = (rec == 0) || (rec->data_[i] == 0);

        if (empty) {
            { if (s54.data) delete[] s54.data; s54.data = nullptr; s54.length = 0; }
            _mtw_Globals_longToTimeStringNoSeconds((long long) (int) (long) timeHolder, 0);
            void *e;
            int *rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;

            e = ::operator new(0xc);
            ((String *) e)->Set((s54).data);
            ((void **) rowData)[0] = e;
            e = ::operator new(0xc);
            ((String *) e)->Set(((String *) _mtw_GameText_getText(*(void **) gtHolder, 0xae))->data);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[1] = e;

            e = ::operator new(0xc);
            if (i == 0) ((String *) e)->Set(((String *) _mtw_GameText_getText(*(void **) gtHolder, 0x1e6))->data);
            else ((String *) e)->ctor_char(gCrbDash, false);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[2] = e;

            e = ::operator new(0xc);
            ((String *) e)->ctor_char(gCrbDash, false);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[3] = e;

            e = ::operator new(0xc);
            ((String *) e)->ctor_char(gCrbDash, false);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[4] = e;

            e = ::operator new(0xc);
            ((String *) e)->ctor_char(gCrbDash, false);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[5] = e;
        } else {
            { if (s54.data) delete[] s54.data; s54.data = nullptr; s54.length = 0; }
            GameRecord *slot = rec->data_[i];
            _mtw_Globals_longToTimeStringNoSeconds((long long) (int) (long) slot, slot->playTimeObj);
            void *e;
            int *rowData;

            e = ::operator new(0xc);
            ((String *) e)->Set((s54).data);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[0] = e;

            e = ::operator new(0xc);
            ((String *) e)->Set(((String *) &slot->pilotName)->data);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[1] = e;

            e = ::operator new(0xc);
            if (i == 0) ((String *) e)->Set(((String *) _mtw_GameText_getText(*(void **) gtHolder, 0x1e6))->data);
            else ((String *) e)->ctor_char(gCrbDash, false);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[2] = e;

            void *credits = ::operator new(0xc);
            _mtw_Layout_formatCredits(credits);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[3] = credits;

            void *combined = ::operator new(0xc);
            void *label = _mtw_GameText_getText(*(void **) gtHolder, 0x141);
            String s6c;
            s6c.ctor_char(gCrbDash, false);
            String s60;
            _mtw_String_op_plus(&s60, label);
            String s78;
            s78.Set(((String *) &slot->killsText)->data);
            _mtw_String_op_plus(combined, &s60);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[4] = combined;

            e = ::operator new(0xc);
            float rank = slot->rank;
            void *rankTxt;
            if (rank <= 0.0f) rankTxt = _mtw_GameText_getText(*(void **) gtHolder, 0x207);
            else if (rank <= 0.5f) rankTxt = _mtw_GameText_getText(*(void **) gtHolder, 0x207);
            else if (rank <= 1.0f) rankTxt = _mtw_GameText_getText(*(void **) gtHolder, 0x207);
            else rankTxt = _mtw_GameText_getText(*(void **) gtHolder, 0x19);
            ((String *) e)->Set(((String *) rankTxt)->data);
            rowData = (int *) ((Array<void *> *) this->recordRows->data_[i])->data_;
            ((void **) rowData)[5] = e;
        }
    }

    if (this->okButton != 0) {
        ::operator delete(_mtw_TouchButton_dtor(this->okButton));
        this->okButton = 0;
    }
    Layout *layout = (Layout *) *(void **) gCrbLayout;
    char backForm = *(uint8_t *) *(void **) gCrbBackFlag;
    int extra = layout->field_0x108;
    void *okBtn = ::operator new(0xc8);
    int screenW = *(int *) *(void **) gCrbScreenW;
    int x = (screenW - this->listX) - layout->buttonInsetX;
    int y = layout->field_0x20_top + layout->field_0xc + extra;
    void *okLabel = _mtw_GameText_getText(*(void **) gtHolder, inSaveMode ? 0x1fa : 0x1f9);
    if (backForm)
        _mtw_TouchButton_ctor7(okBtn, okLabel, 7, x, y, 0x12);
    else
        _mtw_TouchButton_ctorFull(okBtn, okLabel, 0, x, y, layout->field_0x2a4_metricA, 0x12, 0x04);
    this->okButton = okBtn;

    _mtw_TouchButton_setPosition(okBtn,
                                 (screenW - this->listX) - layout->buttonInsetX,
                                 (layout->field_0x70_rowHeight + this->listRowGap) * this->selectedRow
                                 + layout->field_0xc + extra + layout->field_0x20_top +
                                 this->scrollOffset);

    if (this->backButton != 0) {
        ::operator delete(_mtw_TouchButton_dtor(this->backButton));
        this->backButton = 0;
    }
    void *backBtn = ::operator new(0xc8);
    void *backLabel = _mtw_GameText_getText(*(void **) gtHolder, 0x41);
    _mtw_TouchButton_ctor7(backBtn, backLabel, 7,
                           (screenW - this->listX) - layout->buttonInsetX,
                           layout->field_0xc + extra + layout->field_0x20_top, 0x12);
    this->backButton = backBtn;
}

void *_mtw_Status_getShip(void *status);

void *_mtw_Item_make(int itemDef);

void *_mtw_Item_makeQty(int itemDef, int qty);

void _mtw_Ship_setItem(void *ship, void *item, int slot);

static void *const gValStatus = nullptr;
static void *const gValShipTable = nullptr;
static void *const gValGalaxy = nullptr;
static void *const gValOptA = nullptr;
static void *const gValAch = nullptr;
static void *const gValOptB = nullptr;
static void *const gValOptHolder = nullptr;
static void *const gValFmod = nullptr;
static void *const gValTransition = nullptr;

typedef void (*TransitionFn)(void *app, int mode);

void MenuTouchWindow::startValkyrie() {
    void **statusHolder = (void **) gValStatus;
    _mtw_Status_resetGame();
    for (int i = 0x2d; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool) (unsigned char) (long) *statusHolder);

    void *status = *statusHolder;
    void *mission = ::operator new(0x78);
    _mtw_Mission_ctor(mission);
    _mtw_Status_setMission(status);

    ShipDefTable *row = *(ShipDefTable **) (*(int *) (*(void **) gValShipTable) + 4);
    status = *statusHolder;
    _mtw_Ship_makeShip(row->shipDefId);
    _mtw_Status_setShip(status);

    void *ship = *statusHolder;
    int race = (int) (long) _mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    void *it;
    void *sh;
    it = _mtw_Item_make(row->itemDef_primary);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(row->itemDef_secondary);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_makeQty(row->itemDef_missile, 0xa);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(row->itemDef_equip0x144);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(row->itemDef_equip0xcc);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_makeQty(row->itemDef_equip0x158, 0xa);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 2);
    it = _mtw_Item_make(row->itemDef_equip0x154);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 3);

    _mtw_Status_setCredits(*statusHolder);
    void *station = *statusHolder;
    _mtw_Galaxy_getStation(*(void **) gValGalaxy, 0x5b);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    OptionsRecord *opt = (OptionsRecord *) *(void **) gValOptA;
    void *ach = *(void **) gValAch;
    OptionsRecord *optB = (OptionsRecord *) *(void **) gValOptB;
    ((Status *) *statusHolder)->preSetField0x84 = 0x1a0a;
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
    opt->flag_dword_0x32 = 0;
    opt->flag_0x38 = 1;
    optB->flag_0x34 = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*(void **) gValAch, 0x1e, 1);
    _mtw_RecordHandler_saveOptions(*(void **) gValOptHolder);
    _mtw_Status_setKills(*statusHolder, 0xc5);
    _mtw_FModSound_stop(*(void **) gValFmod);

    void **app = (void **) gValTransition;
    optB->fadeValue = this->fadeValue;
    ModuleTransitionThunk *thunk = (ModuleTransitionThunk *) app;
    thunk->transitionFn(*app, 5);
}

static void *const gBgLayout = nullptr;
static void *const gBgScreenW = nullptr;
static void *const gBgScrollDiv = nullptr;
static void *const gBgListPosX = nullptr;
static void *const gBgFmod = nullptr;
static void *const gBgFlagA = nullptr;
static void *const gBgFlagB = nullptr;
static void *const gBgFlagC = nullptr;
static void *const gBgFlagD = nullptr;
static void *const gBgObjA = nullptr;
static void *const gBgObjB = nullptr;
static void *const gBgScreenH2 = nullptr;
static void *const gBgScrollImg = nullptr;
static void *const gBgScreenW2 = nullptr;

int MenuTouchWindow::OnTouchBegin(int y, int x, void *touchId) {
    if (this->messageShowing != 0) {
        _mtw_ChoiceWindow_OnTouchBegin(this->choiceWindow, y);
        return 0;
    }

    Layout *layout = (Layout *) *(void **) gBgLayout;
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
            if (leftMargin < x && x < *(int *) *(void **) gBgScreenW - layout->field_0x10_rightMargin) {
                int rowH = layout->field_0x70_rowHeight;
                int gap = this->listRowGap;
                int top = layout->field_0x20_top;
                int off = this->scrollOffset;
                int row = _mtw_idiv(x + ((-top - leftMargin) - off), 1);
                if (row < *(int *) *(void **) gBgScrollDiv) {
                    int *posX = (int *) *(void **) gBgListPosX;
                    this->selectedRow = row;
                    float v = _mtw_TouchButton_setPosition(this->okButton,
                                                           (*posX - this->listX) - layout->buttonInsetX,
                                                           row * (gap + rowH) + top + leftMargin + off +
                                                           layout->field_0x108);
                    _mtw_FModSound_play(*(void **) gBgFmod, 0x7c, 0, v);
                }
            }
            if (oldRow == this->selectedRow)
                _mtw_TouchButton_OnTouchBegin(this->okButton, y);
        }
        break;
        case 3: {
            if (*(uint8_t *) *(void **) gBgFlagA == 0) {
                void **arr = (void **) this->optionsButtons;
                for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                    _mtw_TouchButton_OnTouchBegin(((void **) arr[1])[i], y);
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
                _mtw_FModSound_play(*(void **) gBgFmod, 0x7c, 0, 0);
                b28 = layout->buttonInsetX;
                top = b28 + this->listTopY;
            }
            int bottom = top + this->listEntryHeight;
            if (bottom < y && y < (this->listTopY - b28) + this->listBottomY &&
                b28 + layout->field_0xc_leftMargin < x &&
                x < layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth) {
                this->downButtonPressed = 1;
                _mtw_FModSound_play(*(void **) gBgFmod, 0x7c, 0, 0);
            }
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnCC, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD0, y, x);
            _mtw_TouchSlider_OnTouchBegin(*(void **) ((Array<TouchSlider *> *) this->sliders)->data_, y);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD4, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnD8, y, x);
            _mtw_TouchButton_OnTouchBeginXY(this->optBtnDC, y, x);
            void **arr = (void **) this->sliders;
            for (unsigned int i = 1; i < *(unsigned int *) arr; i++)
                _mtw_TouchSlider_OnTouchBegin(((void **) arr[1])[i], y);
            if (*(uint8_t *) *(void **) gBgFlagB != 0 && this->scrollExtraButton != 0)
                _mtw_TouchButton_OnTouchBegin(this->scrollExtraButton, y);
        }
        break;
        case 4: {
            _mtw_ScrollTouchWindow_OnTouchBegin(this->scrollWindowA, y);
            void **arr = (void **) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr[1])[i];
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
            void **arr = (void **) this->sliders;
            unsigned int n = *(unsigned int *) arr;
            for (unsigned int i = 1; i < n; i++) {
                if (i == 5 && ((Layout *) *(void **) gBgLayout)->field_0x284_sliderSlot5Enabled == 0) continue;
                _mtw_TouchSlider_OnTouchBegin(((void **) arr[1])[i], y);
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
                _mtw_FModSound_play(*(void **) gBgFmod, 0x7c, 0, 0);
                layout = (Layout *) *(void **) gBgLayout;
                b28 = layout->buttonInsetX;
                top = b28 + this->listTopY;
            }
            int bottom = top + this->listEntryHeight;
            if (bottom < y && y < (this->listTopY - b28) + this->listBottomY &&
                b28 + layout->field_0xc_leftMargin < x &&
                x < layout->field_0x20_top + layout->field_0xc_leftMargin + this->listEntryWidth) {
                this->downButtonPressed = 1;
                _mtw_FModSound_play(*(void **) gBgFmod, 0x7c, 0, 0);
            }
            _mtw_TouchButton_OnTouchBegin(this->optBtnCC, y);
            _mtw_TouchButton_OnTouchBegin(this->optBtnD0, y);
            _mtw_TouchSlider_OnTouchBegin(*(void **) ((Array<TouchSlider *> *) this->sliders)->data_, y);
        }
        break;
        case 9:
            _mtw_MissionsWindow_OnTouchBegin(this->missionsWindow, y);
            break;
        case 0xb: {
            if (*(uint8_t *) *(void **) gBgFlagC != 0 && *(uint8_t *) *(void **) gBgFlagD == 0) {
                _mtw_TouchButton_OnTouchBegin(this->cinematicBtnA, y);
                _mtw_TouchButton_OnTouchBegin(this->cinematicBtnB, y);
                GameSettings *steerCtl = *(GameSettings **) gBgObjA;
                GameSettings *fireCtl = *(GameSettings **) gBgObjB;
                if (touchId != 0 &&
                    (this->cinematicTouchIdA != 0 || y > 0xd1 || this->cinematicTouchIdB == touchId ||
                     x <= steerCtl->steerAnchorX - 0x14 ||
                     steerCtl->steerAnchorX + 300 <= x)) {
                    if (this->cinematicTouchIdB == touchId || touchId == 0 ||
                        this->cinematicTouchIdB != 0 || y <= *(int *) *(void **) gBgScreenH2 - 0xdc ||
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
            void **arr = (void **) this->buttonsB4;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(((void **) arr[1])[i], y);
        }
        break;
        case 0xe: {
            void **arr = (void **) this->buttonsB0;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(((void **) arr[1])[i], y);
        }
        break;
        case 0x10: {
            void **arr = (void **) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr[1])[i];
                if ((unsigned int) (e[0] - 0x65) < 5 && e[1] == 0)
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
        }
            [[fallthrough]];
        case 0xf: {
            _mtw_ScrollTouchWindow_OnTouchBegin(this->scrollWindowB, y);
            void **arr = (void **) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr[1])[i];
                if ((e[0] | 8) == 0x3c && e[1] == 0)
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
            void **slots = (void **) this->scrollSlots;
            for (unsigned int i = 0; i < *(unsigned int *) slots; i++)
                _mtw_TouchButton_OnTouchBegin(((void **) slots[1])[i], y);

            void *img = *(void **) gBgScrollImg;
            int bound = *(int *) *(void **) gBgScreenW2;
            int b28 = ((Layout *) *(void **) gBgLayout)->buttonInsetX;
            int iw = g_PaintCanvas->GetImage2DWidth((unsigned int) (long) img);
            unsigned char hit;
            if ((bound - b28) - iw < y) {
                int lc = ((Layout *) *(void **) gBgLayout)->field_0xc_leftMargin;
                int tp = ((Layout *) *(void **) gBgLayout)->field_0x20_top;
                int ih = g_PaintCanvas->GetImage2DHeight((unsigned int) (long) img);
                hit = (x < ih + tp + lc) ? 1 : 0;
            } else hit = 0;
            this->scrollbarHit = hit;

            b28 = ((Layout *) *(void **) gBgLayout)->buttonInsetX;
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
            void **arr = (void **) this->buttonsB8;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(((void **) arr[1])[i], y);
        }
        break;
        default: {
            void **arr = (void **) this->buttons;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchBegin(((void **) arr[1])[i], y);
            void **arr2 = (void **) this->scrollEntries;
            unsigned int n = *(unsigned int *) arr2;
            for (unsigned int i = 0; i < n; i++) {
                int *e = (int *) ((void **) arr2[1])[i];
                if ((unsigned int) (e[0] - 0x17) < 2) _mtw_TouchButton_OnTouchBegin(e, y);
            }
            for (unsigned int i = 0; i < n; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr2[1])[i];
                unsigned int t = e[0], id = e[1];
                bool go;
                if (t == 5 && id == 0) go = true;
                else {
                    unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                    go = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
                }
                if (go) {
                    _mtw_TouchButton_OnTouchBegin(e, y);
                    arr2 = (void **) this->scrollEntries;
                    n = *(unsigned int *) arr2;
                }
            }
        }
        break;
    }

    int r = _mtw_Layout_OnTouchBegin(*(void **) gBgLayout, y);
    if (r != 0 && this->menuState == 0xd)
        this->pendingActivate = 1;
    return 0;
}

static void *const gLoadStatusFlags = nullptr;
static void *const gLoadGameText = nullptr;
static void *const gLoadAppHolder = nullptr;
static void *const gLoadResetCell = nullptr;

int MenuTouchWindow::loadGame(int slot) {
    void *rh = ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    void *rec = _mtw_RecordHandler_readRecord(rh);

    if (rec == 0) {
        _mtw_Status_resetGame();
        void *cw = this->choiceWindow;
        void *s = _mtw_GameText_getText(*(void **) gLoadGameText, 0x64);
        _mtw_ChoiceWindow_set(cw, s, false);
        this->loadFailedDialogShowing = 1;
        this->messageShowing = 1;
        ::operator delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    Status *flags = (Status *) *(void **) gLoadStatusFlags;
    GameRecord *record = (GameRecord *) rec;
    bool versionOk = (record->versionMismatchFlag == 0) || (flags->flag_0x37 != 0);
    if (versionOk) {
        bool dlcOk = (record->dlcRequiredFlag == 0) || (flags->flag_0x35 != 0);
        if (dlcOk) {
            _mtw_Status_resetGame();
            _mtw_GameRecord_load(rec);
            ::operator delete(_mtw_RecordHandler_dtor(rh));
            ::operator delete(_mtw_GameRecord_dtor(rec));
            void *app = *(void **) gLoadAppHolder;
            void *ms = _mtw_AppMgr_GetApplicationModule(*(void **) app, 5);
            ((ModStation *) ms)->setGameLoaded();
            *(int *) *(void **) gLoadResetCell = 0;
            _mtw_AppMgr_SetCurrentApplicationModule(*(void **) app, 5);
            return 1;
        }

        void *cw = this->choiceWindow;
        void *s = _mtw_GameText_getText(*(void **) gLoadGameText, 0x65);
        _mtw_ChoiceWindow_set(cw, s, false);
        this->messageShowing = 1;
        ::operator delete(_mtw_GameRecord_dtor(rec));
        ::operator delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    void *cw = this->choiceWindow;
    void *s = _mtw_GameText_getText(*(void **) gLoadGameText, 0x66);
    _mtw_ChoiceWindow_set(cw, s, false);
    this->messageShowing = 1;
    ::operator delete(_mtw_GameRecord_dtor(rec));
    ::operator delete(_mtw_RecordHandler_dtor(rh));
    return 0;
}

void _mtw_TouchButton_ctor(void *btn, void *label, int a, int x, int y, int w,
                                      char type, char id);

static void *const gAddBtnScreenW = nullptr;
static void *const gAddBtnLayout = nullptr;
static void *const gAddBtnScreenH = nullptr;
static void *const gAddBtnPosX = nullptr;
static void *const gAddBtnPosY = nullptr;
static void *const gAddBtnCount = nullptr;

void MenuTouchWindow::addButton(int id, AbyssEngine::String label, int row, Array<TouchButton *> *arr, int yOff) {
    void *btn = ::operator new(0xc8);

    int btnW = this->buttonWidth;
    int screenW = *(int *) *(void **) gAddBtnScreenW;
    int screenH = *(int *) *(void **) gAddBtnScreenH;
    int rowH = ((Layout *) *(void **) gAddBtnLayout)->field_0x30_rowHeight;

    int x = screenW / 2 - btnW / 2;
    int y = (rowH + this->buttonRowGap) * row + (yOff - this->buttonYBias) + screenH / 2;

    _mtw_TouchButton_ctor(btn, &label, 0, x, y, btnW, 0x11, 0x04);
    ((TouchButton *) btn)->field_0x0 = id;
    ((TouchButton *) btn)->field_0x4 = id >> 31;
    ArrayAdd<TouchButton *>((TouchButton *) btn, *arr);

    int *posX = (int *) *(void **) gAddBtnPosX;
    int *posY = (int *) *(void **) gAddBtnPosY;
    for (uint32_t i = 0; i < arr->size_; i++) {
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
    *(uint32_t *) *(void **) gAddBtnCount = arr->size_;
}

void MenuTouchWindow::setCutsceneMode(bool mode) {
    this->cutsceneMode = (uint8_t) mode;
    void **arr = (void **) this->buttons;
    for (uint32_t i = 0; i < *(uint32_t *) arr; i++) {
        TouchButton *btn = (TouchButton *) ((void **) arr[1])[i];
        if (btn->field_0x0 == 0x13 && btn->field_0x4 == 0) {
            _mtw_TouchButton_setVisible(btn, (bool) ((uint8_t) mode ^ 1));
        }
        arr = (void **) this->buttons;
    }
}

static void *const gPrevScreenW = nullptr;
static void *const gPrevLayout = nullptr;
static void *const gPrevRowCnt = nullptr;

typedef void (*RefreshFn)();

static void *const gPrevRefreshThunk = nullptr;

void MenuTouchWindow::loadPreviewRecords() {
    this->scrollOffset = 0;

    this->dragLastX = 0;
    this->dragVelocity = 0;
    this->inertiaDecay = 0;
    this->inertiaVel = 0.0f;
    this->dragStartX = 0;
    this->dragging = 0;

    int *metrics = (int *) *(void **) gPrevScreenW;
    Layout *layout = (Layout *) *(void **) gPrevLayout;
    int *rowObj = (int *) *(void **) gPrevRowCnt;

    this->contentHeight = (((metrics[0] - layout->field_0xc) - layout->windowTopInset)
                           - layout->field_0x20) - layout->field_0x24;

    this->pageHeight = rowObj[0] * (layout->field_0x70_rowHeight + this->listRowGap);

    void *rec = this->previewRecords;
    if (rec != 0) {
        ::operator delete(_mtw_Array_GameRecord_dtor(rec));
        this->previewRecords = 0;
    }
    void *rh = ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    this->previewRecords = _mtw_RecordHandler_readAllPreviewRecords(rh);
    _mtw_RecordHandler_dtor(rh);

    ((RefreshThunk *) gPrevRefreshThunk)->refreshFn();
}

static void *const gSaveGameText = nullptr;

void MenuTouchWindow::saveGame(int slot) {
    void *rh = ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    _mtw_RecordHandler_recordStoreWrite(rh, slot);
    _mtw_RecordHandler_recordStoreWritePreview(rh, slot);

    int *arr = (int *) ((Array<GameRecord *> *) this->previewRecords)->data_;
    void *rec = *(void **) (arr + slot);
    int *cell;
    if (rec == 0) {
        cell = arr + slot;
    } else {
        ::operator delete(_mtw_GameRecord_dtor(rec));
        cell = (int *) &((Array<GameRecord *> *) this->previewRecords)->data_[slot];
    }
    *cell = 0;

    void *preview = _mtw_RecordHandler_recordStoreReadPreview(rh);
    *(void **) &((Array<GameRecord *> *) this->previewRecords)->data_[slot] = preview;
    ::operator delete(_mtw_RecordHandler_dtor(rh));

    _mtw_createRecordButtons(this, true);
    void *cw = this->choiceWindow;
    void *s = _mtw_GameText_getText(*(void **) gSaveGameText, 0x32);
    _mtw_ChoiceWindow_set(cw, s, false);
    this->saveDialogShowing = 0;
    this->messageShowing = 1;
}

static void *const gUpStatusGuard = nullptr;
static void *const gUpLayout = nullptr;
static void *const gUpScreenW = nullptr;
static void *const gUpCanvas = nullptr;
static void *const gUpOptObj = nullptr;
static void *const gUpRecHandler = nullptr;
static void *const gUpGameText = nullptr;
static void *const gUpStatusObj = nullptr;
static void *const gUpDlcFlags = nullptr;
static void *const gUpScreenH = nullptr;
static void *const gUpListLayout = nullptr;

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
            Layout *layout = (Layout *) *(void **) gUpLayout;
            void *canvas = *(void **) gUpCanvas;
            this->contentHeight = (((*(int *) *(void **) gUpScreenW - layout->field_0x10_rightMargin)
                                    - layout->field_0xc) - layout->field_0x20) - layout->field_0x24;
            int ih = ((PaintCanvas *) canvas)->GetImage2DHeight(this->scrollbarImageId);
            this->contentHeightCache = ih;
            OptionsRecord *optObj = (OptionsRecord *) *(void **) gUpOptObj;
            int rowH = layout->field_0x2c_rowHeight;
            this->menuState = 0xf;
            this->messageShowing = 0;
            this->dlcMessageShowing = 0;
            this->pageHeight = (ih + rowH) * 5;
            appData->purchaseReadyFlag = 0;
            optObj->flag_0x3b = 1;
            _mtw_RecordHandler_saveOptions(*(void **) gUpRecHandler);
            busy = this->dlcMessageShowing;
        }
        if (busy != 0) {
            if (appData->purchaseErrorFlag != 0) {
                void *cw = this->choiceWindow;
                void *s = _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[0]);
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
        void *cw = this->choiceWindow;
        switch (code) {
            case 0: {
                Status *status = (Status *) *(void **) gUpStatusObj;
                status->flag_0x35 = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[1]));
            }
            break;
            case 1: {
                ((Status *) *(void **) gUpStatusObj)->mode_0x114 = 3;
                ((OptionsRecord *) *(void **) gUpOptObj)->flag_0x36 = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[2]));
            }
            break;
            case 2: {
                Status *status = (Status *) *(void **) gUpStatusObj;
                status->flag_0x37 = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[3]));
            }
            break;
            case 3: {
                ((OptionsRecord *) *(void **) gUpOptObj)->flag_0x38 = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[4]));
            }
            break;
            case 4: {
                OptionsRecord *flags = (OptionsRecord *) *(void **) gUpOptObj;
                Status *status = (Status *) *(void **) gUpStatusObj;
                flags->flag_0x35 = 1;
                flags->flag_0x39 = 1;
                flags->flag_0x37 = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[5]));
            }
            break;
        }
        _mtw_RecordHandler_saveOptions(*(void **) gUpRecHandler);
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        appData->purchaseResultFlag = 0;
        handled = true;
    } else if (this->purchaseRestorePending != 0 && appData->purchaseResultFlag != 0) {
        _mtw_RecordHandler_saveOptions(*(void **) gUpRecHandler);
        if (this->purchaseRestorePending != 0) {
            void *cw = this->choiceWindow;
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[6]));
            this->purchaseRestorePending = 0;
        }
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        appData->purchaseResultFlag = 0;
        handled = true;
    }
    (void) handled;

    if (*(uint8_t *) *(void **) gUpStatusGuard != 0) {
        OptionsRecord *flags = (OptionsRecord *) *(void **) gUpDlcFlags;
        if (this->messageShowing == 0 && flags->flag_0x35 == 0) {
            void *cw = this->choiceWindow;
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, g_mtw_upTextIds[7]));
            this->supernovaPurchaseDialogShowing = 1;
            this->messageShowing = 1;
            flags->flag_0x35 = 1;
            _mtw_Status_setSystemVisibility(*(void **) gUpStatusObj, 0x19, true);
            _mtw_RecordHandler_saveOptions(*(void **) gUpRecHandler);
        }
    }

    int state = this->menuState;
    if (state == 0) {
        if (this->backgroundEnabled == 1) {
            int rowGap = this->buttonRowGap;
            Layout *layout = (Layout *) *(void **) gUpListLayout;
            void **arr = (void **) this->buttons;
            unsigned int n = *(unsigned int *) arr;
            int total = n * layout->field_0x30_rowHeight;
            int screenH = *(int *) *(void **) gUpScreenH;
            for (unsigned int i = 0; i < n; i++) {
                _mtw_TouchButton_setYPosition(((void **) arr[1])[i],
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
                arr = (void **) this->buttons;
                n = *(unsigned int *) arr;
            }
        }
    } else if (state == 3) {
        int rowGap = this->buttonRowGap;
        Layout *layout = (Layout *) *(void **) gUpListLayout;
        void **arr = (void **) this->optionsButtons;
        unsigned int n = *(unsigned int *) arr;
        int firstCount = *(int *) *(void **) this->buttons;
        int total = n * layout->field_0x30_rowHeight;
        int screenH = *(int *) *(void **) gUpScreenH;
        for (unsigned int i = 0; i < n; i++) {
            _mtw_TouchButton_setYPosition(((void **) arr[1])[i],
                                          (layout->field_0x30_rowHeight + this->buttonRowGap) * i +
                                          (screenH / 2 - (int) (
                                               (unsigned int) ((firstCount - 1) * rowGap + total) >> 1)));
            arr = (void **) this->optionsButtons;
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
                void *cw = this->choiceWindow;
                int id = (r == 2) ? g_mtw_upTextIds[8] : g_mtw_upTextIds[9];
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, id));
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
            void *cw = this->choiceWindow;
            int id = (r == 2) ? g_mtw_upTextIds[10] : g_mtw_upTextIds[11];
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **) gUpGameText, id));
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

static void *const gAppHolder = nullptr;
static void *const gDlcGameText = nullptr;

void MenuTouchWindow::callDlcMenu() {
    void *holder = gAppHolder;

    MtwAppData *ad = (MtwAppData *) _mtw_GetApplicationData(*(void **) holder);
    ad->dlcMenuAckFlag = 0;
    ad = (MtwAppData *) _mtw_GetApplicationData(*(void **) holder);
    void *gt = gDlcGameText;
    ad->dlcMenuRequestFlag = 1;
    void *win = this->choiceWindow;
    this->messageShowing = 1;
    this->dlcMessageShowing = 1;
    void *s1 = _mtw_GameText_getText(*(void **) gt, 0x47);
    void *s2 = _mtw_GameText_getText(*(void **) gt, 0x1a9);
    return _mtw_DlcMenu_call(win, s1, s2);
}

void _mtw_draw_mainMenu(void *self);

void _mtw_draw_loadSaveList(void *self);

void _mtw_draw_optionsTabs(void *self);

void _mtw_draw_scrollA(void *self);

void _mtw_draw_audioOptions(void *self);

void _mtw_draw_keyConfig(void *self);

void _mtw_draw_missions(void *self);

void _mtw_draw_cinematic(void *self);

void _mtw_draw_buttonsB4(void *self);

void _mtw_draw_store(void *self);

void _mtw_draw_buttonsB0(void *self);

void _mtw_draw_scrollB(void *self);

void _mtw_draw_buttonsB8(void *self);

static void *const gDrawDrawingFlag = nullptr;
static void *const gDrawStatePub = nullptr;
static void *const gDrawPosX = nullptr;
static void *const gDrawPosY = nullptr;
static void *const gDrawCountObj = nullptr;
static void *const gDrawColorSrc = nullptr;

void MenuTouchWindow::draw() {
    *(int *) *(void **) gDrawDrawingFlag = 1;
    int state = this->menuState;
    *(int *) *(void **) gDrawStatePub = state;

    Array<void *> *btnArr = this->buttons;
    if (state == 0xc) btnArr = this->buttonsB4;
    if (state == 0xe) btnArr = this->buttonsB0;
    void **arr = (void **) btnArr;

    int *posX = (int *) *(void **) gDrawPosX;
    int *posY = (int *) *(void **) gDrawPosY;
    for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
        if (i < 10) {
            char buf[12];
            _mtw_TouchButton_getPosition(buf, ((void **) arr[1])[i]);
            posX[i] = (int) *(float *) (buf + 0);
            _mtw_TouchButton_getPosition(buf, ((void **) arr[1])[i]);
            posY[i] = (int) *(float *) (buf + 4);
        }
    }
    *(unsigned int *) *(void **) gDrawCountObj = *(unsigned int *) arr;

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

void _mtw_Layout_OnTouchMove(void *layout, int y);

static void *const gMvLayout = nullptr;
static void *const gMvScreenW = nullptr;
static void *const gMvFlagA = nullptr;
static void *const gMvFlagB = nullptr;
static void *const gMvFmod = nullptr;

static void *const gMvScrollBound = nullptr;

int MenuTouchWindow::OnTouchMove(int y, int x, void *touchId) {
    if (this->messageShowing != 0) {
        _mtw_ChoiceWindow_OnTouchMove(this->choiceWindow, y);
        return 0;
    }
    Layout *layout = (Layout *) *(void **) gMvLayout;
    if (layout->choiceWindowOpen != 0) {
        _mtw_Layout_OnTouchMove(layout, y);
        return 0;
    }

    int state = this->menuState;
    switch (state - 1) {
        case 0:
        case 1:
            if (layout->field_0xc < x &&
                x < *(int *) *(void **) gMvScreenW - layout->field_0x10_rightMargin) {
                int dx = x - this->dragLastX;
                this->dragLastX = x;
                this->dragVelocity = dx;
                this->inertiaDecay = 0x3f800000;
                this->scrollOffset = this->scrollOffset + dx;
            }
            _mtw_TouchButton_OnTouchMove(this->okButton, y);
            break;
        case 2: {
            if (*(uint8_t *) *(void **) gMvFlagA == 0) {
                void **arr = (void **) this->optionsButtons;
                for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                    _mtw_TouchButton_OnTouchMove(((void **) arr[1])[i], y);
            } else {
                _mtw_TouchButton_OnTouchMove(this->okButton, y);
                this->upButtonPressed = 0;
                int base = layout->buttonInsetX;
                int top = base + this->listTopY;
                int bottom = this->listEntryHeight + top;
                if (top < y && y < bottom &&
                    layout->field_0xc + base < x &&
                    x < layout->field_0xc + layout->field_0x20_top + this->listEntryWidth)
                    this->upButtonPressed = 1;
                if (bottom < y && y < (this->listTopY - base) + this->listBottomY &&
                    base + layout->field_0xc < x &&
                    x < layout->field_0x20_top + layout->field_0xc + this->listEntryWidth)
                    this->downButtonPressed = 1;
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnCC, y, x);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnD0, y, x);
                _mtw_TouchSlider_OnTouchMove(*(void **) ((Array<TouchSlider *> *) this->sliders)->data_, y);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnD4, y, x);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnD8, y, x);
                _mtw_TouchButton_OnTouchMoveXY(this->optBtnDC, y, x);
                {
                    void *fmod = *(void **) gMvFmod;
                    int *sl = (int *) ((Array<TouchSlider *> *) this->sliders)->data_;
                    if (_mtw_FModSound_tryToStopMusicForBGMusic() == 0)
                        _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **) sl)[1]));
                    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **) sl)[2]));
                    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **) sl)[3]));
                }
                void **arr = (void **) this->sliders;
                for (unsigned int i = 1; i < *(unsigned int *) arr; i++)
                    _mtw_TouchSlider_OnTouchMove(((void **) arr[1])[i], y);
                if (*(uint8_t *) *(void **) gMvFlagB != 0 && this->okButton != 0)
                    _mtw_TouchButton_OnTouchMove(this->okButton, y);
            }
        }
        break;
        case 3: {
            _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowA, y);
            void **arr = (void **) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr[1])[i];
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
                void *fmod = *(void **) gMvFmod;
                int *sl = (int *) ((Array<TouchSlider *> *) this->sliders)->data_;
                if (_mtw_FModSound_tryToStopMusicForBGMusic() == 0)
                    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **) sl)[1]));
                _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **) sl)[2]));
                _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **) sl)[3]));
            }
            {
                void **arr = (void **) this->sliders;
                for (unsigned int i = 1; i < *(unsigned int *) arr; i++) {
                    if (i == 5 && ((Layout *) *(void **) gMvLayout)->field_0x284_sliderSlot5Enabled == 0) continue;
                    _mtw_TouchSlider_OnTouchMove(((void **) arr[1])[i], y);
                }
            }
            break;
        case 7: {
            this->upButtonPressed = 0;
            int base = layout->buttonInsetX;
            int top = base + this->listTopY;
            int bottom = this->listEntryHeight + top;
            if (top < y && y < bottom &&
                layout->field_0xc + base < x &&
                x < layout->field_0xc + layout->field_0x20_top + this->listEntryWidth)
                this->upButtonPressed = 1;
            if (bottom < y && y < (this->listTopY - base) + this->listBottomY &&
                base + layout->field_0xc < x &&
                x < layout->field_0x20_top + layout->field_0xc + this->listEntryWidth)
                this->downButtonPressed = 1;
            _mtw_TouchButton_OnTouchMove(this->optBtnCC, y);
            _mtw_TouchButton_OnTouchMove(this->optBtnD0, y);
            _mtw_TouchSlider_OnTouchMove(*(void **) ((Array<TouchSlider *> *) this->sliders)->data_, y);
        }
        break;
        case 8:
            _mtw_MissionsWindow_OnTouchMove(this->missionsWindow, y);
            break;
        case 0xa:
            _mtw_steerFromTouchId(this, y, x, touchId);
            break;
        case 0xb: {
            void **arr = (void **) this->buttonsB4;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(((void **) arr[1])[i], y);
        }
        break;
        case 0xd: {
            void **arr = (void **) this->buttonsB0;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(((void **) arr[1])[i], y);
        }
        break;
        case 0xe: {
            Layout *slayout = (Layout *) *(void **) gMvLayout;

            _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowB, y);

            void **arr = (void **) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr[1])[i];
                if ((e[0] | 8) == 0x3c && e[1] == 0)
                    _mtw_TouchButton_OnTouchMove(e, y);
            }

            void **slots = (void **) this->scrollSlots;
            for (unsigned int i = 0; i < *(unsigned int *) slots; i++)
                _mtw_TouchButton_OnTouchMove(((void **) slots[1])[i], y);

            if (slayout->field_0xc < x &&
                x < *(int *) *(void **) gMvScrollBound - slayout->field_0x10_rightMargin) {
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
            void **arr = (void **) this->scrollEntries;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr[1])[i];
                if ((unsigned int) (e[0] - 0x65) < 5)
                    _mtw_TouchButton_OnTouchMove(e, y);
            }
            {
                Layout *slayout = (Layout *) *(void **) gMvLayout;

                _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowB, y);

                void **arr2 = (void **) this->scrollEntries;
                for (unsigned int i = 0; i < *(unsigned int *) arr2; i++) {
                    unsigned int *e = (unsigned int *) ((void **) arr2[1])[i];
                    if ((e[0] | 8) == 0x3c && e[1] == 0)
                        _mtw_TouchButton_OnTouchMove(e, y);
                }

                void **slots = (void **) this->scrollSlots;
                for (unsigned int i = 0; i < *(unsigned int *) slots; i++)
                    _mtw_TouchButton_OnTouchMove(((void **) slots[1])[i], y);

                if (slayout->field_0xc < x &&
                    x < *(int *) *(void **) gMvScrollBound - slayout->field_0x10_rightMargin) {
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
            void **arr = (void **) this->buttonsB8;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(((void **) arr[1])[i], y);
        }
        break;
        default: {
            void **arr = (void **) this->buttons;
            for (unsigned int i = 0; i < *(unsigned int *) arr; i++)
                _mtw_TouchButton_OnTouchMove(((void **) arr[1])[i], y);
            void **arr2 = (void **) this->scrollEntries;
            unsigned int n = *(unsigned int *) arr2;
            for (unsigned int i = 0; i < n; i++) {
                int *e = (int *) ((void **) arr2[1])[i];
                if ((unsigned int) (e[0] - 0x17) < 2)
                    _mtw_TouchButton_OnTouchBegin(e, y);
            }
            for (unsigned int i = 0; i < n; i++) {
                unsigned int *e = (unsigned int *) ((void **) arr2[1])[i];
                unsigned int t = e[0], id = e[1];
                bool hit;
                if (t == 5 && id == 0) hit = true;
                else {
                    unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                    hit = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
                }
                if (hit) {
                    _mtw_TouchButton_OnTouchMove(e, y);
                    arr2 = (void **) this->scrollEntries;
                    n = *(unsigned int *) arr2;
                }
            }
        }
        break;
    }

    _mtw_Layout_OnTouchMove((void *) *(void **) gMvLayout, y);
    return 0;
}

void _mtw_buildMenu(void *self, int menuType);

static void *const gCtorLayout = nullptr;

MenuTouchWindow::MenuTouchWindow(int menuType) {
    Layout *layout = (Layout *) *(void **) gCtorLayout;
    this->buttonWidth = layout->field_0x294_buttonWidth;
    this->buttonYBias = layout->field_0x298_buttonYBias;
    this->buttonRowGap = layout->field_0x29c_buttonRowGap;
    this->listRowGap = layout->field_0x2a0_listRowGap;
    this->metricA = layout->field_0x2a4_metricA;
    this->metricB = layout->field_0x2a8_metricB;
    this->metricC = layout->field_0x2ac_metricC;

    this->backgroundEnabled = menuType;

    void *arr1 = ::operator new(0xc);
    _mtw_Array_TB_ctor(arr1);
    this->buttons = (Array<void *> *) arr1;
    void *arr2 = ::operator new(0xc);
    _mtw_Array_TB_ctor(arr2);
    this->scrollEntries = (Array<void *> *) arr2;

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
    void **arr = (void **) this->buttons;
    if (arr != 0) {
        for (uint32_t i = 0; i < *(uint32_t *) arr; i++) {
            TouchButton *btn = (TouchButton *) ((void **) arr[1])[i];
            if (btn != 0 && btn->field_0x0 == 0x12 && btn->field_0x4 == 0) {
                _mtw_TouchButton_setVisible(btn, visible);
                arr = (void **) this->buttons;
            }
        }
    }
}

static void *const gDlsLayout = nullptr;
static void *const gDlsCanvas = nullptr;
static void *const gDlsScreenW = nullptr;
static void *const gDlsScrollOn = nullptr;
static void *const gDlsScrollNo = nullptr;
static void *const gDlsFlagA = nullptr;
static void *const gDlsFlagB = nullptr;
static void *const gDlsTileCnt = nullptr;
static void *const gDlsRowCount = nullptr;
static void *const gDlsRowMax = nullptr;
static const char gDlsBoxStr[] = "";
static void *const gDlsFont = nullptr;
static void *const gDlsImgFact = nullptr;

void MenuTouchWindow::drawLoadSaveMenu(bool param1) {
    (void) param1;
    Layout *layout = (Layout *) *(void **) gDlsLayout;
    void *canvas = *(void **) gDlsCanvas;

    int rowBaseY = layout->field_0x10c_rowBaseY;
    ((PaintCanvas *) canvas)->SetColor(0xffffffff);

    int scrollOff = this->listX;
    int margin = layout->buttonInsetX;
    int strip58 = layout->field_0x110_strip58;
    int strip5c = layout->field_0x114_strip5c;
    int screenBound = *(int *) *(void **) gDlsScreenW;
    int inner = screenBound + margin * -2 + scrollOff * -2;

    if (*(uint8_t *) *(void **) gDlsScrollOn != 0 && *(uint8_t *) *(void **) gDlsScrollNo == 0) {
        strip5c = 8;
        if (*(uint8_t *) *(void **) gDlsFlagA == 0) {
            strip58 = 0xc;
            if (*(uint8_t *) *(void **) gDlsTileCnt == 0) {
                strip5c = 4;
                strip58 = 6;
            }
        } else {
            strip58 = 8;
            strip5c = 5;
        }
        int iw = ((PaintCanvas *) canvas)->GetImage2DWidth(this->scrollbarImageId);
        int ih = ((PaintCanvas *) canvas)->GetImage2DHeight(this->scrollbarImageId);
        int count = _mtw_idiv((int) (long) *(void **) gDlsTileCnt, 1);
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
                                 + layout->field_0x20_top + layout->field_0xc + layout->field_0x108);

    int rowCount = *(int *) *(void **) gDlsRowCount;
    int rowMax = *(int *) *(void **) gDlsRowMax;

    for (int i = 0; i < rowCount; i++) {
        int rowY = (layout->field_0x70_rowHeight + this->listRowGap) * i + this->scrollOffset
                   + layout->field_0x20_top + layout->field_0xc;
        if (rowY < 0 || rowY > rowMax) continue;

        ((PaintCanvas *) canvas)->SetColor(0xffffffff);
        int boxX = layout->buttonInsetX + this->listX;
        String box;
        box.ctor_char(gDlsBoxStr, false);
        int mode = (i == this->selectedRow) ? 4 : 3;
        _mtw_Layout_drawBox(layout, mode, boxX, rowY, inner - 3, layout->field_0x70_rowHeight, &box);

        void *font = *(void **) *(void **) gDlsFont;
        int yName = strip58 + rowY;
        String **cols = (String **) (long) ((Array<void *> *) this->recordRows->data_[i])->data_;

        ((PaintCanvas *) canvas)->DrawString((unsigned int) (long) font, *cols[0],
                                             layout->buttonInsetX + this->listX + layout->field_0x2c_rowHeight,
                                             (char) yName, false);

        GameRecord *slot = ((Array<GameRecord *> *) this->previewRecords)->data_[i];
        if (slot != 0) {
            unsigned int shipId = slot->shipId;
            if (shipId < 0x40) {
                _mtw_ImageFactory_drawShip(*(void **) gDlsImgFact, shipId,
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

void *_mtw_Status_getShip(void *status);

void *_mtw_Item_make(int itemDef);

void *_mtw_Item_makeQty(int itemDef, int qty);

void _mtw_Ship_setItem(void *ship, void *item, int slot);

static void *const gSnStatus = nullptr;
static void *const gSnShipTable = nullptr;
static void *const gSnGalaxy = nullptr;
static void *const gSnOptA = nullptr;
static void *const gSnAch = nullptr;
static void *const gSnOptB = nullptr;
static void *const gSnOptHolder = nullptr;
static void *const gSnFmod = nullptr;
static void *const gSnTransition = nullptr;

typedef void (*TransitionFn)(void *app, int mode);

void MenuTouchWindow::startSupernova() {
    void **statusHolder = (void **) gSnStatus;
    _mtw_Status_resetGame();
    for (int i = 0x54; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool) (unsigned char) (long) *statusHolder);

    _mtw_Status_setMission(*statusHolder);
    void *status = *statusHolder;
    ShipDefTable *row = *(ShipDefTable **) (*(int *) (*(void **) gSnShipTable) + 4);
    _mtw_Ship_makeShip(row->shipDefId_0x78);
    _mtw_Status_setShip(status);

    void *ship = *statusHolder;
    int race = (int) (long) _mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    void *it;
    void *sh;
    it = _mtw_Item_make(row->itemDef_0x2c0);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(row->itemDef_0x50);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_makeQty(row->itemDef_missile, 0x14);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_makeQty(row->itemDef_0xb0, 0x14);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(row->itemDef_equip0x144);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(row->itemDef_equip0xcc);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(row->itemDef_0x110);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 2);
    it = _mtw_Item_make(row->itemDef_equip0x158);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 3);
    it = _mtw_Item_make(row->itemDef_equip0x154);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 4);
    it = _mtw_Item_make(row->itemDef_0xe0);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_setItem(sh, it, 5);
    it = _mtw_Item_makeQty(row->itemDef_cargo0x1e8, 8);
    sh = _mtw_Status_getShip(*statusHolder);
    _mtw_Ship_addCargo(sh, it);

    _mtw_Status_setCredits(*statusHolder);
    void *station = *statusHolder;
    _mtw_Galaxy_getStation(*(void **) gSnGalaxy, 0x46);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    OptionsRecord *opt = (OptionsRecord *) *(void **) gSnOptA;
    OptionsRecord *optB = (OptionsRecord *) *(void **) gSnOptB;
    ((Status *) *statusHolder)->preSetField0x84 = 0x1a0a;
    opt->flag_dword_0x20 = 0x1010101;
    opt->flag_0x17 = 1;
    opt->flag_0x36 = 0;
    opt->flag_dword_0x32 = 0;
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
    opt->flag_word_0x38 = 0x101;
    void *ach = *(void **) gSnAch;
    optB->flag_0x34 = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*(void **) gSnAch, 0x1e, 1);
    _mtw_Status_setKills(*statusHolder, 0x182);
    _mtw_RecordHandler_saveOptions(*(void **) gSnOptHolder);
    _mtw_FModSound_stop(*(void **) gSnFmod);

    void **app = (void **) gSnTransition;
    optB->fadeValue = this->fadeValue;
    ModuleTransitionThunk *thunk = (ModuleTransitionThunk *) app;
    thunk->transitionFn(*app, 5);
}

static void *const gGof2Fmod = nullptr;
static void *const gGof2StatusObj = nullptr;
static void *const gGof2Transition = nullptr;

typedef void (*TransitionFn3)(void *app, int a, int b);

void MenuTouchWindow::startGOF2() {
    _mtw_Status_resetGame();
    void *snd = *(void **) gGof2Fmod;

    ((Status *) *(void **) gGof2StatusObj)->fadeValue = this->fadeValue;
    float v = _mtw_FModSound_stop(snd);
    _mtw_FModSound_play(snd, 0x8f, 0, v);
    void *appHolder = *(void **) gGof2Transition;
    ModuleTransitionThunk *thunk = (ModuleTransitionThunk *) appHolder;
    thunk->transitionFn3(*(void **) appHolder, 2, 0);
}

