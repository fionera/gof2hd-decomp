#include "gof2/RecordHandler.h"
#include "gof2/Agent.h"


extern "C" void RecordHandler_readRecordTail(int param);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void Array_GR_ctor(void *a);
extern "C" void ArraySetLength_GR(unsigned int n, void *a);
extern "C" void *RecordHandler_recordStoreRead(RecordHandler *self, int idx);
extern "C" void *RecordHandler_recordStoreReadPreview(RecordHandler *self, int idx);
extern "C" int AEFile_GetDeviceFreeSpace();
extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);
extern "C" void Array_SC_ctor(void *a);
extern "C" void ArraySetLength_SC(unsigned int n, void *a);
extern "C" signed char *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void ArrayReleaseArrays_SC(void *a);
extern "C" int RecordHandler_readRecordAsByteArray(RecordHandler *self, signed char **out, int slot, bool fromBackup);
extern "C" void RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, int n, int slot, bool toBackup);
extern "C" void RecordHandler_addHash(RecordHandler *self, int slot);
extern "C" void AEString_int_ctor(void *dst, int v);
extern "C" void AEString_concat(void *dst, void *a);
extern "C" void AEString_dtor(void *s);
extern "C" void AEFile_FileDelete(void *path);
extern "C" void RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, unsigned n, int slot, bool toBackup);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete(void *p);
extern "C" void SHA256_Init(void *c);
extern "C" void SHA256_Update(void *c, const void *data, int n);
extern "C" void SHA256_Final(unsigned char *md, void *c);
extern "C" void *RH_memcpy(void *dst, const void *src, unsigned int n);
extern "C" void String_int_ctor(void *dst, int v);
extern "C" void *String_concat(void *dst, void *a, void *b);
extern "C" void String_dtor(void *s);
extern "C" int AEFile_FileExist(void *s);
extern "C" void AEFile_OpenRead(void *s, unsigned int *fd);
extern "C" int AEFile_GetFileSize(unsigned int fd);
extern "C" void AEFile_Read(int n, signed char *buf, unsigned int fd);
extern "C" void AEFile_Close(unsigned int fd);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" void AEString_default_ctor(void *s);
extern "C" void AEString_copy_ctor(void *dst, void *src, int copy);
extern "C" void Wanted_setImageParts(void *w, int *parts);
extern "C" void Wanted_setActive(void *w, bool v);
extern "C" void Wanted_setTerminated(void *w, bool v);
extern "C" void Wanted_setCurrentLocation(void *w, int v);
extern "C" void Wanted_setTravelsTo(void *w, int v);
extern "C" void Wanted_setLastSeen(void *w, int v);
extern "C" void RecordHandler_csd_tail(void *p, int one, int count, void *arr2);
extern "C" void *GR_op_new(unsigned int sz);
extern "C" void GameRecord_ctor(void *gr);
extern "C" void AEFile_Read_i64(void *dst, unsigned int fd);
extern "C" void AEFile_Read_i32(void *dst, unsigned int fd);
extern "C" void AEFile_Read_bool(void *dst, unsigned int fd, int b);
extern "C" void AEFile_Read_f32(void *dst, unsigned int fd);
extern "C" void String_default_ctor(void *s);
extern "C" void String_cstr_ctor(void *s, const char *cs, bool b);
extern "C" void String_assign(void *dst, void *src);
extern "C" void *String_copy_ctor(void *dst, void *src, bool b);
extern "C" void AEFile_FileDelete(void *s);
extern "C" void AEFile_OpenWrite(void *s, unsigned int *fd);
extern "C" void AEFile_Write(int n, signed char *buf, unsigned int fd);
extern "C" void AEFile_Write_i64(long long v, unsigned int fd);
extern "C" void AEFile_Write_i32(int v, unsigned int fd);
extern "C" void AEFile_Write_str(void *s, unsigned int fd, int b);
extern "C" void AEFile_Write_f32(int v, unsigned int fd);
extern "C" long long Status_getPlayingTime(Status *s);
extern "C" int Status_getCredits(Status *s);
extern "C" Station *Status_getStation(Status *s);
extern "C" void *Station_getName(void *dst, Station *st);
extern "C" SolarSystem *Status_getSystem(Status *s);
extern "C" void *SolarSystem_getName(void *dst, SolarSystem *ss);
extern "C" int Status_getCurrentCampaignMission(Status *s);
extern "C" int Status_getLevel(Status *s);
extern "C" Ship *Status_getShip(Status *s);
extern "C" int Ship_getIndex(Ship *sh);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEString_cstr_ctor(void *dst, const char *s, int copy);
extern "C" int Agent_getCosts(void *a);
extern "C" int Agent_getSellSystemIndex(void *a);
extern "C" int Agent_getSellBlueprintIndex(void *a);
extern "C" int Agent_getEvent(void *a);
extern "C" int Agent_getIndex(void *a);
extern "C" int Agent_getOffer(void *a);
extern "C" int Agent_getRace(void *a);
extern "C" int Agent_getSellItemIndex(void *a);
extern "C" int Agent_getSellItemPrice(void *a);
extern "C" int Agent_getSellItemQuantity(void *a);
extern "C" int Agent_getStation(void *a);
extern "C" int Agent_getSystem(void *a);
extern "C" int Agent_getWingmanFriendsCount(void *a);
extern "C" int Agent_isMale(void *a);
extern "C" int Agent_hasReward(void *a);
extern "C" int Agent_hasAcceptedOffer(void *a);
extern "C" int *Agent_getImageParts(void *a);
extern "C" int Agent_getSellModIndex(void *a);
extern "C" void Agent_getMissionString(void *out, void *a);
extern "C" void Agent_getName(void *out, void *a);
extern "C" void Agent_getStationName(void *out, void *a);
extern "C" void Agent_getSystemName(void *out, void *a);
extern "C" void *Agent_getMission(void *a);
extern "C" void RecordHandler_writeMission(RecordHandler *self, void *m, unsigned int fd);
extern "C" int Mission_getType(void *m);
extern "C" int Mission_isEmpty(void *m);
extern "C" void Mission_getClientName(void *out, void *m);
extern "C" void Mission_getTargetName(void *out, void *m);
extern "C" void Mission_getTargetStationName(void *out, void *m);
extern "C" void Mission_getTargetSystemName(void *out, void *m);
extern "C" int Mission_isCampaignMission(void *m);
extern "C" int *Mission_getClientImage(void *m);
extern "C" int Mission_getClientRace(void *m);
extern "C" int Mission_getCosts(void *m);
extern "C" int Mission_getBonus(void *m);
extern "C" int Mission_getReward(void *m);
extern "C" int Mission_getTargetStation(void *m);
extern "C" int Mission_getDifficulty(void *m);
extern "C" int Mission_getProductionGoodIndex(void *m);
extern "C" int Mission_getProductionGoodAmount(void *m);
extern "C" int Mission_getStatusValue(void *m);
extern "C" int Mission_isVisible(void *m);
extern "C" void *Mission_getAgent(void *m);
extern "C" void RecordHandler_writeAgent(RecordHandler *self, void *agent, unsigned int fd);
extern "C" int AEFile_FileExist(void *path);
extern "C" unsigned int AEFile_OpenRead(void *path, unsigned int *fd);
extern "C" void AEFile_ReadByte(void *out, unsigned int fd);
extern "C" void AEFile_ReadFloat(void *out, unsigned int fd);
extern "C" void AEFile_ReadShort(void *out, unsigned int fd);
extern "C" int RecordHandler_checkHash(unsigned int fd);
extern "C" void GameText_setLanguage(short obj, int lang);
extern "C" void Globals_loadFont(int kind);
extern "C" void FModSound_setAudioLanguage(void *s, int lang);
extern "C" int FModSound_enableCategory(void *s, int on);
extern "C" int FModSound_setVolume(void *s, int v);
extern "C" void AEFile_OpenRead(void *path, unsigned int *fd);
extern "C" void *RecordHandler_readAgent(RecordHandler *self, unsigned int fd);
extern "C" void Mission_ctorEmpty(void *self, int type, int reward, int targetStation);
extern "C" void Mission_setCosts(void *m, int v);
extern "C" void Mission_setBonus(void *m, int v);
extern "C" void Mission_setProductionGoods(void *m, int idx, int amt);
extern "C" void Mission_setStatusValue(void *m, int v);
extern "C" void Mission_setVisible(void *m, bool v);
extern "C" void Mission_setAgent(void *m, void *agent);
extern "C" void Mission_setTargetName(void *m, void *name);
extern "C" void Agent_setMission(void *agent, void *m);
extern "C" void AEFile_OpenWrite(void *path, unsigned int *fd);
extern "C" void AEFile_WriteByte(int v, unsigned int fd);
extern "C" void AEFile_WriteFloat(int v, unsigned int fd);
extern "C" void AEFile_WriteShort(int v, unsigned int fd);
extern "C" int GameText_getLanguage();
extern "C" void RecordHandler_addHashToOptions(RecordHandler *self);
extern "C" void *RecordHandler_readMission(RecordHandler *self, unsigned int fd);
extern "C" void Agent_setCosts(void *a, int v);
extern "C" void Agent_setEvent(void *a, int v);
extern "C" void Agent_setOffer(void *a, int v);
extern "C" void Agent_setSellItemData(void *a, int idx, int qty, int price);
extern "C" void Agent_setWingmanFriendNames(void *a, void *arr);
extern "C" void Agent_giveRewardAtNextChat(void *a, bool v);
extern "C" void Agent_setOfferAccepted(void *a, bool v);
extern "C" void Agent_setImageParts(void *a, int *parts);
extern "C" void Agent_setMissionString(void *a, void *s);
extern "C" void Agent_setStationName(void *a, void *s);
extern "C" void Agent_setSystemName(void *a, void *s);
extern "C" void Agent_setMission(void *a, void *m);
extern "C" void ArrayStr_ctor(void *a);
extern "C" void ArraySetLength_Str(unsigned n, void *a);
extern "C" int Wanted_isActive(void *w);
extern "C" int Wanted_isTerminated(void *w);
extern "C" int Wanted_getCurrentLocation(void *w);
extern "C" int Wanted_getTravelsTo(void *w);
extern "C" int Wanted_getLastSeen(void *w);
extern "C" void Wanted_getName(void *out, void *w);
extern "C" int Wanted_getIndex(void *w);
extern "C" int Wanted_getBoard(void *w);
extern "C" int Wanted_getRace(void *w);
extern "C" int Wanted_isMale(void *w);
extern "C" int Wanted_getShip(void *w);
extern "C" int Wanted_getWeapon(void *w);
extern "C" int Wanted_getHitpoints(void *w);
extern "C" int Wanted_getLoot(void *w);
extern "C" int Wanted_getLootAmount(void *w);
extern "C" int Wanted_getReward(void *w);
extern "C" int Wanted_getRequiredBounties(void *w);
extern "C" int Wanted_getRequiredMission(void *w);
extern "C" int Wanted_getNumWingmen(void *w);
extern "C" int *Wanted_getImageParts(void *w);
extern "C" void *Station_getName(void *dst, void *st);
extern "C" int Ship_getIndex(void *sh);
extern "C" void AEFile_WriteLong(long long v, unsigned int fd);
extern "C" int Galaxy_getVisited(void *g);
extern "C" int Status_getCredits();
extern "C" int Status_getRating(void *st);
extern "C" long long Status_getPlayingTime();
extern "C" int Status_getKills(void *st);
extern "C" int Status_getMissionCount(void *st);
extern "C" int Status_getLevel();
extern "C" int Status_getLastXP(void *st);
extern "C" int Status_getGoodsProduced(void *st);
extern "C" int Status_getStationsVisited(void *st);
extern "C" int Status_getCurrentCampaignMission();
extern "C" void *Status_getFreelanceMission();
extern "C" void *Status_getCampaignMission();
extern "C" int Status_getJumpgateUsed(void *st);
extern "C" int Status_getCapturedCrates(void *st);
extern "C" int Status_getBoughtEquipment(void *st);
extern "C" int Status_getPirateKills(void *st);
extern "C" void RecordHandler_recordStoreWrite_body(RecordHandler *self, unsigned int fd);
extern "C" void AEFile_ReadLong(void *out, unsigned int fd);
extern "C" void GameRecord_ctor(void *self);
extern "C" void Array_bool_ctor(void *a);
extern "C" void ArraySetLength_bool(unsigned n, void *a);
extern "C" void RecordHandler_recordStoreRead_body(RecordHandler *self, void *rec, unsigned int fd);
extern "C" void AEFile_Read(unsigned int n, unsigned char *buf, unsigned int fd);
extern "C" void Array_UC_ctor(void *a);
extern "C" void ArrayAdd_UC(unsigned char *src, unsigned int n, void *a);
extern "C" int RecordHandler_readOptionsFileAsByteArray(RecordHandler *self, signed char **out);
extern "C" void RecordHandler_writeByteArrayAsOptionsFile(RecordHandler *self, signed char *buf, int n);

