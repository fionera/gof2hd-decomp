# AbyssEngine::TransformRelease
elf 0x6c314 body 78
Sig: undefined __stdcall TransformRelease(Engine * param_1, Transform * * param_2)

## decompile
```c

/* AbyssEngine::TransformRelease(AbyssEngine::Engine*, AbyssEngine::Transform**) */

void AbyssEngine::TransformRelease(Engine *param_1,Transform **param_2)

{
  Transform *pTVar1;
  uint uVar2;
  int iVar3;
  
  pTVar1 = *param_2;
  if (pTVar1 != (Transform *)0x0) {
    iVar3 = 0;
    for (uVar2 = 0; uVar2 < *(uint *)(pTVar1 + 0x4c); uVar2 = uVar2 + 1) {
      TransformRelease(param_1,(Transform **)(*(int *)(pTVar1 + 0x50) + iVar3));
      iVar3 = iVar3 + 4;
      pTVar1 = *param_2;
    }
    iVar3 = 0;
    for (uVar2 = 0; uVar2 < *(uint *)(pTVar1 + 0x3c); uVar2 = uVar2 + 1) {
      MeshRelease(param_1,(Mesh **)(*(int *)(pTVar1 + 0x40) + iVar3));
      iVar3 = iVar3 + 4;
      pTVar1 = *param_2;
    }
  }
  return;
}

```

## target disasm
```
  0007c314: push {r4,r5,r6,r7,lr}
  0007c316: add r7,sp,#0xc
  0007c318: push.w r8
  0007c31c: mov r8,r0
  0007c31e: ldr r0,[r1,#0x0]
  0007c320: mov r4,r1
  0007c322: cbz r0,0x0007c35c
  0007c324: movs r6,#0x0
  0007c326: movs r5,#0x0
  0007c328: b 0x0007c33a
  0007c32a: ldr r0,[r0,#0x50]
  0007c32c: adds r1,r0,r6
  0007c32e: mov r0,r8
  0007c330: blx 0x0006ebcc
  0007c334: adds r6,#0x4
  0007c336: ldr r0,[r4,#0x0]
  0007c338: adds r5,#0x1
  0007c33a: ldr r1,[r0,#0x4c]
  0007c33c: cmp r5,r1
  0007c33e: bcc 0x0007c32a
  0007c340: movs r6,#0x0
  0007c342: movs r5,#0x0
  0007c344: b 0x0007c356
  0007c346: ldr r0,[r0,#0x40]
  0007c348: adds r1,r0,r6
  0007c34a: mov r0,r8
  0007c34c: blx 0x0006ebd8
  0007c350: adds r6,#0x4
  0007c352: ldr r0,[r4,#0x0]
  0007c354: adds r5,#0x1
  0007c356: ldr r1,[r0,#0x3c]
  0007c358: cmp r5,r1
  0007c35a: bcc 0x0007c346
  0007c35c: pop.w r8
  0007c360: pop {r4,r5,r6,r7,pc}
```
