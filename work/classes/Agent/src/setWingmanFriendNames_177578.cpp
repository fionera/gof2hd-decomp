#include "class.h"

struct VObj { void (*vt[8])(void *); };

extern "C" void *Agent_operator_new(uint32_t sz);
extern "C" void Agent_operator_delete(void *p);
extern "C" void *Array_StringPtr_dtor(void *arr);
extern "C" void Array_StringPtr_ctor(void *arr);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void ArrayAdd_StringPtr(void *str, void *arr);
// final tail-called veneer (frees the incoming param array's backing)
extern "C" void Agent_finishWingman(void);

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
