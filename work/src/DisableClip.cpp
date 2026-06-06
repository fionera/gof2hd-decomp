extern "C" void DisableClip_tail(int);

extern "C" void DisableClip()
{
    return DisableClip_tail(0xc11);
}
