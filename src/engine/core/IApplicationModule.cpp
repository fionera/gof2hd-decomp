#include "gof2/engine/core/IApplicationModule.h"




// ---- SetApplicationManager_74154.cpp ----
void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
    IApplicationModule *self = this;
    self->applicationManager = manager;
    self->paintCanvas = *(ApplicationManager **)manager;
}
