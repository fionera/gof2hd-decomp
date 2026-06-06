# Player::setRadius
elf 0xa2a60 body 4
Sig: undefined __thiscall setRadius(Player * this, int param_1)

## decompile
```c

/* Player::setRadius(int) */

void __thiscall Player::setRadius(Player *this,int param_1)

{
  *(int *)(this + 0x40) = param_1;
  return;
}

```
## target disasm
```
  000b2a60: str r1,[r0,#0x40]
  000b2a62: bx lr
```
