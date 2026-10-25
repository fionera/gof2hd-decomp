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
#include <cstdint>
#include <cstddef>

namespace {
    // Byte-addressed flag view of a GameRecord's packed toggle bytes.
    inline bool &recordFlag(GameRecord *rec, int offset) {
        return reinterpret_cast<bool *>(rec)[offset];
    }

    // View of Globals::hints as raw boolean flags.
    struct HintFlags {
        bool flag[59];
    };

    // View of Globals::options as the options file serializes it (floats = volumes/sensitivities,
    // bools = toggles).
    struct OptionsBuffer {
        float f_0x00;
        float f_0x04;
        float volumeSfx;
        bool byte_0x0c;
        bool byte_0x0d;
        bool byte_0x0e;
        uint8_t pad_0x0f;
        bool byte_0x10;
        bool byte_0x11;
        uint8_t pad_0x12[2];
        float f_0x14;
        float f_0x18;
        float f_0x1c;
        float volumeMusic;
        float f_0x24;
        float f_0x28;
        float difficulty;
        uint8_t pad_0x30[2];
        bool byte_0x32;
        bool byte_0x33;
        bool byte_0x34;
        bool byte_0x35;
        bool byte_0x36;
        bool byte_0x37;
        bool byte_0x38;
        uint8_t pad_0x39;
        bool byte_0x3a;
        bool byte_0x3b;
        bool byte_0x3c;
        bool byte_0x3d;
        bool byte_0x3e;
        bool byte_0x3f;
        bool byte_0x40;
        bool byte_0x41;
        uint8_t pad_0x42[2];
        float volumeAmbient;
        bool byte_0x48;
        bool byte_0x49;
        bool byte_0x4a;
        bool byte_0x4b;
        bool byte_0x4c;
        bool byte_0x4d;
        bool byte_0x4e;
        uint8_t pad_0x4f;
        int32_t i32_0x50;
        int32_t i32_0x54;
        int32_t i32_0x58;
        uint8_t pad_0x5c[4];
        bool byte_0x60;
        bool byte_0x61;
        bool byte_0x62;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(offsetof(OptionsBuffer, volumeSfx) == 0x08, "OptionsBuffer.volumeSfx");
    static_assert(offsetof(OptionsBuffer, byte_0x11) == 0x11, "OptionsBuffer.byte_0x11");
    static_assert(offsetof(OptionsBuffer, volumeMusic) == 0x20, "OptionsBuffer.volumeMusic");
    static_assert(offsetof(OptionsBuffer, byte_0x32) == 0x32, "OptionsBuffer.byte_0x32");
    static_assert(offsetof(OptionsBuffer, volumeAmbient) == 0x44, "OptionsBuffer.volumeAmbient");
    static_assert(offsetof(OptionsBuffer, i32_0x50) == 0x50, "OptionsBuffer.i32_0x50");
    static_assert(offsetof(OptionsBuffer, byte_0x62) == 0x62, "OptionsBuffer.byte_0x62");
    static_assert(sizeof(OptionsBuffer) == 0x64, "OptionsBuffer size");
#endif
}

const char *g_android_origami_super_club = nullptr;

// The vendored SHA-256 API takes an opaque context pointer; this is the real
// libcrypto SHA256_CTX storage size.
struct Sha256Context {
    unsigned char state[0x70];
};

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_SetOrigamiSuperClub( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jobject /*thiz*/, jstring value) {
    jboolean isCopy;
    g_android_origami_super_club = env->GetStringUTFChars(value, &isCopy);
}

extern "C" void Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_ReleaseOrigamiSuperClub( // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    JNIEnv *env, jobject /*thiz*/, jstring value) {
    env->ReleaseStringUTFChars(value, g_android_origami_super_club);
}









Array<GameRecord *> *RecordHandler::readAllRecords() {
    Array<GameRecord *> *records = new Array<GameRecord *>();
    ArraySetLength(Globals::recordSlots, *records);
    for (int i = 0; i < Globals::recordSlots; i++) {
        (*records)[i] = this->recordStoreRead(i);
    }
    return records;
}

Array<GameRecord *> *RecordHandler::readAllPreviewRecords() {
    Array<GameRecord *> *records = new Array<GameRecord *>();
    ArraySetLength(Globals::recordSlots, *records);
    for (int i = 0; i < Globals::recordSlots; i++) {
        (*records)[i] = this->recordStoreReadPreview(i);
    }
    return records;
}

bool RecordHandler::notEnoughMemory() {
    return (int)AEFile::GetDeviceFreeSpace() < 900;
}

void RecordHandler::convertSDVersionSaves() {
    Array<signed char *> *records = new Array<signed char *>();
    ArraySetLength(Globals::recordSlots, *records);
    Array<signed char *> *previews = new Array<signed char *>();
    ArraySetLength(Globals::recordSlots, *previews);

    int *recordSizes = new int[Globals::recordSlots];
    int *previewSizes = new int[Globals::recordSlots];
    for (int i = 0; i < Globals::recordSlots; i++) {
        recordSizes[i] = this->readRecordAsByteArray(&(*records)[i], i, false);
        previewSizes[i] = this->readRecordAsByteArray(&(*previews)[i], i, true);
        AEFile::FileDelete(this->recordDir + String(i));
        AEFile::FileDelete(this->backupDir + String(i));
    }

    // SD-version slots are shifted up by one; the last slot wraps to slot 0.
    for (int i = 0; i < Globals::recordSlots - 1; i++) {
        if ((*records)[i] != 0) {
            this->writeByteArrayAsRecord((*records)[i], recordSizes[i], i + 1, false);
            this->addHash(i + 1);
            this->writeByteArrayAsRecord((*previews)[i], previewSizes[i], i + 1, true);
        }
    }
    int last = Globals::recordSlots - 1;
    if ((*records)[last] != 0) {
        this->writeByteArrayAsRecord((*records)[last], recordSizes[last], 0, false);
        this->addHash(0);
        this->writeByteArrayAsRecord((*previews)[Globals::recordSlots - 1], previewSizes[Globals::recordSlots - 1], 0,
                                     true);
    }

    ArrayReleaseArrays(*records);
    ArrayReleaseArrays(*previews);
    delete[] recordSizes;
    delete[] previewSizes;
}

void RecordHandler::addHash(int slot) {
    signed char *data = 0;
    int length = this->readRecordAsByteArray(&data, slot, false);
    if (length < 0) {
        return;
    }
    unsigned char *digest = new unsigned char[32];
    Sha256Context *context = new Sha256Context;
    SHA256_Init(context);
    SHA256_Update(context, data, length);
    char salt[] = "#+\xc2\xa7R0LL3r(0aSter&_%=$+#";
    SHA256_Update(context, salt, 25);
    SHA256_Update(context, g_android_origami_super_club, 16);
    SHA256_Final(digest, context);

    signed char *hashed = new signed char[length + 32];
    memcpy(hashed, data, length);
    memcpy(hashed + length, digest, 32);
    this->writeByteArrayAsRecord(hashed, length + 32, slot, false);

    delete[] data;
    data = 0;
    delete[] hashed;
    delete[] digest;
    delete context;
}

int RecordHandler::readRecordAsByteArray(signed char **out, int slot, bool fromBackup) {
    String path = (fromBackup ? this->backupDir : this->recordDir) + String(slot);
    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);
        int size = AEFile::GetFileSize(fd);
        *out = new signed char[size];
        AEFile::Read(size, *out, fd);
        AEFile::Close(fd);
        return size;
    }
    return -1;
}

GameRecord *RecordHandler::readRecord(int slot) {
    return this->recordStoreRead(slot);
}

void RecordHandler::recoverSDVersionSaves() {
}

Wanted *RecordHandler::readWanted(unsigned int fd) {
    bool active = false;
    bool terminated = false;
    int currentLocation = -1;
    int travelsTo = -1;
    int lastSeen = -1;
    AEFile::Read(active, fd);
    AEFile::Read(terminated, fd);
    AEFile::Read(currentLocation, fd);
    AEFile::Read(travelsTo, fd);
    AEFile::Read(lastSeen, fd);

    String name;

    int idx = 0, board = 0, race = 0;
    bool male = true;
    int ship = 0, weapon = 0, hp = 0, loot = 0, lootAmt = 0;
    int reward = 0, reqBounties = 0, reqMission = 0, numWingmen = 0;

    AEFile::Read(name, fd, true);
    AEFile::Read(idx, fd);
    AEFile::Read(board, fd);
    AEFile::Read(race, fd);
    AEFile::Read(male, fd);
    AEFile::Read(ship, fd);
    AEFile::Read(weapon, fd);
    AEFile::Read(hp, fd);
    AEFile::Read(loot, fd);
    AEFile::Read(lootAmt, fd);
    AEFile::Read(reward, fd);
    AEFile::Read(reqBounties, fd);
    AEFile::Read(reqMission, fd);
    AEFile::Read(numWingmen, fd);

    Wanted *w = new Wanted(idx, String(name, false), board, race, male, ship, weapon, hp, loot,
                           lootAmt, reward, reqBounties, reqMission, numWingmen);

    int *parts = new int[5];
    for (int i = 0; i < 5; i++) {
        AEFile::Read(parts[i], fd);
    }
    w->setImageParts(parts);
    w->setActive(active);
    w->setTerminated(terminated);
    w->setCurrentLocation(currentLocation);
    w->setTravelsTo(travelsTo);
    w->setLastSeen(lastSeen);

    return w;
}

void RecordHandler::changeSaveDirectoryToBackupDirectory() {
    Array<signed char *> *records = new Array<signed char *>();
    ArraySetLength(Globals::recordSlots, *records);
    Array<signed char *> *previews = new Array<signed char *>();
    ArraySetLength(Globals::recordSlots, *previews);

    int *recordSizes = new int[Globals::recordSlots];
    int *previewSizes = new int[Globals::recordSlots];
    for (int i = 0; i < Globals::recordSlots; i++) {
        recordSizes[i] = this->readRecordAsByteArray(&(*records)[i], i, false);
        previewSizes[i] = this->readRecordAsByteArray(&(*previews)[i], i, true);
    }

    AbyssEngine::Engine::backupSaveGames = 1;
    for (int i = 0; i < Globals::recordSlots; i++) {
        if ((*records)[i] != 0) {
            this->writeByteArrayAsRecord((*records)[i], recordSizes[i], i, false);
            this->addHash(i + 1);
            this->writeByteArrayAsRecord((*previews)[i], previewSizes[i], i, true);
        }
    }

    ArrayReleaseArrays(*records);
    ArrayReleaseArrays(*previews);
    delete[] recordSizes;
    delete[] previewSizes;
}

