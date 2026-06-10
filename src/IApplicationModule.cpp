#include "gof2/IApplicationModule.h"




// ---- SetApplicationManager_74154.cpp ----
extern "C" void IApplicationModule_SetApplicationManager(IApplicationModule *self, ApplicationManager *manager)
{
    self->field_0x8 = manager;
    self->field_0x4 = *(ApplicationManager **)manager;
}
