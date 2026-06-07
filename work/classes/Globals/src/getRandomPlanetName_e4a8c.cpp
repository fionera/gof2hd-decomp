#include "class.h"

struct FileRead;
struct Station;
extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void FileRead_ctor(FileRead *p);
extern "C" int AERandom_nextInt(int rng, int bound);
extern "C" Station *FileRead_loadStation(FileRead *p, int which);
extern "C" void Station_getName(String *ret, Station *s);
extern "C" void *Station_dtor(Station *s);
extern "C" void *FileRead_dtor(FileRead *p);
extern void *const gPlanetRng __attribute__((visibility("hidden")));

// r0 = sret buffer for the returned name String; it is returned (r0 preserved -> the final
// delete is a regular call, not a tail call).
extern "C" String *Globals_getRandomPlanetName(String *ret)
{
    FileRead *f = (FileRead *)operator_new(1);
    FileRead_ctor(f);
    int which = AERandom_nextInt(*(int *)gPlanetRng, 0x64);
    Station *st = FileRead_loadStation(f, which);
    Station_getName(ret, st);
    if (st != 0) {
        operator_delete(Station_dtor(st));
    }
    operator_delete(FileRead_dtor(f));
    return ret;
}
