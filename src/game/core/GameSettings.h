#ifndef GOF2_GAME_GAMESETTINGS_H
#define GOF2_GAME_GAMESETTINGS_H

#include <cstddef>
#include <cstdint>

struct GameSettings {
    uint8_t field_0x0[0x35];

    // Byte 0x35 is read as two distinct flags across files: blackMarketUnlockedFlag (Status.cpp)
    // and (formerly) settingSkipCampaignFlag (Level.cpp). They are the same 1-byte slot; the
    // Level.cpp use was renamed to blackMarketUnlockedFlag.
    uint8_t blackMarketUnlockedFlag;

    uint8_t hardCoreFlag;

    char settingSkipIntroFlag;

    uint8_t field_0x38[0x1c];

    int steerAnchorX;
    int fireAnchorX;
};

#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(GameSettings, blackMarketUnlockedFlag) == 0x35, "blackMarketUnlockedFlag");
static_assert(offsetof(GameSettings, hardCoreFlag) == 0x36, "hardCoreFlag");
#endif

#endif
