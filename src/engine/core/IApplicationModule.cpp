#include "gof2/engine/core/IApplicationModule.h"

void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
    this->applicationManager = manager;
    this->paintCanvas = *(ApplicationManager **)manager;
}
