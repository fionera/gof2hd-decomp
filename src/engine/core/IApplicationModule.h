#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"

namespace AbyssEngine {
    class ApplicationManager;
    class PaintCanvas;
}

namespace AbyssEngine {
    class IApplicationModule;
}
using ::AbyssEngine::ApplicationManager;


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

        virtual void OnTouchBegin(int x, int y, void *data);

        virtual void OnTouchMove(int x, int y, void *data);

        virtual void OnTouchEnd(int x, int y, void *data);

        virtual void OnUpdate() {
        }

        virtual void OnRender3D() {
        }

        virtual void OnRender2D() {
        }

        virtual void OnSuspend();

        virtual void OnResume();

        virtual int ShowLoadingScreen() { return 0; }

        void SetApplicationManager(ApplicationManager *manager);
    };
}

using ::AbyssEngine::IApplicationModule;
#endif
