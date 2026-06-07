# Radar::elipsoidIntersect
elf 0x12df48 body 206
Sig: undefined __stdcall elipsoidIntersect(int param_1, int param_2, Vector param_3)

## decompile
```c

/* Radar::elipsoidIntersect(int, int, AbyssEngine::AEMath::Vector) */

void Radar::elipsoidIntersect
               (undefined4 *param_1,int param_2,int param_3,int param_4,undefined4 param_5,
               undefined4 param_6,undefined4 param_7)

{
  uint uVar1;
  byte bVar2;
  float extraout_r0;
  uint in_fpscr;
  uint uVar3;
  undefined8 in_d0;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  fVar6 = (float)VectorSignedToFloat(*(int *)(param_2 + 0x108) - param_4,
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar7 = (float)VectorSignedToFloat(*(int *)(param_2 + 0x104) - param_3,
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar8 = *(float *)(param_2 + 0x110) * fVar6 * fVar6 + *(float *)(param_2 + 0x10c) * fVar7 * fVar7;
  uVar1 = in_fpscr & 0xfffffff | (uint)(fVar8 < 0.0) << 0x1f;
  uVar3 = uVar1 | (uint)NAN(fVar8) << 0x1c;
  if ((byte)(uVar1 >> 0x1f) == ((byte)(uVar3 >> 0x1c) & 1)) {
    Globals::sqrt((Globals *)**(undefined4 **)(DAT_0013e018 + 0x13dfaa),
                  (double)CONCAT44((int)((ulonglong)in_d0 >> 0x20),fVar6 * fVar6));
    fVar8 = (fVar8 - extraout_r0) / fVar8;
    if ((!NAN(fVar8)) &&
       (uVar1 = uVar3 & 0xfffffff | (uint)(fVar8 == 1.0) << 0x1e | (uint)(1.0 <= fVar8) << 0x1d,
       bVar2 = (byte)(uVar1 >> 0x18), !(bool)(bVar2 >> 5 & 1) || (bool)(bVar2 >> 6))) {
      fVar4 = (float)VectorSignedToFloat(param_4,(byte)(uVar1 >> 0x16) & 3);
      fVar5 = (float)VectorSignedToFloat(param_3,(byte)(uVar1 >> 0x16) & 3);
      param_6 = VectorSignedToFloat((int)(fVar4 + fVar8 * fVar6),(byte)(uVar1 >> 0x16) & 3);
      param_5 = VectorSignedToFloat((int)(fVar5 + fVar8 * fVar7),(byte)(uVar1 >> 0x16) & 3);
    }
  }
  *param_1 = param_5;
  param_1[1] = param_6;
  param_1[2] = param_7;
  return;
}

```

## target disasm
```
  0013df48: push {r4,r5,r6,r7,lr}
  0013df4a: add r7,sp,#0xc
  0013df4c: push {r8,r9,r10}
  0013df50: vpush {d8,d9,d10}
  0013df54: mov r4,r0
  0013df56: ldr.w r0,[r1,#0x108]
  0013df5a: ldr.w r8,[r7,#0x10]
  0013df5e: mov r10,r3
  0013df60: subs r0,r0,r3
  0013df62: mov r9,r2
  0013df64: vmov s0,r0
  0013df68: vcvt.f32.s32 s16,s0
  0013df6c: ldr.w r0,[r1,#0x104]
  0013df70: subs r0,r0,r2
  0013df72: vmov s0,r0
  0013df76: vcvt.f32.s32 s18,s0
  0013df7a: vldr.32 s4,[r1,#0x110]
  0013df7e: vmul.f32 s0,s16,s16
  0013df82: vldr.32 s2,[r1,#0x10c]
  0013df86: ldrd r6,r5,[r7,#0x8]
  0013df8a: vmul.f32 s6,s18,s18
  0013df8e: vmul.f32 s20,s4,s0
  0013df92: vmla.f32 s20,s2,s6
  0013df96: vcmpe.f32 s20,#0
  0013df9a: vmrs apsr,fpscr
  0013df9e: blt 0x0013e004
  0013dfa0: vmov r1,s20
  0013dfa4: ldr r0,[0x0013e018]
  0013dfa6: add r0,pc
  0013dfa8: ldr r0,[r0,#0x0]
  0013dfaa: ldr r0,[r0,#0x0]
  0013dfac: blx 0x000760e4
  0013dfb0: vmov s0,r0
  0013dfb4: vsub.f32 s0,s20,s0
  0013dfb8: vdiv.f32 s0,s0,s20
  0013dfbc: vcmpe.f32 s0,#0
  0013dfc0: vmrs apsr,fpscr
  0013dfc4: blt 0x0013e004
  0013dfc6: vmov.f32 s2,0x3f800000
  0013dfca: vcmpe.f32 s0,s2
  0013dfce: vmrs apsr,fpscr
  0013dfd2: bhi 0x0013e004
  0013dfd4: vmov s2,r10
  0013dfd8: vmov s4,r9
  0013dfdc: vcvt.f32.s32 s2,s2
  0013dfe0: vcvt.f32.s32 s4,s4
  0013dfe4: vmla.f32 s2,s0,s16
  0013dfe8: vmla.f32 s4,s0,s18
  0013dfec: vcvt.s32.f32 s0,s2
  0013dff0: vcvt.s32.f32 s2,s4
  0013dff4: vcvt.f32.s32 s0,s0
  0013dff8: vcvt.f32.s32 s2,s2
  0013dffc: vmov r5,s0
  0013e000: vmov r6,s2
  0013e004: strd r6,r5,[r4,#0x0]
  0013e008: str.w r8,[r4,#0x8]
  0013e00c: vpop {d8,d9,d10}
  0013e010: pop.w {r8,r9,r10}
  0013e014: pop {r4,r5,r6,r7,pc}
```
