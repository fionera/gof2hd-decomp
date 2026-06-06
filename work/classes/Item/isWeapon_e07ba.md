# Item::isWeapon
elf 0xe07ba body 12
Sig: undefined __thiscall isWeapon(Item * this)

## decompile
```c

/* Item::isWeapon() */

bool __thiscall Item::isWeapon(Item *this)

{
  return *(uint *)(this + 4) < 3;
}

```
## target disasm
```
  000f07ba: ldr r1,[r0,#0x4]
  000f07bc: movs r0,#0x0
  000f07be: cmp r1,#0x3
  000f07c0: it cc
  000f07c2: mov.cc r0,#0x1
  000f07c4: bx lr
```
