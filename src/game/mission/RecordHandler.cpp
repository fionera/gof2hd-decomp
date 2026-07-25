#include "game/mission/RecordHandler.h"
#include "game/mission/GameRecord.h"
#include "game/mission/PendingProduct.h"
#include "game/mission/Achievements.h"
#include "game/mission/BluePrint.h"
#include "engine/core/GameText.h"
#include "engine/render/PaintCanvas.h"
#include "game/ui/Layout.h"
#include "game/core/Globals.h"
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

#include <jni.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <cstdint>
#include <cstddef>

namespace {
#pragma pack(push, 1)
    struct OptionsBuffer {
        int32_t i32_0x00;
        int32_t i32_0x04;
        float volumeSfx;
        uint8_t byte_0x0c;
        uint8_t byte_0x0d;
        uint8_t byte_0x0e;
        uint8_t pad_0x0f;
        uint8_t byte_0x10;
        uint8_t byte_0x11;
        uint8_t pad_0x12[2];
        int32_t i32_0x14;
        int32_t i32_0x18;
        int32_t i32_0x1c;
        float volumeMusic;
        int32_t i32_0x24;
        int32_t i32_0x28;
        uint8_t pad_0x2c[6];
        uint8_t byte_0x32;
        uint8_t byte_0x33;
        uint8_t byte_0x34;
        uint8_t byte_0x35;
        uint8_t byte_0x36;
        uint8_t byte_0x37;
        uint8_t byte_0x38;
        uint8_t pad_0x39;
        uint8_t byte_0x3a;
        uint8_t byte_0x3b;
        uint8_t byte_0x3c;
        uint8_t byte_0x3d;
        uint8_t byte_0x3e;
        uint8_t byte_0x3f;
        uint8_t byte_0x40;
        uint8_t byte_0x41;
        uint8_t pad_0x42[2];
        float volumeAmbient;
        uint8_t byte_0x48;
        uint8_t byte_0x49;
        uint8_t byte_0x4a;
        uint8_t byte_0x4b;
        uint8_t byte_0x4c;
        uint8_t byte_0x4d;
        uint8_t byte_0x4e;
        uint8_t pad_0x4f;
        int32_t i32_0x50;
        int32_t i32_0x54;
        int32_t i32_0x58;
        uint8_t pad_0x5c[4];
        uint8_t byte_0x60;
        uint8_t byte_0x61;
        uint8_t byte_0x62;
    };
#pragma pack(pop)

#if __SIZEOF_POINTER__ == 4
    static_assert(offsetof(OptionsBuffer, volumeSfx) == 0x08, "OptionsBuffer.volumeSfx");
    static_assert(offsetof(OptionsBuffer, byte_0x11) == 0x11, "OptionsBuffer.byte_0x11");
    static_assert(offsetof(OptionsBuffer, volumeMusic) == 0x20, "OptionsBuffer.volumeMusic");
    static_assert(offsetof(OptionsBuffer, byte_0x32) == 0x32, "OptionsBuffer.byte_0x32");
    static_assert(offsetof(OptionsBuffer, volumeAmbient) == 0x44, "OptionsBuffer.volumeAmbient");
    static_assert(offsetof(OptionsBuffer, i32_0x50) == 0x50, "OptionsBuffer.i32_0x50");
    static_assert(offsetof(OptionsBuffer, byte_0x62) == 0x62, "OptionsBuffer.byte_0x62");
    static_assert(sizeof(OptionsBuffer) == 0x63, "OptionsBuffer size");
#endif
}

