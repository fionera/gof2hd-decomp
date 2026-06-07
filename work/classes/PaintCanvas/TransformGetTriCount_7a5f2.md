# PaintCanvas::TransformGetTriCount
elf 0x7a5f2 body 84
Sig: undefined __thiscall TransformGetTriCount(PaintCanvas * this, Transform * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformGetTriCount(AbyssEngine::Transform*) */

int __thiscall AbyssEngine::PaintCanvas::TransformGetTriCount(PaintCanvas *this,Transform *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (param_1 == (Transform *)0x0) {
    iVar2 = 0;
  }
  else {
    iVar4 = *(int *)(param_1 + 0x3c);
    iVar2 = 0;
    for (iVar3 = 0; iVar4 != iVar3; iVar3 = iVar3 + 1) {
      iVar1 = MeshGetTriCount(this,*(Mesh **)(*(int *)(param_1 + 0x40) + iVar3 * 4));
      iVar2 = iVar2 + iVar1;
    }
    iVar4 = *(int *)(param_1 + 0x4c);
    for (iVar3 = 0; iVar4 != iVar3; iVar3 = iVar3 + 1) {
      iVar1 = TransformGetTriCount(this,*(Transform **)(*(int *)(param_1 + 0x50) + iVar3 * 4));
      iVar2 = iVar2 + iVar1;
    }
  }
  return iVar2;
}

```

## target disasm
```
  0008a5f2: push {r4,r5,r6,r7,lr}
  0008a5f4: add r7,sp,#0xc
  0008a5f6: push {r8,r9,r11}
  0008a5fa: mov r5,r1
  0008a5fc: mov r9,r0
  0008a5fe: cbz r1,0x0008a63c
  0008a600: ldr.w r8,[r5,#0x3c]
  0008a604: movs r4,#0x0
  0008a606: movs r6,#0x0
  0008a608: b 0x0008a61a
  0008a60a: ldr r0,[r5,#0x40]
  0008a60c: ldr.w r1,[r0,r6,lsl #0x2]
  0008a610: mov r0,r9
  0008a612: blx 0x0006fc94
  0008a616: add r4,r0
  0008a618: adds r6,#0x1
  0008a61a: cmp r8,r6
  0008a61c: bne 0x0008a60a
  0008a61e: ldr.w r8,[r5,#0x4c]
  0008a622: movs r6,#0x0
  0008a624: b 0x0008a636
  0008a626: ldr r0,[r5,#0x50]
  0008a628: ldr.w r1,[r0,r6,lsl #0x2]
  0008a62c: mov r0,r9
  0008a62e: blx 0x0006fc88
  0008a632: add r4,r0
  0008a634: adds r6,#0x1
  0008a636: cmp r8,r6
  0008a638: bne 0x0008a626
  0008a63a: b 0x0008a63e
  0008a63c: movs r4,#0x0
  0008a63e: mov r0,r4
  0008a640: pop.w {r8,r9,r11}
  0008a644: pop {r4,r5,r6,r7,pc}
```
