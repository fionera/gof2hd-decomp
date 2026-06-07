# LodMeshMerger::transformMesh
elf 0x181550 body 504
Sig: undefined __stdcall transformMesh(Mesh * param_1, Matrix * param_2)

## decompile
```c

/* LodMeshMerger::transformMesh(AbyssEngine::Mesh*, AbyssEngine::AEMath::Matrix const&) */

void LodMeshMerger::transformMesh(Mesh *param_1,Matrix *param_2)

{
  Mesh MVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  float fVar6;
  int *piVar7;
  Mesh *pMVar8;
  uint uVar9;
  void *pvVar10;
  int iVar11;
  uint uVar12;
  float fVar13;
  undefined8 uVar14;
  longlong lVar15;
  undefined8 uVar16;
  undefined8 local_70;
  undefined4 local_68;
  undefined8 local_60;
  undefined4 local_58;
  float local_54;
  undefined4 local_50;
  float local_48;
  float local_44;
  float fStack_40;
  int local_3c;
  
  piVar7 = *(int **)(DAT_00191748 + 0x191568);
  local_3c = *piVar7;
  pMVar8 = operator_new(0x88);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(pMVar8 + 0x82) = 0;
  *(undefined4 *)(pMVar8 + 0x7e) = 0;
  pMVar8[0x38] = (Mesh)0x0;
  *(undefined4 *)(pMVar8 + 0x4c) = 0x3f800000;
  *(undefined4 *)(pMVar8 + 0x50) = 0;
  *(undefined4 *)(pMVar8 + 0x54) = 0;
  *(undefined4 *)(pMVar8 + 0x59) = 0;
  *(undefined4 *)(pMVar8 + 0x70) = 0;
  *(undefined4 *)(pMVar8 + 0x74) = uVar2;
  *(undefined4 *)(pMVar8 + 0x78) = uVar3;
  *(undefined4 *)(pMVar8 + 0x7c) = uVar4;
  *(undefined4 *)(pMVar8 + 0x60) = 0;
  *(undefined4 *)(pMVar8 + 100) = uVar2;
  *(undefined4 *)(pMVar8 + 0x68) = uVar3;
  *(undefined4 *)(pMVar8 + 0x6c) = uVar4;
  *(undefined4 *)(pMVar8 + 0x28) = 0;
  *(undefined4 *)(pMVar8 + 0x2c) = uVar2;
  *(undefined4 *)(pMVar8 + 0x30) = uVar3;
  *(undefined4 *)(pMVar8 + 0x34) = uVar4;
  *(undefined4 *)(pMVar8 + 0x14) = 0;
  *(undefined4 *)(pMVar8 + 0x18) = uVar2;
  *(undefined4 *)(pMVar8 + 0x1c) = uVar3;
  *(undefined4 *)(pMVar8 + 0x20) = uVar4;
  *(undefined4 *)(pMVar8 + 4) = 0;
  *(undefined4 *)(pMVar8 + 8) = uVar2;
  *(undefined4 *)(pMVar8 + 0xc) = uVar3;
  *(undefined4 *)(pMVar8 + 0x10) = uVar4;
  *(undefined4 *)(pMVar8 + 0x3c) = 0;
  *(undefined4 *)(pMVar8 + 0x40) = uVar2;
  *(undefined4 *)(pMVar8 + 0x44) = uVar3;
  *(undefined4 *)(pMVar8 + 0x48) = uVar4;
  *(undefined4 *)(pMVar8 + 0x55) = 0;
  uVar12 = (uint)*(ushort *)(param_1 + 2);
  *(ushort *)(pMVar8 + 2) = *(ushort *)(param_1 + 2);
  *(undefined2 *)(pMVar8 + 0x28) = *(undefined2 *)(param_1 + 0x28);
  MVar1 = *param_1;
  uVar9 = (uint)(byte)MVar1;
  *pMVar8 = MVar1;
  if (((byte)MVar1 & 2) != 0) {
    *(undefined4 *)(pMVar8 + 8) = *(undefined4 *)(param_1 + 8);
  }
  if ((int)(uVar9 << 0x1c) < 0) {
    *(undefined4 *)(pMVar8 + 0xc) = *(undefined4 *)(param_1 + 0xc);
  }
  if ((int)(uVar9 << 0x1b) < 0) {
    *(undefined4 *)(pMVar8 + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
  }
  if (((byte)MVar1 & 1) != 0) {
    pvVar10 = operator_new__(uVar12 * 0xc);
    iVar11 = 0;
    *(void **)(pMVar8 + 4) = pvVar10;
    for (uVar9 = 0; uVar9 < uVar12; uVar9 = uVar9 + 1) {
      AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_60,(Vector *)param_2);
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(pMVar8 + 4) + iVar11),(Vector *)&local_60);
      iVar11 = iVar11 + 0xc;
      uVar12 = (uint)*(ushort *)(param_1 + 2);
    }
    uVar9 = (uint)(byte)*param_1;
  }
  if ((int)(uVar9 << 0x1d) < 0) {
    pvVar10 = operator_new__(uVar12 * 0xc);
    iVar11 = 0;
    *(void **)(pMVar8 + 0x10) = pvVar10;
    for (uVar9 = 0; uVar9 < uVar12; uVar9 = uVar9 + 1) {
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_48,(Vector *)param_2);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_60,(Vector *)&local_48);
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(pMVar8 + 0x10) + iVar11),(Vector *)&local_60);
      iVar11 = iVar11 + 0xc;
      uVar12 = (uint)*(ushort *)(param_1 + 2);
    }
  }
  local_48 = *(float *)(param_1 + 0x48);
  local_44 = local_48;
  fStack_40 = local_48;
  AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_60,(Vector *)param_2);
  fVar6 = local_48;
  uVar5 = CONCAT44(fStack_40,local_44);
  AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_70,(Vector *)param_2);
  uVar16 = FloatVectorNeg(uVar5,2,2);
  uVar14 = FloatVectorCompareGreaterThan(uVar5,0,2);
  lVar15 = VectorBitwiseSelect(uVar14,uVar5,uVar16);
  if ((float)((ulonglong)lVar15 >> 0x20) < (float)lVar15) {
    lVar15 = lVar15 << 0x20;
  }
  local_54 = (float)((ulonglong)lVar15 >> 0x20);
  fVar13 = -fVar6;
  if (0.0 < fVar6) {
    fVar13 = fVar6;
  }
  if (local_54 < fVar13) {
    local_54 = fVar13;
  }
  local_50 = 0x3f800000;
  local_58 = local_68;
  local_60 = local_70;
  AbyssEngine::AEMath::BSphere::operator=((BSphere *)(pMVar8 + 0x3c),(BSphere *)&local_60);
  iVar11 = *piVar7 - local_3c;
  if (iVar11 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar11);
  }
  return;
}

```

