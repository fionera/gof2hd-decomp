#include "gof2/engine/core/IApplicationModule.h"

void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
    IApplicationModule *self = this;
    self->applicationManager = manager;
    self->paintCanvas = *(ApplicationManager **)manager;
}
