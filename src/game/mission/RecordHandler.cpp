#include "game/mission/RecordHandler.h"
#include "game/mission/GameRecord.h"
#include "game/mission/PendingProduct.h"
#include "game/mission/Achievements.h"
#include "game/mission/BluePrint.h"
#include "engine/core/GameText.h"
#include "game/mission/Item.h"
#include "game/world/Standing.h"
#include "game/ship/Ship.h"
#include "game/world/Galaxy.h"
#include "engine/file/AEFile.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Status.h"
#include "game/world/Station.h"
#include "game/mission/Mission.h"
#include "game/world/SolarSystem.h"
#include "game/world/Wanted.h"
#include "game/ship/Agent.h"
#include "platform/libc.h"

// SHA-256 hashing used to sign records (digest appended on write, verified on read).
extern "C" void SHA256_Init(void *c);
extern "C" void SHA256_Update(void *c, const void *data, int n);
extern "C" void SHA256_Final(unsigned char *md, void *c);
extern "C" void AEFile_ReadBool(void *out, unsigned int fd);
extern "C" void AEFile_ReadInt(void *out, unsigned int fd);
extern "C" void AEFile_ReadString(void *out, unsigned int fd, int flag);
extern "C" void AEFile_Read_i64(void *dst, unsigned int fd);
extern "C" void AEFile_Read_i32(void *dst, unsigned int fd);
extern "C" void AEFile_Read_bool(void *dst, unsigned int fd, int b);
extern "C" void AEFile_Read_f32(void *dst, unsigned int fd);
extern "C" void AEFile_Write_i64(long long v, unsigned int fd);
extern "C" void AEFile_Write_i32(int v, unsigned int fd);
extern "C" void AEFile_Write_str(void *s, unsigned int fd, int b);
extern "C" void AEFile_Write_f32(int v, unsigned int fd);
extern "C" void AEFile_WriteInt(int v, unsigned int fd);
extern "C" void AEFile_WriteBool(int v, unsigned int fd);
extern "C" void AEFile_WriteString(void *s, unsigned int fd, int flag);
extern "C" void AEFile_ReadByte(void *out, unsigned int fd);
extern "C" void AEFile_ReadFloat(void *out, unsigned int fd);
extern "C" void AEFile_ReadShort(void *out, unsigned int fd);
int RecordHandler_checkHash(unsigned int fd);
void Globals_loadFont(int kind);
extern "C" void AEFile_WriteByte(int v, unsigned int fd);
extern "C" void AEFile_WriteFloat(int v, unsigned int fd);
extern "C" void AEFile_WriteShort(int v, unsigned int fd);
int GameText_getLanguage();
extern "C" void AEFile_WriteLong(long long v, unsigned int fd);
extern "C" void AEFile_ReadLong(void *out, unsigned int fd);

// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;

// RecordHandler::readAllRecords() -> Array<GameRecord*>*
void * RecordHandler::readAllRecords() {
    Array<void*> *arr = new Array<void*>();
    int *cnt = g_RH_recordCount;
    arr->resize(*cnt);
    for (int i = 0; i < *cnt; i++) {
        void *r = this->recordStoreRead(i);
        (*arr)[i] = r;
    }
    return arr;
}

// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_recordCount;

// RecordHandler::readAllPreviewRecords() -> Array<GameRecord*>*
void * RecordHandler::readAllPreviewRecords() {
    Array<void*> *arr = new Array<void*>();
    int *cnt = g_RH_recordCount;
    arr->resize(*cnt);
    for (int i = 0; i < *cnt; i++) {
        void *r = this->recordStoreReadPreview(i);
        (*arr)[i] = r;
    }
    return arr;
}

// RecordHandler::notEnoughMemory() -> bool: free space < 900.
uint8_t RecordHandler_notEnoughMemory()
{
    return AEFile::GetDeviceFreeSpace() < 900;
}

__attribute__((visibility("hidden"))) extern int *g_CSV_count;   // P, *P = record count

// RecordHandler::convertSDVersionSaves()
void RecordHandler::convertSDVersionSaves() {
    Array<signed char*> *a0 = new Array<signed char*>();
    int *cnt = g_CSV_count;
    a0->resize(*cnt);
    Array<signed char*> *a1 = new Array<signed char*>();
    a1->resize(*cnt);

    unsigned n = *cnt;
    int *sizes0 = new int[n];
    int *sizes1 = new int[n];

    for (int i = 0; i < (int)n; i++) {
        sizes0[i] = this->readRecordAsByteArray(&(*a0)[i], i, false);
        sizes1[i] = this->readRecordAsByteArray(&(*a1)[i], i, true);

        String num, path;
        num.ctor_int(i);
        path = this->recordDir + num;
        AEFile::FileDelete(path);

        num.ctor_int(i);
        path = this->backupDir + num;
        AEFile::FileDelete(path);
        n = *cnt;
    }

    int i = 0;
    while (true) {
        int last = (int)n - 1;
        int j = i;
        signed char *rec = 0;
        bool found = false;
        while (j < last) {
            rec = (*a0)[j];
            if (rec != 0) { found = true; break; }
            j++;
        }
        if (!found) {
            // process the final slot (index last) then finish
            signed char *lastRec = (*a0)[last];
            if (lastRec != 0) {
                this->writeByteArrayAsRecord(lastRec, sizes0[last], 0, false);
                this->addHash(0);
                int idx = (int)(*cnt) - 1;
                this->writeByteArrayAsRecord((*a1)[idx], sizes1[idx], 0, true);
            }
            break;
        }
        int next = j + 1;
        this->writeByteArrayAsRecord(rec, sizes0[j], next, false);
        this->addHash(next);
        this->writeByteArrayAsRecord((*a1)[j], sizes1[j], next, true);
        i = next;
        n = *cnt;
    }

    for (signed char *e : *a0) delete[] e;
    a0->clear();
    for (signed char *e : *a1) delete[] e;
    a1->clear();
    delete a0;
    delete a1;
    delete[] sizes0;
    delete[] sizes1;

    return;
}

__attribute__((visibility("hidden"))) extern const unsigned char RH_ah_salt[]; // (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ah_key;        // (16-byte key)

// RecordHandler::addHash(int slot)
void RecordHandler::addHash(int slot) {
    signed char *data = 0;
    int len = this->readRecordAsByteArray(&data, slot, false);
    if (-1 < len) {
        unsigned char *md = new unsigned char[0x20];
        unsigned char *c = new unsigned char[0x70];
        SHA256_Init(c);
        SHA256_Update(c, data, len);
        SHA256_Update(c, RH_ah_salt, 0x19);
        SHA256_Update(c, *RH_ah_key, 0x10);
        SHA256_Final(md, c);

        signed char *out = new signed char[len + 0x20];
        memcpy(out, data, len);
        unsigned long long *dst = (unsigned long long *)(out + len);
        unsigned long long *src = (unsigned long long *)md;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        this->writeByteArrayAsRecord(out, len + 0x20, slot, false);

        delete[] data;
        delete[] out;
        delete[] md;
        delete[] c;
    }
}

// RecordHandler::readRecordAsByteArray(signed char**, int, bool)
int RecordHandler::readRecordAsByteArray(signed char **out, int slot, bool fromBackup) {
    String num;
    String path;
    unsigned int fd;
    int sz;

    num.ctor_int(slot);
    String &dir = fromBackup ? this->backupDir : this->recordDir;
    path = dir + num;

    if (AEFile::FileExist(path) != 0) {
        AEFile::OpenRead(path, &fd);
        sz = AEFile::GetFileSize(fd);
        signed char *b = new signed char[sz | (sz >> 31)];
        *out = b;
        AEFile::Read(sz, b, fd);
        AEFile::Close(fd);
    } else {
        sz = -1;
    }
    return sz;
}


// RecordHandler::readWanted(unsigned int fd) -> Wanted* (in r0 on return).
void * RecordHandler::readWanted(unsigned int fd) {
    (void)this;
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

    String name;
    name.ctor();

    int idx = 0, board = 0, race = 0;
    bool male = true;
    int ship = 0, weapon = 0, hp = 0, loot = 0, lootAmt = 0;
    int reward = 0, reqBounties = 0, reqMission = 0, numWingmen = 0;

    AEFile_ReadString(&name, fd, 1);
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

    Wanted *w = new Wanted(idx, name, board, race, male, ship, weapon, hp, loot,
                           lootAmt, reward, reqBounties, reqMission, numWingmen);

    int *parts = new int[5];
    int *p = parts;
    for (unsigned i = 0; i < 5; i++) {
        AEFile_ReadInt(p, fd);
        p++;
    }
    w->setImageParts(parts);
    w->setActive(active);
    w->setTerminated(terminated);
    w->setCurrentLocation(currentLocation);
    w->setTravelsTo(travelsTo);
    w->setLastSeen(lastSeen);

    return w;
}

