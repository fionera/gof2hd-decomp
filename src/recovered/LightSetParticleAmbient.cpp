extern "C" void LightSetParticleAmbient(void *self, unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int *out = (unsigned int *)((char *)self + 0x314);
    out[0] = a;
    out[1] = b;
    out[2] = c;
}
