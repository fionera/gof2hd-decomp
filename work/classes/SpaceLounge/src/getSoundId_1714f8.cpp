#include "class.h"

extern "C" int Agent_getRace(Agent *agent);
extern "C" int Agent_isMale(Agent *agent);
extern "C" int Agent_getOffer(Agent *agent);
extern "C" void *Agent_getMission(Agent *agent);
extern "C" int Mission_isEmpty(void *mission);
extern "C" int Mission_getType(void *mission);
extern "C" int Agent_hasAcceptedOffer(Agent *agent);
extern "C" void *Agent_getImageParts(Agent *agent);

extern "C" void Globals_getAgentMissionText(void *out, int textId, Agent *agent);
extern "C" int AERandom_nextInt(void *random, int limit);
extern "C" void *GameText_getText(void *texts, int textId);
extern "C" int String_Compare(void *lhs, void *rhs);
extern "C" void String_dtor(void *s);
extern "C" int SpaceLounge_getSpecificSoundForRace(int dummy, int soundId, int race, bool male);

extern "C" int *SpaceLounge_getSoundId_missionText;
extern "C" void *SpaceLounge_getSoundId_offerText;
extern "C" void *SpaceLounge_getSoundId_offer2358910;
extern "C" void *SpaceLounge_getSoundId_offer5;
extern "C" void *SpaceLounge_getSoundId_offer7;
extern "C" void *SpaceLounge_getSoundId_offer4;
extern "C" void *SpaceLounge_getSoundId_offer6;
extern "C" void *SpaceLounge_getSoundId_offer0_11;
extern "C" void *SpaceLounge_getSoundId_offer0_else;
extern "C" void *SpaceLounge_getSoundId_chance;
extern "C" void *SpaceLounge_getSoundId_offer1;
extern "C" void *SpaceLounge_getSoundId_accepted;
extern "C" void *SpaceLounge_getSoundId_specialText;
extern "C" void *SpaceLounge_getSoundId_specialRandom;

static inline int random_from(void *slot, int limit)
{
    return AERandom_nextInt(*(void **)slot, limit);
}

extern "C" int SpaceLounge_getSoundId(SpaceLounge *, Agent *agent)
{
    char missionText[12];

    int race = Agent_getRace(agent);
    bool male = Agent_isMale(agent);
    int offer = Agent_getOffer(agent);
    void *mission = Agent_getMission(agent);
    int missionType;
    if (mission == 0) {
        missionType = -1;
    } else {
        mission = Agent_getMission(agent);
        if (Mission_isEmpty(mission) != 0) {
            missionType = -1;
        } else {
            mission = Agent_getMission(agent);
            missionType = Mission_getType(mission);
        }
    }

    Globals_getAgentMissionText(missionText, *SpaceLounge_getSoundId_missionText, agent);

    bool checkSpecialText = true;
    int soundId;
    switch (offer) {
    case 0:
        if (missionType == 0 || missionType == 0xb) {
            soundId = random_from(&SpaceLounge_getSoundId_offer0_11, 4) + 0x301;
        } else if (missionType == 0xc) {
            soundId = random_from(&SpaceLounge_getSoundId_offer0_else, 4) + 0x2fa;
        } else {
            void *random = *(void **)&SpaceLounge_getSoundId_offer0_else;
            int first = AERandom_nextInt(random, 2);
            if (first == 0) {
                soundId = AERandom_nextInt(random, 4) + 0x31f;
            } else {
                soundId = AERandom_nextInt(random, 4) + 0x309;
            }
        }
        break;
    case 1:
        soundId = random_from(&SpaceLounge_getSoundId_offer1, 2) + 0x30d;
        break;
    case 2:
    case 3:
    case 8:
    case 9:
    case 10:
        soundId = random_from(&SpaceLounge_getSoundId_offer2358910, 2) + 0x2f7;
        break;
    case 4:
        soundId = random_from(&SpaceLounge_getSoundId_offer4, 2) + 0x2fe;
        break;
    case 5:
        soundId = random_from(&SpaceLounge_getSoundId_offer5, 4) + 0x31b;
        break;
    case 6:
        soundId = random_from(&SpaceLounge_getSoundId_offer6, 4) + 0x323;
        break;
    case 7:
        soundId = random_from(&SpaceLounge_getSoundId_offer7, 4) + 0x305;
        break;
    default:
        soundId = -1;
        break;
    }

    if (offer != 1) {
        checkSpecialText = false;
        if (random_from(&SpaceLounge_getSoundId_chance, 100) < 30) {
            soundId = random_from(&SpaceLounge_getSoundId_offer1, 2) + 0x30d;
            checkSpecialText = true;
        }
    }

    int dummy = 0;
    if (Agent_hasAcceptedOffer(agent) != 0) {
        dummy = random_from(&SpaceLounge_getSoundId_accepted, 2);
        soundId = dummy + 0x30d;
    }

    if (checkSpecialText) {
        void *texts = *(void **)&SpaceLounge_getSoundId_specialText;
        void *text = GameText_getText(*(void **)texts, 0x334);
        if (String_Compare(missionText, text) != 0) {
            text = GameText_getText(*(void **)texts, 0x338);
            if (String_Compare(missionText, text) != 0) {
                text = GameText_getText(*(void **)texts, 0x33b);
                if (String_Compare(missionText, text) != 0) {
                    text = GameText_getText(*(void **)texts, 0x341);
                    dummy = String_Compare(missionText, text);
                    if (dummy != 0) {
                        goto special_done;
                    }
                }
            }
        }
        dummy = random_from(&SpaceLounge_getSoundId_specialRandom, 2);
        soundId = dummy + 0x314;
    }

special_done:
    if (race == 3) {
        void *parts = Agent_getImageParts(agent);
        if (parts == 0) {
            race = 3;
            dummy = 0;
        } else {
            parts = Agent_getImageParts(agent);
            dummy = I(parts, 0x0);
            race = 0;
            if (dummy == 2) {
                race = 3;
            }
        }
    }

    int result = SpaceLounge_getSpecificSoundForRace(dummy, soundId, race, male);
    String_dtor(missionText);
    return result;
}
