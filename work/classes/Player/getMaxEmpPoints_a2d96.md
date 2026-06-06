# Player::getMaxEmpPoints
elf 0xa2d96 body 6
Sig: undefined __thiscall getMaxEmpPoints(Player * this)

## decompile
```c

/* Player::getMaxEmpPoints() */

undefined4 __thiscall Player::getMaxEmpPoints(Player *this)

{
  return *(undefined4 *)(this + 0x80);
}

```
## target disasm
```
  000b2d96: ldr.w r0,[r0,#0x80]
  000b2d9a: bx lr
```
