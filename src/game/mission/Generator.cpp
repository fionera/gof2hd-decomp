#include "gof2/game/mission/Generator.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Achievements.h"
// Several headers (Agent/SolarSystem/Station/Wanted) each define an identical
// global `struct RetStr` (the 12-byte by-value String aggregate) unconditionally.
// Station.h provides the canonical one (Mission.h delegates to it); rename the
// `RetStr` tag to a unique name for the other headers so their definitions don't
// collide. Generator never names RetStr directly and discards these getters'
// return values, so the distinct tags are harmless.
#define RetStr RetStr
#include "gof2/game/ship/Agent.h"
#undef RetStr
#include "gof2/game/core/Globals.h"
#include "gof2/game/world/Station.h"
#include "gof2/game/mission/Mission.h"
#define RetStr RetStr
#include "gof2/game/world/SolarSystem.h"
#undef RetStr
#include "gof2/game/world/Standing.h"
#define RetStr RetStr
#include "gof2/game/world/Wanted.h"
#undef RetStr
#include "gof2/game/mission/Status.h"




// ---- ctor/dtor ----
// Generator is a stateless helper: nothing to initialise or release.
Generator::Generator() {}
Generator::~Generator() {}

// ---- ABI shims --------------------------------------------------------------
// Generator is allocated on demand (operator new(1)) at several call sites that
// only have the raw object pointer, so these extern "C" entry points wrap the
// real special members and methods. Generator holds no state, so construction
// and destruction are trivial in-place operations.
extern "C" void Generator_ctor(Generator *g)
{
    new (g) Generator();
}

extern "C" void *Generator_dtor(Generator *g)
{
    g->~Generator();
    return g;
}

// "_oi" variants: identical work, reached from the inlined ModStation station
// setup. They forward straight to the corresponding Generator members. The
// Generator pointer arrives in r0 and the Station in r1 (mirroring the binary).
extern "C" void Generator_ctor_oi(Generator *g)
{
    new (g) Generator();
}

extern "C" void *Generator_dtor_oi(Generator *g)
{
    g->~Generator();
    return g;
}

extern "C" void Generator_computerTradeGoods_oi(Generator *g, Station *station)
{
    g->computerTradeGoods(station);
}

extern "C" int Generator_getShipBuyList_oi(Generator *g, Station *station)
{
    return (int)(intptr_t)g->getShipBuyList(station);
}

// ---- computerTradeGoods_953a0.cpp ----
static AbyssEngine::AERandom **volatile g_Generator_tradeRandom;

void Generator::computerTradeGoods(Station *station) {
    if ((Station_getIndex(station) != 0x6c) &&
        (station = (Station *)Station_getItems(station), station != 0)) {
        Array<Item *> *items = (Array<Item *> *)station;
        AbyssEngine::AERandom **random = g_Generator_tradeRandom;
        uint32_t i = 0;
        while (i < items->size()) {
            int amount = ((Item *)(items->data()[i]))->getAmount();
            int take = ((AbyssEngine::AERandom *)(*random))->nextInt();
            if (take < amount) {
                ((Item *)(items->data()[i]))->changeAmount(-take);
            }
            ++i;
        }
    }
}

// ---- generateStationIndex_9672c.cpp ----
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_stationRandom;
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_stationStatus;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_stationBlockList[0x34];

int Generator::generateStationIndex(Array<SolarSystem *> *systems, int station) {
    AbyssEngine::AERandom **randomPtr = g_Generator_stationRandom;
    Status **statusPtr = g_Generator_stationStatus;
    bool accepted = false;
    int selected = 0;

    do {
        if (accepted) {
            return selected;
        }

        int roll = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
        selected = station;
        if (roll >= 20) {
            roll = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            if (roll < 40) {
                int currentIndex = ((Status *)(*statusPtr))->getSystem();
                Array<int> *stations =
                    (Array<int> *)((SolarSystem *)(systems->data()[currentIndex]))->getStations();
                int pick = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
                selected = stations->data()[pick];
            } else {
                selected = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            }
        }

        int currentSystemIndex = ((Status *)(*statusPtr))->getSystem();
        if (currentSystemIndex == 0xf) {
            Array<int> *stations =
                (Array<int> *)((SolarSystem *)(systems->data()[((Status *)(*statusPtr))->getSystem()]))->getStations();
            Array<int> *stations2 =
                (Array<int> *)((SolarSystem *)(systems->data()[((Status *)(*statusPtr))->getSystem()]))->getStations();
            int pick = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            selected = stations->data()[pick];
        }

        uint32_t blocked = 0;
        while (blocked <= 0x33) {
            if (selected == g_Generator_stationBlockList[blocked]) {
                break;
            }
            ++blocked;
        }
        accepted = blocked > 0x33;

        uint32_t systemIndex = 0;
        while (systemIndex < systems->size()) {
            if (((SolarSystem *)(systems->data()[systemIndex]))->stationIsInSystem_int(selected) != 0) {
                break;
            }
            ++systemIndex;
        }
        if (systemIndex >= systems->size()) {
            systemIndex = 0;
        }

        if ((uint32_t)(selected - 0x6d) < 5) {
            accepted = false;
        }

        Array<bool> *visibilities =
            ((Status *)(*statusPtr))->getSystemVisibilities();
        if (visibilities != 0 && systemIndex < visibilities->size()) {
            accepted = (*visibilities)[systemIndex];
        }

        if (((SolarSystem *)(systems->data()[systemIndex]))->getRoutes() == 0) {
            int current = ((Status *)(*statusPtr))->getSystem();
            if (systemIndex != (uint32_t)current) {
                accepted = false;
            }
        }
        accepted = accepted & true;
    } while (true);
}

// ---- createAgents_96884.cpp ----
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_agentsStatus;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_agentsRandom;
extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_agentsItems;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_offerItemIds[12];
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_offerShipIds[6];
extern "C" __attribute__((visibility("hidden"))) Array<Ship *> **volatile
    g_Generator_agentsShips;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_galaxy;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_storyNames;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **volatile
    g_Generator_storyImages;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_enemyRaces[4];
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_enemyNames;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **volatile
    g_Generator_enemyImages;

