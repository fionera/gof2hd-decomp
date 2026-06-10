#include "Agent.h"


extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void String_assign(void *dst, String12 src);
extern "C" void *Agent_operator_new(uint32_t sz);
extern "C" void Agent_operator_delete(void *p);
extern "C" void *Array_StringPtr_dtor(void *arr);
extern "C" void Array_StringPtr_ctor(void *arr);
extern "C" void ArrayAdd_StringPtr(void *str, void *arr);
extern "C" void Agent_finishWingman(void);
extern "C" __attribute__((nothrow)) void Agent_operator_delete_arr(void *p);
extern "C" void String_dtor(void *s);

// ---- getStationName_17770a.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getStationName(Agent *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x78, false);
    return r;
}

// ---- hasAcceptedOffer_1776f2.cpp ----
extern "C" uint8_t Agent_hasAcceptedOffer(Agent *self)
{
    return F<uint8_t>(self, 0x84);
}

// ---- isStoryAgent_17753a.cpp ----
extern "C" bool Agent_isStoryAgent(Agent *self)
{
    return F<int>(self, 0x58) == 0;
}

// ---- getWingmanNames_177670.cpp ----
extern "C" void *Agent_getWingmanNames(Agent *self)
{
    return F<void *>(self, 0x90);
}

// ---- getImageParts_177556.cpp ----
extern "C" int *Agent_getImageParts(Agent *self)
{
    return F<int *>(self, 0x88);
}

// ---- getMission_177566.cpp ----
struct Mission;

extern "C" Mission *Agent_getMission(Agent *self)
{
    return F<Mission *>(self, 0x8c);
}

// ---- setMission_177560.cpp ----
struct Mission;

extern "C" void Agent_setMission(Agent *self, Mission *mission)
{
    F<Mission *>(self, 0x8c) = mission;
}

// ---- getSellModIndex_177732.cpp ----
extern "C" int Agent_getSellModIndex(Agent *self)
{
    return F<int>(self, 0x94);
}

// ---- setStationName_177718.cpp ----
// AbyssEngine::String::operator=(String* dst, String src-by-value)

// Agent::setStationName(String) — this in r0, String by value in r1..r3.
// Tail-calls operator= on the field at +0x78.
extern "C" void Agent_setStationName(Agent *self, String12 src)
{
    String_assign((char *)self + 0x78, src);
}

// ---- getMissionString_177676.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getMissionString(Agent *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x6c, false);
    return r;
}

// ---- isMale_177524.cpp ----
extern "C" uint8_t Agent_isMale(Agent *self)
{
    return F<uint8_t>(self, 0x50);
}

// ---- getName_177504.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns String by value. The copy-ctor returns void, so the compiler cannot
// assume r0 survives the call and must keep a frame + restore the sret pointer.
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getName(Agent *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x00, false);
    return r;
}

// ---- getModPricePercentage_177738.cpp ----
// 4-entry price table loaded PC-relative (table contents in local rodata).
static const int kModPriceTable[4] = { 0, 0, 0, 0 };

extern "C" int Agent_getModPricePercentage(Agent *self)
{
    uint32_t i = F<uint32_t>(self, 0x94);
    if (i < 4)
        return kModPriceTable[i];
    return 0x28;
}

// ---- setOfferAccepted_1776f8.cpp ----
extern "C" void Agent_setOfferAccepted(Agent *self, bool v)
{
    F<uint8_t>(self, 0x84) = v;
}

// ---- nextEvent_177532.cpp ----
extern "C" void Agent_nextEvent(Agent *self)
{
    F<int>(self, 0x54) = F<int>(self, 0x54) + 1;
}

// ---- hasReward_1776fe.cpp ----
extern "C" uint8_t Agent_hasReward(Agent *self)
{
    return F<uint8_t>(self, 0x85);
}

// ---- setImageParts_177550.cpp ----
extern "C" void Agent_setImageParts(Agent *self, int *parts)
{
    F<int *>(self, 0x88) = parts;
}

