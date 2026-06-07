#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_touch_screenH;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_sound;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_touch_status;

extern "C" void ChoiceWindow_OnTouchBegin(void *window, int x, int y);
extern "C" void Layout_OnTouchBegin(void *layout, int x, int y);
extern "C" void TouchButton_OnTouchBegin(void *button, int x, int y);
extern "C" void FModSound_stop(void *sound, int id);
extern "C" void FModSound_play(void *sound, int id, void *pos, float volume);
extern "C" uint8_t *Status_getSystemVisibilities(void *status);
extern "C" int Status_hardCoreMode(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int SolarSystem_getRoutes(void *system);
extern "C" int SystemPathFinder_getJumpDistance(void *finder, void *systems, int from, int to);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void ArrayReleaseClasses_Station(void *arr);
extern "C" void *Array_Station_dtor(void *arr);
extern "C" void Array_Station_ctor(void *arr);
extern "C" void FileRead_ctor(void *reader);
extern "C" void *FileRead_dtor(void *reader);
extern "C" void *FileRead_loadStationsBinary(void *reader, void *system);

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
    Layout_OnTouchBegin(layout, x, y);
    if ((field<uint8_t>(this, 0xa8) != 0 && field<uint8_t>(this, 0x13a) != 0) ||
        field<uint8_t>(this, 0x13b) != 0) {
        return 0;
    }
    TouchButton_OnTouchBegin(ptr_field(this, 0x4c), x, y);
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
        for (uint32_t i = 0; i < systems->length; i++) {
            Array<uint8_t> *vis = (Array<uint8_t> *)Status_getSystemVisibilities(status);
            if (vis != 0 && i < vis->length && vis->data[i] != 0) {
                Vector_assign((Vector *)((char *)this + 0x78),
                              ((Array<Vector *> *)ptr_field(this, 0x194))->data[i]);
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
                        FileRead_loadStationsBinary(reader, ((Array<void *> *)ptr_field(this, 0x54))->data[i]);
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
                        if (SolarSystem_getRoutes(((Array<void *> *)ptr_field(this, 0x54))->data[i]) == 0) {
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
        for (uint32_t i = 0; i < stations->length; i++) {
            Vector_assign((Vector *)((char *)this + 0x78), stations->data[i]);
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
