# PaintCanvas::TransformAddMeshId
elf 0x7b1c6 body 194
Sig: undefined __thiscall TransformAddMeshId(PaintCanvas * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformAddMeshId(unsigned int, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::TransformAddMeshId(PaintCanvas *this,uint param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  bool bVar7;
  bool bVar8;
  
  bVar8 = param_1 <= *(uint *)(this + 0x158);
  bVar7 = *(uint *)(this + 0x158) == param_1;
  if (bVar8 && !bVar7) {
    bVar8 = param_2 <= *(uint *)(this + 0x24);
    bVar7 = *(uint *)(this + 0x24) == param_2;
  }
  if (!bVar8 || bVar7) {
    return;
  }
  ArrayAdd<AbyssEngine::Mesh*>
            (*(Mesh **)(*(int *)(this + 0x28) + param_2 * 4),
             (Array *)(*(int *)(*(int *)(this + 0x15c) + param_1 * 4) + 0x3c));
  AEMath::BSphere::Merge
            ((BSphere *)(*(int *)(*(int *)(this + 0x15c) + param_1 * 4) + 0xd4),
             (BSphere *)(*(int *)(*(int *)(this + 0x28) + param_2 * 4) + 0x3c));
  iVar2 = *(int *)(*(int *)(*(int *)(this + 0x28) + param_2 * 4) + 0x34);
  if (iVar2 != 0) {
    iVar3 = *(int *)(iVar2 + 0xfc);
    iVar1 = *(int *)(*(int *)(this + 0x15c) + param_1 * 4);
    iVar5 = *(int *)(iVar1 + 0xfc);
    bVar7 = *(uint *)(iVar1 + 0xf8) < *(uint *)(iVar2 + 0xf8);
    iVar2 = (iVar5 - iVar3) - (uint)bVar7;
    if (iVar2 < 0 != (SBORROW4(iVar5,iVar3) != SBORROW4(iVar5 - iVar3,(uint)bVar7))) {
      Transform::SetAnimationLength(CONCAT44(iVar2,iVar1));
      iVar1 = *(int *)(*(int *)(this + 0x15c) + param_1 * 4);
    }
    uVar4 = *(uint *)(iVar1 + 0x100);
    iVar3 = *(int *)(iVar1 + 0x104);
    iVar2 = *(int *)(*(int *)(*(int *)(this + 0x28) + param_2 * 4) + 0x34);
    uVar6 = *(uint *)(iVar2 + 0x100);
    iVar2 = *(int *)(iVar2 + 0x104);
    if ((uVar4 == 0 && iVar3 == 0) ||
       ((int)((iVar2 - iVar3) - (uint)(uVar6 < uVar4)) < 0 !=
        (SBORROW4(iVar2,iVar3) != SBORROW4(iVar2 - iVar3,(uint)(uVar6 < uVar4))))) {
      *(uint *)(iVar1 + 0x100) = uVar6;
      *(int *)(iVar1 + 0x104) = iVar2;
    }
    Transform::SetAnimationState(iVar1,2,0);
  }
  (*(code *)(DAT_001ab2b4 + 0x1ab2b8))(*(undefined4 *)(*(int *)(this + 0x15c) + param_1 * 4));
  return;
}

```

## target disasm
```
  0008b1c6: push {r4,r5,r6,r7,lr}
  0008b1c8: add r7,sp,#0xc
  0008b1ca: push.w r11
  0008b1ce: mov r5,r0
  0008b1d0: ldr.w r0,[r0,#0x158]
  0008b1d4: mov r6,r2
  0008b1d6: mov r4,r1
  0008b1d8: cmp r0,r1
  0008b1da: itt hi
  0008b1dc: ldr.hi r0,[r5,#0x24]
  0008b1de: cmp.hi r0,r6
  0008b1e0: bhi 0x0008b1e8
  0008b1e2: pop.w r11
  0008b1e6: pop {r4,r5,r6,r7,pc}
  0008b1e8: ldr.w r1,[r5,#0x15c]
  0008b1ec: ldr r0,[r5,#0x28]
  0008b1ee: ldr.w r1,[r1,r4,lsl #0x2]
  0008b1f2: ldr.w r0,[r0,r6,lsl #0x2]
  0008b1f6: adds r1,#0x3c
  0008b1f8: blx 0x0006ecbc
  0008b1fc: ldr.w r1,[r5,#0x15c]
  0008b200: ldr r0,[r5,#0x28]
  0008b202: ldr.w r1,[r1,r4,lsl #0x2]
  0008b206: ldr.w r2,[r0,r6,lsl #0x2]
  0008b20a: add.w r0,r1,#0xd4
  0008b20e: add.w r1,r2,#0x3c
  0008b212: blx 0x0006eca4
  0008b216: ldr r0,[r5,#0x28]
  0008b218: ldr.w r0,[r0,r6,lsl #0x2]
  0008b21c: ldr r1,[r0,#0x34]
  0008b21e: cbz r1,0x0008b274
  0008b220: ldr.w r0,[r5,#0x15c]
  0008b224: ldrd r2,r3,[r1,#0xf8]
  0008b228: ldr.w r0,[r0,r4,lsl #0x2]
  0008b22c: ldrd r1,r12,[r0,#0xf8]
  0008b230: subs r1,r1,r2
  0008b232: sbcs.w r1,r12,r3
  0008b236: bge 0x0008b244
  0008b238: blx 0x0006fd0c
  0008b23c: ldr.w r0,[r5,#0x15c]
  0008b240: ldr.w r0,[r0,r4,lsl #0x2]
  0008b244: ldr r1,[r5,#0x28]
  0008b246: ldr.w r1,[r1,r6,lsl #0x2]
  0008b24a: ldrd r6,r3,[r0,#0x100]
  0008b24e: ldr r1,[r1,#0x34]
  0008b250: ldrd r12,r2,[r1,#0x100]
  0008b254: orrs.w r1,r6,r3
  0008b258: beq 0x0008b264
  0008b25a: subs.w r1,r12,r6
  0008b25e: sbcs.w r1,r2,r3
  0008b262: bge 0x0008b26c
  0008b264: add.w r1,r0,#0x100
  0008b268: strd r12,r2,[r1,#0x0]
  0008b26c: movs r1,#0x2
  0008b26e: movs r2,#0x0
  0008b270: blx 0x0006fd18
  0008b274: ldr.w r0,[r5,#0x15c]
  0008b278: ldr.w r0,[r0,r4,lsl #0x2]
  0008b27c: pop.w r11
  0008b280: pop.w {r4,r5,r6,r7,lr}
  0008b284: b.w 0x001ab2a8
```
