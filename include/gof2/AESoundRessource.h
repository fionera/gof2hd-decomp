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

    ~AESoundRessource();
};

} // namespace AbyssEngine



struct AESoundStackFrame {
    char info[12];
    int index;
    volatile int cookie;
};
#endif
