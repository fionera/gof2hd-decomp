# MeshMerger::transformMesh
elf 0x173d90 body 406
Sig: undefined __stdcall transformMesh(Mesh * param_1, Matrix * param_2)

## decompile
```c

/* MeshMerger::transformMesh(AbyssEngine::Mesh*, AbyssEngine::AEMath::Matrix const&) */

void MeshMerger::transformMesh(Mesh *param_1,Matrix *param_2)

{
  Mesh MVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  Mesh *pMVar6;
  uint uVar7;
  void *pvVar8;
  int iVar9;
  uint uVar10;
  undefined8 local_50;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined8 local_38;
  undefined4 local_30;
  int local_28;
  
  piVar5 = *(int **)(DAT_00183f28 + 0x183da4);
  local_28 = *piVar5;
  pMVar6 = operator_new(0x88);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(pMVar6 + 0x82) = 0;
  *(undefined4 *)(pMVar6 + 0x7e) = 0;
  pMVar6[0x38] = (Mesh)0x0;
  *(undefined4 *)(pMVar6 + 0x4c) = 0x3f800000;
  *(undefined4 *)(pMVar6 + 0x50) = 0;
  *(undefined4 *)(pMVar6 + 0x54) = 0;
  *(undefined4 *)(pMVar6 + 0x59) = 0;
  *(undefined4 *)(pMVar6 + 0x70) = 0;
  *(undefined4 *)(pMVar6 + 0x74) = uVar2;
  *(undefined4 *)(pMVar6 + 0x78) = uVar3;
  *(undefined4 *)(pMVar6 + 0x7c) = uVar4;
  *(undefined4 *)(pMVar6 + 0x60) = 0;
  *(undefined4 *)(pMVar6 + 100) = uVar2;
  *(undefined4 *)(pMVar6 + 0x68) = uVar3;
  *(undefined4 *)(pMVar6 + 0x6c) = uVar4;
  *(undefined4 *)(pMVar6 + 0x28) = 0;
  *(undefined4 *)(pMVar6 + 0x2c) = uVar2;
  *(undefined4 *)(pMVar6 + 0x30) = uVar3;
  *(undefined4 *)(pMVar6 + 0x34) = uVar4;
  *(undefined4 *)(pMVar6 + 0x14) = 0;
  *(undefined4 *)(pMVar6 + 0x18) = uVar2;
  *(undefined4 *)(pMVar6 + 0x1c) = uVar3;
  *(undefined4 *)(pMVar6 + 0x20) = uVar4;
  *(undefined4 *)(pMVar6 + 4) = 0;
  *(undefined4 *)(pMVar6 + 8) = uVar2;
  *(undefined4 *)(pMVar6 + 0xc) = uVar3;
  *(undefined4 *)(pMVar6 + 0x10) = uVar4;
  *(undefined4 *)(pMVar6 + 0x3c) = 0;
  *(undefined4 *)(pMVar6 + 0x40) = uVar2;
  *(undefined4 *)(pMVar6 + 0x44) = uVar3;
  *(undefined4 *)(pMVar6 + 0x48) = uVar4;
  *(undefined4 *)(pMVar6 + 0x55) = 0;
  uVar10 = (uint)*(ushort *)(param_1 + 2);
  *(ushort *)(pMVar6 + 2) = *(ushort *)(param_1 + 2);
  *(undefined2 *)(pMVar6 + 0x28) = *(undefined2 *)(param_1 + 0x28);
  MVar1 = *param_1;
  uVar7 = (uint)(byte)MVar1;
  *pMVar6 = MVar1;
  if (((byte)MVar1 & 2) != 0) {
    *(undefined4 *)(pMVar6 + 8) = *(undefined4 *)(param_1 + 8);
  }
  if ((int)(uVar7 << 0x1c) < 0) {
    *(undefined4 *)(pMVar6 + 0xc) = *(undefined4 *)(param_1 + 0xc);
  }
  if ((int)(uVar7 << 0x1b) < 0) {
    *(undefined4 *)(pMVar6 + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
  }
  if (((byte)MVar1 & 1) != 0) {
    pvVar8 = operator_new__(uVar10 * 0xc);
    iVar9 = 0;
    *(void **)(pMVar6 + 4) = pvVar8;
    for (uVar7 = 0; uVar7 < uVar10; uVar7 = uVar7 + 1) {
      AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_50,(Vector *)param_2);
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(pMVar6 + 4) + iVar9),(Vector *)&local_50);
      iVar9 = iVar9 + 0xc;
      uVar10 = (uint)*(ushort *)(param_1 + 2);
    }
    uVar7 = (uint)(byte)*param_1;
  }
  if ((int)(uVar7 << 0x1d) < 0) {
    pvVar8 = operator_new__(uVar10 * 0xc);
    iVar9 = 0;
    *(void **)(pMVar6 + 0x10) = pvVar8;
    for (uVar7 = 0; uVar7 < uVar10; uVar7 = uVar7 + 1) {
      AbyssEngine::AEMath::MatrixRotateVector((Matrix *)&local_38,(Vector *)param_2);
      AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_50,(Vector *)&local_38);
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(pMVar6 + 0x10) + iVar9),(Vector *)&local_50);
      iVar9 = iVar9 + 0xc;
      uVar10 = (uint)*(ushort *)(param_1 + 2);
    }
  }
  AbyssEngine::AEMath::MatrixTransformVector((Matrix *)&local_38,(Vector *)param_2);
  uStack_44 = *(undefined4 *)(param_1 + 0x48);
  local_48 = local_30;
  local_40 = 0x3f800000;
  local_50 = local_38;
  AbyssEngine::AEMath::BSphere::operator=((BSphere *)(pMVar6 + 0x3c),(BSphere *)&local_50);
  iVar9 = *piVar5 - local_28;
  if (iVar9 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar9);
  }
  return;
}

```