// Global holder: g -> P, *P -> the record count.
__attribute__((visibility("hidden"))) extern int *g_RH_csd_count;
// Flag holder: g -> P, *P -> the flag object (byte set to 1).
__attribute__((visibility("hidden"))) extern char **g_RH_csd_flag;

// RecordHandler::changeSaveDirectoryToBackupDirectory()
void RecordHandler::changeSaveDirectoryToBackupDirectory() {
    Array<signed char*> *a0 = new Array<signed char*>();
    int *cnt = g_RH_csd_count;
    a0->resize(*cnt);
    Array<signed char*> *a1 = new Array<signed char*>();
    a1->resize(*cnt);

    unsigned int n = *cnt;
    int *sizes0 = new int[n];
    int *sizes1 = new int[n];

    for (int i = 0; i < (int)n; i++) {
        sizes0[i] = this->readRecordAsByteArray(&(*a0)[i], i, false);
        sizes1[i] = this->readRecordAsByteArray(&(*a1)[i], i, true);
        n = *cnt;
    }

    **g_RH_csd_flag = 1;
    int i = 0;
    while (i < (int)n) {
        signed char *rec = (*a0)[i];
        if (rec == 0) {
            i++;
        } else {
            this->writeByteArrayAsRecord(rec, sizes0[i], i, false);
            int next = i + 1;
            this->addHash(next);
            this->writeByteArrayAsRecord((*a1)[i], sizes1[i], i, true);
            n = *cnt;
            i = next;
        }
    }

    for (signed char *e : *a0) delete[] e;
    a0->clear();
    for (signed char *e : *a1) delete[] e;
    a1->clear();
    delete a0;
    delete a1;
    delete[] sizes0;
    delete[] sizes1;
}

// RecordHandler::recordStoreReadPreview(int)
void * RecordHandler::recordStoreReadPreview(int slot) {
    String path;
    String num;

    num.ctor_int(slot);
    path = this->backupDir + num;

    unsigned int fd;
    void *gr = 0;
    if (AEFile::FileExist(path) != 0) {
        AEFile::OpenRead(path, &fd);
        gr = new GameRecord();
        // RAWREAD: GameRecord payload is a positional save-image blob (no per-field members in GameRecord.h).
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
    return gr;
}

// Path-prefix literals for the options file and the record directories.
__attribute__((visibility("hidden"))) extern const char RH_lit0[];
__attribute__((visibility("hidden"))) extern const char RH_lit1[];
__attribute__((visibility("hidden"))) extern const char RH_lit2[];

RecordHandler::RecordHandler()
    : currentMission(0),
      currentAgent(0),
      optionsPath(RH_lit0),
      recordDir(RH_lit1),
      backupDir(RH_lit2) {
}

RecordHandler::~RecordHandler() {
}

// RecordHandler::writeByteArrayAsOptionsFile(signed char*, int)
void RecordHandler::writeByteArrayAsOptionsFile(signed char *buf, int n) {
    String tmp;
    unsigned int fd;

    tmp.ctor_copy(&this->optionsPath, false);
    if (AEFile::FileExist(tmp) != 0)
        AEFile::FileDelete(tmp);
    AEFile::OpenWrite(tmp, &fd);
    ((AEFile *)(n))->Write(buf, fd);
    AEFile::Close(fd);
}

// Holds the object whose +0x2c float is written near the end of the preview record.
__attribute__((visibility("hidden"))) extern int *g_RH_wp_float;

// RecordHandler::recordStoreWritePreview(int)
int RecordHandler::recordStoreWritePreview_int(int slot) {
    String path;
    String num;
    unsigned int fd;

    num.ctor_int(slot);
    path = this->backupDir + num;

    if (AEFile::FileExist(path) != 0)
        AEFile::FileDelete(path);
    AEFile::OpenWrite(path, &fd);

    AEFile_Write_i64(gStatus->getPlayingTime(), fd);
    AEFile_Write_i32(gStatus->getCredits(), fd);

    num = ((Station *)(&num))->getName();
    AEFile_Write_str(&num, fd, true);

    num = ((SolarSystem *)(&num))->getName();
    AEFile_Write_str(&num, fd, true);

    AEFile_Write_i32(gStatus->getCurrentCampaignMission(), fd);
    AEFile_Write_i32(gStatus->getLevel(), fd);
    AEFile_Write_f32(*(int *)((char *)*(void **)g_RH_wp_float + 0x2c), fd);   // RAWREAD: opaque global object (g_RH_wp_float), unmodeled +0x2c float
    AEFile_Write_i32(((Ship *)(gStatus->getShip()))->getIndex(), fd);
    AEFile::Close(fd);
    return 1;
}

// RecordHandler::recordStoreWritePreview(int) — the single-arg preview writer pulls the
// preview fields from the live Status/Station/SolarSystem/Ship state; the body matches
// recordStoreWritePreview_int(int).
void RecordHandler::recordStoreWritePreview(int slot) {
    this->recordStoreWritePreview_int(slot);
}

__attribute__((visibility("hidden"))) extern const char g_WA_empty1[];
__attribute__((visibility("hidden"))) extern const char g_WA_empty2[];

// RecordHandler::writeAgent(Agent*, unsigned int fd)
void RecordHandler::writeAgent(Agent *agentPtr, unsigned int fd) {
    RecordHandler *self = this;
    Agent *agent = agentPtr;
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

    String s;
    s = agent->getMissionString();
    AEFile_WriteString(&s, fd, 1);
    s = agent->getName();
    AEFile_WriteString(&s, fd, 1);
    s = agent->getStationName();
    AEFile_WriteString(&s, fd, 1);
    s = agent->getSystemName();
    AEFile_WriteString(&s, fd, 1);

    void *f0c = agent->wingman1;
    if (f0c == 0) {
        s = String(g_WA_empty1);
        AEFile_WriteString(&s, fd, 1);
    } else {
        AEFile_WriteString(f0c, fd, 1);
    }
    void *f10 = agent->wingman2;
    if (f10 == 0) {
        s = String(g_WA_empty2);
        AEFile_WriteString(&s, fd, 1);
    } else {
        AEFile_WriteString(f10, fd, 1);
    }

    self->currentAgent = agent;
    Mission *mission = agent->getMission();
    if (mission == 0 || self->currentMission == mission) {
        AEFile_WriteInt(-1, fd);
    } else {
        AEFile_WriteInt(1, fd);
        self->writeMission(mission, fd);
    }

    return;
}


// RecordHandler::writeMission(Mission*, unsigned int fd)
void RecordHandler::writeMission(Mission *m, unsigned int fd) {
    RecordHandler *self = this;
    AEFile_WriteInt(m->getType(), fd);
    if (m->isEmpty() == 0) {
        String s;
        s = m->getClientName();
        AEFile_WriteString(&s, fd, 1);
        s = m->getTargetName();
        AEFile_WriteString(&s, fd, 1);
        s = m->getTargetStationName();
        AEFile_WriteString(&s, fd, 1);
        s = m->getTargetSystemName();
        AEFile_WriteString(&s, fd, 1);

        AEFile_WriteBool(m->isCampaignMission(), fd);
        if (m->getClientImage() == 0) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(5, fd);
            for (int i = 0; i != 5; i++) {
                int *img = (int *)(long)m->getClientImage();
                AEFile_WriteInt(img[i], fd);
            }
        }
        AEFile_WriteInt(m->getClientRace(), fd);
        AEFile_WriteInt(m->getCosts(), fd);
        AEFile_WriteInt(m->getBonus(), fd);
        AEFile_WriteInt(m->getReward(), fd);
        AEFile_WriteInt(m->getTargetStation(), fd);
        AEFile_WriteInt(m->getDifficulty(), fd);
        AEFile_WriteInt(m->getProductionGoodIndex(), fd);
        AEFile_WriteInt(m->getProductionGoodAmount(), fd);
        AEFile_WriteInt(m->getStatusValue(), fd);
        AEFile_WriteBool(m->isVisible(), fd);

        self->currentMission = m;
        Agent *agent = m->getAgent();
        if (agent == 0 || self->currentAgent == agent) {
            AEFile_WriteInt(-1, fd);
        } else {
            AEFile_WriteInt(1, fd);
            self->writeAgent(agent, fd);
        }
    }

    return;
}

