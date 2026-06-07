# ImageFactory::drawItem
elf 0x11c9b4 body 146
Sig: undefined __thiscall drawItem(ImageFactory * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* ImageFactory::drawItem(int, int, int, int) */

void __thiscall
ImageFactory::drawItem(ImageFactory *this,int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  int *piVar5;
  undefined8 uVar6;
  
  piVar5 = *(int **)(DAT_0012ca48 + 0x12c9c8);
  puVar4 = *(uint **)(DAT_0012ca4c + 0x12c9cc);
  iVar2 = *piVar5;
  AbyssEngine::PaintCanvas::SetColor(*puVar4);
  Sprite::setFrame(*(Sprite **)this,param_2);
  uVar6 = Sprite::setPosition(*(Sprite **)this,param_3,param_4);
  Sprite::draw((float)uVar6,(float)((ulonglong)uVar6 >> 0x20));
  sVar1 = 0xef0;
  if (param_1 < 0xb0) {
    sVar1 = 0x898;
  }
  AbyssEngine::PaintCanvas::Image2DCreate
            ((PaintCanvas *)*puVar4,sVar1 + (short)param_1,(uint *)&stack0xffffffd4);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar4,-1,param_3);
  iVar3 = *piVar5;
  iVar2 = iVar3 - iVar2;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2,iVar3);
  }
  return;
}

```

## target disasm
```
  0012c9b4: push {r4,r5,r6,r7,lr}
  0012c9b6: add r7,sp,#0xc
  0012c9b8: push {r5,r6,r7,r8,r9,r10,r11}
  0012c9bc: mov r4,r0
  0012c9be: ldr r0,[0x0012ca48]
  0012c9c0: mov r10,r1
  0012c9c2: ldr r1,[0x0012ca4c]
  0012c9c4: add r0,pc
  0012c9c6: mov r8,r3
  0012c9c8: add r1,pc
  0012c9ca: mov r6,r2
  0012c9cc: ldr.w r9,[r0,#0x0]
  0012c9d0: mov.w r11,#0xffffffff
  0012c9d4: ldr r5,[r1,#0x0]
  0012c9d6: ldr.w r1,[r9,#0x0]
  0012c9da: ldr r0,[r5,#0x0]
  0012c9dc: str r1,[sp,#0x8]
  0012c9de: mov.w r1,#0xffffffff
  0012c9e2: blx 0x0006fac0
  0012c9e6: ldr r0,[r4,#0x0]
  0012c9e8: mov r1,r6
  0012c9ea: blx 0x00074d70
  0012c9ee: ldr r6,[r7,#0x8]
  0012c9f0: mov r1,r8
  0012c9f2: ldr r0,[r4,#0x0]
  0012c9f4: mov r2,r6
  0012c9f6: blx 0x00076060
  0012c9fa: ldr r0,[r4,#0x0]
  0012c9fc: mov.w r1,#0x3f800000
  0012ca00: mov.w r2,#0x3f800000
  0012ca04: blx 0x0007606c
  0012ca08: mov.w r1,#0xef0
  0012ca0c: str.w r11,[sp,#0x4]
  0012ca10: cmp.w r10,#0xb0
  0012ca14: it lt
  0012ca16: movw.lt r1,#0x898
  0012ca1a: add r1,r10
  0012ca1c: ldr r0,[r5,#0x0]
  0012ca1e: add r2,sp,#0x4
  0012ca20: uxth r1,r1
  0012ca22: blx 0x00071cf8
  0012ca26: ldr r0,[r5,#0x0]
  0012ca28: mov r2,r8
  0012ca2a: ldr r1,[sp,#0x4]
  0012ca2c: mov r3,r6
  0012ca2e: blx 0x00072dc0
  0012ca32: ldr r0,[sp,#0x8]
  0012ca34: ldr.w r1,[r9,#0x0]
  0012ca38: subs r0,r1,r0
  0012ca3a: itt eq
  0012ca3c: pop.eq.w {r1,r2,r3,r8,r9,r10,r11}
  0012ca40: pop.eq {r4,r5,r6,r7,pc}
  0012ca42: blx 0x0006e824
```
