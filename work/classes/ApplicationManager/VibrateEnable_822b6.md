# ApplicationManager::VibrateEnable
elf 0x822b6 body 6
Sig: undefined __thiscall VibrateEnable(ApplicationManager * this, bool param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::VibrateEnable(bool) */

void __thiscall
AbyssEngine::ApplicationManager::VibrateEnable(ApplicationManager *this,bool param_1)

{
  this[0xb2] = (ApplicationManager)param_1;
  return;
}

```

## target disasm
```
  000922b6: strb.w r1,[r0,#0xb2]
  000922ba: bx lr
```
