# Level::removeObjectives
elf 0xc626a body 8
Sig: undefined __thiscall removeObjectives(Level * this)

## decompile
```c

/* Level::removeObjectives() */

void __thiscall Level::removeObjectives(Level *this)

{
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  return;
}

```
## target disasm
```
  000d626a: movs r1,#0x0
  000d626c: strd r1,r1,[r0,#0x28]
  000d6270: bx lr
```