Array<Agent *> *Generator::createAgents(Station *station) {
    Status **statusPtr = g_Generator_agentsStatus;
    AbyssEngine::AERandom **randomPtr = g_Generator_agentsRandom;
    Status *status = *statusPtr;

    Array<Agent *> *result = 0;
    if (((Status *)(status))->inSupernovaSystem() == 0) {
        Array<Agent *> *existing = (Array<Agent *> *)(long)((Status *)(status))->getAgents();
        int mission = ((Status *)(status))->getCurrentCampaignMission();
        bool keepExisting = mission > 0x10;
        if (((Status *)(status))->dlc1Won() == 0) {
            keepExisting = mission > 0x10 && Station_getIndex(station) != 0x6a;
        }

        uint32_t count = 0;
        for (uint32_t i = 0; i < existing->size(); ++i) {
            int agentStation = ((Agent *)(existing->data()[i]))->getStation();
            int stationIndex = Station_getIndex(station);
            if (agentStation == stationIndex && keepExisting) {
                ++count;
            }
        }

        if (Station_getIndex(station) != 0x6c) {
            int roll = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            if ((int)(roll + count + 3) < 5) {
                count += 3 + ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            } else {
                count = 5;
            }
        }

        result = (Array<Agent *> *)operator new(0xc);
        Array_agent_ptr_ctor(result);
        ArraySetLength_agent_ptr(count, result);

        uint32_t out = 0;
        for (uint32_t i = 0; i < existing->size(); ++i) {
            Agent *agent = existing->data()[i];
            if (((Agent *)(agent))->getStation() == Station_getIndex(station) &&
                keepExisting) {
                result->data()[out++] = agent;
                int offer = ((Agent *)(agent))->getOffer();
                if (offer == 9) {
                    int index;
                    if (((Status *)(status))->getCurrentCampaignMission() < 0x8e) {
                        index = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt() + 2;
                    } else {
                        index = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
                    }
                    int itemId = g_Generator_offerItemIds[index];
                    Item *item = (*g_Generator_agentsItems)->data()[itemId];
                    int amount = ((uint32_t)(index - 3) < 2) ? 10 : 1;
                    ((Agent *)(agent))->setSellItemData(itemId, amount, ((Item *)(item))->getSinglePrice() * amount);
                    ((Agent *)(agent))->setOfferAccepted(false);
                } else if (offer == 10) {
                    Array<int> *choices = (Array<int> *)operator new(0xc);
                    Array_int_ctor(choices);
                    for (int j = 0; j != 6; ++j) {
                        int shipId = g_Generator_offerShipIds[j];
                        if (((Station *)((Station *)status->field_14c))->hasShip(shipId) == 0) {
                            Ship *ship = ((Status *)(status))->getShip();
                            if (((Ship *)(ship))->getIndex() != shipId) {
                                ArrayAdd_int(shipId, choices);
                            }
                        }
                    }
                    if (choices->size() != 0) {
                        int pick =
                            ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
                        int shipId = choices->data()[pick];
                        ((Agent *)(agent))->setSellItemData(shipId, 1, ((Ship *)((*g_Generator_agentsShips)
                                              ->data()[shipId]))->getPrice());
                        ((Agent *)(agent))->setOfferAccepted(false);
                    } else {
                        ((Agent *)(agent))->setOfferAccepted(true);
                    }
                    operator delete(Array_int_dtor(choices));
                }
            }
        }

        Array<SolarSystem *> *systems =
            (Array<SolarSystem *> *)((Galaxy *)(*g_Generator_galaxy))->getSystems();
        if (((Status *)(status))->getCurrentCampaignMission() == 0x17 &&
            Station_getIndex(station) == 10) {
            Agent *agent = (Agent *)operator new(0x98);
            AbyssEngine::String name;
            Globals_getRandomName(&name, **(int **)(&g_Generator_storyNames),
                                  0, 1);
            int stationIndex = Station_getIndex(station);
            int systemIndex = ((Status *)(status))->getSystem();
            ((Agent *)(agent))->ctor(-1, &name, stationIndex, systemIndex, 0, 1, -1, -1, -1, -1);
            AbyssEngine_String_dtor(&name);
            ((Agent *)(agent))->setOffer(2);
            ((Agent *)(agent))->setSellItemData(0x44, 1, 0);
            ((Agent *)(agent))->setImageParts(((ImageFactory *)(*g_Generator_storyImages))->createChar(1, 0));
            result->data()[out++] = agent;
        }

        bool hasWingman = false;
        for (uint32_t i = out; i < result->size(); ++i) {
            result->data()[i] = createAgent(station);
            Agent *agent = result->data()[i];
            if (((Agent *)(agent))->getOffer() == 6) {
                if (hasWingman) {
                    hasWingman = true;
                    ((Agent *)(agent))->setOffer(1);
                } else {
                    hasWingman = true;
                }
            } else if (((Agent *)(agent))->getOffer() == 0) {
                ((Agent *)(agent))->setMission(createMission(agent, systems));
            }
        }

        if (((AbyssEngine::AERandom *)(*randomPtr))->nextInt() < 0x23) {
            void *standing = (void *)(long)((Status *)(status))->getStanding();
            for (uint32_t raceIndex = 0; raceIndex < 4; ++raceIndex) {
                int race = g_Generator_enemyRaces[raceIndex];
                if (((Standing *)(standing))->isEnemy(race) != 0) {
                    for (uint32_t i = 0; i < result->size(); ++i) {
                        Agent *agent = result->data()[i];
                        if (((Agent *)(agent))->isGenericAgent() &&
                            ((Agent *)(agent))->getOffer() != 7) {
                            result->data()[i] = 0;
                            agent = (Agent *)operator new(0x98);
                            AbyssEngine::String name;
                            Globals_getRandomName(&name, *g_Generator_enemyNames,
                                                  race, 1);
                            int stationIndex = Station_getIndex(station);
                            int systemIndex =
                                ((Status *)(status))->getSystem();
                            ((Agent *)(agent))->ctor(-1, &name, stationIndex, systemIndex, race, 1, -1, -1, -1, -1);
                            result->data()[i] = agent;
                            AbyssEngine_String_dtor(&name);
                            ((Agent *)(agent))->setOffer(7);
                            ((Agent *)(agent))->setImageParts(((ImageFactory *)(*g_Generator_enemyImages))->createChar(1, race));
                            break;
                        }
                    }
                }
            }
        }

        if ((((Status *)(status))->getCurrentCampaignMission() == 0x17 &&
             Station_getIndex(station) == 10) ||
            ((AbyssEngine::AERandom *)(*randomPtr))->nextInt() == 1) {
            for (; out < result->size(); ++out) {
                Agent *agent = result->data()[out];
                if (!((Agent *)(agent))->isStoryAgent() && ((Agent *)(agent))->getOffer() == 0 &&
                    ((Agent *)(agent))->getMission() != 0) {
                    Mission *m = ((Agent *)(agent))->getMission();
                    int reward = ((Mission *)(m))->getReward();
                    if (reward < 50000) {
                        int newReward = 50000;
                        if (reward * 10 < 50000) {
                            newReward = reward * 10;
                        }
                        ((Mission *)(m))->setReward(newReward);
                        break;
                    }
                }
            }
        }
    }
    return result;
}

