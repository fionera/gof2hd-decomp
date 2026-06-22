#ifndef GOF2_GAME_GAMESETTINGS_H
#define GOF2_GAME_GAMESETTINGS_H

#include <cstdint>

// Foreign engine "game settings" record reached through the global
// `g_init_settings` (a GameSettings** that points at the live settings object).
// Only a handful of byte flags are understood so far; the leading bytes are
// kept as an opaque pad so named fields land at their exact original offsets
// without shifting anything. Mid-record bytes that are not yet understood get
// their own placeholders so a later field can claim them in place.
//
// Confirmed from android_2.0.16 Level::init (Ghidra @ 000be304):
//   *(char *)(settings + 0x37) == 0  -> settingSkipIntroFlag
//   *(char *)(settings + 0x35) == 0  -> settingSkipCampaignFlag
// Confirmed from android_2.0.16 Status::resetGame (Ghidra @ 000bd304):
//   cVar1 = *(char *)(settings + 0x36)            -> hardCoreFlag (new-game
//           hardcore difficulty; sets Status::field_0x114 = 3 when nonzero)
//   if (*(char *)(settings + 0x35) != 0) ...      -> blackMarketUnlockedFlag
//           (unlocks solar-system visibility index 0x19)
// All reads are 1-byte (char) loads.
//
// 0x35 and 0x36 each carry multiple understood meanings across call sites, so
// each offset is a union of its known names. They all alias the SAME byte; the
// mangling is unaffected because only the spelling of the access changes.
//
// Confirmed from android_2.0.16 Globals::Globals (Ghidra @ 000f3228) and
// MenuTouchWindow::OnTouchBegin (Ghidra @ 00139bd4, case 0xb cinematic
// hit-testing):
//   *(int *)(settings + 0x54)  -> steerAnchorX  (int32 X anchor of the
//           cinematic steer hit-zone; hit window is [x-0x14, x+300])
//   *(int *)(settings + 0x58)  -> fireAnchorX   (int32 X anchor of the
//           cinematic fire hit-zone; hit window is [x-0x14, x+0xe6])
// Both are initialized to aspect/orientation-dependent constants in
// Globals::Globals (puVar8[0x15]/[0x16] = 0x247/0x33e/0x19f, 0x201/0x2da/0x16d).
struct GameSettings {
    uint8_t field_0x0[0x35]; // 0x00..0x34 unmodeled
    union {
        // 0x35 char option/flag byte
        uint8_t blackMarketUnlockedFlag; // unlocks system 0x19 (Status::resetGame)
        char settingSkipCampaignFlag; // skip-campaign-intro option (Level::init)
        uint8_t field_0x35;
    };

    union {
        // 0x36 char flag byte
        uint8_t hardCoreFlag; // new-game hardcore difficulty (Status::resetGame)
        uint8_t field_0x36;
    };

    char settingSkipIntroFlag; // 0x37 char: skip-intro option flag

    uint8_t field_0x38[0x1c]; // 0x38..0x53 unmodeled

    int steerAnchorX; // 0x54 int32: X anchor of cinematic steer hit-zone
    int fireAnchorX;  // 0x58 int32: X anchor of cinematic fire hit-zone
};

#endif // GOF2_GAME_GAMESETTINGS_H
