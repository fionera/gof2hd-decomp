#include "class.h"

extern "C" int Agent_getRace(Agent *self);
extern "C" int Agent_isMale(Agent *self);
extern "C" int *Agent_getImageParts(Agent *self);
extern "C" int AERandom_nextInt(void *self, int max);

__attribute__((visibility("hidden"))) extern void **g_dw_random;
__attribute__((visibility("hidden"))) extern int g_dw_germanBriefingTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanSuccessTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanOtherTexts[];

extern "C" int DialogueWindow_pickGermanGenericTextBecauseWeSaved100EurosWithThat(
    DialogueWindow *self, int kind, Agent *agent)
{
    (void)self;
    int race = Agent_getRace(agent);
    int male;
    if (race < 10) {
        race = Agent_getRace(agent);
        male = Agent_isMale(agent);
        if (race == 3) {
            if (Agent_getImageParts(agent) == 0) {
                race = 3;
            } else {
                int *parts = Agent_getImageParts(agent);
                race = 0;
                if (*parts == 2) race = 3;
            }
        }
    } else {
        male = Agent_isMale(agent);
        race = 10;
    }

    int index;
    int *texts;
    void **random = g_dw_random;
    if (kind == 2) {
        index = AERandom_nextInt(*random, 2);
        texts = g_dw_germanSuccessTexts;
    } else {
        if (kind != 0) {
            index = AERandom_nextInt(*random, 2);
            texts = g_dw_germanOtherTexts;
        } else {
            index = AERandom_nextInt(*random, 2);
            texts = g_dw_germanBriefingTexts;
        }
    }

    int *picked = (int *)((char *)texts + index * 4 + 0x48);
    if (male != 0) {
        picked = (int *)((char *)texts + race * 8 + index * 4);
    }
    return *picked;
}
