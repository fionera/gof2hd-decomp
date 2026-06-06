struct AEGeometry;

extern "C" void setScaling_tail(AEGeometry *, float, float, float);

extern "C" void setScaling(AEGeometry *self, float scale)
{
    return setScaling_tail(self, scale, scale, scale);
}
