#include "class.h"

__attribute__((minsize))
Array<Station *> *FileRead::loadStationsBinary(SolarSystem *system)
{
    Array<Station *> *stations = new Array<Station *>();

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

    Array<int32_t> *ids = system->getStations();
    ArraySetLength<Station *>(ids->size, stations);

    String name;
    uint32_t out = 0;
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
        for (uint32_t i = 0; i < ids->size; i++) {
            if ((uint32_t)ids->data[i] == stationId) {
                Station *station = new Station(name, a, b, c, d);
                stations->data[out++] = station;
            }
            if (out == ids->size) {
                AEFile::Close(handle);
                return stations;
            }
        }
    }
    AEFile::Close(handle);
    return stations;
}
