struct PlayerEgo {
    char pad[0x204];
    unsigned char chargingDrive;
};

extern "C" void resetChargingDrive(PlayerEgo *self)
{
    self->chargingDrive = 0;
}