__attribute__((visibility("hidden"))) extern unsigned char *g_LO_settings; // settings struct base
__attribute__((visibility("hidden"))) extern int **g_LO_statusObj;   // *->obj (field 0xfa)
__attribute__((visibility("hidden"))) extern int *g_LO_nameSlot;     // name String* holder
__attribute__((visibility("hidden"))) extern int *g_LO_extraInt;     // int
__attribute__((visibility("hidden"))) extern bool *g_LO_flag1;       // bool
__attribute__((visibility("hidden"))) extern bool *g_LO_flag2;       // bool
__attribute__((visibility("hidden"))) extern int *g_LO_textObj;      // *->obj
__attribute__((visibility("hidden"))) extern int *g_LO_fontKind;     // *->int
__attribute__((visibility("hidden"))) extern int *g_LO_fmodSlot;     // FModSound* holder

// RecordHandler::loadOptions()
void RecordHandler::loadOptions() {
    String &path = this->optionsPath;
    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);
        int valid = RecordHandler_checkHash(fd);
        AEFile::Close(fd);
        if (valid != 0) {
            AEFile::OpenRead(path, &fd);
            unsigned char *s = g_LO_settings;

            AEFile_ReadByte(s + 0x10, fd);
            AEFile_ReadByte(s + 0x11, fd);
            *(unsigned char *)(*(int *)g_LO_statusObj + 0xfa) = s[0x11];   // RAWREAD: opaque status handle (g_LO_statusObj), original-layout byte 0xfa
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
                // Free the previously-stored option name (a heap String*) before reading the new one.
                delete (String *)(long)*nameSlot;
                *nameSlot = 0;
            }

            String name;
            name.ctor();
            AEFile_ReadString(&name, fd, 0);
            String *ns = new String(name);
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
                // Apply the per-category enable flags and volumes just read from the options file.
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


// RecordHandler::loadResolutionValue() — reads the options file at self+8 into a settings
// scratch struct. self is in r0; the path String lives at self+8.
void RecordHandler::loadResolutionValue() {
    String &path = this->optionsPath;
    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);

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

        String name;
        name.ctor();
        AEFile_ReadString(&name, fd, 0);

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

// RecordHandler::loadResolutionValue(float) — the overload taking a resolution argument
// reads the same options-file settings block as the no-arg form; the argument is not used
// by the read path, so it shares the sibling's body.
void RecordHandler::loadResolutionValue(float resolution) {
    (void)resolution;
    this->loadResolutionValue();
}

// RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool)
int RecordHandler::writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup) {
    String num;
    String path;
    unsigned int fd;

    num.ctor_int(slot);
    String &dir = toBackup ? this->backupDir : this->recordDir;
    path = dir + num;

    if (AEFile::FileExist(path) != 0)
        AEFile::FileDelete(path);
    AEFile::OpenWrite(path, &fd);
    ((AEFile *)(n))->Write(buf, fd);
    AEFile::Close(fd);
    return n;
}


// RecordHandler::readMission(unsigned int fd) -> Mission* (in r0).
void * RecordHandler::readMission(unsigned int fd) {
    void *mission = 0;

    int type = 0;
    AEFile_ReadInt(&type, fd);
    if (type != -1) {
        String clientName, targetName, targetStation, targetSystem;
        clientName.ctor();
        AEFile_ReadString(&clientName, fd, 1);
        targetName.ctor();
        AEFile_ReadString(&targetName, fd, 1);
        targetStation.ctor();
        AEFile_ReadString(&targetStation, fd, 1);
        targetSystem.ctor();
        AEFile_ReadString(&targetSystem, fd, 1);

        bool isEmpty = false;
        AEFile_ReadBool(&isEmpty, fd);

        unsigned imgCount = 0;
        AEFile_ReadInt(&imgCount, fd);
        int *img = 0;
        if ((int)imgCount >= 1) {
            img = new int[imgCount];
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
        void *agent = (hasAgent < 1) ? 0 : this->readAgent(fd);

        if (!isEmpty) {
            String nameCopy;
            nameCopy.ctor_copy(&clientName, false);
            mission = new Mission(type, nameCopy, img, clientRace, reward, targetStationIdx, difficulty);
        } else {
            mission = new Mission(type, reward, targetStationIdx);
        }
        ((Mission *)(mission))->setCosts(costs);
        ((Mission *)(mission))->setBonus(bonus);
        ((Mission *)(mission))->setProductionGoods(prodIdx, prodAmt);
        ((Mission *)(mission))->setStatusValue(statusValue);
        ((Mission *)(mission))->setVisible(visible);
        ((Mission *)(mission))->setAgent((Agent *)agent);

        String tgtNameCopy;
        tgtNameCopy.ctor_copy(&targetName, false);
        ((Mission *)(mission))->setTargetName(tgtNameCopy);

        if (agent != 0) {
            ((Agent *)(agent))->setMission((Mission *)mission);
        }

    }

    return mission;
}

__attribute__((visibility("hidden"))) extern unsigned char *g_SO_settings; // settings struct
__attribute__((visibility("hidden"))) extern String **g_SO_playerName; // *->String*
__attribute__((visibility("hidden"))) extern const char g_SO_defName[];
__attribute__((visibility("hidden"))) extern int *g_SO_extraInt;     // *->int
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_flag1; // byte
__attribute__((visibility("hidden"))) extern unsigned char *g_SO_flag2; // byte

// RecordHandler::saveOptions()
void RecordHandler::saveOptions() {
    String &path = this->optionsPath;
    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);

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

    String *name = *g_SO_playerName;
    if (name == 0) {
        String def(g_SO_defName);
        AEFile_WriteString(&def, fd, 0);
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
    this->addHashToOptions();

    return;
}


// RecordHandler::readAgent(unsigned int fd) -> Agent* (in r0).
void * RecordHandler::readAgent(unsigned int fd) {
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
        img = new int[imgCount];
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

    String missionStr, name, stationName, systemName, strE, strF;
    missionStr.ctor();
    name.ctor();
    stationName.ctor();
    systemName.ctor();
    strE.ctor();
    strF.ctor();
    AEFile_ReadString(&missionStr, fd, 1);
    AEFile_ReadString(&name, fd, 1);
    AEFile_ReadString(&stationName, fd, 1);
    AEFile_ReadString(&systemName, fd, 1);
    AEFile_ReadString(&strE, fd, 1);
    AEFile_ReadString(&strF, fd, 1);

    int hasMission = 0;
    AEFile_ReadInt(&hasMission, fd);
    void *mission = (hasMission < 1) ? 0 : this->readMission(fd);

    String nameCopy;
    nameCopy.ctor_copy(&name, false);
    Agent *agent = new Agent(idx, nameCopy, station, system, race, male, sellSys, sellBp, sellMod, sellItemIdx);

    ((Agent *)(agent))->setCosts(costs);
    ((Agent *)(agent))->setEvent(event);
    ((Agent *)(agent))->setOffer(offer);
    ((Agent *)(agent))->setSellItemData(sellItemPrice, sellItemQty, sellItemIdx);

    if (strE.size() != 0) {
        String *s = new String();
        s->ctor_copy(&strE, false);
        agent->wingman1 = s;
    }
    if (strF.size() != 0) {
        String *s = new String();
        s->ctor_copy(&strF, false);
        agent->wingman2 = s;
    }
    agent->wingmanCount = wingmen;

    Array<String*> *arr = new Array<String*>();
    arr->resize(wingmen);
    for (int i = 0; i < (int)wingmen; i++) {
        String *s = new String();
        String *src = (i == 0) ? &strE : &strF;
        s->ctor_copy(src, false);
        (*arr)[i] = s;
    }
    ((Agent *)(agent))->setWingmanFriendNames(arr);
    ((Agent *)(agent))->giveRewardAtNextChat(hasReward);
    ((Agent *)(agent))->setOfferAccepted(accepted);
    ((Agent *)(agent))->setImageParts(img);

    String tmp;
    tmp.ctor_copy(&missionStr, false);
    ((Agent *)(agent))->setMissionString(tmp);
    tmp.ctor_copy(&stationName, false);
    ((Agent *)(agent))->setStationName(tmp);
    tmp.ctor_copy(&systemName, false);
    ((Agent *)(agent))->setSystemName(tmp);
    ((Agent *)(agent))->setMission((Mission *)mission);

    agent->field_0x24 = raw24;
    agent->field_0x25 = raw25;

    return agent;
}


// RecordHandler::writeWanted(Wanted*, unsigned int fd)
void RecordHandler::writeWanted(Wanted *w, unsigned int fd) {
    (void)this;
    AEFile_WriteBool(w->isActive(), fd);
    AEFile_WriteBool(w->isTerminated(), fd);
    AEFile_WriteInt(w->getCurrentLocation(), fd);
    AEFile_WriteInt(w->getTravelsTo(), fd);
    AEFile_WriteInt(w->getLastSeen(), fd);

    String name;
    name = w->getName();
    AEFile_WriteString(&name, fd, 1);

    AEFile_WriteInt(w->getIndex(), fd);
    AEFile_WriteInt(w->getBoard(), fd);
    AEFile_WriteInt(w->getRace(), fd);
    AEFile_WriteBool(w->isMale(), fd);
    AEFile_WriteInt(w->getShip(), fd);
    AEFile_WriteInt(w->getWeapon(), fd);
    AEFile_WriteInt(w->getHitpoints(), fd);
    AEFile_WriteInt(w->getLoot(), fd);
    AEFile_WriteInt(w->getLootAmount(), fd);
    AEFile_WriteInt(w->getReward(), fd);
    AEFile_WriteInt(w->getRequiredBounties(), fd);
    AEFile_WriteInt(w->getRequiredMission(), fd);
    AEFile_WriteInt(w->getNumWingmen(), fd);

    for (int i = 0; i != 5; i++) {
        int *parts = w->getImageParts();
        AEFile_WriteInt(parts[i], fd);
    }

    return;
}

// RecordHandler::recordStoreWritePreview(GameRecord*, int)
int RecordHandler::recordStoreWritePreview(GameRecord *rec, int slot) {
    if (rec == 0)
        return 0;

    String path;
    String num;
    unsigned int fd;

    num.ctor_int(slot);
    path = this->backupDir + num;

    if (AEFile::FileExist(path) != 0)
        AEFile::FileDelete(path);
    AEFile::OpenWrite(path, &fd);

    // RAWREAD: GameRecord payload is a positional save-image blob (no per-field members in GameRecord.h).
    AEFile_Write_i64(*(long long *)((char *)rec + 0x10), fd);
    AEFile_Write_i32(*(int *)((char *)rec + 0x8), fd);

    num = ((Station *)(&num))->getName();
    AEFile_Write_str(&num, fd, true);

    AEFile_Write_str((char *)rec + 0x188, fd, true);
    AEFile_Write_i32(*(int *)((char *)rec + 0x40), fd);
    AEFile_Write_i32(*(int *)((char *)rec + 0x20), fd);
    AEFile_Write_f32(*(int *)((char *)rec + 0x11c), fd);
    AEFile_Write_i32(((Ship *)(*(Ship **)((char *)rec + 0x130)))->getIndex(), fd);   // RAWREAD: GameRecord+0x130 (blob; Ship* slot not modeled in GameRecord.h)
    AEFile::Close(fd);
    return 1;
}

// The remaining game-state serialization (ship, station, standings, items, missions, agents).
// A long straight-line writer over the live object graph; delegated to a helper to keep this
// translation tractable while preserving the open/write-prefix/close/hash control flow.

// RecordHandler::recordStoreWrite(int slot)
void RecordHandler::recordStoreWrite(int slot) {
    String num, path;
    num.ctor_int(slot);
    path = this->recordDir + num;

    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);

    // Visited-systems bitmap (0x87 entries).
    long visited = (long)gGalaxy->getVisited();
    AEFile_WriteInt(0x87, fd);
    for (unsigned i = 0; i < 0x87; i++) {
        AEFile_WriteBool(*(bool *)(visited + i), fd);
    }

    AEFile_WriteInt(gStatus->getCredits(), fd);
    AEFile_WriteInt(gStatus->getRating(), fd);
    AEFile_WriteLong(gStatus->getPlayingTime(), fd);
    AEFile_WriteInt(gStatus->getKills(), fd);
    AEFile_WriteInt(gStatus->getMissionCount(), fd);
    AEFile_WriteInt(gStatus->getLevel(), fd);
    AEFile_WriteInt(gStatus->getLastXP(), fd);
    AEFile_WriteInt(gStatus->getGoodsProduced(), fd);
    AEFile_WriteInt(gStatus->getStationsVisited(), fd);
    AEFile_WriteInt(gStatus->getCurrentCampaignMission(), fd);
    this->writeMission(gStatus->getFreelanceMission(), fd);
    this->writeMission(gStatus->getCampaignMissionPtr(), fd);
    AEFile_WriteInt(gStatus->getJumpgateUsed(), fd);
    AEFile_WriteInt(gStatus->getCapturedCrates(), fd);
    AEFile_WriteInt(gStatus->getBoughtEquipment(), fd);
    AEFile_WriteInt(gStatus->getPirateKills(), fd);
    AEFile_WriteInt(gStatus->field_80, fd);

    // Remaining object-graph serialization.
    this->recordStoreWrite_body(fd);

    AEFile::Close(fd);
    this->addHash(slot);

    return;
}

