struct PlayerEgo {
    void *hitpoints;
};

extern "C" void getHitpoints_tail(void *);

extern "C" void getHitpoints(PlayerEgo *self)
{
    return getHitpoints_tail(self->hitpoints);
}
