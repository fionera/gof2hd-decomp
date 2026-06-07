# PaintCanvas::MeshChangeMaterialIntern
elf 0x7a322 body 80
Sig: undefined __thiscall MeshChangeMaterialIntern(PaintCanvas * this, Transform * param_1, Material * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshChangeMaterialIntern(AbyssEngine::Transform*,
   AbyssEngine::Material*) */

void __thiscall
AbyssEngine::PaintCanvas::MeshChangeMaterialIntern
          (PaintCanvas *this,Transform *param_1,Material *param_2)

{
  uint uVar1;
  
  if ((param_1 != (Transform *)0x0) && (param_2 != (Material *)0x0)) {
    for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x3c); uVar1 = uVar1 + 1) {
      MeshChangeMaterialIntern(this,*(Mesh **)(*(int *)(param_1 + 0x40) + uVar1 * 4),param_2);
    }
    for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x4c); uVar1 = uVar1 + 1) {
      MeshChangeMaterialIntern(this,*(Transform **)(*(int *)(param_1 + 0x50) + uVar1 * 4),param_2);
    }
  }
  return;
}

```

## target disasm
```
  0008a322: push {r4,r5,r6,r7,lr}
  0008a324: add r7,sp,#0xc
  0008a326: push.w r8
  0008a32a: mov r8,r2
  0008a32c: mov r5,r1
  0008a32e: mov r6,r0
  0008a330: cbz r1,0x0008a36c
  0008a332: cmp.w r8,#0x0
  0008a336: beq 0x0008a36c
  0008a338: movs r4,#0x0
  0008a33a: b 0x0008a34c
  0008a33c: ldr r0,[r5,#0x40]
  0008a33e: mov r2,r8
  0008a340: ldr.w r1,[r0,r4,lsl #0x2]
  0008a344: mov r0,r6
  0008a346: blx 0x0006fc64
  0008a34a: adds r4,#0x1
  0008a34c: ldr r0,[r5,#0x3c]
  0008a34e: cmp r4,r0
  0008a350: bcc 0x0008a33c
  0008a352: movs r4,#0x0
  0008a354: b 0x0008a366
  0008a356: ldr r0,[r5,#0x50]
  0008a358: mov r2,r8
  0008a35a: ldr.w r1,[r0,r4,lsl #0x2]
  0008a35e: mov r0,r6
  0008a360: blx 0x0006fc70
  0008a364: adds r4,#0x1
  0008a366: ldr r0,[r5,#0x4c]
  0008a368: cmp r4,r0
  0008a36a: bcc 0x0008a356
  0008a36c: pop.w r8
  0008a370: pop {r4,r5,r6,r7,pc}
```
