# Player::updateDamageRate
elf 0xa2730 body 130
Sig: undefined __thiscall updateDamageRate(Player * this)

## decompile
```c

/* Player::updateDamageRate() */

void __thiscall Player::updateDamageRate(Player *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x84),(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x78),(byte)(in_fpscr >> 0x16) & 3);
  fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x90),(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x8c),(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x80),(byte)(in_fpscr >> 0x16) & 3);
  fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x94),(byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x7c),(byte)(in_fpscr >> 0x16) & 3);
  fVar7 = (*(float *)(this + 0x88) / fVar7) * DAT_000b27b4;
  fVar5 = (fVar6 / fVar5) * DAT_000b27b4;
  fVar2 = (fVar2 / fVar3) * DAT_000b27b4;
  *(int *)(this + 0x98) = (int)((fVar1 / fVar4) * DAT_000b27b4);
  *(int *)(this + 0xa8) = (int)fVar7;
  *(int *)(this + 0xac) = (int)fVar5;
  *(int *)(this + 0xb0) = (int)fVar2;
  return;
}

```
## target disasm
```
  000b2730: vldr.32 s6,[r0,#0x84]
  000b2734: vldr.32 s0,[r0,#0x78]
  000b2738: vldr.32 s2,[r0,#0x7c]
  000b273c: vldr.32 s4,[r0,#0x80]
  000b2740: vcvt.f32.s32 s6,s6
  000b2744: vldr.32 s8,[r0,#0x90]
  000b2748: vcvt.f32.s32 s0,s0
  000b274c: vldr.32 s10,[r0,#0x8c]
  000b2750: vcvt.f32.s32 s8,s8
  000b2754: vldr.32 s12,[r0,#0x94]
  000b2758: vcvt.f32.s32 s10,s10
  000b275c: vcvt.f32.s32 s4,s4
  000b2760: vcvt.f32.s32 s12,s12
  000b2764: vldr.32 s14,[r0,#0x88]
  000b2768: vcvt.f32.s32 s2,s2
  000b276c: vdiv.f32 s0,s0,s6
  000b2770: vdiv.f32 s2,s2,s4
  000b2774: vdiv.f32 s6,s14,s12
  000b2778: vdiv.f32 s8,s10,s8
  000b277c: vldr.32 s4,[pc,#0x34]
  000b2780: vmul.f32 s0,s0,s4
  000b2784: vmul.f32 s6,s6,s4
  000b2788: vmul.f32 s8,s8,s4
  000b278c: vmul.f32 s2,s2,s4
  000b2790: vcvt.s32.f32 s0,s0
  000b2794: vcvt.s32.f32 s4,s6
  000b2798: vcvt.s32.f32 s6,s8
  000b279c: vcvt.s32.f32 s2,s2
  000b27a0: vstr.32 s0,[r0,#0x98]
  000b27a4: vstr.32 s4,[r0,#0xa8]
  000b27a8: vstr.32 s6,[r0,#0xac]
  000b27ac: vstr.32 s2,[r0,#0xb0]
  000b27b0: bx lr
```
