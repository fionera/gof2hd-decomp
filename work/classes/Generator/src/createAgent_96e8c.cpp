#include "class.h"

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

    int race = SolarSystem_getRace(Status_getSystem(status));
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
               Status_getCurrentCampaignMission(status) < 0x10) {
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
    int systemIndex = SolarSystem_getIndex(Status_getSystem(status));
    Agent_ctor(agent, -1, &name, stationIndex, systemIndex, race, male, -1, -1,
               -1, -1);
    AbyssEngine_String_dtor(&name);
    Agent_setOffer(agent, offer);
    ImageFactory **factoryPtr = g_Generator_imageFactory;
    Agent_setImageParts(agent, ImageFactory_createChar(*factoryPtr, male, race));

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
            friendNames->data[i] = friendName;
        }
        Agent_setWingmanFriendNames(agent, friendNames);
        int costRoll = AERandom_nextInt(*randomPtr, 0x514);
        Agent_setCosts(agent, (costRoll + 700) * (count + 1));
        if (Status_hardCoreMode(status) != 0) {
            Agent_setCosts(agent, Agent_getCosts(agent) * 7);
        }
    } else if (Agent_getOffer(agent) == 2) {
        Array<Item *> **itemsPtr = g_Generator_agentItems;
        Array<Item *> *items = *itemsPtr;
        int selected;
        do {
            do {
                selected = AERandom_nextInt(*randomPtr, items->length);
            } while ((uint32_t)(selected - 0xd9) < 2);
        } while (selected == 0x83 || selected == 0xa4 || selected == 0xaf ||
                 Item_getIngredients(items->data[selected]) != 0 ||
                 Item_getSinglePrice(items->data[selected]) == 0 ||
                 Item_getOccurence(items->data[selected]) == 0);

        Item *item = items->data[selected];
        int amount = AERandom_nextInt(*randomPtr, 0xf);
        int type = Item_getType(item);
        if (type == 3 || Item_getType(item) == 0 || Item_getType(item) == 2) {
            amount = 1;
        } else {
            amount += 5;
        }
        float factor =
            (float)(AERandom_nextInt(*randomPtr, 0x78) + 0x28) / 100.0f;
        int price = Item_getSinglePrice((*itemsPtr)->data[selected]);
        Agent_setSellItemData(agent, selected, amount,
                              amount * (int)(factor * (float)price));
    }

    return agent;
}