// ---- readRecord_cd25c.cpp ----
// Tail-called veneer at 0x1ac058 (recordStoreRead dispatch).

// RecordHandler::readRecord(int)
extern "C" void RecordHandler_readRecord(int param)
{
    return RecordHandler_readRecordTail(param);
}

// ---- readAllRecords_cbcdc.cpp ----
// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;


// RecordHandler::readAllRecords() -> Array<GameRecord*>*
extern "C" void *RecordHandler_readAllRecords(RecordHandler *self)
{
    void *arr = RH_op_new(0xc);
    Array_GR_ctor(arr);
    int *cnt = g_RH_recordCount;
    ArraySetLength_GR(*cnt, arr);
    for (int i = 0; i < *cnt; i++) {
        void *r = RecordHandler_recordStoreRead(self, i);
        ((void **)I(arr, 4))[i] = r;
    }
    return arr;
}

// ---- readAllPreviewRecords_cbb48.cpp ----
// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;


// RecordHandler::readAllPreviewRecords() -> Array<GameRecord*>*
extern "C" void *RecordHandler_readAllPreviewRecords(RecordHandler *self)
{
    void *arr = RH_op_new(0xc);
    Array_GR_ctor(arr);
    int *cnt = g_RH_recordCount;
    ArraySetLength_GR(*cnt, arr);
    for (int i = 0; i < *cnt; i++) {
        void *r = RecordHandler_recordStoreReadPreview(self, i);
        ((void **)I(arr, 4))[i] = r;
    }
    return arr;
}

