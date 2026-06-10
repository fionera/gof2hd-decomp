#include "gof2/StarMap.h"
#include "gof2/Achievements.h"
#include "gof2/ApplicationManager.h"
#include "gof2/Engine.h"
#include "gof2/GameText.h"
#include "gof2/Layout.h"
#include "gof2/Mission.h"
#include "gof2/SolarSystem.h"
#include "gof2/Station.h"
#include "gof2/Status.h"
#include "gof2/String.h"
#include "gof2/TouchButton.h"

// Engine helpers that are invoked with more than one argument type in this translation
// unit (the canvas handle appears both as a raw uint32_t id and as a void* in different
// call paths). Declared as ordinary (overloadable) C++ functions rather than extern "C"
// so both forms resolve; the engine provides the real implementations at link time.
void *PaintCanvas_CameraGetCurrent(void *canvas);
void *PaintCanvas_CameraGetCurrent(uint32_t canvas);
void  PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
void  PaintCanvas_CameraSetLocal(uint32_t canvas, void *matrix);
void  PaintCanvas_CameraSetCurrent(void *canvas, uint32_t camera);
void  PaintCanvas_CameraSetCurrent(uint32_t canvas, uint32_t camera);
void *PaintCanvas_TransformGetTransform(void *canvas);
void *PaintCanvas_TransformGetTransform(uint32_t canvas);
void  AEGeometry_setPosition(void *geometry, Vector *pos);
void  AEGeometry_setPosition(void *geometry, float x, float y, float z);
void *SystemPathFinder_getSystemPath(void *finder, void *systems, int from, int to);


extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_render_canvas;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_render_geometry)(void *);
extern "C" void PaintCanvas_SetColor(uint32_t canvas, uint32_t color);
extern "C" void PaintCanvas_End3d(uint32_t canvas);
extern "C" void PaintCanvas_Begin3d(uint32_t canvas);
extern "C" void PaintCanvas_SetTexture(uint32_t canvas, uint32_t texture, uint32_t color);
extern "C" void PaintCanvas_SetBlendMode(uint32_t canvas, int mode);
extern "C" void AEGeometry_render(void *geometry);
extern "C" void StarMap_render_tail();
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_alien_text;
extern "C" void ChoiceWindow_set(void *choiceWindow, String *text, bool enabled);
extern "C" void *Array_int_dtor(void *arr);
extern "C" void ArrayReleaseClasses_Vector(void *arr);
extern "C" void *Array_Vector_dtor(void *arr);
extern "C" void *AEGeometry_dtor(void *geometry);
extern "C" void *SystemPathFinder_dtor(void *finder);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_draw_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_text;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_draw_font;
extern "C" float EaseInOut_GetValue(void *ease);
extern "C" float EaseInOut_GetMinValue(void *ease);
extern "C" float EaseInOut_GetMaxValue(void *ease);
extern "C" void PaintCanvas_SetColorRGBA(uint32_t canvas, int r, int g, int b, int a);
extern "C" void PaintCanvas_DrawLine(uint32_t canvas, int x0, int y0, int x1, int y1);
extern "C" void PaintCanvas_DrawImage2D(uint32_t canvas, uint32_t image, int x, int y, int anchor);
extern "C" void PaintCanvas_DrawString(uint32_t canvas, void *font, String *text, int x, int y, bool flag);
extern "C" int PaintCanvas_GetImage2DWidth(uint32_t canvas, uint32_t image);
extern "C" void Layout_drawHeader(void *layout, String *text);
extern "C" void ChoiceWindow_draw(void *choice);
extern "C" void String_add(String *out, String *a, String *b);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void *Status_getSystem(void *status);
extern "C" long long Status_getPlayingTime(void *status);
extern "C" void *Status_getShip(void *status);
extern "C" int Ship_hasJumpDriveIntegrated(void *ship);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" void *SolarSystem_getRoutes(void *system);
extern "C" int SolarSystem_getRace(void *system);
extern "C" int SolarSystem_getSecurityLevel(void *system);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_status;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_store0_a;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_flag_a;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_jumpFlag_a;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_jumpCost_a;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_targetStation;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_flag_b;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_status2;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_jumpFlag_b;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_jumpCost_b;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_achievements;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_sound;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_modstation_flag;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_canvas;
extern "C" void Level_setInitStreamOut();
extern "C" int Status_jumpgateUsed(void *status);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Status_getStation(void *status);
extern "C" int Ship_hasVolatileGoods(void *ship);
extern "C" int Station_getSystem(void *station);
extern "C" void ArrayReleaseClasses_Station(void *arr);
extern "C" void *Array_Station_dtor(void *arr);
extern "C" void FModSound_stop(void *sound, int id);
extern "C" void StarMap_depart_tail(void *canvas, int mode);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_sound;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_end_text;
extern "C" int ChoiceWindow_OnTouchEnd(void *window, int x, int y);
extern "C" void FModSound_play(void *sound, int id, void *pos, float volume);
extern "C" void ChoiceWindow_set(void *window, String *text, bool yesNo);
extern "C" void *ChoiceWindow_ctor(void *window);
extern "C" int Station_getIndex(void *station);
extern "C" void AEGeometry_getPosition(Vector *out, void *geometry);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void MatrixGetPosition(Vector *out, void *matrix);
extern "C" void EaseInOut_SetRange(void *ease, float from, float to);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_lights_engine;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_lights_canvas;
extern "C" void PaintCanvas_LightEnable(void *canvas, int light, bool enabled);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_update_sound;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_update_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_update_screenH;
extern "C" void *AEGeometry_getMatrix(void *geometry);
extern "C" void AEGeometry_setScaling(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setDirection(void *geometry, Vector *dir);
extern "C" void AEGeometry_setRotation(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_rotate(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setVisible(void *geometry, bool visible);
extern "C" int PaintCanvas_GetScreenPosition(void *canvas, Vector *world, Vector *screen);
extern "C" void Transform_Update(void *transform, int dt);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void Vector_sub_assign(Vector *dst, Vector *src);
extern "C" void VectorNormalize(Vector *out, Vector *value);
extern "C" void EaseInOut_Update(void *ease, float dt);
extern "C" float EaseInOut_GetCurrentValue(void *ease);
extern "C" void FModSound_setParamValue(void *sound, int event, int id, float value);
extern "C" void ArrayReleaseClasses_AEGeometry(void *arr);
extern "C" void *Array_AEGeometry_dtor(void *arr);
extern "C" void ArrayRelease_bool(void *arr);
extern "C" void *Array_bool_dtor(void *arr);
extern "C" uint8_t *Status_getSystemVisibilities(void *status);
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_ctor_vecCtor)(void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_galaxy;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_ctor_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_random;
extern "C" void *Galaxy_getSystems(void *galaxy);
extern "C" void *AEGeometry_ctor_canvas(void *geometry, uint32_t canvas);
extern "C" void *AEGeometry_ctor_image(void *geometry, uint16_t image, uint32_t canvas, bool flag);
extern "C" void AEGeometry_addChild(void *geometry, uint32_t child);
extern "C" void Array_AEGeometry_ctor(void *arr);
extern "C" void Array_Vector_ctor(void *arr);
extern "C" void ArraySetLength_AEGeometry(uint32_t n, void *arr);
extern "C" void ArraySetLength_Vector(uint32_t n, void *arr);
extern "C" void AERandom_reset(void *random);
extern "C" int SolarSystem_getTextureIndex(void *system);
extern "C" int SolarSystem_getX(void *system);
extern "C" int SolarSystem_getY(void *system);
extern "C" int SolarSystem_getZ(void *system);
extern "C" void Transform_SetAnimationState(void *transform, int state, int frame);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_touch_screenH;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_sound;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_status;
extern "C" void ChoiceWindow_OnTouchBegin(void *window, int x, int y);
extern "C" int Status_hardCoreMode(void *status);
extern "C" int SystemPathFinder_getJumpDistance(void *finder, void *systems, int from, int to);
extern "C" void Array_Station_ctor(void *arr);
extern "C" void FileRead_ctor(void *reader);
extern "C" void *FileRead_dtor(void *reader);
extern "C" void *FileRead_loadStationsBinary(void *reader, void *system);
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_move_guard;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_move_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_move_canvas;
extern "C" void ChoiceWindow_OnTouchMove(void *window, int x, int y);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_drawKey_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_drawKey_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_drawKey_screenH;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_drawKey_drawImage)(uint32_t, uint32_t, int, int);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_text;
extern "C" __attribute__((visibility("hidden"))) String *(*g_StarMap_drawKey_getText)(void *, int);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_drawKey_font;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_drawKey_drawString)(uint32_t, void *, String *, int, int, bool);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_system_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_random;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_engine;
extern "C" void *SolarSystem_getStations(void *system);
extern "C" int Station_getTextureIndex(void *station);
extern "C" void Array_bool_ctor(void *arr);
extern "C" void ArraySetLength_Station(uint32_t n, void *arr);
extern "C" void ArraySetLength_bool(uint32_t n, void *arr);
extern "C" void AEGeometry_translate(void *geometry, Vector *pos);
extern "C" int AERandom_nextInt(void *random, int bound);
extern "C" void AERandom_setSeed(void *random, long long seed);
extern "C" void PaintCanvas_Image2DCreate(uint32_t canvas, uint16_t image, void *out);
extern "C" void PaintCanvas_TextureCreate(uint32_t canvas, uint16_t image, void *out, bool flag);
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_info_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_info_screenH;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_info_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_font;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_text;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_info_isGerman;
extern "C" int PaintCanvas_GetTextWidth(uint32_t canvas, String *font);
extern "C" int Station_getTecLevel(void *station);
extern "C" int SolarSystem_getWarpGateIndex(void *system);
extern "C" void *Status_getCampaignMission(void *status);
extern "C" void *Status_getFreelanceMission(void *status);
extern "C" void *Status_getPendingProducts(void *status);
extern "C" int Status_isFreighterMissionStation(void *status, int station);
extern "C" int Status_getFreighterMissionStationBit(void *status, int station);
extern "C" int Mission_getType(void *mission);
extern "C" int Mission_getTargetStation(void *mission);
extern "C" int Mission_getStatusValue(void *mission);
extern "C" void *Mission_getAgent(void *mission);
extern "C" int Agent_getStation(void *agent);
extern "C" int Ship_hasCargo(void *ship, int cargo);
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_init_canvas;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_init_imageCreate)(uint32_t, int, void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_text;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_init_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_init_screenH;
extern "C" void PaintCanvas_FogEnable(uint32_t canvas, int enabled, bool immediate);
extern "C" void PaintCanvas_CameraCreate(uint32_t canvas, void *out);
extern "C" void PaintCanvas_CameraSetPerspective(uint32_t canvas, float fov, float nearPlane, float farPlane, float aspect);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z, float w);
extern "C" void *EaseInOut_ctor(void *ease);
extern "C" void *TouchButton_ctor(void *button, String *text, int a, int x, int y, int size);
extern "C" void *SystemPathFinder_ctor(void *finder);
extern "C" int Ship_getCargo(void *ship);
extern "C" int Item_getAmount(void *item);

