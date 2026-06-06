struct PlayerEgo {
    void *m_pPlayer;
    char pad[0x108];
    int currentSecondaryWeaponIndex;
};

extern "C" void getShieldDamageRate_tail(void *);

extern "C" void getShieldDamageRate(PlayerEgo *self)
{
    return getShieldDamageRate_tail(self->m_pPlayer);
}

extern "C" int getShieldDamageRate_getCurrentSecondaryWeaponIndex(PlayerEgo *self)
{
    return self->currentSecondaryWeaponIndex;
}
