// APK / patch-zip bootstrap helpers.
//
// The game ships its assets inside the application's APK (a zip archive) plus an
// optional patch zip. These two free functions open those archives once at startup
// and stash the libzip handles in the global slots that FileInterfaceAndroid later
// reads (gZipMain / gZipPatch) when resolving asset paths.

// libzip entry points (declared in C linkage to match the shared library).
extern "C" void *zip_open(const char *path, int flags, int *errorp);
extern "C" int zip_get_num_files(void *za);
extern "C" const char *zip_get_name(void *za, int index, int flags);

// The handle slots shared with FileInterfaceAndroid. Each is the address of a
// void* cell that holds the opened archive handle.
extern void **gZipMain __attribute__((visibility("hidden")));
extern void **gZipPatch __attribute__((visibility("hidden")));

// Opens the APK archive at `path` and, on success, walks its directory so the
// libzip name table is fully populated. Returns true once the archive is open.
extern "C" int loadAPK(const char *path)
{
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

// Opens both the main APK (`apkPath`) and the patch zip (`patchPath`). Returns true
// only when both archives opened successfully.
extern "C" int loadAPKAndZip(const char *apkPath, const char *patchPath)
{
    void *apk = zip_open(apkPath, 0, 0);
    *gZipMain = apk;

    void *patch = zip_open(patchPath, 0, 0);
    *gZipPatch = patch;

    return apk != 0 && patch != 0;
}
