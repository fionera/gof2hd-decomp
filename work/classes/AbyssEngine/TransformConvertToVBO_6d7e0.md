# AbyssEngine::TransformConvertToVBO
elf 0x6d7e0 body 56
Sig: undefined __stdcall TransformConvertToVBO(Transform * param_1)

## decompile
```c

/* AbyssEngine::TransformConvertToVBO(AbyssEngine::Transform*) */

undefined4 AbyssEngine::TransformConvertToVBO(Transform *param_1)

{
  uint uVar1;
  
  if (param_1 != (Transform *)0x0) {
    for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x3c); uVar1 = uVar1 + 1) {
      MeshConvertToVBO(*(Mesh **)(*(int *)(param_1 + 0x40) + uVar1 * 4));
    }
    for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x4c); uVar1 = uVar1 + 1) {
      TransformConvertToVBO(*(Transform **)(*(int *)(param_1 + 0x50) + uVar1 * 4));
    }
  }
  return 1;
}

```

## target disasm
```
  0007d7e0: push {r4,r5,r7,lr}
  0007d7e2: add r7,sp,#0x8
  0007d7e4: mov r4,r0
  0007d7e6: cbz r0,0x0007d814
  0007d7e8: movs r5,#0x0
  0007d7ea: b 0x0007d7f8
  0007d7ec: ldr r0,[r4,#0x40]
  0007d7ee: ldr.w r0,[r0,r5,lsl #0x2]
  0007d7f2: blx 0x0006eb0c
  0007d7f6: adds r5,#0x1
  0007d7f8: ldr r0,[r4,#0x3c]
  0007d7fa: cmp r5,r0
  0007d7fc: bcc 0x0007d7ec
  0007d7fe: movs r5,#0x0
  0007d800: b 0x0007d80e
  0007d802: ldr r0,[r4,#0x50]
  0007d804: ldr.w r0,[r0,r5,lsl #0x2]
  0007d808: blx 0x0006eb78
  0007d80c: adds r5,#0x1
  0007d80e: ldr r0,[r4,#0x4c]
  0007d810: cmp r5,r0
  0007d812: bcc 0x0007d802
  0007d814: movs r0,#0x1
  0007d816: pop {r4,r5,r7,pc}
```
