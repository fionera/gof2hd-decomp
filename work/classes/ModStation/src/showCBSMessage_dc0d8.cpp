#include "class.h"

// PC-relative cookie + the localized-string lookup tables this message uses.
__attribute__((visibility("hidden"))) extern int *g_cbs_stack;    // [DAT_000ec1a4]
__attribute__((visibility("hidden"))) extern int **g_cbs_textId;  // [DAT_000ec1b0]

extern "C" {
void  String_ctor_lit_cbs(String *s, const char *lit, int own);
void  String_dtor_cbs(String *s);
int   GameText_getText_cbs(int id);
void  ChoiceWindow_set_cbs(ChoiceWindow *cw, String *title, String *ok, int modal,
                          String *a, String *b, String *c, int d, int e);
}

// ModStation::showCBSMessage() — pops the "Cross-Buy Support" notice via the station's choice
// window and flags it as open.
extern "C" void ModStation_showCBSMessage(ModStation *self)
{
    // String is opaque (0xc bytes); use raw storage for the stack temporaries.
    char emptyA[12], emptyB[12], ok[12];
    String_ctor_lit_cbs((String *)emptyA, "", 0);
    String_ctor_lit_cbs((String *)emptyB, "", 0);

    ChoiceWindow *cw = (ChoiceWindow *)P(self, 0x70);
    String *title = (String *)GameText_getText_cbs(**g_cbs_textId);
    String_ctor_lit_cbs((String *)ok, "", 0);
    ChoiceWindow_set_cbs(cw, title, (String *)ok, 1, (String *)emptyA, (String *)emptyB,
                         (String *)emptyA, -1, -1);
    String_dtor_cbs((String *)ok);

    // mark both "CBS message open" flags.
    C(self, 0xc1) = 1;   // this[1].field_4C + 1
    C(self, 0x6b) = 1;   // m_nStarMapWindowOpen + 3

    String_dtor_cbs((String *)emptyB);
    String_dtor_cbs((String *)emptyA);
}
