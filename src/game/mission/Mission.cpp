#include "gof2/game/mission/Mission.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/game/world/Station.h"
#include "gof2/game/core/String.h"

struct Systems;

extern "C" void String_cstr_ctor(void *out, const char *s, bool);
extern "C" void *String_assign_ref(void *self, const String12 &rhs);
extern "C" void String_default_ctor(void *s);
extern "C" int Station_getSystem(Station *s);
extern "C" void Station_dtor_finish(Station *s);

uint8_t Mission::isInstantActionMission() {
    return this->instantAction;
}

// `struct RetStr` provided by gof2/Station.h (via Mission.h).

RetStr Mission::getTargetStationName() {
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)this + 0x40), false);
    return r;
}

bool Mission::isCampaignMission() {
    return this->campaign != 0;
}

// AbyssEngine::String::String(String* out, const char* cstr, bool) -> 0x6ee18

// Returns a fixed description String built from a string literal.
RetStr Mission::getDescription() {
    RetStr r;
    String_cstr_ctor(&r, "", false);
    return r;
}

RetStr Mission::getTargetSystemName() {
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)this + 0x4c), false);
    return r;
}

struct GameText;

// String::String(String* out, const char* cstr, bool) -> 0x6ee18.
// String::String(String* out, const String* src, bool) -> 0x6f028.
// GameText::getText(GameText* table, int id) -> 0x72f70 returns a String*.
// Text table singleton (hidden -> single pc-relative deref).
__attribute__((visibility("hidden"))) extern GameText **g_gameText;

// Mission::getName(): campaign missions use a fixed literal name; freelance ones
// look up "<id+0x162>" in the global text table.
RetStr Mission::getName() {
    RetStr r;
    if (this->campaign != 0) {
        String_cstr_ctor(&r, "", false);
    } else {
        void *txt = ((GameText *)(*g_gameText))->getText(this->id + 0x162);
        ((String *)(&r))->ctor_copy((String *)(txt), false);
    }
    return r;
}

void Mission::setProductionGoods(int a, int b) {
    this->productionGoodsA = a;
    this->productionGoodsB = b;
}

// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548

// Mission::setTargetName(String by value): the String has a non-trivial copy
// ctor/dtor so it is passed by invisible reference (pointer in r1).
void * Mission::setTargetName(const String12 &rhs) {
    return String_assign_ref((char *)this + 0x1c, rhs);
}

// AbyssEngine::String::String(String* out, const String* src, bool) -> 0x6f028

// Returns the client name String (offset 0x10) by value. The void copy-ctor forces
// a frame + non-tail blx (the sret r0 must be restored).
RetStr Mission::getClientName() {
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)this + 0x10), false);
    return r;
}

uint8_t Mission::isVisible() {
    return this->visible;
}

bool Mission::isEmpty() {
    return this->id == -1;
}

RetStr Mission::getTargetName() {
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)this + 0x1c), false);
    return r;
}

void Mission::setInstantActionMission(bool v) {
    this->instantAction = v;
}

// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548

void * Mission::setTargetSystemName(const String12 &rhs) {
    return String_assign_ref((char *)this + 0x4c, rhs);
}

void Mission::setVisible(bool v) {
    this->visible = v;
}

// Mission::Mission(int, String, int*, int, int, int, int) -> 0x15b64c. The by-value
// String is passed by invisible reference -> model it as a pointer parameter.

// Mission::clone(): deep-copies this mission into a freshly allocated Mission via
// the 7-arg constructor, then copies the instant-action flag. The on-stack String
// temp triggers the -Oz stack-protector canary.
Mission * Mission::clone() {
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Mission *m = (Mission *)::operator new(0x78);
    int id = this->id;
    ((String *)(name))->ctor_copy((String *)((char *)this + 0x10), false);
    ((Mission *)(m))->ctor7(id, name, this->field_0x28, this->field_0x2c, this->field_0x30, this->targetStation, this->reward);
    ((String *)(name))->dtor();
    m->instantAction = this->instantAction;
    return m;
}

struct Galaxy;
struct Station;

// Galaxy singleton pointer global (hidden -> single pc-relative deref to the value).
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Sets the target station index (+0x3c) and caches that station's name into the
// targetStationName String (+0x40). The char-array-backed String temp triggers
// the -Oz/-fstack-protector canary the target emits.
void Mission::setTargetStation(int idx) {
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Galaxy **gp = g_galaxy;
    this->targetStation = idx;
    Station *st = (Station *)((Galaxy *)(*gp))->getStation(idx);
    ((Station *)(name))->getName();
    ((String *)((char *)this + 0x40))->assign((String *)name);
    ((String *)(name))->dtor();
}

// Mission::~Mission() (the inner/base dtor at 0x15ba54) returns `this` in r0.
// Deallocation/base helper tail-called at the end (0x1ab098).

// Deleting/complete destructor: runs the inner dtor (which returns `this`), then
// tail-calls the finisher with that pointer — so `this` need not be saved.
void Mission::dtor() {
    ((Mission *)(((Mission *)(this))->dtor_inner()))->dtor_finish();
}

struct Galaxy;
struct Station;

