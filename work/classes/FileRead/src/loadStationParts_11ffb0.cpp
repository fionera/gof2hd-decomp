#include "class.h"

__attribute__((minsize))
Array<int32_t> *FileRead::loadStationParts(int32_t id, int32_t special)
{
    uint32_t exists;
    {
        String path("station_parts.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *parts = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("station_parts.bin", &handle);

        int32_t wanted = id + 1;
        if (special == 1) {
            wanted = 0x65;
        }

        uint32_t (*readShort)(int16_t &, uint32_t) = &AEFile::ReadSwitched;
        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;

        for (uint32_t i = 0; i < 0x88; i++) {
            char group;
            char count;
            int16_t station;
            AEFile::Read(group, handle);
            AEFile::ReadSwitched(station, handle);
            AEFile::Read(count, handle);

            parts = new Array<int32_t>();
            ArraySetLength<int32_t>((int8_t)count * 7 + 7, parts);
            int32_t *data = parts->data;
            data[0] = station;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            data[4] = 0;
            data[5] = 0x800;
            data[6] = 0;

            for (uint32_t j = 7, off = 7; j < parts->size; j += 7, off += 7) {
                int16_t value;
                readShort(value, handle);
                data[off] = value;
                readInt(data[off + 1], handle);
                readInt(data[off + 2], handle);
                readInt(data[off + 3], handle);
                readShort(value, handle);
                data[off + 4] = value;
                readShort(value, handle);
                data[off + 5] = value;
                readShort(value, handle);
                data[off + 6] = value;
            }

            if (wanted == (int8_t)group) {
                return parts;
            }
            ArrayRelease<int32_t>(parts);
        }
        AEFile::Close(handle);
    }
    return parts;
}
