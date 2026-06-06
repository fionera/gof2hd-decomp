struct PlayerEgo {
    void *m_pPlayer;
};

extern "C" void getHullDamageRate_tail(void *);

extern "C" void getHullDamageRate(PlayerEgo *self)
{
    return getHullDamageRate_tail(self->m_pPlayer);
}
