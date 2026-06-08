#include "class.h"

struct GameText;

// String::String(String* out, const char* cstr, bool) -> 0x6ee18.
extern "C" void String_cstr_ctor(void *out, const char *s, bool);
// String::String(String* out, const String* src, bool) -> 0x6f028.
extern "C" void String_copy_ctor(void *out, void *src, bool);
// GameText::getText(GameText* table, int id) -> 0x72f70 returns a String*.
extern "C" void *GameText_getText(GameText *table, int id);
// Text table singleton (hidden -> single pc-relative deref).
__attribute__((visibility("hidden"))) extern GameText **g_gameText;

struct RetStr { uint32_t a, b, c; };

// Mission::getName(): campaign missions use a fixed literal name; freelance ones
// look up "<id+0x162>" in the global text table.
extern "C" RetStr Mission_getName(Mission *self) {
    RetStr r;
    if (self->f_64 != 0) {
        String_cstr_ctor(&r, "", false);
    } else {
        void *txt = GameText_getText(*g_gameText, self->f_c + 0x162);
        String_copy_ctor(&r, txt, false);
    }
    return r;
}
