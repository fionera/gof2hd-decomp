#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine { class ApplicationManager; }
using ::AbyssEngine::ApplicationManager;
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

    IApplicationModule() {}
    virtual ~IApplicationModule() {}

    // Default no-op virtual interface, in vtable order. Concrete modules
    // (MTitle, ...) override the handlers they care about; the rest fall through
    // to these inline passthrough stubs. The bodies are inline-in-class so the
    // base only contributes them to its vtable -- exactly as the shipped binary,
    // which emits no standalone out-of-line symbol for any of them.
    virtual int       OnInitialize()                            { return 0; }
    virtual void      OnRelease()                               {}
    virtual long long OnKeyPress(long long key, long long)      { return key; }
    virtual long long OnKeyRelease(long long key, long long)    { return key; }
    virtual int       OnTouchBegin(int x, int)                  { return x; }
    virtual int       OnTouchMove(int x, int)                   { return x; }
    virtual void      OnTouchEnd(int, int)                      {}
    virtual int       OnTouchBegin(int x, int, void *)          { return x; }
    virtual int       OnTouchMove(int x, int, void *)           { return x; }
    virtual int       OnTouchEnd(int x, int, void *)            { return x; }
    virtual void      OnUpdate()                                {}
    virtual void      OnRender3D()                              {}
    virtual void      OnRender2D()                              {}
    virtual void      OnSuspend()                               {}
    virtual void      OnResume()                                {}
    virtual int       ShowLoadingScreen()                       { return 0; }

    void SetApplicationManager(ApplicationManager *manager);
};
}  // namespace AbyssEngine
using ::AbyssEngine::IApplicationModule;
#endif
