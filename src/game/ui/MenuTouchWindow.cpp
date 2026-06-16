#include "game/ui/MenuTouchWindow.h"
#include "game/core/PaintCanvasClass.h"
#include "engine/render/Engine.h"

extern PaintCanvas *g_PaintCanvas;

extern "C" void *_mtw_GameText_getText(void *gt, int id);
void _mtw_ChoiceWindow_set(void *cw, void *s1, void *s2, bool b);
extern "C" void _mtw_render3D_inner(void *obj);
extern "C" int  _mtw_Layout_OnTouchEnd(void *layout, int y, int x);
extern "C" int  _mtw_ChoiceWindow_OnTouchEnd(void *cw, int y);
extern "C" void _mtw_FModSound_resumeAll(void *snd);
extern "C" void _mtw_FModSound_stopAll();
extern "C" void _mtw_AppMgr_SetCurrentApplicationModule(void *app, int id);
extern "C" void _mtw_AppMgr_Quit(void *app);
extern "C" void _mtw_Globals_reportLeaderboards();
extern "C" int  _mtw_onTouchEnd_listState(void *self, int y, int x, int state);
extern "C" int  _mtw_onTouchEnd_optionsState(void *self, int y, int x);
extern "C" int  _mtw_onTouchEnd_scrollState(void *self, int y, int x, int which);
extern "C" int  _mtw_onTouchEnd_missionsState(void *self, int y, int x);
extern "C" int  _mtw_onTouchEnd_cinematicState(void *self, int y, int x);
extern "C" int  _mtw_onTouchEnd_genericButtons(void *self, int y, int x, int fieldOff);
extern "C" void *_mtw_Array_TB_dtor(void *arr);
extern "C" void *_mtw_Array_TS_dtor(void *arr);
extern "C" void *_mtw_Array_Str_dtor(void *arr);
extern "C" void *_mtw_TouchButton_dtor(void *p);
extern "C" void *_mtw_ChoiceWindow_dtor(void *p);
extern "C" void *_mtw_ScrollTouchWindow_dtor(void *p);
extern "C" void *_mtw_Array_Str_dtor(void *p);
extern "C" void *_mtw_Array_StrArr_dtor(void *p);
extern "C" void _mtw_Array_StrArr_ctor(void *p);
extern "C" void _mtw_Array_Str_ctor(void *p);
extern "C" void _mtw_ArraySetLength_StrArr(int n, void *arr);
extern "C" void _mtw_ArraySetLength_Str(int n, void *arr);
extern "C" void _mtw_Globals_longToTimeStringNoSeconds(long long t, void *out);
extern "C" void _mtw_Layout_formatCredits(void *out);
extern "C" void _mtw_TouchButton_ctor7(void *btn, void *label, int a, int x, int y, char type);
extern "C" void _mtw_Status_resetGame();
extern "C" void _mtw_Status_nextCampaignMission(bool a);
extern "C" void _mtw_Mission_ctor(void *m);
extern "C" void _mtw_Status_setMission(void *status);
extern "C" void *_mtw_Ship_makeShip(int shipDef);
extern "C" void _mtw_Status_setShip(void *status);
extern "C" void _mtw_Ship_setRace(void *ship, int race);
extern "C" void *_mtw_Item_makeItem(int itemDef, int qty);
extern "C" void *_mtw_makeItem2(int itemDef);
extern "C" void _mtw_Status_setCredits(void *status);
extern "C" void *_mtw_Galaxy_getStation(void *galaxy, int idx);
extern "C" void _mtw_Status_setStation(void *status);
extern "C" void _mtw_Status_setSystemVisibility(void *status, int sys, bool vis);
extern "C" void _mtw_Achievements_setMedal(void *ach, int id, int n);
extern "C" void _mtw_RecordHandler_saveOptions(void *rh);
extern "C" void _mtw_Status_setKills(void *status);
extern "C" void _mtw_ChoiceWindow_OnTouchBegin(void *cw, int y);
extern "C" int  _mtw_Layout_OnTouchBegin(void *layout, int y);
extern "C" void _mtw_TouchButton_OnTouchBegin(void *btn, int y);
extern "C" void _mtw_TouchButton_OnTouchBeginXY(void *btn, int y, int x);
extern "C" float _mtw_TouchButton_setPosition(void *btn, int x, int y);
extern "C" void _mtw_TouchSlider_OnTouchBegin(void *sl, int y);
extern "C" void _mtw_ScrollTouchWindow_OnTouchBegin(void *w, int y);
extern "C" void _mtw_MissionsWindow_OnTouchBegin(void *w, int y);
extern "C" void _mtw_FModSound_play(void *snd, int id, void *pos, float v);
extern "C" int  _mtw_idiv(int a, int b);
extern "C" void _mtw_RecordHandler_ctor(void *rh);
extern "C" void *_mtw_RecordHandler_readRecord(void *rh);
extern "C" void *_mtw_RecordHandler_dtor(void *rh);
extern "C" void *_mtw_GameRecord_dtor(void *gr);
extern "C" void _mtw_GameRecord_load(void *gr);
void _mtw_ChoiceWindow_set(void *cw, void *s, bool b);
extern "C" void *_mtw_AppMgr_GetApplicationModule(void *app, int id);
extern "C" void _mtw_ModStation_setGameLoaded(void *ms);
extern "C" void _mtw_ArrayAdd_TouchButton(void *btn, void *arr);
extern "C" void _mtw_TouchButton_getPosition(void *out, void *btn);
extern "C" void _mtw_TouchButton_setVisible(void *btn, bool v);
extern "C" void *_mtw_Array_GameRecord_dtor(void *p);
extern "C" void *_mtw_RecordHandler_readAllPreviewRecords(void *rh);
extern "C" void _mtw_RecordHandler_recordStoreWrite(void *rh, int slot);
extern "C" void _mtw_RecordHandler_recordStoreWritePreview(void *rh, int slot);
extern "C" void *_mtw_RecordHandler_recordStoreReadPreview(void *rh);
extern "C" void _mtw_createRecordButtons(void *self, bool b);
extern "C" void *_mtw_AppMgr_GetApplicationData();
extern "C" void *_mtw_AppMgr_GetEngine();
void _mtw_ChoiceWindow_set(void *cw, void *s);
extern "C" void _mtw_ChoiceWindow_update(void *cw);
extern "C" void _mtw_TouchButton_setYPosition(void *btn, int y);
extern "C" int  _mtw_TouchButton_isVisible(void *btn);
extern "C" void _mtw_ScrollTouchWindow_update(void *w);
extern "C" void _mtw_MissionsWindow_update(void *w);
extern "C" void _mtw_startSupernovaChallenge_impl(void *self);
extern "C" void *_mtw_GetApplicationData(void *app);
extern "C" void _mtw_DlcMenu_call(void *win, void *s1, void *s2);
extern "C" void _mtw_Layout_drawBG();
extern "C" void _mtw_ChoiceWindow_OnTouchMove(void *cw, int y);
extern "C" void _mtw_TouchButton_OnTouchMove(void *btn, int y);
extern "C" void _mtw_TouchButton_OnTouchMoveXY(void *btn, int y, int x);
extern "C" void _mtw_TouchSlider_OnTouchMove(void *sl, int y);
extern "C" float _mtw_TouchSlider_getValue(void *sl);
extern "C" void _mtw_ScrollTouchWindow_OnTouchMove(void *w, int y);
extern "C" void _mtw_MissionsWindow_OnTouchMove(void *w, int y);
extern "C" int  _mtw_FModSound_tryToStopMusicForBGMusic();
extern "C" void _mtw_FModSound_setVolume(void *snd, float v);
extern "C" void _mtw_Array_TB_ctor(void *a);
extern "C" void _mtw_loadPreviewRecords(void *self);
extern "C" void _mtw_TouchButton_draw(void *btn);
extern "C" void _mtw_Layout_drawBox(void *layout, int mode, int x, int y, int w, int h, void *str);
extern "C" void _mtw_ImageFactory_drawShip(void *imgF, unsigned int shipId, int x, int y);
extern "C" void _mtw_Ship_addCargo(void *ship, void *item);
extern "C" float _mtw_FModSound_stop(void *snd);

// hidden PC-relative pointer-to-pointer global (the GameText singleton holder).
extern void *const gSupernovaGameText __attribute__((visibility("hidden")));

// MenuTouchWindow::showSupernovaMessage()
void MenuTouchWindow::showSupernovaMessage()
{
    void *cw = this->choiceWindow;
    void *gt = *(void **)gSupernovaGameText;
    void *s1 = _mtw_GameText_getText(gt, 0x266);
    void *s2 = _mtw_GameText_getText(*(void **)gSupernovaGameText, 0x267);
    _mtw_ChoiceWindow_set(cw, s1, s2, false);
    this->messageShowing = 1;
    this->supernovaMessageShowing = 1;
}

// MenuTouchWindow::isInMissionScreen()
bool MenuTouchWindow::isInMissionScreen()
{
    return this->menuState == 9;
}

// MenuTouchWindow::isShowingMessage()
uint8_t MenuTouchWindow::isShowingMessage()
{
    return this->messageShowing;
}

// MenuTouchWindow::isMakingScreenshot() -> (unsigned)field < 0x80000000, i.e. signed >= 0.
bool MenuTouchWindow::isMakingScreenshot()
{
    return this->screenshotState < 0x80000000u;
}

// MenuTouchWindow::hideMessage()
void MenuTouchWindow::hideMessage()
{
    this->messageShowing = 0;
}

// MenuTouchWindow::render3D()
void MenuTouchWindow::render3D()
{
    if (this->menuState != 9)
        return;
    void *obj = this->missionsWindow;
    if (obj == 0)
        return;
    return _mtw_render3D_inner(obj);
}

// MenuTouchWindow::inCinematicMode()
bool MenuTouchWindow::inCinematicMode()
{
    return this->menuState == 0xd;
}

// MenuTouchWindow::getRelativeScrollStartPos()
//   r = CONST; if (this->0x194 <= 0) r = -(float)(int)this->0x194 / (float)(int)this->0x22c;
float MenuTouchWindow::getRelativeScrollStartPos()
{
    int a = this->scrollOffset;
    if (a > 0) {
        union { unsigned u; float f; } c;
        c.u = 0x9a132100u;
        return c.f;
    }
    return -(float)a / (float)this->pageHeight;
}

// MenuTouchWindow::OnTouchEnd(int y, int x). The menu's master action dispatcher: when a
// ChoiceWindow dialog is open it routes the release to the right confirm/cancel handler
// (case 0 below); otherwise it forwards to whichever widget set owns the current state
// (this->0x16c) and runs that state's button actions. The per-state action bodies are large
// and share a lot of structure, so each is delegated to an extern handler that performs the
// genuine work for that state. r0=this, r1=y, r2=x. Returns 0.

// Per-state release handlers (return non-zero when they fully consumed the event).

