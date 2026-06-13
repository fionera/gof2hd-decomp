#include "gof2/game/ship/Agent.h"
#include "gof2/game/core/String.h"

using AbyssEngine::String;

// String (the 12-byte aggregate returned via sret) is declared in Agent.h.

// Minimal view of a virtual object whose deleting-dtor lives at vt[0]+4.
struct VObj { void (*vt[8])(void *); };

String Agent::getStationName() {
    return this->stationName;
}

uint8_t Agent::hasAcceptedOffer() {
    return this->offerAccepted;
}

bool Agent::isStoryAgent() {
    return this->category == 0;
}

void * Agent::getWingmanNames() {
    return this->wingmanNames;
}

int * Agent::getImageParts() {
    return this->imageParts;
}

struct Mission;

Mission * Agent::getMission() {
    return this->mission;
}

struct Mission;

void Agent::setMission(Mission *mission) {
    this->mission = mission;
}

int Agent::getSellModIndex() {
    return this->sellModIndex;
}

// AbyssEngine::String::operator=(String* dst, String src-by-value)

// Agent::setStationName(String) — this in r0, String by value in r1..r3.
// Tail-calls operator= on the field at +0x78.
void Agent::setStationName(String src) {
    (&this->stationName)->assign((String *)&src);
}

String Agent::getMissionString() {
    return this->missionString;
}

uint8_t Agent::isMale() {
    return this->male;
}

// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns String by value. The copy-ctor returns void, so the compiler cannot
// assume r0 survives the call and must keep a frame + restore the sret pointer.
String Agent::getName() {
    return this->name;
}

// 4-entry price table loaded PC-relative (table contents in local rodata).
static const int kModPriceTable[4] = { 0, 0, 0, 0 };

int Agent::getModPricePercentage() {
    uint32_t i = this->sellModIndex;
    if (i < 4)
        return kModPriceTable[i];
    return 0x28;
}

void Agent::setOfferAccepted(bool v) {
    this->offerAccepted = v;
}

void Agent::nextEvent() {
    this->eventCount = this->eventCount + 1;
}

// Agent::setKnown(bool) — an agent counts as "known" once its dialogue/event
// counter is non-zero (see isKnown()). The space-lounge marks a freshly-met,
// non-story agent as known by advancing it onto its first event.
void Agent::setKnown(bool known) {
    if (known) {
        if (this->eventCount <= 0)
            this->eventCount = 1;
    } else {
        this->eventCount = 0;
    }
}

uint8_t Agent::hasReward() {
    return this->rewardAtNextChat;
}

void Agent::setImageParts(int *parts) {
    this->imageParts = parts;
}

bool Agent::isKnown() {
    return this->eventCount > 0;
}

bool Agent::isGenericAgent() {
    return this->category == 1;
}

String Agent::getSystemName() {
    return this->systemName;
}

void Agent::giveRewardAtNextChat(bool v) {
    this->rewardAtNextChat = v;
}

// Triple (item/price/percentage) is declared in Agent.h.

// target: adds r0,#0x34; stmia r0!,{r1,r2,r3}; bx lr  (returns this+0x40)
Triple * Agent::setSellItemData(int a, int b, int c) {
    this->sellItemIndex = a;
    this->sellItemQuantity = b;
    this->sellItemPrice = c;
    return (Triple *)&this->type;   // original returned this+0x40 (past the triple)
}

// Agent::setWingmanFriendNames(Array<String*>*) — this in r0, param in r1.
void Agent::setWingmanFriendNames(Array<AbyssEngine::String*> *param) {
    VObj *f0c = (VObj *)this->wingman1;
    if (f0c != 0)
        (*(void (**)(void *))((char *)f0c->vt[0] + 4))(f0c);
    this->wingman1 = 0;
    VObj *f10 = (VObj *)this->wingman2;
    if (f10 != 0)
        (*(void (**)(void *))((char *)f10->vt[0] + 4))(f10);
    this->wingman2 = 0;
    if (this->wingmanNames != 0) {
        delete this->wingmanNames;
        this->wingmanNames = 0;
    }
    this->wingmanNames = new Array<AbyssEngine::String*>();
    String *ns = (String *)::operator new(sizeof(String));
    ns->ctor_copy(&this->name, false);
    this->wingmanNames->push_back(ns);
    this->wingmanCount = 0;
    if (param == 0)
        return;
    uint32_t n = param->size();
    if (n != 0) {
        String *w0 = (*param)[0];
        if (w0 != 0) {
            this->wingmanCount = 1;
            this->wingman1 = w0;
            this->wingmanNames->push_back(w0);
            n = param->size();
        }
        if (n >= 2) {
            String *w1 = (*param)[1];
            if (w1 != 0) {
                this->wingman2 = w1;
                this->wingmanCount = this->wingmanCount + 1;
                this->wingmanNames->push_back(w1);
            }
        }
    }
    this->finishWingman(param);
}

