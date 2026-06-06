struct PaintCanvas {
    char pad[0x34];
    void *worldViewMatrix;
};

extern "C" void SetWorldViewMatrix_tail(void *);

extern "C" void SetWorldViewMatrix(PaintCanvas *self)
{
    return SetWorldViewMatrix_tail(self->worldViewMatrix);
}