extern void *const gEndLayoutGuard __attribute__((visibility("hidden"))); // *holder -> layout (*piVar17)
extern void *const gEndStatusObj   __attribute__((visibility("hidden")));
extern void *const gEndAppHolder   __attribute__((visibility("hidden")));
extern void *const gEndFmod        __attribute__((visibility("hidden")));
extern void *const gEndQuitApp     __attribute__((visibility("hidden")));
extern void *const gEndModuleId    __attribute__((visibility("hidden"))); // *holder -> module id
extern void *const gEndPendingFlag __attribute__((visibility("hidden"))); // *holder -> int set on confirm

int MenuTouchWindow::OnTouchEnd(int y, int x)
{
    this->pendingActivate = 0;
    this->dragging = 0;

    // Modal Layout (e.g. virtual keyboard) intercepts first.
    char *layoutGuard = (char *)*(void **)gEndLayoutGuard;
    if (*layoutGuard != 0) {
        if (_mtw_Layout_OnTouchEnd(layoutGuard, y, x) != 0)
            *layoutGuard = 0;
        return 0;
    }

    int state = this->menuState;

    // ---- case 0: a ChoiceWindow dialog is up; resolve its confirm/cancel ----
    if (state == 0) {
        if (this->messageShowing != 0) {
            void *cw = this->choiceWindow;
            if (this->returnToMenuShowing != 0) {
                // quit-confirmation dialog
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    this->returnToMenuShowing = 0;
                    this->messageShowing = 0;
                } else if (r == 0) {
                    _mtw_AppMgr_Quit(*(void **)gEndQuitApp);
                }
                return 0;
            }
            if (this->genericConfirmB != 0) {
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r != 1) {
                    if (r != 0) return 0;
                    *(int *)*(void **)gEndPendingFlag = 1;
                }
                this->messageShowing = 0;
                this->genericConfirmB = 0;
                return 0;
            }
            if (this->quitConfirmShowing != 0) {
                // return-to-menu confirmation
                int r = _mtw_ChoiceWindow_OnTouchEnd(cw, y);
                if (r == 1) {
                    this->quitConfirmShowing = 0; this->messageShowing = 0; this->returnToMenuSubFlag = 0;
                    return 0;
                }
                if (r != 0) return 0;
                _mtw_FModSound_resumeAll(*(void **)gEndFmod);
                _mtw_FModSound_stopAll();
                *(int *)*(void **)gEndModuleId = 2;
                _mtw_AppMgr_SetCurrentApplicationModule(*(void **)gEndAppHolder,
                    *(int *)*(void **)gEndModuleId);
                this->quitConfirmShowing = 0;
                this->messageShowing = 0;
                return 0;
            }
            if (this->leaderboardDialogShowing != 0) {
                unsigned int r = (unsigned int)_mtw_ChoiceWindow_OnTouchEnd(cw, y);
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
                this->messageShowing = 0; this->genericConfirmA = 0; return 0;
            } else if (this->supernovaMessageShowing != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                this->supernovaMessageShowing = 0;
            } else if (this->supernovaPurchaseDialogShowing != 0 && _mtw_ChoiceWindow_OnTouchEnd(cw, y) == 0) {
                this->supernovaPurchaseDialogShowing = 0;
            } else {
                // generic dialog dismissal
                _mtw_ChoiceWindow_OnTouchEnd(cw, y);
            }
            this->messageShowing = 0;
            return 0;
        }
        return _mtw_onTouchEnd_genericButtons(this, y, x, 0x4), 0;
    }

    // ---- non-zero states: forward to the owning widget set ----
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

// MenuTouchWindow::~MenuTouchWindow(). Tears down all owned arrays, buttons, choice window,
// scroll windows and two heap buffers.
extern "C" void _mtw_ArrayReleaseClasses_TB(void *arr);       // Array<TouchButton*> release
extern "C" void _mtw_ArrayReleaseClasses_TS(void *arr);       // Array<TouchSlider*> release
extern "C" void _mtw_ArrayReleaseClasses_Str(void *arr);      // Array<String*> release

static inline void freeArrayTB(void **slot) {
    void *a = *slot;
    if (a != 0) {
        _mtw_ArrayReleaseClasses_TB(a);
        void *b = *slot;
        if (b != 0) ::operator delete(_mtw_Array_TB_dtor(b));
    }
    *slot = 0;
}
static inline void freeObj(void **slot, void *(*dtor)(void *)) {
    void *o = *slot;
    if (o != 0) ::operator delete(dtor(o));
    *slot = 0;
}

MenuTouchWindow::~MenuTouchWindow()
{
    freeArrayTB((void **)&this->buttons);
    freeArrayTB((void **)&this->optionsButtons);
    freeArrayTB((void **)&this->scrollEntries);
    freeArrayTB((void **)&this->buttonsB4);
    freeArrayTB((void **)&this->buttonsB8);
    freeArrayTB((void **)&this->buttonsB0);

    // Array<TouchSlider*> at 0xec
    {
        void *a = this->sliders;
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TS(a);
            void *b = this->sliders;
            if (b != 0) ::operator delete(_mtw_Array_TS_dtor(b));
        }
        this->sliders = 0;
    }

    freeArrayTB((void **)&this->scrollSlots);

    // Array<String*> at previewStrings0 / previewStrings1
    {
        void **slots[2] = { (void **)&this->previewStrings0, (void **)&this->previewStrings1 };
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

    // single TouchButton members
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

    // two heap byte-buffers freed with operator delete[]
    if (this->heapBufA != 0) ::operator delete[](this->heapBufA);
    this->heapBufA = 0;
    if (this->heapBufB != 0) ::operator delete[](this->heapBufB);
    this->heapBufB = 0;
}

// MenuTouchWindow::createRecordButtons(bool inSaveMode).
// Rebuilds the per-slot text rows (Array<Array<String*>>) shown in the load/save list and
// the OK/back buttons. r0=this, r1=inSaveMode.

extern "C" void _mtw_ArrayReleaseClasses_Str(void *arr);          // Array<String*> release
extern "C" void _mtw_ArrayReleaseClasses_StrArr(void *arr);       // Array<Array<String*>*> release
extern "C" void *_mtw_String_op_plus(void *out, void *rhs);        // operator+
extern "C" void _mtw_TouchButton_ctorFull(void *btn, void *label, int a, int x, int y, int w,
                                          char type, char id);

// PC-relative holders.
extern void *const gCrbRowCount __attribute__((visibility("hidden")));   // *holder -> [0] = row count
extern void *const gCrbTimeVal  __attribute__((visibility("hidden")));   // *holder -> time/playtime value
extern void *const gCrbGameText __attribute__((visibility("hidden")));   // *holder -> GameText
extern const char gCrbDash[]    __attribute__((visibility("hidden")));   // "-" placeholder
extern void *const gCrbLayout   __attribute__((visibility("hidden")));   // *holder -> layout obj
extern void *const gCrbScreenW  __attribute__((visibility("hidden")));   // *holder -> [0] = screen width
extern void *const gCrbBackFlag __attribute__((visibility("hidden")));   // *holder -> [0]: byte, use 7-button form

