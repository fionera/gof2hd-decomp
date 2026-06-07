#include "class.h"

struct String;

struct Status {
    unsigned int *getWingmen();
};

namespace AbyssEngine {
    int String_Compare(String *a, String *b);
}

extern "C" void Level_wingmanDied_all(Status *obj, int zero);
extern "C" void Level_wingmanDied_one(String *ship, unsigned int *list);

__attribute__((visibility("hidden"))) extern Status **g_wingmanDied;

void Level::wingmanDied(int name) {
    Status **slot = g_wingmanDied;
    unsigned int *list = (*slot)->getWingmen();
    if (list == 0) {
        return;
    }
    if (__builtin_expect(*list < 2, 0)) {
        return Level_wingmanDied_all(*slot, 0);
    }
    for (unsigned int i = 0; i < *list; i = i + 1) {
        String *w = ((String **)list[1])[i];
        if (AbyssEngine::String_Compare(w, (String *)name) == 0) {
            return Level_wingmanDied_one(((String **)list[1])[i], list);
        }
    }
}
