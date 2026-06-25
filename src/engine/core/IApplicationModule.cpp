#include "engine/core/IApplicationModule.h"
#include "engine/core/ApplicationManager.h"

namespace AbyssEngine {
    void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
        this->applicationManager = manager;
        this->paintCanvas = static_cast<PaintCanvas *>(manager->paintCanvas);
    }
    // OnTouchBegin/Move/End(3-arg), OnSuspend, OnResume are inline in the header
    // so no key function homes IApplicationModule's vtable here (matches original).
}
