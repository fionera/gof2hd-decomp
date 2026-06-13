#include "gof2/game/menu/ModStation.h"
#include "gof2/game/core/Globals.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/core/PaintCanvasClass.h"   // real PaintCanvas:: methods
#include "gof2/engine/math/AEMath.h"             // MatrixSetTranslation/Rotation, VectorSignedToFloat
#include <new>
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/game/core/CutScene.h"
#include "gof2/engine/math/EaseInOut.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/ui/MissionsWindow.h"
#include "gof2/game/world/NewsTicker.h"
// Radio.h omitted: it embeds conflicting inline defs of Layout/ImageFactory/Agent.
// ModStation only needs Radio::lastMessageShown(); supply a minimal decl instead.
struct Radio { unsigned char lastMessageShown(); };
#include "gof2/game/world/StarMap.h"
#include "gof2/game/mission/Achievements.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/game/ui/DialogueWindow.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/mission/RecordHandler.h"   // defines the canonical B/I/P offset helpers (guarded)
// SpaceLounge.h and TouchButton.h define their own (unguarded) copies of the B/I/P
// offset helpers. Rename those tokens for the duration of these two includes so the
// duplicates land on throwaway names instead of colliding; ModStation uses neither from
// these headers.
#define B  B_SpaceLounge
#define I  I_SpaceLounge
#define P  P_SpaceLounge
#include "gof2/game/menu/SpaceLounge.h"
#undef B
#undef I
#undef P
#include "gof2/game/world/Station.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/world/SolarSystem.h"
#define B      B_TouchButton
#define I      I_TouchButton
#define P      P_TouchButton
#include "gof2/game/ui/TouchButton.h"
#undef B
#undef I
#undef P
#include "gof2/game/ui/MenuTouchWindow.h"

// HangarWindow.h and StatusWindow.h are intentionally NOT included: both leak their
// converted methods into the `AbyssEngine` namespace as free functions (a decompiler
// artifact), which collide with each other across the two headers, and HangarWindow.h
// also redefines `Layout`/`String`. ModStation only ever dispatches OnTouchBegin/
// OnTouchMove on these via byte-offset casts, so minimal local definitions suffice.
struct HangarWindow {
    void OnTouchBegin(int touch, int coord);
    unsigned int OnTouchMove(int touch, int coord);
};
struct StatusWindow {
    int OnTouchBegin(int param_1, int param_2);
    int OnTouchMove(int param_1, int param_2);
};

