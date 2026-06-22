#ifndef GOF2_RADIOMESSAGE_H
#define GOF2_RADIOMESSAGE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Radio;
class Objective;
class PlayerEgo;
class LevelScript;
class Player;

class RadioMessage {
public:
    Radio *radio; // owning radio
    Objective *objective; // associated mission objective (condition type 0x0b)
    int textID; // dialogue text id
    int imageID; // speaker portrait image id
    int conditionType; // selects the trigger evaluated in triggered()
    int conditionValue; // condition parameter (target index/count/threshold)
    int targetCount; // number of entries in targetIndices
    int *targetIndices; // enemy-list indices this message watches
    uint8_t triggeredFlag; // condition currently satisfied flag
    uint8_t over; // message finished/consumed flag
    int lastRouteIndex; // last seen route waypoint (condition types 0/0x19)

    RadioMessage(int textID, int imageID, int conditionType, int conditionValue, int targetCount);

    RadioMessage(int textID, int imageID, int conditionType, int conditionValue);

    RadioMessage(int textID, int imageID, Objective *objective);

    ~RadioMessage();

    void setRadio(Radio *radio);

    int getTextID();

    int getImageID();

    int getSoundID();

    void finish();

    uint8_t isOver();

    void reset();

    void trigger();

    uint8_t isTriggered();

    int triggered(int64_t time, PlayerEgo *ego, LevelScript *script);
};
#endif
