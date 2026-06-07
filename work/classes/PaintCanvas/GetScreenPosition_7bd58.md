# PaintCanvas::GetScreenPosition
elf 0x7bd58 body 576
Sig: undefined __thiscall GetScreenPosition(PaintCanvas * this, Matrix * param_1, Vector * param_2, Vector * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Matrix&,
   AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector&) */

void __thiscall
AbyssEngine::PaintCanvas::GetScreenPosition
          (PaintCanvas *this,Matrix *param_1,Vector *param_2,Vector *param_3)

{
  uint uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined4 *puVar4;
  int iVar5;
  uint uVar6;
  undefined4 uVar7;
  Matrix *pMVar8;
  int *piVar9;
  byte bVar10;
  byte bVar11;
  uint in_fpscr;
  uint uVar12;
  float extraout_s0;
  float fVar13;
  float fVar14;
  float fVar15;
  float unaff_s16;
  double dVar16;
  double dVar17;
  Matrix aMStack_dc [12];
  AEMath aAStack_d0 [64];
  undefined4 local_90;
  uint local_8c;
  uint local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined4 local_58;
  Matrix aMStack_50 [12];
  int local_44;
  
  piVar9 = *(int **)(DAT_0008bfb0 + 0x8bd70);
  local_44 = *piVar9;
  AEMath::MatrixTransformVector(aMStack_50,(Vector *)param_1);
  uVar3 = DAT_0008bfa0;
  uVar2 = DAT_0008bf98;
  if (*(uint *)(this + 0x170) < *(uint *)(this + 0x164)) {
    if (*this == (PaintCanvas)0x0) {
      pMVar8 = (Matrix *)&local_90;
      AEMath::MatrixInverseTransformVector
                (pMVar8,(Vector *)
                        (*(int *)(*(int *)(this + 0x168) + *(uint *)(this + 0x170) * 4) + 0xc));
    }
    else {
      uStack_74 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uStack_70 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uStack_6c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      puVar4 = (undefined4 *)((uint)&local_90 | 4);
      *puVar4 = 0;
      puVar4[1] = uStack_74;
      puVar4[2] = uStack_70;
      puVar4[3] = uStack_6c;
      local_78 = 0;
      uStack_68 = uVar2;
      uStack_60 = uVar3;
      local_90 = 0x3f800000;
      local_7c = 0x3f800000;
      local_58 = 0x3f800000;
      AEMath::MatrixIdentity(aAStack_d0,(Matrix *)&local_90);
      iVar5 = Engine::GetGravValue(*(Engine **)(this + 0x34));
      uVar6 = AEMath::Sinf((float)(*(double *)(iVar5 + 8) * DAT_0008bfa8));
      local_90 = AEMath::Cosf(extraout_s0);
      local_8c = uVar6 ^ 0x80000000;
      local_80 = uVar6;
      local_7c = local_90;
      __aeabi_memcpy((Matrix *)aAStack_d0,
                     *(int *)(*(int *)(this + 0x168) + *(int *)(this + 0x170) * 4) + 0xc,0x3c);
      AEMath::Matrix::operator*=((Matrix *)aAStack_d0,(Matrix *)&local_90);
      pMVar8 = aMStack_dc;
      AEMath::MatrixInverseTransformVector(pMVar8,(Vector *)aAStack_d0);
    }
    AEMath::Vector::operator=(param_3,(Vector *)pMVar8);
    fVar14 = *(float *)(param_3 + 8);
    iVar5 = *(int *)(*(int *)(this + 0x168) + *(int *)(this + 0x170) * 4);
    if (fVar14 <= *(float *)(iVar5 + 4)) {
      fVar13 = fVar14 * *(float *)(iVar5 + 0x4c);
      uVar6 = in_fpscr & 0xfffffff | (uint)(fVar13 == 0.0) << 0x1e;
      bVar10 = (byte)(uVar6 >> 0x1e);
      if (bVar10 == 0) {
        unaff_s16 = fVar14 * *(float *)(iVar5 + 0x48);
        uVar6 = in_fpscr & 0xfffffff | (uint)(unaff_s16 == 0.0) << 0x1e;
        bVar10 = (byte)(uVar6 >> 0x1e);
      }
      if (bVar10 == 0) {
        fVar15 = *(float *)param_3;
        uVar7 = GetWidth();
        iVar5 = GetWidth();
        fVar14 = *(float *)(param_3 + 4);
        dVar16 = (double)VectorSignedToFloat(uVar7,(byte)(uVar6 >> 0x16) & 3);
        dVar17 = (double)VectorSignedToFloat(iVar5 >> 1,(byte)(uVar6 >> 0x16) & 3);
        *(float *)param_3 = (float)(dVar17 - (((double)fVar15 * 0.5) / (double)fVar13) * dVar16);
        uVar7 = GetHeight();
        dVar16 = (double)VectorSignedToFloat(uVar7,(byte)(uVar6 >> 0x16) & 3);
        iVar5 = GetHeight();
        dVar17 = (double)VectorSignedToFloat(iVar5 >> 1,(byte)(uVar6 >> 0x16) & 3);
        fVar13 = *(float *)param_3;
        fVar14 = (float)(dVar17 + (((double)fVar14 * 0.5) / (double)unaff_s16) * dVar16);
        uVar1 = uVar6 & 0xfffffff | (uint)(fVar13 < 0.0) << 0x1f;
        uVar12 = uVar1 | (uint)NAN(fVar13) << 0x1c;
        *(float *)(param_3 + 4) = fVar14;
        bVar10 = (byte)(uVar1 >> 0x1f);
        bVar11 = (byte)(uVar12 >> 0x1c) & 1;
        if (bVar10 == bVar11) {
          uVar12 = uVar6 & 0xfffffff | (uint)(fVar14 < 0.0) << 0x1f | (uint)NAN(fVar14) << 0x1c;
          bVar11 = (byte)(uVar12 >> 0x18);
          bVar10 = bVar11 >> 7;
          bVar11 = bVar11 >> 4 & 1;
        }
        if (bVar10 == bVar11) {
          uVar7 = GetWidth();
          fVar14 = (float)VectorSignedToFloat(uVar7,(byte)(uVar12 >> 0x16) & 3);
          uVar12 = uVar12 & 0xfffffff;
          if (fVar13 < fVar14) {
            uVar7 = GetHeight();
            VectorSignedToFloat(uVar7,(byte)(uVar12 >> 0x16) & 3);
          }
        }
      }
    }
  }
  iVar5 = *piVar9 - local_44;
  if (iVar5 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar5);
  }
  return;
}

