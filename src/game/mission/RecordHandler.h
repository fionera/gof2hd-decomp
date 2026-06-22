#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "fieldaccess.h"
#include "GameRecord.h"
#include "Mission.h"
#include "engine/render/Engine.h"
#include "game/world/Wanted.h"

#pragma pack(push, 1)
struct OptionsRecord {
    uint8_t field_0x00[8];
    uint16_t flag_word_0x8;
    uint8_t flag_0xa;
    uint8_t field_0x0b[2];

    union {
        uint16_t flag_word_0xd;

        struct {
            uint8_t musicMasterVolumeFlag;
            uint8_t autoAdvanceEnabled;
        };
    };

    uint8_t flag_0xf;
    uint8_t field_0x10[3];
    uint8_t flag_0x13;
    uint8_t field_0x14;
    uint8_t flag_0x15;
    uint8_t field_0x16;
    uint8_t flag_0x17;
    uint8_t field_0x18[4];
    uint16_t flag_word_0x1c;
    uint8_t flag_0x1e;
    uint8_t field_0x1f;
    int32_t flag_dword_0x20;
    uint8_t flag_0x24;
    uint8_t field_0x25;
    uint8_t flag_0x26;
    uint8_t field_0x27[5];
    uint32_t fadeValue;
    uint8_t field_0x30;
    uint8_t flag_0x31;

    union {
        int32_t flag_dword_0x32;

        struct {
            uint8_t field_0x32[2];
            uint8_t flag_0x34;
            uint8_t flag_0x35;
        };
    };

    uint8_t flag_0x36;
    uint8_t flag_0x37;

    union {
        uint16_t flag_word_0x38;

        struct {
            uint8_t flag_0x38;
            uint8_t flag_0x39;
        };
    };

    uint8_t field_0x3a;
    uint8_t flag_0x3b;
    uint8_t field_0x3c[0xc];
    uint8_t firstRunPreviewChecked;
};
#pragma pack(pop)

void BuildResourceList(AbyssEngine::Engine * engine);

void loadingScreen(AbyssEngine::PaintCanvas *canvas, int progress, void *resourceHolder);

class RecordHandler {
public:
    Mission *currentMission;
    void *currentAgent;
    String optionsPath;
    String recordDir;
    String backupDir;

    RecordHandler();

    ~RecordHandler();

    void addHash(int slot);

    void addHashToOptions();

    void changeSaveDirectoryToBackupDirectory();

    bool checkHash(unsigned int fd);

    void convertSDVersionSaves();

    bool notEnoughMemory();

    void loadOptions();

    void loadResolutionValue(float resolution);

    void *readAgent(unsigned int fd);

    void *readAllPreviewRecords();

    void *readAllRecords();

    void *readMission(unsigned int fd);

    int readOptionsFileAsByteArray(signed char **out);

    void *readRecord(int slot);

    int readRecordAsByteArray(signed char **out, int slot, bool fromBackup);

    void recoverSDVersionSaves();

    void *readWanted(unsigned int fd);

    void *recordStoreRead(int slot);

    void *recordStoreReadPreview(int slot);

    void recordStoreWrite(int slot);

    int recordStoreWritePreview(GameRecord *rec, int slot);

    void recordStoreWritePreview(int slot);

    void saveOptions();

    void writeAgent(Agent *agentPtr, unsigned int fd);

    void writeByteArrayAsOptionsFile(signed char *buf, int n);

    int writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup);

    void writeMission(Mission *m, unsigned int fd);

    void writeWanted(Wanted *w, unsigned int fd);
};
#endif
