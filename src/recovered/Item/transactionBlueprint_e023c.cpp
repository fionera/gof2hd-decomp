#include "class.h"

__attribute__((minsize))
int Item::transactionBlueprint(bool fabricate)
{
    if (fabricate) {
        int blueprints = blueprintAmount;
        if (blueprints > 0) {
            int currentAmount = amount;
            int currentPrice = price;
            blueprintAmount = blueprints - 1;
            amount = currentAmount + 1;
            return -currentPrice;
        }
    } else {
        int currentAmount = amount;
        if (currentAmount > 0) {
            int currentPrice = price;
            int blueprints = blueprintAmount;
            amount = currentAmount - 1;
            blueprintAmount = blueprints + 1;
            return currentPrice;
        }
    }

    return 0;
}
