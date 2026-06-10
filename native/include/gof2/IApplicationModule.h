#ifndef GOF2_IAPPLICATIONMODULE_H
#define GOF2_IAPPLICATIONMODULE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
struct ApplicationManager;
struct IApplicationModule;
}

using ApplicationManager = AbyssEngine::ApplicationManager;
using IApplicationModule = AbyssEngine::IApplicationModule;

struct IApplicationModule {
    ApplicationManager* field_0x4;      // +0x4
    ApplicationManager* field_0x8;      // +0x8
};
#endif
