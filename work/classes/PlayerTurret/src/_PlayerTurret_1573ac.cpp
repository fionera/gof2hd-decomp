#include "class.h"

extern "C" PlayerTurret *PlayerTurret_completeDtor(PlayerTurret *self);
extern "C" void PlayerTurret_operator_delete(void *ptr);

struct PlayerTurret {
    ~PlayerTurret() noexcept(false);
};

PlayerTurret::~PlayerTurret() noexcept(false)
{
    PlayerTurret_operator_delete(PlayerTurret_completeDtor(this));
}
