# ApplicationManager::GetKeyState
elf 0x83e84 body 6
Sig: undefined __thiscall GetKeyState(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetKeyState() */

undefined8 __thiscall AbyssEngine::ApplicationManager::GetKeyState(ApplicationManager *this)

{
  return *(undefined8 *)(this + 0x80);
}

```

## target disasm
```
  00093e84: ldrd r0,r1,[r0,#0x80]
  00093e88: bx lr
```