// ---- missionChanged_c9b48.cpp ----
uint8_t StarMap::missionChanged()
{
    return field<uint8_t>(this, 0xdc);
}

// ---- render_c8058.cpp ----
void StarMap::render()
{
    uint32_t *canvasHolder = g_StarMap_render_canvas;
    PaintCanvas_SetColor(*canvasHolder, 0xffffffffu);
    void (*renderGeometry)(void *) = g_StarMap_render_geometry;
    renderGeometry(ptr_field(this, 0x1b0));
    renderGeometry(ptr_field(this, 0x1b4));
    renderGeometry(ptr_field(this, 0x1b8));
    renderGeometry(ptr_field(this, 0x6c));
    PaintCanvas_End3d(*canvasHolder);
    PaintCanvas_Begin3d(*canvasHolder);
    if (ptr_field(this, 0xa4) != 0) {
        PaintCanvas_SetTexture(*canvasHolder, field<uint32_t>(this, 0x178), 0xffffffffu);
        PaintCanvas_SetBlendMode(*canvasHolder, 2);
        AEGeometry_render(ptr_field(this, 0x1bc));
        AEGeometry_render(ptr_field(this, 0xa4));
    }
    if (ptr_field(this, 0xf8) != 0) {
        return StarMap_render_tail();
    }
}

// ---- isInPlanetMode_c8040.cpp ----
bool StarMap::isInPlanetMode()
{
    return field<int32_t>(this, 4) == 3;
}

// ---- askForJumpIntoAlienWorld_c9b50.cpp ----
void StarMap::askForJumpIntoAlienWorld()
{
    void *window = *(void *volatile *)((char *)this + 0x5c);
    field<uint8_t>(this, 0x120) = 1;
    String *text = ((GameText *)(*g_StarMap_alien_text))->getText(0x1a6);
    ChoiceWindow_set(window, text, true);
    field<uint8_t>(this, 0xa9) = 1;
}

// ---- setJumpMapMode_c804c.cpp ----
void StarMap::setJumpMapMode(bool enabled, bool value)
{
    volatile uint8_t *bytes = (volatile uint8_t *)this;
    bytes[0xab] = (uint8_t)value;
    bytes[0xaa] = (uint8_t)enabled;
}

// ---- setStart_c7dd8.cpp ----
struct SystemPathFinder;


void StarMap::setStart(int start, int target)
{
    field<int32_t>(this, 0x1dc) = target;
    field<int32_t>(this, 0x1e0) = start;
    void *path = ptr_field(this, 0xa0);
    if (path != 0) {
        operator delete(Array_int_dtor(path));
    }
    ptr_field(this, 0xa0) = 0;
    ptr_field(this, 0xa0) =
        SystemPathFinder_getSystemPath((SystemPathFinder *)ptr_field(this, 0x50), ptr_field(this, 0x54),
                                       start, field<int32_t>(this, 0x104));
}

// ---- _StarMap_c749c.cpp ----
StarMap::~StarMap()
{
    void *p;

    p = ptr_field(this, 0x194);
    if (p != 0) {
        ArrayReleaseClasses_Vector(p);
        p = ptr_field(this, 0x194);
        if (p != 0) {
            operator delete(Array_Vector_dtor(p));
        }
    }
    ptr_field(this, 0x194) = 0;

    p = ptr_field(this, 0x198);
    if (p != 0) {
        ArrayReleaseClasses_Vector(p);
        p = ptr_field(this, 0x198);
        if (p != 0) {
            operator delete(Array_Vector_dtor(p));
        }
    }
    ptr_field(this, 0x198) = 0;

    p = ptr_field(this, 0x1bc);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1bc) = 0;

    p = ptr_field(this, 0x1b0);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1b0) = 0;

    p = ptr_field(this, 0x1b4);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1b4) = 0;

    p = ptr_field(this, 0x1b8);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0x1b8) = 0;

    p = ptr_field(this, 0x17c);
    if (p != 0) {
        operator delete(p);
    }
    ptr_field(this, 0x17c) = 0;

    p = ptr_field(this, 0x180);
    if (p != 0) {
        operator delete(p);
    }
    ptr_field(this, 0x180) = 0;

    p = ptr_field(this, 0x184);
    if (p != 0) {
        operator delete(p);
    }
    ptr_field(this, 0x184) = 0;

    p = ptr_field(this, 0xf8);
    if (p != 0) {
        operator delete(AEGeometry_dtor(p));
    }
    ptr_field(this, 0xf8) = 0;

    p = ptr_field(this, 0x50);
    if (p != 0) {
        operator delete(SystemPathFinder_dtor(p));
    }
    ptr_field(this, 0x50) = 0;
}

// ---- draw_c90f4.cpp ----
void StarMap::draw()
{
    String tmp;

    int mode = field<int32_t>(this, 4);
    int alpha = mode == 0 ? 0xff : 0;
    field<int32_t>(this, 0x1a4) = alpha;
    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        float v = EaseInOut_GetValue(ptr_field(this, 0x184));
        float min = EaseInOut_GetMinValue(ptr_field(this, 0x184));
        float max = EaseInOut_GetMaxValue(ptr_field(this, 0x184));
        float t = (v - min) / (max - min);
        if (field<uint8_t>(this, 0x138) != 0) {
            t = 1.0f - t;
        }
        field<int32_t>(this, 0x1a4) = (int)(t * 255.0f);
    }

    uint32_t canvas = *g_StarMap_draw_canvas;
    if (mode != 3 || field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
        Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x194);
        for (uint32_t i = 0; i < positions->size(); i++) {
            Vector_assign((Vector *)((char *)this + 0x78), positions->data()[i]);
            void *routes = SolarSystem_getRoutes(((Array<void *> *)ptr_field(this, 0x54))->data()[i]);
            if (routes != 0) {
                Array<int> *r = (Array<int> *)routes;
                for (uint32_t j = 0; j < r->size(); j++) {
                    uint32_t to = (uint32_t)r->data()[j];
                    if (to < positions->size()) {
                        Vector_assign((Vector *)((char *)this + 0x84), positions->data()[to]);
                        if (field<float>(this, 0x80) >= 0.0f || field<float>(this, 0x8c) >= 0.0f) {
                            PaintCanvas_DrawLine(canvas, (int)field<float>(this, 0x78),
                                                 (int)field<float>(this, 0x7c),
                                                 (int)field<float>(this, 0x84),
                                                 (int)field<float>(this, 0x88));
                        }
                    }
                }
            }
        }
        for (uint32_t i = 0; i < ((Array<void *> *)ptr_field(this, 0x54))->size(); i++) {
            drawOnScreenInfo((int)i, false);
        }
    }

    if (field<int32_t>(this, 0x60) >= 0 &&
        (field<uint8_t>(this, 0x118) == 0 || field<int32_t>(this, 0x11c) > 3999)) {
        drawOnScreenInfo(field<int32_t>(this, 0x60), false);
    }

    if (ptr_field(this, 0xa4) != 0 && ptr_field(this, 0x58) != 0) {
        void *system = ((Array<void *> *)ptr_field(this, 0x54))->data()[field<int32_t>(this, 0x60)];
        if (((SolarSystem *)(system))->hasNoOwner() == 0) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4) ^ 0xff);
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x34),
                                    field<int32_t>(*g_StarMap_draw_layout, 0x2c),
                                    field<int32_t>(*g_StarMap_draw_layout, 0xc) +
                                        field<int32_t>(*g_StarMap_draw_layout, 0x2c),
                                    0);
            ((SolarSystem *)(&tmp))->getName();
            PaintCanvas_DrawString(canvas, *g_StarMap_draw_font, &tmp,
                                   PaintCanvas_GetImage2DWidth(canvas, field<uint32_t>(this, 0x34)) +
                                       field<int32_t>(*g_StarMap_draw_layout, 0x2c) * 2,
                                   field<int32_t>(*g_StarMap_draw_layout, 0xc) +
                                       field<int32_t>(*g_StarMap_draw_layout, 0x2c) + 2,
                                   false);
            ((String *)(&tmp))->dtor();
        }
        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        for (uint32_t i = 0; i < stations->size(); i++) {
            if (i != (uint32_t)field<int32_t>(this, 0x64)) {
                drawOnScreenInfo((int)i, true);
            }
        }
        if (field<int32_t>(this, 0x64) >= 0) {
            drawOnScreenInfo(field<int32_t>(this, 0x64), true);
        }
    }

    if (field<uint8_t>(this, 0x108) != 0) {
        drawKey();
    }
    ((String *)(&tmp))->ctor_copy(((GameText *)(*g_StarMap_draw_text))->getText(0x190), false);
    Layout_drawHeader(*g_StarMap_draw_layout, &tmp);
    ((String *)(&tmp))->dtor();
    ((Layout *)(*g_StarMap_draw_layout))->drawEmptyFooter();
    ((TouchButton *)(ptr_field(this, 0x4c)))->draw();
    if (field<uint8_t>(this, 0xa9) != 0) {
        ChoiceWindow_draw(ptr_field(this, 0x5c));
    }
}

