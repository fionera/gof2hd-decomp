#include "class.h"

Objective *Objective::addObjective(Objective *objective)
{
    Array<Objective *> *children = F<Array<Objective *> *>(this, 0x10);
    if (children == 0) {
        children = (Array<Objective *> *)operator new(sizeof(Array<Objective *>));
        Array_ObjectivePtr_ctor(children);
        F<Array<Objective *> *>(this, 0x10) = children;
    }
    ArrayAdd_Objective(objective, children);
    return this;
}