## target disasm
```
  00191550: push {r4,r5,r6,r7,lr}
  00191552: add r7,sp,#0xc
  00191554: push {r7,r8,r9,r10,r11}
  00191558: vpush {d8,d9}
  0019155c: sub sp,#0x48
  0019155e: mov r5,r0
  00191560: ldr r0,[0x00191748]
  00191562: mov r9,r1
  00191564: add r0,pc
  00191566: ldr r0,[r0,#0x0]
  00191568: str r0,[sp,#0x8]
  0019156a: ldr r0,[r0,#0x0]
  0019156c: str r0,[sp,#0x44]
  0019156e: movs r0,#0x88
  00191570: blx 0x0006eb24
  00191574: mov r10,r0
  00191576: movs r0,#0x0
  00191578: vmov.i32 q8,#0x0
  0019157c: mov.w r1,#0x3f800000
  00191580: str.w r0,[r10,#0x82]
  00191584: str.w r0,[r10,#0x7e]
  00191588: strb.w r0,[r10,#0x38]
  0019158c: strd r1,r0,[r10,#0x4c]
  00191590: add.w r1,r10,#0x70
  00191594: str.w r0,[r10,#0x54]
  00191598: str.w r0,[r10,#0x59]
  0019159c: vst1.32 {d16,d17},[r1]
  001915a0: add.w r1,r10,#0x60
  001915a4: vst1.32 {d16,d17},[r1]
  001915a8: add.w r1,r10,#0x28
  001915ac: vst1.32 {d16,d17},[r1]
  001915b0: add.w r1,r10,#0x14
  001915b4: vst1.16 {d16,d17},[r1]
  001915b8: add.w r1,r10,#0x4
  001915bc: vst1.16 {d16,d17},[r1]
  001915c0: add.w r1,r10,#0x3c
  001915c4: vst1.32 {d16,d17},[r1]
  001915c8: str.w r0,[r10,#0x55]
  001915cc: ldrh.w r11,[r5,#0x2]
  001915d0: strh.w r11,[r10,#0x2]
  001915d4: ldrh r0,[r5,#0x28]
  001915d6: strh.w r0,[r10,#0x28]
  001915da: ldrb r0,[r5,#0x0]
  001915dc: str r1,[sp,#0x4]
  001915de: strb.w r0,[r10,#0x0]
  001915e2: tst r0,#0x2
  001915e6: itt ne
  001915e8: ldr.ne r1,[r5,#0x8]
  001915ea: str.w.ne r1,[r10,#0x8]
  001915ee: lsls r1,r0,#0x1c
  001915f0: itt mi
  001915f2: ldr.mi r1,[r5,#0xc]
  001915f4: str.w.mi r1,[r10,#0xc]
  001915f8: lsls r1,r0,#0x1b
  001915fa: itt mi
  001915fc: ldr.mi r1,[r5,#0x2c]
  001915fe: str.w.mi r1,[r10,#0x2c]
  00191602: lsls r1,r0,#0x1f
  00191604: str.w r9,[sp,#0xc]
  00191608: beq 0x0019164c
  0019160a: add.w r0,r11,r11, lsl #0x1
  0019160e: lsls r0,r0,#0x2
  00191610: blx 0x0006ec08
  00191614: add.w r8,sp,#0x20
  00191618: movs r6,#0x0
  0019161a: movs r4,#0x0
  0019161c: str.w r0,[r10,#0x4]
  00191620: b 0x00191642
  00191622: ldr r0,[r5,#0x4]
  00191624: mov r1,r9
  00191626: adds r2,r0,r6
  00191628: mov r0,r8
  0019162a: blx 0x0006f688
  0019162e: ldr.w r0,[r10,#0x4]
  00191632: mov r1,r8
  00191634: add r0,r6
  00191636: blx 0x0006eb3c
  0019163a: adds r6,#0xc
  0019163c: ldrh.w r11,[r5,#0x2]
  00191640: adds r4,#0x1
  00191642: uxth.w r0,r11
  00191646: cmp r4,r0
  00191648: bcc 0x00191622
  0019164a: ldrb r0,[r5,#0x0]
  0019164c: lsls r0,r0,#0x1d
  0019164e: bpl 0x001916a8
  00191650: uxth.w r0,r11
  00191654: lsls r0,r0,#0x1
  00191656: uxtah r0,r0,r11
  0019165a: lsls r0,r0,#0x2
  0019165c: blx 0x0006ec08
  00191660: add r6,sp,#0x38
  00191662: add r4,sp,#0x20
  00191664: mov.w r8,#0x0
  00191668: mov.w r9,#0x0
  0019166c: str.w r0,[r10,#0x10]
  00191670: b 0x001916a0
  00191672: ldr r0,[r5,#0x10]
  00191674: ldr r1,[sp,#0xc]
  00191676: add.w r2,r0,r8
  0019167a: mov r0,r6
  0019167c: blx 0x0006f694
  00191680: mov r0,r4
  00191682: mov r1,r6
  00191684: blx 0x0006ec80
  00191688: ldr.w r0,[r10,#0x10]
  0019168c: mov r1,r4
  0019168e: add r0,r8
  00191690: blx 0x0006eb3c
  00191694: add.w r8,r8,#0xc
  00191698: ldrh.w r11,[r5,#0x2]
  0019169c: add.w r9,r9,#0x1
  001916a0: uxth.w r0,r11
  001916a4: cmp r9,r0
  001916a6: bcc 0x00191672
  001916a8: ldr r0,[r5,#0x48]
  001916aa: add r2,sp,#0x38
  001916ac: strd r0,r0,[sp,#0x38]
  001916b0: ldr r4,[sp,#0xc]
  001916b2: str r0,[sp,#0x40]
  001916b4: add r0,sp,#0x20
  001916b6: mov r1,r4
  001916b8: blx 0x0006f688
  001916bc: add.w r2,r5,#0x3c
  001916c0: add r0,sp,#0x10
  001916c2: mov r1,r4
  001916c4: vldr.64 d8,[sp,#0x3c]
  001916c8: vldr.32 s18,[sp,#0x38]
  001916cc: blx 0x0006f688
  001916d0: vneg.f32 d16,d8
  001916d4: vcgt.f32 d1,d8,#0
  001916d8: vbsl d1,d8,d16
  001916dc: mov.w r1,#0x3f800000
  001916e0: vneg.f32 s0,s18
  001916e4: vcmpe.f32 s2,s3
  001916e8: vmrs apsr,fpscr
  001916ec: vcmpe.f32 s18,#0
  001916f0: it gt
  001916f2: vmov.gt.f32 s3,s2
  001916f6: vmrs apsr,fpscr
  001916fa: it gt
  001916fc: vmov.gt.f32 s0,s18
  00191700: vcmpe.f32 s0,s3
  00191704: vmrs apsr,fpscr
  00191708: it gt
  0019170a: vmov.gt.f32 s3,s0
  0019170e: ldr r0,[sp,#0x18]
  00191710: str r1,[sp,#0x30]
  00191712: add r1,sp,#0x20
  00191714: str r0,[sp,#0x28]
  00191716: ldr r0,[sp,#0x4]
  00191718: vldr.64 d16,[sp,#0x10]
  0019171c: vstr.64 d16,[sp,#0x20]
  00191720: vstr.32 s3,[sp,#0x2c]
  00191724: blx 0x0006eb18
  00191728: ldr r0,[sp,#0x44]
  0019172a: ldr r1,[sp,#0x8]
  0019172c: ldr r1,[r1,#0x0]
  0019172e: subs r0,r1,r0
  00191730: itttt eq
  00191732: mov.eq r0,r10
  00191734: add.eq sp,#0x48
  00191736: vpop.eq {d8,d9}
  0019173a: add.eq sp,#0x4
  0019173c: itt eq
  0019173e: pop.eq.w {r8,r9,r10,r11}
  00191742: pop.eq {r4,r5,r6,r7,pc}
  00191744: blx 0x0006e824
```