// ---- depart_c7e0c.cpp ----
void StarMap::depart(bool jump)
{
    int selected = field<int32_t>(this, 0x64);
    if (selected < 0) {
        return;
    }

    void **statusHolder = g_StarMap_depart_status;
    void *status = *statusHolder;
    if (field<uint8_t>(this, 0xaa) != 0) {
        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        ((Status *)(status))->departStation(stations->data()[selected]);
        *g_StarMap_depart_store0_a = 0;
        Level_setInitStreamOut();
        int used = Status_jumpgateUsed(status);
        if (jump) {
            used = field<uint8_t>(this, 0xab);
        }
        if (jump && used != 0) {
            int toSystem = Station_getSystem(stations->data()[selected]);
            int current = SolarSystem_getIndex(Status_getSystem(status));
            *g_StarMap_depart_jumpFlag_a = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                *g_StarMap_depart_jumpCost_a = field<int32_t>(this, 0x1d0);
            }
        } else {
            *g_StarMap_depart_flag_a = 0;
        }
    } else {
        if (Status_getCurrentCampaignMission(status) == 3) {
            goto cleanup;
        }

        int *statusWords = (int *)status;
        statusWords[0x5c / 4] = -1;
        statusWords[0x60 / 4] = -1;
        statusWords[0x64 / 4] = -1;
        statusWords[0x68 / 4] = -1;
        ((Status *)(status))->departStation(Status_getStation(status));

        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        void *target = stations->data()[field<int32_t>(this, 0x64)];
        if (((Station *)(target))->equals(Status_getStation(status)) == 0) {
            *g_StarMap_depart_targetStation = (int)(intptr_t)target;
        }

        if (jump) {
            void *ship = Status_getShip(status);
            if (Ship_hasVolatileGoods(ship) != 0) {
                goto no_jump;
            }
            if (Ship_hasJumpDriveIntegrated(Status_getShip(status)) == 0 && field<uint8_t>(this, 0xab) == 0) {
                goto no_jump;
            }
            int toSystem = Station_getSystem(*g_StarMap_depart_status2);
            int current = SolarSystem_getIndex(Status_getSystem(status));
            *g_StarMap_depart_jumpFlag_b = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                *g_StarMap_depart_jumpCost_b = field<int32_t>(this, 0x1d0);
            }
        } else {
no_jump:
            *g_StarMap_depart_flag_b = 0;
        }
        ((Achievements *)(*g_StarMap_depart_achievements))->resetNewMedals();
    }

cleanup:
    {
        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        for (uint32_t i = 0; i < stations->size(); i++) {
            if (i != (uint32_t)field<int32_t>(this, 0x64)) {
                void *station = stations->data()[i];
                if (station != 0) {
                    operator delete(((Station *)(station))->dtor());
                }
                stations->data()[i] = 0;
            }
        }
        if (stations != 0) {
            operator delete(Array_Station_dtor(stations));
        }
        ptr_field(this, 0x58) = 0;
    }
    FModSound_stop(*g_StarMap_depart_sound, 0x66);
    *g_StarMap_depart_modstation_flag = 1;
    return StarMap_depart_tail(*g_StarMap_depart_canvas, 2);
}

// ---- OnTouchEnd_cae90.cpp ----
static inline float absf_end(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::OnTouchEnd(int x, int y)
{
    String help;

    if (field<uint8_t>(this, 0xa9) != 0) {
        int result = ChoiceWindow_OnTouchEnd(ptr_field(this, 0x5c), x, y);
        if (result == 1) {
            field<uint8_t>(this, 0xa9) = 0;
            field<uint8_t>(this, 0x120) = 0;
            return;
        }
        if (result != 0) {
            return;
        }
        field<uint8_t>(this, 0xa9) = 0;
        if (field<uint8_t>(this, 0x1e4) != 0) {
            field<uint8_t>(this, 0x1e4) = 0;
            return;
        }
        if (field<int32_t>(this, 4) == 3) {
            void *station = ((Array<void *> *)ptr_field(this, 0x58))->data()[field<int32_t>(this, 0x64)];
            if (Station_getIndex(station) == Station_getIndex(Status_getStation(*g_StarMap_end_status))) {
                field<uint8_t>(this, 0x1e4) = 0;
                return;
            }
        }
        if (field<uint8_t>(this, 0x120) != 0 && field<int32_t>(this, 4) == 0 &&
            field<uint8_t>(this, 0xa8) == 0 && field<uint8_t>(this, 0x118) == 0) {
            field<uint8_t>(this, 0x120) = 0;
            field<uint8_t>(this, 0) = 1;
            PaintCanvas_CameraSetCurrent(*g_StarMap_end_canvas, field<uint32_t>(this, 0x74));
            return;
        }
        if (field<uint8_t>(this, 0xa8) == 0 && field<int32_t>(this, 4) == 3) {
            if (field<uint8_t>(this, 0xab) == 0 ||
                field<int32_t>(this, 0x1d0) <= field<int32_t>(this, 0x1d8)) {
                if (field<uint8_t>(this, 0xaa) == 0) {
                    depart(true);
                    return;
                }
            } else if (field<int32_t>(this, 0x1d0) == 1 && field<uint8_t>(this, 0xaa) == 0) {
                depart(false);
                return;
            }
            field<uint8_t>(this, 0) = 1;
            PaintCanvas_CameraSetCurrent(*g_StarMap_end_canvas, field<uint32_t>(this, 0x74));
        }
        return;
    }

    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        return;
    }
    void *layout = *g_StarMap_end_layout;
    if (*(uint8_t *)layout == 0 && ((Layout *)(layout))->OnTouchEnd(x, y) != 0) {
        if (field<int32_t>(this, 4) == 3 && field<uint8_t>(this, 0xf4) != 0) {
            field<uint8_t>(this, 0x139) = 1;
            field<int32_t>(this, 0x168) = 0;
            field<int32_t>(this, 0x16c) = 0;
            field<int32_t>(this, 0x170) = 0;
            EaseInOut_SetRange(ptr_field(this, 0x17c), field<float>(this, 0x78), field<float>(this, 0x78));
            EaseInOut_SetRange(ptr_field(this, 0x180), field<float>(this, 0x7c), field<float>(this, 0x7c));
            EaseInOut_SetRange(ptr_field(this, 0x184), field<float>(this, 0x80), field<float>(this, 0x80));
            FModSound_play(*g_StarMap_end_sound, 0x6b, 0, 0.0f);
        } else {
            PaintCanvas_CameraSetCurrent(*g_StarMap_end_canvas, field<uint32_t>(this, 0x74));
            FModSound_stop(*g_StarMap_end_sound, 0x66);
        }
        return;
    }
    if (field<uint8_t>(this, 0xa8) != 0 && field<uint8_t>(this, 0x13a) != 0) {
        return;
    }
    if (((TouchButton *)(ptr_field(this, 0x4c)))->OnTouchEnd(x, y) != 0) {
        field<uint8_t>(this, 0x108) ^= 1;
    }
    if (field<uint8_t>(this, 0x174) != 0) {
        float dx = field<float>(this, 0x150);
        float dy = field<float>(this, 0x154);
        field<int32_t>(this, 0x168) = 0x3f666666;
        field<uint8_t>(this, 0x174) = 0;
        field<float>(this, 0x16c) = absf_end(dx) > 3.0f ? dx : 0.0f;
        field<float>(this, 0x170) = absf_end(dy) > 3.0f ? dy : 0.0f;
        if (field<int32_t>(this, 4) == 0) {
            if (field<int32_t>(this, 0x60) >= 0 &&
                field<uint8_t>(this, 0x118) == 0 &&
                field<uint8_t>(this, 0xa8) == 0 &&
                field<int32_t>(this, 0x19c) == field<int32_t>(this, 0x60)) {
                if (field<uint8_t>(this, 0xab) == 0 &&
                    ((SolarSystem *)(Status_getSystem(*g_StarMap_end_status)))->systemIsInSystemRoutes(SolarSystem_getIndex(Status_getSystem(*g_StarMap_end_status))) == 0) {
                    ChoiceWindow_set(ptr_field(this, 0x5c), ((GameText *)(*g_StarMap_end_text))->getText(0x1a4), false);
                    field<uint8_t>(this, 0xa9) = 1;
                    return;
                }
                FModSound_play(*g_StarMap_end_sound, 0x6a, 0, 0.0f);
                initStarSystem();
                field<uint8_t>(this, 0x138) = 1;
                Vector p;
                AEGeometry_getPosition(&p, ptr_field(this, 0xa4));
                Vector_assign((Vector *)((char *)this + 0x78), &p);
                field<float>(this, 0x80) += 20.0f;
                MatrixGetPosition(&p, PaintCanvas_CameraGetLocal(*g_StarMap_end_canvas));
                Vector_assign((Vector *)((char *)this + 0x84), &p);
                EaseInOut_SetRange(ptr_field(this, 0x17c), field<float>(this, 0x84), field<float>(this, 0x78));
                EaseInOut_SetRange(ptr_field(this, 0x180), field<float>(this, 0x88), field<float>(this, 0x7c));
                EaseInOut_SetRange(ptr_field(this, 0x184), field<float>(this, 0x8c), field<float>(this, 0x80));
                field<int32_t>(this, 0x168) = 0;
                field<int32_t>(this, 0x16c) = 0;
                field<int32_t>(this, 0x170) = 0;
            } else if (field<int32_t>(this, 0x60) >= 0) {
                field<int32_t>(this, 0x19c) = -1;
                field<uint8_t>(this, 0x13a) = 1;
            }
        } else if (field<int32_t>(this, 0x64) >= 0) {
            if (field<int32_t>(this, 0x1a0) == field<int32_t>(this, 0x64)) {
                if (ptr_field(this, 0x5c) == 0) {
                    ptr_field(this, 0x5c) = ChoiceWindow_ctor(operator new(0x5c));
                }
                ChoiceWindow_set(ptr_field(this, 0x5c), ((GameText *)(*g_StarMap_end_text))->getText(0x1a3), true);
                field<uint8_t>(this, 0xa9) = 1;
            } else {
                FModSound_play(*g_StarMap_end_sound, 0x69, 0, 0.0f);
                field<uint8_t>(this, 0x13b) = 1;
            }
        }
    }
    if (((Layout *)(layout))->helpPressed() != 0) {
        ((String *)(&help))->ctor_copy(((GameText *)(*g_StarMap_end_text))->getText(0x1a5), false);
        ((Layout *)(layout))->initHelpWindow(&help);
        ((String *)(&help))->dtor();
    }
}

// ---- initLights_c7d9c.cpp ----
void StarMap::initLights()
{
    void *engine = ((ApplicationManager *)(*g_StarMap_lights_engine))->GetEngine();
    ((Engine *)(engine))->LightSetMaterialColorAmbient(0.5f, 0.5f, 0.5f);
    return PaintCanvas_LightEnable(*g_StarMap_lights_canvas, 0, true);
}

