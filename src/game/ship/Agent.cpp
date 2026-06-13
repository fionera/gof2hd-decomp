#include "gof2/game/ship/Agent.h"
#include "gof2/game/core/String.h"

using AbyssEngine::String12;

// RetStr (the 12-byte aggregate returned via sret) is declared in Agent.h.

// Minimal view of a virtual object whose deleting-dtor lives at vt[0]+4.
struct VObj { void (*vt[8])(void *); };

RetStr Agent::getStationName() {
    Agent *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0x78), false);
    return r;
}

uint8_t Agent::hasAcceptedOffer() {
    Agent *self = this;
    return self->offerAccepted;
}

bool Agent::isStoryAgent() {
    Agent *self = this;
    return self->category == 0;
}

void * Agent::getWingmanNames() {
    Agent *self = this;
    return self->wingmanNames;
}

int * Agent::getImageParts() {
    Agent *self = this;
    return self->imageParts;
}

struct Mission;

Mission * Agent::getMission() {
    Agent *self = this;
    return self->mission;
}

struct Mission;

void Agent::setMission(Mission *mission) {
    Agent *self = this;
    self->mission = mission;
}

int Agent::getSellModIndex() {
    Agent *self = this;
    return self->sellModIndex;
}

// AbyssEngine::String::operator=(String* dst, String src-by-value)

// Agent::setStationName(String) — this in r0, String by value in r1..r3.
// Tail-calls operator= on the field at +0x78.
void Agent::setStationName(String12 src) {
    Agent *self = this;
    ((String *)((char *)self + 0x78))->assign((String *)&src);
}

RetStr Agent::getMissionString() {
    Agent *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0x6c), false);
    return r;
}

uint8_t Agent::isMale() {
    Agent *self = this;
    return self->male;
}

// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns String by value. The copy-ctor returns void, so the compiler cannot
// assume r0 survives the call and must keep a frame + restore the sret pointer.
RetStr Agent::getName() {
    Agent *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0x00), false);
    return r;
}

// 4-entry price table loaded PC-relative (table contents in local rodata).
static const int kModPriceTable[4] = { 0, 0, 0, 0 };

int Agent::getModPricePercentage() {
    Agent *self = this;
    uint32_t i = self->sellModIndex;
    if (i < 4)
        return kModPriceTable[i];
    return 0x28;
}

void Agent::setOfferAccepted(bool v) {
    Agent *self = this;
    self->offerAccepted = v;
}

void Agent::nextEvent() {
    Agent *self = this;
    self->eventCount = self->eventCount + 1;
}

// Agent::setKnown(bool) — an agent counts as "known" once its dialogue/event
// counter is non-zero (see isKnown()). The space-lounge marks a freshly-met,
// non-story agent as known by advancing it onto its first event.
void Agent::setKnown(bool known) {
    Agent *self = this;
    if (known) {
        if (self->eventCount <= 0)
            self->eventCount = 1;
    } else {
        self->eventCount = 0;
    }
}

uint8_t Agent::hasReward() {
    Agent *self = this;
    return self->rewardAtNextChat;
}

void Agent::setImageParts(int *parts) {
    Agent *self = this;
    self->imageParts = parts;
}

bool Agent::isKnown() {
    Agent *self = this;
    return self->eventCount > 0;
}

bool Agent::isGenericAgent() {
    Agent *self = this;
    return self->category == 1;
}

RetStr Agent::getSystemName() {
    Agent *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0x18), false);
    return r;
}

void Agent::giveRewardAtNextChat(bool v) {
    Agent *self = this;
    self->rewardAtNextChat = v;
}

// Triple (item/price/percentage) is declared in Agent.h.

// target: adds r0,#0x34; stmia r0!,{r1,r2,r3}; bx lr  (returns this+0x40)
Triple * Agent::setSellItemData(int a, int b, int c) {
    Agent *self = this;
    Triple *p = (Triple *)((char *)self + 0x34);
    p->a = a;
    p->b = b;
    p->c = c;
    return p + 1;
}

// Agent::setWingmanFriendNames(Array<String*>*) — this in r0, param in r1.
void Agent::setWingmanFriendNames(Array<AbyssEngine::String*> *param) {
    Agent *self = this;
    VObj *f0c = (VObj *)self->wingman1;
    if (f0c != 0)
        (*(void (**)(void *))((char *)f0c->vt[0] + 4))(f0c);
    self->wingman1 = 0;
    VObj *f10 = (VObj *)self->wingman2;
    if (f10 != 0)
        (*(void (**)(void *))((char *)f10->vt[0] + 4))(f10);
    self->wingman2 = 0;
    if (self->wingmanNames != 0) {
        delete self->wingmanNames;
        self->wingmanNames = 0;
    }
    self->wingmanNames = new Array<AbyssEngine::String*>();
    String *ns = (String *)::operator new(sizeof(String));
    ns->ctor_copy((String *)(self), false);
    self->wingmanNames->push_back(ns);
    self->wingmanCount = 0;
    if (param == 0)
        return;
    uint32_t n = param->size();
    if (n != 0) {
        String *w0 = (*param)[0];
        if (w0 != 0) {
            self->wingmanCount = 1;
            self->wingman1 = w0;
            self->wingmanNames->push_back(w0);
            n = param->size();
        }
        if (n >= 2) {
            String *w1 = (*param)[1];
            if (w1 != 0) {
                self->wingman2 = w1;
                self->wingmanCount = self->wingmanCount + 1;
                self->wingmanNames->push_back(w1);
            }
        }
    }
    self->finishWingman(param);
}

