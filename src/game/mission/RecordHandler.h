#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "common.h"   // provides AbyssEngine::String (+ the `using String`) and Array<T>

// Game types referenced by pointer only.
struct Agent;
struct Mission;

// RecordHandler — save/record serialization for the persistent game state.
// The two leading pointers track the mission/agent currently being (de)serialized so the
// writer can break self-reference cycles; the three String members hold the file-path
// prefixes for the options file and the primary/backup record directories.
class RecordHandler {
public:
    Mission *currentMission;   // mission being serialized (cycle guard)
    void    *currentAgent;     // agent being serialized (cycle guard)
    String   optionsPath;      // options file path
    String   recordDir;        // primary save-record directory prefix
    String   backupDir;        // backup save-record directory prefix

    RecordHandler();

    void   addHash(int slot);
    void   addHashToOptions();
    void   changeSaveDirectoryToBackupDirectory();
    void   convertSDVersionSaves();
    void   loadOptions();
    void   loadResolutionValue();
    void  *readAgent(unsigned int fd);
    void  *readAllPreviewRecords();
    void  *readAllRecords();
    void  *readMission(unsigned int fd);
    int    readOptionsFileAsByteArray(signed char **out);
    int    readRecordAsByteArray(signed char **out, int slot, bool fromBackup);
    void  *readWanted(unsigned int fd);
    void  *recordStoreRead(int slot);
    void   recordStoreRead_body(void *rec, unsigned int fd);
    void   recordStoreWrite_body(unsigned int fd);
    void  *recordStoreReadPreview(int slot);
    void   recordStoreWrite(int slot);
    int    recordStoreWritePreview(void *rec, int slot);
    int    recordStoreWritePreview_int(int slot);
    void   saveOptions();
    void   writeAgent(void *agentPtr, unsigned int fd);
    void   writeByteArrayAsOptionsFile(signed char *buf, int n);
    int    writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup);
    void   writeMission(void *m, unsigned int fd);
    void   writeWanted(void *w, unsigned int fd);
};
#endif