// ---- update_c9b90.cpp ----
static inline float absf_update(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::update(int dt)
{
    Matrix matrix;
    Vector tmp;

    field<int32_t>(this, 0x18) = dt;
    if (field<int32_t>(this, 4) == 0 || field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x194);
        for (uint32_t i = 0; i < positions->size(); i++) {
            AEGeometry_getPosition(&tmp, ((Array<void *> *)ptr_field(this, 0x68))->data()[i]);
            int visible = PaintCanvas_GetScreenPosition(*g_StarMap_update_canvas, &tmp, positions->data()[i]);
            positions->data()[i]->z = visible != 0 ? 1.0f : -1.0f;
        }
    }
    if (field<int32_t>(this, 4) == 3 || field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x198);
        if (positions != 0) {
            for (uint32_t i = 0; i < positions->size(); i++) {
                AEGeometry_getPosition(&tmp, ((Array<void *> *)ptr_field(this, 0x90))->data()[i + 1]);
                int visible = PaintCanvas_GetScreenPosition(*g_StarMap_update_canvas, &tmp, positions->data()[i]);
                positions->data()[i]->z = visible != 0 ? 1.0f : -1.0f;
            }
        }
    }

    if (ptr_field(this, 0xf8) != 0) {
        float v = EaseInOut_GetValue(ptr_field(this, 0x184));
        float min = EaseInOut_GetMinValue(ptr_field(this, 0x184));
        float max = EaseInOut_GetMaxValue(ptr_field(this, 0x184));
        float t = (v - min) / (max - min);
        if (field<uint8_t>(this, 0x139) != 0) {
            t = 1.0f - t;
        } else if (field<uint8_t>(this, 0x138) == 0 && field<int32_t>(this, 4) == 0) {
            t = 1.0f;
        }
        float scale = (float)(0.5 + (double)t * 0.5);
        AEGeometry_setScaling(ptr_field(this, 0xf8), scale, scale, scale);
        if (ptr_field(this, 0xa4) != 0 && field<int32_t>(this, 0x1c4) >= 0) {
            AEGeometry_getPosition(&tmp,
                                   ((Array<void *> *)ptr_field(this, 0x68))->data()[field<int32_t>(*g_StarMap_update_status, 0x7c)]);
            AEGeometry_setPosition(ptr_field(this, 0xf8), &tmp);
        }
        void *canvas = *g_StarMap_update_canvas;
        PaintCanvas_CameraGetCurrent(canvas);
        MatrixGetPosition(&tmp, PaintCanvas_CameraGetLocal(canvas));
        Vector_assign((Vector *)((char *)this + 0x78), &tmp);
        AEGeometry_getPosition(&tmp, ptr_field(this, 0xf8));
        Vector_sub_assign((Vector *)((char *)this + 0x78), &tmp);
        VectorNormalize(&tmp, (Vector *)((char *)this + 0x78));
        Vector_assign((Vector *)((char *)this + 0x78), &tmp);
        field<float>(this, 0x78) += 0.5f;
        AEGeometry_setDirection(ptr_field(this, 0xf8), (Vector *)((char *)this + 0x78));
        Transform_Update(PaintCanvas_TransformGetTransform(canvas), dt);
    }

    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        float step = (float)(dt * 15);
        EaseInOut_Update(ptr_field(this, 0x17c), step);
        EaseInOut_Update(ptr_field(this, 0x180), step);
        EaseInOut_Update(ptr_field(this, 0x184), step);
        tmp.x = EaseInOut_GetCurrentValue(ptr_field(this, 0x17c));
        tmp.y = EaseInOut_GetCurrentValue(ptr_field(this, 0x180));
        tmp.z = EaseInOut_GetCurrentValue(ptr_field(this, 0x184));
        Vector_assign((Vector *)((char *)this + 0x78), &tmp);
        void *canvas = *g_StarMap_update_canvas;
        __builtin_memcpy(&matrix, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        MatrixSetTranslation(&matrix, field<float>(this, 0x78), field<float>(this, 0x7c), field<float>(this, 0x80));
        PaintCanvas_CameraSetLocal(canvas, &matrix);
        if (absf_update(field<float>(this, 0x78) - EaseInOut_GetMaxValue(ptr_field(this, 0x17c))) <= 1.0f &&
            absf_update(field<float>(this, 0x7c) - EaseInOut_GetMaxValue(ptr_field(this, 0x180))) <= 1.0f &&
            absf_update(field<float>(this, 0x80) - EaseInOut_GetMaxValue(ptr_field(this, 0x184))) <= 1.0f) {
            if (field<uint8_t>(this, 0x138) == 0) {
                if (ptr_field(this, 0x90) != 0) {
                    ArrayReleaseClasses_AEGeometry(ptr_field(this, 0x90));
                    operator delete(Array_AEGeometry_dtor(ptr_field(this, 0x90)));
                }
                ptr_field(this, 0x90) = 0;
                if (ptr_field(this, 0x94) != 0) {
                    ArrayReleaseClasses_AEGeometry(ptr_field(this, 0x94));
                    operator delete(Array_AEGeometry_dtor(ptr_field(this, 0x94)));
                }
                ptr_field(this, 0x94) = 0;
                if (ptr_field(this, 0x98) != 0) {
                    operator delete(ptr_field(this, 0x98));
                }
                ptr_field(this, 0x98) = 0;
                if (ptr_field(this, 0x9c) != 0) {
                    operator delete(ptr_field(this, 0x9c));
                }
                ptr_field(this, 0x9c) = 0;
                if (ptr_field(this, 0x100) != 0) {
                    ArrayRelease_bool(ptr_field(this, 0x100));
                    operator delete(Array_bool_dtor(ptr_field(this, 0x100)));
                }
                ptr_field(this, 0x100) = 0;
                if (ptr_field(this, 0xa4) != 0) {
                    operator delete(AEGeometry_dtor(ptr_field(this, 0xa4)));
                }
                field<int32_t>(this, 4) = 0;
                ptr_field(this, 0xa4) = 0;
                AEGeometry_setVisible(((Array<void *> *)ptr_field(this, 0x68))->data()[field<int32_t>(this, 0x60)], true);
            } else {
                field<int32_t>(this, 4) = 3;
            }
            field<uint16_t>(this, 0x138) = 0;
        }
        return;
    }

    FModSound_setParamValue(*g_StarMap_update_sound, 0, 0x66, 0.0f);
    if (field<int32_t>(this, 4) == 3) {
        if (field<uint8_t>(this, 0x13b) == 0 && field<uint8_t>(this, 0x174) == 0) {
            float vx = field<float>(this, 0x168) * field<float>(this, 0x16c);
            float vy = field<float>(this, 0x168) * field<float>(this, 0x170);
            field<float>(this, 0x16c) = vx;
            field<float>(this, 0x170) = vy;
            if (absf_update(vx) > 0.5f) {
                field<float>(this, 0x188) += vx;
            }
            if (absf_update(vy) > 0.5f) {
                field<float>(this, 0x18c) += vy;
            }
        } else if (field<uint8_t>(this, 0x13b) != 0 && field<int32_t>(this, 0x64) >= 0) {
            int target = 0x8000 - ((int *)ptr_field(this, 0x98))[field<int32_t>(this, 0x64)];
            int diff = (int)field<float>(this, 0x188) - target;
            field<float>(this, 0x188) += (float)(diff < 0 ? -diff : diff) * (diff < 0 ? 0.25f : -0.25f);
            if (absf_update((float)diff) < 11.0f) {
                field<uint8_t>(this, 0x13b) = 0;
            }
        }
        float speed = absf_update(field<float>(this, 0x16c) + field<float>(this, 0x170));
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        field<float>(this, 0x1c0) += speed;
        if (ptr_field(this, 0x90) != 0) {
            Array<void *> *geoms = (Array<void *> *)ptr_field(this, 0x90);
            for (uint32_t i = 0; i < geoms->size(); i++) {
                if (geoms->data()[i] != 0) {
                    AEGeometry_rotate(geoms->data()[i], (float)dt * 0.001f, 0.0f, 0.001f);
                }
            }
        }
        field<float>(this, 0x188) = (float)((int)field<float>(this, 0x188) & 0xffff);
        if (field<float>(this, 0x18c) < -90.0f) {
            field<float>(this, 0x18c) = -90.0f;
        }
        if (field<float>(this, 0x18c) > 90.0f) {
            field<float>(this, 0x18c) = 90.0f;
        }
        AEGeometry_setRotation(ptr_field(this, 0xa4), field<float>(this, 0x18c), 0.0f, field<float>(this, 0x188));
        return;
    }

    if (field<int32_t>(this, 4) == 0) {
        if (field<int32_t>(this, 0x114) >= 0 && field<uint8_t>(this, 0x118) != 0 &&
            field<int32_t>(this, 0x11c) < 4000) {
            field<int32_t>(this, 0x11c) += dt;
            if (field<int32_t>(this, 0x11c) > 3999) {
                OnTouchBegin(*g_StarMap_update_screenW >> 1, *g_StarMap_update_screenH >> 1);
            }
            float scale = ((float)field<int32_t>(this, 0x11c) / 4000.0f) * 1.5f;
            AEGeometry_setScaling(((Array<void *> *)ptr_field(this, 0x68))->data()[field<int32_t>(this, 0x114)],
                                  scale, scale, scale);
        }
        if (field<uint8_t>(this, 0x174) == 0) {
            float vx = field<float>(this, 0x168) * field<float>(this, 0x16c);
            float vy = field<float>(this, 0x168) * field<float>(this, 0x170);
            field<float>(this, 0x16c) = vx;
            field<float>(this, 0x170) = vy;
            if (absf_update(vx) > 0.5f) {
                field<int32_t>(this, 0x13c) = (int)((float)field<int32_t>(this, 0x13c) + vx);
            }
            if (absf_update(vy) > 0.5f) {
                field<int32_t>(this, 0x140) = (int)((float)field<int32_t>(this, 0x140) + vy);
            }
        }
        void *canvas = *g_StarMap_update_canvas;
        __builtin_memcpy(&matrix, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        MatrixSetTranslation(&matrix,
                             (field<float>(this, 8) + (float)field<int32_t>(this, 0x13c)) * 20.0f,
                             0.0f,
                             (field<float>(this, 0xc) + (float)field<int32_t>(this, 0x140)) * 20.0f);
        PaintCanvas_CameraSetLocal(canvas, &matrix);
        if (field<uint8_t>(this, 0x13a) != 0) {
            Array<uint8_t> *vis = (Array<uint8_t> *)Status_getSystemVisibilities(*g_StarMap_update_status);
            uint32_t selected = field<uint32_t>(this, 0x60);
            if (vis != 0 && selected < vis->size() && vis->data()[selected] != 0) {
                Vector_assign((Vector *)((char *)this + 0x78),
                              ((Array<Vector *> *)ptr_field(this, 0x194))->data()[selected]);
                float targetX = (float)(*g_StarMap_update_screenW >> 1);
                float targetY = (float)(*g_StarMap_update_screenH >> 1);
                float sx = (field<float>(this, 0x78) - targetX) / -30.0f;
                float sy = (field<float>(this, 0x7c) - targetY) / -30.0f;
                field<float>(this, 0x16c) = sx;
                field<float>(this, 0x170) = sy;
                if (absf_update(sx) <= 2.0f && absf_update(sy) <= 2.0f) {
                    field<uint8_t>(this, 0x13a) = 0;
                    field<int32_t>(this, 0x19c) = field<int32_t>(this, 0x60);
                }
            }
        }
    }
}

