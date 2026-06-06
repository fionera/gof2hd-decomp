# Player::setMaxArmorHP
elf 0xa2d8a body 8
Sig: undefined __thiscall setMaxArmorHP(Player * this, int param_1)

## decompile
```c

/* Player::setMaxArmorHP(int) */

void __thiscall Player::setMaxArmorHP(Player *this,int param_1)

{
  *(int *)(this + 0x8c) = param_1;
  *(int *)(this + 0x90) = param_1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2d8a: strd r1,r1,[r0,#0x8c]
  000b2d8e: b.w 0x001abcd8
```
