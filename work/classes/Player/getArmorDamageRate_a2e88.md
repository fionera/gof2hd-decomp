# Player::getArmorDamageRate
elf 0xa2e88 body 6
Sig: undefined __thiscall getArmorDamageRate(Player * this)

## decompile
```c

/* Player::getArmorDamageRate() */

undefined4 __thiscall Player::getArmorDamageRate(Player *this)

{
  return *(undefined4 *)(this + 0xac);
}

```
## target disasm
```
  000b2e88: ldr.w r0,[r0,#0xac]
  000b2e8c: bx lr
```
