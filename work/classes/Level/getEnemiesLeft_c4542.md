# Level::getEnemiesLeft
elf 0xc4542 body 6
Sig: undefined __thiscall getEnemiesLeft(Level * this)

## decompile
```c

/* Level::getEnemiesLeft() */

undefined4 __thiscall Level::getEnemiesLeft(Level *this)

{
  return *(undefined4 *)(this + 0x118);
}

```
## target disasm
```
  000d4542: ldr.w r0,[r0,#0x118]
  000d4546: bx lr
```
