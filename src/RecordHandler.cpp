#include "gof2/RecordHandler.h"
#include "gof2/Achievements.h"
#include "gof2/BluePrint.h"
#include "gof2/GameText.h"
#include "gof2/Item.h"
#include "gof2/Standing.h"
#include "gof2/Ship.h"
#include "gof2/Galaxy.h"
#include "gof2/AEFile.h"
#include "gof2/FModSound.h"
#include "gof2/Status.h"
// Station.h is the canonical home of `struct RetStr`; pull it in first so the
// shared definition wins. SolarSystem.h / Wanted.h / Agent.h each redefine an
// identical `struct RetStr` unguarded, which would be a redefinition error here.
// Rename their local copies during inclusion so only Station.h's definition is
// used (the value is layout-identical and always discarded at the call sites).
#include "gof2/Station.h"
#include "gof2/Mission.h"
#define RetStr RetStr
#include "gof2/SolarSystem.h"
#undef RetStr
#define RetStr RetStr
#include "gof2/Wanted.h"
#undef RetStr
#define RetStr RetStr
#include "gof2/Agent.h"
#undef RetStr
#include "gof2/String.h"


extern "C" void *RH_op_new(unsigned int sz);
extern "C" void Array_GR_ctor(void *a);
extern "C" void ArraySetLength_GR(unsigned int n, void *a);
extern "C" void Array_SC_ctor(void *a);
extern "C" void ArraySetLength_SC(unsigned int n, void *a);
extern "C" signed char *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void ArrayReleaseArrays_SC(void *a);
extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" void AEString_dtor(void *s);
extern "C" void RH_op_delete(void *p);
extern "C" void SHA256_Init(void *c);
extern "C" void SHA256_Update(void *c, const void *data, int n);
extern "C" void SHA256_Final(unsigned char *md, void *c);
extern "C" void *RH_memcpy(void *dst, const void *src, unsigned int n);
extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void *GR_op_new(unsigned int sz);
extern "C" void GameRecord_ctor(void *gr);
extern "C" void AEFile_Read_i64(void *dst, unsigned int fd);
extern "C" void AEFile_Read_i32(void *dst, unsigned int fd);
extern "C" void AEFile_Read_bool(void *dst, unsigned int fd, int b);
extern "C" void AEFile_Read_f32(void *dst, unsigned int fd);
extern "C" void String_default_ctor(void *s);
extern "C" void String_cstr_ctor(void *s, const char *cs, bool b);
extern "C" void *String_copy_ctor(void *dst, void *src, bool b);
extern "C" void AEFile_Write_i64(long long v, unsigned int fd);
extern "C" void AEFile_Write_i32(int v, unsigned int fd);
extern "C" void AEFile_Write_str(void *s, unsigned int fd, int b);
extern "C" void AEFile_Write_f32(int v, unsigned int fd);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int copy);
extern "C" void AEFile_ReadByte(void *out, unsigned int fd);
extern "C" void AEFile_ReadFloat(void *out, unsigned int fd);
extern "C" void AEFile_ReadShort(void *out, unsigned int fd);
int RecordHandler_checkHash(unsigned int fd);
void Globals_loadFont(int kind);
extern "C" void Mission_ctorEmpty(void *self, int type, int reward, int targetStation);
extern "C" void AEFile_WriteByte(int v, unsigned int fd);
extern "C" void AEFile_WriteFloat(int v, unsigned int fd);
extern "C" void AEFile_WriteShort(int v, unsigned int fd);
int GameText_getLanguage();
extern "C" void ArrayStr_ctor(void *a);
extern "C" void ArraySetLength_Str(unsigned n, void *a);
extern "C" void AEFile_WriteLong(long long v, unsigned int fd);
extern "C" void AEFile_ReadLong(void *out, unsigned int fd);
extern "C" void Array_bool_ctor(void *a);
extern "C" void ArraySetLength_bool(unsigned n, void *a);
extern "C" void Array_UC_ctor(void *a);
extern "C" void ArrayAdd_UC(unsigned char *src, unsigned int n, void *a);

// ---- readRecord_cd25c.cpp ----
// Tail-called veneer at 0x1ac058 (recordStoreRead dispatch).

// RecordHandler::readRecord(int)
void RecordHandler_readRecord(int param)
{
    return ((RecordHandler *)(param))->readRecordTail();
}

// readRecord()'s tail veneer: a thin dispatch trampoline into the record-store reader.
// In the binary it is a bare `b.w` veneer that forwards its receiver register unchanged;
// there is no per-instance work to perform here beyond completing that branch.
void RecordHandler::readRecordTail()
{
}

// changeSaveDirectoryToBackupDirectory() cleanup tail: the two byte-array Arrays and the
// primary size buffer have already been released by the caller; free the backup size
// buffer here. It rides in the receiver register, so `this` is that buffer. The remaining
// args are the caller's residual registers.
void RecordHandler::csd_tail(int one, int count, void *backupRecords)
{
    (void)one;
    (void)count;
    (void)backupRecords;
    RH_op_delete_arr(this);
}

// ---- readAllRecords_cbcdc.cpp ----
// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;


// RecordHandler::readAllRecords() -> Array<GameRecord*>*
void * RecordHandler::readAllRecords() {
    RecordHandler *self = this;
    void *arr = RH_op_new(0xc);
    Array_GR_ctor(arr);
    int *cnt = g_RH_recordCount;
    ArraySetLength_GR(*cnt, arr);
    for (int i = 0; i < *cnt; i++) {
        void *r = ((RecordHandler *)(self))->recordStoreRead(i);
        ((void **)I(arr, 4))[i] = r;
    }
    return arr;
}

// ---- readAllPreviewRecords_cbb48.cpp ----
// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;


// RecordHandler::readAllPreviewRecords() -> Array<GameRecord*>*
void * RecordHandler::readAllPreviewRecords() {
    RecordHandler *self = this;
    void *arr = RH_op_new(0xc);
    Array_GR_ctor(arr);
    int *cnt = g_RH_recordCount;
    ArraySetLength_GR(*cnt, arr);
    for (int i = 0; i < *cnt; i++) {
        void *r = ((RecordHandler *)(self))->recordStoreReadPreview(i);
        ((void **)I(arr, 4))[i] = r;
    }
    return arr;
}

// ---- notEnoughMemory_cbb30.cpp ----
// RecordHandler::notEnoughMemory() -> bool: free space < 900.
uint8_t RecordHandler_notEnoughMemory()
{
    return AEFile::GetDeviceFreeSpace() < 900;
}

// ---- convertSDVersionSaves_cffa8.cpp ----
__attribute__((visibility("hidden"))) extern int *g_CSV_guard;   // DAT_000e01b8 -> guard holder
__attribute__((visibility("hidden"))) extern int *g_CSV_count;   // DAT_000e01bc -> P, *P = record count

// RecordHandler::convertSDVersionSaves()
void RecordHandler::convertSDVersionSaves() {
    RecordHandler *self = this;
    int *guardP = g_CSV_guard;
    volatile int saved = *guardP;

    void *a0 = RH_op_new(0xc);
    Array_SC_ctor(a0);
    int *cnt = g_CSV_count;
    ArraySetLength_SC(*cnt, a0);
    void *a1 = RH_op_new(0xc);
    Array_SC_ctor(a1);
    ArraySetLength_SC(*cnt, a1);

    unsigned n = *cnt;
    unsigned long long bytes = (unsigned long long)n * 4;
    unsigned int sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned int)bytes;
    signed char *sizes0 = RH_op_new_arr(sz);
    signed char *sizes1 = RH_op_new_arr(sz);

    for (int i = 0; i < (int)n; i++) {
        ((int *)sizes0)[i] = ((RecordHandler *)(self))->readRecordAsByteArray((signed char **)(I(a0, 4) + i * 4), i, false);
        ((int *)sizes1)[i] = ((RecordHandler *)(self))->readRecordAsByteArray((signed char **)(I(a1, 4) + i * 4), i, true);

        char num[12], path[12];
        AEString_int_ctor(num, i);
        AEString_concat(path, (char *)self + 0x14);
        AEFile::FileDelete(*(String *)(path));

        AEString_int_ctor(num, i);
        AEString_concat(path, (char *)self + 0x20);
        AEFile::FileDelete(*(String *)(path));
        n = *cnt;
    }

    int i = 0;
    while (true) {
        int last = (int)n - 1;
        int j = i;
        signed char *rec = 0;
        bool found = false;
        while (j < last) {
            rec = *(signed char **)(I(a0, 4) + j * 4);
            if (rec != 0) { found = true; break; }
            j++;
        }
        if (!found) {
            // process the final slot (index last) then finish
            signed char *lastRec = *(signed char **)(I(a0, 4) + last * 4);
            if (lastRec != 0) {
                ((RecordHandler *)(self))->writeByteArrayAsRecord(lastRec, ((int *)sizes0)[last], 0, false);
                ((RecordHandler *)(self))->addHash(0);
                int idx = (int)(*cnt) * 4 - 4;
                ((RecordHandler *)(self))->writeByteArrayAsRecord(*(signed char **)(I(a1, 4) + idx), *(int *)(sizes1 + idx), 0, true);
            }
            break;
        }
        int next = j + 1;
        ((RecordHandler *)(self))->writeByteArrayAsRecord(rec, ((int *)sizes0)[j], next, false);
        ((RecordHandler *)(self))->addHash(next);
        ((RecordHandler *)(self))->writeByteArrayAsRecord(*(signed char **)(I(a1, 4) + j * 4), ((int *)sizes1)[j], next, true);
        i = next;
        n = *cnt;
    }

    ArrayReleaseArrays_SC(a0);
    ArrayReleaseArrays_SC(a1);
    RH_op_delete_arr(sizes0);
    RH_op_delete_arr(sizes1);

    return;
}

// ---- addHash_cdea0.cpp ----
__attribute__((visibility("hidden"))) extern int *g_AH_guard;                  // DAT_000ddfa0 -> guard holder
__attribute__((visibility("hidden"))) extern const unsigned char RH_ah_salt[]; // DAT_000ddfa8 (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ah_key;        // DAT_000ddfac (16-byte key)

// RecordHandler::addHash(int slot)
void RecordHandler::addHash(int slot) {
    RecordHandler *self = this;
    int *guardP = g_AH_guard;
    volatile int saved = *guardP;

    signed char *data = 0;
    int len = ((RecordHandler *)(self))->readRecordAsByteArray(&data, slot, false);
    if (-1 < len) {
        unsigned char *md = (unsigned char *)RH_op_new_arr(0x20);
        void *c = RH_op_new(0x70);
        SHA256_Init(c);
        void (*update)(void *, const void *, int) = SHA256_Update;
        update(c, data, len);
        update(c, RH_ah_salt, 0x19);
        update(c, *RH_ah_key, 0x10);
        SHA256_Final(md, c);

        signed char *out = (signed char *)RH_op_new_arr(len + 0x20);
        RH_memcpy(out, data, len);
        unsigned long long *dst = (unsigned long long *)(out + len);
        unsigned long long *src = (unsigned long long *)md;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        ((RecordHandler *)(self))->writeByteArrayAsRecord(out, len + 0x20, slot, false);

        void (*del)(void *) = (void (*)(void *))RH_op_delete_arr;
        del(data);
        data = 0;
        del(out);
        del(md);
        RH_op_delete(c);
    }

    return;
}

// ---- readRecordAsByteArray_cdbfc.cpp ----
// RecordHandler::readRecordAsByteArray(signed char**, int, bool)
int RecordHandler::readRecordAsByteArray(signed char **out, int slot, bool fromBackup) {
    RecordHandler *self = this;
    char num[12];
    char path[12];
    unsigned int fd;
    int sz;

    String_int_ctor(num, slot);
    char *dir = (char *)self + (fromBackup ? 0x20 : 0x14);
    String_concat(path, dir, num);
    ((String *)(num))->dtor();

    if (AEFile::FileExist(*(String *)(path)) != 0) {
        AEFile::OpenRead(*(String *)path, &fd);
        sz = AEFile::GetFileSize(fd);
        signed char *b = RH_op_new_arr(sz | (sz >> 31));
        *out = b;
        AEFile::Read(sz, b, fd);
        AEFile::Close(fd);
    } else {
        sz = -1;
    }
    ((String *)(path))->dtor();
    return sz;
}

// ---- readWanted_ce6c8.cpp ----

__attribute__((visibility("hidden"))) extern int *g_RW_guard;   // DAT_000de8a4 -> guard holder

