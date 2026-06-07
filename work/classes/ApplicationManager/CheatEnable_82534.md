# ApplicationManager::CheatEnable
elf 0x82534 body 6
Sig: undefined __thiscall CheatEnable(ApplicationManager * this, bool param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::CheatEnable(bool) */

void __thiscall AbyssEngine::ApplicationManager::CheatEnable(ApplicationManager *this,bool param_1)

{
  this[0x34] = (ApplicationManager)param_1;
  return;
}

```

## target disasm
```
  00092534: strb.w r1,[r0,#0x34]
  00092538: bx lr
```
