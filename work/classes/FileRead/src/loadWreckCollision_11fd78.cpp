#include "class.h"

__attribute__((minsize))
Array<int32_t> *FileRead::loadWreckCollision(int32_t id)
{
    uint32_t exists;
    {
        String path("wreck_collision.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *result = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("wreck_collision.bin", &handle);

        int32_t wreck = 0;
        uint32_t count = 0;
        for (uint32_t i = 0; i < 6; i++) {
            AEFile::Read(wreck, handle);
            AEFile::Read((int32_t &)count, handle);
            count++;
            if (wreck == id) {
                result = new Array<int32_t>();
                int32_t *buffer = new int32_t[count];
                AEFile::Read(count << 2, buffer, handle);
                ArraySetLength<int32_t>(count, result);
                for (int32_t j = 0; j < (int32_t)count; j++) {
                    result->data[j] = buffer[j];
                }
                delete[] buffer;
                AEFile::Close(handle);
                return result;
            }
            AEFile::Skip(count << 2, handle);
        }
        AEFile::Close(handle);
        result = 0;
    }
    return result;
}
