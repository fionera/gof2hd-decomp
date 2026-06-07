#include "class.h"

extern "C" void IApplicationModule_SetApplicationManager(IApplicationModule *self, ApplicationManager *manager)
{
    *(ApplicationManager **)((char *)self + 0x8) = manager;
    *(ApplicationManager **)((char *)self + 0x4) = *(ApplicationManager **)manager;
}