void MenuTouchWindow::createRecordButtons(bool inSaveMode)
{

    // Tear down any existing 2D row array.
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
    this->recordRows = (Array<void *> *)outer;
    int rowCount = *(int *)*(void **)gCrbRowCount;
    _mtw_ArraySetLength_StrArr(rowCount, outer);

    void *timeHolder = *(void **)gCrbTimeVal;
    void *gtHolder   = *(void **)gCrbGameText;

    for (int i = 0; i < rowCount; i++) {
        void *row = ::operator new(0xc);
        _mtw_Array_Str_ctor(row);
        this->recordRows->data_[i] = row;
        _mtw_ArraySetLength_Str(6, this->recordRows->data_[i]);

        String s48; s48.ctor();
        String s54;

        void *rec = this->previewRecords;
        bool empty = (rec == 0) || (*(int *)(i32(rec, 4) + i * 4) == 0);

        if (empty) {
            s54.ctor();
            _mtw_Globals_longToTimeStringNoSeconds((long long)(int)(long)timeHolder, 0);
            void *e;
            int *rowData = (int *)i32(this->recordRows->data_[i], 4);

            e = ::operator new(0xc); ((String *)e)->ctor_copy(&s54, false); ((void **)rowData)[0] = e;
            e = ::operator new(0xc); ((String *)e)->ctor_copy((String *)_mtw_GameText_getText(*(void **)gtHolder, 0x1e6), false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[1] = e;

            e = ::operator new(0xc);
            if (i == 0) ((String *)e)->ctor_copy((String *)_mtw_GameText_getText(*(void **)gtHolder, 0x1e6), false);
            else ((String *)e)->ctor_char(gCrbDash, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[2] = e;

            e = ::operator new(0xc); ((String *)e)->ctor_char(gCrbDash, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[3] = e;

            e = ::operator new(0xc); ((String *)e)->ctor_char(gCrbDash, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[4] = e;

            e = ::operator new(0xc); ((String *)e)->ctor_char(gCrbDash, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[5] = e;
        } else {
            s54.ctor();
            int slot = *(int *)(i32(rec, 4) + i * 4);
            _mtw_Globals_longToTimeStringNoSeconds((long long)slot, *(void **)(slot + 0x10));
            void *e;
            int *rowData;

            e = ::operator new(0xc); ((String *)e)->ctor_copy(&s54, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[0] = e;

            e = ::operator new(0xc);
            // RAWREAD: GameRecord+0x194/+0x20/+0x11c (preview record held as int32 in Array<GameRecord*>; name/kills/rank members not modeled in GameRecord.h)
            ((String *)e)->ctor_copy((String *)(void *)(*(int *)(i32(this->previewRecords, 4) + i * 4) + 0x194), false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[1] = e;

            e = ::operator new(0xc);
            if (i == 0) ((String *)e)->ctor_copy((String *)_mtw_GameText_getText(*(void **)gtHolder, 0x1e6), false);
            else ((String *)e)->ctor_char(gCrbDash, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[2] = e;

            void *credits = ::operator new(0xc);
            _mtw_Layout_formatCredits(credits);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[3] = credits;

            // build "Kills: " + value string
            void *combined = ::operator new(0xc);
            void *label = _mtw_GameText_getText(*(void **)gtHolder, 0x141);
            String s6c; s6c.ctor_char(gCrbDash, false);
            String s60; _mtw_String_op_plus(&s60, label);
            String s78; s78.ctor_copy((String *)(long)(*(int *)(i32(this->previewRecords, 4) + i * 4) + 0x20), false);
            _mtw_String_op_plus(combined, &s60);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[4] = combined;

            e = ::operator new(0xc);
            float rank = *(float *)(*(int *)(i32(this->previewRecords, 4) + i * 4) + 0x11c);
            void *rankTxt;
            if (rank <= 0.0f) rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x207);
            else if (rank <= 0.5f) rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x207);
            else if (rank <= 1.0f) rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x207);
            else rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x19);
            ((String *)e)->ctor_copy((String *)rankTxt, false);
            rowData = (int *)i32(this->recordRows->data_[i], 4); ((void **)rowData)[5] = e;
        }
    }

    // OK button (+0xc4)
    if (this->okButton != 0) { ::operator delete(_mtw_TouchButton_dtor(this->okButton)); this->okButton = 0; }
    int *layout = (int *)*(void **)gCrbLayout;
    char backForm = *(char *)*(void **)gCrbBackFlag;
    int extra = layout[0x42]; // [0x108]
    void *okBtn = ::operator new(0xc8);
    int screenW = *(int *)*(void **)gCrbScreenW;
    int x = (screenW - this->listX) - layout[0xa]; // -[0x28]
    int y = layout[8] + layout[3] + extra;              // [0x20]+[0xc]
    void *okLabel = _mtw_GameText_getText(*(void **)gtHolder, backForm ? 0x1fa : 0x1f9);
    if (backForm)
        _mtw_TouchButton_ctor7(okBtn, okLabel, 7, x, y, 0x12);
    else
        _mtw_TouchButton_ctorFull(okBtn, okLabel, 0, x, y, layout[0xa9] /*+0x2a4*/, 0x12, 0x04);
    this->okButton = okBtn;

    _mtw_TouchButton_setPosition(okBtn,
        (screenW - this->listX) - layout[0xa],
        (layout[0x1c] + this->listRowGap) * this->selectedRow + layout[3] + extra + layout[8] + this->scrollOffset);

    // back button (+0xc8)
    if (this->backButton != 0) { ::operator delete(_mtw_TouchButton_dtor(this->backButton)); this->backButton = 0; }
    void *backBtn = ::operator new(0xc8);
    void *backLabel = _mtw_GameText_getText(*(void **)gtHolder, 0x41);
    _mtw_TouchButton_ctor7(backBtn, backLabel, 7,
        (screenW - this->listX) - layout[0xa],
        layout[3] + extra + layout[8], 0x12);
    this->backButton = backBtn;
}

// MenuTouchWindow::startValkyrie() - sets up the "Valkyrie" pre-made savegame and jumps
// into the game. Walks 0x2d campaign missions forward, builds a ship with fixed loadout,
// flips a set of option flags, awards medals, then transitions to the game module.

extern "C" void *_mtw_Status_getShip(void *status);       // pcVar13: returns ship from status
extern "C" void *_mtw_Item_make(int itemDef);             // pcVar12: makeItem(def)
extern "C" void _mtw_Ship_setItem(void *ship, void *item, int slot); // pcVar9

// PC-relative holders.
extern void *const gValStatus    __attribute__((visibility("hidden"))); // *holder -> Status object
extern void *const gValShipTable __attribute__((visibility("hidden"))); // *holder -> def table (.+4 -> defs)
extern void *const gValGalaxy    __attribute__((visibility("hidden")));
extern void *const gValOptA      __attribute__((visibility("hidden"))); // option flags object
extern void *const gValAch       __attribute__((visibility("hidden"))); // Achievements holder
extern void *const gValOptB      __attribute__((visibility("hidden"))); // second flags object (+0x34,+0x2c)
extern void *const gValOptHolder __attribute__((visibility("hidden")));
extern void *const gValFmod      __attribute__((visibility("hidden")));
extern void *const gValTransition __attribute__((visibility("hidden")));

typedef void (*TransitionFn)(void *app, int mode);

void MenuTouchWindow::startValkyrie()
{
    void **statusHolder = (void **)gValStatus;
    _mtw_Status_resetGame();
    for (int i = 0x2d; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool)(unsigned char)(long)*statusHolder);

    void *status = *statusHolder;
    void *mission = ::operator new(0x78);
    _mtw_Mission_ctor(mission);
    _mtw_Status_setMission(status);

    int *defs = (int *)(*(int *)(*(void **)gValShipTable) + 4);
    status = *statusHolder;
    _mtw_Ship_makeShip(*(int *)(defs[0] + 0x14)); // defs is table; [4]->row; +0x14
    _mtw_Status_setShip(status);

    void *ship = *statusHolder;
    int race = (int)(long)_mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    int *row = (int *)(*(int *)(*(void **)gValShipTable) + 4);
    void *it; void *sh;
    it = _mtw_Item_make(*(int *)(row[0] + 8));   sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0x14));sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x90));sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0x144));sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0xcc)); sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x158));sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 2);
    it = _mtw_Item_make(*(int *)(row[0] + 0x154));sh = _mtw_Status_getShip(*statusHolder);_mtw_Ship_setItem(sh, it, 3);

    _mtw_Status_setCredits(*statusHolder);
    void *station = *statusHolder;
    _mtw_Galaxy_getStation(*(void **)gValGalaxy, 0x5b);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    // RAWREAD: opt+* / optB+* / Status(*statusHolder)+0x84 (untyped char* holders gValOptA/gValOptB/gValStatus;
    //          option-flag objects and Status pre-set fields not modeled)
    char *opt = (char *)*(void **)gValOptA;
    void *ach = *(void **)gValAch;
    char *optB = (char *)*(void **)gValOptB;
    *(int *)(*(char **)statusHolder + 0x84) = 0x1a0a;
    *(unsigned short *)(opt + 8) = 0x101;
    *(unsigned short *)(opt + 0xd) = 0x101;
    *(unsigned short *)(opt + 0x1c) = 0x101;
    *(int *)(opt + 0x20) = 0x1010101;
    opt[0x17] = 1; opt[0xa] = 1; opt[0x15] = 1; opt[0x13] = 1; opt[0xf] = 1;
    opt[0x1e] = 1; opt[0x24] = 1; opt[0x36] = 0; *(int *)(opt + 0x32) = 0; opt[0x38] = 1;
    optB[0x34] = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*(void **)gValAch, 0x1e, 1);
    _mtw_RecordHandler_saveOptions(*(void **)gValOptHolder);
    _mtw_Status_setKills(*statusHolder);
    _mtw_FModSound_stop(*(void **)gValFmod);

    void **app = (void **)gValTransition;
    *(uint32_t *)(optB + 0x2c) = this->fadeValue;
    TransitionFn fn = *(TransitionFn *)((char *)app); // DAT_001ab904 thunk
    fn(*app, 5);
}

// MenuTouchWindow::OnTouchBegin(int y, int x, int touchId). Routes the press to whichever
// sub-widget owns the current menu state (this->0x16c), kicking off scroll drags, slider
// grabs, list selection and the in-flight 3D steer. r0=this, r1=y, r2=x, r3=touchId. Returns 0.

extern void *const gBgLayout   __attribute__((visibility("hidden"))); // *holder -> layout (=*piVar5)
extern void *const gBgScreenW  __attribute__((visibility("hidden")));
extern void *const gBgScrollDiv __attribute__((visibility("hidden")));
extern void *const gBgListPosX __attribute__((visibility("hidden")));
extern void *const gBgFmod     __attribute__((visibility("hidden")));
extern void *const gBgFlagA    __attribute__((visibility("hidden")));
extern void *const gBgFlagB    __attribute__((visibility("hidden")));
extern void *const gBgFlagC    __attribute__((visibility("hidden")));
extern void *const gBgFlagD    __attribute__((visibility("hidden")));
extern void *const gBgObjA     __attribute__((visibility("hidden")));
extern void *const gBgObjB     __attribute__((visibility("hidden")));
extern void *const gBgScreenH2 __attribute__((visibility("hidden")));
extern void *const gBgScrollImg __attribute__((visibility("hidden")));
extern void *const gBgScreenW2 __attribute__((visibility("hidden")));

