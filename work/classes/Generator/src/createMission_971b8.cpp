#include "class.h"

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
    if (SolarSystem_getIndex(Status_getSystem(status)) == 0xf) {
        Array<int> *stations = SolarSystem_getStations(Status_getSystem(status));
        targetStation = stations->data[0] + AERandom_nextInt(random, 4);
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
            uint8_t *data = used->data;
            if (data[pick] != 0) {
                uint32_t sum = 0;
                for (uint32_t i = 0; i < used->length; ++i) {
                    sum += data[i];
                }
                if (sum == 0xe) {
                    for (uint32_t i = 0; i < used->length; ++i) {
                        data[i] = 0;
                    }
                }
                continue;
            }
            data[pick] = 1;
        }
        if (pick == 0xc) {
            selected = true;
            if (SolarSystem_getIndex(Status_getSystem(status)) != 0x19) {
                type = pick;
            } else {
                type = 0xc;
            }
        } else {
            selected = true;
        }
    }

    if (Status_getCurrentCampaignMission(status) < 0x10 ||
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
            while (targetStation == Station_getIndex(Status_getStation(status))) {
                targetStation = generateStationIndex(systems, Agent_getStation(agent));
            }
        }
        if (race < 4 && type == 0xd) {
            SolarSystem *agentSystem = systems->data[Agent_getSystem(agent)];
            if (SolarSystem_getRoutes(agentSystem) == 0) {
                type = AERandom_nextInt(random, 2) == 0 ? 1 : 4;
            } else {
                bool ok = false;
                while (!ok) {
                    targetStation =
                        generateStationIndex(systems, Agent_getStation(agent));
                    for (uint32_t i = 0; i < systems->length; ++i) {
                        if (SolarSystem_stationIsInSystem(systems->data[i],
                                                          targetStation) &&
                            (uint32_t)SolarSystem_getRace(systems->data[i]) ==
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
    Agent_getName(&agentName, agent);
    int difficulty =
        AERandom_nextInt(random,
                         Status_getCurrentCampaignMission(status) < 0x10 ? 2 : 9) +
        1;

    int itemId = 0;
    int amount = 0;
    if (type == 8) {
        Array<Item *> *items = *g_Generator_missionItems;
        do {
            itemId = AERandom_nextInt(random, items->length - 0x61) + 0x61;
        } while (Item_getOccurence(items->data[itemId]) == 0 ||
                 Item_getSinglePrice(items->data[itemId]) == 0 ||
                 Item_getIngredients(items->data[itemId]) != 0 ||
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
            for (uint32_t i = 0; i < systems->length; ++i) {
                if (SolarSystem_stationIsInSystem(systems->data[i],
                                                  targetStation)) {
                    int *prob = Galaxy_getAsteroidProbabilities(
                        *g_Generator_missionGalaxy,
                        Status_getStation(status));
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
        systems->data[Station_getSystem(Status_getStation(status))];
    SolarSystem *to = systems->data[Station_getSystem(Galaxy_getStation(
        *g_Generator_missionGalaxy))];
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
                         Item_getSinglePrice((*g_Generator_missionItems)
                                                 ->data[itemId])) *
                 1.2f;
    } else if (type == 3 || type == 5) {
        reward += reward;
    } else if (type == 0xb) {
        reward = reward * 0.4f + ((reward * 0.4f) / 5.0f) * (float)amount;
    }

    int level = Status_getLevel(status);
    reward += (float)(level * level * level * 10);

    int bonus = 0;
    if ((type | 4) != 0xc) {
        int rawBonus =
            (int)(reward * (float)Standing_getMissionBonus(
                                Status_getStanding(status), Agent_getRace(agent)));
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
    Mission_ctor_full(mission, type, &missionName, Agent_getImageParts(agent),
                      race, rewardInt, targetStation, difficulty);

    int costs = rewardInt / 10 + AERandom_nextInt(random, rewardInt / 10);
    if (type == 8) {
        costs = (int)((float)costs * 0.5f);
    } else if (type == 6) {
        AbyssEngine::String targetName;
        Globals_getRandomName(&targetName, *g_Generator_targetNames, 0, 1);
        Mission_setTargetName(mission, &targetName);
        AbyssEngine_String_dtor(&targetName);
    }
    int costRem = costs % 50;
    int roundedCosts = costs + costRem;
    if (roundedCosts % 50 != 0) {
        roundedCosts = costs - costRem;
    }
    Mission_setCosts(mission, roundedCosts);
    Mission_setProductionGoods(mission, itemId, amount);
    Mission_setBonus(mission, bonus);

    for (uint32_t i = 0; i < systems->length; ++i) {
        if (SolarSystem_stationIsInSystem(systems->data[i], targetStation)) {
            AbyssEngine::String systemName;
            SolarSystem_getName(&systemName, systems->data[i]);
            Mission_setTargetSystemName(mission, &systemName);
            AbyssEngine_String_dtor(&systemName);
            break;
        }
    }

    AbyssEngine_String_dtor(&missionName);
    AbyssEngine_String_dtor(&agentName);
    return mission;
}
