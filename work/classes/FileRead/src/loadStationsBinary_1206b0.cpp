#include "class.h"

__attribute__((minsize))
int32_t FileRead::loadStationsBinary()
{
    uint32_t exists;
    {
        String path("stations_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("stations_binary.bin", &handle);
        ArraySetLength<int8_t>(0x195, (Array<int8_t> *)0);

        int32_t color2;
        int32_t color1;
        int32_t color0;
        int32_t offset = 0;
        String name;
        for (uint32_t i = 0; i < 0x87; i++) {
            AEFile::ReadSwitched(name, handle);
            AEFile::ReadSwitched(offset, handle);
            AEFile::ReadSwitched(color0, handle);
            AEFile::ReadSwitched(color1, handle);
            AEFile::ReadSwitched(color2, handle);
            char *colors = *(char **)4;
            colors[offset++] = (char)color0;
            colors[offset++] = (char)color1;
            colors[offset++] = (char)color2;
        }
        AEFile::Close(handle);
    }
    return 0;
}
