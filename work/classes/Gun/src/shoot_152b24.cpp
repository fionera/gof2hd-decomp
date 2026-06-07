#include "class.h"

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Matrix { float m[15]; };
} }
using AbyssEngine::AEMath::Matrix;

// Gun::shootAt(Matrix, int, Player*, bool)
extern "C" void Gun_shootAt(Gun *self, Matrix m, int n, Player *p, bool b);

// Gun::shoot(Matrix, int, bool) — forwards to shootAt with a null Player.
extern "C" void Gun_shoot(Gun *self, Matrix m, int n, bool b)
{
    Gun_shootAt(self, m, n, 0, b);
}
