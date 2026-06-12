#ifndef GOF2_PLAYERTURRET_H
#define GOF2_PLAYERTURRET_H
#include "gof2/common.h"
// Galaxy on Fire 2 - PlayerTurret (a KIPlayer subclass: sentry guns / turrets).
// One unified layout. Pointer-typed slots are named f_<off>; scalar fields that the
// code reads through the offset helpers below keep the deterministic field_0xNN scheme.
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

struct IntArray {
    uint32_t length;
    int *data;
    uint32_t capacity;
};

struct PlayerArray {
    uint32_t length;
    Player **data;
    uint32_t capacity;
};

// Byte-offset accessors used for fields whose individual names were not recovered and
// for reads/writes into cross-class objects (Player/AEGeometry/Level/...).
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
    // Named pointer slots (offset encoded in the name). Padding keeps every named slot
    // at its real byte offset so the I()/P()/UC()/TP() helpers and the named members
    // address the same storage.
    void *f_0;                          // 0x0  vtable
    unsigned char _pad_4[4];
    void *f_8;                          // 0x8  main geometry
    void *f_c;                          // 0xc
    unsigned char _pad_10[0x50 - 0x10];
    void *f_50;                         // 0x50
    void *f_54;                         // 0x54 level / particle-system manager
    unsigned char _pad_58[0x78 - 0x58];
    void *f_78;                         // 0x78 visible geometry
    unsigned char _pad_7c[0x13c - 0x7c];
    void *f_13c;                        // 0x13c explosion
    void *f_140;                        // 0x140 base geometry
    void *f_144;                        // 0x144 turret geometry
    void *f_148;                        // 0x148 helper geometry
    void *f_14c;                        // 0x14c current enemy
    void *f_150;                        // 0x150 previous enemy
    void *f_154;                        // 0x154 host KIPlayer
    void *f_158;                        // 0x158 host offset Vector
    void *f_15c;                        // 0x15c
    unsigned char _pad_160[0x168 - 0x160];

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
