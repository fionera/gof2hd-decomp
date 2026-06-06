# Player::setShootingEnabled
elf 0xa29c6 body 6
Sig: undefined __thiscall setShootingEnabled(Player * this, bool param_1)

## decompile
```c

/* Player::setShootingEnabled(bool) */

void __thiscall Player::setShootingEnabled(Player *this,bool param_1)

{
  this[0xc3] = (Player)param_1;
  return;
}

```
## target disasm
```
  000b29c6: strb.w r1,[r0,#0xc3]
  000b29ca: bx lr
```
