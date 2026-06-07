# ApplicationManager::GetActionState
elf 0x83e8a body 6
Sig: undefined __thiscall GetActionState(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetActionState() */

undefined8 __thiscall AbyssEngine::ApplicationManager::GetActionState(ApplicationManager *this)

{
  return *(undefined8 *)(this + 0xa0);
}

```

## target disasm
```
  00093e8a: ldrd r0,r1,[r0,#0xa0]
  00093e8e: bx lr
```
