# Player::getRadius
elf 0xa2ce0 body 4
Sig: undefined __thiscall getRadius(Player * this)

## decompile
```c

/* Player::getRadius() */

undefined4 __thiscall Player::getRadius(Player *this)

{
  return *(undefined4 *)(this + 0x40);
}

```
## target disasm
```
  000b2ce0: ldr r0,[r0,#0x40]
  000b2ce2: bx lr
```
