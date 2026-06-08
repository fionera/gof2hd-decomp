# AbyssEngine::ImageFontSetYOffset
elf 0x72278 body 6
Sig: undefined __stdcall ImageFontSetYOffset(ImageFont * param_1, short param_2)

## decompile
```c

/* AbyssEngine::ImageFontSetYOffset(AbyssEngine::ImageFont*, short) */

void AbyssEngine::ImageFontSetYOffset(ImageFont *param_1,short param_2)

{
  if (param_1 != (ImageFont *)0x0) {
    *(short *)(param_1 + 0x12) = param_2;
  }
  return;
}

```

## target disasm
```
  00082278: cbz r0,0x0008227c
  0008227a: strh r1,[r0,#0x12]
  0008227c: bx lr
```
