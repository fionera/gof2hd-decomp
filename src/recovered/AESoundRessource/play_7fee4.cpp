#include "class.h"

extern "C" void AESoundRessource_play_impl(AbyssEngine::AESoundRessource *self, int sound, int volume);

extern "C" void AESoundRessource_play_7fee4(AbyssEngine::AESoundRessource *self, int sound)
{
    return AESoundRessource_play_impl(self, sound, 0);
}