GameRecord *RecordHandler::recordStoreReadPreview(int slot) {
    String path = this->backupDir + String(slot);
    GameRecord *gr = 0;
    if (AEFile::FileExist(path) != 0) {
        unsigned int fd;
        AEFile::OpenRead(path, &fd);
        gr = new GameRecord();

        AEFile::Read(gr->playTime, fd);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->field_0x08), fd);
        AEFile::Read(gr->pilotName, fd, true);
        AEFile::Read(gr->field_0x188, fd, true);
        AEFile::Read(gr->field_0x40, fd);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->killsText), fd);
        AEFile::Read(gr->rank, fd);
        AEFile::Read(reinterpret_cast<int32_t &>(gr->shipId), fd);
        AEFile::Close(fd);
    }
    return gr;
}

RecordHandler::RecordHandler() {
    optionsPath = String("gof2_save_options");
    recordDir = String("gof2_save_game_");
    backupDir = String("gof2_save_game_preview_");
}

RecordHandler::~RecordHandler() {
}

int RecordHandler::writeByteArrayAsOptionsFile(signed char *buf, int n) {
    String tmp(this->optionsPath, false);
    if (AEFile::FileExist(tmp) != 0)
        AEFile::FileDelete(tmp);
    unsigned int fd;
    AEFile::OpenWrite(tmp, &fd);
    AEFile::Write((uint32_t)n, buf, fd);
    AEFile::Close(fd);
    return n;
}

int RecordHandler::recordStoreWritePreview(int slot) {
    String path = this->backupDir + String(slot);
    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);
    AEFile::Write(Globals::status->getPlayingTime(), fd);
    AEFile::Write((int32_t) Globals::status->getCredits(), fd);
    AEFile::Write(Globals::status->getStation()->getName(), fd, true);
    AEFile::Write(reinterpret_cast<SolarSystem *>(Globals::status->getSystem())->getName(), fd, true);
    AEFile::Write((int32_t) Globals::status->getCurrentCampaignMission(), fd);
    AEFile::Write((int32_t) Globals::status->getLevel(), fd);
    AEFile::Write(reinterpret_cast<OptionsBuffer *>(Globals::options)->difficulty, fd);
    AEFile::Write((int32_t) Globals::status->getShip()->getIndex(), fd);
    AEFile::Close(fd);
    return 1;
}



void RecordHandler::writeAgent(Agent *agentPtr, unsigned int fd) {
    AEFile::Write((int32_t)agentPtr->getCosts(), fd);
    AEFile::Write((int32_t)agentPtr->getSellSystemIndex(), fd);
    AEFile::Write((int32_t)agentPtr->getSellBlueprintIndex(), fd);
    AEFile::Write((int32_t)agentPtr->getEvent(), fd);
    AEFile::Write((int32_t)agentPtr->getIndex(), fd);
    AEFile::Write((int32_t)agentPtr->getOffer(), fd);
    AEFile::Write((int32_t)agentPtr->getRace(), fd);
    AEFile::Write((int32_t)agentPtr->getSellItemIndex(), fd);
    AEFile::Write((int32_t)agentPtr->getSellItemPrice(), fd);
    AEFile::Write((int32_t)agentPtr->getSellItemQuantity(), fd);
    AEFile::Write((int32_t)agentPtr->getStation(), fd);
    AEFile::Write((int32_t)agentPtr->getSystem(), fd);
    AEFile::Write((int32_t)agentPtr->getWingmanFriendsCount(), fd);
    AEFile::Write(agentPtr->isMale(), fd);
    AEFile::Write(agentPtr->hasReward(), fd);
    AEFile::Write(agentPtr->hasAcceptedOffer(), fd);
    AEFile::Write(agentPtr->wasAskedForDifficulty, fd);
    AEFile::Write(agentPtr->wasAskedForLocation, fd);

    if (agentPtr->getImageParts() == 0) {
        AEFile::Write((int32_t)-1, fd);
    } else {
        AEFile::Write((int32_t)5, fd);
        for (int i = 0; i != 5; i++) {
            int *img = agentPtr->getImageParts();
            AEFile::Write((int32_t)img[i], fd);
        }
    }
    if (0x12 < agentPtr->getIndex()) {
        AEFile::Write((int32_t)agentPtr->getSellModIndex(), fd);
    }

    {String s = agentPtr->getMissionString(); AEFile::Write(s, fd, true);}
    {String s = agentPtr->getName(); AEFile::Write(s, fd, true);}
    {String s = agentPtr->getStationName(); AEFile::Write(s, fd, true);}
    {String s = agentPtr->getSystemName(); AEFile::Write(s, fd, true);}

    String *f0c = agentPtr->wingman1;
    if (f0c == 0) {
        String s("");
        AEFile::Write(s, fd, true);
    } else {
        AEFile::Write(*f0c, fd, true);
    }
    String *f10 = agentPtr->wingman2;
    if (f10 == 0) {
        String s("");
        AEFile::Write(s, fd, true);
    } else {
        AEFile::Write(*f10, fd, true);
    }

    this->currentAgent = agentPtr;
    Mission *mission = agentPtr->getMission();
    if (mission == 0 || this->currentMission == mission) {
        AEFile::Write((int32_t)-1, fd);
    } else {
        AEFile::Write((int32_t)1, fd);
        this->writeMission(mission, fd);
    }

}

