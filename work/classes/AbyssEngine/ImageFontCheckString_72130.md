# AbyssEngine::ImageFontCheckString
elf 0x72130 body 14
Sig: undefined __stdcall ImageFontCheckString(ImageFont * param_1, ushort * param_2, uint param_3)

## decompile
```c

/* AbyssEngine::ImageFontCheckString(AbyssEngine::ImageFont*, unsigned short const*, unsigned int)
    */

void AbyssEngine::ImageFontCheckString(ImageFont *param_1,ushort *param_2,uint param_3)

{
  ushort uVar1;
  uint uVar2;
  
  uVar1 = 0;
  do {
    uVar2 = (uint)uVar1;
    uVar1 = uVar1 + 1;
  } while (uVar2 < param_3);
  return;
}

```

## target disasm
```
  00082130: movs r0,#0x0
  00082132: adds r1,r0,#0x1
  00082134: uxth r0,r0
  00082136: cmp r0,r2
  00082138: mov r0,r1
  0008213a: bcc 0x00082132
  0008213c: bx lr
```
