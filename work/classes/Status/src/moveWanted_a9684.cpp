#include "class.h"

struct FileRead { FileRead(); };
struct SystemPathFinder;

__attribute__((visibility("hidden"))) extern Status **g_mwStatus;     // DAT_000b99d4
__attribute__((visibility("hidden"))) extern Station **g_mwPrevStation; // DAT_000b99d8

extern "C" {
int Wanted_isActive_mw(Wanted *w);                                   // 0x73774
int Wanted_isTerminated_mw(Wanted *w);                               // 0x71a04
int Status_inAlienOrbit_mw();                                        // 0x723d0
int Wanted_getCurrentLocation_mw(Wanted *w);                         // 0x73780
int Station_getIndex(Station *s);                                    // 0x71824
void FileRead_ctor(FileRead *fr);                                    // 0x72124
void *FileRead_loadSystemsBinary_mw(FileRead *fr);                   // 0x7378c
void *FileRead_dtor_mw(FileRead *fr);                                // 0x7213c
void operator_delete(void *p);                                       // 0x6eb48
void SystemPathFinder_ctor(SystemPathFinder *p);                     // 0x73798
void *SystemPathFinder_dtor(SystemPathFinder *p);                    // 0x73810
void *FileRead_loadStation_mw(int sysAndIndex);                      // 0x737a4
int Station_getSystem_mw(void *s);                                   // 0x7189c
int Wanted_getTravelsTo_mw(Wanted *w);                               // 0x737b0
void Wanted_setLastSeen_mw(Wanted *w, int loc);                      // 0x737c8
void Wanted_setCurrentLocation_mw(Wanted *w, int loc);               // 0x737bc
void Wanted_setTravelsTo_mw(Wanted *w, int loc);                     // 0x737ec
void *Station_dtor_mw(void *s);                                      // 0x7360c
void *Globals_getRandomStation_mw();                                 // 0x737d4
void *SystemPathFinder_getSystemPath(SystemPathFinder *p, void *sys, int from, int to); // 0x737e0
int SolarSystem_getRoutes_mw();                                      // 0x71aac
int SolarSystem_getWarpGateIndex_mw(void *ss);                       // 0x737f8
void ArrayReleaseClasses_SolarSystem(void *a);                       // 0x73804
void *Array_SolarSystem_dtor(void *a);                               // ... part of dtor chain
void *Array_int_dtor_mw(void *a);                                    // 0x70204
int aeabi_idiv_mw(int a, int b);                                     // 0x7198c
int aeabi_idivmod_mw(int a, int b);                                  // 0x6f514
void Status_moveWantedTail();                                        // tail 0x1ab098
}

