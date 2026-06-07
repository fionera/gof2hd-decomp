#include "class.h"

struct FileRead { FileRead(); ~FileRead(); };
struct Galaxy;
struct SolarSystem;

__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;
extern "C" int Station_getSystem(Station *s);
extern "C" int Galaxy_getSystem(Galaxy *g, int sysIdx);
extern "C" void FileRead_ctor(FileRead *fr);
extern "C" void *FileRead_loadStationsBinary(FileRead *fr, SolarSystem *sys);
extern "C" void *FileRead_dtor(FileRead *fr);
extern "C" void operator_delete(void *p);
extern "C" void ArrayReleaseClasses_String(Array<String *> *a);
extern "C" void *Array_String_dtor(Array<String *> *a);
extern "C" void Array_String_ctor(Array<String *> *a);
extern "C" void ArraySetLength_String(uint32_t n, Array<String *> *a);
extern "C" void ArrayRelease_int(void *a);
extern "C" void *Array_int_dtor(void *a);
extern "C" void Array_int_ctor(void *a);
extern "C" void ArraySetLength_int(uint32_t n, void *a);
extern "C" Array<Station *> *SolarSystem_getStations(SolarSystem *sys);
extern "C" int Station_getIndex(Station *s);
extern "C" void String_ctor_empty(String *s);
extern "C" void Station_getName(String *out, Station *s);
extern "C" int Station_getTextureIndex(Station *s);
extern "C" void ArrayReleaseClasses_Station(void *a);
extern "C" void Array_Station_dtor_tail(void *a);

// Travels to `s`: marks its system visible, then rebuilds the planet name and texture tables
// for every station in the destination system.
void Status::setStation(Station *s) {
    if (station == s) {
        return;
    }
    station = s;
    Galaxy *gal = *g_galaxy;
    system = Galaxy_getSystem(gal, Station_getSystem(s));
    if (system == 0) {
        return;
    }
    ((uint8_t *)field_b4->data)[Station_getSystem(station)] = 1;
    FileRead *fr = (FileRead *)operator new(1);
    FileRead_ctor(fr);
    Array<Station *> *list = (Array<Station *> *)FileRead_loadStationsBinary(fr, (SolarSystem *)(void *)(uint32_t)system);
    operator_delete(FileRead_dtor(fr));
    if (planetNames != 0) {
        ArrayReleaseClasses_String((Array<String *> *)(void *)(uint32_t)planetNames);
        if (planetNames != 0) {
            operator_delete(Array_String_dtor((Array<String *> *)(void *)(uint32_t)planetNames));
        }
    }
    planetNames = 0;
    Array<String *> *names = (Array<String *> *)operator new(0xc);
    Array_String_ctor(names);
    planetNames = (int32_t)(uint32_t)names;
    ArraySetLength_String(list->size, names);
    if (planetTextures != 0) {
        ArrayRelease_int((void *)(uint32_t)planetTextures);
        if (planetTextures != 0) {
            operator_delete(Array_int_dtor((void *)(uint32_t)planetTextures));
        }
    }
    planetTextures = 0;
    void *texs = operator new(0xc);
    Array_int_ctor(texs);
    planetTextures = (int32_t)(uint32_t)texs;
    ArraySetLength_int(list->size, texs);
    Array<Station *> *stations = SolarSystem_getStations((SolarSystem *)(void *)(uint32_t)system);
    unsigned i = 0;
    while (true) {
        if (SolarSystem_getStations((SolarSystem *)(void *)(uint32_t)system)->size <= i) {
            break;
        }
        Station *cur;
        unsigned j = 0;
        while (true) {
            if (list->size <= j) {
                goto next;
            }
            cur = ((Station **)list->data)[j];
            int target = ((int *)stations->data)[i];
            int idx = Station_getIndex(cur);
            j = j + 1;
            if (target == idx) {
                break;
            }
        }
        {
            int ccm = currentCampaignMission;
            String *nm = (String *)operator new(0xc);
            if (ccm == 0) {
                String_ctor_empty(nm);
            } else {
                Station_getName(nm, cur);
            }
            ((String **)(*(int *)((char *)((Array<String *> *)(void *)(uint32_t)planetNames) + 4)))[i] = nm;
            ((int *)(*(int *)((char *)((void *)(uint32_t)planetTextures) + 4)))[i] = Station_getTextureIndex(cur);
        }
    next:
        i = i + 1;
    }
    ArrayReleaseClasses_Station(list);
    Array_Station_dtor_tail(list);
}
