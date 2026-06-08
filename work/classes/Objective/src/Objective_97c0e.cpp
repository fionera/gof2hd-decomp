#include "class.h"

Objective::Objective(int type, int value, int calcValue, Level *level)
{
    this->f_0 = type;
    this->f_4 = value;
    this->f_8 = calcValue;
    this->f_c = level;
    this->f_10 = 0;
    this->f_14 = 0;
    if (type == 0xd)
        this->f_18 = calcValue;
}
