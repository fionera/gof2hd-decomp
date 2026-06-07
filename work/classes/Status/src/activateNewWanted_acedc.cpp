#include "class.h"

struct FileRead { FileRead(); ~FileRead(); };
struct SystemPathFinder;
struct SolarSystem;

__attribute__((visibility("hidden"))) extern Status **g_anwStatus;     // bceec
__attribute__((visibility("hidden"))) extern int g_anwSysMask;          // bd290
__attribute__((visibility("hidden"))) extern int **g_anwRandStation;    // bd294
__attribute__((visibility("hidden"))) extern int *g_anwRandom;          // bd1dc

extern "C" int __aeabi_idiv(int a, int b);
extern "C" int __aeabi_idivmod(int a, int b);
extern "C" int SolarSystem_getRace(int sys);
extern "C" int Wanted_getBoard(Wanted *w);
extern "C" int Wanted_getRequiredMission(Wanted *w);
extern "C" int Station_getIndex(Station *s);
extern "C" int Wanted_isActive(Wanted *w);
extern "C" int Wanted_isTerminated(Wanted *w);
extern "C" int Wanted_getRequiredBounties(Wanted *w);
extern "C" void Wanted_setActive(Wanted *w, bool v);
extern "C" void SystemPathFinder_ctor(SystemPathFinder *p);
extern "C" void *SystemPathFinder_dtor(SystemPathFinder *p);
extern "C" void FileRead_ctor(FileRead *fr);
extern "C" int FileRead_loadSystemsBinary();
extern "C" void *FileRead_dtor(FileRead *fr);
extern "C" void operator_delete(void *p);
extern "C" Station *Globals_getRandomStation();
extern "C" int Station_getSystem(Station *s);
extern "C" void *Station_dtor(Station *s);
extern "C" int SolarSystem_getRoutes(SolarSystem *s);
extern "C" void Wanted_setLastSeen(Wanted *w, int v);
extern "C" void Wanted_setTravelsTo(Wanted *w, int v);
extern "C" int AERandom_nextInt(int rnd);
extern "C" int *SystemPathFinder_getSystemPath(SystemPathFinder *p, void *systems, int from, int to);
extern "C" int SolarSystem_getStations_i(SolarSystem *s);
extern "C" void Wanted_setCurrentLocation(Wanted *w, int v);
extern "C" void *Array_int_dtor(void *a);
extern "C" void ArrayReleaseClasses_SolarSystem(void *a);
extern "C" void *Array_SolarSystem_dtor(void *a);

// Walks the wanted roster and activates any newly-eligible bounty, routing it onto a random
// reachable station pair. Returns the number of bounties activated.
int Status::activateNewWanted() {
    Status **slot = g_anwStatus;
    unsigned *w = (unsigned *)(*slot)->wanted;
    if (w == 0) {
        return 0;
    }
    void *systems = 0;
    int activated = 0;
    SystemPathFinder *pf = 0;
    for (unsigned i = 0; i < *w; i = i + 1) {
        Wanted *cur = ((Wanted **)w[1])[i];
        if (cur == 0) {
            continue;
        }
        unsigned lo, hi;
        if (i < 2) {
            hi = 4;
            lo = 2;
        } else {
            int rem = __aeabi_idivmod(i - 1, 6);
            lo = __aeabi_idiv(rem, 3) + 2;
            hi = rem / 2 + 4;
        }
        if (SolarSystem_getRace((*slot)->system) != Wanted_getBoard(cur)) {
            continue;
        }
        if ((*slot)->currentCampaignMission < Wanted_getRequiredMission(cur)) {
            continue;
        }
        if (i < 2 && (*slot)->currentCampaignMission > Wanted_getRequiredMission(cur)) {
            continue;
        }
        if ((*slot)->inAlienOrbit() != 0) {
            continue;
        }
        if (Station_getIndex((*slot)->station) == 0x6c) {
            continue;
        }
        if (Wanted_isActive(cur) != 0) {
            continue;
        }
        if (Wanted_isTerminated(cur) != 0) {
            continue;
        }
        int board = Wanted_getBoard(cur);
        int avail = (board < 4) ? (*slot)->collectedBounties[board] : 0;
        if (avail < Wanted_getRequiredBounties(cur)) {
            continue;
        }
        Wanted_setActive(cur, true);
        if (pf == 0) {
            pf = (SystemPathFinder *)operator new(1);
            SystemPathFinder_ctor(pf);
            FileRead *fr = (FileRead *)operator new(1);
            FileRead_ctor(fr);
            systems = (void *)FileRead_loadSystemsBinary();
            operator_delete(FileRead_dtor(fr));
        }
        activated = activated + 1;
        int *path;
        int fromSys, toSys;
        do {
            Station *a = Globals_getRandomStation();
            unsigned asys = Station_getSystem(a);
            unsigned *vis = (unsigned *)(*slot)->systemVisibilities;
            bool ok;
            while (true) {
                ok = false;
                if (vis != 0 && asys < *vis) {
                    ok = *(char *)(vis[1] + asys) != 0;
                }
                if (SolarSystem_getRoutes((SolarSystem *)(void *)asys) != 0 && ok &&
                    !(asys < 0x1d && (1 << (asys & 0xff) & g_anwSysMask) != 0)) {
                    break;
                }
                if (a != 0) {
                    operator_delete(Station_dtor(a));
                }
                a = Globals_getRandomStation();
                asys = Station_getSystem(a);
            }
            Station *b = Globals_getRandomStation();
            unsigned bsys = Station_getSystem(b);
            bool ok2;
            while (true) {
                ok2 = false;
                if (vis != 0 && bsys < *vis) {
                    ok2 = *(char *)(vis[1] + bsys) != 0;
                }
                if (SolarSystem_getRoutes((SolarSystem *)(void *)bsys) != 0 && ok2 &&
                    !(bsys < 0x1d && (1 << (bsys & 0xff) & g_anwSysMask) != 0) && asys != bsys) {
                    break;
                }
                if (b != 0) {
                    operator_delete(Station_dtor(b));
                }
                b = Globals_getRandomStation();
                bsys = Station_getSystem(b);
            }
            Wanted_setLastSeen(cur, Station_getIndex(a));
            Wanted_setTravelsTo(cur, Station_getIndex(b));
            fromSys = Station_getSystem(a);
            toSys = Station_getSystem(b);
            if (a != 0) {
                operator_delete(Station_dtor(a));
            }
            if (b != 0) {
                operator_delete(Station_dtor(b));
            }
            path = SystemPathFinder_getSystemPath(pf, systems, fromSys, toSys);
        } while (path == 0 || (unsigned)*path < lo || hi < (unsigned)*path);
        int *rnd = g_anwRandom;
        int pick = path[1] + AERandom_nextInt(*rnd) * 4;
        pick = *(int *)pick;
        SolarSystem *dst = (SolarSystem *)((int *)((unsigned *)systems)[1])[pick];
        if (SolarSystem_getStations_i(dst) != 0) {
            int idx = AERandom_nextInt(*rnd);
            int st = ((int *)(*(int *)(SolarSystem_getStations_i(dst) + 4)))[idx];
            Wanted_setCurrentLocation(cur, st);
        }
        operator_delete(Array_int_dtor(path));
    }
    if (pf != 0) {
        operator_delete(SystemPathFinder_dtor(pf));
    }
    if (systems != 0) {
        ArrayReleaseClasses_SolarSystem(systems);
        operator_delete(Array_SolarSystem_dtor(systems));
    }
    return activated;
}
