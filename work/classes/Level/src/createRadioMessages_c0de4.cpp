#include "class.h"

struct RadioMessage;

extern "C" {
void *Level_opnew_crms(unsigned size);
void  ArrayRadio_ctor_crms(void *a);
void  ArraySetLength_Radio_crms(unsigned n, void *a);
void  RadioMessage_ctor_crms(RadioMessage *m, int id, int speaker, int kind, int delay);
// A couple of cases use a randomized delay constant the original loads from a data slot.
int   crms_randDelay(int which);
}

// One scripted chatter line.
struct RMSpec { int id, speaker, kind, delay; };

// Builds an Array<RadioMessage*> at this+0x114 from a static spec table.
static void buildQueue(char *self, const RMSpec *specs, unsigned n)
{
    void *arr = Level_opnew_crms(0xc);
    ArrayRadio_ctor_crms(arr);
    *(void **)(self + 0x114) = arr;
    ArraySetLength_Radio_crms(n, arr);
    for (unsigned i = 0; i < n; i = i + 1) {
        RadioMessage *m = (RadioMessage *)Level_opnew_crms(0x28);
        RadioMessage_ctor_crms(m, specs[i].id, specs[i].speaker, specs[i].kind, specs[i].delay);
        ((RadioMessage **)(*(int *)((char *)arr + 4)))[i] = m;
    }
}

