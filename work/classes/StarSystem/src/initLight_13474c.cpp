#include "class.h"

typedef Engine *(*EngineGetter)(void *app);

__attribute__((visibility("hidden"))) extern void **g_StarSystem_init_app;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_init_status;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_init_canvas;
__attribute__((visibility("hidden"))) extern EngineGetter g_StarSystem_init_getEngine;
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_sunColors[];
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_lightColors[];
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_stationColors[];

extern "C" Engine *ApplicationManager_GetEngine(void *app);
extern "C" void *Status_getSystem(void *status);
extern "C" uint32_t SolarSystem_getTextureIndex(void *system);
extern "C" uint32_t SolarSystem_getIndex(void *system);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Status_getStation(void *status);
extern "C" uint32_t Station_getTextureIndex(void *station);
extern "C" void Engine_LightSetGlobalSceneColorAmbient(Engine *engine, float r, float g, float b);
extern "C" void Engine_LightSetRimColor(Engine *engine, float r, float g, float b);
extern "C" void Engine_LightSetMaterialColorAmbient(Engine *engine, float r, float g, float b);
extern "C" void Engine_LightSetMaterialColorDiffuse(Engine *engine, float r, float g, float b);
extern "C" void Engine_LightSetMaterialColorSpecular(Engine *engine, float r, float g, float b);
extern "C" void Engine_LightSetMaterialColorShininess(Engine *engine, float s);
extern "C" void Engine_LightSetLightDirection(Engine *engine, float x, float y, float z, int light);
extern "C" void Engine_LightSetLightColorAmbient(Engine *engine, float r, float g, float b, int light);
extern "C" void Engine_LightSetLightColorDiffuse(Engine *engine, float r, float g, float b, int light);
extern "C" void Engine_LightSetLightColorSpecular(Engine *engine, float r, float g, float b, int light);
extern "C" void Engine_LightSetParticleAmbient(Engine *engine, float r, float g, float b);
extern "C" void PaintCanvas_FogEnable(void *canvas, int enabled, int immediate);
extern "C" void PaintCanvas_FogSetParameter(void *canvas, int pname, int mode, float density,
                                             float start, uint32_t color);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transform_id);

