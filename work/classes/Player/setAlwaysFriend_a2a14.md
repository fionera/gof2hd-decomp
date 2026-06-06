# Player::setAlwaysFriend
elf 0xa2a14 body 20
Sig: undefined __thiscall setAlwaysFriend(Player * this, bool param_1)

## decompile
```c

/* Player::setAlwaysFriend(bool) */

void __thiscall Player::setAlwaysFriend(Player *this,bool param_1)

{
  this[0xed] = (Player)param_1;
  *(undefined2 *)(this + 0x5c) = 0x100;
  this[0xe0] = (Player)0x0;
  return;
}

```
## target disasm
```
  000b2a14: strb.w r1,[r0,#0xed]
  000b2a18: mov.w r1,#0x100
  000b2a1c: strh.w r1,[r0,#0x5c]
  000b2a20: movs r1,#0x0
  000b2a22: strb.w r1,[r0,#0xe0]
  000b2a26: bx lr
```
