# Player::setGammaHP
elf 0xa2d30 body 42
Sig: undefined __thiscall setGammaHP(Player * this, int param_1)

## decompile
```c

/* Player::setGammaHP(int) */

void __thiscall Player::setGammaHP(Player *this,int param_1)

{
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  uVar2 = uVar1;
  if (param_1 != DAT_000b2d5c) {
    uVar2 = DAT_000b2d60;
  }
  if (100 < param_1) {
    uVar1 = uVar2;
  }
  *(undefined4 *)(this + 0xb8) = uVar1;
  (*(code *)(DAT_001abce4 + 0x1abce8))();
  return;
}

```
## target disasm
```
  000b2d30: vmov s0,r1
  000b2d34: ldr r2,[0x000b2d5c]
  000b2d36: vldr.32 s2,[pc,#0x28]
  000b2d3a: vcvt.f32.s32 s0,s0
  000b2d3e: cmp r1,r2
  000b2d40: vmov.f32 s4,s0
  000b2d44: it ne
  000b2d46: vmov.ne.f32 s4,s2
  000b2d4a: cmp r1,#0x64
  000b2d4c: it gt
  000b2d4e: vmov.gt.f32 s0,s4
  000b2d52: vstr.32 s0,[r0,#0xb8]
  000b2d56: b.w 0x001abcd8
```
