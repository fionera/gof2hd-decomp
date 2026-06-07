# AEGeometry::setLodMeshesWithMeshIds
elf 0xa4b88 body 248
Sig: undefined __thiscall setLodMeshesWithMeshIds(AEGeometry * this, ushort * param_1, uint * param_2, int * param_3, int param_4)

## decompile
```c

/* AEGeometry::setLodMeshesWithMeshIds(unsigned short*, unsigned int*, int*, int) */

void __thiscall
AEGeometry::setLodMeshesWithMeshIds
          (AEGeometry *this,ushort *param_1,uint *param_2,int *param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  code *pcVar6;
  int iVar7;
  int iVar8;
  
  iVar1 = param_4 * 2;
  pcVar6 = *(code **)(DAT_000b4c80 + 0xb4ba0);
  if (CARRY4(param_4,param_4)) {
    iVar1 = -1;
  }
  uVar2 = (*pcVar6)(iVar1);
  *(undefined4 *)(this + 0x5c) = uVar2;
  uVar2 = (undefined4)((ulonglong)(uint)param_4 * 8);
  if ((int)((ulonglong)(uint)param_4 * 8 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  uVar2 = (*pcVar6)(uVar2);
  *(undefined4 *)(this + 100) = uVar2;
  uVar2 = (undefined4)((ulonglong)(uint)param_4 * 4);
  *(int *)(this + 0x50) = param_4;
  if ((int)((ulonglong)(uint)param_4 * 4 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  uVar2 = (*pcVar6)(uVar2);
  iVar1 = 0;
  iVar8 = 0;
  *(undefined4 *)(this + 0x54) = uVar2;
  for (iVar7 = 0; iVar7 < param_4; iVar7 = iVar7 + 1) {
    *(ushort *)(*(int *)(this + 0x5c) + iVar7 * 2) = param_1[iVar7];
    iVar4 = *(int *)(this + 100);
    iVar3 = *(int *)((int)param_3 + iVar8);
    *(int *)(iVar4 + iVar1) = iVar3;
    *(int *)(iVar4 + iVar1 + 4) = iVar3 >> 0x1f;
    AbyssEngine::PaintCanvas::TransformCreate
              (*(PaintCanvas **)(this + 0x2c),(uint *)(*(int *)(this + 0x54) + iVar8));
    AbyssEngine::PaintCanvas::TransformAddMeshId
              (*(PaintCanvas **)(this + 0x2c),*(uint *)(*(int *)(this + 0x54) + iVar8),
               *(uint *)((int)param_2 + iVar8));
    iVar3 = *(int *)(this + 100);
    uVar5 = *(uint *)(iVar3 + iVar1);
    iVar4 = *(int *)(iVar3 + iVar1 + 4);
    *(int *)(iVar3 + iVar1) = (int)((ulonglong)uVar5 * (ulonglong)uVar5);
    *(uint *)(iVar3 + iVar1 + 4) =
         uVar5 * iVar4 + uVar5 * iVar4 + (int)((ulonglong)uVar5 * (ulonglong)uVar5 >> 0x20);
    if (*(uint *)(this + 0x14) != 0xffffffff) {
      AbyssEngine::PaintCanvas::TransformAddChild
                (*(PaintCanvas **)(this + 0x2c),*(uint *)(*(int *)(this + 0x54) + iVar8),
                 *(uint *)(this + 0x14));
    }
    iVar1 = iVar1 + 8;
    iVar8 = iVar8 + 4;
  }
  return;
}

```

