#ifndef GOF2_GAMERECORD_H
#define GOF2_GAMERECORD_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "fieldaccess.h"

class GameRecord {
public:
    void *data;

    uint8_t payload0a[0x0c];

    void *playTimeObj;

    uint8_t payload0b[0x0c];

    int killsText;

    uint8_t payload1[0xf1];

    uint8_t dlcRequiredFlag;
    uint8_t pad_0x116;

    uint8_t versionMismatchFlag;
    uint8_t pad_0x118[0x4];

    float rank;

    uint8_t payload2[0x68];

    String nameString;

    String pilotName;

    uint32_t shipId;

    uint8_t payload3[0x24];

    GameRecord();

    ~GameRecord();

    void load();
};

#endif
