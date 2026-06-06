struct PaintCanvas {
    char pad[0x34];
    void *height;
};

extern "C" void GetHeight_tail(void *);

extern "C" void GetHeight(PaintCanvas *self)
{
    return GetHeight_tail(self->height);
}
