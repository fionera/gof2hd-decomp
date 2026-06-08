#include "class.h"

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
    if (Status_inSupernovaSystem(status) == 0) {
        Array<Agent *> *existing = Status_getAgents(status);
        int mission = Status_getCurrentCampaignMission(status);
        bool keepExisting = mission > 0x10;
        if (Status_dlc1Won(status) == 0) {
            keepExisting = mission > 0x10 && Station_getIndex(station) != 0x6a;
        }

        uint32_t count = 0;
        for (uint32_t i = 0; i < existing->length; ++i) {
            int agentStation = Agent_getStation(existing->data[i]);
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
        for (uint32_t i = 0; i < existing->length; ++i) {
            Agent *agent = existing->data[i];
            if (Agent_getStation(agent) == Station_getIndex(station) &&
                keepExisting) {
                result->data[out++] = agent;
                int offer = Agent_getOffer(agent);
                if (offer == 9) {
                    int index;
                    if (Status_getCurrentCampaignMission(status) < 0x8e) {
                        index = AERandom_nextInt(*randomPtr, 7) + 2;
                    } else {
                        index = AERandom_nextInt(*randomPtr, 9);
                    }
                    int itemId = g_Generator_offerItemIds[index];
                    Item *item = (*g_Generator_agentsItems)->data[itemId];
                    int amount = ((uint32_t)(index - 3) < 2) ? 10 : 1;
                    Agent_setSellItemData(agent, itemId, amount,
                                          Item_getSinglePrice(item) * amount);
                    Agent_setOfferAccepted(agent, false);
                } else if (offer == 10) {
                    Array<int> *choices = (Array<int> *)operator new(0xc);
                    Array_int_ctor(choices);
                    for (int j = 0; j != 6; ++j) {
                        int shipId = g_Generator_offerShipIds[j];
                        if (Station_hasShip(F<Station *>(status, 0x14c),
                                            shipId) == 0) {
                            Ship *ship = Status_getShip(status);
                            if (Ship_getIndex(ship) != shipId) {
                                ArrayAdd_int(shipId, choices);
                            }
                        }
                    }
                    if (choices->length != 0) {
                        int pick =
                            AERandom_nextInt(*randomPtr, choices->length);
                        int shipId = choices->data[pick];
                        Agent_setSellItemData(
                            agent, shipId, 1,
                            Ship_getPrice((*g_Generator_agentsShips)
                                              ->data[shipId]));
                        Agent_setOfferAccepted(agent, false);
                    } else {
                        Agent_setOfferAccepted(agent, true);
                    }
                    operator delete(Array_int_dtor(choices));
                }
            }
        }

        Array<SolarSystem *> *systems =
            Galaxy_getSystems(*g_Generator_galaxy);
        if (Status_getCurrentCampaignMission(status) == 0x17 &&
            Station_getIndex(station) == 10) {
            Agent *agent = (Agent *)operator new(0x98);
            AbyssEngine::String name;
            Globals_getRandomName(&name, **(int **)(&g_Generator_storyNames),
                                  0, 1);
            int stationIndex = Station_getIndex(station);
            int systemIndex = SolarSystem_getIndex(Status_getSystem(status));
            Agent_ctor(agent, -1, &name, stationIndex, systemIndex, 0, 1, -1,
                       -1, -1, -1);
            AbyssEngine_String_dtor(&name);
            Agent_setOffer(agent, 2);
            Agent_setSellItemData(agent, 0x44, 1, 0);
            Agent_setImageParts(agent,
                                ImageFactory_createChar(*g_Generator_storyImages,
                                                        1, 0));
            result->data[out++] = agent;
        }

        bool hasWingman = false;
        for (uint32_t i = out; i < result->length; ++i) {
            result->data[i] = createAgent(station);
            Agent *agent = result->data[i];
            if (Agent_getOffer(agent) == 6) {
                if (hasWingman) {
                    hasWingman = true;
                    Agent_setOffer(agent, 1);
                } else {
                    hasWingman = true;
                }
            } else if (Agent_getOffer(agent) == 0) {
                Agent_setMission(agent, createMission(agent, systems));
            }
        }

        if (AERandom_nextInt(*randomPtr, 100) < 0x23) {
            void *standing = Status_getStanding(status);
            for (uint32_t raceIndex = 0; raceIndex < 4; ++raceIndex) {
                int race = g_Generator_enemyRaces[raceIndex];
                if (Standing_isEnemy(standing, race) != 0) {
                    for (uint32_t i = 0; i < result->length; ++i) {
                        Agent *agent = result->data[i];
                        if (Agent_isGenericAgent(agent) &&
                            Agent_getOffer(agent) != 7) {
                            result->data[i] = 0;
                            agent = (Agent *)operator new(0x98);
                            AbyssEngine::String name;
                            Globals_getRandomName(&name, *g_Generator_enemyNames,
                                                  race, 1);
                            int stationIndex = Station_getIndex(station);
                            int systemIndex =
                                SolarSystem_getIndex(Status_getSystem(status));
                            Agent_ctor(agent, -1, &name, stationIndex,
                                       systemIndex, race, 1, -1, -1, -1, -1);
                            result->data[i] = agent;
                            AbyssEngine_String_dtor(&name);
                            Agent_setOffer(agent, 7);
                            Agent_setImageParts(
                                agent, ImageFactory_createChar(
                                           *g_Generator_enemyImages, 1, race));
                            break;
                        }
                    }
                }
            }
        }

        if ((Status_getCurrentCampaignMission(status) == 0x17 &&
             Station_getIndex(station) == 10) ||
            AERandom_nextInt(*randomPtr, 100) == 1) {
            for (; out < result->length; ++out) {
                Agent *agent = result->data[out];
                if (!Agent_isStoryAgent(agent) && Agent_getOffer(agent) == 0 &&
                    Agent_getMission(agent) != 0) {
                    Mission *m = Agent_getMission(agent);
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