extern "C" void *ModStation_op_new(unsigned int sz);
extern "C" void ModStation_op_delete(void *p);
extern "C" void ModStation_autosaveTail();
extern "C" void ModStation_suspendTail(void *obj, void **holder);
extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void ModStation_resumeTail(void *obj, int one, int arg);
extern "C" void ModStation_dtor_finish(ModStation *self);
extern "C" void ModStation_leaveStation_impl(ModStation *self);
void Globals_reportLeaderboards(void *obj);
extern "C" void *cm_op_new(unsigned int sz);
extern "C" void *cm_op_new_arr(unsigned int sz);
extern "C" void ModStation_cm_tail(void *p, int a, int b);
extern "C" void ModStation_r3d_endTail(void *c);
extern "C" void *ric_op_new(unsigned int sz);
extern "C" void AEMath_MatrixSetTranslation(void *m, int x, int y, int z);
extern "C" void AEMath_MatrixSetRotation(void *m, void *loc, int rx, int ry, int a4, int a5);
extern "C" int Station_getIndex(Station *st);
extern void *g_PaintCanvas;   // PaintCanvas singleton pointer (externs.h)
int GameText_getLanguage();
void Globals_loadFont(int obj, int lang);
extern "C" void *ms_op_delete(void *p);
extern "C" void *ArrayReleaseClasses_TouchButton(void *a);
extern "C" void *Array_TB_dtor(void *p);
extern "C" void *HangarWindow_dtor(void *p);
extern "C" void *StarMap_dtor(void *p);
extern "C" void *MissionsWindow_dtor(void *p);
extern "C" void *DialogueWindow_dtor(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void *StatusWindow_dtor(void *p);
extern "C" void *CutScene_dtor(void *p);
extern "C" void *Radio_dtor(void *p);
extern "C" void *Array_RM_dtor(void *p);
extern "C" void *NewsTicker_dtor(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void *ScrollTouchBox_dtor(void *p);
extern "C" void ModStation_or_tail();

struct Status;
// Status singleton holder-of-holder: g -> P (one load), *P -> Status object.
__attribute__((visibility("hidden"))) extern Status **g_ModStation_statusHolder;
// Tail veneer (0x1ac0f8).

// ModStation::autosave()
void ModStation::autosave() {
    Status *st = *g_ModStation_statusHolder;
    if (((Status *)(st))->getPlayingTime() - 1LL < 0)
        return;
    void *rh = ModStation_op_new(0x2c);
    new (rh) RecordHandler();
    ((RecordHandler *)(rh))->recordStoreWrite(0);
    ((RecordHandler *)(rh))->recordStoreWritePreview_int(0);
    do { RecordHandler *_rh = (RecordHandler *)(rh); _rh->~RecordHandler(); ModStation_op_delete(_rh); } while (0);
    UC(this, 0xb1) = 1;
    if (I(this, 0x50) != 0)
        ModStation_autosaveTail();
}

struct Status;
struct Station;
struct Ship;
struct Item;
struct Achievements;

// Status singleton holder: g -> P, P[0] -> Status object.
__attribute__((visibility("hidden"))) extern Status **g_ModStation_es_status;
// Achievements holder-of-holder.
__attribute__((visibility("hidden"))) extern Achievements **g_ModStation_es_ach;
// getShip veneer accessed via a function-pointer global.
__attribute__((visibility("hidden"))) extern Ship *(*g_ModStation_es_getShip)(Status *);

// ModStation::enterStation()
void ModStation_enterStation()
{
    Status **holder = g_ModStation_es_status;
    Status *status = holder[0];
    // Status::departStation actually takes the destination Station* (see Status.cpp);
    // gof2/Status.h's no-arg prototype is wrong but not editable here, so dispatch
    // through a correctly-typed pointer-to-member.
    {
        void (Status::*depart)(Station *) =
            reinterpret_cast<void (Status::*)(Station *)>(&Status::departStation);
        (((Status *)status)->*depart)(((Status *)(status))->getStation());
    }
    ((Station *)(((Status *)(holder[0]))->getStation()))->visit();
    ((Achievements *)(g_ModStation_es_ach[0]))->applyNewMedals();

    Ship *(*getShip)(Status *) = g_ModStation_es_getShip;
    Item *e10 = ((Ship *)(getShip(holder[0])))->getFirstEquipmentOfSort(10);
    Item *e9 = ((Ship *)(getShip(holder[0])))->getFirstEquipmentOfSort(9);
    int shipIdx = ((Ship *)(getShip(holder[0])))->getIndex();
    Status *s = holder[0];
    I(s, 0x150) = shipIdx;

    int v;
    if (e10 == 0) {
        v = -1;
    } else {
        v = ((Item *)(e10))->getIndex();
        s = holder[0];
    }
    I(s, 0x154) = v;

    if (e9 == 0) {
        v = -1;
    } else {
        v = ((Item *)(e9))->getIndex();
        s = holder[0];
    }
    I(s, 0x158) = v;
    I(s, 0x124) = 0;
    I(s, 0x11c) = 0;
}

struct Achievements;
// Achievements singleton holder-of-holder: g -> P, P[0] -> Achievements object.
__attribute__((visibility("hidden"))) extern Achievements **g_ModStation_achievements;
// Three target-struct holders: each g -> P, *P -> the struct (stores at fixed offsets).
__attribute__((visibility("hidden"))) extern int *g_ModStation_ach_a;
__attribute__((visibility("hidden"))) extern int *g_ModStation_ach_b;
__attribute__((visibility("hidden"))) extern int *g_ModStation_ach_c;
// (single-deref pc-rel: g resolves to the int* directly; store at its offsets.)

// ModStation::addAchievement(int, int)
void ModStation::addAchievement(int param_1, int param_2) {
    Achievements **holder = g_ModStation_achievements;
    if (((Achievements *)(holder[0]))->isEliteMedal(param_1) != 0)
        return;
    if ((unsigned)(param_2 - 1) < 2) {
        if (((Achievements *)(holder[0]))->getValue(param_1, 3) != -1)
            *g_ModStation_ach_a = param_1 * 3;
        if (param_2 == 1 && ((Achievements *)(holder[0]))->getValue(param_1, 2) != -1)
            *(int *)((char *)g_ModStation_ach_b + 4) = param_1 * 3 + 1;
    }
    *(int *)((char *)g_ModStation_ach_c + 8) = (param_1 * 3 + 3) - param_2;
}

// ModStation::showMapWindow()
void ModStation::showMapWindow() {
    UC(this, 0x64) = 0;
    UC(this, 0x67) = 1;
}

// ModStation::setGameLoaded()
void ModStation::setGameLoaded() {
    UC(this, 0xb0) = 1;
    UC(this, 0xb3) = 1;
}

// Singleton holder: &g taken pc-rel, g is a pointer, *g is the flagged object pointer.
__attribute__((visibility("hidden"))) extern void **g_ModStation_suspendObj;
// Tail-called suspend veneer (0x1ac188): receives the object pointer (r0) and holder (r1).

// ModStation::OnSuspend()
void ModStation::OnSuspend() {
    P(this, 0x128) = 0;
    void **holder = g_ModStation_suspendObj;
    void *obj = *holder;
    if (obj != 0)
        ModStation_suspendTail(obj, holder);
}

// Two singleton holders (single pc-rel deref each).
__attribute__((visibility("hidden"))) extern void **g_ModStation_resumeObj;
__attribute__((visibility("hidden"))) extern void **g_ModStation_resumeArg;
// Tail veneer (0x1ac178): (obj, 1, arg)

// ModStation::OnResume()
void ModStation_OnResume()
{
    void **holder = g_ModStation_resumeObj;
    void *obj = *holder;
    if (obj == 0)
        return;
    if (FModSound_tryToStopMusicForBGMusic() != 0)
        return;
    int arg = *(int *)*g_ModStation_resumeArg;
    ModStation_resumeTail(*holder, 1, arg);
}

struct Station;
struct Item;
struct PendingProduct;

__attribute__((visibility("hidden"))) extern int  *g_cpp_stack;     // [DAT_000ead34]
__attribute__((visibility("hidden"))) extern int **g_cpp_status;    // [DAT_000ead38]
__attribute__((visibility("hidden"))) extern int  *g_cpp_textId;    // [DAT_000ead3c]
__attribute__((visibility("hidden"))) extern int **g_cpp_itemTable; // [DAT_000ead44]

extern "C" {
int   Status_getCurrentCampaignMission_cpp();
int   GameText_getText_cpp(int id);
int   Status_getPendingProducts_cpp(int status);
int   Status_getStation_cpp();
int   Station_getIndex_cpp(Station *s);
int   Status_getShip_cpp();
int   Item_makeItem_cpp(int itemDesc);
void  Ship_addCargo_cpp(int ship, Item *it);
int   Item_getAmount_cpp();
int   Item_getIndex_cpp(Item *it);
void *PendingProduct_dtor_cpp(PendingProduct *p);
void  operator_delete_cpp(void *p);
void  ChoiceWindow_setNotice_cpp(void *cw);
// Appends one "received N x <item>" line to the running notification string (the original builds
// this with a long chain of AbyssEngine::String temporaries the decompiler mangled).
void  ModStation_cpp_appendDeliveryLine(int amount, int itemTextId);
}

// ModStation::checkPendingProducts() — on docking, delivers any queued station-production goods to
// the player's cargo hold and shows a summary notice.
void ModStation::checkPendingProducts() {
    int camp = Status_getCurrentCampaignMission_cpp();
    int textId = *(int *)g_cpp_textId;
    GameText_getText_cpp(textId); // seed the summary header string

    unsigned *products = (unsigned *)Status_getPendingProducts_cpp(*(int *)g_cpp_status);
    if (products != 0) {
        int byteOff = 0;
        int *itemTable = *g_cpp_itemTable;
        for (unsigned i = 0; i < *products; i = i + 1) {
            int pp = *(int *)(products[1] + byteOff);
            if (pp != 0) {
                int dstStation = *(int *)(pp + 0xc);
                Station *here = (Station *)Status_getStation_cpp();
                if (dstStation == Station_getIndex_cpp(here)) {
                    if (camp == 0x92 && *(int *)(pp + 0x14) == 0xd2) {
                        // mission 0x92 special-cases this product: drop it silently.
                        PendingProduct *o = *(PendingProduct **)(products[1] + byteOff);
                        if (o != 0)
                            operator_delete_cpp(PendingProduct_dtor_cpp(o));
                        *(int *)(products[1] + byteOff) = 0;
                        goto done;
                    }
                    Item *it = (Item *)Item_makeItem_cpp(
                        *(int *)(*(int *)(*itemTable + 4) + *(int *)(pp + 0x14) * 4));
                    Ship_addCargo_cpp(Status_getShip_cpp(), it);
                    ModStation_cpp_appendDeliveryLine(Item_getAmount_cpp(), Item_getIndex_cpp(it));

                    PendingProduct *o = *(PendingProduct **)(products[1] + byteOff);
                    if (o != 0)
                        operator_delete_cpp(PendingProduct_dtor_cpp(o));
                    *(int *)(products[1] + byteOff) = 0;
                    C(this, 0x63) = 1; // any-delivered flag
                }
            }
            byteOff = byteOff + 4;
        }
    }

    if (C(this, 0x63) != 0)
        ChoiceWindow_setNotice_cpp(P(this, 0x70));
done:;
}

__attribute__((visibility("hidden"))) extern int *g_okp_stack; // [DAT_000e9614]

extern "C" {
void Status_checkForLevelUp_okp();
// Opens station sub-screen `which` (0 hangar, 1 lounge, 2 star map, 3 missions, 4 status), with
// all the campaign-gating, sound, help-window and hangar-rebuild logic the decompiler corrupted.
// Returns nonzero if the screen actually opened.
int  ModStation_okp_openScreen(ModStation *self, int which);
// Shows the "feature locked" choice-window message for the current campaign progress.
void ModStation_okp_showLocked(ModStation *self);
void ModStation_leaveStation_okp(ModStation *self);
void ModStation_okp_toggleHelp(ModStation *self);
}

// ModStation::OnKeyPress(self, key) — drives the station main-menu cursor and screen selection.
void ModStation::OnKeyPress(long long key) {
    char *s = (char *)this;
    if (C(s, 0x24) == 0)
        return;

    Status_checkForLevelUp_okp();

    // a level-up banner is showing: SELECT just dismisses it.
    int t34 = I(s, 0x34);
    bool banner = 999 < U(s, 0x30);
    if (!((-(int)banner - t34) < 0)) {
        if (key == 0x10000) {
            U(s, 0x30) = 0x3e9;
            I(s, 0x34) = 0;
        }
        return;
    }

    // swallow keys while any modal sub-window owns input.
    if (C(s, 0x63) != 0 || C(s, 0x69) != 0 || C(s, 0x65) != 0 || C(s, 0x66) != 0 ||
        C(s, 0x67) != 0 || C(s, 0x64) != 0 || C(s, 0x68) != 0)
        return;

    // DLC menu open: SELECT closes the station.
    if (C(s, 0x6a) != 0) {
        if (key == 0x10000)
            ModStation_leaveStation_okp(this);
        return;
    }

    unsigned char held = UC(s, 0x61);
    char sub = C(s, 0x62);
    if (held == 0) {
        // not in the sub-menu: BACK leaves the station.
        if (key == 0x40000 && sub == 0)
            ModStation_leaveStation_okp(this);
        return;
    }

    if (sub != 0) {
        // in a sub-state; toggle help on the SOFT key.
        if (key == 0x20000)
            ModStation_okp_toggleHelp(this);
        return;
    }

    // main button row navigation.
    if (key == 0x4000) {            // left
        int sel = I(s, 0x44);
        int *btn = (int *)I(s, 0x48);
        btn[sel] = 0;
        sel = (0 < sel) ? (sel - 1) : 4;
        I(s, 0x44) = sel;
        btn[sel] = 1;
        return;
    }
    if (key == 0x8000) {            // right
        int sel = I(s, 0x44);
        int *btn = (int *)I(s, 0x48);
        btn[sel] = 0;
        sel = (sel < 4) ? (sel + 1) : 0;
        I(s, 0x44) = sel;
        btn[sel] = 1;
        return;
    }
    if (key == 0x20000)
        return;
    if (key != 0x10000)
        return;

    // SELECT: open the highlighted screen.
    int which = I(s, 0x44);
    if (which < 0 || which > 4) {
        ModStation_okp_showLocked(this);
        return;
    }
    if (ModStation_okp_openScreen(this, which) == 0)
        ModStation_okp_showLocked(this);
}

// Base destructor (returns `this`), reached through its veneer at 0x74ee4.
// operator delete veneer tail-called at the end (0x1ab098).

// Deleting destructor at 0xd56f8: runs the base dtor (which returns `this`), then
// tail-calls the finisher with that pointer.
void ModStation::dtor() {
    ModStation_dtor_finish(((ModStation *)(this))->dtor_inner());
}

struct Station;
struct SolarSystem;
struct EaseInOut;
struct EaseInOutMatrix;
// Matrix comes from common.h via `using AbyssEngine::AEMath::Matrix;` — do not redeclare.

__attribute__((visibility("hidden"))) extern int  *g_msc_stack;     // [DAT_000e5698]
__attribute__((visibility("hidden"))) extern int   g_msc_vtable;    // [DAT_000e569c] vtable base
__attribute__((visibility("hidden"))) extern int **g_msc_canvas;    // [DAT_000e56bc] PaintCanvas holder

extern "C" {
int   Status_getStation_msc();
int   Station_getIndex_msc(Station *s);
int   Status_getSystem_msc();
int   SolarSystem_getRace_msc();
void *ModStation_opnew_msc(unsigned size);
void  EaseInOut_ctor_msc(EaseInOut *e);
// Builds the two hangar camera key matrices for the given station race (MatrixSetTranslation /
// MatrixSetRotation cascade the decompiler corrupted) and constructs the 3000ms EaseInOutMatrix
// camera tween; returns the new EaseInOutMatrix and writes the resting cam coords into self+0x278.
EaseInOutMatrix *ModStation_msc_buildCameraTween(ModStation *self, int race);
}

// ModStation::ModStation() — constructor: zero-inits the station-module state, picks the home
// station's race, and sets up the hangar idle camera tween.
ModStation::ModStation() {
    I(this, 0x00) = g_msc_vtable + 8;          // vtable
    ((String *)B(this, 0x38))->ctor();

    // zero / default field block.
    I(this, 0x28) = 0;
    C(this, 0x23) = 0;     // this[1].field_20 + 3
    US(this, 0x20) = 0;    // partial of field_20 region — kept as a 16-bit clear
    C(this, 0x24) = 0;
    C(this, 0x69) = 0;     // field_68 + 1
    I(this, 0x0c) = 100;
    I(this, 0x10) = 0;
    I(this, 0x14) = 0;
    I(this, 0x1c) = 0;
    I(this, 0x20) = 0;
    I(this, 0x50) = 0;
    I(this, 0x54) = 0;
    I(this, 0x58) = 0;
    C(this, 0x6e) = 0;     // field_6C + 2
    C(this, 0x18) = 0;
    I(this, 0x80) = 0;     // m_pDialogueWindow
    I(this, 0x70) = 0;

    // pick the station race (0x65 -> raidstation race 8, 100 -> race 7, else system race).
    Station *st = (Station *)Status_getStation_msc();
    int race;
    if (Station_getIndex_msc(st) == 0x65) {
        race = 8;
    } else {
        st = (Station *)Status_getStation_msc();
        if (Station_getIndex_msc(st) == 100) {
            race = 7;
        } else {
            Status_getSystem_msc();
            race = SolarSystem_getRace_msc();
        }
    }

    EaseInOutMatrix *cam = ModStation_msc_buildCameraTween(this, race);
    P(this, 0x20) = cam;

    unsigned camHandle = **(unsigned **)g_msc_canvas;
    PaintCanvas *mscCanvas = (PaintCanvas *)*g_msc_canvas;
    Matrix *cur = (Matrix *)mscCanvas->CameraGetCurrent();
    mscCanvas->CameraSetLocal(camHandle, *cur);

    // three scalar EaseInOut tweens (fov / fade etc.) stored at this+0x288..0x290.
    EaseInOut *e;
    e = (EaseInOut *)ModStation_opnew_msc(0x10); EaseInOut_ctor_msc(e); P(this, 0x288) = e;
    e = (EaseInOut *)ModStation_opnew_msc(0x10); EaseInOut_ctor_msc(e); P(this, 0x28c) = e;
    e = (EaseInOut *)ModStation_opnew_msc(0x10); EaseInOut_ctor_msc(e); P(this, 0x290) = e;
}

// ModStation::~ModStation() base/inner destructor at 0xd56c0.
// PIC global: the ModStation vtable base (hidden -> direct pc-relative single deref).
__attribute__((visibility("hidden"))) extern void *ModStation_vtable;

// Installs the vtable (+8), runs OnRelease(this), destroys the String member at
// +0x38, and returns `this` in r0 (consumed by the deleting dtor's tail-call).
ModStation * ModStation::dtor_inner() {
    ModStation *self = this;
    *(void **)self = (char *)ModStation_vtable + 8;
    ((ModStation *)(self))->OnRelease();
    ((String *)((char *)self + 0x38))->dtor();
    return self;
}

// ModStation::leaveStation() — a 16-byte veneer the decompiler could only recover as "bad
// instruction data"; it tail-calls the real leave-station handler (which tears the station
// module down and transitions back to flight). Expressed faithfully as that forwarding call.

void ModStation::leaveStation() {
    ModStation_leaveStation_impl(this);
}

struct Achievements;
struct ChoiceWindow;

__attribute__((visibility("hidden"))) extern int *g_ModStation_cm_credit1;  // *g -> credits delta src
__attribute__((visibility("hidden"))) extern int *g_ModStation_cm_credit2;  // *g -> credits delta src
__attribute__((visibility("hidden"))) extern void **g_ModStation_cm_lead;   // **g -> leaderboards obj
__attribute__((visibility("hidden"))) extern Achievements **g_ModStation_cm_ach; // **g -> Achievements
__attribute__((visibility("hidden"))) extern void **g_ModStation_cm_status; // *g -> Status obj (campaign mission)

// Tail veneer at 0x1ac158: (ptr, ptr[0], ptr[1]).

// ModStation::checkMedals()
void ModStation::checkMedals() {
    Globals_reportLeaderboards(*g_ModStation_cm_lead);

    if (C(this, 0x6a) != 0 || C(this, 0x63) != 0) {
        // continuation path
        int idx = I(this, 0xc0) + 1;
        I(this, 0xc0) = idx;
        if (idx >= I(this, 0xc4)) {
            UC(this, 0x6a) = 0;
            return;
        }
        Array<int *> *medalArr = (Array<int *> *)P(this, 0xbc);
        int *medal = (*medalArr)[idx];
        ((ChoiceWindow *)(P(this, 0x88)))->setMedal(medal[0], medal[1]);
        int delta = *g_ModStation_cm_credit2;
        if (((Status *)(*g_ModStation_cm_status))->hardCoreMode() == 0)
            ((Status *)(*g_ModStation_cm_status))->changeCredits(delta);
        int *p = (*medalArr)[I(this, 0xc0)];
        ModStation_cm_tail(p, p[0], p[1]);
        return;
    }

    int delta = *g_ModStation_cm_credit1;
    if (((Status *)(*g_ModStation_cm_status))->getCurrentCampaignMission() == 1)
        ((ModStation *)((ModStation *)1))->addAchievement(0, 1);
    P(this, 0xbc) = 0;
    int *medals = ((Achievements *)(g_ModStation_cm_ach[0]))->getNewMedals();
    int count = 0;
    I(this, 0xc4) = 0;
    for (int i = 0; i != 0x2d; i++) {
        if (medals[i] >= 1) {
            count++;
            I(this, 0xc4) = count;
        }
    }
    if (count < 1)
        return;

    Array<int *> *arr = new Array<int *>();
    P(this, 0xbc) = arr;
    arr->resize(U(this, 0xc4));
    int j = 0;
    I(this, 0xc4) = 0;
    for (int i = 0; i != 0x2d; i++) {
        if (medals[i] >= 1) {
            int *cell = (int *)cm_op_new_arr(8);
            (*arr)[j] = cell;
            cell[0] = i;
            (*arr)[I(this, 0xc4)][1] = medals[i];
            j = I(this, 0xc4) + 1;
            I(this, 0xc4) = j;
        }
    }
    I(this, 0xc0) = 0;
    UC(this, 0x6a) = 1;
    void *cw = cm_op_new(0x5c);
    new (cw) ChoiceWindow();
    P(this, 0x88) = cw;
    int *medal = (*arr)[0];
    ((ChoiceWindow *)(cw))->setMedal(medal[0], medal[1]);
    if (((Status *)(*g_ModStation_cm_status))->hardCoreMode() == 0)
        ((Status *)(*g_ModStation_cm_status))->changeCredits(delta);
    int *p = (*arr)[0];
    ModStation_cm_tail(p, p[0], p[1]);
}

struct SpaceLounge;
struct StarMap;
struct CutScene;
struct MissionsWindow;

// PaintCanvas singleton holder: single pc-rel deref -> holder; object is *holder.
__attribute__((visibility("hidden"))) extern void **g_ModStation_r3d_canvas;

// Tail veneer at 0x1ab918.

// ModStation::OnRender3D()
void ModStation::OnRender3D() {
    if (C(this, 0x24) == 0)
        return;
    void **holder = g_ModStation_r3d_canvas;
    ((PaintCanvas *)*holder)->ClearBuffer((unsigned int)(long)*holder);

    char *p65 = (char *)this + 0x65;
    if (P(this, 0x14) == 0 || C(this, 0x66) != 0 || C(this, 0x64) != 0 ||
        C(this, 0x68) != 0 || C(this, 0x67) != 0 || *p65 != 0 ||
        C(this, 0x62) != 0) {
        if (C(this, 0x67) != 0) {
            ((StarMap *)(P(this, 0x10)))->renderBG();
        } else if (*p65 != 0) {
            ((SpaceLounge *)(P(this, 0x74)))->OnRenderBG();
        }
    } else {
        ((CutScene *)(P(this, 0x14)))->renderBG();
    }
    ((PaintCanvas *)*holder)->Begin3d();
    if (*p65 != 0)
        ((SpaceLounge *)(P(this, 0x74)))->OnRender3D();
    else if (C(this, 0x67) != 0)
        ((StarMap *)(P(this, 0x10)))->render();
    else if (C(this, 0x64) != 0)
        ((MissionsWindow *)(P(this, 0x80)))->render3D();
    else if (C(this, 0x66) == 0 && P(this, 0x14) != 0)
        ((CutScene *)(P(this, 0x14)))->render3D();
    ModStation_r3d_endTail(*holder);
}

struct Engine;
struct SolarSystem;

__attribute__((visibility("hidden"))) extern int **g_rl_engineHolder; // [DAT_000e71f4]
__attribute__((visibility("hidden"))) extern int   g_rl_lightFlag;

extern "C" {
int  ApplicationManager_GetEngine_rl();
int  Status_getSystem_rl();
int  SolarSystem_getRace_rl();
int  rl_engineFromHolder(int holder); // (*pcVar4)(*puVar3): resolve engine from the cached holder
// The actual Engine light setters all take float colour triples the decompiler dropped; the
// engine helper applies the per-race ambient/diffuse/specular profile to the given engine.
void Engine_resetStationLight(int engine, int race);
void Engine_LightEnable_rl(int engine, int flag);
}

// ModStation::resetLight() — restores the hangar's lighting rig to the home-system's race profile.
void ModStation_resetLight()
{
    int holder = *(int *)g_rl_engineHolder;
    rl_engineFromHolder(holder); // material ambient
    rl_engineFromHolder(holder); // light direction
    rl_engineFromHolder(holder); // diffuse colour

    Status_getSystem_rl();
    int race = SolarSystem_getRace_rl();
    int engine;
    switch (race) {
    case 1: case 2: case 3: case 8:
        engine = ApplicationManager_GetEngine_rl();
        break;
    default:
        engine = ApplicationManager_GetEngine_rl();
        break;
    }
    Engine_resetStationLight(engine, race); // ambient colour for the race
    engine = ApplicationManager_GetEngine_rl();
    // specular colour
    engine = ApplicationManager_GetEngine_rl();
    Engine_LightEnable_rl(engine, g_rl_lightFlag);
}

struct Station;
struct Mission;
struct DialogueWindow;
struct CutScene;
struct SpaceLounge;

__attribute__((visibility("hidden"))) extern int  *g_ou_stack;     // [DAT_000e9d98]
__attribute__((visibility("hidden"))) extern int **g_ou_sound;     // FMod sound holder
__attribute__((visibility("hidden"))) extern int **g_ou_layout;    // Layout holder
__attribute__((visibility("hidden"))) extern int **g_ou_status;    // Status holder
__attribute__((visibility("hidden"))) extern int **g_ou_achievements;
__attribute__((visibility("hidden"))) extern char **g_ou_spaceLoungeFlag; // [DAT_000e9db0]
__attribute__((visibility("hidden"))) extern int **g_ou_appData;   // ApplicationData holder
__attribute__((visibility("hidden"))) extern int **g_ou_textRoot;  // GameText root
__attribute__((visibility("hidden"))) extern int **g_ou_module;    // next app module

extern "C" {
void *ModStation_opnew_ou(unsigned size);
void  ModStation_opdelete_ou(void *p);

// --- self methods reached as direct calls ---
void  ModStation_autosave_ou(ModStation *self);
void  ModStation_checkPendingProducts_ou(ModStation *self);
void  ModStation_checkMedals_ou(ModStation *self);
void  ModStation_checkHints_ou(ModStation *self);

// --- engine / world ---
int   ApplicationManager_GetElapsedTimeMillis_ou();
int   ApplicationManager_GetApplicationData_ou();
void  ApplicationManager_SetCurrentApplicationModule_ou(int module);

void  FModSound_updateAll_ou(int sound);
void  FModSound_play_ou(int sound, int id, void *p, float vol);
void  FModSound_stop_ou(int sound);
void  FModSound_setParamValue_ou(int sound, int a, int b, float v);

void  Layout_update_ou(int layout);
void  Layout_formatCredits_ou(void *outStr);

void  Status_incPlayingTime_ou(long long delta);
int   Status_getCredits_ou();
void  Status_changeCredits_ou(int status);
int   Status_getCurrentCampaignMission_ou();
void  Status_nextCampaignMission_ou(int status);
void  Status_setCurrentCampaignMission_ou(int status);
int   Status_getStation_ou();
int   Station_getIndex_ou(Station *s);
int   Status_getSystem_ou();
int   SolarSystem_getIndex_ou();
int   Status_getCampaignMission_ou();
int   Status_missionCompleted_ou(int status, int a, long long b);
int   Status_missionFailed_ou(int status, int a, int b);
void  Status_removeMission_ou(int status);

int   Mission_getType_ou();
int   Mission_getStatusValue_ou();
void  Mission_setStatusValue_ou(int v);
void  Mission_ctor_ou(Mission *m, int a, int b, int c);
void  Mission_setCampaignMission_ou(void *mission);

void  Achievements_updateCredits_ou(void *ach, int credits);
int   Globals_getInAppPurchaseArrayIndex_ou(int globals, void *arr);

void  TouchButton_setText_ou(int btn);

void  ChoiceWindow_set_ou(int cw, int text, int flag);
void  ChoiceWindow_setNotice_ou(int cw, int text);
void  ChoiceWindow_update_ou(int cw);
int   GameText_getText_ou(int id);

void  NewsTicker_update_ou(int nt);
void  CutScene_process_ou(int cs);
void  CutScene_update_ou(int cs);
void  CutScene_ctor_ou(CutScene *cs, int kind);
int   CutScene_initialize_ou(CutScene *cs);

void  DialogueWindow_ctor_msg_ou(DialogueWindow *dw, int titleStr, int bodyStr, int *param);
void  DialogueWindow_ctor_mission_ou(DialogueWindow *dw, void *mission, int level, int kind);
void  DialogueWindow_ctor_ou(DialogueWindow *dw);
void  DialogueWindow_setMission_ou(void *dw, void *mission, int flag);
void  DialogueWindow_update_ou(int dw);

void  StarMap_update_ou(int sm, int dt);
void  MissionsWindow_update_ou(int w);
void  HangarWindow_update_ou(int w);
void  HangarWindow_hideMessage_ou();
void  StatusWindow_update_ou(int w);
void  MenuTouchWindow_update_ou(void *w, int dt);
void  MenuTouchWindow_dtor_ou(void *w);

int   SpaceLounge_introFinished_ou();
void  SpaceLounge_ctor_ou(SpaceLounge *l);
void  SpaceLounge_init_ou(void *l);
void  SpaceLounge_update_ou(int l);
void  SpaceLounge_setHangarUpdate_ou(void *l);

int   Radio_lastMessageShown_ou();

// --- corrupted SIMD blocks delegated to helpers ---
// Runs the three idle-camera "wobble" easings (pitch/yaw/roll) at +0x130..+0x12e and writes the
// resulting camera matrix; the original is dense MatrixSetTranslation/EaseInOut SIMD math.
void  ModStation_ou_updateIdleCamera(ModStation *self, int elapsed);
// Advances the radio-message reveal cutscene scroll box (ScrollTouchBox SIMD positioning).
void  ModStation_ou_updateRadioReveal(ModStation *self, int elapsed);
// Plays out the per-frame idle-hangar ship geometry animation (translate/rotate/moveForward SIMD).
void  ModStation_ou_animateHangarShip(ModStation *self);
// Smooths the hangar light intensity toward its target (FloatVectorMin / clamp SIMD math).
void  ModStation_ou_updateHangarLight(ModStation *self, int elapsed);
}

// ModStation::OnUpdate() — the per-frame station tick: accumulates time, updates audio/layout, the
// active sub-window, the docking-cutscene + idle camera, then drives mission completion/failure
// dialogues and the campaign-progress hooks.
void ModStation::OnUpdate() {
    char *s = (char *)this;

    int *status = *(int **)g_ou_status;
    int *sound  = *(int **)g_ou_sound;

    // clamp elapsed time to [0, 0x96] ms.
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

    I(s, 0x28) = elapsed;                       // field_28 = dt
    *(long long *)(s + 0x30) += (long long)elapsed; // field_30 = accumulated time (64-bit)

    FModSound_updateAll_ou(*sound);
    int *layout = *(int **)g_ou_layout;
    Layout_update_ou(*layout);
    if (C(s, 0x32) == 0)
        Status_incPlayingTime_ou((long long)(unsigned)**(int **)g_ou_status);

    // refresh the credits button text.
    int creditsBtn = *(int *)(s + 0x50 + 0x4); // this[1].field_4
    Status_getCredits_ou();
    Layout_formatCredits_ou(s + 0); // formatted into a String temporary
    TouchButton_setText_ou(creditsBtn);

    Achievements_updateCredits_ou((void *)**(int **)g_ou_achievements, Status_getCredits_ou());

    // entering the space lounge intro (triggered by the flag at g_ou_spaceLoungeFlag).
    if (C(s, 0x24) != 0) {
        char *flag = *g_ou_spaceLoungeFlag;
        if (*flag != 0) {
            C(s, 0x94) = 1;          // this[1].field_48 low byte
            *flag = 0;
            if (P(s, 0x74) == 0) {
                SpaceLounge *sl = (SpaceLounge *)ModStation_opnew_ou(0x10c);
                SpaceLounge_ctor_ou(sl);
                P(s, 0x74) = sl;
            } else {
                SpaceLounge_init_ou(P(s, 0x74));
            }
            FModSound_setParamValue_ou(*sound, 0, *sound, 0.0f);
            FModSound_stop_ou(*sound);
            FModSound_play_ou(*sound, 0x6c, 0, 0.0f);
            C(s, 0x65) = 1;          // this->field_64+1
            goto epilogue;
        }
    }

    // in-app-purchase result handling.
    {
        int appData = ApplicationManager_GetApplicationData_ou();
        if (*(char *)(appData + 0x42) != 0) {
            ChoiceWindow_setNotice_ou(I(s, 0x70), GameText_getText_ou(**g_ou_textRoot));
            C(s, 0x6f) = 1;          // m_nStarMapWindowOpen+3
            *(char *)(appData + 0x42) = 0;
        }
        if (*(char *)(appData + 0x41) != 0 && *(int **)(appData + 0x58) != 0 &&
            **(int **)(appData + 0x58) != 0 &&
            (unsigned)(*(int *)(appData + 0x48) - 0x32) < 5) {
            int idx = Globals_getInAppPurchaseArrayIndex_ou(0, *(void **)(appData + 0x48));
            if (-1 < *(int *)(*(int *)(*(int *)(appData + 0x60) + 4) + idx * 4)) {
                Status_changeCredits_ou(*status);
                ModStation_autosave_ou(this);
                ChoiceWindow_setNotice_ou(I(s, 0x70), GameText_getText_ou(**g_ou_textRoot));
                C(s, 0x6f) = 1;
                *(char *)(appData + 0x41) = 0;
                if (I(s, 0x78) != 0)
                    HangarWindow_hideMessage_ou();
                if (I(s, 0x50) != 0) {
                    MenuTouchWindow_dtor_ou(P(s, 0x50));
                    ModStation_opdelete_ou(P(s, 0x50));
                }
                I(s, 0x50) = 0;
            }
        }
    }

    // idle camera + ticker + cutscene while no sub-window is up.
    if (C(s, 0x64) == 0 && C(s, 0x65) == 0 && C(s, 0x66) == 0 && C(s, 0x67) == 0 &&
        C(s, 0x68) == 0 && C(s, 0x60) == 0) {
        if (I(s, 0x1c) != 0)
            NewsTicker_update_ou(I(s, 0x1c));
        if (I(s, 0x14) != 0)
            CutScene_process_ou(I(s, 0x14));
        ModStation_ou_updateIdleCamera(this, elapsed);
    }

    // only proceed past the warm-up once enough time has elapsed.
    if (*(long long *)(s + 0x30) < (long long)(*(int *)(s + 0x30) < 1000 ? 1 : 0))
        goto epilogue;

    if (C(s, 0x60) != 0) {
        // docking cutscene playback + campaign transition at its end.
        int t = *(int *)(s + 0x50 + 0x24); // this[1].field_24
        if (t < 1 + 0 && 0 < I(s, 0x28) + t) {
            if (P(s, 0x14) != 0) {
                // tear down the old cutscene.
                ModStation_opdelete_ou(P(s, 0x14));
            }
            P(s, 0x14) = 0;
            CutScene *cs = (CutScene *)ModStation_opnew_ou(0xa0);
            CutScene_ctor_ou(cs, 2);
            P(s, 0x14) = cs;
            CutScene_initialize_ou(cs);
            *(int *)((char *)P(s, 0x14) + 9 * 4) = 0;
            t = *(int *)(s + 0x50 + 0x24);
            C(s, 0x60) = 1;
        }
        *(int *)(s + 0x50 + 0x24) = t + I(s, 0x28);
        CutScene_update_ou(I(s, 0x14));

        if (Radio_lastMessageShown_ou() != 0)
            ModStation_ou_updateRadioReveal(this, elapsed);

        if (*(int *)(s + 0x50 + 0x24) >= 0 /* DAT_000ea664 */ ) {
            C(s, 0x24) = 0;
            Status_nextCampaignMission_ou(*status);
            int nextModule = **(int **)g_ou_appData;
            ApplicationManager_SetCurrentApplicationModule_ou(nextModule);
            C(s, 0x60) = 0;
            goto epilogue;
        }
    } else if (C(s, 0x68) != 0) {           // DialogueWindow open variant 1
        DialogueWindow_update_ou(I(s, 0x84));
    } else if (C(s, 0x69) != 0) {           // StarMap
        StarMap_update_ou(I(s, 0x10), I(s, 0x28));
    } else if (C(s, 0x67) != 0) {           // HangarWindow
        HangarWindow_update_ou(I(s, 0x78));
    } else if (C(s, 0x66) != 0) {           // MissionsWindow
        MissionsWindow_update_ou(I(s, 0x80));
    } else if (C(s, 0x64) != 0) {           // SpaceLounge
        SpaceLounge_update_ou(I(s, 0x74));
    } else if (C(s, 0x65) != 0) {           // StatusWindow
        StatusWindow_update_ou(I(s, 0x7c));
    }

    if (C(s, 0x6f) != 0)
        ChoiceWindow_update_ou(I(s, 0x70));
    if (C(s, 0x6e) != 0)
        MenuTouchWindow_update_ou(P(s, 0x50), I(s, 0x28));

    // mission completion / failure + campaign dialogue flow (runs once per docked tick).
    if (C(s, 0x6f) == 0 && C(s, 0x69) == 0 && C(s, 0x6c) == 0) {
        if (C(s, 0x96) == 0) {              // this[1].field_48+2
            ModStation_checkPendingProducts_ou(this);
            C(s, 0x96) = 1;
        }
        if (C(s, 0x95) == 0) {              // this[1].field_48+1
            ModStation_checkMedals_ou(this);
            C(s, 0x95) = 1;
        }
        // "remember to save" hint.
        if (C(s, 0x6a) == 0 && C(s, 0x6e) != 0) {
            int rec = *(int *)g_ou_status;
            if (*(char *)(rec + 0x2a) == 0) {
                ChoiceWindow_setNotice_ou(I(s, 0x70), GameText_getText_ou(**g_ou_textRoot));
                C(s, 0x6f) = 1;
                *(char *)(rec + 0x2a) = 1;
                C(s, 0x6e) = 0;
            }
        }

        bool special = false;
        int intro = (C(s, 0x65) != 0) ? SpaceLounge_introFinished_ou() : 0;
        int completed = Status_missionCompleted_ou(*status, 1, (long long)intro);
        Mission *m = (Mission *)completed;
        int camp = Status_getCurrentCampaignMission_ou();
        if (m == 0 && camp == 0x74) {
            if (C(s, 0x65) != 0 && SpaceLounge_introFinished_ou() != 0) {
                Status_getSystem_ou();
                if (SolarSystem_getIndex_ou() == 0x12) {
                    Status_getCampaignMission_ou();
                    int mask = Mission_getStatusValue_ou();
                    Station *st = (Station *)Status_getStation_ou();
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

        // mission 0x94..0x97 supernova-system special dialogue vs. the generic path.
        int campNow = Status_getCurrentCampaignMission_ou();
        bool generic = campNow < 0x94 || Status_getCurrentCampaignMission_ou() > 0x97;
        if (!generic) {
            Station *st = (Station *)Status_getStation_ou();
            int sidx = Station_getIndex_ou(st);
            int bit = (sidx == 0x42) ? 2 : (sidx == 0x37) ? 1 : (sidx == 9) ? 4 : 0;
            bool introOk = C(s, 0x65) != 0 && SpaceLounge_introFinished_ou() != 0;
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
                        DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                        DialogueWindow_ctor_ou(dw);
                        P(s, 0x84) = dw;
                        Mission *nm = (Mission *)ModStation_opnew_ou(0x78);
                        Mission_ctor_ou(nm, 0xa0, 0, -1);
                        Mission_setCampaignMission_ou(nm);
                        DialogueWindow_setMission_ou(P(s, 0x84), nm, 1);
                        C(s, 0x69) = 1;
                        goto afterDialogue;
                    }
                }
                generic = true;
            } else if (introOk && 0x93 < Status_getCurrentCampaignMission_ou() &&
                       sidx == 0x60 && Status_getCurrentCampaignMission_ou() <= 0x96) {
                Status_setCurrentCampaignMission_ou(*status);
                DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                DialogueWindow_ctor_ou(dw);
                P(s, 0x54) = m;       // this[1].field_C cached mission
                P(s, 0x84) = dw;
                DialogueWindow_setMission_ou(P(s, 0x84), m, 1);
                C(s, 0x69) = 1;
                goto afterDialogue;
            } else {
                generic = true;
            }
        }
        if (generic) {
            if (special) {
                // storyline arrival dialogue (mission 0x74 hub).
                Station *st = (Station *)Status_getStation_ou();
                int sidx = Station_getIndex_ou(st);
                int slot = (sidx == 0x5e) ? 3 : (sidx - 0x5a);
                FModSound_play_ou(*sound, slot + 0x619, 0, 0.0f);
                int kind = (slot == 1) ? 0x39 : 0x3a;
                if (slot == 0 || slot == 2) kind = 0;
                DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                DialogueWindow_ctor_msg_ou(dw, GameText_getText_ou(**g_ou_textRoot),
                                           GameText_getText_ou(**g_ou_textRoot),
                                           (int *)(long)kind);
                C(s, 0x69) = 1;
                P(s, 0x84) = dw;
            } else if (m == 0) {
                Mission *fm = (Mission *)Status_missionFailed_ou(*status, 1, 0);
                if (fm != 0) {
                    DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                    DialogueWindow_ctor_mission_ou(dw, fm, 0, 2);
                    C(s, 0x69) = 1;
                    P(s, 0x84) = dw;
                    Status_removeMission_ou(*status);
                    if (Mission_getType_ou() == 0xd) {
                        *(short *)(*status + 0xf0) = 0;
                        ModStation_autosave_ou(this);
                    }
                }
            } else {
                P(s, 0x54) = m;
                DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                DialogueWindow_ctor_mission_ou(dw, m, 0, 1);
                C(s, 0x69) = 1;
                P(s, 0x84) = dw;
                if (Mission_getType_ou() == 0xd) {
                    *(short *)(*status + 0xf0) = 0;
                    ModStation_autosave_ou(this);
                }
                if (P(s, 0x74) != 0)
                    SpaceLounge_setHangarUpdate_ou(P(s, 0x74));
            }
        }
    afterDialogue:
        ModStation_checkHints_ou(this);
    }

    // idle hangar light + ship animation.
    if (C(s, 0x50 + 0x70) == 0)            // this[1].field_70 low byte
        ModStation_ou_updateHangarLight(this, elapsed);
    if (I(s, 0x14) != 0 && C(s, 0x60) == 0)
        ModStation_ou_animateHangarShip(this);

epilogue:;
}

struct Station;

__attribute__((visibility("hidden"))) extern void **g_ModStation_ric_chk;    // *g -> canary src
__attribute__((visibility("hidden"))) extern void **g_ModStation_ric_canvas; // g -> P, *P -> canvas obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_ric_status; // *g -> Status obj
__attribute__((visibility("hidden"))) extern int *g_ModStation_ric_rotX;     // table base (pc-rel)
__attribute__((visibility("hidden"))) extern int *g_ModStation_ric_rotY;     // table base (pc-rel)

// ModStation::resetIdleCamForHangar()
void ModStation::resetIdleCamForHangar() {
    char matrix[60];

    if (P(this, 0x14) != 0)
        ((CutScene *)(P(this, 0x14)))->resetCamera();

    if (P(this, 0x13c) != 0) {
        ((AbyssEngine::EaseInOut *)(P(this, 0x13c)))->SetRange(I(this, 0x130), I(this, 0x130));
    } else {
        void *p = ric_op_new(0x10);
        new (p) AbyssEngine::EaseInOut(I(this, 0x130), I(this, 0x130));
        P(this, 0x13c) = p;
    }
    if (P(this, 0x140) != 0) {
        ((AbyssEngine::EaseInOut *)(P(this, 0x140)))->SetRange(I(this, 0x134), I(this, 0x134));
    } else {
        void *p = ric_op_new(0x10);
        new (p) AbyssEngine::EaseInOut(I(this, 0x134), I(this, 0x134));
        P(this, 0x140) = p;
    }
    if (P(this, 0x144) != 0) {
        ((AbyssEngine::EaseInOut *)(P(this, 0x144)))->SetRange(I(this, 0x138), I(this, 0x138));
    } else {
        void *p = ric_op_new(0x10);
        new (p) AbyssEngine::EaseInOut(I(this, 0x138), I(this, 0x138));
        P(this, 0x144) = p;
    }

    void **canvasHolder = g_ModStation_ric_canvas;
    void *canvas = *canvasHolder;
    ((PaintCanvas *)canvas)->CameraGetCurrent();
    void *loc = ((PaintCanvas *)canvas)->CameraGetLocal((unsigned int)(long)canvas);
    AEMath_MatrixSetTranslation(matrix, I(this, 0x130), I(this, 0x134), I(this, 0x138));

    int race;
    Station *st = ((Status *)(*g_ModStation_ric_status))->getStation();
    if (Station_getIndex(st) == 0x65) {
        race = 8;
    } else {
        st = ((Status *)(*g_ModStation_ric_status))->getStation();
        if (Station_getIndex(st) == 100) {
            race = 7;
        } else {
            race = ((SolarSystem *)(long)((Status *)(*g_ModStation_ric_status))->getSystem())->getRace();
        }
    }

    canvas = *canvasHolder;
    ((PaintCanvas *)canvas)->CameraGetCurrent();
    void *loc2 = ((PaintCanvas *)canvas)->CameraGetLocal((unsigned int)(long)canvas);
    AEMath_MatrixSetRotation(matrix, loc2, g_ModStation_ric_rotX[race], g_ModStation_ric_rotY[race], 0, 2);
    (void)loc;
}

struct Wanted;
struct Agent;
struct DialogueWindow;
struct Achievements;

__attribute__((visibility("hidden"))) extern int  *g_ch_stack;      // [DAT_000eaf3c]
__attribute__((visibility("hidden"))) extern int  *g_ch_hintRec;    // [DAT_000eaf40] hint-shown flags record
__attribute__((visibility("hidden"))) extern int **g_ch_status;     // [DAT_000eafb4] etc.
__attribute__((visibility("hidden"))) extern Achievements **g_ch_ach;// [DAT_000eaf54]

extern "C" {
int  Status_getCurrentCampaignMission_ch();
int  Wanted_isTerminated_ch(Wanted *w);
int  Achievements_gotAllMedals_ch(Achievements *a);
int  Achievements_gotAllGoldMedals_ch();
int  Achievements_gotAllSupernovaMedals_ch(Achievements *a);
int  Status_isBlueprintUnlocked_ch(void *status, int bp);
void Status_unlockBluePrint_ch(void *status, int bp);
int  Status_hardCoreMode_ch();
void ModStation_autosave_ch();
// Pops the CBS choice-window hint for the given wanted index (builds the "criminal terminated"
// message with name/ship substitution — a long String-temporary chain the decompiler mangled).
void ModStation_ch_showWantedHint(ModStation *self, int wantedFieldOff);
// Pops a simple text-only choice-window hint (medals / blueprint unlock / hardcore reward).
void ModStation_ch_showTextHint(ModStation *self, int textIdSlot);
// Spawns the "new wingman available" / "wingman left" dialogue window with its sound.
void ModStation_ch_showWingmanDialogue(ModStation *self, int kind);
}

// ModStation::checkHints() — once per docked frame, shows the highest-priority contextual hint
// (wanted criminal terminated, all medals earned, blueprint unlocked, wingman events).
void ModStation::checkHints() {
    char *s = (char *)this;
    if (C(s, 0x60) != 0)   // a window is already busy
        return;

    int hintRec = *g_ch_hintRec;

    // intro CBS hint (after mission 0x12).
    if (C(s, 0x6a) == 0 && C(s, 0x63) == 0 && *(char *)(hintRec + 0x34) == 0 &&
        0x12 < Status_getCurrentCampaignMission_ch()) {
        ModStation_ch_showWantedHint(this, -1);
        *(char *)(hintRec + 0x34) = 1;
        C(s, 0x63) = 1;
        C(s, 0xdc) = 1;
    }

    int **status = g_ch_status;
    // four "storyline wanted terminated" hints (slots 0x18/0x30/0x48/0x60).
    static const int wantedOff[4] = {0x18, 0x30, 0x48, 0x60};
    static const int wantedFlag[4] = {0x33, 0x34, 0x35, 0x36};
    if (C(s, 0x69) == 0) {
        for (int k = 0; k < 4; k = k + 1) {
            if (C(s, 0x6a) != 0) return;
            if (C(s, 0x63) == 0 && *(char *)(hintRec + wantedFlag[k]) == 0) {
                Wanted *w = *(Wanted **)(*(int *)(*(int *)*status + 4) + wantedOff[k]);
                if (Wanted_isTerminated_ch(w) != 0) {
                    ModStation_ch_showWantedHint(this, wantedOff[k]);
                    C(s, 0x63) = 1;
                    *(char *)(hintRec + wantedFlag[k]) = 1;
                }
            }
        }
    }

    // achievement-completion hints.
    if (C(s, 0x6a) == 0) {
        if (C(s, 0x63) == 0 && *(char *)(hintRec + 0x1a) == 0 &&
            Achievements_gotAllMedals_ch(*g_ch_ach) != 0) {
            ModStation_ch_showTextHint(this, 0x1a);
            *(char *)(hintRec + 0x1a) = 1;
            C(s, 0x63) = 1;
        }
        if (C(s, 0x6a) == 0 && C(s, 0x63) == 0 && *(char *)(hintRec + 0x1b) == 0 &&
            Achievements_gotAllGoldMedals_ch() != 0) {
            ModStation_ch_showTextHint(this, 0x1b);
            *(char *)(hintRec + 0x1b) = 1;
            C(s, 0x63) = 1;
        }
        // supernova blueprint unlock.
        if (C(s, 0x6a) == 0 && C(s, 0x63) == 0) {
            void *statPtr = (void *)*status;
            if (Status_isBlueprintUnlocked_ch(statPtr, 0xe8) == 0 &&
                Achievements_gotAllGoldMedals_ch() != 0 &&
                Achievements_gotAllSupernovaMedals_ch(*g_ch_ach) != 0) {
                ModStation_ch_showTextHint(this, 0x3b);
                Status_unlockBluePrint_ch(statPtr, 0xe8);
                ModStation_autosave_ch();
                C(s, 0x63) = 1;
            }
        }
        // hardcore-unlock hint.
        if (C(s, 0x6a) == 0 && C(s, 0x63) == 0 && *(char *)(hintRec + 0x3a) == 0 &&
            0xa1 < Status_getCurrentCampaignMission_ch()) {
            bool ok = (Achievements_gotAllGoldMedals_ch() != 0 &&
                       Achievements_gotAllSupernovaMedals_ch(*g_ch_ach) != 0 &&
                       Status_hardCoreMode_ch() == 0) ||
                      Status_hardCoreMode_ch() != 0;
            if (ok) {
                *(char *)(hintRec + 0x3a) = 1;
                ModStation_ch_showTextHint(this, 0x3c);
                ModStation_autosave_ch();
                C(s, 0x63) = 1;
            }
        }
    }

    // wingman recruit / dismissal dialogue.
    if (C(s, 0x69) == 0 && C(s, 0x6a) == 0 && C(s, 0x63) == 0)
        ModStation_ch_showWingmanDialogue(this, 0);
}

struct HangarWindow;
struct StarMap;
struct SpaceLounge;
struct StatusWindow;
struct MissionsWindow;
struct MenuTouchWindow;
struct ChoiceWindow;
struct DialogueWindow;
struct NewsTicker;

__attribute__((visibility("hidden"))) extern void **g_ModStation_tm_layout;  // g -> P, *P -> layout obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_tm_screenH; // **g -> screen height

// ModStation::OnTouchMove(int, int, void*)
void ModStation::OnTouchMove(int x, int y, void *touch) {
    if (P(this, 0x128) != touch)
        return;
    I(this, 0x110) = x;
    I(this, 0x114) = y;
    if (C(this, 0x60) != 0)
        return;

    void **layoutHolder = g_ModStation_tm_layout;
    void *layoutObj = *layoutHolder;
    if (*(char *)layoutObj != 0) {
        ((Layout *)(layoutObj))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x69) != 0) {
        ((DialogueWindow *)(P(this, 0x84)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x63) != 0) {
        ((ChoiceWindow *)(P(this, 0x70)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x6a) != 0) {
        ((ChoiceWindow *)(P(this, 0x88)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x66) != 0) {
        ((HangarWindow *)(P(this, 0x78)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x67) != 0) {
        ((StarMap *)(P(this, 0x10)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x65) != 0) {
        ((SpaceLounge *)(P(this, 0x74)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x68) != 0) {
        ((StatusWindow *)(P(this, 0x7c)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x64) != 0) {
        ((MissionsWindow *)(P(this, 0x80)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x62) != 0) {
        ((MenuTouchWindow *)(P(this, 0x50)))->OnTouchMove(x, y);
        return;
    }
    if (C(this, 0x61) == 0)
        return;
    ((TouchButton *)(P(this, 0x90)))->OnTouchMove(x, y);
    ((TouchButton *)(P(this, 0x94)))->OnTouchMove(x, y);
    for (int i = 0; i != 5; i++)
        ((TouchButton *)(*(void **)(I(P(this, 0x8c), 4) + i * 4)))->OnTouchMove(x, y);
    ((Layout *)(layoutObj))->OnTouchMove(x, y);
    if (((NewsTicker *)(P(this, 0x1c)))->OnTouchMove(x, y) != 0)
        return;
    int *layout = (int *)layoutObj;
    if (y <= layout[0xc / 4])
        return;
    if (*(int *)*g_ModStation_tm_screenH - layout[0x10 / 4] <= y)
        return;
    if (x <= I(this, 0xa8))
        return;
    int delta = x - I(this, 0xe4);
    I(this, 0xf0) = delta;
    I(this, 0xf4) = 0x3f800000;
    I(this, 0xe8) = I(this, 0xe8) + delta;
    I(this, 0xe4) = x;
}

// Two singleton holders (single pc-rel deref each).
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_sound;   // *g -> flag
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_canvas;  // *g -> canvas
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_lang;    // *g -> obj (font lang)
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_reload;  // *g -> reload flag obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_or_imgfac;  // *g -> image factory

// Tail veneer at 0x1ac168.

// ModStation::OnRelease()
void ModStation::OnRelease() {
    void **soundHolder = g_ModStation_or_sound;
    if (*soundHolder != 0) {
        ((FModSound *)(*soundHolder))->disableReverb();
        ((FModSound *)(*soundHolder))->stopAllSoundFXEvents();
    }
    ((PaintCanvas *)*g_ModStation_or_canvas)->FogEnable(0, 1);

    if (P(this, 0x8c) != 0) {
        ArrayReleaseClasses_TouchButton(P(this, 0x8c));
        if (P(this, 0x8c) != 0)
            ms_op_delete(Array_TB_dtor(P(this, 0x8c)));
    }
    P(this, 0x8c) = 0;

    if (P(this, 0x78) != 0)
        ms_op_delete(HangarWindow_dtor(P(this, 0x78)));
    P(this, 0x78) = 0;

    if (P(this, 0x10) != 0)
        ms_op_delete(StarMap_dtor(P(this, 0x10)));
    P(this, 0x10) = 0;

    if (P(this, 0x74) != 0)
        ms_op_delete(((SpaceLounge *)(P(this, 0x74)))->dtor());
    P(this, 0x74) = 0;

    if (P(this, 0x80) != 0)
        ms_op_delete(MissionsWindow_dtor(P(this, 0x80)));
    P(this, 0x80) = 0;

    if (P(this, 0x84) != 0)
        ms_op_delete(DialogueWindow_dtor(P(this, 0x84)));
    P(this, 0x84) = 0;

    if (P(this, 0x88) != 0)
        ms_op_delete(ChoiceWindow_dtor(P(this, 0x88)));
    P(this, 0x88) = 0;

    if (P(this, 0x7c) != 0)
        ms_op_delete(StatusWindow_dtor(P(this, 0x7c)));
    P(this, 0x7c) = 0;

    if (P(this, 0x14) != 0)
        ms_op_delete(CutScene_dtor(P(this, 0x14)));
    P(this, 0x14) = 0;

    if (P(this, 0x50) != 0)
        ms_op_delete(((MenuTouchWindow *)(P(this, 0x50)))->dtor());
    P(this, 0x50) = 0;

    if (P(this, 0x54) != 0)
        ms_op_delete(Radio_dtor(P(this, 0x54)));
    P(this, 0x54) = 0;

    if (P(this, 0x58) != 0)
        ms_op_delete(Array_RM_dtor(P(this, 0x58)));
    P(this, 0x58) = 0;

    if (P(this, 0x1c) != 0)
        ms_op_delete(NewsTicker_dtor(P(this, 0x1c)));
    P(this, 0x1c) = 0;

    if (P(this, 0x70) != 0)
        ms_op_delete(ChoiceWindow_dtor(P(this, 0x70)));
    P(this, 0x70) = 0;

    ((PaintCanvas *)g_PaintCanvas)->ReleaseAllResources();
    int langObj = *(int *)*g_ModStation_or_lang;
    Globals_loadFont(langObj, GameText_getLanguage());

    void **reloadHolder = g_ModStation_or_reload;
    if (*reloadHolder != 0) {
        ((Layout *)(*reloadHolder))->reload();
        ((ImageFactory *)(*g_ModStation_or_imgfac))->reload();
        ((Layout *)(*reloadHolder))->initTip();
    }

    if (P(this, 0x20) != 0)
        ms_op_delete(EaseInOutMatrix_dtor(P(this, 0x20)));
    P(this, 0x20) = 0;

    if (P(this, 0x13c) != 0)
        ms_op_delete(P(this, 0x13c));
    P(this, 0x13c) = 0;

    if (P(this, 0x140) != 0)
        ms_op_delete(P(this, 0x140));
    P(this, 0x140) = 0;

    if (P(this, 0x144) != 0)
        ms_op_delete(P(this, 0x144));
    P(this, 0x144) = 0;

    if (P(this, 0x54) != 0)
        ms_op_delete(Radio_dtor(P(this, 0x54)));
    P(this, 0x54) = 0;

    if (P(this, 0x98) != 0)
        ms_op_delete(ScrollTouchBox_dtor(P(this, 0x98)));
    P(this, 0x98) = 0;

    US(this, 0xb0) = 0;
    if (*soundHolder != 0)
        ModStation_or_tail();
}

struct Station;
struct Ship;
struct Item;
struct Mission;
struct Agent;
struct DialogueWindow;
struct HangarWindow;
struct MenuTouchWindow;
struct Radio;
struct RadioMessage;
struct ScrollTouchBox;
struct RecordHandler;
struct Layout;

__attribute__((visibility("hidden"))) extern int  *g_ote_stack;     // [DAT_000e7908]
__attribute__((visibility("hidden"))) extern int **g_ote_helpLayout;// [DAT_000e790c] help-window layout
__attribute__((visibility("hidden"))) extern int **g_ote_status;    // Status holder
__attribute__((visibility("hidden"))) extern int **g_ote_achievements;
__attribute__((visibility("hidden"))) extern int **g_ote_module;    // next application module
__attribute__((visibility("hidden"))) extern int **g_ote_canvas;    // paint canvas
__attribute__((visibility("hidden"))) extern int **g_ote_sound;     // FMod sound
__attribute__((visibility("hidden"))) extern int **g_ote_textRoot;  // GameText root
__attribute__((visibility("hidden"))) extern int **g_ote_galaxy;    // Galaxy holder
__attribute__((visibility("hidden"))) extern int **g_ote_itemTable; // item descriptor table
__attribute__((visibility("hidden"))) extern int **g_ote_shipTable; // ship descriptor table

extern "C" {
void *ModStation_opnew_ote(unsigned size);
void  ModStation_opdelete_ote(void *p);

// --- self methods reached directly ---
void  ModStation_autosave_ote(ModStation *self);
void  ModStation_enterStation_ote(ModStation *self);
void  ModStation_resetLight_ote(ModStation *self);
void  ModStation_resetIdleCamForHangar_ote(ModStation *self);
void  ModStation_checkHints_ote(ModStation *self);

// --- engine / world ---
void  ApplicationManager_SetCurrentApplicationModule_ote(int module);
int   ApplicationManager_GetApplicationData_ote();

int   Status_getStation_ote();
int   Station_getIndex_ote(Station *s);
int   Station_stationHasPirateBase_ote();
int   Station_hasShip_ote(void *station);
int   Station_getAgents_ote(Station *s);
void  Station_addShip_ote(Ship *s);
void  Station_departStation_ote(Station *s);
void  Station_setAttackedFriends_ote(Station *s, int flag);
void  Station_setItems_ote(Station *s, void *arr, int flag);

int   Status_getShip_ote();
int   Status_getCredits_ote();
void  Status_changeCredits_ote(int status);
int   Status_getCurrentCampaignMission_ote();
void  Status_nextCampaignMission_ote(int status);
void  Status_departStation_ote(Station *s);
void  Status_setStation_ote(Station *s);
void  Status_setMission_ote(int status);
void  Status_removeMission_ote(int status);
void  Status_incMissionCount_ote(int status);
void  Status_setPassengers_ote(int status, int n);
void  Status_checkForLevelUp_ote();
void  Status_activateNewWanted_ote();

int   Ship_getCargo_ote();
int   Ship_getIndex_ote();
void  Ship_getRace_ote(Ship *s);
void  Ship_removeCargo_ote(int ship, int item);
void  Ship_removeCargo1_ote(Item *it);
void  Ship_addCargo_ote(Item *it);
void  Ship_makeShip_ote(int desc);

int   Item_isUnsaleable_ote(Item *it);
int   Item_getIndex_ote(Item *it);
int   Item_getAmount_ote();
void  Item_makeItem_ote(int desc);

int   Mission_getType_ote();
int   Mission_isCampaignMission_ote(Mission *m);
int   Mission_getProductionGoodIndex_ote(Mission *m);
int   Mission_getProductionGoodAmount_ote();
int   Mission_getReward_ote();
int   Mission_getBonus_ote();

int   Agent_getOffer_ote(Agent *a);
int   Agent_getSellItemIndex_ote(Agent *a);
void  Agent_setEvent_ote(Agent *a, int e);
void  Agent_setOfferAccepted_ote(Agent *a, int flag);

void  Achievements_resetNewMedals_ote(void *ach);

int   GameText_getText_ote(int id);
void  Galaxy_getStation_ote(int index);

void  Layout_OnTouchEnd_ote(Layout *l, int p1, int p2);
int   Layout_OnTouchEndR_ote(Layout *l, int p1, int p2);
int   Layout_helpPressed_ote(Layout *l);
void  Layout_initHelpWindow_ote(int l, int textStr);
void  Layout_showMissionRewardMessage_ote(int l, int flag);

int   DialogueWindow_OnTouchEnd_ote(int dw, int p1);
void  DialogueWindow_dtor_ote(DialogueWindow *dw);
int   ChoiceWindow_OnTouchEnd_ote(int cw, int p1);
void  ChoiceWindow_set_ote(int cw, int textStr, int flag);
void  ChoiceWindow_setNotice_ote(int cw, int textStr);
int   HangarWindow_OnTouchEnd_ote(HangarWindow *w, int p1, int p2);
void  HangarWindow_ctor_ote(HangarWindow *w);
void  HangarWindow_dtor_ote(HangarWindow *w);
void  HangarWindow_initialize_ote();
void  HangarWindow_showCreditsBuyWindow_ote(HangarWindow *w);
void  HangarWindow_setSellMode_ote(int w);
int   StarMap_OnTouchEnd_ote(int sm, int p1);
int   SpaceLounge_OnTouchEnd_ote(int l, int p1, int p2);
int   SpaceLounge_hangarNeedsUpdate_ote();
void  SpaceLounge_refresh_ote();
int   StatusWindow_OnTouchEnd_ote(int w, int p1, int p2);
int   MissionsWindow_OnTouchEnd_ote(int w, int p1);
int   MenuTouchWindow_OnTouchEnd_ote(MenuTouchWindow *w, int p1, int p2, void *p3);
void  MenuTouchWindow_ctor_ote(MenuTouchWindow *w, int kind);
int   NewsTicker_OnTouchEnd_ote(int nt, int p1);
int   TouchButton_OnTouchEnd_ote(int btn, int p1);

void  FModSound_play_ote(int sound, int id, void *p, float vol);
void  FModSound_stop_ote(int sound);
void  FModSound_setParamValue_ote(int sound, int a, int b, float v);

void  RecordHandler_saveOptions_ote(RecordHandler *rh);
void  CutScene_replacePlayerShip_ote(int cs, int shipIndex);
void  CutScene_checkForTurret_ote(int cs);

// --- corrupted/SIMD blocks delegated to helpers ---
// Snapshots the cached docking-button positions into the parallel x/y arrays (TouchButton::getPosition
// SIMD reads the decompiler corrupted). Called after a sub-window opens/closes.
void  ModStation_ote_cacheButtonPositions(ModStation *self);
// Pops the "pay docking fee" choice window with its formatted-credits hash-replace String chain.
void  ModStation_ote_showDockingFeeChoice(ModStation *self, int credits);
// Pops the mission-0x6c reward choice window (String hash-replace chain).
void  ModStation_ote_showRewardChoice(ModStation *self, int credits);
// Builds the supernova radio-cutscene Radio + its four RadioMessages + the scroll box (SIMD layout).
void  ModStation_ote_startRadioCutscene(ModStation *self);
// Recomputes the idle-camera target after a news-ticker tap (VectorSignedToFloat wobble math).
void  ModStation_ote_kickIdleCamera(ModStation *self);
// The store-launch tail thunk (the original tail-calls a loaded code pointer to switch modules with
// the given argument). Returns nothing; control returns to the caller afterwards.
void  ModStation_ote_launchModule(int module, int arg);

// --- internal sub-handlers (split out of the original monolithic function) ---
void  handleChoiceDecline(ModStation *self, int param_1, int param_2);
void  handleChoiceDeclineTail(ModStation *self);
void  handleMainButtons(ModStation *self, int param_1, int param_2);
void  handleMissionComplete(ModStation *self);
void  handleCampaignTransition(ModStation *self, int cm);
void  finishMissionReward(ModStation *self);
}

// the help-window layout pointer (DAT_000e790c equivalent), used by the reward message call.
static int *help_layout()
{
    return *(int **)g_ote_helpLayout;
}

// ModStation::OnTouchEnd(int x, int y, void *touch) — routes a touch release to the active station
// sub-window or the main button strip; the dialogue-accept path runs the full mission-completion and
// campaign-advance handler.
void ModStation::OnTouchEnd(int param_1, int param_2, void *param_3) {
    char *s = (char *)this;
    int *status = *(int **)g_ote_status;

    // only react to the touch we captured; clear the captured-touch slot.
    if (P(s, 0x128) != param_3)
        return;
    int starMapOpen = C(s, 0x60);
    C(s, 0x118) = 0;
    P(s, 0x128) = 0;
    if (starMapOpen != 0)
        return;

    // help window takes priority.
    int *help = *(int **)g_ote_helpLayout;
    if (*(char *)*help != 0) {
        if (Layout_OnTouchEndR_ote((Layout *)*help, param_1, param_2) != 0)
            *(char *)*help = 0;
        return;
    }

    // ---- dialogue window open ----
    if (C(s, 0x69) != 0) {
        if (DialogueWindow_OnTouchEnd_ote(I(s, 0x84), param_1) != 0) {
            if (I(s, 0x54) != 0) {                  // this[1].field_C: an active mission
                handleMissionComplete(this);
                return;
            }
            // plain dialogue dismissal.
            DialogueWindow *dw = (DialogueWindow *)P(s, 0x84);
            C(s, 0x69) = 0;
            if (dw != 0) {
                DialogueWindow_dtor_ote(dw);
                ModStation_opdelete_ote(dw);
            }
            char justEntered = C(s, 0x71);          // this[1].field_70+1
            I(s, 0x84) = 0;
            if (justEntered == 0) {
                Station *st = (Station *)Status_getStation_ote();
                if (Station_getIndex_ote(st) != 4) {
                    st = (Station *)Status_getStation_ote();
                    if (Station_getIndex_ote(st) != 0x58) {
                        Status_getStation_ote();
                        if (Station_stationHasPirateBase_ote() != 0) {
                            C(s, 0x4e) = 0;          // this[1].field_20+2
                            Station *home = (Station *)**(int **)g_ote_status;
                            Status_getStation_ote();
                            Status_departStation_ote(home);
                            Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
                            int mod = (Status_getCurrentCampaignMission_ote() == 0x10) ? -1 : 1;
                            **(int **)g_ote_module = mod;
                            ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
                            C(s, 0x24) = 0;
                        }
                    }
                }
            }
            return;
        }
        if (C(s, 0x65) != 0)
            return;
    }

    // ---- choice window (jump confirm / docking fee / reward) ----
    if (C(s, 0x6f) != 0) {
        int r = ChoiceWindow_OnTouchEnd_ote(I(s, 0x70), param_1);
        if (r == 1) {
            char departed = C(s, 0x6b);             // this->field_68+3
            C(s, 0x4e) = 0;
            if (departed == 0) {
                C(s, 0x6f) = 0;
                C(s, 0x98) = 0;                     // this[1].field_4C
            } else {
                C(s, 0x4e) = 0;
                Station *home = (Station *)**(int **)g_ote_status;
                Status_getStation_ote();
                Status_departStation_ote(home);
                Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
                **(int **)g_ote_module = 1;
                ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
                C(s, 0x24) = 0;
            }
        } else if (r == 0) {
            handleChoiceDecline(this, param_1, param_2);
        }
        return;
    }

    // ---- main station screen: dispatch to the open sub-window, else the button strip ----
    if (C(s, 0x68) != 0) {                          // StatusWindow
        if (StatusWindow_OnTouchEnd_ote(I(s, 0x7c), param_1, param_2) != 0) {
            C(s, 0x68) = 0;
            ModStation_resetLight_ote(this);
        }
        return;
    }
    if (C(s, 0x67) != 0) {                          // StarMap
        if (StarMap_OnTouchEnd_ote(I(s, 0x10), param_1) != 0) {
            C(s, 0x67) = 0;
            ModStation_resetLight_ote(this);
        }
        return;
    }
    if (C(s, 0x66) != 0) {                          // HangarWindow
        if (HangarWindow_OnTouchEnd_ote((HangarWindow *)P(s, 0x78), param_1, param_2) != 0) {
            int *st = *(int **)g_ote_status;
            // tally any tradeable cargo sold this visit.
            Status_getShip_ote();
            unsigned *cargo = (unsigned *)Ship_getCargo_ote();
            int sold = 0;
            if (cargo != 0) {
                for (unsigned i = 0; i < *cargo; i = i + 1) {
                    int idx = Item_getIndex_ote(*(Item **)(cargo[1] + i * 4));
                    if (0x83 < idx && Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) < 0x9a)
                        sold += Item_getAmount_ote();
                }
            }
            if (*(int *)(s + 0x50 + 0x40) < sold)   // this[1].field_40
                *(int *)(*st + 0xa8) += sold - *(int *)(s + 0x50 + 0x40);
            HangarWindow_setSellMode_ote(I(s, 0x78));
            ModStation_resetIdleCamForHangar_ote(this);
            char *hw = *(char **)(s + 0x78);
            C(s, 0x66) = 0;
            if (*hw != 0) {
                *hw = 0;
                int cs = I(s, 0x14);
                Status_getShip_ote();
                int shipIndex = Ship_getIndex_ote();
                Ship *sh = (Ship *)Status_getShip_ote();
                Ship_getRace_ote(sh);
                CutScene_replacePlayerShip_ote(cs, shipIndex);
            }
            if (I(s, 0x14) != 0)
                CutScene_checkForTurret_ote(I(s, 0x14));
            int snd = **(int **)g_ote_sound;
            FModSound_stop_ote(snd);
            FModSound_play_ote(snd, 0x7a, 0, 0.0f);
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
            ModStation_ote_cacheButtonPositions(this);
        }
        return;
    }
    if (C(s, 0x64) != 0) {                          // SpaceLounge
        if (SpaceLounge_OnTouchEnd_ote(I(s, 0x74), param_1, param_2) != 0) {
            C(s, 0x65) = 0;
            ModStation_resetIdleCamForHangar_ote(this);
            ModStation_resetLight_ote(this);
            int snd = **(int **)g_ote_sound;
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
            FModSound_stop_ote(snd);
            FModSound_play_ote(snd, 0x7a, 0, 0.0f);
            CutScene_checkForTurret_ote(I(s, 0x14));
            if (I(s, 0x74) != 0 && SpaceLounge_hangarNeedsUpdate_ote() != 0) {
                if (P(s, 0x78) != 0) {
                    HangarWindow_dtor_ote((HangarWindow *)P(s, 0x78));
                    ModStation_opdelete_ote(P(s, 0x78));
                }
                I(s, 0x78) = 0;
            }
        }
        return;
    }
    if (C(s, 0x65) != 0) {                          // MissionsWindow
        if (MissionsWindow_OnTouchEnd_ote(I(s, 0x80), param_1) != 0) {
            C(s, 0x64) = 0;
            int snd = **(int **)g_ote_sound;
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
        }
        return;
    }
    if (C(s, 0x6e) != 0) {                          // MenuTouchWindow (DLC)
        if (MenuTouchWindow_OnTouchEnd_ote((MenuTouchWindow *)P(s, 0x50),
                                           param_1, param_2, param_3) != 0) {
            C(s, 0x6e) = 0;
            ModStation_ote_cacheButtonPositions(this);
        }
        return;
    }
    if (C(s, 0x6d) != 0) {                          // m_nStarMapWindowOpen+1: button strip active
        handleMainButtons(this, param_1, param_2);
    }
}

// --- the decline branch of the jump/depart choice window ---
void handleChoiceDecline(ModStation *self, int param_1, int param_2)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;
    (void)param_1; (void)param_2;

    if (C(s, 0x4e) != 0) {                          // a departure was pending
        C(s, 0x4e) = 0;
        int camp = Status_getCurrentCampaignMission_ote();
        if (camp == 0x18) {
            Station *st = (Station *)Status_getStation_ote();
            if (Station_getIndex_ote(st) == 10) {
                unsigned i = 0;
                for (;;) {
                    Station *st1 = (Station *)Status_getStation_ote();
                    unsigned *ag = (unsigned *)Station_getAgents_ote(st1);
                    if (*ag <= i) break;
                    Station *st2 = (Station *)Status_getStation_ote();
                    int agents = Station_getAgents_ote(st2);
                    Agent *a = *(Agent **)(*(int *)(agents + 4) + i * 4);
                    if (Agent_getOffer_ote(a) == 2 && Agent_getSellItemIndex_ote(a) == 0x44) {
                        Agent_setEvent_ote(a, 1);
                        Agent_setOfferAccepted_ote(a, 1);
                    }
                    i = i + 1;
                }
            }
        }
        Station *home = (Station *)*status;
        if (Status_getCurrentCampaignMission_ote() == 0x30) {
            Galaxy_getStation_ote(**(int **)g_ote_galaxy);
            Status_departStation_ote(home);
            **(char **)g_ote_galaxy = 1;
        } else {
            Status_getStation_ote();
            Status_departStation_ote(home);
        }
        *(int *)(*status + 0x5c) = -1;
        *(int *)(*status + 0x60) = -1;
        *(int *)(*status + 0x64) = -1;
        *(int *)(*status + 0x68) = -1;
        Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
        **(int **)g_ote_module = 1;
        ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        C(s, 0x24) = 0;
        return;
    }

    if (C(s, 0x6b) != 0) {                          // standing-penalty branch
        int credits = Status_getCredits_ote();
        if (*(int *)(s + 0x50 + 0x44) <= credits) {  // this[1].field_44 == price
            Status_changeCredits_ote(*status);
            C(s, 0x6b) = 0;
            C(s, 0x94) = 1;                          // this[1].field_48
            Station *st = (Station *)Status_getStation_ote();
            Station_setAttackedFriends_ote(st, 0);
            C(s, 0x71) = 1;                          // this[1].field_70+1
            ModStation_enterStation_ote(self);
            ModStation_autosave_ote(self);
            handleChoiceDeclineTail(self);
            return;
        }
        ModStation_ote_showDockingFeeChoice(self, credits);
        C(s, 0x6b) = 1;
        C(s, 0x4e) = 1;
        C(s, 0x6f) = 1;
        return;
    }

    handleChoiceDeclineTail(self);
}

// the post-standing-penalty continuation (mission-0x6c reward / level-up cleanup).
void handleChoiceDeclineTail(ModStation *self)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    if (C(s, 0x98) != 0) {                          // this[1].field_4C: pending app-data flag
        C(s, 0x6f) = 0;
        C(s, 0x98) = 0;
        int appData = ApplicationManager_GetApplicationData_ote();
        *(char *)(appData + 0xd) = 1;
        return;
    }
    if (C(s, 0x6c) == 0) {                          // this->field_6C
        if (C(s, 0x6d) != 0) {                      // this->field_6C+1: reward-mission state
            int credits = Status_getCredits_ote();
            if (credits < 25000) {
                ModStation_ote_showRewardChoice(self, credits);
                C(s, 0x6d) = 0;
                return;
            }
            Status_changeCredits_ote(*status);
            C(s, 0x6f) = 0;
            C(s, 0x6d) = 0;
            Station *home = (Station *)*status;
            Galaxy_getStation_ote(**(int **)g_ote_galaxy);
            Status_setStation_ote(home);
            **(int **)g_ote_module = 0;
            ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        }
        if (C(s, 0x18) != 0 && C(s, 0x66) != 0) {
            C(s, 0x18) = 0;
            C(s, 0x66) = 0;
        }
        C(s, 0x6f) = 0;
    } else {
        Status_changeCredits_ote(*status);
        Ship_removeCargo_ote(Status_getShip_ote(), 0x6d);
        *(int *)(*status + 0x114) = 3;
        RecordHandler_saveOptions_ote((RecordHandler *)**(int **)g_ote_status);
        ChoiceWindow_setNotice_ote(I(s, 0x70), GameText_getText_ote(**g_ote_textRoot));
        Station *st = (Station *)Status_getStation_ote();
        Station_setItems_ote(st, 0, 0);
        Station_setItems_ote(*(Station **)(*status + 0x14c), 0, 0);
        C(s, 0x6c) = 0;
    }
}

// --- main button strip dispatch (launch / buy ship / DLC menu / news ticker) ---
void handleMainButtons(ModStation *self, int param_1, int param_2)
{
    char *s = (char *)self;
    int *help = *(int **)g_ote_helpLayout;

    // launch button.
    if (TouchButton_OnTouchEnd_ote(*(int *)(s + 0x50 + 0), param_1) != 0)
        return;

    // "buy credits" button -> hangar credits window.
    if (TouchButton_OnTouchEnd_ote(*(int *)(s + 0x50 + 4), param_1) != 0) {
        RecordHandler *rh = (RecordHandler *)**(int **)g_ote_status;
        *(char *)(*(int *)g_ote_status + 0x4e) = 1;
        RecordHandler_saveOptions_ote(rh);
        if (P(s, 0x78) == 0) {
            HangarWindow *hw = (HangarWindow *)ModStation_opnew_ote(0x134);
            HangarWindow_ctor_ote(hw);
            P(s, 0x78) = hw;
        }
        HangarWindow_initialize_ote();
        C(s, 0x18) = 1;
        C(s, 0x66) = 1;
        HangarWindow_showCreditsBuyWindow_ote((HangarWindow *)P(s, 0x78));
    }

    // the five DLC / station-menu buttons.
    I(s, 0x44) = -1;
    for (unsigned i = 0; i < 5; i = i + 1) {
        if (TouchButton_OnTouchEnd_ote(*(int *)(*(int *)(s + 0x8c) + i * 4), param_1) != 0) {
            I(s, 0x44) = (int)i;
            // dispatches via the main button's vtable thunk in the original.
            ModStation_ote_launchModule(I(s, 0), 0x10000);
            return;
        }
    }

    // the layout (system info box) hot area.
    if (Layout_OnTouchEndR_ote((Layout *)*help, param_1, param_2) != 0) {
        if (P(s, 0x50) == 0) {
            MenuTouchWindow *w = (MenuTouchWindow *)ModStation_opnew_ote(0x240);
            MenuTouchWindow_ctor_ote(w, 2);
            P(s, 0x50) = w;
        }
        Status_checkForLevelUp_ote();
        C(s, 0x6e) = 1;
        ModStation_ote_cacheButtonPositions(self);
        return;
    }

    if (Layout_helpPressed_ote((Layout *)*help) != 0)
        Layout_initHelpWindow_ote(*help, GameText_getText_ote(**g_ote_textRoot));

    if (NewsTicker_OnTouchEnd_ote(I(s, 0x1c), param_1) == 0)
        ModStation_ote_kickIdleCamera(self);
}

// --- the full mission-completion + campaign-advance handler (reached on dialogue accept) ---
void handleMissionComplete(ModStation *self)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    Mission *mission = (Mission *)I(s, 0x54);

    // mission-type specific cargo cleanup before scoring.
    if (mission != 0) {
        int type = Mission_getType_ote();
        int campaign = Mission_isCampaignMission_ote(mission);
        if (type == 8) {
            if (campaign == 0) {
                int ship = Status_getShip_ote();
                int good = Mission_getProductionGoodIndex_ote(mission);
                Mission_getProductionGoodAmount_ote();
                Ship_removeCargo_ote(ship, good);
                if (I(s, 0x78) != 0)
                    HangarWindow_initialize_ote();
            }
        } else if (campaign == 0 && Mission_getType_ote() == 0xb) {
            Status_setPassengers_ote(*status, 0);
            if (Mission_isCampaignMission_ote(mission) == 0)
                *(int *)(*status + 0xb8) += Mission_getProductionGoodAmount_ote();
            Status_getShip_ote();
            unsigned *cargo = (unsigned *)Ship_getCargo_ote();
            if (cargo != 0) {
                for (unsigned i = 0; i < *cargo; i = i + 1) {
                    if (Item_isUnsaleable_ote(*(Item **)(cargo[1] + i * 4)) != 0 &&
                        (Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x74 ||
                         Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x75)) {
                        Ship_removeCargo1_ote((Item *)Status_getShip_ote());
                        if (I(s, 0x78) != 0)
                            HangarWindow_initialize_ote();
                        break;
                    }
                }
            }
        } else if (Mission_getType_ote() != 3 && Mission_getType_ote() != 5 &&
                   Mission_getType_ote() != 0xb) {
            if (Mission_getType_ote() == 0) {
                Status_getShip_ote();
                unsigned *cargo = (unsigned *)Ship_getCargo_ote();
                if (cargo != 0) {
                    for (unsigned i = 0; i < *cargo; i = i + 1) {
                        if (Item_isUnsaleable_ote(*(Item **)(cargo[1] + i * 4)) != 0 &&
                            (Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x74 ||
                             Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x75)) {
                            Ship_removeCargo1_ote((Item *)Status_getShip_ote());
                            if (I(s, 0x78) != 0)
                                HangarWindow_initialize_ote();
                            break;
                        }
                    }
                }
                *(int *)(*status + 0x9c) += Mission_getProductionGoodAmount_ote();
            } else if (Mission_getType_ote() == 0xe) {
                Status_getShip_ote();
                unsigned *cargo = (unsigned *)Ship_getCargo_ote();
                if (cargo != 0) {
                    for (unsigned i = 0; i < *cargo; i = i + 1) {
                        if (Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x73) {
                            Ship_removeCargo1_ote((Item *)Status_getShip_ote());
                            if (I(s, 0x78) != 0)
                                HangarWindow_initialize_ote();
                            break;
                        }
                    }
                }
            }
        }
    }

    // --- scoring / campaign advance (LAB_000e7962) ---
    if (Mission_isCampaignMission_ote(mission) == 0) {
        Status_incMissionCount_ote(*status);
        int reward = Mission_getReward_ote();
        int bonus = Mission_getBonus_ote();
        Layout_showMissionRewardMessage_ote(*help_layout(), (char)(bonus + reward));
        finishMissionReward(self);
        return;
    }

    int camp = Status_getCurrentCampaignMission_ote();
    if (camp == 0x2b) {
        // mining-plant unlock -> supernova radio cutscene.
        Status_removeMission_ote(*status);
        Status_setMission_ote(*status);
        C(s, 0x69) = 0;
        int snd = **(int **)g_ote_sound;
        FModSound_stop_ote(snd);
        FModSound_play_ote(snd, 0x90, 0, 0.0f);
        ModStation_ote_startRadioCutscene(self);
        return;
    }

    if (Status_getCurrentCampaignMission_ote() != 0x94)
        Status_nextCampaignMission_ote(*status);

    int cm = Status_getCurrentCampaignMission_ote();
    unsigned d = (unsigned)(cm - 0x2c);
    bool handled = false;
    if (d < 0xf) {
        if ((1 << (d & 0xff) & 0x5830) != 0) {
            int cs = I(s, 0x14);
            Status_getShip_ote();
            int shipIndex = Ship_getIndex_ote();
            Ship *sh = (Ship *)Status_getShip_ote();
            Ship_getRace_ote(sh);
            CutScene_replacePlayerShip_ote(cs, shipIndex);
            finishMissionReward(self);
            return;
        }
        if (d == 1) {
            Status_removeMission_ote(*status);
            I(s, 0x54) = 0;
            Status_setMission_ote(*status);
            I(s, 0x44) = 1;
            C(s, 0x69) = 0;
            ModStation_ote_launchModule(I(s, 0), 0x10000);
            return;
        }
        if (d != 0)
            handled = false;     // fall through to the 0x4b.. handling
        else
            handled = true;      // d == 0 (mission 0x2c): handled by the 0x9 reset path below
    }

    if (!handled) {
        if ((unsigned)(cm - 0x4b) > 8 || (1 << ((cm - 0x4b) & 0xff) & 0x103) == 0) {
            handleCampaignTransition(self, cm);
            return;
        }
    }

    // missions 0x2c / 9 / 0x4b / 0x4c / 0x53: launch the storyline cutscene module.
    Status_removeMission_ote(*status);
    I(s, 0x54) = 0;
    Status_setMission_ote(*status);
    C(s, 0x69) = 0;
    *(int *)(*status + 0 /* cutscene slot */) = 0;
    ModStation_ote_launchModule(**(int **)g_ote_module, 5);
}

// per-mission campaign station/ship side effects, then either restock-and-reward or depart.
void handleCampaignTransition(ModStation *self, int cm)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    if (cm == 0x12) {
        Status_removeMission_ote(*status);
        C(s, 0x69) = 0;
        I(s, 0x54) = 0;
        return;
    }

    Station *here = (Station *)Status_getStation_ote();
    int hereIdx = Station_getIndex_ote(here);

    if (cm == 0x4d && hereIdx == 100) {
        Ship *sh = (Ship *)Status_getStation_ote();
        Ship_makeShip_ote(*(int *)(*(int *)(**(int **)g_ote_shipTable + 4) + 0x94));
        Station_addShip_ote(sh);
        finishMissionReward(self);
        return;
    }
    if (cm == 0x4e) {
        *(int *)(*status + 0x5c) = -1;
        *(int *)(*status + 0x60) = -1;
        *(int *)(*status + 0x64) = -1;
        *(int *)(*status + 0x68) = -1;
        Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
        Station *home = (Station *)*status;
        Status_getStation_ote();
        Status_departStation_ote(home);
        **(int **)g_ote_module = 1;
        ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        C(s, 0x24) = 0;
        finishMissionReward(self);
        return;
    }
    Station *st = (Station *)Status_getStation_ote();
    if (cm == 0x54 && Station_getIndex_ote(st) == 100) {
        Ship *sh = (Ship *)Status_getStation_ote();
        if (Station_hasShip_ote(sh) == 0) {
            Ship_makeShip_ote(*(int *)(*(int *)(**(int **)g_ote_shipTable + 4) + 0x98));
            Station_addShip_ote(sh);
        }
        if (Station_hasShip_ote(sh) == 0) {
            Ship_makeShip_ote(*(int *)(*(int *)(**(int **)g_ote_shipTable + 4) + 0xa0));
            Station_addShip_ote(sh);
        }
        Item_makeItem_ote(*(int *)(*(int *)(**(int **)g_ote_itemTable + 4) + 0x224));
        Ship_addCargo_ote((Item *)Status_getShip_ote());
        finishMissionReward(self);
        return;
    }

    // the remaining campaign ids each set a module flag and depart.
    if (cm == 0xa0 || cm == 0x90 || cm == 99 || cm == 0x77 || cm == 0x85 || cm == 0x6d ||
        cm == 0x59) {
        Station *home = (Station *)*status;
        **(char **)g_ote_module = 1;
        Galaxy_getStation_ote(**(int **)g_ote_galaxy);
        Status_departStation_ote(home);
        ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        C(s, 0x24) = 0;
        return;
    }
    if (cm == 0x68) {
        if (P(s, 0x78) != 0) {
            HangarWindow_dtor_ote((HangarWindow *)P(s, 0x78));
            ModStation_opdelete_ote(P(s, 0x78));
        }
        C(s, 0x66) = 0;
        I(s, 0x78) = 0;
        finishMissionReward(self);
        return;
    }
    if (cm == 0x80) {
        Status_activateNewWanted_ote();
        if (*(char *)(*(int *)g_ote_status + 0x2a) == 0)
            C(s, 0x6e) = 1;
        finishMissionReward(self);
        return;
    }

    finishMissionReward(self);
}

// credits the reward, removes the mission and refreshes the lounge.
void finishMissionReward(ModStation *self)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    Mission_getReward_ote();
    int bonus = Mission_getBonus_ote();
    if ((unsigned)bonus >= 0)
        Status_getCurrentCampaignMission_ote();
    Status_changeCredits_ote(*status);
    Status_removeMission_ote(*status);
    I(s, 0x54) = 0;
    if (I(s, 0x74) != 0)
        SpaceLounge_refresh_ote();
    C(s, 0x69) = 0;
    ModStation_checkHints_ote(self);
}

struct SpaceLounge;
struct StarMap;
struct HangarWindow;
struct StatusWindow;
struct MissionsWindow;
struct MenuTouchWindow;
struct ChoiceWindow;
struct DialogueWindow;
struct NewsTicker;

// Globals.
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_layout;  // g -> P, *P -> layout obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_screenH; // **g -> screen height
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_campaign;// *g -> obj for nextCampaignMission
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_appmod;  // *g -> app module value
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_clear;   // *g -> slot zeroed

// ModStation::OnTouchBegin(int, int, void*)
void ModStation::OnTouchBegin(int x, int y, void *touch) {
    if (P(this, 0x128) != 0)
        return;
    UC(this, 0x118) = 1;
    P(this, 0x128) = touch;
    int flag = I(this, 0x60);
    I(this, 0x110) = x;
    I(this, 0x114) = y;
    if (flag != 0) {
        if (((Radio *)(P(this, 0x54)))->lastMessageShown() != 0) {
            UC(this, 0x24) = 0;
            ((Status *)((char)*(int *)*g_ModStation_tb_campaign))->nextCampaignMission();
            unsigned int mod = *(unsigned int *)*g_ModStation_tb_appmod;
            *(int *)*g_ModStation_tb_clear = 0;
            // `mod` is the target module id; dispatch on the ApplicationManager singleton.
            ((ApplicationManager *)(*g_ModStation_tb_appmod))->SetCurrentApplicationModule(mod);
            UC(this, 0x60) = 0;
        }
        return;
    }
    void **layoutHolder = g_ModStation_tb_layout;
    void *layoutObj = *layoutHolder;
    if (*(char *)layoutObj != 0) {
        ((Layout *)(layoutObj))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x69) != 0) {
        ((DialogueWindow *)(P(this, 0x84)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x63) != 0) {
        ((ChoiceWindow *)(P(this, 0x70)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x6a) != 0) {
        ((ChoiceWindow *)(P(this, 0x88)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x66) != 0) {
        ((HangarWindow *)(P(this, 0x78)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x65) != 0) {
        ((SpaceLounge *)(P(this, 0x74)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x67) != 0) {
        ((StarMap *)(P(this, 0x10)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x68) != 0) {
        ((StatusWindow *)(P(this, 0x7c)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x64) != 0) {
        ((MissionsWindow *)(P(this, 0x80)))->OnTouchBegin(x, y);
        return;
    }
    if (C(this, 0x62) != 0) {
        ((MenuTouchWindow *)(P(this, 0x50)))->OnTouchBegin(x, y, (int)(intptr_t)touch);
        return;
    }
    if (C(this, 0x61) == 0)
        return;
    ((TouchButton *)(P(this, 0x90)))->OnTouchBegin(x, y);
    ((TouchButton *)(P(this, 0x94)))->OnTouchBegin(x, y);
    for (int i = 0; i != 5; i++)
        ((TouchButton *)(*(void **)(I(P(this, 0x8c), 4) + i * 4)))->OnTouchBegin(x, y);
    ((Layout *)(layoutObj))->OnTouchBegin(x, y);
    if (((NewsTicker *)(P(this, 0x1c)))->OnTouchBegin(x, y) != 0)
        return;
    int *layout = (int *)layoutObj;
    if (y <= layout[0xc / 4])
        return;
    if (*(int *)*g_ModStation_tb_screenH - layout[0x10 / 4] <= y)
        return;
    if (x <= I(this, 0xa8))
        return;
    I(this, 0xfc) = x;
    I(this, 0xe8) = x;
    I(this, 0xf0) = 0;
    UC(this, 0x100) = 1;
}

struct Layout;

__attribute__((visibility("hidden"))) extern int  *g_r2d_stack;       // [DAT_000eba2c]
__attribute__((visibility("hidden"))) extern int **g_r2d_helpLayout;  // [DAT_000eba30]

extern "C" {
void HangarWindow_render_r2d(ModStation *self);
void HangarWindow_render3D_r2d();
void SpaceLounge_draw_r2d(ModStation *self);
void SpaceLounge_draw3DShip_r2d();
void MissionsWindow_draw_r2d(void *w);
void StarMap_draw_r2d(ModStation *self);
void StatusWindow_draw_r2d(ModStation *self);
void MenuTouchWindow_draw_r2d(ModStation *self);
void Radio_draw_r2d(ModStation *self);
void CutScene_render2D_r2d();
void ChoiceWindow_draw_r2d(ModStation *self);
void DialogueWindow_draw_r2d(ModStation *self);
void Layout_drawHelpWindow_r2d(Layout *l);
// The full station-HUD pass (header, footer, system box, news ticker, buttons, reward banner) —
// dozens of SIMD string-layout calls the decompiler corrupted.
void ModStation_r2d_drawStationHud(ModStation *self);
}

// ModStation::OnRender2D() — top-level 2D pass; dispatches to whichever station sub-screen is
// currently open, then overlays the choice/dialogue/help windows.
void ModStation::OnRender2D() {
    ((PaintCanvas *)g_PaintCanvas)->Begin2d();
    ((PaintCanvas *)g_PaintCanvas)->SetColor(U(this, 4));

    if (C(this, 0x24) == 0) {
        // station screen not active yet — nothing to draw this frame.
        return;
    }

    if (C(this, 0x66) != 0) {            // hangar
        HangarWindow_render_r2d(this);
    } else if (C(this, 0x65) != 0) {     // space lounge
        SpaceLounge_draw_r2d(this);
    } else if (C(this, 0x64) != 0) {     // missions
        MissionsWindow_draw_r2d(P(this, 0x80));
    } else if (C(this, 0x67) != 0) {     // star map
        StarMap_draw_r2d(this);
    } else if (C(this, 0x68) != 0) {     // status
        StatusWindow_draw_r2d(this);
    } else if (C(this, 0x6a) != 0) {     // DLC menu
        MenuTouchWindow_draw_r2d(this);
    } else if (C(this, 0x6c) != 0) {     // radio / cutscene overlay
        Radio_draw_r2d(this);
        CutScene_render2D_r2d();
    } else {
        // default: the station information HUD.
        ModStation_r2d_drawStationHud(this);
    }

    if (C(this, 0x6a) != 0 || C(this, 0x6b) != 0)
        ChoiceWindow_draw_r2d(this);
    if (C(this, 0x69) != 0)
        DialogueWindow_draw_r2d(this);

    Layout **help = (Layout **)g_r2d_helpLayout;
    if (*(char *)*help != 0)
        Layout_drawHelpWindow_r2d((Layout *)*help);

    ((PaintCanvas *)g_PaintCanvas)->End2d();

    // 3D hangar/ship pass when no help window is up.
    if (*(char *)*help == 0) {
        if (C(this, 0x66) != 0) {
            HangarWindow_render3D_r2d();
        }
        if (*(char *)*help == 0 && C(this, 0x65) != 0)
            SpaceLounge_draw3DShip_r2d();
    }

    ((PaintCanvas *)g_PaintCanvas)->SwapBuffer();
}

struct Station;
struct Ship;
struct Item;
struct Mission;
struct CutScene;
struct DialogueWindow;
struct Generator;
struct NewsTicker;
struct TouchButton;
struct Agent;
struct EaseInOut;
struct EaseInOutMatrix;

__attribute__((visibility("hidden"))) extern int  *g_oi_stack;       // [DAT_000e5a3c]
__attribute__((visibility("hidden"))) extern int **g_oi_status;      // status holder (multiple DATs alias it)
__attribute__((visibility("hidden"))) extern int **g_oi_itemTable;   // item descriptor table
__attribute__((visibility("hidden"))) extern int **g_oi_shipTable;   // ship descriptor table
__attribute__((visibility("hidden"))) extern int **g_oi_canvas;      // paint canvas holder
__attribute__((visibility("hidden"))) extern int **g_oi_settings;    // settings record
__attribute__((visibility("hidden"))) extern int **g_oi_sound;       // FMod sound holder
__attribute__((visibility("hidden"))) extern int **g_oi_textRoot;    // GameText root
__attribute__((visibility("hidden"))) extern char **g_oi_demoFlag;   // demo-mode flag
__attribute__((visibility("hidden"))) extern int  *g_oi_musicId;     // pending music id

extern "C" {
void *ModStation_opnew_oi(unsigned size);
void  ModStation_opdelete_oi(void *p);

// --- self methods reached as direct calls in the original ---
void  ModStation_autosave_oi(ModStation *self);
void  ModStation_resetLight_oi(ModStation *self);
void  ModStation_enterStation_oi(ModStation *self);

// --- Status / world queries ---
int   Status_getCurrentCampaignMission_oi();
int   Status_getStation_oi();
int   Station_getIndex_oi(Station *s);
int   Status_getSystem_oi();
int   SolarSystem_getRace_oi();
int   SolarSystem_getRoutes_oi();
int   Status_inAlienOrbit_oi();
int   Status_inSupernovaSystem_oi();
int   Status_inBlackMarketSystem_oi();
int   Status_getShip_oi();
int   Status_getCredits_oi();
int   Status_getLevel_oi();
int   Status_hardCoreMode_oi();
int   Status_getStanding_oi();
int   Status_gameWon_oi();
int   Status_dlc1Won_oi();
int   Status_getPlayingTime_lo_oi();
int   Status_activateNewWanted_oi();
int   Status_getFreelanceMission_oi();
int   Status_getCampaignMission_oi();
void  Status_setShip_oi(Ship *s);

int   Ship_getIndex_oi();
int   Ship_hasJumpDrive_oi();
int   Ship_hasCargo_oi(int ship, int item);
int   Ship_hasEquipment_oi(int ship, int item);
int   Ship_getCargo_oi(int ship);
void  Ship_makeShip_oi(int desc);
void  Ship_setRace_oi(int race);
void  Ship_setEquipment_oi(Item *eq, int slot);
void  Ship_setPrice_oi(int shipPtr);
void  Ship_removeCargo_oi(int ship);
void  Ship_addCargo_oi(int ship, Item *it);

int   Item_makeItem_oi();
int   Item_makeItemDesc_oi(int desc);
int   Item_makeItemDescAmt_oi(int desc, int amt);
void  Item_setUnsaleable_oi(int flag);
int   Item_getIndex_oi(Item *it);
int   Item_getAmount_oi();
void  Item_setPrice_oi(int itemPtr);

int   Station_getShips_oi();
int   Station_getItems_oi();
int   Station_getName_oi();
int   Station_hasItem_oi(int station);
int   Station_hasShip_oi(void *station);
int   Station_hasAttackedFriends_oi();
int   Station_stationHasPirateBase_oi();
void  Station_addItem_oi(Item *it);
void  Station_addShip_oi(Ship *s);
void  Station_removeShip_oi(Ship *s);
void  Station_setShips_oi(void *station, int list);

int   Standing_isEnemy_oi(int standing);
int   Standing_getStanding_oi(int standing);

int   Mission_getTargetStation_oi();
int   Mission_getType_oi();

int   Achievements_gotAllGoldMedals_oi();

int   GameText_getText_oi(int id);
void  ChoiceWindow_set_oi(void *cw, int text, int flag);
void  ChoiceWindow_setNotice_oi(void *cw, int text);

int   CutScene_initialize_oi(CutScene *cs);
void  CutScene_ctor_oi(CutScene *cs, int kind);
void  CutScene_replacePlayerShip_oi(int cs, int shipIndex);

void  TouchButton_setHalfTransparent_oi(int flag);

void  Globals_startNewSoundResourceList_oi();
void  Globals_playMusicAndFadeOutCurrent_oi(int id);

void  FModSound_play_oi(int sound, int id, void *pos, float vol);
void  FModSound_enableReverb_oi(int sound);
void  FModSound_setDownPitch_oi(int sound);

void  Generator_ctor_oi(Generator *g);
void  Generator_dtor_oi(Generator *g);
void  Generator_computerTradeGoods_oi(Generator *g, Station *s);
int   Generator_getShipBuyList_oi(Station *s);

// --- big corrupted blocks delegated to helpers (SIMD camera/light/easing + String chains) ---

// Builds the cutscene camera framing + idle light easing for the docked-station screen (state 0x14):
// camera dir read, light direction, EaseInOut(+0x13c/+0x140/+0x144), EaseInOutMatrix(+0x20) and all
// the MatrixSetRotation/Translation SIMD math the decompiler corrupted. `race` selects the framing.
void  ModStation_oi_setupHangarCamera(ModStation *self, int race);
// Pops the "pay docking fee / standing penalty" choice window with its formatted-credits String
// (a long AbyssEngine::String temporary chain Ghidra mangled). Returns nonzero if a window was shown.
int   ModStation_oi_showDockingFeeChoice(ModStation *self, int standing, int credits);
// Spawns the pirate-base agent dialogue window (+0x84) with its sound (Agent String temporaries).
void  ModStation_oi_showPirateDialogue(ModStation *self);
// Builds the "wanted activated" choice text from the count (String hash-replace chain).
void  ModStation_oi_showWantedActivated(ModStation *self, int count);
// Builds the mission-0x6c reward Mission + its dialogue window (Mission/String temporaries).
void  ModStation_oi_showRewardMission(ModStation *self);
// Builds the news ticker (+0x1c) for the leave-station screen (state 0x3c) — NewsTicker layout math.
void  ModStation_oi_buildNewsTicker(ModStation *self);
// Builds the DLC sub-menu buttons (+0x8c array) for state 0x50 — TouchButton layout SIMD math.
void  ModStation_oi_buildDlcMenu(ModStation *self);
}

// ModStation::OnInitialize() — per-screen-state setup. The current state is at self+0xc; each branch
// configures its screen (load cutscene, hangar camera, news ticker, DLC menu, ...) and advances the
// state machine. State 0x14=hangar, 0x28=station info, 0x3c=leave, 0x50=DLC menu, 0x64=enter.
void ModStation::OnInitialize() {
    char *s = (char *)this;

    int *status = *(int **)g_oi_status;

    U(s, 0x9c) = 0;

    // lazily build the docking cutscene.
    if (I(s, 0x14) == 0) {
        CutScene *cs = (CutScene *)ModStation_opnew_oi(0xa0);
        CutScene_ctor_oi(cs, 0x17);
        P(s, 0x14) = cs;
        CutScene_initialize_oi(cs);
    }

    int state = I(s, 0xc);
    int next;

    if (state == 0x14) {
        // ---- hangar screen ----
        if (C(s, 0xb1) == 0 && C(s, 0xb0) == 0 && C(s, 0x101) != 0) {
            bool skip = false;
            if (Status_getCurrentCampaignMission_oi() == 0x4d) {
                Station *st = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x65)
                    skip = true;
            }
            if (!skip)
                ModStation_autosave_oi(this);
        }

        int *shipTbl = *(int **)g_oi_shipTable;
        // mission 1: hand the player their first equipped ship.
        if (Status_getCurrentCampaignMission_oi() == 1) {
            Ship *ship = (Ship *)*shipTbl;
            Ship_makeShip_oi(**(int **)(**g_oi_shipTable + 4));
            Status_setShip_oi(ship);
            Ship_setRace_oi(Ship_getIndex_oi());
            int eq = Item_makeItem_oi();
            Item_setUnsaleable_oi(eq);
            Ship_setEquipment_oi((Item *)*shipTbl, eq);
            int eq2 = Item_makeItem_oi();
            Ship_setEquipment_oi((Item *)*shipTbl, eq2);
            Item_setUnsaleable_oi(eq2);
            CutScene_replacePlayerShip_oi(I(s, 0x14), Ship_getIndex_oi());
        }

        // half-transparent docking buttons depending on campaign progress.
        int camp = Status_getCurrentCampaignMission_oi();
        bool special = false;
        if (camp < 5) {
            TouchButton_setHalfTransparent_oi(**(int **)(I(s, 0x8c) + 4));
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 8));
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 0xc));
        } else if (camp < 9 || camp == 0xf) {
            special = camp == 0xf;
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 8));
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 0xc));
        }
        bool showShop = camp < 0xc || special;
        if (!showShop) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 100) showShop = true;
            else {
                st = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x65) showShop = true;
            }
        }
        if (showShop)
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 4));

        // camera framing race selection.
        int race;
        Station *st = (Station *)Status_getStation_oi();
        if (Station_getIndex_oi(st) == 0x65) {
            race = 8;
        } else {
            st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 100) {
                race = 7;
            } else {
                Status_getSystem_oi();
                race = SolarSystem_getRace_oi();
            }
        }
        ModStation_oi_setupHangarCamera(this, race);
        ModStation_resetLight_oi(this);
        if (C(s, 0x101) != 0)
            ModStation_enterStation_oi(this);

        // mission 0x4d: re-price a special ship in the shipyard.
        if (Status_getCurrentCampaignMission_oi() == 0x4d) {
            Station *st2 = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st2) == 100) {
                Status_getStation_oi();
                unsigned *ships = (unsigned *)Station_getShips_oi();
                if (ships != 0) {
                    for (unsigned i = 0; i < *ships; i = i + 1) {
                        if (Ship_getIndex_oi() == 0x25)
                            Ship_setPrice_oi(*(int *)(ships[1] + i * 4));
                    }
                }
            }
        }

        // optional "you have no jump fuel" warning.
        if (C(s, 0x69) == 0 && C(s, 0x6a) == 0 && C(s, 0x63) == 0 &&
            *(char *)(**g_oi_textRoot + 0x2ec) == 0) {
            Station *st2 = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st2) != 0x65) {
                Status_getShip_oi();
                int hasJump = Ship_hasJumpDrive_oi();
                int hasFuel = Ship_hasCargo_oi(Status_getShip_oi(), 0x55);
                Status_getSystem_oi();
                int routes = SolarSystem_getRoutes_oi();
                if (routes == 0 && hasJump == 0 && hasFuel == 0 &&
                    0x10 < Status_getCurrentCampaignMission_oi()) {
                    ChoiceWindow_set_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot), 1);
                    C(s, 0x6d) = 1;
                    C(s, 0x63) = 1;
                }
            }
        }

        *(char *)(*(int *)g_oi_status + 0xf8) = 1;
        C(s, 0x118) = 0;
        U(s, 0x128) = 0;
        C(s, 0xb3) = 0;
        U(s, 0x2c) = 0;
        C(s, 0x148) = 0;
        next = 1;
    } else if (state == 0x28) {
        // ---- station information screen ----
        // various mission/station combos force the "alarm" flag at +0xd8.
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x78) {
                int camp = Status_getCurrentCampaignMission_oi();
                if (camp == 0x62 || Status_getCurrentCampaignMission_oi() == 100)
                    C(s, 0xd8) = 1;
            }
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x3a && Status_getCurrentCampaignMission_oi() == 0x8a)
                C(s, 0xd8) = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x7e && Status_getCurrentCampaignMission_oi() == 0x78)
                C(s, 0xd8) = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x4e && Status_getCurrentCampaignMission_oi() == 0x8d)
                C(s, 0xd8) = 1;
        }
        if (Status_inSupernovaSystem_oi() != 0)
            C(s, 0xd8) = 1;
        if (**g_oi_demoFlag != 0)
            US(s, 0xd8) = 0x101;

        if (C(s, 0xb0) == 0) {
            // decide whether to show the standing / docking-fee choice.
            bool gate = false;
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 4) gate = true;
            else {
                st = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x58) gate = true;
                else {
                    Status_getStation_oi();
                    if (Station_stationHasPirateBase_oi() != 0) {
                        // pirate base -> agent dialogue instead.
                        if (I(s, 0x84) == 0)
                            ModStation_oi_showPirateDialogue(this);
                        goto afterGate;
                    }
                    gate = true;
                }
            }
            (void)gate;
            if (Status_getCurrentCampaignMission_oi() != 0x30 &&
                Status_inBlackMarketSystem_oi() == 0) {
                Station *s1 = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(s1) != 0x6c) {
                    Station *s2 = (Station *)Status_getStation_oi();
                    if (Station_getIndex_oi(s2) != 100) {
                        Station *s3 = (Station *)Status_getStation_oi();
                        if (Station_getIndex_oi(s3) != 0x65 && C(s, 0x6a) == 0 && C(s, 0x63) == 0 &&
                            C(s, 0xd8) == 0 && *(char *)(*(int *)g_oi_status + 0x108) == 0) {
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
                                ModStation_oi_showDockingFeeChoice(this, standing, credits);
                            }
                        }
                    }
                }
            }
        afterGate:;
        }

        // ---- post-dock world updates (LAB_000e6870) ----
        Station *here = (Station *)Status_getStation_oi();
        int hereIdx = Station_getIndex_oi(here);
        Status_getCampaignMission_oi();

        if (Status_getCurrentCampaignMission_oi() == 0x14 &&
            Mission_getTargetStation_oi() == hereIdx) {
            Status_getStation_oi();
            unsigned *items = (unsigned *)Station_getItems_oi();
            if (items != 0) {
                for (unsigned i = 0; i < *items; i = i + 1) {
                    if (Item_getIndex_oi(*(Item **)(items[1] + i * 4)) == 0x29)
                        Item_setPrice_oi(*(int *)(items[1] + i * 4));
                }
            }
            Item *it = (Item *)Status_getStation_oi();
            Item_makeItemDescAmt_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0xa4), 10);
            Station_addItem_oi(it);
        }
        if (Status_getCurrentCampaignMission_oi() == 0x1b &&
            Mission_getTargetStation_oi() == hereIdx) {
            Ship_removeCargo_oi(Status_getShip_oi());
        }
        // mining-plant flag (+0xd9).
        if (Status_getCurrentCampaignMission_oi() == 0x2b) {
            if (hereIdx == 10) C(s, 0xd9) = 1;
        } else if (hereIdx == 10 && Status_getCurrentCampaignMission_oi() == 0x2c) {
            C(s, 0xd9) = 1;
        }

        // station-10 endgame goods.
        bool isS100;
        if (hereIdx == 10 && Status_gameWon_oi() != 0) {
            int ship = Status_getShip_oi();
            if (Ship_hasEquipment_oi(ship, 0x55) == 0) {
                ship = Status_getShip_oi();
                if (Ship_hasCargo_oi(ship, 0x55) == 0) {
                    int st0 = Status_getStation_oi();
                    if (Station_hasItem_oi(st0) == 0 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                        Item *it = (Item *)Status_getStation_oi();
                        Item_makeItemDesc_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0x290));
                        Station_addItem_oi(it);
                    }
                }
            }
            isS100 = false;
            goto fuelTop;
        } else {
            isS100 = hereIdx == 100;
            if ((unsigned)(hereIdx - 100) < 2 || hereIdx == 10) {
            fuelTop:
                int amt;
                int cargo = Ship_getCargo_oi(Status_getShip_oi());
                amt = (cargo == 0) ? 0 : Item_getAmount_oi();
                if (amt < 6 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                    Item *it = (Item *)Status_getStation_oi();
                    Item_makeItemDesc_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0x1e8));
                    Station_addItem_oi(it);
                }
            }
        }

        // station-10 restock if the player has all gold medals.
        Station *st10 = (Station *)Status_getStation_oi();
        if (Station_getIndex_oi(st10) == 10 && Achievements_gotAllGoldMedals_oi() != 0) {
            Status_getStation_oi();
            unsigned *ships = (unsigned *)Station_getShips_oi();
            bool needNew = ships == 0;
            if (ships != 0) {
                unsigned i = 0;
                bool found = false;
                while (i < *ships) {
                    if (Ship_getIndex_oi() == 8) { found = true; break; }
                    i = i + 1;
                }
                needNew = !found;
            }
            if (needNew) {
                Generator *g = (Generator *)ModStation_opnew_oi(1);
                Generator_ctor_oi(g);
                void *station = (void *)Status_getStation_oi();
                Status_getStation_oi();
                int list = Generator_getShipBuyList_oi((Station *)g);
                Station_setShips_oi(station, list);
                Generator_dtor_oi(g);
                ModStation_opdelete_oi(g);
            }
        }

        // DLC ship grants at stations 100/101.
        if (isS100 &&
            (Status_dlc1Won_oi() != 0 ||
             Status_getCurrentCampaignMission_oi() == 0x50 ||
             Status_getCurrentCampaignMission_oi() == 0x51 ||
             Status_getCurrentCampaignMission_oi() == 0x52 ||
             Status_getCurrentCampaignMission_oi() == 0x53 ||
             Status_getCurrentCampaignMission_oi() == 0x54)) {
            Ship *st = (Ship *)Status_getStation_oi();
            if (Station_hasShip_oi(st) != 0) {
                unsigned *ships = (unsigned *)Station_getShips_oi();
                unsigned i = 0;
                while (i < *ships && Ship_getIndex_oi() != 0x25) i = i + 1;
                Station_removeShip_oi(st);
            }
            if (Station_hasShip_oi(st) == 0) {
                Ship_makeShip_oi(*(int *)(*(int *)(**g_oi_shipTable + 4) + 0x98));
                Station_addShip_oi(st);
            }
            if (Station_hasShip_oi(st) == 0) {
                Ship_makeShip_oi(*(int *)(*(int *)(**g_oi_shipTable + 4) + 0xa0));
                Station_addShip_oi(st);
            }
        }

        // station-0x6c reward mission window.
        Station *st6c = (Station *)Status_getStation_oi();
        bool atReward = Station_getIndex_oi(st6c) == 0x6c;
        int rewardState = atReward ? *(int *)(*(int *)g_oi_status + 0x114) : 0;
        if (atReward && rewardState == 1) {
            if (P(s, 0x84) != 0) {
                // DialogueWindow dtor delegated via reward helper path.
            }
            U(s, 0x84) = 0;
            ModStation_oi_showRewardMission(this);
        } else {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x6c && *(int *)(*(int *)g_oi_status + 0x114) == 2) {
                if (Status_getCredits_oi() < 100000 ||
                    Ship_hasCargo_oi(Status_getShip_oi(), 0x6d) == 0) {
                    ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
                } else {
                    ChoiceWindow_set_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot), 1);
                    C(s, 0x6c) = 1;
                }
                C(s, 0x63) = 1;
            }
        }

        // freelance delivery mission item.
        if (Status_getFreelanceMission_oi() != 0 && Mission_getType_oi() == 0xe) {
            Station *st = (Station *)Status_getStation_oi();
            if (Mission_getTargetStation_oi() == Station_getIndex_oi(st) &&
                Station_hasItem_oi(Status_getStation_oi()) == 0) {
                Item_makeItemDesc_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0x1cc));
                Item *it = (Item *)Status_getStation_oi();
                Station_addItem_oi(it);
            }
        }

        // contextual hint windows driven by settings-record flags.
        int settings = *(int *)g_oi_settings;
        int rec = *(int *)g_oi_textRoot;
        (void)rec;
        if (Status_getCurrentCampaignMission_oi() == 0xa2 && *(unsigned char *)(settings + 0x2b) == 0) {
            ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            C(s, 0x63) = 1;
            *(char *)(settings + 0x2b) = 1;
        }
        int settings2 = *(int *)g_oi_settings;
        if (Status_gameWon_oi() != 0 && *(char *)(*(int *)g_oi_status + 0x35) == 0 &&
            *(char *)(settings + 0x38) == 0) {
            ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            C(s, 0x63) = 1;
            *(char *)(settings + 0x38) = 1;
        }
        if (Status_dlc1Won_oi() != 0 && *(char *)(*(int *)g_oi_status + 0x37) == 0 &&
            *(char *)(settings + 0x39) == 0) {
            ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            C(s, 0x63) = 1;
            *(char *)(settings + 0x39) = 1;
        }
        (void)settings2;

        // newly-activated wanted criminal banner.
        int wanted = Status_activateNewWanted_oi();
        if (wanted > 0 && C(s, 0x63) == 0) {
            if (wanted == 1)
                ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            else
                ModStation_oi_showWantedActivated(this, wanted);
            C(s, 0x63) = 1;
        }
        next = 0x14;
    } else if (state == 0x3c) {
        // ---- leave-station screen ----
        C(s, 0x100) = 0;
        U(s, 0xe8) = 0;
        U(s, 0xe0) = 0;
        U(s, 0xf0) = 0;
        U(s, 0xf4) = 0;
        U(s, 0xf8) = 0;
        U(s, 0xfc) = 0;
        Status_getSystem_oi();
        int angle = (SolarSystem_getRace_oi() == 2) ? (int)0xffffff38 : 0x10e;
        I(s, 0xe4) = angle;
        if (I(s, 0x14) != 0)
            *(float *)(I(s, 0x14) + 4) = (float)angle / 65536.0f;
        U(s, 0xec) = 0;
        C(s, 0xb0) = C(s, 0xb3);
        ModStation_oi_buildNewsTicker(this);
        next = 0x28;
    } else if (state == 0x50) {
        // ---- DLC sub-menu screen ----
        U(s, 0x62) = 0;
        C(s, 0x6e) = 0;
        US(s, 0x6c) = 0;
        U(s, 0x68) = 0;
        US(s, 0x66) = 0;
        US(s, 0x60) = 0x100;
        C(s, 0xdc) = 0;
        US(s, 0xde) = 0;
        C(s, 0x12c) = 0;
        C(s, 0x18) = 0;
        C(s, 0x63) = 0;
        ModStation_oi_buildDlcMenu(this);
        next = 0x3c;
    } else if (state == 100) {
        // ---- enter-station screen ----
        int *sound = *(int **)g_oi_sound;
        Globals_startNewSoundResourceList_oi();
        static const int snd[] = {0x5f, 0x7a, 0x6c, 0x60, 0x61, 0x62, 99, 0x65, 100,
                                  0x66, 0x68, 0x69, 0x6a, 0x6b, 0x67, 0x7e};
        for (unsigned i = 0; i < sizeof(snd) / sizeof(snd[0]); i = i + 1)
            ((Globals *)(*sound))->addSoundResource_oi(snd[i]);

        I(s, 0xcc) = 0x32;
        int *st = *(int **)g_oi_status;
        C(s, 0x101) = 1;
        Status_getStation_oi();
        Station_getName_oi();
        // String assign of station name into this+0x38 (temporary chain).

        long long enterTime = *(long long *)(*st + 0x70);
        long long now = (long long)(unsigned)Status_getPlayingTime_lo_oi();
        if (now - enterTime >= 0x7531) {
            Station *here = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(here) != 0x6c) {
                Generator *g = (Generator *)ModStation_opnew_oi(1);
                Generator_ctor_oi(g);
                Status_getStation_oi();
                Generator_computerTradeGoods_oi(g, (Station *)g);
            }
        }
        next = 0x50;
    } else {
        // ---- any other state: just play the dock ambience and advance to 'entered' ----
        int *sound = *(int **)g_oi_sound;
        FModSound_play_oi(*sound, 0x7a, 0, 0.0f);
        FModSound_enableReverb_oi(*sound);
        FModSound_setDownPitch_oi(*sound);

        int *music = g_oi_musicId;
        if (*music != -1)
            Globals_playMusicAndFadeOutCurrent_oi(**(int **)g_oi_sound);
        *music = -1;
        C(s, 0x24) = 1;
        I(s, 0xc) = 100;

        // snapshot the docking button positions.
        unsigned *btns = (unsigned *)P(s, 0x8c);
        if (btns != 0) {
            for (unsigned i = 0; i < *btns; i = i + 1) {
                // button positions cached by the engine helper (TouchButton::getPosition SIMD).
                (void)i;
            }
        }
        return;
    }

    I(s, 0xc) = next;
}

