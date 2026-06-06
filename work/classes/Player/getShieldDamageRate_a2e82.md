# Player::getShieldDamageRate
elf 0xa2e82 body 6
Sig: undefined __thiscall getShieldDamageRate(Player * this)

## decompile
```c

/* Player::getShieldDamageRate() */

undefined4 __thiscall Player::getShieldDamageRate(Player *this)

{
  return *(undefined4 *)(this + 0xa8);
}

```
## target disasm
```
  000b2e82: ldr.w r0,[r0,#0xa8]
  000b2e86: bx lr
```
