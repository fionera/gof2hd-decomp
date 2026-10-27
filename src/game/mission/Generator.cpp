#include "game/mission/Generator.h"
#include "engine/render/ImageFactory.h"
#include "game/ship/Ship.h"
#include "engine/core/AERandom.h"
#include "game/world/Galaxy.h"
#include "game/mission/Item.h"
#include "game/mission/Achievements.h"
#include "game/ship/Agent.h"
#include "game/core/Globals.h"
#include "game/world/Station.h"
#include "game/mission/Mission.h"
#include "game/world/SolarSystem.h"
#include "game/world/Standing.h"
#include "game/world/Wanted.h"
#include "game/mission/Status.h"


// Stations that can never be picked as a mission destination.
static const int kBlockedMissionStations[52] = {
    10, 22, 27, 29, 30, 48, 55, 56, 76, 79, 91, 98, 1, 33, 47, 86, 58, 65, 66, 74, 81, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 83, 85, 92, 109, 110, 11, 113, 121, 126, 131, 38, 90, 91, 93, 94, 82, 40, 15, 60, 95, 70, 80,
};


static const int kAgentOfferItems[9] = {200, 220, 208, 213, 216, 228, 229, 230, 231};
static const int kAgentOfferShips[6] = {55, 56, 57, 58, 59, 60};
static const int kAgentEnemyRaces[4] = {2, 3, 0, 1};


// Race each ship type is sold with (index = ship index).
static const int kShipRaces[64] = {
    3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0, 2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
    8, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 2, 1, 3, 3, 3, 3, 1, 1, 0, 8, 1, 1, 0, 3, 2, 0, 0, 8, 1, 3, 1,
};



// Items sold at the Kaamo Club station (106).
static const int kKaamoStationItems[10] = {101, 102, 103, 107, 108, 109, 114, 124, 0, 0};
// Items never auto-unlocked into a shop when their occurrence is 0.
static const int kNeverUnlockedItems[9] = {200, 220, 208, 213, 216, 228, 229, 230, 231};


// Chance (in %) that a randomly drawn loot item of each Item type is accepted.
static const int kLootTypeChance[5] = {10, 40, 2, 10, 100};


static const int
kaamoSpecialItems[9] = {200, 220, 208, 213, 216, 228, 229, 230, 231};


