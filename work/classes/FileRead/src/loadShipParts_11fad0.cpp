#include "class.h"

__attribute__((minsize))
Array<int32_t> *FileRead::loadShipParts(int32_t id)
{
    uint32_t exists;
    {
        String path("ship_parts.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *parts = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("ship_parts.bin", &handle);

        uint32_t (*readShort)(int16_t &, uint32_t) = &AEFile::ReadSwitched;
        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;
        int32_t wanted = id + 1;
        char group;
        char count;

        for (uint32_t i = 0; i <= 0x40; i++) {
            if (i > 0x3f) {
                AEFile::Close(handle);
                break;
            }
            AEFile::Read(group, handle);
            AEFile::Read(count, handle);

            parts = new Array<int32_t>();
            ArraySetLength<int32_t>((int8_t)count * 10, parts);
            int32_t *data = parts->data;
            for (uint32_t j = 0, off = 0; j < parts->size; j += 10, off += 10) {
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
                readShort(value, handle);
                data[off + 7] = value;
                readShort(value, handle);
                data[off + 8] = value;
                readShort(value, handle);
                data[off + 9] = value;
            }
            if (wanted == (int8_t)group) {
                break;
            }
        }
    }
    return parts;
}