static inline uint32_t rgba_scaled(uint32_t color, float scale)
{
    uint32_t r = (uint32_t)((float)((color >> 24) & 0xff) * scale);
    uint32_t g = (uint32_t)((float)((color >> 16) & 0xff) * scale);
    uint32_t b = (uint32_t)((float)((color >> 8) & 0xff) * scale);
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

extern "C" void StarSystem_initLight(StarSystem *self)
{
    void *app = *g_StarSystem_init_app;
    Engine *engine = ApplicationManager_GetEngine(app);
    I(engine, 0x32c) = 2;

    void *status = *g_StarSystem_init_status;
    uint32_t baseIndex;
    bool normalSystem;
    if (B(self, 0x28) == 0) {
        void *system = Status_getSystem(status);
        baseIndex = SolarSystem_getTextureIndex(system) * 3;
        normalSystem = true;
    } else {
        baseIndex = 0x1e;
        normalSystem = false;
    }

    float ambientScale = 0.5f;
    FL(self, 0x00) = g_StarSystem_init_sunColors[baseIndex];
    FL(self, 0x04) = g_StarSystem_init_sunColors[baseIndex + 1];
    FL(self, 0x08) = g_StarSystem_init_sunColors[baseIndex + 2];

    if (normalSystem) {
        void *system = Status_getSystem(status);
        uint32_t tex = SolarSystem_getTextureIndex(system);
        int mission = Status_getCurrentCampaignMission(status);
        if (tex == 0x0f && (mission == 0x59 || mission > 0x9d)) {
            FL(self, 0x00) *= 0.5f;
            FL(self, 0x04) *= 0.5f;
            FL(self, 0x08) *= 0.5f;
        }
        if (B(self, 0x28) == 0) {
            system = Status_getSystem(status);
            tex = SolarSystem_getTextureIndex(system);
            ambientScale = 0.5f;
            mission = Status_getCurrentCampaignMission(status);
            if (tex == 0x0f && mission != 0x59) {
                ambientScale = mission < 0x9e ? 0.35f : 0.6f;
            }
        }
    }

    uint32_t stationColorIndex = 0x17;
    if (normalSystem && B(self, 0x28) == 0) {
        void *station = Status_getStation(status);
        stationColorIndex = Station_getTextureIndex(station) * 3;
    }

    float lr = g_StarSystem_init_stationColors[stationColorIndex];
    float lg = g_StarSystem_init_stationColors[stationColorIndex + 1];
    float lb = g_StarSystem_init_stationColors[stationColorIndex + 2];
    if (normalSystem && B(self, 0x28) == 0) {
        void *system = Status_getSystem(status);
        if (SolarSystem_getTextureIndex(system) == 0x0f &&
            Status_getCurrentCampaignMission(status) != 0x59) {
            if (Status_getCurrentCampaignMission(status) < 0x9e) {
                lr = lr;
            }
        }
    }

    EngineGetter getEngine = g_StarSystem_init_getEngine;
    engine = getEngine(app);
    Engine_LightSetGlobalSceneColorAmbient(engine, ambientScale * FL(self, 0x00),
                                           ambientScale * FL(self, 0x04),
                                           ambientScale * FL(self, 0x08));
    engine = getEngine(app);
    Engine_LightSetRimColor(engine, g_StarSystem_init_lightColors[baseIndex] * 3.0f,
                            g_StarSystem_init_lightColors[baseIndex + 1] * 3.0f,
                            g_StarSystem_init_lightColors[baseIndex + 2] * 3.0f);
    Engine_LightSetMaterialColorAmbient(getEngine(app), 1.0f, 1.0f, 1.0f);
    Engine_LightSetMaterialColorDiffuse(getEngine(app), 1.0f, 1.0f, 1.0f);
    Engine_LightSetMaterialColorSpecular(getEngine(app), 1.0f, 1.0f, 1.0f);
    Engine_LightSetMaterialColorShininess(getEngine(app), 0.7f);
    Engine_LightSetLightDirection(getEngine(app), FL(self, 0x30), FL(self, 0x34), FL(self, 0x38), 0x4000);
    Engine_LightSetLightColorAmbient(getEngine(app), 0.0f, 0.0f, 0.0f, 0x4000);
    Engine_LightSetLightColorDiffuse(getEngine(app), FL(self, 0x00), FL(self, 0x04), FL(self, 0x08), 0x4000);
    Engine_LightSetLightColorSpecular(getEngine(app), 2.0f, 2.0f, 2.0f, 0x4000);
    Engine_LightSetLightDirection(getEngine(app), 0.0f, 0.0f, 1.0f, 0x4001);
    Engine_LightSetLightColorDiffuse(getEngine(app), lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    Engine_LightSetLightColorSpecular(getEngine(app), lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    Engine_LightSetParticleAmbient(getEngine(app), FL(self, 0x00), FL(self, 0x04), FL(self, 0x08));

    void *canvas = *g_StarSystem_init_canvas;
    PaintCanvas_FogEnable(canvas, 0, 1);
    B(self, 0x54) = 0;
    if (B(self, 0x28) != 0) {
        return;
    }

    uint32_t tex = SolarSystem_getTextureIndex(Status_getSystem(status));
    uint32_t fogColor = 0;
    switch (tex) {
    case 0x0b:
        fogColor = 0x808080ff;
        break;
    case 0x0c:
        fogColor = 0xa0a0a0ff;
        break;
    case 0x0f:
        if (Status_getCurrentCampaignMission(status) == 0x59) {
            return;
        }
        fogColor = 0x707070ff;
        PaintCanvas_FogSetParameter(canvas, 0x2601, 0, 0.0f, 1.0f, fogColor);
        PaintCanvas_FogEnable(canvas, 1, 1);
        B(self, 0x54) = 1;
        for (uint32_t i = 1; i < array_len(P(self, 0x1c)); ++i) {
            AEGeometry **geoms = (AEGeometry **)array_data(P(self, 0x1c));
            void *transform = PaintCanvas_TransformGetTransform(canvas, I(geoms[i], 0x0c));
            U(transform, 0x48) = fogColor;
        }
        return;
    case 0x10:
        fogColor = 0x909090ff;
        break;
    case 0x11:
        fogColor = 0x707090ff;
        break;
    case 0x12:
        fogColor = 0x909070ff;
        break;
    default:
        return;
    }

    PaintCanvas_FogSetParameter(canvas, 0x2601, 0, 0.0f, 1.0f, fogColor);
    PaintCanvas_FogEnable(canvas, 1, 1);
    B(self, 0x54) = 1;
    uint32_t scaled = rgba_scaled(fogColor, 0.65f);
    for (uint32_t i = 1; i < array_len(P(self, 0x1c)); ++i) {
        AEGeometry **geoms = (AEGeometry **)array_data(P(self, 0x1c));
        void *transform = PaintCanvas_TransformGetTransform(canvas, I(geoms[i], 0x0c));
        U(transform, 0x48) = scaled;
    }
}
