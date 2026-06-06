# Player::getKIPlayer
elf 0xa2a6a body 6
Sig: undefined __thiscall getKIPlayer(Player * this)

## decompile
```c

/* Player::getKIPlayer() */

undefined4 __thiscall Player::getKIPlayer(Player *this)

{
  return *(undefined4 *)(this + 0xd0);
}

```
## target disasm
```
  000b2a6a: ldr.w r0,[r0,#0xd0]
  000b2a6e: bx lr
```
