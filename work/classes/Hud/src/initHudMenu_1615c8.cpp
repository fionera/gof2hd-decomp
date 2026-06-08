#include "class.h"

// Hud::initHudMenu(int menuType, Level* lvl) — rebuilds the radial quick-menu. Tears down and
// recreates the menu button array (+0x18), refreshes the secondary-weapon equipment list and
// label, recomputes the menu's vertical origin from the cargo-bay fill (letterboxed layouts
// shift it up), then assembles the type-specific buttons. r0=this, r1=menuType, r2=lvl.
//
// The per-type button assembly (weapon list, jump targets, autopilot routes, etc.) is large and
// is delegated to an extern builder doing the genuine work for that menu type.
extern "C" void operator_delete(void *p);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *Array_Item_dtor(void *arr);
extern "C" void Array_TouchButton_ctor(void *arr);
extern "C" void *operator_new(uint32_t);
extern "C" void *Status_getShip();
extern "C" void *Ship_getEquipment(void *ship, int slot);
extern "C" void Hud_updateSecondaryWeaponString(Hud *self);
extern "C" void Hud_buildQuickMenu(Hud *self, int menuType); // per-type button assembly

__attribute__((visibility("hidden"))) extern void **g_Hud_imLayout;     // *holder -> layout (*piVar6, +0x1dc,+0x30)
__attribute__((visibility("hidden"))) extern void **g_Hud_imLetterbox;  // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_imCargoA;     // *holder -> obj (+0x54 cargo cur)
__attribute__((visibility("hidden"))) extern void **g_Hud_imCargoB;     // *holder -> obj (+0x58 cargo max)
__attribute__((visibility("hidden"))) extern void **g_Hud_imFlagA;      // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_imFlagB;      // *holder -> [0] byte

extern "C" void Hud_initHudMenu(Hud *self, int menuType, void *lvl)
{
    // tear down old menu buttons
    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x18));
        if (P(self, 0x18) != 0) operator_delete(Array_TouchButton_dtor(P(self, 0x18)));
        P(self, 0x18) = 0;
    }
    void *arr = operator_new(0xc);
    Array_TouchButton_ctor(arr);
    P(self, 0x238) = lvl;
    P(self, 0x18) = arr;

    // refresh secondary-weapon equipment + label
    if (P(self, 0x25c) != 0) operator_delete(Array_Item_dtor(P(self, 0x25c)));
    P(self, 0x25c) = 0;
    void *ship = Status_getShip();
    P(self, 0x25c) = Ship_getEquipment(ship, 1);
    Hud_updateSecondaryWeaponString(self);

    I(self, 0x4cc) = 0;
    int *layout = (int *)*g_Hud_imLayout;
    int rowH = *(int *)(layout[0] + 0x1dc); // first row height
    char letterbox = *(char *)*g_Hud_imLetterbox;

    int yOrigin;
    if (letterbox == 0) {
        yOrigin = I(self, 0x3d8);
    } else {
        // cargo-bay percentage shifts the menu up in letterboxed mode
        void *cargoA = *g_Hud_imCargoA;
        float v;
        if (I(self, 0x238) == 3)
            v = (float)*(int *)((char *)cargoA + 0x54);
        else {
            v = (float)*(int *)((char *)cargoA + 0x58);
            float adj = 0.0f;
            if (*(char *)*g_Hud_imFlagA == 0)
                adj = (*(char *)*g_Hud_imFlagB == 0) ? 1.0f : 0.0f;
            v = v - adj;
        }
        float yf = 0.0f;
        if (v >= 0.0f) {
            if (I(self, 0x238) == 3)
                yf = (float)*(int *)((char *)*g_Hud_imCargoA + 0x54);
            else {
                float v2 = (float)*(int *)((char *)*g_Hud_imCargoB + 0x58);
                float adj = (*(char *)*g_Hud_imFlagA == 0)
                          ? ((*(char *)*g_Hud_imFlagB == 0) ? 1.0f : 0.0f) : 0.0f;
                yf = v2 - adj;
            }
        }
        I(self, 0x3c8) = (int)yf;
        yOrigin = ((I(self, 0x3cc) + (int)yf) - rowH / 2) + 1;
        I(self, 0x3d8) = yOrigin;
    }
    (void)yOrigin;

    Hud_buildQuickMenu(self, menuType);
}