// ---- createMission_971b8.cpp ----
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_missionStatus;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_missionRandom;
extern "C" __attribute__((visibility("hidden"))) uint8_t *volatile
    g_Generator_missionFlags;
extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_missionItems;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_missionGalaxy;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_targetNames;

Mission *Generator::createMission(Agent *agent,
                                  Array<SolarSystem *> *systems) {
    Status *status = *g_Generator_missionStatus;
    AbyssEngine::AERandom *random = *g_Generator_missionRandom;

    int agentStation = ((Agent *)(agent))->getStation();
    int targetStation = generateStationIndex(systems, agentStation);
    if (((Status *)(status))->getSystem() == 0xf) {
        Array<int> *stations = (Array<int> *)((SolarSystem *)(systems->data()[((Status *)(status))->getSystem()]))->getStations();
        targetStation = stations->data()[0] + ((AbyssEngine::AERandom *)(random))->nextInt();
    }

    uint32_t race = ((Agent *)(agent))->getRace();
    uint32_t type = 0;
    bool selected = false;
    int guard = 999;
    Array<uint8_t> *used =
        F<Array<uint8_t> *>(*g_Generator_missionStatus, 0x50);
    while (!selected) {
        selected = guard < 1;
        uint32_t pick = ((AbyssEngine::AERandom *)(random))->nextInt();
        --guard;
        type = pick;
        if (pick == 8 || (pick == 10 && race >= 4)) {
            continue;
        }
        if (used != 0) {
            uint8_t *data = used->data();
            if (data[pick] != 0) {
                uint32_t sum = 0;
                for (uint32_t i = 0; i < used->size(); ++i) {
                    sum += data[i];
                }
                if (sum == 0xe) {
                    for (uint32_t i = 0; i < used->size(); ++i) {
                        data[i] = 0;
                    }
                }
                continue;
            }
            data[pick] = 1;
        }
        if (pick == 0xc) {
            selected = true;
            if (((Status *)(status))->getSystem() != 0x19) {
                type = pick;
            } else {
                type = 0xc;
            }
        } else {
            selected = true;
        }
    }

    if (((Status *)(status))->getCurrentCampaignMission() < 0x10 ||
        (type == 0xf && g_Generator_missionFlags[0x37] == 0)) {
        switch (((AbyssEngine::AERandom *)(random))->nextInt()) {
        case 1:
            type = 0;
            break;
        case 2:
            type = 7;
            break;
        case 3:
            type = 4;
            break;
        case 4:
            type = 0xc;
            targetStation = ((Agent *)(agent))->getStation();
            break;
        default:
            type = 0xb;
            break;
        }
    } else if (type == 0xc) {
        targetStation = ((Agent *)(agent))->getStation();
    }

    if (((Agent *)(agent))->getOffer() == 5) {
        targetStation = ((Agent *)(agent))->getStation();
        type = 8;
    } else {
        if (type < 0xf && ((1U << (type & 0xff)) & 0x4801U) != 0) {
            while (targetStation == Station_getIndex(((Status *)(status))->getStation())) {
                targetStation = generateStationIndex(systems, ((Agent *)(agent))->getStation());
            }
        }
        if (race < 4 && type == 0xd) {
            SolarSystem *agentSystem = systems->data()[((Agent *)(agent))->getSystem()];
            if (((SolarSystem *)(agentSystem))->getRoutes() == 0) {
                type = ((AbyssEngine::AERandom *)(random))->nextInt() == 0 ? 1 : 4;
            } else {
                bool ok = false;
                while (!ok) {
                    targetStation =
                        generateStationIndex(systems, ((Agent *)(agent))->getStation());
                    for (uint32_t i = 0; i < systems->size(); ++i) {
                        if (((SolarSystem *)(systems->data()[i]))->stationIsInSystem_int(targetStation) &&
                            (uint32_t)((SolarSystem *)(systems->data()[i]))->getRace() ==
                                race) {
                            ok = true;
                            break;
                        }
                    }
                }
                type = 0xd;
            }
        }
    }

    AbyssEngine::String agentName;
    ((Agent *)(&agentName))->getName();
    int difficulty =
        ((AbyssEngine::AERandom *)(random))->nextInt() +
        1;

    int itemId = 0;
    int amount = 0;
    if (type == 8) {
        Array<Item *> *items = *g_Generator_missionItems;
        do {
            itemId = ((AbyssEngine::AERandom *)(random))->nextInt() + 0x61;
        } while (((Item *)(items->data()[itemId]))->getOccurence() == 0 ||
                 ((Item *)(items->data()[itemId]))->getSinglePrice() == 0 ||
                 ((Item *)(items->data()[itemId]))->getIngredients() != 0 ||
                 ((itemId - 0x61) & 0xfffffffe) == 0x78 ||
                 itemId == 0x75 || ((itemId - 0x61) & 0xfffffffe) == 0x12 ||
                 itemId == 0x83 || itemId == 0xa4 || itemId == 0xaf);
        amount = ((AbyssEngine::AERandom *)(random))->nextInt() + 5;
    } else {
        switch (type) {
        case 0:
            itemId = ((AbyssEngine::AERandom *)(random))->nextInt();
            amount = (int)(((float)difficulty / 10.0f) * 35.0f) + 5;
            break;
        case 2:
            amount = (int)((float)((AbyssEngine::AERandom *)(random))->nextInt()) + 2;
            break;
        case 3:
        case 5:
            itemId = type == 3 ? 0x74 : 0x75;
            amount = (int)(((float)difficulty / 10.0f) * 8.0f) + 2;
            break;
        case 0xb:
            amount = (int)(((float)difficulty / 10.0f) * 18.0f) + 2;
            break;
        case 0xf:
            amount = ((AbyssEngine::AERandom *)(random))->nextInt() + 30;
            for (uint32_t i = 0; i < systems->size(); ++i) {
                if (((SolarSystem *)(systems->data()[i]))->stationIsInSystem_int(targetStation)) {
                    int *prob = (int *)((Galaxy *)(*g_Generator_missionGalaxy))->getAsteroidProbabilities(((Status *)(status))->getStation());
                    itemId = prob[((AbyssEngine::AERandom *)(random))->nextInt() * 2];
                    break;
                }
            }
            break;
        default:
            amount = 0;
            itemId = 0;
            break;
        }
    }

    if (difficulty > 10) {
        difficulty = 10;
    }

    SolarSystem *from =
        systems->data()[Station_getSystem(((Status *)(status))->getStation())];
    SolarSystem *to = systems->data()[Station_getSystem((Station *)(long)((Galaxy *)(*g_Generator_missionGalaxy))->getStation(targetStation))];
    int distance = ((Galaxy *)(*g_Generator_missionGalaxy))->distance(to, from);
    float reward = ((float)distance / 1000.0f + 1.0f) *
                   (float)((int)(((float)difficulty / 10.0f) * 1400.0f) +
                           1500);
    if (type == 9) {
        reward *= 0.25f;
    } else if (type == 7) {
        reward *= 0.5f;
    } else if (type == 8) {
        reward = (float)(amount *
                         ((Item *)((*g_Generator_missionItems)
                                                 ->data()[itemId]))->getSinglePrice()) *
                 1.2f;
    } else if (type == 3 || type == 5) {
        reward += reward;
    } else if (type == 0xb) {
        reward = reward * 0.4f + ((reward * 0.4f) / 5.0f) * (float)amount;
    }

    int level = ((Status *)(status))->getLevel();
    reward += (float)(level * level * level * 10);

    int bonus = 0;
    if ((type | 4) != 0xc) {
        int rawBonus =
            (int)(reward * (float)((Standing *)(((Status *)(status))->getStanding()))->getMissionBonus(((Agent *)(agent))->getRace()));
        bonus = rawBonus + rawBonus % 50;
        if (bonus % 50 != 0) {
            bonus = rawBonus - rawBonus % 50;
        }
    }

    int rewardInt = (int)reward;
    int remainder = rewardInt % 50;
    rewardInt = ((rewardInt + remainder) % 50 == 0) ? rewardInt + remainder
                                                    : rewardInt - remainder;

    Mission *mission = (Mission *)operator new(0x78);
    AbyssEngine::String missionName;
    Mission_ctor_full(mission, type, &missionName, ((Agent *)(agent))->getImageParts(),
                      race, rewardInt, targetStation, difficulty);

    int costs = rewardInt / 10 + ((AbyssEngine::AERandom *)(random))->nextInt();
    if (type == 8) {
        costs = (int)((float)costs * 0.5f);
    } else if (type == 6) {
        AbyssEngine::String targetName;
        Globals_getRandomName(&targetName, *g_Generator_targetNames, 0, 1);
        // String passed by value via its 12-byte aggregate ABI representation.
        ((Mission *)(mission))->setTargetName(*(const AbyssEngine::String12 *)&targetName);
        AbyssEngine_String_dtor(&targetName);
    }
    int costRem = costs % 50;
    int roundedCosts = costs + costRem;
    if (roundedCosts % 50 != 0) {
        roundedCosts = costs - costRem;
    }
    ((Mission *)(mission))->setCosts(roundedCosts);
    ((Mission *)(mission))->setProductionGoods(itemId, amount);
    ((Mission *)(mission))->setBonus(bonus);

    for (uint32_t i = 0; i < systems->size(); ++i) {
        if (((SolarSystem *)(systems->data()[i]))->stationIsInSystem_int(targetStation)) {
            AbyssEngine::String systemName;
            ((SolarSystem *)(&systemName))->getName();
            ((Mission *)(mission))->setTargetSystemName(*(const AbyssEngine::String12 *)&systemName);
            AbyssEngine_String_dtor(&systemName);
            break;
        }
    }

    AbyssEngine_String_dtor(&missionName);
    AbyssEngine_String_dtor(&agentName);
    return mission;
}

