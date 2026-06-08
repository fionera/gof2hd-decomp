# AbyssEngine::MeshIntersect
elf 0x6d818 body 670
Sig: undefined __thiscall MeshIntersect(AbyssEngine * this, float param_1, float param_2, Mesh * param_3)

## decompile
```c

/* AbyssEngine::MeshIntersect(float, float, AbyssEngine::Mesh*) */

float __thiscall
AbyssEngine::MeshIntersect(AbyssEngine *this,float param_1,float param_2,Mesh *param_3)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  float in_r2;
  float *pfVar5;
  float *pfVar6;
  uint uVar7;
  int in_r3;
  uint uVar8;
  uint uVar9;
  float fVar10;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  
  uVar9 = 0;
  do {
    uVar2 = DAT_0007dab8;
    if (*(ushort *)(in_r3 + 0x28) <= uVar9) {
      *(undefined4 *)this = DAT_0007dab8;
      *(undefined4 *)(this + 4) = uVar2;
      return param_1;
    }
    iVar3 = *(int *)(in_r3 + 4);
    iVar4 = *(int *)(in_r3 + 0x2c) + uVar9 * 2;
    pfVar5 = (float *)(iVar3 + (uint)*(ushort *)(*(int *)(in_r3 + 0x2c) + uVar9 * 2) * 0xc);
    fVar16 = *pfVar5;
    fVar17 = pfVar5[2];
    pfVar6 = (float *)(iVar3 + (uint)*(ushort *)(iVar4 + 2) * 0xc);
    pfVar5 = (float *)(iVar3 + (uint)*(ushort *)(iVar4 + 4) * 0xc);
    fVar19 = pfVar6[2];
    fVar18 = *pfVar6;
    param_1 = fVar17;
    fVar10 = fVar19;
    if (fVar19 < fVar17) {
      param_1 = fVar19;
      fVar10 = fVar17;
    }
    fVar12 = fVar16;
    fVar11 = fVar18;
    if (fVar18 < fVar16) {
      fVar12 = fVar18;
      fVar11 = fVar16;
    }
    fVar15 = pfVar5[2];
    fVar14 = *pfVar5;
    fVar13 = fVar15;
    if (fVar15 < fVar10) {
      fVar13 = fVar10;
    }
    if (fVar15 < param_1) {
      param_1 = fVar15;
    }
    fVar10 = fVar14;
    if (fVar14 < fVar11) {
      fVar10 = fVar11;
    }
    if (fVar14 < fVar12) {
      fVar12 = fVar14;
    }
    if ((((-1 < (int)((uint)(fVar13 < in_r2) << 0x1f)) && (fVar12 <= (float)param_3)) &&
        (-1 < (int)((uint)(fVar10 < (float)param_3) << 0x1f))) && (param_1 <= in_r2)) {
      fVar12 = fVar19 - fVar17;
      fVar13 = fVar18 - fVar16;
      fVar11 = fVar12 * fVar12 + fVar13 * fVar13;
      fVar10 = SQRT(fVar11);
      if (NAN(fVar10)) {
        fVar10 = (float)sqrtf(fVar10,param_2,fVar11);
        param_2 = extraout_s1;
      }
      param_1 = ((fVar13 / fVar10) * in_r2 + (-fVar12 / fVar10) * (float)param_3) -
                (fVar17 * (fVar13 / fVar10) + fVar16 * (-fVar12 / fVar10));
      if (param_1 <= 0.0) {
        fVar12 = fVar15 - fVar19;
        fVar13 = fVar14 - fVar18;
        fVar11 = fVar12 * fVar12 + fVar13 * fVar13;
        fVar10 = SQRT(fVar11);
        if (NAN(fVar10)) {
          fVar10 = (float)sqrtf(fVar10,param_2,fVar11);
          param_2 = extraout_s1_00;
        }
        param_1 = ((fVar13 / fVar10) * in_r2 + (-fVar12 / fVar10) * (float)param_3) -
                  (fVar19 * (fVar13 / fVar10) + fVar18 * (-fVar12 / fVar10));
        if (param_1 <= 0.0) {
          fVar17 = fVar17 - fVar15;
          fVar16 = fVar16 - fVar14;
          fVar18 = fVar17 * fVar17 + fVar16 * fVar16;
          fVar10 = SQRT(fVar18);
          if (NAN(fVar10)) {
            fVar10 = (float)sqrtf(fVar10,param_2,fVar18);
            param_2 = extraout_s1_01;
          }
          param_1 = ((fVar16 / fVar10) * in_r2 + (-fVar17 / fVar10) * (float)param_3) -
                    (fVar15 * (fVar16 / fVar10) + fVar14 * (-fVar17 / fVar10));
          if (param_1 <= 0.0) {
            iVar3 = *(int *)(in_r3 + 8);
            iVar4 = *(int *)(in_r3 + 0x2c) + uVar9 * 2;
            uVar9 = (uint)*(ushort *)(*(int *)(in_r3 + 0x2c) + uVar9 * 2);
            uVar8 = (uint)*(ushort *)(iVar4 + 2);
            uVar7 = (uint)*(ushort *)(iVar4 + 4);
            *(float *)this =
                 (*(float *)(iVar3 + uVar9 * 8) + *(float *)(iVar3 + uVar8 * 8) +
                 *(float *)(iVar3 + uVar7 * 8)) / 3.0;
            cVar1 = **(char **)(DAT_0007dabc + 0x7da96);
            fVar10 = (*(float *)((uVar9 << 3 | 4) + iVar3) + *(float *)((uVar8 << 3 | 4) + iVar3) +
                     *(float *)(iVar3 + (uVar7 << 3 | 4))) / 3.0;
            *(float *)(this + 4) = fVar10;
            if (cVar1 == '\0') {
              return fVar10;
            }
            *(float *)(this + 4) = 1.0 - fVar10;
            return 1.0 - fVar10;
          }
        }
      }
    }
    uVar9 = uVar9 + 3;
  } while( true );
}

```

