#ifndef GOF2_RADIOMESSAGE_H
#define GOF2_RADIOMESSAGE_H
#include "common.h"

// Forward declarations for types referenced by pointer only.
class Radio;
class Objective;
class PlayerEgo;
class LevelScript;
class Player;

// A scripted radio/dialogue message with a trigger condition. Each frame
// triggered() evaluates conditionType against the level state and, when met,
// marks itself as the radio's current message.
class RadioMessage {
public:
    Radio* radio;            // owning radio
    Objective* objective;    // associated mission objective (condition type 0x0b)
    int textID;              // dialogue text id
    int imageID;             // speaker portrait image id
    int conditionType;       // selects the trigger evaluated in triggered()
    int conditionValue;      // condition parameter (target index/count/threshold)
    int targetCount;         // number of entries in targetIndices
    int* targetIndices;      // enemy-list indices this message watches
    uint8_t triggeredFlag;   // condition currently satisfied flag
    uint8_t over;            // message finished/consumed flag
    int lastRouteIndex;      // last seen route waypoint (condition types 0/0x19)

    RadioMessage(int textID, int imageID, int conditionType, int conditionValue, int targetCount);
    RadioMessage(int textID, int imageID, int conditionType, int conditionValue);
    RadioMessage(int textID, int imageID, Objective* objective);
    ~RadioMessage();

    void finish();
    uint8_t isOver();
    void reset();
    void trigger();
    uint8_t isTriggered();
    int triggered(int64_t time, PlayerEgo* ego, LevelScript* script);

private:
    // Mark the message (and, when satisfied, promote it to the radio's current
    // message); return whether the condition fired.
    int setResult(int value);
    int triggerResult();
    // Resolve the i-th watched enemy from the ego's enemy list.
    Player* selectTarget(Array<Player*>* list, int i);
};
#endif
