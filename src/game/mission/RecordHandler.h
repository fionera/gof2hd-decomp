#ifndef GOF2_RECORDHANDLER_H
#define GOF2_RECORDHANDLER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

struct Agent;
struct Mission;
struct Wanted;
struct GameRecord;

namespace AbyssEngine {
    class PaintCanvas;
    class Engine;
}

// OptionsRecord — the persistent game options/settings record managed by RecordHandler
// (see saveOptions/loadOptions). The pointer is held opaquely as g_ModMainMenu_initOptions
// in the menu code; only fields actually accessed by name are modeled here. Unmodeled bytes
// are left as raw padding so existing offsets are preserved exactly.
//
// The leading block (0x00..0x47) is a table of per-feature option/unlock flags. The
// dev-unlock helpers (startValkyrie/startSupernova) and the DLC purchase-result dispatch
// set these directly by raw offset; widths below are confirmed against the original
// (undefined1=byte, undefined2=u16, undefined4=i32). Offset 0x38 is written both as a
// byte (0x38/0x39) and (in the supernova path) as a u16 covering 0x38/0x39, so it carries
// a union word alias; every access width resolves to a named field at the same memory.
//
// Packed: several flag words sit at odd offsets (0xd, 0x1c, 0x32) that the original code
// reaches with unaligned u16/i32 stores. #pragma pack(1) keeps each named field at its
// exact raw byte offset (no inserted alignment padding) so the layout matches the binary.
#pragma pack(push, 1)
struct OptionsRecord {
    uint8_t  field_0x00[8];          // +0x00: unmodeled leading option bytes
    uint16_t flag_word_0x8;          // +0x08: u16 option-flag pair
    uint8_t  flag_0xa;               // +0x0a
    uint8_t  field_0x0b[2];          // +0x0b: padding
    union {                          // +0x0d: MenuTouchWindow writes a u16 0x101 here, but the two
        uint16_t flag_word_0xd;      //        bytes are also read independently as single bytes
        struct {                     //        (loadOptions, Globals setMus, DialogueWindow auto-advance)
            uint8_t  musicMasterVolumeFlag; // +0x0d: music master-volume/enable byte (setMus channel 1)
            uint8_t  autoAdvanceEnabled;    // +0x0e: voiceover auto-advance enabled (setMus channel 3)
        };
    };
    uint8_t  flag_0xf;               // +0x0f
    uint8_t  field_0x10[3];          // +0x10: padding
    uint8_t  flag_0x13;              // +0x13
    uint8_t  field_0x14;             // +0x14: padding
    uint8_t  flag_0x15;              // +0x15
    uint8_t  field_0x16;             // +0x16: padding
    uint8_t  flag_0x17;              // +0x17
    uint8_t  field_0x18[4];          // +0x18: padding
    uint16_t flag_word_0x1c;         // +0x1c: u16 option-flag pair
    uint8_t  flag_0x1e;              // +0x1e
    uint8_t  field_0x1f;             // +0x1f: padding
    int32_t  flag_dword_0x20;        // +0x20: i32 option-flag word
    uint8_t  flag_0x24;              // +0x24
    uint8_t  field_0x25;             // +0x25: padding
    uint8_t  flag_0x26;              // +0x26
    uint8_t  field_0x27[5];          // +0x27: padding
    uint32_t fadeValue;              // +0x2c: cross-fade value stashed before a module transition
    uint8_t  field_0x30;             // +0x30: padding
    uint8_t  flag_0x31;              // +0x31
    union {                          // +0x32: i32 word (startValkyrie/supernova) overlaps 0x34/0x35
        int32_t flag_dword_0x32;     //        bytes set independently elsewhere (DLC dispatch)
        struct {
            uint8_t field_0x32[2];   // +0x32: padding (low half of the dword, never read as bytes)
            uint8_t flag_0x34;       // +0x34
            uint8_t flag_0x35;       // +0x35
        };
    };
    uint8_t  flag_0x36;              // +0x36
    uint8_t  flag_0x37;              // +0x37
    union {                          // +0x38: byte at 0x38/0x39 (supernova path writes a u16 here)
        uint16_t flag_word_0x38;
        struct {
            uint8_t flag_0x38;       // +0x38
            uint8_t flag_0x39;       // +0x39
        };
    };
    uint8_t  field_0x3a;             // +0x3a: padding
    uint8_t  flag_0x3b;              // +0x3b
    uint8_t  field_0x3c[0xc];        // +0x3c: padding up to firstRunPreviewChecked
    uint8_t  firstRunPreviewChecked; // +0x48: set once the first-run saved-game preview probe has run
};
#pragma pack(pop)

// Builds the application's startup resource list (textures/sounds/fonts the active
// device profile needs). Called from OnCreateApplication; the body lives in its own TU.
void BuildResourceList(AbyssEngine::Engine *engine);

// Full-screen "loading / saving" splash: clears the frame, draws the window chrome
// (background, header, empty footer), the centered status caption and the spinner image,
// and finally a horizontal progress bar filled to `progress` percent. `resourceHolder`
// points at the resource id of the font/spinner to use (and is refreshed if the font for
// the current language is not loaded yet); a null holder draws an empty frame.
void loadingScreen(AbyssEngine::PaintCanvas *canvas, int progress, void *resourceHolder);

// RecordHandler — save/record serialization for the persistent game state.
// The two leading pointers track the mission/agent currently being (de)serialized so the
// writer can break self-reference cycles; the three String members hold the file-path
// prefixes for the options file and the primary/backup record directories.
class RecordHandler {
public:
    Mission *currentMission; // mission being serialized (cycle guard)
    void *currentAgent; // agent being serialized (cycle guard)
    String optionsPath; // options file path
    String recordDir; // primary save-record directory prefix
    String backupDir; // backup save-record directory prefix

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
