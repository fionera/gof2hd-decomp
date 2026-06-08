#include "class.h"

bool Objective::isSurvivalObjective()
{
    return F<int>(this, 0x0) == 3;
}
