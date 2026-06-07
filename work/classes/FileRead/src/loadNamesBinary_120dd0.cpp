#include "class.h"

extern AbyssEngine::AERandom *gNameRandomA;
extern AbyssEngine::AERandom *gNameRandomB;

__attribute__((minsize))
Array<String *> *FileRead::loadNamesBinary(int32_t type, bool first, bool second)
{
    Array<String *> *names = 0;
    String path("", false);

    switch (type) {
    case 0: {
        const char *text = "names_0_ab";
        if (!first) {
            text = "names_0_b";
        }
        if (!second) {
            text = "names_0_a";
        }
        String tmp(text, false);
        path = tmp;
        break;
    }
    case 1: {
        const char *text = "names_1_b";
        if (!second) {
            text = "names_1_a";
        }
        String tmp(text, false);
        path = tmp;
        break;
    }
    case 2: {
        const char *text = "names_2_b";
        if (!second) {
            text = "names_2_a";
        }
        String tmp(text, false);
        path = tmp;
        break;
    }
    case 3: {
        if (second) {
            int32_t value = gNameRandomA->nextInt(2);
            const char *text = "names_3_a";
            if (value != 0) {
                text = "names_3_b";
            }
            String tmp(text, false);
            path = tmp;
        } else {
            int32_t value = gNameRandomA->nextInt(2);
            const char *text = "names_3_c";
            if (value != 0) {
                text = "names_3_d";
            }
            String tmp(text, false);
            path = tmp;
        }
        break;
    }
    case 4: {
        const char *text = "names_4_b";
        if (!second) {
            text = "names_4_a";
        }
        String tmp(text, false);
        path = tmp;
        break;
    }
    case 5: {
        if (!second) {
            goto done;
        }
        String tmp("names_5", false);
        path = tmp;
        break;
    }
    case 6: {
        const char *text = "names_6_b";
        if (!second) {
            text = "names_6_a";
        }
        String tmp(text, false);
        path = tmp;
        break;
    }
    case 7: {
        if (!second) {
            goto done;
        }
        String tmp("names_7", false);
        path = tmp;
        break;
    }
    case 8: {
        if (second) {
            int32_t value = gNameRandomB->nextInt(2);
            const char *text = "names_8_a";
            if (value != 0) {
                text = "names_8_b";
            }
            String tmp(text, false);
            path = tmp;
        } else {
            int32_t value = gNameRandomB->nextInt(2);
            const char *text = "names_8_c";
            if (value != 0) {
                text = "names_8_d";
            }
            String tmp(text, false);
            path = tmp;
        }
        break;
    }
    default:
        goto done;
    }

    {
        String suffix(".bin", false);
        String combined = suffix + path;
        path = combined;
    }

    if (AEFile::FileExist(path) != 0) {
        uint32_t handle;
        uint32_t count;
        AEFile::OpenRead(path, &handle);
        AEFile::ReadSwitched((int32_t &)count, handle);

        names = new Array<String *>();
        ArraySetLength<String *>(count, names);

        String tmp;
        for (int32_t i = 0; i < (int32_t)count; i++) {
            AEFile::ReadSwitched(tmp, handle);
            String *entry = new String(tmp, false);
            names->data[i] = entry;
        }
        AEFile::Close(handle);
    }

done:
    return names;
}
