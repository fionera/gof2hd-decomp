# Player::setKIPlayer
elf 0xa2a64 body 6
Sig: undefined __thiscall setKIPlayer(Player * this, KIPlayer * param_1)

## decompile
```c

/* Player::setKIPlayer(KIPlayer*) */

void __thiscall Player::setKIPlayer(Player *this,KIPlayer *param_1)

{
  *(KIPlayer **)(this + 0xd0) = param_1;
  return;
}

```
## target disasm
```
  000b2a64: str.w r1,[r0,#0xd0]
  000b2a68: bx lr
```
