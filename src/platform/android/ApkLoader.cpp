

extern "C" void *zip_open(const char *path, int flags, int *errorp);

extern "C" int zip_get_num_files(void *za);

extern "C" const char *zip_get_name(void *za, int index, int flags);

extern void **gZipMain __attribute__((visibility("hidden")));
extern void **gZipPatch __attribute__((visibility("hidden")));

extern "C" int loadAPK(const char *path) {
    void *za = zip_open(path, 0, 0);
    *gZipMain = za;
    if (za == 0)
        return 0;

    int count = zip_get_num_files(za);
    for (int i = 0; i < count; ++i) {
        if (zip_get_name(*gZipMain, i, 0) == 0)
            break;
    }
    return 1;
}

extern "C" int loadAPKAndZip(const char *apkPath, const char *patchPath) {
    void *apk = zip_open(apkPath, 0, 0);
    *gZipMain = apk;

    void *patch = zip_open(patchPath, 0, 0);
    *gZipPatch = patch;

    return apk != 0 && patch != 0;
}
