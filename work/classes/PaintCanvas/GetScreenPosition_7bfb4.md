# PaintCanvas::GetScreenPosition
elf 0x7bfb4 body 388
Sig: undefined __thiscall GetScreenPosition(PaintCanvas * this, Matrix * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetScreenPosition(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Vector&) */

void __thiscall
AbyssEngine::PaintCanvas::GetScreenPosition(PaintCanvas *this,Matrix *param_1,Vector *param_2)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  byte bVar5;
  byte bVar6;
  uint in_fpscr;
  uint uVar7;
  uint uVar8;
  float fVar9;
  float fVar10;
  undefined8 in_d6;
  undefined4 in_s14;
  float unaff_s16;
  double dVar11;
  double dVar12;
  double dVar13;
  undefined8 local_48;
  undefined4 local_40;
  int local_3c;
  
  piVar4 = *(int **)(DAT_0008c138 + 0x8bfca);
  local_3c = *piVar4;
  local_48 = in_d6;
  local_40 = in_s14;
  if (*(uint *)(this + 0x170) < *(uint *)(this + 0x164)) {
    local_40 = *(undefined4 *)(param_1 + 0x2c);
    local_48 = CONCAT44(*(undefined4 *)(param_1 + 0x1c),*(undefined4 *)(param_1 + 0xc));
    AEMath::Vector::operator=(param_2,(Vector *)&local_48);
    iVar2 = *(int *)(*(int *)(this + 0x168) + *(int *)(this + 0x170) * 4);
    fVar9 = *(float *)(iVar2 + 0x4c) * *(float *)(param_2 + 8);
    uVar7 = in_fpscr & 0xfffffff | (uint)(fVar9 == 0.0) << 0x1e;
    bVar5 = (byte)(uVar7 >> 0x1e);
    if (bVar5 == 0) {
      unaff_s16 = *(float *)(param_2 + 8) * *(float *)(iVar2 + 0x48);
      uVar7 = in_fpscr & 0xfffffff | (uint)(unaff_s16 == 0.0) << 0x1e;
      bVar5 = (byte)(uVar7 >> 0x1e);
    }
    if (bVar5 == 0) {
      fVar10 = *(float *)param_2;
      uVar3 = GetWidth();
      iVar2 = GetWidth();
      dVar12 = (double)VectorSignedToFloat(uVar3,(byte)(uVar7 >> 0x16) & 3);
      dVar11 = ((double)*(float *)(param_2 + 4) * 0.5) / (double)unaff_s16;
      dVar13 = (double)VectorSignedToFloat(iVar2 >> 1,(byte)(uVar7 >> 0x16) & 3);
      *(float *)param_2 = (float)(dVar13 - (((double)fVar10 * 0.5) / (double)fVar9) * dVar12);
      uVar3 = GetHeight();
      dVar12 = (double)VectorSignedToFloat(uVar3,(byte)(uVar7 >> 0x16) & 3);
      iVar2 = GetHeight();
      dVar13 = (double)VectorSignedToFloat(iVar2 >> 1,(byte)(uVar7 >> 0x16) & 3);
      fVar9 = (float)(dVar13 + dVar11 * dVar12);
      *(float *)(param_2 + 4) = fVar9;
      if (*(float *)(param_2 + 8) <=
          *(float *)(*(int *)(*(int *)(this + 0x168) + *(int *)(this + 0x170) * 4) + 4)) {
        uVar1 = uVar7 & 0xfffffff | (uint)(fVar9 < 0.0) << 0x1f;
        uVar8 = uVar1 | (uint)NAN(fVar9) << 0x1c;
        bVar5 = (byte)(uVar1 >> 0x1f);
        bVar6 = (byte)(uVar8 >> 0x1c) & 1;
        fVar9 = SUB84(dVar11,0);
        if (bVar5 == bVar6) {
          fVar9 = *(float *)param_2;
          uVar8 = uVar7 & 0xfffffff | (uint)(fVar9 < 0.0) << 0x1f | (uint)NAN(fVar9) << 0x1c;
          bVar6 = (byte)(uVar8 >> 0x18);
          bVar5 = bVar6 >> 7;
          bVar6 = bVar6 >> 4 & 1;
        }
        if (bVar5 == bVar6) {
          uVar3 = GetWidth();
          fVar10 = (float)VectorSignedToFloat(uVar3,(byte)(uVar8 >> 0x16) & 3);
          uVar8 = uVar8 & 0xfffffff;
          if (fVar9 < fVar10) {
            uVar3 = GetHeight();
            VectorSignedToFloat(uVar3,(byte)(uVar8 >> 0x16) & 3);
          }
        }
      }
    }
  }
  iVar2 = *piVar4 - local_3c;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0008bfb4: push {r4,r5,r6,r7,lr}
  0008bfb6: add r7,sp,#0xc
  0008bfb8: push.w r8
  0008bfbc: vpush {d6,d7,d8,d9,d10,d11}
  0008bfc0: mov r4,r0
  0008bfc2: ldr r0,[0x0008c138]
  0008bfc4: mov r5,r2
  0008bfc6: add r0,pc
  0008bfc8: ldr.w r8,[r0,#0x0]
  0008bfcc: ldr.w r0,[r8,#0x0]
  0008bfd0: str r0,[sp,#0xc]
  0008bfd2: ldr.w r0,[r4,#0x164]
  0008bfd6: ldr.w r2,[r4,#0x170]
  0008bfda: cmp r2,r0
  0008bfdc: bcs 0x0008c0c8
  0008bfde: ldr r2,[r1,#0x1c]
  0008bfe0: ldr r0,[r1,#0xc]
  0008bfe2: ldr r1,[r1,#0x2c]
  0008bfe4: strd r0,r2,[sp,#0x0]
  0008bfe8: mov r0,r5
  0008bfea: str r1,[sp,#0x8]
  0008bfec: mov r1,sp
  0008bfee: blx 0x0006eb3c
  0008bff2: ldr.w r0,[r4,#0x168]
  0008bff6: movs r6,#0x0
  0008bff8: ldr.w r1,[r4,#0x170]
  0008bffc: vldr.32 s2,[r5,#0x8]
  0008c000: ldr.w r0,[r0,r1,lsl #0x2]
  0008c004: vldr.32 s0,[r0,#0x4c]
  0008c008: vmul.f32 s0,s0,s2
  0008c00c: vcmpe.f32 s0,#0
  0008c010: vmrs apsr,fpscr
  0008c014: itttt ne
  0008c016: vldr.ne.32 s4,[r0,#0x48]
  0008c01a: vmul.ne.f32 s16,s2,s4
  0008c01e: vcmpe.ne.f32 s16,#0
  0008c022: vmrs.ne apsr,fpscr
  0008c026: beq 0x0008c0ca
  0008c028: vldr.32 s2,[r5]
  0008c02c: mov r0,r4
  0008c02e: vmov.f64 d9,0x3fe0000000000000
  0008c032: vcvt.f64.f32 d16,s2
  0008c036: vmul.f64 d10,d16,d9
  0008c03a: vcvt.f64.f32 d11,s0
  0008c03e: blx 0x0006fafc
  0008c042: mov r6,r0
  0008c044: mov r0,r4
  0008c046: blx 0x0006fafc
  0008c04a: vdiv.f64 d16,d10,d11
  0008c04e: asrs r0,r0,#0x1
  0008c050: vldr.32 s0,[r5,#0x4]
  0008c054: vcvt.f64.f32 d20,s16
  0008c058: vcvt.f64.f32 d17,s0
  0008c05c: vmov s0,r6
  0008c060: vmul.f64 d17,d17,d9
  0008c064: vcvt.f64.s32 d18,s0
  0008c068: vmov s0,r0
  0008c06c: mov r0,r4
  0008c06e: vdiv.f64 d8,d17,d20
  0008c072: vcvt.f64.s32 d19,s0
  0008c076: vmls.f64 d19,d16,d18
  0008c07a: vcvt.f32.f64 s0,d19
  0008c07e: vstr.32 s0,[r5]
  0008c082: blx 0x0006fb08
  0008c086: vmov s0,r0
  0008c08a: mov r0,r4
  0008c08c: vcvt.f64.s32 d9,s0
  0008c090: blx 0x0006fb08
  0008c094: asrs r0,r0,#0x1
  0008c096: vmov s0,r0
  0008c09a: vcvt.f64.s32 d16,s0
  0008c09e: vmla.f64 d16,d8,d9
  0008c0a2: vcvt.f32.f64 s0,d16
  0008c0a6: vstr.32 s0,[r5,#0x4]
  0008c0aa: ldr.w r0,[r4,#0x168]
  0008c0ae: ldr.w r1,[r4,#0x170]
  0008c0b2: vldr.32 s2,[r5,#0x8]
  0008c0b6: ldr.w r0,[r0,r1,lsl #0x2]
  0008c0ba: vldr.32 s4,[r0,#0x4]
  0008c0be: vcmpe.f32 s2,s4
  0008c0c2: vmrs apsr,fpscr
  0008c0c6: ble 0x0008c0e4
  0008c0c8: movs r6,#0x0
  0008c0ca: ldr r0,[sp,#0xc]
  0008c0cc: ldr.w r1,[r8,#0x0]
  0008c0d0: subs r0,r1,r0
  0008c0d2: itttt eq
  0008c0d4: mov.eq r0,r6
  0008c0d6: vpop.eq {d6,d7,d8,d9,d10,d11}
  0008c0da: pop.eq.w r8
  0008c0de: pop.eq {r4,r5,r6,r7,pc}
  0008c0e0: blx 0x0006e824
  0008c0e4: vcmpe.f32 s0,#0
  0008c0e8: movs r6,#0x0
  0008c0ea: vmrs apsr,fpscr
  0008c0ee: ittt ge
  0008c0f0: vldr.ge.32 s16,[r5]
  0008c0f4: vcmpe.ge.f32 s16,#0
  0008c0f8: vmrs.ge apsr,fpscr
  0008c0fc: blt 0x0008c0ca
  0008c0fe: mov r0,r4
  0008c100: blx 0x0006fafc
  0008c104: vmov s0,r0
  0008c108: movs r6,#0x0
  0008c10a: vcvt.f32.s32 s0,s0
  0008c10e: vcmpe.f32 s16,s0
  0008c112: vmrs apsr,fpscr
  0008c116: bpl 0x0008c0ca
  0008c118: mov r0,r4
  0008c11a: vldr.32 s16,[r5,#0x4]
  0008c11e: blx 0x0006fb08
  0008c122: vmov s0,r0
  0008c126: vcvt.f32.s32 s0,s0
  0008c12a: vcmpe.f32 s16,s0
  0008c12e: vmrs apsr,fpscr
  0008c132: it mi
  0008c134: mov.mi r6,#0x1
  0008c136: b 0x0008c0ca
```
