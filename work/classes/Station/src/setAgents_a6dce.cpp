#include "class.h"

struct Agent;
extern "C" void ArrayReleaseClasses_Agent(void *arr);
extern "C" void *Array_Agent_dtor(void *arr);
extern "C" void Station_operator_delete(void *p);

extern "C" void Station_setAgents(Station *self, void *agents)
{
    void *cur = self->f_30;
    if (cur != agents) {
        if (cur != 0) {
            ArrayReleaseClasses_Agent(cur);
            void *a2 = self->f_30;
            if (a2 != 0)
                Station_operator_delete(Array_Agent_dtor(a2));
        }
        self->f_30 = agents;
    }
}
