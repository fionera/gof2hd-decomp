# MeshMerger::init
elf 0x173ca6 body 234
Sig: int __thiscall init(MeshMerger * this, EVP_PKEY_CTX * ctx)

## decompile
```c

/* MeshMerger::init() */

int __thiscall MeshMerger::init(MeshMerger *this,EVP_PKEY_CTX *ctx)

{
  undefined2 uVar1;
  short sVar2;
  short sVar3;
  Mesh *pMVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 extraout_r1;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  short sVar11;
  short sVar12;
  bool bVar13;
  bool bVar14;
  
  if ((byte)this[6] != 0) {
    return (uint)(byte)this[6];
  }
  for (iVar10 = 0; iVar9 = *(int *)this, iVar10 < iVar9; iVar10 = iVar10 + 1) {
    iVar8 = 0;
    iVar6 = (int)*(char *)(*(int *)(this + 0x24) + iVar10);
    bVar14 = SBORROW4(iVar6,-1);
    iVar9 = iVar6 + 1;
    bVar13 = iVar6 == -1;
    if (-2 < iVar6) {
      iVar7 = *(int *)(this + 0x30);
      bVar14 = SBORROW4(iVar7,iVar6);
      iVar9 = iVar7 - iVar6;
      bVar13 = iVar7 == iVar6;
    }
    if (bVar13 || iVar9 < 0 != bVar14) {
      *(undefined1 *)(*(int *)(this + 0x24) + iVar10) = 0;
    }
    for (; iVar8 < *(int *)(this + 0x30); iVar8 = iVar8 + 1) {
      pMVar4 = *(Mesh **)(*(int *)(this + 8) + (*(int *)this * iVar8 + iVar10) * 4);
      if (pMVar4 != (Mesh *)0x0) {
        uVar5 = transformMesh(pMVar4,(Matrix *)(*(int *)(this + 0x1c) + iVar10 * 0x3c));
        *(undefined4 *)(*(int *)(this + 0x18) + (*(int *)this * iVar8 + iVar10) * 4) = uVar5;
      }
    }
  }
  sVar11 = 0;
  sVar12 = 0;
  for (iVar10 = 0; iVar10 < iVar9; iVar10 = iVar10 + 1) {
    iVar6 = *(int *)(*(int *)(this + 8) + iVar10 * 4);
    sVar2 = *(short *)(iVar6 + 2);
    sVar3 = __aeabi_uidiv(*(undefined2 *)(iVar6 + 0x28),3);
    sVar12 = sVar12 + sVar3;
    sVar11 = sVar11 + sVar2;
  }
  uVar1 = *(undefined2 *)(this + 4);
  AbyssEngine::PaintCanvas::MeshCreate
            (*(PaintCanvas **)(this + 0xc),sVar11,sVar12,(int)*(char *)**(undefined4 **)(this + 8));
  uVar5 = AbyssEngine::PaintCanvas::MeshGetPointer
                    (*(PaintCanvas **)(this + 0xc),*(uint *)(this + 0x10));
  *(undefined4 *)(this + 0x20) = uVar5;
  AbyssEngine::PaintCanvas::TransformCreate(*(PaintCanvas **)(this + 0xc),(uint *)(this + 0x14));
  AbyssEngine::PaintCanvas::TransformAddMeshId
            (*(PaintCanvas **)(this + 0xc),*(uint *)(this + 0x14),*(uint *)(this + 0x10));
  this[0x34] = (MeshMerger)0x1;
  iVar10 = (*(code *)(DAT_001ac6d4 + 0x1ac6d8))(this,extraout_r1,uVar1,this + 0x10);
  return iVar10;
}

```

