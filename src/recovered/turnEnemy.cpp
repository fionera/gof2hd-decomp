struct Player {
    unsigned char field_00[0xe0];
    unsigned char enemy;
};

extern "C" void turnEnemy(Player *self)
{
    self->enemy = 1;
}
