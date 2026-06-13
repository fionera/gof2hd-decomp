#include "gof2/game/world/Wanted.h"
#include "gof2/game/core/String.h"

using AbyssEngine::String;

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

uint8_t Wanted::isTerminated() {
    return this->terminated;
}

void Wanted::setTerminated(bool v) {
    this->terminated = v;
}

uint8_t Wanted::isActive() {
    return this->active;
}

void Wanted::setActive(bool v) {
    this->active = v;
}

// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns the name String (stored at offset 0x0) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
// String is defined in gof2/Wanted.h.

String Wanted::getName() {
    Wanted *self = this;
    return *(String *)(&self->name);
}

// String::String(String* this) default ctor -> 0x6efbc (returns this in r0)
// String::operator=(String* this, const String* rhs) -> 0x6f2b0

// The String argument has a non-trivial copy ctor/dtor in the real engine, so the
// C++ ABI passes it by invisible reference -> it arrives as a pointer in r2.
Wanted * Wanted::ctor(int p1, const String &p2, int p3, int p4, bool p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14) {
    this->name.ctor();
    this->index = p1;
    this->name.assign((String *)&p2);
    this->board = p3;
    this->race = p4;
    this->male = p5;
    this->terminated = 0;
    this->active = 0;
    this->currentLocation = -1;
    this->travelsTo = -1;
    this->lastSeen = -1;
    this->shipId = p6;
    this->weapon = p7;
    this->hitpoints = p8;
    this->lootItemId = p9;
    this->lootAmount = p10;
    this->reward = p11;
    this->requiredBounties = p12;
    this->requiredMission = p13;
    this->numWingmen = p14;
    return this;
}

// operator delete[](void*) -> 0x6ebfc
// Base subobject destructor (String::~String at offset 0), returns the object pointer.

// Frees the buffer at +0x40 (array delete), clears it, then tail-calls the base dtor.
Wanted * Wanted::dtor() {
    void *p = this->imageParts;
    if (p != 0) {
        ::operator delete[](p);
    }
    this->imageParts = 0;
    return ((Wanted *)(this))->base_dtor();
}

// ---- ((Wanted *)(base subobject destructor))->base_dtor() ----
// Wanted's only base subobject is its leading AbyssEngine::String `name` (offset 0x0).
// The compiler-generated base-object destructor therefore just runs String::~String on
// `name` and returns the (Wanted*) object pointer for the destructor chain. Wanted::dtor()
// above frees the imageParts buffer first, then tail-calls this.
Wanted * Wanted::base_dtor() {
    name.dtor();
    return this;
}
