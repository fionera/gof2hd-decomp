#ifndef GOF2_AESOUNDRESSOURCE_H
#define GOF2_AESOUNDRESSOURCE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

struct AESoundInfo {
    int id;
    int field_4;
    int field_8;
};
struct AESoundInterface;

class AESoundRessource {
public:
    char* soundInfoTable;                    // +0x0  AESoundInfo* table base (byte addressed)
    uint32_t soundCount;                 // +0x4  count
    char* sounds;                    // +0x8  Array<AESoundInterface*> data
    volatile int numSounds;            // +0x10

    AESoundRessource();
    ~AESoundRessource();

    // Release every loaded AESoundInterface and clear its slot.
    void freeAllRessources();
    // Install the sound-info table (`info`, `count` entries) and size the sound array.
    void SetSound(AESoundInfo *info, int count);
    // Look up the entry whose id == `id`; copies it to `info` and reports the slot in `index`.
    void getSoundInfo(int id, AESoundInfo *info, int *index);
    // Lazily create + load the AESoundInterface for sound `id`.
    void init(int id);
    // Play sound `id` at the given volume.
    void play_impl(int id, int volume);
    // No-op in this build (kept for the loop-mode bookkeeping hook).
    void checkLooping();
};

} // namespace AbyssEngine



struct AESoundStackFrame {
    char info[12];
    int index;
    volatile int cookie;
};
#endif
