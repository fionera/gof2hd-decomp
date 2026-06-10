#include "gof2/IApplicationModule.h"




// ---- SetApplicationManager_74154.cpp ----
void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
    IApplicationModule *self = this;
    self->field_0x8 = manager;
    self->field_0x4 = *(ApplicationManager **)manager;
}
