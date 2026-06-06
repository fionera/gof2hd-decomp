struct PlayerEgo {
    char pad[0x2f4];
    unsigned char alignToHorizon;
};

extern "C" void alignToHorizon(PlayerEgo *self)
{
    self->alignToHorizon = 1;
}
