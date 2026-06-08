#include "class.h"

namespace FMOD { struct EventCategory; }

// FModSound::pauseAllPlayingSoundFXEvents()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) { getCategory(h,&cat);
//       cat->vtbl[0](cat,&a,&b); if (a==1) setPaused(h,1); }
extern "C" int FMOD_Event_getCategory(void *event, FMOD::EventCategory **out);
extern "C" int FMOD_Event_setPaused(void *event, int paused);

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

    unsigned isPlaying(int p1);
    void pauseAllPlayingSoundFXEvents();
};
void FModSound::pauseAllPlayingSoundFXEvents()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM)) {
            void *h = pp(this, i * 4 + OFF_EVENTS);
            if (h && isPlaying(i)) {
                FMOD::EventCategory *cat;
                FMOD_Event_getCategory(h, &cat);
                int a, b;
                void **vt = *(void ***)cat;
                typedef void (*Fn)(void *, int *, int *);
                ((Fn)vt[0])(cat, &a, &b);
                if (a == 1)
                    FMOD_Event_setPaused(pp(this, i * 4 + OFF_EVENTS), 1);
            }
        }
    }
}
