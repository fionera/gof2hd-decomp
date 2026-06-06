# Player::damage
elf 0xa3694 body 12
Sig: undefined __thiscall damage(Player * this, int param_1)

## decompile
```c

/* Player::damage(int) */

void __thiscall Player::damage(Player *this,int param_1)

{
  (*(code *)(DAT_001abcf4 + 0x1abcf8))(this,param_1,0,0xffffffff);
  return;
}

```
## target disasm
```
  000b3694: movs r2,#0x0
  000b3696: mov.w r3,#0xffffffff
  000b369a: b.w 0x001abce8
  001abce8: bx pc
```
