# Player::resetDamageDoneByPlayer
elf 0xa2f96 body 14
Sig: undefined __thiscall resetDamageDoneByPlayer(Player * this)

## decompile
```c

/* Player::resetDamageDoneByPlayer() */

void __thiscall Player::resetDamageDoneByPlayer(Player *this)

{
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0xdc) = 0;
  this[0xe0] = (Player)0x0;
  return;
}

```
## target disasm
```
  000b2f96: movs r1,#0x0
  000b2f98: str r1,[r0,#0x6c]
  000b2f9a: str.w r1,[r0,#0xdc]
  000b2f9e: strb.w r1,[r0,#0xe0]
  000b2fa2: bx lr
```