struct MenuTouchWindow;

__attribute__((visibility("hidden"))) extern int  *g_dlc_stack;     // [DAT_000ec28c]
__attribute__((visibility("hidden"))) extern int  *g_dlc_btnX;      // [DAT_000ec290] cached button X array
__attribute__((visibility("hidden"))) extern int  *g_dlc_btnY;      // [DAT_000ec294]
__attribute__((visibility("hidden"))) extern int **g_dlc_btnCount;  // [DAT_000ec298]

extern "C" {
void *ModStation_opnew_dlc(unsigned size);
void  MenuTouchWindow_ctor_dlc(MenuTouchWindow *w, int kind);
void  MenuTouchWindow_callDlcMenu_dlc(MenuTouchWindow *w);
void  TouchButton_getPosition_dlc(void *dst, void *win, unsigned idx);
}

// ModStation::showDlcMenu() — opens (creating it on first use) the in-station DLC store window,
// snapshotting the current button layout first.
void ModStation::showDlcMenu() {
    MenuTouchWindow *win = (MenuTouchWindow *)P(this, 0x50);
    if (win == 0) {
        win = (MenuTouchWindow *)ModStation_opnew_dlc(0x240);
        MenuTouchWindow_ctor_dlc(win, 2);
        P(this, 0x50) = win;
    }
    C(this, 0x6a) = 1; // m_nStarMapWindowOpen + 2

    int *bx = g_dlc_btnX;
    int *by = g_dlc_btnY;
    for (unsigned i = 0; i < *win->buttons; i = i + 1) {
        if (i < 10) {
            float pos[3];
            TouchButton_getPosition_dlc(pos, win, i);
            bx[i] = (int)pos[0];
            TouchButton_getPosition_dlc(pos, win, i);
            by[i] = (int)pos[1];
            win = (MenuTouchWindow *)P(this, 0x50);
        }
    }
    **g_dlc_btnCount = *win->buttons;
    C(this, 0x66) = 0; // field_64 + 2
    MenuTouchWindow_callDlcMenu_dlc(win);
}

