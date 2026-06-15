#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "gof2/common.h"

class ApplicationManager;
struct PaintCanvas;

class IApplicationModule {
public:
    PaintCanvas        *paintCanvas;
    ApplicationManager *applicationManager;

    void SetApplicationManager(ApplicationManager *manager);
};
#endif