// RecordHandler::readWanted(unsigned int fd) -> Wanted* (in r0 on return).
void * RecordHandler::readWanted(unsigned int fd) {
    RecordHandler *self = this;
    (void)self;
    int *guardP = g_RW_guard;
    volatile int saved = *guardP;

    bool active = false;
    bool terminated = false;
    int currentLocation = -1;
    int travelsTo = -1;
    int lastSeen = -1;
    AEFile_ReadBool(&active, fd);
    AEFile_ReadBool(&terminated, fd);
    AEFile_ReadInt(&currentLocation, fd);
    AEFile_ReadInt(&travelsTo, fd);
    AEFile_ReadInt(&lastSeen, fd);

    char name[12];
    AEString_default_ctor(name);

    int idx = 0, board = 0, race = 0;
    bool male = true;
    int ship = 0, weapon = 0, hp = 0, loot = 0, lootAmt = 0;
    int reward = 0, reqBounties = 0, reqMission = 0, numWingmen = 0;

    AEFile_ReadString(name, fd, 1);
    AEFile_ReadInt(&idx, fd);
    AEFile_ReadInt(&board, fd);
    AEFile_ReadInt(&race, fd);
    AEFile_ReadBool(&male, fd);
    AEFile_ReadInt(&ship, fd);
    AEFile_ReadInt(&weapon, fd);
    AEFile_ReadInt(&hp, fd);
    AEFile_ReadInt(&loot, fd);
    AEFile_ReadInt(&lootAmt, fd);
    AEFile_ReadInt(&reward, fd);
    AEFile_ReadInt(&reqBounties, fd);
    AEFile_ReadInt(&reqMission, fd);
    AEFile_ReadInt(&numWingmen, fd);

    void *w = RH_op_new(0x54);
    String12 nameCopy;
    AEString_copy_ctor(&nameCopy, name, 0);
    ((Wanted *)(w))->ctor(idx, nameCopy, board, race, male, ship, weapon, hp, loot, lootAmt, reward, reqBounties, reqMission, numWingmen);

    int *parts = (int *)RH_op_new_arr(0x14);
    int *p = parts;
    for (unsigned i = 0; i < 5; i++) {
        AEFile_ReadInt(p, fd);
        p++;
    }
    ((Wanted *)(w))->setImageParts(parts);
    ((Wanted *)(w))->setActive(active);
    ((Wanted *)(w))->setTerminated(terminated);
    ((Wanted *)(w))->setCurrentLocation(currentLocation);
    ((Wanted *)(w))->setTravelsTo(travelsTo);
    ((Wanted *)(w))->setLastSeen(lastSeen);

    return w;
}

// ---- changeSaveDirectoryToBackupDirectory_d0258.cpp ----
// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_csd_count;
// Flag holder: g -> P, *P -> the flag object (byte set to 1).
__attribute__((visibility("hidden"))) extern char **g_RH_csd_flag;

// Tail veneer at 0x1ab108: (ptr, 1, count, arr2).

// RecordHandler::changeSaveDirectoryToBackupDirectory()
void RecordHandler::changeSaveDirectoryToBackupDirectory() {
    RecordHandler *self = this;
    void *a0 = RH_op_new(0xc);
    Array_SC_ctor(a0);
    int *cnt = g_RH_csd_count;
    ArraySetLength_SC(*cnt, a0);
    void *a1 = RH_op_new(0xc);
    Array_SC_ctor(a1);
    ArraySetLength_SC(*cnt, a1);

    unsigned int n = *cnt;
    unsigned long long bytes = (unsigned long long)n * 4;
    unsigned int sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned int)bytes;
    signed char *sizes0 = RH_op_new_arr(sz);
    signed char *sizes1 = RH_op_new_arr(sz);

    for (int i = 0; i < (int)n; i++) {
        ((int *)sizes0)[i] = ((RecordHandler *)(self))->readRecordAsByteArray((signed char **)(I(a0, 4) + i * 4), i, false);
        ((int *)sizes1)[i] = ((RecordHandler *)(self))->readRecordAsByteArray((signed char **)(I(a1, 4) + i * 4), i, true);
        n = *cnt;
    }

    **g_RH_csd_flag = 1;
    int i = 0;
    while (i < (int)n) {
        signed char *rec = *(signed char **)(I(a0, 4) + i * 4);
        if (rec == 0) {
            i++;
        } else {
            ((RecordHandler *)(self))->writeByteArrayAsRecord(rec, ((int *)sizes0)[i], i, false);
            int next = i + 1;
            ((RecordHandler *)(self))->addHash(next);
            ((RecordHandler *)(self))->writeByteArrayAsRecord(*(signed char **)(I(a1, 4) + i * 4), ((int *)sizes1)[i], i, true);
            n = *cnt;
            i = next;
        }
    }

    ArrayReleaseArrays_SC(a0);
    ArrayReleaseArrays_SC(a1);
    RH_op_delete_arr(sizes0);
    ((RecordHandler *)(sizes1))->csd_tail(1, n, a1);
}

// ---- _RecordHandler_cbb08.cpp ----
// String::~String function pointer (single pc-rel deref to the fn-ptr global).
__attribute__((visibility("hidden"))) extern void (*g_RH_stringDtor)(void *s);

// RecordHandler::~RecordHandler() — destroys the three String members at +0x20,
// +0x14, +0x8 (high to low) and returns `this`.
RecordHandler * RecordHandler::dtor() {
    RecordHandler *self = this;
    void (*dtor)(void *) = g_RH_stringDtor;
    dtor((char *)self + 0x20);
    dtor((char *)self + 0x14);
    dtor((char *)self + 0x8);
    return self;
}

// ---- recordStoreReadPreview_cbbf0.cpp ----
// RecordHandler::recordStoreReadPreview(int)
void * RecordHandler::recordStoreReadPreview(int slot) {
    RecordHandler *self = this;
    char path[12];
    char num[12];

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    ((String *)(num))->dtor();

    unsigned int &fd = *(unsigned int *)num;  // reuse the now-dead String temp slot
    void *gr = 0;
    if (AEFile::FileExist(*(String *)(path)) != 0) {
        AEFile::OpenRead(*(String *)path, &fd);
        gr = GR_op_new(0x1c8);
        GameRecord_ctor(gr);
        AEFile_Read_i64((char *)gr + 0x10, fd);
        AEFile_Read_i32((char *)gr + 0x8, fd);
        AEFile_Read_bool((char *)gr + 0x194, fd, true);
        AEFile_Read_bool((char *)gr + 0x188, fd, true);
        AEFile_Read_i32((char *)gr + 0x40, fd);
        AEFile_Read_i32((char *)gr + 0x20, fd);
        AEFile_Read_f32((char *)gr + 0x11c, fd);
        AEFile_Read_i32((char *)gr + 0x1a0, fd);
        AEFile::Close(fd);
    }
    ((String *)(path))->dtor();
    return gr;
}

// ---- RecordHandler_cba2c.cpp ----
// String literal pointers (each via a hidden pc-rel reloc).
__attribute__((visibility("hidden"))) extern const char RH_lit0[];
__attribute__((visibility("hidden"))) extern const char RH_lit1[];
__attribute__((visibility("hidden"))) extern const char RH_lit2[];


// RecordHandler::RecordHandler() — default-constructs the three String members at
// +0x8, +0x14, +0x20, then assigns each from a literal, and returns `this`.
RecordHandler * RecordHandler::ctor() {
    RecordHandler *self = this;
    char tmp[12];
    char *m0 = (char *)self + 0x8;
    char *m1 = (char *)self + 0x14;
    char *m2 = (char *)self + 0x20;

    String_default_ctor(m0);
    String_default_ctor(m1);
    String_default_ctor(m2);

    String_cstr_ctor(tmp, RH_lit0, false);
    ((String *)(m0))->assign((String *)tmp);
    ((String *)(tmp))->dtor();

    String_cstr_ctor(tmp, RH_lit1, false);
    ((String *)(m1))->assign((String *)tmp);
    ((String *)(tmp))->dtor();

    String_cstr_ctor(tmp, RH_lit2, false);
    ((String *)(m2))->assign((String *)tmp);
    ((String *)(tmp))->dtor();

    return self;
}

// ---- writeByteArrayAsOptionsFile_cddd8.cpp ----
// RecordHandler::writeByteArrayAsOptionsFile(signed char*, int)
void RecordHandler::writeByteArrayAsOptionsFile(signed char *buf, int n) {
    RecordHandler *self = this;
    char tmp[12];
    unsigned int fd;

    if (AEFile::FileExist(*(String *)String_copy_ctor(tmp, (char *)self + 0x8, false)) != 0)
        AEFile::FileDelete(*(String *)(tmp));
    AEFile::OpenWrite(*(String *)tmp, &fd);
    ((AEFile *)(n))->Write(buf, fd);
    AEFile::Close(fd);
    ((String *)(tmp))->dtor();
}

// ---- recordStoreWritePreview_cfd78.cpp ----
struct Status;
struct Station;
struct SolarSystem;
struct Ship;

// Status singleton holder-of-holder.
__attribute__((visibility("hidden"))) extern Status **g_RH_wp_status;
// Global for the float written near the end (single pc-rel -> P; *P -> obj; obj->_2c).
__attribute__((visibility("hidden"))) extern int *g_RH_wp_float;