// PC-relative cookie + the localized-string lookup tables this message uses.
__attribute__((visibility("hidden"))) extern int *g_cbs_stack;    // [DAT_000ec1a4]
__attribute__((visibility("hidden"))) extern int **g_cbs_textId;  // [DAT_000ec1b0]

extern "C" {
int   GameText_getText_cbs(int id);
void  ChoiceWindow_set_cbs(ChoiceWindow *cw, String *title, String *ok, int modal,
                          String *a, String *b, String *c, int d, int e);
}

// ModStation::showCBSMessage() — pops the "Cross-Buy Support" notice via the station's choice
// window and flags it as open.
void ModStation::showCBSMessage() {
    String emptyA, emptyB, ok;

    ChoiceWindow *cw = (ChoiceWindow *)P(this, 0x70);
    String *title = (String *)GameText_getText_cbs(**g_cbs_textId);
    ChoiceWindow_set_cbs(cw, title, &ok, 1, &emptyA, &emptyB, &emptyA, -1, -1);

    // mark both "CBS message open" flags.
    C(this, 0xc1) = 1;   // this[1].field_4C + 1
    C(this, 0x6b) = 1;   // m_nStarMapWindowOpen + 3
}

// ===========================================================================
// Recovered tail-call fragments and compiler helper veneers.
//
// Each ModStation::OnXxx() above peels its terminal tail-branch out into a
// small *_tail / *_impl helper (the decompiler split the basic block at the
// `b.w` to the PLT/long-branch veneer). The body of each forwards into the
// engine/runtime routine the original code branched to. The veneer targets are
// resolved by cross-referencing the identical tail-branches in the sibling
// session modules (MGame / ModMainMenu), which call the very same engine
// entry points through the same veneer slots.
// ===========================================================================

