#include "class.h"

extern FileInterface *g_fileInterface;
extern Array<AELowLevelFile *> *g_openFiles;

__attribute__((minsize)) void AEFile::Close(uint32_t handle) {
    if (g_fileInterface != 0) {
        Array<AELowLevelFile *> *files = g_openFiles;
        if (files->size <= handle) {
            goto done;
        }
        AELowLevelFile **slot = &files->data[handle];
        AELowLevelFile *file = *slot;
        if (file != 0) {
            file->vtable->Close(file);
            slot = &g_openFiles->data[handle];
        }
        *slot = 0;
    }
done:
    return;
}
