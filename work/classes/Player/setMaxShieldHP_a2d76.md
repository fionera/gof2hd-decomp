# Player::setMaxShieldHP
elf 0xa2d76 body 20
Sig: undefined __thiscall setMaxShieldHP(Player * this, int param_1)

## decompile
```c

/* Player::setMaxShieldHP(int) */

void __thiscall Player::setMaxShieldHP(Player *this,int param_1)

{
  uint in_fpscr;
  undefined4 uVar1;
  
  uVar1 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  *(int *)(this + 0x94) = param_1;
  *(undefined4 *)(this + 0x88) = uVar1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2d76: vmov s0,r1
  000b2d7a: vcvt.f32.s32 s0,s0
  000b2d7e: str.w r1,[r0,#0x94]
  000b2d82: vstr.32 s0,[r0,#0x88]
  000b2d86: b.w 0x001abcd8
```