// RecordHandler::recordStoreWritePreview(int)
int RecordHandler::recordStoreWritePreview_int(int slot) {
    RecordHandler *self = this;
    char path[12];
    char num[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    ((String *)(num))->dtor();

    if (AEFile::FileExist(*(String *)(path)) != 0)
        AEFile::FileDelete(*(String *)(path));
    AEFile::OpenWrite(*(String *)path, &fd);

    Status **sh = g_RH_wp_status;
    AEFile_Write_i64(((Status *)(*sh))->getPlayingTime(), fd);
    AEFile_Write_i32(((Status *)(*sh))->getCredits(), fd);

    ((Station *)(num))->getName();
    AEFile_Write_str(num, fd, true);
    ((String *)(num))->dtor();

    ((SolarSystem *)(num))->getName();
    AEFile_Write_str(num, fd, true);
    ((String *)(num))->dtor();

    AEFile_Write_i32(((Status *)(*sh))->getCurrentCampaignMission(), fd);
    AEFile_Write_i32(((Status *)(*sh))->getLevel(), fd);
    AEFile_Write_f32(I(*(void **)g_RH_wp_float, 0x2c), fd);
    AEFile_Write_i32(((Ship *)(((Status *)(*sh))->getShip()))->getIndex(), fd);
    AEFile::Close(fd);
    ((String *)(path))->dtor();
    return 1;
}

// ---- writeAgent_cf9f0.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WA_guard;       // DAT_000dfc2c -> guard holder
__attribute__((visibility("hidden"))) extern const char g_WA_empty1[]; // DAT_000dfc38
__attribute__((visibility("hidden"))) extern const char g_WA_empty2[]; // DAT_000dfc3c

// RecordHandler::writeAgent(Agent*, unsigned int fd)
void RecordHandler::writeAgent(void *agentPtr, unsigned int fd) {
    RecordHandler *self = this;
    int *guardP = g_WA_guard;
    volatile int saved = *guardP;

    Agent *agent = (Agent *)agentPtr;
    AEFile_WriteInt(((Agent *)(agent))->getCosts(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getSellSystemIndex(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getSellBlueprintIndex(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getEvent(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getIndex(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getOffer(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getRace(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getSellItemIndex(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getSellItemPrice(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getSellItemQuantity(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getStation(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getSystem(), fd);
    AEFile_WriteInt(((Agent *)(agent))->getWingmanFriendsCount(), fd);
    AEFile_WriteBool(((Agent *)(agent))->isMale(), fd);
    AEFile_WriteBool(((Agent *)(agent))->hasReward(), fd);
    AEFile_WriteBool(((Agent *)(agent))->hasAcceptedOffer(), fd);
    AEFile_WriteBool(agent->field_0x24, fd);
    AEFile_WriteBool(agent->field_0x25, fd);

    if (((Agent *)(agent))->getImageParts() == 0) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(5, fd);
        for (int i = 0; i != 5; i++) {
            int *img = ((Agent *)(agent))->getImageParts();
            AEFile_WriteInt(img[i], fd);
        }
    }
    if (0x12 < ((Agent *)(agent))->getIndex()) {
        AEFile_WriteInt(((Agent *)(agent))->getSellModIndex(), fd);
    }

    char s[12];
    ((Agent *)(s))->getMissionString();
    AEFile_WriteString(s, fd, 1);
    ((Agent *)(s))->getName();
    AEFile_WriteString(s, fd, 1);
    ((Agent *)(s))->getStationName();
    AEFile_WriteString(s, fd, 1);
    ((Agent *)(s))->getSystemName();
    AEFile_WriteString(s, fd, 1);

    void *f0c = agent->wingman1;
    if (f0c == 0) {
        AEString_cstr_ctor(s, g_WA_empty1, 0);
        AEFile_WriteString(s, fd, 1);
    } else {
        AEFile_WriteString(f0c, fd, 1);
    }
    void *f10 = agent->wingman2;
    if (f10 == 0) {
        AEString_cstr_ctor(s, g_WA_empty2, 0);
        AEFile_WriteString(s, fd, 1);
    } else {
        AEFile_WriteString(f10, fd, 1);
    }

    self->currentAgent = agent;
    void *mission = ((Agent *)(agent))->getMission();
    if (mission == 0 || *(void **)self == mission) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(1, fd);
        ((RecordHandler *)(self))->writeMission(mission, fd);
    }

    return;
}

// ---- writeMission_cf840.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WM_guard;   // DAT_000df9e8 -> guard holder

// RecordHandler::writeMission(Mission*, unsigned int fd)
void RecordHandler::writeMission(void *m, unsigned int fd) {
    RecordHandler *self = this;
    int *guardP = g_WM_guard;
    volatile int saved = *guardP;

    AEFile_WriteInt(((Mission *)(m))->getType(), fd);
    if (((Mission *)(m))->isEmpty() == 0) {
        char s[12];
        ((Mission *)(s))->getClientName();
        AEFile_WriteString(s, fd, 1);
        ((Mission *)(s))->getTargetName();
        AEFile_WriteString(s, fd, 1);
        ((Mission *)(s))->getTargetStationName();
        AEFile_WriteString(s, fd, 1);
        ((Mission *)(s))->getTargetSystemName();
        AEFile_WriteString(s, fd, 1);

        AEFile_WriteBool(((Mission *)(m))->isCampaignMission(), fd);
        if (((Mission *)(m))->getClientImage() == 0) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(5, fd);
            for (int i = 0; i != 5; i++) {
                int *img = (int *)(long)((Mission *)(m))->getClientImage();
                AEFile_WriteInt(img[i], fd);
            }
        }
        AEFile_WriteInt(((Mission *)(m))->getClientRace(), fd);
        AEFile_WriteInt(((Mission *)(m))->getCosts(), fd);
        AEFile_WriteInt(((Mission *)(m))->getBonus(), fd);
        AEFile_WriteInt(((Mission *)(m))->getReward(), fd);
        AEFile_WriteInt(((Mission *)(m))->getTargetStation(), fd);
        AEFile_WriteInt(((Mission *)(m))->getDifficulty(), fd);
        AEFile_WriteInt(((Mission *)(m))->getProductionGoodIndex(), fd);
        AEFile_WriteInt(((Mission *)(m))->getProductionGoodAmount(), fd);
        AEFile_WriteInt(((Mission *)(m))->getStatusValue(), fd);
        AEFile_WriteBool(((Mission *)(m))->isVisible(), fd);

        *(void **)self = m;
        void *agent = ((Mission *)(m))->getAgent();
        if (agent == 0 || self->currentAgent == agent) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(1, fd);
            ((RecordHandler *)(self))->writeAgent(agent, fd);
        }
    }

    return;
}

// ---- loadOptions_cd450.cpp ----
__attribute__((visibility("hidden"))) extern int *g_LO_guard;        // DAT_000dd780 -> guard holder
__attribute__((visibility("hidden"))) extern unsigned char *g_LO_settings; // DAT_000dd784 -> settings struct base
__attribute__((visibility("hidden"))) extern int **g_LO_statusObj;   // DAT_000dd78c -> *->obj (field 0xfa)
__attribute__((visibility("hidden"))) extern int *g_LO_nameSlot;     // DAT_000dd794 -> name String* holder
__attribute__((visibility("hidden"))) extern int *g_LO_extraInt;     // DAT_000dd798 -> int
__attribute__((visibility("hidden"))) extern bool *g_LO_flag1;       // DAT_000dd79c -> bool
__attribute__((visibility("hidden"))) extern bool *g_LO_flag2;       // DAT_000dd7a0 -> bool
__attribute__((visibility("hidden"))) extern int *g_LO_textObj;      // DAT_000dd7a4 -> *->obj
__attribute__((visibility("hidden"))) extern int *g_LO_fontKind;     // DAT_000dd7a8 -> *->int
__attribute__((visibility("hidden"))) extern int *g_LO_fmodSlot;     // DAT_000dd7ac -> FModSound* holder

// RecordHandler::loadOptions()
void RecordHandler::loadOptions() {
    RecordHandler *self = this;
    int *guardP = g_LO_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile::FileExist(*(String *)(path)) != 0) {
        unsigned int fd;
        AEFile::OpenRead(*(String *)path, &fd);
        int valid = RecordHandler_checkHash(fd);
        AEFile::Close(fd);
        if (valid != 0) {
            AEFile::OpenRead(*(String *)path, &fd);
            unsigned char *s = g_LO_settings;

            AEFile_ReadByte(s + 0x10, fd);
            AEFile_ReadByte(s + 0x11, fd);
            *(unsigned char *)(*(int *)g_LO_statusObj + 0xfa) = s[0x11];
            AEFile_ReadInt(s + 0x14, fd);
            AEFile_ReadInt(s + 0x18, fd);
            AEFile_ReadInt(s + 0x1c, fd);
            AEFile_ReadInt(s + 0x00, fd);
            AEFile_ReadByte(s + 0x0c, fd);
            AEFile_ReadByte(s + 0x0d, fd);
            AEFile_ReadInt(s + 0x04, fd);
            AEFile_ReadInt(s + 0x24, fd);
            AEFile_ReadInt(s + 0x28, fd);

            int *nameSlot = g_LO_nameSlot;
            if (*nameSlot != 0) {
                void (**vt)(void *) = *(void (***)(void *))(*nameSlot);
                vt[1]((void *)(long)*nameSlot);
                *nameSlot = 0;
            }

            char name[12];
            AEString_default_ctor(name);
            int hasName = 0;
            // local_24 reuse — read marker int decides whether a name string follows
            AEFile_ReadString(name, fd, 0);
            (void)hasName;
            // (target reads a length field; if non-zero, allocate a String)
            void *ns = RH_op_new(0xc);
            char nameCopy[12];
            AEString_copy_ctor(nameCopy, name, 0);
            *nameSlot = (int)(long)ns;

            AEFile_ReadInt(g_LO_extraInt, fd);
            AEFile_ReadBool(g_LO_flag1, fd);
            AEFile_ReadInt(s + 0x54, fd);
            AEFile_ReadInt(s + 0x58, fd);
            AEFile_ReadFloat(s + 0x08, fd);
            AEFile_ReadBool(s + 0x0e, fd);
            AEFile_ReadBool(s + 0x32, fd);
            AEFile_ReadBool(s + 0x33, fd);
            AEFile_ReadBool(s + 0x34, fd);
            AEFile_ReadFloat(s + 0x20, fd);

            short lang = -1;
            AEFile_ReadShort(&lang, fd);
            AEFile_ReadBool(g_LO_flag2, fd);
            AEFile_ReadBool(s + 0x35, fd);
            AEFile_ReadBool(s + 0x36, fd);
            AEFile_ReadBool(s + 0x3a, fd);
            AEFile_ReadBool(s + 0x3b, fd);
            AEFile_ReadBool(s + 0x3c, fd);
            AEFile_ReadBool(s + 0x37, fd);
            AEFile_ReadBool(s + 0x3d, fd);
            AEFile_ReadBool(s + 0x3e, fd);
            AEFile_ReadBool(s + 0x3f, fd);
            AEFile_ReadBool(s + 0x40, fd);
            AEFile_ReadBool(s + 0x41, fd);
            AEFile_ReadFloat(s + 0x44, fd);
            AEFile_ReadBool(s + 0x48, fd);
            AEFile_ReadBool(s + 0x38, fd);
            AEFile_ReadBool(s + 0x4e, fd);
            AEFile_ReadBool(s + 0x49, fd);
            AEFile_ReadBool(s + 0x4a, fd);
            AEFile_ReadBool(s + 0x4b, fd);
            AEFile_ReadBool(s + 0x4c, fd);
            AEFile_ReadBool(s + 0x4d, fd);
            AEFile_ReadInt(s + 0x50, fd);
            AEFile_ReadBool(s + 0x60, fd);
            AEFile_ReadBool(s + 0x61, fd);
            AEFile::Close(fd);

            int langVal = lang;
            if (-1 < langVal) {
                if (langVal == 9) langVal = 0;
                ((GameText *)((short)**(int **)g_LO_textObj))->setLanguage_i(langVal);
                Globals_loadFont(**(int **)g_LO_fontKind);
            }

            int *fmodSlot = g_LO_fmodSlot;
            if (*fmodSlot != 0) {
                void *fm = (void *)(long)*fmodSlot;
                ((FModSound *)(fm))->setAudioLanguage(lang);
                // Decompiler collapsed the per-category enable/volume args; reconstructed
                // from the just-read flag fields (s+0x32/0x33/0x34) and volume floats
                // (s+0x08/0x20/0x44). enableCategory(int,bool); setVolume(int,float).
                ((FModSound *)(fm))->enableCategory(0, *(bool *)(s + 0x32));
                ((FModSound *)(fm))->enableCategory(1, *(bool *)(s + 0x33));
                ((FModSound *)(fm))->enableCategory(2, *(bool *)(s + 0x34));
                ((FModSound *)(fm))->setVolume(0, *(float *)(s + 0x08));
                ((FModSound *)(fm))->setVolume(1, *(float *)(s + 0x20));
                ((FModSound *)(fm))->setVolume(2, *(float *)(s + 0x44));
            }

            AEFile_ReadBool(s + 0x62, fd);
        }
    }

    return;
}

// ---- loadResolutionValue_cd260.cpp ----
__attribute__((visibility("hidden"))) extern int *g_LRV_guard;   // DAT_000dd444 -> guard holder

// RecordHandler::loadResolutionValue() — reads the options file at self+8 into a settings
// scratch struct. self is in r0; the path String lives at self+8.
void RecordHandler::loadResolutionValue() {
    RecordHandler *self = this;
    int *guardP = g_LRV_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile::FileExist(*(String *)(path)) != 0) {
        unsigned int fd;
        AEFile::OpenRead(*(String *)path, &fd);

        // Scratch settings record (mirrors the on-stack layout the target fills in).
        unsigned char buf[0x4c];
        char *r = (char *)buf;

        AEFile_ReadByte(r + 0x10, fd);
        AEFile_ReadByte(r + 0x11, fd);
        AEFile_ReadInt(r + 0x14, fd);
        AEFile_ReadInt(r + 0x18, fd);
        AEFile_ReadInt(r + 0x1c, fd);
        AEFile_ReadInt(r + 0x00, fd);
        AEFile_ReadByte(r + 0x0c, fd);
        AEFile_ReadByte(r + 0x0d, fd);
        AEFile_ReadInt(r + 0x04, fd);
        AEFile_ReadInt(r + 0x24, fd);
        AEFile_ReadInt(r + 0x28, fd);

        char name[12];
        AEString_default_ctor(name);
        AEFile_ReadString(name, fd, 0);

        int iv9c;
        bool b9d;
        AEFile_ReadInt(&iv9c, fd);
        AEFile_ReadBool(&b9d, fd);
        AEFile_ReadInt(r + 0x54, fd);
        AEFile_ReadInt(r + 0x58, fd);
        AEFile_ReadFloat(r + 0x08, fd);
        AEFile_ReadBool(r + 0x0e, fd);
        AEFile_ReadBool(r + 0x32, fd);
        AEFile_ReadBool(r + 0x33, fd);
        AEFile_ReadBool(r + 0x34, fd);
        AEFile_ReadFloat(r + 0x20, fd);

        short resVal = -1;
        AEFile_ReadShort(&resVal, fd);
        AEFile_ReadBool(&b9d, fd);
        AEFile_ReadBool(r + 0x35, fd);
        AEFile_ReadBool(r + 0x36, fd);
        AEFile_ReadBool(r + 0x3a, fd);
        AEFile_ReadBool(r + 0x3b, fd);
        AEFile_ReadBool(r + 0x3c, fd);
        AEFile_ReadBool(r + 0x37, fd);
        AEFile_ReadBool(r + 0x3d, fd);
        AEFile_ReadBool(r + 0x3e, fd);
        AEFile_ReadBool(r + 0x3f, fd);
        AEFile_ReadBool(r + 0x40, fd);
        AEFile_ReadBool(r + 0x41, fd);
        AEFile_ReadFloat(r + 0x44, fd);

        AEFile::Close(fd);
    }

    return;
}

// ---- writeByteArrayAsRecord_cdcb0.cpp ----
// RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool)
int RecordHandler::writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup) {
    RecordHandler *self = this;
    char num[12];
    char path[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    char *dir = (char *)self + (toBackup ? 0x20 : 0x14);
    String_concat(path, dir, num);
    ((String *)(num))->dtor();

    if (AEFile::FileExist(*(String *)(path)) != 0)
        AEFile::FileDelete(*(String *)(path));
    AEFile::OpenWrite(*(String *)path, &fd);
    ((AEFile *)(n))->Write(buf, fd);
    AEFile::Close(fd);
    ((String *)(path))->dtor();
    return n;
}

// ---- readMission_cdfb4.cpp ----
extern "C" void Mission_ctorFull(void *self, int type, void *clientName, int *img, int clientRace,
                                 int reward, int targetStation, int difficulty);

__attribute__((visibility("hidden"))) extern int *g_RM_guard;   // DAT_000de244 -> guard holder

// RecordHandler::readMission(unsigned int fd) -> Mission* (in r0).
void * RecordHandler::readMission(unsigned int fd) {
    RecordHandler *self = this;
    int *guardP = g_RM_guard;
    volatile int saved = *guardP;
    void *mission = 0;

    int type = 0;
    AEFile_ReadInt(&type, fd);
    if (type != -1) {
        char clientName[12], targetName[12], targetStation[12], targetSystem[12];
        AEString_default_ctor(clientName);
        AEFile_ReadString(clientName, fd, 1);
        AEString_default_ctor(targetName);
        AEFile_ReadString(targetName, fd, 1);
        AEString_default_ctor(targetStation);
        AEFile_ReadString(targetStation, fd, 1);
        AEString_default_ctor(targetSystem);
        AEFile_ReadString(targetSystem, fd, 1);

        bool isEmpty = false;
        AEFile_ReadBool(&isEmpty, fd);

        unsigned imgCount = 0;
        AEFile_ReadInt(&imgCount, fd);
        int *img = 0;
        if ((int)imgCount >= 1) {
            unsigned long long bytes = (unsigned long long)imgCount * 4;
            unsigned sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned)bytes;
            img = (int *)RH_op_new_arr(sz);
            int *p = img;
            for (int i = 0; i < (int)imgCount; i++) {
                AEFile_ReadInt(p, fd);
                p++;
            }
        }

        int clientRace = 0, costs = 0, bonus = 0, reward = 0, targetStationIdx = 0;
        int difficulty = 0, prodIdx = 0, prodAmt = 0, statusValue = 0;
        bool visible = false;
        AEFile_ReadInt(&clientRace, fd);
        AEFile_ReadInt(&costs, fd);
        AEFile_ReadInt(&bonus, fd);
        AEFile_ReadInt(&reward, fd);
        AEFile_ReadInt(&targetStationIdx, fd);
        AEFile_ReadInt(&difficulty, fd);
        AEFile_ReadInt(&prodIdx, fd);
        AEFile_ReadInt(&prodAmt, fd);
        AEFile_ReadInt(&statusValue, fd);
        AEFile_ReadBool(&visible, fd);

        int hasAgent = 0;
        AEFile_ReadInt(&hasAgent, fd);
        void *agent = (hasAgent < 1) ? 0 : ((RecordHandler *)(self))->readAgent(fd);

        if (!isEmpty) {
            mission = RH_op_new(0x78);
            char nameCopy[12];
            AEString_copy_ctor(nameCopy, clientName, 0);
            Mission_ctorFull(mission, type, nameCopy, img, clientRace, reward, targetStationIdx,
                             difficulty);
        } else {
            mission = RH_op_new(0x78);
            Mission_ctorEmpty(mission, type, reward, targetStationIdx);
        }
        ((Mission *)(mission))->setCosts(costs);
        ((Mission *)(mission))->setBonus(bonus);
        ((Mission *)(mission))->setProductionGoods(prodIdx, prodAmt);
        ((Mission *)(mission))->setStatusValue(statusValue);
        ((Mission *)(mission))->setVisible(visible);
        ((Mission *)(mission))->setAgent((Agent *)agent);

        String12 tgtNameCopy;
        AEString_copy_ctor(&tgtNameCopy, targetName, 0);
        ((Mission *)(mission))->setTargetName(tgtNameCopy);

        if (agent != 0) {
            ((Agent *)(agent))->setMission((Mission *)mission);
        }

    }

    return mission;
}

// ---- saveOptions_cd8c8.cpp ----
__attribute__((visibility("hidden"))) extern int *g_SO_guard;        // DAT_000ddad4 -> guard holder
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_settings; // DAT_000ddad8 -> settings struct
__attribute__((visibility("hidden"))) extern void **g_SO_playerName; // DAT_000ddae4 -> *->String*
__attribute__((visibility("hidden"))) extern const char g_SO_defName[]; // DAT_000ddae8
__attribute__((visibility("hidden"))) extern int *g_SO_extraInt;     // DAT_000ddaec -> *->int
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_flag1; // DAT_000ddaf4 -> byte
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_flag2; // DAT_000ddaf8 -> byte

// RecordHandler::saveOptions()
void RecordHandler::saveOptions() {
    RecordHandler *self = this;
    int *guardP = g_SO_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile::FileExist(*(String *)(path)) != 0) {
        AEFile::FileDelete(*(String *)(path));
    }
    unsigned int fd;
    AEFile::OpenWrite(*(String *)path, &fd);

    unsigned char *s = g_SO_settings;

    AEFile_WriteByte(s[0x10], fd);
    AEFile_WriteByte(s[0x11], fd);
    AEFile_WriteInt(*(int *)(s + 0x14), fd);
    AEFile_WriteInt(*(int *)(s + 0x18), fd);
    AEFile_WriteInt(*(int *)(s + 0x1c), fd);
    AEFile_WriteInt(*(int *)(s + 0x00), fd);
    AEFile_WriteByte(s[0x0c], fd);
    AEFile_WriteByte(s[0x0d], fd);
    AEFile_WriteInt(*(int *)(s + 0x04), fd);
    AEFile_WriteInt(*(int *)(s + 0x24), fd);
    AEFile_WriteInt(*(int *)(s + 0x28), fd);

    void *name = *g_SO_playerName;
    if (name == 0) {
        char def[12];
        AEString_cstr_ctor(def, g_SO_defName, 0);
        AEFile_WriteString(def, fd, 0);
    } else {
        AEFile_WriteString(name, fd, 0);
    }

    AEFile_WriteInt(*g_SO_extraInt, fd);
    AEFile_WriteByte(*g_SO_flag1, fd);
    AEFile_WriteInt(*(int *)(s + 0x54), fd);
    AEFile_WriteInt(*(int *)(s + 0x58), fd);
    AEFile_WriteInt(*(int *)(s + 0x08), fd);
    AEFile_WriteByte(s[0x0e], fd);
    AEFile_WriteByte(s[0x32], fd);
    AEFile_WriteByte(s[0x33], fd);
    AEFile_WriteByte(s[0x34], fd);
    AEFile_WriteInt(*(int *)(s + 0x20), fd);

    AEFile_WriteShort(GameText_getLanguage(), fd);
    AEFile_WriteByte(*g_SO_flag2, fd);
    AEFile_WriteByte(s[0x35], fd);
    AEFile_WriteByte(s[0x36], fd);
    AEFile_WriteByte(s[0x3a], fd);
    AEFile_WriteByte(s[0x3b], fd);
    AEFile_WriteByte(s[0x3c], fd);
    AEFile_WriteByte(s[0x37], fd);
    AEFile_WriteByte(s[0x3d], fd);
    AEFile_WriteByte(s[0x3e], fd);
    AEFile_WriteByte(s[0x3f], fd);
    AEFile_WriteByte(s[0x40], fd);
    AEFile_WriteByte(s[0x41], fd);
    AEFile_WriteInt(*(int *)(s + 0x44), fd);
    AEFile_WriteByte(s[0x48], fd);
    AEFile_WriteByte(s[0x38], fd);
    AEFile_WriteByte(s[0x4e], fd);
    AEFile_WriteByte(s[0x49], fd);
    AEFile_WriteByte(s[0x4a], fd);
    AEFile_WriteByte(s[0x4b], fd);
    AEFile_WriteByte(s[0x4c], fd);
    AEFile_WriteByte(s[0x4d], fd);
    AEFile_WriteInt(*(int *)(s + 0x50), fd);
    AEFile_WriteByte(s[0x60], fd);
    AEFile_WriteByte(s[0x61], fd);
    AEFile_WriteByte(s[0x62], fd);
    AEFile::Close(fd);
    ((RecordHandler *)(self))->addHashToOptions();

    return;
}

// ---- readAgent_ce250.cpp ----

__attribute__((visibility("hidden"))) extern int *g_RA_guard;   // DAT_000de608 -> guard holder

// RecordHandler::readAgent(unsigned int fd) -> Agent* (in r0).
void * RecordHandler::readAgent(unsigned int fd) {
    RecordHandler *self = this;
    int *guardP = g_RA_guard;
    volatile int saved = *guardP;

    int costs = 0, sellSys = 0, sellBp = 0, event = 0, idx = 0, offer = 0, race = 0;
    int sellItemIdx = 0, sellItemPrice = 0, sellItemQty = 0, station = 0, system = 0;
    unsigned wingmen = 0;
    AEFile_ReadInt(&costs, fd);
    AEFile_ReadInt(&sellSys, fd);
    AEFile_ReadInt(&sellBp, fd);
    AEFile_ReadInt(&event, fd);
    AEFile_ReadInt(&idx, fd);
    AEFile_ReadInt(&offer, fd);
    AEFile_ReadInt(&race, fd);
    AEFile_ReadInt(&sellItemIdx, fd);
    AEFile_ReadInt(&sellItemPrice, fd);
    AEFile_ReadInt(&sellItemQty, fd);
    AEFile_ReadInt(&station, fd);
    AEFile_ReadInt(&system, fd);
    AEFile_ReadInt(&wingmen, fd);

    unsigned char male = 0;
    bool hasReward = false, accepted = false;
    unsigned char raw24 = 0, raw25 = 0;
    AEFile_ReadByte(&male, fd);
    AEFile_ReadByte(&hasReward, fd);
    AEFile_ReadByte(&accepted, fd);
    AEFile_ReadByte(&raw24, fd);
    AEFile_ReadByte(&raw25, fd);

    unsigned imgCount = 0;
    AEFile_ReadInt(&imgCount, fd);
    int *img = 0;
    if (0 < (int)imgCount) {
        unsigned long long bytes = (unsigned long long)imgCount * 4;
        unsigned sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned)bytes;
        img = (int *)RH_op_new_arr(sz);
        int *p = img;
        for (int i = 0; i < (int)imgCount; i++) {
            AEFile_ReadInt(p, fd);
            p++;
        }
    }

    int sellMod = -1;
    if (0x12 < idx) {
        AEFile_ReadInt(&sellMod, fd);
    }

    char missionStr[12], name[12], stationName[12], systemName[12], strE[12], strF[12];
    AEString_default_ctor(missionStr);
    AEString_default_ctor(name);
    AEString_default_ctor(stationName);
    AEString_default_ctor(systemName);
    AEString_default_ctor(strE);
    AEString_default_ctor(strF);
    AEFile_ReadString(missionStr, fd, 1);
    AEFile_ReadString(name, fd, 1);
    AEFile_ReadString(stationName, fd, 1);
    AEFile_ReadString(systemName, fd, 1);
    AEFile_ReadString(strE, fd, 1);
    AEFile_ReadString(strF, fd, 1);

    int hasMission = 0;
    AEFile_ReadInt(&hasMission, fd);
    void *mission = (hasMission < 1) ? 0 : ((RecordHandler *)(self))->readMission(fd);

    Agent *agent = (Agent *)RH_op_new(0x98);
    char nameCopy[12];
    AEString_copy_ctor(nameCopy, name, 0);
    ((Agent *)(agent))->ctor(idx, nameCopy, station, system, race, male, sellSys, sellBp, sellMod, sellItemIdx);

    ((Agent *)(agent))->setCosts(costs);
    ((Agent *)(agent))->setEvent(event);
    ((Agent *)(agent))->setOffer(offer);
    ((Agent *)(agent))->setSellItemData(sellItemPrice, sellItemQty, sellItemIdx);

    if (*((int *)strE + 2) != 0) {
        void *s = RH_op_new(0xc);
        AEString_copy_ctor(s, strE, 0);
        agent->wingman1 = s;
    }
    if (*((int *)strF + 2) != 0) {
        void *s = RH_op_new(0xc);
        AEString_copy_ctor(s, strF, 0);
        agent->wingman2 = s;
    }
    agent->wingmanCount = wingmen;

    void *arr = RH_op_new(0xc);
    ArrayStr_ctor(arr);
    ArraySetLength_Str(wingmen, arr);
    for (int i = 0; i < (int)wingmen; i++) {
        void *s = RH_op_new(0xc);
        void *src = (i == 0) ? (void *)strE : (void *)strF;
        AEString_copy_ctor(s, src, 0);
        *(void **)(*(char **)((char *)arr + 4) + i * 4) = s;
    }
    ((Agent *)(agent))->setWingmanFriendNames((uint32_t *)arr);
    ((Agent *)(agent))->giveRewardAtNextChat(hasReward);
    ((Agent *)(agent))->setOfferAccepted(accepted);
    ((Agent *)(agent))->setImageParts(img);

    String12 tmp;
    AEString_copy_ctor(&tmp, missionStr, 0);
    ((Agent *)(agent))->setMissionString(&tmp);
    AEString_copy_ctor(&tmp, stationName, 0);
    ((Agent *)(agent))->setStationName(tmp);
    AEString_copy_ctor(&tmp, systemName, 0);
    ((Agent *)(agent))->setSystemName(tmp);
    ((Agent *)(agent))->setMission((Mission *)mission);

    agent->field_0x24 = raw24;
    agent->field_0x25 = raw25;


    return agent;
}

// ---- writeWanted_cfc40.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WW_guard;   // DAT_000dfd70 -> guard holder

// RecordHandler::writeWanted(Wanted*, unsigned int fd)
void RecordHandler::writeWanted(void *w, unsigned int fd) {
    RecordHandler *self = this;
    (void)self;
    int *guardP = g_WW_guard;
    volatile int saved = *guardP;

    AEFile_WriteBool(((Wanted *)(w))->isActive(), fd);
    AEFile_WriteBool(((Wanted *)(w))->isTerminated(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getCurrentLocation(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getTravelsTo(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getLastSeen(), fd);

    char name[12];
    ((Wanted *)(name))->getName();
    AEFile_WriteString(name, fd, 1);

    AEFile_WriteInt(((Wanted *)(w))->getIndex(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getBoard(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getRace(), fd);
    AEFile_WriteBool(((Wanted *)(w))->isMale(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getShip(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getWeapon(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getHitpoints(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getLoot(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getLootAmount(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getReward(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getRequiredBounties(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getRequiredMission(), fd);
    AEFile_WriteInt(((Wanted *)(w))->getNumWingmen(), fd);

    for (int i = 0; i != 5; i++) {
        int *parts = ((Wanted *)(w))->getImageParts();
        AEFile_WriteInt(parts[i], fd);
    }

    return;
}

// ---- recordStoreWritePreview_cfea4.cpp ----
struct Station;
struct Ship;


// RecordHandler::recordStoreWritePreview(GameRecord*, int)
int RecordHandler::recordStoreWritePreview(void *rec, int slot) {
    RecordHandler *self = this;
    if (rec == 0)
        return 0;

    char path[12];
    char num[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    ((String *)(num))->dtor();

    if (AEFile::FileExist(*(String *)(path)) != 0)
        AEFile::FileDelete(*(String *)(path));
    AEFile::OpenWrite(*(String *)path, &fd);

    AEFile_Write_i64(*(long long *)((char *)rec + 0x10), fd);
    AEFile_Write_i32(I(rec, 0x8), fd);

    ((Station *)(num))->getName();
    AEFile_Write_str(num, fd, true);
    ((String *)(num))->dtor();

    AEFile_Write_str((char *)rec + 0x188, fd, true);
    AEFile_Write_i32(I(rec, 0x40), fd);
    AEFile_Write_i32(I(rec, 0x20), fd);
    AEFile_Write_f32(I(rec, 0x11c), fd);
    AEFile_Write_i32(((Ship *)(*(Ship **)((char *)rec + 0x130)))->getIndex(), fd);
    AEFile::Close(fd);
    ((String *)(path))->dtor();
    return 1;
}

// ---- recordStoreWrite_ce8ac.cpp ----
// The remaining game-state serialization (ship, station, standings, items, missions, agents).
// A long straight-line writer over the live object graph; delegated to a helper to keep this
// translation tractable while preserving the open/write-prefix/close/hash control flow.

__attribute__((visibility("hidden"))) extern int *g_RSW_guard;    // DAT_000dec58 -> guard holder
__attribute__((visibility("hidden"))) extern void **g_RSW_galaxy; // DAT_000dec5c -> *->Galaxy
__attribute__((visibility("hidden"))) extern int *g_RSW_status;   // DAT_000dec60 -> *->Status

// RecordHandler::recordStoreWrite(int slot)
void RecordHandler::recordStoreWrite(int slot) {
    RecordHandler *self = this;
    int *guardP = g_RSW_guard;
    volatile int saved = *guardP;

    char num[12], path[12];
    AEString_int_ctor(num, slot);
    AEString_concat(path, (char *)self + 0x14);

    if (AEFile::FileExist(*(String *)(path)) != 0) {
        AEFile::FileDelete(*(String *)(path));
    }
    unsigned int fd;
    AEFile::OpenWrite(*(String *)path, &fd);

    // Visited-systems bitmap (0x87 entries).
    long visited = (long)((Galaxy *)(*g_RSW_galaxy))->getVisited();
    AEFile_WriteInt(0x87, fd);
    for (unsigned i = 0; i < 0x87; i++) {
        AEFile_WriteBool(*(bool *)(visited + i), fd);
    }

    int *status = g_RSW_status;
    Status *st = (Status *)(void *)(long)*status;
    AEFile_WriteInt(((Status *)(st))->getCredits(), fd);
    AEFile_WriteInt(((Status *)(st))->getRating(), fd);
    AEFile_WriteLong(((Status *)(st))->getPlayingTime(), fd);
    AEFile_WriteInt(((Status *)(st))->getKills(), fd);
    AEFile_WriteInt(((Status *)(st))->getMissionCount(), fd);
    AEFile_WriteInt(((Status *)(st))->getLevel(), fd);
    AEFile_WriteInt(((Status *)(st))->getLastXP(), fd);
    AEFile_WriteInt(((Status *)(st))->getGoodsProduced(), fd);
    AEFile_WriteInt(((Status *)(st))->getStationsVisited(), fd);
    AEFile_WriteInt(((Status *)(st))->getCurrentCampaignMission(), fd);
    ((RecordHandler *)(self))->writeMission(st->getFreelanceMission(), fd);
    ((RecordHandler *)(self))->writeMission((void *)(intptr_t)st->getCampaignMission(), fd);
    AEFile_WriteInt(((Status *)(st))->getJumpgateUsed(), fd);
    AEFile_WriteInt(((Status *)(st))->getCapturedCrates(), fd);
    AEFile_WriteInt(((Status *)(st))->getBoughtEquipment(), fd);
    AEFile_WriteInt(((Status *)(st))->getPirateKills(), fd);
    AEFile_WriteInt(*(int *)(*status + 0x80), fd);

    // Remaining object-graph serialization.
    ((RecordHandler *)(self))->recordStoreWrite_body(fd);

    AEFile::Close(fd);
    ((RecordHandler *)(self))->addHash(slot);

    return;
}

// ---- recordStoreWrite body (game object-graph serialization) ----
// Continuation of recordStoreWrite(): writes the live object graph (boolean bitmaps, item
// and ship inventories for the player/wingman/stations, standings, blueprints, pending
// products, wingmen list, agents, option flags, wanteds, bounties and the ship-mod tables)
// to the already-open file `fd`. The caller handles open/prefix, Close and addHash.
__attribute__((visibility("hidden"))) extern void **g_RSW_status_p;   // -> Status*
__attribute__((visibility("hidden"))) extern void **g_RSW_achievements; // -> Achievements*
__attribute__((visibility("hidden"))) extern void  *g_RSW_optFlags;   // option-flag block base
__attribute__((visibility("hidden"))) extern void  *g_RSW_uiFlags;    // secondary flag block base
__attribute__((visibility("hidden"))) extern int    g_RSW_modVersion; // ship-mod table version tag

// Item / Achievements / Standing live in out-of-batch headers whose full layout is not
// pulled in here; their accessors are reached through extern "C" shims (codebase style).

// Helper: write a length-prefixed bool[] backed by an Array<bool> at status+off.
static void RSW_writeBoolArray(void *status, int off, unsigned int fd) {
    unsigned int *arr = *(unsigned int **)((char *)status + off);
    AEFile_WriteInt(*arr, fd);
    for (unsigned i = 0; i < *arr; i++) {
        AEFile_WriteBool(*(bool *)(arr[1] + i), fd);
    }
}

// Helper: write a length-prefixed int[] backed by an Array<int> at status+off.
static void RSW_writeIntArray(void *status, int off, unsigned int fd) {
    unsigned int *arr = *(unsigned int **)((char *)status + off);
    AEFile_WriteInt(*arr, fd);
    for (unsigned i = 0; i < *arr; i++) {
        AEFile_WriteInt(*(int *)(arr[1] + i * 4), fd);
    }
}

// Helper: write a ship's equipment list (index, amount, unsaleable) or 0 when absent.
static void RSW_writeEquipment(Ship *ship, unsigned int fd) {
    unsigned int *eq = (unsigned int *)((Ship *)ship)->getEquipment();
    if (eq == 0) { AEFile_WriteInt(0, fd); return; }
    AEFile_WriteInt(*eq, fd);
    for (unsigned i = 0; i < *eq; i++) {
        Item *it = *(Item **)(eq[1] + i * 4);
        if (it == 0) { AEFile_WriteInt(-1, fd); continue; }
        AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
        AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
        AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
    }
}

// Helper: write a ship's cargo list (index, amount, single price, unsaleable) or 0.
static void RSW_writeCargo(Ship *ship, unsigned int fd) {
    unsigned int *cg = (unsigned int *)((Ship *)ship)->getCargo();
    if (cg == 0) { AEFile_WriteInt(0, fd); return; }
    AEFile_WriteInt(*cg, fd);
    for (unsigned i = 0; i < *cg; i++) {
        Item *it = *(Item **)(cg[1] + i * 4);
        AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
        AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
        AEFile_WriteInt(((Item *)(it))->getSinglePrice(), fd);
        AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
    }
}

// Helper: write a ship's installed-mod list or 0 when the ship has none.
static void RSW_writeMods(Ship *ship, unsigned int fd) {
    if (ship == 0) { AEFile_WriteInt(0, fd); return; }
    unsigned int *mods = (unsigned int *)((Ship *)ship)->getMods();
    if (mods == 0) { AEFile_WriteInt(0, fd); return; }
    AEFile_WriteInt(*mods, fd);
    for (unsigned i = 0; i < *mods; i++) {
        AEFile_WriteInt(*(int *)(mods[1] + i * 4), fd);
    }
}

void RecordHandler::recordStoreWrite_body(unsigned int fd) {
    void *status = *g_RSW_status_p;
    char *flags = (char *)g_RSW_optFlags;   // option/progress flag block
    char *ui = (char *)g_RSW_uiFlags;       // secondary flag block

    // Standings unlock bitmap and the various progress / discovery bitmaps.
    AEFile_WriteInt(*(int *)((char *)status + 0x7c), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x84), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x88), fd);
    RSW_writeBoolArray(status, 0x94, fd);
    RSW_writeBoolArray(status, 0x98, fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x9c), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0xa0), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0xa4), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0xa8), fd);
    RSW_writeBoolArray(status, 0xac, fd);
    AEFile_WriteInt(*(int *)((char *)status + 0xb0), fd);
    RSW_writeBoolArray(status, 0xb4, fd);
    AEFile_WriteInt(*(int *)((char *)status + 0xb8), fd);
    AEFile_WriteLong(*(long long *)((char *)status + 0xc0), fd);
    for (int off = 0xc8; off <= 0xec; off += 4) {
        AEFile_WriteInt(*(int *)((char *)status + off), fd);
    }

    // Achievement medals (0x2d entries).
    int medals = (int)(intptr_t)((Achievements *)(*g_RSW_achievements))->getMedals();
    AEFile_WriteInt(0x2d, fd);
    for (unsigned i = 0; i < 0x2d; i++) {
        AEFile_WriteInt(*(int *)(medals + i * 4), fd);
    }

    // Player ship: index, race, equipment, cargo.
    Ship *ship = ((Status *)status)->getShip();
    AEFile_WriteInt(((Ship *)ship)->getIndex(), fd);
    AEFile_WriteInt(((Ship *)ship)->getRace(), fd);
    RSW_writeEquipment(ship, fd);
    RSW_writeCargo(ship, fd);

    // Station stack (3 fixed slots + current station): items, ships, agents, hostility flag.
    unsigned int *stack = (unsigned int *)((Status *)status)->getStationStack();
    AEFile_WriteInt(3, fd);
    for (unsigned i = 0; i < *stack + 1; i++) {
        Station *cur = (i == *stack) ? (Station *)((Status *)status)->getStation()
                                     : *(Station **)(stack[1] + i * 4);
        if (cur == 0) { AEFile_WriteInt(-1, fd); continue; }
        AEFile_WriteInt(((Station *)cur)->getIndex(), fd);

        unsigned int *items = (unsigned int *)((Station *)cur)->getItems();
        if (items == 0) { AEFile_WriteInt(0, fd); }
        else {
            AEFile_WriteInt(*items, fd);
            for (unsigned j = 0; j < *items; j++) {
                Item *it = *(Item **)(items[1] + j * 4);
                AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
                AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
                AEFile_WriteInt(((Item *)(it))->getSinglePrice(), fd);
                AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
            }
        }
        unsigned int *ships = (unsigned int *)((Station *)cur)->getShips();
        if (ships == 0) { AEFile_WriteInt(0, fd); }
        else {
            AEFile_WriteInt(*ships, fd);
            for (unsigned j = 0; j < *ships; j++) {
                Ship *s = *(Ship **)(ships[1] + j * 4);
                AEFile_WriteInt(((Ship *)s)->getIndex(), fd);
                AEFile_WriteInt(((Ship *)s)->getRace(), fd);
            }
        }
        unsigned int *agents = (unsigned int *)((Station *)cur)->getAgents();
        if (agents == 0) { AEFile_WriteInt(0, fd); }
        else {
            AEFile_WriteInt(*agents, fd);
            for (unsigned j = 0; j < *agents; j++) {
                ((RecordHandler *)this)->writeAgent(*(void **)(agents[1] + j * 4), fd);
            }
        }
        AEFile_WriteBool(((Station *)cur)->hasAttackedFriends(), fd);
    }

    // Standings (2 races).
    int standings = (int)(intptr_t)((Standing *)(((Status *)(status))->getStanding()))->getStandings();
    AEFile_WriteInt(2, fd);
    for (unsigned i = 0; i < 2; i++) {
        AEFile_WriteInt(*(int *)(standings + i * 4), fd);
    }

    // Blueprints: per blueprint the ingredient list, then its scalar fields and name.
    unsigned int *bps = (unsigned int *)((Status *)status)->getBluePrints();
    AEFile_WriteInt(*bps, fd);
    for (unsigned i = 0; i < *bps; i++) {
        int *bp = *(int **)(bps[1] + i * 4);
        unsigned int *ingredients = (unsigned int *)bp[0];
        for (unsigned j = 0; j < *ingredients; j++) {
            AEFile_WriteInt(*(int *)(ingredients[1] + j * 4), fd);
        }
        AEFile_WriteInt(bp[1], fd);
        AEFile_WriteBool((bool)(char)bp[2], fd);
        AEFile_WriteInt(bp[3], fd);
        AEFile_WriteInt(bp[4], fd);
        AEFile_WriteString((char *)(bp + 5), fd, true);
    }

    // Pending products (only the populated slots are written; -1 when none).
    unsigned int *pending = (unsigned int *)((Status *)status)->getPendingProducts();
    if (pending == 0) { AEFile_WriteInt(-1, fd); }
    else {
        int count = 0;
        for (unsigned i = 0; i < *pending; i++) {
            if (*(int *)(pending[1] + i * 4) != 0) count++;
        }
        if (count == 0) { AEFile_WriteInt(-1, fd); }
        else {
            AEFile_WriteInt(count, fd);
            for (unsigned i = 0; i < *pending; i++) {
                int pp = *(int *)(pending[1] + i * 4);
                if (pp == 0) continue;
                AEFile_WriteInt(*(int *)(pp + 0x14), fd);
                AEFile_WriteInt(*(int *)(pp + 0x10), fd);
                AEFile_WriteInt(*(int *)(pp + 0xc), fd);
                AEFile_WriteString(*(char **)(pending[1] + i * 4), fd, true);
            }
        }
    }

    // Wingmen list (String[]) + its trailing scalar fields, or -1 when absent.
    unsigned int *wingmen = *(unsigned int **)((char *)status + 0x24);
    if (wingmen == 0) { AEFile_WriteInt(-1, fd); }
    else {
        AEFile_WriteInt(*wingmen, fd);
        for (unsigned i = 0; i < *wingmen; i++) {
            AEFile_WriteString(*(char **)(wingmen[1] + i * 4), fd, true);
        }
        AEFile_WriteInt(*(int *)((char *)status + 0x2c), fd);
        AEFile_WriteInt(*(int *)((char *)status + 0x30), fd);
        AEFile_WriteInt(5, fd);
        for (unsigned i = 0; i < 5; i++) {
            AEFile_WriteInt(*(int *)(*(int *)((char *)status + 0x28) + i * 4), fd);
        }
    }

    AEFile_WriteInt(*(int *)((char *)status + 0x34), fd);
    RSW_writeBoolArray(status, 0x38, fd);
    RSW_writeIntArray(status, 0x40, fd);
    RSW_writeIntArray(status, 0x3c, fd);
    RSW_writeIntArray(status, 0x48, fd);
    RSW_writeIntArray(status, 0x44, fd);
    RSW_writeBoolArray(status, 0x4c, fd);

    // Agents.
    unsigned int *agents = (unsigned int *)((Status *)status)->getAgents();
    AEFile_WriteInt(*agents, fd);
    for (unsigned i = 0; i < *agents; i++) {
        ((RecordHandler *)this)->writeAgent(*(void **)(agents[1] + i * 4), fd);
    }

    // Tutorial / one-shot progress flags (option block +0x8 .. +0x24).
    for (int off = 0x8; off <= 0x20; off++) {
        if (off == 0x21) continue;            // 0x22 is written before 0x21 below
        AEFile_WriteBool(*(bool *)(flags + off), fd);
    }
    AEFile_WriteBool(*(bool *)(flags + 0x22), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x21), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x23), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x24), fd);

    AEFile_WriteLong(*(long long *)((char *)status + 0x100), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x25), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x26), fd);

    // Optional wingman ship (index, race, equipment, cargo) or 0.
    Ship *wingShip = *(Ship **)((char *)status + 0x8c);
    if (wingShip == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(1, fd);
        AEFile_WriteInt(((Ship *)wingShip)->getIndex(), fd);
        AEFile_WriteInt(((Ship *)wingShip)->getRace(), fd);
        RSW_writeEquipment(wingShip, fd);
        RSW_writeCargo(wingShip, fd);
    }

    RSW_writeIntArray(status, 0x90, fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x10c), fd);
    AEFile_WriteBool(*(bool *)((char *)status + 0x110), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x114), fd);
    AEFile_WriteBool(*(bool *)((char *)status + 0x111), fd);

    // Current station's items / ships (re-read off the current station).
    Station *station = (Station *)((Status *)status)->getStation();
    unsigned int *sItems = (unsigned int *)((Station *)station)->getItems();
    if (sItems == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(*sItems, fd);
        for (unsigned i = 0; i < *sItems; i++) {
            Item *it = *(Item **)(sItems[1] + i * 4);
            AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
            AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
            AEFile_WriteInt(((Item *)(it))->getSinglePrice(), fd);
            AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
        }
    }
    unsigned int *sShips = (unsigned int *)((Station *)station)->getShips();
    if (sShips == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(*sShips, fd);
        for (unsigned i = 0; i < *sShips; i++) {
            Ship *s = *(Ship **)(sShips[1] + i * 4);
            AEFile_WriteInt(((Ship *)s)->getIndex(), fd);
            AEFile_WriteInt(((Ship *)s)->getRace(), fd);
        }
    }

    AEFile_WriteBool(*(bool *)(flags + 0x27), fd);
    AEFile_WriteBool(*(bool *)(ui + 0x35), fd);
    AEFile_WriteBool(*(bool *)(ui + 0x36), fd);
    RSW_writeBoolArray(status, 0x54, fd);

    // Ship-mod table version tag + mod lists for player, wingman and every station ship.
    AEFile_WriteInt(g_RSW_modVersion, fd);
    RSW_writeMods(ship, fd);
    RSW_writeMods(wingShip, fd);
    if (sShips == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(*sShips, fd);
        for (unsigned i = 0; i < *sShips; i++) {
            RSW_writeMods(*(Ship **)(sShips[1] + i * 4), fd);
        }
    }
    for (unsigned i = 0; i < *stack + 1; i++) {
        Station *cur = (i == *stack) ? (Station *)((Status *)status)->getStation()
                                     : *(Station **)(stack[1] + i * 4);
        if (cur == 0) continue;
        unsigned int *cShips = (unsigned int *)((Station *)cur)->getShips();
        if (cShips == 0) { AEFile_WriteInt(0, fd); continue; }
        AEFile_WriteInt(*cShips, fd);
        for (unsigned j = 0; j < *cShips; j++) {
            RSW_writeMods(*(Ship **)(cShips[1] + j * 4), fd);
        }
    }

    // Wanted board, collected bounties, and the remaining one-shot UI/progress flags.
    unsigned int *wanted = *(unsigned int **)status;
    AEFile_WriteInt(*wanted, fd);
    for (unsigned i = 0; i < *wanted; i++) {
        ((RecordHandler *)this)->writeWanted(*(void **)(wanted[1] + i * 4), fd);
    }
    for (unsigned i = 0; i < 4; i++) {
        AEFile_WriteInt(((Status *)status)->getCollectedBounties(i), fd);
    }
    AEFile_WriteBool(*(bool *)(ui + 0x37), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x178), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x28), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x29), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2c), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2a), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2b), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2e), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2f), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x30), fd);
    RSW_writeBoolArray(status, 0x58, fd);
    AEFile_WriteBool(*(bool *)(flags + 0x31), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2d), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x32), fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x118), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x33), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x34), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x35), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x36), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x37), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x38), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x39), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x3a), fd);
}