int MenuTouchWindow::OnTouchBegin(int y, int x, int touchId)
{
    if (this->messageShowing != 0) { _mtw_ChoiceWindow_OnTouchBegin(this->choiceWindow, y); return 0; }

    char *layout = (char *)*(void **)gBgLayout;
    if (*layout != 0) { _mtw_Layout_OnTouchBegin(layout, y); return 0; }

    int state = this->menuState;
    switch (state) {
    case 1:
    case 2: {
        this->dragStartX = x;
        this->dragLastX = x;
        this->dragVelocity = 0;
        this->dragging = 1;
        int oldRow = this->selectedRow;
        int leftMargin = *(int *)(layout + 0xc);
        if (leftMargin < x && x < *(int *)*(void **)gBgScreenW - *(int *)(layout + 0x10)) {
            int rowH = *(int *)(layout + 0x70);
            int gap = this->listRowGap;
            int top = *(int *)(layout + 0x20);
            int off = this->scrollOffset;
            int row = _mtw_idiv(x + ((-top - leftMargin) - off), 1);
            if (row < *(int *)*(void **)gBgScrollDiv) {
                int *posX = (int *)*(void **)gBgListPosX;
                this->selectedRow = row;
                float v = _mtw_TouchButton_setPosition(this->okButton,
                    (*posX - this->listX) - *(int *)(layout + 0x28),
                    row * (gap + rowH) + top + leftMargin + off + *(int *)(layout + 0x108));
                _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, v);
            }
        }
        if (oldRow == this->selectedRow)
            _mtw_TouchButton_OnTouchBegin(this->okButton, y);
    } break;
    case 3: {
        if (*(char *)*(void **)gBgFlagA == 0) {
            void **arr = (void **)this->optionsButtons;
            for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
                _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
            break;
        }
        _mtw_TouchButton_OnTouchBegin(this->scrollUpButton, y);
        this->upButtonPressed = 0;
        int b28 = *(int *)(layout + 0x28);
        int top = b28 + this->listTopY;
        if (top < y && y < this->listEntryHeight + top &&
            *(int *)(layout + 0xc) + b28 < x &&
            x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + this->listEntryWidth) {
            this->upButtonPressed = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
            b28 = *(int *)(layout + 0x28);
            top = b28 + this->listTopY;
        }
        int bottom = top + this->listEntryHeight;
        if (bottom < y && y < (this->listTopY - b28) + this->listBottomY &&
            b28 + *(int *)(layout + 0xc) < x &&
            x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + this->listEntryWidth) {
            this->downButtonPressed = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
        }
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnCC, y, x);
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnD0, y, x);
        _mtw_TouchSlider_OnTouchBegin(*(void **)i32(this->sliders, 4), y);
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnD4, y, x);
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnD8, y, x);
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnDC, y, x);
        void **arr = (void **)this->sliders;
        for (unsigned int i = 1; i < *(unsigned int *)arr; i++)
            _mtw_TouchSlider_OnTouchBegin(((void **)arr[1])[i], y);
        if (*(char *)*(void **)gBgFlagB != 0 && this->scrollExtraButton != 0)
            _mtw_TouchButton_OnTouchBegin(this->scrollExtraButton, y);
    } break;
    case 4: {
        _mtw_ScrollTouchWindow_OnTouchBegin(this->scrollWindowA, y);
        void **arr = (void **)this->scrollEntries;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            unsigned int t = e[0];
            if (((unsigned int)(t - 0x6a) < 5 && ((1u << ((t - 0x6a) & 0xff)) & 0x19u) != 0)
                || (e[1] == 0 && t == 0x16))
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
    } break;
    case 6:
    case 0xd:
        break;
    case 7: {
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnD4, y, x);
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnD8, y, x);
        _mtw_TouchButton_OnTouchBeginXY(this->optBtnDC, y, x);
        void **arr = (void **)this->sliders;
        unsigned int n = *(unsigned int *)arr;
        for (unsigned int i = 1; i < n; i++) {
            if (i == 5 && (unsigned char)*(char *)(*(char **)gBgLayout + 0x284) == 0) continue;
            _mtw_TouchSlider_OnTouchBegin(((void **)arr[1])[i], y);
        }
    } break;
    case 8: {
        this->upButtonPressed = 0;
        int b28 = *(int *)(layout + 0x28);
        int top = b28 + this->listTopY;
        if (top < y && y < this->listEntryHeight + top &&
            *(int *)(layout + 0xc) + b28 < x &&
            x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + this->listEntryWidth) {
            this->upButtonPressed = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
            layout = (char *)*(void **)gBgLayout;
            b28 = *(int *)(layout + 0x28);
            top = b28 + this->listTopY;
        }
        int bottom = top + this->listEntryHeight;
        if (bottom < y && y < (this->listTopY - b28) + this->listBottomY &&
            b28 + *(int *)(layout + 0xc) < x &&
            x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + this->listEntryWidth) {
            this->downButtonPressed = 1;
            _mtw_FModSound_play(*(void **)gBgFmod, 0x7c, 0, 0);
        }
        _mtw_TouchButton_OnTouchBegin(this->optBtnCC, y);
        _mtw_TouchButton_OnTouchBegin(this->optBtnD0, y);
        _mtw_TouchSlider_OnTouchBegin(*(void **)i32(this->sliders, 4), y);
    } break;
    case 9:
        _mtw_MissionsWindow_OnTouchBegin(this->missionsWindow, y);
        break;
    case 0xb: {
        if (*(char *)*(void **)gBgFlagC != 0 && *(char *)*(void **)gBgFlagD == 0) {
            _mtw_TouchButton_OnTouchBegin(this->cinematicBtnA, y);
            _mtw_TouchButton_OnTouchBegin(this->cinematicBtnB, y);
            // RAWREAD: gBgObjA+0x54 / gBgObjB+0x58 (untyped char* holders; cinematic hit-zone X anchors, objects not modeled)
            if (touchId != 0 &&
                (this->cinematicTouchIdA != 0 || y > 0xd1 || this->cinematicTouchIdB == touchId ||
                 x <= *(int *)(*(char **)gBgObjA + 0x54) - 0x14 ||
                 *(int *)(*(char **)gBgObjA + 0x54) + 300 <= x)) {
                if (this->cinematicTouchIdB == touchId || touchId == 0 ||
                    this->cinematicTouchIdB != 0 || y <= *(int *)*(void **)gBgScreenH2 - 0xdc ||
                    x <= *(int *)(*(char **)gBgObjB + 0x58) - 0x14 ||
                    *(int *)(*(char **)gBgObjB + 0x58) + 0xe6 <= x) {
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
    } break;
    case 0xc: {
        void **arr = (void **)this->buttonsB4;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
    } break;
    case 0xe: {
        void **arr = (void **)this->buttonsB0;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
    } break;
    case 0x10: {
        void **arr = (void **)this->scrollEntries;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            if ((unsigned int)(e[0] - 0x65) < 5 && e[1] == 0)
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
    } // fallthrough into 0xf
    [[fallthrough]];
    case 0xf: {
        _mtw_ScrollTouchWindow_OnTouchBegin(this->scrollWindowB, y);
        void **arr = (void **)this->scrollEntries;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            if ((e[0] | 8) == 0x3c && e[1] == 0)
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
        void **slots = (void **)this->scrollSlots;
        for (unsigned int i = 0; i < *(unsigned int *)slots; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)slots[1])[i], y);

        void *img = *(void **)gBgScrollImg;
        int bound = *(int *)*(void **)gBgScreenW2;
        int b28 = *(int *)(*(char **)gBgLayout + 0x28);
        int iw = g_PaintCanvas->GetImage2DWidth((unsigned int)(long)img);
        unsigned char hit;
        if ((bound - b28) - iw < y) {
            int lc = *(int *)(*(char **)gBgLayout + 0xc);
            int tp = *(int *)(*(char **)gBgLayout + 0x20);
            int ih = g_PaintCanvas->GetImage2DHeight((unsigned int)(long)img);
            hit = (x < ih + tp + lc) ? 1 : 0;
        } else hit = 0;
        this->scrollbarHit = hit;

        b28 = *(int *)(*(char **)gBgLayout + 0x28);
        iw = g_PaintCanvas->GetImage2DWidth((unsigned int)(long)img);
        if (y < iw + b28) {
            this->dragStartX = x;
            this->dragLastX = x;
            this->dragVelocity = 0;
            this->dragging = 1;
        }
    } break;
    case 0x11: {
        void **arr = (void **)this->buttonsB8;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
    } break;
    default: {
        void **arr = (void **)this->buttons;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchBegin(((void **)arr[1])[i], y);
        void **arr2 = (void **)this->scrollEntries;
        unsigned int n = *(unsigned int *)arr2;
        for (unsigned int i = 0; i < n; i++) {
            int *e = (int *)((void **)arr2[1])[i];
            if ((unsigned int)(e[0] - 0x17) < 2) _mtw_TouchButton_OnTouchBegin(e, y);
        }
        for (unsigned int i = 0; i < n; i++) {
            unsigned int *e = (unsigned int *)((void **)arr2[1])[i];
            unsigned int t = e[0], id = e[1];
            bool go;
            if (t == 5 && id == 0) go = true;
            else {
                unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                go = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
            }
            if (go) { _mtw_TouchButton_OnTouchBegin(e, y); arr2 = (void **)this->scrollEntries; n = *(unsigned int *)arr2; }
        }
    } break;
    }

    int r = _mtw_Layout_OnTouchBegin(*(void **)gBgLayout, y);
    if (r != 0 && this->menuState == 0xd)
        this->pendingActivate = 1;
    return 0;
}

// PC-relative singleton holders.
extern void *const gLoadStatusFlags __attribute__((visibility("hidden")));   // *holder -> flags, +0x35/+0x37
extern void *const gLoadGameText __attribute__((visibility("hidden")));      // *holder -> GameText
extern void *const gLoadAppHolder __attribute__((visibility("hidden")));     // *holder -> app
extern void *const gLoadResetCell __attribute__((visibility("hidden")));     // *holder -> int cell to clear

// MenuTouchWindow::loadGame(int slot)
int MenuTouchWindow::loadGame(int slot)
{
    void *rh = ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    void *rec = _mtw_RecordHandler_readRecord(rh);

    if (rec == 0) {
        _mtw_Status_resetGame();
        void *cw = this->choiceWindow;
        void *s = _mtw_GameText_getText(*(void **)gLoadGameText, 0x64);
        _mtw_ChoiceWindow_set(cw, s, false);
        this->loadFailedDialogShowing = 1;
        this->messageShowing = 1;
        ::operator delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    void *flags = *(void **)gLoadStatusFlags;
    // RAWREAD: flags+0x37 / flags+0x35 (untyped void* holder gLoadStatusFlags; Status flag object, byte members not modeled)
    bool versionOk = (u8(rec, 0x117) == 0) || (*(char *)((char *)flags + 0x37) != 0);
    if (versionOk) {
        bool dlcOk = (u8(rec, 0x115) == 0) || (*(char *)((char *)flags + 0x35) != 0);
        if (dlcOk) {
            // fully compatible -> load it
            _mtw_Status_resetGame();
            _mtw_GameRecord_load(rec);
            ::operator delete(_mtw_RecordHandler_dtor(rh));
            ::operator delete(_mtw_GameRecord_dtor(rec));
            void *app = *(void **)gLoadAppHolder;
            void *ms = _mtw_AppMgr_GetApplicationModule(*(void **)app, 5);
            _mtw_ModStation_setGameLoaded(ms);
            *(int *)*(void **)gLoadResetCell = 0;
            _mtw_AppMgr_SetCurrentApplicationModule(*(void **)app, 5);
            return 1;
        }
        // DLC missing
        void *cw = this->choiceWindow;
        void *s = _mtw_GameText_getText(*(void **)gLoadGameText, 0x65);
        _mtw_ChoiceWindow_set(cw, s, false);
        this->messageShowing = 1;
        ::operator delete(_mtw_GameRecord_dtor(rec));
        ::operator delete(_mtw_RecordHandler_dtor(rh));
        return 0;
    }

    // version mismatch
    void *cw = this->choiceWindow;
    void *s = _mtw_GameText_getText(*(void **)gLoadGameText, 0x66);
    _mtw_ChoiceWindow_set(cw, s, false);
    this->messageShowing = 1;
    ::operator delete(_mtw_GameRecord_dtor(rec));
    ::operator delete(_mtw_RecordHandler_dtor(rh));
    return 0;
}

extern "C" void _mtw_TouchButton_ctor(void *btn, void *label, int a, int x, int y, int w,
                                      char type, char id);

// PC-relative singleton holders.
extern void *const gAddBtnScreenW __attribute__((visibility("hidden")));  // *holder -> [0]=screen width
extern void *const gAddBtnLayout  __attribute__((visibility("hidden")));  // *holder -> layout, +0x30=row height
extern void *const gAddBtnScreenH __attribute__((visibility("hidden")));  // *holder -> [0]=screen height
extern void *const gAddBtnPosX    __attribute__((visibility("hidden")));  // x cache array
extern void *const gAddBtnPosY    __attribute__((visibility("hidden")));  // y cache array
extern void *const gAddBtnCount   __attribute__((visibility("hidden")));  // *holder -> count cell

// MenuTouchWindow::addButton(int id, String label, int row, Array<TouchButton*>* arr, int yOff)
void MenuTouchWindow::addButton(int id, void *label, int row, void *arr, int yOff)
{
    void *btn = ::operator new(0xc8);

    int btnW = this->buttonWidth;
    int screenW = *(int *)*(void **)gAddBtnScreenW;
    int screenH = *(int *)*(void **)gAddBtnScreenH;
    int rowH = *(int *)((char *)*(void **)gAddBtnLayout + 0x30);

    int x = screenW / 2 - btnW / 2;
    int y = (rowH + this->buttonRowGap) * row + (yOff - this->buttonYBias) + screenH / 2;

    _mtw_TouchButton_ctor(btn, label, 0, x, y, btnW, 0x11, 0x04);
    i32(btn, 0) = id;
    i32(btn, 4) = id >> 31;
    _mtw_ArrayAdd_TouchButton(btn, arr);

    int *posX = (int *)*(void **)gAddBtnPosX;
    int *posY = (int *)*(void **)gAddBtnPosY;
    for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
        if (i < 10) {
            char buf[12];
            void *b = ((void **)i32(arr, 4))[i];
            _mtw_TouchButton_getPosition(buf, b);
            posX[i] = (int)*(float *)(buf + 0);
            b = ((void **)i32(arr, 4))[i];
            _mtw_TouchButton_getPosition(buf, b);
            posY[i] = (int)*(float *)(buf + 4);
        }
    }
    *(uint32_t *)*(void **)gAddBtnCount = *(uint32_t *)arr;
}

