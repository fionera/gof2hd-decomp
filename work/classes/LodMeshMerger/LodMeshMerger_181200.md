# LodMeshMerger::LodMeshMerger
elf 0x181200 body 380
Sig: undefined __thiscall LodMeshMerger(LodMeshMerger * this, int param_1, int param_2, PaintCanvas * param_3, ushort param_4)

## decompile
```c

/* LodMeshMerger::LodMeshMerger(int, int, AbyssEngine::PaintCanvas*, unsigned short) */

void __thiscall
LodMeshMerger::LodMeshMerger
          (LodMeshMerger *this,int param_1,int param_2,PaintCanvas *param_3,ushort param_4)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  uint uVar4;
  void *pvVar5;
  uint uVar6;
  int iVar7;
  undefined4 *puVar8;
  int *piVar9;
  int iVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 local_88 [5];
  undefined4 local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined4 local_50;
  int local_4c;
  
  piVar9 = *(int **)(DAT_001913a8 + 0x19121c);
  local_4c = *piVar9;
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 8));
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x28) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x2c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(PaintCanvas **)(this + 0x14) = param_3;
  *(int *)this = param_1;
  *(int *)(this + 0x38) = param_2;
  ArraySetLength<AbyssEngine::Mesh*>(param_2 * param_1,(Array *)(this + 8));
  uVar6 = *(uint *)this;
  lVar1 = (ulonglong)(uVar6 * param_2) * 4;
  uVar4 = (uint)lVar1;
  if ((int)((ulonglong)lVar1 >> 0x20) != 0) {
    uVar4 = 0xffffffff;
  }
  pvVar5 = operator_new__(uVar4);
  *(void **)(this + 0x24) = pvVar5;
  __aeabi_memclr4(pvVar5,uVar6 * param_2 * 4);
  pvVar5 = operator_new__(uVar6 | (int)uVar6 >> 0x1f);
  *(void **)(this + 0x2c) = pvVar5;
  __aeabi_memclr(pvVar5,uVar6);
  uVar4 = (uint)((ulonglong)uVar6 * 0x3c);
  if ((int)((ulonglong)uVar6 * 0x3c >> 0x20) != 0) {
    uVar4 = 0xffffffff;
  }
  pvVar5 = operator_new__(uVar4);
  if (uVar6 != 0) {
    iVar7 = 0;
    do {
      AbyssEngine::AEMath::Matrix::Matrix((Matrix *)((int)pvVar5 + iVar7));
      iVar7 = iVar7 + 0x3c;
    } while (uVar6 * 0x3c - iVar7 != 0);
  }
  uVar3 = DAT_001913a0;
  uVar2 = DAT_00191398;
  puVar8 = (undefined4 *)((uint)local_88 | 4);
  uVar11 = 0;
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar13 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar14 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar7 = 0;
  *(void **)(this + 0x28) = pvVar5;
  for (iVar10 = 0; uVar4 = *(uint *)this, iVar10 < (int)uVar4; iVar10 = iVar10 + 1) {
    local_88[0] = 0x3f800000;
    *puVar8 = uVar11;
    puVar8[1] = uVar12;
    puVar8[2] = uVar13;
    puVar8[3] = uVar14;
    local_74 = 0x3f800000;
    uStack_60 = uVar2;
    uStack_58 = uVar3;
    local_50 = 0x3f800000;
    local_70 = uVar11;
    uStack_6c = uVar12;
    uStack_68 = uVar13;
    uStack_64 = uVar14;
    AbyssEngine::AEMath::Matrix::operator=
              ((Matrix *)(*(int *)(this + 0x28) + iVar7),(Matrix *)local_88);
    iVar7 = iVar7 + 0x3c;
  }
  uVar6 = uVar4 | (int)uVar4 >> 0x1f;
  pvVar5 = operator_new__(uVar6);
  *(void **)(this + 0x30) = pvVar5;
  for (iVar7 = 0; iVar7 < (int)uVar4; iVar7 = iVar7 + 1) {
    *(undefined1 *)((int)pvVar5 + iVar7) = 1;
  }
  pvVar5 = operator_new__(uVar6);
  *(void **)(this + 0x34) = pvVar5;
  for (iVar7 = 0; iVar7 < (int)uVar4; iVar7 = iVar7 + 1) {
    *(undefined1 *)((int)pvVar5 + iVar7) = 1;
  }
  this[0x3c] = (LodMeshMerger)0x0;
  *(ushort *)(this + 4) = param_4;
  this[6] = (LodMeshMerger)0x0;
  iVar7 = *piVar9 - local_4c;
  if (iVar7 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar7);
}

```

