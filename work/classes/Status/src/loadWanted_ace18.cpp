#include "class.h"

struct FileRead { FileRead(); ~FileRead(); };
extern "C" void FileRead_ctor(FileRead *);
extern "C" int FileRead_loadWanted();
extern "C" void FileRead_dtor(FileRead *);
extern "C" void operator_delete_tail(void *);

// Reads the wanted/bounty list from disk via a transient FileRead helper.
void Status::loadWanted() {
    FileRead *fr = (FileRead *)operator new(1);
    FileRead_ctor(fr);
    wanted = (Array<Wanted *> *)FileRead_loadWanted();
    FileRead_dtor(fr);
    operator_delete_tail(fr);
}
