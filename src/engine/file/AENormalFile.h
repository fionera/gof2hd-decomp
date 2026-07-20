#ifndef GOF2_AENORMALFILE_H
#define GOF2_AENORMALFILE_H
#include "AELowLevelFile.h"
#include "FileInterface.h"

class FileInterface;

class AENormalFile : public AELowLevelFile {
public:
    FileInterface *file;

    AENormalFile(FileInterface *file);

    ~AENormalFile() override;

    uint32_t Write(uint32_t bytes, void *buffer) override; // lint: void_ptr virtual buffer param; Pv baked into mangled name

    // lint: void_ptr virtual buffer param; Pv baked into mangled name

    uint32_t Read(uint32_t bytes, void *buffer) override; // lint: void_ptr virtual buffer param; Pv baked into mangled name

    // lint: void_ptr virtual buffer param; Pv baked into mangled name

    uint32_t Skip(uint32_t bytes) override;

    uint32_t Release() override;

    uint32_t GetFileSize() override;
};
#endif
