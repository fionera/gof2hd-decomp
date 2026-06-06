#include "class.h"

extern FileInterface *g_fileInterface;

inline void *operator new(uint32_t, void *ptr) noexcept
{
    return ptr;
}

void AEFile::collectPakFiles(const String &path)
{
    struct StringSlot {
        uint32_t words[3];
    };
    struct Frame {
        void *savedPathChars;
        StringSlot suffix;
        StringSlot entryCopy;
        StringSlot entry;
        void *stackGuard;
    };

    FileInterface **fileInterfacePtr = &g_fileInterface;
    void **stackGuardPtr = &__stack_chk_guard;
    Frame frame;
    frame.stackGuard = *stackGuardPtr;

    if (*fileInterfacePtr != 0) {
        if ((*fileInterfacePtr)->enabled != 0) {
            char *pathChars = path.GetAEChar();
            if ((*fileInterfacePtr)->vtable->OpenDirectory(*fileInterfacePtr, pathChars, 0) != 0) {
                frame.savedPathChars = pathChars;
                String *entry = reinterpret_cast<String *>(&frame.entry);
                String *entryCopy = reinterpret_cast<String *>(&frame.entryCopy);
                String *suffix = reinterpret_cast<String *>(&frame.suffix);
                const char *pakSuffix = ".pak";
                new (entry) String();
                goto readDirectory;

            directoryEntry:
                new (entryCopy) String(*entry, false);
                new (suffix) String(pakSuffix, false);
                {
                    int32_t index = entry->IndexOf(*suffix);
                    suffix->~String();
                    if (index != -1) {
                        collectFilesInPakFiles(*entryCopy);
                    }
                }
                entryCopy->~String();

            readDirectory:
                if ((*fileInterfacePtr)->vtable->ReadDirectory(*fileInterfacePtr, *entry) != 0) {
                    goto directoryEntry;
                }
                entry->~String();
                pathChars = static_cast<char *>(frame.savedPathChars);
            }
            operator delete(pathChars);
            sortPakFileEntryList();
        }
    }

    if (*stackGuardPtr != frame.stackGuard) {
        __stack_chk_fail();
    }
}
