# Level::stealFriendCargo
elf 0xc625c body 8
Sig: undefined __thiscall stealFriendCargo(Level * this)

## decompile
```c

/* Level::stealFriendCargo() */

void __thiscall Level::stealFriendCargo(Level *this)

{
  this[0x13c] = (Level)0x1;
  return;
}

```
## target disasm
```
  000d625c: movs r1,#0x1
  000d625e: strb.w r1,[r0,#0x13c]
  000d6262: bx lr
```
