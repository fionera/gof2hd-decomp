#include "class.h"

extern "C" AENormalFile *_ZN12AENormalFileD2Ev(AENormalFile *self);
extern "C" void AENormalFile_deleteTail(AENormalFile *self);

extern "C" void _ZN12AENormalFileD0Ev(AENormalFile *self)
{
    AENormalFile_deleteTail(_ZN12AENormalFileD2Ev(self));
}
