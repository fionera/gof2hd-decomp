# AbyssEngine::SpriteSystemRelease
elf 0x87064 body 84
Sig: undefined __stdcall SpriteSystemRelease(Engine * param_1, SpriteSystem * * param_2)

## decompile
```c

/* AbyssEngine::SpriteSystemRelease(AbyssEngine::Engine*, AbyssEngine::SpriteSystem**) */

void AbyssEngine::SpriteSystemRelease(Engine *param_1,SpriteSystem **param_2)

{
  SpriteSystem *pSVar1;
  
  if (*param_2 != (SpriteSystem *)0x0) {
    pSVar1 = *param_2 + 4;
    if (*(void **)pSVar1 != (void *)0x0) {
      operator_delete__(*(void **)pSVar1);
      pSVar1 = *param_2 + 4;
    }
    *(undefined4 *)pSVar1 = 0;
    pSVar1 = *param_2 + 8;
    if (*(void **)pSVar1 != (void *)0x0) {
      operator_delete__(*(void **)pSVar1);
      pSVar1 = *param_2 + 8;
    }
    *(undefined4 *)pSVar1 = 0;
    MeshRelease(param_1,(Mesh **)(*param_2 + 0x10));
    if (*param_2 != (SpriteSystem *)0x0) {
      operator_delete(*param_2);
    }
    *param_2 = (SpriteSystem *)0x0;
  }
  return;
}

```

## target disasm
```
  00097064: push {r4,r5,r6,r7,lr}
  00097066: add r7,sp,#0xc
  00097068: push.w r11
  0009706c: mov r4,r1
  0009706e: ldr r1,[r1,#0x0]
  00097070: mov r5,r0
  00097072: cbz r1,0x000970b2
  00097074: ldr.w r0,[r1,#0x4]!
  00097078: cbz r0,0x00097082
  0009707a: blx 0x0006ebfc
  0009707e: ldr r0,[r4,#0x0]
  00097080: adds r1,r0,#0x4
  00097082: movs r6,#0x0
  00097084: str r6,[r1,#0x0]
  00097086: ldr r1,[r4,#0x0]
  00097088: ldr.w r0,[r1,#0x8]!
  0009708c: cbz r0,0x00097098
  0009708e: blx 0x0006ebfc
  00097092: ldr r0,[r4,#0x0]
  00097094: add.w r1,r0,#0x8
  00097098: str r6,[r1,#0x0]
  0009709a: ldr r0,[r4,#0x0]
  0009709c: add.w r1,r0,#0x10
  000970a0: mov r0,r5
  000970a2: blx 0x0006ebd8
  000970a6: ldr r0,[r4,#0x0]
  000970a8: cbz r0,0x000970ae
  000970aa: blx 0x0006eb48
  000970ae: movs r0,#0x0
  000970b0: str r0,[r4,#0x0]
  000970b2: pop.w r11
  000970b6: pop {r4,r5,r6,r7,pc}
```
