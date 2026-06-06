#include "class.h"

struct Status {
    int getCredits();
};

extern Status *status;

__attribute__((minsize)) int Item::transaction(bool buy, int, bool useCredits)
{
    if (buy) {
        int currentStationAmount = stationAmount;
        if (currentStationAmount > 0) {
            int currentPrice;
            if (useCredits) {
                currentPrice = price;
            } else {
                int credits = status->getCredits();
                currentPrice = price;
                if (credits < currentPrice) {
                    return 0;
                }
                currentStationAmount = stationAmount;
            }
            amount += 1;
            stationAmount = currentStationAmount - 1;
            return -currentPrice;
        }
    } else if (amount > 0) {
        amount -= 1;
        stationAmount += 1;
        return price;
    }
    return 0;
}
