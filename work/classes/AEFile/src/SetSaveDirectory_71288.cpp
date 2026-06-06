#include "class.h"

extern FileInterface *gAEFileInterface;

inline void *operator new(uint32_t, void *ptr) noexcept {
    return ptr;
}

void AEFile::SetSaveDirectory(String path) {
    struct StackFrame {
        unsigned char savePathStorage[sizeof(String)];
        void * volatile cookie;
    } frame;
    frame.cookie = __stack_chk_guard;

    FileInterface *fileInterface = gAEFileInterface;
    if (fileInterface != 0) {
        String *savePath = reinterpret_cast<String *>(frame.savePathStorage);
        new (savePath) String(path, false);
        void (*setSaveDirectory)(FileInterface *, const String &) =
            fileInterface->vtable->SetSaveDirectory;
        setSaveDirectory(fileInterface, *savePath);
        savePath->~String();
    }

    if (frame.cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
    __builtin_unreachable();
}
