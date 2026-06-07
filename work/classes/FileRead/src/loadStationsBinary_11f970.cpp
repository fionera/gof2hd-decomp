#include "class.h"

__attribute__((minsize))
Array<Station *> *FileRead::loadStationsBinary(int16_t *ids, int32_t count)
{
    Array<Station *> *stations = new Array<Station *>();
    ArraySetLength<Station *>(count, stations);

    uint32_t exists;
    {
        String path("stations_binary.bin", false);
        exists = AEFile::FileExist(path);
    }
    if (exists == 0) {
        return 0;
    }

    uint32_t handle;
    AEFile::OpenRead("stations_binary.bin", &handle);

    String name;
    int32_t out = 0;
    for (uint32_t stationId = 0; stationId < 0x87; stationId++) {
        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        AEFile::ReadSwitched(name, handle);
        name.ConvertFromUTF8();
        AEFile::ReadSwitched(a, handle);
        AEFile::ReadSwitched(b, handle);
        AEFile::ReadSwitched(c, handle);
        AEFile::ReadSwitched(d, handle);
        for (int32_t i = 0; i < count; i++) {
            if (stationId == (uint32_t)ids[i]) {
                Station *station = new Station(name, a, b, c, d);
                stations->data[out] = station;
                out++;
            }
        }
    }
    AEFile::Close(handle);
    return stations;
}