// ---- StarMap_c67a4.cpp ----
StarMap::StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4)
{
    void (*vecCtor)(void *) = g_StarMap_ctor_vecCtor;
    Vector zero = {0.0f, 0.0f, 0.0f};
    this->field_0x78 = zero;
    field<int32_t>(this, 0x88) = 0;
    field<int32_t>(this, 0x8c) = 0;
    vecCtor((char *)this + 0xac);
    vecCtor((char *)this + 0xbc);
    vecCtor((char *)this + 0xcc);
    field<Vector>(this, 0x154) = zero;
    field<Vector>(this, 0x144) = zero;
    field<int32_t>(this, 0x164) = 0;
    field<int32_t>(this, 0x188) = 0;
    field<int32_t>(this, 0x18c) = 0;
    field<int32_t>(this, 0x190) = 0;

    field<int32_t>(this, 4) = 0;
    field<int32_t>(this, 0x1c8) = field<int32_t>(*g_StarMap_ctor_status, 0x88);
    field<int32_t>(this, 0x60) = -1;
    ptr_field(this, 0xa4) = 0;
    ptr_field(this, 0x58) = 0;
    ptr_field(this, 0x194) = 0;
    ptr_field(this, 0x198) = 0;
    field<uint8_t>(this, 0x1e4) = 0;
    field<uint8_t>(this, 1) = 0;
    field<uint8_t>(this, 0x120) = 0;
    ptr_field(this, 0x17c) = 0;
    ptr_field(this, 0x180) = 0;
    ptr_field(this, 0x184) = 0;
    field<int32_t>(this, 0x168) = 0;
    field<int32_t>(this, 0x16c) = 0;
    field<int32_t>(this, 0x170) = 0;

    ptr_field(this, 0xfc) = operator new(0x14);
    ptr_field(this, 0x54) = Galaxy_getSystems(*g_StarMap_ctor_galaxy);
    field<int32_t>(this, 0x10) = 500;
    field<int32_t>(this, 0x14) = 500;

    void *root = operator new(0xc0);
    AEGeometry_ctor_canvas(root, *g_StarMap_ctor_canvas);
    ptr_field(this, 0x6c) = root;

    void *systemsGeom = operator new(0xc);
    Array_AEGeometry_ctor(systemsGeom);
    ptr_field(this, 0x68) = systemsGeom;
    void *systemPositions = operator new(0xc);
    Array_Vector_ctor(systemPositions);
    ptr_field(this, 0x194) = systemPositions;
    ArraySetLength_AEGeometry(0x22, systemsGeom);
    ArraySetLength_Vector(0x22, systemPositions);

    Array<void *> *systems = (Array<void *> *)ptr_field(this, 0x54);
    Array<void *> *geoms = (Array<void *> *)ptr_field(this, 0x68);
    Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x194);
    for (uint32_t i = 0; i < geoms->size(); i++) {
        void *sys = systems->data()[i];
        int tex = SolarSystem_getTextureIndex(sys);
        uint16_t image = (uint16_t)(tex + 0x4696);
        if (i == 0x1b && Status_getCurrentCampaignMission(*g_StarMap_ctor_status) > 0x9d) {
            image = 0x469b;
        }
        void *geom = operator new(0xc0);
        AEGeometry_ctor_image(geom, image, *g_StarMap_ctor_canvas, false);
        geoms->data()[i] = geom;
        AEGeometry_setScaling(geom, 1.0f, 1.0f, 1.0f);
        float px = (float)((int)(((100 - SolarSystem_getX(sys)) / 100.0f) * 20000.0f) - 10000);
        float py = (float)((int)(((100 - SolarSystem_getY(sys)) / 100.0f) * 18000.0f) - 9000);
        float pz = (float)((int)(((100 - SolarSystem_getZ(sys)) / 100.0f) * 9000.0f) + 1000);
        AEGeometry_setPosition(geom, pz, px, py);
        AEGeometry_addChild(root, field<uint32_t>(geom, 0xc));
        Vector *pos = (Vector *)operator new(0xc);
        *pos = zero;
        positions->data()[i] = pos;
    }

    AERandom_reset(*g_StarMap_ctor_random);
    ptr_field(this, 0xf8) = 0;
    field<uint8_t>(this, 0xa9) = 0;
    if (Status_getCurrentCampaignMission(*g_StarMap_ctor_status) > 0x1f &&
        field<int32_t>(*g_StarMap_ctor_status, 0x7c) >= 0) {
        void *marker = operator new(0xc0);
        AEGeometry_ctor_image(marker, 0x4262, *g_StarMap_ctor_canvas, false);
        ptr_field(this, 0xf8) = marker;
        Vector p;
        AEGeometry_getPosition(&p, geoms->data()[field<int32_t>(*g_StarMap_ctor_status, 0x7c)]);
        AEGeometry_setPosition(marker, p.x, p.y, p.z);
        Transform_SetAnimationState(PaintCanvas_TransformGetTransform(*g_StarMap_ctor_canvas), 2, 0);
        AEGeometry_setRotation(marker, 0.0f, 0.0f, 0.0f);
    }

    init(jumpMapMode, mission, param3, param4);
}

// ---- OnTouchBegin_ca7e0.cpp ----
static inline float absf_local(float v)
{
    return v < 0.0f ? -v : v;
}

uint32_t StarMap::OnTouchBegin(int x, int y)
{
    if (field<uint8_t>(this, 0xa9) != 0) {
        ChoiceWindow_OnTouchBegin(ptr_field(this, 0x5c), x, y);
        return 0;
    }
    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        return 0;
    }

    void *layout = *g_StarMap_touch_layout;
    ((Layout *)(layout))->OnTouchBegin(x, y);
    if ((field<uint8_t>(this, 0xa8) != 0 && field<uint8_t>(this, 0x13a) != 0) ||
        field<uint8_t>(this, 0x13b) != 0) {
        return 0;
    }
    ((TouchButton *)(ptr_field(this, 0x4c)))->OnTouchBegin(x, y);
    if (field<int32_t>(layout, 0xc) >= y || y >= *g_StarMap_touch_screenH - field<int32_t>(layout, 0x10)) {
        return 0;
    }
    if (field<uint8_t>(this, 0x118) != 0 && field<int32_t>(this, 0x11c) < 4000) {
        return 0;
    }

    void *sound = *g_StarMap_touch_sound;
    FModSound_stop(sound, 0x66);
    FModSound_play(sound, 0x66, 0, 0.0f);

    float fx = (float)x;
    float fy = (float)y;
    field<float>(this, 0x15c) = fx;
    field<float>(this, 0x160) = fy;
    field<float>(this, 0x144) = fx;
    field<float>(this, 0x148) = fy;
    field<int32_t>(this, 0x150) = 0;
    field<int32_t>(this, 0x154) = 0;
    field<uint8_t>(this, 0x174) = 1;
    field<uint8_t>(this, 0x13a) = 0;

    int oldSystem = field<int32_t>(this, 0x60);
    if (field<int32_t>(this, 4) == 0) {
        field<int32_t>(this, 0x1d0) = 0;
        field<int32_t>(this, 0x60) = -1;
        void *status = *g_StarMap_touch_status;
        Array<void *> *systems = (Array<void *> *)ptr_field(this, 0x68);
        for (uint32_t i = 0; i < systems->size(); i++) {
            Array<uint8_t> *vis = (Array<uint8_t> *)Status_getSystemVisibilities(status);
            if (vis != 0 && i < vis->size() && vis->data()[i] != 0) {
                Vector_assign((Vector *)((char *)this + 0x78),
                              ((Array<Vector *> *)ptr_field(this, 0x194))->data()[i]);
                if (field<float>(this, 0x80) > 0.0f &&
                    absf_local(field<float>(this, 0x78) - fx) < (float)field<int32_t>(this, 0x1c8) &&
                    absf_local(field<float>(this, 0x7c) - fy) < (float)field<int32_t>(this, 0x1c8)) {
                    field<int32_t>(this, 0x60) = (int)i;
                    void *stations = ptr_field(this, 0x58);
                    if (stations != 0) {
                        ArrayReleaseClasses_Station(stations);
                        if (ptr_field(this, 0x58) != 0) {
                            operator delete(Array_Station_dtor(ptr_field(this, 0x58)));
                        }
                        ptr_field(this, 0x58) = 0;
                    }
                    void *arr = operator new(0xc);
                    Array_Station_ctor(arr);
                    ptr_field(this, 0x58) = arr;
                    void *reader = operator new(1);
                    FileRead_ctor(reader);
                    ptr_field(this, 0x58) =
                        FileRead_loadStationsBinary(reader, ((Array<void *> *)ptr_field(this, 0x54))->data()[i]);
                    operator delete(FileRead_dtor(reader));
                    if (oldSystem != field<int32_t>(this, 0x60)) {
                        FModSound_play(sound, 0x67, 0, 0.0f);
                    }
                    int current = SolarSystem_getIndex(Status_getSystem(status));
                    int dist = SystemPathFinder_getJumpDistance(ptr_field(this, 0x50), ptr_field(this, 0x54),
                                                                current, field<int32_t>(this, 0x60));
                    field<int32_t>(this, 0x1d0) = dist;
                    if (dist == 0 && current != field<int32_t>(this, 0x60)) {
                        field<int32_t>(this, 0x1d0) = 4;
                        if (SolarSystem_getRoutes(((Array<void *> *)ptr_field(this, 0x54))->data()[i]) == 0) {
                            field<uint8_t>(this, 0x1d4) = 1;
                        }
                    }
                    if (Status_hardCoreMode(status) != 0) {
                        field<int32_t>(this, 0x1d0) <<= 1;
                    }
                    return 0;
                }
            }
        }
    } else if (field<int32_t>(this, 4) == 3) {
        Array<Vector *> *stations = (Array<Vector *> *)ptr_field(this, 0x198);
        for (uint32_t i = 0; i < stations->size(); i++) {
            Vector_assign((Vector *)((char *)this + 0x78), stations->data()[i]);
            if (field<float>(this, 0x80) > 0.0f &&
                absf_local(field<float>(this, 0x78) - fx) < (float)field<int32_t>(this, 0x1c8) &&
                absf_local(field<float>(this, 0x7c) - fy) < (float)field<int32_t>(this, 0x1c8)) {
                field<int32_t>(this, 0x64) = (int)i;
                FModSound_play(sound, 0x68, 0, 0.0f);
                return 0;
            }
        }
    }
    return 0;
}

