#ifndef GOF2_GAMERECORD_H
#define GOF2_GAMERECORD_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class GameRecord {
public:
    // Heap buffer holding the visited-systems bitmap (0x87 entries) and the
    // record's variable-length payload.
    void *data;                     // +0x00

    // Packed save-image payload up to the first named preview field. Reached
    // positionally by the serializer and by load().
    uint8_t  payload0a[0x0c];       // +0x04 .. +0x0F

    // String* holding the play-time / timestamp text for this record; passed as
    // the String* out-param to Globals::longToTimeStringNoSeconds when building
    // the save-slot preview row (MenuTouchWindow createRecordButtons).
    void    *playTimeObj;           // +0x10

    uint8_t  payload0b[0x0c];       // +0x14 .. +0x1F

    // Pilot level / kills count from the save image (written from
    // Status::getLevel()). The save-slot preview takes its address to build the
    // "Kills: <n>" label; recordStoreReadPreview reads it back as an int.
    int      killsText;             // +0x20

    uint8_t  payload1[0xf1];        // +0x24 .. +0x115

    // DLC-required flag byte. Non-zero means the save needs DLC content that
    // must be present to load (MenuTouchWindow gate at +0x115).
    uint8_t  dlcRequiredFlag;       // +0x115
    uint8_t  pad_0x116;             // +0x116

    // Version-mismatch flag byte. Non-zero means the save was written by an
    // incompatible build (MenuTouchWindow gate at +0x117).
    uint8_t  versionMismatchFlag;   // +0x117
    uint8_t  pad_0x118[0x4];        // +0x118 .. +0x11c

    // Pilot rank, float (recordStoreReadPreview reads it as a float; the ctor
    // and load() touch the same word as +0x11c).
    float    rank;                  // +0x11c

    uint8_t  payload2[0x68];        // +0x120 .. +0x188

    // System name shown in the save-slot preview (read second by
    // recordStoreReadPreview into +0x188).
    String   nameString;            // +0x188

    // Station / pilot name shown in the save-slot preview (read first by
    // recordStoreReadPreview into +0x194).
    String   pilotName;             // +0x194

    // Index of the player's ship at save time (0xffffffff when unset). Drives
    // the ship thumbnail in the save-slot list.
    uint32_t shipId;                // +0x1a0

    uint8_t  payload3[0x24];        // +0x1a4 .. +0x1c8

    GameRecord();
    ~GameRecord();

    // Apply this saved snapshot to the live game singletons (Status, Galaxy,
    // Achievements, the player Ship and the station stack).
    void load();
};

#endif // GOF2_GAMERECORD_H
