# Level::getAsteroidsLeft
elf 0xc453c body 6
Sig: undefined __thiscall getAsteroidsLeft(Level * this)

## decompile
```c

/* Level::getAsteroidsLeft() */

undefined4 __thiscall Level::getAsteroidsLeft(Level *this)

{
  return *(undefined4 *)(this + 0x128);
}

```
## target disasm
```
  000d453c: ldr.w r0,[r0,#0x128]
  000d4540: bx lr
```
