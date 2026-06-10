#include "Mission.h"


extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
extern "C" void *GameText_getText(GameText *table, int id);
extern "C" void *String_assign_ref(void *self, const String12 &rhs);
extern "C" void *operator_new(uint32_t n);
extern "C" void String_dtor(void *s);
extern "C" Station *Galaxy_getStation(Galaxy *g, int idx);
extern "C" void Station_getName(void *out, Station *s);
extern "C" void String_assign(void *self, void *rhs);
extern "C" Mission *Mission_dtor_inner(Mission *self);
extern "C" void Mission_dtor_finish(Mission *self);
extern "C" void String_default_ctor(void *s);
extern "C" Systems *Galaxy_getSystems(Galaxy *g);
extern "C" Station *Status_getStation(Status *s);
extern "C" int Station_getSystem(Station *s);
extern "C" float Galaxy_distance(Galaxy *g, SolarSystem *a, SolarSystem *b);
extern "C" Station *Station_dtor(Station *s);
extern "C" void Station_dtor_finish(Station *s);

// ---- isInstantActionMission_15bb78.cpp ----
extern "C" uint8_t Mission_isInstantActionMission(Mission *self) {
    return F<uint8_t>(self, 0x5c);
}

// ---- getTargetStationName_15bb90.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Mission_getTargetStationName(Mission *self) {
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x40, false);
    return r;
}

// ---- isCampaignMission_15bb7e.cpp ----
extern "C" bool Mission_isCampaignMission(Mission *self) {
    return F<int>(self, 0x64) != 0;
}

// ---- getDescription_15bb2c.cpp ----
// AbyssEngine::String::String(String* out, const char* cstr, bool) -> 0x6ee18
struct RetStr { uint32_t a, b, c; };

// Returns a fixed description String built from a string literal.
extern "C" RetStr Mission_getDescription(Mission *self) {
    RetStr r;
    String_cstr_ctor(&r, "", false);
    return r;
}

// ---- getTargetSystemName_15bb9e.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Mission_getTargetSystemName(Mission *self) {
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

struct RetStr { uint32_t a, b, c; };

// Mission::getName(): campaign missions use a fixed literal name; freelance ones
// look up "<id+0x162>" in the global text table.
extern "C" RetStr Mission_getName(Mission *self) {
    RetStr r;
    if (F<int>(self, 0x64) != 0) {
        String_cstr_ctor(&r, "", false);
    } else {
        void *txt = GameText_getText(*g_gameText, F<int>(self, 0xc) + 0x162);
        String_copy_ctor(&r, txt, false);
    }
    return r;
}

// ---- setProductionGoods_15bad0.cpp ----
extern "C" void Mission_setProductionGoods(Mission *self, int a, int b) {
    F<int>(self, 0x68) = a;
    F<int>(self, 0x6c) = b;
}

// ---- setTargetName_15bcc2.cpp ----
// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548

// Mission::setTargetName(String by value): the String has a non-trivial copy
// ctor/dtor so it is passed by invisible reference (pointer in r1).
extern "C" void *Mission_setTargetName(Mission *self, const String12 &rhs) {
    return String_assign_ref((char *)self + 0x1c, rhs);
}

// ---- getClientName_15bb44.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> 0x6f028
struct RetStr { uint32_t a, b, c; };

// Returns the client name String (offset 0x10) by value. The void copy-ctor forces
// a frame + non-tail blx (the sret r0 must be restored).
extern "C" RetStr Mission_getClientName(Mission *self) {
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x10, false);
    return r;
}

// ---- isVisible_15baba.cpp ----
extern "C" uint8_t Mission_isVisible(Mission *self) {
    return F<uint8_t>(self, 0x74);
}

// ---- isEmpty_15baa8.cpp ----
extern "C" bool Mission_isEmpty(Mission *self) {
    return F<int>(self, 0xc) == -1;
}

// ---- getTargetName_15bcb4.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Mission_getTargetName(Mission *self) {
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x1c, false);
    return r;
}

// ---- setInstantActionMission_15bb72.cpp ----
extern "C" void Mission_setInstantActionMission(Mission *self, bool v) {
    F<uint8_t>(self, 0x5c) = v;
}

// ---- setTargetSystemName_15bbac.cpp ----
// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548

extern "C" void *Mission_setTargetSystemName(Mission *self, const String12 &rhs) {
    return String_assign_ref((char *)self + 0x4c, rhs);
}

// ---- setVisible_15bab4.cpp ----
extern "C" void Mission_setVisible(Mission *self, bool v) {
    F<uint8_t>(self, 0x74) = v;
}

// ---- clone_15bccc.cpp ----
// Mission::Mission(int, String, int*, int, int, int, int) -> 0x15b64c. The by-value
// String is passed by invisible reference -> model it as a pointer parameter.
extern "C" void Mission_ctor7(Mission *self, int id, const void *name,
                              int a, int b, int c, int d, int e);