```

## target disasm
```
  0008bd58: push {r4,r5,r6,r7,lr}
  0008bd5a: add r7,sp,#0xc
  0008bd5c: push {r8,r9,r10}
  0008bd60: vpush {d8,d9,d10,d11}
  0008bd64: sub sp,#0xa0
  0008bd66: mov r5,r0
  0008bd68: ldr r0,[0x0008bfb0]
  0008bd6a: mov r8,r3
  0008bd6c: add r0,pc
  0008bd6e: ldr.w r10,[r0,#0x0]
  0008bd72: ldr.w r0,[r10,#0x0]
  0008bd76: str r0,[sp,#0x9c]
  0008bd78: add r0,sp,#0x90
  0008bd7a: blx 0x0006f688
  0008bd7e: ldr.w r1,[r5,#0x164]
  0008bd82: ldr.w r0,[r5,#0x170]
  0008bd86: cmp r0,r1
  0008bd88: bcs 0x0008be7a
  0008bd8a: ldrb r1,[r5,#0x0]
  0008bd8c: cmp r1,#0x0
  0008bd8e: beq 0x0008be3e
  0008bd90: adr r0,[0x8bf98]
  0008bd92: add.w r9,sp,#0x50
  0008bd96: vmov.i32 q8,#0x0
  0008bd9a: vld1.64 {d18,d19},[r0]
  0008bd9e: orr r0,r9,#0x4
  0008bda2: mov r1,r9
  0008bda4: vst1.32 {d16,d17},[r0]
  0008bda8: add.w r0,r9,#0x18
  0008bdac: vst1.64 {d16,d17},[r0]
  0008bdb0: add.w r0,r9,#0x28
  0008bdb4: vst1.64 {d18,d19},[r0]
  0008bdb8: mov.w r0,#0x3f800000
  0008bdbc: str r0,[sp,#0x50]
  0008bdbe: str r0,[sp,#0x64]
  0008bdc0: str r0,[sp,#0x88]
  0008bdc2: add r0,sp,#0x10
  0008bdc4: blx 0x0006fb2c
  0008bdc8: ldr r0,[r5,#0x34]
  0008bdca: blx 0x0006fb38
  0008bdce: vldr.64 d17,[r0,#0x8]
  0008bdd2: vldr.64 d16,[pc,#0x1d4]
  0008bdd6: ldr r0,[r5,#0x30]
  0008bdd8: vmul.f64 d16,d17,d16
  0008bddc: cmp r0,#0x1
  0008bdde: vcvt.f32.f64 s0,d16
  0008bde2: vneg.f32 s2,s0
  0008bde6: it eq
  0008bde8: vmov.eq.f32 s2,s0
  0008bdec: vmov r4,s2
  0008bdf0: mov r0,r4
  0008bdf2: blx 0x0006f1a8
  0008bdf6: mov r6,r0
  0008bdf8: mov r0,r4
  0008bdfa: blx 0x0006f49c
  0008bdfe: str r0,[sp,#0x50]
  0008be00: add r4,sp,#0x10
  0008be02: str r0,[sp,#0x64]
  0008be04: eor r0,r6,#0x80000000
  0008be08: str r0,[sp,#0x54]
  0008be0a: movs r2,#0x3c
  0008be0c: str r6,[sp,#0x60]
  0008be0e: ldr.w r0,[r5,#0x168]
  0008be12: ldr.w r1,[r5,#0x170]
  0008be16: ldr.w r0,[r0,r1,lsl #0x2]
  0008be1a: add.w r1,r0,#0xc
  0008be1e: mov r0,r4
  0008be20: blx 0x0006f1e4
  0008be24: mov r0,r4
  0008be26: mov r1,r9
  0008be28: blx 0x0006f4a8
  0008be2c: add r6,sp,#0x4
  0008be2e: add r2,sp,#0x90
  0008be30: mov r1,r4
  0008be32: mov r0,r6
  0008be34: blx 0x0006f4b4
  0008be38: mov r0,r8
  0008be3a: mov r1,r6
  0008be3c: b 0x0008be58
  0008be3e: ldr.w r1,[r5,#0x168]
  0008be42: add r4,sp,#0x50
  0008be44: add r2,sp,#0x90
  0008be46: ldr.w r0,[r1,r0,lsl #0x2]
  0008be4a: add.w r1,r0,#0xc
  0008be4e: mov r0,r4
  0008be50: blx 0x0006f4b4
  0008be54: mov r0,r8
  0008be56: mov r1,r4
  0008be58: blx 0x0006eb3c
  0008be5c: ldr.w r0,[r5,#0x168]
  0008be60: ldr.w r1,[r5,#0x170]
  0008be64: vldr.32 s2,[r8,#0x8]
  0008be68: ldr.w r0,[r0,r1,lsl #0x2]
  0008be6c: vldr.32 s0,[r0,#0x4]
  0008be70: vcmpe.f32 s2,s0
  0008be74: vmrs apsr,fpscr
  0008be78: ble 0x0008be7e
  0008be7a: movs r6,#0x0
  0008be7c: b 0x0008bf40
  0008be7e: vldr.32 s0,[r0,#0x4c]
  0008be82: movs r6,#0x0
  0008be84: vmul.f32 s0,s2,s0
  0008be88: vcmpe.f32 s0,#0
  0008be8c: vmrs apsr,fpscr
  0008be90: itttt ne
  0008be92: vldr.ne.32 s4,[r0,#0x48]
  0008be96: vmul.ne.f32 s16,s2,s4
  0008be9a: vcmpe.ne.f32 s16,#0
  0008be9e: vmrs.ne apsr,fpscr
  0008bea2: beq 0x0008bf40
  0008bea4: vldr.32 s2,[r8]
  0008bea8: mov r0,r5
  0008beaa: vmov.f64 d9,0x3fe0000000000000
  0008beae: vcvt.f64.f32 d16,s2
  0008beb2: vmul.f64 d10,d16,d9
  0008beb6: vcvt.f64.f32 d11,s0
  0008beba: blx 0x0006fafc
  0008bebe: mov r4,r0
  0008bec0: mov r0,r5
  0008bec2: blx 0x0006fafc
  0008bec6: vdiv.f64 d16,d10,d11
  0008beca: asrs r0,r0,#0x1
  0008becc: vldr.32 s0,[r8,#0x4]
  0008bed0: vcvt.f64.f32 d20,s16
  0008bed4: vcvt.f64.f32 d17,s0
  0008bed8: vmov s0,r4
  0008bedc: vmul.f64 d17,d17,d9
  0008bee0: vcvt.f64.s32 d18,s0
  0008bee4: vmov s0,r0
  0008bee8: mov r0,r5
  0008beea: vdiv.f64 d8,d17,d20
  0008beee: vcvt.f64.s32 d19,s0
  0008bef2: vmls.f64 d19,d16,d18
  0008bef6: vcvt.f32.f64 s0,d19
  0008befa: vstr.32 s0,[r8]
  0008befe: blx 0x0006fb08
  0008bf02: vmov s0,r0
  0008bf06: mov r0,r5
  0008bf08: vcvt.f64.s32 d9,s0
  0008bf0c: blx 0x0006fb08
  0008bf10: asrs r0,r0,#0x1
  0008bf12: movs r6,#0x0
  0008bf14: vmov s0,r0
  0008bf18: vcvt.f64.s32 d16,s0
  0008bf1c: vmla.f64 d16,d8,d9
  0008bf20: vldr.32 s16,[r8]
  0008bf24: vcvt.f32.f64 s0,d16
  0008bf28: vcmpe.f32 s16,#0
  0008bf2c: vstr.32 s0,[r8,#0x4]
  0008bf30: vmrs apsr,fpscr
  0008bf34: itt ge
  0008bf36: vcmpe.ge.f32 s0,#0
  0008bf3a: vmrs.ge apsr,fpscr
  0008bf3e: bge 0x0008bf5e
  0008bf40: ldr r0,[sp,#0x9c]
  0008bf42: ldr.w r1,[r10,#0x0]
  0008bf46: subs r0,r1,r0
  0008bf48: itttt eq
  0008bf4a: mov.eq r0,r6
  0008bf4c: add.eq sp,#0xa0
  0008bf4e: vpop.eq {d8,d9,d10,d11}
  0008bf52: pop.eq.w {r8,r9,r10}
  0008bf56: it eq
  0008bf58: pop.eq {r4,r5,r6,r7,pc}
  0008bf5a: blx 0x0006e824
  0008bf5e: mov r0,r5
  0008bf60: blx 0x0006fafc
  0008bf64: vmov s0,r0
  0008bf68: movs r6,#0x0
  0008bf6a: vcvt.f32.s32 s0,s0
  0008bf6e: vcmpe.f32 s16,s0
  0008bf72: vmrs apsr,fpscr
  0008bf76: bpl 0x0008bf40
  0008bf78: mov r0,r5
  0008bf7a: vldr.32 s16,[r8,#0x4]
  0008bf7e: blx 0x0006fb08
  0008bf82: vmov s0,r0
  0008bf86: vcvt.f32.s32 s0,s0
  0008bf8a: vcmpe.f32 s16,s0
  0008bf8e: vmrs apsr,fpscr
  0008bf92: it mi
  0008bf94: mov.mi r6,#0x1
  0008bf96: b 0x0008bf40
```
