#ifndef GOF2_PLAYERCREATURE_H
#define GOF2_PLAYERCREATURE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct PlayerCreature;
struct KIPlayer;
struct Player;
struct AEGeometry;
struct FModSound;

namespace AbyssEngine {
namespace AEMath {

void MatrixMultiply(Matrix &lhs, const Matrix &rhs);
Matrix MatrixSetRotation(Matrix &matrix, float x, float y, float z);

}
}

class PlayerCreature {
public:
    char* vtable;                    // +0x0
    Player* player;                  // +0x4
    AEGeometry* geometry;              // +0x8
    char  _pad_0xc[0x54 - 0xc];
    void* level;                   // +0x54
    float posX;                   // +0x58
    float posY;                   // +0x5c
    float posZ;                   // +0x60
    char  _pad_0x64[0x78 - 0x64];
    AEGeometry* renderGeometry;             // +0x78
    char  _pad_0x7c[0x88 - 0x7c];
    int state;                     // +0x88
    char  _pad_0x8c[0xac - 0x8c];
    int creatureType;                     // +0xac
    char  _pad_0xb0[0x124 - 0xb0];
    int lastElapsed;                    // +0x124
    uint16_t raging;               // +0x128
    uint8_t hooked;                // +0x129
    uint8_t caught;                // +0x12a
    uint8_t _pad_0x12b;
    float rageScale;                  // +0x12c
    int rageTimer;                    // +0x130
    int maxEndurance;                    // +0x134
    int endurance;                    // +0x138
    int lastHitpoints;                    // +0x13c
    int itemIndex;                    // +0x140
    Matrix rageMatrix;                 // +0x144 rage-shake rotation matrix (0x3c bytes)

    PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                   float x, float y, float z);
    uint8_t isHooked();
    void calmDown();
    void unhook();
    void render();
    int getEndurance();
    int getWeight();
    uint8_t isCaught();
    void rage(int amount);
    int getMaxEndurance();
    int getItemIndex();
    void reset();
    void hook(int value);
    void update(int elapsed);

    // Tail fragments split out of render()/hook()/the deleting destructor: each one
    // completes its operation by handing control to the corresponding base behaviour.
    void render_tail();         // render()'s tail: render through the PlayerJunk base
    void hook_tail(int value);  // hook()'s tail: enrage the creature with the hook amount
    void dtor_tail();           // deleting destructor tail: free the object
};
#endif
