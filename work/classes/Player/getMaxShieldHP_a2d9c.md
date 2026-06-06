# Player::getMaxShieldHP
elf 0xa2d9c body 6
Sig: undefined __thiscall getMaxShieldHP(Player * this)

## decompile
```c

/* Player::getMaxShieldHP() */

undefined4 __thiscall Player::getMaxShieldHP(Player *this)

{
  return *(undefined4 *)(this + 0x94);
}

```
## target disasm
```
  000b2d9c: ldr.w r0,[r0,#0x94]
  000b2da0: bx lr
```
