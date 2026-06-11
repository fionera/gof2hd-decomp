#include "gof2/Generator.h"
#include "gof2/Galaxy.h"
#include "gof2/Item.h"
#include "gof2/Achievements.h"
// Several headers (Agent/SolarSystem/Station/Wanted) each define an identical
// global `struct RetStr` (the 12-byte by-value String aggregate) unconditionally.
// Station.h provides the canonical one (Mission.h delegates to it); rename the
// `RetStr` tag to a unique name for the other headers so their definitions don't
// collide. Generator never names RetStr directly and discards these getters'
// return values, so the distinct tags are harmless.
#define RetStr RetStr
#include "gof2/Agent.h"
#undef RetStr
#include "gof2/Globals.h"
#include "gof2/Station.h"
#include "gof2/Mission.h"
#define RetStr RetStr
#include "gof2/SolarSystem.h"
#undef RetStr
#include "gof2/Standing.h"
#define RetStr RetStr
#include "gof2/Wanted.h"
#undef RetStr
#include "gof2/Status.h"




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
            int take = AERandom_nextInt(*random, 3);
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

        int roll = AERandom_nextInt(*randomPtr, 100);
        selected = station;
        if (roll >= 20) {
            roll = AERandom_nextInt(*randomPtr, 100);
            if (roll < 40) {
                int currentIndex = ((Status *)(*statusPtr))->getSystem();
                Array<int> *stations =
                    SolarSystem_getStations(systems->data()[currentIndex]);
                int pick = AERandom_nextInt(*randomPtr, stations->size());
                selected = stations->data()[pick];
            } else {
                selected = AERandom_nextInt(*randomPtr, 0x87);
            }
        }

        int currentSystemIndex = ((Status *)(*statusPtr))->getSystem();
        if (currentSystemIndex == 0xf) {
            Array<int> *stations =
                SolarSystem_getStations(systems->data()[((Status *)(*statusPtr))->getSystem()]);
            Array<int> *stations2 =
                SolarSystem_getStations(systems->data()[((Status *)(*statusPtr))->getSystem()]);
            int pick = AERandom_nextInt(*randomPtr, stations2->size());
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

        if (SolarSystem_getRoutes(systems->data()[systemIndex]) == 0) {
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
            int agentStation = Agent_getStation(existing->data()[i]);
            int stationIndex = Station_getIndex(station);
            if (agentStation == stationIndex && keepExisting) {
                ++count;
            }
        }

        if (Station_getIndex(station) != 0x6c) {
            int roll = AERandom_nextInt(*randomPtr, 2);
            if ((int)(roll + count + 3) < 5) {
                count += 3 + AERandom_nextInt(*randomPtr, 2);
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
            if (Agent_getStation(agent) == Station_getIndex(station) &&
                keepExisting) {
                result->data()[out++] = agent;
                int offer = Agent_getOffer(agent);
                if (offer == 9) {
                    int index;
                    if (((Status *)(status))->getCurrentCampaignMission() < 0x8e) {
                        index = AERandom_nextInt(*randomPtr, 7) + 2;
                    } else {
                        index = AERandom_nextInt(*randomPtr, 9);
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
                            if (Ship_getIndex(ship) != shipId) {
                                ArrayAdd_int(shipId, choices);
                            }
                        }
                    }
                    if (choices->size() != 0) {
                        int pick =
                            AERandom_nextInt(*randomPtr, choices->size());
                        int shipId = choices->data()[pick];
                        ((Agent *)(agent))->setSellItemData(shipId, 1, Ship_getPrice((*g_Generator_agentsShips)
                                              ->data()[shipId]));
                        ((Agent *)(agent))->setOfferAccepted(false);
                    } else {
                        ((Agent *)(agent))->setOfferAccepted(true);
                    }
                    operator delete(Array_int_dtor(choices));
                }
            }
        }

        Array<SolarSystem *> *systems =
            Galaxy_getSystems(*g_Generator_galaxy);
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
            Agent_setOffer(agent, 2);
            ((Agent *)(agent))->setSellItemData(0x44, 1, 0);
            ((Agent *)(agent))->setImageParts(ImageFactory_createChar(*g_Generator_storyImages,
                                                        1, 0));
            result->data()[out++] = agent;
        }

        bool hasWingman = false;
        for (uint32_t i = out; i < result->size(); ++i) {
            result->data()[i] = createAgent(station);
            Agent *agent = result->data()[i];
            if (Agent_getOffer(agent) == 6) {
                if (hasWingman) {
                    hasWingman = true;
                    Agent_setOffer(agent, 1);
                } else {
                    hasWingman = true;
                }
            } else if (Agent_getOffer(agent) == 0) {
                ((Agent *)(agent))->setMission(createMission(agent, systems));
            }
        }

        if (AERandom_nextInt(*randomPtr, 100) < 0x23) {
            void *standing = (void *)(long)((Status *)(status))->getStanding();
            for (uint32_t raceIndex = 0; raceIndex < 4; ++raceIndex) {
                int race = g_Generator_enemyRaces[raceIndex];
                if (((Standing *)(standing))->isEnemy(race) != 0) {
                    for (uint32_t i = 0; i < result->size(); ++i) {
                        Agent *agent = result->data()[i];
                        if (((Agent *)(agent))->isGenericAgent() &&
                            Agent_getOffer(agent) != 7) {
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
                            Agent_setOffer(agent, 7);
                            ((Agent *)(agent))->setImageParts(ImageFactory_createChar(
                                           *g_Generator_enemyImages, 1, race));
                            break;
                        }
                    }
                }
            }
        }

        if ((((Status *)(status))->getCurrentCampaignMission() == 0x17 &&
             Station_getIndex(station) == 10) ||
            AERandom_nextInt(*randomPtr, 100) == 1) {
            for (; out < result->size(); ++out) {
                Agent *agent = result->data()[out];
                if (!((Agent *)(agent))->isStoryAgent() && Agent_getOffer(agent) == 0 &&
                    ((Agent *)(agent))->getMission() != 0) {
                    Mission *m = ((Agent *)(agent))->getMission();
                    int reward = Mission_getReward(m);
                    if (reward < 50000) {
                        int newReward = 50000;
                        if (reward * 10 < 50000) {
                            newReward = reward * 10;
                        }
                        Mission_setReward(m, newReward);
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

    int agentStation = Agent_getStation(agent);
    int targetStation = generateStationIndex(systems, agentStation);
    if (((Status *)(status))->getSystem() == 0xf) {
        Array<int> *stations = SolarSystem_getStations(systems->data()[((Status *)(status))->getSystem()]);
        targetStation = stations->data()[0] + AERandom_nextInt(random, 4);
    }

    uint32_t race = Agent_getRace(agent);
    uint32_t type = 0;
    bool selected = false;
    int guard = 999;
    Array<uint8_t> *used =
        F<Array<uint8_t> *>(*g_Generator_missionStatus, 0x50);
    while (!selected) {
        selected = guard < 1;
        uint32_t pick = AERandom_nextInt(random, 15);
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
        switch (AERandom_nextInt(random, 5)) {
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
            targetStation = Agent_getStation(agent);
            break;
        default:
            type = 0xb;
            break;
        }
    } else if (type == 0xc) {
        targetStation = Agent_getStation(agent);
    }

    if (Agent_getOffer(agent) == 5) {
        targetStation = Agent_getStation(agent);
        type = 8;
    } else {
        if (type < 0xf && ((1U << (type & 0xff)) & 0x4801U) != 0) {
            while (targetStation == Station_getIndex(((Status *)(status))->getStation())) {
                targetStation = generateStationIndex(systems, Agent_getStation(agent));
            }
        }
        if (race < 4 && type == 0xd) {
            SolarSystem *agentSystem = systems->data()[Agent_getSystem(agent)];
            if (SolarSystem_getRoutes(agentSystem) == 0) {
                type = AERandom_nextInt(random, 2) == 0 ? 1 : 4;
            } else {
                bool ok = false;
                while (!ok) {
                    targetStation =
                        generateStationIndex(systems, Agent_getStation(agent));
                    for (uint32_t i = 0; i < systems->size(); ++i) {
                        if (((SolarSystem *)(systems->data()[i]))->stationIsInSystem_int(targetStation) &&
                            (uint32_t)SolarSystem_getRace(systems->data()[i]) ==
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
        AERandom_nextInt(random,
                         ((Status *)(status))->getCurrentCampaignMission() < 0x10 ? 2 : 9) +
        1;

    int itemId = 0;
    int amount = 0;
    if (type == 8) {
        Array<Item *> *items = *g_Generator_missionItems;
        do {
            itemId = AERandom_nextInt(random, items->size() - 0x61) + 0x61;
        } while (((Item *)(items->data()[itemId]))->getOccurence() == 0 ||
                 ((Item *)(items->data()[itemId]))->getSinglePrice() == 0 ||
                 ((Item *)(items->data()[itemId]))->getIngredients() != 0 ||
                 ((itemId - 0x61) & 0xfffffffe) == 0x78 ||
                 itemId == 0x75 || ((itemId - 0x61) & 0xfffffffe) == 0x12 ||
                 itemId == 0x83 || itemId == 0xa4 || itemId == 0xaf);
        amount = AERandom_nextInt(random, 15) + 5;
    } else {
        switch (type) {
        case 0:
            itemId = AERandom_nextInt(random, 7);
            amount = (int)(((float)difficulty / 10.0f) * 35.0f) + 5;
            break;
        case 2:
            amount = (int)((float)AERandom_nextInt(random, 4)) + 2;
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
            amount = AERandom_nextInt(random, 90) + 30;
            for (uint32_t i = 0; i < systems->size(); ++i) {
                if (((SolarSystem *)(systems->data()[i]))->stationIsInSystem_int(targetStation)) {
                    int *prob = (int *)((Galaxy *)(*g_Generator_missionGalaxy))->getAsteroidProbabilities(((Status *)(status))->getStation());
                    itemId = prob[AERandom_nextInt(random, 3) * 2];
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
    int distance = Galaxy_distance(*g_Generator_missionGalaxy, to, from);
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
            (int)(reward * (float)((Standing *)(((Status *)(status))->getStanding()))->getMissionBonus(Agent_getRace(agent)));
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

    int costs = rewardInt / 10 + AERandom_nextInt(random, rewardInt / 10);
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
    Mission_setCosts(mission, roundedCosts);
    ((Mission *)(mission))->setProductionGoods(itemId, amount);
    Mission_setBonus(mission, bonus);

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

    int race = SolarSystem_getRace((SolarSystem *)(long)((Status *)(status))->getSystem());
    if (AERandom_nextInt(*randomPtr, 100) < 20) {
        race = AERandom_nextInt(*randomPtr, 8);
    }

    int offer = -1;
    bool valid = false;
    do {
        offer = AERandom_nextInt(*randomPtr, 7);
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

    if (AERandom_nextInt(*randomPtr, 100) < 0x21) {
        offer = 0;
    } else if ((uint32_t)(offer - 5) < 2 &&
               ((Status *)(status))->getCurrentCampaignMission() < 0x10) {
        offer = 0;
    }

    int male = 1;
    if (race == 0 && offer != 6) {
        male = 0;
        if (AERandom_nextInt(*randomPtr, 100) < 0x3c) {
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
    Agent_setOffer(agent, offer);
    ImageFactory **factoryPtr = g_Generator_imageFactory;
    ((Agent *)(agent))->setImageParts(ImageFactory_createChar(*factoryPtr, male, race));

    if (Agent_getOffer(agent) == 6) {
        uint32_t count = AERandom_nextInt(*randomPtr, 3);
        Array<AbyssEngine::String *> *friendNames =
            (Array<AbyssEngine::String *> *)operator new(0xc);
        Array_string_ptr_ctor(friendNames);
        ArraySetLength_string_ptr(count, friendNames);
        for (int i = 0; i < (int)count; ++i) {
            AbyssEngine::String *friendName =
                (AbyssEngine::String *)operator new(0xc);
            Globals_getRandomName(friendName, *names, Agent_getRace(agent), 1);
            friendNames->data()[i] = friendName;
        }
        // setWingmanFriendNames takes an opaque Array<String*>* (declared as uint32_t*).
        ((Agent *)(agent))->setWingmanFriendNames((uint32_t *)friendNames);
        int costRoll = AERandom_nextInt(*randomPtr, 0x514);
        Agent_setCosts(agent, (costRoll + 700) * (count + 1));
        if (((Status *)(status))->hardCoreMode() != 0) {
            Agent_setCosts(agent, Agent_getCosts(agent) * 7);
        }
    } else if (Agent_getOffer(agent) == 2) {
        Array<Item *> **itemsPtr = g_Generator_agentItems;
        Array<Item *> *items = *itemsPtr;
        int selected;
        do {
            do {
                selected = AERandom_nextInt(*randomPtr, items->size());
            } while ((uint32_t)(selected - 0xd9) < 2);
        } while (selected == 0x83 || selected == 0xa4 || selected == 0xaf ||
                 ((Item *)(items->data()[selected]))->getIngredients() != 0 ||
                 ((Item *)(items->data()[selected]))->getSinglePrice() == 0 ||
                 ((Item *)(items->data()[selected]))->getOccurence() == 0);

        Item *item = items->data()[selected];
        int amount = AERandom_nextInt(*randomPtr, 0xf);
        int type = ((Item *)(item))->getType();
        if (type == 3 || ((Item *)(item))->getType() == 0 || ((Item *)(item))->getType() == 2) {
            amount = 1;
        } else {
            amount += 5;
        }
        float factor =
            (float)(AERandom_nextInt(*randomPtr, 0x78) + 0x28) / 100.0f;
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
    Ship *ship = Ship_makeShip(base, -1);
    ArrayAdd_ship_ptr(ship, list);
    Ship *added = list->data()[list->size() - 1];
    Ship_setRace(added, race);
    Ship_adjustPrice(added);
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
            if (Ship_hasJumpDriveIntegrated(base) &&
                Ship_getIndex(base) != 0x25) {
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

    int race = SolarSystem_getRace((SolarSystem *)(long)((Status *)(status))->getSystem());
    bool gold = Station_getIndex(station) == 10 &&
                ((Achievements *)(*g_Generator_achievements))->gotAllGoldMedals();
    int stationIndex = Station_getIndex(station);
    int count;
    if (gold) {
        count = 1;
    } else {
        count = AERandom_nextInt(*g_Generator_shipRandom, 6);
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
                AERandom_nextInt(*g_Generator_shipRandom, 100) < 0x16) {
                int roll = AERandom_nextInt(*g_Generator_shipRandom, 5);
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
                    Ship_getIndex(result->data()[j]) == shipIndex) {
                    unique = false;
                    break;
                }
            }
        }
        result->data()[i] = Ship_makeShip(allShips->data()[shipIndex], -1);
        Ship_setRace(result->data()[i], g_Generator_shipRaces[shipIndex]);
        Ship_adjustPrice(result->data()[i]);
    }

    bool terranBonus = false;
    int raceFlag = 0;
    if (race == 0) {
        if (AERandom_nextInt(*g_Generator_shipRandom, 7) == 0) {
            addShip(result, allShips->data()[0x3e], 3);
        }
        terranBonus = true;
    } else if (race == 1) {
        raceFlag = 1;
        if (AERandom_nextInt(*g_Generator_shipRandom, 5) == 0) {
            addShip(result, allShips->data()[0x3f], 1);
        }
    } else if (race == 2 &&
               AERandom_nextInt(*g_Generator_shipRandom, 8) == 0) {
        addShip(result, allShips->data()[0x3d], 1);
    }

    if (g_Generator_shipFlags[0x35] && ((Status *)(status))->dlc1Won() && raceFlag) {
        if (AERandom_nextInt(*g_Generator_shipRandom, 2) == 0) {
            addShip(result, allShips->data()[0x27], 1);
        }
        if (AERandom_nextInt(*g_Generator_shipRandom, 2) == 0) {
            addShip(result, allShips->data()[0x29], 1);
        }
    }

    if (g_Generator_shipFlags[0x37] != 0) {
        if (raceFlag &&
            AERandom_nextInt(*g_Generator_shipRandom, 4) == 0) {
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
            AERandom_nextInt(*g_Generator_shipRandom, 8) == 0) {
            addShip(result, allShips->data()[0x33], 0);
        }
    }

    if (Station_getSystem(station) == 0x11 &&
        AERandom_nextInt(*g_Generator_shipRandom, 3) == 0) {
        addShip(result, allShips->data()[0x2a], 1);
    }
    if (Station_getSystem(station) == 0x11 &&
        AERandom_nextInt(*g_Generator_shipRandom, 3) == 0) {
        addShip(result, allShips->data()[0x2b], 2);
    }
    if (Station_getSystem(station) == 0x11 &&
        AERandom_nextInt(*g_Generator_shipRandom, 3) == 0) {
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
    Array<SolarSystem *> *systems = Galaxy_getSystems(galaxy);
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
            amount = AERandom_nextInt(*g_Generator_itemRandom, 10) + 1;
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
                    int roll = AERandom_nextInt(random, 30);
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
                    SolarSystem_getRace(systems->data()[Station_getSystem(station)]) !=
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
                AERandom_nextInt(random, 100) >= randomGate) {
                continue;
            }
            if (((Item *)(item))->getIndex() != 0x7a && itemTec < minTec &&
                AERandom_nextInt(random, 100) >= 0x3d) {
                continue;
            }
        }

        int minSystem = ((Item *)(item))->getMinPriceSystem();
        int minX = SolarSystem_getX(systems->data()[minSystem]);
        int minY = SolarSystem_getY(systems->data()[minSystem]);
        int stationSystem = Station_getSystem(station);
        int stationX = SolarSystem_getX(systems->data()[stationSystem]);
        int stationY = SolarSystem_getY(systems->data()[stationSystem]);
        int distance =
            ((Galaxy *)(galaxy))->invDistancePercent(stationX, stationY, minX, minY);
        int amount = AERandom_nextInt(random, 15) + 5;

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
                    AERandom_nextInt(random, 10) + 10 < amount) {
                    amount = AERandom_nextInt(random, 10) + 10;
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

    int pairCount = AERandom_nextInt(*randomPtr, 3);
    if (AERandom_nextInt(*randomPtr, 3) == 0) {
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
            selected = AERandom_nextInt(*randomPtr, itemCount);
            Item *item = items->data()[selected];
            type = ((Item *)(item))->getType();
            if (((Item *)(item))->getIngredients() == 0 &&
                AERandom_nextInt(*randomPtr, 100) < g_Generator_typeChances[type]) {
                int roll = AERandom_nextInt(*randomPtr, 100);
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
                count = AERandom_nextInt(*randomPtr, 9);
                type = 4;
            } else {
                count = AERandom_nextInt(*randomPtr, 3);
            }
        } else {
            result->data()[out] = AERandom_nextInt(*randomPtr, 10) + 0x9a;
            count = AERandom_nextInt(*randomPtr, 9);
            type = 4;
        }
        result->data()[out | 1] = count + 1;
    }

    Status **statusPtr = g_Generator_status;
    Ship *ship = ((Status *)(*statusPtr))->getShip();
    if (Ship_hasJumpDrive(ship) != 0) {
        ship = ((Status *)(*statusPtr))->getShip();
        if (Ship_hasCargo(ship, 0x7a, 1) == 0 &&
            AERandom_nextInt(*randomPtr, 100) < 10) {
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
