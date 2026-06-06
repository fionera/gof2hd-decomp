struct CutScene {
    void *first;
};

extern "C" void render2D_tail(void *);

extern "C" void render2D(CutScene *self)
{
    return render2D_tail(self->first);
}
