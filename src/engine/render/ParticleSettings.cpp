#include "engine/render/ParticleSettings.h"
#include "platform/libc.h"

ParticleSettings::ParticleSettings() {
    // The 48 embedded ParticleSet::name Strings default-construct automatically.
}

ParticleSettings::~ParticleSettings() {
    // The 48 embedded ParticleSet::name Strings destruct automatically.
}

// init() builds the table of built-in emitter presets. The preset names come from the
// shared ParticleSettings_str blob indexed by a byte offset; the per-entry sub-emitter
// init / copy helpers are the recovered external entry points.

extern "C" {
void *ParticleSet_assign(void *dst, const void *src);
void ParticleSettings_initSub(void *dst, void *parent);
extern const char ParticleSettings_str[];
}

// Reinterpret a uint32_t slot as a float lvalue (several ParticleSet fields hold a float
// bit pattern in a generic 32-bit slot).
static inline float &asFloat(uint32_t &slot) {
    return reinterpret_cast<float &>(slot);
}

int ParticleSettings::init() {
    String buf;

    // --- Defaults applied to all 48 emitter slots -----------------------------------
    for (uint32_t i = 0; i < 0x30; i = i + 1) {
        buf.ctor_char(ParticleSettings_str, false);
        this->sets[i].lifeRandom = 0;
        asFloat(this->sets[i].startSize) = 0.0f;
        asFloat(this->sets[i].endSize) = 0.0f;
        asFloat(this->sets[i].velocityFromSlot) = 0.0f;
        this->sets[i].oneShot = 0;             // DAT_00194068 low  = 0x00000000
        this->sets[i].color0 = 0x00ff00ff;     // DAT_00194068 high
        this->sets[i].color1 = 0x00ff00ff;     // DAT_00194070 low
        this->sets[i].fadeFrames = 0;          // DAT_00194070 high = 0
        this->sets[i].flags = 0x10;
        this->sets[i].count = 1;
        asFloat(this->sets[i].lifeBase) = 100.0f;   // DAT_001940dc
        this->sets[i].lifetime = 100;
        asFloat(this->sets[i].flLifetime) = 500.0f; // DAT_001940e0
        asFloat(this->sets[i].colorFlag) = 0.0f;
        this->sets[i].posBase = 500;
        memset(&this->sets[i].posSpread, 0, 0x48);
        asFloat(this->sets[i].uvU1) = 63.0f;   // DAT_00194078
        asFloat(this->sets[i].uvV1) = 63.0f;
        this->sets[i].speedThreshold = 0;
        this->sets[i].frames = 0;
    }

    // --- sets[0]: "engine" base trail -----------------------------------------------
    buf.ctor_char(ParticleSettings_str + 401400, false);  // DAT_00194108
    this->sets[0].oneShot = 1;                 // DAT_00194210 low  = 0x00000001
    this->sets[0].color0 = 0xffffffff;         // DAT_00194210 high
    this->sets[0].color1 = 0x000000ff;         // DAT_00194218 low
    this->sets[0].fadeFrames = 0;              // DAT_00194218 high = 0
    this->sets[0].posBase = (int32_t)0xfffffe0c;         // DAT_001941f0 low  = -500
    this->sets[0].posSpread = 0;                         // DAT_001941f0 high = 0x00000000
    this->sets[0].ySpread = 0;                           // DAT_001941f8 low  = 0x00000000
    this->sets[0].velSpread = 100;                       // DAT_001941f8 high = 0x00000064
    asFloat(this->sets[0].uvU0) = 0.6875f;               // DAT_00194200 low  = 0x3f300000
    asFloat(this->sets[0].uvV0) = 0.001f;                // DAT_00194200 high = 0x3a83126f
    asFloat(this->sets[0].uvU1) = 0.7425f;               // DAT_00194208 low  = 0x3f3e147b
    asFloat(this->sets[0].uvV1) = 0.0625f;               // DAT_00194208 high = 0x3d800000
    this->sets[0].lifetime = 100;
    asFloat(this->sets[0].flLifetime) = 20.0f;           // DAT_00194228 = 0x41a00000
    this->sets[0].flags = 0x11;
    this->sets[0].count = 0x14;
    asFloat(this->sets[0].lifeBase) = 250.0f;            // DAT_00194230
    this->sets[0].field_0x54 = 0;
    asFloat(this->sets[0].velDir) = -4000.0f;            // DAT_0019422c
    asFloat(this->sets[0].posRight) = 60.0f;             // DAT_00194234
    asFloat(this->sets[0].posUp) = 30.0f;                // DAT_00194224
    asFloat(this->sets[0].posDir) = -900.0f;             // DAT_001941ec
    this->sets[0].speedThreshold = 1;
    ParticleSettings_initSub(&this->sets[29], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[30], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[31], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[32], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[33], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[34], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[35], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[36], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[37], &this->sets[0]);
    ParticleSettings_initSub(&this->sets[38], &this->sets[0]);

    // --- sets[4]: white spark field -------------------------------------------------
    buf.ctor_char(ParticleSettings_str + 401182, false);  // DAT_00194238
    asFloat(this->sets[4].flLifetime) = 200.0f;          // DAT_001942e4
    this->sets[4].lifetime = 2000;
    this->sets[4].color0 = 0xffffffff;
    this->sets[4].flags = 0x81;
    this->sets[4].count = 500;
    asFloat(this->sets[4].lifeBase) = 20.0f;             // DAT_00194228 = 0x41a00000 (carried in uVar18)
    this->sets[4].lifeRandom = 0x28;
    this->sets[4].velocityFromSlot = 0;
    this->sets[4].oneShot = 0;
    this->sets[4].color1 = 0;
    this->sets[4].velBaseY = 0;
    asFloat(this->sets[4].velDir) = -30000.0f;           // DAT_001942f0
    this->sets[4].posBase = 0;
    this->sets[4].posSpread = 10000;
    this->sets[4].ySpread = 10000;
    this->sets[4].velSpread = 5;
    this->sets[4].field_0x54 = 0;
    asFloat(this->sets[4].uvU0) = 0.0f;                  // DAT_00194eb8 low  = 0x00000000
    asFloat(this->sets[4].uvV0) = 1.0f;                  // DAT_00194eb8 high = 0x3f800000
    asFloat(this->sets[4].uvU1) = 1.0f;                  // DAT_00194ec0 low  = 0x3f800000
    asFloat(this->sets[4].uvV1) = 0.0f;                  // DAT_00194ec0 high = 0x00000000
    this->sets[4].fadeFrames = 500;
    asFloat(this->sets[4].posRight) = 10000.0f;          // DAT_001942e0 = 0x461c4000
    asFloat(this->sets[4].posUp) = 5000.0f;              // DAT_001942e8
    asFloat(this->sets[4].posDir) = 2000.0f;             // DAT_001942f4
    this->sets[4].speedThreshold = 1;

    // --- sets[5]: smoke puff --------------------------------------------------------
    buf.ctor_char(ParticleSettings_str + 401012, false);  // DAT_001942ec
    this->sets[5].oneShot = 0;                 // DAT_00194ec8 low  = 0x00000000
    this->sets[5].color0 = 0xffffffff;         // DAT_00194ec8 high
    this->sets[5].color1 = 0;                  // DAT_00194ed0 low  = 0x00000000
    this->sets[5].fadeFrames = 0x32;           // DAT_00194ed0 high = 0x00000032
    this->sets[5].flags = (uint32_t)(176146 + 0x5c000);  // DAT_00194388 = 0x0002b012
    this->sets[5].count = 0x32;
    asFloat(this->sets[5].lifeBase) = 70.0f;             // DAT_00194390
    asFloat(this->sets[5].startSize) = 0.0f;
    asFloat(this->sets[5].endSize) = 500.0f;             // DAT_00194394
    this->sets[5].velocityFromSlot = 0;
    this->sets[5].lifetime = 2000;
    asFloat(this->sets[5].flLifetime) = 200.0f;          // DAT_001942e4 = 0x43480000 (carried in uVar17)
    asFloat(this->sets[5].velBaseY) = -15.0f;            // DAT_00194398
    this->sets[5].velDir = 0;
    this->sets[5].posBase = 0;
    this->sets[5].posSpread = 10000;
    this->sets[5].ySpread = 10000;
    this->sets[5].velSpread = 5;
    this->sets[5].field_0x54 = 0;
    this->sets[5].posRight = 0;
    this->sets[5].posUp = 0;
    asFloat(this->sets[5].posDir) = 20000.0f;            // DAT_00194384
    asFloat(this->sets[5].uvU0) = 0.876953f;             // DAT_00194ed8 low  = 0x3f607ffe
    asFloat(this->sets[5].uvV0) = 0.248047f;             // DAT_00194ed8 high = 0x3e7e0008
    asFloat(this->sets[5].uvU1) = 0.935547f;             // DAT_00194ee0 low  = 0x3f6f8002
    asFloat(this->sets[5].uvV1) = 0.189453f;             // DAT_00194ee0 high = 0x3e41fff8
    this->sets[5].speedThreshold = 1;

    // --- sets[6]: bigger smoke puff (variant of sets[5]) ----------------------------
    buf.ctor_char(ParticleSettings_str + 400875, false);  // DAT_0019438c
    asFloat(this->sets[6].lifeBase) = 50.0f;             // DAT_001943e0
    this->sets[6].lifetime = 500;
    asFloat(this->sets[6].endSize) = asFloat(this->sets[5].endSize) * 5.0f;

    // --- sets[2]: muzzle flash, sets[3] copies it -----------------------------------
    buf.ctor_char(ParticleSettings_str + 400742, false);  // DAT_001943e4
    ParticleSet *muzzleFlash = &this->sets[2];
    this->sets[2].posBase = 0;
    this->sets[2].posSpread = 0;
    this->sets[2].ySpread = 0;
    this->sets[2].velSpread = 0;
    this->sets[2].flags = 176146;              // DAT_0019449c = 0x0002b012
    this->sets[2].count = 0xb;
    asFloat(this->sets[2].lifeBase) = 200.0f;            // DAT_001944a4 = 0x43480000
    this->sets[2].lifetime = 800;
    asFloat(this->sets[2].flLifetime) = 125.0f;          // DAT_00194494
    this->sets[2].oneShot = 0;
    this->sets[2].color0 = 0x555555ff;         // DAT_00194490 = bytes ff 55 55 55 (LE)
    this->sets[2].color1 = 0;
    asFloat(this->sets[2].colorFlag) = 125.0f;           // DAT_00194494 (carried in uVar20)
    this->sets[2].velDir = 0;
    this->sets[2].field_0x54 = 0;
    asFloat(this->sets[2].posRight) = 55.0f;             // DAT_0019448c
    asFloat(this->sets[2].posUp) = 50.0f;                // DAT_001943e0 (carried in uVar15)
    asFloat(this->sets[2].posDir) = -230.0f;             // DAT_00194498 = 0xc3660000
    asFloat(this->sets[2].uvU0) = 0.12109375f;           // DAT_001944a8
    asFloat(this->sets[2].uvV0) = 0.998046875f;          // DAT_001944a0
    this->sets[2].uvU1 = 0x3b000000;
    asFloat(this->sets[2].uvV1) = 0.998046875f;          // DAT_001944a0 (carried in uVar6)
    this->sets[2].speedThreshold = 1;
    ParticleSet_assign(&this->sets[3], (void *)muzzleFlash);
    asFloat(this->sets[3].lifeBase) = 100.0f;            // DAT_001944b4 = 0x42c80000
    this->sets[3].lifetime = 1000;
    asFloat(this->sets[3].posRight) = 160.0f;            // DAT_001944b8
    asFloat(this->sets[3].posUp) = 90.0f;                // DAT_001944b0

    // --- sets[13]: explosion ring (first definition) --------------------------------
    buf.ctor_char(ParticleSettings_str, false);
    this->sets[13].oneShot = 0;                // DAT_00195330 low  = 0x00000000
    this->sets[13].color0 = 0xffffffff;        // DAT_00195330 high
    this->sets[13].color1 = 0x000000ff;        // DAT_00195338 low  = 0x000000ff
    this->sets[13].fadeFrames = 0x64;          // DAT_00195338 high = 0x00000064
    this->sets[13].flags = 0x11;
    this->sets[13].count = 0x3c;
    asFloat(this->sets[13].lifeBase) = 150.0f;           // DAT_00194548
    this->sets[13].lifetime = 0x5dc;
    asFloat(this->sets[13].flLifetime) = 75.0f;          // DAT_0019454c
    this->sets[13].posBase = 0;
    this->sets[13].velSpread = 0x19;
    this->sets[13].field_0x54 = 0;
    this->sets[13].velDir = 0;
    asFloat(this->sets[13].posRight) = 60.0f;            // DAT_00194550
    asFloat(this->sets[13].posUp) = 30.0f;               // DAT_00194554
    asFloat(this->sets[13].uvU0) = 0.0f;
    asFloat(this->sets[13].uvV0) = 0.0f;
    this->sets[13].uvU1 = 0x3f800000;          // 1.0f
    this->sets[13].uvV1 = 0x3f800000;          // 1.0f
    this->sets[13].speedThreshold = 1;

    // --- sets[8]: glow dots ---------------------------------------------------------
    buf.ctor_char(ParticleSettings_str + 400482, false);  // DAT_00194544
    this->sets[8].posBase = 0;                  // DAT_00195350 low  = 0x00000000
    this->sets[8].posSpread = 40000;            // DAT_00195350 high = 0x00009c40
    this->sets[8].ySpread = 40000;              // DAT_00195358 low  = 0x00009c40
    this->sets[8].velSpread = 0;                // DAT_00195358 high = 0x00000000
    asFloat(this->sets[8].uvU0) = 0.0f;                  // DAT_00195340 low  = 0x00000000
    asFloat(this->sets[8].uvV0) = 0.0f;                  // DAT_00195340 high = 0x00000000
    asFloat(this->sets[8].uvU1) = 1.0f;                  // DAT_00195348 low  = 0x3f800000
    asFloat(this->sets[8].uvV1) = 1.0f;                  // DAT_00195348 high = 0x3f800000
    this->sets[8].flags = 0x01000041;          // DAT_001945e0
    this->sets[8].count = 0x1e;
    this->sets[8].lifeBase = 0x47000000;
    this->sets[8].lifetime = 0x7fffffff;        // DAT_001945dc
    this->sets[8].color0 = 0xffffffff;
    this->sets[8].color1 = 0xffffffff;
    this->sets[8].fadeFrames = 0;
    this->sets[8].field_0x54 = 0;
    this->sets[8].velDir = 0;
    this->sets[8].posRight = 0;
    this->sets[8].posUp = 0;
    this->sets[8].posDir = 0;

    // --- sets[7]: faint grey dust ---------------------------------------------------
    buf.ctor_char(ParticleSettings_str, false);
    this->sets[7].posBase = 0;
    this->sets[7].posSpread = 0;
    this->sets[7].ySpread = 0;
    this->sets[7].velSpread = 0;
    asFloat(this->sets[7].uvU0) = 0.01f;                 // DAT_00195360 low  = 0x3c23d70a
    asFloat(this->sets[7].uvV0) = 0.01f;                 // DAT_00195360 high = 0x3c23d70a
    asFloat(this->sets[7].uvU1) = 0.99f;                 // DAT_00195368 low  = 0x3f7d70a4
    asFloat(this->sets[7].uvV1) = 0.99f;                 // DAT_00195368 high = 0x3f7d70a4
    this->sets[7].flags = 0x81;
    this->sets[7].count = 0xf;
    asFloat(this->sets[7].lifeBase) = 10000.0f;          // DAT_00194670
    this->sets[7].lifetime = 0x7fffffff;        // DAT_001945dc (carried in uVar15)
    this->sets[7].color0 = 0xbbbbbbbb;
    this->sets[7].color1 = 0xbbbbbbbb;
    this->sets[7].fadeFrames = 0;
    this->sets[7].field_0x54 = 0;
    this->sets[7].velDir = 0;
    asFloat(this->sets[7].posRight) = 10000.0f;          // DAT_00194670
    asFloat(this->sets[7].posUp) = 5000.0f;              // DAT_00194678
    asFloat(this->sets[7].posDir) = 1000.0f;             // DAT_00194674

    // --- sets[9]: spark burst, copied into sets[16] ---------------------------------
    buf.ctor_char(ParticleSettings_str + 400195, false);  // DAT_0019466c
    ParticleSet *spark = &this->sets[9];
    this->sets[9].lifetime = 700;
    this->sets[9].flLifetime = 0x41000000;     // 8.0f
    this->sets[9].flags = 0x02000021;          // DAT_0019473c
    ParticleSet *dustCloud = &this->sets[16];
    this->sets[9].count = 0x10;
    asFloat(this->sets[9].lifeBase) = 100.0f;            // DAT_00194740
    this->sets[9].lifeRandom = 1000;
    this->sets[9].color0 = 0xffffffff;
    this->sets[9].color1 = 0xffffffff;
    this->sets[9].fadeFrames = 0;
    this->sets[9].velDir = 0;
    this->sets[9].posRight = 0;
    this->sets[9].posUp = 0;
    asFloat(this->sets[9].posDir) = -250.0f;             // DAT_00194744
    this->sets[9].posBase = 500;
    this->sets[9].posSpread = 300;
    this->sets[9].ySpread = 300;
    this->sets[9].velSpread = 0;
    this->sets[9].field_0x54 = 0;
    this->sets[9].drag = 0x3f800000;           // 1.0f
    asFloat(this->sets[9].posDirRandom) = 500.0f;        // DAT_00195370 low  = 0x43fa0000
    asFloat(this->sets[9].uvU0) = 0.0f;                  // DAT_00195370 high = 0x00000000
    asFloat(this->sets[9].uvV0) = 0.0f;                  // DAT_00195378 low  = 0x00000000
    asFloat(this->sets[9].uvU1) = 0.25f;                 // DAT_00195378 high = 0x3e800000
    this->sets[9].oneShot = 1;
    this->sets[9].uvV1 = 0x3e800000;           // 0.25f
    this->sets[9].frames = 0x10;
    ParticleSet_assign((void *)dustCloud, (void *)spark);

    // --- sets[16]: dust cloud (variant of sets[9]) ----------------------------------
    buf.ctor_char(ParticleSettings_str + 399927, false);  // DAT_00194748
    asFloat(this->sets[16].posDir) = -300.0f;            // DAT_00195380 low  = 0xc3960000
    asFloat(this->sets[16].posDirRandom) = 800.0f;       // DAT_00195380 high = 0x44480000
    asFloat(this->sets[16].uvU0) = 0.876953125f;         // DAT_00195388 low  = 0x3f608000
    asFloat(this->sets[16].uvV0) = 0.001953125f;         // DAT_00195388 high = 0x3b000000
    this->sets[16].frames = 0;
    this->sets[16].color1 = 0xff;
    this->sets[16].fadeFrames = 300;
    this->sets[16].lifetime = 300;
    this->sets[16].flLifetime = 0x41000000;    // 8.0f
    this->sets[16].count = 0xd;
    asFloat(this->sets[16].lifeBase) = 200.0f;           // DAT_001947d4
    this->sets[16].lifeRandom = 0x578;
    this->sets[16].posSpread = 600;
    this->sets[16].ySpread = 300;
    asFloat(this->sets[16].uvU1) = 0.9970703125f;        // DAT_001947dc
    asFloat(this->sets[16].uvV1) = 0.2470703125f;        // DAT_001947d8
    ParticleSet_assign(&this->sets[17], (void *)dustCloud);
    this->sets[17].lifetime = 400;

    // --- sets[17]: dust cloud variant -----------------------------------------------
    buf.ctor_char(ParticleSettings_str, false);
    asFloat(this->sets[17].uvU0) = 0.876953125f;         // DAT_00195390 low  = 0x3f608000
    asFloat(this->sets[17].uvV0) = 0.251953125f;         // DAT_00195390 high = 0x3e810000
    asFloat(this->sets[17].uvU1) = 0.9970703125f;        // DAT_00195398 low  = 0x3f7f4000
    asFloat(this->sets[17].uvV1) = 0.4970703125f;        // DAT_00195398 high = 0x3efe8000
    ParticleSet_assign(&this->sets[18], (void *)dustCloud);
    this->sets[18].lifetime = 200;

    // --- sets[18]: dust cloud variant -----------------------------------------------
    buf.ctor_char(ParticleSettings_str, false);
    asFloat(this->sets[18].uvU0) = 0.751953125f;         // DAT_001953a0 low  = 0x3f408000
    asFloat(this->sets[18].uvV0) = 0.501953125f;         // DAT_001953a0 high = 0x3f008000
    asFloat(this->sets[18].uvU1) = 0.9970703125f;        // DAT_001953a8 low  = 0x3f7f4000
    asFloat(this->sets[18].uvV1) = 0.7470703125f;        // DAT_001953a8 high = 0x3f3f4000

    // --- sets[12]: fire trail -------------------------------------------------------
    buf.ctor_char(ParticleSettings_str, false);
    this->sets[12].lifetime = 0x4e2;
    asFloat(this->sets[12].flLifetime) = 60.0f;          // DAT_00194914
    this->sets[12].flags = 0x02000021;         // DAT_00194918
    this->sets[12].count = 0x4c;
    asFloat(this->sets[12].lifeBase) = 250.0f;           // DAT_0019491c
    this->sets[12].lifeRandom = 0x32;
    this->sets[12].color0 = 0xffffffff;
    this->sets[12].color1 = 0xffffff00;
    this->sets[12].fadeFrames = 0x96;
    asFloat(this->sets[12].velDir) = -6000.0f;           // DAT_00194924
    this->sets[12].posRight = 0;
    this->sets[12].posUp = 0;
    asFloat(this->sets[12].posDir) = -700.0f;            // DAT_00194920
    this->sets[12].posBase = 0xfa;
    this->sets[12].posSpread = 0;
    this->sets[12].ySpread = 0;
    this->sets[12].velSpread = 0;
    this->sets[12].field_0x54 = 0;
    this->sets[12].drag = 0;
    asFloat(this->sets[12].posDirRandom) = 0.0f;         // DAT_001953b0 low  = 0x00000000
    asFloat(this->sets[12].uvU0) = 0.0f;                 // DAT_001953b0 high = 0x00000000
    asFloat(this->sets[12].uvV0) = 0.0f;                 // DAT_001953b8 low  = 0x00000000
    asFloat(this->sets[12].uvU1) = 0.25f;                // DAT_001953b8 high = 0x3e800000
    this->sets[12].oneShot = 0;
    this->sets[12].uvV1 = 0x3e800000;          // 0.25f
    this->sets[12].frames = 0x10;

    // --- sets[13]: fire column (second, full definition) ----------------------------
    buf.ctor_char(ParticleSettings_str + 399539, false);  // DAT_00194910
    this->sets[13].lifetime = 1000;
    asFloat(this->sets[13].flLifetime) = 300.0f;         // DAT_001949ec
    this->sets[13].flags = (uint32_t)(33554465 - 0x10);  // DAT_001949e8 = 0x02000021
    this->sets[13].count = 0x137;
    asFloat(this->sets[13].lifeBase) = 800.0f;           // DAT_001949f0 = 0x44480000
    this->sets[13].color0 = 0xffffffff;
    this->sets[13].lifeRandom = 0;
    this->sets[13].color1 = 0xffffff00;
    this->sets[13].fadeFrames = 0x96;
    this->sets[13].velDir = 0;
    this->sets[13].posRight = 0;
    asFloat(this->sets[13].posUp) = 100.0f;              // DAT_001949f4
    asFloat(this->sets[13].posDirRandom) = 0.0f;         // DAT_001953b0 low (carried in uVar16) = 0x00000000
    asFloat(this->sets[13].uvU0) = 0.0f;                 // DAT_001953b0 high = 0x00000000
    asFloat(this->sets[13].uvV0) = 0.0f;                 // DAT_001953b8 low  = 0x00000000
    asFloat(this->sets[13].uvU1) = 0.25f;                // DAT_001953b8 high = 0x3e800000
    asFloat(this->sets[13].posDir) = 200.0f;             // DAT_001949f8
    this->sets[13].posBase = 0x5dc;
    this->sets[13].posSpread = 0;
    this->sets[13].ySpread = 0;
    this->sets[13].velSpread = 0;
    this->sets[13].field_0x54 = 0;
    this->sets[13].drag = 0;
    this->sets[13].oneShot = 0;
    this->sets[13].uvV1 = 0x3e800000;          // 0.25f
    this->sets[13].frames = 0x10;

    // --- sets[10]: streak (first variant) -------------------------------------------
    buf.ctor_char(ParticleSettings_str + 399421, false);  // DAT_001949e4
    this->sets[10].posSpread = 0;
    this->sets[10].ySpread = 0;
    this->sets[10].velSpread = 0;
    this->sets[10].field_0x54 = 0;
    this->sets[10].lifetime = 0x1c2;
    int32_t streakFlags = 33554465 + 0xe0;     // DAT_00194a9c = 0x02000021
    this->sets[10].flags = (uint32_t)streakFlags;
    this->sets[10].count = 10;
    asFloat(this->sets[10].lifeBase) = 300.0f;           // DAT_00194aa0
    this->sets[10].lifeRandom = 100;
    this->sets[10].color0 = 0xffffffff;
    this->sets[10].color1 = 0xffffffff;
    this->sets[10].fadeFrames = 0;
    this->sets[10].posBase = 500;
    this->sets[10].velDir = 0;
    this->sets[10].posRight = 0;
    this->sets[10].posUp = 0;
    this->sets[10].posDir = 0;
    this->sets[10].drag = 0;
    asFloat(this->sets[10].uvU0) = 0.0f;
    asFloat(this->sets[10].uvV0) = 0.0f;
    this->sets[10].uvU1 = 0x3e800000;          // 0.25f
    this->sets[10].uvV1 = 0x3e800000;          // 0.25f
    this->sets[10].frames = 0x10;

    // --- sets[11]: streak (second variant) ------------------------------------------
    buf.ctor_char(ParticleSettings_str + 399242, false);  // DAT_00194a98
    this->sets[11].posSpread = 0;
    this->sets[11].ySpread = 0;
    this->sets[11].velSpread = 0;
    this->sets[11].field_0x54 = 0;
    this->sets[11].lifetime = 0x5dc;
    this->sets[11].flags = (uint32_t)streakFlags;
    this->sets[11].count = 10;
    asFloat(this->sets[11].lifeBase) = 2000.0f;          // DAT_00194b34
    this->sets[11].lifeRandom = 1000;
    this->sets[11].color0 = 0xffffffff;
    this->sets[11].color1 = 0xffffffff;
    this->sets[11].fadeFrames = 0;
    this->sets[11].posBase = 500;
    this->sets[11].velDir = 0;
    this->sets[11].posRight = 0;
    this->sets[11].posUp = 0;
    this->sets[11].posDir = 0;
    this->sets[11].drag = 0;
    asFloat(this->sets[11].uvU0) = 0.0f;
    asFloat(this->sets[11].uvV0) = 0.0f;
    this->sets[11].uvU1 = 0x3e800000;          // 0.25f
    this->sets[11].uvV1 = 0x3e800000;          // 0.25f
    this->sets[11].frames = 0x10;

    // --- sets[15]: ember / debris, copied into sets[19], sets[14], sets[40] ----------
    buf.ctor_char(ParticleSettings_str + 399109, false);  // DAT_00194b30
    ParticleSet *ember = &this->sets[15];
    this->sets[15].lifetime = 0x5dc;
    asFloat(this->sets[15].flLifetime) = 20.0f;          // DAT_00194bec
    this->sets[15].flags = 0x02000021;         // DAT_00194bf0
    this->sets[15].count = 0x1e;
    asFloat(this->sets[15].lifeBase) = 666.0f;           // DAT_00194bf4
    this->sets[15].lifeRandom = 0xde;
    this->sets[15].color0 = 0xffffffff;
    this->sets[15].color1 = 0xffffff00;
    this->sets[15].fadeFrames = 200;
    this->sets[15].velDir = 0;
    this->sets[15].posUp = 0;
    asFloat(this->sets[15].posDir) = -250.0f;            // DAT_00194bf8
    asFloat(this->sets[15].posDirRandom) = 500.0f;       // DAT_00194bfc
    this->sets[15].posBase = 0x8ae;
    this->sets[15].posSpread = 200;
    this->sets[15].ySpread = 200;
    this->sets[15].velSpread = 0;
    this->sets[15].drag = 0xc0000000;          // -2.0f
    this->sets[15].oneShot = 1;
    asFloat(this->sets[15].uvU0) = 0.0f;                 // DAT_001953c0 low (carried in uVar16) = 0x00000000
    asFloat(this->sets[15].uvV0) = 0.0f;                 // DAT_001953c0 high = 0x00000000
    asFloat(this->sets[15].uvU1) = 0.25f;                // DAT_001953c8 low  = 0x3e800000
    asFloat(this->sets[15].uvV1) = 0.25f;                // DAT_001953c8 high = 0x3e800000
    this->sets[15].frames = 0x10;

    // --- sets[42]: spark variant ----------------------------------------------------
    buf.ctor_char(ParticleSettings_str + 398969, false);  // DAT_00194be8
    this->sets[42].lifetime = 600;
    this->sets[42].flags = 0x02000021;         // DAT_00194bf0 (carried in uVar17)
    asFloat(this->sets[42].flLifetime) = 20.0f;          // DAT_00194bec (carried in uVar15)
    this->sets[42].count = 0xc;
    asFloat(this->sets[42].lifeBase) = 400.0f;           // DAT_00194cf4
    this->sets[42].lifeRandom = 200;
    this->sets[42].color0 = 0xffffffb2;
    this->sets[42].color1 = 0xffffff00;
    this->sets[42].fadeFrames = 100;
    this->sets[42].posBase = 0xde;
    this->sets[42].velSpread = 0;
    this->sets[42].velDir = 0;
    this->sets[42].posUp = 0;
    asFloat(this->sets[42].posDir) = 200.0f;             // DAT_00194cf8
    this->sets[42].posSpread = 100;
    asFloat(this->sets[42].posDirRandom) = 200.0f;       // DAT_00194cf8 (carried in uVar15)
    this->sets[42].ySpread = 100;
    asFloat(this->sets[42].drag) = -1.0f;                // DAT_00194cfc
    this->sets[42].oneShot = 1;
    this->sets[42].frames = 0x10;
    asFloat(this->sets[42].uvU0) = 0.0f;                 // DAT_001953c0 low (carried in uVar16) = 0x00000000
    asFloat(this->sets[42].uvV0) = 0.0f;                 // DAT_001953c0 high = 0x00000000
    asFloat(this->sets[42].uvU1) = 0.25f;                // DAT_001953c8 low  = 0x3e800000
    asFloat(this->sets[42].uvV1) = 0.25f;                // DAT_001953c8 high = 0x3e800000

    // --- sets[19]: ember copy -------------------------------------------------------
    ParticleSet_assign(&this->sets[19], (void *)ember);
    buf.ctor_char(ParticleSettings_str + 398596, false);  // DAT_00194d00
    this->sets[19].posSpread = 0;
    this->sets[19].ySpread = 0;
    this->sets[19].velSpread = 0;
    this->sets[19].field_0x54 = 0;
    this->sets[19].lifetime = 0x5dc;
    this->sets[19].flags = (uint32_t)streakFlags;
    this->sets[19].count = 10;
    asFloat(this->sets[19].lifeBase) = 5000.0f;          // DAT_00194d88
    this->sets[19].lifeRandom = 1000;
    this->sets[19].color0 = 0xffffffff;
    this->sets[19].color1 = 0xffffffff;
    this->sets[19].fadeFrames = 0;
    this->sets[19].posBase = 500;
    this->sets[19].velDir = 0;
    this->sets[19].posRight = 0;
    this->sets[19].posUp = 0;
    this->sets[19].posDir = 0;
    this->sets[19].drag = 0;

    // --- sets[14]: ember copy -------------------------------------------------------
    ParticleSet_assign(&this->sets[14], (void *)ember);
    buf.ctor_char(ParticleSettings_str + 398477, false);  // DAT_00194d8c
    asFloat(this->sets[14].posDir) = 100.0f;             // DAT_00194dc4

    // --- sets[40]: ember copy -------------------------------------------------------
    ParticleSet_assign(&this->sets[40], (void *)ember);
    buf.ctor_char(ParticleSettings_str + 398438, false);  // DAT_00194dc8
    this->sets[40].field_0x54 = 5000;
    ParticleSet *streak2 = &this->sets[20];
    asFloat(this->sets[40].posDir) = 500.0f;             // DAT_00194e24
    asFloat(this->sets[40].lifeBase) = 1800.0f;          // DAT_00194e28
    this->sets[40].lifeRandom = 300;

    // --- sets[20]: streak copy (from sets[11]) --------------------------------------
    ParticleSet_assign((void *)streak2, &this->sets[11]);
    buf.ctor_char(ParticleSettings_str + 398366, false);  // DAT_00194e2c
    this->sets[20].lifetime = 1000;
    asFloat(this->sets[20].lifeBase) = 10000.0f;         // DAT_00194e6c
    this->sets[20].lifeRandom = 1000;

    // --- sets[21]: streak copy ------------------------------------------------------
    ParticleSet_assign(&this->sets[21], (void *)streak2);
    buf.ctor_char(ParticleSettings_str + 398324, false);  // DAT_00194e70
    ParticleSet *jet = &this->sets[22];
    this->sets[21].lifetime = 1000;
    asFloat(this->sets[21].lifeBase) = 1600.0f;          // DAT_00194eb0
    this->sets[21].lifeRandom = 200;

    // --- sets[22]: jet stream (copy of sets[9]) -------------------------------------
    ParticleSet_assign((void *)jet, (void *)spark);
    buf.ctor_char(ParticleSettings_str + 398286, false);  // DAT_00194eb4
    this->sets[22].lifetime = 1000;
    asFloat(this->sets[22].flLifetime) = 9.0f;           // DAT_001952c4
    asFloat(this->sets[22].posDirRandom) = 500.0f;       // DAT_001952cc
    this->sets[22].posSpread = 3000;
    this->sets[22].ySpread = 800;
    this->sets[22].count = 1000;
    asFloat(this->sets[22].lifeBase) = 4000.0f;          // DAT_001952c8
    this->sets[22].lifeRandom = 2000;

    // --- sets[41]: jet copy ---------------------------------------------------------
    ParticleSet_assign(&this->sets[41], (void *)jet);
    buf.ctor_char(ParticleSettings_str + 398148, false);  // DAT_001952d0
    asFloat(this->sets[41].posDir) = -4000.0f;           // DAT_001952d4
    this->sets[41].posSpread = 1000;
    asFloat(this->sets[41].posDirRandom) = 8000.0f;      // DAT_0019530c

    // --- sets[23]: jet copy ---------------------------------------------------------
    ParticleSet_assign(&this->sets[23], (void *)spark);
    buf.ctor_char(ParticleSettings_str + 398106, false);  // DAT_00195310
    this->sets[23].lifetime = 1000;
    asFloat(this->sets[23].flLifetime) = 9.0f;           // DAT_001952c4 (carried in uVar15)
    asFloat(this->sets[23].lifeBase) = 4000.0f;          // DAT_001952c8 (carried in uVar17)
    this->sets[23].lifeRandom = 1000;
    asFloat(this->sets[23].posDirRandom) = 500.0f;       // DAT_001952cc
    this->sets[23].posSpread = 5000;
    this->sets[23].ySpread = 0x5dc;

    // --- sets[24]: jet copy ---------------------------------------------------------
    ParticleSet_assign(&this->sets[24], (void *)spark);
    buf.ctor_char(ParticleSettings_str + 398045, false);  // DAT_00195314
    ParticleSet *ringEmitter = &this->sets[25];
    this->sets[24].lifetime = 1000;
    asFloat(this->sets[24].flLifetime) = 5.0f;           // DAT_00195318
    asFloat(this->sets[24].lifeBase) = 2000.0f;          // DAT_0019531c
    this->sets[24].lifeRandom = 5000;
    this->sets[24].posDirRandom = 0;
    this->sets[24].posSpread = 1000;
    this->sets[24].ySpread = 3000;

    // --- sets[25]: ring emitter (sub of sets[2]) ------------------------------------
    ParticleSettings_initSub((void *)ringEmitter, (void *)muzzleFlash);
    this->sets[25].posRight = 0;
    this->sets[25].posUp = 0;
    this->sets[25].posDir = 0;
    asFloat(this->sets[25].uvU0) = 0.001953125f;         // DAT_001953d0 low  = 0x3b000000
    asFloat(this->sets[25].uvV0) = 0.875f;               // DAT_001953d0 high = 0x3f600000
    asFloat(this->sets[25].uvU1) = 0.125f;               // DAT_001953d8 low  = 0x3e000000
    asFloat(this->sets[25].uvV1) = 0.625f;               // DAT_001953d8 high = 0x3f200000
    this->sets[25].lifetime = 1000;
    asFloat(this->sets[25].flLifetime) = 50.0f;          // DAT_00195320
    this->sets[25].count = 0x19;
    asFloat(this->sets[25].lifeBase) = 50.0f;            // DAT_00195320 (carried in uVar15)
    this->sets[25].color0 = 0xffffffff;
    this->sets[25].color1 = 0;
    this->sets[25].fadeFrames = 0;

    // --- sets[26]: ring sub ---------------------------------------------------------
    ParticleSettings_initSub(&this->sets[26], (void *)ringEmitter);
    asFloat(this->sets[26].lifeBase) = 100.0f;           // DAT_00195324
    asFloat(this->sets[26].uvU0) = 0.125f;               // DAT_001953e0 low  = 0x3e000000
    asFloat(this->sets[26].uvV0) = 0.875f;               // DAT_001953e0 high = 0x3f600000
    asFloat(this->sets[26].uvU1) = 0.25f;                // DAT_001953e8 low  = 0x3e800000
    asFloat(this->sets[26].uvV1) = 0.625f;               // DAT_001953e8 high = 0x3f200000

    // --- sets[27]: ring sub ---------------------------------------------------------
    ParticleSettings_initSub(&this->sets[27], (void *)ringEmitter);
    asFloat(this->sets[27].lifeBase) = 150.0f;           // DAT_00195328
    asFloat(this->sets[27].uvU0) = 0.25f;                // DAT_001953f0 low  = 0x3e800000
    asFloat(this->sets[27].uvV0) = 0.875f;               // DAT_001953f0 high = 0x3f600000
    asFloat(this->sets[27].uvU1) = 0.375f;               // DAT_001953f8 low  = 0x3ec00000
    asFloat(this->sets[27].uvV1) = 0.625f;               // DAT_001953f8 high = 0x3f200000

    // --- sets[28]: ring sub ---------------------------------------------------------
    ParticleSettings_initSub(&this->sets[28], (void *)ringEmitter);
    asFloat(this->sets[28].lifeBase) = 70.0f;            // DAT_0019532c
    asFloat(this->sets[28].uvU0) = 0.75f;                // DAT_00195400 low  = 0x3f400000
    asFloat(this->sets[28].uvV0) = 0.5f;                 // DAT_00195400 high = 0x3f000000
    asFloat(this->sets[28].uvU1) = 0.87109375f;          // DAT_00195408 low  = 0x3f5f0000
    asFloat(this->sets[28].uvV1) = 0.0f;                 // DAT_00195408 high = 0x00000000

    // --- sets[39]: ring sub (special flags) -----------------------------------------
    ParticleSettings_initSub(&this->sets[39], (void *)ringEmitter);
    ParticleSet *plume = &this->sets[43];
    this->sets[39].flags = (uint32_t)(176146 + 0x100000);  // DAT_00195450 = 0x0002b012
    asFloat(this->sets[39].lifeBase) = 100.0f;           // DAT_00195324 = 0x42c80000 (carried in uVar15)
    this->sets[39].posDir = 0;
    this->sets[39].posUp = 0;
    this->sets[39].posRight = 0;
    this->sets[39].lifetime = 3000;
    asFloat(this->sets[39].flLifetime) = 125.0f;         // DAT_00195454
    this->sets[39].count = 0x1d;
    this->sets[39].color0 = 0xffffffff;
    this->sets[39].color1 = 0;
    asFloat(this->sets[39].uvU0) = 0.751953125f;         // DAT_00195464
    asFloat(this->sets[39].uvV0) = 0.498046875f;         // DAT_00195460
    asFloat(this->sets[39].uvU1) = 0.998046875f;         // DAT_00195458
    this->sets[39].uvV1 = 0x3b000000;          // 0.0019531f

    // --- sets[43]: plume (sub of sets[16]) ------------------------------------------
    ParticleSettings_initSub((void *)plume, (void *)dustCloud);
    this->sets[43].posDir = 0;
    this->sets[43].posUp = 0;
    this->sets[43].posRight = 0;
    this->sets[43].flLifetime = 0x3f000000;    // 0.5f
    this->sets[43].count = 10;
    asFloat(this->sets[43].lifeBase) = 8000.0f;          // DAT_0019545c
    this->sets[43].posBase = 1000;
    this->sets[43].lifeRandom = 6000;
    this->sets[43].lifetime = 5000;
    this->sets[43].posSpread = 0x5dc;
    this->sets[43].ySpread = 0x5dc;
    this->sets[43].oneShot = 1;                // DAT_00195410 low  = 0x00000001
    this->sets[43].color0 = 0xffffffff;        // DAT_00195410 high = 0xffffffff
    this->sets[43].color1 = 0;                 // DAT_00195418 low  = 0x00000000
    this->sets[43].fadeFrames = 2500;          // DAT_00195418 high = 0x000009c4
    asFloat(this->sets[43].uvU0) = 0.001f;               // DAT_00195420 low  = 0x3a83126f
    asFloat(this->sets[43].uvV0) = 0.001f;               // DAT_00195420 high = 0x3a83126f
    asFloat(this->sets[43].uvU1) = 0.499f;               // DAT_00195428 low  = 0x3eff7cee
    asFloat(this->sets[43].uvV1) = 0.499f;               // DAT_00195428 high = 0x3eff7cee

    // --- sets[44]: plume sub --------------------------------------------------------
    ParticleSettings_initSub(&this->sets[44], (void *)plume);
    asFloat(this->sets[44].uvU0) = 0.501f;               // DAT_00195430 low  = 0x3f004189
    asFloat(this->sets[44].uvV0) = 0.001f;               // DAT_00195430 high = 0x3a83126f
    asFloat(this->sets[44].uvU1) = 0.999f;               // DAT_00195438 low  = 0x3f7fbe77
    asFloat(this->sets[44].uvV1) = 0.499f;               // DAT_00195438 high = 0x3eff7cee

    // --- sets[45]: plume sub --------------------------------------------------------
    ParticleSettings_initSub(&this->sets[45], (void *)plume);
    asFloat(this->sets[45].uvU0) = 0.001f;               // DAT_00195440 low  = 0x3a83126f
    asFloat(this->sets[45].uvV0) = 0.501f;               // DAT_00195440 high = 0x3f004189
    asFloat(this->sets[45].uvU1) = 0.499f;               // DAT_00195448 low  = 0x3eff7cee
    asFloat(this->sets[45].uvV1) = 0.999f;               // DAT_00195448 high = 0x3f7fbe77

    // --- sets[46]: plume sub --------------------------------------------------------
    ParticleSettings_initSub(&this->sets[46], (void *)plume);
    asFloat(this->sets[46].uvU0) = 0.501f;               // DAT_0019546c = 0x3f004189
    asFloat(this->sets[46].uvV0) = 0.501f;
    asFloat(this->sets[46].uvU1) = 0.999f;               // DAT_00195468 = 0x3f7fbe77
    asFloat(this->sets[46].uvV1) = 0.999f;

    // --- sets[47]: fire trail sub (of sets[12]) -------------------------------------
    ParticleSettings_initSub(&this->sets[47], &this->sets[12]);
    buf.ctor_char(ParticleSettings_str + 397416, false);  // DAT_00195470
    this->sets[47].frames = 0x10;
    asFloat(this->sets[47].uvU0) = 0.0f;                 // DAT_001953c0 low (carried in uVar16) = 0x00000000
    asFloat(this->sets[47].uvV0) = 0.0f;                 // DAT_001953c0 high = 0x00000000
    asFloat(this->sets[47].uvU1) = 0.25f;                // DAT_001953c8 low  = 0x3e800000
    asFloat(this->sets[47].uvV1) = 0.25f;                // DAT_001953c8 high = 0x3e800000
    return 0;
}

