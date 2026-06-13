#include "gof2/Wanted.h"
#include "gof2/String.h"


using AbyssEngine::String12;

extern "C" Wanted *String_default_ctor(Wanted *self);
extern "C" Wanted *Wanted_base_dtor(Wanted *self);

// ---- trivial field accessors (inlined in the binary; recovered from the
//      ctor's field layout). These back the Wanted_getX/setX extern shims. ----
int  Wanted::getIndex()            { return index; }
int  Wanted::getBoard()            { return board; }
int  Wanted::getRace()             { return race; }
int  Wanted::isMale()              { return male; }
int  Wanted::getShip()             { return shipId; }
int  Wanted::getWeapon()           { return weapon; }
int  Wanted::getHitpoints()        { return hitpoints; }
int  Wanted::getLoot()             { return lootItemId; }
int  Wanted::getLootAmount()       { return lootAmount; }
int  Wanted::getReward()           { return reward; }
int  Wanted::getRequiredBounties() { return requiredBounties; }
int  Wanted::getRequiredMission()  { return requiredMission; }
int  Wanted::getNumWingmen()       { return numWingmen; }
int  Wanted::getCurrentLocation()  { return currentLocation; }
int  Wanted::getTravelsTo()        { return travelsTo; }
int  Wanted::getLastSeen()         { return lastSeen; }
int *Wanted::getImageParts()       { return (int *)imageParts; }

void Wanted::setImageParts(int *parts)   { imageParts = parts; }
void Wanted::setCurrentLocation(int v)   { currentLocation = v; }
void Wanted::setTravelsTo(int v)         { travelsTo = v; }
void Wanted::setLastSeen(int v)          { lastSeen = v; }

// ---- isTerminated_12298a.cpp ----
uint8_t Wanted::isTerminated() {
    Wanted *self = this;
    return self->terminated;
}

// ---- setTerminated_12296c.cpp ----
void Wanted::setTerminated(bool v) {
    Wanted *self = this;
    self->terminated = v;
}

// ---- isActive_122984.cpp ----
uint8_t Wanted::isActive() {
    Wanted *self = this;
    return self->active;
}

// ---- setActive_122972.cpp ----
void Wanted::setActive(bool v) {
    Wanted *self = this;
    self->active = v;
}

// ---- getName_122924.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns the name String (stored at offset 0x0) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
// RetStr is defined in gof2/Wanted.h.

RetStr Wanted::getName() {
    Wanted *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)(&self->name), false);
    return r;
}

// ---- Wanted_122890.cpp ----
// String::String(String* this) default ctor -> 0x6efbc (returns this in r0)
// String::operator=(String* this, const String* rhs) -> 0x6f2b0

// The String argument has a non-trivial copy ctor/dtor in the real engine, so the
// C++ ABI passes it by invisible reference -> it arrives as a pointer in r2.
Wanted * Wanted::ctor(int p1, const String12 &p2, int p3, int p4, bool p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14) {
    Wanted *self = this;
    Wanted *r = String_default_ctor(self);
    r->index = p1;
    ((String *)(r))->assign((String *)&p2);
    self->board = p3;
    self->race = p4;
    self->male = p5;
    self->terminated = 0;
    self->active = 0;
    self->currentLocation = -1;
    self->travelsTo = -1;
    self->lastSeen = -1;
    self->shipId = p6;
    self->weapon = p7;
    self->hitpoints = p8;
    self->lootItemId = p9;
    self->lootAmount = p10;
    self->reward = p11;
    self->requiredBounties = p12;
    self->requiredMission = p13;
    self->numWingmen = p14;
    return self;
}

// ---- _Wanted_122904.cpp ----
// operator delete[](void*) -> 0x6ebfc
// Base subobject destructor (String::~String at offset 0), returns the object pointer.

// Frees the buffer at +0x40 (array delete), clears it, then tail-calls the base dtor.
Wanted * Wanted::dtor() {
    Wanted *self = this;
    void *p = self->imageParts;
    if (p != 0) {
        ::operator delete[](p);
    }
    self->imageParts = 0;
    return Wanted_base_dtor(self);
}

// ---- Wanted_base_dtor (base subobject destructor) ----
// Wanted's only base subobject is its leading AbyssEngine::String `name` (offset 0x0).
// The compiler-generated base-object destructor therefore just runs String::~String on
// `name` and returns the (Wanted*) object pointer for the destructor chain. Wanted::dtor()
// above frees the imageParts buffer first, then tail-calls this.
Wanted * Wanted::base_dtor() {
    name.dtor();
    return this;
}