// Status::moveWanted()
//   Advances each active, non-terminated wanted criminal one hop along a pathfound route toward
//   its destination (subject to system-visibility and forbidden-system rules), updating its
//   current location / travels-to / last-seen state.
void Status::moveWanted()
{
    Status *self = this;
    bool loaded = false;
    void *systemsTable = 0;
    SystemPathFinder *pf = 0;
    Status **statusHolder = g_mwStatus;
    Station **prevHolder = g_mwPrevStation;

    for (unsigned i = 0; i < *(unsigned *)self->wanted; i = i + 1) {
        Wanted *w = ((Wanted **)(*(char **)((char *)self->wanted + 4)))[i];
        if (Wanted_isActive_mw(w) == 0)
            continue;
        if (Wanted_isTerminated_mw(((Wanted **)(self->wanted->data))[i]) != 0)
            continue;
        if (Status_inAlienOrbit_mw() != 0)
            continue;

        int loc = Wanted_getCurrentLocation_mw(((Wanted **)(self->wanted->data))[i]);
        if (loc == Station_getIndex((*statusHolder)->station))
            continue;
        if (*prevHolder != 0) {
            if (Station_getIndex(*prevHolder) ==
                Wanted_getCurrentLocation_mw(((Wanted **)(self->wanted->data))[i]))
                continue;
        }

        // lazily load the systems table + pathfinder on first need.
        if (!loaded) {
            FileRead fr;
            FileRead_ctor(&fr);
            systemsTable = FileRead_loadSystemsBinary_mw(&fr);
            operator_delete(FileRead_dtor_mw(&fr));
            pf = (SystemPathFinder *)operator new(1);
            SystemPathFinder_ctor(pf);
            loaded = true;
        }

        // resolve the from/to stations.
        Wanted_getCurrentLocation_mw(((Wanted **)(self->wanted->data))[i]);
        void *fromSt = FileRead_loadStation_mw(0);
        int fromSys = Station_getSystem_mw(fromSt);
        Wanted_getTravelsTo_mw(((Wanted **)(self->wanted->data))[i]);
        void *toSt = FileRead_loadStation_mw(0);
        int toSys = Station_getSystem_mw(toSt);

        void *path = 0;
        if (Station_getIndex((Station *)fromSt) == Station_getIndex((Station *)toSt)) {
            // already at destination system: re-roll a random reachable target.
            unsigned lo, hi;
            if (i < 2) { hi = 4; lo = 2; }
            else { int r = aeabi_idivmod_mw(i - 1, 6); lo = aeabi_idiv_mw(r, 3) + 2; hi = r / 2 + 4; }
            Wanted *pw = ((Wanted **)(self->wanted->data))[i];
            Wanted_setLastSeen_mw(pw, Wanted_getCurrentLocation_mw(pw));
            if (toSt != 0) operator_delete(Station_dtor_mw(toSt));
            toSt = Globals_getRandomStation_mw();
            toSys = Station_getSystem_mw(toSt);
            for (;;) {
                path = SystemPathFinder_getSystemPath(pf, systemsTable, fromSys, toSys);
                Station_getSystem_mw(toSt);
                int routes = SolarSystem_getRoutes_mw();
                int dsys = Station_getSystem_mw(toSt);
                bool ok = path != 0 && routes != 0 &&
                          *(unsigned *)path >= lo && *(unsigned *)path <= hi &&
                          ((char *)(*statusHolder)->systemVisibilities->data)[dsys] != 0 &&
                          dsys != 0x1b && dsys != 0x1c && dsys != 0x19 && dsys != 0x1a && dsys != 6 &&
                          dsys != Station_getSystem_mw(fromSt);
                if (ok) {
                    Wanted_setTravelsTo_mw(((Wanted **)(self->wanted->data))[i],
                                           Station_getIndex((Station *)toSt));
                    break;
                }
                if (toSt != 0) operator_delete(Station_dtor_mw(toSt));
                toSt = Globals_getRandomStation_mw();
                toSys = Station_getSystem_mw(toSt);
            }
        } else if (fromSys == toSys) {
            Wanted *pw = ((Wanted **)(self->wanted->data))[i];
            Wanted_setCurrentLocation_mw(pw, Wanted_getTravelsTo_mw(pw));
            path = 0;
        } else {
            path = SystemPathFinder_getSystemPath(pf, systemsTable, fromSys, toSys);
            int wg = SolarSystem_getWarpGateIndex_mw(
                *(void **)(*(int *)((char *)systemsTable + 4) + *(int *)(*(int *)((char *)path + 4) + 4) * 4));
            Wanted_setCurrentLocation_mw(((Wanted **)(self->wanted->data))[i], wg);
        }

        if (toSt != 0) operator_delete(Station_dtor_mw(toSt));
        if (fromSt != 0) operator_delete(Station_dtor_mw(fromSt));
        if (path != 0) operator_delete(Array_int_dtor_mw(path));
    }

    if (systemsTable != 0) {
        ArrayReleaseClasses_SolarSystem(systemsTable);
        operator_delete(Array_SolarSystem_dtor(systemsTable));
    }
    if (pf != 0) {
        SystemPathFinder_dtor(pf);
        Status_moveWantedTail();
    }
}
