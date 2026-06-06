struct PaintCanvas {
    char pad[0x34];
    void *texture;
};

extern "C" void SetTexture_tail(void *);

extern "C" void SetTexture(PaintCanvas *self, unsigned int)
{
    return SetTexture_tail(self->texture);
}
