#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class ApplicationManager;
namespace AbyssEngine { class PaintCanvas; }
using ::AbyssEngine::PaintCanvas;

class IApplicationModule {
public:
    PaintCanvas        *paintCanvas;
    ApplicationManager *applicationManager;

    void SetApplicationManager(ApplicationManager *manager);
};
#endif
