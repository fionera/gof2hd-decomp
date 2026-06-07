# ApplicationManager::SoundMusicEnable
elf 0x8226a body 6
Sig: undefined __thiscall SoundMusicEnable(ApplicationManager * this, bool param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundMusicEnable(bool) */

void __thiscall
AbyssEngine::ApplicationManager::SoundMusicEnable(ApplicationManager *this,bool param_1)

{
  this[0xb1] = (ApplicationManager)param_1;
  return;
}

```

## target disasm
```
  0009226a: strb.w r1,[r0,#0xb1]
  0009226e: bx lr
```