// ---- recordStoreRead body (game object-graph deserialization) ----
// Continuation of recordStoreRead(): reconstructs the saved object graph into the GameRecord
// `rec` (ship/wingman/station inventories, standings, blueprints, pending products, wingmen,
// agents, bitmaps, flags, mods, wanteds and bounties) from the already-open file `fd`. The
// caller handled open/prefix/hash and performs Close.
extern "C" void *Galaxy_getStationByIndex(int idx);
extern "C" void *Standing_new();                         // operator new + Standing::Standing
extern "C" void  Standing_setStandingsArr(void *st, int *standings);
// operator new + BluePrint(index)
extern "C" void *PendingProduct_make(int a, void *name, int c, int d);
extern "C" void *Array_Item_new();                       // operator new + Array<Item*>::Array
extern "C" void *Array_Ship_new();
extern "C" void *Array_Agent_new();
extern "C" void *Array_Station_new();
extern "C" void *Array_Wanted_new();
extern "C" void *Array_Str_new();
extern "C" void *Array_PP_new();
extern "C" void  ArraySetLength_Item(unsigned n, void *a);
extern "C" void  ArraySetLength_Ship(unsigned n, void *a);
extern "C" void  ArraySetLength_Agent(unsigned n, void *a);
extern "C" void  ArraySetLength_Station(unsigned n, void *a);
extern "C" void  ArraySetLength_Wanted(unsigned n, void *a);
extern "C" void  ArraySetLength_PP(unsigned n, void *a);
extern "C" void *RH_str_make(void *src);                 // operator new + String copy-ctor
// Item-definition tables and the ship-definition table (PC-relative singletons).
__attribute__((visibility("hidden"))) extern void **g_RSR_itemDefs;
__attribute__((visibility("hidden"))) extern void **g_RSR_shipDefs;
__attribute__((visibility("hidden"))) extern void **g_RSR_status_p;
__attribute__((visibility("hidden"))) extern int    g_RSR_modVersion;
extern "C" void Array_int_ctor(void *a);
extern "C" void ArraySetLength_int(unsigned n, void *a);
extern "C" void AEString_assign(void *dst, void *src);