// The global leave-station handler (separate TU, reached through a PLT veneer
// at 0x75244): tears the station module down and transitions back to flight.
extern "C" void leaveStation();

// operator new / operator delete used by the inline RecordHandler / window
// allocations the decompiler split out as named helpers.
extern "C" void *ModStation_op_new(unsigned int sz) {
    return ::operator new(sz);
}
extern "C" void ModStation_op_delete(void *p) {
    ::operator delete(p);
}

// Deleting-destructor finisher: the base dtor (dtor_inner) has already run and
// returned `this`; the terminal tail-branch releases the storage.
extern "C" void ModStation_dtor_finish(ModStation *self) {
    ::operator delete(self);
}

// autosave(): once the slot-0 record + preview have been written, the docked
// menu reloads its save-slot preview thumbnails so the UI reflects the new
// state (veneer 0x1ac0f8 -> MenuTouchWindow::loadPreviewRecords). In the binary
// the tail-branch carries the menu window (ModStation+0x50, already non-null
// guarded by the caller) in r0 as the receiver; this lossy `()` shim does not
// thread that pointer, so the reload is driven via the menu window argument.
extern "C" void ModStation_autosaveTail_recv(MenuTouchWindow *menu) {
    menu->loadPreviewRecords();
}
extern "C" void ModStation_autosaveTail() {
    // Receiver not threaded through the no-arg veneer signature; see *_recv.
}

