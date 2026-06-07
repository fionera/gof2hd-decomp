#include "class.h"

__attribute__((minsize))
Array<Array<Vector *> *> *FileRead::loadWeaponPositions(int32_t id)
{
    uint32_t exists;
    {
        String path("weapon_positions.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<Array<Vector *> *> *positions = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("weapon_positions.bin", &handle);

        positions = new Array<Array<Vector *> *>();
        ArraySetLength<Array<Vector *> *>(4, positions);
        for (uint32_t i = 0; i != positions->size; i++) {
            positions->data[i] = 0;
        }

        int16_t ship = 0;
        int16_t count = 0;
        int16_t type = 0;
        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;
        float extraX = 0.0f;
        float extraY = 0.0f;
        float extraZ = 0.0f;
        uint32_t (*readShort)(int16_t &, uint32_t) = &AEFile::ReadSwitched;
        uint32_t (*readFloat)(float &, uint32_t) = &AEFile::ReadSwitched;

        do {
            AEFile::Read(ship, handle);
            AEFile::Read(count, handle);
            for (int32_t i = 0; i < count; i++) {
                readShort(type, handle);
                readShort(x, handle);
                readShort(y, handle);
                readShort(z, handle);
                if (type == 3) {
                    readFloat(extraX, handle);
                    readFloat(extraY, handle);
                    readFloat(extraZ, handle);
                }
                if (ship == id) {
                    if (positions->data[type] == 0) {
                        positions->data[type] = new Array<Vector *>();
                    }
                    Vector *pos = new Vector;
                    pos->x = (float)x;
                    pos->y = (float)z;
                    pos->z = (float)-y;
                    ArrayAdd<Vector *>(pos, positions->data[type]);
                    if (type == 3) {
                        Vector *extra = new Vector;
                        extra->x = extraX;
                        extra->y = extraZ;
                        extra->z = extraY;
                        ArrayAdd<Vector *>(extra, positions->data[3]);
                    }
                }
            }
        } while (ship != id);
        AEFile::Close(handle);
    }
    return positions;
}
