#include "class.h"

// Stack String slots are 0xc bytes; we drive them via the engine's String entry points.
extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void String_default(void *s);                              // String::String()
void String_fromC(void *s, const char *text, bool copy);   // String::String(char*, bool)
void String_dtor(void *s);                                 // String::~String()
void String_concat(void *out, void *lhs, void *rhs);       // out = lhs + rhs (operator+)
void String_appendAssign(void *self, void *rhs);           // self += rhs (operator+=)

void *GameText_getText(int id);

int *Achievements_getMedals(void *ach);

int  BluePrint_getIndex(void *bp);

extern void *g_swh_achievements; // *(DAT_169f68): achievements root
extern void *g_swh_status;       // various *(DAT...): status/campaign singletons share this base
extern void *g_swh_gameText;     // GameText id source
}

// StatusWindow::getMedalHintText(String *out, int medalIndex)
extern "C" void StatusWindow_getMedalHintText(void *outStr, int medalIndex)
{
    void *volatile cookie = __stack_chk_guard;

    int *medals = Achievements_getMedals(*(void **)g_swh_achievements);
    int state = medals[medalIndex];

    String_default(outStr);

    // Only "in-progress" (state == 2) medals get a detailed checklist.
    if (state != 0) {
        char tmpA[0xc];  // intro/prefix line
        char tmpB[0xc];  // concatenation scratch

        // Helper macro-ish: append GameText(id) prefixed by 'lead'.
        // Each branch builds one header then iterates over a sub-goal list.
        if (medalIndex == 2 && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = GameText_getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            String_dtor(tmpB);
            String_dtor(tmpA);

            void *base = *(void **)g_swh_status;
            unsigned int *list = *(unsigned int **)((char *)base + 0x94);
            for (unsigned int i = 0; i < *list; i++) {
                if (*(char *)(list[1] + i) == 0) {
                    String_fromC(tmpA, "\n", false);
                    void *t = GameText_getText(0x594 + (int)i);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    String_dtor(tmpB);
                    String_dtor(tmpA);
                }
            }
        } else if (medalIndex == 3 && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = GameText_getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            String_dtor(tmpB);
            String_dtor(tmpA);

            void *base = *(void **)g_swh_status;
            unsigned int *list = *(unsigned int **)((char *)base + 0x98);
            for (unsigned int i = 0; i < *list; i++) {
                if (*(char *)(list[1] + i) == 0) {
                    String_fromC(tmpA, "\n", false);
                    void *t = GameText_getText(0x59f + (int)i);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    String_dtor(tmpB);
                    String_dtor(tmpA);
                }
            }
        } else if (medalIndex == 9 && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = GameText_getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            String_dtor(tmpB);
            String_dtor(tmpA);

            void *base = *(void **)g_swh_status;
            unsigned int *list = *(unsigned int **)((char *)base + 0xac);
            for (unsigned int i = 0; i < *list; i++) {
                if (*(char *)(list[1] + i) == 0) {
                    String_fromC(tmpA, "\n", false);
                    void *t = GameText_getText(0x57e + (int)i);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    String_dtor(tmpB);
                    String_dtor(tmpA);
                }
            }
        } else if (medalIndex == 0xd && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = GameText_getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            String_dtor(tmpB);
            String_dtor(tmpA);

            void *root = *(void **)g_swh_status;
            for (unsigned int i = 0; i < 0xd; i++) {
                void *bp = *(void **)(*(int *)(*(int *)((char *)root + 0x18) + 4) + i * 4);
                if (*(char *)((char *)bp + 8) == 0) {
                    String_fromC(tmpA, "\n", false);
                    int idx = BluePrint_getIndex(bp);
                    void *t = GameText_getText(idx + 0x4fa);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    String_dtor(tmpB);
                    String_dtor(tmpA);
                }
            }
        } else if (medalIndex == 0xe && state == 2) {
            String_fromC(tmpA, "\n", false);
            void *hdr = GameText_getText(0x114);
            String_concat(tmpB, tmpA, hdr);
            String_appendAssign(outStr, tmpB);
            String_dtor(tmpB);
            String_dtor(tmpA);

            void *root = *(void **)g_swh_status;
            for (unsigned int i = 0; i < 0xd; i++) {
                void *bp = *(void **)(*(int *)(*(int *)((char *)root + 0x18) + 4) + i * 4);
                if (*(int *)((char *)bp + 0xc) == 0) {
                    String_fromC(tmpA, "\n", false);
                    int idx = BluePrint_getIndex(bp);
                    void *t = GameText_getText(idx + 0x4fa);
                    String_concat(tmpB, tmpA, t);
                    String_appendAssign(outStr, tmpB);
                    String_dtor(tmpB);
                    String_dtor(tmpA);
                }
            }
        }
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
