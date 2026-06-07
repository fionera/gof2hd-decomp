#include "class.h"

struct Achievements;
struct ChoiceWindow;

__attribute__((visibility("hidden"))) extern int *g_ModStation_cm_credit1;  // *g -> credits delta src
__attribute__((visibility("hidden"))) extern int *g_ModStation_cm_credit2;  // *g -> credits delta src
__attribute__((visibility("hidden"))) extern void **g_ModStation_cm_lead;   // **g -> leaderboards obj
__attribute__((visibility("hidden"))) extern Achievements **g_ModStation_cm_ach; // **g -> Achievements
__attribute__((visibility("hidden"))) extern void **g_ModStation_cm_status; // *g -> Status obj (campaign mission)

extern "C" void Globals_reportLeaderboards(void *obj);
extern "C" int Status_getCurrentCampaignMission(void *s);
extern "C" void ModStation_addAchievement(ModStation *self, int a, int b);
extern "C" int *Achievements_getNewMedals(Achievements *a);
extern "C" void *cm_op_new(unsigned int sz);
extern "C" void Array_int_ctor(void *a);
extern "C" void ArraySetLength_int(unsigned int n, void *a);
extern "C" void *cm_op_new_arr(unsigned int sz);
extern "C" void ChoiceWindow_ctor(void *cw);
extern "C" void ChoiceWindow_setMedal(void *cw, int a, int b);
extern "C" int Status_hardCoreMode();
extern "C" void Status_changeCredits(int delta);
// Tail veneer at 0x1ac158: (ptr, ptr[0], ptr[1]).
extern "C" void ModStation_cm_tail(void *p, int a, int b);

// ModStation::checkMedals()
extern "C" void ModStation_checkMedals(ModStation *self)
{
    Globals_reportLeaderboards(*g_ModStation_cm_lead);

    if (C(self, 0x6a) != 0 || C(self, 0x63) != 0) {
        // continuation path
        int idx = I(self, 0xc0) + 1;
        I(self, 0xc0) = idx;
        if (idx >= I(self, 0xc4)) {
            UC(self, 0x6a) = 0;
            return;
        }
        int *medal = *(int **)(I(P(self, 0xbc), 4) + idx * 4);
        ChoiceWindow_setMedal(P(self, 0x88), medal[0], medal[1]);
        int delta = *g_ModStation_cm_credit2;
        if (Status_hardCoreMode() == 0)
            Status_changeCredits(delta);
        int *p = *(int **)(I(P(self, 0xbc), 4) + I(self, 0xc0) * 4);
        ModStation_cm_tail(p, p[0], p[1]);
        return;
    }

    int delta = *g_ModStation_cm_credit1;
    if (Status_getCurrentCampaignMission(*g_ModStation_cm_status) == 1)
        ModStation_addAchievement((ModStation *)1, 0, 1);
    P(self, 0xbc) = 0;
    int *medals = Achievements_getNewMedals(g_ModStation_cm_ach[0]);
    int count = 0;
    I(self, 0xc4) = 0;
    for (int i = 0; i != 0x2d; i++) {
        if (medals[i] >= 1) {
            count++;
            I(self, 0xc4) = count;
        }
    }
    if (count < 1)
        return;

    void *arr = cm_op_new(0xc);
    Array_int_ctor(arr);
    P(self, 0xbc) = arr;
    ArraySetLength_int(U(self, 0xc4), arr);
    int j = 0;
    I(self, 0xc4) = 0;
    for (int i = 0; i != 0x2d; i++) {
        if (medals[i] >= 1) {
            void *cell = cm_op_new_arr(8);
            *(void **)(I(P(self, 0xbc), 4) + j * 4) = cell;
            int data = I(P(self, 0xbc), 4);
            *(int *)*(int **)(data + j * 4) = i;
            *(int *)(*(int *)(data + I(self, 0xc4) * 4) + 4) = medals[i];
            j = I(self, 0xc4) + 1;
            I(self, 0xc4) = j;
        }
    }
    I(self, 0xc0) = 0;
    UC(self, 0x6a) = 1;
    void *cw = cm_op_new(0x5c);
    ChoiceWindow_ctor(cw);
    P(self, 0x88) = cw;
    int *medal = (int *)*(int **)(I(P(self, 0xbc), 4));
    ChoiceWindow_setMedal(cw, medal[0], medal[1]);
    if (Status_hardCoreMode() == 0)
        Status_changeCredits(delta);
    int *p = (int *)*(int **)(I(P(self, 0xbc), 4));
    ModStation_cm_tail(p, p[0], p[1]);
}
