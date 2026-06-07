#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_lights_engine;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_lights_canvas;

extern "C" void *ApplicationManager_GetEngine(void *manager);
extern "C" void Engine_LightSetMaterialColorAmbient(void *engine, float r, float g, float b);
extern "C" void PaintCanvas_LightEnable(void *canvas, int light, bool enabled);

void StarMap::initLights()
{
    void *engine = ApplicationManager_GetEngine(*g_StarMap_lights_engine);
    Engine_LightSetMaterialColorAmbient(engine, 0.5f, 0.5f, 0.5f);
    return PaintCanvas_LightEnable(*g_StarMap_lights_canvas, 0, true);
}
