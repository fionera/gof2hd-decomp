#include "class.h"

using AbyssEngine::AEMath::Vector;

extern "C" PlayerArray *Player_getEnemies(Player *self);
extern "C" int Player_isDead(Player *self);
extern "C" int Player_isActive(Player *self);
extern "C" void *Player_getKIPlayer(Player *self);
extern "C" void Player_getPosition(Vector *out, Player *self);
extern "C" void Vector_sub(Vector *out, const Vector *a, const Vector *b);
extern "C" float VectorLength(const Vector *v);

struct PlayerTurret {
    // @portable-fields
    void* f_0; // 0x0
    unsigned char _pad_4[4];
    void* f_8; // 0x8
    void* f_c; // 0xc
    unsigned char _pad_10[64];
    void* f_50; // 0x50
    void* f_54; // 0x54
    unsigned char _pad_58[32];
    void* f_78; // 0x78
    unsigned char _pad_7c[192];
    void* f_13c; // 0x13c
    void* f_140; // 0x140
    void* f_144; // 0x144
    void* f_148; // 0x148
    void* f_14c; // 0x14c
    void* f_150; // 0x150
    void* f_154; // 0x154
    void* f_158; // 0x158
    void* f_15c; // 0x15c

    void pickEnemy();
};

void PlayerTurret::pickEnemy()
{
    char vectorFrame[24];

    if (I(this, 0x130) > 3000) {
        int bestRange = I(this, 0x164);
        I(this, 0x130) = 0;
        this->f_14c = 0;

        PlayerArray *enemies = Player_getEnemies(TP<Player>(this, 0x4));
        if (enemies != 0) {
            Vector *position = (Vector *)B(this, 0x2c);
            uint32_t i = 0;
            goto check;
        next:
            {
                Player *enemy = enemies->data[i];

                if (Player_isDead(enemy) == 0 && Player_isActive(enemy) != 0) {
                    bool accepted = false;
                    if (UC(enemy, 0x69) != 0 && UC(TP<Player>(this, 0x4), 0x5c) != 0) {
                        accepted = true;
                    } else if (UC(this, 0x3f) == 0) {
                        if (Player_getKIPlayer(enemy) != 0) {
                            accepted = true;
                        }
                    } else if (UC(enemy, 0x5c) != 0) {
                        accepted = true;
                    }

                    if (accepted) {
                        Player_getPosition((Vector *)vectorFrame, enemy);
                        Vector_sub((Vector *)(vectorFrame + 12), position, (Vector *)vectorFrame);
                        int distance = (int)VectorLength((Vector *)(vectorFrame + 12));
                        if (distance < bestRange) {
                            void *current = this->f_14c;
                            if (current == 0 || current != this->f_150) {
                                bestRange = distance;
                                this->f_14c = enemy;
                            }
                        }
                    }
                }
            }
            i = i + 1;
        check:
            if (i < enemies->length) {
                goto next;
            }
        }
    }
}
