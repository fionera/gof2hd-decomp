#include "class.h"

struct Status {
    int getCredits();
};

extern Status *status;

__attribute__((minsize)) bool Item::checkCredits()
{
    int credits = status->getCredits();
    return credits >= price;
}
