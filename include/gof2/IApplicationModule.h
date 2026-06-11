#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct IApplicationModule {
    ApplicationManager* paintCanvas;      // +0x4
    ApplicationManager* applicationManager;      // +0x8

    // ---- methods (converted from free functions) ----
    void SetApplicationManager(ApplicationManager *manager);
};
#endif
