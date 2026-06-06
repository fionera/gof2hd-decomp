struct PaintCanvas {
    char pad[0x34];
    void *width;
};

extern "C" void GetWidth_tail(void *);

extern "C" void GetWidth(PaintCanvas *self)
{
    return GetWidth_tail(self->width);
}
