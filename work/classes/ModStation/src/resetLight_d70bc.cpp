#include "class.h"

struct Engine;
struct SolarSystem;

__attribute__((visibility("hidden"))) extern int **g_rl_engineHolder; // [DAT_000e71f4]
__attribute__((visibility("hidden"))) extern int   g_rl_lightFlag;    // DAT_000e71f0

extern "C" {
int  ApplicationManager_GetEngine_rl();
int  Status_getSystem_rl();
int  SolarSystem_getRace_rl();
int  rl_engineFromHolder(int holder); // (*pcVar4)(*puVar3): resolve engine from the cached holder
// The actual Engine light setters all take float colour triples the decompiler dropped; the
// engine helper applies the per-race ambient/diffuse/specular profile to the given engine.
void Engine_resetStationLight(int engine, int race);
void Engine_LightEnable_rl(int engine, int flag);
}

// ModStation::resetLight() — restores the hangar's lighting rig to the home-system's race profile.
extern "C" void ModStation_resetLight()
{
    int holder = *(int *)g_rl_engineHolder;
    rl_engineFromHolder(holder); // material ambient
    rl_engineFromHolder(holder); // light direction
    rl_engineFromHolder(holder); // diffuse colour

    Status_getSystem_rl();
    int race = SolarSystem_getRace_rl();
    int engine;
    switch (race) {
    case 1: case 2: case 3: case 8:
        engine = ApplicationManager_GetEngine_rl();
        break;
    default:
        engine = ApplicationManager_GetEngine_rl();
        break;
    }
    Engine_resetStationLight(engine, race); // ambient colour for the race
    engine = ApplicationManager_GetEngine_rl();
    // specular colour
    engine = ApplicationManager_GetEngine_rl();
    Engine_LightEnable_rl(engine, g_rl_lightFlag);
}
