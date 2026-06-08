#include "class.h"

extern "C" void FModSound_stop(int snd);
extern "C" int FModSound_play(int snd, int track, int zero, int vol);
extern "C" int Status_inAlienOrbit();
extern "C" int Status_getCurrentCampaignMission();
extern "C" int Status_getSystem();
extern "C" int SolarSystem_getRace();
extern "C" int Status_getStation();
extern "C" int Station_getIndex(int station);
extern "C" int Status_inSupernovaSystem();
extern "C" int Status_inDeepScienceOrbit();
extern "C" int Status_getMission();
extern "C" int Mission_isEmpty();
extern "C" int Mission_getTargetStation();

extern void *const gPM_snd0 __attribute__((visibility("hidden")));   // DAT_000f57fc
extern void *const gPM_snd1 __attribute__((visibility("hidden")));   // DAT_000f5808 / 0x580c
extern void *const gPM_snd2 __attribute__((visibility("hidden")));   // DAT_000f5814
extern void *const gPM_sndStatus __attribute__((visibility("hidden"))); // DAT_000f5804
extern const int gPM_table0 __attribute__((visibility("hidden")));   // DAT_000f5800
extern const int gPM_table1 __attribute__((visibility("hidden")));   // DAT_000f5810

// Globals::playMusicAndFadeOutCurrent(int mode) — mode in r1.
extern "C" int Globals_playMusicAndFadeOutCurrent(int prev, int mode)
{
    int snd;
    int track;
    int vol = 0;

    if (mode == 2) {
        int *sndP = *(int **)gPM_snd2;
        FModSound_stop(*sndP);
        snd = *sndP;
        track = 0x91;
        return FModSound_play(snd, track, 0, vol);
    }
    if (mode == 1) {
        int *statSnd = *(int **)gPM_sndStatus;
        if (Status_inAlienOrbit() != 0) {
            int *sndP = *(int **)gPM_snd1;
            FModSound_stop(*sndP);
            snd = *sndP;
            track = 0x88;
            int m = Status_getCurrentCampaignMission();
            if (m > 0x92 && Status_getCurrentCampaignMission() < 0x9a) {
                track = 0x91;
            }
            return FModSound_play(snd, track, 0, vol);
        }
        Status_getSystem();
        SolarSystem_getRace();
        int *sndP = *(int **)gPM_snd1;
        FModSound_stop(*sndP);
        if (Station_getIndex(Status_getStation()) == 0x6c) {
            return FModSound_play(*sndP, 0x92, 0, vol);
        }
        if (Station_getIndex(Status_getStation()) == 0x65) {
            return FModSound_play(*sndP, 0x93, 0, vol);
        }
        if (Status_inSupernovaSystem() != 0) {
            if (Status_getCurrentCampaignMission() == 0x59) {
                return FModSound_play(*sndP, 0x8be, 0, vol);
            }
            if (Status_getMission() != 0 && Mission_isEmpty() == 0) {
                Status_getMission();
                int tgt = Mission_getTargetStation();
                if (tgt == Station_getIndex(Status_getStation())) {
                    int cm = Status_getCurrentCampaignMission();
                    track = cm < 0x6a ? 0x8c1 : 0x8c2;
                    return FModSound_play(*sndP, track, 0, vol);
                }
            }
            return FModSound_play(*sndP, 0x94, 0, vol);
        }
        if (Status_inDeepScienceOrbit() != 0) {
            return FModSound_play(*sndP, 0x98, 0, vol);
        }
        if (Station_getIndex(Status_getStation()) == 0x78 &&
            (Status_getCurrentCampaignMission() == 0x7e ||
             Status_getCurrentCampaignMission() == 0x85)) {
            return FModSound_play(*sndP, 0x8bf, 0, vol);
        }
        const int *table = &gPM_table1;
        track = table[SolarSystem_getRace()];
        return FModSound_play(*sndP, track, 0, vol);
    }
    if (mode != 0) {
        return prev;
    }

    Status_getSystem();
    int race = SolarSystem_getRace();
    int *sndP = *(int **)gPM_snd0;
    FModSound_stop(*sndP);
    if (Station_getIndex(Status_getStation()) == 0x6c) {
        return FModSound_play(*sndP, 0x84, 0, vol);
    }
    if (Station_getIndex(Status_getStation()) == 0x65) {
        return FModSound_play(*sndP, 0x83, 0, vol);
    }
    int idx = Station_getIndex(Status_getStation());
    if (idx == 10 || Station_getIndex(Status_getStation()) == 100) {
        if (Station_getIndex(Status_getStation()) == 10 &&
            Status_getCurrentCampaignMission() == 0x9f) {
            return FModSound_play(*sndP, 0x90, 0, vol);
        }
        return FModSound_play(*sndP, 0x85, 0, vol);
    }
    const int *table = &gPM_table0;
    track = table[race];
    return FModSound_play(*sndP, track, 0, vol);
}