// Scale the lifetime and colour components of every ParticleSet by `scale` (or its
// reciprocal, depending on per-set flag bits).
void ParticleSettings::multiplyAll(float scale) {
    float recip = 1.0f / ((scale + 1.0f) * 0.5f);
    for (int i = 0; i < 48; ++i) {
        ParticleSet &p = this->sets[i];
        float lifeScale, lifeBase;
        if (p.flags & 0x20) {
            lifeBase = asFloat(p.flLifetime);
            lifeScale = scale;
        } else if ((int)(p.flags << 0x1b) < 0) {
            lifeBase = asFloat(p.flLifetime);
            lifeScale = 1.0f / scale;
        } else {
            continue;
        }
        unsigned c34 = p.color0;
        unsigned c38 = p.color1;
        float comp10 = (float)p.count;
        float r34 = recip * (float)(int)(c34 & 0xff);
        float r38 = recip * (float)(int)(c38 & 0xff);
        asFloat(p.flLifetime) = lifeBase * lifeScale;
        unsigned u34 = (0.0f < r34) ? (unsigned)r34 : 0;
        unsigned u38 = (0.0f < r38) ? (unsigned)r38 : 0;
        if (u34 > 0xfe) u34 = 0xff;
        if (u38 > 0xfe) u38 = 0xff;
        p.color0 = u34 | (c34 & 0xffffff00);
        p.color1 = u38 | (c38 & 0xffffff00);
        p.count = (int)(comp10 * scale);
    }
}

// out = b*t + (1-t)*a over the lifetime/colour/scale slots. `count` (+0x28) is stored as
// an int but blended in float space; lifeBase/endSize/velDir are plain floats.
void ParticleSettings::Interpolate(int a, int b, float t, int out) {
    ParticleSet &pa = this->sets[a];
    ParticleSet &pb = this->sets[b];
    ParticleSet &po = this->sets[out];
    float omt = 1.0f - t;
    po.count = (int)(omt * (float)pa.count + (float)pb.count * t);
    asFloat(po.lifeBase) = omt * asFloat(pa.lifeBase) + asFloat(pb.lifeBase) * t;
    asFloat(po.endSize) = omt * asFloat(pa.endSize) + asFloat(pb.endSize) * t;
    asFloat(po.velDir) = omt * asFloat(pa.velDir) + asFloat(pb.velDir) * t;
}