// ---- recordStoreWrite body (game object-graph serialization) ----
// Continuation of recordStoreWrite(): writes the live object graph (boolean bitmaps, item
// and ship inventories for the player/wingman/stations, standings, blueprints, pending
// products, wingmen list, agents, option flags, wanteds, bounties and the ship-mod tables)
// to the already-open file `fd`. The caller handles open/prefix, Close and addHash.
__attribute__((visibility("hidden"))) extern void  *g_RSW_optFlags;   // option-flag block base
__attribute__((visibility("hidden"))) extern void  *g_RSW_uiFlags;    // secondary flag block base
__attribute__((visibility("hidden"))) extern int    g_RSW_modVersion; // ship-mod table version tag

// Item / Achievements / Standing live in out-of-batch headers whose full layout is not
// pulled in here; their accessors are reached through extern "C" shims (codebase style).

// Helper: write a length-prefixed bool[] backed by an Array<bool>.
static void RSW_writeBoolArray(Array<bool> *arr, unsigned int fd) {
    AEFile_WriteInt(arr->size(), fd);
    for (unsigned i = 0; i < arr->size(); i++) {
        AEFile_WriteBool((*arr)[i], fd);
    }
}

// Helper: write a length-prefixed int[] backed by an Array<int>.
static void RSW_writeIntArray(Array<int> *arr, unsigned int fd) {
    AEFile_WriteInt(arr->size(), fd);
    for (unsigned i = 0; i < arr->size(); i++) {
        AEFile_WriteInt((*arr)[i], fd);
    }
}

// Helper: write a ship's equipment list (index, amount, unsaleable) or 0 when absent.
static void RSW_writeEquipment(Ship *ship, unsigned int fd) {
    Array<Item*> *eq = ((Ship *)ship)->getEquipment();
    if (eq == 0) { AEFile_WriteInt(0, fd); return; }
    AEFile_WriteInt(eq->size(), fd);
    for (unsigned i = 0; i < eq->size(); i++) {
        Item *it = (*eq)[i];
        if (it == 0) { AEFile_WriteInt(-1, fd); continue; }
        AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
        AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
        AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
    }
}

// Helper: write a ship's cargo list (index, amount, single price, unsaleable) or 0.
static void RSW_writeCargo(Ship *ship, unsigned int fd) {
    Array<Item*> *cg = ((Ship *)ship)->getCargo();
    if (cg == 0) { AEFile_WriteInt(0, fd); return; }
    AEFile_WriteInt(cg->size(), fd);
    for (unsigned i = 0; i < cg->size(); i++) {
        Item *it = (*cg)[i];
        AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
        AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
        AEFile_WriteInt(((Item *)(it))->getSinglePrice(), fd);
        AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
    }
}

// Helper: write a ship's installed-mod list or 0 when the ship has none.
static void RSW_writeMods(Ship *ship, unsigned int fd) {
    if (ship == 0) { AEFile_WriteInt(0, fd); return; }
    Array<int> *mods = ((Ship *)ship)->getMods();
    if (mods == 0) { AEFile_WriteInt(0, fd); return; }
    AEFile_WriteInt(mods->size(), fd);
    for (unsigned i = 0; i < mods->size(); i++) {
        AEFile_WriteInt((*mods)[i], fd);
    }
}

