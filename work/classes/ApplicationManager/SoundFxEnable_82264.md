# ApplicationManager::SoundFxEnable
elf 0x82264 body 6
Sig: undefined __thiscall SoundFxEnable(ApplicationManager * this, bool param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundFxEnable(bool) */

void __thiscall
AbyssEngine::ApplicationManager::SoundFxEnable(ApplicationManager *this,bool param_1)

{
  this[0xb0] = (ApplicationManager)param_1;
  return;
}

```

## target disasm
```
  00092264: strb.w r1,[r0,#0xb0]
  00092268: bx lr
```
