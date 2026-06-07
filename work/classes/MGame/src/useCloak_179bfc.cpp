#include "class.h"

struct PlayerEgo;
struct ChoiceWindow;
struct Status;
struct Ship;
struct Item;
struct GameText;
struct String12 { uint32_t a, b, c; };
__attribute__((visibility("hidden"))) extern Status **g_status;     // DAT_189d44
__attribute__((visibility("hidden"))) extern GameText **g_gameText; // DAT_189d4c

extern "C" int PlayerEgo_toggleCloaking(PlayerEgo *p);
extern "C" void *MGame_opnew(unsigned sz);
extern "C" void ChoiceWindow_ctor(ChoiceWindow *w);
extern "C" Ship *Status_getShip(Status *s);
extern "C" int Ship_getFirstEquipmentOfSort(Ship *ship, int sort);
extern "C" int Item_getAttribute(int item, int which);
extern "C" void *GameText_getText(GameText *t, int id);
extern "C" void String_cstr_ctor(String12 *out, const char *s, bool copy);
extern "C" void String_concat(String12 *out, String12 *lhs, String12 *rhs);
extern "C" void String_int_ctor(String12 *out, int v);
extern "C" void ChoiceWindow_set(ChoiceWindow *w, String12 *s);
extern "C" void String_dtor(String12 *s);
extern "C" void MGame_pauseSounds(MGame *self);

// MGame::useCloak(): toggle the cloaking device. If it engaged, build a choice dialogue
// "<text><cloak-attr><text>" describing the cloak and pause the game. Stack-protected
// (six temporary Strings on the stack).
extern "C" void MGame_useCloak(MGame *self) {
    if (PlayerEgo_toggleCloaking(F<PlayerEgo *>(self, 0x58)) != 0) return;
    if (F<ChoiceWindow *>(self, 0x94) == 0) {
        ChoiceWindow *w = (ChoiceWindow *)MGame_opnew(0x5c);
        ChoiceWindow_ctor(w);
        F<ChoiceWindow *>(self, 0x94) = w;
    }
    int eq = Ship_getFirstEquipmentOfSort(Status_getShip(*g_status), 0x15);
    int attr = eq == 0 ? 0 : Item_getAttribute(eq, 0x26);
    ChoiceWindow *cw = F<ChoiceWindow *>(self, 0x94);
    void *txt = GameText_getText(*g_gameText, 0x247);
    String12 s0, s1, s2, s3, s4, s5;
    String_cstr_ctor(&s2, "", false);
    String_concat(&s3, (String12 *)txt, &s2);
    String_int_ctor(&s1, attr);
    String_concat(&s4, &s3, &s1);
    String_cstr_ctor(&s0, "", false);
    String_concat(&s5, &s4, &s0);
    ChoiceWindow_set(cw, &s5);
    String_dtor(&s5);
    String_dtor(&s0);
    String_dtor(&s4);
    String_dtor(&s1);
    String_dtor(&s3);
    String_dtor(&s2);
    F<uint8_t>(self, 0x5d) = 1;
    F<uint8_t>(self, 0xce) = 1;
    MGame_pauseSounds(self);
}