Generator::Generator() {
}
Generator::~Generator() {
}
void Generator::computerTradeGoods(Station *station) {
    if (station->getIndex() == 108) {
        return;
    }
    Array<Item *> *items = station->getItems();
    if (items == nullptr) {
        return;
    }
    for (uint32_t i = 0; i < items->size(); ++i) {
        int amount = (*items)[i]->getAmount();
        int take = Globals::rnd->nextInt(3);
        if (take < amount) {
            (*items)[i]->changeAmount(-take);
        }
    }
}
bool Generator::isKaamoSpecialItem(int item) {
    for (uint32_t i = 0;;) {
        if (i > 8) {
            return false;
        }
        int current = kaamoSpecialItems[i];
        ++i;
        if (current == item) {
            return true;
        }
    }
}
Array<Item *> *Generator::getItemBuyList(Station *station) {
    if (station->getIndex() == 78 && Globals::status->getCurrentCampaignMission() <= 6) {
        Array<Item *> *items = new Array<Item *>();
        ArraySetLength(3, *items);
        items->data()[0] = (*Globals::items)[0]->makeItem(1, 0);
        items->data()[1] = (*Globals::items)[22]->makeItem(1, 0);
        items->data()[2] = (*Globals::items)[55]->makeItem(1, 0);
        return items;
    }
    if (station->getIndex() == 108 || Globals::status->inSupernovaSystem()) {
        return nullptr;
    }

    int stationId = station->getIndex();
    Array<Item *> *result = new Array<Item *>();
    Array<Item *> *items = Globals::items;
    Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
    int stationTec = station->getTecLevel();
    int minTec = stationTec / 2;
    if (stationTec < 4) {
        minTec = 1;
    }
    if ((stationId | 2) == 107) {
        minTec = 0;
    }

    if (Globals::status->getCurrentCampaignMission() == 139 && station->getSystem() == 25) {
        ArrayAdd((*items)[190]->makeItem(), *result);
    }
    if (stationId == 126) {
        Item *item = (*items)[209]->makeItem();
        int amount = Globals::status->getCurrentCampaignMission() == 117 ? 1 : Globals::rnd->nextInt(10) + 1;
        ArrayAdd(item->makeItem(amount), *result);
    }

    Globals::rnd->reset();
    float priceFactor = 1.5f;
    if ((float) (Globals::status->getCurrentCampaignMission() + 25) / 45.0f < 1.5f) {
        priceFactor = (float) (Globals::status->getCurrentCampaignMission() + 25) / 45.0f;
    }

    for (int i = 0; i < items->size(); ++i) {
        Item *item = (*items)[i];
        int systemGood = item->getIndex() >= 132 && item->getIndex() < 154;
        bool stationExclusive = false;
        if (station->getIndex() == item->getAttribute(61)) {
            if (item->getIndex() >= 196 && item->getIndex() <= 196 &&
                Globals::status->getCurrentCampaignMission() < 142) {
                stationExclusive = false;
            } else if (item->getIndex() >= 198 && item->getIndex() <= 200 &&
                       Globals::status->getCurrentCampaignMission() < 142) {
                stationExclusive = false;
            } else {
                stationExclusive = true;
            }
        }
        if (stationId == 106) {
            unsigned k = 0;
            while (true) {
                if (k > 9) {
                    goto next;
                }
                // Both operands are evaluated by the original, including getIndex().
                if (systemGood | (kKaamoStationItems[k++] == item->getIndex())) {
                    break;
                }
            }
        }

        {
            int occurrence = item->getOccurence();
            if (item->getIndex() == 122 && Globals::energyCellsProbChange != 0) {
                occurrence = (int) ((float) occurrence +
                                    (float) (Globals::energyCellsProbChange * occurrence) * 0.01f);
            }
            if (item->getType() == 1 && Globals::secondaryWeaponsProbChange != 0) {
                occurrence = (int) ((float) occurrence +
                                    (float) (Globals::secondaryWeaponsProbChange * occurrence) * 0.01f);
            }
            int index = item->getIndex();
            int sort = item->getSort();
            int mission = Globals::status->getCurrentCampaignMission();
            if (occurrence == 0 && ((index >= 196 && Globals::options[55]) || (index <= 195 && Globals::options[53]))) {
                occurrence = 0;
                if ((item->getType() == 4) | (index == 85)) {
                    occurrence = 0;
                } else if (item->getIngredients() != nullptr) {
                    occurrence = 0;
                } else if (index == 181 && mission < 59) {
                    occurrence = 0;
                } else if (((sort == 34) | (sort == 33 || sort == 35)) == 1 && mission < 142) {
                    occurrence = 0;
                } else if (sort == 36 || ((mission < 142) & (sort == 43)) == 1) {
                    occurrence = 0;
                } else if ((((index == 209) | (index == 210) | (index == 217) | (index == 218)) |
                            ((mission < 94) & (index == 205))) == 1) {
                    occurrence = 0;
                } else {
                    bool blocked = false;
                    for (unsigned k = 0; k < 9; ++k) {
                        if (kNeverUnlockedItems[k] == index) {
                            blocked = true;
                            break;
                        }
                    }
                    if (!blocked && (sort != 29 || Globals::status->inBlackMarketSystem())) {
                        int roll = Globals::rnd->nextInt(30);
                        occurrence = (int) ((float) roll + ((float) item->getTecLevel() / -10.0f + 1.0f) * 30.0f);
                    }
                }
            }

            int tec = item->getTecLevel();
            if (!stationExclusive) {
                if (item->getIngredients() != 0) goto next;
                int slot = (int) (i & 0x7fffffff);
                if ((unsigned) (slot - 217) < 2 || slot == 164 || slot == 175) goto next;
                if (tec > station->getTecLevel() || occurrence == 0) goto next;
                if (item->getSinglePrice() == 0) goto next;
                if (item->getAttribute(60) == 1 && (*systems)[station->getSystem()]->getRace() != 1) goto next;
                if (stationId == 106) {
                    // Kaamo supplies goods from every system.
                } else if (systemGood != 1) {
                    // Other items are not tied to a system.
                } else if (item->getIndex() != station->getSystem() + 132) {
                    goto next;
                }
            }
            if (Globals::status->hardCoreMode() && (item->getSort() == 23 || item->getSort() == 24)) goto next;
            switch (stationId) {
                case 101:
                    if (!item->isWeapon()) goto next;
                    break;
                case 105:
                    if (!item->isWeapon() && item->getSort() != 28) goto next;
                    break;
                case 107:
                    if (item->getType() != 3) goto next;
                    break;
            }
            if (stationId == 106 && item->getType() != 4) goto next;

            int chance = (int) (priceFactor * (float) occurrence);
            if (!stationExclusive) {
                if (tec > stationTec) {
                    if (systemGood != 1) goto next;
                }
                if (Globals::rnd->nextInt(100) >= chance) goto next;
                if (((item->getIndex() != 122) & (tec < minTec)) && Globals::rnd->nextInt(100) > 60) goto next;
            }

            int minX = (*systems)[item->getMinPriceSystem()]->getX();
            int minY = (*systems)[item->getMinPriceSystem()]->getY();
            int distance = Globals::galaxy->invDistancePercent((*systems)[station->getSystem()]->getX(),
                                                               (*systems)[station->getSystem()]->getY(), minX, minY);
            int base = Globals::rnd->nextInt(15);
            int amount = base + 5;
            if (item->getType() == 4 || item->getType() == 1) {
                if (item->getIndex() == 109) {
                    amount = amount < 2 ? 1 : amount / 2;
                } else if (item->getType() == 4) {
                    if (distance > 50) {
                        float far = (float) (distance - 50) / 50.0f;
                        int scale = (int) (far * (Globals::status->hardCoreMode() ? 2.0f : 20.0f));
                        if (scale <= 1) {
                            scale = 1;
                        }
                        amount *= scale;
                    }
                    if (item->getIndex() == 110 && amount > Globals::rnd->nextInt(10) + 10) {
                        amount = Globals::rnd->nextInt(10) + 10;
                    }
                }
            } else {
                amount = amount < 5 ? 1 : amount / 5;
            }
            ArrayAdd(item->makeItem(amount), *result);
        }
    next:;
    }
    return result;
}
Array<Ship *> *Generator::getShipBuyList(Station *station) {
    if ((station->getSystem() == 15 && Globals::status->getCurrentCampaignMission() < 16) ||
        station->getIndex() == 101 || station->getIndex() == 108 || Globals::status->inSupernovaSystem()) {
        return nullptr;
    }

    if (station->getIndex() == 100 && Globals::status->dlc1Won()) {
        Array<Ship *> *list = new Array<Ship *>();
        for (int i = 0; i != 64; ++i) {
            Ship *ship = (*Globals::ships)[i];
            if (ship->hasJumpDriveIntegrated() && ship->getIndex() != 37) {
                ArrayAdd(ship->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(kShipRaces[i]);
                list->data()[list->size() - 1]->adjustPrice();
            }
        }
        return list;
    }

    if (station->getIndex() == 107) {
        Array<Ship *> *list = new Array<Ship *>();
        for (int i = 0; i != 50; ++i) {
            if (kShipRaces[i] == 8) {
                ArrayAdd((*Globals::ships)[i]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(8);
                list->data()[list->size() - 1]->adjustPrice();
            }
        }
        if (Globals::options[55] && Globals::status->wanted != nullptr && Globals::status->wanted->size() != 0) {
            if ((*Globals::status->wanted)[6]->isTerminated()) {
                ArrayAdd((*Globals::ships)[45]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(8);
                list->data()[list->size() - 1]->adjustPrice();
            }
            if ((*Globals::status->wanted)[12]->isTerminated()) {
                ArrayAdd((*Globals::ships)[46]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(8);
                list->data()[list->size() - 1]->adjustPrice();
            }
            if ((*Globals::status->wanted)[18]->isTerminated()) {
                ArrayAdd((*Globals::ships)[47]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(8);
                list->data()[list->size() - 1]->adjustPrice();
            }
            if ((*Globals::status->wanted)[24]->isTerminated()) {
                ArrayAdd((*Globals::ships)[48]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(8);
                list->data()[list->size() - 1]->adjustPrice();
            }
        }
        return list;
    }

    int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
    bool allGold = station->getIndex() == 10 && Globals::achievements->gotAllGoldMedals();
    int stationId = station->getIndex();
    int stationIndex = station->getIndex();
    int count;
    if (allGold) {
        count = 1;
    } else {
        count = Globals::rnd->nextInt(6);
        if (stationId == 41) {
            ++count;
        }
        if (count == 0) {
            return nullptr;
        }
    }

    Array<Ship *> *list = new Array<Ship *>();
    ArraySetLength(count, *list);
    int goldShip = allGold ? 8 : 10;
    for (int slot = 0; slot < count; ++slot) {
        bool first = slot == 0;
        int fixedShip = allGold | (first && stationId == 41);
        int forced = fixedShip || ((stationIndex == 78) && first);
        int fixed = fixedShip ? goldShip : 0;
        int ship = 0;
        bool unique = false;
        while (!unique) {
            ship = fixed;
            if (!forced) {
                ship = Globals::globals->getRandomEnemyFighter(race);
            }
            if (count >= 2 && Globals::rnd->nextInt(100) <= 21) {
                int otherRace = Globals::rnd->nextInt(5);
                ship = fixed;
                if (!forced) {
                    ship = Globals::globals->getRandomEnemyFighter(otherRace == 4 || otherRace == race ? 8 : otherRace);
                }
            }
            unique = true;
            for (int j = 0; j < count; ++j) {
                if (list->data()[j] != nullptr && list->data()[j]->getIndex() == ship) {
                    unique = false;
                    break;
                }
            }
        }
        list->data()[slot] = (*Globals::ships)[ship]->makeShip(-1);
        list->data()[slot]->setRace(kShipRaces[ship]);
        list->data()[slot]->adjustPrice();
    }

    bool vossExtras = false;
    bool terranExtras = false;
    switch (race) {
        case 0:
            if (Globals::rnd->nextInt(7) == 0) {
                ArrayAdd((*Globals::ships)[62]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(3);
                list->data()[list->size() - 1]->adjustPrice();
            }
            terranExtras = true;
            break;
        case 1:
            if (__builtin_expect(Globals::rnd->nextInt(5) == 0, 0)) { // lint: arm_builtin (cold path)
                ArrayAdd((*Globals::ships)[63]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(1);
                list->data()[list->size() - 1]->adjustPrice();
            }
            vossExtras = true;
            break;
        case 2:
            if (Globals::rnd->nextInt(8) == 0) {
                ArrayAdd((*Globals::ships)[61]->makeShip(-1), *list);
                list->data()[list->size() - 1]->setRace(1);
                list->data()[list->size() - 1]->adjustPrice();
            }
            break;
    }
    if (Globals::options[53] && (Globals::status->dlc1Won() & vossExtras)) {
        if (Globals::rnd->nextInt(2) == 0) {
            ArrayAdd((*Globals::ships)[39]->makeShip(-1), *list);
            list->data()[list->size() - 1]->setRace(1);
            list->data()[list->size() - 1]->adjustPrice();
        }
        if (Globals::rnd->nextInt(2) == 0) {
            ArrayAdd((*Globals::ships)[41]->makeShip(-1), *list);
            list->data()[list->size() - 1]->setRace(1);
            list->data()[list->size() - 1]->adjustPrice();
        }
    }
    if (Globals::options[55]) {
        if (vossExtras && Globals::rnd->nextInt(4) == 0) {
            ArrayAdd((*Globals::ships)[54]->makeShip(-1), *list);
            list->data()[list->size() - 1]->setRace(1);
            list->data()[list->size() - 1]->adjustPrice();
        }
        if (station->getIndex() == 120 && Globals::status->getCurrentCampaignMission() >= 159 &&
            (Globals::status->hardCoreMode() ||
             (!Globals::status->hardCoreMode() && Globals::achievements->gotAllSupernovaMedals()))) {
            ArrayAdd((*Globals::ships)[44]->makeShip(-1), *list);
            list->data()[list->size() - 1]->setRace(1);
            list->data()[list->size() - 1]->adjustPrice();
        }
        if (station->getIndex() == 120 && Globals::status->getCurrentCampaignMission() >= 159) {
            ArrayAdd((*Globals::ships)[49]->makeShip(-1), *list);
            list->data()[list->size() - 1]->setRace(1);
            list->data()[list->size() - 1]->adjustPrice();
        }
        if (terranExtras && Globals::rnd->nextInt(8) == 0) {
            ArrayAdd((*Globals::ships)[51]->makeShip(-1), *list);
            list->data()[list->size() - 1]->setRace(0);
            list->data()[list->size() - 1]->adjustPrice();
        }
    }
    if (station->getSystem() == 17 && Globals::rnd->nextInt(3) == 0) {
        ArrayAdd((*Globals::ships)[42]->makeShip(-1), *list);
        list->data()[list->size() - 1]->setRace(1);
        list->data()[list->size() - 1]->adjustPrice();
    }
    if (station->getSystem() == 17 && Globals::rnd->nextInt(3) == 0) {
        ArrayAdd((*Globals::ships)[43]->makeShip(-1), *list);
        list->data()[list->size() - 1]->setRace(2);
        list->data()[list->size() - 1]->adjustPrice();
    }
    if (station->getSystem() == 17 && Globals::rnd->nextInt(3) == 0) {
        ArrayAdd((*Globals::ships)[52]->makeShip(-1), *list);
        list->data()[list->size() - 1]->setRace(0);
        list->data()[list->size() - 1]->adjustPrice();
    }
    return list;
}
Array<int> *Generator::getLootList(int itemIndex, int amount) {
    Array<Item *> *items = Globals::items;
    if (itemIndex >= 0) {
        Array<int> *result = new Array<int>();
        ArrayAdd(itemIndex, *result);
        ArrayAdd(amount, *result);
        return result;
    }

    int pairCount = Globals::rnd->nextInt(3);
    if (Globals::rnd->nextInt(3) == 0) {
        return nullptr;
    }

    Array<int> *result = new Array<int>();
    ArraySetLength(pairCount == 0 ? 2u : (uint32_t) (pairCount << 1), *result);

    Array<Item *> *allItems = Globals::items;
    int type = 0;
    uint32_t out;
    for (out = 0; out < result->size(); out += 2) {
        bool found = false;
        int selected = 0;
        for (int tries = 0; tries < 100 && !found; tries++) {
            selected = Globals::rnd->nextInt(items->size());
            type = (*allItems)[selected]->getType();
            found = false;
            if ((*allItems)[selected]->getIngredients() == 0 &&
                Globals::rnd->nextInt(100) < kLootTypeChance[type] &&
                Globals::rnd->nextInt(100) < (*items)[selected]->getOccurence() &&
                (*allItems)[selected]->getSinglePrice() >= 1) {
                if ((uint32_t) (selected - 217) < 2 || selected == 164 || selected == 175) {
                    found = false;
                } else if (type != 4 && (*allItems)[selected]->getTecLevel() > 7) {
                    found = false;
                } else {
                    found = true;
                }
            }
        }

        int count;
        if (found) {
            result->data()[out] = selected;
            if (type == 4) {
                count = Globals::rnd->nextInt(9);
            } else {
                count = Globals::rnd->nextInt(3);
            }
        } else {
            result->data()[out] = Globals::rnd->nextInt(10) + 154;
            count = Globals::rnd->nextInt(9);
            type = 4;
        }
        result->data()[out | 1] = count + 1;
    }

    if (Globals::status->getShip()->hasJumpDrive() && !Globals::status->getShip()->hasCargo(122, 1) &&
        Globals::rnd->nextInt(100) < 10) {
        result->data()[0] = 122;
    }
    return result;
}
int Generator::generateStationIndex(Array<SolarSystem *> *systems, int station) {
    bool accepted = false;
    int selected = 0;
    while (!accepted) {
        selected = station;
        if (Globals::rnd->nextInt(100) >= 20) {
            if (Globals::rnd->nextInt(100) < 40) {
                Array<int> *stations = (Array<int> *) (*systems)[((SolarSystem *) (intptr_t) Globals::status->getSystem())
                                                                          ->getIndex()]->getStations();
                selected = (*stations)[Globals::rnd->nextInt(stations->size())];
            } else {
                selected = Globals::rnd->nextInt(135);
            }
        }
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 15) {
            Array<int> *stations = (Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations();
            selected = (*stations)[Globals::rnd->nextInt(
                    ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())->size())];
        }

        accepted = true;
        for (unsigned k = 0; k <= 51; ++k) {
            if (selected == kBlockedMissionStations[k]) {
                accepted = false;
                break;
            }
        }

        unsigned systemIndex = 0;
        while (true) {
            if (systemIndex >= systems->size()) {
                systemIndex = 0;
                break;
            }
            if ((*systems)[systemIndex]->stationIsInSystem(selected)) {
                break;
            }
            ++systemIndex;
        }
        if ((unsigned) (selected - 109) < 5) {
            accepted = false;
        }
        Array<bool> *visibilities = Globals::status->getSystemVisibilities();
        if (visibilities != nullptr && visibilities->size() > systemIndex) {
            accepted = (*visibilities)[systemIndex];
        }
        if ((*systems)[systemIndex]->getRoutes() == nullptr &&
            systemIndex != (unsigned) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex()) {
            accepted = false;
        }
    }
    return selected;
}
Array<Agent *> *Generator::createAgents(Station *station) {
    Array<Agent *> *result = nullptr;
    if (!Globals::status->inSupernovaSystem()) {
    Array<Agent *> *existing = (Array<Agent *> *) (intptr_t) Globals::status->getAgents();
    int mission = Globals::status->getCurrentCampaignMission();
    bool keep = mission > 16;
    if (!Globals::status->dlc1Won()) {
        keep = (mission > 16) & (station->getIndex() != 106);
    }

    uint32_t count = 0;
    for (uint32_t i = 0; i < existing->size(); ++i) {
        count += (existing->data()[i]->getStation() == station->getIndex()) & keep;
    }
    if (station->getIndex() != 108) {
        if ((int) (Globals::rnd->nextInt(2) + count + 3) <= 4) {
            count += 3 + Globals::rnd->nextInt(2);
        } else {
            count = 5;
        }
    }

    uint32_t out = 0;
    result = new Array<Agent *>();
    ArraySetLength(count, *result);
    for (uint32_t i = 0; i < existing->size(); ++i) {
        if (existing->data()[i]->getStation() != station->getIndex() || !keep) {
            continue;
        }
        result->data()[out++] = existing->data()[i];
        int campaign = Globals::status->getCurrentCampaignMission();
        if (existing->data()[i]->getOffer() == 9) {
            int index = campaign >= 142 ? Globals::rnd->nextInt(9) : Globals::rnd->nextInt(7) + 2;
            int itemId = kAgentOfferItems[index];
            existing->data()[i]->setSellItemData(itemId, (uint32_t) (index - 3) < 2 ? 10 : 1,
                                                 (*Globals::items)[itemId]->getSinglePrice() *
                                                         ((uint32_t) (index - 3) < 2 ? 10 : 1));
            existing->data()[i]->setOfferAccepted(false);
        } else if (existing->data()[i]->getOffer() == 10) {
            Array<int> *choices = new Array<int>();
            for (int j = 0; j != 6; ++j) {
                int shipId = kAgentOfferShips[j];
                if (!Globals::status->voidStation->hasShip(shipId) &&
                    Globals::status->getShip()->getIndex() != shipId) {
                    ArrayAdd(shipId, *choices);
                }
            }
            if (choices->size() != 0) {
                int shipId = (*choices)[Globals::rnd->nextInt(choices->size())];
                existing->data()[i]->setSellItemData(shipId, 1, (*Globals::ships)[shipId]->getPrice());
                existing->data()[i]->setOfferAccepted(false);
            } else {
                existing->data()[i]->setOfferAccepted(true);
            }
            delete choices;
        }
    }

    Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
    if (Globals::status->getCurrentCampaignMission() == 23 && station->getIndex() == 10) {
        Agent *agent = new Agent(-1, Globals::globals->getRandomName(0, true), station->getIndex(),
                                 ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex(), 0, true, -1,
                                 -1, -1, -1);
        agent->setOffer(2);
        agent->setSellItemData(68, 1, 100000);
        agent->setImageParts(Globals::imageFactory->createChar(true, 0));
        result->data()[out++] = agent;
    }

    bool hasWingman = false;
    for (uint32_t i = out; i < result->size(); ++i) {
        result->data()[i] = createAgent(station);
        if (result->data()[i]->getOffer() == 6) {
            if (hasWingman) {
                result->data()[i]->setOffer(1);
            }
            hasWingman = true;
        } else if (result->data()[i]->getOffer() == 0) {
            Agent *agent = result->data()[i];
            agent->setMission(createMission(agent, systems));
        }
    }

    if (Globals::rnd->nextInt(100) < 35) {
        Standing *standing = (Standing *) (intptr_t) Globals::status->getStanding();
        for (uint32_t r = 0; r < 4; ++r) {
            int race = kAgentEnemyRaces[r];
            if (!standing->isEnemy(race)) {
                continue;
            }
            for (uint32_t i = 0; i < result->size(); ++i) {
                if (result->data()[i]->isGenericAgent() && result->data()[i]->getOffer() != 7) {
                    result->data()[i] = nullptr;
                    result->data()[i] = new Agent(-1, Globals::globals->getRandomName(race, true),
                                                  station->getIndex(),
                                                  ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex(),
                                                  race, true, -1, -1, -1, -1);
                    result->data()[i]->setOffer(7);
                    result->data()[i]->setImageParts(Globals::imageFactory->createChar(true, race));
                    break;
                }
            }
        }
    }

    if ((Globals::status->getCurrentCampaignMission() == 23 && station->getIndex() == 10) ||
        Globals::rnd->nextInt(100) == 1) {
        for (uint32_t i = out; i < result->size(); ++i) {
            Agent *agent = result->data()[i];
            if (!agent->isStoryAgent() && agent->getOffer() == 0 && agent->getMission() != nullptr &&
                agent->getMission()->getReward() < 50000) {
                int reward = agent->getMission()->getReward() * 10;
                agent->getMission()->setReward(reward < 50000 ? reward : 50000);
                break;
            }
        }
    }
    }
    return result;
}
Agent *Generator::createAgent(Station *station) {
    int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
    if (Globals::rnd->nextInt(100) < 20) {
        race = Globals::rnd->nextInt(8);
    }

    bool valid = false;
    int offer = -1;
    while (!valid) {
        offer = Globals::rnd->nextInt(7);
        valid = false;
        if (race != 1 || offer != 6) {
            valid = true;
            if ((uint32_t) (offer - 3) <= 7) {
                valid = ((0x1cU >> (uint8_t) (offer - 3)) & 1) != 0;
            }
        }
    }
    if (Globals::rnd->nextInt(100) < 33 ||
        ((offer == 5 || offer == 6) && Globals::status->getCurrentCampaignMission() <= 15)) {
        offer = 0;
    }

    bool male = true;
    if (race == 0 && offer != 6) {
        male = Globals::rnd->nextInt(100) < 60;
    }

    Agent *agent = new Agent(-1, Globals::globals->getRandomName(race, male), station->getIndex(),
                             ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex(), race, male, -1, -1,
                             -1, -1);
    agent->setOffer(offer);
    agent->setImageParts(Globals::imageFactory->createChar(male, race));

    if (agent->getOffer() == 6) {
        int count = Globals::rnd->nextInt(3);
        Array<AbyssEngine::String *> *friendNames = new Array<AbyssEngine::String *>();
        ArraySetLength(count, *friendNames);
        for (int i = 0; i < count; ++i) {
            friendNames->data()[i] = new AbyssEngine::String(Globals::globals->getRandomName(agent->getRace(), true));
        }
        agent->setWingmanFriendNames(friendNames);
        agent->setCosts((Globals::rnd->nextInt(1300) + 700) * (count + 1));
        if (Globals::status->hardCoreMode()) {
            agent->setCosts(agent->getCosts() * 7);
        }
    } else if (agent->getOffer() == 2) {
        Array<Item *> *items = Globals::items;
        int selected;
        do {
            selected = Globals::rnd->nextInt(items->size());
        } while ((uint32_t) (selected - 217) < 2 || selected == 131 || selected == 164 || selected == 175 ||
                 (*items)[selected]->getIngredients() != nullptr || (*items)[selected]->getSinglePrice() == 0 ||
                 (*items)[selected]->getOccurence() == 0);
        Item *item = (*items)[selected];
        int amount = Globals::rnd->nextInt(15) + 5;
        if (item->getType() == 3 || item->getType() == 0 || item->getType() == 2) {
            amount = 1;
        }
        float factor = (float) (Globals::rnd->nextInt(120) + 40) / 100.0f;
        agent->setSellItemData(selected, amount,
                               amount * (int) (factor * (float) (*Globals::items)[selected]->getSinglePrice()));
    }
    return agent;
}
Mission *Generator::createMission(Agent *agent, Array<SolarSystem *> *systems) {
    int targetStation = generateStationIndex(systems, agent->getStation());
    if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 15) {
        targetStation = ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                                ->data()[0] +
                        Globals::rnd->nextInt(4);
    }

    uint32_t race = agent->getRace();
    int32_t type = 0;
    short guard = 999;
    bool selected = false;
    Array<bool> *used = Globals::status->field_50;
    for (; !selected; --guard) {
        selected = guard < 1;
        short pick = Globals::rnd->nextInt(15);
        type = pick;
        if (pick == 8) {
            continue;
        }
        if (pick == 10 && race > 3) {
            continue;
        }
        if (used != nullptr) {
            if (used->data()[pick]) {
                int sum = 0;
                uint32_t i;
                for (i = 0; i < used->size(); ++i) {
                    sum += used->data()[i];
                }
                if (sum == 14) {
                    for (uint32_t i = 0; i < used->size(); ++i) {
                        used->data()[i] = false;
                    }
                }
                continue;
            }
            used->data()[pick] = true;
        }
        if (pick == 12) {
            selected = true;
            type = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() != 25 ? pick : 12;
        } else {
            selected = true;
        }
    }

    if (Globals::status->getCurrentCampaignMission() < 16 || (type == 15 && !Globals::options[0x37])) {
        int choice = Globals::rnd->nextInt(5);
        if (choice == 0) {
            type = 11;
        } else if (choice == 1) {
            type = 0;
        } else if (choice == 2) {
            type = 7;
        } else if (choice == 3) {
            type = 4;
        } else if (choice == 4) {
            type = 12;
            targetStation = agent->getStation();
        }
    } else if (type == 12) {
        targetStation = agent->getStation();
    }

    if (agent->getOffer() == 5) {
        targetStation = agent->getStation();
        type = 8;
    } else {
        if ((uint32_t) type < 15 && ((1U << type) & 0x4801U) != 0) {
            while (targetStation == Globals::status->getStation()->getIndex()) {
                targetStation = generateStationIndex(systems, agent->getStation());
            }
        }
        if (race < 4 && type == 13) {
            int agentSystem = agent->getSystem();
            if (systems->data()[agentSystem]->getRoutes() == nullptr) {
                type = 4;
                if (Globals::rnd->nextInt(2) == 0) {
                    type = 1;
                }
            } else {
                bool found = false;
                while (!found) {
                    targetStation = generateStationIndex(systems, agent->getStation());
                    for (uint32_t i = 0; i < systems->size(); ++i) {
                        if (systems->data()[i]->stationIsInSystem(targetStation) &&
                            (uint32_t) systems->data()[i]->getRace() == race) {
                            found = true;
                            break;
                        }
                    }
                }
                type = 13;
            }
        }
    }

    AbyssEngine::String agentName = agent->getName();
    int difficulty;
    if (Globals::status->getCurrentCampaignMission() <= 15) {
        difficulty = Globals::rnd->nextInt(2);
    } else {
        difficulty = Globals::rnd->nextInt(9);
    }

    int itemId = 0;
    int amount = 0;
    if (type == 8) {
        Array<Item *> *items = Globals::items;
        do {
            itemId = Globals::rnd->nextInt(items->size() - 97) + 97;
        } while ((*items)[itemId]->getOccurence() == 0 || (*items)[itemId]->getSinglePrice() == 0 ||
                 (*items)[itemId]->getIngredients() != nullptr || itemId == 175 || itemId == 164 || itemId == 131 ||
                 itemId == 116 || itemId == 115 || itemId == 117 || itemId == 218 || itemId == 217);
        amount = Globals::rnd->nextInt(15) + 5;
        difficulty = (*items)[itemId]->getTecLevel();
    } else {
        ++difficulty;
        switch (type) {
            case 0:
                itemId = Globals::rnd->nextInt(7);
                amount = (int) ((float) difficulty / 10.0f * 95.0f) + 5;
                break;
            case 2:
                amount = (int) ((float) Globals::rnd->nextInt(4)) + 2;
                break;
            case 3:
            case 5:
                itemId = type == 3 ? 116 : 117;
                amount = (int) ((float) difficulty / 10.0f * 8.0f) + 2;
                break;
            case 11:
                amount = (int) ((float) difficulty / 10.0f * 18.0f) + 2;
                break;
            case 15:
                amount = (int) ((float) Globals::rnd->nextInt(90)) + 30;
                for (uint32_t i = 0; i < systems->size(); ++i) {
                    if (systems->data()[i]->stationIsInSystem(targetStation)) {
                        itemId = Globals::galaxy->getAsteroidProbabilities(Globals::status->getStation())
                                         [Globals::rnd->nextInt(3) * 2];
                        break;
                    }
                }
                break;
        }
    }

    if (difficulty >= 10) {
        difficulty = 10;
    }

    Galaxy *galaxy = Globals::galaxy;
    int fromSystem = Globals::status->getStation()->getSystem();
    SolarSystem *toSystem =
            systems->data()[((Station *) (intptr_t) galaxy->getStation(targetStation))->getSystem()];
    SolarSystem *fromSystemPtr = systems->data()[fromSystem];
    float reward = (galaxy->distance(fromSystemPtr, toSystem) / 1200.0f + 1.0f) *
                   (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500);
    if (type == 7) {
        reward *= 0.7f;
    } else if (type == 9) {
        reward *= 1.2f;
    } else if (type == 8) {
        reward = (float) ((*Globals::items)[itemId]->getMaxPrice() * amount) * 1.7f;
    } else if (type == 3) {
        reward *= 2.0f;
    } else if (type == 11) {
        reward *= 0.6f;
        reward += reward / 5.0f * (float) amount;
    } else if (type == 5) {
        reward *= 2.0f;
    }

    int level = Globals::status->getLevel();
    reward += (float) (level * level * level * 10);

    int bonus;
    if ((type | 4) == 12) {
        bonus = 0;
    } else {
        int rawBonus = (int) (reward * ((Standing *) (intptr_t) Globals::status->getStanding())
                                               ->getMissionBonus(agent->getRace()));
        int rem = rawBonus % 50;
        bonus = rawBonus + rem;
        if (bonus % 50 != 0) {
            bonus = rawBonus - rem;
        }
    }

    float rem = (float) ((int) reward % 50);
    if ((int) (reward + rem) % 50 == 0) {
        reward = reward + rem;
    } else {
        reward = reward - rem;
    }

    Mission *mission = new Mission(type, agentName, agent->getImageParts(), race, (int) reward, targetStation,
                                   difficulty);

    int costs = (int) (reward / 10.0f + (float) Globals::rnd->nextInt((int) reward / 10));
    if (type == 8) {
        costs = (int) ((float) costs * 0.5f);
    } else if (type == 6) {
        mission->setTargetName(Globals::globals->getRandomName(0, true));
    }
    int costRem = costs % 50;
    if ((costs + costRem) % 50 == 0) {
        costs += costRem;
    } else {
        costs -= costRem;
    }
    mission->setCosts(costs);
    mission->setProductionGoods(itemId, amount);
    mission->setBonus(bonus);

    for (uint32_t i = 0; i < systems->size(); ++i) {
        if (systems->data()[i]->stationIsInSystem(targetStation)) {
            mission->setTargetSystemName(systems->data()[i]->getName());
            break;
        }
    }
    return mission;
}