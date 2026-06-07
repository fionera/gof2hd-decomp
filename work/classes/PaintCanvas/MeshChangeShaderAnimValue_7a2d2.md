# PaintCanvas::MeshChangeShaderAnimValue
elf 0x7a2d2 body 80
Sig: undefined __thiscall MeshChangeShaderAnimValue(PaintCanvas * this, Transform * param_1, float param_2, uint param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(AbyssEngine::Transform*, float, unsigned int)
    */

void __thiscall
AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
          (PaintCanvas *this,Transform *param_1,float param_2,uint param_3)

{
  uint uVar1;
  
  if (param_1 != (Transform *)0x0) {
    for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x3c); uVar1 = uVar1 + 1) {
      param_2 = (float)MeshChangeShaderAnimValue
                                 (this,*(Mesh **)(*(int *)(param_1 + 0x40) + uVar1 * 4),param_2,
                                  param_3);
    }
    for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x4c); uVar1 = uVar1 + 1) {
      param_2 = (float)MeshChangeShaderAnimValue
                                 (this,*(Transform **)(*(int *)(param_1 + 0x50) + uVar1 * 4),param_2
                                  ,param_3);
    }
  }
  return;
}

```

## target disasm
```
  0008a2d2: push {r4,r5,r6,r7,lr}
  0008a2d4: add r7,sp,#0xc
  0008a2d6: push {r8,r9,r11}
  0008a2da: mov r8,r3
  0008a2dc: mov r9,r2
  0008a2de: mov r6,r1
  0008a2e0: mov r4,r0
  0008a2e2: cbz r1,0x0008a31c
  0008a2e4: movs r5,#0x0
  0008a2e6: b 0x0008a2fa
  0008a2e8: ldr r0,[r6,#0x40]
  0008a2ea: mov r2,r9
  0008a2ec: mov r3,r8
  0008a2ee: ldr.w r1,[r0,r5,lsl #0x2]
  0008a2f2: mov r0,r4
  0008a2f4: blx 0x0006fc58
  0008a2f8: adds r5,#0x1
  0008a2fa: ldr r0,[r6,#0x3c]
  0008a2fc: cmp r5,r0
  0008a2fe: bcc 0x0008a2e8
  0008a300: movs r5,#0x0
  0008a302: b 0x0008a316
  0008a304: ldr r0,[r6,#0x50]
  0008a306: mov r2,r9
  0008a308: mov r3,r8
  0008a30a: ldr.w r1,[r0,r5,lsl #0x2]
  0008a30e: mov r0,r4
  0008a310: blx 0x0006fc4c
  0008a314: adds r5,#0x1
  0008a316: ldr r0,[r6,#0x4c]
  0008a318: cmp r5,r0
  0008a31a: bcc 0x0008a304
  0008a31c: pop.w {r8,r9,r11}
  0008a320: pop {r4,r5,r6,r7,pc}
```
