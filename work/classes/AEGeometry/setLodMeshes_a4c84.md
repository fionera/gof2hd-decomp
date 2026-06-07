# AEGeometry::setLodMeshes
elf 0xa4c84 body 248
Sig: undefined __thiscall setLodMeshes(AEGeometry * this, ushort * param_1, int * param_2, int param_3)

## decompile
```c

/* AEGeometry::setLodMeshes(unsigned short*, int*, int) */

void __thiscall AEGeometry::setLodMeshes(AEGeometry *this,ushort *param_1,int *param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  code *pcVar6;
  int iVar7;
  int iVar8;
  
  iVar1 = param_3 * 2;
  if (CARRY4(param_3,param_3)) {
    iVar1 = -1;
  }
  pcVar6 = *(code **)(DAT_000b4d7c + 0xb4c9c);
  uVar2 = (*pcVar6)(iVar1);
  *(undefined4 *)(this + 0x5c) = uVar2;
  uVar2 = (undefined4)((ulonglong)(uint)param_3 * 8);
  if ((int)((ulonglong)(uint)param_3 * 8 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  uVar2 = (*pcVar6)(uVar2);
  *(undefined4 *)(this + 100) = uVar2;
  uVar2 = (undefined4)((ulonglong)(uint)param_3 * 4);
  *(int *)(this + 0x50) = param_3;
  if ((int)((ulonglong)(uint)param_3 * 4 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  uVar2 = (*pcVar6)(uVar2);
  iVar1 = 0;
  iVar8 = 0;
  *(undefined4 *)(this + 0x54) = uVar2;
  for (iVar7 = 0; iVar7 < param_3; iVar7 = iVar7 + 1) {
    *(ushort *)(*(int *)(this + 0x5c) + iVar7 * 2) = param_1[iVar7];
    iVar4 = *(int *)(this + 100);
    iVar3 = *(int *)((int)param_2 + iVar8);
    *(int *)(iVar4 + iVar1) = iVar3;
    *(int *)(iVar4 + iVar1 + 4) = iVar3 >> 0x1f;
    AbyssEngine::PaintCanvas::TransformCreate
              (*(PaintCanvas **)(this + 0x2c),(uint *)(*(int *)(this + 0x54) + iVar8));
    AbyssEngine::PaintCanvas::TransformAddMesh
              (*(uint *)(this + 0x2c),(ushort)*(undefined4 *)(*(int *)(this + 0x54) + iVar8),
               SUB21(param_1[iVar7],0));
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
  000b4c84: push {r4,r5,r6,r7,lr}
  000b4c86: add r7,sp,#0xc
  000b4c88: push {r5,r6,r7,r8,r9,r10,r11}
  000b4c8c: str r2,[sp,#0x4]
  000b4c8e: mov r10,r1
  000b4c90: ldr r1,[0x000b4d7c]
  000b4c92: mov r9,r0
  000b4c94: adds r0,r3,r3
  000b4c96: mov r5,r3
  000b4c98: add r1,pc
  000b4c9a: it cs
  000b4c9c: mov.cs.w r0,#0xffffffff
  000b4ca0: ldr r4,[r1,#0x0]
  000b4ca2: blx r4
  000b4ca4: movs r1,#0x8
  000b4ca6: str.w r0,[r9,#0x5c]
  000b4caa: umull r1,r2,r5,r1
  000b4cae: cmp r2,#0x0
  000b4cb0: it ne
  000b4cb2: mov.ne r2,#0x1
  000b4cb4: cmp r2,#0x0
  000b4cb6: it ne
  000b4cb8: mov.ne.w r1,#0xffffffff
  000b4cbc: mov r0,r1
  000b4cbe: blx r4
  000b4cc0: movs r1,#0x4
  000b4cc2: str.w r0,[r9,#0x64]
  000b4cc6: umull r1,r2,r5,r1
  000b4cca: str r5,[sp,#0x8]
  000b4ccc: str.w r5,[r9,#0x50]
  000b4cd0: cmp r2,#0x0
  000b4cd2: it ne
  000b4cd4: mov.ne r2,#0x1
  000b4cd6: cmp r2,#0x0
  000b4cd8: it ne
  000b4cda: mov.ne.w r1,#0xffffffff
  000b4cde: mov r0,r1
  000b4ce0: blx r4
  000b4ce2: movs r4,#0x0
  000b4ce4: mov.w r11,#0x0
  000b4ce8: mov.w r8,#0x0
  000b4cec: str.w r0,[r9,#0x54]
  000b4cf0: b 0x000b4d70
  000b4cf2: ldr.w r0,[r9,#0x5c]
  000b4cf6: ldrh.w r1,[r10,r8,lsl #0x1]
  000b4cfa: strh.w r1,[r0,r8,lsl #0x1]
  000b4cfe: ldr r0,[sp,#0x4]
  000b4d00: ldr.w r1,[r9,#0x64]
  000b4d04: ldr.w r0,[r0,r11]
  000b4d08: str r0,[r1,r4]
  000b4d0a: add r1,r4
  000b4d0c: asrs r0,r0,#0x1f
  000b4d0e: str r0,[r1,#0x4]
  000b4d10: ldr.w r1,[r9,#0x54]
  000b4d14: ldr.w r0,[r9,#0x2c]
  000b4d18: add r1,r11
  000b4d1a: blx 0x000720ac
  000b4d1e: ldr.w r1,[r9,#0x54]
  000b4d22: movs r3,#0x0
  000b4d24: ldrh.w r2,[r10,r8,lsl #0x1]
  000b4d28: ldr.w r0,[r9,#0x2c]
  000b4d2c: ldr.w r1,[r1,r11]
  000b4d30: blx 0x0007303c
  000b4d34: ldr.w r0,[r9,#0x64]
  000b4d38: adds r2,r0,r4
  000b4d3a: ldr r1,[r0,r4]
  000b4d3c: ldr r3,[r2,#0x4]
  000b4d3e: umull r6,r5,r1,r1
  000b4d42: mla r5,r1,r3,r5
  000b4d46: str r6,[r0,r4]
  000b4d48: mla r1,r1,r3,r5
  000b4d4c: str r1,[r2,#0x4]
  000b4d4e: ldr.w r2,[r9,#0x14]
  000b4d52: adds r0,r2,#0x1
  000b4d54: beq 0x000b4d66
  000b4d56: ldr.w r1,[r9,#0x54]
  000b4d5a: ldr.w r0,[r9,#0x2c]
  000b4d5e: ldr.w r1,[r1,r11]
  000b4d62: blx 0x000720d0
  000b4d66: adds r4,#0x8
  000b4d68: add.w r11,r11,#0x4
  000b4d6c: add.w r8,r8,#0x1
  000b4d70: ldr r0,[sp,#0x8]
  000b4d72: cmp r8,r0
  000b4d74: blt 0x000b4cf2
  000b4d76: pop.w {r1,r2,r3,r8,r9,r10,r11}
  000b4d7a: pop {r4,r5,r6,r7,pc}
```