const char *g_android_origami_super_club = nullptr;

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetOrigamiSuperClub( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jobject /*thiz*/, jstring value) {
    jboolean isCopy;
    g_android_origami_super_club = env->GetStringUTFChars(value, &isCopy);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ReleaseOrigamiSuperClub( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jobject /*thiz*/, jstring value) {
    env->ReleaseStringUTFChars(value, g_android_origami_super_club);
}








static bool g_LS_gameSaving = false;

void loadingScreen(AbyssEngine::PaintCanvas *canvas, int progress, void *resourceHolder) { // lint: void_ptr (exported free function; mangled signature is fixed)
    // lint: void_ptr (exported free function; mangled signature is fixed)
    canvas->ClearBuffer(0xff);
    canvas->Begin2d();
    canvas->SetColor(0xffffffffu);

    if (resourceHolder == nullptr) {
        canvas->End2d();
        canvas->SwapBuffer();
        return;
    }

    unsigned int resId = *static_cast<unsigned int *>(resourceHolder);
    if (GameText::getLanguage() == 9 ||
        !canvas->ResourceLoaded(resId, static_cast<AbyssEngine::ResourceType>(1))) {
        Globals::globals->loadFont(GameText::getLanguage());
        *static_cast<unsigned int *>(resourceHolder) = resId;
    }

    unsigned int spinner = 0xffffffffu;
    unsigned int barFill = 0xffffffffu;
    Globals::Canvas->Image2DCreate(0x504, spinner);
    Globals::Canvas->Image2DCreate(0x505, barFill);

    int barFillW = Globals::Canvas->GetImage2DWidth(barFill);
    int spinnerW = Globals::Canvas->GetImage2DWidth(spinner);
    int barFillH = Globals::Canvas->GetImage2DHeight(barFill);

    int topMargin = *reinterpret_cast<int *>((unsigned char *) Globals::globals + 0x27c);

    Globals::layout->drawBG();
    Globals::layout->drawHeader();
    Globals::layout->drawEmptyFooter(false);

    int captionId = g_LS_gameSaving ? 0x18a : 0x189;
    String caption(*Globals::gameText->getText(captionId), false);

    if (progress < 0) progress = 100;
    int barShortfall = 100;
    if (progress > 0) barShortfall = 100 - progress;

    int *layoutFields = *reinterpret_cast<int **>(Globals::layout);
    int globalsCenterX = *reinterpret_cast<int *>((unsigned char *) Globals::globals);
    int barBaseline = *reinterpret_cast<int *>((unsigned char *) Globals::globals + 0x290);

    int captionW = canvas->GetTextWidth(resId, caption);
    int captionX = (Globals::w / 2) - (captionW / 2);
    int captionY = (Globals::h / 2) - topMargin - layoutFields[1] - layoutFields[0x280 / 4];
    canvas->DrawString(resId, caption, captionX, captionY, false);

    int spinnerX = (globalsCenterX / 2) - (spinnerW / 2);
    int spinnerY = (Globals::h / 2) - topMargin;
    canvas->DrawImage2D(spinner, spinnerX, spinnerY);

    float fillW = static_cast<float>(barShortfall < 0 ? 0 : barShortfall) * 0.01f *
                  static_cast<float>(barFillW);
    int barTransY = barBaseline + ((Globals::h / 2) - topMargin);
    int barCenterX = globalsCenterX / 2;

    if (GameText::getLanguage() == 9) {
        float pivot = static_cast<float>(barCenterX + (barFillW / 2)) - fillW;
        Globals::Canvas->DrawRegion2D(barFill,
                                  static_cast<int>(static_cast<float>(barFillW) - fillW), 0,
                                  static_cast<int>(fillW), barFillH,
                                  0.0f, 0, 0,
                                  static_cast<int>(pivot), barTransY);
    } else {
        Globals::Canvas->DrawRegion2D(barFill,
                                  0, 0,
                                  static_cast<int>(fillW), barFillH,
                                  0.0f, 0, 0,
                                  barCenterX - (barFillW / 2), barTransY);
    }

    canvas->End2d();
    canvas->SwapBuffer();
}

static int *g_RH_recordCount = nullptr;

void *RecordHandler::readAllRecords() { // lint: void_ptr (method signature; return type fixed by header)
    // lint: void_ptr (method signature; return type fixed by header)
    Array<GameRecord *> *arr = new Array<GameRecord *>();
    int *cnt = g_RH_recordCount;
    ArraySetLength(*cnt, *arr);
    for (int i = 0; i < *cnt; i++) {
        GameRecord *r = this->recordStoreRead(i);
        (*arr)[i] = r;
    }
    return arr;
}

void *RecordHandler::readAllPreviewRecords() { // lint: void_ptr (method signature; return type fixed by header)
    // lint: void_ptr (method signature; return type fixed by header)
    Array<GameRecord *> *arr = new Array<GameRecord *>();
    int *cnt = g_RH_recordCount;
    ArraySetLength(*cnt, *arr);
    for (int i = 0; i < *cnt; i++) {
        GameRecord *r = this->recordStoreReadPreview(i);
        (*arr)[i] = r;
    }
    return arr;
}

bool RecordHandler::notEnoughMemory() {
    return (int)AEFile::GetDeviceFreeSpace() < 900;
}

static int *g_CSV_count = nullptr;

void RecordHandler::convertSDVersionSaves() {
    Array<signed char *> *a0 = new Array<signed char *>();
    int *cnt = g_CSV_count;
    ArraySetLength(*cnt, *a0);
    Array<signed char *> *a1 = new Array<signed char *>();
    ArraySetLength(*cnt, *a1);

    unsigned n = *cnt;
    int *sizes0 = new int[n];
    int *sizes1 = new int[n];

    for (int i = 0; i < (int) n; i++) {
        sizes0[i] = this->readRecordAsByteArray(&(*a0)[i], i, false);
        sizes1[i] = this->readRecordAsByteArray(&(*a1)[i], i, true);

        String num, path;
        num.Set((long long) (i));
        path = this->recordDir + num;
        AEFile::FileDelete(path);

        num.Set((long long) (i));
        path = this->backupDir + num;
        AEFile::FileDelete(path);
        n = *cnt;
    }

    int i = 0;
    while (true) {
        int last = (int) n - 1;
        int j = i;
        signed char *rec = 0;
        bool found = false;
        while (j < last) {
            rec = (*a0)[j];
            if (rec != 0) {
                found = true;
                break;
            }
            j++;
        }
        if (!found) {
            signed char *lastRec = (*a0)[last];
            if (lastRec != 0) {
                this->writeByteArrayAsRecord(lastRec, sizes0[last], 0, false);
                this->addHash(0);
                int idx = (int) (*cnt) - 1;
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

    ArrayReleaseArrays(*a0);
    ArrayReleaseArrays(*a1);
    delete a0;
    delete a1;
    delete[] sizes0;
    delete[] sizes1;

    return;
}

static const unsigned char RH_ah_salt[0x19] = {};

static unsigned char **RH_ah_key = nullptr;

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
        unsigned long long *dst = (unsigned long long *) (out + len);
        unsigned long long *src = (unsigned long long *) md;
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

int RecordHandler::readRecordAsByteArray(signed char **out, int slot, bool fromBackup) {
    String num;
    String path;
    unsigned int fd;
    int sz;

    num.Set((long long) (slot));
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

GameRecord *RecordHandler::readRecord(int slot) {
    return this->recordStoreRead(slot);
}

void RecordHandler::recoverSDVersionSaves() {
}

void *RecordHandler::readWanted(unsigned int fd) { // lint: void_ptr (method signature; return type fixed by header)
    // lint: void_ptr (method signature; return type fixed by header)
    (void) this;
    bool active = false;
    bool terminated = false;
    int currentLocation = -1;
    int travelsTo = -1;
    int lastSeen = -1;
    AEFile::Read(active, fd);
    AEFile::Read(terminated, fd);
    AEFile::Read(reinterpret_cast<int32_t &>(currentLocation), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(travelsTo), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(lastSeen), fd);

    String name;

    int idx = 0, board = 0, race = 0;
    bool male = true;
    int ship = 0, weapon = 0, hp = 0, loot = 0, lootAmt = 0;
    int reward = 0, reqBounties = 0, reqMission = 0, numWingmen = 0;

    AEFile::Read(name, fd, (bool)1);
    AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(board), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(race), fd);
    AEFile::Read(male, fd);
    AEFile::Read(reinterpret_cast<int32_t &>(ship), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(weapon), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(hp), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(loot), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(lootAmt), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(reward), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(reqBounties), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(reqMission), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(numWingmen), fd);

    Wanted *w = new Wanted(idx, name, board, race, male, ship, weapon, hp, loot,
                           lootAmt, reward, reqBounties, reqMission, numWingmen);

    int *parts = new int[5];
    int *p = parts;
    for (unsigned i = 0; i < 5; i++) {
        AEFile::Read(reinterpret_cast<int32_t &>(*p), fd);
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

static int *g_RH_csd_count = nullptr;

static char **g_RH_csd_flag = nullptr;

void RecordHandler::changeSaveDirectoryToBackupDirectory() {
    Array<signed char *> *a0 = new Array<signed char *>();
    int *cnt = g_RH_csd_count;
    ArraySetLength(*cnt, *a0);
    Array<signed char *> *a1 = new Array<signed char *>();
    ArraySetLength(*cnt, *a1);

    unsigned int n = *cnt;
    int *sizes0 = new int[n];
    int *sizes1 = new int[n];

    for (int i = 0; i < (int) n; i++) {
        sizes0[i] = this->readRecordAsByteArray(&(*a0)[i], i, false);
        sizes1[i] = this->readRecordAsByteArray(&(*a1)[i], i, true);
        n = *cnt;
    }

    **g_RH_csd_flag = 1;
    int i = 0;
    while (i < (int) n) {
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

    ArrayReleaseArrays(*a0);
    ArrayReleaseArrays(*a1);
    delete a0;
    delete a1;
    delete[] sizes0;
    delete[] sizes1;
}

GameRecord *RecordHandler::recordStoreReadPreview(int slot) {
    String path;
    String num;

    num.Set((long long) (slot));
    path = this->backupDir + num;

    unsigned int fd;
    GameRecord *gr = 0;
    if (AEFile::FileExist(path) != 0) {
        AEFile::OpenRead(path, &fd);
        gr = new GameRecord();

        AEFile::Read(reinterpret_cast<int64_t &>(gr->playTimeObj), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->field_0x08), fd);
        AEFile::Read(reinterpret_cast<String &>(gr->pilotName), fd, (bool)1);
        AEFile::Read(reinterpret_cast<String &>(gr->field_0x188), fd, (bool)1);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->field_0x40), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->killsText), fd);
        AEFile::Read(gr->rank, fd);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->shipId), fd);
        AEFile::Close(fd);
    }
    return gr;
}

static const char RH_lit0[] = "";

static const char RH_lit1[] = "";

static const char RH_lit2[] = "";

RecordHandler::RecordHandler()
    : currentMission(0),
      currentAgent(0),
      optionsPath(RH_lit0),
      recordDir(RH_lit1),
      backupDir(RH_lit2) {
}

RecordHandler::~RecordHandler() {
}

void RecordHandler::writeByteArrayAsOptionsFile(signed char *buf, int n) {
    String tmp(this->optionsPath, false);
    unsigned int fd;

    if (AEFile::FileExist(tmp) != 0)
        AEFile::FileDelete(tmp);
    AEFile::OpenWrite(tmp, &fd);
    AEFile::Write((uint32_t)n, buf, fd);
    AEFile::Close(fd);
}

static int *g_RH_wp_float = nullptr;

struct RankHolder {
    uint8_t _pad[0x2c];
    int32_t rankBits;
};

void RecordHandler::recordStoreWritePreview(int slot) {
    String path;
    String num;
    unsigned int fd;

    num.Set((long long) (slot));
    path = this->backupDir + num;

    if (AEFile::FileExist(path) != 0)
        AEFile::FileDelete(path);
    AEFile::OpenWrite(path, &fd);

    AEFile::Write(Globals::status->getPlayingTime(), fd);
    AEFile::Write((int32_t)Globals::status->getCredits(), fd);

    num = ((Station *) (&num))->getName();
    AEFile::Write(num, fd, (bool)true);

    num = ((SolarSystem *) (&num))->getName();
    AEFile::Write(num, fd, (bool)true);

    AEFile::Write((int32_t)Globals::status->getCurrentCampaignMission(), fd);
    AEFile::Write((int32_t)Globals::status->getLevel(), fd);
    AEFile::Write(reinterpret_cast<float &>((*reinterpret_cast<RankHolder **>(g_RH_wp_float))->rankBits), fd);

    AEFile::Write((int32_t)((Ship *) (Globals::status->getShip()))->getIndex(), fd);
    AEFile::Close(fd);
}

static const char g_WA_empty1[] = "";

static const char g_WA_empty2[] = "";

void RecordHandler::writeAgent(Agent *agentPtr, unsigned int fd) {
    RecordHandler * self = this;
    Agent *agent = agentPtr;
    AEFile::Write((int32_t)((Agent *) (agent))->getCosts(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getSellSystemIndex(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getSellBlueprintIndex(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getEvent(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getIndex(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getOffer(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getRace(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getSellItemIndex(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getSellItemPrice(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getSellItemQuantity(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getStation(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getSystem(), fd);
    AEFile::Write((int32_t)((Agent *) (agent))->getWingmanFriendsCount(), fd);
    AEFile::Write(((Agent *) (agent))->isMale(), fd);
    AEFile::Write(((Agent *) (agent))->hasReward(), fd);
    AEFile::Write(((Agent *) (agent))->hasAcceptedOffer(), fd);
    AEFile::Write(agent->wasAskedForDifficulty, fd);
    AEFile::Write(agent->wasAskedForLocation, fd);

    if (((Agent *) (agent))->getImageParts() == 0) {
        AEFile::Write((int32_t)-1, fd);
    } else {
        AEFile::Write((int32_t)5, fd);
        for (int i = 0; i != 5; i++) {
            int *img = ((Agent *) (agent))->getImageParts();
            AEFile::Write((int32_t)img[i], fd);
        }
    }
    if (0x12 < ((Agent *) (agent))->getIndex()) {
        AEFile::Write((int32_t)((Agent *) (agent))->getSellModIndex(), fd);
    }

    {String s = agent->getMissionString(); AEFile::Write(s, fd, (bool)1);}
    {String s = agent->getName(); AEFile::Write(s, fd, (bool)1);}
    {String s = agent->getStationName(); AEFile::Write(s, fd, (bool)1);}
    {String s = agent->getSystemName(); AEFile::Write(s, fd, (bool)1);}

    String *f0c = agent->wingman1;
    if (f0c == 0) {
        String s(g_WA_empty1);
        AEFile::Write(s, fd, (bool)1);
    } else {
        AEFile::Write(*f0c, fd, (bool)1);
    }
    String *f10 = agent->wingman2;
    if (f10 == 0) {
        String s(g_WA_empty2);
        AEFile::Write(s, fd, (bool)1);
    } else {
        AEFile::Write(*f10, fd, (bool)1);
    }

    self->currentAgent = agent;
    Mission *mission = agent->getMission();
    if (mission == 0 || self->currentMission == mission) {
        AEFile::Write((int32_t)-1, fd);
    } else {
        AEFile::Write((int32_t)1, fd);
        self->writeMission(mission, fd);
    }

    return;
}

void RecordHandler::writeMission(Mission *m, unsigned int fd) {
    AEFile::Write((int32_t)m->getType(), fd);
    if (m->isEmpty() == 0) {
        {String s = m->getClientName(); AEFile::Write(s, fd, (bool)1);}
        {String s = m->getTargetName(); AEFile::Write(s, fd, (bool)1);}
        {String s = m->getTargetStationName(); AEFile::Write(s, fd, (bool)1);}
        {String s = m->getTargetSystemName(); AEFile::Write(s, fd, (bool)1);}

        AEFile::Write((bool)m->isCampaignMission(), fd);
        if (m->getClientImage() == 0) {
            AEFile::Write((int32_t)-1, fd);
        } else {
            AEFile::Write((int32_t)5, fd);
            for (int i = 0; i != 5; i++) {
                int *img = (int *) (long) m->getClientImage();
                AEFile::Write((int32_t)img[i], fd);
            }
        }
        AEFile::Write((int32_t)m->getClientRace(), fd);
        AEFile::Write((int32_t)m->getCosts(), fd);
        AEFile::Write((int32_t)m->getBonus(), fd);
        AEFile::Write((int32_t)m->getReward(), fd);
        AEFile::Write((int32_t)m->getTargetStation(), fd);
        AEFile::Write((int32_t)m->getDifficulty(), fd);
        AEFile::Write((int32_t)m->getProductionGoodIndex(), fd);
        AEFile::Write((int32_t)m->getProductionGoodAmount(), fd);
        AEFile::Write((int32_t)m->getStatusValue(), fd);
        AEFile::Write(m->isVisible(), fd);

        this->currentMission = m;
        Agent *agent = m->getAgent();
        if (agent == 0 || this->currentAgent == agent) {
            AEFile::Write((int32_t)-1, fd);
        } else {
            AEFile::Write((int32_t)1, fd);
            this->writeAgent(agent, fd);
        }
    }

    return;
}

static unsigned char *g_LO_settings = nullptr;

static int **g_LO_statusObj = nullptr;

static int *g_LO_nameSlot = nullptr;

static int *g_LO_extraInt = nullptr;

static bool *g_LO_flag1 = nullptr;

static bool *g_LO_flag2 = nullptr;

static int *g_LO_textObj = nullptr;

static int *g_LO_fontKind = nullptr;

static int *g_LO_fmodSlot = nullptr;

void RecordHandler::loadOptions() {
    String &path = this->optionsPath;
    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);
        int valid = this->checkHash(fd);
        AEFile::Close(fd);
        if (valid != 0) {
            AEFile::OpenRead(path, &fd);
            OptionsBuffer *s = reinterpret_cast<OptionsBuffer *>(g_LO_settings);

            AEFile::Read(s->byte_0x10, fd);
            AEFile::Read(s->byte_0x11, fd);
            reinterpret_cast<Status *>(*g_LO_statusObj)->_pad_0xfa[0] = s->byte_0x11;

            AEFile::Read(s->i32_0x14, fd);
            AEFile::Read(s->i32_0x18, fd);
            AEFile::Read(s->i32_0x1c, fd);
            AEFile::Read(s->i32_0x00, fd);
            AEFile::Read(s->byte_0x0c, fd);
            AEFile::Read(s->byte_0x0d, fd);
            AEFile::Read(s->i32_0x04, fd);
            AEFile::Read(s->i32_0x24, fd);
            AEFile::Read(s->i32_0x28, fd);

            int *nameSlot = g_LO_nameSlot;
            if (*nameSlot != 0) {
                delete (String *) (long) *nameSlot;
                *nameSlot = 0;
            }

            String name;
            {
                if (name.data) delete[] name.data;
                name.data = nullptr;
                name.length = 0;
            }
            AEFile::Read(name, fd, (bool)false);
            String *ns = new String(name);
            *nameSlot = (int) (long) ns;

            AEFile::Read(*g_LO_extraInt, fd);
            AEFile::Read(*g_LO_flag1, fd);
            AEFile::Read(s->i32_0x54, fd);
            AEFile::Read(s->i32_0x58, fd);
            AEFile::Read(s->volumeSfx, fd);
            AEFile::Read(s->byte_0x0e, fd);
            AEFile::Read(s->byte_0x32, fd);
            AEFile::Read(s->byte_0x33, fd);
            AEFile::Read(s->byte_0x34, fd);
            AEFile::Read(s->volumeMusic, fd);

            short lang = -1;
            AEFile::Read(reinterpret_cast<int16_t &>(lang), fd);
            AEFile::Read(*g_LO_flag2, fd);
            AEFile::Read(s->byte_0x35, fd);
            AEFile::Read(s->byte_0x36, fd);
            AEFile::Read(s->byte_0x3a, fd);
            AEFile::Read(s->byte_0x3b, fd);
            AEFile::Read(s->byte_0x3c, fd);
            AEFile::Read(s->byte_0x37, fd);
            AEFile::Read(s->byte_0x3d, fd);
            AEFile::Read(s->byte_0x3e, fd);
            AEFile::Read(s->byte_0x3f, fd);
            AEFile::Read(s->byte_0x40, fd);
            AEFile::Read(s->byte_0x41, fd);
            AEFile::Read(s->volumeAmbient, fd);
            AEFile::Read(s->byte_0x48, fd);
            AEFile::Read(s->byte_0x38, fd);
            AEFile::Read(s->byte_0x4e, fd);
            AEFile::Read(s->byte_0x49, fd);
            AEFile::Read(s->byte_0x4a, fd);
            AEFile::Read(s->byte_0x4b, fd);
            AEFile::Read(s->byte_0x4c, fd);
            AEFile::Read(s->byte_0x4d, fd);
            AEFile::Read(s->i32_0x50, fd);
            AEFile::Read(s->byte_0x60, fd);
            AEFile::Read(s->byte_0x61, fd);
            AEFile::Close(fd);

            int langVal = lang;
            if (-1 < langVal) {
                if (langVal == 9) langVal = 0;
                ((GameText *) ((short) **(int **) g_LO_textObj))->setLanguage_i(langVal);
                Globals::globals->loadFont(**(int **) g_LO_fontKind);
            }

            int *fmodSlot = g_LO_fmodSlot;
            if (*fmodSlot != 0) {
                FModSound *fm = (FModSound *) (long) *fmodSlot;
                ((FModSound *) (fm))->setAudioLanguage(lang);

                ((FModSound *) (fm))->enableCategory(0, s->byte_0x32 != 0);
                ((FModSound *) (fm))->enableCategory(1, s->byte_0x33 != 0);
                ((FModSound *) (fm))->enableCategory(2, s->byte_0x34 != 0);
                ((FModSound *) (fm))->setVolume(0, s->volumeSfx);
                ((FModSound *) (fm))->setVolume(1, s->volumeMusic);
                ((FModSound *) (fm))->setVolume(2, s->volumeAmbient);
            }

            AEFile::Read(s->byte_0x62, fd);
        }
    }

    return;
}

void RecordHandler::loadResolutionValue(float resolution) {
    (void) resolution;
    String &path = this->optionsPath;
    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);

        OptionsBuffer buf;
        OptionsBuffer *r = &buf;

        AEFile::Read(r->byte_0x10, fd);
        AEFile::Read(r->byte_0x11, fd);
        AEFile::Read(r->i32_0x14, fd);
        AEFile::Read(r->i32_0x18, fd);
        AEFile::Read(r->i32_0x1c, fd);
        AEFile::Read(r->i32_0x00, fd);
        AEFile::Read(r->byte_0x0c, fd);
        AEFile::Read(r->byte_0x0d, fd);
        AEFile::Read(r->i32_0x04, fd);
        AEFile::Read(r->i32_0x24, fd);
        AEFile::Read(r->i32_0x28, fd);

        String name;
        {
            if (name.data) delete[] name.data;
            name.data = nullptr;
            name.length = 0;
        }
        AEFile::Read(name, fd, (bool)false);

        int iv9c;
        bool b9d;
        AEFile::Read(reinterpret_cast<int32_t &>(iv9c), fd);
        AEFile::Read(b9d, fd);
        AEFile::Read(r->i32_0x54, fd);
        AEFile::Read(r->i32_0x58, fd);
        AEFile::Read(r->volumeSfx, fd);
        AEFile::Read(r->byte_0x0e, fd);
        AEFile::Read(r->byte_0x32, fd);
        AEFile::Read(r->byte_0x33, fd);
        AEFile::Read(r->byte_0x34, fd);
        AEFile::Read(r->volumeMusic, fd);

        short resVal = -1;
        AEFile::Read(reinterpret_cast<int16_t &>(resVal), fd);
        AEFile::Read(b9d, fd);
        AEFile::Read(r->byte_0x35, fd);
        AEFile::Read(r->byte_0x36, fd);
        AEFile::Read(r->byte_0x3a, fd);
        AEFile::Read(r->byte_0x3b, fd);
        AEFile::Read(r->byte_0x3c, fd);
        AEFile::Read(r->byte_0x37, fd);
        AEFile::Read(r->byte_0x3d, fd);
        AEFile::Read(r->byte_0x3e, fd);
        AEFile::Read(r->byte_0x3f, fd);
        AEFile::Read(r->byte_0x40, fd);
        AEFile::Read(r->byte_0x41, fd);
        AEFile::Read(r->volumeAmbient, fd);

        AEFile::Close(fd);
    }

    return;
}

int RecordHandler::writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup) {
    String num;
    String path;
    unsigned int fd;

    num.Set((long long) (slot));
    String &dir = toBackup ? this->backupDir : this->recordDir;
    path = dir + num;

    if (AEFile::FileExist(path) != 0)
        AEFile::FileDelete(path);
    AEFile::OpenWrite(path, &fd);
    ((AEFile *) (n))->Write(buf, fd);
    AEFile::Close(fd);
    return n;
}

void *RecordHandler::readMission(unsigned int fd) { // lint: void_ptr (method signature; return type fixed by header)
    // lint: void_ptr (method signature; return type fixed by header)
    Mission *mission = 0;

    int type = 0;
    AEFile::Read(reinterpret_cast<int32_t &>(type), fd);
    if (type != -1) {
        String clientName, targetName, targetStation, targetSystem;
        {
            if (clientName.data) delete[] clientName.data;
            clientName.data = nullptr;
            clientName.length = 0;
        }
        AEFile::Read(clientName, fd, (bool)true);
        {
            if (targetName.data) delete[] targetName.data;
            targetName.data = nullptr;
            targetName.length = 0;
        }
        AEFile::Read(targetName, fd, (bool)true);
        {
            if (targetStation.data) delete[] targetStation.data;
            targetStation.data = nullptr;
            targetStation.length = 0;
        }
        AEFile::Read(targetStation, fd, (bool)true);
        {
            if (targetSystem.data) delete[] targetSystem.data;
            targetSystem.data = nullptr;
            targetSystem.length = 0;
        }
        AEFile::Read(targetSystem, fd, (bool)true);

        bool isEmpty = false;
        AEFile::Read(isEmpty, fd);

        unsigned imgCount = 0;
        AEFile::Read(imgCount, fd);
        int *img = 0;
        if ((int) imgCount >= 1) {
            img = new int[imgCount];
            int *p = img;
            for (int i = 0; i < (int) imgCount; i++) {
                AEFile::Read(reinterpret_cast<int32_t &>(*p), fd);
                p++;
            }
        }

        int clientRace = 0, costs = 0, bonus = 0, reward = 0, targetStationIdx = 0;
        int difficulty = 0, prodIdx = 0, prodAmt = 0, statusValue = 0;
        bool visible = false;
        AEFile::Read(reinterpret_cast<int32_t &>(clientRace), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(costs), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(bonus), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(reward), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(targetStationIdx), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(difficulty), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(prodIdx), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(prodAmt), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(statusValue), fd);
        AEFile::Read(visible, fd);

        int hasAgent = 0;
        AEFile::Read(reinterpret_cast<int32_t &>(hasAgent), fd);
        Agent *agent = (hasAgent < 1) ? 0 : (Agent *) this->readAgent(fd);

        if (!isEmpty) {
            String nameCopy;
            nameCopy.Set((clientName).data);
            mission = new Mission(type, nameCopy, img, clientRace, reward, targetStationIdx, difficulty);
        } else {
            mission = new Mission(type, reward, targetStationIdx);
        }
        ((Mission *) (mission))->setCosts(costs);
        ((Mission *) (mission))->setBonus(bonus);
        ((Mission *) (mission))->setProductionGoods(prodIdx, prodAmt);
        ((Mission *) (mission))->setStatusValue(statusValue);
        ((Mission *) (mission))->setVisible(visible);
        ((Mission *) (mission))->setAgent((Agent *) agent);

        String tgtNameCopy;
        tgtNameCopy.Set((targetName).data);
        ((Mission *) (mission))->setTargetName(tgtNameCopy);

        if (agent != 0) {
            ((Agent *) (agent))->setMission((Mission *) mission);
        }
    }

    return mission;
}

static unsigned char *g_SO_settings = nullptr;

static String **g_SO_playerName = nullptr;

static const char g_SO_defName[] = "";

static int *g_SO_extraInt = nullptr;

static unsigned char *g_SO_flag1 = nullptr;

static unsigned char *g_SO_flag2 = nullptr;

void RecordHandler::saveOptions() {
    String &path = this->optionsPath;
    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);

    OptionsBuffer *s = reinterpret_cast<OptionsBuffer *>(g_SO_settings);

    AEFile::Write(s->byte_0x10, fd);
    AEFile::Write(s->byte_0x11, fd);
    AEFile::Write(s->i32_0x14, fd);
    AEFile::Write(s->i32_0x18, fd);
    AEFile::Write(s->i32_0x1c, fd);
    AEFile::Write(s->i32_0x00, fd);
    AEFile::Write(s->byte_0x0c, fd);
    AEFile::Write(s->byte_0x0d, fd);
    AEFile::Write(s->i32_0x04, fd);
    AEFile::Write(s->i32_0x24, fd);
    AEFile::Write(s->i32_0x28, fd);

    String *name = *g_SO_playerName;
    if (name == 0) {
        String def(g_SO_defName);
        AEFile::Write(def, fd, (bool)false);
    } else {
        AEFile::Write(*name, fd, (bool)false);
    }

    AEFile::Write((int32_t)*g_SO_extraInt, fd);
    AEFile::Write(*g_SO_flag1, fd);
    AEFile::Write(s->i32_0x54, fd);
    AEFile::Write(s->i32_0x58, fd);
    AEFile::Write(s->volumeSfx, fd);
    AEFile::Write(s->byte_0x0e, fd);
    AEFile::Write(s->byte_0x32, fd);
    AEFile::Write(s->byte_0x33, fd);
    AEFile::Write(s->byte_0x34, fd);
    AEFile::Write(s->volumeMusic, fd);

    AEFile::Write((int16_t)GameText::getLanguage(), fd);
    AEFile::Write(*g_SO_flag2, fd);
    AEFile::Write(s->byte_0x35, fd);
    AEFile::Write(s->byte_0x36, fd);
    AEFile::Write(s->byte_0x3a, fd);
    AEFile::Write(s->byte_0x3b, fd);
    AEFile::Write(s->byte_0x3c, fd);
    AEFile::Write(s->byte_0x37, fd);
    AEFile::Write(s->byte_0x3d, fd);
    AEFile::Write(s->byte_0x3e, fd);
    AEFile::Write(s->byte_0x3f, fd);
    AEFile::Write(s->byte_0x40, fd);
    AEFile::Write(s->byte_0x41, fd);
    AEFile::Write(s->volumeAmbient, fd);
    AEFile::Write(s->byte_0x48, fd);
    AEFile::Write(s->byte_0x38, fd);
    AEFile::Write(s->byte_0x4e, fd);
    AEFile::Write(s->byte_0x49, fd);
    AEFile::Write(s->byte_0x4a, fd);
    AEFile::Write(s->byte_0x4b, fd);
    AEFile::Write(s->byte_0x4c, fd);
    AEFile::Write(s->byte_0x4d, fd);
    AEFile::Write(s->i32_0x50, fd);
    AEFile::Write(s->byte_0x60, fd);
    AEFile::Write(s->byte_0x61, fd);
    AEFile::Write(s->byte_0x62, fd);
    AEFile::Close(fd);
    this->addHashToOptions();

    return;
}

void *RecordHandler::readAgent(unsigned int fd) { // lint: void_ptr (method signature; return type fixed by header)
    // lint: void_ptr (method signature; return type fixed by header)
    int costs = 0, sellSys = 0, sellBp = 0, event = 0, idx = 0, offer = 0, race = 0;
    int sellItemIdx = 0, sellItemPrice = 0, sellItemQty = 0, station = 0, system = 0;
    unsigned wingmen = 0;
    AEFile::Read(reinterpret_cast<int32_t &>(costs), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(sellSys), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(sellBp), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(event), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(offer), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(race), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(sellItemIdx), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(sellItemPrice), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(sellItemQty), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(station), fd);
    AEFile::Read(reinterpret_cast<int32_t &>(system), fd);
    AEFile::Read(wingmen, fd);

    unsigned char male = 0;
    bool hasReward = false, accepted = false;
    unsigned char raw24 = 0, raw25 = 0;
    AEFile::Read(male, fd);
    AEFile::Read(hasReward, fd);
    AEFile::Read(accepted, fd);
    AEFile::Read(raw24, fd);
    AEFile::Read(raw25, fd);

    unsigned imgCount = 0;
    AEFile::Read(imgCount, fd);
    int *img = 0;
    if (0 < (int) imgCount) {
        img = new int[imgCount];
        int *p = img;
        for (int i = 0; i < (int) imgCount; i++) {
            AEFile::Read(reinterpret_cast<int32_t &>(*p), fd);
            p++;
        }
    }

    int sellMod = -1;
    if (0x12 < idx) {
        AEFile::Read(reinterpret_cast<int32_t &>(sellMod), fd);
    }

    String missionStr, name, stationName, systemName, strE, strF;
    {
        if (missionStr.data) delete[] missionStr.data;
        missionStr.data = nullptr;
        missionStr.length = 0;
    }
    {
        if (name.data) delete[] name.data;
        name.data = nullptr;
        name.length = 0;
    }
    {
        if (stationName.data) delete[] stationName.data;
        stationName.data = nullptr;
        stationName.length = 0;
    }
    {
        if (systemName.data) delete[] systemName.data;
        systemName.data = nullptr;
        systemName.length = 0;
    }
    {
        if (strE.data) delete[] strE.data;
        strE.data = nullptr;
        strE.length = 0;
    }
    {
        if (strF.data) delete[] strF.data;
        strF.data = nullptr;
        strF.length = 0;
    }
    AEFile::Read(missionStr, fd, (bool)true);
    AEFile::Read(name, fd, (bool)true);
    AEFile::Read(stationName, fd, (bool)true);
    AEFile::Read(systemName, fd, (bool)true);
    AEFile::Read(strE, fd, (bool)true);
    AEFile::Read(strF, fd, (bool)true);

    int hasMission = 0;
    AEFile::Read(reinterpret_cast<int32_t &>(hasMission), fd);
    Mission *mission = (hasMission < 1) ? 0 : (Mission *) this->readMission(fd);

    String nameCopy;
    nameCopy.Set((name).data);
    Agent *agent = new Agent(idx, nameCopy, station, system, race, male, sellSys, sellBp, sellMod, sellItemIdx);

    ((Agent *) (agent))->setCosts(costs);
    ((Agent *) (agent))->setEvent(event);
    ((Agent *) (agent))->setOffer(offer);
    ((Agent *) (agent))->setSellItemData(sellItemPrice, sellItemQty, sellItemIdx);

    if (strE.size() != 0) {
        String *s = new String();
        s->Set((strE).data);
        agent->wingman1 = s;
    }
    if (strF.size() != 0) {
        String *s = new String();
        s->Set((strF).data);
        agent->wingman2 = s;
    }
    agent->wingmanCount = wingmen;

    Array<String *> *arr = new Array<String *>();
    ArraySetLength(wingmen, *arr);
    for (int i = 0; i < (int) wingmen; i++) {
        String *s = new String();
        String *src = (i == 0) ? &strE : &strF;
        s->Set((src)->data);
        (*arr)[i] = s;
    }
    ((Agent *) (agent))->setWingmanFriendNames(arr);
    ((Agent *) (agent))->giveRewardAtNextChat(hasReward);
    ((Agent *) (agent))->setOfferAccepted(accepted);
    ((Agent *) (agent))->setImageParts(img);

    String tmp;
    tmp.Set((missionStr).data);
    ((Agent *) (agent))->setMissionString(tmp);
    tmp.Set((stationName).data);
    ((Agent *) (agent))->setStationName(tmp);
    tmp.Set((systemName).data);
    ((Agent *) (agent))->setSystemName(tmp);
    ((Agent *) (agent))->setMission((Mission *) mission);

    agent->wasAskedForDifficulty = raw24;
    agent->wasAskedForLocation = raw25;

    return agent;
}

void RecordHandler::writeWanted(Wanted *w, unsigned int fd) {
    (void) this;
    AEFile::Write(w->isActive(), fd);
    AEFile::Write(w->isTerminated(), fd);
    AEFile::Write((int32_t)w->getCurrentLocation(), fd);
    AEFile::Write((int32_t)w->getTravelsTo(), fd);
    AEFile::Write((int32_t)w->getLastSeen(), fd);

    {String name = w->getName(); AEFile::Write(name, fd, (bool)1);}

    AEFile::Write((int32_t)w->getIndex(), fd);
    AEFile::Write((int32_t)w->getBoard(), fd);
    AEFile::Write((int32_t)w->getRace(), fd);
    AEFile::Write((uint8_t)w->isMale(), fd);
    AEFile::Write((int32_t)w->getShip(), fd);
    AEFile::Write((int32_t)w->getWeapon(), fd);
    AEFile::Write((int32_t)w->getHitpoints(), fd);
    AEFile::Write((int32_t)w->getLoot(), fd);
    AEFile::Write((int32_t)w->getLootAmount(), fd);
    AEFile::Write((int32_t)w->getReward(), fd);
    AEFile::Write((int32_t)w->getRequiredBounties(), fd);
    AEFile::Write((int32_t)w->getRequiredMission(), fd);
    AEFile::Write((int32_t)w->getNumWingmen(), fd);

    for (int i = 0; i != 5; i++) {
        int *parts = w->getImageParts();
        AEFile::Write((int32_t)parts[i], fd);
    }

    return;
}

int RecordHandler::recordStoreWritePreview(GameRecord *rec, int slot) {
    if (rec == 0)
        return 0;

    String path;
    String num;
    unsigned int fd;

    num.Set((long long) (slot));
    path = this->backupDir + num;

    if (AEFile::FileExist(path) != 0)
        AEFile::FileDelete(path);
    AEFile::OpenWrite(path, &fd);

    AEFile::Write(reinterpret_cast<int64_t &>(rec->playTimeObj), fd);
    AEFile::Write((int32_t)rec->field_0x08, fd);

    num = ((Station *) (&num))->getName();
    AEFile::Write(num, fd, (bool)true);

    AEFile::Write(reinterpret_cast<String &>(rec->field_0x188), fd, (bool)true);
    AEFile::Write((int32_t)rec->field_0x40, fd);
    AEFile::Write((int32_t)rec->killsText, fd);
    AEFile::Write(rec->rank, fd);
    AEFile::Write((int32_t)reinterpret_cast<Ship *>((intptr_t) rec->field_0x130)->getIndex(), fd);

    AEFile::Close(fd);
    return 1;
}

#pragma pack(push, 1)
struct OptFlagsBlob {
    uint8_t flag[0x3b];
};

struct UiFlagsBlob {
    uint8_t flag[0x38];
};
#pragma pack(pop)

static OptFlagsBlob *g_RSW_optFlags = nullptr;

static UiFlagsBlob *g_RSW_uiFlags = nullptr;

static int g_RSW_modVersion = 0;

void RecordHandler::recordStoreWrite(int slot) {
    String num, path;
    num.Set((long long) (slot));
    path = this->recordDir + num;

    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);

    uint8_t *visited = Globals::galaxy->getVisited();
    AEFile::Write((int32_t)0x87, fd);
    for (unsigned i = 0; i < 0x87; i++) {
        AEFile::Write((bool)(visited[i] != 0), fd);
    }

    AEFile::Write((int32_t)Globals::status->getCredits(), fd);
    AEFile::Write((int32_t)Globals::status->getRating(), fd);
    AEFile::Write(Globals::status->getPlayingTime(), fd);
    AEFile::Write((int32_t)Globals::status->getKills(), fd);
    AEFile::Write((int32_t)Globals::status->getMissionCount(), fd);
    AEFile::Write((int32_t)Globals::status->getLevel(), fd);
    AEFile::Write((int32_t)Globals::status->getLastXP(), fd);
    AEFile::Write((int32_t)Globals::status->getGoodsProduced(), fd);
    AEFile::Write((int32_t)Globals::status->getStationsVisited(), fd);
    AEFile::Write((int32_t)Globals::status->getCurrentCampaignMission(), fd);
    this->writeMission(Globals::status->getFreelanceMission(), fd);
    this->writeMission(reinterpret_cast<Mission *>(Globals::status->getCampaignMission()), fd);
    AEFile::Write((int32_t)Globals::status->getJumpgateUsed(), fd);
    AEFile::Write((int32_t)Globals::status->getCapturedCrates(), fd);
    AEFile::Write((int32_t)Globals::status->getBoughtEquipment(), fd);
    AEFile::Write((int32_t)Globals::status->getPirateKills(), fd);
    AEFile::Write((int32_t)Globals::status->field_80, fd);

    {
        Status *status = Globals::status;
        OptFlagsBlob *flags = reinterpret_cast<OptFlagsBlob *>(g_RSW_optFlags);
        UiFlagsBlob *ui = reinterpret_cast<UiFlagsBlob *>(g_RSW_uiFlags);

        AEFile::Write(status->field_7c, fd);
        AEFile::Write((int32_t)status->field_84, fd);
        AEFile::Write((int32_t)(intptr_t)status->field_90, fd);

        {
            Array<bool> *arr = status->field_94;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }
        {
            Array<bool> *arr = status->field_98;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }
        AEFile::Write((int32_t)status->field_9c, fd);
        AEFile::Write((int32_t)status->field_a0, fd);
        AEFile::Write((int32_t)status->field_a4, fd);
        AEFile::Write((int32_t)status->field_a8, fd);
        {
            Array<bool> *arr = status->field_ac;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }
        AEFile::Write((int32_t)status->field_b0, fd);
        {
            Array<bool> *arr = status->field_b4;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }
        AEFile::Write((int32_t)status->field_b8, fd);
        AEFile::Write(reinterpret_cast<int64_t &>(status->field_c8), fd);

        AEFile::Write(status->field_c8, fd);
        AEFile::Write((int32_t)status->field_cc, fd);
        AEFile::Write((int32_t)status->field_d0, fd);
        AEFile::Write((int32_t)status->field_d4, fd);
        AEFile::Write((int32_t)status->field_d8, fd);
        AEFile::Write((int32_t)status->field_dc, fd);
        AEFile::Write((int32_t)status->field_e0, fd);
        AEFile::Write((int32_t)status->field_e4, fd);
        AEFile::Write((int32_t)status->field_e8, fd);
        AEFile::Write((int32_t)status->field_ec, fd);

        int *medals = Globals::achievements->getMedals();
        AEFile::Write((int32_t)0x2d, fd);
        for (unsigned i = 0; i < 0x2d; i++) {
            AEFile::Write((int32_t)medals[i], fd);
        }

        Ship *ship = status->getShip();
        AEFile::Write((int32_t)((Ship *) ship)->getIndex(), fd);
        AEFile::Write((int32_t)((Ship *) ship)->getRace(), fd);
        {
            Array<Item *> *eq = ((Ship *) ship)->getEquipment();
            if (eq == 0) {
                AEFile::Write((int32_t)0, fd);
            } else {
                AEFile::Write((int32_t)eq->size(), fd);
                for (unsigned i = 0; i < eq->size(); i++) {
                    Item *it = (*eq)[i];
                    if (it == 0) {
                        AEFile::Write((int32_t)-1, fd);
                        continue;
                    }
                    AEFile::Write((int32_t)((Item *) (it))->getIndex(), fd);
                    AEFile::Write((int32_t)((Item *) (it))->getAmount(), fd);
                    AEFile::Write((bool)((Item *) (it))->isUnsaleable(), fd);
                }
            }
        }
        {
            Array<Item *> *cg = ((Ship *) ship)->getCargo();
            if (cg == 0) {
                AEFile::Write((int32_t)0, fd);
            } else {
                AEFile::Write((int32_t)cg->size(), fd);
                for (unsigned i = 0; i < cg->size(); i++) {
                    Item *it = (*cg)[i];
                    AEFile::Write((int32_t)((Item *) (it))->getIndex(), fd);
                    AEFile::Write((int32_t)((Item *) (it))->getAmount(), fd);
                    AEFile::Write((int32_t)((Item *) (it))->getSinglePrice(), fd);
                    AEFile::Write((bool)((Item *) (it))->isUnsaleable(), fd);
                }
            }
        }

        Array<Station *> *stack = status->getStationStack();
        AEFile::Write((int32_t)3, fd);
        for (unsigned i = 0; i < stack->size() + 1; i++) {
            Station *cur = (i == stack->size())
                               ? (Station *) status->getStation()
                               : (*stack)[i];
            if (cur == 0) {
                AEFile::Write((int32_t)-1, fd);
                continue;
            }
            AEFile::Write((int32_t)((Station *) cur)->getIndex(), fd);

            Array<Item *> *items = (Array<Item *> *) ((Station *) cur)->getItems();
            if (items == 0) { AEFile::Write((int32_t)0, fd); } else {
                AEFile::Write((int32_t)items->size(), fd);
                for (unsigned j = 0; j < items->size(); j++) {
                    Item *it = (*items)[j];
                    AEFile::Write((int32_t)((Item *) (it))->getIndex(), fd);
                    AEFile::Write((int32_t)((Item *) (it))->getAmount(), fd);
                    AEFile::Write((int32_t)((Item *) (it))->getSinglePrice(), fd);
                    AEFile::Write((bool)((Item *) (it))->isUnsaleable(), fd);
                }
            }
            Array<Ship *> *ships = (Array<Ship *> *) ((Station *) cur)->getShips();
            if (ships == 0) { AEFile::Write((int32_t)0, fd); } else {
                AEFile::Write((int32_t)ships->size(), fd);
                for (unsigned j = 0; j < ships->size(); j++) {
                    Ship *s = (*ships)[j];
                    AEFile::Write((int32_t)((Ship *) s)->getIndex(), fd);
                    AEFile::Write((int32_t)((Ship *) s)->getRace(), fd);
                }
            }
            Array<Agent *> *agents = (Array<Agent *> *) ((Station *) cur)->getAgents();
            if (agents == 0) { AEFile::Write((int32_t)0, fd); } else {
                AEFile::Write((int32_t)agents->size(), fd);
                for (unsigned j = 0; j < agents->size(); j++) {
                    this->writeAgent((*agents)[j], fd);
                }
            }
            AEFile::Write((bool)((Station *) cur)->hasAttackedFriends(), fd);
        }

        int *standings = ((Standing *) (status->getStanding()))->getStandings();
        AEFile::Write((int32_t)2, fd);
        for (unsigned i = 0; i < 2; i++) {
            AEFile::Write((int32_t)standings[i], fd);
        }

        Array<BluePrint *> *bps = status->bluePrints;
        AEFile::Write((int32_t)bps->size(), fd);
        for (unsigned i = 0; i < bps->size(); i++) {
            BluePrint *bp = (*bps)[i];
            Array<int> *ingredients = bp->ingredientCounters;
            for (unsigned j = 0; j < ingredients->size(); j++) {
                AEFile::Write((int32_t)(*ingredients)[j], fd);
            }
            AEFile::Write(bp->spentValue, fd);
            AEFile::Write((bool)(bp->locked != 0), fd);
            AEFile::Write(bp->productionCount, fd);
            AEFile::Write(bp->stationIndex, fd);
            AEFile::Write(bp->stationName, fd, (bool)true);
        }

        Array<PendingProduct *> *pending = status->pendingProducts;
        if (pending == 0) { AEFile::Write((int32_t)-1, fd); } else {
            int count = 0;
            for (unsigned i = 0; i < pending->size(); i++) {
                if ((*pending)[i] != 0) count++;
            }
            if (count == 0) { AEFile::Write((int32_t)-1, fd); } else {
                AEFile::Write((int32_t)count, fd);
                for (unsigned i = 0; i < pending->size(); i++) {
                    PendingProduct *pp = (*pending)[i];
                    if (pp == 0) continue;

                    AEFile::Write((int32_t)pp->blueprintIndex, fd);
                    AEFile::Write((int32_t)pp->quantity, fd);
                    AEFile::Write((int32_t)pp->stationIndex, fd);
                    AEFile::Write(pp->stationName, fd, (bool)true);
                }
            }
        }

        Array<String *> *wingmen = reinterpret_cast<Array<String *> *>((intptr_t) status->wingmen);

        if (wingmen == 0) { AEFile::Write((int32_t)-1, fd); } else {
            AEFile::Write((int32_t)wingmen->size(), fd);
            for (unsigned i = 0; i < wingmen->size(); i++) {
                AEFile::Write(*(*wingmen)[i], fd, (bool)true);
            }
            AEFile::Write((int32_t)status->fadeValue, fd);

            AEFile::Write((int32_t)status->fadeValue, fd);
            AEFile::Write((int32_t)5, fd);

            int *wingmenStats = reinterpret_cast<int *>((intptr_t) reinterpret_cast<int32_t &>(status->_byte_0x28));
            for (unsigned i = 0; i < 5; i++) {
                AEFile::Write((int32_t)wingmenStats[i], fd);
            }
        }

        AEFile::Write(reinterpret_cast<int32_t &>(status->_byte_0x34), fd);
        {
            Array<bool> *arr = status->systemVisibilities;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }
        {
            Array<int> *arr = status->field_0x40;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((int32_t)(*arr)[i], fd);
            }
        }
        {
            Array<int> *arr = status->field_0x3c;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((int32_t)(*arr)[i], fd);
            }
        }
        {
            Array<int> *arr = status->field_0x48;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((int32_t)(*arr)[i], fd);
            }
        }
        {
            Array<int> *arr = status->field_0x44;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((int32_t)(*arr)[i], fd);
            }
        }
        {
            Array<bool> *arr = status->field_4c;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }

        Array<Agent *> *agents = status->agents;
        AEFile::Write((int32_t)agents->size(), fd);
        for (unsigned i = 0; i < agents->size(); i++) {
            this->writeAgent((*agents)[i], fd);
        }

        for (int off = 0x8; off <= 0x20; off++) {
            if (off == 0x21) continue;
            AEFile::Write((bool)(flags->flag[off] != 0), fd);
        }
        AEFile::Write((bool)(flags->flag[0x22] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x21] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x23] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x24] != 0), fd);

        AEFile::Write(reinterpret_cast<int64_t &>(status->field_10c), fd);

        AEFile::Write((bool)(flags->flag[0x25] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x26] != 0), fd);

        Ship *wingShip = reinterpret_cast<Ship *>(status->field_94);

        if (wingShip == 0) { AEFile::Write((int32_t)0, fd); } else {
            AEFile::Write((int32_t)1, fd);
            AEFile::Write((int32_t)((Ship *) wingShip)->getIndex(), fd);
            AEFile::Write((int32_t)((Ship *) wingShip)->getRace(), fd);
            {
                Array<Item *> *eq = ((Ship *) wingShip)->getEquipment();
                if (eq == 0) {
                    AEFile::Write((int32_t)0, fd);
                } else {
                    AEFile::Write((int32_t)eq->size(), fd);
                    for (unsigned i = 0; i < eq->size(); i++) {
                        Item *it = (*eq)[i];
                        if (it == 0) {
                            AEFile::Write((int32_t)-1, fd);
                            continue;
                        }
                        AEFile::Write((int32_t)((Item *) (it))->getIndex(), fd);
                        AEFile::Write((int32_t)((Item *) (it))->getAmount(), fd);
                        AEFile::Write((bool)((Item *) (it))->isUnsaleable(), fd);
                    }
                }
            }
            {
                Array<Item *> *cg = ((Ship *) wingShip)->getCargo();
                if (cg == 0) {
                    AEFile::Write((int32_t)0, fd);
                } else {
                    AEFile::Write((int32_t)cg->size(), fd);
                    for (unsigned i = 0; i < cg->size(); i++) {
                        Item *it = (*cg)[i];
                        AEFile::Write((int32_t)((Item *) (it))->getIndex(), fd);
                        AEFile::Write((int32_t)((Item *) (it))->getAmount(), fd);
                        AEFile::Write((int32_t)((Item *) (it))->getSinglePrice(), fd);
                        AEFile::Write((bool)((Item *) (it))->isUnsaleable(), fd);
                    }
                }
            }
        }

        {
            Array<int> *arr = status->field_90;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((int32_t)(*arr)[i], fd);
            }
        }
        AEFile::Write(status->field_10c, fd);
        AEFile::Write((bool)status->field_110, fd);
        AEFile::Write(status->field_114, fd);
        AEFile::Write((bool)status->field_0x111, fd);

        Station *station = (Station *) status->getStation();
        Array<Item *> *sItems = (Array<Item *> *) ((Station *) station)->getItems();
        if (sItems == 0) { AEFile::Write((int32_t)0, fd); } else {
            AEFile::Write((int32_t)sItems->size(), fd);
            for (unsigned i = 0; i < sItems->size(); i++) {
                Item *it = (*sItems)[i];
                AEFile::Write((int32_t)((Item *) (it))->getIndex(), fd);
                AEFile::Write((int32_t)((Item *) (it))->getAmount(), fd);
                AEFile::Write((int32_t)((Item *) (it))->getSinglePrice(), fd);
                AEFile::Write((bool)((Item *) (it))->isUnsaleable(), fd);
            }
        }
        Array<Ship *> *sShips = (Array<Ship *> *) ((Station *) station)->getShips();
        if (sShips == 0) { AEFile::Write((int32_t)0, fd); } else {
            AEFile::Write((int32_t)sShips->size(), fd);
            for (unsigned i = 0; i < sShips->size(); i++) {
                Ship *s = (*sShips)[i];
                AEFile::Write((int32_t)((Ship *) s)->getIndex(), fd);
                AEFile::Write((int32_t)((Ship *) s)->getRace(), fd);
            }
        }

        AEFile::Write((bool)(flags->flag[0x27] != 0), fd);
        AEFile::Write((bool)(ui->flag[0x35] != 0), fd);
        AEFile::Write((bool)(ui->flag[0x36] != 0), fd);
        {
            Array<bool> *arr = status->field_54;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }

        AEFile::Write((int32_t)g_RSW_modVersion, fd);
        {
            Ship *mShip = ship;
            Array<int> *mods = (mShip == 0) ? 0 : ((Ship *) mShip)->getMods();
            if (mShip == 0 || mods == 0) {
                AEFile::Write((int32_t)0, fd);
            } else {
                AEFile::Write((int32_t)mods->size(), fd);
                for (unsigned i = 0; i < mods->size(); i++) {
                    AEFile::Write((int32_t)(*mods)[i], fd);
                }
            }
        }
        {
            Ship *mShip = wingShip;
            Array<int> *mods = (mShip == 0) ? 0 : ((Ship *) mShip)->getMods();
            if (mShip == 0 || mods == 0) {
                AEFile::Write((int32_t)0, fd);
            } else {
                AEFile::Write((int32_t)mods->size(), fd);
                for (unsigned i = 0; i < mods->size(); i++) {
                    AEFile::Write((int32_t)(*mods)[i], fd);
                }
            }
        }
        if (sShips == 0) { AEFile::Write((int32_t)0, fd); } else {
            AEFile::Write((int32_t)sShips->size(), fd);
            for (unsigned i = 0; i < sShips->size(); i++) {
                Ship *mShip = (*sShips)[i];
                Array<int> *mods = (mShip == 0) ? 0 : ((Ship *) mShip)->getMods();
                if (mShip == 0 || mods == 0) {
                    AEFile::Write((int32_t)0, fd);
                } else {
                    AEFile::Write((int32_t)mods->size(), fd);
                    for (unsigned k = 0; k < mods->size(); k++) {
                        AEFile::Write((int32_t)(*mods)[k], fd);
                    }
                }
            }
        }
        for (unsigned i = 0; i < stack->size() + 1; i++) {
            Station *cur = (i == stack->size())
                               ? (Station *) status->getStation()
                               : (*stack)[i];
            if (cur == 0) continue;
            Array<Ship *> *cShips = (Array<Ship *> *) ((Station *) cur)->getShips();
            if (cShips == 0) {
                AEFile::Write((int32_t)0, fd);
                continue;
            }
            AEFile::Write((int32_t)cShips->size(), fd);
            for (unsigned j = 0; j < cShips->size(); j++) {
                Ship *mShip = (*cShips)[j];
                Array<int> *mods = (mShip == 0) ? 0 : ((Ship *) mShip)->getMods();
                if (mShip == 0 || mods == 0) {
                    AEFile::Write((int32_t)0, fd);
                } else {
                    AEFile::Write((int32_t)mods->size(), fd);
                    for (unsigned k = 0; k < mods->size(); k++) {
                        AEFile::Write((int32_t)(*mods)[k], fd);
                    }
                }
            }
        }

        Array<Wanted *> *wanted = status->wanted;
        AEFile::Write((int32_t)wanted->size(), fd);
        for (unsigned i = 0; i < wanted->size(); i++) {
            this->writeWanted((*wanted)[i], fd);
        }
        for (unsigned i = 0; i < 4; i++) {
            AEFile::Write((int32_t)status->getCollectedBounties(i), fd);
        }
        AEFile::Write((bool)(ui->flag[0x37] != 0), fd);
        AEFile::Write((int32_t)status->field_178, fd);
        AEFile::Write((bool)(flags->flag[0x28] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x29] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x2c] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x2a] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x2b] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x2e] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x2f] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x30] != 0), fd);
        {
            Array<bool> *arr = status->field_58;
            AEFile::Write((int32_t)arr->size(), fd);
            for (unsigned i = 0; i < arr->size(); i++) {
                AEFile::Write((bool)(*arr)[i], fd);
            }
        }
        AEFile::Write((bool)(flags->flag[0x31] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x2d] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x32] != 0), fd);
        AEFile::Write((int32_t)status->field_118, fd);
        AEFile::Write((bool)(flags->flag[0x33] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x34] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x35] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x36] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x37] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x38] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x39] != 0), fd);
        AEFile::Write((bool)(flags->flag[0x3a] != 0), fd);
    }

    AEFile::Close(fd);
    this->addHash(slot);

    return;
}