// Mission::clone(): deep-copies this mission into a freshly allocated Mission via
// the 7-arg constructor, then copies the instant-action flag. The on-stack String
// temp triggers the -Oz stack-protector canary.
extern "C" Mission *Mission_clone(Mission *self) {
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Mission *m = (Mission *)operator_new(0x78);
    int id = self->f_c;
    String_copy_ctor(name, (char *)self + 0x10, false);
    Mission_ctor7(m, id, name,
                  self->f_28, self->f_2c, self->f_30,
                  self->f_3c, self->f_58);
    String_dtor(name);
    m->f_5c = self->f_5c;
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
extern "C" void Mission_setTargetStation(Mission *self, int idx) {
    unsigned char name[sizeof(String12)] __attribute__((aligned(4)));
    Galaxy **gp = g_galaxy;
    self->f_3c = idx;
    Station *st = Galaxy_getStation(*gp, idx);
    Station_getName(name, st);
    String_assign((char *)self + 0x40, name);
    String_dtor(name);
}

// ---- _Mission_15ba98.cpp ----
// Mission::~Mission() (the inner/base dtor at 0x15ba54) returns `this` in r0.
// Deallocation/base helper tail-called at the end (0x1ab098).

// Deleting/complete destructor: runs the inner dtor (which returns `this`), then
// tail-calls the finisher with that pointer — so `this` need not be saved.
extern "C" void Mission_dtor(Mission *self) {
    Mission_dtor_finish(Mission_dtor_inner(self));
}

// ---- Mission_15b64c.cpp ----
struct Galaxy;
struct Station;

__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Mission::Mission(int id, String client, int* targets, int a, int b, int station, int reward)
// — the freelance-mission constructor.
extern "C" Mission *
Mission_ctor7(Mission *self, int id, const void *client, int a, int b, int c, int station, int reward) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    self->f_c = id;
    String_assign((char *)self + 0x10, (void *)client);
    self->f_28 = a;
    self->f_2c = b;
    self->f_30 = c;
    self->f_3c = station;
    Station *st = Galaxy_getStation(*g_galaxy, station);
    Station_getName(tmp, st);
    String_assign((char *)self + 0x40, tmp);
    String_dtor(tmp);
    self->f_58 = reward;
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x4c, tmp);
    String_dtor(tmp);
    self->f_74 = 1;
    self->f_64 = 0;
    self->f_5c = 0;
    self->f_4 = 0;
    self->f_8 = 0;
    self->f_38 = 0;
    return self;
}

// ---- Mission_15b978.cpp ----
// PIC global (hidden -> single pc-relative deref): the Mission vtable base.
__attribute__((visibility("hidden"))) extern void *Mission_vtable;

// Mission::Mission(int) — like the default ctor but stores the given id at +0xc.
extern "C" Mission *Mission_ctor_int(Mission *self, int id) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x10, tmp);
    String_dtor(tmp);
    self->f_64 = 0;
    self->f_74 = 0;
    self->f_8 = 0;
    self->f_c = id;
    self->f_28 = 0;
    self->f_2c = 0;
    self->f_30 = 0;
    self->f_58 = 0;
    self->f_5c = 0;
    self->f_70 = 0;
    self->f_4 = 0;
    self->f_38 = 0;
    self->f_3c = 0;
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
extern "C" void Mission_calcDistance(Mission *self) {
    Galaxy **gp = g_galaxy;
    Station *st = Galaxy_getStation(*gp, self->f_3c);
    Systems *sys = Galaxy_getSystems(*gp);
    Galaxy *g = *gp;
    int i1 = Station_getSystem(Status_getStation(*g_status));
    SolarSystem *a = sys->data[i1];
    int i2 = Station_getSystem(st);
    SolarSystem *b = sys->data[i2];
    self->f_60 = (int)Galaxy_distance(g, a, b);
    if (st != 0) {
        Station_dtor_finish(Station_dtor(st));
    }
}

// ---- Mission_15b770.cpp ----
struct Galaxy;
struct Station;

__attribute__((visibility("hidden"))) extern void *Mission_vtable;
__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Mission::Mission(int id, int goods, int station) — the campaign-mission constructor.
extern "C" Mission *
Mission_ctor3(Mission *self, int id, int goods, int station) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    self->f_30 = goods;
    self->f_3c = station;
    self->f_28 = 0;
    self->f_8 = 0;
    self->f_c = id;
    if (station < 0) {
        String_cstr_ctor(tmp, "", false);
    } else {
        Station *st = Galaxy_getStation(*g_galaxy, station);
        Station_getName(tmp, st);
    }
    String_assign((char *)self + 0x40, tmp);
    String_dtor(tmp);
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x4c, tmp);
    String_dtor(tmp);
    self->f_74 = 1;
    self->f_64 = 1;
    self->f_4 = 0;
    self->f_5c = 0;
    self->f_70 = 0;
    self->f_38 = 0;
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
extern "C" Mission *Mission_dtor_inner(Mission *self) {
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
extern "C" Mission *Mission_ctor_default(Mission *self) {
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    *(void **)self = (char *)Mission_vtable + 8;
    String_default_ctor((char *)self + 0x10);
    String_default_ctor((char *)self + 0x1c);
    String_default_ctor((char *)self + 0x40);
    String_default_ctor((char *)self + 0x4c);
    String_cstr_ctor(tmp, "", false);
    String_assign((char *)self + 0x10, tmp);
    String_dtor(tmp);
    self->f_64 = 0;
    self->f_74 = 0;
    self->f_8 = 0;
    self->f_c = -1;
    self->f_28 = 0;
    self->f_2c = 0;
    self->f_30 = 0;
    self->f_58 = 0;
    self->f_5c = 0;
    self->f_70 = 0;
    self->f_4 = 0;
    self->f_38 = 0;
    self->f_3c = 0;
    return self;
}
