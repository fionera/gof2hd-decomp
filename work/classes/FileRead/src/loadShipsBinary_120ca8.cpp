#include "class.h"

__attribute__((minsize))
Array<Ship *> *FileRead::loadShipsBinary()
{
    uint32_t exists;
    {
        String path("ships_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<Ship *> *ships = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("ships_binary.bin", &handle);

        ships = new Array<Ship *>();
        ArraySetLength<Ship *>(0x40, ships);

        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;
        for (uint32_t i = 0; i < 0x40; i++) {
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t d;
            int32_t e;
            int32_t f;
            int32_t g;
            int32_t h;
            int32_t speed;
            readInt(a, handle);
            readInt(b, handle);
            readInt(c, handle);
            readInt(d, handle);
            readInt(e, handle);
            readInt(f, handle);
            readInt(g, handle);
            readInt(h, handle);
            readInt(speed, handle);
            Ship *ship = new Ship(a, b, c, d, e, f, g, h, (float)speed);
            ships->data[i] = ship;
        }
        AEFile::Close(handle);
    }
    return ships;
}
