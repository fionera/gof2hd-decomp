# Player::getMaxArmorHP
elf 0xa2ddc body 6
Sig: undefined __thiscall getMaxArmorHP(Player * this)

## decompile
```c

/* Player::getMaxArmorHP() */

undefined4 __thiscall Player::getMaxArmorHP(Player *this)

{
  return *(undefined4 *)(this + 0x90);
}

```
## target disasm
```
  000b2ddc: ldr.w r0,[r0,#0x90]
  000b2de0: bx lr
```
