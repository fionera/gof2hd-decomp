#include "class.h"

extern "C" void FileInterfaceAndroid_ctor(void *self);
extern "C" void AEFile_SetInterface(void *fileInterface);

extern "C" void Engine_initFileInterface(Engine *self)
{
    void *fileInterface = operator new(0x38);
    FileInterfaceAndroid_ctor(fileInterface);
    *(void **)((char *)self + 0x24) = fileInterface;
    return AEFile_SetInterface(fileInterface);
}
