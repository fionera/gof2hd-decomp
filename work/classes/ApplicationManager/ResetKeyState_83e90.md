# ApplicationManager::ResetKeyState
elf 0x83e90 body 8
Sig: undefined __thiscall ResetKeyState(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::ResetKeyState() */

void __thiscall AbyssEngine::ApplicationManager::ResetKeyState(ApplicationManager *this)

{
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  return;
}

```

## target disasm
```
  00093e90: movs r1,#0x0
  00093e92: strd r1,r1,[r0,#0x80]
  00093e96: bx lr
```
