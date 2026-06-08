#include "class.h"

// Hud::catchCargo(int amount, int cargoVal, bool a, bool docked, bool mission, bool p6,
//                 bool p7, int aggregateKey) — builds the cargo / docking / mission HUD event
// line and pushes it to the queue (aggregating repeated pickups of the same kind). r0=this.
extern "C" void *GameText_getText(void *gt, int id);
extern "C" void String_op_assign(void *dst, void *src);
extern "C" void String_ctor_copy(void *s, void *src, bool b);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_ctor_int(void *s, int v);
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" void String_dtor(void *s);
extern "C" void *operator_new(uint32_t);
extern "C" void ListItem_ctor1(void *li, void *str, int kind);
extern "C" void ListItem_ctor(void *li, void *str);
extern "C" int  Hud_sameHudEventAsBeforeAggregate(Hud *self, void *str);
extern "C" void Hud_addToEventQueue(Hud *self, void *item);
extern "C" void Hud_catchCargoFinish(void *self); // DAT_001ac654 tail thunk (mission case)
extern "C" void *Status_getMission();
extern "C" int  Mission_getType(void *m);
extern "C" void Status_replaceHash(void *out, void *tmpl, void *a, void *b, void *c);

__attribute__((visibility("hidden"))) extern void **g_Hud_ccGameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_ccTemplate; // *holder -> base format String
extern const char g_Hud_ccHashX[]  __attribute__((visibility("hidden")));
extern const char g_Hud_ccHashN[]  __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit[]   __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit2[]  __attribute__((visibility("hidden")));

extern "C" void Hud_catchCargo(Hud *self, int amount, int cargoVal,
                               bool a, bool docked, bool mission, bool p6, bool p7,
                               int aggregateKey)
{
    I(self, 0x1d0) = 0;
    UC(self, 0x235) = (unsigned char)docked;

    if (mission) {
        // mission cargo: two replaceHash passes producing the localized line
        void *gt = *g_Hud_ccGameText;
        void *base = GameText_getText(gt, 0x219);
        void *dst = B(self, 0x1f4);
        String_op_assign(dst, base);

        void *tmpl = *g_Hud_ccTemplate;
        char a40[12]; String_ctor_copy(a40, dst, false);
        Status_getMission();
        Mission_getType(Status_getMission());
        void *typeTxt = GameText_getText(gt, 0);
        char a4c[12]; String_ctor_copy(a4c, typeTxt, false);
        char a58[12]; String_ctor_cstr(a58, g_Hud_ccHashX, false);
        char out1[12]; Status_replaceHash(out1, tmpl, a40, a4c, a58);
        String_op_assign(dst, out1);
        String_dtor(out1); String_dtor(a58); String_dtor(a4c); String_dtor(a40);

        tmpl = *g_Hud_ccTemplate;
        char a64[12]; String_ctor_copy(a64, dst, false);
        char a70[12]; String_ctor_int(a70, 1);
        char a7c[12]; String_ctor_cstr(a7c, g_Hud_ccHashN, false);
        char out2[12]; Status_replaceHash(out2, tmpl, a64, a70, a7c);
        String_op_assign(dst, out2);
        String_dtor(out2); String_dtor(a7c); String_dtor(a70); String_dtor(a64);

        void *item = operator_new(0x48);
        void *str = operator_new(0xc);
        String_ctor_copy(str, dst, false);
        ListItem_ctor(item, str);
        *(int *)((char *)item + 0x2c) = cargoVal;
        Hud_addToEventQueue(self, item);
        return;
    }

    if (docked) {
        // docking confirmation line
        void *gt = *g_Hud_ccGameText;
        void *txt = GameText_getText(gt, 0x18a);
        String_op_assign(B(self, 0x1f4), txt);
        void *item = operator_new(0x48);
        void *str = operator_new(0xc);
        String_ctor_copy(str, B(self, 0x1f4), false);
        ListItem_ctor1(item, str, 1);
        Hud_catchCargoFinish(self);
        return;
    }

    if (!a) return; // amount==0 fast path (LAB_001734a6)

    void *gt = *g_Hud_ccGameText;

    // aggregate with previous "+N <unit>" event if allowed
    if (aggregateKey != 0 && self->f_26c != 0) {
        char a0[12]; String_ctor_int(a0, I(self, 0x52c));
        char ac[12]; String_ctor_cstr(ac, g_Hud_ccUnit, false);
        char a94[12]; String_concat(a94, a0, ac);
        char a88[12]; String_ctor_copy(a88, a94, false);
        void *unit = GameText_getText(gt, 0);
        char k34[12]; String_concat(k34, a88, unit);
        String_dtor(a88); String_dtor(a94); String_dtor(ac); String_dtor(a0);

        char b8[12]; String_ctor_copy(b8, k34, false);
        int idx = Hud_sameHudEventAsBeforeAggregate(self, b8);
        String_dtor(b8);
        if (idx >= 0) {
            I(self, 0x268) = 2000;
            int newAmt = I(self, 0x52c) + (a ? 1 : 0);
            I(self, 0x52c) = newAmt;
            char nAc[12]; String_ctor_int(nAc, newAmt);
            char nC4[12]; String_ctor_cstr(nC4, g_Hud_ccUnit2, false);
            char nA0[12]; String_concat(nA0, nAc, nC4);
            char n94[12]; String_ctor_copy(n94, nA0, false);
            void *u2 = GameText_getText(gt, 0);
            char n88[12]; String_concat(n88, n94, u2);
            String_op_assign(*(void **)(*(int *)(I(self->f_264, 4) + idx * 4) + 0x1c), n88);
            String_dtor(n88); String_dtor(n94); String_dtor(nA0); String_dtor(nC4); String_dtor(nAc);
            String_dtor(k34);
            return;
        }
        String_dtor(k34);
    }

    // fresh "+N <unit>" event
    I(self, 0x52c) = (a ? 1 : 0);
    char a0[12]; String_ctor_int(a0, I(self, 0x52c));
    char ac[12]; String_ctor_cstr(ac, g_Hud_ccUnit, false);
    char a94[12]; String_concat(a94, a0, ac);
    char a88[12]; String_ctor_copy(a88, a94, false);
    void *unit = GameText_getText(gt, 0);
    char k34[12]; String_concat(k34, a88, unit);
    String_op_assign(B(self, 0x1f4), k34);
    String_dtor(k34); String_dtor(a88); String_dtor(a94); String_dtor(ac); String_dtor(a0);

    void *item = operator_new(0x48);
    void *str = operator_new(0xc);
    String_ctor_copy(str, B(self, 0x1f4), false);
    ListItem_ctor(item, str);
    *(int *)((char *)item + 0x2c) = cargoVal;
    if (!p7 || p6) *(int *)((char *)item + 0x30) = 2;
    if (p7) *(unsigned char *)((char *)item + 0x24) = 1;
    Hud_addToEventQueue(self, item);
}