// Read a saved item (index/amount[/price]/unsaleable) into the table slot, or null when -1.
static void *RSR_readItem(unsigned int fd, bool withPrice) {
    int idx = 0;
    AEFile_ReadInt(&idx, fd);
    if (idx == -1) return 0;
    int amount = 0, price = 0; bool uns = false;
    AEFile_ReadInt(&amount, fd);
    if (withPrice) AEFile_ReadInt(&price, fd);
    AEFile_Read_bool(&uns, fd, 0);
    void *def = (*(void ***)g_RSR_itemDefs)[idx];
    ((Item *)(def))->getMaxPrice();
    void *it = ((Item *)(def))->makeItem(amount);
    if (withPrice) ((Item *)(it))->setPrice(price);
    ((Item *)(it))->setUnsaleable(uns);
    return it;
}

void RecordHandler::recordStoreRead_body(void *recv, unsigned int fd) {
    char *rec = (char *)recv;
    RecordHandler *self = this;
    void **shipDefs = *(void ***)g_RSR_shipDefs;

    AEFile_ReadInt(rec + 0x74, fd);
    AEFile_ReadInt(rec + 0x78, fd);
    AEFile_ReadInt(rec + 0x7c, fd);
    AEFile_ReadInt(rec + 0x80, fd);

    // bitmap at rec+0x84
    void *flags84 = RH_op_new(0xc); Array_bool_ctor(flags84);
    *(void **)(rec + 0x84) = flags84;
    int n84 = 0; AEFile_ReadInt(&n84, fd); ArraySetLength_bool(n84, flags84);
    for (int i = 0; i < n84; i++) AEFile_ReadBool((bool *)(*(int *)((char *)flags84 + 4) + i), fd);

    AEFile_ReadInt(rec + 0x88, fd);

    void *flags8c = RH_op_new(0xc); Array_bool_ctor(flags8c);
    *(void **)(rec + 0x8c) = flags8c;
    int n8c = 0; AEFile_ReadInt(&n8c, fd); ArraySetLength_bool(n8c, flags8c);
    for (int i = 0; i < n8c; i++) AEFile_ReadBool((bool *)(*(int *)((char *)flags8c + 4) + i), fd);

    AEFile_ReadInt(rec + 0x90, fd);
    AEFile_ReadLong(rec + 0x98, fd);
    for (int off = 0xa0; off <= 0xc4; off += 4) AEFile_ReadInt(rec + off, fd);

    // int[] at rec+0x60 (length at rec+100/0x64)
    AEFile_ReadInt(rec + 0x64, fd);
    unsigned len60 = *(unsigned *)(rec + 0x64);
    int *buf60 = (int *)RH_op_new_arr(((unsigned long long)len60 * 4 >> 32) ? 0xffffffff : len60 * 4);
    *(void **)(rec + 0x60) = buf60;
    for (unsigned i = 0; i < len60; i++) { AEFile_ReadInt(buf60 + i, fd); }

    // Player ship: definition index, race, equipment, cargo.
    int shipIdx = 0; AEFile_ReadInt(&shipIdx, fd);
    void *pship = ((Ship *)((int)(intptr_t)shipDefs[shipIdx]))->makeShip(-1);
    *(void **)(rec + 0x130) = pship;
    int race = 0; AEFile_ReadInt(&race, fd); ((Ship *)(pship))->setRace(race);

    int eqN = 0; AEFile_ReadInt(&eqN, fd);
    if (eqN > 0) {
        void *items = Array_Item_new(); ArraySetLength_Item(eqN, items);
        for (int i = 0; i < eqN; i++)
            *(void **)(*(int *)((char *)items + 4) + i * 4) = RSR_readItem(fd, false);
        ((Ship *)(pship))->replaceEquipment((Array<Item*>*)items);
    }
    int cgN = 0; AEFile_ReadInt(&cgN, fd);
    if (cgN > 0) {
        void *items = Array_Item_new(); ArraySetLength_Item(cgN, items);
        for (int i = 0; i < cgN; i++)
            *(void **)(*(int *)((char *)items + 4) + i * 4) = RSR_readItem(fd, true);
        ((Ship *)(pship))->replaceCargo((Array<Item*>*)items);
    }

    // Station stack (3 slots + current). For each present station: items, ships, agents, flag.
    void *stationArr = Array_Station_new();
    int stN = 0; AEFile_ReadInt(&stN, fd); ArraySetLength_Station(stN, stationArr);
    unsigned stCount = *(unsigned *)stationArr;
    for (unsigned k = 0; k < stCount + 1; k++) {
        int sIdx = 0; AEFile_ReadInt(&sIdx, fd);
        void *st = 0;
        if (sIdx != -1) {
            st = Galaxy_getStationByIndex(sIdx);
            int iN = 0; AEFile_ReadInt(&iN, fd);
            if (iN > 0) {
                void *items = Array_Item_new(); ArraySetLength_Item(iN, items);
                for (int i = 0; i < iN; i++)
                    *(void **)(*(int *)((char *)items + 4) + i * 4) = RSR_readItem(fd, true);
                ((Station *)(st))->setItems((uint32_t *)items, false);
            }
            int shN = 0; AEFile_ReadInt(&shN, fd);
            if (shN > 0) {
                void *ships = Array_Ship_new(); ArraySetLength_Ship(shN, ships);
                for (int i = 0; i < shN; i++) {
                    int si = 0; AEFile_ReadInt(&si, fd);
                    void *sh = ((Ship *)((int)(intptr_t)shipDefs[si]))->makeShip(-1);
                    *(void **)(*(int *)((char *)ships + 4) + i * 4) = sh;
                    int r = 0; AEFile_ReadInt(&r, fd); ((Ship *)(sh))->setRace(r);
                }
                ((Station *)(st))->setShipsArr(ships);
            }
            int agN = 0; AEFile_ReadInt(&agN, fd);
            if (agN > 0) {
                void *agents = Array_Agent_new(); ArraySetLength_Agent(agN, agents);
                for (int i = 0; i < agN; i++)
                    *(void **)(*(int *)((char *)agents + 4) + i * 4) = self->readAgent(fd);
                ((Station *)(st))->setAgents(agents);
            }
            bool atk = false; AEFile_Read_bool(&atk, fd, 0);
            ((Station *)(st))->setAttackedFriends(atk);
        }
        if (k == *(unsigned *)stationArr) *(void **)(rec + 0x138) = st;
        else *(void **)(*(int *)((char *)stationArr + 4) + k * 4) = st;
    }
    *(void **)(rec + 0x5c) = stationArr;

    // Standings (count then values).
    int sdN = 0; AEFile_ReadInt(&sdN, fd);
    int *standings = (int *)RH_op_new_arr(((unsigned long long)sdN * 4 >> 32) ? 0xffffffff : sdN * 4);
    for (int i = 0; i < sdN; i++) AEFile_ReadInt(standings + i, fd);
    void *standing = Standing_new();
    *(void **)(rec + 0x13c) = standing;
    Standing_setStandingsArr(standing, standings);

    // Blueprints.
    void *bpArr = Array_Item_new();   // generic Array<T*> container reuse
    int bpN = 0; AEFile_ReadInt(&bpN, fd); ArraySetLength_Item(bpN, bpArr);
    void *statusG = *g_RSR_status_p;
    for (unsigned i = 0; i < *(unsigned *)bpArr; i++) {
        unsigned int *liveBps = (unsigned int *)((Status *)statusG)->getBluePrints();
        int liveIdx = ((BluePrint *)(*(void **)(liveBps[1] + i * 4)))->getIndexOf();
        int *bp = (int *)BluePrint::make(liveIdx);
        *(void **)(*(int *)((char *)bpArr + 4) + i * 4) = bp;
        unsigned int *ingredients = (unsigned int *)bp[0];
        int byteoff = 0;
        for (unsigned j = 0; j < *ingredients; j++) { AEFile_ReadInt((void *)(intptr_t)(ingredients[1] + byteoff), fd); byteoff += 4; }
        AEFile_ReadInt(bp + 1, fd);
        AEFile_ReadByte((bool *)(bp + 2), fd);
        AEFile_ReadInt(bp + 3, fd);
        AEFile_ReadInt(bp + 4, fd);
        char tmp[12]; AEString_default_ctor(tmp); AEFile_ReadString(tmp, fd, 1);
        AEString_assign((void *)(bp + 5), (void *)tmp); AEString_dtor(tmp);
    }
    *(void **)(rec + 0x140) = bpArr;

    // Pending products (-1 / 0 == none).
    int ppN = 0; AEFile_ReadInt(&ppN, fd);
    if (ppN < 1) { *(void **)(rec + 0x144) = 0; }
    else {
        void *ppArr = Array_PP_new(); ArraySetLength_PP(ppN, ppArr);
        for (unsigned i = 0; i < *(unsigned *)ppArr; i++) {
            int a = 0, c = 0, d = 0; AEFile_ReadInt(&a, fd); AEFile_ReadInt(&c, fd); AEFile_ReadInt(&d, fd);
            char nm[12]; AEString_default_ctor(nm); AEFile_ReadString(nm, fd, 1);
            void *nameCopy = RH_str_make(nm);
            void *pp = PendingProduct_make(a, nameCopy, d, c);
            *(void **)(*(int *)((char *)ppArr + 4) + i * 4) = pp;
            AEString_dtor(nm);
        }
        *(void **)(rec + 0x144) = ppArr;
    }

    // Wingmen string list (+ trailing scalar/int[] fields) or none.
    int wmN = 0; AEFile_ReadInt(&wmN, fd);
    if (wmN < 1) { *(void **)(rec + 0x14c) = 0; }
    else {
        void *strArr = Array_Str_new(); ArraySetLength_Item(wmN, strArr);
        for (int i = 0; i < wmN; i++) {
            char nm[12]; AEString_default_ctor(nm); AEFile_ReadString(nm, fd, 1);
            *(void **)(*(int *)((char *)strArr + 4) + i * 4) = RH_str_make(nm);
            AEString_dtor(nm);
        }
        *(void **)(rec + 0x14c) = strArr;
        AEFile_ReadInt(rec + 0x150, fd);
        AEFile_ReadInt(rec + 0x154, fd);
        int cnt = 0; AEFile_ReadInt(&cnt, fd);
        int *buf158 = (int *)RH_op_new_arr(((unsigned long long)cnt * 4 >> 32) ? 0xffffffff : cnt * 4);
        *(void **)(rec + 0x158) = buf158;
        for (int i = 0; i < cnt; i++) AEFile_ReadInt(buf158 + i, fd);
    }

    AEFile_ReadInt(rec + 0x15c, fd);

    // bool[] rec+0x160
    int b160 = 0; AEFile_ReadInt(&b160, fd);
    void *arr160 = RH_op_new(0xc); Array_bool_ctor(arr160); *(void **)(rec + 0x160) = arr160;
    ArraySetLength_bool(b160, arr160);
    for (unsigned i = 0; i < **(unsigned **)(rec + 0x160); i++)
        AEFile_ReadBool((bool *)((*(unsigned **)(rec + 0x160))[1] + i), fd);

    // int[] rec+0x168, 0x164, 0x170, 0x16c
    const int intArrOffs[4] = {0x168, 0x164, 0x170, 0x16c};
    for (int a = 0; a < 4; a++) {
        int cnt = 0; AEFile_ReadInt(&cnt, fd);
        void *arr = RH_op_new(0xc); Array_int_ctor(arr); *(void **)(rec + intArrOffs[a]) = arr;
        ArraySetLength_int(cnt, arr);
        int byteoff = 0;
        for (unsigned i = 0; i < **(unsigned **)(rec + intArrOffs[a]); i++) {
            AEFile_ReadInt((void *)(intptr_t)((*(int **)(rec + intArrOffs[a]))[1] + byteoff), fd); byteoff += 4;
        }
    }

    // bool[] rec+0x174
    int b174 = 0; AEFile_ReadInt(&b174, fd);
    void *arr174 = RH_op_new(0xc); Array_bool_ctor(arr174); *(void **)(rec + 0x174) = arr174;
    ArraySetLength_bool(b174, arr174);
    for (unsigned i = 0; i < **(unsigned **)(rec + 0x174); i++)
        AEFile_ReadBool((bool *)((*(unsigned **)(rec + 0x174))[1] + i), fd);

    // Agents.
    int agN = 0; AEFile_ReadInt(&agN, fd);
    void *agents = Array_Agent_new(); *(void **)(rec + 0x148) = agents; ArraySetLength_Agent(agN, agents);
    for (unsigned i = 0; i < **(unsigned **)(rec + 0x148); i++)
        *(void **)(*(int *)(*(int *)(rec + 0x148) + 4) + i * 4) = self->readAgent(fd);

    // Single-byte flag block rec+0xe4 .. 0x100.
    for (int off = 0xe4; off <= 0x100; off++) AEFile_ReadByte(rec + off, fd);
    AEFile_ReadFloat(rec + 0x11c, fd);
    AEFile_ReadLong(rec + 0xc8, fd);
    AEFile_ReadByte(rec + 0x101, fd);
    AEFile_ReadByte(rec + 0x102, fd);

    // Optional wingman ship.
    int hasWing = 0; AEFile_ReadInt(&hasWing, fd);
    if (hasWing > 0) {
        int wi = 0; AEFile_ReadInt(&wi, fd);
        void *wship = ((Ship *)((int)(intptr_t)shipDefs[wi]))->makeShip(-1);
        *(void **)(rec + 0x134) = wship;
        int r = 0; AEFile_ReadInt(&r, fd); ((Ship *)(wship))->setRace(r);
        int eN = 0; AEFile_ReadInt(&eN, fd);
        if (eN > 0) {
            void *items = Array_Item_new(); ArraySetLength_Item(eN, items);
            for (int i = 0; i < eN; i++)
                *(void **)(*(int *)((char *)items + 4) + i * 4) = RSR_readItem(fd, false);
            ((Ship *)(wship))->replaceEquipment((Array<Item*>*)items);
        }
        int cN = 0; AEFile_ReadInt(&cN, fd);
        if (cN > 0) {
            void *items = Array_Item_new(); ArraySetLength_Item(cN, items);
            for (int i = 0; i < cN; i++)
                *(void **)(*(int *)((char *)items + 4) + i * 4) = RSR_readItem(fd, true);
            ((Ship *)(wship))->replaceCargo((Array<Item*>*)items);
        }
    }

    // int[] rec+0x70.
    void *arr70 = RH_op_new(0xc); Array_int_ctor(arr70); *(void **)(rec + 0x70) = arr70;
    int n70 = 0; AEFile_ReadInt(&n70, fd); ArraySetLength_int(n70, arr70);
    { int byteoff = 0; for (int i = 0; i < n70; i++) { AEFile_ReadInt((void *)(intptr_t)(*(int *)((char *)arr70 + 4) + byteoff), fd); byteoff += 4; } }

    AEFile_ReadInt(rec + 0xd0, fd);
    AEFile_ReadByte(rec + 0xd4, fd);
    AEFile_ReadInt(rec + 0xd8, fd);
    AEFile_ReadByte(rec + 0xdc, fd);

    // Station shop items (rec+0x180) and ships (rec+0x184).
    int siN = 0; AEFile_ReadInt(&siN, fd);
    if (siN > 0) {
        void *items = Array_Item_new(); ArraySetLength_Item(siN, items);
        for (int i = 0; i < siN; i++)
            *(void **)(*(int *)((char *)items + 4) + i * 4) = RSR_readItem(fd, true);
        *(void **)(rec + 0x180) = items;
    }
    int ssN = 0; AEFile_ReadInt(&ssN, fd);
    if (ssN > 0) {
        void *ships = Array_Ship_new(); ArraySetLength_Ship(ssN, ships);
        for (int i = 0; i < ssN; i++) {
            int si = 0; AEFile_ReadInt(&si, fd);
            void *sh = ((Ship *)((int)(intptr_t)shipDefs[si]))->makeShip(-1);
            *(void **)(*(int *)((char *)ships + 4) + i * 4) = sh;
            int r = 0; AEFile_ReadInt(&r, fd); ((Ship *)(sh))->setRace(r);
        }
        *(void **)(rec + 0x184) = ships;
    }

    AEFile_ReadByte(rec + 0x103, fd);
    AEFile_ReadByte(rec + 0x115, fd);
    AEFile_ReadByte(rec + 0x116, fd);

    // bool[] rec+0x178
    int b178 = 0; AEFile_ReadInt(&b178, fd);
    void *arr178 = RH_op_new(0xc); Array_bool_ctor(arr178); *(void **)(rec + 0x178) = arr178;
    ArraySetLength_bool(b178, arr178);
    for (unsigned i = 0; i < **(unsigned **)(rec + 0x178); i++)
        AEFile_ReadBool((bool *)((*(unsigned **)(rec + 0x178))[1] + i), fd);

    // Version-gated tail: ship-mod tables, wanteds, bounties and the final flag block.
    AEFile_ReadInt(rec + 0x1bc, fd);
    if (*(int *)(rec + 0x1bc) != g_RSR_modVersion) return;

    int m0 = 0; AEFile_ReadInt(&m0, fd);
    if (m0 > 0) { void *a = RH_op_new(0xc); Array_int_ctor(a); ArraySetLength_int(m0, a);
        int bo = 0; for (int i = 0; i < m0; i++) { AEFile_ReadInt((void *)(intptr_t)(*(int *)((char *)a + 4) + bo), fd); bo += 4; }
        ((Ship *)(*(void **)(rec + 0x130)))->setMods((Array<int>*)a); }
    int m1 = 0; AEFile_ReadInt(&m1, fd);
    if (m1 > 0) { void *a = RH_op_new(0xc); Array_int_ctor(a); ArraySetLength_int(m1, a);
        int bo = 0; for (int i = 0; i < m1; i++) { AEFile_ReadInt((void *)(intptr_t)(*(int *)((char *)a + 4) + bo), fd); bo += 4; }
        ((Ship *)(*(void **)(rec + 0x134)))->setMods((Array<int>*)a); }

    int stShipGroups = 0; AEFile_ReadInt(&stShipGroups, fd);
    for (int g = 0; g < stShipGroups; g++) {
        int mc = 0; AEFile_ReadInt(&mc, fd);
        if (mc > 0) { void *a = RH_op_new(0xc); Array_int_ctor(a); ArraySetLength_int(mc, a);
            int bo = 0; for (int i = 0; i < mc; i++) { AEFile_ReadInt((void *)(intptr_t)(*(int *)((char *)a + 4) + bo), fd); bo += 4; }
            ((Ship *)(*(void **)(*(int *)(*(int *)(rec + 0x184) + 4) + g * 4)))->setMods((Array<int>*)a); }
    }

    // Per-station ship mods (applied incrementally via addMod).
    void *stArr = *(void **)(rec + 0x5c);
    for (unsigned k = 0; k < *(unsigned *)stArr + 1; k++) {
        void *cur = (k == *(unsigned *)stArr) ? *(void **)(rec + 0x138)
                                              : *(void **)(*(int *)((char *)stArr + 4) + k * 4);
        if (cur == 0) continue;
        int groups = 0; AEFile_ReadInt(&groups, fd);
        for (int i = 0; i < groups; i++) {
            int mc = 0; AEFile_ReadInt(&mc, fd);
            for (int j = 0; j < mc; j++) {
                int mod = 0; AEFile_ReadInt(&mod, fd);
                unsigned int *cShips = (unsigned int *)((Station *)cur)->getShips();
                ((Ship *)(*(void **)(cShips[1] + i * 4)))->addMod(mod);
            }
        }
    }

    // Wanteds.
    int wN = 0; AEFile_ReadInt(&wN, fd);
    if (wN > 0) {
        void *wArr = Array_Wanted_new(); *(void **)(rec + 0x1b4) = wArr; ArraySetLength_Wanted(wN, wArr);
        for (int i = 0; i < wN; i++)
            *(void **)(*(int *)(*(int *)(rec + 0x1b4) + 4) + i * 4) = self->readWanted(fd);
    }

    // Collected bounties + the remaining one-shot UI/progress flags.
    for (int i = 0; i < 4; i++) AEFile_ReadInt(rec + 0x1a4 + i * 4, fd);
    AEFile_ReadByte(rec + 0x117, fd);
    AEFile_ReadInt(rec + 0x1b8, fd);
    AEFile_ReadByte(rec + 0x104, fd);
    AEFile_ReadByte(rec + 0x105, fd);
    AEFile_ReadByte(rec + 0x108, fd);
    AEFile_ReadByte(rec + 0x106, fd);
    AEFile_ReadByte(rec + 0x107, fd);
    AEFile_ReadByte(rec + 0x10a, fd);
    AEFile_ReadByte(rec + 0x10b, fd);
    AEFile_ReadByte(rec + 0x10c, fd);

    int bX = 0; AEFile_ReadInt(&bX, fd);
    void *arr17c = RH_op_new(0xc); Array_bool_ctor(arr17c); *(void **)(rec + 0x17c) = arr17c;
    ArraySetLength_bool(bX, arr17c);
    for (unsigned i = 0; i < **(unsigned **)(rec + 0x17c); i++)
        AEFile_ReadBool((bool *)((*(unsigned **)(rec + 0x17c))[1] + i), fd);

    AEFile_ReadByte(rec + 0x119, fd);
    AEFile_ReadByte(rec + 0x109, fd);
    AEFile_ReadByte(rec + 0x11a, fd);
    AEFile_ReadInt(rec + 0xe0, fd);
    for (int off = 0x10d; off <= 0x114; off++) AEFile_ReadByte(rec + off, fd);
}