// MenuTouchWindow::setCutsceneMode(bool). Ghidra mislabels: r0=this, r1=mode.
// Sets this->0x238 = mode, then hides/shows all type-0x13 buttons (id 0) to !mode.
void MenuTouchWindow::setCutsceneMode(bool mode)
{
    this->cutsceneMode = (uint8_t)mode;
    void **arr = (void **)this->buttons;
    for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
        void *btn = ((void **)arr[1])[i];
        if (i32(btn, 0) == 0x13 && i32(btn, 4) == 0) {
            _mtw_TouchButton_setVisible(btn, (bool)((uint8_t)mode ^ 1));
        }
        arr = (void **)this->buttons;
    }
}

// PC-relative singleton holders used to lay out the scroll geometry.
extern void *const gPrevScreenW __attribute__((visibility("hidden")));   // *holder -> screen metrics, [0]=width
extern void *const gPrevLayout __attribute__((visibility("hidden")));    // *holder -> layout, +0x10/0xc/0x20/0x24/0x70
extern void *const gPrevRowCnt __attribute__((visibility("hidden")));    // *holder -> [0]=row count
typedef void (*RefreshFn)();
extern void *const gPrevRefreshThunk __attribute__((visibility("hidden")));

// MenuTouchWindow::loadPreviewRecords()
void MenuTouchWindow::loadPreviewRecords()
{
    this->scrollOffset = 0;
    // zero the drag / inertia state block (originally two overlapping 16-byte clears
    // at +0x20c and +0x215 spanning +0x20c..+0x224)
    this->dragLastX = 0;
    this->dragVelocity = 0;
    this->inertiaDecay = 0;
    this->inertiaVel = 0.0f;
    this->dragStartX = 0;
    this->dragging = 0;

    int *metrics = (int *)*(void **)gPrevScreenW;
    int *layout = (int *)*(void **)gPrevLayout;
    int *rowObj = (int *)*(void **)gPrevRowCnt;

    // content height = width - margins (+0x10,+0xc,+0x20,+0x24 of layout)
    this->contentHeight = (((metrics[0] - layout[3]) - layout[2]) - layout[8]) - layout[9];
    // page height = rowCount * (rowHeight + this->0x1b4)
    this->pageHeight = rowObj[0] * (layout[0x1c] + this->listRowGap);

    void *rec = this->previewRecords;
    if (rec != 0) {
        ::operator delete(_mtw_Array_GameRecord_dtor(rec));
        this->previewRecords = 0;
    }
    void *rh = ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    this->previewRecords = _mtw_RecordHandler_readAllPreviewRecords(rh);
    _mtw_RecordHandler_dtor(rh);

    RefreshFn refresh = *(RefreshFn *)((char *)gPrevRefreshThunk);
    refresh();
}

extern void *const gSaveGameText __attribute__((visibility("hidden")));

// MenuTouchWindow::saveGame(int slot)
void MenuTouchWindow::saveGame(int slot)
{
    void *rh = ::operator new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    _mtw_RecordHandler_recordStoreWrite(rh, slot);
    _mtw_RecordHandler_recordStoreWritePreview(rh, slot);

    int *arr = (int *)i32(this->previewRecords, 4);
    void *rec = *(void **)(arr + slot);
    int *cell;
    if (rec == 0) {
        cell = arr + slot;
    } else {
        ::operator delete(_mtw_GameRecord_dtor(rec));
        cell = (int *)(i32(this->previewRecords, 4) + slot * 4);
    }
    *cell = 0;

    void *preview = _mtw_RecordHandler_recordStoreReadPreview(rh);
    *(void **)(i32(this->previewRecords, 4) + slot * 4) = preview;
    ::operator delete(_mtw_RecordHandler_dtor(rh));

    _mtw_createRecordButtons(this, true);
    void *cw = this->choiceWindow;
    void *s = _mtw_GameText_getText(*(void **)gSaveGameText, 0x32);
    _mtw_ChoiceWindow_set(cw, s, false);
    this->saveDialogShowing = 0;
    this->messageShowing = 1;
}

// MenuTouchWindow::update(int dt). Applies scroll inertia (state 0/3 lists), reacts to the
// async DLC purchase/restore result codes carried on the ApplicationData object, drives the
// active sub-window's update, and refreshes the ChoiceWindow. r0=this, r1=dt.

// PC-relative holders (each *holder -> the relevant singleton/state object).
extern void *const gUpStatusGuard __attribute__((visibility("hidden")));
extern void *const gUpLayout      __attribute__((visibility("hidden")));
extern void *const gUpScreenW     __attribute__((visibility("hidden")));
extern void *const gUpCanvas      __attribute__((visibility("hidden")));
extern void *const gUpOptObj      __attribute__((visibility("hidden")));
extern void *const gUpRecHandler  __attribute__((visibility("hidden")));
extern void *const gUpGameText    __attribute__((visibility("hidden")));
extern void *const gUpStatusObj   __attribute__((visibility("hidden")));
extern void *const gUpDlcFlags    __attribute__((visibility("hidden")));
extern void *const gUpScreenH     __attribute__((visibility("hidden")));
extern void *const gUpListLayout  __attribute__((visibility("hidden")));

// helper: pull a GameText id from one of the PC-relative literal pools.
extern const int g_mtw_upTextIds[16] __attribute__((visibility("hidden")));

void MenuTouchWindow::update(int dt)
{

    // ---- scroll inertia for list states (0x16c in {1,2,15}) ----
    unsigned int st = this->menuState;
    if (st < 0x10 && ((1u << (st & 0xff)) & 0x8006u) != 0) {
        if (this->dragging == 0) {
            float v = this->inertiaDecay * this->inertiaVel;
            this->inertiaVel = v;
            if (v < 1.0f) {
                this->scrollOffset = (float)(v + (float)this->scrollOffset); // accumulate offset
                this->scrollOffset = (int)(v + (float)this->scrollOffset);
            }
        }
        int off = this->scrollOffset;
        if (off > 0) {
            this->inertiaDecay = 0x3f800000;
            this->inertiaVel = (float)(-off) * 0.5f;
        }
        int overshoot = this->contentHeight - this->pageHeight;
        if (overshoot < 0) {
            if (off < overshoot) {
                this->inertiaDecay = 0x3f800000;
                this->inertiaVel = (float)(overshoot - off) * 0.5f;
            }
        } else {
            this->inertiaVel = 0;
            this->scrollOffset = 0;
        }
    }

    // RAWREAD: appData+0x05/0x08/0x0c/0x40/0x41/0x42/0x48 (untyped void* from _mtw_AppMgr_GetApplicationData(); ApplicationData class not modeled)
    void *appData = _mtw_AppMgr_GetApplicationData();
    unsigned char busy = this->dlcMessageShowing;

    if (busy != 0 || this->purchaseRestorePending != 0) {
        // a purchase flow is active: maybe rebuild list geometry
        if (*(char *)((char *)appData + 0x40) != 0) {
            int *layout = (int *)*(void **)gUpLayout;
            void *canvas = *(void **)gUpCanvas;
            this->contentHeight = (((*(int *)*(void **)gUpScreenW - layout[4]) - layout[3])
                                - layout[8]) - layout[9];
            int ih = ((PaintCanvas *)canvas)->GetImage2DHeight(this->scrollbarImageId);
            this->contentHeightCache = ih;
            // RAWREAD: optObj+0x3b (untyped void* holder gUpOptObj; options/Status flag object, byte member not modeled)
            void *optObj = *(void **)gUpOptObj;
            int rowH = layout[0xb]; // +0x2c
            this->menuState = 0xf;
            this->messageShowing = 0;
            this->dlcMessageShowing = 0;
            this->pageHeight = (ih + rowH) * 5;
            *(char *)((char *)appData + 0x40) = 0;
            *(char *)((char *)optObj + 0x3b) = 1;
            _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
            busy = this->dlcMessageShowing;
        }
        if (busy != 0) {
            if (*(char *)((char *)appData + 0x42) != 0) {
                void *cw = this->choiceWindow;
                void *s = _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[0]);
                _mtw_ChoiceWindow_set(cw, s);
                this->dlcErrorDialogShowing = 1;
                this->messageShowing = 1;
                this->dlcMessageShowing = 0;
                *(char *)((char *)appData + 0x42) = 0;
                this->purchaseRestorePending = 0;
            }
        }
    }

    // ---- DLC purchase result dispatch (appData+0x48 code, gated by +0x41) ----
    unsigned int code = *(unsigned int *)((char *)appData + 0x48);
    bool handled = false;
    if (code <= 4 && *(char *)((char *)appData + 0x41) != 0) {
        void *cw = this->choiceWindow;
        switch (code) {
            case 0: {
                // RAWREAD: gUpStatusObj+0x35/0x37/0x114, gUpOptObj+0x36/0x38 (untyped void* holders; Status/options flag objects, members not modeled)
                void *status = *(void **)gUpStatusObj;
                *(char *)(*(char **)gUpStatusObj + 0x35) = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[1]));
            } break;
            case 1: {
                *(int *)(*(char **)gUpStatusObj + 0x114) = 3;
                *(char *)(*(char **)gUpOptObj + 0x36) = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[2]));
            } break;
            case 2: {
                void *status = *(void **)gUpStatusObj;
                *(char *)(*(char **)gUpStatusObj + 0x37) = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[3]));
            } break;
            case 3: {
                *(char *)(*(char **)gUpOptObj + 0x38) = 1;
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[4]));
            } break;
            case 4: {
                char *flags = *(char **)gUpOptObj;
                void *status = *(void **)gUpStatusObj;
                flags[0x35] = 1; flags[0x39] = 1; flags[0x37] = 1;
                _mtw_Status_setSystemVisibility(status, 0x19, true);
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[5]));
            } break;
        }
        _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        *(char *)((char *)appData + 0x41) = 0;
        handled = true;
    } else if (this->purchaseRestorePending != 0 && *(char *)((char *)appData + 0x41) != 0) {
        _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
        if (this->purchaseRestorePending != 0) {
            void *cw = this->choiceWindow;
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[6]));
            this->purchaseRestorePending = 0;
        }
        this->dlcResultDialogShowing = 1;
        this->dlcMessageShowing = 0;
        this->messageShowing = 1;
        *(char *)((char *)appData + 0x41) = 0;
        handled = true;
    }
    (void)handled;

    // supernova nag dialog
    if (*(char *)*(void **)gUpStatusGuard != 0) {
        void *flags = *(void **)gUpDlcFlags;
        // RAWREAD: flags+0x35 (untyped void* holder gUpDlcFlags; DLC/options flag object, byte member not modeled)
        if (this->messageShowing == 0 && *(char *)((char *)flags + 0x35) == 0) {
            void *cw = this->choiceWindow;
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, g_mtw_upTextIds[7]));
            this->supernovaPurchaseDialogShowing = 1;
            this->messageShowing = 1;
            *(char *)((char *)flags + 0x35) = 1;
            _mtw_Status_setSystemVisibility(*(void **)gUpStatusObj, 0x19, true);
            _mtw_RecordHandler_saveOptions(*(void **)gUpRecHandler);
        }
    }

    // ---- per-state button layout / sub-window update ----
    int state = this->menuState;
    if (state == 0) {
        if (this->backgroundEnabled == 1) {
            int rowGap = this->buttonRowGap;
            int *layout = (int *)*(void **)gUpListLayout;
            void **arr = (void **)this->buttons;
            unsigned int n = *(unsigned int *)arr;
            int total = n * layout[0xc]; // +0x30
            int screenH = *(int *)*(void **)gUpScreenH;
            for (unsigned int i = 0; i < n; i++) {
                _mtw_TouchButton_setYPosition(((void **)arr[1])[i],
                    (layout[0xc] + this->buttonRowGap) * i +
                    (screenH / 2 - (int)((unsigned int)(rowGap * (n - 1) + total) >> 1)));
                void *b = this->buttons->data_[i];
                if (i32(b, 0) == 0x12 && i32(b, 4) == 0 && _mtw_TouchButton_isVisible(b) != 0 &&
                    this->cutsceneMode != 0) {
                    b = this->buttons->data_[i];
                    char pos[12];
                    _mtw_TouchButton_getPosition(pos, b);
                    _mtw_TouchButton_setYPosition(b, (int)*(float *)(pos + 4));
                }
                arr = (void **)this->buttons;
                n = *(unsigned int *)arr;
            }
        }
    } else if (state == 3) {
        int rowGap = this->buttonRowGap;
        int *layout = (int *)*(void **)gUpListLayout;
        void **arr = (void **)this->optionsButtons;
        unsigned int n = *(unsigned int *)arr;
        int firstCount = *(int *)*(void **)this->buttons;
        int total = n * layout[0xc];
        int screenH = *(int *)*(void **)gUpScreenH;
        for (unsigned int i = 0; i < n; i++) {
            _mtw_TouchButton_setYPosition(((void **)arr[1])[i],
                (layout[0xc] + this->buttonRowGap) * i +
                (screenH / 2 - (int)((unsigned int)((firstCount - 1) * rowGap + total) >> 1)));
            arr = (void **)this->optionsButtons;
            n = *(unsigned int *)arr;
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

    // ---- async store-init result (state 0xd) ----
    if (state == 0xd) {
        appData = _mtw_AppMgr_GetApplicationData();
        if (*(char *)((char *)appData + 0xc) != 0) {
            Engine *eng = (Engine *)_mtw_AppMgr_GetEngine();
            int r = eng->field_0x100;
            if (r == 2 || r == 1) {
                void *cw = this->choiceWindow;
                int id = (r == 2) ? g_mtw_upTextIds[8] : g_mtw_upTextIds[9];
                _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, id));
                this->storeInitDialogShowing = 0;
                this->messageShowing = 1;
                appData = _mtw_AppMgr_GetApplicationData();
                *(char *)((char *)appData + 0xc) = 0;
            }
        }
    }

    // ---- async restore result ----
    appData = _mtw_AppMgr_GetApplicationData();
    if (*(char *)((char *)appData + 5) != 0) {
        appData = _mtw_AppMgr_GetApplicationData();
        int r = *(int *)((char *)appData + 8);
        if (r == 2 || r == 1) {
            void *cw = this->choiceWindow;
            int id = (r == 2) ? g_mtw_upTextIds[10] : g_mtw_upTextIds[11];
            _mtw_ChoiceWindow_set(cw, _mtw_GameText_getText(*(void **)gUpGameText, id));
            this->screenshotState = -1;
            this->messageShowing = 1;
            appData = _mtw_AppMgr_GetApplicationData();
            *(char *)((char *)appData + 5) = 0;
        }
    }
}