__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Mission::Mission(int id, String client, int* targets, int a, int b, int station, int reward)
// — the freelance-mission constructor.
Mission * Mission::ctor7(int id, const void *client, int a, int b, int c, int station, int reward) {
    Mission *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    self->id = id;
    ((String *)((char *)self + 0x10))->assign((String *)client);
    self->field_0x28 = a;
    self->field_0x2c = b;
    self->field_0x30 = c;
    self->targetStation = station;
    Station *st = (Station *)((Galaxy *)(*g_galaxy))->getStation(station);
    ((Station *)(tmp))->getName();
    ((String *)((char *)self + 0x40))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    self->reward = reward;
    String_cstr_ctor(tmp, "", false);
    ((String *)((char *)self + 0x4c))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    self->visible = 1;
    self->campaign = 0;
    self->instantAction = 0;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0x38 = 0;
    return self;
}

// PIC global (hidden -> single pc-relative deref): the Mission vtable base.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;

// Mission::Mission(int) — like the default ctor but stores the given id at +0xc.
Mission * Mission::ctor_int(int id) {
    Mission *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    String_cstr_ctor(tmp, "", false);
    ((String *)((char *)self + 0x10))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    self->campaign = 0;
    self->visible = 0;
    self->field_0x8 = 0;
    self->id = id;
    self->field_0x28 = 0;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    self->reward = 0;
    self->instantAction = 0;
    self->field_0x70 = 0;
    self->field_0x4 = 0;
    self->field_0x38 = 0;
    self->targetStation = 0;
    return self;
}

struct Galaxy;
struct Station;
struct Status;
struct SolarSystem;
struct Systems { uint32_t n; SolarSystem **data; uint32_t cap; };

// Singletons held as pointer-to-pointer in hidden globals.
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;
__attribute__((visibility("hidden"))) extern Status **g_status;

// Computes the distance (light years) from the player's current system to this
// mission's target station system, storing the truncated value at +0x60.
void Mission::calcDistance() {
    Galaxy **gp = g_galaxy;
    Station *st = (Station *)((Galaxy *)(*gp))->getStation(this->targetStation);
    Systems *sys = (Systems *)((Galaxy *)(*gp))->getSystems();
    Galaxy *g = *gp;
    int i1 = Station_getSystem(((Status *)(*g_status))->getStation());
    SolarSystem *a = sys->data[i1];
    int i2 = Station_getSystem(st);
    SolarSystem *b = sys->data[i2];
    this->distance = (int)((Galaxy *)(g))->distance(a, b);
    if (st != 0) {
        // Station::~Station() returns `this` in r0; the finisher consumes that pointer.
        ((Station *)(st))->dtor();
        Station_dtor_finish(st);
    }
}

struct Galaxy;
struct Station;

__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Mission::Mission(int id, int goods, int station) — the campaign-mission constructor.
Mission * Mission::ctor3(int id, int goods, int station) {
    Mission *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    self->field_0x30 = goods;
    self->targetStation = station;
    self->field_0x28 = 0;
    self->field_0x8 = 0;
    self->id = id;
    if (station < 0) {
        String_cstr_ctor(tmp, "", false);
    } else {
        Station *st = (Station *)((Galaxy *)(*g_galaxy))->getStation(station);
        ((Station *)(tmp))->getName();
    }
    ((String *)((char *)self + 0x40))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    String_cstr_ctor(tmp, "", false);
    ((String *)((char *)self + 0x4c))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    self->visible = 1;
    self->campaign = 1;
    self->field_0x4 = 0;
    self->instantAction = 0;
    self->field_0x70 = 0;
    self->field_0x38 = 0;
    return self;
}

// PIC globals: the Mission vtable base and the String::~String function pointer.
// Hidden visibility -> direct pc-relative single-deref (no GOT indirection),
// matching the target's `ldr; add pc; ldr` access.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern void (*String_dtor_fn)(void *s);

// Inner destructor: installs the vtable, then destroys the four String members
// (at 0x4c, 0x40, 0x1c, 0x10 — high to low), and returns `this`.
Mission * Mission::dtor_inner() {
    Mission *self = this;
    void (*dtor)(void *) = String_dtor_fn;
    *(void **)self = (char *)Mission_vtable + 8;
    dtor((char *)self + 0x4c);
    dtor((char *)self + 0x40);
    dtor((char *)self + 0x1c);
    dtor((char *)self + 0x10);
    return self;
}

// PIC globals (hidden -> single pc-relative deref): the Mission vtable base.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;
// String::String(String* this) default ctor -> 0x6efbc.
// String::String(String* out, const char* cstr, bool) -> 0x6ee18.
// String::operator=(String* this, const String* rhs) -> 0x6f2b0.
// String::~String(String* this) -> 0x6ee30.

// Mission::Mission() — default constructor. Installs the vtable, default-constructs
// the four String members, sets the name from a literal, and zero-inits the rest.
// The on-stack String temp triggers the -Oz stack-protector canary.
Mission * Mission::ctor_default() {
    Mission *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    String_cstr_ctor(tmp, "", false);
    ((String *)((char *)self + 0x10))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    self->campaign = 0;
    self->visible = 0;
    self->field_0x8 = 0;
    self->id = -1;
    self->field_0x28 = 0;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    self->reward = 0;
    self->instantAction = 0;
    self->field_0x70 = 0;
    self->field_0x4 = 0;
    self->field_0x38 = 0;
    self->targetStation = 0;
    return self;
}

