# AbyssEngine::ImageFontGetYOffset
elf 0x721da body 12
Sig: undefined __stdcall ImageFontGetYOffset(ImageFont * param_1)

## decompile
```c

/* AbyssEngine::ImageFontGetYOffset(AbyssEngine::ImageFont*) */

int AbyssEngine::ImageFontGetYOffset(ImageFont *param_1)

{
  short sVar1;
  
  if (param_1 == (ImageFont *)0x0) {
    sVar1 = 0;
  }
  else {
    sVar1 = *(short *)(param_1 + 0x12);
  }
  return (int)sVar1;
}

```

## target disasm
```
  000821da: cmp r0,#0x0
  000821dc: ite ne
  000821de: ldrh.ne r0,[r0,#0x12]
  000821e0: mov.eq r0,#0x0
  000821e2: sxth r0,r0
  000821e4: bx lr
```
