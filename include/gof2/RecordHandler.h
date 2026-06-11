#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// RecordHandler — top-level class (NO namespace). Byte-exact decomp scaffold.
// Save/record I/O. Fields accessed by byte offset from each work-item's target disasm.


struct RecordHandler;     // opaque; only ever a RecordHandler* + offset-cast.

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
    void* currentAgent;                    // +0x4

    // ---- methods (converted from free functions) ----
    void addHash(int slot);
    void addHashToOptions();
    void changeSaveDirectoryToBackupDirectory();
    void convertSDVersionSaves();
    RecordHandler * ctor();
    RecordHandler * dtor();
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