void RecordHandler::recordStoreWrite_body(unsigned int fd) {
    Status *status = gStatus;
    char *flags = (char *)g_RSW_optFlags;   // option/progress flag block
    char *ui = (char *)g_RSW_uiFlags;       // secondary flag block

    // Standings unlock bitmap and the various progress / discovery bitmaps.
    AEFile_WriteInt(status->field_7c, fd);
    AEFile_WriteInt(status->field_84, fd);
    AEFile_WriteInt(*(int *)((char *)status + 0x88), fd);   // RAWREAD: Status+0x88 (no named member; gap between field_84 and field_8c)
    RSW_writeBoolArray(status->field_94, fd);
    RSW_writeBoolArray(status->field_98, fd);
    AEFile_WriteInt(status->field_9c, fd);
    AEFile_WriteInt(status->field_a0, fd);
    AEFile_WriteInt(status->field_a4, fd);
    AEFile_WriteInt(status->field_a8, fd);
    RSW_writeBoolArray(status->field_ac, fd);
    AEFile_WriteInt(status->field_b0, fd);
    RSW_writeBoolArray(status->field_b4, fd);
    AEFile_WriteInt(status->field_b8, fd);
    AEFile_WriteLong(*(long long *)((char *)status + 0xc0), fd);   // RAWREAD: Status+0xc0 (i64 spans field_c0/field_c4 int32 members)
    for (int off = 0xc8; off <= 0xec; off += 4) {
        AEFile_WriteInt(*(int *)((char *)status + off), fd);       // RAWREAD: Status+0xc8..0xec (loop over field_c8..field_ec)
    }

    // Achievement medals (0x2d entries).
    int medals = (int)(intptr_t)gAchievements->getMedals();
    AEFile_WriteInt(0x2d, fd);
    for (unsigned i = 0; i < 0x2d; i++) {
        AEFile_WriteInt(*(int *)(medals + i * 4), fd);
    }

    // Player ship: index, race, equipment, cargo.
    Ship *ship = status->getShip();
    AEFile_WriteInt(((Ship *)ship)->getIndex(), fd);
    AEFile_WriteInt(((Ship *)ship)->getRace(), fd);
    RSW_writeEquipment(ship, fd);
    RSW_writeCargo(ship, fd);

    // Station stack (3 fixed slots + current station): items, ships, agents, hostility flag.
    Array<Station*> *stack = status->getStationStack();
    AEFile_WriteInt(3, fd);
    for (unsigned i = 0; i < stack->size() + 1; i++) {
        Station *cur = (i == stack->size()) ? (Station *)status->getStation()
                                     : (*stack)[i];
        if (cur == 0) { AEFile_WriteInt(-1, fd); continue; }
        AEFile_WriteInt(((Station *)cur)->getIndex(), fd);

        Array<Item*> *items = (Array<Item*> *)((Station *)cur)->getItems();
        if (items == 0) { AEFile_WriteInt(0, fd); }
        else {
            AEFile_WriteInt(items->size(), fd);
            for (unsigned j = 0; j < items->size(); j++) {
                Item *it = (*items)[j];
                AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
                AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
                AEFile_WriteInt(((Item *)(it))->getSinglePrice(), fd);
                AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
            }
        }
        Array<Ship*> *ships = (Array<Ship*> *)((Station *)cur)->getShips();
        if (ships == 0) { AEFile_WriteInt(0, fd); }
        else {
            AEFile_WriteInt(ships->size(), fd);
            for (unsigned j = 0; j < ships->size(); j++) {
                Ship *s = (*ships)[j];
                AEFile_WriteInt(((Ship *)s)->getIndex(), fd);
                AEFile_WriteInt(((Ship *)s)->getRace(), fd);
            }
        }
        Array<Agent*> *agents = (Array<Agent*> *)((Station *)cur)->getAgents();
        if (agents == 0) { AEFile_WriteInt(0, fd); }
        else {
            AEFile_WriteInt(agents->size(), fd);
            for (unsigned j = 0; j < agents->size(); j++) {
                this->writeAgent((*agents)[j], fd);
            }
        }
        AEFile_WriteBool(((Station *)cur)->hasAttackedFriends(), fd);
    }

    // Standings (2 races).
    int standings = (int)(intptr_t)((Standing *)(status->getStanding()))->getStandings();
    AEFile_WriteInt(2, fd);
    for (unsigned i = 0; i < 2; i++) {
        AEFile_WriteInt(*(int *)(standings + i * 4), fd);
    }

    // Blueprints: per blueprint the ingredient list, then its scalar fields and name.
    unsigned int *bps = (unsigned int *)status->getBluePrints();
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
    unsigned int *pending = (unsigned int *)status->getPendingProducts();
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
                // RAWREAD: original-layout PendingProduct offsets; the cleaned struct's String pushes
                // the scalar members to 0x18+, so these +0xc..+0x14 reads don't map to named members.
                AEFile_WriteInt(*(int *)(pp + 0x14), fd);
                AEFile_WriteInt(*(int *)(pp + 0x10), fd);
                AEFile_WriteInt(*(int *)(pp + 0xc), fd);
                AEFile_WriteString(*(char **)(pending[1] + i * 4), fd, true);
            }
        }
    }

    // Wingmen list (String[]) + its trailing scalar fields, or -1 when absent.
    unsigned int *wingmen = *(unsigned int **)((char *)status + 0x24);  // RAWREAD: Status+0x24 (wingmen field is int32 reinterpreted as Array<String*>*)
    if (wingmen == 0) { AEFile_WriteInt(-1, fd); }
    else {
        AEFile_WriteInt(*wingmen, fd);
        for (unsigned i = 0; i < *wingmen; i++) {
            AEFile_WriteString(*(char **)(wingmen[1] + i * 4), fd, true);
        }
        AEFile_WriteInt(*(int *)((char *)status + 0x2c), fd);     // RAWREAD: Status+0x2c (no named member; between field_0x28 and field_0x30)
        AEFile_WriteInt(status->field_0x30, fd);
        AEFile_WriteInt(5, fd);
        for (unsigned i = 0; i < 5; i++) {
            AEFile_WriteInt(*(int *)(*(int *)((char *)status + 0x28) + i * 4), fd);  // RAWREAD: Status+0x28 (field_0x28 int32 reinterpreted as int[] base)
        }
    }

    AEFile_WriteInt(status->passengers, fd);
    RSW_writeBoolArray(status->systemVisibilities, fd);
    RSW_writeIntArray(status->field_0x40, fd);
    RSW_writeIntArray(status->field_0x3c, fd);
    RSW_writeIntArray(status->field_0x48, fd);
    RSW_writeIntArray(status->field_0x44, fd);
    RSW_writeBoolArray(status->field_4c, fd);

    // Agents.
    Array<Agent*> *agents = status->agents;
    AEFile_WriteInt(agents->size(), fd);
    for (unsigned i = 0; i < agents->size(); i++) {
        this->writeAgent((*agents)[i], fd);
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

    AEFile_WriteLong(*(long long *)((char *)status + 0x100), fd);   // RAWREAD: Status+0x100 (i64 spans field_0x100/field_0x104 int32 members)
    AEFile_WriteBool(*(bool *)(flags + 0x25), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x26), fd);

    // Optional wingman ship (index, race, equipment, cargo) or 0.
    Ship *wingShip = *(Ship **)((char *)status + 0x8c);   // RAWREAD: Status+0x8c (field_8c int32 reinterpreted as Ship*)
    if (wingShip == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(1, fd);
        AEFile_WriteInt(((Ship *)wingShip)->getIndex(), fd);
        AEFile_WriteInt(((Ship *)wingShip)->getRace(), fd);
        RSW_writeEquipment(wingShip, fd);
        RSW_writeCargo(wingShip, fd);
    }

    RSW_writeIntArray(status->field_90, fd);
    AEFile_WriteInt(status->field_10c, fd);
    AEFile_WriteBool(status->field_110, fd);
    AEFile_WriteInt(status->field_114, fd);
    AEFile_WriteBool(status->field_0x111, fd);

    // Current station's items / ships (re-read off the current station).
    Station *station = (Station *)status->getStation();
    Array<Item*> *sItems = (Array<Item*> *)((Station *)station)->getItems();
    if (sItems == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(sItems->size(), fd);
        for (unsigned i = 0; i < sItems->size(); i++) {
            Item *it = (*sItems)[i];
            AEFile_WriteInt(((Item *)(it))->getIndex(), fd);
            AEFile_WriteInt(((Item *)(it))->getAmount(), fd);
            AEFile_WriteInt(((Item *)(it))->getSinglePrice(), fd);
            AEFile_WriteBool(((Item *)(it))->isUnsaleable(), fd);
        }
    }
    Array<Ship*> *sShips = (Array<Ship*> *)((Station *)station)->getShips();
    if (sShips == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(sShips->size(), fd);
        for (unsigned i = 0; i < sShips->size(); i++) {
            Ship *s = (*sShips)[i];
            AEFile_WriteInt(((Ship *)s)->getIndex(), fd);
            AEFile_WriteInt(((Ship *)s)->getRace(), fd);
        }
    }

    AEFile_WriteBool(*(bool *)(flags + 0x27), fd);
    AEFile_WriteBool(*(bool *)(ui + 0x35), fd);
    AEFile_WriteBool(*(bool *)(ui + 0x36), fd);
    RSW_writeBoolArray(status->field_54, fd);

    // Ship-mod table version tag + mod lists for player, wingman and every station ship.
    AEFile_WriteInt(g_RSW_modVersion, fd);
    RSW_writeMods(ship, fd);
    RSW_writeMods(wingShip, fd);
    if (sShips == 0) { AEFile_WriteInt(0, fd); }
    else {
        AEFile_WriteInt(sShips->size(), fd);
        for (unsigned i = 0; i < sShips->size(); i++) {
            RSW_writeMods((*sShips)[i], fd);
        }
    }
    for (unsigned i = 0; i < stack->size() + 1; i++) {
        Station *cur = (i == stack->size()) ? (Station *)status->getStation()
                                     : (*stack)[i];
        if (cur == 0) continue;
        Array<Ship*> *cShips = (Array<Ship*> *)((Station *)cur)->getShips();
        if (cShips == 0) { AEFile_WriteInt(0, fd); continue; }
        AEFile_WriteInt(cShips->size(), fd);
        for (unsigned j = 0; j < cShips->size(); j++) {
            RSW_writeMods((*cShips)[j], fd);
        }
    }

    // Wanted board, collected bounties, and the remaining one-shot UI/progress flags.
    Array<Wanted*> *wanted = status->wanted;   // engine Array<Wanted*> payload
    AEFile_WriteInt(wanted->size(), fd);
    for (unsigned i = 0; i < wanted->size(); i++) {
        this->writeWanted((*wanted)[i], fd);
    }
    for (unsigned i = 0; i < 4; i++) {
        AEFile_WriteInt(status->getCollectedBounties(i), fd);
    }
    AEFile_WriteBool(*(bool *)(ui + 0x37), fd);
    AEFile_WriteInt(status->field_178, fd);
    AEFile_WriteBool(*(bool *)(flags + 0x28), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x29), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2c), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2a), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2b), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2e), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2f), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x30), fd);
    RSW_writeBoolArray(status->field_58, fd);
    AEFile_WriteBool(*(bool *)(flags + 0x31), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x2d), fd);
    AEFile_WriteBool(*(bool *)(flags + 0x32), fd);
    AEFile_WriteInt(status->field_118, fd);
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
extern "C" void *RH_str_make(void *src);                 // operator new + String copy-ctor
// Item-definition tables and the ship-definition table (PC-relative singletons).
__attribute__((visibility("hidden"))) extern void **g_RSR_itemDefs;
__attribute__((visibility("hidden"))) extern void **g_RSR_shipDefs;
__attribute__((visibility("hidden"))) extern int    g_RSR_modVersion;

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
    // RAWREAD: rec is the GameRecord save-image; its payload is a positional blob with no
    // per-field members in GameRecord.h, so all `rec + 0xNN` writes target the opaque payload.
    char *rec = (char *)recv;
    void **shipDefs = *(void ***)g_RSR_shipDefs;

    AEFile_ReadInt(rec + 0x74, fd);
    AEFile_ReadInt(rec + 0x78, fd);
    AEFile_ReadInt(rec + 0x7c, fd);
    AEFile_ReadInt(rec + 0x80, fd);

    // bitmap at rec+0x84
    Array<bool> *flags84 = new Array<bool>();
    *(void **)(rec + 0x84) = flags84;
    int n84 = 0; AEFile_ReadInt(&n84, fd); flags84->resize(n84);
    for (int i = 0; i < n84; i++) { bool v = false; AEFile_ReadBool(&v, fd); (*flags84)[i] = v; }

    AEFile_ReadInt(rec + 0x88, fd);

    Array<bool> *flags8c = new Array<bool>();
    *(void **)(rec + 0x8c) = flags8c;
    int n8c = 0; AEFile_ReadInt(&n8c, fd); flags8c->resize(n8c);
    for (int i = 0; i < n8c; i++) { bool v = false; AEFile_ReadBool(&v, fd); (*flags8c)[i] = v; }

    AEFile_ReadInt(rec + 0x90, fd);
    AEFile_ReadLong(rec + 0x98, fd);
    for (int off = 0xa0; off <= 0xc4; off += 4) AEFile_ReadInt(rec + off, fd);

    // int[] at rec+0x60 (length at rec+100/0x64)
    AEFile_ReadInt(rec + 0x64, fd);
    unsigned len60 = *(unsigned *)(rec + 0x64);
    int *buf60 = new int[len60];
    *(void **)(rec + 0x60) = buf60;
    for (unsigned i = 0; i < len60; i++) { AEFile_ReadInt(buf60 + i, fd); }

    // Player ship: definition index, race, equipment, cargo.
    int shipIdx = 0; AEFile_ReadInt(&shipIdx, fd);
    void *pship = ((Ship *)((int)(intptr_t)shipDefs[shipIdx]))->makeShip(-1);
    *(void **)(rec + 0x130) = pship;
    int race = 0; AEFile_ReadInt(&race, fd); ((Ship *)(pship))->setRace(race);

    int eqN = 0; AEFile_ReadInt(&eqN, fd);
    if (eqN > 0) {
        Array<Item*> *items = new Array<Item*>(); items->resize(eqN);
        for (int i = 0; i < eqN; i++)
            (*items)[i] = (Item *)RSR_readItem(fd, false);
        ((Ship *)(pship))->replaceEquipment(items);
    }
    int cgN = 0; AEFile_ReadInt(&cgN, fd);
    if (cgN > 0) {
        Array<Item*> *items = new Array<Item*>(); items->resize(cgN);
        for (int i = 0; i < cgN; i++)
            (*items)[i] = (Item *)RSR_readItem(fd, true);
        ((Ship *)(pship))->replaceCargo(items);
    }

    // Station stack (3 slots + current). For each present station: items, ships, agents, flag.
    Array<Station*> *stationArr = new Array<Station*>();
    int stN = 0; AEFile_ReadInt(&stN, fd); stationArr->resize(stN);
    unsigned stCount = stationArr->size();
    for (unsigned k = 0; k < stCount + 1; k++) {
        int sIdx = 0; AEFile_ReadInt(&sIdx, fd);
        void *st = 0;
        if (sIdx != -1) {
            st = Galaxy_getStationByIndex(sIdx);
            int iN = 0; AEFile_ReadInt(&iN, fd);
            if (iN > 0) {
                Array<Item*> *items = new Array<Item*>(); items->resize(iN);
                for (int i = 0; i < iN; i++)
                    (*items)[i] = (Item *)RSR_readItem(fd, true);
                ((Station *)(st))->setItems(items, false);
            }
            int shN = 0; AEFile_ReadInt(&shN, fd);
            if (shN > 0) {
                Array<Ship*> *ships = new Array<Ship*>(); ships->resize(shN);
                for (int i = 0; i < shN; i++) {
                    int si = 0; AEFile_ReadInt(&si, fd);
                    void *sh = ((Ship *)((int)(intptr_t)shipDefs[si]))->makeShip(-1);
                    (*ships)[i] = (Ship *)sh;
                    int r = 0; AEFile_ReadInt(&r, fd); ((Ship *)(sh))->setRace(r);
                }
                ((Station *)(st))->setShipsArr(ships);
            }
            int agN = 0; AEFile_ReadInt(&agN, fd);
            if (agN > 0) {
                Array<Agent*> *agents = new Array<Agent*>(); agents->resize(agN);
                for (int i = 0; i < agN; i++)
                    (*agents)[i] = (Agent *)this->readAgent(fd);
                ((Station *)(st))->setAgents(agents);
            }
            bool atk = false; AEFile_Read_bool(&atk, fd, 0);
            ((Station *)(st))->setAttackedFriends(atk);
        }
        if (k == stationArr->size()) *(void **)(rec + 0x138) = st;
        else (*stationArr)[k] = (Station *)st;
    }
    *(void **)(rec + 0x5c) = stationArr;

    // Standings (count then values).
    int sdN = 0; AEFile_ReadInt(&sdN, fd);
    int *standings = new int[sdN];
    for (int i = 0; i < sdN; i++) AEFile_ReadInt(standings + i, fd);
    void *standing = Standing_new();
    *(void **)(rec + 0x13c) = standing;
    Standing_setStandingsArr(standing, standings);

    // Blueprints.
    Array<void*> *bpArr = new Array<void*>();   // generic Array<T*> container reuse
    int bpN = 0; AEFile_ReadInt(&bpN, fd); bpArr->resize(bpN);
    for (unsigned i = 0; i < bpArr->size(); i++) {
        unsigned int *liveBps = (unsigned int *)gStatus->getBluePrints();
        int liveIdx = ((BluePrint *)(*(void **)(liveBps[1] + i * 4)))->getIndexOf();
        int *bp = (int *)BluePrint::make(liveIdx);
        (*bpArr)[i] = bp;
        unsigned int *ingredients = (unsigned int *)bp[0];
        int byteoff = 0;
        for (unsigned j = 0; j < *ingredients; j++) { AEFile_ReadInt((void *)(intptr_t)(ingredients[1] + byteoff), fd); byteoff += 4; }
        AEFile_ReadInt(bp + 1, fd);
        AEFile_ReadByte((bool *)(bp + 2), fd);
        AEFile_ReadInt(bp + 3, fd);
        AEFile_ReadInt(bp + 4, fd);
        String tmp; tmp.ctor(); AEFile_ReadString(&tmp, fd, 1);
        *(String *)(bp + 5) = tmp;
    }
    *(void **)(rec + 0x140) = bpArr;

    // Pending products (-1 / 0 == none).
    int ppN = 0; AEFile_ReadInt(&ppN, fd);
    if (ppN < 1) { *(void **)(rec + 0x144) = 0; }
    else {
        Array<PendingProduct*> *ppArr = new Array<PendingProduct*>(); ppArr->resize(ppN);
        for (unsigned i = 0; i < ppArr->size(); i++) {
            int a = 0, c = 0, d = 0; AEFile_ReadInt(&a, fd); AEFile_ReadInt(&c, fd); AEFile_ReadInt(&d, fd);
            String nm; nm.ctor(); AEFile_ReadString(&nm, fd, 1);
            void *nameCopy = RH_str_make(&nm);
            void *pp = PendingProduct::make(a, (const String *)nameCopy, d, c);
            (*ppArr)[i] = (PendingProduct *)pp;
        }
        *(void **)(rec + 0x144) = ppArr;
    }

    // Wingmen string list (+ trailing scalar/int[] fields) or none.
    int wmN = 0; AEFile_ReadInt(&wmN, fd);
    if (wmN < 1) { *(void **)(rec + 0x14c) = 0; }
    else {
        Array<String*> *strArr = new Array<String*>(); strArr->resize(wmN);
        for (int i = 0; i < wmN; i++) {
            String nm; nm.ctor(); AEFile_ReadString(&nm, fd, 1);
            (*strArr)[i] = (String *)RH_str_make(&nm);
        }
        *(void **)(rec + 0x14c) = strArr;
        AEFile_ReadInt(rec + 0x150, fd);
        AEFile_ReadInt(rec + 0x154, fd);
        int cnt = 0; AEFile_ReadInt(&cnt, fd);
        int *buf158 = new int[cnt];
        *(void **)(rec + 0x158) = buf158;
        for (int i = 0; i < cnt; i++) AEFile_ReadInt(buf158 + i, fd);
    }

    AEFile_ReadInt(rec + 0x15c, fd);

    // bool[] rec+0x160
    int b160 = 0; AEFile_ReadInt(&b160, fd);
    Array<bool> *arr160 = new Array<bool>(); *(void **)(rec + 0x160) = arr160;
    arr160->resize(b160);
    for (unsigned i = 0; i < arr160->size(); i++) { bool v = false; AEFile_ReadBool(&v, fd); (*arr160)[i] = v; }

    // int[] rec+0x168, 0x164, 0x170, 0x16c
    const int intArrOffs[4] = {0x168, 0x164, 0x170, 0x16c};
    for (int a = 0; a < 4; a++) {
        int cnt = 0; AEFile_ReadInt(&cnt, fd);
        Array<int> *arr = new Array<int>(); *(void **)(rec + intArrOffs[a]) = arr;
        arr->resize(cnt);
        for (unsigned i = 0; i < arr->size(); i++) {
            AEFile_ReadInt(&(*arr)[i], fd);
        }
    }

    // bool[] rec+0x174
    int b174 = 0; AEFile_ReadInt(&b174, fd);
    Array<bool> *arr174 = new Array<bool>(); *(void **)(rec + 0x174) = arr174;
    arr174->resize(b174);
    for (unsigned i = 0; i < arr174->size(); i++) { bool v = false; AEFile_ReadBool(&v, fd); (*arr174)[i] = v; }

    // Agents.
    int agN = 0; AEFile_ReadInt(&agN, fd);
    Array<Agent*> *agents = new Array<Agent*>(); *(void **)(rec + 0x148) = agents; agents->resize(agN);
    for (unsigned i = 0; i < agents->size(); i++)
        (*agents)[i] = (Agent *)this->readAgent(fd);

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
            Array<Item*> *items = new Array<Item*>(); items->resize(eN);
            for (int i = 0; i < eN; i++)
                (*items)[i] = (Item *)RSR_readItem(fd, false);
            ((Ship *)(wship))->replaceEquipment(items);
        }
        int cN = 0; AEFile_ReadInt(&cN, fd);
        if (cN > 0) {
            Array<Item*> *items = new Array<Item*>(); items->resize(cN);
            for (int i = 0; i < cN; i++)
                (*items)[i] = (Item *)RSR_readItem(fd, true);
            ((Ship *)(wship))->replaceCargo(items);
        }
    }

    // int[] rec+0x70.
    Array<int> *arr70 = new Array<int>(); *(void **)(rec + 0x70) = arr70;
    int n70 = 0; AEFile_ReadInt(&n70, fd); arr70->resize(n70);
    for (int i = 0; i < n70; i++) { AEFile_ReadInt(&(*arr70)[i], fd); }

    AEFile_ReadInt(rec + 0xd0, fd);
    AEFile_ReadByte(rec + 0xd4, fd);
    AEFile_ReadInt(rec + 0xd8, fd);
    AEFile_ReadByte(rec + 0xdc, fd);

    // Station shop items (rec+0x180) and ships (rec+0x184).
    int siN = 0; AEFile_ReadInt(&siN, fd);
    if (siN > 0) {
        Array<Item*> *items = new Array<Item*>(); items->resize(siN);
        for (int i = 0; i < siN; i++)
            (*items)[i] = (Item *)RSR_readItem(fd, true);
        *(void **)(rec + 0x180) = items;
    }
    int ssN = 0; AEFile_ReadInt(&ssN, fd);
    Array<Ship*> *recShips = 0;
    if (ssN > 0) {
        Array<Ship*> *ships = new Array<Ship*>(); ships->resize(ssN);
        for (int i = 0; i < ssN; i++) {
            int si = 0; AEFile_ReadInt(&si, fd);
            void *sh = ((Ship *)((int)(intptr_t)shipDefs[si]))->makeShip(-1);
            (*ships)[i] = (Ship *)sh;
            int r = 0; AEFile_ReadInt(&r, fd); ((Ship *)(sh))->setRace(r);
        }
        *(void **)(rec + 0x184) = ships;
        recShips = ships;
    }

    AEFile_ReadByte(rec + 0x103, fd);
    AEFile_ReadByte(rec + 0x115, fd);
    AEFile_ReadByte(rec + 0x116, fd);

    // bool[] rec+0x178
    int b178 = 0; AEFile_ReadInt(&b178, fd);
    Array<bool> *arr178 = new Array<bool>(); *(void **)(rec + 0x178) = arr178;
    arr178->resize(b178);
    for (unsigned i = 0; i < arr178->size(); i++) { bool v = false; AEFile_ReadBool(&v, fd); (*arr178)[i] = v; }

    // Version-gated tail: ship-mod tables, wanteds, bounties and the final flag block.
    AEFile_ReadInt(rec + 0x1bc, fd);
    if (*(int *)(rec + 0x1bc) != g_RSR_modVersion) return;

    int m0 = 0; AEFile_ReadInt(&m0, fd);
    if (m0 > 0) { Array<int> *a = new Array<int>(); a->resize(m0);
        for (int i = 0; i < m0; i++) { AEFile_ReadInt(&(*a)[i], fd); }
        ((Ship *)(*(void **)(rec + 0x130)))->setMods(a); }
    int m1 = 0; AEFile_ReadInt(&m1, fd);
    if (m1 > 0) { Array<int> *a = new Array<int>(); a->resize(m1);
        for (int i = 0; i < m1; i++) { AEFile_ReadInt(&(*a)[i], fd); }
        ((Ship *)(*(void **)(rec + 0x134)))->setMods(a); }

    int stShipGroups = 0; AEFile_ReadInt(&stShipGroups, fd);
    for (int g = 0; g < stShipGroups; g++) {
        int mc = 0; AEFile_ReadInt(&mc, fd);
        if (mc > 0) { Array<int> *a = new Array<int>(); a->resize(mc);
            for (int i = 0; i < mc; i++) { AEFile_ReadInt(&(*a)[i], fd); }
            ((Ship *)((*recShips)[g]))->setMods(a); }
    }

    // Per-station ship mods (applied incrementally via addMod).
    Array<Station*> *stArr = (Array<Station*> *)*(void **)(rec + 0x5c);
    for (unsigned k = 0; k < stArr->size() + 1; k++) {
        void *cur = (k == stArr->size()) ? *(void **)(rec + 0x138)
                                              : (void *)(*stArr)[k];
        if (cur == 0) continue;
        int groups = 0; AEFile_ReadInt(&groups, fd);
        for (int i = 0; i < groups; i++) {
            int mc = 0; AEFile_ReadInt(&mc, fd);
            for (int j = 0; j < mc; j++) {
                int mod = 0; AEFile_ReadInt(&mod, fd);
                Array<Ship*> *cShips = (Array<Ship*> *)((Station *)cur)->getShips();
                ((Ship *)((*cShips)[i]))->addMod(mod);
            }
        }
    }

    // Wanteds.
    int wN = 0; AEFile_ReadInt(&wN, fd);
    if (wN > 0) {
        Array<Wanted*> *wArr = new Array<Wanted*>(); *(void **)(rec + 0x1b4) = wArr; wArr->resize(wN);
        for (int i = 0; i < wN; i++)
            (*wArr)[i] = (Wanted *)this->readWanted(fd);
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
    Array<bool> *arr17c = new Array<bool>(); *(void **)(rec + 0x17c) = arr17c;
    arr17c->resize(bX);
    for (unsigned i = 0; i < arr17c->size(); i++) { bool v = false; AEFile_ReadBool(&v, fd); (*arr17c)[i] = v; }

    AEFile_ReadByte(rec + 0x119, fd);
    AEFile_ReadByte(rec + 0x109, fd);
    AEFile_ReadByte(rec + 0x11a, fd);
    AEFile_ReadInt(rec + 0xe0, fd);
    for (int off = 0x10d; off <= 0x114; off++) AEFile_ReadByte(rec + off, fd);
}