// ---- OnTouchMove_cab0c.cpp ----
void StarMap::OnTouchMove(int x, int y)
{
    Matrix matrix;

    if (field<uint8_t>(this, 0xa9) != 0) {
        ChoiceWindow_OnTouchMove(ptr_field(this, 0x5c), x, y);
        return;
    }
    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        return;
    }
    void *layout = *g_StarMap_move_layout;
    ((Layout *)(layout))->OnTouchMove(x, y);
    if ((field<uint8_t>(this, 0xa8) != 0 && field<uint8_t>(this, 0x13a) != 0) ||
        field<uint8_t>(this, 0x13b) != 0) {
        return;
    }
    ((TouchButton *)(ptr_field(this, 0x4c)))->OnTouchMove(x, y);
    if (field<uint8_t>(this, 0x174) == 0) {
        return;
    }

    float fx = (float)x;
    float fy = (float)y;
    float lastX = field<float>(this, 0x15c);
    float lastY = field<float>(this, 0x160);
    field<int32_t>(this, 0x168) = 0x3f800000;
    field<float>(this, 0x15c) = fx;
    field<float>(this, 0x160) = fy;
    float dx = (fx - lastX) * field<float>(this, 0x1cc);
    float dy = field<float>(this, 0x1cc) * (fy - lastY);
    field<float>(this, 0x150) = dx;
    field<float>(this, 0x154) = dy;

    if (field<int32_t>(this, 4) == 0) {
        float speed = absf_local(dx + dy);
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        float targetX = (float)field<int32_t>(this, 0x13c);
        float targetY = (float)field<int32_t>(this, 0x140);
        field<int32_t>(this, 0x13c) = (int)(dx + targetX);
        field<int32_t>(this, 0x140) = (int)(dy + targetY);
        field<float>(this, 0x1c0) = field<float>(this, 0x1c0) + speed;
        if (absf_local(field<float>(this, 0x144) - fx) > 3.0f ||
            absf_local(field<float>(this, 0x148) - fy) > 3.0f) {
            field<int32_t>(this, 0x19c) = -1;
            field<int32_t>(this, 0x60) = -1;
            field<int32_t>(this, 0x1d0) = 0;
        }
        void *canvas = *g_StarMap_move_canvas;
        PaintCanvas_CameraGetCurrent(canvas);
        __builtin_memcpy(&matrix, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        PaintCanvas_CameraSetLocal(canvas, &matrix);
        return;
    }

    if (absf_local(field<float>(this, 0x144) - fx) <= 3.0f ||
        absf_local(field<float>(this, 0x148) - fy) <= 3.0f) {
        field<int32_t>(this, 0x1a0) = -1;
        field<int32_t>(this, 0x64) = -1;
    }

    float rotZ = dx * 16.0f;
    float rotX = dy * 16.0f;
    float pitch = field<float>(this, 0x18c) + rotX;
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }
    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    float yaw = (float)((int)(field<float>(this, 0x188) + rotZ) & 0xffff);
    float absZ = absf_local(rotZ);
    float absX = absf_local(rotX);
    field<float>(this, 0x150) = rotZ;
    field<float>(this, 0x154) = rotX;
    field<float>(this, 0x16c) = absZ > 3.0f ? rotZ : 0.0f;
    field<float>(this, 0x170) = absX > 3.0f ? rotX : 0.0f;
    field<float>(this, 0x188) = yaw;
    field<float>(this, 0x18c) = pitch;
    AEGeometry_setRotation(ptr_field(this, 0xa4), field<float>(this, 0x170), yaw, field<float>(this, 0x16c));
}

// ---- drawKey_c80e8.cpp ----
void StarMap::drawKey()
{
    uint32_t *canvasHolder = g_StarMap_drawKey_canvas;
    int imageWidth = PaintCanvas_GetImage2DWidth(*canvasHolder, field<uint32_t>(this, 0x30));
    void *layout = *g_StarMap_drawKey_layout;
    int screenW = *g_StarMap_drawKey_screenW;
    int screenH = *g_StarMap_drawKey_screenH;
    int boxW = field<int32_t>(this, 0x10c);
    int boxH = field<int32_t>(this, 0x110);
    int marginY = field<int32_t>(layout, 4);
    int padY = field<int32_t>(layout, 8);
    int rightPad = field<int32_t>(layout, 0x10);
    int lineH = field<int32_t>(layout, 0x2c);

    String empty;
    ((String *)(&empty))->ctor_char("", false);
    int x = screenH - boxW;
    ((Layout *)(layout))->drawBox(7, x, ((screenW - rightPad) - boxH) - padY, boxW, padY + boxH, &empty);
    int drawX = x + lineH;
    int textX = imageWidth + lineH + drawX;
    int y = ((screenW - lineH) - rightPad) - marginY;
    ((String *)(&empty))->dtor();

    void (*drawImage)(uint32_t, uint32_t, int, int) = g_StarMap_drawKey_drawImage;
    drawImage(*canvasHolder, field<uint32_t>(this, 0x20), drawX, y);

    void **textHolder = g_StarMap_drawKey_text;
    String *(*getText)(void *, int) = g_StarMap_drawKey_getText;
    void **fontHolder = g_StarMap_drawKey_font;
    void (*drawString)(uint32_t, void *, String *, int, int, bool) = g_StarMap_drawKey_drawString;

    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x112), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x30), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x191), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x2c), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x223), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x28), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x22c), textX, y, false);
    y -= field<int32_t>(*g_StarMap_drawKey_layout, 4);
    drawImage(*canvasHolder, field<uint32_t>(this, 0x24), drawX, y);
    drawString(*canvasHolder, *fontHolder, getText(*textHolder, 0x22b), textX, y, false);
}

