#include "class.h"

// Global slot holding a pointer P; *P -> Q; *(Q+0x80) is the compared value.
extern int **const gAlienAttackSingleton __attribute__((visibility("hidden")));

extern "C" bool Station_isAttackedByAliens(Station *self)
{
    return self->f_c == *(int *)((char *)(*gAlienAttackSingleton) + 0x80);
}