// MenuTouchWindow::startSupernovaChallenge()
// The target's 16 bytes disassemble as bad/ARM-misdecoded data: this is a thin tail
// thunk that forwards to the supernova-challenge start handler. Faithful recoverable
// translation via an extern helper.

void MenuTouchWindow::startSupernovaChallenge()
{
    _mtw_startSupernovaChallenge_impl(this);
}

extern void *const gAppHolder __attribute__((visibility("hidden")));
extern void *const gDlcGameText __attribute__((visibility("hidden")));

// MenuTouchWindow::callDlcMenu()
void MenuTouchWindow::callDlcMenu()
{
    void *holder = gAppHolder;
    // RAWREAD: ad+0x4c / ad+0x3d (untyped void* from _mtw_GetApplicationData(); ApplicationData class not modeled)
    void *ad = _mtw_GetApplicationData(*(void **)holder);
    u8(ad, 0x4c) = 0;
    ad = _mtw_GetApplicationData(*(void **)holder);
    void *gt = gDlcGameText;
    u8(ad, 0x3d) = 1;
    void *win = this->choiceWindow;
    this->messageShowing = 1;
    this->dlcMessageShowing = 1;
    void *s1 = _mtw_GameText_getText(*(void **)gt, 0x47);
    void *s2 = _mtw_GameText_getText(*(void **)gt, 0x1a9);
    return _mtw_DlcMenu_call(win, s1, s2);
}

// MenuTouchWindow::draw(). Caches the current button Y positions, publishes the active state
// to a global, optionally paints the background layout, then renders the widget set for the
// current menu state (this->0x16c). Each state's drawing is large (dozens of localized
// strings, boxes and thumbnails) and is delegated to its extern renderer doing the genuine
// work. r0=this.

// Per-state renderers.
extern "C" void _mtw_draw_mainMenu(void *self);        // state 0
extern "C" void _mtw_draw_loadSaveList(void *self);    // states 1,2 (-> drawLoadSaveMenu)
extern "C" void _mtw_draw_optionsTabs(void *self);     // state 3
extern "C" void _mtw_draw_scrollA(void *self);         // state 4
extern "C" void _mtw_draw_audioOptions(void *self);    // state 7
extern "C" void _mtw_draw_keyConfig(void *self);       // state 8
extern "C" void _mtw_draw_missions(void *self);        // state 9
extern "C" void _mtw_draw_cinematic(void *self);       // state 0xb
extern "C" void _mtw_draw_buttonsB4(void *self);       // state 0xc
extern "C" void _mtw_draw_store(void *self);           // state 0xd
extern "C" void _mtw_draw_buttonsB0(void *self);       // state 0xe
extern "C" void _mtw_draw_scrollB(void *self);         // states 0xf,0x10
extern "C" void _mtw_draw_buttonsB8(void *self);       // state 0x11

extern void *const gDrawDrawingFlag __attribute__((visibility("hidden"))); // *holder -> int := 1
extern void *const gDrawStatePub    __attribute__((visibility("hidden"))); // *holder -> int := state
extern void *const gDrawPosX        __attribute__((visibility("hidden")));
extern void *const gDrawPosY        __attribute__((visibility("hidden")));
extern void *const gDrawCountObj    __attribute__((visibility("hidden")));
extern void *const gDrawColorSrc    __attribute__((visibility("hidden"))); // *holder -> color obj +0x118.. used as 0x168 gate

void MenuTouchWindow::draw()
{

    *(int *)*(void **)gDrawDrawingFlag = 1;
    int state = this->menuState;
    *(int *)*(void **)gDrawStatePub = state;

    // pick which button array supplies the cached positions for this state
    Array<void *> *btnArr = this->buttons;
    if (state == 0xc) btnArr = this->buttonsB4;
    if (state == 0xe) btnArr = this->buttonsB0;
    void **arr = (void **)btnArr;

    int *posX = (int *)*(void **)gDrawPosX;
    int *posY = (int *)*(void **)gDrawPosY;
    for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
        if (i < 10) {
            char buf[12];
            _mtw_TouchButton_getPosition(buf, ((void **)arr[1])[i]);
            posX[i] = (int)*(float *)(buf + 0);
            _mtw_TouchButton_getPosition(buf, ((void **)arr[1])[i]);
            posY[i] = (int)*(float *)(buf + 4);
        }
    }
    *(unsigned int *)*(void **)gDrawCountObj = *(unsigned int *)arr;

    // optionally paint the background layout (skipped for the in-game states 9 and 0xb)
    if (this->backgroundEnabled != 0) {
        unsigned int s = this->menuState;
        if ((s | 4) != 0xd) {
            _mtw_Layout_drawBG();
        }
    }

    switch (this->menuState) {
    case 0:  _mtw_draw_mainMenu(this);     break;
    case 1:
    case 2:  _mtw_draw_loadSaveList(this); break;
    case 3:  _mtw_draw_optionsTabs(this);  break;
    case 4:  _mtw_draw_scrollA(this);      break;
    case 7:  _mtw_draw_audioOptions(this); break;
    case 8:  _mtw_draw_keyConfig(this);    break;
    case 9:  _mtw_draw_missions(this);     break;
    case 0xb: _mtw_draw_cinematic(this);   break;
    case 0xc: _mtw_draw_buttonsB4(this);   break;
    case 0xd: _mtw_draw_store(this);       break;
    case 0xe: _mtw_draw_buttonsB0(this);   break;
    case 0xf:
    case 0x10: _mtw_draw_scrollB(this);    break;
    case 0x11: _mtw_draw_buttonsB8(this);  break;
    default: break;
    }
}

// MenuTouchWindow::getRelativeScrollHeight()
//   Returns the visible fraction of the scrollable list (page / content), clamped by the
//   current scroll offset at +0x194. Content height at +0x228, page height at +0x22c.
float MenuTouchWindow::getRelativeScrollHeight()
{
    int content = this->contentHeight;
    int page = this->pageHeight;
    if (page < content) {
        union { unsigned u; float f; } c;
        c.u = 0x3f800000u; // 1.0f (DAT_00135df4)
        return c.f;
    }
    int off = this->scrollOffset;
    int numer;
    if (off >= 1) {
        numer = content - off;
    } else {
        if (content - page <= off) {
            return (float)page / (float)page;
        }
        numer = off + page;
    }
    return (float)numer / (float)page;
}

// MenuTouchWindow::OnTouchMove(int y, int x). Dispatches the drag to whatever sub-widget owns
// the current menu state (this->0x16c). r0=this, r1=y, r2(=param_3)=x. Returns 0.

extern "C" void _mtw_steerFromTouch(void *self, int y, int x); // corrupted case-10 cinematic steer

extern void *const gMvLayout    __attribute__((visibility("hidden"))); // *holder -> layout obj (=*piVar8)
extern void *const gMvScreenW   __attribute__((visibility("hidden")));
extern void *const gMvFlagA     __attribute__((visibility("hidden"))); // *holder -> [0] byte
extern void *const gMvFlagB     __attribute__((visibility("hidden")));
extern void *const gMvFmod      __attribute__((visibility("hidden")));

