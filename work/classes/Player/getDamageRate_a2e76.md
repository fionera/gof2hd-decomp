# Player::getDamageRate
elf 0xa2e76 body 6
Sig: undefined __thiscall getDamageRate(Player * this)

## decompile
```c

/* Player::getDamageRate() */

undefined4 __thiscall Player::getDamageRate(Player *this)

{
  return *(undefined4 *)(this + 0x98);
}

```
## target disasm
```
  000b2e76: ldr.w r0,[r0,#0x98]
  000b2e7a: bx lr
```
