# MeshMerger::MeshMerger
elf 0x173a60 body 364
Sig: undefined __thiscall MeshMerger(MeshMerger * this, int param_1, int param_2, PaintCanvas * param_3, ushort param_4)

## decompile
```c

/* MeshMerger::MeshMerger(int, int, AbyssEngine::PaintCanvas*, unsigned short) */

void __thiscall
MeshMerger::MeshMerger(MeshMerger *this,int param_1,int param_2,PaintCanvas *param_3,ushort param_4)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined4 uVar4;
  void *pvVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  uint uVar10;
  undefined4 *puVar11;
  int *piVar12;
  code *pcVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
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
  
  uVar14 = 0;
  uVar15 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar16 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar17 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar12 = *(int **)(DAT_00183bf0 + 0x183a7e);
  pcVar13 = *(code **)(DAT_00183bf4 + 0x183a84);
  lVar1 = (ulonglong)(uint)(param_2 * param_1) * 4;
  uVar9 = (undefined4)lVar1;
  local_4c = *piVar12;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = uVar15;
  *(undefined4 *)(this + 0x20) = uVar16;
  *(undefined4 *)(this + 0x24) = uVar17;
  *(PaintCanvas **)(this + 0xc) = param_3;
  *(int *)this = param_1;
  *(int *)(this + 0x30) = param_2;
  if ((int)((ulonglong)lVar1 >> 0x20) != 0) {
    uVar9 = 0xffffffff;
  }
  uVar4 = (*pcVar13)(uVar9);
  iVar6 = param_2 * param_1 * 4;
  *(undefined4 *)(this + 8) = uVar4;
  __aeabi_memclr4(uVar4,iVar6);
  uVar9 = (*pcVar13)(uVar9);
  *(undefined4 *)(this + 0x18) = uVar9;
  __aeabi_memclr4(uVar9,iVar6);
  uVar9 = (*pcVar13)(param_1 | param_1 >> 0x1f);
  *(undefined4 *)(this + 0x24) = uVar9;
  __aeabi_memclr(uVar9,param_1);
  uVar9 = (undefined4)((ulonglong)(uint)param_1 * 0x3c);
  if ((int)((ulonglong)(uint)param_1 * 0x3c >> 0x20) != 0) {
    uVar9 = 0xffffffff;
  }
  iVar6 = (*pcVar13)(uVar9);
  if (param_1 != 0) {
    iVar7 = 0;
    do {
      AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(iVar6 + iVar7));
      iVar7 = iVar7 + 0x3c;
    } while (param_1 * 0x3c != iVar7);
  }
  uVar3 = DAT_00183be8;
  uVar2 = DAT_00183be0;
  puVar11 = (undefined4 *)((uint)local_88 | 4);
  iVar7 = 0;
  *(int *)(this + 0x1c) = iVar6;
  for (iVar6 = 0; uVar10 = *(uint *)this, iVar6 < (int)uVar10; iVar6 = iVar6 + 1) {
    local_88[0] = 0x3f800000;
    *puVar11 = uVar14;
    puVar11[1] = uVar15;
    puVar11[2] = uVar16;
    puVar11[3] = uVar17;
    local_74 = 0x3f800000;
    uStack_60 = uVar2;
    uStack_58 = uVar3;
    local_50 = 0x3f800000;
    local_70 = uVar14;
    uStack_6c = uVar15;
    uStack_68 = uVar16;
    uStack_64 = uVar17;
    AbyssEngine::AEMath::Matrix::operator=
              ((Matrix *)(*(int *)(this + 0x1c) + iVar7),(Matrix *)local_88);
    iVar7 = iVar7 + 0x3c;
  }
  uVar8 = uVar10 | (int)uVar10 >> 0x1f;
  pvVar5 = operator_new__(uVar8);
  *(void **)(this + 0x28) = pvVar5;
  for (iVar6 = 0; iVar6 < (int)uVar10; iVar6 = iVar6 + 1) {
    *(undefined1 *)((int)pvVar5 + iVar6) = 1;
  }
  pvVar5 = operator_new__(uVar8);
  *(void **)(this + 0x2c) = pvVar5;
  for (iVar6 = 0; iVar6 < (int)uVar10; iVar6 = iVar6 + 1) {
    *(undefined1 *)((int)pvVar5 + iVar6) = 1;
  }
  this[0x34] = (MeshMerger)0x0;
  *(ushort *)(this + 4) = param_4;
  this[6] = (MeshMerger)0x0;
  iVar6 = *piVar12 - local_4c;
  if (iVar6 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar6);
}

```

