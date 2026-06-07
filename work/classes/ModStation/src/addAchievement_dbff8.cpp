#include "class.h"

struct Achievements;
// Achievements singleton holder-of-holder: g -> P, P[0] -> Achievements object.
__attribute__((visibility("hidden"))) extern Achievements **g_ModStation_achievements;
// Three target-struct holders: each g -> P, *P -> the struct (stores at fixed offsets).
__attribute__((visibility("hidden"))) extern int *g_ModStation_ach_a;
__attribute__((visibility("hidden"))) extern int *g_ModStation_ach_b;
__attribute__((visibility("hidden"))) extern int *g_ModStation_ach_c;
// (single-deref pc-rel: g resolves to the int* directly; store at its offsets.)

extern "C" int Achievements_isEliteMedal(Achievements *a, int id);
extern "C" int Achievements_getValue(Achievements *a, int id, int which);

// ModStation::addAchievement(int, int)
extern "C" void ModStation_addAchievement(ModStation *self, int param_1, int param_2)
{
    Achievements **holder = g_ModStation_achievements;
    if (Achievements_isEliteMedal(holder[0], param_1) != 0)
        return;
    if ((unsigned)(param_2 - 1) < 2) {
        if (Achievements_getValue(holder[0], param_1, 3) != -1)
            *g_ModStation_ach_a = param_1 * 3;
        if (param_2 == 1 && Achievements_getValue(holder[0], param_1, 2) != -1)
            *(int *)((char *)g_ModStation_ach_b + 4) = param_1 * 3 + 1;
    }
    *(int *)((char *)g_ModStation_ach_c + 8) = (param_1 * 3 + 3) - param_2;
}