// Tail of setWingmanFriendNames (engine 0x18762a -> 0x1ab098): once the source
// array's String* elements have been moved into wingmanNames, destruct the
// Array<String*> and release its heap backing (delete = vector dtor + operator
// delete, the compiler's tail-called teardown of the consumed source array).
void Agent::finishWingman(Array<AbyssEngine::String*> *consumedArray) {
    delete consumedArray;
}

RetStr Agent::getWingmanName(int idx) {
    Agent *self = this;
    void *src;
    if (idx == 1) {
        src = self->wingman1;
    } else if (idx == 0) {
        src = self;
    } else {
        src = self->wingman2;
    }
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)(src), false);
    return r;
}

void Agent::setSystemName(String12 src) {
    Agent *self = this;
    ((String *)((char *)self + 0x18))->assign((String *)&src);
}

// Agent::~Agent() — real C++ destructor so the demangled symbol contains "~Agent".

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gStringDtor __attribute__((visibility("hidden")));

__attribute__((minsize)) Agent::~Agent() noexcept(false)
{
    if (this->imageParts != 0)               // +0x88 imageParts
        ::operator delete[](this->imageParts);

    VObj *o = (VObj *)this->wingman1;        // +0xc  virtual; dtor at vt[0]+4
    this->imageParts = 0;
    if (o != 0) {
        (*(void (**)(void *))((char *)o->vt[0] + 4))(o);
        this->wingman1 = 0;
    }

    dtor_fn d = gStringDtor;
    d((char *)this + 0x78);
    d((char *)this + 0x6c);
    d((char *)this + 0x18);
    d(this);
}

// String temp lifecycle helpers (compiler emits the canary via -fstack-protector).

// Agent::setMissionString(String) — this in r0, source String* in r1.
void Agent::setMissionString(void *src) {
    Agent *self = this;
    String12 tmp;
    ((String *)(&tmp))->ctor_copy((String *)(src), false);
    ((String *)((char *)self + 0x6c))->assign((String *)&tmp);
    ((String *)(&tmp))->dtor();
}

// ---- simple field accessors (recovered) ----
// Single-instruction veneers that load/store a member; offsets confirmed from
// the constructor's initializer list above.

int Agent::getStation() {
    return this->field_0x44;
}

int Agent::getSystem() {
    return this->field_0x48;
}

int Agent::getRace() {
    return this->field_0x4c;
}

int Agent::getIndex() {
    return (int)this->type;
}

int Agent::getCosts() {
    return this->field_0x60;
}

void Agent::setCosts(int costs) {
    this->field_0x60 = costs;
}

int Agent::getOffer() {
    return this->field_0x5c;
}

void Agent::setOffer(int offer) {
    this->field_0x5c = offer;
}

int Agent::getEvent() {
    return this->eventCount;
}

void Agent::setEvent(int event) {
    this->eventCount = event;
}

int Agent::getSellItemIndex() {
    return this->field_0x34;
}

int Agent::getSellItemQuantity() {
    return this->field_0x38;
}

int Agent::getSellItemPrice() {
    return this->field_0x3c;
}

void Agent::setSellItemPrice(int price) {
    this->field_0x3c = price;
}

int Agent::getSellSystemIndex() {
    return this->field_0x64;
}

int Agent::getSellBlueprintIndex() {
    return this->field_0x68;
}

int Agent::getWingmanFriendsCount() {
    return this->wingmanCount;
}

extern "C" void *String_default_ctor(void *s);         // String::String() -> this

// Agent::Agent(int kind, String name, int p4, int p5, int p6, bool p7,
//              int p8, int p9, int p10, int p11)
Agent * Agent::ctor(unsigned kind, void *name, int p4, int p5, int p6, char p7, int p8, int p9, int p10, int p11) {
    Agent *self = this;
    void *s0 = String_default_ctor(self);
    String_default_ctor((char *)s0 + 0x18);
    String_default_ctor((char *)self + 0x6c);
    String_default_ctor((char *)self + 0x78);
    self->type = kind;
    ((String *)(self))->assign((String *)name);
    self->field_0x44 = p4;
    self->field_0x48 = p5;
    self->field_0x4c = p6;
    self->male = p7;
    self->eventCount = 0;
    self->field_0x30 = -1;
    self->field_0x5c = -1;
    self->field_0x64 = p8;
    if (p8 >= 0)
        self->field_0x5c = 4;
    self->field_0x68 = p9;
    if (p9 >= 0)
        self->field_0x5c = 3;
    self->offerAccepted = 0;
    self->field_0x24 = 0;
    self->wingman1 = 0;
    self->wingman2 = 0;
    self->wingmanCount = 0;
    self->field_0x34 = 0;
    self->field_0x38 = 0;
    self->field_0x3c = p11;
    self->field_0x60 = 0;
    self->imageParts = 0;
    self->mission = 0;
    self->wingmanNames = 0;
    self->field_0x28 = -1;
    self->field_0x2c = -1;
    self->category = kind >> 31;
    if (p10 >= 0)
        self->field_0x5c = 8;
    if (kind == 0x19)
        self->field_0x5c = 9;
    else if (kind == 0x1a)
        self->field_0x5c = 10;
    self->sellModIndex = p10;
    return self;
}