void RecordHandler::writeMission(Mission *m, unsigned int fd) {
    AEFile::Write((int32_t)m->getType(), fd);
    if (m->isEmpty() == 0) {
        {String s = m->getClientName(); AEFile::Write(s, fd, true);}
        {String s = m->getTargetName(); AEFile::Write(s, fd, true);}
        {String s = m->getTargetStationName(); AEFile::Write(s, fd, true);}
        {String s = m->getTargetSystemName(); AEFile::Write(s, fd, true);}

        AEFile::Write((bool)m->isCampaignMission(), fd);
        if (m->getClientImage() == 0) {
            AEFile::Write((int32_t)-1, fd);
        } else {
            AEFile::Write((int32_t)5, fd);
            for (int i = 0; i != 5; i++) {
                int *img = m->getClientImage();
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

}

void RecordHandler::loadOptions() {
    if (AEFile::FileExist(this->optionsPath) == 0) {
        return;
    }
    unsigned int fd;
    AEFile::OpenRead(this->optionsPath, &fd);
    bool valid = this->checkHash(fd);
    AEFile::Close(fd);
    if (!valid) {
        return;
    }
    AEFile::OpenRead(this->optionsPath, &fd);
    OptionsBuffer *options = reinterpret_cast<OptionsBuffer *>(Globals::options);
    AEFile::Read(options->byte_0x10, fd);
    AEFile::Read(options->byte_0x11, fd);
    Globals::status->_pad_0xfa[0] = options->byte_0x11;
    AEFile::Read(options->f_0x14, fd);
    AEFile::Read(options->f_0x18, fd);
    AEFile::Read(options->f_0x1c, fd);
    AEFile::Read(options->f_0x00, fd);
    AEFile::Read(options->byte_0x0c, fd);
    AEFile::Read(options->byte_0x0d, fd);
    AEFile::Read(options->f_0x04, fd);
    AEFile::Read(options->f_0x24, fd);
    AEFile::Read(options->f_0x28, fd);

    if (Globals::instantActionPlayerName != 0) {
        delete Globals::instantActionPlayerName;
        Globals::instantActionPlayerName = 0;
    }
    String name;
    AEFile::Read(name, fd, false);
    if (name.length != 0) {
        Globals::instantActionPlayerName = new String(name);
    }

    AEFile::Read(Globals::lastRecordWritten, fd);
    AEFile::Read(Globals::first_start_ever, fd);
    AEFile::Read(options->i32_0x54, fd);
    AEFile::Read(options->i32_0x58, fd);
    AEFile::Read(options->volumeSfx, fd);
    AEFile::Read(options->byte_0x0e, fd);
    AEFile::Read(options->byte_0x32, fd);
    AEFile::Read(options->byte_0x33, fd);
    AEFile::Read(options->byte_0x34, fd);
    AEFile::Read(options->volumeMusic, fd);
    short language = -1;
    AEFile::Read(language, fd);
    AEFile::Read(Globals::startLiteVersionWithMoreCredits, fd);
    AEFile::Read(options->byte_0x35, fd);
    AEFile::Read(options->byte_0x36, fd);
    AEFile::Read(options->byte_0x3a, fd);
    AEFile::Read(options->byte_0x3b, fd);
    AEFile::Read(options->byte_0x3c, fd);
    AEFile::Read(options->byte_0x37, fd);
    AEFile::Read(options->byte_0x3d, fd);
    AEFile::Read(options->byte_0x3e, fd);
    AEFile::Read(options->byte_0x3f, fd);
    AEFile::Read(options->byte_0x40, fd);
    AEFile::Read(options->byte_0x41, fd);
    AEFile::Read(options->volumeAmbient, fd);
    AEFile::Read(options->byte_0x48, fd);
    AEFile::Read(options->byte_0x38, fd);
    AEFile::Read(options->byte_0x4e, fd);
    AEFile::Read(options->byte_0x49, fd);
    AEFile::Read(options->byte_0x4a, fd);
    AEFile::Read(options->byte_0x4b, fd);
    AEFile::Read(options->byte_0x4c, fd);
    AEFile::Read(options->byte_0x4d, fd);
    AEFile::Read(options->i32_0x50, fd);
    AEFile::Read(options->byte_0x60, fd);
    AEFile::Read(options->byte_0x61, fd);
    AEFile::Close(fd);

    if (language > -1) {
        if (language == 9) {
            language = 0;
        }
        Globals::gameText->setLanguage(language, 3401);
        Globals::globals->loadFont(language);
    }
    if (Globals::sound != 0) {
        Globals::sound->setAudioLanguage(language);
        Globals::sound->enableCategory(1, options->byte_0x0d);
        Globals::sound->enableCategory(2, options->byte_0x0c);
        Globals::sound->enableCategory(3, options->byte_0x0e);
        Globals::sound->setVolume(1, options->f_0x00);
        Globals::sound->setVolume(2, options->f_0x04);
        Globals::sound->setVolume(3, options->volumeSfx);
    }
    // The original reads this flag after the handle has already been closed.
    AEFile::Read(options->byte_0x62, fd);
}

float RecordHandler::loadResolutionValue(float resolution) {
    if (AEFile::FileExist(this->optionsPath) != 0) {
        unsigned int fd;
        AEFile::OpenRead(this->optionsPath, &fd);
        OptionsBuffer options;
        AEFile::Read(options.byte_0x10, fd);
        AEFile::Read(options.byte_0x11, fd);
        AEFile::Read(options.f_0x14, fd);
        AEFile::Read(options.f_0x18, fd);
        AEFile::Read(options.f_0x1c, fd);
        AEFile::Read(options.f_0x00, fd);
        AEFile::Read(options.byte_0x0c, fd);
        AEFile::Read(options.byte_0x0d, fd);
        AEFile::Read(options.f_0x04, fd);
        AEFile::Read(options.f_0x24, fd);
        AEFile::Read(options.f_0x28, fd);
        String name;
        AEFile::Read(name, fd, false);
        int lastRecord;
        bool flag;
        AEFile::Read(lastRecord, fd);
        AEFile::Read(flag, fd);
        AEFile::Read(options.i32_0x54, fd);
        AEFile::Read(options.i32_0x58, fd);
        AEFile::Read(options.volumeSfx, fd);
        AEFile::Read(options.byte_0x0e, fd);
        AEFile::Read(options.byte_0x32, fd);
        AEFile::Read(options.byte_0x33, fd);
        AEFile::Read(options.byte_0x34, fd);
        AEFile::Read(options.volumeMusic, fd);
        short language = -1;
        AEFile::Read(language, fd);
        AEFile::Read(flag, fd);
        AEFile::Read(options.byte_0x35, fd);
        AEFile::Read(options.byte_0x36, fd);
        AEFile::Read(options.byte_0x3a, fd);
        AEFile::Read(options.byte_0x3b, fd);
        AEFile::Read(options.byte_0x3c, fd);
        AEFile::Read(options.byte_0x37, fd);
        AEFile::Read(options.byte_0x3d, fd);
        AEFile::Read(options.byte_0x3e, fd);
        AEFile::Read(options.byte_0x3f, fd);
        AEFile::Read(options.byte_0x40, fd);
        AEFile::Read(options.byte_0x41, fd);
        AEFile::Read(options.volumeAmbient, fd);
        resolution = options.volumeAmbient;
        AEFile::Close(fd);
    }
    return resolution;
}

int RecordHandler::writeByteArrayAsRecord(signed char *buf, int n, int slot, bool toBackup) {
    String path = (toBackup ? this->backupDir : this->recordDir) + String(slot);
    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);
    AEFile::Write((uint32_t) n, buf, fd);
    AEFile::Close(fd);
    return n;
}

Mission *RecordHandler::readMission(unsigned int fd) {
    int type = 0;
    AEFile::Read(type, fd);
    if (type == -1) {
        return Mission::empty;
    }
    int *imageParts = 0;
    String clientName;
    AEFile::Read(clientName, fd, true);
    String targetName;
    AEFile::Read(targetName, fd, true);
    String targetStationName;
    AEFile::Read(targetStationName, fd, true);
    String targetSystemName;
    AEFile::Read(targetSystemName, fd, true);
    bool isEmpty = false;
    AEFile::Read(isEmpty, fd);
    int imageCount = 0;
    AEFile::Read(imageCount, fd);
    if (imageCount >= 1) {
        imageParts = new int[imageCount];
        for (int i = 0; i < imageCount; i++) {
            AEFile::Read(imageParts[i], fd);
        }
    }

    int clientRace = 0;
    int costs = 0;
    int bonus = 0;
    int reward = 0;
    int targetStation = 0;
    int difficulty = 0;
    int productionItem = 0;
    int productionAmount = 0;
    int statusValue = 0;
    bool visible = false;
    AEFile::Read(clientRace, fd);
    AEFile::Read(costs, fd);
    AEFile::Read(bonus, fd);
    AEFile::Read(reward, fd);
    AEFile::Read(targetStation, fd);
    AEFile::Read(difficulty, fd);
    AEFile::Read(productionItem, fd);
    AEFile::Read(productionAmount, fd);
    AEFile::Read(statusValue, fd);
    AEFile::Read(visible, fd);

    int hasAgent = 0;
    AEFile::Read(hasAgent, fd);
    Agent *agent = hasAgent >= 1 ? this->readAgent(fd) : 0;

    Mission *mission;
    if (isEmpty) {
        mission = new Mission(type, reward, targetStation);
    } else {
        mission = new Mission(type, clientName, imageParts, clientRace, reward, targetStation, difficulty);
    }
    mission->setCosts(costs);
    mission->setBonus(bonus);
    mission->setProductionGoods(productionItem, productionAmount);
    mission->setStatusValue(statusValue);
    mission->setVisible(visible);
    mission->setAgent(agent);
    mission->setTargetName(targetName);
    if (agent != 0) {
        agent->setMission(mission);
    }
    return mission;
}

void RecordHandler::saveOptions() {
    if (AEFile::FileExist(this->optionsPath) != 0) {
        AEFile::FileDelete(this->optionsPath);
    }
    unsigned int fd;
    AEFile::OpenWrite(this->optionsPath, &fd);
    OptionsBuffer *options = reinterpret_cast<OptionsBuffer *>(Globals::options);
    AEFile::Write(options->byte_0x10, fd);
    AEFile::Write(options->byte_0x11, fd);
    AEFile::Write(options->f_0x14, fd);
    AEFile::Write(options->f_0x18, fd);
    AEFile::Write(options->f_0x1c, fd);
    AEFile::Write(options->f_0x00, fd);
    AEFile::Write(options->byte_0x0c, fd);
    AEFile::Write(options->byte_0x0d, fd);
    AEFile::Write(options->f_0x04, fd);
    AEFile::Write(options->f_0x24, fd);
    AEFile::Write(options->f_0x28, fd);
    if (Globals::instantActionPlayerName != 0) {
        AEFile::Write(*Globals::instantActionPlayerName, fd, false);
    } else {
        AEFile::Write(String(""), fd, false);
    }
    AEFile::Write((int32_t) Globals::lastRecordWritten, fd);
    AEFile::Write(Globals::first_start_ever, fd);
    AEFile::Write(options->i32_0x54, fd);
    AEFile::Write(options->i32_0x58, fd);
    AEFile::Write(options->volumeSfx, fd);
    AEFile::Write(options->byte_0x0e, fd);
    AEFile::Write(options->byte_0x32, fd);
    AEFile::Write(options->byte_0x33, fd);
    AEFile::Write(options->byte_0x34, fd);
    AEFile::Write(options->volumeMusic, fd);
    AEFile::Write(GameText::getLanguage(), fd);
    AEFile::Write(Globals::startLiteVersionWithMoreCredits, fd);
    AEFile::Write(options->byte_0x35, fd);
    AEFile::Write(options->byte_0x36, fd);
    AEFile::Write(options->byte_0x3a, fd);
    AEFile::Write(options->byte_0x3b, fd);
    AEFile::Write(options->byte_0x3c, fd);
    AEFile::Write(options->byte_0x37, fd);
    AEFile::Write(options->byte_0x3d, fd);
    AEFile::Write(options->byte_0x3e, fd);
    AEFile::Write(options->byte_0x3f, fd);
    AEFile::Write(options->byte_0x40, fd);
    AEFile::Write(options->byte_0x41, fd);
    AEFile::Write(options->volumeAmbient, fd);
    AEFile::Write(options->byte_0x48, fd);
    AEFile::Write(options->byte_0x38, fd);
    AEFile::Write(options->byte_0x4e, fd);
    AEFile::Write(options->byte_0x49, fd);
    AEFile::Write(options->byte_0x4a, fd);
    AEFile::Write(options->byte_0x4b, fd);
    AEFile::Write(options->byte_0x4c, fd);
    AEFile::Write(options->byte_0x4d, fd);
    AEFile::Write(options->i32_0x50, fd);
    AEFile::Write(options->byte_0x60, fd);
    AEFile::Write(options->byte_0x61, fd);
    AEFile::Write(options->byte_0x62, fd);
    AEFile::Close(fd);
    this->addHashToOptions();
}

Agent *RecordHandler::readAgent(unsigned int fd) {
    int costs = 0;
    AEFile::Read(costs, fd);
    int sellSystem = 0;
    AEFile::Read(sellSystem, fd);
    int sellBluePrint = 0;
    AEFile::Read(sellBluePrint, fd);
    int event = 0;
    AEFile::Read(event, fd);
    int kind = 0;
    AEFile::Read(kind, fd);
    int offer = 0;
    AEFile::Read(offer, fd);
    int race = 0;
    AEFile::Read(race, fd);
    int sellItemIndex = 0;
    AEFile::Read(sellItemIndex, fd);
    int sellItemPrice = 0;
    AEFile::Read(sellItemPrice, fd);
    int sellItemQuantity = 0;
    AEFile::Read(sellItemQuantity, fd);
    int station = 0;
    AEFile::Read(station, fd);
    int system = 0;
    AEFile::Read(system, fd);
    int wingmen = 0;
    AEFile::Read(wingmen, fd);
    bool male = false;
    AEFile::Read(male, fd);
    bool hasReward = false;
    AEFile::Read(hasReward, fd);
    bool accepted = false;
    AEFile::Read(accepted, fd);
    bool askedForDifficulty = false;
    AEFile::Read(askedForDifficulty, fd);
    bool askedForLocation = false;
    AEFile::Read(askedForLocation, fd);

    int imageCount = 0;
    AEFile::Read(imageCount, fd);
    int *imageParts = 0;
    if (imageCount >= 1) {
        imageParts = new int[imageCount];
        for (int i = 0; i < imageCount; i++) {
            AEFile::Read(imageParts[i], fd);
        }
    }
    int sellMod = -1;
    if (kind >= 19) {
        AEFile::Read(sellMod, fd);
    }

    String missionString;
    String name;
    String stationName;
    String systemName;
    String wingmanName1;
    String wingmanName2;
    AEFile::Read(missionString, fd, true);
    AEFile::Read(name, fd, true);
    AEFile::Read(stationName, fd, true);
    AEFile::Read(systemName, fd, true);
    AEFile::Read(wingmanName1, fd, true);
    AEFile::Read(wingmanName2, fd, true);

    Mission *mission;
    int hasMission = 0;
    AEFile::Read(hasMission, fd);
    if (hasMission >= 1) {
        mission = this->readMission(fd);
    } else {
        mission = 0;
    }

    Agent *agent = new Agent(kind, name, station, system, race, male, sellSystem, sellBluePrint, sellMod,
                             sellItemPrice);
    agent->setCosts(costs);
    agent->setEvent(event);
    agent->setOffer(offer);
    agent->setSellItemData(sellItemIndex, sellItemQuantity, sellItemPrice);
    if (wingmanName1.length != 0) {
        agent->wingman1 = new String(wingmanName1);
    }
    if (wingmanName2.length != 0) {
        agent->wingman2 = new String(wingmanName2);
    }
    agent->wingmanCount = wingmen;
    Array<String *> *friendNames = new Array<String *>();
    ArraySetLength(wingmen, *friendNames);
    for (int i = 0; i < wingmen; i++) {
        (*friendNames)[i] = new String(i == 0 ? wingmanName1 : wingmanName2);
    }
    agent->setWingmanFriendNames(friendNames);
    agent->giveRewardAtNextChat(hasReward);
    agent->setOfferAccepted(accepted);
    agent->setImageParts(imageParts);
    agent->setMissionString(missionString);
    agent->setStationName(stationName);
    agent->setSystemName(systemName);
    agent->setMission(mission);
    agent->wasAskedForDifficulty = askedForDifficulty;
    agent->wasAskedForLocation = askedForLocation;
    return agent;
}

void RecordHandler::writeWanted(Wanted *w, unsigned int fd) {
    AEFile::Write(w->isActive(), fd);
    AEFile::Write(w->isTerminated(), fd);
    AEFile::Write((int32_t)w->getCurrentLocation(), fd);
    AEFile::Write((int32_t)w->getTravelsTo(), fd);
    AEFile::Write((int32_t)w->getLastSeen(), fd);

    {String name = w->getName(); AEFile::Write(name, fd, true);}

    AEFile::Write((int32_t)w->getIndex(), fd);
    AEFile::Write((int32_t)w->getBoard(), fd);
    AEFile::Write((int32_t)w->getRace(), fd);
    AEFile::Write(w->isMale(), fd);
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

}

int RecordHandler::recordStoreWritePreview(GameRecord *rec, int slot) {
    if (rec == 0) {
        return 0;
    }
    String path = this->backupDir + String(slot);
    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);
    AEFile::Write(rec->playTime, fd);
    AEFile::Write((int32_t) rec->field_0x08, fd);
    AEFile::Write(reinterpret_cast<Station *>(rec->field_0x138)->getName(), fd, true);
    AEFile::Write(rec->field_0x188, fd, true);
    AEFile::Write((int32_t) rec->field_0x40, fd);
    AEFile::Write((int32_t) rec->killsText, fd);
    AEFile::Write(rec->rank, fd);
    AEFile::Write((int32_t) reinterpret_cast<Ship *>(rec->field_0x130)->getIndex(), fd);
    AEFile::Close(fd);
    return 1;
}

int RecordHandler::recordStoreWrite(int slot) {
    String path = this->recordDir + String(slot);
    if (AEFile::FileExist(path) != 0) {
        AEFile::FileDelete(path);
    }
    unsigned int fd;
    AEFile::OpenWrite(path, &fd);

    bool *visited = Globals::galaxy->getVisited();
    AEFile::Write((int32_t) 135, fd);
    for (unsigned i = 0; i < 135; i++) {
        AEFile::Write(visited[i], fd);
    }

    AEFile::Write((int32_t) Globals::status->getCredits(), fd);
    AEFile::Write((int32_t) Globals::status->getRating(), fd);
    AEFile::Write(Globals::status->getPlayingTime(), fd);
    AEFile::Write((int32_t) Globals::status->getKills(), fd);
    AEFile::Write((int32_t) Globals::status->getMissionCount(), fd);
    AEFile::Write((int32_t) Globals::status->getLevel(), fd);
    AEFile::Write((int32_t) Globals::status->getLastXP(), fd);
    AEFile::Write((int32_t) Globals::status->getGoodsProduced(), fd);
    AEFile::Write((int32_t) Globals::status->getStationsVisited(), fd);
    AEFile::Write((int32_t) Globals::status->getCurrentCampaignMission(), fd);
    this->writeMission(Globals::status->getFreelanceMission(), fd);
    this->writeMission(reinterpret_cast<Mission *>(Globals::status->getCampaignMission()), fd);
    AEFile::Write((int32_t) Globals::status->getJumpgateUsed(), fd);
    AEFile::Write((int32_t) Globals::status->getCapturedCrates(), fd);
    AEFile::Write((int32_t) Globals::status->getBoughtEquipment(), fd);
    AEFile::Write((int32_t) Globals::status->getPirateKills(), fd);
    AEFile::Write((int32_t) Globals::status->field_80, fd);
    AEFile::Write((int32_t) Globals::status->field_7c, fd);
    AEFile::Write((int32_t) Globals::status->field_84, fd);
    AEFile::Write((int32_t) Globals::status->field_88, fd);
    AEFile::Write((int32_t) Globals::status->field_94->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_94->size(); i++) {
        AEFile::Write((*Globals::status->field_94)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_98->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_98->size(); i++) {
        AEFile::Write((*Globals::status->field_98)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_9c, fd);
    AEFile::Write((int32_t) Globals::status->field_a0, fd);
    AEFile::Write((int32_t) Globals::status->field_a4, fd);
    AEFile::Write((int32_t) Globals::status->field_a8, fd);
    AEFile::Write((int32_t) Globals::status->field_ac->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_ac->size(); i++) {
        AEFile::Write((*Globals::status->field_ac)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_b0, fd);
    AEFile::Write((int32_t) Globals::status->field_b4->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_b4->size(); i++) {
        AEFile::Write((*Globals::status->field_b4)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_b8, fd);
    AEFile::Write(reinterpret_cast<int64_t &>(Globals::status->field_c0), fd);
    AEFile::Write((int32_t) Globals::status->field_c8, fd);
    AEFile::Write((int32_t) Globals::status->field_cc, fd);
    AEFile::Write((int32_t) Globals::status->field_d0, fd);
    AEFile::Write((int32_t) Globals::status->field_d4, fd);
    AEFile::Write((int32_t) Globals::status->field_d8, fd);
    AEFile::Write((int32_t) Globals::status->field_dc, fd);
    AEFile::Write((int32_t) Globals::status->field_e0, fd);
    AEFile::Write((int32_t) Globals::status->field_e4, fd);
    AEFile::Write((int32_t) Globals::status->field_e8, fd);
    AEFile::Write((int32_t) Globals::status->field_ec, fd);

    int *medals = Globals::achievements->getMedals();
    AEFile::Write((int32_t) 45, fd);
    for (unsigned i = 0; i < 45; i++) {
        AEFile::Write((int32_t) medals[i], fd);
    }

    AEFile::Write((int32_t) Globals::status->getShip()->getIndex(), fd);
    AEFile::Write((int32_t) Globals::status->getShip()->getRace(), fd);
    Array<Item *> *items = Globals::status->getShip()->getEquipment();
    if (items != 0) {
        AEFile::Write((int32_t) items->size(), fd);
        for (unsigned i = 0; i < items->size(); i++) {
            if ((*items)[i] != 0) {
                AEFile::Write((int32_t) (*items)[i]->getIndex(), fd);
                AEFile::Write((int32_t) (*items)[i]->getAmount(), fd);
                AEFile::Write((*items)[i]->isUnsaleable(), fd);
            } else {
                AEFile::Write((int32_t) -1, fd);
            }
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }
    Array<Item *> *cargo = Globals::status->getShip()->getCargo();
    if (cargo != 0) {
        AEFile::Write((int32_t) cargo->size(), fd);
        for (unsigned i = 0; i < cargo->size(); i++) {
            AEFile::Write((int32_t) (*cargo)[i]->getIndex(), fd);
            AEFile::Write((int32_t) (*cargo)[i]->getAmount(), fd);
            AEFile::Write((int32_t) (*cargo)[i]->getSinglePrice(), fd);
            AEFile::Write((*cargo)[i]->isUnsaleable(), fd);
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }

    Array<Station *> *stack = Globals::status->getStationStack();
    Array<Item *> *stationItems;
    AEFile::Write((int32_t) 3, fd);
    for (unsigned i = 0; i < stack->size() + 1; i++) {
        Station *station = i == stack->size() ? Globals::status->getStation() : (*stack)[i];
        if (station == 0) {
            AEFile::Write((int32_t) -1, fd);
            continue;
        }
        AEFile::Write((int32_t) station->getIndex(), fd);
        stationItems = station->getItems();
        if (stationItems != 0) {
            AEFile::Write((int32_t) stationItems->size(), fd);
            for (unsigned j = 0; j < stationItems->size(); j++) {
                AEFile::Write((int32_t) (*stationItems)[j]->getIndex(), fd);
                AEFile::Write((int32_t) (*stationItems)[j]->getAmount(), fd);
                AEFile::Write((int32_t) (*stationItems)[j]->getSinglePrice(), fd);
                AEFile::Write((*stationItems)[j]->isUnsaleable(), fd);
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
        Array<Ship *> *ships = station->getShips();
        if (ships != 0) {
            AEFile::Write((int32_t) ships->size(), fd);
            for (unsigned j = 0; j < ships->size(); j++) {
                AEFile::Write((int32_t) (*ships)[j]->getIndex(), fd);
                AEFile::Write((int32_t) (*ships)[j]->getRace(), fd);
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
        Array<Agent *> *agents = station->getAgents();
        if (agents != 0) {
            AEFile::Write((int32_t) agents->size(), fd);
            for (unsigned j = 0; j < agents->size(); j++) {
                this->writeAgent((*agents)[j], fd);
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
        AEFile::Write(station->hasAttackedFriends(), fd);
    }

    int *standings = reinterpret_cast<Standing *>(Globals::status->getStanding())->getStandings();
    AEFile::Write((int32_t) 2, fd);
    for (unsigned i = 0; i < 2; i++) {
        AEFile::Write((int32_t) standings[i], fd);
    }

    Array<BluePrint *> *bluePrints = reinterpret_cast<Array<BluePrint *> *>(Globals::status->getBluePrints());
    AEFile::Write((int32_t) bluePrints->size(), fd);
    for (unsigned i = 0; i < bluePrints->size(); i++) {
        BluePrint *bluePrint = (*bluePrints)[i];
        for (unsigned j = 0; j < bluePrint->ingredientCounters->size(); j++) {
            AEFile::Write((int32_t) (*bluePrint->ingredientCounters)[j], fd);
        }
        AEFile::Write((int32_t) bluePrint->spentValue, fd);
        AEFile::Write(bluePrint->locked, fd);
        AEFile::Write((int32_t) bluePrint->productionCount, fd);
        AEFile::Write((int32_t) bluePrint->stationIndex, fd);
        AEFile::Write(bluePrint->stationName, fd, true);
    }

    Array<PendingProduct *> *pending =
        Globals::status->getPendingProducts();
    if (pending != 0) {
        auto countProducts = [](Array<PendingProduct *> *products) {
            int count = 0;
            for (unsigned i = 0; i < products->size(); ++i)
                if ((*products)[i]) ++count;
            return count;
        };
        int count = countProducts(pending);
        if (count != 0) {
            AEFile::Write((int32_t) count, fd);
            for (unsigned i = 0; i < pending->size(); i++) {
                if ((*pending)[i] != 0) {
                    AEFile::Write((int32_t) (*pending)[i]->blueprintIndex, fd);
                    AEFile::Write((int32_t) (*pending)[i]->quantity, fd);
                    AEFile::Write((int32_t) (*pending)[i]->stationIndex, fd);
                    AEFile::Write((*pending)[i]->stationName, fd, true);
                }
            }
        } else {
            AEFile::Write((int32_t) -1, fd);
        }
    } else {
        AEFile::Write((int32_t) -1, fd);
    }

    if (Globals::status->wingmen != 0) {
        AEFile::Write((int32_t) Globals::status->wingmen->size(), fd);
        for (unsigned i = 0; i < Globals::status->wingmen->size(); i++) {
            AEFile::Write(*(*Globals::status->wingmen)[i], fd, true);
        }
        AEFile::Write((int32_t) Globals::status->field_0x2c, fd);
        AEFile::Write((int32_t) Globals::status->fadeValue, fd);
        AEFile::Write((int32_t) 5, fd);
        for (unsigned i = 0; i <= 4; i++) {
            AEFile::Write((int32_t) ((int *) (intptr_t) reinterpret_cast<int32_t &>(Globals::status->_byte_0x28))[i],
                          fd);
        }
    } else {
        AEFile::Write((int32_t) -1, fd);
    }
    AEFile::Write(reinterpret_cast<int32_t &>(Globals::status->_byte_0x34), fd);
    AEFile::Write((int32_t) Globals::status->systemVisibilities->size(), fd);
    for (unsigned i = 0; i < Globals::status->systemVisibilities->size(); i++) {
        AEFile::Write((*Globals::status->systemVisibilities)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_0x40->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_0x40->size(); i++) {
        AEFile::Write((int32_t) (*Globals::status->field_0x40)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_0x3c->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_0x3c->size(); i++) {
        AEFile::Write((int32_t) (*Globals::status->field_0x3c)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_0x48->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_0x48->size(); i++) {
        AEFile::Write((int32_t) (*Globals::status->field_0x48)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_0x44->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_0x44->size(); i++) {
        AEFile::Write((int32_t) (*Globals::status->field_0x44)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_4c->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_4c->size(); i++) {
        AEFile::Write((*Globals::status->field_4c)[i], fd);
    }
    Array<Agent *> *agents = reinterpret_cast<Array<Agent *> *>(Globals::status->getAgents());
    AEFile::Write((int32_t) agents->size(), fd);
    for (unsigned i = 0; i < agents->size(); i++) {
        this->writeAgent((*agents)[i], fd);
    }

    HintFlags *hints = reinterpret_cast<HintFlags *>(Globals::hints);
    AEFile::Write(hints->flag[8], fd);
    AEFile::Write(hints->flag[9], fd);
    AEFile::Write(hints->flag[10], fd);
    AEFile::Write(hints->flag[11], fd);
    AEFile::Write(hints->flag[12], fd);
    AEFile::Write(hints->flag[13], fd);
    AEFile::Write(hints->flag[14], fd);
    AEFile::Write(hints->flag[15], fd);
    AEFile::Write(hints->flag[16], fd);
    AEFile::Write(hints->flag[17], fd);
    AEFile::Write(hints->flag[18], fd);
    AEFile::Write(hints->flag[19], fd);
    AEFile::Write(hints->flag[20], fd);
    AEFile::Write(hints->flag[21], fd);
    AEFile::Write(hints->flag[22], fd);
    AEFile::Write(hints->flag[23], fd);
    AEFile::Write(hints->flag[24], fd);
    AEFile::Write(hints->flag[25], fd);
    AEFile::Write(hints->flag[26], fd);
    AEFile::Write(hints->flag[27], fd);
    AEFile::Write(hints->flag[28], fd);
    AEFile::Write(hints->flag[29], fd);
    AEFile::Write(hints->flag[30], fd);
    AEFile::Write(hints->flag[31], fd);
    AEFile::Write(hints->flag[32], fd);
    AEFile::Write(hints->flag[34], fd);
    AEFile::Write(hints->flag[33], fd);
    AEFile::Write(hints->flag[35], fd);
    AEFile::Write(hints->flag[36], fd);
    OptionsBuffer *options = reinterpret_cast<OptionsBuffer *>(Globals::options);
    AEFile::Write(options->difficulty, fd);
    AEFile::Write(reinterpret_cast<int64_t &>(Globals::status->field_0x100), fd);
    AEFile::Write(hints->flag[37], fd);
    AEFile::Write(hints->flag[38], fd);

    Ship *wingman = reinterpret_cast<Ship *>(Globals::status->field_8c);
    if (wingman != 0) {
        AEFile::Write((int32_t) 1, fd);
        AEFile::Write((int32_t) reinterpret_cast<Ship *>(Globals::status->field_8c)->getIndex(), fd);
        AEFile::Write((int32_t) reinterpret_cast<Ship *>(Globals::status->field_8c)->getRace(), fd);
        Array<Item *> *wingEquipment = reinterpret_cast<Ship *>(Globals::status->field_8c)->getEquipment();
        if (wingEquipment != 0) {
            AEFile::Write((int32_t) wingEquipment->size(), fd);
            for (unsigned i = 0; i < wingEquipment->size(); i++) {
                if ((*wingEquipment)[i] != 0) {
                    AEFile::Write((int32_t) (*wingEquipment)[i]->getIndex(), fd);
                    AEFile::Write((int32_t) (*wingEquipment)[i]->getAmount(), fd);
                    AEFile::Write((*wingEquipment)[i]->isUnsaleable(), fd);
                } else {
                    AEFile::Write((int32_t) -1, fd);
                }
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
        items = reinterpret_cast<Ship *>(Globals::status->field_8c)->getCargo();
        if (items != 0) {
            AEFile::Write((int32_t) items->size(), fd);
            for (unsigned i = 0; i < items->size(); i++) {
                AEFile::Write((int32_t) (*items)[i]->getIndex(), fd);
                AEFile::Write((int32_t) (*items)[i]->getAmount(), fd);
                AEFile::Write((int32_t) (*items)[i]->getSinglePrice(), fd);
                AEFile::Write((*items)[i]->isUnsaleable(), fd);
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }

    AEFile::Write((int32_t) Globals::status->field_90->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_90->size(); i++) {
        AEFile::Write((int32_t) (*Globals::status->field_90)[i], fd);
    }
    AEFile::Write((int32_t) Globals::status->field_10c, fd);
    AEFile::Write(Globals::status->field_110, fd);
    AEFile::Write((int32_t) Globals::status->field_114, fd);
    AEFile::Write(Globals::status->field_0x111, fd);

    items = Globals::status->voidStation->getItems();
    if (items != 0) {
        AEFile::Write((int32_t) items->size(), fd);
        for (unsigned i = 0; i < items->size(); i++) {
            AEFile::Write((int32_t) (*items)[i]->getIndex(), fd);
            AEFile::Write((int32_t) (*items)[i]->getAmount(), fd);
            AEFile::Write((int32_t) (*items)[i]->getSinglePrice(), fd);
            AEFile::Write((*items)[i]->isUnsaleable(), fd);
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }
    Array<Ship *> *voidShips = Globals::status->voidStation->getShips();
    if (voidShips != 0) {
        AEFile::Write((int32_t) voidShips->size(), fd);
        for (unsigned i = 0; i < voidShips->size(); i++) {
            AEFile::Write((int32_t) (*voidShips)[i]->getIndex(), fd);
            AEFile::Write((int32_t) (*voidShips)[i]->getRace(), fd);
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }

    AEFile::Write(hints->flag[39], fd);
    AEFile::Write(options->byte_0x35, fd);
    AEFile::Write(options->byte_0x36, fd);
    AEFile::Write((int32_t) Globals::status->field_54->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_54->size(); i++) {
        AEFile::Write((*Globals::status->field_54)[i], fd);
    }

    AEFile::Write((int32_t) 0x6e6a78, fd);
    Array<int> *mods = Globals::status->getShip()->getMods();
    if (mods != 0) {
        AEFile::Write((int32_t) mods->size(), fd);
        for (unsigned i = 0; i < mods->size(); i++) {
            AEFile::Write((int32_t) (*mods)[i], fd);
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }
    if (Globals::status->field_8c != 0) {
        mods = reinterpret_cast<Ship *>(Globals::status->field_8c)->getMods();
        if (mods != 0) {
            AEFile::Write((int32_t) mods->size(), fd);
            for (unsigned i = 0; i < mods->size(); i++) {
                AEFile::Write((int32_t) (*mods)[i], fd);
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }
    if (voidShips != 0) {
        AEFile::Write((int32_t) voidShips->size(), fd);
        for (unsigned i = 0; i < voidShips->size(); i++) {
            mods = (*voidShips)[i]->getMods();
            if (mods != 0) {
                AEFile::Write((int32_t) mods->size(), fd);
                for (unsigned j = 0; j < mods->size(); j++) {
                    AEFile::Write((int32_t) (*mods)[j], fd);
                }
            } else {
                AEFile::Write((int32_t) 0, fd);
            }
        }
    } else {
        AEFile::Write((int32_t) 0, fd);
    }
    for (unsigned i = 0; i < stack->size() + 1; i++) {
        Station *station;
        if (i == stack->size()) {
            station = Globals::status->getStation();
        } else {
            station = (*stack)[i];
        }
        if (station == 0) {
            continue;
        }
        Array<Ship *> *ships = station->getShips();
        if (ships != 0) {
            AEFile::Write((int32_t) ships->size(), fd);
            for (unsigned j = 0; j < ships->size(); j++) {
                if ((*ships)[j]->getMods() != 0) {
                    AEFile::Write((uint32_t) (*ships)[j]->getMods()->size(), fd);
                    mods = (*ships)[j]->getMods();
                    for (unsigned k = 0; k < mods->size(); k++) {
                        AEFile::Write((int32_t) (*mods)[k], fd);
                    }
                } else {
                    AEFile::Write((int32_t) 0, fd);
                }
            }
        } else {
            AEFile::Write((int32_t) 0, fd);
        }
    }

    AEFile::Write((int32_t) Globals::status->wanted->size(), fd);
    for (unsigned i = 0; i < Globals::status->wanted->size(); i++) {
        this->writeWanted((*Globals::status->wanted)[i], fd);
    }
    for (int i = 0; i < 4; i++) {
        AEFile::Write((int32_t) Globals::status->getCollectedBounties(i), fd);
    }
    AEFile::Write(options->byte_0x37, fd);
    AEFile::Write((int32_t) Globals::status->field_178, fd);
    AEFile::Write(hints->flag[40], fd);
    AEFile::Write(hints->flag[41], fd);
    AEFile::Write(hints->flag[44], fd);
    AEFile::Write(hints->flag[42], fd);
    AEFile::Write(hints->flag[43], fd);
    AEFile::Write(hints->flag[46], fd);
    AEFile::Write(hints->flag[47], fd);
    AEFile::Write(hints->flag[48], fd);
    AEFile::Write((int32_t) Globals::status->field_58->size(), fd);
    for (unsigned i = 0; i < Globals::status->field_58->size(); i++) {
        AEFile::Write((*Globals::status->field_58)[i], fd);
    }
    AEFile::Write(hints->flag[49], fd);
    AEFile::Write(hints->flag[45], fd);
    AEFile::Write(hints->flag[50], fd);
    AEFile::Write((int32_t) Globals::status->field_118, fd);
    AEFile::Write(hints->flag[51], fd);
    AEFile::Write(hints->flag[52], fd);
    AEFile::Write(hints->flag[53], fd);
    AEFile::Write(hints->flag[54], fd);
    AEFile::Write(hints->flag[55], fd);
    AEFile::Write(hints->flag[56], fd);
    AEFile::Write(hints->flag[57], fd);
    AEFile::Write(hints->flag[58], fd);
    AEFile::Close(fd);
    this->addHash(slot);
    Globals::lastRecordWritten = slot;
    this->saveOptions();
    return 1;
}


int RecordHandler::readOptionsFileAsByteArray(signed char **out) {
    String tmp(this->optionsPath, false);
    int sz;
    if (AEFile::FileExist(tmp) != 0) {
        unsigned int fd;
        AEFile::OpenRead(tmp, &fd);
        sz = AEFile::GetFileSize(fd);
        signed char *buf = new signed char[sz];
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
    int32_t *visitedCount = 0;
    String path = this->recordDir + String(slot);
    if (AEFile::FileExist(path) == 0) {
        return rec;
    }
    unsigned int fd;
    AEFile::OpenRead(path, &fd);
    bool valid = this->checkHash(fd);
    AEFile::Close(fd);
    if (valid) {
        AEFile::OpenRead(path, &fd);
        rec = new GameRecord();
        visitedCount = &reinterpret_cast<int32_t &>(rec->field_0x04);
        *visitedCount = 0;
        AEFile::Read(*visitedCount, fd);
        for (unsigned i = 0; i < (unsigned)*visitedCount; i++) {
            AEFile::Read(reinterpret_cast<bool *>(rec->data)[i], fd);
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x08), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x0c), fd);
        AEFile::Read(rec->playTime, fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x18), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x1c), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->killsText), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x24), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x28), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x3c), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x40), fd);
        reinterpret_cast<Mission *&>(rec->field_0x54) = this->readMission(fd);
        reinterpret_cast<Mission *&>(rec->field_0x58) = this->readMission(fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x30), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x34), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x38), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x2c), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x44), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x48), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x4c), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x50), fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x68) = new Array<bool>();
        int count68 = 0;
        AEFile::Read(count68, fd);
        ArraySetLength(count68, *reinterpret_cast<Array<bool> *&>(rec->field_0x68));
        for (int i = 0; i < count68; i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x68))[i], fd);
        }
        reinterpret_cast<Array<bool> *&>(rec->field_0x6c) = new Array<bool>();
        int count6c = 0;
        AEFile::Read(count6c, fd);
        ArraySetLength(count6c, *reinterpret_cast<Array<bool> *&>(rec->field_0x6c));
        for (int i = 0; i < count6c; i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x6c))[i], fd);
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x74), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x78), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x7c), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x80), fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x84) = new Array<bool>();
        int count84 = 0;
        AEFile::Read(count84, fd);
        ArraySetLength(count84, *reinterpret_cast<Array<bool> *&>(rec->field_0x84));
        for (int i = 0; i < count84; i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x84))[i], fd);
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x88), fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x8c) = new Array<bool>();
        int count8c = 0;
        AEFile::Read(count8c, fd);
        ArraySetLength(count8c, *reinterpret_cast<Array<bool> *&>(rec->field_0x8c));
        for (int i = 0; i < count8c; i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x8c))[i], fd);
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x90), fd);
        AEFile::Read(reinterpret_cast<int64_t &>(rec->field_0x98), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xa0), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xa4), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xa8), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xac), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xb0), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xb4), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xb8), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xbc), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xc0), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xc4), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x64), fd);
        reinterpret_cast<int *&>(rec->field_0x60) = new int[reinterpret_cast<int32_t &>(rec->field_0x64)];
        for (int i = 0; i < reinterpret_cast<int32_t &>(rec->field_0x64); i++) {
            AEFile::Read(reinterpret_cast<int *&>(rec->field_0x60)[i], fd);
        }

        int shipIndex = 0;
        AEFile::Read(shipIndex, fd);
        reinterpret_cast<Ship *&>(rec->field_0x130) = (*Globals::ships)[shipIndex]->makeShip(-1);
        int shipRace = 0;
        AEFile::Read(shipRace, fd);
        reinterpret_cast<Ship *&>(rec->field_0x130)->setRace(shipRace);
        int equipmentCount = 0;
        AEFile::Read(equipmentCount, fd);
        if (equipmentCount >= 1) {
            Array<Item *> *equipment = new Array<Item *>();
            ArraySetLength(equipmentCount, *equipment);
            for (int i = 0; i < equipmentCount; i++) {
                int index = 0;
                AEFile::Read(index, fd);
                if (index != -1) {
                    int amount = 0;
                    AEFile::Read(amount, fd);
                    (*equipment)[i] = (*Globals::items)[index]->makeItem(
                        amount, (*Globals::items)[index]->getMaxPrice());
                    bool unsaleable = false;
                    AEFile::Read(unsaleable, fd);
                    (*equipment)[i]->setUnsaleable(unsaleable);
                } else {
                    (*equipment)[i] = 0;
                }
            }
            reinterpret_cast<Ship *&>(rec->field_0x130)->replaceEquipment(equipment);
        }
        int cargoCount = 0;
        AEFile::Read(cargoCount, fd);
        if (cargoCount >= 1) {
            Array<Item *> *cargo = new Array<Item *>();
            ArraySetLength(cargoCount, *cargo);
            for (int i = 0; i < cargoCount; i++) {
                int index;
                int amount;
                int price = 0;
                AEFile::Read(index, fd);
                AEFile::Read(amount, fd);
                AEFile::Read(price, fd);
                bool unsaleable = false;
                AEFile::Read(unsaleable, fd);
                (*cargo)[i] = (*Globals::items)[index]->makeItem(
                    amount, (*Globals::items)[index]->getMaxPrice());
                (*cargo)[i]->setPrice(price);
                (*cargo)[i]->setUnsaleable(unsaleable);
            }
            reinterpret_cast<Ship *&>(rec->field_0x130)->replaceCargo(cargo);
        }

        Array<Station *> *stack = new Array<Station *>();
        int stackCount = 0;
        AEFile::Read(stackCount, fd);
        ArraySetLength(stackCount, *stack);
        for (unsigned s = 0; s < stack->size() + 1; s++) {
            Station *station = 0;
            int stationIndex = 0;
            AEFile::Read(stationIndex, fd);
            if (stationIndex != -1) {
                station = Globals::galaxy->getStation(stationIndex);
                AEFile::Read(cargoCount, fd);
                if (cargoCount >= 1) {
                    Array<Item *> *items = new Array<Item *>();
                    ArraySetLength(cargoCount, *items);
                    for (int i = 0; i < cargoCount; i++) {
                        int index;
                        int amount;
                        int price = 0;
                        AEFile::Read(index, fd);
                        AEFile::Read(amount, fd);
                        AEFile::Read(price, fd);
                        bool unsaleable = false;
                        AEFile::Read(unsaleable, fd);
                        (*items)[i] = (*Globals::items)[index]->makeItem(
                            amount, (*Globals::items)[index]->getMaxPrice());
                        (*items)[i]->setPrice(price);
                        (*items)[i]->setUnsaleable(unsaleable);
                    }
                    station->setItems(items, false);
                }
                int shipCount = 0;
                AEFile::Read(shipCount, fd);
                if (shipCount >= 1) {
                    Array<Ship *> *shipList = new Array<Ship *>();
                    ArraySetLength(shipCount, *shipList);
                    for (int i = 0; i < shipCount; i++) {
                        int index = 0;
                        AEFile::Read(index, fd);
                        (*shipList)[i] = (*Globals::ships)[index]->makeShip(-1);
                        int race = 0;
                        AEFile::Read(race, fd);
                        (*shipList)[i]->setRace(race);
                    }
                    station->setShips(shipList, false);
                }
                int agentCount = 0;
                AEFile::Read(agentCount, fd);
                if (agentCount >= 1) {
                    Array<Agent *> *agents = new Array<Agent *>();
                    ArraySetLength(agentCount, *agents);
                    for (int i = 0; i < agentCount; i++) {
                        (*agents)[i] = this->readAgent(fd);
                    }
                    station->setAgents(agents);
                }
                bool attackedFriends = false;
                AEFile::Read(attackedFriends, fd);
                station->setAttackedFriends(attackedFriends);
            }
            if (s == stack->size()) {
                reinterpret_cast<Station *&>(rec->field_0x138) = station;
            } else {
                (*stack)[s] = station;
            }
        }
        reinterpret_cast<Array<Station *> *&>(rec->field_0x5c) = stack;

        int standingCount = 0;
        AEFile::Read(standingCount, fd);
        Standing *standing;
        int *standingValues = new int[standingCount];
        for (int i = 0; i < standingCount; i++) {
            AEFile::Read(standingValues[i], fd);
        }
        standing = new Standing();
        reinterpret_cast<Standing *&>(rec->field_0x13c) = standing;
        standing->setStandings(standingValues);

        Array<BluePrint *> *bluePrints = new Array<BluePrint *>();
        int bluePrintCount = 0;
        AEFile::Read(bluePrintCount, fd);
        ArraySetLength(bluePrintCount, *bluePrints);
        for (unsigned i = 0; i < bluePrints->size(); i++) {
            (*bluePrints)[i] = new BluePrint(
                (*reinterpret_cast<Array<BluePrint *> *>(Globals::status->getBluePrints()))[i]->getIndex());
            BluePrint *bluePrint = (*bluePrints)[i];
            for (unsigned j = 0; j < bluePrint->ingredientCounters->size(); j++) {
                AEFile::Read((*bluePrint->ingredientCounters)[j], fd);
            }
            AEFile::Read(bluePrint->spentValue, fd);
            AEFile::Read(bluePrint->locked, fd);
            AEFile::Read(bluePrint->productionCount, fd);
            AEFile::Read(bluePrint->stationIndex, fd);
            String stationName;
            AEFile::Read(stationName, fd, true);
            bluePrint->stationName = stationName;
        }
        reinterpret_cast<Array<BluePrint *> *&>(rec->field_0x140) = bluePrints;

        int pendingCount = 0;
        AEFile::Read(pendingCount, fd);
        if (pendingCount >= 1) {
            Array<PendingProduct *> *pending = new Array<PendingProduct *>();
            ArraySetLength(pendingCount, *pending);
            for (unsigned i = 0; i < pending->size(); i++) {
                int blueprintIndex;
                int quantity;
                int stationIndex = 0;
                AEFile::Read(blueprintIndex, fd);
                AEFile::Read(quantity, fd);
                AEFile::Read(stationIndex, fd);
                String stationName;
                AEFile::Read(stationName, fd, true);
                (*pending)[i] = new PendingProduct(blueprintIndex, stationName, stationIndex, quantity);
            }
            reinterpret_cast<Array<PendingProduct *> *&>(rec->field_0x144) = pending;
        } else {
            rec->field_0x144 = 0;
        }

        int wingmanCount = 0;
        AEFile::Read(wingmanCount, fd);
        if (wingmanCount >= 1) {
            Array<String *> *wingmen = new Array<String *>();
            ArraySetLength(wingmanCount, *wingmen);
            for (int i = 0; i < wingmanCount; i++) {
                String name;
                AEFile::Read(name, fd, true);
                (*wingmen)[i] = new String(name);
            }
            reinterpret_cast<Array<String *> *&>(rec->field_0x14c) = wingmen;
            AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x150), fd);
            AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x154), fd);
            int portraitCount = 0;
            AEFile::Read(portraitCount, fd);
            reinterpret_cast<int *&>(rec->field_0x158) = new int[portraitCount];
            for (int i = 0; i < portraitCount; i++) {
                AEFile::Read(reinterpret_cast<int *&>(rec->field_0x158)[i], fd);
            }
        } else {
            rec->field_0x14c = 0;
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x15c), fd);
        int count = 0;
        AEFile::Read(count, fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x160) = new Array<bool>();
        ArraySetLength(count, *reinterpret_cast<Array<bool> *&>(rec->field_0x160));
        for (unsigned i = 0; i < reinterpret_cast<Array<bool> *&>(rec->field_0x160)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x160))[i], fd);
        }
        AEFile::Read(count, fd);
        reinterpret_cast<Array<int> *&>(rec->field_0x168) = new Array<int>();
        ArraySetLength(count, *reinterpret_cast<Array<int> *&>(rec->field_0x168));
        for (unsigned i = 0; i < reinterpret_cast<Array<int> *&>(rec->field_0x168)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<int> *&>(rec->field_0x168))[i], fd);
        }
        AEFile::Read(count, fd);
        reinterpret_cast<Array<int> *&>(rec->field_0x164) = new Array<int>();
        ArraySetLength(count, *reinterpret_cast<Array<int> *&>(rec->field_0x164));
        for (unsigned i = 0; i < reinterpret_cast<Array<int> *&>(rec->field_0x164)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<int> *&>(rec->field_0x164))[i], fd);
        }
        AEFile::Read(count, fd);
        reinterpret_cast<Array<int> *&>(rec->field_0x170) = new Array<int>();
        ArraySetLength(count, *reinterpret_cast<Array<int> *&>(rec->field_0x170));
        for (unsigned i = 0; i < reinterpret_cast<Array<int> *&>(rec->field_0x170)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<int> *&>(rec->field_0x170))[i], fd);
        }
        AEFile::Read(count, fd);
        reinterpret_cast<Array<int> *&>(rec->field_0x16c) = new Array<int>();
        ArraySetLength(count, *reinterpret_cast<Array<int> *&>(rec->field_0x16c));
        for (unsigned i = 0; i < reinterpret_cast<Array<int> *&>(rec->field_0x16c)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<int> *&>(rec->field_0x16c))[i], fd);
        }
        AEFile::Read(count, fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x174) = new Array<bool>();
        ArraySetLength(count, *reinterpret_cast<Array<bool> *&>(rec->field_0x174));
        for (unsigned i = 0; i < reinterpret_cast<Array<bool> *&>(rec->field_0x174)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x174))[i], fd);
        }
        int agentCount = 0;
        AEFile::Read(agentCount, fd);
        reinterpret_cast<Array<Agent *> *&>(rec->field_0x148) = new Array<Agent *>();
        ArraySetLength(agentCount, *reinterpret_cast<Array<Agent *> *&>(rec->field_0x148));
        for (unsigned i = 0; i < reinterpret_cast<Array<Agent *> *&>(rec->field_0x148)->size(); i++) {
            (*reinterpret_cast<Array<Agent *> *&>(rec->field_0x148))[i] = this->readAgent(fd);
        }
        AEFile::Read(recordFlag(rec, 0xe4), fd);
        AEFile::Read(recordFlag(rec, 0xe5), fd);
        AEFile::Read(recordFlag(rec, 0xe6), fd);
        AEFile::Read(recordFlag(rec, 0xe7), fd);
        AEFile::Read(recordFlag(rec, 0xe8), fd);
        AEFile::Read(recordFlag(rec, 0xe9), fd);
        AEFile::Read(recordFlag(rec, 0xea), fd);
        AEFile::Read(recordFlag(rec, 0xeb), fd);
        AEFile::Read(recordFlag(rec, 0xec), fd);
        AEFile::Read(recordFlag(rec, 0xed), fd);
        AEFile::Read(recordFlag(rec, 0xee), fd);
        AEFile::Read(recordFlag(rec, 0xef), fd);
        AEFile::Read(recordFlag(rec, 0xf0), fd);
        AEFile::Read(recordFlag(rec, 0xf1), fd);
        AEFile::Read(recordFlag(rec, 0xf2), fd);
        AEFile::Read(recordFlag(rec, 0xf3), fd);
        AEFile::Read(recordFlag(rec, 0xf4), fd);
        AEFile::Read(recordFlag(rec, 0xf5), fd);
        AEFile::Read(recordFlag(rec, 0xf6), fd);
        AEFile::Read(recordFlag(rec, 0xf7), fd);
        AEFile::Read(recordFlag(rec, 0xf8), fd);
        AEFile::Read(recordFlag(rec, 0xf9), fd);
        AEFile::Read(recordFlag(rec, 0xfa), fd);
        AEFile::Read(recordFlag(rec, 0xfb), fd);
        AEFile::Read(recordFlag(rec, 0xfc), fd);
        AEFile::Read(recordFlag(rec, 0xfe), fd);
        AEFile::Read(recordFlag(rec, 0xfd), fd);
        AEFile::Read(recordFlag(rec, 0xff), fd);
        AEFile::Read(recordFlag(rec, 0x100), fd);
        AEFile::Read(rec->rank, fd);
        AEFile::Read(reinterpret_cast<int64_t &>(rec->field_0xc8), fd);
        AEFile::Read(recordFlag(rec, 0x101), fd);
        AEFile::Read(recordFlag(rec, 0x102), fd);
        int hasWingmanShip = 0;
        AEFile::Read(hasWingmanShip, fd);
        if (hasWingmanShip >= 1) {
            int wingmanShipIndex = 0;
            AEFile::Read(wingmanShipIndex, fd);
            reinterpret_cast<Ship *&>(rec->field_0x134) = (*Globals::ships)[wingmanShipIndex]->makeShip(-1);
            int wingmanRace = 0;
            AEFile::Read(wingmanRace, fd);
            reinterpret_cast<Ship *&>(rec->field_0x134)->setRace(wingmanRace);
            int equipmentCount = 0;
            AEFile::Read(equipmentCount, fd);
            if (equipmentCount >= 1) {
                Array<Item *> *equipment = new Array<Item *>();
                ArraySetLength(equipmentCount, *equipment);
                for (int i = 0; i < equipmentCount; i++) {
                    int index = 0;
                    AEFile::Read(index, fd);
                    if (index != -1) {
                        int amount = 0;
                        AEFile::Read(amount, fd);
                        (*equipment)[i] = (*Globals::items)[index]->makeItem(
                            amount, (*Globals::items)[index]->getMaxPrice());
                        bool unsaleable = false;
                        AEFile::Read(unsaleable, fd);
                        (*equipment)[i]->setUnsaleable(unsaleable);
                    } else {
                        (*equipment)[i] = 0;
                    }
                }
                reinterpret_cast<Ship *&>(rec->field_0x134)->replaceEquipment(equipment);
            }
            int itemCount = 0;
            AEFile::Read(itemCount, fd);
            if (itemCount >= 1) {
                Array<Item *> *cargo = new Array<Item *>();
                ArraySetLength(itemCount, *cargo);
                for (int i = 0; i < itemCount; i++) {
                    int index;
                    int amount;
                    int price = 0;
                    AEFile::Read(index, fd);
                    AEFile::Read(amount, fd);
                    AEFile::Read(price, fd);
                    bool unsaleable = false;
                    AEFile::Read(unsaleable, fd);
                    (*cargo)[i] = (*Globals::items)[index]->makeItem(
                        amount, (*Globals::items)[index]->getMaxPrice());
                    (*cargo)[i]->setPrice(price);
                    (*cargo)[i]->setUnsaleable(unsaleable);
                }
                reinterpret_cast<Ship *&>(rec->field_0x134)->replaceCargo(cargo);
            }
        }

        reinterpret_cast<Array<int> *&>(rec->field_0x70) = new Array<int>();
        int visitedSystems = 0;
        AEFile::Read(visitedSystems, fd);
        ArraySetLength(visitedSystems, *reinterpret_cast<Array<int> *&>(rec->field_0x70));
        for (int i = 0; i < visitedSystems; i++) {
            AEFile::Read((*reinterpret_cast<Array<int> *&>(rec->field_0x70))[i], fd);
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xd0), fd);
        AEFile::Read(recordFlag(rec, 0xd4), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xd8), fd);
        AEFile::Read(recordFlag(rec, 0xdc), fd);
        int itemCount = 0;
        AEFile::Read(itemCount, fd);
        if (itemCount >= 1) {
            Array<Item *> *items = new Array<Item *>();
            ArraySetLength(itemCount, *items);
            for (int i = 0; i < itemCount; i++) {
                int index;
                int amount;
                int price = 0;
                AEFile::Read(index, fd);
                AEFile::Read(amount, fd);
                AEFile::Read(price, fd);
                bool unsaleable = false;
                AEFile::Read(unsaleable, fd);
                (*items)[i] = (*Globals::items)[index]->makeItem(
                    amount, (*Globals::items)[index]->getMaxPrice());
                (*items)[i]->setPrice(price);
                (*items)[i]->setUnsaleable(unsaleable);
            }
            reinterpret_cast<Array<Item *> *&>(rec->field_0x180) = items;
        }
        int shipCount = 0;
        AEFile::Read(shipCount, fd);
        if (shipCount >= 1) {
            Array<Ship *> *shipList = new Array<Ship *>();
            ArraySetLength(shipCount, *shipList);
            for (int i = 0; i < shipCount; i++) {
                int index = 0;
                AEFile::Read(index, fd);
                (*shipList)[i] = (*Globals::ships)[index]->makeShip(-1);
                int race = 0;
                AEFile::Read(race, fd);
                (*shipList)[i]->setRace(race);
            }
            reinterpret_cast<Array<Ship *> *&>(rec->field_0x184) = shipList;
        }
        AEFile::Read(recordFlag(rec, 0x103), fd);
        AEFile::Read(recordFlag(rec, 0x115), fd);
        AEFile::Read(recordFlag(rec, 0x116), fd);
        AEFile::Read(count, fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x178) = new Array<bool>();
        ArraySetLength(count, *reinterpret_cast<Array<bool> *&>(rec->field_0x178));
        for (unsigned i = 0; i < reinterpret_cast<Array<bool> *&>(rec->field_0x178)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x178))[i], fd);
        }
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x1bc), fd);
        if (rec->field_0x1bc != 0x6e6a78) {
            AEFile::Close(fd);
            return rec;
        }
        int modCount = 0;
        AEFile::Read(modCount, fd);
        Array<int> *mods;
        if (modCount >= 1) {
            mods = new Array<int>();
            ArraySetLength(modCount, *mods);
            for (int i = 0; i < modCount; i++) {
                AEFile::Read((*mods)[i], fd);
            }
            reinterpret_cast<Ship *&>(rec->field_0x130)->setMods(mods);
        }
        AEFile::Read(modCount, fd);
        if (modCount >= 1) {
            Array<int> *mods = new Array<int>();
            ArraySetLength(modCount, *mods);
            for (int i = 0; i < modCount; i++) {
                AEFile::Read((*mods)[i], fd);
            }
            reinterpret_cast<Ship *&>(rec->field_0x134)->setMods(mods);
        }
        int voidShipCount = 0;
        AEFile::Read(voidShipCount, fd);
        if (voidShipCount >= 1) {
            for (int k = 0; k < voidShipCount; k++) {
                modCount = 0;
                AEFile::Read(modCount, fd);
                if (modCount >= 1) {
                    Array<int> *mods = new Array<int>();
                    ArraySetLength(modCount, *mods);
                    for (int i = 0; i < modCount; i++) {
                        AEFile::Read((*mods)[i], fd);
                    }
                    (*reinterpret_cast<Array<Ship *> *&>(rec->field_0x184))[k]->setMods(mods);
                }
            }
        }
        for (unsigned s = 0; s < stack->size() + 1; s++) {
            Station *station = s == stack->size() ? reinterpret_cast<Station *&>(rec->field_0x138) : (*stack)[s];
            if (station == 0) {
                continue;
            }
            int shipCount = 0;
            AEFile::Read(shipCount, fd);
            if (shipCount >= 1) {
                for (int j = 0; j < shipCount; j++) {
                    int count2 = 0;
                    AEFile::Read(count2, fd);
                    for (int m = 0; m < count2; m++) {
                        int mod;
                        AEFile::Read(mod, fd);
                        (*station->getShips())[j]->addMod(mod);
                    }
                }
            }
        }

        count = 0;
        AEFile::Read(count, fd);
        if (count >= 1) {
            reinterpret_cast<Array<Wanted *> *&>(rec->field_0x1b4) = new Array<Wanted *>();
            ArraySetLength(count, *reinterpret_cast<Array<Wanted *> *&>(rec->field_0x1b4));
            for (int i = 0; i < count; i++) {
                (*reinterpret_cast<Array<Wanted *> *&>(rec->field_0x1b4))[i] = this->readWanted(fd);
            }
        }
        for (unsigned i = 0; i < 4; i++) {
            AEFile::Read(rec->collectedBounties[i], fd);
        }
        AEFile::Read(recordFlag(rec, 0x117), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0x1b8), fd);
        AEFile::Read(recordFlag(rec, 0x104), fd);
        AEFile::Read(recordFlag(rec, 0x105), fd);
        AEFile::Read(recordFlag(rec, 0x108), fd);
        AEFile::Read(recordFlag(rec, 0x106), fd);
        AEFile::Read(recordFlag(rec, 0x107), fd);
        AEFile::Read(recordFlag(rec, 0x10a), fd);
        AEFile::Read(recordFlag(rec, 0x10b), fd);
        AEFile::Read(recordFlag(rec, 0x10c), fd);
        AEFile::Read(count, fd);
        reinterpret_cast<Array<bool> *&>(rec->field_0x17c) = new Array<bool>();
        ArraySetLength(count, *reinterpret_cast<Array<bool> *&>(rec->field_0x17c));
        for (unsigned i = 0; i < reinterpret_cast<Array<bool> *&>(rec->field_0x17c)->size(); i++) {
            AEFile::Read((*reinterpret_cast<Array<bool> *&>(rec->field_0x17c))[i], fd);
        }
        AEFile::Read(recordFlag(rec, 0x119), fd);
        AEFile::Read(recordFlag(rec, 0x109), fd);
        AEFile::Read(recordFlag(rec, 0x11a), fd);
        AEFile::Read(reinterpret_cast<int32_t &>(rec->field_0xe0), fd);
        AEFile::Read(recordFlag(rec, 0x10d), fd);
        AEFile::Read(recordFlag(rec, 0x10e), fd);
        AEFile::Read(recordFlag(rec, 0x10f), fd);
        AEFile::Read(recordFlag(rec, 0x110), fd);
        AEFile::Read(recordFlag(rec, 0x111), fd);
        AEFile::Read(recordFlag(rec, 0x113), fd);
        AEFile::Read(recordFlag(rec, 0x112), fd);
        AEFile::Read(recordFlag(rec, 0x114), fd);
        AEFile::Close(fd);
    }
    return rec;
}

