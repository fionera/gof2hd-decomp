# Player::isDamaged
elf 0xa36da body 6
Sig: undefined __thiscall isDamaged(Player * this)

## decompile
```c

/* Player::isDamaged() */

Player __thiscall Player::isDamaged(Player *this)

{
  return this[0xc1];
}

```
## target disasm
```
  000b36da: ldrb.w r0,[r0,#0xc1]
  000b36de: bx lr
```
