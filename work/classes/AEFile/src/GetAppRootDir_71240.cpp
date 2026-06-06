#include "class.h"

extern FileInterface *fileInterface;

__attribute__((minsize))
const char *AEFile::GetAppRootDir()
{
    FileInterface *interface = fileInterface;
    if (interface != 0) {
        return interface->vtable->GetAppRootDir(interface);
    }
    return 0;
}
