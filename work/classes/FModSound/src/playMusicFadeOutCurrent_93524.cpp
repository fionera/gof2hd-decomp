#include "class.h"

// FModSound::playMusicFadeOutCurrent(int p1)
//   if (this[0] != p1) { if (this[0]==-1) {this[0]=p1; s=p1;} setParamValue(0,s,0.1f); this[4]=p1; }
static const float kFade = 0.1f;

struct FModSound {
    void setParamValue(int a, int b, float v);
    void playMusicFadeOutCurrent(int p1);
};
void FModSound::playMusicFadeOutCurrent(int p1)
{
    int s = i32(this, 0);
    if (s == p1)
        return;
    if (s == -1) {
        i32(this, 0) = p1;
        s = p1;
    }
    setParamValue(0, s, kFade);
    i32(this, 4) = p1;
}