// Tail of setWingmanFriendNames (engine 0x18762a -> 0x1ab098): once the source
// array's String* elements have been moved into wingmanNames, destruct the
// Array<String*> and release its heap backing (delete = vector dtor + operator
// delete, the compiler's tail-called teardown of the consumed source array).
void Agent::finishWingman(Array<AbyssEngine::String*> *consumedArray) {
    delete consumedArray;
}

String Agent::getWingmanName(int idx) {
    if (idx == 1)
        return *(String *)this->wingman1;
    if (idx == 0)
        return this->name;
    return *(String *)this->wingman2;
}

void Agent::setSystemName(String src) {
    (&this->systemName)->assign((String *)&src);
}

// Agent::~Agent() — real C++ destructor so the demangled symbol contains "~Agent".

__attribute__((minsize)) Agent::~Agent() noexcept(false)
{
    if (this->imageParts != 0)
        ::operator delete[](this->imageParts);

    VObj *o = (VObj *)this->wingman1;        // virtual; deleting-dtor at vt[0]+4
    this->imageParts = 0;
    if (o != 0) {
        (*(void (**)(void *))((char *)o->vt[0] + 4))(o);
        this->wingman1 = 0;
    }
    // name / systemName / missionString / stationName are real String members and
    // are destroyed automatically when ~Agent() returns.
}

// String temp lifecycle helpers (compiler emits the canary via -fstack-protector).

// Agent::setMissionString(String) — this in r0, source String* in r1.
void Agent::setMissionString(void *src) {
    String tmp;
    ((String *)(&tmp))->ctor_copy((String *)(src), false);
    (&this->missionString)->assign((String *)&tmp);
    ((String *)(&tmp))->dtor();
}

// ---- simple field accessors (recovered) ----
// Single-instruction veneers that load/store a member; offsets confirmed from
// the constructor's initializer list above.

int Agent::getStation() {
    return this->station;
}

int Agent::getSystem() {
    return this->system;
}

int Agent::getRace() {
    return this->race;
}

int Agent::getIndex() {
    return (int)this->type;
}

int Agent::getCosts() {
    return this->costs;
}

void Agent::setCosts(int costs) {
    this->costs = costs;
}

int Agent::getOffer() {
    return this->offer;
}

void Agent::setOffer(int offer) {
    this->offer = offer;
}

int Agent::getEvent() {
    return this->eventCount;
}

void Agent::setEvent(int event) {
    this->eventCount = event;
}

int Agent::getSellItemIndex() {
    return this->sellItemIndex;
}

int Agent::getSellItemQuantity() {
    return this->sellItemQuantity;
}

int Agent::getSellItemPrice() {
    return this->sellItemPrice;
}

void Agent::setSellItemPrice(int price) {
    this->sellItemPrice = price;
}

int Agent::getSellSystemIndex() {
    return this->sellSystemIndex;
}

int Agent::getSellBlueprintIndex() {
    return this->sellBlueprintIndex;
}

int Agent::getWingmanFriendsCount() {
    return this->wingmanCount;
}

// Agent::Agent(int kind, String name, int p4, int p5, int p6, bool p7,
//              int p8, int p9, int p10, int p11)
Agent::Agent(unsigned kind, void *name, int p4, int p5, int p6, char p7, int p8, int p9, int p10, int p11) {
    ((String *)((char *)this + 0x00))->ctor();
    (&this->systemName)->ctor();
    (&this->missionString)->ctor();
    (&this->stationName)->ctor();
    this->type = kind;
    ((String *)(this))->assign((String *)name);
    this->station = p4;
    this->system = p5;
    this->race = p6;
    this->male = p7;
    this->eventCount = 0;
    this->field_0x30 = -1;
    this->offer = -1;
    this->sellSystemIndex = p8;
    if (p8 >= 0)
        this->offer = 4;
    this->sellBlueprintIndex = p9;
    if (p9 >= 0)
        this->offer = 3;
    this->offerAccepted = 0;
    this->field_0x24 = 0;
    this->wingman1 = 0;
    this->wingman2 = 0;
    this->wingmanCount = 0;
    this->sellItemIndex = 0;
    this->sellItemQuantity = 0;
    this->sellItemPrice = p11;
    this->costs = 0;
    this->imageParts = 0;
    this->mission = 0;
    this->wingmanNames = 0;
    this->field_0x28 = -1;
    this->field_0x2c = -1;
    this->category = kind >> 31;
    if (p10 >= 0)
        this->offer = 8;
    if (kind == 0x19)
        this->offer = 9;
    else if (kind == 0x1a)
        this->offer = 10;
    this->sellModIndex = p10;
}
