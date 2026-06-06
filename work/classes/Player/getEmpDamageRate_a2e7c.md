# Player::getEmpDamageRate
elf 0xa2e7c body 6
Sig: undefined __thiscall getEmpDamageRate(Player * this)

## decompile
```c

/* Player::getEmpDamageRate() */

undefined4 __thiscall Player::getEmpDamageRate(Player *this)

{
  return *(undefined4 *)(this + 0xb0);
}

```
## target disasm
```
  000b2e7c: ldr.w r0,[r0,#0xb0]
  000b2e80: bx lr
```