// ---- readOptionsFileAsByteArray_cdd4c.cpp ----
// RecordHandler::readOptionsFileAsByteArray(signed char**)
int RecordHandler::readOptionsFileAsByteArray(signed char **out) {
    RecordHandler *self = this;
    char tmp[12];
    unsigned int fd;
    int sz;

    String_copy_ctor(tmp, (char *)self + 0x8, false);
    if (AEFile::FileExist(*(String *)(tmp)) != 0) {
        AEFile::OpenRead(*(String *)tmp, &fd);
        sz = AEFile::GetFileSize(fd);
        signed char *buf = RH_op_new_arr(sz | (sz >> 31));
        *out = buf;
        AEFile::Read(sz, buf, fd);
        AEFile::Close(fd);
    } else {
        sz = -1;
    }
    ((String *)(tmp))->dtor();
    return sz;
}

// ---- recordStoreRead_cbd34.cpp ----
// Deep GameRecord field read (ship, stations, standings, blueprints, pending products, agents,
// ships, wanteds, flags). A very long straight-line reader delegated to a helper so this
// translation preserves the path/hash/open/prefix/close flow while staying tractable.

__attribute__((visibility("hidden"))) extern int *g_RSR_guard;   // DAT_000dc094 -> guard holder

// RecordHandler::recordStoreRead(int slot)
void * RecordHandler::recordStoreRead(int slot) {
    RecordHandler *self = this;
    int *guardP = g_RSR_guard;
    volatile int saved = *guardP;

    char *rec = 0;
    char num[12], path[12];
    AEString_int_ctor(num, slot);
    AEString_concat(path, (char *)self + 0x14);

    if (AEFile::FileExist(*(String *)(path)) != 0) {
        unsigned int fd;
        AEFile::OpenRead(*(String *)path, &fd);
        int valid = RecordHandler_checkHash(fd);
        AEFile::Close(fd);
        if (valid != 0) {
            AEFile::OpenRead(*(String *)path, &fd);
            rec = (char *)RH_op_new(0x1c8);
            GameRecord_ctor(rec);

            // Visited-systems bitmap at rec+4 (count then booleans).
            char *visited = rec + 4;
            *(int *)visited = 0;
            AEFile_ReadInt(visited, fd);
            for (unsigned i = 0; i < *(unsigned *)visited; i++) {
                AEFile_ReadBool((bool *)(*(int *)rec + i), fd);
            }

            AEFile_ReadInt(rec + 8, fd);
            AEFile_ReadInt(rec + 0xc, fd);
            AEFile_ReadLong(rec + 0x10, fd);
            AEFile_ReadInt(rec + 0x18, fd);
            AEFile_ReadInt(rec + 0x1c, fd);
            AEFile_ReadInt(rec + 0x20, fd);
            AEFile_ReadInt(rec + 0x24, fd);
            AEFile_ReadInt(rec + 0x28, fd);
            AEFile_ReadInt(rec + 0x3c, fd);
            AEFile_ReadInt(rec + 0x40, fd);
            *(void **)(rec + 0x54) = ((RecordHandler *)(self))->readMission(fd);
            *(void **)(rec + 0x58) = ((RecordHandler *)(self))->readMission(fd);
            AEFile_ReadInt(rec + 0x30, fd);
            AEFile_ReadInt(rec + 0x34, fd);
            AEFile_ReadInt(rec + 0x38, fd);
            AEFile_ReadInt(rec + 0x2c, fd);
            AEFile_ReadInt(rec + 0x44, fd);
            AEFile_ReadInt(rec + 0x48, fd);
            AEFile_ReadInt(rec + 0x4c, fd);
            AEFile_ReadInt(rec + 0x50, fd);

            // Two boolean arrays (mission flags / equipment flags).
            void *flags0 = RH_op_new(0xc);
            Array_bool_ctor(flags0);
            *(void **)(rec + 0x68) = flags0;
            unsigned n0 = 0;
            AEFile_ReadInt(&n0, fd);
            ArraySetLength_bool(n0, flags0);
            for (int i = 0; i < (int)n0; i++) {
                AEFile_ReadBool((bool *)(*(int *)((char *)flags0 + 4) + i), fd);
            }
            void *flags1 = RH_op_new(0xc);
            Array_bool_ctor(flags1);
            *(void **)(rec + 0x6c) = flags1;
            unsigned n1 = 0;
            AEFile_ReadInt(&n1, fd);
            ArraySetLength_bool(n1, flags1);
            for (int i = 0; i < (int)n1; i++) {
                AEFile_ReadBool((bool *)(*(int *)((char *)flags1 + 4) + i), fd);
            }

            // Remaining object-graph read.
            ((RecordHandler *)(self))->recordStoreRead_body(rec, fd);

            AEFile::Close(fd);
        }
    }

    return rec;
    return rec;
}