// RecordHandler::readOptionsFileAsByteArray(signed char**)
int RecordHandler::readOptionsFileAsByteArray(signed char **out) {
    String tmp;
    unsigned int fd;
    int sz;

    tmp.ctor_copy(&this->optionsPath, false);
    if (AEFile::FileExist(tmp) != 0) {
        AEFile::OpenRead(tmp, &fd);
        sz = AEFile::GetFileSize(fd);
        signed char *buf = new signed char[sz | (sz >> 31)];
        *out = buf;
        AEFile::Read(sz, buf, fd);
        AEFile::Close(fd);
    } else {
        sz = -1;
    }
    return sz;
}

// Deep GameRecord field read (ship, stations, standings, blueprints, pending products, agents,
// ships, wanteds, flags). A very long straight-line reader delegated to a helper so this
// translation preserves the path/hash/open/prefix/close flow while staying tractable.


// RecordHandler::recordStoreRead(int slot)
void * RecordHandler::recordStoreRead(int slot) {
    char *rec = 0;
    String num, path;
    num.ctor_int(slot);
    path = this->recordDir + num;

    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);
        int valid = RecordHandler_checkHash(fd);
        AEFile::Close(fd);
        if (valid != 0) {
            AEFile::OpenRead(path, &fd);
            // RAWREAD: rec is the GameRecord save-image; its payload is a positional blob with no
            // per-field members in GameRecord.h, so all `rec + 0xNN` accesses target the opaque payload.
            rec = (char *)new GameRecord();

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
            *(void **)(rec + 0x54) = this->readMission(fd);
            *(void **)(rec + 0x58) = this->readMission(fd);
            AEFile_ReadInt(rec + 0x30, fd);
            AEFile_ReadInt(rec + 0x34, fd);
            AEFile_ReadInt(rec + 0x38, fd);
            AEFile_ReadInt(rec + 0x2c, fd);
            AEFile_ReadInt(rec + 0x44, fd);
            AEFile_ReadInt(rec + 0x48, fd);
            AEFile_ReadInt(rec + 0x4c, fd);
            AEFile_ReadInt(rec + 0x50, fd);

            // Two boolean arrays (mission flags / equipment flags).
            Array<bool> *flags0 = new Array<bool>();
            *(void **)(rec + 0x68) = flags0;
            unsigned n0 = 0;
            AEFile_ReadInt(&n0, fd);
            flags0->resize(n0);
            for (int i = 0; i < (int)n0; i++) {
                bool v = false; AEFile_ReadBool(&v, fd); (*flags0)[i] = v;
            }
            Array<bool> *flags1 = new Array<bool>();
            *(void **)(rec + 0x6c) = flags1;
            unsigned n1 = 0;
            AEFile_ReadInt(&n1, fd);
            flags1->resize(n1);
            for (int i = 0; i < (int)n1; i++) {
                bool v = false; AEFile_ReadBool(&v, fd); (*flags1)[i] = v;
            }

            // Remaining object-graph read.
            this->recordStoreRead_body(rec, fd);

            AEFile::Close(fd);
        }
    }

    return rec;
}

