# Player::getEmpPoints
elf 0xa2d92 body 4
Sig: undefined __thiscall getEmpPoints(Player * this)

## decompile
```c

/* Player::getEmpPoints() */

undefined4 __thiscall Player::getEmpPoints(Player *this)

{
  return *(undefined4 *)(this + 0x7c);
}

```
## target disasm
```
  000b2d92: ldr r0,[r0,#0x7c]
  000b2d94: bx lr
```
