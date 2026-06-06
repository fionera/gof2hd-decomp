# Level::getEnemyGuns
elf 0xc6278 body 6
Sig: undefined __thiscall getEnemyGuns(Level * this)

## decompile
```c

/* Level::getEnemyGuns() */

undefined4 __thiscall Level::getEnemyGuns(Level *this)

{
  return *(undefined4 *)(this + 0xe8);
}

```
## target disasm
```
  000d6278: ldr.w r0,[r0,#0xe8]
  000d627c: bx lr
```
