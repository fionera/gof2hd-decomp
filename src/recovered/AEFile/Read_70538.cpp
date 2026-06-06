#include "class.h"

extern FileInterface *g_AEFile_fileInterface;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;

uint32_t AEFile::Read(uint32_t bytes, void *buffer, uint32_t handle)
{
    if (g_AEFile_fileInterface != 0 && handle < g_AEFile_openFiles->size) {
        AELowLevelFile *file = g_AEFile_openFiles->data[handle];
        if (file != 0) {
            return file->vtable->Read(file, bytes, buffer);
        }
    }
    return 0;
}

namespace {
__attribute__((used)) void ae_file_70538_trailer() {}
}
