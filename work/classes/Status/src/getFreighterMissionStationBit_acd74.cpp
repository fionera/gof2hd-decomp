#include "class.h"

// Maps a station id to its bit position in the freighter-mission visited mask (0 if none).
int Status::getFreighterMissionStationBit(int station) {
    if (station == 0xf) return 4;
    if (station == 0x5f) return 6;
    if (station == 0x28) return 3;
    if (station == 0x2d) return 9;
    if (station == 0x3c) return 5;
    if (station == 0x46) return 7;
    if (station == 0x50) return 8;
    if (station == 0x55) return 1;
    if (station == 0x1e) return 2;
    return 0;
}