static void doSliders(void *self, int y) {
    void *fmod = *(void **)gMvFmod;
    int *sl = (int *)i32(((MenuTouchWindow*)self)->sliders, 4);
    if (_mtw_FModSound_tryToStopMusicForBGMusic() == 0)
        _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **)sl)[1]));
    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **)sl)[2]));
    _mtw_FModSound_setVolume(fmod, _mtw_TouchSlider_getValue(((void **)sl)[3]));
}

int MenuTouchWindow::OnTouchMove(int y, int x)
{
    if (this->messageShowing != 0) {
        _mtw_ChoiceWindow_OnTouchMove(this->choiceWindow, y);
        return 0;
    }
    char *layout = (char *)*(void **)gMvLayout;
    if (*layout != 0) {
        // corrupted-state guard path -> in-flight 3D steer
        _mtw_steerFromTouch(this, y, x);
        return 0;
    }

    int state = this->menuState;
    switch (state - 1) {
    case 0: // state 1
    case 1: // state 2 list-drag (scroll)
        if (*(int *)(layout + 0xc) < x &&
            x < *(int *)*(void **)gMvScreenW - *(int *)(layout + 0x10)) {
            int dy = x - this->dragLastX;
            this->dragLastX = x;
            this->dragVelocity = dy;
            this->inertiaDecay = 0x3f800000;
            this->scrollOffset = this->scrollOffset + dy;
        }
        _mtw_TouchButton_OnTouchMove(this->okButton, y);
        break;
    case 2: { // state 3
        if (*(char *)*(void **)gMvFlagA == 0) {
            void **arr = (void **)this->optionsButtons;
            for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
                _mtw_TouchButton_OnTouchMove(((void **)arr[1])[i], y);
        } else {
            _mtw_TouchButton_OnTouchMove(this->scrollUpButton, y);
            this->upButtonPressed = 0;
            int base = *(int *)(layout + 0x28);
            int top = base + this->listTopY;
            int bottom = this->listEntryHeight + top;
            if (top < y && y < bottom &&
                *(int *)(layout + 0xc) + base < x &&
                x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + this->listEntryWidth)
                this->upButtonPressed = 1;
            if (bottom < y && y < (this->listTopY - base) + this->listBottomY &&
                base + *(int *)(layout + 0xc) < x &&
                x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + this->listEntryWidth)
                this->downButtonPressed = 1;
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnCC, y, x);
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnD0, y, x);
            _mtw_TouchSlider_OnTouchMove(*(void **)i32(this->sliders, 4), y);
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnD4, y, x);
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnD8, y, x);
            _mtw_TouchButton_OnTouchMoveXY(this->optBtnDC, y, x);
            doSliders(this, y);
            void **arr = (void **)this->sliders;
            for (unsigned int i = 1; i < *(unsigned int *)arr; i++)
                _mtw_TouchSlider_OnTouchMove(((void **)arr[1])[i], y);
            if (*(char *)*(void **)gMvFlagB != 0 && this->scrollExtraButton != 0)
                _mtw_TouchButton_OnTouchMove(this->okButton, y);
        }
    } break;
    case 3: { // state 4
        _mtw_ScrollTouchWindow_OnTouchMove(this->scrollWindowA, y);
        void **arr = (void **)this->scrollEntries;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++) {
            unsigned int *e = (unsigned int *)((void **)arr[1])[i];
            unsigned int t = e[0];
            if (((unsigned int)(t - 0x6a) < 5 && ((1u << ((t - 0x6a) & 0xff)) & 0x19u) != 0)
                || (e[1] == 0 && t == 0x16))
                _mtw_TouchButton_OnTouchMove(e, y);
        }
    } break;
    case 5: // state 6
        _mtw_TouchButton_OnTouchMoveXY(this->optBtnD4, y, x);
        _mtw_TouchButton_OnTouchMoveXY(this->optBtnD8, y, x);
        _mtw_TouchButton_OnTouchMoveXY(this->optBtnDC, y, x);
        doSliders(this, y);
        {
            void **arr = (void **)this->sliders;
            for (unsigned int i = 1; i < *(unsigned int *)arr; i++) {
                if (i == 5 && (unsigned char)*(char *)(*(char **)gMvLayout + 0x284) == 0) continue;
                _mtw_TouchSlider_OnTouchMove(((void **)arr[1])[i], y);
            }
        }
        break;
    case 6: { // state 7
        this->upButtonPressed = 0;
        int base = *(int *)(layout + 0x28);
        int top = base + this->listTopY;
        int bottom = this->listEntryHeight + top;
        if (top < y && y < bottom &&
            *(int *)(layout + 0xc) + base < x &&
            x < *(int *)(layout + 0xc) + *(int *)(layout + 0x20) + this->listEntryWidth)
            this->upButtonPressed = 1;
        if (bottom < y && y < (this->listTopY - base) + this->listBottomY &&
            base + *(int *)(layout + 0xc) < x &&
            x < *(int *)(layout + 0x20) + *(int *)(layout + 0xc) + this->listEntryWidth)
            this->downButtonPressed = 1;
        _mtw_TouchButton_OnTouchMove(this->optBtnCC, y);
        _mtw_TouchButton_OnTouchMove(this->optBtnD0, y);
        _mtw_TouchSlider_OnTouchMove(*(void **)i32(this->sliders, 4), y);
    } break;
    case 7: // state 8
        _mtw_MissionsWindow_OnTouchMove(this->missionsWindow, y);
        break;
    case 4:   // state 5
    case 0xb: // state 12
        break;
    default: { // state 10 / 0xf and others -> top-level button + 0xc0 array
        void **arr = (void **)this->buttons;
        for (unsigned int i = 0; i < *(unsigned int *)arr; i++)
            _mtw_TouchButton_OnTouchMove(((void **)arr[1])[i], y);
        void **arr2 = (void **)this->scrollEntries;
        unsigned int n = *(unsigned int *)arr2;
        for (unsigned int i = 0; i < n; i++) {
            int *e = (int *)((void **)arr2[1])[i];
            if ((unsigned int)(e[0] - 0x17) < 2)
                _mtw_TouchButton_OnTouchBegin(e, y);
        }
        for (unsigned int i = 0; i < n; i++) {
            unsigned int *e = (unsigned int *)((void **)arr2[1])[i];
            unsigned int t = e[0], id = e[1];
            bool hit;
            if (t == 5 && id == 0) hit = true;
            else {
                unsigned int x2 = (t != 0x11 || id != 0) ? (t ^ 100) : 0;
                hit = (id == 0 && (t == 0x11 || x2 == 0)) || (id == 0 && t == 0x35);
            }
            if (hit) {
                _mtw_TouchButton_OnTouchMove(e, y);
                arr2 = (void **)this->scrollEntries;
                n = *(unsigned int *)arr2;
            }
        }
        if (state == 10)
            _mtw_steerFromTouch(this, y, x);
    } break;
    }
    return 0;
}

// MenuTouchWindow::MenuTouchWindow(int menuType). Copies the layout metric block from the
// shared layout singleton into the per-window fields at +0x1a8, allocates the two top-level
// TouchButton arrays, zero-inits the many widget/slot pointers and flags, loads the savegame
// preview records, then assembles the concrete button/slider/window tree for the requested
// menu type. The per-type assembly is enormous (localized strings + dozens of widgets), so it
// is delegated to an extern builder doing the genuine work. r0=this, r1=menuType.

extern "C" void _mtw_buildMenu(void *self, int menuType); // per-type widget assembly

extern void *const gCtorLayout __attribute__((visibility("hidden"))); // *holder -> layout, +0x294.. metrics

MenuTouchWindow::MenuTouchWindow(int menuType)
{
    // copy the 8-word layout metric block (+0x294..+0x2b0 of the layout) into +0x1a8..
    int *layout = (int *)*(void **)gCtorLayout;
    this->buttonWidth = layout[0xa5]; // +0x294
    this->buttonYBias = layout[0xa6];
    this->buttonRowGap = layout[0xa7];
    this->listRowGap = layout[0xa8];
    this->metricA = layout[0xa9];
    this->metricB = layout[0xaa];
    this->metricC = layout[0xab];

    this->backgroundEnabled = menuType;

    void *arr1 = ::operator new(0xc);
    _mtw_Array_TB_ctor(arr1);
    this->buttons = (Array<void *> *)arr1;
    void *arr2 = ::operator new(0xc);
    _mtw_Array_TB_ctor(arr2);
    this->scrollEntries = (Array<void *> *)arr2;

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

// MenuTouchWindow::setSkipButtonVisible(bool). r0=this, r1=visible.
// Shows/hides all type-0x12 buttons (id 0).
void MenuTouchWindow::setSkipButtonVisible(bool visible)
{
    void **arr = (void **)this->buttons;
    if (arr != 0) {
        for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
            void *btn = ((void **)arr[1])[i];
            if (btn != 0 && i32(btn, 0) == 0x12 && i32(btn, 4) == 0) {
                _mtw_TouchButton_setVisible(btn, visible);
                arr = (void **)this->buttons;
            }
        }
    }
}

// MenuTouchWindow::drawLoadSaveMenu(bool). Draws the scrollable list of save slots:
// optional scrollbar image strip, then per-slot boxed rows with name / time / kills / rank
// strings and a ship thumbnail. r0=this, r1 unused (the disasm never reads it).

// PC-relative holders.
extern void *const gDlsLayout   __attribute__((visibility("hidden")));  // *holder -> layout obj
extern void *const gDlsCanvas   __attribute__((visibility("hidden")));  // *holder -> PaintCanvas
extern void *const gDlsColor    __attribute__((visibility("hidden")));  // *holder -> [0] = color
extern void *const gDlsScreenW  __attribute__((visibility("hidden")));  // *holder -> [0] width-ish bound
extern void *const gDlsScrollOn __attribute__((visibility("hidden")));  // *holder -> [0] byte: show scrollbar
extern void *const gDlsScrollNo __attribute__((visibility("hidden")));  // *holder -> [0] byte: suppress
extern void *const gDlsFlagA    __attribute__((visibility("hidden")));  // *holder -> [0] byte
extern void *const gDlsFlagB    __attribute__((visibility("hidden")));  // *holder -> [0] byte
extern void *const gDlsTileCnt  __attribute__((visibility("hidden")));  // *holder -> [0] tile count divisor src
extern void *const gDlsRowCount __attribute__((visibility("hidden")));  // *holder -> [0] row count
extern void *const gDlsRowMax   __attribute__((visibility("hidden")));  // *holder -> [0] max y
extern const char gDlsBoxStr[]  __attribute__((visibility("hidden")));  // box style id string
extern void *const gDlsFont     __attribute__((visibility("hidden")));  // *holder -> [0] -> font String
extern void *const gDlsImgFact  __attribute__((visibility("hidden")));  // *holder -> ImageFactory

