#include "class.h"

// FModSound::~FModSound() -> release(this); (returns this)
struct FModSound {
    void release();
    ~FModSound();
};
FModSound::~FModSound()
{
    release();
}