// ---- createAgent_96e8c.cpp ----
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_agentStatus;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_agentRandom;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_nameSource;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **volatile
    g_Generator_imageFactory;
extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_agentItems;

Agent *Generator::createAgent(Station *station) {
    Status **statusPtr = g_Generator_agentStatus;
    AbyssEngine::AERandom **randomPtr = g_Generator_agentRandom;
    Status *status = *statusPtr;

    int race = ((SolarSystem *)((SolarSystem *)(long)((Status *)(status))->getSystem()))->getRace();
    if (((AbyssEngine::AERandom *)(*randomPtr))->nextInt() < 20) {
        race = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
    }

    int offer = -1;
    bool valid = false;
    do {
        offer = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
        if (race == 1) {
            valid = false;
        }
        if (race != 1 || offer != 6) {
            valid = true;
            if ((uint32_t)(offer - 3) < 8) {
                valid = ((0x1cU >> (uint8_t)(offer - 3)) & 1) != 0;
            }
        }
    } while (!valid);

    if (((AbyssEngine::AERandom *)(*randomPtr))->nextInt() < 0x21) {
        offer = 0;
    } else if ((uint32_t)(offer - 5) < 2 &&
               ((Status *)(status))->getCurrentCampaignMission() < 0x10) {
        offer = 0;
    }

    int male = 1;
    if (race == 0 && offer != 6) {
        male = 0;
        if (((AbyssEngine::AERandom *)(*randomPtr))->nextInt() < 0x3c) {
            male = 1;
        }
    }

    Agent *agent = (Agent *)operator new(0x98);
    int *names = g_Generator_nameSource;
    AbyssEngine::String name;
    Globals_getRandomName(&name, *names, race, male);
    int stationIndex = Station_getIndex(station);
    int systemIndex = ((Status *)(status))->getSystem();
    ((Agent *)(agent))->ctor(-1, &name, stationIndex, systemIndex, race, male, -1, -1, -1, -1);
    AbyssEngine_String_dtor(&name);
    ((Agent *)(agent))->setOffer(offer);
    ImageFactory **factoryPtr = g_Generator_imageFactory;
    ((Agent *)(agent))->setImageParts(((ImageFactory *)(*factoryPtr))->createChar(male, race));

    if (((Agent *)(agent))->getOffer() == 6) {
        uint32_t count = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
        Array<AbyssEngine::String *> *friendNames =
            (Array<AbyssEngine::String *> *)operator new(0xc);
        Array_string_ptr_ctor(friendNames);
        ArraySetLength_string_ptr(count, friendNames);
        for (int i = 0; i < (int)count; ++i) {
            AbyssEngine::String *friendName =
                (AbyssEngine::String *)operator new(0xc);
            Globals_getRandomName(friendName, *names, ((Agent *)(agent))->getRace(), 1);
            friendNames->data()[i] = friendName;
        }
        // setWingmanFriendNames takes an opaque Array<String*>* (declared as uint32_t*).
        ((Agent *)(agent))->setWingmanFriendNames((uint32_t *)friendNames);
        int costRoll = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
        ((Agent *)(agent))->setCosts((costRoll + 700) * (count + 1));
        if (((Status *)(status))->hardCoreMode() != 0) {
            ((Agent *)(agent))->setCosts(((Agent *)(agent))->getCosts() * 7);
        }
    } else if (((Agent *)(agent))->getOffer() == 2) {
        Array<Item *> **itemsPtr = g_Generator_agentItems;
        Array<Item *> *items = *itemsPtr;
        int selected;
        do {
            do {
                selected = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            } while ((uint32_t)(selected - 0xd9) < 2);
        } while (selected == 0x83 || selected == 0xa4 || selected == 0xaf ||
                 ((Item *)(items->data()[selected]))->getIngredients() != 0 ||
                 ((Item *)(items->data()[selected]))->getSinglePrice() == 0 ||
                 ((Item *)(items->data()[selected]))->getOccurence() == 0);

        Item *item = items->data()[selected];
        int amount = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
        int type = ((Item *)(item))->getType();
        if (type == 3 || ((Item *)(item))->getType() == 0 || ((Item *)(item))->getType() == 2) {
            amount = 1;
        } else {
            amount += 5;
        }
        float factor =
            (float)(((AbyssEngine::AERandom *)(*randomPtr))->nextInt() + 0x28) / 100.0f;
        int price = ((Item *)((*itemsPtr)->data()[selected]))->getSinglePrice();
        ((Agent *)(agent))->setSellItemData(selected, amount, amount * (int)(factor * (float)price));
    }

    return agent;
}