// ---- isKnown_17756c.cpp ----
extern "C" bool Agent_isKnown(Agent *self)
{
    return F<int>(self, 0x54) > 0;
}

// ---- isGenericAgent_177544.cpp ----
extern "C" bool Agent_isGenericAgent(Agent *self)
{
    return F<int>(self, 0x58) == 1;
}

// ---- getSystemName_17771e.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getSystemName(Agent *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x18, false);
    return r;
}

// ---- giveRewardAtNextChat_177704.cpp ----
extern "C" void Agent_giveRewardAtNextChat(Agent *self, bool v)
{
    F<uint8_t>(self, 0x85) = v;
}

// ---- setSellItemData_1776ec.cpp ----
struct Triple { int a, b, c; };

// target: adds r0,#0x34; stmia r0!,{r1,r2,r3}; bx lr  (returns this+0x40)
extern "C" Triple *Agent_setSellItemData(Agent *self, int a, int b, int c)
{
    Triple *p = (Triple *)((char *)self + 0x34);
    p->a = a;
    p->b = b;
    p->c = c;
    return p + 1;
}

// ---- setWingmanFriendNames_177578.cpp ----
struct VObj { void (*vt[8])(void *); };

// final tail-called veneer (frees the incoming param array's backing)

// Agent::setWingmanFriendNames(Array<String*>*) — this in r0, param in r1.
extern "C" void Agent_setWingmanFriendNames(Agent *self, uint32_t *param)
{
    VObj *f0c = F<VObj *>(self, 0xc);
    if (f0c != 0)
        (*(void (**)(void *))((char *)f0c->vt[0] + 4))(f0c);
    F<VObj *>(self, 0xc) = 0;
    VObj *f10 = F<VObj *>(self, 0x10);
    if (f10 != 0)
        (*(void (**)(void *))((char *)f10->vt[0] + 4))(f10);
    F<VObj *>(self, 0x10) = 0;
    if (F<void *>(self, 0x90) != 0) {
        Agent_operator_delete(Array_StringPtr_dtor(F<void *>(self, 0x90)));
        F<void *>(self, 0x90) = 0;
    }
    void *na = Agent_operator_new(0xc);
    Array_StringPtr_ctor(na);
    F<void *>(self, 0x90) = na;
    void *ns = Agent_operator_new(0xc);
    String_copy_ctor(ns, self, false);
    ArrayAdd_StringPtr(ns, F<void *>(self, 0x90));
    F<int>(self, 0x14) = 0;
    if (param == 0)
        return;
    uint32_t n = param[0];
    if (n != 0) {
        void *w0 = *(void **)(param[1]);
        if (w0 != 0) {
            F<int>(self, 0x14) = 1;
            F<void *>(self, 0xc) = w0;
            ArrayAdd_StringPtr(w0, F<void *>(self, 0x90));
            n = param[0];
        }
        if (n >= 2) {
            void *w1 = ((void **)param[1])[1];
            if (w1 != 0) {
                F<void *>(self, 0x10) = w1;
                F<int>(self, 0x14) = F<int>(self, 0x14) + 1;
                ArrayAdd_StringPtr(w1, F<void *>(self, 0x90));
            }
        }
    }
    Array_StringPtr_dtor(param);
    Agent_finishWingman();
}

// ---- getWingmanName_177658.cpp ----
struct RetStr { uint32_t a, b, c; };

extern "C" RetStr Agent_getWingmanName(Agent *self, int idx)
{
    void *src;
    if (idx == 1) {
        src = F<void *>(self, 0xc);
    } else if (idx == 0) {
        src = self;
    } else {
        src = F<void *>(self, 0x10);
    }
    RetStr r;
    String_copy_ctor(&r, src, false);
    return r;
}

// ---- setSystemName_17772c.cpp ----
extern "C" void Agent_setSystemName(Agent *self, String12 src)
{
    String_assign((char *)self + 0x18, src);
}

