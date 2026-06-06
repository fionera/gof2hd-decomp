struct Player {
    char pad[0x7c];
    int maxEmpPoints;
    int empPoints;
};

extern "C" void setMaxEmpPoints_tail();

extern "C" void setMaxEmpPoints(Player *self, int value)
{
    self->maxEmpPoints = value;
    self->empPoints = value;
    return setMaxEmpPoints_tail();
}