// ---- getShipBuyList_95bcc.cpp ----
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_shipStatus;
extern "C" __attribute__((visibility("hidden"))) Array<Ship *> **volatile
    g_Generator_allShips;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_shipRandom;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_shipRaces[0x40];
extern "C" __attribute__((visibility("hidden"))) uint8_t *volatile
    g_Generator_shipFlags;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_achievements;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_globals;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_wantedList;

static void addShip(Array<Ship *> *list, Ship *base, int race) {
    Ship *ship = ((Ship *)(base))->makeShip(-1);
    ArrayAdd_ship_ptr(ship, list);
    Ship *added = list->data()[list->size() - 1];
    ((Ship *)(added))->setRace(race);
    ((Ship *)(added))->adjustPrice();
}

Array<Ship *> *Generator::getShipBuyList(Station *station) {
    Status *status = *g_Generator_shipStatus;
    if ((Station_getSystem(station) == 0xf &&
         ((Status *)(status))->getCurrentCampaignMission() < 0x10) ||
        Station_getIndex(station) == 0x65 ||
        Station_getIndex(station) == 0x6c ||
        ((Status *)(status))->inSupernovaSystem() != 0) {
        return 0;
    }

    Array<Ship *> *allShips = *g_Generator_allShips;

    if (Station_getIndex(station) == 100 && ((Status *)(status))->dlc1Won() != 0) {
        Array<Ship *> *result = (Array<Ship *> *)operator new(0xc);
        Array_ship_ptr_ctor(result);
        for (int i = 0; i != 0x40; ++i) {
            Ship *base = allShips->data()[i];
            if (((Ship *)(base))->hasJumpDriveIntegrated() &&
                ((Ship *)(base))->getIndex() != 0x25) {
                addShip(result, base, g_Generator_shipRaces[i]);
            }
        }
        return result;
    }

    if (Station_getIndex(station) == 0x6b) {
        Array<Ship *> *result = (Array<Ship *> *)operator new(0xc);
        Array_ship_ptr_ctor(result);
        for (int i = 0; i != 0x32; ++i) {
            if (g_Generator_shipRaces[i] == 8) {
                addShip(result, allShips->data()[i], 8);
            }
        }
        if (g_Generator_shipFlags[0x37] != 0) {
            Array<void *> *wanted = *(Array<void *> **)*g_Generator_wantedList;
            if (wanted != 0 && wanted->size() != 0) {
                const int offsets[4] = {0x18, 0x30, 0x48, 0x60};
                const int ships[4] = {0x2d, 0x2e, 0x2f, 0x30};
                for (int i = 0; i != 4; ++i) {
                    void *w = *(void **)((char *)wanted->data() + offsets[i]);
                    if (((Wanted *)(w))->isTerminated()) {
                        addShip(result, allShips->data()[ships[i]], 8);
                    }
                }
            }
        }
        return result;
    }

    int race = ((SolarSystem *)((SolarSystem *)(long)((Status *)(status))->getSystem()))->getRace();
    bool gold = Station_getIndex(station) == 10 &&
                ((Achievements *)(*g_Generator_achievements))->gotAllGoldMedals();
    int stationIndex = Station_getIndex(station);
    int count;
    if (gold) {
        count = 1;
    } else {
        count = ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt();
        if (stationIndex == 0x29) {
            ++count;
        }
        if (count == 0) {
            return 0;
        }
    }

    Array<Ship *> *result = (Array<Ship *> *)operator new(0xc);
    Array_ship_ptr_ctor(result);
    ArraySetLength_ship_ptr(count, result);

    int forcedShip = gold ? 8 : 10;
    for (int i = 0; i < count; ++i) {
        bool first = i == 0;
        bool forced = gold || (first && stationIndex == 0x29);
        int shipIndex = forced ? forcedShip : 0;
        bool unique = false;
        while (!unique) {
            if (!forced && (stationIndex != 0x4e || !first)) {
                shipIndex =
                    ((Globals *)(*g_Generator_globals))->getRandomEnemyFighter(race);
            }
            if (count > 1 &&
                ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() < 0x16) {
                int roll = ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt();
                if (!forced && (stationIndex != 0x4e || !first)) {
                    int enemyRace = roll;
                    if (enemyRace == race || enemyRace == 4) {
                        enemyRace = 8;
                    }
                    shipIndex = ((Globals *)(*g_Generator_globals))->getRandomEnemyFighter(enemyRace);
                }
            }
            unique = true;
            for (int j = 0; j < count; ++j) {
                if (result->data()[j] != 0 &&
                    ((Ship *)(result->data()[j]))->getIndex() == shipIndex) {
                    unique = false;
                    break;
                }
            }
        }
        result->data()[i] = ((Ship *)(allShips->data()[shipIndex]))->makeShip(-1);
        ((Ship *)(result->data()[i]))->setRace(g_Generator_shipRaces[shipIndex]);
        ((Ship *)(result->data()[i]))->adjustPrice();
    }

    bool terranBonus = false;
    int raceFlag = 0;
    if (race == 0) {
        if (((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
            addShip(result, allShips->data()[0x3e], 3);
        }
        terranBonus = true;
    } else if (race == 1) {
        raceFlag = 1;
        if (((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
            addShip(result, allShips->data()[0x3f], 1);
        }
    } else if (race == 2 &&
               ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
        addShip(result, allShips->data()[0x3d], 1);
    }

    if (g_Generator_shipFlags[0x35] && ((Status *)(status))->dlc1Won() && raceFlag) {
        if (((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
            addShip(result, allShips->data()[0x27], 1);
        }
        if (((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
            addShip(result, allShips->data()[0x29], 1);
        }
    }

    if (g_Generator_shipFlags[0x37] != 0) {
        if (raceFlag &&
            ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
            addShip(result, allShips->data()[0x36], 1);
        }
        if (Station_getIndex(station) == 0x78 &&
            ((Status *)(status))->getCurrentCampaignMission() > 0x9e &&
            (((Status *)(status))->hardCoreMode() ||
             ((Achievements *)(*g_Generator_achievements))->gotAllSupernovaMedals())) {
            addShip(result, allShips->data()[0x2c], 1);
        }
        if (Station_getIndex(station) == 0x78 &&
            ((Status *)(status))->getCurrentCampaignMission() > 0x9e) {
            addShip(result, allShips->data()[0x31], 1);
        }
        if (terranBonus &&
            ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
            addShip(result, allShips->data()[0x33], 0);
        }
    }

    if (Station_getSystem(station) == 0x11 &&
        ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
        addShip(result, allShips->data()[0x2a], 1);
    }
    if (Station_getSystem(station) == 0x11 &&
        ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
        addShip(result, allShips->data()[0x2b], 2);
    }
    if (Station_getSystem(station) == 0x11 &&
        ((AbyssEngine::AERandom *)(*g_Generator_shipRandom))->nextInt() == 0) {
        addShip(result, allShips->data()[0x34], 0);
    }

    return result;
}

// ---- getItemBuyList_95428.cpp ----
typedef Item *(*ItemFactory)(Item *, int, int);

extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_itemStatus;
extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_allItems;
extern "C" __attribute__((visibility("hidden"))) void **volatile
    g_Generator_itemGalaxy;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_itemRandom;
extern "C" __attribute__((visibility("hidden"))) ItemFactory volatile
    g_Generator_introFactory;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_jumpDriveBoost;
extern "C" __attribute__((visibility("hidden"))) int *volatile
    g_Generator_weaponBoost;
extern "C" __attribute__((visibility("hidden"))) uint8_t *volatile
    g_Generator_unlockFlags;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_kaamoAllowed[10];
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_blockedItems[9];

Array<Item *> *Generator::getItemBuyList(Station *station) {
    Status *status = *g_Generator_itemStatus;
    int stationIndex = Station_getIndex(station);

    if (stationIndex == 0x4e &&
        ((Status *)(status))->getCurrentCampaignMission() < 7) {
        Array<Item *> *items = (Array<Item *> *)operator new(0xc);
        Array_item_ptr_ctor(items);
        ArraySetLength_item_ptr(3, items);
        Array<Item *> *all = *g_Generator_allItems;
        ItemFactory makeIntro = g_Generator_introFactory;
        items->data()[0] = makeIntro(all->data()[0], 1, 0);
        items->data()[1] = makeIntro(all->data()[0x16], 1, 0);
        items->data()[2] = makeIntro(all->data()[0x37], 1, 0);
        return items;
    }

    if (Station_getIndex(station) == 0x6c ||
        ((Status *)(status))->inSupernovaSystem() != 0) {
        return 0;
    }

    uint32_t stationId = Station_getIndex(station);
    Array<Item *> *result = (Array<Item *> *)operator new(0xc);
    Array_item_ptr_ctor(result);

    Array<Item *> *allItems = *g_Generator_allItems;
    void *galaxy = *g_Generator_itemGalaxy;
    Array<SolarSystem *> *systems = (Array<SolarSystem *> *)((Galaxy *)(galaxy))->getSystems();
    int stationTec = Station_getTecLevel(station);
    int minTec = stationTec / 2;
    if (stationTec < 4) {
        minTec = 1;
    }
    if ((stationId | 2) == 0x6b) {
        minTec = 0;
    }

    if (((Status *)(status))->getCurrentCampaignMission() == 0x8b &&
        Station_getSystem(station) == 0x19) {
        ArrayAdd_item_ptr(((Item *)(allItems->data()[0xbe]))->makeItem(), result);
    }

    if (stationId == 0x7e) {
        Item *item = ((Item *)(allItems->data()[0xd1]))->makeItem();
        int amount = 1;
        if (((Status *)(status))->getCurrentCampaignMission() != 0x75) {
            amount = ((AbyssEngine::AERandom *)(*g_Generator_itemRandom))->nextInt() + 1;
        }
        ((Item *)(item))->setAmount(amount);
        ArrayAdd_item_ptr(item, result);
    }

    AbyssEngine::AERandom *random = *g_Generator_itemRandom;
    float campaignFactor =
        (float)(((Status *)(status))->getCurrentCampaignMission() + 0x19) / 100.0f;
    if (campaignFactor > 1.5f) {
        campaignFactor = 1.5f;
    }

    for (uint32_t i = 0; i < allItems->size(); ++i) {
        Item *item = allItems->data()[i];
        int itemIndex = ((Item *)(item))->getIndex();
        bool systemGood = itemIndex >= 0x84 && itemIndex < 0x9a;
        bool forcedStation = false;

        if (Station_getIndex(station) == ((Item *)(item))->getAttribute(0x3d)) {
            if ((((Item *)(item))->getIndex() < 0xc4 || ((Item *)(item))->getIndex() > 0xc4 ||
                 ((Status *)(status))->getCurrentCampaignMission() > 0x8d) &&
                (((Item *)(item))->getIndex() < 0xc6 || ((Item *)(item))->getIndex() > 0xc8 ||
                 ((Status *)(status))->getCurrentCampaignMission() > 0x8d)) {
                forcedStation = true;
            }
        }

        if (stationId == 0x6a) {
            bool allowed = false;
            for (uint32_t j = 0; j < 10; ++j) {
                if (g_Generator_kaamoAllowed[j] == ((Item *)(item))->getIndex() ||
                    systemGood) {
                    allowed = true;
                    break;
                }
            }
            if (!allowed) {
                continue;
            }
        }

        int occurrence = ((Item *)(item))->getOccurence();
        if (((Item *)(item))->getIndex() == 0x7a && *g_Generator_jumpDriveBoost != 0) {
            occurrence = (int)((float)occurrence +
                               (float)(occurrence *
                                       *g_Generator_jumpDriveBoost) *
                                   0.05f);
        }
        if (((Item *)(item))->getType() == 1 && *g_Generator_weaponBoost != 0) {
            occurrence =
                (int)((float)occurrence +
                      (float)(occurrence * *g_Generator_weaponBoost) * 0.05f);
        }

        int sort = ((Item *)(item))->getSort();
        int mission = ((Status *)(status))->getCurrentCampaignMission();
        if (occurrence == 0 &&
            ((itemIndex > 0xc3 && g_Generator_unlockFlags[0x37] != 0) ||
             (itemIndex < 0xc4 && g_Generator_unlockFlags[0x35] != 0))) {
            occurrence = 0;
            if (itemIndex != 0x55 && ((Item *)(item))->getType() != 4 &&
                ((Item *)(item))->getIngredients() == 0 &&
                (itemIndex != 0xb5 || mission > 0x3a) &&
                (((sort != 0x22 && (sort | 2) != 0x23) || mission > 0x8d) &&
                 (sort != 0x24 && (mission > 0x8d || sort != 0x2b))) &&
                ((1 < (uint32_t)((itemIndex & 0xfffffff7) - 0xd1)) &&
                 (mission > 0x5d || itemIndex != 0xcd))) {
                bool blocked = false;
                for (uint32_t j = 0; j < 9; ++j) {
                    if (g_Generator_blockedItems[j] == itemIndex) {
                        blocked = true;
                    }
                }
                if (!blocked &&
                    (sort != 0x1d || ((Status *)(status))->inBlackMarketSystem() != 0)) {
                    int roll = ((AbyssEngine::AERandom *)(random))->nextInt();
                    occurrence =
                        (int)((float)roll +
                              ((float)((Item *)(item))->getTecLevel() / -10.0f + 1.0f) *
                                  30.0f);
                }
            }
        }

        int itemTec = ((Item *)(item))->getTecLevel();
        bool accept = forcedStation;
        if (!accept) {
            if (((Item *)(item))->getIngredients() == 0 &&
                (uint32_t)((i & 0x7fffffff) - 0xd9) > 1 && i != 0xa4 &&
                i != 0xaf && occurrence != 0 &&
                itemTec <= Station_getTecLevel(station) &&
                ((Item *)(item))->getSinglePrice() != 0) {
                if (((Item *)(item))->getAttribute(0x3c) == 1 &&
                    ((SolarSystem *)(systems->data()[Station_getSystem(station)]))->getRace() !=
                        1) {
                    continue;
                }
                if (stationId != 0x6a && systemGood &&
                    ((Item *)(item))->getIndex() != Station_getSystem(station) + 0x84) {
                    continue;
                }
                accept = true;
            }
        }

        if (!accept) {
            continue;
        }
        if (((Status *)(status))->hardCoreMode() &&
            (((Item *)(item))->getSort() == 0x17 || ((Item *)(item))->getSort() == 0x18)) {
            continue;
        }
        if (stationId == 0x6b && ((Item *)(item))->getType() != 3) {
            continue;
        }
        if (stationId == 0x69 &&
            (!((Item *)(item))->isWeapon() && ((Item *)(item))->getSort() != 0x1c)) {
            continue;
        }
        if (stationId == 0x65 && !((Item *)(item))->isWeapon()) {
            continue;
        }
        if (stationId == 0x6a && ((Item *)(item))->getType() != 4) {
            continue;
        }

        int randomGate = (int)(campaignFactor * (float)occurrence);
        if (!forcedStation) {
            if ((itemTec > stationTec && !systemGood) ||
                ((AbyssEngine::AERandom *)(random))->nextInt() >= randomGate) {
                continue;
            }
            if (((Item *)(item))->getIndex() != 0x7a && itemTec < minTec &&
                ((AbyssEngine::AERandom *)(random))->nextInt() >= 0x3d) {
                continue;
            }
        }

        int minSystem = ((Item *)(item))->getMinPriceSystem();
        int minX = ((SolarSystem *)(systems->data()[minSystem]))->getX();
        int minY = ((SolarSystem *)(systems->data()[minSystem]))->getY();
        int stationSystem = Station_getSystem(station);
        int stationX = ((SolarSystem *)(systems->data()[stationSystem]))->getX();
        int stationY = ((SolarSystem *)(systems->data()[stationSystem]))->getY();
        int distance =
            ((Galaxy *)(galaxy))->invDistancePercent(stationX, stationY, minX, minY);
        int amount = ((AbyssEngine::AERandom *)(random))->nextInt() + 5;

        if (((Item *)(item))->getType() == 4 || ((Item *)(item))->getType() == 1) {
            if (((Item *)(item))->getIndex() != 0x6d && ((Item *)(item))->getType() == 4) {
                if (distance > 0x32) {
                    float hard = ((Status *)(status))->hardCoreMode() ? 2.0f : 20.0f;
                    int scaled = (int)(((float)(distance - 0x32) / 100.0f) *
                                       hard);
                    if (scaled < 2) {
                        scaled = 1;
                    }
                    amount *= scaled;
                }
                if (((Item *)(item))->getIndex() == 0x6e &&
                    ((AbyssEngine::AERandom *)(random))->nextInt() + 10 < amount) {
                    amount = ((AbyssEngine::AERandom *)(random))->nextInt() + 10;
                }
            }
        } else {
            amount /= 5;
            if (amount < 1) {
                amount = 1;
            }
        }

        Item *newItem = ((Item *)(item))->makeItem();
        ((Item *)(newItem))->setAmount(amount);
        ArrayAdd_item_ptr(newItem, result);
    }

    return result;
}

// ---- getLootList_96504.cpp ----
extern "C" __attribute__((visibility("hidden"))) Array<Item *> **volatile
    g_Generator_lootItems;
extern "C" __attribute__((visibility("hidden"))) AbyssEngine::AERandom **volatile
    g_Generator_lootRandom;
extern "C" __attribute__((visibility("hidden"))) int volatile
    g_Generator_typeChances[8];
extern "C" __attribute__((visibility("hidden"))) Status **volatile
    g_Generator_status;

Array<int> *Generator::getLootList(int itemIndex, int amount) {
    if (itemIndex >= 0) {
        Array<int> *result = (Array<int> *)operator new(0xc);
        Array_int_ctor(result);
        ArrayAdd_int(itemIndex, result);
        ArrayAdd_int(amount, result);
        return result;
    }

    Array<Item *> **itemsPtr = g_Generator_lootItems;
    Array<Item *> *itemsForOccurence = *itemsPtr;
    Array<Item *> *items = *itemsPtr;
    AbyssEngine::AERandom **randomPtr = g_Generator_lootRandom;
    int itemCount = (int)itemsForOccurence->size();

    int pairCount = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
    if (((AbyssEngine::AERandom *)(*randomPtr))->nextInt() == 0) {
        return 0;
    }

    Array<int> *result = (Array<int> *)operator new(0xc);
    Array_int_ctor(result);
    uint32_t length = (uint32_t)(pairCount << 1);
    if (pairCount == 0) {
        length = 2;
    }
    ArraySetLength_int(length, result);

    for (uint32_t out = 0; out < result->size(); out += 2) {
        bool found = false;
        int type = 0;
        int selected = 0;

        for (uint32_t tries = 0; !found && tries < 100; ++tries) {
            selected = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            Item *item = items->data()[selected];
            type = ((Item *)(item))->getType();
            if (((Item *)(item))->getIngredients() == 0 &&
                ((AbyssEngine::AERandom *)(*randomPtr))->nextInt() < g_Generator_typeChances[type]) {
                int roll = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
                int occurrence =
                    ((Item *)(itemsForOccurence->data()[selected]))->getOccurence();
                if (roll < occurrence && ((Item *)(item))->getSinglePrice() >= 1) {
                    found = true;
                    if ((uint32_t)(selected - 0xd9) < 2 || selected == 0xa4 ||
                        selected == 0xaf) {
                        found = false;
                    } else if (type != 4 && ((Item *)(item))->getTecLevel() > 7) {
                        found = false;
                    }
                }
            }
        }

        int count;
        if (found) {
            result->data()[out] = selected;
            if (type == 4) {
                count = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
                type = 4;
            } else {
                count = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            }
        } else {
            result->data()[out] = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt() + 0x9a;
            count = ((AbyssEngine::AERandom *)(*randomPtr))->nextInt();
            type = 4;
        }
        result->data()[out | 1] = count + 1;
    }

    Status **statusPtr = g_Generator_status;
    Ship *ship = ((Status *)(*statusPtr))->getShip();
    if (((Ship *)(ship))->hasJumpDrive() != 0) {
        ship = ((Status *)(*statusPtr))->getShip();
        if (((Ship *)(ship))->hasCargo(0x7a, 1) == 0 &&
            ((AbyssEngine::AERandom *)(*randomPtr))->nextInt() < 10) {
            result->data()[0] = 0x7a;
        }
    }
    return result;
}

// ---- isKaamoSpecialItem_95404.cpp ----
extern "C" __attribute__((visibility("hidden"))) const volatile int
    kaamoSpecialItems[9];

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