// ---- initStarSystem_c7580.cpp ----
void StarMap::initStarSystem()
{
    void *system = ((Array<void *> *)ptr_field(this, 0x54))->data()[field<int32_t>(this, 0x60)];
    Array<void *> *stationIds = (Array<void *> *)SolarSystem_getStations(system);
    uint32_t count = stationIds->size();

    void *stations = operator new(0xc);
    Array_Station_ctor(stations);
    ptr_field(this, 0x58) = stations;
    ArraySetLength_Station(count, stations);
    void *reader = operator new(1);
    FileRead_ctor(reader);
    ptr_field(this, 0x58) = FileRead_loadStationsBinary(reader, system);
    operator delete(FileRead_dtor(reader));

    uint32_t bytes = count > 0x3fffffffu ? 0xffffffffu : count * 4;
    ptr_field(this, 0x98) = operator new(bytes);
    ptr_field(this, 0x9c) = operator new(bytes);
    field<int32_t>(this, 0x1c4) = -1;
    AERandom_setSeed(*g_StarMap_system_random, (long long)SolarSystem_getIndex(system) * 1000);

    void *stationGeoms = operator new(0xc);
    Array_AEGeometry_ctor(stationGeoms);
    ptr_field(this, 0x90) = stationGeoms;
    ArraySetLength_AEGeometry(count + 1, stationGeoms);

    void *root = operator new(0xc0);
    AEGeometry_ctor_canvas(root, *g_StarMap_system_canvas);
    ptr_field(this, 0xa4) = root;

    void *used = operator new(0xc);
    Array_bool_ctor(used);
    ptr_field(this, 0x100) = used;
    ArraySetLength_bool(((Array<void *> *)stationGeoms)->size(), used);
    for (uint32_t i = 0; i < ((Array<uint8_t> *)used)->size(); i++) {
        ((Array<uint8_t> *)used)->data()[i] = 0;
    }

    Array<void *> *stationArr = (Array<void *> *)ptr_field(this, 0x58);
    Array<void *> *geomArr = (Array<void *> *)ptr_field(this, 0x90);
    for (uint32_t i = 1; i < geomArr->size(); i++) {
        uint32_t stationIndex = i - 1;
        int tex = Station_getTextureIndex(stationArr->data()[stationIndex]);
        void *geom = operator new(0xc0);
        AEGeometry_ctor_image(geom, (uint16_t)(tex + 0x4704), *g_StarMap_system_canvas, false);
        geomArr->data()[i] = geom;
        ((int *)ptr_field(this, 0x98))[stationIndex] =
            AERandom_nextInt(*g_StarMap_system_random, ((Array<uint8_t> *)used)->size()) *
            (0x10000 / (int)((Array<uint8_t> *)used)->size());
        int dist = (i == 1) ? 0x1900 : ((int *)ptr_field(this, 0x9c))[i - 2];
        dist += AERandom_nextInt(*g_StarMap_system_random, 0x15e0) + 0x640;
        ((int *)ptr_field(this, 0x9c))[stationIndex] = dist;
        Vector pos = {0.0f, 0.0f, (float)dist};
        AEGeometry_translate(geom, &pos);
        float scale = (float)(tex << 4) * 0.001f;
        AEGeometry_setScaling(geom, scale, scale, scale);
        AEGeometry_addChild(root, field<uint32_t>(geom, 0xc));
        if (ptr_field(this, 0xf8) != 0 && field<int32_t>(this, 0x60) == field<int32_t>(*g_StarMap_system_status, 0x7c) &&
            Station_getIndex(stationArr->data()[stationIndex]) == field<int32_t>(*g_StarMap_system_status, 0x80)) {
            field<uint32_t>(this, 0x1c4) = i;
        }
    }

    AEGeometry_setVisible(((Array<void *> *)ptr_field(this, 0x90))->data()[1], false);

    void *rings = operator new(0xc);
    Array_AEGeometry_ctor(rings);
    ptr_field(this, 0x94) = rings;
    ArraySetLength_AEGeometry(count, rings);
    for (uint32_t i = 0; i < ((Array<void *> *)rings)->size(); i++) {
        void *ring = operator new(0xc0);
        AEGeometry_ctor_image(ring, 0x1a7b, *g_StarMap_system_canvas, false);
        ((Array<void *> *)rings)->data()[i] = ring;
        AEGeometry_addChild(root, field<uint32_t>(ring, 0xc));
        float scale = (float)(((int *)ptr_field(this, 0x9c))[i] << 1) * 0.001f;
        AEGeometry_setScaling(ring, scale, scale, scale);
    }

    Vector selected;
    AEGeometry_getPosition(&selected, ((Array<void *> *)ptr_field(this, 0x68))->data()[field<int32_t>(this, 0x60)]);
    AEGeometry_setPosition(root, &selected);
    AEGeometry_setScaling(root, 0.0078125f, 0.0078125f, 0.0078125f);
    AEGeometry_setRotation(root, 0.0f, 0.0f, 0.0f);
    field<int32_t>(this, 0x188) = 0x45800000;
    field<float>(this, 0x18c) = 0.0f;
    field<int32_t>(this, 0x64) = -1;
    PaintCanvas_Image2DCreate(*g_StarMap_system_canvas, (uint16_t)(0x4500 + SolarSystem_getRace(system)),
                              (char *)this + 0x34);

    if (ptr_field(this, 0x198) != 0) {
        ArrayReleaseClasses_Vector(ptr_field(this, 0x198));
        if (ptr_field(this, 0x198) != 0) {
            operator delete(Array_Vector_dtor(ptr_field(this, 0x198)));
        }
    }
    ptr_field(this, 0x198) = 0;
    void *stationPositions = operator new(0xc);
    Array_Vector_ctor(stationPositions);
    ptr_field(this, 0x198) = stationPositions;
    ArraySetLength_Vector(((Array<void *> *)ptr_field(this, 0x58))->size(), stationPositions);
    for (uint32_t i = 0; i < ((Array<Vector *> *)stationPositions)->size(); i++) {
        Vector *p = (Vector *)operator new(0xc);
        p->x = 0.0f;
        p->y = 0.0f;
        p->z = 0.0f;
        ((Array<Vector *> *)stationPositions)->data()[i] = p;
    }

    ((Engine *)(*g_StarMap_system_engine))->LightSetLightDirection(0.0f, 0.0f, 1.0f, 1);
    if (ptr_field(this, 0x1bc) != 0) {
        operator delete(AEGeometry_dtor(ptr_field(this, 0x1bc)));
    }
    field<uint32_t>(this, 0x178) = 0xffffffffu;
    ptr_field(this, 0x1bc) = 0;
    uint16_t texture = (uint16_t)(0x2700 + SolarSystem_getTextureIndex(system));
    if (field<int32_t>(this, 0x60) == 0x1b) {
        texture = 0x2734;
    }
    PaintCanvas_TextureCreate(*g_StarMap_system_canvas, texture, (char *)this + 0x178, false);
    void *planet = operator new(0xc0);
    AEGeometry_ctor_image(planet, 0x1a70, *g_StarMap_system_canvas, false);
    ptr_field(this, 0x1bc) = planet;
    AEGeometry_setPosition(planet, &selected);
    AEGeometry_setRotation(planet, 0.0f, 0.0f, 0.0f);
    AEGeometry_setScaling(planet, 1.0f, 1.0f, 1.0f);
    AEGeometry_setVisible(((Array<void *> *)ptr_field(this, 0x68))->data()[field<int32_t>(this, 0x60)], false);
}

// ---- drawOnScreenInfo_c8354.cpp ----
void StarMap::drawOnScreenInfo(int index, bool stationMode)
{
    String name;
    String line;
    String value;

    Array<Vector *> *positions =
        stationMode ? (Array<Vector *> *)ptr_field(this, 0x198) : (Array<Vector *> *)ptr_field(this, 0x194);
    Vector_assign((Vector *)((char *)this + 0x78), positions->data()[index]);
    float x = field<float>(this, 0x78);
    float y = field<float>(this, 0x7c);
    if (x < 0.0f || x > (float)(*g_StarMap_info_screenW + 0x32) ||
        y < 0.0f || y > (float)(*g_StarMap_info_screenH + 0x32)) {
        return;
    }

    uint32_t canvas = *g_StarMap_info_canvas;
    PaintCanvas_SetColor(canvas, 0xffffffffu);
    int *icons = (int *)ptr_field(this, 0xfc);
    for (int i = 0; i != 5; i++) {
        icons[i] = -1;
    }

    if (stationMode) {
        void *station = ((Array<void *> *)ptr_field(this, 0x58))->data()[index];
        if (((Station *)(station))->isDiscovered() != 0) {
            icons[0] = field<int32_t>(this, 0x30);
        }
        int current = Station_getIndex(Status_getStation(*g_StarMap_info_status));
        if (current == Station_getIndex(station)) {
            icons[3] = field<int32_t>(this, 0x2c);
        }
        ((Station *)(&name))->getName();
        int textW = PaintCanvas_GetTextWidth(canvas, (String *)*g_StarMap_info_font);
        int drawX = (int)(x - (float)(textW / 2));
        int drawY = (int)(y + (float)(field<int32_t>(this, 0x1a8) >> 1) - 3.0f);
        if (field<int32_t>(this, 0x64) == index) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0x80, 0, field<int32_t>(this, 0x1a4));
            PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX, drawY, false);
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
            if (Station_getTecLevel(station) > 0) {
                ((String *)(&line))->ctor_copy(((GameText *)(*g_StarMap_info_text))->getText(0x200), false);
                ((String *)(&value))->ctor_int(Station_getTecLevel(station));
                String_add(&name, &line, &value);
                PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX,
                                       drawY + field<int32_t>(*g_StarMap_info_layout, 4), false);
                ((String *)(&name))->dtor();
                ((String *)(&value))->dtor();
                ((String *)(&line))->dtor();
            }
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x40), (int)x, (int)y, 0x11);
        } else {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x44), (int)x, (int)y, 0x11);
            PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX, drawY, false);
        }
        ((String *)(&name))->dtor();
    } else {
        void *system = ((Array<void *> *)ptr_field(this, 0x54))->data()[index];
        if (((SolarSystem *)(system))->isFullyDiscovered() != 0) {
            icons[0] = field<int32_t>(this, 0x30);
        }
        if (Status_getCurrentCampaignMission(*g_StarMap_info_status) == 0x34 &&
            ((SolarSystem *)(system))->getStationEnumIndex(0x4a) >= 0) {
            icons[1] = field<int32_t>(this, 0x24);
        }
        void *mission = Status_getCampaignMission(*g_StarMap_info_status);
        if (mission != 0 && ((Mission *)(mission))->isEmpty() == 0) {
            int target = Mission_getTargetStation(mission);
            if (((SolarSystem *)(system))->getStationEnumIndex(target) >= 0) {
                icons[2] = field<int32_t>(this, 0x28);
            }
        }
        void *freelance = Status_getFreelanceMission(*g_StarMap_info_status);
        if (freelance != 0 && ((Mission *)(freelance))->isEmpty() == 0) {
            int target = Mission_getTargetStation(freelance);
            if (((SolarSystem *)(system))->getStationEnumIndex(target) >= 0) {
                icons[2] = field<int32_t>(this, 0x28);
            }
        }
        ((SolarSystem *)(&name))->getName();
        int textW = PaintCanvas_GetTextWidth(canvas, (String *)*g_StarMap_info_font);
        int drawX = (int)(x - (float)(textW / 2));
        int drawY = (int)(y + (float)(field<int32_t>(this, 0x1a8) >> 1) - 3.0f);
        void *currentSystem = Status_getSystem(*g_StarMap_info_status);
        if (SolarSystem_getIndex(currentSystem) == SolarSystem_getIndex(system)) {
            ((Layout *)(*g_StarMap_info_layout))->getPulseValue((float)field<int32_t>(this, 0x1a4));
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x48), (int)x, (int)y, 0x11);
        }
        if (field<int32_t>(this, 0x60) == index) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0x80, 0, field<int32_t>(this, 0x1a4));
        } else if (field<int32_t>(this, 0x60) >= 0) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
        }
        PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX, drawY, false);
        ((String *)(&name))->dtor();
        PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
        if (((SolarSystem *)(system))->hasNoOwner() == 0) {
            uint32_t image = field<uint32_t>(this, 0x130);
            int race = SolarSystem_getRace(system);
            if (race == 2) {
                image = field<uint32_t>(this, 0x12c);
            } else if (race == 1) {
                image = field<uint32_t>(this, 0x128);
            } else if (race == 0) {
                image = field<uint32_t>(this, 0x124);
            }
            PaintCanvas_DrawImage2D(canvas, image, drawX, drawY + 0xfd, 0x11);
        }
        if (field<int32_t>(this, 0x60) == index) {
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x40), (int)x, (int)y, 0x11);
        } else {
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x44), (int)x, (int)y, 0x11);
        }
    }

    float iconY = (float)(int)((field<float>(this, 0x7c) - (float)(field<int32_t>(this, 0x1a8) >> 1)) + 10.0f);
    float iconX = (float)(int)(field<float>(this, 0x78) + (float)(field<int32_t>(this, 0x1a8) >> 1) - 7.0f);
    for (int i = 0; i != 5; i++) {
        int image = icons[i];
        if (image != -1) {
            int dx = (i == 0 && *g_StarMap_info_isGerman != 0) ? 0xc : 0x12;
            PaintCanvas_DrawImage2D(canvas, (uint32_t)image, (int)iconX - dx, (int)iconY, 0);
        }
    }
}