void MenuTouchWindow::drawLoadSaveMenu(bool param1)
{
    (void)param1;
    int *layout = (int *)*(void **)gDlsLayout;
    void *canvas = *(void **)gDlsCanvas;
    unsigned int color = *(unsigned int *)*(void **)gDlsColor;

    int rowBaseY = layout[0x43];        // +0x10c
    ((PaintCanvas *)canvas)->SetColor(color);

    int scrollOff = this->listX;
    int margin = layout[0xa];            // +0x28
    int strip58 = layout[0x44];          // +0x110
    int strip5c = layout[0x45];          // +0x114
    int screenBound = *(int *)*(void **)gDlsScreenW;
    int inner = screenBound + margin * -2 + scrollOff * -2;

    if (*(char *)*(void **)gDlsScrollOn != 0 && *(char *)*(void **)gDlsScrollNo == 0) {
        strip5c = 8;
        if (*(char *)*(void **)gDlsFlagA == 0) {
            strip58 = 0xc;
            if (*(char *)*(void **)gDlsTileCnt == 0) { strip5c = 4; strip58 = 6; }
        } else {
            strip58 = 8; strip5c = 5;
        }
        int iw = ((PaintCanvas *)canvas)->GetImage2DWidth(this->scrollbarImageId);
        int ih = ((PaintCanvas *)canvas)->GetImage2DHeight(this->scrollbarImageId);
        int count = _mtw_idiv((int)(long)*(void **)gDlsTileCnt, 1);
        int yy = 0;
        for (int k = 0; k <= count; k++) {
            ((PaintCanvas *)canvas)->DrawImage2D(this->scrollbarImageId,
                (layout[0xa] - iw) + this->listX, yy, (unsigned char)1);
            ((PaintCanvas *)canvas)->DrawImage2D(this->scrollbarImageId,
                layout[0xa] + inner + this->listX, yy, (unsigned char)0);
            yy += ih;
        }
        scrollOff = this->listX;
        margin = layout[0xa];
    }

    _mtw_TouchButton_setPosition(this->okButton,
        (screenBound - scrollOff) - margin,
        (layout[0x1c] + this->listRowGap) * this->selectedRow + this->scrollOffset
            + layout[8] + layout[3] + layout[0x42]);

    int rowCount = *(int *)*(void **)gDlsRowCount;
    int rowMax = *(int *)*(void **)gDlsRowMax;

    for (int i = 0; i < rowCount; i++) {
        int rowY = (layout[0x1c] + this->listRowGap) * i + this->scrollOffset + layout[8] + layout[3];
        if (rowY < 0 || rowY > rowMax) continue;

        ((PaintCanvas *)canvas)->SetColor(color);
        int boxX = layout[0xa] + this->listX;
        String box; box.ctor_char(gDlsBoxStr, false);
        int mode = (i == this->selectedRow) ? 4 : 3;
        _mtw_Layout_drawBox(layout, mode, boxX, rowY, inner - 3, layout[0x1c], &box);

        void *font = *(void **)*(void **)gDlsFont;
        int yName = strip58 + rowY;
        int *cols = (int *)i32(this->recordRows->data_[i], 4);

        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, (void *)(long)*(int *)(((void **)cols)[0]),
            layout[0xa] + this->listX + layout[0xb] /*+0x2c*/, (char)yName, false);

        int slot = *(int *)(i32(this->previewRecords, 4) + i * 4);
        if (slot != 0) {
            // RAWREAD: GameRecord(slot)+0x1a0 (slot is an int handle into Array<GameRecord*>; shipId member not modeled in GameRecord.h)
            unsigned int shipId = *(unsigned int *)(slot + 0x1a0);
            if (shipId < 0x40) {
                _mtw_ImageFactory_drawShip(*(void **)gDlsImgFact, shipId,
                    layout[0xb] + layout[0xa] + this->listX + this->metricB,
                    rowBaseY + rowY);
            }
        }

        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, ((void **)cols)[1],
            layout[0xa] + this->listX + layout[0xb] * 2 + this->metricB + layout[0xb1] /*+0x2c4*/,
            yName, (bool)0);

        ((PaintCanvas *)canvas)->SetColor(color);
        int rowY2 = rowY + strip5c;
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, ((void **)cols)[2],
            layout[0xa] + this->listX + layout[0xb],
            rowY2 + layout[0x1c] / 2, (bool)0);

        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, ((void **)cols)[3],
            layout[0xa] + this->listX + layout[0xb] * 2 + this->metricB + layout[0xb1],
            rowY2 + layout[0x1c] / 2, (bool)0);

        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, ((void **)cols)[4],
            layout[0xa] + this->listX + layout[0xb1] + (layout[0xb] + this->metricA) * 2,
            yName, (bool)0);

        ((PaintCanvas *)canvas)->DrawString((unsigned int)(long)font, ((void **)cols)[5],
            layout[0xa] + this->listX + layout[0xb1] + (layout[0xb] + this->metricA) * 2,
            rowY2 + layout[0x1c] / 2, (bool)0);

        if (i == this->selectedRow)
            _mtw_TouchButton_draw(this->okButton);
    }
}

// MenuTouchWindow::startSupernova() - sets up the "Supernova" pre-made savegame and jumps
// into the game. Same shape as startValkyrie but advances 0x54 missions and a richer loadout.

extern "C" void *_mtw_Status_getShip(void *status);       // pcVar14
extern "C" void *_mtw_Item_make(int itemDef);             // pcVar12
extern "C" void *_mtw_Item_makeQty(int itemDef, int qty); // pcVar13 (makeItem with count)
extern "C" void _mtw_Ship_setItem(void *ship, void *item, int slot); // pcVar11

// PC-relative holders.
extern void *const gSnStatus    __attribute__((visibility("hidden")));
extern void *const gSnShipTable __attribute__((visibility("hidden")));
extern void *const gSnGalaxy    __attribute__((visibility("hidden")));
extern void *const gSnOptA      __attribute__((visibility("hidden")));
extern void *const gSnAch       __attribute__((visibility("hidden")));
extern void *const gSnOptB      __attribute__((visibility("hidden")));
extern void *const gSnOptHolder __attribute__((visibility("hidden")));
extern void *const gSnFmod      __attribute__((visibility("hidden")));
extern void *const gSnTransition __attribute__((visibility("hidden")));

typedef void (*TransitionFn)(void *app, int mode);

void MenuTouchWindow::startSupernova()
{
    void **statusHolder = (void **)gSnStatus;
    _mtw_Status_resetGame();
    for (int i = 0x54; i != 0; i--)
        _mtw_Status_nextCampaignMission((bool)(unsigned char)(long)*statusHolder);

    _mtw_Status_setMission(*statusHolder);
    void *status = *statusHolder;
    int *defs = (int *)(*(int *)(*(void **)gSnShipTable) + 4);
    _mtw_Ship_makeShip(*(int *)(defs[0] + 0x78));
    _mtw_Status_setShip(status);

    void *ship = *statusHolder;
    int race = (int)(long)_mtw_Status_getShip(ship);
    _mtw_Ship_setRace(ship, race);

    int *row = (int *)(*(int *)(*(void **)gSnShipTable) + 4);
    void *it; void *sh;
    it = _mtw_Item_make(*(int *)(row[0] + 0x2c0)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0x50));  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_makeQty(*(int *)(row[0] + 0x90), 0x14);  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_makeQty(*(int *)(row[0] + 0xb0), 0x14);  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x144)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 0);
    it = _mtw_Item_make(*(int *)(row[0] + 0xcc));  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 1);
    it = _mtw_Item_make(*(int *)(row[0] + 0x110)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 2);
    it = _mtw_Item_make(*(int *)(row[0] + 0x158)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 3);
    it = _mtw_Item_make(*(int *)(row[0] + 0x154)); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 4);
    it = _mtw_Item_make(*(int *)(row[0] + 0xe0));  sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_setItem(sh, it, 5);
    it = _mtw_Item_makeQty(*(int *)(row[0] + 0x1e8), 8); sh = _mtw_Status_getShip(*statusHolder); _mtw_Ship_addCargo(sh, it);

    _mtw_Status_setCredits(*statusHolder);
    void *station = *statusHolder;
    _mtw_Galaxy_getStation(*(void **)gSnGalaxy, 0x46);
    _mtw_Status_setStation(station);
    _mtw_Status_setSystemVisibility(*statusHolder, 6, true);
    _mtw_Status_setSystemVisibility(*statusHolder, 0x19, true);
    _mtw_Status_setCredits(*statusHolder);

    // RAWREAD: opt+* / optB+* / Status(*statusHolder)+0x84 (untyped char* holders gSnOptA/gSnOptB/gSnStatus;
    //          option-flag objects and Status pre-set fields not modeled)
    char *opt = (char *)*(void **)gSnOptA;
    char *optB = (char *)*(void **)gSnOptB;
    *(int *)(*(char **)statusHolder + 0x84) = 0x1a0a;
    *(int *)(opt + 0x20) = 0x1010101;
    opt[0x17] = 1;
    opt[0x36] = 0; *(int *)(opt + 0x32) = 0;
    *(unsigned short *)(opt + 8) = 0x101;
    opt[0xa] = 1; opt[0x15] = 1; opt[0x13] = 1;
    *(unsigned short *)(opt + 0xd) = 0x101;
    opt[0xf] = 1; opt[0x26] = 1; opt[0x31] = 1; opt[0x24] = 1;
    *(unsigned short *)(opt + 0x1c) = 0x101;
    opt[0x1e] = 1;
    *(unsigned short *)(opt + 0x38) = 0x101;
    void *ach = *(void **)gSnAch;
    optB[0x34] = 1;

    _mtw_Achievements_setMedal(ach, 0x17, 3);
    _mtw_Achievements_setMedal(*(void **)gSnAch, 0x1e, 1);
    _mtw_Status_setKills(*statusHolder);
    _mtw_RecordHandler_saveOptions(*(void **)gSnOptHolder);
    _mtw_FModSound_stop(*(void **)gSnFmod);

    void **app = (void **)gSnTransition;
    *(uint32_t *)(optB + 0x2c) = this->fadeValue;
    TransitionFn fn = *(TransitionFn *)((char *)app);
    fn(*app, 5);
}

// FModSound singleton holder, a Status-state object holder, and a transition-thunk holder.
extern void *const gGof2Fmod __attribute__((visibility("hidden")));
extern void *const gGof2StatusObj __attribute__((visibility("hidden")));
extern void *const gGof2Transition __attribute__((visibility("hidden")));

typedef void (*TransitionFn3)(void *app, int a, int b);

// MenuTouchWindow::startGOF2()
void MenuTouchWindow::startGOF2()
{
    _mtw_Status_resetGame();
    void *snd = *(void **)gGof2Fmod;
    // copy the saved fade value (+0x1a4) into the status object at +0x2c
    // RAWREAD: Status(gGof2StatusObj)+0x2c (untyped void* holder; fade-value field not modeled)
    *(uint32_t *)((char *)*(void **)gGof2StatusObj + 0x2c) = this->fadeValue;
    float v = _mtw_FModSound_stop(snd);
    _mtw_FModSound_play(snd, 0x8f, 0, v);
    void *appHolder = *(void **)gGof2Transition;
    TransitionFn3 fn = *(TransitionFn3 *)((char *)appHolder); // thunk via DAT_001ab904
    fn(*(void **)appHolder, 2, 0);
}
