#include "gof2/engine/core/IApplicationModule.h"
#include "gof2/engine/core/ApplicationManager.h"

void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
    this->applicationManager = manager;
    this->paintCanvas = static_cast<PaintCanvas *>(manager->paintCanvas);
}
