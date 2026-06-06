struct PaintCanvas {
    char pad[0x158];
    unsigned int transformCount;
    void **transforms;
};

extern "C" void *TransformGetTransform(PaintCanvas *self, unsigned int index)
{
    if (self->transformCount <= index) {
        return 0;
    }
    return self->transforms[index];
}
