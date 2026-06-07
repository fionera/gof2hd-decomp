#include "class.h"

__attribute__((minsize))
Array<SpacePoint *> *FileRead::loadSpacePoints(int32_t id, int32_t group)
{
    uint32_t exists;
    {
        String path("space_points.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<SpacePoint *> *points = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("space_points.bin", &handle);

        uint16_t current = 0;
        uint16_t count = 0;
        do {
            AEFile::Read(current, handle);
            AEFile::Read(count, handle);
            if ((uint32_t)current != (uint32_t)id) {
                AEFile::Skip((uint32_t)count * 0x26, handle);
            }
        } while ((uint32_t)current != (uint32_t)id);

        points = new Array<SpacePoint *>();

        Matrix matrix;
        Matrix rotation;
        Vector direction;
        Vector position;
        uint32_t (*readFloat)(float &, uint32_t) = &AEFile::ReadSwitched;
        const float angleScale = 0.017453292f;
        const float rollScale = -0.017453292f;

        for (uint32_t i = 0; i < count; i++) {
            uint16_t type;
            float a;
            float b;
            float c;
            float rx;
            float ry;
            float rz;
            float sx;
            float sy;
            float sz;

            AEFile::Read(type, handle);
            readFloat(a, handle);
            readFloat(b, handle);
            readFloat(c, handle);
            readFloat(rx, handle);
            readFloat(ry, handle);
            readFloat(rz, handle);
            readFloat(sx, handle);
            readFloat(sy, handle);
            readFloat(sz, handle);

            position.x = a;
            position.y = c;
            position.z = -b;

            matrix.m[0] = 1.0f;
            matrix.m[1] = 0.0f;
            matrix.m[2] = 0.0f;
            matrix.m[3] = 0.0f;
            matrix.m[4] = 0.0f;
            matrix.m[5] = 1.0f;
            matrix.m[6] = 0.0f;
            matrix.m[7] = 0.0f;
            matrix.m[8] = 0.0f;
            matrix.m[9] = 0.0f;
            matrix.m[10] = 1.0f;
            matrix.m[11] = 0.0f;
            matrix.m[12] = 0.0f;
            matrix.m[13] = 0.0f;
            matrix.m[14] = 0.0f;
            matrix.m[15] = 1.0f;

            MatrixSetRotation(&rotation, &matrix, rx * angleScale, rz * angleScale, ry * rollScale, 1);
            matrix = rotation;
            MatrixGetDir((Vector *)&rotation, &matrix);
            direction = *(Vector *)&rotation;

            uint32_t selected = 0;
            uint32_t requested = group;
            if (group != -1) {
                selected = i >> 1;
            }
            if (group == -1 || selected == requested) {
                SpacePoint *point = new SpacePoint(type, &position, &direction, i);
                ArrayAdd<SpacePoint *>(point, points);
            }
        }
        AEFile::Close(handle);
    }
    return points;
}
