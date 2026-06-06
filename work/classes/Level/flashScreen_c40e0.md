# Level::flashScreen
elf 0xc40e0 body 308
Sig: undefined __thiscall flashScreen(Level * this, int param_1)

## decompile
```c

/* Level::flashScreen(int) */

void __thiscall Level::flashScreen(Level *this,int param_1)

{
  undefined4 uVar1;
  float *pfVar2;
  uint in_fpscr;
  float fVar3;
  int iVar4;
  int iVar5;
  float fVar6;
  
  *(int *)(this + 0x15c) = param_1;
  this[0x158] = (Level)0x1;
  uVar1 = 2000;
  if (param_1 == 1) {
    uVar1 = 7000;
  }
  if (2 < param_1) {
    uVar1 = 10000;
  }
  *(undefined4 *)(this + 0x150) = uVar1;
  *(undefined4 *)(this + 0x154) = uVar1;
  uVar1 = DAT_000d4214;
  if (param_1 == 2) {
    fVar6 = **(float **)(DAT_000d421c + 0xd413e);
    fVar3 = **(float **)(DAT_000d4220 + 0xd414c);
    *(float *)(this + 0x140) = **(float **)(DAT_000d4218 + 0xd4130) * 1.5;
    fVar3 = fVar3 * 1.5;
    *(float *)(this + 0x144) = fVar6 * 1.5;
  }
  else {
    if (param_1 == 4) {
      *(undefined4 *)(this + 0x140) = 0;
      *(undefined4 *)(this + 0x144) = 0;
      *(undefined4 *)(this + 0x148) = uVar1;
      goto LAB_000d420c;
    }
    if (param_1 == 3) {
      *(undefined4 *)(this + 0x140) = 0;
      *(undefined4 *)(this + 0x144) = 0;
      *(undefined4 *)(this + 0x148) = 0;
      PlayerEgo::hitCamera(*(PlayerEgo **)(this + 0xf0));
      goto LAB_000d420c;
    }
    fVar3 = 5.0;
    if (param_1 == 1) {
      fVar3 = 8.0;
    }
    iVar5 = (int)(fVar3 * **(float **)(DAT_000d4224 + 0xd417c));
    if (0xfe < iVar5) {
      iVar5 = 0xff;
    }
    if (iVar5 < 0xb) {
      iVar5 = 10;
    }
    uVar1 = VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
    pfVar2 = *(float **)(DAT_000d4228 + 0xd41b2);
    *(undefined4 *)(this + 0x140) = uVar1;
    iVar5 = (int)(fVar3 * *pfVar2);
    if (0xfe < iVar5) {
      iVar5 = 0xff;
    }
    if (iVar5 < 0xb) {
      iVar5 = 10;
    }
    uVar1 = VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
    pfVar2 = *(float **)(DAT_000d422c + 0xd41e0);
    *(undefined4 *)(this + 0x144) = uVar1;
    iVar4 = (int)(fVar3 * *pfVar2);
    iVar5 = 0xff;
    if (iVar4 < 0xff) {
      iVar5 = iVar4;
    }
    if (iVar5 < 0xb) {
      iVar5 = 10;
    }
    fVar3 = (float)VectorSignedToFloat(iVar5,(byte)(in_fpscr >> 0x16) & 3);
  }
  *(float *)(this + 0x148) = fVar3;
LAB_000d420c:
  *(undefined4 *)(this + 0x14c) = DAT_000d4214;
  return;
}

```
## target disasm
```
  000d40e0: push {r4,r6,r7,lr}
  000d40e2: add r7,sp,#0x8
  000d40e4: mov r4,r0
  000d40e6: str.w r1,[r0,#0x15c]
  000d40ea: movs r0,#0x1
  000d40ec: cmp r1,#0x1
  000d40ee: strb.w r0,[r4,#0x158]
  000d40f2: mov.w r0,#0x7d0
  000d40f6: it eq
  000d40f8: movw.eq r0,#0x1b58
  000d40fc: cmp r1,#0x2
  000d40fe: it gt
  000d4100: movw.gt r0,#0x2710
  000d4104: strd r0,r0,[r4,#0x150]
  000d4108: beq 0x000d4126
  000d410a: cmp r1,#0x4
  000d410c: beq 0x000d415e
  000d410e: cmp r1,#0x3
  000d4110: bne 0x000d416c
  000d4112: ldr.w r0,[r4,#0xf0]
  000d4116: movs r1,#0x0
  000d4118: strd r1,r1,[r4,#0x140]
  000d411c: str.w r1,[r4,#0x148]
  000d4120: blx 0x000729ac
  000d4124: b 0x000d420c
  000d4126: ldr r0,[0x000d4218]
  000d4128: vmov.f32 s0,0x3fc00000
  000d412c: add r0,pc
  000d412e: ldr r0,[r0,#0x0]
  000d4130: vldr.32 s2,[r0]
  000d4134: ldr r0,[0x000d421c]
  000d4136: vmul.f32 s2,s2,s0
  000d413a: add r0,pc
  000d413c: ldr r0,[r0,#0x0]
  000d413e: vldr.32 s4,[r0]
  000d4142: ldr r0,[0x000d4220]
  000d4144: vmul.f32 s4,s4,s0
  000d4148: add r0,pc
  000d414a: ldr r0,[r0,#0x0]
  000d414c: vldr.32 s6,[r0]
  000d4150: vstr.32 s2,[r4,#0x140]
  000d4154: vmul.f32 s0,s6,s0
  000d4158: vstr.32 s4,[r4,#0x144]
  000d415c: b 0x000d4208
  000d415e: movs r1,#0x0
  000d4160: ldr r0,[0x000d4214]
  000d4162: strd r1,r1,[r4,#0x140]
  000d4166: str.w r0,[r4,#0x148]
  000d416a: b 0x000d420c
  000d416c: ldr r0,[0x000d4224]
  000d416e: vmov.f32 s2,0x41000000
  000d4172: vmov.f32 s0,0x40a00000
  000d4176: cmp r1,#0x1
  000d4178: add r0,pc
  000d417a: mov.w r1,#0xa
  000d417e: ldr r0,[r0,#0x0]
  000d4180: it eq
  000d4182: vmov.eq.f32 s0,s2
  000d4186: vldr.32 s2,[r0]
  000d418a: movs r0,#0xff
  000d418c: vmul.f32 s2,s0,s2
  000d4190: vcvt.s32.f32 s2,s2
  000d4194: vmov r2,s2
  000d4198: cmp r2,#0xff
  000d419a: it ge
  000d419c: mov.ge r2,r0
  000d419e: cmp r2,#0xa
  000d41a0: it le
  000d41a2: mov.le r2,r1
  000d41a4: vmov s2,r2
  000d41a8: vcvt.f32.s32 s2,s2
  000d41ac: ldr r2,[0x000d4228]
  000d41ae: add r2,pc
  000d41b0: ldr r2,[r2,#0x0]
  000d41b2: vstr.32 s2,[r4,#0x140]
  000d41b6: vldr.32 s2,[r2]
  000d41ba: vmul.f32 s2,s0,s2
  000d41be: vcvt.s32.f32 s2,s2
  000d41c2: vmov r2,s2
  000d41c6: cmp r2,#0xff
  000d41c8: it ge
  000d41ca: mov.ge r2,r0
  000d41cc: cmp r2,#0xa
  000d41ce: it le
  000d41d0: mov.le r2,r1
  000d41d2: vmov s2,r2
  000d41d6: vcvt.f32.s32 s2,s2
  000d41da: ldr r2,[0x000d422c]
  000d41dc: add r2,pc
  000d41de: ldr r2,[r2,#0x0]
  000d41e0: vstr.32 s2,[r4,#0x144]
  000d41e4: vldr.32 s2,[r2]
  000d41e8: vmul.f32 s0,s0,s2
  000d41ec: vcvt.s32.f32 s0,s0
  000d41f0: vmov r2,s0
  000d41f4: cmp r2,#0xff
  000d41f6: it lt
  000d41f8: mov.lt r0,r2
  000d41fa: cmp r0,#0xa
  000d41fc: it le
  000d41fe: mov.le r0,r1
  000d4200: vmov s0,r0
  000d4204: vcvt.f32.s32 s0,s0
  000d4208: vstr.32 s0,[r4,#0x148]
  000d420c: ldr r0,[0x000d4214]
  000d420e: str.w r0,[r4,#0x14c]
  000d4212: pop {r4,r6,r7,pc}
```
