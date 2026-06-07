# ImageFactory::drawItem
elf 0x11ca60 body 102
Sig: undefined __stdcall drawItem(int param_1, int param_2, int param_3)

## decompile
```c

/* ImageFactory::drawItem(int, int, int) */

void ImageFactory::drawItem(int param_1,int param_2,int param_3)

{
  short sVar1;
  int iVar2;
  int *piVar3;
  uint *puVar4;
  
  piVar3 = *(int **)(DAT_0012cac8 + 0x12ca74);
  puVar4 = *(uint **)(DAT_0012cacc + 0x12ca78);
  iVar2 = *piVar3;
  AbyssEngine::PaintCanvas::SetColor(*puVar4);
  sVar1 = 0xef0;
  if (param_2 < 0xb0) {
    sVar1 = 0x898;
  }
  AbyssEngine::PaintCanvas::Image2DCreate
            ((PaintCanvas *)*puVar4,sVar1 + (short)param_2,(uint *)&stack0xffffffd8);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar4,-1,param_3);
  if (*piVar3 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0012ca60: push {r4,r5,r6,r7,lr}
  0012ca62: add r7,sp,#0xc
  0012ca64: push {r6,r7,r8,r9,r10}
  0012ca68: ldr r0,[0x0012cac8]
  0012ca6a: mov r6,r1
  0012ca6c: ldr r1,[0x0012cacc]
  0012ca6e: mov r8,r3
  0012ca70: add r0,pc
  0012ca72: mov r9,r2
  0012ca74: add r1,pc
  0012ca76: mov.w r10,#0xffffffff
  0012ca7a: ldr r4,[r0,#0x0]
  0012ca7c: ldr r5,[r1,#0x0]
  0012ca7e: ldr r1,[r4,#0x0]
  0012ca80: ldr r0,[r5,#0x0]
  0012ca82: str r1,[sp,#0x4]
  0012ca84: mov.w r1,#0xffffffff
  0012ca88: blx 0x0006fac0
  0012ca8c: mov.w r1,#0xef0
  0012ca90: str.w r10,[sp,#0x0]
  0012ca94: cmp r6,#0xb0
  0012ca96: it lt
  0012ca98: movw.lt r1,#0x898
  0012ca9c: add r1,r6
  0012ca9e: ldr r0,[r5,#0x0]
  0012caa0: mov r2,sp
  0012caa2: uxth r1,r1
  0012caa4: blx 0x00071cf8
  0012caa8: ldr r0,[r5,#0x0]
  0012caaa: mov r2,r9
  0012caac: ldr r1,[sp,#0x0]
  0012caae: mov r3,r8
  0012cab0: blx 0x00072dc0
  0012cab4: ldr r0,[sp,#0x4]
  0012cab6: ldr r1,[r4,#0x0]
  0012cab8: subs r0,r1,r0
  0012caba: itt eq
  0012cabc: pop.eq.w {r2,r3,r8,r9,r10}
  0012cac0: pop.eq {r4,r5,r6,r7,pc}
  0012cac2: blx 0x0006e824
```