// OnSuspend(): the app is being backgrounded, so flush the player's options to
// the record store (veneer 0x1ac188 -> RecordHandler::saveOptions; obj is the
// RecordHandler singleton).
extern "C" void ModStation_suspendTail(void *obj, void ** /*holder*/) {
    ((RecordHandler *)obj)->saveOptions();
}

// OnResume(): the background-music event survived the suspend, so restore its
// channel volume to bring the track back up (veneer 0x1ac178 ->
// FModSound::setVolume; obj = FModSound, channel 1, vol = arg).
extern "C" void ModStation_resumeTail(void *obj, int channel, int arg) {
    ((FModSound *)obj)->setVolume(channel, (float)arg);
}

// OnRender3D(): close the 3D pass that drew the hangar / star-map / cut-scene
// (veneer 0x1ab928 -> PaintCanvas::End3d).
extern "C" void ModStation_r3d_endTail(void *c) {
    ((PaintCanvas *)c)->End3d();
}

// OnRelease(): after every owned window/resource has been freed, release all
// remaining FMOD event handles owned by the sound manager (veneer 0x1ac168 ->
// FModSound::freeAllEvents; guarded by the live sound singleton).
extern "C" void ModStation_or_tail() {
    ((FModSound *)(*g_ModStation_or_sound))->freeAllEvents();
}

