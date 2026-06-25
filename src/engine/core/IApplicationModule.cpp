#include "engine/core/IApplicationModule.h"
#include "engine/core/ApplicationManager.h"

namespace AbyssEngine {
    void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
        this->applicationManager = manager;
        this->paintCanvas = static_cast<PaintCanvas *>(manager->paintCanvas);
    }


}
