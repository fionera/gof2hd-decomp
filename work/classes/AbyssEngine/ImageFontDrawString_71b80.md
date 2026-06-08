# AbyssEngine::ImageFontDrawString
elf 0x71b80 body 58
Sig: undefined __stdcall ImageFontDrawString(ImageFont * param_1, ushort * param_2, int param_3, int param_4, PaintCanvas * param_5, Engine * param_6, bool param_7)

## decompile
```c

/* AbyssEngine::ImageFontDrawString(AbyssEngine::ImageFont*, unsigned short const*, int, int,
   AbyssEngine::PaintCanvas*, AbyssEngine::Engine*, bool) */

void AbyssEngine::ImageFontDrawString
               (ImageFont *param_1,ushort *param_2,int param_3,int param_4,PaintCanvas *param_5,
               Engine *param_6,bool param_7)

{
  uint uVar1;
  ushort uVar2;
  
  if ((param_1 != (ImageFont *)0x0) && (param_2 != (ushort *)0x0)) {
    uVar2 = 0;
    do {
      uVar1 = (uint)uVar2;
      uVar2 = uVar2 + 1;
    } while (param_2[uVar1] != 0);
    ImageFontDrawString(param_1,param_2,uVar1,param_3,param_4,param_5,param_6,param_7);
    return;
  }
  return;
}

```

## target disasm
```
  00081b80: mov r12,r2
  00081b82: cbz r0,0x00081bb8
  00081b84: cbz r1,0x00081bb8
  00081b86: push {r4,r5,r6,r7,lr}
  00081b88: add r7,sp,#0xc
  00081b8a: push.w r8
  00081b8e: sub sp,#0x10
  00081b90: add.w lr,r7,#0x8
  00081b94: movs r6,#0x0
  00081b96: ldm.w lr,{r5,r8,lr}
  00081b9a: uxth r2,r6
  00081b9c: adds r6,#0x1
  00081b9e: ldrh.w r4,[r1,r2,lsl #0x1]
  00081ba2: cmp r4,#0x0
  00081ba4: bne 0x00081b9a
  00081ba6: stm sp,{r3,r5,r8,lr}
  00081baa: mov r3,r12
  00081bac: blx 0x0006f520
  00081bb0: add sp,#0x10
  00081bb2: pop.w r8
  00081bb6: pop {r4,r5,r6,r7,pc}
  00081bb8: bx lr
```