// ---- checkHash_cd7b0.cpp ----
__attribute__((visibility("hidden"))) extern int *g_CH_guard;          // DAT_000dd8b8 -> guard holder
__attribute__((visibility("hidden"))) extern const unsigned char RH_ch_salt[]; // DAT_000dd8c0 (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ch_key;        // DAT_000dd8c4 (16-byte key)

// RecordHandler::checkHash(unsigned int fd) -> bool (1=valid, 0=invalid).
int RecordHandler_checkHash(unsigned int fd)
{
    int *guardP = g_CH_guard;
    volatile int saved = *guardP;
    int result = 0;

    int sz = AEFile::GetFileSize(fd);
    if (-1 < sz) {
        unsigned char *buf = (unsigned char *)RH_op_new_arr((unsigned)sz);
        AEFile::Read((unsigned)sz, (signed char *)buf, fd);
        void *arr = RH_op_new(0xc);
        Array_UC_ctor(arr);
        ArrayAdd_UC(buf, (unsigned)sz, arr);
        RH_op_delete_arr(buf);

        unsigned len = *(unsigned *)arr;
        if (0x21 < len) {
            unsigned char *md = (unsigned char *)RH_op_new_arr(0x20);
            void *c = RH_op_new(0x70);
            SHA256_Init(c);
            unsigned char *data = *(unsigned char **)((char *)arr + 4);
            void (*update)(void *, const void *, int) = SHA256_Update;
            update(c, data, (int)len - 0x20);
            update(c, RH_ch_salt, 0x19);
            update(c, *RH_ch_key, 0x10);
            SHA256_Final(md, c);

            unsigned i = 0;
            // compare 32 computed bytes against the trailing 32 bytes of the record
            while (i < 0x20) {
                unsigned char want = data[(int)len + i - 0x20];
                if (md[i] != want) break;
                i++;
            }
            result = (i > 0x1f) ? 1 : 0;
            RH_op_delete_arr(md);
            RH_op_delete(c);
        }
    }

    return result;
}

