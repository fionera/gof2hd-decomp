# Player::setMaxEmpPoints
elf 0xa2d64 body 8
Sig: undefined __thiscall setMaxEmpPoints(Player * this, int param_1)

## decompile
```c

/* Player::setMaxEmpPoints(int) */

void __thiscall Player::setMaxEmpPoints(Player *this,int param_1)

{
  *(int *)(this + 0x7c) = param_1;
  *(int *)(this + 0x80) = param_1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2d64: strd r1,r1,[r0,#0x7c]
  000b2d68: b.w 0x001abcd8
```
