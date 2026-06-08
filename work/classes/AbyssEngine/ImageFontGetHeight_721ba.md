# AbyssEngine::ImageFontGetHeight
elf 0x721ba body 32
Sig: undefined __stdcall ImageFontGetHeight(ImageFont * param_1)

## decompile
```c

/* AbyssEngine::ImageFontGetHeight(AbyssEngine::ImageFont*) */

int AbyssEngine::ImageFontGetHeight(ImageFont *param_1)

{
  int iVar1;
  
  if (param_1 != (ImageFont *)0x0) {
    iVar1 = (int)*(float *)(*(int *)(**(int **)(param_1 + 0xc) + 4) + 0x1c);
    if (iVar1 == 0x18) {
      iVar1 = 0x13;
    }
    return iVar1;
  }
  return 0;
}

```

## target disasm
```
  000821ba: cbz r0,0x000821d6
  000821bc: ldr r0,[r0,#0xc]
  000821be: ldr r0,[r0,#0x0]
  000821c0: ldr r0,[r0,#0x4]
  000821c2: vldr.32 s0,[r0,#0x1c]
  000821c6: vcvt.s32.f32 s0,s0
  000821ca: vmov r0,s0
  000821ce: cmp r0,#0x18
  000821d0: it eq
  000821d2: sub.eq r0,#0x5
  000821d4: bx lr
  000821d6: movs r0,#0x0
  000821d8: bx lr
```
