#ifndef GOF2_GAME_GAMESETTINGS_H
#define GOF2_GAME_GAMESETTINGS_H

#include <cstdint>

struct GameSettings {
    uint8_t field_0x0[0x35];

    union {
        // lint: union_decl byte 0x35 read as two distinct flags cross-file: blackMarketUnlockedFlag (Status.cpp) and settingSkipCampaignFlag (Level.cpp)
        uint8_t blackMarketUnlockedFlag;
        char settingSkipCampaignFlag;
    };

    uint8_t hardCoreFlag;

    char settingSkipIntroFlag;

    uint8_t field_0x38[0x1c];

    int steerAnchorX;
    int fireAnchorX;
};

#endif
