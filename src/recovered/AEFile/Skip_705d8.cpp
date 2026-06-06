#include "class.h"

extern FileInterface *g_fileInterface;
extern Array<AELowLevelFile *> *g_openFiles;

uint32_t AEFile::Skip(uint32_t bytes, uint32_t handle)
{
    if (g_fileInterface != 0) {
        Array<AELowLevelFile *> *files = g_openFiles;
        if (handle < files->size) {
            AELowLevelFile *file = files->data[handle];
            if (file != 0) {
                return file->vtable->Skip(file, bytes);
            }
        }
    }
    return 0;
}
