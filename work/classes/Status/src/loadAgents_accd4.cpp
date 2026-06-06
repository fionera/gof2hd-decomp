#include "class.h"

struct FileRead { FileRead(); ~FileRead(); };
extern "C" void FileRead_ctor(FileRead *);
extern "C" int FileRead_loadAgents();
extern "C" void FileRead_dtor(FileRead *);
extern "C" void operator_delete_tail(void *);

// Reads the agent roster from disk via a transient FileRead helper.
void Status::loadAgents() {
    FileRead *fr = (FileRead *)operator new(1);
    FileRead_ctor(fr);
    agents = (Array<Agent *> *)FileRead_loadAgents();
    FileRead_dtor(fr);
    operator_delete_tail(fr);
}
