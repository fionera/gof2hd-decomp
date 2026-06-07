#include "class.h"

extern "C" int AEFile_GetDeviceFreeSpace();

// RecordHandler::notEnoughMemory() -> bool: free space < 900.
extern "C" uint8_t RecordHandler_notEnoughMemory()
{
    return AEFile_GetDeviceFreeSpace() < 900;
}