## target disasm
```
  0007d818: push {r4,r5,r6,r7,lr}
  0007d81a: add r7,sp,#0xc
  0007d81c: push.w r11
  0007d820: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  0007d824: vmov s16,r2
  0007d828: mov r5,r3
  0007d82a: vmov s18,r1
  0007d82e: mov r4,r0
  0007d830: movs r6,#0x0
  0007d832: b 0x0007da18
  0007d834: ldr r1,[r5,#0x2c]
  0007d836: ldr r0,[r5,#0x4]
  0007d838: ldrh.w r2,[r1,r6,lsl #0x1]
  0007d83c: add.w r1,r1,r6, lsl #0x1
  0007d840: ldrh r3,[r1,#0x2]
  0007d842: ldrh r1,[r1,#0x4]
  0007d844: add.w r2,r2,r2, lsl #0x1
  0007d848: add.w r3,r3,r3, lsl #0x1
  0007d84c: add.w r2,r0,r2, lsl #0x2
  0007d850: add.w r1,r1,r1, lsl #0x1
  0007d854: vldr.32 s24,[r2]
  0007d858: vldr.32 s26,[r2,#0x8]
  0007d85c: add.w r2,r0,r3, lsl #0x2
  0007d860: add.w r0,r0,r1, lsl #0x2
  0007d864: vmov.f32 s2,s24
  0007d868: vldr.32 s30,[r2,#0x8]
  0007d86c: vmov.f32 s0,s26
  0007d870: vldr.32 s28,[r2]
  0007d874: vcmpe.f32 s26,s30
  0007d878: vmrs apsr,fpscr
  0007d87c: vmov.f32 s4,s30
  0007d880: vcmpe.f32 s24,s28
  0007d884: vmov.f32 s6,s28
  0007d888: it gt
  0007d88a: vmov.gt.f32 s4,s26
  0007d88e: it gt
  0007d890: vmov.gt.f32 s0,s30
  0007d894: vmrs apsr,fpscr
  0007d898: it gt
  0007d89a: vmov.gt.f32 s6,s24
  0007d89e: it gt
  0007d8a0: vmov.gt.f32 s2,s28
  0007d8a4: vldr.32 s22,[r0,#0x8]
  0007d8a8: vldr.32 s20,[r0]
  0007d8ac: vcmpe.f32 s4,s22
  0007d8b0: vmrs apsr,fpscr
  0007d8b4: vmov.f32 s8,s22
  0007d8b8: vcmpe.f32 s0,s22
  0007d8bc: it gt
  0007d8be: vmov.gt.f32 s8,s4
  0007d8c2: vmrs apsr,fpscr
  0007d8c6: vcmpe.f32 s6,s20
  0007d8ca: vmov.f32 s4,s20
  0007d8ce: it gt
  0007d8d0: vmov.gt.f32 s0,s22
  0007d8d4: vmrs apsr,fpscr
  0007d8d8: vcmpe.f32 s2,s20
  0007d8dc: it gt
  0007d8de: vmov.gt.f32 s4,s6
  0007d8e2: vmrs apsr,fpscr
  0007d8e6: vcmpe.f32 s8,s16
  0007d8ea: it gt
  0007d8ec: vmov.gt.f32 s2,s20
  0007d8f0: vmrs apsr,fpscr
  0007d8f4: bmi.w 0x0007da16
  0007d8f8: vcmpe.f32 s2,s18
  0007d8fc: vmrs apsr,fpscr
  0007d900: bgt.w 0x0007da16
  0007d904: vcmpe.f32 s4,s18
  0007d908: vmrs apsr,fpscr
  0007d90c: bmi.w 0x0007da16
  0007d910: vcmpe.f32 s0,s16
  0007d914: vmrs apsr,fpscr
  0007d918: bgt 0x0007da16
  0007d91a: vsub.f32 s4,s30,s26
  0007d91e: vsub.f32 s17,s28,s24
  0007d922: vmul.f32 s2,s4,s4
  0007d926: vneg.f32 s19,s4
  0007d92a: vmla.f32 s2,s17,s17
  0007d92e: vsqrt.f32 s0,s2
  0007d932: vcmpe.f32 s0,s0
  0007d936: vmrs apsr,fpscr
  0007d93a: bvc 0x0007d948
  0007d93c: vmov r0,s2
  0007d940: blx 0x0006ec50
  0007d944: vmov s0,r0
  0007d948: vdiv.f32 s2,s17,s0
  0007d94c: vdiv.f32 s0,s19,s0
  0007d950: vmul.f32 s4,s26,s2
  0007d954: vmul.f32 s2,s2,s16
  0007d958: vmla.f32 s4,s24,s0
  0007d95c: vmla.f32 s2,s0,s18
  0007d960: vsub.f32 s0,s2,s4
  0007d964: vcmpe.f32 s0,#0
  0007d968: vmrs apsr,fpscr
  0007d96c: bgt 0x0007da16
  0007d96e: vsub.f32 s4,s22,s30
  0007d972: vsub.f32 s17,s20,s28
  0007d976: vmul.f32 s2,s4,s4
  0007d97a: vneg.f32 s19,s4
  0007d97e: vmla.f32 s2,s17,s17
  0007d982: vsqrt.f32 s0,s2
  0007d986: vcmpe.f32 s0,s0
  0007d98a: vmrs apsr,fpscr
  0007d98e: bvc 0x0007d99c
  0007d990: vmov r0,s2
  0007d994: blx 0x0006ec50
  0007d998: vmov s0,r0
  0007d99c: vdiv.f32 s2,s17,s0
  0007d9a0: vdiv.f32 s0,s19,s0
  0007d9a4: vmul.f32 s4,s30,s2
  0007d9a8: vmul.f32 s2,s2,s16
  0007d9ac: vmla.f32 s4,s28,s0
  0007d9b0: vmla.f32 s2,s0,s18
  0007d9b4: vsub.f32 s0,s2,s4
  0007d9b8: vcmpe.f32 s0,#0
  0007d9bc: vmrs apsr,fpscr
  0007d9c0: bgt 0x0007da16
  0007d9c2: vsub.f32 s4,s26,s22
  0007d9c6: vsub.f32 s24,s24,s20
  0007d9ca: vmul.f32 s2,s4,s4
  0007d9ce: vneg.f32 s26,s4
  0007d9d2: vmla.f32 s2,s24,s24
  0007d9d6: vsqrt.f32 s0,s2
  0007d9da: vcmpe.f32 s0,s0
  0007d9de: vmrs apsr,fpscr
  0007d9e2: bvc 0x0007d9f0
  0007d9e4: vmov r0,s2
  0007d9e8: blx 0x0006ec50
  0007d9ec: vmov s0,r0
  0007d9f0: vdiv.f32 s2,s24,s0
  0007d9f4: vdiv.f32 s0,s26,s0
  0007d9f8: vmul.f32 s4,s22,s2
  0007d9fc: vmul.f32 s2,s2,s16
  0007da00: vmla.f32 s4,s20,s0
  0007da04: vmla.f32 s2,s0,s18
  0007da08: vsub.f32 s0,s2,s4
  0007da0c: vcmpe.f32 s0,#0
  0007da10: vmrs apsr,fpscr
  0007da14: ble 0x0007da30
  0007da16: adds r6,#0x3
  0007da18: ldrh r0,[r5,#0x28]
  0007da1a: cmp r6,r0
  0007da1c: bcc.w 0x0007d834
  0007da20: ldr r0,[0x0007dab8]
  0007da22: strd r0,r0,[r4,#0x0]
  0007da26: vpop {d8,d9,d10,d11,d12,d13,d14,d15}
  0007da2a: pop.w r11
  0007da2e: pop {r4,r5,r6,r7,pc}
  0007da30: ldr r1,[r5,#0x2c]
  0007da32: vmov.f32 s4,0x40400000
  0007da36: ldr r0,[r5,#0x8]
  0007da38: add.w r2,r1,r6, lsl #0x1
  0007da3c: ldrh.w r1,[r1,r6,lsl #0x1]
  0007da40: ldrh r3,[r2,#0x2]
  0007da42: ldrh r2,[r2,#0x4]
  0007da44: add.w r6,r0,r3, lsl #0x3
  0007da48: vldr.32 s0,[r6]
  0007da4c: add.w r6,r0,r1, lsl #0x3
  0007da50: vldr.32 s2,[r6]
  0007da54: add.w r6,r0,r2, lsl #0x3
  0007da58: vadd.f32 s0,s2,s0
  0007da5c: vldr.32 s2,[r6]
  0007da60: movs r6,#0x4
  0007da62: orr.w r3,r6,r3, lsl #0x3
  0007da66: orr.w r1,r6,r1, lsl #0x3
  0007da6a: add r3,r0
  0007da6c: add r1,r0
  0007da6e: vadd.f32 s0,s0,s2
  0007da72: vdiv.f32 s0,s0,s4
  0007da76: vstr.32 s0,[r4]
  0007da7a: vldr.32 s2,[r1]
  0007da7e: orr.w r1,r6,r2, lsl #0x3
  0007da82: vldr.32 s0,[r3]
  0007da86: add r0,r1
  0007da88: vadd.f32 s0,s2,s0
  0007da8c: vldr.32 s2,[r0]
  0007da90: ldr r0,[0x0007dabc]
  0007da92: add r0,pc
  0007da94: ldr r0,[r0,#0x0]
  0007da96: vadd.f32 s0,s0,s2
  0007da9a: ldrb r0,[r0,#0x0]
  0007da9c: cmp r0,#0x0
  0007da9e: vdiv.f32 s0,s0,s4
  0007daa2: vstr.32 s0,[r4,#0x4]
  0007daa6: beq 0x0007da26
  0007daa8: vmov.f32 s2,0x3f800000
  0007daac: vsub.f32 s0,s2,s0
  0007dab0: vstr.32 s0,[r4,#0x4]
  0007dab4: b 0x0007da26
```
