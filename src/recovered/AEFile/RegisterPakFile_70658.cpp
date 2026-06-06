#include "class.h"

void AEFile::RegisterPakFile(String &path)
{
    collectFilesInPakFiles(path);
    return sortPakFileEntryList();
}