__attribute__((visibility("hidden"))) extern const unsigned char RH_ch_salt[]; // (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ch_key;        // (16-byte key)

// RecordHandler::checkHash(unsigned int fd) -> bool (1=valid, 0=invalid).
int RecordHandler_checkHash(unsigned int fd)
{
    int result = 0;

    int sz = AEFile::GetFileSize(fd);
    if (-1 < sz) {
        unsigned char *buf = new unsigned char[(unsigned)sz];
        AEFile::Read((unsigned)sz, (signed char *)buf, fd);
        Array<unsigned char> *arr = new Array<unsigned char>();
        arr->insert(arr->end(), buf, buf + (unsigned)sz);
        delete[] buf;

        unsigned len = arr->size();
        if (0x21 < len) {
            unsigned char *md = new unsigned char[0x20];
            unsigned char *c = new unsigned char[0x70];
            SHA256_Init(c);
            unsigned char *data = arr->data();
            SHA256_Update(c, data, (int)len - 0x20);
            SHA256_Update(c, RH_ch_salt, 0x19);
            SHA256_Update(c, *RH_ch_key, 0x10);
            SHA256_Final(md, c);

            unsigned i = 0;
            // compare 32 computed bytes against the trailing 32 bytes of the record
            while (i < 0x20) {
                unsigned char want = data[(int)len + i - 0x20];
                if (md[i] != want) break;
                i++;
            }
            result = (i > 0x1f) ? 1 : 0;
            delete[] md;
            delete[] c;
        }
    }

    return result;
}

// SHA-256 salt blob (25 bytes) and the 16-byte signing key for the options file.
__attribute__((visibility("hidden"))) extern const unsigned char RH_aho_salt[];
__attribute__((visibility("hidden"))) extern unsigned char **RH_aho_key;

// RecordHandler::addHashToOptions()
void RecordHandler::addHashToOptions() {
    signed char *data = 0;
    int len = this->readOptionsFileAsByteArray(&data);
    if (len < 0)
        return;

    unsigned char *md = new unsigned char[0x20];
    unsigned char *c = new unsigned char[0x70];
    SHA256_Init(c);
    SHA256_Update(c, data, len);
    SHA256_Update(c, RH_aho_salt, 0x19);
    SHA256_Update(c, *RH_aho_key, 0x10);
    SHA256_Final(md, c);

    signed char *out = new signed char[len + 0x20];
    memcpy(out, data, len);
    unsigned long long *dst = (unsigned long long *)(out + len);
    unsigned long long *src = (unsigned long long *)md;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    this->writeByteArrayAsOptionsFile(out, len + 0x20);

    delete[] data;
    delete[] out;
    delete[] md;
    delete[] c;
}
