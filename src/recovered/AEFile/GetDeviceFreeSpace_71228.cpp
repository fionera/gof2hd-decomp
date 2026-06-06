#include "class.h"

extern FileInterface *fileInterface;

__attribute__((minsize))
uint32_t AEFile::GetDeviceFreeSpace() {
    FileInterface *interface = fileInterface;
    if (interface != 0) {
        return interface->vtable->GetDeviceFreeSpace(interface);
    }
    return 0;
}