// ---- notEnoughMemory_cbb30.cpp ----
// RecordHandler::notEnoughMemory() -> bool: free space < 900.
extern "C" uint8_t RecordHandler_notEnoughMemory()
{
    return AEFile_GetDeviceFreeSpace() < 900;
}

// ---- convertSDVersionSaves_cffa8.cpp ----
__attribute__((visibility("hidden"))) extern int *g_CSV_guard;   // DAT_000e01b8 -> guard holder
__attribute__((visibility("hidden"))) extern int *g_CSV_count;   // DAT_000e01bc -> P, *P = record count

// RecordHandler::convertSDVersionSaves()
extern "C" void RecordHandler_convertSDVersionSaves(RecordHandler *self)
{
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
        ((int *)sizes0)[i] = RecordHandler_readRecordAsByteArray(
            self, (signed char **)(I(a0, 4) + i * 4), i, false);
        ((int *)sizes1)[i] = RecordHandler_readRecordAsByteArray(
            self, (signed char **)(I(a1, 4) + i * 4), i, true);

        char num[12], path[12];
        AEString_int_ctor(num, i);
        AEString_concat(path, (char *)self + 0x14);
        AEFile_FileDelete(path);
        AEString_dtor(path);
        AEString_dtor(num);

        AEString_int_ctor(num, i);
        AEString_concat(path, (char *)self + 0x20);
        AEFile_FileDelete(path);
        AEString_dtor(path);
        AEString_dtor(num);
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
                RecordHandler_writeByteArrayAsRecord(self, lastRec, ((int *)sizes0)[last], 0,
                                                     false);
                RecordHandler_addHash(self, 0);
                int idx = (int)(*cnt) * 4 - 4;
                RecordHandler_writeByteArrayAsRecord(
                    self, *(signed char **)(I(a1, 4) + idx), *(int *)(sizes1 + idx), 0, true);
            }
            break;
        }
        int next = j + 1;
        RecordHandler_writeByteArrayAsRecord(self, rec, ((int *)sizes0)[j], next, false);
        RecordHandler_addHash(self, next);
        RecordHandler_writeByteArrayAsRecord(self, *(signed char **)(I(a1, 4) + j * 4),
                                             ((int *)sizes1)[j], next, true);
        i = next;
        n = *cnt;
    }

    ArrayReleaseArrays_SC(a0);
    ArrayReleaseArrays_SC(a1);
    RH_op_delete_arr(sizes0);
    RH_op_delete_arr(sizes1);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- addHash_cdea0.cpp ----
__attribute__((visibility("hidden"))) extern int *g_AH_guard;                  // DAT_000ddfa0 -> guard holder
__attribute__((visibility("hidden"))) extern const unsigned char RH_ah_salt[]; // DAT_000ddfa8 (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ah_key;        // DAT_000ddfac (16-byte key)

