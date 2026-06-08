#include "class.h"

namespace FMOD { struct Event; struct EventParameter; }

// FModSound::setParamValue(FMOD::Event* e, int paramIdx, float val)
extern "C" int FMOD_Event_getParameterByIndex(void *event, int idx, FMOD::EventParameter **out);
extern "C" int FMOD_EventParameter_setValue(FMOD::EventParameter *p, float v);

struct FModSound {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    uint8_t f_8; // 0x8
    unsigned char _pad_9[3];
    void* f_c; // 0xc
    uint8_t f_10; // 0x10
    unsigned char _pad_11[9199];
    int f_2400; // 0x2400
    int f_2404; // 0x2404
    int f_2408; // 0x2408
    int f_240c; // 0x240c
    unsigned char _pad_2410[20];
    void* f_2424; // 0x2424
    void* f_2428; // 0x2428
    void* f_242c; // 0x242c
    void* f_2430; // 0x2430
    void* f_2434; // 0x2434
    void* f_2438; // 0x2438
 void setParamValue(FMOD::Event *e, int paramIdx, float val); };
void FModSound::setParamValue(FMOD::Event *e, int paramIdx, float val)
{
    if (e != 0 && u32(this, OFF_SYSTEM) != 0) {
        FMOD::EventParameter *p;
        FMOD_Event_getParameterByIndex(e, paramIdx, &p);
        FMOD_EventParameter_setValue(p, val);
    }
}
