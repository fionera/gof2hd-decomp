#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "gof2/common.h"
// RecordHandler — top-level class (NO namespace). Save/record I/O.
// Members named from accessor bodies, ctor init and usage. The three String members
// are real String objects (auto ctor/dtor); the two leading pointers track the
// mission/agent currently being (de)serialized so the writer can break self-reference cycles.

// Game types referenced by pointer only — keep opaque.
struct Agent;
struct Mission;
struct Wanted;
struct GameRecord;

// ---- tiny offset-cast helpers -------------------------------------------------
#ifndef GOF2_BIP_HELPERS
#define GOF2_BIP_HELPERS
static inline char*           B (void* p, int off) { return (char*)p + off; }
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }
#endif

class RecordHandler {
public:
    void*  currentMission;                 // +0x0   mission being serialized (cycle guard)
    void*  currentAgent;                   // +0x4   agent being serialized (cycle guard)
    String optionsPath;                    // +0x8   options file path
    String recordDir;                      // +0x14  primary save-record directory prefix
    String backupDir;                      // +0x20  backup save-record directory prefix

    // ---- methods (converted from free functions) ----
    void addHash(int slot);
    void addHashToOptions();
    void changeSaveDirectoryToBackupDirectory();
    void convertSDVersionSaves();
    RecordHandler();
    ~RecordHandler();
    void loadOptions();
    void loadResolutionValue();
    void * readAgent(unsigned int fd);
    void * readAllPreviewRecords();
    void * readAllRecords();
    void * readMission(unsigned int fd);
    int readOptionsFileAsByteArray(signed char **out);
    int readRecordAsByteArray(signed char **out, int slot, bool fromBackup);
    void * readWanted(unsigned int fd);
    void * recordStoreRead(int slot);
    void recordStoreRead_body(void *rec, unsigned int fd);
    void recordStoreWrite_body(unsigned int fd);
    void readRecordTail();
    void csd_tail(int one, int count, void *backupRecords);
    void * recordStoreReadPreview(int slot);
    void recordStoreWrite(int slot);
    int recordStoreWritePreview(void *rec, int slot);
    int recordStoreWritePreview_int(int slot);
    void saveOptions();
    void writeAgent(void *agentPtr, unsigned int fd);
    void writeByteArrayAsOptionsFile(signed char *buf, int n);
    int writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup);
    void writeMission(void *m, unsigned int fd);
    void writeWanted(void *w, unsigned int fd);
};
#endif
