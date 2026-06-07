# AEGeometry::setLodChildMeshes
elf 0xa4a98 body 164
Sig: undefined __thiscall setLodChildMeshes(AEGeometry * this, ushort * param_1)

## decompile
```c

/* AEGeometry::setLodChildMeshes(unsigned short*) */

void __thiscall AEGeometry::setLodChildMeshes(AEGeometry *this,ushort *param_1)

{
  uint uVar1;
  void *pvVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  uVar5 = *(uint *)(this + 0x50);
  if (0 < (int)uVar5) {
    uVar1 = uVar5 * 2;
    if (CARRY4(uVar5,uVar5)) {
      uVar1 = 0xffffffff;
    }
    pvVar2 = operator_new__(uVar1);
    *(void **)(this + 0x60) = pvVar2;
    uVar1 = (uint)((ulonglong)uVar5 * 4);
    if ((int)((ulonglong)uVar5 * 4 >> 0x20) != 0) {
      uVar1 = 0xffffffff;
    }
    pvVar2 = operator_new__(uVar1);
    iVar4 = 0;
    *(void **)(this + 0x58) = pvVar2;
    for (iVar3 = 0; iVar3 < (int)uVar5; iVar3 = iVar3 + 1) {
      *(ushort *)(*(int *)(this + 0x60) + iVar3 * 2) = param_1[iVar3];
      AbyssEngine::PaintCanvas::TransformCreate
                (*(PaintCanvas **)(this + 0x2c),(uint *)(*(int *)(this + 0x58) + iVar4));
      AbyssEngine::PaintCanvas::TransformAddMesh
                (*(uint *)(this + 0x2c),(ushort)*(undefined4 *)(*(int *)(this + 0x58) + iVar4),
                 SUB21(param_1[iVar3],0));
      AbyssEngine::PaintCanvas::TransformAddChild
                (*(PaintCanvas **)(this + 0x2c),*(uint *)(*(int *)(this + 0x54) + iVar4),
                 *(uint *)(*(int *)(this + 0x58) + iVar4));
      AbyssEngine::PaintCanvas::TransformRemoveChild
                (*(PaintCanvas **)(this + 0x2c),*(uint *)(*(int *)(this + 0x54) + iVar4),
                 *(uint *)(this + 0x14));
      AbyssEngine::PaintCanvas::TransformAddChild
                (*(PaintCanvas **)(this + 0x2c),*(uint *)(*(int *)(this + 0x54) + iVar4),
                 *(uint *)(this + 0x14));
      iVar4 = iVar4 + 4;
      uVar5 = *(uint *)(this + 0x50);
    }
  }
  return;
}

```

## target disasm
```
  000b4a98: push {r4,r5,r6,r7,lr}
  000b4a9a: add r7,sp,#0xc
  000b4a9c: push {r8,r9,r11}
  000b4aa0: ldr.w r9,[r0,#0x50]
  000b4aa4: mov r8,r1
  000b4aa6: mov r5,r0
  000b4aa8: cmp.w r9,#0x1
  000b4aac: blt 0x000b4b36
  000b4aae: adds.w r0,r9,r9
  000b4ab2: it cs
  000b4ab4: mov.cs.w r0,#0xffffffff
  000b4ab8: blx 0x0006ec08
  000b4abc: movs r1,#0x4
  000b4abe: str r0,[r5,#0x60]
  000b4ac0: umull r1,r2,r9,r1
  000b4ac4: cmp r2,#0x0
  000b4ac6: it ne
  000b4ac8: mov.ne r2,#0x1
  000b4aca: cmp r2,#0x0
  000b4acc: it ne
  000b4ace: mov.ne.w r1,#0xffffffff
  000b4ad2: mov r0,r1
  000b4ad4: blx 0x0006ec08
  000b4ad8: movs r6,#0x0
  000b4ada: movs r4,#0x0
  000b4adc: str r0,[r5,#0x58]
  000b4ade: b 0x000b4b32
  000b4ae0: ldr r0,[r5,#0x60]
  000b4ae2: ldrh.w r1,[r8,r4,lsl #0x1]
  000b4ae6: strh.w r1,[r0,r4,lsl #0x1]
  000b4aea: ldr r1,[r5,#0x58]
  000b4aec: ldr r0,[r5,#0x2c]
  000b4aee: add r1,r6
  000b4af0: blx 0x000720ac
  000b4af4: ldr r1,[r5,#0x58]
  000b4af6: movs r3,#0x0
  000b4af8: ldrh.w r2,[r8,r4,lsl #0x1]
  000b4afc: ldr r0,[r5,#0x2c]
  000b4afe: ldr r1,[r1,r6]
  000b4b00: blx 0x0007303c
  000b4b04: ldr r0,[r5,#0x2c]
  000b4b06: ldrd r1,r2,[r5,#0x54]
  000b4b0a: ldr r2,[r2,r6]
  000b4b0c: ldr r1,[r1,r6]
  000b4b0e: blx 0x000720d0
  000b4b12: ldr r1,[r5,#0x54]
  000b4b14: ldr r2,[r5,#0x14]
  000b4b16: ldr r0,[r5,#0x2c]
  000b4b18: ldr r1,[r1,r6]
  000b4b1a: blx 0x0007210c
  000b4b1e: ldr r1,[r5,#0x54]
  000b4b20: ldr r2,[r5,#0x14]
  000b4b22: ldr r0,[r5,#0x2c]
  000b4b24: ldr r1,[r1,r6]
  000b4b26: blx 0x000720d0
  000b4b2a: adds r6,#0x4
  000b4b2c: ldr.w r9,[r5,#0x50]
  000b4b30: adds r4,#0x1
  000b4b32: cmp r4,r9
  000b4b34: blt 0x000b4ae0
  000b4b36: pop.w {r8,r9,r11}
  000b4b3a: pop {r4,r5,r6,r7,pc}
```