void *Galaxy_getStationByIndex(int idx); // lint: void_ptr (external symbol; mangled signature is fixed)

void *Standing_new(); // lint: void_ptr (external symbol; mangled signature is fixed)

void Standing_setStandingsArr(void *st, int *standings); // lint: void_ptr (external symbol; mangled signature is fixed)

void *RH_str_make(void *src); // lint: void_ptr (external symbol; mangled signature is fixed)

static Item ***g_RSR_itemDefs = nullptr;

static Ship ***g_RSR_shipDefs = nullptr;

static int g_RSR_modVersion = 0;

int RecordHandler::readOptionsFileAsByteArray(signed char **out) {
    String tmp(this->optionsPath, false);
    unsigned int fd;
    int sz;
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

GameRecord *RecordHandler::recordStoreRead(int slot) {
    GameRecord *rec = 0;
    String num, path;
    num.Set((long long) (slot));
    path = this->recordDir + num;

    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);
        int valid = this->checkHash(fd);
        AEFile::Close(fd);
        if (valid != 0) {
            AEFile::OpenRead(path, &fd);

            rec = new GameRecord();

            rec->field_0x04 = 0;
            AEFile::Read(rec->field_0x04, fd);
            bool *visitedFlags = reinterpret_cast<bool *>(rec->data);
            for (unsigned i = 0; i < rec->field_0x04; i++) {
                AEFile::Read(visitedFlags[i], fd);
            }

            AEFile::Read(rec->field_0x08, fd);
            AEFile::Read(rec->field_0x0c, fd);
            AEFile::Read(reinterpret_cast<int64_t &>(rec->playTimeObj), fd);
            AEFile::Read(rec->field_0x18, fd);
            AEFile::Read(rec->field_0x1c, fd);
            AEFile::Read(rec->killsText, fd);
            AEFile::Read(rec->field_0x24, fd);
            AEFile::Read(rec->field_0x28, fd);
            AEFile::Read(rec->field_0x3c, fd);
            AEFile::Read(rec->field_0x40, fd);
            *reinterpret_cast<Mission **>(&rec->field_0x54) = (Mission *) this->readMission(fd);
            *reinterpret_cast<Mission **>(&rec->field_0x58) = (Mission *) this->readMission(fd);
            AEFile::Read(rec->field_0x30, fd);
            AEFile::Read(rec->field_0x34, fd);
            AEFile::Read(rec->field_0x38, fd);
            AEFile::Read(rec->field_0x2c, fd);
            AEFile::Read(rec->field_0x44, fd);
            AEFile::Read(rec->field_0x48, fd);
            AEFile::Read(rec->field_0x4c, fd);
            AEFile::Read(rec->field_0x50, fd);

            Array<bool> *flags0 = new Array<bool>();
            *reinterpret_cast<Array<bool> **>(&rec->field_0x68) = flags0;
            unsigned n0 = 0;
            AEFile::Read(n0, fd);
            ArraySetLength(n0, *flags0);
            for (int i = 0; i < (int) n0; i++) {
                bool v = false;
                AEFile::Read(v, fd);
                (*flags0)[i] = v;
            }
            Array<bool> *flags1 = new Array<bool>();
            *reinterpret_cast<Array<bool> **>(&rec->field_0x6c) = flags1;
            unsigned n1 = 0;
            AEFile::Read(n1, fd);
            ArraySetLength(n1, *flags1);
            for (int i = 0; i < (int) n1; i++) {
                bool v = false;
                AEFile::Read(v, fd);
                (*flags1)[i] = v;
            }

            {
                Ship **shipDefs = *g_RSR_shipDefs;

                AEFile::Read(rec->field_0x74, fd);
                AEFile::Read(rec->field_0x78, fd);
                AEFile::Read(rec->field_0x7c, fd);
                AEFile::Read(rec->field_0x80, fd);

                Array<bool> *flags84 = new Array<bool>();
                *reinterpret_cast<Array<bool> **>(&rec->field_0x84) = flags84;
                int n84 = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(n84), fd);
                ArraySetLength(n84, *flags84);
                for (int i = 0; i < n84; i++) {
                    bool v = false;
                    AEFile::Read(v, fd);
                    (*flags84)[i] = v;
                }

                AEFile::Read(rec->field_0x88, fd);

                Array<bool> *flags8c = new Array<bool>();
                *reinterpret_cast<Array<bool> **>(&rec->field_0x8c) = flags8c;
                int n8c = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(n8c), fd);
                ArraySetLength(n8c, *flags8c);
                for (int i = 0; i < n8c; i++) {
                    bool v = false;
                    AEFile::Read(v, fd);
                    (*flags8c)[i] = v;
                }

                AEFile::Read(rec->field_0x90, fd);
                AEFile::Read(reinterpret_cast<int64_t &>(rec->field_0x98), fd);

                AEFile::Read(rec->field_0xa0, fd);
                AEFile::Read(rec->field_0xa4, fd);
                AEFile::Read(rec->field_0xa8, fd);
                AEFile::Read(rec->field_0xac, fd);
                AEFile::Read(rec->field_0xb0, fd);
                AEFile::Read(rec->field_0xb4, fd);
                AEFile::Read(rec->field_0xb8, fd);
                AEFile::Read(rec->field_0xbc, fd);
                AEFile::Read(rec->field_0xc0, fd);
                AEFile::Read(rec->field_0xc4, fd);

                AEFile::Read(rec->field_0x64, fd);
                unsigned len60 = rec->field_0x64;
                int *buf60 = new int[len60];
                *reinterpret_cast<int **>(&rec->field_0x60) = buf60;
                for (unsigned i = 0; i < len60; i++) { AEFile::Read(reinterpret_cast<int32_t &>(buf60[i]), fd); }

                int shipIdx = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(shipIdx), fd);
                Ship *pship = ((Ship *) ((int) (intptr_t) shipDefs[shipIdx]))->makeShip(-1);
                *reinterpret_cast<Ship **>(&rec->field_0x130) = pship;
                int race = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(race), fd);
                ((Ship *) (pship))->setRace(race);

                int eqN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(eqN), fd);
                if (eqN > 0) {
                    Array<Item *> *items = new Array<Item *>();
                    ArraySetLength(eqN, *items);
                    for (int i = 0; i < eqN; i++) {
                        int idx = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
                        if (idx == -1) {
                            (*items)[i] = (Item *) 0;
                        } else {
                            int amount = 0;
                            bool uns = false;
                            AEFile::Read(reinterpret_cast<int32_t &>(amount), fd);
                            AEFile::Read(uns, fd);
                            Item *def = (*g_RSR_itemDefs)[idx];
                            ((Item *) (def))->getMaxPrice();
                            Item *it = def->makeItem(amount);
                            ((Item *) (it))->setUnsaleable(uns);
                            (*items)[i] = (Item *) it;
                        }
                    }
                    ((Ship *) (pship))->replaceEquipment(items);
                }
                int cgN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(cgN), fd);
                if (cgN > 0) {
                    Array<Item *> *items = new Array<Item *>();
                    ArraySetLength(cgN, *items);
                    for (int i = 0; i < cgN; i++) {
                        int idx = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
                        if (idx == -1) {
                            (*items)[i] = (Item *) 0;
                        } else {
                            int amount = 0, price = 0;
                            bool uns = false;
                            AEFile::Read(reinterpret_cast<int32_t &>(amount), fd);
                            AEFile::Read(reinterpret_cast<int32_t &>(price), fd);
                            AEFile::Read(uns, fd);
                            Item *def = (*g_RSR_itemDefs)[idx];
                            ((Item *) (def))->getMaxPrice();
                            Item *it = def->makeItem(amount);
                            ((Item *) (it))->setPrice(price);
                            ((Item *) (it))->setUnsaleable(uns);
                            (*items)[i] = (Item *) it;
                        }
                    }
                    ((Ship *) (pship))->replaceCargo(items);
                }

                Array<Station *> *stationArr = new Array<Station *>();
                int stN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(stN), fd);
                ArraySetLength(stN, *stationArr);
                unsigned stCount = stationArr->size();
                for (unsigned k = 0; k < stCount + 1; k++) {
                    int sIdx = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(sIdx), fd);
                    Station *st = 0;
                    if (sIdx != -1) {
                        st = (Station *) Galaxy_getStationByIndex(sIdx);
                        int iN = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(iN), fd);
                        if (iN > 0) {
                            Array<Item *> *items = new Array<Item *>();
                            ArraySetLength(iN, *items);
                            for (int i = 0; i < iN; i++) {
                                int idx = 0;
                                AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
                                if (idx == -1) {
                                    (*items)[i] = (Item *) 0;
                                } else {
                                    int amount = 0, price = 0;
                                    bool uns = false;
                                    AEFile::Read(reinterpret_cast<int32_t &>(amount), fd);
                                    AEFile::Read(reinterpret_cast<int32_t &>(price), fd);
                                    AEFile::Read(uns, fd);
                                    Item *def = (*g_RSR_itemDefs)[idx];
                                    ((Item *) (def))->getMaxPrice();
                                    Item *it = def->makeItem(amount);
                                    ((Item *) (it))->setPrice(price);
                                    ((Item *) (it))->setUnsaleable(uns);
                                    (*items)[i] = (Item *) it;
                                }
                            }
                            ((Station *) (st))->setItems(items, false);
                        }
                        int shN = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(shN), fd);
                        if (shN > 0) {
                            Array<Ship *> *ships = new Array<Ship *>();
                            ArraySetLength(shN, *ships);
                            for (int i = 0; i < shN; i++) {
                                int si = 0;
                                AEFile::Read(reinterpret_cast<int32_t &>(si), fd);
                                Ship *sh = ((Ship *) ((int) (intptr_t) shipDefs[si]))->makeShip(-1);
                                (*ships)[i] = (Ship *) sh;
                                int r = 0;
                                AEFile::Read(reinterpret_cast<int32_t &>(r), fd);
                                ((Ship *) (sh))->setRace(r);
                            }
                            ((Station *) (st))->setShips(ships, false);
                        }
                        int agN = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(agN), fd);
                        if (agN > 0) {
                            Array<Agent *> *agents = new Array<Agent *>();
                            ArraySetLength(agN, *agents);
                            for (int i = 0; i < agN; i++)
                                (*agents)[i] = (Agent *) this->readAgent(fd);
                            ((Station *) (st))->setAgents(agents);
                        }
                        bool atk = false;
                        AEFile::Read(atk, fd);
                        ((Station *) (st))->setAttackedFriends(atk);
                    }
                    if (k == stationArr->size()) *reinterpret_cast<Station **>(&rec->field_0x138) = st;
                    else (*stationArr)[k] = (Station *) st;
                }
                *reinterpret_cast<Array<Station *> **>(&rec->field_0x5c) = stationArr;

                int sdN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(sdN), fd);
                int *standings = new int[sdN];
                for (int i = 0; i < sdN; i++) AEFile::Read(reinterpret_cast<int32_t &>(standings[i]), fd);
                Standing *standing = (Standing *) Standing_new();
                *reinterpret_cast<Standing **>(&rec->field_0x13c) = standing;
                Standing_setStandingsArr(standing, standings);

                Array<BluePrint *> *bpArr = new Array<BluePrint *>();
                int bpN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(bpN), fd);
                ArraySetLength(bpN, *bpArr);
                for (unsigned i = 0; i < bpArr->size(); i++) {
                    Array<BluePrint *> *liveBps = Globals::status->bluePrints;
                    int liveIdx = (*liveBps)[i]->getIndex();
                    BluePrint *bp = new BluePrint(liveIdx);
                    (*bpArr)[i] = bp;
                    Array<int> *ingredients = bp->ingredientCounters;
                    for (unsigned j = 0; j < ingredients->size(); j++) {
                        AEFile::Read(reinterpret_cast<int32_t &>((*ingredients)[j]), fd);
                    }
                    AEFile::Read(bp->spentValue, fd);
                    AEFile::Read(bp->locked, fd);
                    AEFile::Read(bp->productionCount, fd);
                    AEFile::Read(bp->stationIndex, fd);
                    String tmp;
                    {
                        if (tmp.data) delete[] tmp.data;
                        tmp.data = nullptr;
                        tmp.length = 0;
                    }
                    AEFile::Read(tmp, fd, (bool)true);
                    bp->stationName = tmp;
                }
                *reinterpret_cast<Array<BluePrint *> **>(&rec->field_0x140) = bpArr;

                int ppN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(ppN), fd);
                if (ppN < 1) { *reinterpret_cast<Array<PendingProduct *> **>(&rec->field_0x144) = 0; } else {
                    Array<PendingProduct *> *ppArr = new Array<PendingProduct *>();
                    ArraySetLength(ppN, *ppArr);
                    for (unsigned i = 0; i < ppArr->size(); i++) {
                        int a = 0, c = 0, d = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(a), fd);
                        AEFile::Read(reinterpret_cast<int32_t &>(c), fd);
                        AEFile::Read(reinterpret_cast<int32_t &>(d), fd);
                        String nm;
                        {
                            if (nm.data) delete[] nm.data;
                            nm.data = nullptr;
                            nm.length = 0;
                        }
                        AEFile::Read(nm, fd, (bool)true);
                        String *nameCopy = (String *) RH_str_make(&nm);
                        PendingProduct *pp = new PendingProduct(a, *nameCopy, d, c);
                        (*ppArr)[i] = pp;
                    }
                    *reinterpret_cast<Array<PendingProduct *> **>(&rec->field_0x144) = ppArr;
                }

                int wmN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(wmN), fd);
                if (wmN < 1) { *reinterpret_cast<Array<String *> **>(&rec->field_0x14c) = 0; } else {
                    Array<String *> *strArr = new Array<String *>();
                    ArraySetLength(wmN, *strArr);
                    for (int i = 0; i < wmN; i++) {
                        String nm;
                        {
                            if (nm.data) delete[] nm.data;
                            nm.data = nullptr;
                            nm.length = 0;
                        }
                        AEFile::Read(nm, fd, (bool)true);
                        (*strArr)[i] = (String *) RH_str_make(&nm);
                    }
                    *reinterpret_cast<Array<String *> **>(&rec->field_0x14c) = strArr;
                    AEFile::Read(rec->field_0x150, fd);
                    AEFile::Read(rec->field_0x154, fd);
                    int cnt = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(cnt), fd);
                    int *buf158 = new int[cnt];
                    *reinterpret_cast<int **>(&rec->field_0x158) = buf158;
                    for (int i = 0; i < cnt; i++) AEFile::Read(reinterpret_cast<int32_t &>(buf158[i]), fd);
                }

                AEFile::Read(rec->field_0x15c, fd);

                int b160 = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(b160), fd);
                Array<bool> *arr160 = new Array<bool>();
                *reinterpret_cast<Array<bool> **>(&rec->field_0x160) = arr160;
                ArraySetLength(b160, *arr160);
                for (unsigned i = 0; i < arr160->size(); i++) {
                    bool v = false;
                    AEFile::Read(v, fd);
                    (*arr160)[i] = v;
                }

                const int intArrOffs[4] = {0x168, 0x164, 0x170, 0x16c};
                for (int a = 0; a < 4; a++) {
                    int cnt = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(cnt), fd);
                    Array<int> *arr = new Array<int>();
                    *reinterpret_cast<Array<int> **>(reinterpret_cast<char *>(rec) + intArrOffs[a]) = arr;
                    ArraySetLength(cnt, *arr);
                    for (unsigned i = 0; i < arr->size(); i++) {
                        AEFile::Read(reinterpret_cast<int32_t &>((*arr)[i]), fd);
                    }
                }

                int b174 = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(b174), fd);
                Array<bool> *arr174 = new Array<bool>();
                *reinterpret_cast<Array<bool> **>(&rec->field_0x174) = arr174;
                ArraySetLength(b174, *arr174);
                for (unsigned i = 0; i < arr174->size(); i++) {
                    bool v = false;
                    AEFile::Read(v, fd);
                    (*arr174)[i] = v;
                }

                int agN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(agN), fd);
                Array<Agent *> *agents = new Array<Agent *>();
                *reinterpret_cast<Array<Agent *> **>(&rec->field_0x148) = agents;
                ArraySetLength(agN, *agents);
                for (unsigned i = 0; i < agents->size(); i++)
                    (*agents)[i] = (Agent *) this->readAgent(fd);

                uint8_t *recBytes = reinterpret_cast<uint8_t *>(rec);
                for (int off = 0xe4; off <= 0x100; off++) AEFile::Read(recBytes[off], fd);
                AEFile::Read(rec->rank, fd);
                AEFile::Read(reinterpret_cast<int64_t &>(rec->field_0xc8), fd);
                AEFile::Read(rec->field_0x101, fd);
                AEFile::Read(rec->field_0x102, fd);

                int hasWing = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(hasWing), fd);
                if (hasWing > 0) {
                    int wi = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(wi), fd);
                    Ship *wship = ((Ship *) ((int) (intptr_t) shipDefs[wi]))->makeShip(-1);
                    *reinterpret_cast<Ship **>(&rec->field_0x134) = wship;
                    int r = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(r), fd);
                    ((Ship *) (wship))->setRace(r);
                    int eN = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(eN), fd);
                    if (eN > 0) {
                        Array<Item *> *items = new Array<Item *>();
                        ArraySetLength(eN, *items);
                        for (int i = 0; i < eN; i++) {
                            int idx = 0;
                            AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
                            if (idx == -1) {
                                (*items)[i] = (Item *) 0;
                            } else {
                                int amount = 0;
                                bool uns = false;
                                AEFile::Read(reinterpret_cast<int32_t &>(amount), fd);
                                AEFile::Read(uns, fd);
                                Item *def = (*g_RSR_itemDefs)[idx];
                                ((Item *) (def))->getMaxPrice();
                                Item *it = def->makeItem(amount);
                                ((Item *) (it))->setUnsaleable(uns);
                                (*items)[i] = (Item *) it;
                            }
                        }
                        ((Ship *) (wship))->replaceEquipment(items);
                    }
                    int cN = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(cN), fd);
                    if (cN > 0) {
                        Array<Item *> *items = new Array<Item *>();
                        ArraySetLength(cN, *items);
                        for (int i = 0; i < cN; i++) {
                            int idx = 0;
                            AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
                            if (idx == -1) {
                                (*items)[i] = (Item *) 0;
                            } else {
                                int amount = 0, price = 0;
                                bool uns = false;
                                AEFile::Read(reinterpret_cast<int32_t &>(amount), fd);
                                AEFile::Read(reinterpret_cast<int32_t &>(price), fd);
                                AEFile::Read(uns, fd);
                                Item *def = (*g_RSR_itemDefs)[idx];
                                ((Item *) (def))->getMaxPrice();
                                Item *it = def->makeItem(amount);
                                ((Item *) (it))->setPrice(price);
                                ((Item *) (it))->setUnsaleable(uns);
                                (*items)[i] = (Item *) it;
                            }
                        }
                        ((Ship *) (wship))->replaceCargo(items);
                    }
                }

                Array<int> *arr70 = new Array<int>();
                *reinterpret_cast<Array<int> **>(&rec->field_0x70) = arr70;
                int n70 = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(n70), fd);
                ArraySetLength(n70, *arr70);
                for (int i = 0; i < n70; i++) { AEFile::Read(reinterpret_cast<int32_t &>((*arr70)[i]), fd); }

                AEFile::Read(rec->field_0xd0, fd);
                AEFile::Read(rec->field_0xd4, fd);
                AEFile::Read(rec->field_0xd8, fd);
                AEFile::Read(rec->field_0xdc, fd);

                int siN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(siN), fd);
                if (siN > 0) {
                    Array<Item *> *items = new Array<Item *>();
                    ArraySetLength(siN, *items);
                    for (int i = 0; i < siN; i++) {
                        int idx = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(idx), fd);
                        if (idx == -1) {
                            (*items)[i] = (Item *) 0;
                        } else {
                            int amount = 0, price = 0;
                            bool uns = false;
                            AEFile::Read(reinterpret_cast<int32_t &>(amount), fd);
                            AEFile::Read(reinterpret_cast<int32_t &>(price), fd);
                            AEFile::Read(uns, fd);
                            Item *def = (*g_RSR_itemDefs)[idx];
                            ((Item *) (def))->getMaxPrice();
                            Item *it = def->makeItem(amount);
                            ((Item *) (it))->setPrice(price);
                            ((Item *) (it))->setUnsaleable(uns);
                            (*items)[i] = (Item *) it;
                        }
                    }
                    *reinterpret_cast<Array<Item *> **>(&rec->field_0x180) = items;
                }
                int ssN = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(ssN), fd);
                Array<Ship *> *recShips = 0;
                if (ssN > 0) {
                    Array<Ship *> *ships = new Array<Ship *>();
                    ArraySetLength(ssN, *ships);
                    for (int i = 0; i < ssN; i++) {
                        int si = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(si), fd);
                        Ship *sh = ((Ship *) ((int) (intptr_t) shipDefs[si]))->makeShip(-1);
                        (*ships)[i] = (Ship *) sh;
                        int r = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(r), fd);
                        ((Ship *) (sh))->setRace(r);
                    }
                    *reinterpret_cast<Array<Ship *> **>(&rec->field_0x184) = ships;
                    recShips = ships;
                }

                AEFile::Read(rec->field_0x103, fd);
                AEFile::Read(rec->dlcRequiredFlag, fd);
                AEFile::Read(rec->field_0x116, fd);

                int b178 = 0;
                AEFile::Read(reinterpret_cast<int32_t &>(b178), fd);
                Array<bool> *arr178 = new Array<bool>();
                *reinterpret_cast<Array<bool> **>(&rec->field_0x178) = arr178;
                ArraySetLength(b178, *arr178);
                for (unsigned i = 0; i < arr178->size(); i++) {
                    bool v = false;
                    AEFile::Read(v, fd);
                    (*arr178)[i] = v;
                }

                AEFile::Read(rec->field_0x1bc, fd);
                if (rec->field_0x1bc == g_RSR_modVersion) {
                    int m0 = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(m0), fd);
                    if (m0 > 0) {
                        Array<int> *a = new Array<int>();
                        ArraySetLength(m0, *a);
                        for (int i = 0; i < m0; i++) { AEFile::Read(reinterpret_cast<int32_t &>((*a)[i]), fd); }
                        (*reinterpret_cast<Ship **>(&rec->field_0x130))->setMods(a);
                    }
                    int m1 = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(m1), fd);
                    if (m1 > 0) {
                        Array<int> *a = new Array<int>();
                        ArraySetLength(m1, *a);
                        for (int i = 0; i < m1; i++) { AEFile::Read(reinterpret_cast<int32_t &>((*a)[i]), fd); }
                        (*reinterpret_cast<Ship **>(&rec->field_0x134))->setMods(a);
                    }

                    int stShipGroups = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(stShipGroups), fd);
                    for (int g = 0; g < stShipGroups; g++) {
                        int mc = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(mc), fd);
                        if (mc > 0) {
                            Array<int> *a = new Array<int>();
                            ArraySetLength(mc, *a);
                            for (int i = 0; i < mc; i++) { AEFile::Read(reinterpret_cast<int32_t &>((*a)[i]), fd); }
                            ((Ship *) ((*recShips)[g]))->setMods(a);
                        }
                    }

                    Array<Station *> *stArr = *reinterpret_cast<Array<Station *> **>(&rec->field_0x5c);
                    for (unsigned k = 0; k < stArr->size() + 1; k++) {
                        Station *cur = (k == stArr->size())
                                           ? *reinterpret_cast<Station **>(&rec->field_0x138)
                                           : (*stArr)[k];
                        if (cur == 0) continue;
                        int groups = 0;
                        AEFile::Read(reinterpret_cast<int32_t &>(groups), fd);
                        for (int i = 0; i < groups; i++) {
                            int mc = 0;
                            AEFile::Read(reinterpret_cast<int32_t &>(mc), fd);
                            for (int j = 0; j < mc; j++) {
                                int mod = 0;
                                AEFile::Read(reinterpret_cast<int32_t &>(mod), fd);
                                Array<Ship *> *cShips = (Array<Ship *> *) ((Station *) cur)->getShips();
                                ((Ship *) ((*cShips)[i]))->addMod(mod);
                            }
                        }
                    }

                    int wN = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(wN), fd);
                    if (wN > 0) {
                        Array<Wanted *> *wArr = new Array<Wanted *>();
                        *reinterpret_cast<Array<Wanted *> **>(&rec->field_0x1b4) = wArr;
                        ArraySetLength(wN, *wArr);
                        for (int i = 0; i < wN; i++)
                            (*wArr)[i] = (Wanted *) this->readWanted(fd);
                    }

                    AEFile::Read(rec->field_0x1a4, fd);
                    AEFile::Read(rec->field_0x1a8, fd);
                    AEFile::Read(rec->field_0x1ac, fd);
                    AEFile::Read(rec->field_0x1b0, fd);
                    AEFile::Read(rec->versionMismatchFlag, fd);
                    AEFile::Read(rec->field_0x1b8, fd);
                    AEFile::Read(rec->field_0x104, fd);
                    AEFile::Read(rec->field_0x105, fd);
                    AEFile::Read(rec->field_0x108, fd);
                    AEFile::Read(rec->field_0x106, fd);
                    AEFile::Read(rec->field_0x107, fd);
                    AEFile::Read(rec->field_0x10a, fd);
                    AEFile::Read(rec->field_0x10b, fd);
                    AEFile::Read(rec->field_0x10c, fd);

                    int bX = 0;
                    AEFile::Read(reinterpret_cast<int32_t &>(bX), fd);
                    Array<bool> *arr17c = new Array<bool>();
                    *reinterpret_cast<Array<bool> **>(&rec->field_0x17c) = arr17c;
                    ArraySetLength(bX, *arr17c);
                    for (unsigned i = 0; i < arr17c->size(); i++) {
                        bool v = false;
                        AEFile::Read(v, fd);
                        (*arr17c)[i] = v;
                    }

                    AEFile::Read(rec->field_0x119, fd);
                    AEFile::Read(rec->field_0x109, fd);
                    AEFile::Read(rec->field_0x11a, fd);
                    AEFile::Read(rec->field_0xe0, fd);

                    uint8_t *recBytes2 = reinterpret_cast<uint8_t *>(rec);
                    for (int off = 0x10d; off <= 0x114; off++) AEFile::Read(recBytes2[off], fd);
                }
            }

            AEFile::Close(fd);
        }
    }

    return rec;
}