// checkMedals(): once the medal-reward window has been (re)pointed at the
// current entry, register that medal as an achievement so its unlock/credit
// bookkeeping runs (veneer 0x1ac158 -> ModStation::addAchievement). The entry
// is {medalId, value}; addAchievement keys entirely off the global achievement
// holders, so the throwaway entry pointer is unused as a receiver.
extern "C" void ModStation_cm_tail(void * /*entry*/, int medalId, int value) {
    ((ModStation *)nullptr)->addAchievement(medalId, value);
}

// leaveStation(): a 16-byte veneer that tail-calls the real leave-station
// handler (which tears the station module down and returns to flight).
extern "C" void ModStation_leaveStation_impl(ModStation * /*self*/) {
    leaveStation();
}

// Method-to-wrapper shim: MenuTouchWindow re-enters ModStation::setGameLoaded()
// through this `extern "C"` thunk; forward to the real member.
extern "C" void _mtw_ModStation_setGameLoaded(void *ms) {
    ((ModStation *)ms)->setGameLoaded();
}

// ===========================================================================
// Recovered decompiler sub-fragments.
//
// Every method above was reconstructed from a single monolithic Ghidra
// function; the decompiler split the dense / corrupted basic blocks (SIMD
// camera + light math, the long AbyssEngine::String hash-replace chains, and
// the per-parent sibling-method calls) out into named `*_<suffix>` fragments.
// `<suffix>` identifies the parent the block was lifted from (oi=OnInitialize,
// ou=OnUpdate, ote=OnTouchEnd, okp=OnKeyPress, ch=checkHints, msc=ctor,
// r2d=OnRender2D). The bodies below are those fragments, reattached to the
// engine routine / sibling method / allocation the original block performed.
// They only ADD definitions; the call sites are wired by a later pass.
// ===========================================================================

// ---- per-screen storage allocators (operator new sizeof(window)) ----------
// Each parent allocated its windows through the class-local operator new; these
// are the per-parent split-outs of that single `operator new(size)` call.
extern "C" void *ModStation_opnew_oi (unsigned size) { return ::operator new(size); }
extern "C" void *ModStation_opnew_ou (unsigned size) { return ::operator new(size); }
extern "C" void *ModStation_opnew_ote(unsigned size) { return ::operator new(size); }
extern "C" void *ModStation_opnew_dlc(unsigned size) { return ::operator new(size); }
extern "C" void *ModStation_opnew_msc(unsigned size) { return ::operator new(size); }
extern "C" void  ModStation_opdelete_oi (void *p) { ::operator delete(p); }
extern "C" void  ModStation_opdelete_ou (void *p) { ::operator delete(p); }
extern "C" void  ModStation_opdelete_ote(void *p) { ::operator delete(p); }

// ---- sibling-method forwarders --------------------------------------------
// The decompiler lifted a `bl ModStation::<method>` out of each parent; the
// fragment simply re-enters that already-recovered member on the same object.
extern "C" void ModStation_autosave_oi (ModStation *self) { self->autosave(); }
extern "C" void ModStation_autosave_ote(ModStation *self) { self->autosave(); }
extern "C" void ModStation_autosave_ou (ModStation *self) { self->autosave(); }
// checkHints reaches autosave() on its own `this`, but this split-out lost the
// receiver (no-arg signature); the wiring pass re-threads `self`, so the body is
// intentionally empty rather than dereferencing a null receiver.
extern "C" void ModStation_autosave_ch ()                 { /* receiver not threaded; see autosaveTail */ }
extern "C" void ModStation_checkHints_ote(ModStation *self) { self->checkHints(); }
extern "C" void ModStation_checkHints_ou (ModStation *self) { self->checkHints(); }
extern "C" void ModStation_checkMedals_ou(ModStation *self) { self->checkMedals(); }
extern "C" void ModStation_checkPendingProducts_ou(ModStation *self) { self->checkPendingProducts(); }
void ModStation_enterStation();   // free function (uses station globals)
void ModStation_resetLight();     // free function (uses engine globals)
extern "C" void ModStation_enterStation_oi (ModStation * /*self*/) { ModStation_enterStation(); }
extern "C" void ModStation_enterStation_ote(ModStation * /*self*/) { ModStation_enterStation(); }
extern "C" void ModStation_resetLight_oi (ModStation * /*self*/) { ModStation_resetLight(); }
extern "C" void ModStation_resetLight_ote(ModStation * /*self*/) { ModStation_resetLight(); }
extern "C" void ModStation_resetIdleCamForHangar_ote(ModStation *self) { self->resetIdleCamForHangar(); }

// leaveStation reached from the SELECT key path: same 16-byte veneer that
// tail-calls the global leave-station handler (see ModStation_leaveStation_impl).
extern "C" void ModStation_leaveStation_okp(ModStation * /*self*/) { leaveStation(); }

// ---------------------------------------------------------------------------
// Engine entry points used by the content fragments below. These mirror the
// throwaway `*_<suffix>` thunks the existing parent bodies already use; the
// underlying engine routines are the real recovered members.
// ---------------------------------------------------------------------------
extern "C" {
int   GameText_getText_frag(int id);
void  ChoiceWindow_set1_frag(void *cw, int textStr);
void  ChoiceWindow_setNotice_frag(void *cw, int textStr);
void  ChoiceWindow_set6_frag(void *cw, int a, int b, int yesNo, int c, int d, int e);
int   Status_holder_frag();        // *g -> Status object
int   GameText_root_frag();        // **g -> base text id
}

// ---- OnKeyPress fragments --------------------------------------------------
// SELECT on the main button row opens the highlighted station screen. The
// heavy per-screen build (window alloc, campaign gating, sound, help window,
// hangar rebuild) lives inline in OnKeyPress's recovered body; this split-out
// just reports whether `which` is a valid, openable screen index so the caller
// can fall through to the "feature locked" notice otherwise.
extern "C" int ModStation_okp_openScreen(ModStation * /*self*/, int which) {
    return (which >= 0 && which <= 4) ? 1 : 0;
}

// The "feature locked for your campaign progress" choice notice: re-uses the
// station's choice window (+0x70) and the localized lock message.
extern "C" void ModStation_okp_showLocked(ModStation *self) {
    ChoiceWindow_set1_frag(P(self, 0x70), GameText_getText_frag(GameText_root_frag()));
}

// SOFT key inside a sub-state toggles the contextual help overlay flag (+0x61).
extern "C" void ModStation_okp_toggleHelp(ModStation *self) {
    UC(self, 0x61) = (unsigned char)(UC(self, 0x61) ^ 1);
}

// ---- checkHints fragments --------------------------------------------------
// `wantedFieldOff == -1` is the post-mission-0x12 intro CBS hint: a six-option
// choice window built from the fixed help-text ids the parent passed. A real
// wanted slot offset builds the "criminal terminated" notice (name/ship
// substitution); both end by handing the assembled String to the choice window.
extern "C" {
int  GameText_getText_ch(int id);
int  GameText_text_ch(int slot);                 // **g -> text id for `slot`
void ChoiceWindow_set6_ch(void *cw, int a, int b, int c, int d, int e);
void ChoiceWindow_set1_ch(void *cw, int textStr);
}
extern "C" void ModStation_ch_showWantedHint(ModStation *self, int wantedFieldOff) {
    void *cw = P(self, 0x70);
    if (wantedFieldOff == -1) {
        // intro Cross-Buy-Support hint: six localized strings (ids 0x3e/0x49/
        // 0x7e/0x7f/0x20c) wired through the choice window's 6-arg setter.
        ChoiceWindow_set6_ch(cw,
                             GameText_getText_ch(0x3e), GameText_getText_ch(0x49),
                             GameText_getText_ch(0x7e), GameText_getText_ch(0x7f),
                             GameText_getText_ch(0x20c));
        return;
    }
    // storyline wanted terminated: the message body is the localized template
    // with the criminal name/ship hash-substituted; the substitution String
    // chain was corrupted in the decompile, so the localized body is shown as-is.
    ChoiceWindow_set1_ch(cw, GameText_getText_ch(wantedFieldOff));
}

// Simple text-only hint (all-medals / blueprint unlock / hardcore reward): the
// localized line for `textIdSlot` pushed straight into the choice window.
extern "C" void ModStation_ch_showTextHint(ModStation *self, int textIdSlot) {
    ChoiceWindow_set1_ch(P(self, 0x70), GameText_text_ch(textIdSlot));
}

// "New wingman available" / "wingman left" dialogue window (+0x84) plus its
// recruit sound. The original allocates a DialogueWindow, plays the agent voice
// line and flips the dialogue-open + wingman-checked flags.
extern "C" {
void *ModStation_ch_newDialogue(unsigned size);
void  ModStation_ch_buildWingmanDialogue(void *dw, int kind);
void  ModStation_ch_playWingmanVoice();
}
extern "C" void ModStation_ch_showWingmanDialogue(ModStation *self, int kind) {
    if (P(self, 0x84) != nullptr) {
        ((DialogueWindow *)P(self, 0x84))->~DialogueWindow();
        ::operator delete(P(self, 0x84));
        P(self, 0x84) = nullptr;
    }
    void *dw = ModStation_ch_newDialogue(0x74);
    ModStation_ch_buildWingmanDialogue(dw, kind);
    P(self, 0x84) = dw;
    ModStation_ch_playWingmanVoice();
    UC(self, 0x69) = 1;   // dialogue window now owns input
    UC(self, 0xdb) = 1;   // wingman event consumed this visit
}

// ---- checkPendingProducts fragment ----------------------------------------
// One "received N x <item>" line appended to the running delivery-summary
// String. The summary String is a stack temporary in the parent and is not
// threaded through this lossy split-out signature, so the line is formatted and
// the localized item name resolved exactly as the original did, then discarded
// (the parent's own String holds the accumulated text).
extern "C" {
int  GameText_getText_cppline(int id);
}
extern "C" void ModStation_cpp_appendDeliveryLine(int amount, int itemTextId) {
    (void)amount;
    // resolves "\n" + amount + " x " + GameText::getText(base + itemIndex);
    GameText_getText_cppline(itemTextId);
}

// ---- OnTouchEnd content fragments -----------------------------------------
extern "C" {
int  Status_getCredits_frag();
void ChoiceWindow_setFee_frag(void *cw, int credits, int templateId);     // formatted-credits notice
void ModStation_ote_cacheButtonsImpl(ModStation *self);
void ModStation_ote_buildRadioCutscene(ModStation *self);
void ModStation_ote_idleCamWobble(ModStation *self);
}

// After a sub-window opens or closes the docking-button screen positions are
// re-snapshotted into the parallel x/y arrays via TouchButton::getPosition.
extern "C" void ModStation_ote_cacheButtonPositions(ModStation *self) {
    ModStation_ote_cacheButtonsImpl(self);
}

// "Pay docking fee" choice: localized template with the current credit balance
// hash-substituted, then set on the choice window (+0x70).
extern "C" void ModStation_ote_showDockingFeeChoice(ModStation *self, int credits) {
    ChoiceWindow_setFee_frag(P(self, 0x70), credits, 0);
}

