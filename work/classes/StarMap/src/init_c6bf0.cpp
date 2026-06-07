#include "class.h"

extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_init_canvas;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_init_imageCreate)(uint32_t, int, void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_init_text;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_init_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_init_screenH;

extern "C" void PaintCanvas_FogEnable(uint32_t canvas, int enabled, bool immediate);
extern "C" int PaintCanvas_GetImage2DWidth(uint32_t canvas, uint32_t image);
extern "C" void *PaintCanvas_CameraGetCurrent(uint32_t canvas);
extern "C" void PaintCanvas_CameraCreate(uint32_t canvas, void *out);
extern "C" void PaintCanvas_CameraSetPerspective(uint32_t canvas, float fov, float nearPlane, float farPlane, float aspect);
extern "C" void PaintCanvas_CameraSetLocal(uint32_t canvas, void *camera);
extern "C" void PaintCanvas_CameraSetCurrent(uint32_t canvas, uint32_t camera);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z, float w);
extern "C" void *Status_getSystem(void *status);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Status_getShip(void *status);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int SolarSystem_getRoutes(void *system);
extern "C" void AEGeometry_getPosition(Vector *out, void *geometry);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void ArrayReleaseClasses_Station(void *arr);
extern "C" void *Array_Station_dtor(void *arr);
extern "C" void Array_Station_ctor(void *arr);
extern "C" void FileRead_ctor(void *reader);
extern "C" void *FileRead_dtor(void *reader);
extern "C" void *FileRead_loadStationsBinary(void *reader, void *system);
extern "C" void *EaseInOut_ctor(void *ease);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void *TouchButton_ctor(void *button, String *text, int a, int x, int y, int size);
extern "C" void *ChoiceWindow_ctor(void *window);
extern "C" void *SystemPathFinder_ctor(void *finder);
extern "C" int Mission_isEmpty(Mission *mission);
extern "C" int Mission_isVisible(Mission *mission);
extern "C" int Mission_getType(Mission *mission);
extern "C" int Mission_getTargetStation(Mission *mission);
extern "C" void *SystemPathFinder_getSystemPath(void *finder, void *systems, int from, int to);
extern "C" int PaintCanvas_GetTextWidth(uint32_t canvas, String *font);
extern "C" int Ship_getCargo(void *ship);
extern "C" int Item_getAmount(void *item);
extern "C" void *AEGeometry_ctor_image(void *geometry, uint16_t image, uint32_t canvas, bool flag);
extern "C" void AEGeometry_setRotation(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setPosition(void *geometry, float x, float y, float z);

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
        AEGeometry_getPosition(&pos, ((Array<void *> *)ptr_field(this, 0x68))->data[param4]);
        Vector_assign((Vector *)((char *)this + 0x78), &pos);
    } else if (!jumpMapMode || field<uint8_t>(this, 0xf4) == 0) {
        AEGeometry_getPosition(&pos, ((Array<void *> *)ptr_field(this, 0x68))->data[field<int32_t>(this, 0x60)]);
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
            FileRead_loadStationsBinary(reader, ((Array<void *> *)ptr_field(this, 0x54))->data[field<int32_t>(this, 0x60)]);
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
    String *back = GameText_getText(*g_StarMap_init_text, 0x190);
    TouchButton_ctor(button, back, 0, *g_StarMap_init_screenW - field<int32_t>(*g_StarMap_init_layout, 0x2c),
                     *g_StarMap_init_screenH - field<int32_t>(*g_StarMap_init_layout, 0x2c), 0x22);
    ptr_field(this, 0x4c) = button;
    ptr_field(this, 0xa0) = 0;
    ptr_field(this, 0x5c) = ChoiceWindow_ctor(operator new(0x5c));
    ptr_field(this, 0x50) = SystemPathFinder_ctor(operator new(1));

    if (jumpMapMode && field<int32_t>(this, 4) == 0 && mission != 0 && Mission_isEmpty(mission) == 0 &&
        (Mission_isVisible(mission) != 0 || Mission_getType(mission) == 0xe)) {
        field<int32_t>(this, 0x104) = -1;
        field<int32_t>(this, 0x60) = -1;
        int target = Mission_getTargetStation(mission);
        Array<void *> *systems = (Array<void *> *)ptr_field(this, 0x54);
        for (uint32_t i = 0; i < systems->length; i++) {
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
        int width = PaintCanvas_GetTextWidth(canvas, GameText_getText(*g_StarMap_init_text, 0x112 + i));
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