// ---- _Agent_1774b8.cpp ----
// Agent::~Agent() — real C++ destructor so the demangled symbol contains "~Agent".

typedef unsigned int uint32_t;


typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gStringDtor __attribute__((visibility("hidden")));

struct VObj { dtor_fn vt[8]; };

struct Agent {
    void *name_str[3];   // 0x00 String (this+0x0)
    VObj *field_c;        // 0x0c  virtual; dtor at vt[1]
    unsigned char pad_10[0x88 - 0x10];
    void *imageParts;     // 0x88
    ~Agent() noexcept(false);
};

__attribute__((minsize)) Agent::~Agent() noexcept(false)
{
    if (this->imageParts != 0)
        Agent_operator_delete_arr(this->imageParts);

    VObj *o = this->field_c;
    this->imageParts = 0;
    if (o != 0) {
        (*(dtor_fn *)((char *)o->vt[0] + 4))(o);
        this->field_c = 0;
    }

    dtor_fn d = gStringDtor;
    d((char *)this + 0x78);
    d((char *)this + 0x6c);
    d((char *)this + 0x18);
    d(this);
}

// ---- setMissionString_177684.cpp ----
// String temp lifecycle helpers (compiler emits the canary via -fstack-protector).
extern "C" void String_assign(void *dst, void *src);   // operator=(String*, String*)

// Agent::setMissionString(String) — this in r0, source String* in r1.
extern "C" void Agent_setMissionString(Agent *self, void *src)
{
    char tmp[12];
    String_copy_ctor(tmp, src, false);
    String_assign((char *)self + 0x6c, tmp);
    String_dtor(tmp);
}

// ---- Agent_1773c4.cpp ----
extern "C" void *String_default_ctor(void *s);         // String::String() -> this
extern "C" void String_assign(void *dst, void *src);   // operator=(String*, String*)

// Agent::Agent(int kind, String name, int p4, int p5, int p6, bool p7,
//              int p8, int p9, int p10, int p11)
extern "C" Agent *Agent_ctor(Agent *self, unsigned kind, void *name,
                             int p4, int p5, int p6, char p7,
                             int p8, int p9, int p10, int p11)
{
    void *s0 = String_default_ctor(self);
    String_default_ctor((char *)s0 + 0x18);
    String_default_ctor((char *)self + 0x6c);
    String_default_ctor((char *)self + 0x78);
    F<unsigned>(self, 0x40) = kind;
    String_assign(self, name);
    F<int>(self, 0x44) = p4;
    F<int>(self, 0x48) = p5;
    F<int>(self, 0x4c) = p6;
    F<char>(self, 0x50) = p7;
    F<int>(self, 0x54) = 0;
    F<int>(self, 0x30) = -1;
    F<int>(self, 0x5c) = -1;
    F<int>(self, 0x64) = p8;
    if (p8 >= 0)
        F<int>(self, 0x5c) = 4;
    F<int>(self, 0x68) = p9;
    if (p9 >= 0)
        F<int>(self, 0x5c) = 3;
    F<uint16_t>(self, 0x84) = 0;
    F<uint16_t>(self, 0x24) = 0;
    F<int>(self, 0xc) = 0;
    F<int>(self, 0x10) = 0;
    F<int>(self, 0x14) = 0;
    F<int>(self, 0x34) = 0;
    F<int>(self, 0x38) = 0;
    F<int>(self, 0x3c) = p11;
    F<int>(self, 0x60) = 0;
    F<int>(self, 0x88) = 0;
    F<int>(self, 0x8c) = 0;
    F<int>(self, 0x90) = 0;
    F<int>(self, 0x28) = -1;
    F<int>(self, 0x2c) = -1;
    F<unsigned>(self, 0x58) = kind >> 31;
    if (p10 >= 0)
        F<int>(self, 0x5c) = 8;
    if (kind == 0x19)
        F<int>(self, 0x5c) = 9;
    else if (kind == 0x1a)
        F<int>(self, 0x5c) = 10;
    F<int>(self, 0x94) = p10;
    return self;
}