## target disasm
```
  00183a60: push {r4,r5,r6,r7,lr}
  00183a62: add r7,sp,#0xc
  00183a64: push {r7,r8,r9,r10,r11}
  00183a68: vpush {d8,d9,d10,d11}
  00183a6c: sub sp,#0x48
  00183a6e: mov r8,r1
  00183a70: mov r4,r0
  00183a72: mul r5,r2,r8
  00183a76: ldr r0,[0x00183bf0]
  00183a78: ldr r1,[0x00183bf4]
  00183a7a: add r0,pc
  00183a7c: vmov.i32 q4,#0x0
  00183a80: add r1,pc
  00183a82: ldr.w r9,[r0,#0x0]
  00183a86: movs r0,#0x4
  00183a88: ldr.w r10,[r1,#0x0]
  00183a8c: umull r6,r0,r5,r0
  00183a90: ldr.w r1,[r9,#0x0]
  00183a94: str r1,[sp,#0x44]
  00183a96: add.w r1,r4,#0x18
  00183a9a: vst1.32 {d8,d9},[r1]
  00183a9e: cmp r0,#0x0
  00183aa0: str r3,[r4,#0xc]
  00183aa2: str.w r8,[r4,#0x0]
  00183aa6: str r2,[r4,#0x30]
  00183aa8: it ne
  00183aaa: mov.ne r0,#0x1
  00183aac: cmp r0,#0x0
  00183aae: it ne
  00183ab0: mov.ne.w r6,#0xffffffff
  00183ab4: mov r0,r6
  00183ab6: blx r10
  00183ab8: lsls r5,r5,#0x2
  00183aba: str r0,[r4,#0x8]
  00183abc: mov r1,r5
  00183abe: blx 0x0006ec14
  00183ac2: mov r0,r6
  00183ac4: blx r10
  00183ac6: mov r1,r5
  00183ac8: str r0,[r4,#0x18]
  00183aca: blx 0x0006ec14
  00183ace: orr.w r0,r8,r8, asr #0x1f
  00183ad2: blx r10
  00183ad4: mov r1,r8
  00183ad6: str r4,[sp,#0x4]
  00183ad8: str r0,[r4,#0x24]
  00183ada: blx 0x0006ec20
  00183ade: movs r0,#0x3c
  00183ae0: umull r0,r1,r8,r0
  00183ae4: cmp r1,#0x0
  00183ae6: it ne
  00183ae8: mov.ne r1,#0x1
  00183aea: cmp r1,#0x0
  00183aec: it ne
  00183aee: mov.ne.w r0,#0xffffffff
  00183af2: blx r10
  00183af4: mov r6,r0
  00183af6: cmp.w r8,#0x0
  00183afa: beq 0x00183b10
  00183afc: rsb r0,r8,r8, lsl #0x4
  00183b00: movs r5,#0x0
  00183b02: lsls r4,r0,#0x2
  00183b04: adds r0,r6,r5
  00183b06: blx 0x0006f118
  00183b0a: adds r5,#0x3c
  00183b0c: cmp r4,r5
  00183b0e: bne 0x00183b04
  00183b10: add r0,sp,#0x8
  00183b12: ldr r1,[sp,#0x4]
  00183b14: add.w r5,r0,#0x28
  00183b18: add.w r4,r0,#0x18
  00183b1c: orr r8,r0,#0x4
  00183b20: adr r0,[0x183be0]
  00183b22: vld1.64 {d10,d11},[r0]
  00183b26: mov.w r10,#0x3f800000
  00183b2a: mov.w r11,#0x0
  00183b2e: str.w r9,[sp,#0x0]
  00183b32: mov.w r9,#0x0
  00183b36: str r6,[r1,#0x1c]
  00183b38: b 0x00183b66
  00183b3a: str.w r10,[sp,#0x8]
  00183b3e: vst1.32 {d8,d9},[r8]
  00183b42: str.w r10,[sp,#0x1c]
  00183b46: vst1.64 {d8,d9},[r4]
  00183b4a: vst1.32 {d10,d11},[r5]
  00183b4e: str.w r10,[sp,#0x40]
  00183b52: ldr r0,[r1,#0x1c]
  00183b54: add r1,sp,#0x8
  00183b56: add r0,r9
  00183b58: blx 0x0006f148
  00183b5c: ldr r1,[sp,#0x4]
  00183b5e: add.w r9,r9,#0x3c
  00183b62: add.w r11,r11,#0x1
  00183b66: ldr r6,[r1,#0x0]
  00183b68: cmp r11,r6
  00183b6a: blt 0x00183b3a
  00183b6c: orr.w r5,r6,r6, asr #0x1f
  00183b70: mov r0,r5
  00183b72: blx 0x0006ec08
  00183b76: ldr r1,[sp,#0x4]
  00183b78: movs r2,#0x1
  00183b7a: ldr r4,[sp,#0x0]
  00183b7c: str r0,[r1,#0x28]
  00183b7e: movs r1,#0x0
  00183b80: b 0x00183b86
  00183b82: strb r2,[r0,r1]
  00183b84: adds r1,#0x1
  00183b86: cmp r1,r6
  00183b88: blt 0x00183b82
  00183b8a: mov r0,r5
  00183b8c: blx 0x0006ec08
  00183b90: ldr r3,[sp,#0x4]
  00183b92: movs r1,#0x0
  00183b94: movs r2,#0x1
  00183b96: str r0,[r3,#0x2c]
  00183b98: b 0x00183b9e
  00183b9a: strb r2,[r0,r1]
  00183b9c: adds r1,#0x1
  00183b9e: cmp r1,r6
  00183ba0: blt 0x00183b9a
  00183ba2: movs r0,#0x0
  00183ba4: ldr r1,[r7,#0x8]
  00183ba6: strb.w r0,[r3,#0x34]
  00183baa: strh r1,[r3,#0x4]
  00183bac: strb r0,[r3,#0x6]
  00183bae: ldr r0,[sp,#0x44]
  00183bb0: ldr r1,[r4,#0x0]
  00183bb2: subs r0,r1,r0
  00183bb4: itttt eq
  00183bb6: mov.eq r0,r3
  00183bb8: add.eq sp,#0x48
  00183bba: vpop.eq {d8,d9,d10,d11}
  00183bbe: add.eq sp,#0x4
  00183bc0: itt eq
  00183bc2: pop.eq.w {r8,r9,r10,r11}
  00183bc6: pop.eq {r4,r5,r6,r7,pc}
  00183bc8: blx 0x0006e824
```
