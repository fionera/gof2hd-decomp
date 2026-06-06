# Level::getPlayerGuns
elf 0xc6272 body 6
Sig: undefined __thiscall getPlayerGuns(Level * this)

## decompile
```c

/* Level::getPlayerGuns() */

undefined4 __thiscall Level::getPlayerGuns(Level *this)

{
  return *(undefined4 *)(this + 0xe4);
}

```
## target disasm
```
  000d6272: ldr.w r0,[r0,#0xe4]
  000d6276: bx lr
```
