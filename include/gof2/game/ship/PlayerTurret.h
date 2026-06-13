#ifndef GOF2_PLAYERTURRET_H
#define GOF2_PLAYERTURRET_H
#include "gof2/common.h"
// Galaxy on Fire 2 - PlayerTurret (a KIPlayer subclass: sentry guns / turrets).
// Full named layout recovered from per-method usage and the KIPlayer/Player base
// layouts. Trailing comments are the original 32-bit field offsets, kept for
// cross-reference; the live layout is natural 64-bit.
struct PlayerTurret;
struct Player;
struct KIPlayer;
struct Level;
struct AEGeometry;
struct Explosion;
struct Standing;
struct TargetFollowCamera;

namespace AbyssEngine {
namespace AEMath {
} // namespace AEMath
} // namespace AbyssEngine

// Byte-offset accessors still used for reads/writes into cross-class objects
// (Player/AEGeometry/Level/...), never for PlayerTurret's own members.
static inline char *B(void *self, uint32_t off) { return (char *)self + off; }
static inline const char *B(const void *self, uint32_t off) { return (const char *)self + off; }
static inline int32_t &I(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline void *&P(void *self, uint32_t off) { return *(void **)((char *)self + off); }

template <class T>
static inline T *TP(void *self, uint32_t off)
{
    return *(T **)((char *)self + off);
}

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

class PlayerTurret {
public:
    // ---- inherited KIPlayer/Player base sub-object region (named at the offsets
    // this class actually touches; padding keeps every member at its real byte offset) ----
    void *vtable;                       // +0x0   vtable
    unsigned char _pad_4[4];
    Player *player;                     // +0x4   underlying Player
    AEGeometry *geometry;               // +0x8   main geometry
    AEGeometry *parentGeometry;         // +0xc   parent/owner geometry
    unsigned char _pad_10[0x25 - 0x10];
    uint8_t turretEnabled;              // +0x25  turret logic active flag
    unsigned char _pad_26[0x28 - 0x26];
    uint32_t standing;                  // +0x28  faction/standing id (KIPlayer shipGroup)
    unsigned char _pad_2c[0x2c - 0x2c];
    AbyssEngine::AEMath::Vector position; // +0x2c  cached world position
    unsigned char _pad_38[0x3e - 0x38];
    uint8_t field_0x3e;                 // +0x3e  set in ctor
    uint8_t isSentryGun;                // +0x3f  sentry-gun vs turret discriminator
    unsigned char _pad_40[0x4c - 0x40];
    uint8_t lootSpawned;                // +0x4c  crate/loot spawned flag
    unsigned char _pad_4d[0x50 - 0x4d];
    Array<int> *loot;                   // +0x50  loot/crate id list (KIPlayer cargo)
    void *level;                        // +0x54  Level / particle-system manager
    float posX;                         // +0x58  KIPlayer cached position X
    float posY;                         // +0x5c  KIPlayer cached position Y
    float posZ;                         // +0x60  KIPlayer cached position Z
    unsigned char _pad_64[0x74 - 0x64];
    int field_0x74;                     // +0x74  KIPlayer flag (set on fighter-turret creation)
    AEGeometry *visibleGeometry;        // +0x78  visible geometry (KIPlayer crateGeometry)
    unsigned char _pad_7c[0x88 - 0x7c];
    int state;                          // +0x88  render/lifecycle state
    unsigned char _pad_8c[0x90 - 0x8c];
    AbyssEngine::AEMath::Vector hostWorldOffset; // +0x90  host offset rotated into world space
    unsigned char _pad_9c[0x9c - 0x9c];
    AbyssEngine::AEMath::Vector aimPoint; // +0x9c  current aim/target point
    unsigned char _pad_a8[0xd8 - 0xa8];
    int field_0xd8;                     // +0xd8  reset to 0 on revive
    unsigned char _pad_dc[0xf5 - 0xdc];
    uint8_t visibleFlag;                // +0xf5  KIPlayer visibility flag
    unsigned char _pad_f6[0x124 - 0xf6];

    // ---- PlayerTurret-specific members ----
    int frameDelta;                     // +0x124 last update delta (ms)
    int spawnInvulnTimer;               // +0x128 spawn invulnerability timer
    int explosionTimer;                 // +0x12c death/explosion timer
    int pickEnemyTimer;                 // +0x130 enemy re-acquisition / fire timer
    int rotationAccum;                  // +0x134 main-geometry rotation accumulator
    int particleSystemId;               // +0x138 engine/death particle system id
    Explosion *explosion;               // +0x13c death explosion
    AEGeometry *baseGeometry;           // +0x140 base geometry
    AEGeometry *turretGeometry;         // +0x144 turret (barrel) geometry
    AEGeometry *helperGeometry;         // +0x148 helper/pivot geometry
    Player *currentEnemy;               // +0x14c current target
    Player *previousEnemy;              // +0x150 previous target
    KIPlayer *host;                     // +0x154 host KIPlayer (mounted ship)
    AbyssEngine::AEMath::Vector hostOffset; // +0x158 mount offset relative to host
    int turretRange;                    // +0x164 max acquisition range

    PlayerTurret(int mesh, Player *player, AEGeometry *geometry, float x, float y, float z);
    ~PlayerTurret() noexcept(false);
    PlayerTurret *completeDtor();

    void setTurretRange(int range);
    void handleSentryGun(int delta);
    void setHost(KIPlayer *host, const Vector &offset);
    void render();
    void renderBase();                  // render() tail: inherited KIPlayer::render()
    void handleTurret(int delta);
    void revive();
    void setPosition(const Vector &position);
    void reset();
    void setLevel(Level *level);
    KIPlayer *getHost();
    void setScaling(float scale);
    void handleRotation(int delta, AEGeometry *mainGeometry, AEGeometry *turretGeometry);
    void update(int delta);
    void pickEnemy();
};

#endif