// ---- addHashToOptions_cdafc.cpp ----
// SHA salt blob (25 bytes) and the second 16-byte key, both via hidden pc-rel relocs.
__attribute__((visibility("hidden"))) extern const unsigned char RH_aho_salt[];
__attribute__((visibility("hidden"))) extern unsigned char **RH_aho_key;


// RecordHandler::addHashToOptions()
void RecordHandler::addHashToOptions() {
    RecordHandler *self = this;
    signed char *data = 0;
    int len = ((RecordHandler *)(self))->readOptionsFileAsByteArray(&data);
    if (len < 0)
        return;

    unsigned char *md = (unsigned char *)RH_op_new_arr(0x20);
    void *c = RH_op_new(0x70);
    SHA256_Init(c);
    void (*update)(void *, const void *, int) = SHA256_Update;
    update(c, data, len);
    update(c, RH_aho_salt, 0x19);
    update(c, *RH_aho_key, 0x10);
    SHA256_Final(md, c);

    signed char *out = (signed char *)RH_op_new_arr(len + 0x20);
    RH_memcpy(out, data, len);
    unsigned long long *dst = (unsigned long long *)(out + len);
    unsigned long long *src = (unsigned long long *)md;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    ((RecordHandler *)(self))->writeByteArrayAsOptionsFile(out, len + 0x20);

    void (*del)(void *) = (void (*)(void *))RH_op_delete_arr;
    del(data);
    data = 0;
    del(out);
    del(md);
    RH_op_delete(c);
}