// ---- init_c6bf0.cpp ----
int StarMap::init(bool jumpMapMode, Mission *mission, bool param3, int param4)
{
    Matrix matrix;
    Vector pos;

    uint32_t canvas = *g_StarMap_init_canvas;
    PaintCanvas_FogEnable(canvas, 0, true);
    field<uint8_t>(this, 0x118) = (uint8_t)param3;
    field<uint8_t>(this, 0xa8) = (uint8_t)jumpMapMode;
    field<int32_t>(this, 0x114) = param4;

    void (*imageCreate)(uint32_t, int, void *) = g_StarMap_init_imageCreate;
    imageCreate(canvas, 0x4a1, (char *)this + 0x124);
    imageCreate(canvas, 0x49c, (char *)this + 0x128);
    imageCreate(canvas, 0x49f, (char *)this + 0x12c);
    imageCreate(canvas, 0x49e, (char *)this + 0x130);
    imageCreate(canvas, 0x452, (char *)this + 0x20);
    imageCreate(canvas, 0x4a2, (char *)this + 0x30);
    imageCreate(canvas, 0x453, (char *)this + 0x2c);
    imageCreate(canvas, 0x455, (char *)this + 0x28);
    imageCreate(canvas, 0x454, (char *)this + 0x24);
    imageCreate(canvas, 0x48c, (char *)this + 0x40);
    imageCreate(canvas, 0x48a, (char *)this + 0x44);
    imageCreate(canvas, 0x4fd, (char *)this + 0x48);
    imageCreate(canvas, 0x545, (char *)this + 0x134);

    field<int32_t>(this, 0x1a8) = PaintCanvas_GetImage2DWidth(canvas, field<uint32_t>(this, 0x40));
    field<int32_t>(this, 0x1ac) = PaintCanvas_GetImage2DWidth(canvas, field<uint32_t>(this, 0x28));
    field<int32_t>(this, 0x1d0) = 0;
    field<uint8_t>(this, 0x1d4) = 0;
    ptr_field(this, 0x1bc) = 0;
    field<int32_t>(this, 0x1c0) = 0;
    field<int32_t>(this, 0x1c4) = -1;
    field<int32_t>(this, 0x19c) = -1;
    field<int32_t>(this, 0x1a0) = -1;
    field<int32_t>(this, 0x1dc) = -1;
    field<int32_t>(this, 0x1e0) = -1;
    field<int32_t>(this, 0xe8) = 0;
    field<int32_t>(this, 0xec) = 0;
    field<float>(this, 8) = 0.0f;
    field<float>(this, 0xc) = 0.0f;
    field<uint8_t>(this, 1) = 0;
    field<uint8_t>(this, 0x174) = 0;
    field<int32_t>(this, 0x138) = 0;
    field<int32_t>(this, 0x13c) = 0;
    field<int32_t>(this, 0x140) = 0;

    ptr_field(this, 0x17c) = EaseInOut_ctor(operator new(0x10));
    ptr_field(this, 0x180) = EaseInOut_ctor(operator new(0x10));
    ptr_field(this, 0x184) = EaseInOut_ctor(operator new(0x10));
    field<int32_t>(this, 0x1cc) = field<int32_t>(*g_StarMap_init_layout, 0x90);
    ptr_field(this, 0x74) = PaintCanvas_CameraGetCurrent(canvas);
    PaintCanvas_CameraCreate(canvas, (char *)this + 0x70);
    PaintCanvas_CameraSetPerspective(canvas, 60.0f, 1.0f, 10000.0f, 1.0f);
    MatrixSetTranslation(&matrix, 0.0f, 0.0f, 0.0f);
    MatrixSetRotation(&matrix, 0.0f, 0.0f, 0.0f, 0.0f);
    PaintCanvas_CameraSetLocal(canvas, &matrix);
    PaintCanvas_CameraSetCurrent(canvas, field<uint32_t>(this, 0x70));

    void *status = *g_StarMap_init_status;
    int campaign = Status_getCurrentCampaignMission(status);
    field<uint8_t>(this, 0xf4) = campaign > 0xf;
    field<int32_t>(this, 4) = campaign > 0xf ? 0 : 3;
    field<int32_t>(this, 0x60) = SolarSystem_getIndex(Status_getSystem(status));

    if (param3 != 0) {
        AEGeometry_getPosition(&pos, ((Array<void *> *)ptr_field(this, 0x68))->data()[param4]);
        Vector_assign((Vector *)((char *)this + 0x78), &pos);
    } else if (!jumpMapMode || field<uint8_t>(this, 0xf4) == 0) {
        AEGeometry_getPosition(&pos, ((Array<void *> *)ptr_field(this, 0x68))->data()[field<int32_t>(this, 0x60)]);
        Vector_assign((Vector *)((char *)this + 0x78), &pos);
    }

    field<float>(this, 8) = field<float>(this, 0x78) / 20.0f;
    field<float>(this, 0xc) = field<float>(this, 0x7c) / 20.0f;
    if (field<int32_t>(this, 4) == 3) {
        field<int32_t>(this, 0x64) = -1;
        initStarSystem();
    } else {
        field<int32_t>(this, 0x19c) = field<int32_t>(this, 0x60);
        if (ptr_field(this, 0x58) != 0) {
            ArrayReleaseClasses_Station(ptr_field(this, 0x58));
            if (ptr_field(this, 0x58) != 0) {
                operator delete(Array_Station_dtor(ptr_field(this, 0x58)));
            }
            ptr_field(this, 0x58) = 0;
        }
        void *arr = operator new(0xc);
        Array_Station_ctor(arr);
        ptr_field(this, 0x58) = arr;
        void *reader = operator new(1);
        FileRead_ctor(reader);
        ptr_field(this, 0x58) =
            FileRead_loadStationsBinary(reader, ((Array<void *> *)ptr_field(this, 0x54))->data()[field<int32_t>(this, 0x60)]);
        operator delete(FileRead_dtor(reader));
    }
    if (param3 != 0) {
        field<int32_t>(this, 0x19c) = -1;
        field<int32_t>(this, 0x60) = -1;
    }

    field<uint8_t>(this, 0x108) = 0;
    field<uint16_t>(this, 0xaa) = 0;
    field<uint8_t>(this, 0) = 0;
    void *button = operator new(0xc8);
    String *back = ((GameText *)(*g_StarMap_init_text))->getText(0x190);
    TouchButton_ctor(button, back, 0, *g_StarMap_init_screenW - field<int32_t>(*g_StarMap_init_layout, 0x2c),
                     *g_StarMap_init_screenH - field<int32_t>(*g_StarMap_init_layout, 0x2c), 0x22);
    ptr_field(this, 0x4c) = button;
    ptr_field(this, 0xa0) = 0;
    ptr_field(this, 0x5c) = ChoiceWindow_ctor(operator new(0x5c));
    ptr_field(this, 0x50) = SystemPathFinder_ctor(operator new(1));

    if (jumpMapMode && field<int32_t>(this, 4) == 0 && mission != 0 && ((Mission *)(mission))->isEmpty() == 0 &&
        (((Mission *)(mission))->isVisible() != 0 || Mission_getType(mission) == 0xe)) {
        field<int32_t>(this, 0x104) = -1;
        field<int32_t>(this, 0x60) = -1;
        int target = Mission_getTargetStation(mission);
        Array<void *> *systems = (Array<void *> *)ptr_field(this, 0x54);
        for (uint32_t i = 0; i < systems->size(); i++) {
            if (target >= 0) {
                field<int32_t>(this, 0x104) = (int)i;
                break;
            }
        }
        if (field<int32_t>(this, 0x104) >= 0) {
            int current = SolarSystem_getIndex(Status_getSystem(status));
            ptr_field(this, 0xa0) =
                SystemPathFinder_getSystemPath(ptr_field(this, 0x50), ptr_field(this, 0x54), current,
                                               field<int32_t>(this, 0x104));
            field<uint8_t>(this, 0x13a) = 1;
            field<int32_t>(this, 0x168) = 0x3f800000;
            field<int32_t>(this, 0x60) = field<int32_t>(this, 0x104);
        }
    }

    field<int32_t>(this, 0x10c) = 0;
    for (int i = 0; i < 6; i++) {
        int width = PaintCanvas_GetTextWidth(canvas, ((GameText *)(*g_StarMap_init_text))->getText(0x112 + i));
        if (field<int32_t>(this, 0x10c) < width) {
            field<int32_t>(this, 0x10c) = width;
        }
    }
    field<int32_t>(this, 0x10c) += field<int32_t>(*g_StarMap_init_layout, 0x8c);
    field<int32_t>(this, 0x110) =
        field<int32_t>(*g_StarMap_init_layout, 4) * 5 + field<int32_t>(*g_StarMap_init_layout, 0x2c) * 2;
    field<int32_t>(this, 0x11c) = 0;
    void *cargo = (void *)Ship_getCargo(Status_getShip(status));
    field<int32_t>(this, 0x1d8) = cargo != 0 ? Item_getAmount(cargo) : 0;

    ptr_field(this, 0x1b0) = AEGeometry_ctor_image(operator new(0xc0), 0x41d2, canvas, false);
    AEGeometry_setRotation(ptr_field(this, 0x1b0), 0.0f, 0.0f, 0.0f);
    AEGeometry_setPosition(ptr_field(this, 0x1b0), 0.0f, 0.0f, 0.0f);
    ptr_field(this, 0x1b4) = AEGeometry_ctor_image(operator new(0xc0), 0x41d3, canvas, false);
    AEGeometry_setRotation(ptr_field(this, 0x1b4), 0.0f, 0.0f, 0.0f);
    AEGeometry_setPosition(ptr_field(this, 0x1b4), 0.0f, 0.0f, 0.0f);
    ptr_field(this, 0x1b8) = AEGeometry_ctor_image(operator new(0xc0), 0x41d4, canvas, false);
    AEGeometry_setRotation(ptr_field(this, 0x1b8), 0.0f, 0.0f, 0.0f);
    AEGeometry_setPosition(ptr_field(this, 0x1b8), 0.0f, 0.0f, 0.0f);
    return 0;
}