bool RecordHandler::checkHash(unsigned int fd) {
    int size = AEFile::GetFileSize(fd);
    if (size < 0) {
        return false;
    }
    unsigned char *buffer = new unsigned char[size];
    AEFile::Read(size, buffer, fd);
    Array<unsigned char> *bytes = new Array<unsigned char>();
    ArrayAdd(buffer, size, *bytes);
    delete[] buffer;

    if (!(bytes->size() > 33)) {
        return false;
    }
    unsigned char *digest = new unsigned char[32];
    Sha256Context *context = new Sha256Context;
    SHA256_Init(context);
    SHA256_Update(context, bytes->data(), bytes->size() - 32);
    char salt[] = "#+\xc2\xa7R0LL3r(0aSter&_%=$+#";
    SHA256_Update(context, salt, 25);
    SHA256_Update(context, g_android_origami_super_club, 16);
    SHA256_Final(digest, context);

    bool valid = true;
    for (int i = 0; i < 32; i++) {
        if (digest[i] != (*bytes)[bytes->size() - 32 + i]) {
            valid = false;
            break;
        }
    }
    delete[] digest;
    delete context;
    return valid;
}

void RecordHandler::addHashToOptions() {
    signed char *data = 0;
    int length = this->readOptionsFileAsByteArray(&data);
    if (length < 0) {
        return;
    }
    unsigned char *digest = new unsigned char[32];
    Sha256Context *context = new Sha256Context;
    SHA256_Init(context);
    SHA256_Update(context, data, length);
    char salt[] = "#+\xc2\xa7R0LL3r(0aSter&_%=$+#";
    SHA256_Update(context, salt, 25);
    SHA256_Update(context, g_android_origami_super_club, 16);
    SHA256_Final(digest, context);

    signed char *hashed = new signed char[length + 32];
    memcpy(hashed, data, length);
    memcpy(hashed + length, digest, 32);
    this->writeByteArrayAsOptionsFile(hashed, length + 32);

    delete[] data;
    data = 0;
    delete[] hashed;
    delete[] digest;
    delete context;
}
