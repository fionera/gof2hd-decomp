#ifndef GOF2_AEPAKFILE_H
#define GOF2_AEPAKFILE_H
#include "AELowLevelFile.h"
#include "FileInterface.h"

class FileInterface;

class AEPakFile : public AELowLevelFile {
public:
    FileInterface *fileInterface;
    uint32_t sizeLimit;
    uint32_t baseOffset;
    uint32_t position;

    AEPakFile(FileInterface *fileInterface, int sizeLimit, int baseOffset);

    ~AEPakFile() override;

    uint32_t Write(uint32_t bytes, void *buffer) override; // lint: void_ptr virtual buffer param; Pv baked into mangled name

    // lint: void_ptr virtual buffer param; Pv baked into mangled name

    uint32_t Read(uint32_t bytes, void *buffer) override; // lint: void_ptr virtual buffer param; Pv baked into mangled name

    // lint: void_ptr virtual buffer param; Pv baked into mangled name

    uint32_t Skip(uint32_t bytes) override;

    uint32_t Release() override;

    uint32_t GetFileSize() override;
};
#endif
