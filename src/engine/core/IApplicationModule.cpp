#include "engine/core/IApplicationModule.h"
#include "engine/core/ApplicationManager.h"

namespace AbyssEngine {

IApplicationModule::IApplicationModule() {}
IApplicationModule::~IApplicationModule() {}

// Default no-op virtual interface. Concrete modules (MTitle, ...) override the
// handlers they care about; the rest fall through to these passthrough stubs.
int       IApplicationModule::OnInitialize()                       { return 0; }
void      IApplicationModule::OnRelease()                          {}
long long IApplicationModule::OnKeyPress(long long key, long long) { return key; }
long long IApplicationModule::OnKeyRelease(long long key, long long) { return key; }
int       IApplicationModule::OnTouchBegin(int x, int)             { return x; }
int       IApplicationModule::OnTouchMove(int x, int)              { return x; }
void      IApplicationModule::OnTouchEnd(int, int)                 {}
int       IApplicationModule::OnTouchBegin(int x, int, void *)     { return x; }
int       IApplicationModule::OnTouchMove(int x, int, void *)      { return x; }
int       IApplicationModule::OnTouchEnd(int x, int, void *)       { return x; }
void      IApplicationModule::OnUpdate()                           {}
void      IApplicationModule::OnRender3D()                         {}
void      IApplicationModule::OnRender2D()                         {}
void      IApplicationModule::OnSuspend()                          {}
void      IApplicationModule::OnResume()                           {}
int       IApplicationModule::ShowLoadingScreen()                  { return 0; }

void IApplicationModule::SetApplicationManager(ApplicationManager *manager) {
    this->applicationManager = manager;
    this->paintCanvas = static_cast<PaintCanvas *>(manager->paintCanvas);
}

}  // namespace AbyssEngine
