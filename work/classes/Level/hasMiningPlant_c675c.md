# Level::hasMiningPlant
elf 0xc675c body 14
Sig: undefined __thiscall hasMiningPlant(Level * this)

## decompile
```c

/* Level::hasMiningPlant() */

bool __thiscall Level::hasMiningPlant(Level *this)

{
  return 0 < *(int *)(this + 0x28c);
}

```
## target disasm
```
  000d675c: ldr.w r1,[r0,#0x28c]
  000d6760: movs r0,#0x0
  000d6762: cmp r1,#0x0
  000d6764: it gt
  000d6766: mov.gt r0,#0x1
  000d6768: bx lr
```
