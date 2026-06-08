# AbyssEngine::ImageRelease
elf 0x6ee94 body 34
Sig: undefined __stdcall ImageRelease(Image * * param_1)

## decompile
```c

/* AbyssEngine::ImageRelease(AbyssEngine::Image**) */

void AbyssEngine::ImageRelease(Image **param_1)

{
  if (*param_1 != (Image *)0x0) {
    operator_delete__(*(void **)(*param_1 + 0xc));
    *(undefined4 *)(*param_1 + 0xc) = 0;
    if (*param_1 != (Image *)0x0) {
      operator_delete(*param_1);
    }
    *param_1 = (Image *)0x0;
  }
  return;
}

```

## target disasm
```
  0007ee94: push {r4,r5,r7,lr}
  0007ee96: add r7,sp,#0x8
  0007ee98: mov r4,r0
  0007ee9a: ldr r0,[r0,#0x0]
  0007ee9c: cbz r0,0x0007eeb4
  0007ee9e: ldr r0,[r0,#0xc]
  0007eea0: blx 0x0006ebfc
  0007eea4: ldr r0,[r4,#0x0]
  0007eea6: movs r5,#0x0
  0007eea8: str r5,[r0,#0xc]
  0007eeaa: ldr r0,[r4,#0x0]
  0007eeac: cbz r0,0x0007eeb2
  0007eeae: blx 0x0006eb48
  0007eeb2: str r5,[r4,#0x0]
  0007eeb4: pop {r4,r5,r7,pc}
```
