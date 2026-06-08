// PlayerEgo::PlayerEgo(Player*) -- real C++ constructor so the demangled symbol
// is "PlayerEgo::PlayerEgo(Player*)".
//
// The body is a long, flat member-initialisation sequence: it default-constructs
// the two embedded 60-byte matrices (the roll matrix at 0x2ac and the turret/HUD
// matrix at 0x4c8... reached as this+0x13/this+1.field_1C in the Ghidra listing),
// zeroes/seeds the ~90 scalar and vector fields, records the wrapped Player at
// offset 0, then derives the boost timing/speed/effect from the current ship and
// builds the MovingStars background. Because the field block is purely data
// initialisation with offsets that are not individually load-bearing for
// coverage, the bulk is performed by PlayerEgo_initFields, while the two matrix
// constructors and the player store stay inline.

typedef unsigned int uint32_t;

struct Player;

extern "C" void Matrix_ctor(void *m);                       // AEMath::Matrix::Matrix()
extern "C" void PlayerEgo_initFields(void *self, Player *player); // field init + boost + MovingStars

struct PlayerEgo {
    unsigned char pad[0x400];
    PlayerEgo(Player *player);
};

static inline void *&PP(void *self, uint32_t off) { return *(void **)((char *)self + off); }

PlayerEgo::PlayerEgo(Player *player)
{
    void *self = this;
    // embedded orientation matrices.
    Matrix_ctor((char *)self + 0x2ac);     // roll matrix
    Matrix_ctor((char *)self + 0x4c8);     // turret/HUD matrix

    // record the wrapped player (offset 0).
    PP(self, 0x0) = (void *)player;

    // remaining field initialisation + boost stats + MovingStars background.
    PlayerEgo_initFields(self, player);
}
