#include "class.h"

__attribute__((minsize))
Array<NewsItem *> *FileRead::loadTicker()
{
    uint32_t exists;
    {
        String path("ticker_binary.bin", false);
        exists = AEFile::FileExist(path);
    }

    Array<NewsItem *> *items = 0;
    if (exists != 0) {
        uint32_t handle;
        AEFile::OpenRead("ticker_binary.bin", &handle);

        items = new Array<NewsItem *>();
        ArraySetLength<NewsItem *>(0x3b, items);

        uint32_t (*readInt)(int32_t &, uint32_t) = &AEFile::ReadSwitched;
        for (uint32_t i = 0; i < 0x3b; i++) {
            int32_t active;
            int32_t flags[4];
            int32_t a;
            int32_t b;
            readInt(active, handle);
            readInt(flags[0], handle);
            readInt(flags[1], handle);
            readInt(flags[2], handle);
            readInt(flags[3], handle);
            readInt(a, handle);
            readInt(b, handle);

            bool *bits = new bool[4];
            for (int32_t j = 0; j != 4; j++) {
                bits[j] = flags[j] != 0;
            }

            NewsItem *item = new NewsItem(i, active != 0, bits, 4, a, b);
            items->data[i] = item;
        }
        AEFile::Close(handle);
    }
    return items;
}