## target disasm
```
  00183ca6: push {r4,r5,r6,r7,lr}
  00183ca8: add r7,sp,#0xc
  00183caa: push {r6,r7,r8,r9,r10}
  00183cae: mov r4,r0
  00183cb0: ldrb r0,[r0,#0x6]
  00183cb2: cbz r0,0x00183cba
  00183cb4: pop.w {r2,r3,r8,r9,r10}
  00183cb8: pop {r4,r5,r6,r7,pc}
  00183cba: mov.w r8,#0x0
  00183cbe: movs r6,#0x0
  00183cc0: b 0x00183d0c
  00183cc2: ldr r0,[r4,#0x24]
  00183cc4: movs r5,#0x0
  00183cc6: ldrsb r1,[r0,r6]
  00183cc8: cmp.w r1,#0xffffffff
  00183ccc: itt ge
  00183cce: ldr.ge r2,[r4,#0x30]
  00183cd0: cmp.ge r2,r1
  00183cd2: it le
  00183cd4: strb.le.w r8,[r0,r6]
  00183cd8: b 0x00183d04
  00183cda: ldr r0,[r4,#0x0]
  00183cdc: ldr r1,[r4,#0x8]
  00183cde: mla r0,r0,r5,r6
  00183ce2: ldr.w r0,[r1,r0,lsl #0x2]
  00183ce6: cbz r0,0x00183d02
  00183ce8: ldr r1,[r4,#0x1c]
  00183cea: rsb r2,r6,r6, lsl #0x4
  00183cee: add.w r1,r1,r2, lsl #0x2
  00183cf2: blx 0x00077e00
  00183cf6: ldr r1,[r4,#0x0]
  00183cf8: ldr r2,[r4,#0x18]
  00183cfa: mla r1,r1,r5,r6
  00183cfe: str.w r0,[r2,r1,lsl #0x2]
  00183d02: adds r5,#0x1
  00183d04: ldr r0,[r4,#0x30]
  00183d06: cmp r5,r0
  00183d08: blt 0x00183cda
  00183d0a: adds r6,#0x1
  00183d0c: ldr r5,[r4,#0x0]
  00183d0e: cmp r6,r5
  00183d10: blt 0x00183cc2
  00183d12: mov.w r8,#0x0
  00183d16: mov.w r9,#0x0
  00183d1a: movs r6,#0x0
  00183d1c: b 0x00183d36
  00183d1e: ldr r0,[r4,#0x8]
  00183d20: ldr.w r1,[r0,r6,lsl #0x2]
  00183d24: ldrh r0,[r1,#0x28]
  00183d26: ldrh.w r10,[r1,#0x2]
  00183d2a: movs r1,#0x3
  00183d2c: blx 0x0006ec2c
  00183d30: add r9,r0
  00183d32: add r8,r10
  00183d34: adds r6,#0x1
  00183d36: cmp r6,r5
  00183d38: blt 0x00183d1e
  00183d3a: ldrd r1,r0,[r4,#0x8]
  00183d3e: ldrh r2,[r4,#0x4]
  00183d40: ldr r1,[r1,#0x0]
  00183d42: ldrsb.w r3,[r1,#0x0]
  00183d46: add.w r1,r4,#0x10
  00183d4a: strd r2,r1,[sp,#0x0]
  00183d4e: uxth.w r1,r8
  00183d52: uxth.w r2,r9
  00183d56: blx 0x00075da8
  00183d5a: ldrd r0,r1,[r4,#0xc]
  00183d5e: blx 0x00072370
  00183d62: ldr r2,[r4,#0xc]
  00183d64: add.w r1,r4,#0x14
  00183d68: str r0,[r4,#0x20]
  00183d6a: mov r0,r2
  00183d6c: blx 0x000720ac
  00183d70: ldrd r0,r2,[r4,#0xc]
  00183d74: ldr r1,[r4,#0x14]
  00183d76: blx 0x00073030
  00183d7a: movs r0,#0x1
  00183d7c: strb.w r0,[r4,#0x34]
  00183d80: mov r0,r4
  00183d82: pop.w {r2,r3,r8,r9,r10}
  00183d86: pop.w {r4,r5,r6,r7,lr}
  00183d8a: b.w 0x001ac6c8
  001ac6c8: bx pc
```
