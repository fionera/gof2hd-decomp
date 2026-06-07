#include "class.h"

__attribute__((minsize))
Array<Item *> *FileRead::loadItemsBinary()
{
    uint32_t exists;
    {
        String path("items_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<Item *> *items = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("items_binary.bin", &handle);

        items = new Array<Item *>();
        ArraySetLength<Item *>(0xe9, items);

        uint32_t count0 = 0;
        uint32_t count1 = 0;
        uint32_t count2 = 0;
        for (uint32_t i = 0; i < 0xe9; i++) {
            Array<int32_t> *a0;
            Array<int32_t> *a1;
            Array<int32_t> *a2;

            AEFile::ReadSwitched((int32_t &)count0, handle);
            if ((int32_t)count0 < 1) {
                a0 = 0;
            } else {
                a0 = new Array<int32_t>();
                ArraySetLength<int32_t>(count0, a0);
                for (int32_t j = 0; j < (int32_t)count0; j++) {
                    AEFile::ReadSwitched(a0->data[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count1, handle);
            if ((int32_t)count1 < 1) {
                a1 = 0;
            } else {
                a1 = new Array<int32_t>();
                ArraySetLength<int32_t>(count1, a1);
                for (int32_t j = 0; j < (int32_t)count1; j++) {
                    AEFile::ReadSwitched(a1->data[j], handle);
                }
            }

            AEFile::ReadSwitched((int32_t &)count2, handle);
            if ((int32_t)count2 < 1) {
                a2 = 0;
            } else {
                a2 = new Array<int32_t>();
                ArraySetLength<int32_t>(count2, a2);
                for (int32_t j = 0; j < (int32_t)count2; j++) {
                    AEFile::ReadSwitched(a2->data[j], handle);
                }
            }

            Item *item = new Item(a0, a1, a2);
            items->data[i] = item;
        }
        AEFile::Close(handle);
    }
    return items;
}
