#include "class.h"

__attribute__((minsize))
Array<int32_t> *FileRead::loadStaticCollision(int32_t id)
{
    uint32_t exists;
    {
        String path("static_collision.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<int32_t> *result = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("static_collision.bin", &handle);

        int32_t object = 0;
        uint32_t count = 0;
        for (uint32_t i = 0; i < 7; i++) {
            AEFile::Read(object, handle);
            AEFile::Read((int32_t &)count, handle);
            count++;
            if (object == id) {
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