// Level::createRadioMessages(int set) — populates the level's radio-chatter queue with the fixed
// dialogue sequence for the requested mission/event set.
extern "C" void Level_createRadioMessages(Level *thisptr, int set)
{
    char *self = (char *)thisptr;
    *(int *)(self + 0x114) = 0;

    switch (set) {
    case 0: {
        static const RMSpec t[] = {
            {0x684,0x11,5,0x5dc},{0x685,0,6,0},{0x686,0,6,1},{0x687,10,6,2},{0x688,0xb,6,3},
            {0x689,9,6,4},{0x68a,9,6,5},{0x68b,9,6,6},{0x68c,0,6,7},{0x693,0,9,0},{0x694,0,6,9},
            {0x695,0,6,10},{0x696,0xf,6,0xb},{0x697,0,6,0xc},{0x698,0,6,0xd},{0x699,0,6,0xe},
            {0x69a,0,0x1b,0xc},{0x69b,0xf,6,0x10},{0x69c,0,6,0x11},{0x69d,0xf,6,0x12},
            {0x69e,0,6,0x13},{0x69f,0,6,0x14},{0x6a0,0xf,6,0x15},
        };
        buildQueue(self, t, 0x17); break;
    }
    case 1: {
        static const RMSpec t[] = {{0x6a1,2,5,10000},{0x6a2,2,6,0},{0x6a3,2,6,1}};
        buildQueue(self, t, 3); break;
    }
    case 7: {
        static const RMSpec t[] = {{0x6dc,2,0x10,0},{0x6dd,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x10: {
        static const RMSpec t[] = {{0x72e,0x13,5,10000},{0x72f,0,6,0},{0x730,0,9,0},
                                   {0x731,1,6,2},{0x732,0,6,3}};
        buildQueue(self, t, 5); break;
    }
    case 0x15: {
        static const RMSpec t[] = {{0x759,10,0x10,0},{0x75a,0,6,0},{0x75b,10,0x19,2},
                                   {0x75c,0xe,8,0},{0x75d,0xe,0x15,0}};
        buildQueue(self, t, 5); break;
    }
    case 0x18: {
        static const RMSpec t[] = {{0x77d,0x13,5,12000},{0x77e,6,6,0},{0x77f,0,6,1},
                                   {0x780,6,0x16,3},{0x781,6,6,3}};
        buildQueue(self, t, 5); break;
    }
    case 0x19: {
        static const RMSpec t[] = {{0x785,0,5,20000},{0x786,6,6,0},{0x787,0,6,1}};
        buildQueue(self, t, 3); break;
    }
    case 0x26: {
        static const RMSpec t[] = {{0x7ed,0x15,5,15000}};
        buildQueue(self, t, 1); break;
    }
    case 0x28: {
        static const RMSpec t[] = {{0x7fa,0,5,10000},{0x7fb,8,6,0},{0x7fc,0,6,1},{0x7fd,7,5,40000},
                                   {0x7fe,0,6,3},{0x7ff,7,0xc,0},{0x800,0,0x18,0}};
        buildQueue(self, t, 7); break;
    }
    case 0x29: {
        // two delays come from data slots DAT_000d19e0 / DAT_000d19e4 at runtime.
        RMSpec t[] = {{0x804,0,5,crms_randDelay(0)},{0x805,7,6,0},{0x806,0,6,1},
                      {0x807,7,6,2},{0x808,7,0x1a,crms_randDelay(1)},{0x809,7,6,4},
                      {0x80f,0,1,0},{0x810,0,6,6}};
        buildQueue(self, t, 8); break;
    }
    case 0x31: {
        static const RMSpec t[] = {{0x848,0,5,8000}};
        buildQueue(self, t, 1); break;
    }
    case 0x32: {
        static const RMSpec t[] = {{0x849,0x3f,5,8000},{0x84a,0,6,0},{0x84b,0x3f,6,1},{0x84c,0,6,2}};
        buildQueue(self, t, 4); break;
    }
    case 0x33: {
        static const RMSpec t[] = {{0x84d,0x3f,5,8000},{0x84e,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x34: {
        static const RMSpec t[] = {{0x84f,0x3f,5,8000},{0x850,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x36: {
        static const RMSpec t[] = {{0x851,0,5,8000}};
        buildQueue(self, t, 1); break;
    }
    case 0x37: {
        static const RMSpec t[] = {{0x85a,0,5,8000},{0x85b,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x38: {
        static const RMSpec t[] = {{0x86a,0x1b,5,8000},{0x86b,0,6,0},{0x86c,0x1c,0x10,0},
                                   {0x86d,0,0x14,3},{0x86e,0,6,3},{0x86f,0x1b,0x1c,0}};
        buildQueue(self, t, 6); break;
    }
    case 0x40: {
        static const RMSpec t[] = {{0x8b4,0,5,8000},{0x8b5,0x14,6,0},{0x8b6,0,6,1},{0x8b7,0x1e,6,2},
                                   {0x8b8,0,6,3},{0x8b9,0x1e,0x14,5},{0x8ba,0,6,5}};
        buildQueue(self, t, 7); break;
    }
    case 0x41: {
        static const RMSpec t[] = {{0x8cb,0,5,12000},{0x8cc,0x14,6,0},{0x8cd,0,6,1}};
        buildQueue(self, t, 3); break;
    }
    case 0x43: {
        static const RMSpec t[] = {{0x8ef,0,0x10,0},{0x8f0,0x1f,6,0},{0x8f1,0x1e,6,1},
                                   {0x8f2,0,0x14,2},{0x8f3,0x1f,6,3},{0x8f4,0,6,4},{0x8f5,0,0x14,8},
                                   {0x8f6,0x1f,6,6},{0x8f7,0,6,7},{0x8f8,0x1f,6,8},{0x8f9,0,6,9},
                                   {0x8fa,0x1f,6,10}};
        buildQueue(self, t, 0xc); break;
    }
    case 0x45: {
        static const RMSpec t[] = {{0x90e,0,5,8000},{0x90f,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x46: {
        static const RMSpec t[] = {{0x910,0,5,8000},{0x911,0,6,0},{0x912,0x22,6,1},{0x913,0,6,2}};
        buildQueue(self, t, 4); break;
    }
    case 0x49: {
        static const RMSpec t[] = {{0x92b,0,5,8000},{0x92c,0,0x10,0},{0x92d,0xb,6,1},{0x92e,0,6,2},
                                   {0x92f,0,0x1b,1},{0x930,0x21,0x1b,2},{0x931,0,0x1b,3},
                                   {0x932,0,6,6}};
        buildQueue(self, t, 8); break;
    }
    case 0x77: {
        static const RMSpec t[] = {{0xab9,0x11,0x1b,1}};
        buildQueue(self, t, 1); break;
    }
    case 0x78: {
        static const RMSpec t[] = {{0xac5,0,5,0x5dc}};
        buildQueue(self, t, 1); break;
    }
    case 0x83: {
        static const RMSpec t[] = {{0xb2b,0,0x1b,2}};
        buildQueue(self, t, 1); break;
    }
    case 0x85: {
        static const RMSpec t[] = {{0xb33,0x11,0x1b,1}};
        buildQueue(self, t, 1); break;
    }
    case 0x87: {
        static const RMSpec t[] = {{0xb43,0x31,0x1b,1},{0xb44,0,6,0},{0xb45,0,0x1b,2},
                                   {0xb46,0,0x1b,3}};
        buildQueue(self, t, 4); break;
    }
    case 0x89: {
        static const RMSpec t[] = {{0xb4f,0x32,5,0x5dc},{0xb50,0,6,0},{0xb51,0x32,6,1},
                                   {0xb52,0,6,2},{0xb53,0x32,6,3}};
        buildQueue(self, t, 5); break;
    }
    case 0x90: {
        static const RMSpec t[] = {{0xb98,0x27,5,7000},{0xb99,0x27,6,0},{0xb9a,0,6,1},
                                   {0xb9b,0x27,6,2}};
        buildQueue(self, t, 4); break;
    }
    case 0x91: {
        static const RMSpec t[] = {{0xb9c,0,5,7000},{0xb9d,0x27,6,0},{0xb9e,0x27,0x1b,5}};
        buildQueue(self, t, 3); break;
    }
    case 0x93: {
        static const RMSpec t[] = {{0xbac,0,5,7000},{0xbad,0,6,0}};
        buildQueue(self, t, 2); break;
    }
    case 0x50: {
        static const RMSpec t[] = {{0x96e,0,5,8000},{0x96f,6,5,25000},{0x970,0x1a,6,1},
                                   {0x971,0,6,2},{0x972,6,6,3}};
        buildQueue(self, t, 5); break;
    }
    default:
        break;
    }
}