// ---- recovered accessors / mutators (0x16baXX..0x16bcXX cluster) ----
// Field offsets are taken verbatim from the per-method disassembly. Where the
// existing struct already names a slot we use it; the few slots accessed only by
// these methods are reached via explicit byte offsets to keep the layout
// byte-compatible with the other translation units.

// Mission::getType() (0x16bae6): the mission "type" is just its id (+0xc).
int Mission::getType() {
    return this->id;
}

// Mission::getReward()/getCosts() both read +0x30; setReward() writes +0x30.
int Mission::getReward() {
    return this->field_0x30;
}

void Mission::setReward(int reward) {
    this->field_0x30 = reward;
}

int Mission::getCosts() {
    return this->field_0x30;     // 0x16bb62: reads +0x30, same slot as reward
}

// Mission::setCosts(int) (0x16bb66): writes the separate costs slot at +0x34.
void Mission::setCosts(int costs) {
    this->field_0x34 = costs;
}

int Mission::getBonus() {
    return this->field_0x38;
}

void Mission::setBonus(int bonus) {
    this->field_0x38 = bonus;
}

// Mission::getDifficulty() (0x16bc20): reads +0x58 (the slot the header calls reward).
int Mission::getDifficulty() {
    return this->reward;
}

int Mission::getDistance() {
    return this->distance;
}

int Mission::getStatusValue() {
    return this->field_0x70;
}

void Mission::setStatusValue(int value) {
    this->field_0x70 = value;
}

// Mission::getAgent()/setAgent() (0x16bcac/0x16bcb0): pointer slot at +0x8.
Agent *Mission::getAgent() {
    return *(Agent **)((char *)this + 0x8);
}

void Mission::setAgent(Agent *agent) {
    *(Agent **)((char *)this + 0x8) = agent;
}

int Mission::getClientImage() {
    return this->field_0x28;
}

int Mission::getClientRace() {
    return this->field_0x2c;
}

// Production-good index lives at +0x68, amount at +0x6c.
int Mission::getProductionGoodIndex() {
    return this->productionGoodsA;
}

int Mission::getProductionGoodAmount() {
    return this->productionGoodsB;
}

int Mission::getTargetStation() {
    return this->targetStation;
}

// Status flags packed into the int at +0x4: failed at byte +0x4, won at byte +0x5.
bool Mission::hasFailed() {
    return *((uint8_t *)this + 0x4) != 0;
}

void Mission::setFailed(bool failed) {
    *((uint8_t *)this + 0x4) = failed ? 1 : 0;
}

bool Mission::hasWon() {
    return *((uint8_t *)this + 0x5) != 0;
}

void Mission::setWon(bool won) {
    *((uint8_t *)this + 0x5) = won ? 1 : 0;
}

// Mission::setCampaignMission(bool) (0x16bb88): writes the campaign flag at +0x64.
void Mission::setCampaignMission(bool flag) {
    this->campaign = flag ? 1 : 0;
}

// Mission::isOutsideMission() (0x16bb40): base implementation always reports true.
bool Mission::isOutsideMission() {
    return true;
}

// Mission::setType(int) (0x16bcc8): the "type" is stored in the same slot as the id at +0xc.
void Mission::setType(int type) {
    this->id = type;
}

// ---- constructor / destructor aliases (0x16ba98 deleting dtor + ctor spellings) ----

// Deleting-destructor finisher (tail of 0x16ba98): the inner ~Mission already ran
// and returned `this`; all that remains is to release the storage.
void Mission::dtor_finish() {
    operator delete(this);
}

// Freelance "almost-killed wanted" path (Level::almostKillWanted): a campaign-style
// mission built from (type, goods, station) — identical to the (int,int,int) ctor.
Mission * Mission::ctor_akw(int type, int goods, int station) {
    return this->ctor3(type, goods, station);
}

// Record load, empty-mission branch (RecordHandler::readMission): (type, reward, station).
Mission * Mission::ctorEmpty(int type, int reward, int targetStation) {
    return this->ctor3(type, reward, targetStation);
}

// Record load, full-mission branch (RecordHandler::readMission): forwards to the
// seven-argument freelance constructor.
Mission * Mission::ctorFull(int type, const void *clientName, int *img, int clientRace,
                            int reward, int targetStation, int difficulty) {
    // ctor7(id, client, a=clientImage, b=clientRace, c=reward, station, reward-slot=difficulty)
    return this->ctor7(type, clientName, (int)(intptr_t)img, clientRace, reward,
                       targetStation, difficulty);
}

// Campaign-flag setter spelling used by Level::almostKillWanted.
void Mission::setCampaign_akw(int flag) {
    this->setCampaignMission(flag != 0);
}

// Won-flag setter spelling used by Level::almostKillWanted.
void Mission::setWon_akw(int flag) {
    this->setWon(flag != 0);
}