## target disasm
```
  00191200: push {r4,r5,r6,r7,lr}
  00191202: add r7,sp,#0xc
  00191204: push {r7,r8,r9,r10,r11}
  00191208: vpush {d8,d9,d10,d11}
  0019120c: sub sp,#0x48
  0019120e: mov r4,r0
  00191210: ldr r0,[0x001913a8]
  00191212: add.w r5,r4,#0x8
  00191216: mov r8,r3
  00191218: add r0,pc
  0019121a: mov r6,r2
  0019121c: mov r9,r1
  0019121e: ldr.w r10,[r0,#0x0]
  00191222: ldr.w r0,[r10,#0x0]
  00191226: str r0,[sp,#0x44]
  00191228: mov r0,r5
  0019122a: blx 0x0006f724
  0019122e: mul r0,r6,r9
  00191232: vmov.i32 q8,#0x0
  00191236: add.w r1,r4,#0x20
  0019123a: vst1.32 {d16,d17},[r1]
  0019123e: str.w r8,[r4,#0x14]
  00191242: str.w r9,[r4,#0x0]
  00191246: str r6,[r4,#0x38]
  00191248: mov r1,r5
  0019124a: str r4,[sp,#0x4]
  0019124c: blx 0x00078814
  00191250: ldr r5,[r4,#0x0]
  00191252: movs r0,#0x4
  00191254: muls r6,r5
  00191256: umull r0,r1,r6,r0
  0019125a: cmp r1,#0x0
  0019125c: it ne
  0019125e: mov.ne r1,#0x1
  00191260: cmp r1,#0x0
  00191262: it ne
  00191264: mov.ne.w r0,#0xffffffff
  00191268: blx 0x0006ec08
  0019126c: lsls r1,r6,#0x2
  0019126e: str r0,[r4,#0x24]
  00191270: blx 0x0006ec14
  00191274: orr.w r0,r5,r5, asr #0x1f
  00191278: blx 0x0006ec08
  0019127c: mov r1,r5
  0019127e: str r0,[r4,#0x2c]
  00191280: blx 0x0006ec20
  00191284: movs r0,#0x3c
  00191286: umull r0,r1,r5,r0
  0019128a: cmp r1,#0x0
  0019128c: it ne
  0019128e: mov.ne r1,#0x1
  00191290: cmp r1,#0x0
  00191292: it ne
  00191294: mov.ne.w r0,#0xffffffff
  00191298: blx 0x0006ec08
  0019129c: mov r6,r0
  0019129e: cbz r5,0x001912b4
  001912a0: rsb r0,r5,r5, lsl #0x4
  001912a4: movs r5,#0x0
  001912a6: lsls r4,r0,#0x2
  001912a8: adds r0,r6,r5
  001912aa: blx 0x0006f118
  001912ae: adds r5,#0x3c
  001912b0: cmp r4,r5
  001912b2: bne 0x001912a8
  001912b4: add r0,sp,#0x8
  001912b6: ldr r1,[sp,#0x4]
  001912b8: add.w r5,r0,#0x28
  001912bc: add.w r4,r0,#0x18
  001912c0: orr r8,r0,#0x4
  001912c4: adr r0,[0x191398]
  001912c6: vmov.i32 q4,#0x0
  001912ca: vld1.64 {d10,d11},[r0]
  001912ce: mov.w r9,#0x3f800000
  001912d2: mov.w r11,#0x0
  001912d6: str.w r10,[sp,#0x0]
  001912da: mov.w r10,#0x0
  001912de: str r6,[r1,#0x28]
  001912e0: b 0x0019130e
  001912e2: str.w r9,[sp,#0x8]
  001912e6: vst1.32 {d8,d9},[r8]
  001912ea: str.w r9,[sp,#0x1c]
  001912ee: vst1.64 {d8,d9},[r4]
  001912f2: vst1.32 {d10,d11},[r5]
  001912f6: str.w r9,[sp,#0x40]
  001912fa: ldr r0,[r1,#0x28]
  001912fc: add r0,r10
  001912fe: add r1,sp,#0x8
  00191300: blx 0x0006f148
  00191304: add.w r10,r10,#0x3c
  00191308: add.w r11,r11,#0x1
  0019130c: ldr r1,[sp,#0x4]
  0019130e: ldr r6,[r1,#0x0]
  00191310: cmp r11,r6
  00191312: blt 0x001912e2
  00191314: orr.w r5,r6,r6, asr #0x1f
  00191318: mov r0,r5
  0019131a: mov r4,r1
  0019131c: blx 0x0006ec08
  00191320: ldr.w r8,[sp,#0x0]
  00191324: str r0,[r4,#0x30]
  00191326: movs r1,#0x0
  00191328: movs r2,#0x1
  0019132a: mov r3,r4
  0019132c: b 0x00191332
  0019132e: strb r2,[r0,r1]
  00191330: adds r1,#0x1
  00191332: cmp r1,r6
  00191334: blt 0x0019132e
  00191336: mov r0,r5
  00191338: mov r4,r3
  0019133a: blx 0x0006ec08
  0019133e: movs r1,#0x0
  00191340: movs r2,#0x1
  00191342: mov r3,r4
  00191344: str r0,[r4,#0x34]
  00191346: b 0x0019134c
  00191348: strb r2,[r0,r1]
  0019134a: adds r1,#0x1
  0019134c: cmp r1,r6
  0019134e: blt 0x00191348
  00191350: movs r0,#0x0
  00191352: ldr r1,[r7,#0x8]
  00191354: strb.w r0,[r3,#0x3c]
  00191358: strh r1,[r3,#0x4]
  0019135a: strb r0,[r3,#0x6]
  0019135c: ldr r0,[sp,#0x44]
  0019135e: ldr.w r1,[r8,#0x0]
  00191362: subs r0,r1,r0
  00191364: itttt eq
  00191366: mov.eq r0,r3
  00191368: add.eq sp,#0x48
  0019136a: vpop.eq {d8,d9,d10,d11}
  0019136e: add.eq sp,#0x4
  00191370: itt eq
  00191372: pop.eq.w {r8,r9,r10,r11}
  00191376: pop.eq {r4,r5,r6,r7,pc}
  00191378: blx 0x0006e824
```
