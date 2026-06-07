# ApplicationManager::SoundEnable
elf 0x8225a body 10
Sig: undefined __thiscall SoundEnable(ApplicationManager * this, bool param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundEnable(bool) */

void __thiscall AbyssEngine::ApplicationManager::SoundEnable(ApplicationManager *this,bool param_1)

{
  this[0xb0] = (ApplicationManager)param_1;
  this[0xb1] = (ApplicationManager)param_1;
  return;
}

```

## target disasm
```
  0009225a: strb.w r1,[r0,#0xb0]
  0009225e: strb.w r1,[r0,#0xb1]
  00092262: bx lr
```
