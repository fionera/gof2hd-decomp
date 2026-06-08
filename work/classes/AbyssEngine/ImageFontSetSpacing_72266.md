# AbyssEngine::ImageFontSetSpacing
elf 0x72266 body 6
Sig: undefined __stdcall ImageFontSetSpacing(ImageFont * param_1, short param_2)

## decompile
```c

/* AbyssEngine::ImageFontSetSpacing(AbyssEngine::ImageFont*, short) */

void AbyssEngine::ImageFontSetSpacing(ImageFont *param_1,short param_2)

{
  if (param_1 != (ImageFont *)0x0) {
    *(short *)(param_1 + 0x10) = param_2;
  }
  return;
}

```

## target disasm
```
  00082266: cbz r0,0x0008226a
  00082268: strh r1,[r0,#0x10]
  0008226a: bx lr
```
