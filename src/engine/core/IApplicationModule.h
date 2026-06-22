#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {
    class ApplicationManager;
}

using ::AbyssEngine::ApplicationManager;

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

namespace AbyssEngine {
    class IApplicationModule {
    public:
        PaintCanvas *paintCanvas;
        ApplicationManager *applicationManager;

        IApplicationModule() {
        }

        virtual ~IApplicationModule() {
        }

        // Default no-op virtual interface, in vtable order. Concrete modules
        // (MTitle, ...) override the handlers they care about; the rest fall through
        // to these inline passthrough stubs. The bodies are inline-in-class so the
        // base only contributes them to its vtable -- exactly as the shipped binary,
        // which emits no standalone out-of-line symbol for any of them.
        virtual int OnInitialize() { return 0; }

        virtual void OnRelease() {
        }

        virtual long long OnKeyPress(long long key, long long) { return key; }
        virtual long long OnKeyRelease(long long key, long long) { return key; }

        virtual void OnTouchBegin(int, int) {
        }

        virtual void OnTouchMove(int, int) {
        }

        virtual void OnTouchEnd(int, int) {
        }

        virtual void OnTouchBegin(int, int, void *) {
        }

        virtual void OnTouchMove(int, int, void *) {
        }

        virtual void OnTouchEnd(int, int, void *) {
        }

        virtual void OnUpdate() {
        }

        virtual void OnRender3D() {
        }

        virtual void OnRender2D() {
        }

        virtual void OnSuspend() {
        }

        virtual void OnResume() {
        }

        virtual int ShowLoadingScreen() { return 0; }

        void SetApplicationManager(ApplicationManager *manager);
    };
} // namespace AbyssEngine
using ::AbyssEngine::IApplicationModule;
#endif
