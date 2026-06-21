#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class ApplicationManager;
namespace AbyssEngine { class PaintCanvas; }
using ::AbyssEngine::PaintCanvas;

// Polymorphic application-module base. Its vtable plants a vptr at offset 0,
// followed by the two engine-owned members. The virtual interface order below
// matches the shipped vtable exactly (the dtor occupies the first two slots).
//
// The real type lives in namespace AbyssEngine (to match the binary's mangled
// names); a global `using` alias keeps the unqualified spelling working for the
// concrete modules that derive from it.
namespace AbyssEngine {
class IApplicationModule {
public:
    PaintCanvas        *paintCanvas;
    ApplicationManager *applicationManager;

    IApplicationModule();
    virtual ~IApplicationModule();

    // Virtual interface, in vtable order.
    virtual int       OnInitialize();
    virtual void      OnRelease();
    virtual long long OnKeyPress(long long key, long long mod);
    virtual long long OnKeyRelease(long long key, long long mod);
    virtual int       OnTouchBegin(int x, int y);
    virtual int       OnTouchMove(int x, int y);
    virtual void      OnTouchEnd(int x, int y);
    virtual int       OnTouchBegin(int x, int y, void *touch);
    virtual int       OnTouchMove(int x, int y, void *touch);
    virtual int       OnTouchEnd(int x, int y, void *touch);
    virtual void      OnUpdate();
    virtual void      OnRender3D();
    virtual void      OnRender2D();
    virtual void      OnSuspend();
    virtual void      OnResume();
    virtual int       ShowLoadingScreen();

    void SetApplicationManager(ApplicationManager *manager);
};
}  // namespace AbyssEngine
using ::AbyssEngine::IApplicationModule;
#endif
