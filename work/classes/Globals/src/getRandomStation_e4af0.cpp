#include "class.h"

struct FileRead;
extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" void FileRead_ctor(FileRead *p);
extern "C" int AERandom_nextInt(int rng, int bound);
extern "C" int FileRead_loadStation(FileRead *p, int which);
extern "C" void *FileRead_dtor(FileRead *p);
extern void *const gStationRng __attribute__((visibility("hidden")));

extern "C" int Globals_getRandomStation()
{
    FileRead *f = (FileRead *)operator_new(1);
    FileRead_ctor(f);
    int which = AERandom_nextInt(*(int *)gStationRng, 0x87);
    int r = FileRead_loadStation(f, which);
    operator_delete(FileRead_dtor(f));
    return r;
}
