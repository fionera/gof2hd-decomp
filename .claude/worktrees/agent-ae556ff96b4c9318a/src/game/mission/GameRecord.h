#ifndef GOF2_GAMERECORD_H
#define GOF2_GAMERECORD_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// GameRecord — a saved-game snapshot (top-level class, no AbyssEngine namespace).
//
// A GameRecord is the in-memory image of a single save slot: it is filled by
// RecordHandler's deserializer and then applied to the live game singletons by
// load(). Because it mirrors the on-disk serialization format, the bulk of the
// record is a flat block of scalar fields and Array<T>* slots addressed by the
// reader/writer; only the heap data pointer, the two embedded name strings and
// the player-ship slot are referenced symbolically here. The remaining record
// payload is kept as a packed storage block so the save layout stays stable.

class GameRecord {
public:
    // Heap buffer holding the visited-systems bitmap (0x87 entries) and the
    // record's variable-length payload.
    void *data;

    // Packed save-image payload (scalar fields and Array<T>* slots laid out to
    // match the on-disk record). Spans the record body up to the embedded
    // strings; reached positionally by the serializer and by load().
    uint8_t payload[0x184];

    // Display name and description shown in the save-slot preview.
    String nameString;
    String descString;

    GameRecord();
    ~GameRecord();

    // Apply this saved snapshot to the live game singletons (Status, Galaxy,
    // Achievements, the player Ship and the station stack).
    void load();
};

#endif // GOF2_GAMERECORD_H
