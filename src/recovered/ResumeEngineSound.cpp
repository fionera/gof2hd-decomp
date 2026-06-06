struct PlayerEgo {
    void *engineSound;
};

extern "C" void ResumeEngineSound_tail(void *, int);

extern "C" void ResumeEngineSound(PlayerEgo *self)
{
    return ResumeEngineSound_tail(self->engineSound, 0);
}