static const unsigned char RH_ch_salt[0x19] = {};

static unsigned char **RH_ch_key = nullptr;

bool RecordHandler::checkHash(unsigned int fd) {
    int result = 0;

    int sz = AEFile::GetFileSize(fd);
    if (-1 < sz) {
        unsigned char *buf = new unsigned char[(unsigned) sz];
        AEFile::Read((unsigned) sz, (signed char *) buf, fd);
        Array<unsigned char> *arr = new Array<unsigned char>();
        ArrayAdd(buf, (unsigned) sz, *arr);
        delete[] buf;

        unsigned len = arr->size();
        if (0x21 < len) {
            unsigned char *md = new unsigned char[0x20];
            unsigned char *c = new unsigned char[0x70];
            SHA256_Init(c);
            unsigned char *data = arr->data();
            SHA256_Update(c, data, (int) len - 0x20);
            SHA256_Update(c, RH_ch_salt, 0x19);
            SHA256_Update(c, *RH_ch_key, 0x10);
            SHA256_Final(md, c);

            result = (CRYPTO_memcmp(md, &data[(int) len - 0x20], 0x20) == 0) ? 1 : 0;
            delete[] md;
            delete[] c;
        }
    }

    return result;
}

static const unsigned char RH_aho_salt[0x19] = {};

static unsigned char **RH_aho_key = nullptr;

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
    unsigned long long *dst = (unsigned long long *) (out + len);
    unsigned long long *src = (unsigned long long *) md;
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
