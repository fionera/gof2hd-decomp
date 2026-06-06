#include "class.h"

extern FileInterface *gAEFileInterface;
extern Array<AELowLevelFile *> *gAEFileOpenFiles;

uint32_t AEFile::Write(uint32_t bytes, const void *buffer, uint32_t handle) {
    if (gAEFileInterface != 0 && handle < gAEFileOpenFiles->size) {
        AELowLevelFile *file = gAEFileOpenFiles->data[handle];
        if (file != 0) {
            return file->vtable->Write(file, bytes, buffer);
        }
    }

    return 0;
}