## target disasm
```
  000b4b88: push {r4,r5,r6,r7,lr}
  000b4b8a: add r7,sp,#0xc
  000b4b8c: push {r5,r6,r7,r8,r9,r10,r11}
  000b4b90: strd r2,r3,[sp,#0x4]
  000b4b94: mov r11,r1
  000b4b96: ldr r1,[0x000b4c80]
  000b4b98: mov r8,r0
  000b4b9a: ldr r4,[r7,#0x8]
  000b4b9c: add r1,pc
  000b4b9e: adds r0,r4,r4
  000b4ba0: ldr r5,[r1,#0x0]
  000b4ba2: it cs
  000b4ba4: mov.cs.w r0,#0xffffffff
  000b4ba8: blx r5
  000b4baa: movs r1,#0x8
  000b4bac: str.w r0,[r8,#0x5c]
  000b4bb0: umull r1,r2,r4,r1
  000b4bb4: cmp r2,#0x0
  000b4bb6: it ne
  000b4bb8: mov.ne r2,#0x1
  000b4bba: cmp r2,#0x0
  000b4bbc: it ne
  000b4bbe: mov.ne.w r1,#0xffffffff
  000b4bc2: mov r0,r1
  000b4bc4: blx r5
  000b4bc6: movs r1,#0x4
  000b4bc8: str.w r0,[r8,#0x64]
  000b4bcc: umull r1,r2,r4,r1
  000b4bd0: str.w r4,[r8,#0x50]
  000b4bd4: cmp r2,#0x0
  000b4bd6: it ne
  000b4bd8: mov.ne r2,#0x1
  000b4bda: cmp r2,#0x0
  000b4bdc: it ne
  000b4bde: mov.ne.w r1,#0xffffffff
  000b4be2: mov r0,r1
  000b4be4: blx r5
  000b4be6: movs r5,#0x0
  000b4be8: mov.w r10,#0x0
  000b4bec: mov.w r9,#0x0
  000b4bf0: str.w r0,[r8,#0x54]
  000b4bf4: b 0x000b4c74
  000b4bf6: ldr.w r0,[r8,#0x5c]
  000b4bfa: ldrh.w r1,[r11,r9,lsl #0x1]
  000b4bfe: strh.w r1,[r0,r9,lsl #0x1]
  000b4c02: ldr r0,[sp,#0x8]
  000b4c04: ldr.w r1,[r8,#0x64]
  000b4c08: ldr.w r0,[r0,r10]
  000b4c0c: str r0,[r1,r5]
  000b4c0e: add r1,r5
  000b4c10: asrs r0,r0,#0x1f
  000b4c12: str r0,[r1,#0x4]
  000b4c14: ldr.w r1,[r8,#0x54]
  000b4c18: ldr.w r0,[r8,#0x2c]
  000b4c1c: add r1,r10
  000b4c1e: blx 0x000720ac
  000b4c22: ldr.w r1,[r8,#0x54]
  000b4c26: ldr r2,[sp,#0x4]
  000b4c28: ldr.w r0,[r8,#0x2c]
  000b4c2c: ldr.w r1,[r1,r10]
  000b4c30: ldr.w r2,[r2,r10]
  000b4c34: blx 0x00073030
  000b4c38: ldr.w r0,[r8,#0x64]
  000b4c3c: adds r2,r0,r5
  000b4c3e: ldr r1,[r0,r5]
  000b4c40: ldr r3,[r2,#0x4]
  000b4c42: umull r4,r6,r1,r1
  000b4c46: mla r6,r1,r3,r6
  000b4c4a: str r4,[r0,r5]
  000b4c4c: mla r1,r1,r3,r6
  000b4c50: str r1,[r2,#0x4]
  000b4c52: ldr.w r2,[r8,#0x14]
  000b4c56: adds r0,r2,#0x1
  000b4c58: beq 0x000b4c6a
  000b4c5a: ldr.w r1,[r8,#0x54]
  000b4c5e: ldr.w r0,[r8,#0x2c]
  000b4c62: ldr.w r1,[r1,r10]
  000b4c66: blx 0x000720d0
  000b4c6a: adds r5,#0x8
  000b4c6c: add.w r10,r10,#0x4
  000b4c70: add.w r9,r9,#0x1
  000b4c74: ldr r0,[r7,#0x8]
  000b4c76: cmp r9,r0
  000b4c78: blt 0x000b4bf6
  000b4c7a: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000b4c7e: pop {r4,r5,r6,r7,pc}
```
