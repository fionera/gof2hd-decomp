struct PaintCanvas {
    char pad[0x34];
    void *fbo;
};

extern "C" void StartDraw2FBO_tail(void *);

extern "C" void StartDraw2FBO(PaintCanvas *self)
{
    return StartDraw2FBO_tail(self->fbo);
}
