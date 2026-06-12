#include "gof2/Mission.h"
#include "gof2/Galaxy.h"
#include "gof2/Status.h"
#include "gof2/GameText.h"
#include "gof2/Station.h"
#include "gof2/String.h"

struct Systems;

extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
extern "C" void *String_assign_ref(void *self, const String12 &rhs);
extern "C" void Mission_dtor_finish(Mission *self);
extern "C" void String_default_ctor(void *s);
extern "C" Systems *Galaxy_getSystems(Galaxy *g);
extern "C" int Station_getSystem(Station *s);
float Galaxy_distance(Galaxy *g, SolarSystem *a, SolarSystem *b);
extern "C" void Station_dtor_finish(Station *s);

// ---- isInstantActionMission_15bb78.cpp ----
uint8_t Mission::isInstantActionMission() {
    Mission *self = this;
    return self->instantAction;
}

// ---- getTargetStationName_15bb90.cpp ----
// `struct RetStr` provided by gof2/Station.h (via Mission.h).

RetStr Mission::getTargetStationName() {
    Mission *self = this;
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x40, false);
    return r;
}

// ---- isCampaignMission_15bb7e.cpp ----
bool Mission::isCampaignMission() {
    Mission *self = this;
    return self->campaign != 0;
}

// ---- getDescription_15bb2c.cpp ----
// AbyssEngine::String::String(String* out, const char* cstr, bool) -> 0x6ee18

// Returns a fixed description String built from a string literal.
RetStr Mission::getDescription() {
    Mission *self = this;
    RetStr r;
    String_cstr_ctor(&r, "", false);
    return r;
}

// ---- getTargetSystemName_15bb9e.cpp ----

RetStr Mission::getTargetSystemName() {
    Mission *self = this;
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x4c, false);
    return r;
}

// ---- getName_15baec.cpp ----
struct GameText;

// String::String(String* out, const char* cstr, bool) -> 0x6ee18.
// String::String(String* out, const String* src, bool) -> 0x6f028.
// GameText::getText(GameText* table, int id) -> 0x72f70 returns a String*.
// Text table singleton (hidden -> single pc-relative deref).
__attribute__((visibility("hidden"))) extern GameText **g_gameText;


// Mission::getName(): campaign missions use a fixed literal name; freelance ones
// look up "<id+0x162>" in the global text table.
RetStr Mission::getName() {
    Mission *self = this;
    RetStr r;
    if (self->campaign != 0) {
        String_cstr_ctor(&r, "", false);
    } else {
        void *txt = ((GameText *)(*g_gameText))->getText(self->id + 0x162);
        String_copy_ctor(&r, txt, false);
    }
    return r;
}

// ---- setProductionGoods_15bad0.cpp ----
void Mission::setProductionGoods(int a, int b) {
    Mission *self = this;
    self->productionGoodsA = a;
    self->productionGoodsB = b;
}

// ---- setTargetName_15bcc2.cpp ----
// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548

// Mission::setTargetName(String by value): the String has a non-trivial copy
// ctor/dtor so it is passed by invisible reference (pointer in r1).
void * Mission::setTargetName(const String12 &rhs) {
    Mission *self = this;
    return String_assign_ref((char *)self + 0x1c, rhs);
}

// ---- getClientName_15bb44.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> 0x6f028

// Returns the client name String (offset 0x10) by value. The void copy-ctor forces
// a frame + non-tail blx (the sret r0 must be restored).
RetStr Mission::getClientName() {
    Mission *self = this;
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x10, false);
    return r;
}

// ---- isVisible_15baba.cpp ----
uint8_t Mission::isVisible() {
    Mission *self = this;
    return self->visible;
}

// ---- isEmpty_15baa8.cpp ----
bool Mission::isEmpty() {
    Mission *self = this;
    return self->id == -1;
}

// ---- getTargetName_15bcb4.cpp ----

RetStr Mission::getTargetName() {
    Mission *self = this;
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x1c, false);
    return r;
}

// ---- setInstantActionMission_15bb72.cpp ----
void Mission::setInstantActionMission(bool v) {
    Mission *self = this;
    self->instantAction = v;
}

// ---- setTargetSystemName_15bbac.cpp ----
// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548

void * Mission::setTargetSystemName(const String12 &rhs) {
    Mission *self = this;
    return String_assign_ref((char *)self + 0x4c, rhs);
}

// ---- setVisible_15bab4.cpp ----
void Mission::setVisible(bool v) {
    Mission *self = this;
    self->visible = v;
}

// ---- clone_15bccc.cpp ----
// Mission::Mission(int, String, int*, int, int, int, int) -> 0x15b64c. The by-value
// String is passed by invisible reference -> model it as a pointer parameter.

// Mission::clone(): deep-copies this mission into a freshly allocated Mission via
// the 7-arg constructor, then copies the instant-action flag. The on-stack String
// temp triggers the -Oz stack-protector canary.
Mission * Mission::clone() {
    Mission *self = this;
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Mission *m = (Mission *)::operator new(0x78);
    int id = self->id;
    String_copy_ctor(name, (char *)self + 0x10, false);
    ((Mission *)(m))->ctor7(id, name, self->field_0x28, self->field_0x2c, self->field_0x30, self->targetStation, self->reward);
    ((String *)(name))->dtor();
    m->instantAction = self->instantAction;
    return m;
}

// ---- setTargetStation_15bbb4.cpp ----
struct Galaxy;
struct Station;

// Galaxy singleton pointer global (hidden -> single pc-relative deref to the value).
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Sets the target station index (+0x3c) and caches that station's name into the
// targetStationName String (+0x40). The char-array-backed String temp triggers
// the -Oz/-fstack-protector canary the target emits.
void Mission::setTargetStation(int idx) {
    Mission *self = this;
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Galaxy **gp = g_galaxy;
    self->targetStation = idx;
    Station *st = (Station *)((Galaxy *)(*gp))->getStation(idx);
    ((Station *)(name))->getName();
    ((String *)((char *)self + 0x40))->assign((String *)name);
    ((String *)(name))->dtor();
}

// ---- _Mission_15ba98.cpp ----
// Mission::~Mission() (the inner/base dtor at 0x15ba54) returns `this` in r0.
// Deallocation/base helper tail-called at the end (0x1ab098).

// Deleting/complete destructor: runs the inner dtor (which returns `this`), then
// tail-calls the finisher with that pointer — so `this` need not be saved.
void Mission::dtor() {
    Mission *self = this;
    Mission_dtor_finish(((Mission *)(self))->dtor_inner());
}

// ---- Mission_15b64c.cpp ----
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

// ---- Mission_15b978.cpp ----
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

// ---- calcDistance_15bc28.cpp ----
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
    Mission *self = this;
    Galaxy **gp = g_galaxy;
    Station *st = (Station *)((Galaxy *)(*gp))->getStation(self->targetStation);
    Systems *sys = Galaxy_getSystems(*gp);
    Galaxy *g = *gp;
    int i1 = Station_getSystem(((Status *)(*g_status))->getStation());
    SolarSystem *a = sys->data[i1];
    int i2 = Station_getSystem(st);
    SolarSystem *b = sys->data[i2];
    self->distance = (int)Galaxy_distance(g, a, b);
    if (st != 0) {
        // Station::~Station() returns `this` in r0; the finisher consumes that pointer.
        ((Station *)(st))->dtor();
        Station_dtor_finish(st);
    }
}

// ---- Mission_15b770.cpp ----
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

// ---- _Mission_15ba54.cpp ----
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

// ---- Mission_15b89c.cpp ----
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
