# AbyssEngine::ImageFontGetSpacing
elf 0x7226c body 12
Sig: undefined __stdcall ImageFontGetSpacing(ImageFont * param_1)

## decompile
```c

/* AbyssEngine::ImageFontGetSpacing(AbyssEngine::ImageFont*) */

int AbyssEngine::ImageFontGetSpacing(ImageFont *param_1)

{
  short sVar1;
  
  if (param_1 == (ImageFont *)0x0) {
    sVar1 = 0;
  }
  else {
    sVar1 = *(short *)(param_1 + 0x10);
  }
  return (int)sVar1;
}

```

## target disasm
```
  0008226c: cmp r0,#0x0
  0008226e: ite ne
  00082270: ldrh.ne r0,[r0,#0x10]
  00082272: mov.eq r0,#0x0
  00082274: sxth r0,r0
  00082276: bx lr
```
