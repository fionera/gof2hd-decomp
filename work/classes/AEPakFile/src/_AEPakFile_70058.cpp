#include "class.h"

extern "C" AEPakFile *_ZN9AEPakFileD2Ev(AEPakFile *self);
extern "C" void AEPakFile_deleteTail(AEPakFile *self);

extern "C" void _ZN9AEPakFileD0Ev(AEPakFile *self)
{
    AEPakFile_deleteTail(_ZN9AEPakFileD2Ev(self));
}
