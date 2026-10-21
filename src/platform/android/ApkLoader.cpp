#include "engine/file/FileInterfaceAndroid.h"

extern "C" int loadAPK(const char *path) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    struct zip *za = zip_open(path, 0, 0);
    APKArchive = za;
    if (za == 0)
        return 0;

    int count = zip_get_num_files(za);
    for (int i = 0; i < count; ++i) {
        if (zip_get_name(APKArchive, i, 0) == 0)
            break;
    }
    return 1;
}

extern "C" int loadAPKAndZip(const char *apkPath, const char *patchPath) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    APKArchive = zip_open(apkPath, 0, 0);
    ZIPArchive = zip_open(patchPath, 0, 0);
    return APKArchive != 0 && ZIPArchive != 0;
}