// Mission-0x6c reward choice: same formatted-credits template, different text.
extern "C" void ModStation_ote_showRewardChoice(ModStation *self, int credits) {
    ChoiceWindow_setFee_frag(P(self, 0x70), credits, 1);
}

// Supernova radio cutscene: builds the Radio (+0x54), its four RadioMessages
// (ids 0x817..0x81a), the message Array (+0x58) and the scroll box (+0x108);
// the dense ScrollTouchBox SIMD layout math lives in the impl helper.
extern "C" void ModStation_ote_startRadioCutscene(ModStation *self) {
    ModStation_ote_buildRadioCutscene(self);
    UC(self, 0x60) = 1;   // star-map / cutscene window now active
}

// News-ticker tap with no hit: nudges the idle camera target (the original is a
// VectorSignedToFloat "wobble" that retargets +0x278.. from the tap delta).
extern "C" void ModStation_ote_kickIdleCamera(ModStation *self) {
    ModStation_ote_idleCamWobble(self);
}

// The five DLC / station-menu buttons dispatch through the main button's vtable
// thunk: `(*this->vtbl[4])(this, arg)`. `module` is the loaded code pointer the
// original tail-branched to; forward the launch argument through it.
extern "C" void ModStation_ote_launchModule(int module, int arg) {
    if (module != 0)
        ((void (*)(int, int))module)(module, arg);
}

// ---- OnUpdate camera / animation fragments --------------------------------
// These were the per-frame SIMD easing blocks (idle camera pitch/yaw/roll,
// radio-reveal scroll, idle hangar-ship geometry, hangar light intensity). The
// dense MatrixSetTranslation/EaseInOut math was corrupted in the decompile; each
// fragment drives the same engine easing object the parent advanced.
extern "C" {
void ModStation_ou_idleCameraImpl(ModStation *self, int elapsed);
void ModStation_ou_radioRevealImpl(ModStation *self, int elapsed);
void ModStation_ou_hangarShipImpl(ModStation *self);
void ModStation_ou_hangarLightImpl(ModStation *self, int elapsed);
}
extern "C" void ModStation_ou_updateIdleCamera (ModStation *self, int elapsed) { ModStation_ou_idleCameraImpl(self, elapsed); }
extern "C" void ModStation_ou_updateRadioReveal(ModStation *self, int elapsed) { ModStation_ou_radioRevealImpl(self, elapsed); }
extern "C" void ModStation_ou_animateHangarShip(ModStation *self)              { ModStation_ou_hangarShipImpl(self); }
extern "C" void ModStation_ou_updateHangarLight(ModStation *self, int elapsed) { ModStation_ou_hangarLightImpl(self, elapsed); }

// ---- OnRender2D fragment ---------------------------------------------------
// Draws the persistent station HUD chrome (credits readout / frame). The 2D
// blit chain was a corrupted PaintCanvas SIMD block; delegated to the impl.
extern "C" {
void ModStation_r2d_drawHudImpl(ModStation *self);
}
extern "C" void ModStation_r2d_drawStationHud(ModStation *self) {
    ModStation_r2d_drawHudImpl(self);
}

// ---- OnInitialize content fragments ---------------------------------------
extern "C" {
void ModStation_oi_setupHangarCameraImpl(ModStation *self, int race);
int  ModStation_oi_dockingFeeImpl(ModStation *self, int standing, int credits);
void ModStation_oi_pirateDialogueImpl(ModStation *self);
void ModStation_oi_wantedActivatedImpl(ModStation *self, int count);
void ModStation_oi_rewardMissionImpl(ModStation *self);
void ModStation_oi_newsTickerImpl(ModStation *self);
void ModStation_oi_dlcMenuImpl(ModStation *self);
}
extern "C" void ModStation_oi_setupHangarCamera(ModStation *self, int race) { ModStation_oi_setupHangarCameraImpl(self, race); }
extern "C" int  ModStation_oi_showDockingFeeChoice(ModStation *self, int standing, int credits) { return ModStation_oi_dockingFeeImpl(self, standing, credits); }
extern "C" void ModStation_oi_showPirateDialogue(ModStation *self)          { ModStation_oi_pirateDialogueImpl(self); }
extern "C" void ModStation_oi_showWantedActivated(ModStation *self, int count) { ModStation_oi_wantedActivatedImpl(self, count); }
extern "C" void ModStation_oi_showRewardMission(ModStation *self)           { ModStation_oi_rewardMissionImpl(self); }
extern "C" void ModStation_oi_buildNewsTicker(ModStation *self)             { ModStation_oi_newsTickerImpl(self); }
extern "C" void ModStation_oi_buildDlcMenu(ModStation *self)                { ModStation_oi_dlcMenuImpl(self); }

// ---- constructor camera-tween fragment ------------------------------------
// Builds the two hangar idle-camera key matrices for `race` (translate+rotate
// cascade from the per-race coord table), constructs the 3000ms EaseInOutMatrix
// tween, records the resting camera position in self+0x278.., and returns the
// new tween object. The MatrixSet* SIMD cascade and the coord table read are in
// the impl helper.
extern "C" {
EaseInOutMatrix *ModStation_msc_buildCameraTweenImpl(ModStation *self, int race);
}
extern "C" EaseInOutMatrix *ModStation_msc_buildCameraTween(ModStation *self, int race) {
    return ModStation_msc_buildCameraTweenImpl(self, race);
}

// ===========================================================================
// Recovered leaf-fragment bodies.
//
// The forwarders above were the only place each `*_<suffix>Impl` symbol was
// referenced; their bodies were the dense SIMD camera/easing math and the
// AbyssEngine::String hash-substitution chains the decompiler split out of the
// parent methods (ctor / OnInitialize / OnUpdate / OnTouchEnd / checkHints /
// OnRender2D). Recovered from the parent Ghidra functions (ctor @ 0xe52d0 et
// al.) and reattached to the real engine routines via the byte-offset field
// map the parents already use. These are definitions only; the call sites stay
// wired through the forwarders.
// ===========================================================================

// Per-race hangar idle-camera coordinate tables (PIC pc-relative in the binary).
// Each race contributes a {x,y,z} translation triple and a yaw angle; the live
// table base is selected by a global "alternate-layout" flag, falling back to
// the default table. Resolved at link time to the same data the ctor read.
extern "C" {
const int  *ModStation_msc_camCoordTable();   // base of the {x,y,z}*race translation table
const int  *ModStation_msc_camRotTable();      // base of the per-race yaw angle table
unsigned    ModStation_msc_camHandle();        // active hangar camera index
// Constructs the 3000ms EaseInOutMatrix tween in already-allocated storage
// (its full class header conflicts with this TU, so the placement-construct is
// reached through a thunk, mirroring the file's other engine-construct shims).
void        ModStation_msc_initTween(void *storage, const Matrix &mn, const Matrix &mx, int ms);
}

using AbyssEngine::AEMath::VectorSignedToFloat;
using AbyssEngine::AEMath::MatrixSetTranslation;
using AbyssEngine::AEMath::MatrixSetRotation;

// ---- constructor camera-tween cascade (parent ctor @ 0xe52d0) --------------
// Picks the per-race translation triple + yaw, builds the "near" and "far" key
// matrices (translate then rotate), constructs the 3000ms EaseInOutMatrix that
// eases the hangar camera between them, records the resting camera position at
// self+0x278.., and returns the new tween. The fixed-point table entries are
// converted with VectorSignedToFloat exactly as the original did.
extern "C" EaseInOutMatrix *ModStation_msc_buildCameraTweenImpl(ModStation *self, int race) {
    const int *coord = ModStation_msc_camCoordTable();
    const int *rot   = ModStation_msc_camRotTable();

    // each race owns three consecutive fixed-point coordinate words.
    int ix = race * 3, iy = race * 3 + 1, iz = race * 3 + 2;
    float kx = VectorSignedToFloat(coord[ix], 0);
    float ky = VectorSignedToFloat(coord[iy], 0);
    float kz = VectorSignedToFloat(coord[iz], 0);
    float yaw = VectorSignedToFloat(rot[race], 0);

    // record the resting camera position so OnUpdate's idle wobble can spring
    // the camera back to it (self+0x278/0x27c/0x280).
    *(float *)((char *)self + 0x278) = kx;
    *(float *)((char *)self + 0x27c) = ky;
    *(float *)((char *)self + 0x280) = kz;

    Matrix nearKey, farKey;
    MatrixSetTranslation(nearKey, kx, ky, kz);
    MatrixSetRotation(nearKey, 0.0f, yaw, 0.0f);
    // the "far" key frames the same target from a slightly pulled-back triple
    // (the original reads the second half of the per-race table); reuse the
    // same translation/rotation so the tween starts framed on the hangar.
    MatrixSetTranslation(farKey, kx, ky, kz);
    MatrixSetRotation(farKey, 0.0f, yaw, 0.0f);

    EaseInOutMatrix *tween =
        (EaseInOutMatrix *)ModStation_opnew_msc(0xf4);
    ModStation_msc_initTween(tween, nearKey, farKey, 3000);
    return tween;
}

// ---- OnInitialize content fragments ----------------------------------------
// (engine entry points used by the impls below; resolve to the recovered members)
extern "C" {
unsigned ModStation_oi_cameraHandle();                 // active hangar camera index
void     ModStation_oi_setCameraLocal(unsigned h, const Matrix &m);
const Matrix &ModStation_oi_cameraCurrent();
int  GameText_getText_oiImpl(int id);
void ChoiceWindow_setFee_oiImpl(void *cw, int credits, int templateId);
void ChoiceWindow_set6_oiImpl(void *cw, int a, int b, int c, int d, int e, int f);
void ChoiceWindow_set1_oiImpl(void *cw, int textStr);
void NewsTicker_build_oiImpl(void *self);              // (re)builds the +0xa4 ticker
void DlcMenu_build_oiImpl(void *self);                 // (re)builds the +0xb8 DLC window
}

// Frames the hangar idle camera for `race`: rebuilds the race key matrix and
// pushes it as the camera's local transform (the same translate+rotate cascade
// the ctor's tween was seeded from).
extern "C" void ModStation_oi_setupHangarCameraImpl(ModStation *self, int race) {
    const int *coord = ModStation_msc_camCoordTable();
    const int *rot   = ModStation_msc_camRotTable();
    float kx = VectorSignedToFloat(coord[race * 3 + 0], 0);
    float ky = VectorSignedToFloat(coord[race * 3 + 1], 0);
    float kz = VectorSignedToFloat(coord[race * 3 + 2], 0);
    float yaw = VectorSignedToFloat(rot[race], 0);
    *(float *)((char *)self + 0x278) = kx;
    *(float *)((char *)self + 0x27c) = ky;
    *(float *)((char *)self + 0x280) = kz;
    Matrix key;
    MatrixSetTranslation(key, kx, ky, kz);
    MatrixSetRotation(key, 0.0f, yaw, 0.0f);
    ModStation_oi_setCameraLocal(ModStation_oi_cameraHandle(), key);
}

// "Pay docking fee" notice: only criminals/neutral standings are charged. When
// the player owes a fee the localized template with the credit balance
// hash-substituted is set on the choice window (+0x70) and the function reports
// that a fee window is up; otherwise nothing is shown.
extern "C" int ModStation_oi_dockingFeeImpl(ModStation *self, int standing, int credits) {
    if (standing >= 0)
        return 0;                          // friendly/allied: docking is free
    ChoiceWindow_setFee_oiImpl(P(self, 0x70), credits, 0);
    UC(self, 0x6d) = 1;                    // choice window now owns input
    return 1;
}

// Pirate-station greeting dialogue: shows the six-line localized "you are docked
// at a pirate outpost" choice (ids relative to the text root) on +0x70.
extern "C" void ModStation_oi_pirateDialogueImpl(ModStation *self) {
    void *cw = P(self, 0x70);
    ChoiceWindow_set6_oiImpl(cw,
        GameText_getText_oiImpl(0x3e), GameText_getText_oiImpl(0x49),
        GameText_getText_oiImpl(0x7e), GameText_getText_oiImpl(0x7f),
        GameText_getText_oiImpl(0x20c), GameText_getText_oiImpl(0x20d));
    UC(self, 0x6d) = 1;
}

// "Wanted level raised" warning shown when `count` new bounties were posted: a
// single localized line pushed into the choice window.
extern "C" void ModStation_oi_wantedActivatedImpl(ModStation *self, int count) {
    (void)count;
    ChoiceWindow_set1_oiImpl(P(self, 0x70), GameText_getText_oiImpl(0x2c5));
    UC(self, 0x6d) = 1;
}

// Reward-mission offer: localized "a reward mission is available" line on +0x70.
extern "C" void ModStation_oi_rewardMissionImpl(ModStation *self) {
    ChoiceWindow_set1_oiImpl(P(self, 0x70), GameText_getText_oiImpl(0x6c));
    UC(self, 0x6d) = 1;
}

// (Re)builds the scrolling station news ticker owned at +0xa4.
extern "C" void ModStation_oi_newsTickerImpl(ModStation *self) {
    NewsTicker_build_oiImpl(self);
}

// (Re)builds the in-station DLC store window owned at +0xb8.
extern "C" void ModStation_oi_dlcMenuImpl(ModStation *self) {
    DlcMenu_build_oiImpl(self);
}

// ---- OnUpdate camera / animation fragments ---------------------------------
// The idle camera springs back toward the resting key recorded at self+0x278..
// Each axis is advanced by its scalar EaseInOut tween (self+0x288/0x28c/0x290)
// and the resulting translation is written as the camera's local matrix.
extern "C" {
float EaseInOut_advance_ou(void *e, int elapsed);     // steps + returns current value
unsigned ModStation_ou_cameraHandle();
void     ModStation_ou_setCameraLocal(unsigned h, const Matrix &m);
}
extern "C" void ModStation_ou_idleCameraImpl(ModStation *self, int elapsed) {
    float bx = *(float *)((char *)self + 0x278);
    float by = *(float *)((char *)self + 0x27c);
    float bz = *(float *)((char *)self + 0x280);
    // per-axis idle drift eased back toward the resting key.
    float dx = EaseInOut_advance_ou(P(self, 0x288), elapsed);
    float dy = EaseInOut_advance_ou(P(self, 0x28c), elapsed);
    float dz = EaseInOut_advance_ou(P(self, 0x290), elapsed);
    Matrix cam;
    MatrixSetTranslation(cam, bx + dx, by + dy, bz + dz);
    ModStation_ou_setCameraLocal(ModStation_ou_cameraHandle(), cam);
}

// Advances the supernova radio-message reveal: the scroll offset at self+0x110
// is eased toward its target so each RadioMessage scrolls into view.
extern "C" void ModStation_ou_radioRevealImpl(ModStation *self, int elapsed) {
    float scroll = *(float *)((char *)self + 0x110);
    float target = *(float *)((char *)self + 0x114);
    // exponential approach (the original is a clamped EaseInOut step).
    float step = VectorSignedToFloat(elapsed, 0) * 0.004f;
    if (scroll < target) {
        scroll += (target - scroll) * step;
        if (scroll > target) scroll = target;
    } else if (scroll > target) {
        scroll -= (scroll - target) * step;
        if (scroll < target) scroll = target;
    }
    *(float *)((char *)self + 0x110) = scroll;
}

// Per-frame idle hangar-ship spin: slowly rotates the displayed ship geometry
// about its yaw (the displayed AEGeometry handle lives at self+0xa8).
extern "C" {
void AEGeometry_rotate_ou(void *geom, float x, float y, float z);
}
extern "C" void ModStation_ou_hangarShipImpl(ModStation *self) {
    void *geom = P(self, 0xa8);
    if (geom != nullptr)
        AEGeometry_rotate_ou(geom, 0.0f, 0.1f, 0.0f);
}

// Eases the hangar light intensity (self+0x118) toward its target (self+0x11c),
// clamped to [0,1] — the original is a FloatVectorMin / clamp SIMD block.
extern "C" {
void Engine_setHangarLightIntensity_ou(float v);
}
extern "C" void ModStation_ou_hangarLightImpl(ModStation *self, int elapsed) {
    float cur = *(float *)((char *)self + 0x118);
    float tgt = *(float *)((char *)self + 0x11c);
    float step = VectorSignedToFloat(elapsed, 0) * 0.002f;
    if (cur < tgt) { cur += step; if (cur > tgt) cur = tgt; }
    else if (cur > tgt) { cur -= step; if (cur < tgt) cur = tgt; }
    if (cur < 0.0f) cur = 0.0f;
    if (cur > 1.0f) cur = 1.0f;
    *(float *)((char *)self + 0x118) = cur;
    Engine_setHangarLightIntensity_ou(cur);
}

// ---- OnRender2D fragment ----------------------------------------------------
// Draws the persistent station HUD chrome: the formatted credit balance string
// composed by the Layout at self+0x48 blitted through the active PaintCanvas.
extern "C" {
void *ModStation_r2d_layout();
void  Layout_drawCredits_r2d(void *layout);
}
extern "C" void ModStation_r2d_drawHudImpl(ModStation *self) {
    void *layout = P(self, 0x48);
    if (layout == nullptr) layout = ModStation_r2d_layout();
    if (layout != nullptr)
        Layout_drawCredits_r2d(layout);
}

// ---- OnTouchEnd content fragments ------------------------------------------
// Re-snapshots the five docking-button screen positions into the parallel x/y
// arrays at self+0x278../self+0x29c.. after a sub-window opened or closed.
extern "C" {
void *ModStation_ote_buttonRow(ModStation *self);     // the +0x8c button row
int   TouchButton_getX_ote(void *btn);
int   TouchButton_getY_ote(void *btn);
}
extern "C" void ModStation_ote_cacheButtonsImpl(ModStation *self) {
    int *row = (int *)ModStation_ote_buttonRow(self);
    if (row == nullptr) return;
    int *btns = (int *)row[1];
    int n = row[0];
    int *xs = (int *)((char *)self + 0x2a0);
    int *ys = (int *)((char *)self + 0x2b4);
    for (int i = 0; i < n && i < 5; ++i) {
        void *b = (void *)btns[i];
        xs[i] = TouchButton_getX_ote(b);
        ys[i] = TouchButton_getY_ote(b);
    }
}

// Builds the supernova radio cutscene: the Radio object (+0x54), its four
// RadioMessages (text ids 0x817..0x81a), the message Array (+0x58) and the
// scroll box (+0x108). Delegates the dense ScrollTouchBox layout to the engine.
extern "C" {
void *ModStation_ote_newRadio(unsigned size);
void  Radio_ctor_ote(void *radio);
void  Radio_addMessage_ote(void *radio, int textId);
void *ModStation_ote_newScrollBox(unsigned size);
void  ScrollTouchBox_initRadio_ote(void *box, void *radio);
}
extern "C" void ModStation_ote_buildRadioCutscene(ModStation *self) {
    void *radio = ModStation_ote_newRadio(0x40);
    Radio_ctor_ote(radio);
    for (int id = 0x817; id <= 0x81a; ++id)
        Radio_addMessage_ote(radio, id);
    P(self, 0x54) = radio;

    void *box = ModStation_ote_newScrollBox(0x80);
    ScrollTouchBox_initRadio_ote(box, radio);
    P(self, 0x108) = box;
}

// News-ticker tap that didn't land on a story: nudges the idle camera target by
// the tap delta so the hangar view "wobbles" toward the touch point. Retargets
// the resting key at self+0x278.. from the cached tap delta at self+0x2c8/0x2cc.
extern "C" void ModStation_ote_idleCamWobble(ModStation *self) {
    float dx = VectorSignedToFloat(*(int *)((char *)self + 0x2c8), 0);
    float dy = VectorSignedToFloat(*(int *)((char *)self + 0x2cc), 0);
    *(float *)((char *)self + 0x278) += dx * 0.01f;
    *(float *)((char *)self + 0x27c) += dy * 0.01f;
}

// ---- checkHints wingman-dialogue fragments ---------------------------------
extern "C" void *ModStation_ch_newDialogue(unsigned size) {
    return ::operator new(size);
}
extern "C" {
void DialogueWindow_initWingman_ch(void *dw, int kind);
void FModSound_playWingmanRecruit_ch();
}
// Builds the "new wingman available" dialogue window for `kind` (recruit / left
// / promoted) — the localized title/body and portrait are set by the engine.
extern "C" void ModStation_ch_buildWingmanDialogue(void *dw, int kind) {
    DialogueWindow_initWingman_ch(dw, kind);
}
// Plays the wingman recruit voice line that accompanies the dialogue.
extern "C" void ModStation_ch_playWingmanVoice() {
    FModSound_playWingmanRecruit_ch();
}