## target disasm
```
  00183d90: push {r4,r5,r6,r7,lr}
  00183d92: add r7,sp,#0xc
  00183d94: push {r8,r9,r10,r11}
  00183d98: sub sp,#0x3c
  00183d9a: mov r5,r0
  00183d9c: ldr r0,[0x00183f28]
  00183d9e: mov r9,r1
  00183da0: add r0,pc
  00183da2: ldr r0,[r0,#0x0]
  00183da4: str r0,[sp,#0x8]
  00183da6: ldr r0,[r0,#0x0]
  00183da8: str r0,[sp,#0x38]
  00183daa: movs r0,#0x88
  00183dac: blx 0x0006eb24
  00183db0: mov r10,r0
  00183db2: movs r0,#0x0
  00183db4: vmov.i32 q8,#0x0
  00183db8: mov.w r1,#0x3f800000
  00183dbc: str.w r0,[r10,#0x82]
  00183dc0: str.w r0,[r10,#0x7e]
  00183dc4: strb.w r0,[r10,#0x38]
  00183dc8: strd r1,r0,[r10,#0x4c]
  00183dcc: add.w r1,r10,#0x70
  00183dd0: str.w r0,[r10,#0x54]
  00183dd4: str.w r0,[r10,#0x59]
  00183dd8: vst1.32 {d16,d17},[r1]
  00183ddc: add.w r1,r10,#0x60
  00183de0: vst1.32 {d16,d17},[r1]
  00183de4: add.w r1,r10,#0x28
  00183de8: vst1.32 {d16,d17},[r1]
  00183dec: add.w r1,r10,#0x14
  00183df0: vst1.16 {d16,d17},[r1]
  00183df4: add.w r1,r10,#0x4
  00183df8: vst1.16 {d16,d17},[r1]
  00183dfc: add.w r1,r10,#0x3c
  00183e00: vst1.32 {d16,d17},[r1]
  00183e04: str.w r0,[r10,#0x55]
  00183e08: ldrh.w r11,[r5,#0x2]
  00183e0c: strh.w r11,[r10,#0x2]
  00183e10: ldrh r0,[r5,#0x28]
  00183e12: strh.w r0,[r10,#0x28]
  00183e16: ldrb r0,[r5,#0x0]
  00183e18: str r1,[sp,#0x4]
  00183e1a: strb.w r0,[r10,#0x0]
  00183e1e: tst r0,#0x2
  00183e22: itt ne
  00183e24: ldr.ne r1,[r5,#0x8]
  00183e26: str.w.ne r1,[r10,#0x8]
  00183e2a: lsls r1,r0,#0x1c
  00183e2c: itt mi
  00183e2e: ldr.mi r1,[r5,#0xc]
  00183e30: str.w.mi r1,[r10,#0xc]
  00183e34: lsls r1,r0,#0x1b
  00183e36: itt mi
  00183e38: ldr.mi r1,[r5,#0x2c]
  00183e3a: str.w.mi r1,[r10,#0x2c]
  00183e3e: lsls r1,r0,#0x1f
  00183e40: str.w r9,[sp,#0xc]
  00183e44: beq 0x00183e88
  00183e46: add.w r0,r11,r11, lsl #0x1
  00183e4a: lsls r0,r0,#0x2
  00183e4c: blx 0x0006ec08
  00183e50: add.w r8,sp,#0x10
  00183e54: movs r6,#0x0
  00183e56: movs r4,#0x0
  00183e58: str.w r0,[r10,#0x4]
  00183e5c: b 0x00183e7e
  00183e5e: ldr r0,[r5,#0x4]
  00183e60: mov r1,r9
  00183e62: adds r2,r0,r6
  00183e64: mov r0,r8
  00183e66: blx 0x0006f688
  00183e6a: ldr.w r0,[r10,#0x4]
  00183e6e: mov r1,r8
  00183e70: add r0,r6
  00183e72: blx 0x0006eb3c
  00183e76: adds r6,#0xc
  00183e78: ldrh.w r11,[r5,#0x2]
  00183e7c: adds r4,#0x1
  00183e7e: uxth.w r0,r11
  00183e82: cmp r4,r0
  00183e84: bcc 0x00183e5e
  00183e86: ldrb r0,[r5,#0x0]
  00183e88: lsls r0,r0,#0x1d
  00183e8a: bpl 0x00183ee4
  00183e8c: uxth.w r0,r11
  00183e90: lsls r0,r0,#0x1
  00183e92: uxtah r0,r0,r11
  00183e96: lsls r0,r0,#0x2
  00183e98: blx 0x0006ec08
  00183e9c: add r6,sp,#0x28
  00183e9e: add r4,sp,#0x10
  00183ea0: mov.w r8,#0x0
  00183ea4: mov.w r9,#0x0
  00183ea8: str.w r0,[r10,#0x10]
  00183eac: b 0x00183edc
  00183eae: ldr r0,[r5,#0x10]
  00183eb0: ldr r1,[sp,#0xc]
  00183eb2: add.w r2,r0,r8
  00183eb6: mov r0,r6
  00183eb8: blx 0x0006f694
  00183ebc: mov r0,r4
  00183ebe: mov r1,r6
  00183ec0: blx 0x0006ec80
  00183ec4: ldr.w r0,[r10,#0x10]
  00183ec8: mov r1,r4
  00183eca: add r0,r8
  00183ecc: blx 0x0006eb3c
  00183ed0: add.w r8,r8,#0xc
  00183ed4: ldrh.w r11,[r5,#0x2]
  00183ed8: add.w r9,r9,#0x1
  00183edc: uxth.w r0,r11
  00183ee0: cmp r9,r0
  00183ee2: bcc 0x00183eae
  00183ee4: ldr r1,[sp,#0xc]
  00183ee6: add.w r2,r5,#0x3c
  00183eea: add r0,sp,#0x28
  00183eec: blx 0x0006f688
  00183ef0: ldr r0,[r5,#0x48]
  00183ef2: ldr r1,[sp,#0x30]
  00183ef4: vldr.64 d16,[sp,#0x28]
  00183ef8: strd r1,r0,[sp,#0x18]
  00183efc: mov.w r0,#0x3f800000
  00183f00: str r0,[sp,#0x20]
  00183f02: add r1,sp,#0x10
  00183f04: ldr r0,[sp,#0x4]
  00183f06: vstr.64 d16,[sp,#0x10]
  00183f0a: blx 0x0006eb18
  00183f0e: ldr r0,[sp,#0x38]
  00183f10: ldr r1,[sp,#0x8]
  00183f12: ldr r1,[r1,#0x0]
  00183f14: subs r0,r1,r0
  00183f16: itttt eq
  00183f18: mov.eq r0,r10
  00183f1a: add.eq sp,#0x3c
  00183f1c: pop.eq.w {r8,r9,r10,r11}
  00183f20: pop.eq {r4,r5,r6,r7,pc}
  00183f22: blx 0x0006e824
```
