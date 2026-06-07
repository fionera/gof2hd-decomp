#include "class.h"

__attribute__((minsize))
Array<Wanted *> *FileRead::loadWanted()
{
    uint32_t exists;
    {
        String path("wanted_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<Wanted *> *wanted = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("wanted_binary.bin", &handle);

        wanted = new Array<Wanted *>();
        ArraySetLength<Wanted *>(0x19, wanted);

        String name;
        uint32_t index = 0;
        while (index < wanted->size) {
            int32_t id;
            int32_t a;
            int32_t b;
            int32_t c;
            int32_t d;
            int32_t e;
            int32_t f;
            int32_t g;
            int32_t h;
            int32_t i;
            int32_t j;
            int32_t k;
            int32_t l;
            int32_t imageCount;

            AEFile::ReadSwitched(name, handle);
            name.ConvertFromUTF8();
            AEFile::ReadSwitched(id, handle);
            AEFile::ReadSwitched(a, handle);
            AEFile::ReadSwitched(b, handle);
            AEFile::ReadSwitched(c, handle);
            AEFile::ReadSwitched(d, handle);
            AEFile::ReadSwitched(e, handle);
            AEFile::ReadSwitched(f, handle);
            AEFile::ReadSwitched(g, handle);
            AEFile::ReadSwitched(h, handle);
            AEFile::ReadSwitched(i, handle);
            AEFile::ReadSwitched(j, handle);
            AEFile::ReadSwitched(k, handle);
            AEFile::ReadSwitched(l, handle);

            Wanted *entry = new Wanted(id, name, a, b, c == 1, d, e, f, g, h, i, j, k, l);
            wanted->data[index] = entry;

            AEFile::ReadSwitched(imageCount, handle);
            uint32_t oldIndex = index++;
            if (imageCount > 0) {
                int32_t *parts = new int32_t[5];
                for (uint32_t n = 0; n < 5; n++) {
                    char part;
                    AEFile::Read(part, handle);
                    parts[n] = part;
                }
                wanted->data[oldIndex]->setImageParts(parts);
            }
        }
        AEFile::Close(handle);
    }
    return wanted;
}
