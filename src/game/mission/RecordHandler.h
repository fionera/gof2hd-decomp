#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "GameRecord.h"
#include "Mission.h"
#include "engine/render/Engine.h"
#include "game/world/Wanted.h"

#include "game/mission/OptionsRecord.h"

namespace AbyssEngine {
    class PaintCanvas;
}

class Agent;
class GameRecord;
class Mission;
class Wanted;

namespace AbyssEngine {
    class Engine;
}

#pragma pack(push, 1)
#pragma pack(pop)

void BuildResourceList(AbyssEngine::Engine *engine);

void loadingScreen(AbyssEngine::PaintCanvas *canvas, int progress, void *resourceHolder); // lint: void_ptr (exported free function; mangled signature is fixed)

// lint: void_ptr (exported free function; mangled signature is fixed)

class RecordHandler {
public:
    Mission *currentMission;
    Agent *currentAgent;
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

    void *readAgent(unsigned int fd); // lint: void_ptr (method signature; return type fixed)

    void *readAllPreviewRecords(); // lint: void_ptr (method signature; return type fixed)

    void *readAllRecords(); // lint: void_ptr (method signature; return type fixed)

    void *readMission(unsigned int fd); // lint: void_ptr (method signature; return type fixed)

    int readOptionsFileAsByteArray(signed char **out);

    GameRecord *readRecord(int slot);

    int readRecordAsByteArray(signed char **out, int slot, bool fromBackup);

    void recoverSDVersionSaves();

    void *readWanted(unsigned int fd); // lint: void_ptr (method signature; return type fixed)

    GameRecord *recordStoreRead(int slot);

    GameRecord *recordStoreReadPreview(int slot);

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
