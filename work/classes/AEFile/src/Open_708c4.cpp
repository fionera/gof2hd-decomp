#include "class.h"

extern FileInterface *g_AEFile_fileInterface;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern char g_AELowLevelNativeFile_vtable[];

typedef void (*StringDestructor)(String *);
extern StringDestructor g_AEString_destructor;

uint32_t AEFile::Open(String &path, FileOpenType openType, uint32_t *handle)
{
    FileInterface *fileInterface = g_AEFile_fileInterface;
    uint32_t result;

    if (fileInterface == 0) {
        result = 0;
    } else {
        uint16_t *text = path;
        AELowLevelFile *file = 0;
        void *nativeHandle = 0;

        if (openType == OPEN_WRITE) {
            String localPath(text, false);
            nativeHandle = fileInterface->vtable->OpenWrite(fileInterface, localPath, path.size, 0);
        } else if (openType == OPEN_APPEND) {
            String localPath(text, false);
            nativeHandle = fileInterface->vtable->OpenAppend(fileInterface, localPath, path.size, 0, 0);
        } else if (openType == OPEN_READ) {
            String localPath(text, false);
            nativeHandle = fileInterface->vtable->OpenRead(fileInterface, localPath, path.size, 0, 0, 0, 0);
            if (nativeHandle == 0) {
                if (*path.operator[](0) != '/') {
                    String prefix("/", false);
                    String oldPath(path, false);
                    String newPath = prefix + oldPath;
                    path = newPath;
                    StringDestructor destruct = g_AEString_destructor;
                    destruct(&newPath);
                    destruct(&oldPath);
                    destruct(&prefix);
                }
                file = findPakFile(path);
                if (file == 0) {
                    result = 0;
                    goto done;
                }
            }
        }

        if (file == 0) {
            if (nativeHandle == 0) {
                result = 0;
                goto done;
            }
            AELowLevelNativeFile *nativeFile = new AELowLevelNativeFile;
            nativeFile->vtable = reinterpret_cast<AELowLevelFileVTable *>(g_AELowLevelNativeFile_vtable + 8);
            nativeFile->handle = nativeHandle;
            file = nativeFile;
        }

        Array<AELowLevelFile *> *files = g_AEFile_openFiles;
        uint32_t count = files->size;
        if (handle == 0) {
            if (count == 0) {
                ArrayAdd(file, *files);
            } else {
                AELowLevelFile **slot = files->data;
                if (*slot != 0) {
                    (*slot)->vtable->Close(*slot);
                    slot = files->data;
                }
                *slot = 0;
                files->data[0] = file;
            }
        } else {
            for (uint32_t i = 1; i < count; i++) {
                if (files->data[i] == 0) {
                    files->data[i] = file;
                    *handle = i;
                    result = 1;
                    goto done;
                }
            }
            if (count == 0) {
                ArrayAdd(static_cast<AELowLevelFile *>(0), *files);
                files = g_AEFile_openFiles;
            }
            ArrayAdd(file, *files);
            *handle = g_AEFile_openFiles->size - 1;
        }
        result = 1;
    }

done:
    return result;
}
