# AbyssEngine::ImageFontRelease
elf 0x7227e body 106
Sig: undefined __stdcall ImageFontRelease(Engine * param_1, ImageFont * * param_2)

## decompile
```c

/* AbyssEngine::ImageFontRelease(AbyssEngine::Engine*, AbyssEngine::ImageFont**) */

void AbyssEngine::ImageFontRelease(Engine *param_1,ImageFont **param_2)

{
  void *pvVar1;
  ImageFont *pIVar2;
  uint uVar3;
  int iVar4;
  
  if (*param_2 != (ImageFont *)0x0) {
    pIVar2 = *param_2 + 4;
    if (*(void **)pIVar2 != (void *)0x0) {
      operator_delete__(*(void **)pIVar2);
      pIVar2 = *param_2 + 4;
    }
    iVar4 = 0;
    uVar3 = 0;
    *(undefined4 *)pIVar2 = 0;
    while( true ) {
      pIVar2 = *param_2;
      pvVar1 = *(void **)(pIVar2 + 0xc);
      if (*(ushort *)pIVar2 <= uVar3) break;
      MeshRelease(param_1,(Mesh **)((int)pvVar1 + iVar4));
      iVar4 = iVar4 + 4;
      uVar3 = uVar3 + 1;
    }
    if (pvVar1 != (void *)0x0) {
      operator_delete__(pvVar1);
      pIVar2 = *param_2;
    }
    *(undefined4 *)(pIVar2 + 0xc) = 0;
    if (*param_2 != (ImageFont *)0x0) {
      operator_delete(*param_2);
    }
    *param_2 = (ImageFont *)0x0;
  }
  return;
}

```

## target disasm
```
  0008227e: push {r4,r5,r6,r7,lr}
  00082280: add r7,sp,#0xc
  00082282: push.w r8
  00082286: mov r8,r1
  00082288: ldr r1,[r1,#0x0]
  0008228a: mov r5,r0
  0008228c: cbz r1,0x000822e2
  0008228e: ldr.w r0,[r1,#0x4]!
  00082292: cbz r0,0x0008229e
  00082294: blx 0x0006ebfc
  00082298: ldr.w r0,[r8,#0x0]
  0008229c: adds r1,r0,#0x4
  0008229e: movs r6,#0x0
  000822a0: movs r4,#0x0
  000822a2: str r6,[r1,#0x0]
  000822a4: b 0x000822b2
  000822a6: adds r1,r0,r6
  000822a8: mov r0,r5
  000822aa: blx 0x0006ebd8
  000822ae: adds r6,#0x4
  000822b0: adds r4,#0x1
  000822b2: ldr.w r1,[r8,#0x0]
  000822b6: ldrh r2,[r1,#0x0]
  000822b8: ldr r0,[r1,#0xc]
  000822ba: cmp r4,r2
  000822bc: bcc 0x000822a6
  000822be: cbz r0,0x000822cc
  000822c0: blx 0x0006ebfc
  000822c4: ldr.w r0,[r8,#0x0]
  000822c8: adds r0,#0xc
  000822ca: b 0x000822d0
  000822cc: add.w r0,r1,#0xc
  000822d0: movs r4,#0x0
  000822d2: str r4,[r0,#0x0]
  000822d4: ldr.w r0,[r8,#0x0]
  000822d8: cbz r0,0x000822de
  000822da: blx 0x0006eb48
  000822de: str.w r4,[r8,#0x0]
  000822e2: pop.w r8
  000822e6: pop {r4,r5,r6,r7,pc}
```