// RecordHandler::addHash(int slot)
extern "C" void RecordHandler_addHash(RecordHandler *self, int slot)
{
    int *guardP = g_AH_guard;
    volatile int saved = *guardP;

    signed char *data = 0;
    int len = RecordHandler_readRecordAsByteArray(self, &data, slot, false);
    if (-1 < len) {
        unsigned char *md = RH_op_new_arr(0x20);
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
        RecordHandler_writeByteArrayAsRecord(self, out, len + 0x20, slot, false);

        void (*del)(void *) = (void (*)(void *))RH_op_delete_arr;
        del(data);
        data = 0;
        del(out);
        del(md);
        RH_op_delete(c);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- readRecordAsByteArray_cdbfc.cpp ----
// RecordHandler::readRecordAsByteArray(signed char**, int, bool)
extern "C" int RecordHandler_readRecordAsByteArray(RecordHandler *self, signed char **out, int slot, bool fromBackup)
{
    char num[12];
    char path[12];
    unsigned int fd;
    int sz;

    String_int_ctor(num, slot);
    char *dir = (char *)self + (fromBackup ? 0x20 : 0x14);
    String_concat(path, dir, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0) {
        AEFile_OpenRead(path, &fd);
        sz = AEFile_GetFileSize(fd);
        signed char *b = RH_op_new_arr(sz | (sz >> 31));
        *out = b;
        AEFile_Read(sz, b, fd);
        AEFile_Close(fd);
    } else {
        sz = -1;
    }
    String_dtor(path);
    return sz;
}

// ---- readWanted_ce6c8.cpp ----
extern "C" void Wanted_ctor(void *self, int idx, void *name, int board, int race, bool male,
                            int ship, int weapon, int hp, int loot, int lootAmt, int reward,
                            int reqBounties, int reqMission, int numWingmen);

__attribute__((visibility("hidden"))) extern int *g_RW_guard;   // DAT_000de8a4 -> guard holder

// RecordHandler::readWanted(unsigned int fd) -> Wanted* (in r0 on return).
extern "C" void *RecordHandler_readWanted(RecordHandler *self, unsigned int fd)
{
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
    char nameCopy[12];
    AEString_copy_ctor(nameCopy, name, 0);
    Wanted_ctor(w, idx, nameCopy, board, race, male, ship, weapon, hp, loot, lootAmt, reward,
                reqBounties, reqMission, numWingmen);
    AEString_dtor(nameCopy);

    int *parts = (int *)RH_op_new_arr(0x14);
    int *p = parts;
    for (unsigned i = 0; i < 5; i++) {
        AEFile_ReadInt(p, fd);
        p++;
    }
    Wanted_setImageParts(w, parts);
    Wanted_setActive(w, active);
    Wanted_setTerminated(w, terminated);
    Wanted_setCurrentLocation(w, currentLocation);
    Wanted_setTravelsTo(w, travelsTo);
    Wanted_setLastSeen(w, lastSeen);
    AEString_dtor(name);

    if (*guardP == saved) {
        return w;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- changeSaveDirectoryToBackupDirectory_d0258.cpp ----
// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_csd_count;
// Flag holder: g -> P, *P -> the flag object (byte set to 1).
__attribute__((visibility("hidden"))) extern char **g_RH_csd_flag;

// Tail veneer at 0x1ab108: (ptr, 1, count, arr2).

// RecordHandler::changeSaveDirectoryToBackupDirectory()
extern "C" void RecordHandler_changeSaveDirectoryToBackupDirectory(RecordHandler *self)
{
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
        ((int *)sizes0)[i] = RecordHandler_readRecordAsByteArray(self, (signed char **)(I(a0, 4) + i * 4), i, false);
        ((int *)sizes1)[i] = RecordHandler_readRecordAsByteArray(self, (signed char **)(I(a1, 4) + i * 4), i, true);
        n = *cnt;
    }

    **g_RH_csd_flag = 1;
    int i = 0;
    while (i < (int)n) {
        signed char *rec = *(signed char **)(I(a0, 4) + i * 4);
        if (rec == 0) {
            i++;
        } else {
            RecordHandler_writeByteArrayAsRecord(self, rec, ((int *)sizes0)[i], i, false);
            int next = i + 1;
            RecordHandler_addHash(self, next);
            RecordHandler_writeByteArrayAsRecord(self, *(signed char **)(I(a1, 4) + i * 4), ((int *)sizes1)[i], i, true);
            n = *cnt;
            i = next;
        }
    }

    ArrayReleaseArrays_SC(a0);
    ArrayReleaseArrays_SC(a1);
    RH_op_delete_arr(sizes0);
    RecordHandler_csd_tail(sizes1, 1, n, a1);
}

// ---- _RecordHandler_cbb08.cpp ----
// String::~String function pointer (single pc-rel deref to the fn-ptr global).
__attribute__((visibility("hidden"))) extern void (*g_RH_stringDtor)(void *s);

// RecordHandler::~RecordHandler() — destroys the three String members at +0x20,
// +0x14, +0x8 (high to low) and returns `this`.
extern "C" RecordHandler *RecordHandler_dtor(RecordHandler *self)
{
    void (*dtor)(void *) = g_RH_stringDtor;
    dtor((char *)self + 0x20);
    dtor((char *)self + 0x14);
    dtor((char *)self + 0x8);
    return self;
}

// ---- recordStoreReadPreview_cbbf0.cpp ----
// RecordHandler::recordStoreReadPreview(int)
extern "C" void *RecordHandler_recordStoreReadPreview(RecordHandler *self, int slot)
{
    char path[12];
    char num[12];

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    String_dtor(num);

    unsigned int &fd = *(unsigned int *)num;  // reuse the now-dead String temp slot
    void *gr = 0;
    if (AEFile_FileExist(path) != 0) {
        AEFile_OpenRead(path, &fd);
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
        AEFile_Close(fd);
    }
    String_dtor(path);
    return gr;
}

// ---- RecordHandler_cba2c.cpp ----
// String literal pointers (each via a hidden pc-rel reloc).
__attribute__((visibility("hidden"))) extern const char RH_lit0[];
__attribute__((visibility("hidden"))) extern const char RH_lit1[];
__attribute__((visibility("hidden"))) extern const char RH_lit2[];


// RecordHandler::RecordHandler() — default-constructs the three String members at
// +0x8, +0x14, +0x20, then assigns each from a literal, and returns `this`.
extern "C" RecordHandler *RecordHandler_ctor(RecordHandler *self)
{
    char tmp[12];
    char *m0 = (char *)self + 0x8;
    char *m1 = (char *)self + 0x14;
    char *m2 = (char *)self + 0x20;

    String_default_ctor(m0);
    String_default_ctor(m1);
    String_default_ctor(m2);

    String_cstr_ctor(tmp, RH_lit0, false);
    String_assign(m0, tmp);
    String_dtor(tmp);

    String_cstr_ctor(tmp, RH_lit1, false);
    String_assign(m1, tmp);
    String_dtor(tmp);

    String_cstr_ctor(tmp, RH_lit2, false);
    String_assign(m2, tmp);
    String_dtor(tmp);

    return self;
}

// ---- writeByteArrayAsOptionsFile_cddd8.cpp ----
// RecordHandler::writeByteArrayAsOptionsFile(signed char*, int)
extern "C" void RecordHandler_writeByteArrayAsOptionsFile(RecordHandler *self, signed char *buf, int n)
{
    char tmp[12];
    unsigned int fd;

    if (AEFile_FileExist(String_copy_ctor(tmp, (char *)self + 0x8, false)) != 0)
        AEFile_FileDelete(tmp);
    AEFile_OpenWrite(tmp, &fd);
    AEFile_Write(n, buf, fd);
    AEFile_Close(fd);
    String_dtor(tmp);
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
extern "C" int RecordHandler_recordStoreWritePreview_int(RecordHandler *self, int slot)
{
    char path[12];
    char num[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0)
        AEFile_FileDelete(path);
    AEFile_OpenWrite(path, &fd);

    Status **sh = g_RH_wp_status;
    AEFile_Write_i64(Status_getPlayingTime(*sh), fd);
    AEFile_Write_i32(Status_getCredits(*sh), fd);

    Station_getName(num, Status_getStation(*sh));
    AEFile_Write_str(num, fd, true);
    String_dtor(num);

    SolarSystem_getName(num, Status_getSystem(*sh));
    AEFile_Write_str(num, fd, true);
    String_dtor(num);

    AEFile_Write_i32(Status_getCurrentCampaignMission(*sh), fd);
    AEFile_Write_i32(Status_getLevel(*sh), fd);
    AEFile_Write_f32(I(*(void **)g_RH_wp_float, 0x2c), fd);
    AEFile_Write_i32(Ship_getIndex(Status_getShip(*sh)), fd);
    AEFile_Close(fd);
    String_dtor(path);
    return 1;
}

// ---- writeAgent_cf9f0.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WA_guard;       // DAT_000dfc2c -> guard holder
__attribute__((visibility("hidden"))) extern const char g_WA_empty1[]; // DAT_000dfc38
__attribute__((visibility("hidden"))) extern const char g_WA_empty2[]; // DAT_000dfc3c

// RecordHandler::writeAgent(Agent*, unsigned int fd)
extern "C" void RecordHandler_writeAgent(RecordHandler *self, void *agent, unsigned int fd)
{
    int *guardP = g_WA_guard;
    volatile int saved = *guardP;

    AEFile_WriteInt(Agent_getCosts(agent), fd);
    AEFile_WriteInt(Agent_getSellSystemIndex(agent), fd);
    AEFile_WriteInt(Agent_getSellBlueprintIndex(agent), fd);
    AEFile_WriteInt(Agent_getEvent(agent), fd);
    AEFile_WriteInt(Agent_getIndex(agent), fd);
    AEFile_WriteInt(Agent_getOffer(agent), fd);
    AEFile_WriteInt(Agent_getRace(agent), fd);
    AEFile_WriteInt(Agent_getSellItemIndex(agent), fd);
    AEFile_WriteInt(Agent_getSellItemPrice(agent), fd);
    AEFile_WriteInt(Agent_getSellItemQuantity(agent), fd);
    AEFile_WriteInt(Agent_getStation(agent), fd);
    AEFile_WriteInt(Agent_getSystem(agent), fd);
    AEFile_WriteInt(Agent_getWingmanFriendsCount(agent), fd);
    AEFile_WriteBool(Agent_isMale(agent), fd);
    AEFile_WriteBool(Agent_hasReward(agent), fd);
    AEFile_WriteBool(Agent_hasAcceptedOffer(agent), fd);
    AEFile_WriteBool(agent->field_0x24, fd);
    AEFile_WriteBool(agent->field_0x25, fd);

    if (Agent_getImageParts(agent) == 0) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(5, fd);
        for (int i = 0; i != 5; i++) {
            int *img = Agent_getImageParts(agent);
            AEFile_WriteInt(img[i], fd);
        }
    }
    if (0x12 < Agent_getIndex(agent)) {
        AEFile_WriteInt(Agent_getSellModIndex(agent), fd);
    }

    char s[12];
    Agent_getMissionString(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);
    Agent_getName(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);
    Agent_getStationName(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);
    Agent_getSystemName(s, agent);
    AEFile_WriteString(s, fd, 1);
    AEString_dtor(s);

    void *f0c = agent->field_0xc;
    if (f0c == 0) {
        AEString_cstr_ctor(s, g_WA_empty1, 0);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
    } else {
        AEFile_WriteString(f0c, fd, 1);
    }
    void *f10 = agent->field_0x10;
    if (f10 == 0) {
        AEString_cstr_ctor(s, g_WA_empty2, 0);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
    } else {
        AEFile_WriteString(f10, fd, 1);
    }

    self->field_0x4 = agent;
    void *mission = Agent_getMission(agent);
    if (mission == 0 || *(void **)self == mission) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(1, fd);
        RecordHandler_writeMission(self, mission, fd);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- writeMission_cf840.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WM_guard;   // DAT_000df9e8 -> guard holder

// RecordHandler::writeMission(Mission*, unsigned int fd)
extern "C" void RecordHandler_writeMission(RecordHandler *self, void *m, unsigned int fd)
{
    int *guardP = g_WM_guard;
    volatile int saved = *guardP;

    AEFile_WriteInt(Mission_getType(m), fd);
    if (Mission_isEmpty(m) == 0) {
        char s[12];
        Mission_getClientName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
        Mission_getTargetName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
        Mission_getTargetStationName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);
        Mission_getTargetSystemName(s, m);
        AEFile_WriteString(s, fd, 1);
        AEString_dtor(s);

        AEFile_WriteBool(Mission_isCampaignMission(m), fd);
        if (Mission_getClientImage(m) == 0) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(5, fd);
            for (int i = 0; i != 5; i++) {
                int *img = Mission_getClientImage(m);
                AEFile_WriteInt(img[i], fd);
            }
        }
        AEFile_WriteInt(Mission_getClientRace(m), fd);
        AEFile_WriteInt(Mission_getCosts(m), fd);
        AEFile_WriteInt(Mission_getBonus(m), fd);
        AEFile_WriteInt(Mission_getReward(m), fd);
        AEFile_WriteInt(Mission_getTargetStation(m), fd);
        AEFile_WriteInt(Mission_getDifficulty(m), fd);
        AEFile_WriteInt(Mission_getProductionGoodIndex(m), fd);
        AEFile_WriteInt(Mission_getProductionGoodAmount(m), fd);
        AEFile_WriteInt(Mission_getStatusValue(m), fd);
        AEFile_WriteBool(Mission_isVisible(m), fd);

        *(void **)self = m;
        void *agent = Mission_getAgent(m);
        if (agent == 0 || self->field_0x4 == agent) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(1, fd);
            RecordHandler_writeAgent(self, agent, fd);
        }
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
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
extern "C" void RecordHandler_loadOptions(RecordHandler *self)
{
    int *guardP = g_LO_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile_FileExist(path) != 0) {
        unsigned int fd;
        unsigned int hashFd = AEFile_OpenRead(path, &fd);
        int valid = RecordHandler_checkHash(hashFd);
        AEFile_Close(fd);
        if (valid != 0) {
            AEFile_OpenRead(path, &fd);
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
            *nameSlot = (int)ns;
            AEString_dtor(nameCopy);

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
            AEFile_Close(fd);

            int langVal = lang;
            if (-1 < langVal) {
                if (langVal == 9) langVal = 0;
                GameText_setLanguage((short)**(int **)g_LO_textObj, langVal);
                Globals_loadFont(**(int **)g_LO_fontKind);
            }

            int *fmodSlot = g_LO_fmodSlot;
            if (*fmodSlot != 0) {
                void *fm = (void *)(long)*fmodSlot;
                FModSound_setAudioLanguage(fm, lang);
                FModSound_enableCategory(fm, 1);
                FModSound_enableCategory(fm, 1);
                int v = FModSound_enableCategory(fm, 1);
                v = FModSound_setVolume(fm, v);
                v = FModSound_setVolume(fm, v);
                FModSound_setVolume(fm, v);
            }

            AEFile_ReadBool(s + 0x62, fd);
            AEString_dtor(name);
        }
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- loadResolutionValue_cd260.cpp ----
__attribute__((visibility("hidden"))) extern int *g_LRV_guard;   // DAT_000dd444 -> guard holder

// RecordHandler::loadResolutionValue() — reads the options file at self+8 into a settings
// scratch struct. self is in r0; the path String lives at self+8.
extern "C" void RecordHandler_loadResolutionValue(RecordHandler *self)
{
    int *guardP = g_LRV_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile_FileExist(path) != 0) {
        unsigned int fd;
        AEFile_OpenRead(path, &fd);

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

        AEFile_Close(fd);
        AEString_dtor(name);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- writeByteArrayAsRecord_cdcb0.cpp ----
// RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool)
extern "C" int RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, int n, int slot, bool toBackup)
{
    char num[12];
    char path[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    char *dir = (char *)self + (toBackup ? 0x20 : 0x14);
    String_concat(path, dir, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0)
        AEFile_FileDelete(path);
    AEFile_OpenWrite(path, &fd);
    AEFile_Write(n, buf, fd);
    AEFile_Close(fd);
    String_dtor(path);
    return n;
}

// ---- readMission_cdfb4.cpp ----
extern "C" void Mission_ctorFull(void *self, int type, void *clientName, int *img, int clientRace,
                                 int reward, int targetStation, int difficulty);

__attribute__((visibility("hidden"))) extern int *g_RM_guard;   // DAT_000de244 -> guard holder

// RecordHandler::readMission(unsigned int fd) -> Mission* (in r0).
extern "C" void *RecordHandler_readMission(RecordHandler *self, unsigned int fd)
{
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
        void *agent = (hasAgent < 1) ? 0 : RecordHandler_readAgent(self, fd);

        if (!isEmpty) {
            mission = RH_op_new(0x78);
            char nameCopy[12];
            AEString_copy_ctor(nameCopy, clientName, 0);
            Mission_ctorFull(mission, type, nameCopy, img, clientRace, reward, targetStationIdx,
                             difficulty);
            AEString_dtor(nameCopy);
        } else {
            mission = RH_op_new(0x78);
            Mission_ctorEmpty(mission, type, reward, targetStationIdx);
        }
        Mission_setCosts(mission, costs);
        Mission_setBonus(mission, bonus);
        Mission_setProductionGoods(mission, prodIdx, prodAmt);
        Mission_setStatusValue(mission, statusValue);
        Mission_setVisible(mission, visible);
        Mission_setAgent(mission, agent);

        char tgtNameCopy[12];
        AEString_copy_ctor(tgtNameCopy, targetName, 0);
        Mission_setTargetName(mission, tgtNameCopy);
        AEString_dtor(tgtNameCopy);

        if (agent != 0) {
            Agent_setMission(agent, mission);
        }

        AEString_dtor(targetSystem);
        AEString_dtor(targetStation);
        AEString_dtor(targetName);
        AEString_dtor(clientName);
    }

    if (*guardP == saved) {
        return mission;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
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
extern "C" void RecordHandler_saveOptions(RecordHandler *self)
{
    int *guardP = g_SO_guard;
    volatile int saved = *guardP;

    void *path = (char *)self + 8;
    if (AEFile_FileExist(path) != 0) {
        AEFile_FileDelete(path);
    }
    unsigned int fd;
    AEFile_OpenWrite(path, &fd);

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
        AEString_dtor(def);
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
    AEFile_Close(fd);
    RecordHandler_addHashToOptions(self);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- readAgent_ce250.cpp ----
extern "C" void Agent_ctor(void *self, int idx, void *name, int station, int system, int race,
                           int male, int sellSys, int sellBp, int sellMod, int sellItemPrice);

__attribute__((visibility("hidden"))) extern int *g_RA_guard;   // DAT_000de608 -> guard holder

// RecordHandler::readAgent(unsigned int fd) -> Agent* (in r0).
extern "C" void *RecordHandler_readAgent(RecordHandler *self, unsigned int fd)
{
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
    void *mission = (hasMission < 1) ? 0 : RecordHandler_readMission(self, fd);

    void *agent = RH_op_new(0x98);
    char nameCopy[12];
    AEString_copy_ctor(nameCopy, name, 0);
    Agent_ctor(agent, idx, nameCopy, station, system, race, male, sellSys, sellBp, sellMod,
               sellItemIdx);
    AEString_dtor(nameCopy);

    Agent_setCosts(agent, costs);
    Agent_setEvent(agent, event);
    Agent_setOffer(agent, offer);
    Agent_setSellItemData(agent, sellItemPrice, sellItemQty, sellItemIdx);

    if (*((int *)strE + 2) != 0) {
        void *s = RH_op_new(0xc);
        AEString_copy_ctor(s, strE, 0);
        agent->field_0xc = s;
    }
    if (*((int *)strF + 2) != 0) {
        void *s = RH_op_new(0xc);
        AEString_copy_ctor(s, strF, 0);
        agent->field_0x10 = s;
    }
    agent->field_0x14 = wingmen;

    void *arr = RH_op_new(0xc);
    ArrayStr_ctor(arr);
    ArraySetLength_Str(wingmen, arr);
    for (int i = 0; i < (int)wingmen; i++) {
        void *s = RH_op_new(0xc);
        void *src = (i == 0) ? (void *)strE : (void *)strF;
        AEString_copy_ctor(s, src, 0);
        *(void **)(*(char **)((char *)arr + 4) + i * 4) = s;
    }
    Agent_setWingmanFriendNames(agent, arr);
    Agent_giveRewardAtNextChat(agent, hasReward);
    Agent_setOfferAccepted(agent, accepted);
    Agent_setImageParts(agent, img);

    char tmp[12];
    AEString_copy_ctor(tmp, missionStr, 0);
    Agent_setMissionString(agent, tmp);
    AEString_dtor(tmp);
    AEString_copy_ctor(tmp, stationName, 0);
    Agent_setStationName(agent, tmp);
    AEString_dtor(tmp);
    AEString_copy_ctor(tmp, systemName, 0);
    Agent_setSystemName(agent, tmp);
    AEString_dtor(tmp);
    Agent_setMission(agent, mission);

    agent->field_0x24 = raw24;
    agent->field_0x25 = raw25;

    AEString_dtor(strF);
    AEString_dtor(strE);
    AEString_dtor(systemName);
    AEString_dtor(stationName);
    AEString_dtor(name);
    AEString_dtor(missionStr);

    if (*guardP == saved) {
        return agent;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- writeWanted_cfc40.cpp ----
__attribute__((visibility("hidden"))) extern int *g_WW_guard;   // DAT_000dfd70 -> guard holder

// RecordHandler::writeWanted(Wanted*, unsigned int fd)
extern "C" void RecordHandler_writeWanted(RecordHandler *self, void *w, unsigned int fd)
{
    (void)self;
    int *guardP = g_WW_guard;
    volatile int saved = *guardP;

    AEFile_WriteBool(Wanted_isActive(w), fd);
    AEFile_WriteBool(Wanted_isTerminated(w), fd);
    AEFile_WriteInt(Wanted_getCurrentLocation(w), fd);
    AEFile_WriteInt(Wanted_getTravelsTo(w), fd);
    AEFile_WriteInt(Wanted_getLastSeen(w), fd);

    char name[12];
    Wanted_getName(name, w);
    AEFile_WriteString(name, fd, 1);
    AEString_dtor(name);

    AEFile_WriteInt(Wanted_getIndex(w), fd);
    AEFile_WriteInt(Wanted_getBoard(w), fd);
    AEFile_WriteInt(Wanted_getRace(w), fd);
    AEFile_WriteBool(Wanted_isMale(w), fd);
    AEFile_WriteInt(Wanted_getShip(w), fd);
    AEFile_WriteInt(Wanted_getWeapon(w), fd);
    AEFile_WriteInt(Wanted_getHitpoints(w), fd);
    AEFile_WriteInt(Wanted_getLoot(w), fd);
    AEFile_WriteInt(Wanted_getLootAmount(w), fd);
    AEFile_WriteInt(Wanted_getReward(w), fd);
    AEFile_WriteInt(Wanted_getRequiredBounties(w), fd);
    AEFile_WriteInt(Wanted_getRequiredMission(w), fd);
    AEFile_WriteInt(Wanted_getNumWingmen(w), fd);

    for (int i = 0; i != 5; i++) {
        int *parts = Wanted_getImageParts(w);
        AEFile_WriteInt(parts[i], fd);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- recordStoreWritePreview_cfea4.cpp ----
struct Station;
struct Ship;


// RecordHandler::recordStoreWritePreview(GameRecord*, int)
extern "C" int RecordHandler_recordStoreWritePreview(RecordHandler *self, void *rec, int slot)
{
    if (rec == 0)
        return 0;

    char path[12];
    char num[12];
    unsigned int fd;

    String_int_ctor(num, slot);
    String_concat(path, (char *)self + 0x20, num);
    String_dtor(num);

    if (AEFile_FileExist(path) != 0)
        AEFile_FileDelete(path);
    AEFile_OpenWrite(path, &fd);

    AEFile_Write_i64(*(long long *)((char *)rec + 0x10), fd);
    AEFile_Write_i32(I(rec, 0x8), fd);

    Station_getName(num, *(void **)((char *)rec + 0x138));
    AEFile_Write_str(num, fd, true);
    String_dtor(num);

    AEFile_Write_str((char *)rec + 0x188, fd, true);
    AEFile_Write_i32(I(rec, 0x40), fd);
    AEFile_Write_i32(I(rec, 0x20), fd);
    AEFile_Write_f32(I(rec, 0x11c), fd);
    AEFile_Write_i32(Ship_getIndex(*(void **)((char *)rec + 0x130)), fd);
    AEFile_Close(fd);
    String_dtor(path);
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
extern "C" void RecordHandler_recordStoreWrite(RecordHandler *self, int slot)
{
    int *guardP = g_RSW_guard;
    volatile int saved = *guardP;

    char num[12], path[12];
    AEString_int_ctor(num, slot);
    AEString_concat(path, (char *)self + 0x14);
    AEString_dtor(num);

    if (AEFile_FileExist(path) != 0) {
        AEFile_FileDelete(path);
    }
    unsigned int fd;
    AEFile_OpenWrite(path, &fd);

    // Visited-systems bitmap (0x87 entries).
    int visited = Galaxy_getVisited(*g_RSW_galaxy);
    AEFile_WriteInt(0x87, fd);
    for (unsigned i = 0; i < 0x87; i++) {
        AEFile_WriteBool(*(bool *)(visited + i), fd);
    }

    int *status = g_RSW_status;
    void *st = (void *)(long)*status;
    AEFile_WriteInt(Status_getCredits(), fd);
    AEFile_WriteInt(Status_getRating(st), fd);
    AEFile_WriteLong(Status_getPlayingTime(), fd);
    AEFile_WriteInt(Status_getKills(st), fd);
    AEFile_WriteInt(Status_getMissionCount(st), fd);
    AEFile_WriteInt(Status_getLevel(), fd);
    AEFile_WriteInt(Status_getLastXP(st), fd);
    AEFile_WriteInt(Status_getGoodsProduced(st), fd);
    AEFile_WriteInt(Status_getStationsVisited(st), fd);
    AEFile_WriteInt(Status_getCurrentCampaignMission(), fd);
    RecordHandler_writeMission(self, Status_getFreelanceMission(), fd);
    RecordHandler_writeMission(self, Status_getCampaignMission(), fd);
    AEFile_WriteInt(Status_getJumpgateUsed(st), fd);
    AEFile_WriteInt(Status_getCapturedCrates(st), fd);
    AEFile_WriteInt(Status_getBoughtEquipment(st), fd);
    AEFile_WriteInt(Status_getPirateKills(st), fd);
    AEFile_WriteInt(*(int *)(*status + 0x80), fd);

    // Remaining object-graph serialization.
    RecordHandler_recordStoreWrite_body(self, fd);

    AEFile_Close(fd);
    RecordHandler_addHash(self, slot);
    AEString_dtor(path);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- readOptionsFileAsByteArray_cdd4c.cpp ----
// RecordHandler::readOptionsFileAsByteArray(signed char**)
extern "C" int RecordHandler_readOptionsFileAsByteArray(RecordHandler *self, signed char **out)
{
    char tmp[12];
    unsigned int fd;
    int sz;

    String_copy_ctor(tmp, (char *)self + 0x8, false);
    if (AEFile_FileExist(tmp) != 0) {
        AEFile_OpenRead(tmp, &fd);
        sz = AEFile_GetFileSize(fd);
        signed char *buf = RH_op_new_arr(sz | (sz >> 31));
        *out = buf;
        AEFile_Read(sz, buf, fd);
        AEFile_Close(fd);
    } else {
        sz = -1;
    }
    String_dtor(tmp);
    return sz;
}

// ---- recordStoreRead_cbd34.cpp ----
// Deep GameRecord field read (ship, stations, standings, blueprints, pending products, agents,
// ships, wanteds, flags). A very long straight-line reader delegated to a helper so this
// translation preserves the path/hash/open/prefix/close flow while staying tractable.

__attribute__((visibility("hidden"))) extern int *g_RSR_guard;   // DAT_000dc094 -> guard holder

// RecordHandler::recordStoreRead(int slot)
extern "C" void RecordHandler_recordStoreRead(RecordHandler *self, int slot)
{
    int *guardP = g_RSR_guard;
    volatile int saved = *guardP;

    char num[12], path[12];
    AEString_int_ctor(num, slot);
    AEString_concat(path, (char *)self + 0x14);
    AEString_dtor(num);

    if (AEFile_FileExist(path) != 0) {
        unsigned int fd;
        unsigned int hashFd = AEFile_OpenRead(path, &fd);
        int valid = RecordHandler_checkHash(hashFd);
        AEFile_Close(fd);
        if (valid != 0) {
            AEFile_OpenRead(path, &fd);
            char *rec = (char *)RH_op_new(0x1c8);
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
            *(void **)(rec + 0x54) = RecordHandler_readMission(self, fd);
            *(void **)(rec + 0x58) = RecordHandler_readMission(self, fd);
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
            RecordHandler_recordStoreRead_body(self, rec, fd);

            AEFile_Close(fd);
        }
    }

    AEString_dtor(path);
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- checkHash_cd7b0.cpp ----
__attribute__((visibility("hidden"))) extern int *g_CH_guard;          // DAT_000dd8b8 -> guard holder
__attribute__((visibility("hidden"))) extern const unsigned char RH_ch_salt[]; // DAT_000dd8c0 (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ch_key;        // DAT_000dd8c4 (16-byte key)

// RecordHandler::checkHash(unsigned int fd) -> bool (1=valid, 0=invalid).
extern "C" int RecordHandler_checkHash(unsigned int fd)
{
    int *guardP = g_CH_guard;
    volatile int saved = *guardP;
    int result = 0;

    int sz = AEFile_GetFileSize(fd);
    if (-1 < sz) {
        unsigned char *buf = RH_op_new_arr((unsigned)sz);
        AEFile_Read((unsigned)sz, buf, fd);
        void *arr = RH_op_new(0xc);
        Array_UC_ctor(arr);
        ArrayAdd_UC(buf, (unsigned)sz, arr);
        RH_op_delete_arr(buf);

        unsigned len = *(unsigned *)arr;
        if (0x21 < len) {
            unsigned char *md = RH_op_new_arr(0x20);
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

    if (*guardP == saved) {
        return result;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}

// ---- addHashToOptions_cdafc.cpp ----
// SHA salt blob (25 bytes) and the second 16-byte key, both via hidden pc-rel relocs.
__attribute__((visibility("hidden"))) extern const unsigned char RH_aho_salt[];
__attribute__((visibility("hidden"))) extern unsigned char **RH_aho_key;


// RecordHandler::addHashToOptions()
extern "C" void RecordHandler_addHashToOptions(RecordHandler *self)
{
    signed char *data = 0;
    int len = RecordHandler_readOptionsFileAsByteArray(self, &data);
    if (len < 0)
        return;

    unsigned char *md = RH_op_new_arr(0x20);
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
    RecordHandler_writeByteArrayAsOptionsFile(self, out, len + 0x20);

    void (*del)(void *) = (void (*)(void *))RH_op_delete_arr;
    del(data);
    data = 0;
    del(out);
    del(md);
    RH_op_delete(c);
}
