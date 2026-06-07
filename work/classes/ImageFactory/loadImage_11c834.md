# ImageFactory::loadImage
elf 0x11c834 body 214
Sig: undefined __thiscall loadImage(ImageFactory * this, int param_1, int param_2, int param_3)

## decompile
```c

/* ImageFactory::loadImage(int, int, int) */

void __thiscall ImageFactory::loadImage(ImageFactory *this,int param_1,int param_2,int param_3)

{
  ImagePart *this_00;
  int *piVar1;
  int iVar2;
  int *piVar3;
  uint local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0012c918 + 0x12c84a);
  iVar2 = *(int *)(DAT_0012c91c + 0x12c84e + param_1 * 0x10 + param_2 * 4);
  local_1c = *piVar3;
  if (iVar2 < 0) goto LAB_0012c8ba;
  local_20 = 0;
  AbyssEngine::PaintCanvas::Image2DCreate
            ((PaintCanvas *)**(undefined4 **)(DAT_0012c920 + 0x12c86e),(short)iVar2 + (short)param_3
             ,&local_20);
  this_00 = operator_new(0x14);
  if (**(char **)(DAT_0012c924 + 0x12c886) == '\0') {
    if (**(char **)(DAT_0012c928 + 0x12c896) != '\0') {
      piVar1 = (int *)(DAT_0012c92c + 0x12c8a0);
      goto LAB_0012c89e;
    }
    piVar1 = (int *)(*(int *)(DAT_0012c938 + 0x12c8de) + param_1 * 0x20 + param_2 * 8);
    iVar2 = *(int *)(DAT_0012c938 + 0x12c8de);
    if (**(char **)(DAT_0012c934 + 0x12c8dc) == '\0') {
      iVar2 = *(int *)(DAT_0012c93c + 0x12c8e4);
    }
    if (**(char **)(DAT_0012c934 + 0x12c8dc) == '\0') {
      piVar1 = (int *)(*(int *)(DAT_0012c93c + 0x12c8e4) + param_1 * 0x20 + param_2 * 8);
    }
  }
  else {
    piVar1 = (int *)(DAT_0012c930 + 0x12c890);
LAB_0012c89e:
    iVar2 = *piVar1;
    piVar1 = (int *)(iVar2 + param_1 * 0x20 + param_2 * 8);
  }
  ImagePart::ImagePart(this_00,local_20,*piVar1,*(int *)(iVar2 + param_1 * 0x20 + param_2 * 8 + 4));
LAB_0012c8ba:
  iVar2 = *piVar3 - local_1c;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0012c834: push {r4,r5,r6,r7,lr}
  0012c836: add r7,sp,#0xc
  0012c838: push.w r8
  0012c83c: sub sp,#0x8
  0012c83e: ldr r0,[0x0012c918]
  0012c840: mov r6,r1
  0012c842: ldr r1,[0x0012c91c]
  0012c844: mov r5,r2
  0012c846: add r0,pc
  0012c848: movs r4,#0x0
  0012c84a: add r1,pc
  0012c84c: ldr.w r8,[r0,#0x0]
  0012c850: add.w r0,r1,r6, lsl #0x4
  0012c854: ldr.w r1,[r0,r2,lsl #0x2]
  0012c858: ldr.w r0,[r8,#0x0]
  0012c85c: str r0,[sp,#0x4]
  0012c85e: cmp r1,#0x0
  0012c860: blt 0x0012c8ba
  0012c862: ldr r0,[0x0012c920]
  0012c864: add r1,r3
  0012c866: mov r2,sp
  0012c868: str r4,[sp,#0x0]
  0012c86a: add r0,pc
  0012c86c: uxth r1,r1
  0012c86e: ldr r0,[r0,#0x0]
  0012c870: ldr r0,[r0,#0x0]
  0012c872: blx 0x00071cf8
  0012c876: movs r0,#0x14
  0012c878: blx 0x0006eb24
  0012c87c: mov r4,r0
  0012c87e: ldr r0,[0x0012c924]
  0012c880: ldr r1,[sp,#0x0]
  0012c882: add r0,pc
  0012c884: ldr r0,[r0,#0x0]
  0012c886: ldrb r0,[r0,#0x0]
  0012c888: cbz r0,0x0012c890
  0012c88a: ldr r0,[0x0012c930]
  0012c88c: add r0,pc
  0012c88e: b 0x0012c89e
  0012c890: ldr r0,[0x0012c928]
  0012c892: add r0,pc
  0012c894: ldr r0,[r0,#0x0]
  0012c896: ldrb r0,[r0,#0x0]
  0012c898: cbz r0,0x0012c8d2
  0012c89a: ldr r0,[0x0012c92c]
  0012c89c: add r0,pc
  0012c89e: ldr r0,[r0,#0x0]
  0012c8a0: add.w r2,r0,r6, lsl #0x5
  0012c8a4: add.w r2,r2,r5, lsl #0x3
  0012c8a8: add.w r0,r0,r6, lsl #0x5
  0012c8ac: ldr r2,[r2,#0x0]
  0012c8ae: add.w r0,r0,r5, lsl #0x3
  0012c8b2: ldr r3,[r0,#0x4]
  0012c8b4: mov r0,r4
  0012c8b6: blx 0x00076048
  0012c8ba: ldr r0,[sp,#0x4]
  0012c8bc: ldr.w r1,[r8,#0x0]
  0012c8c0: subs r0,r1,r0
  0012c8c2: itttt eq
  0012c8c4: mov.eq r0,r4
  0012c8c6: add.eq sp,#0x8
  0012c8c8: pop.eq.w r8
  0012c8cc: pop.eq {r4,r5,r6,r7,pc}
  0012c8ce: blx 0x0006e824
  0012c8d2: ldr r0,[0x0012c934]
  0012c8d4: ldr r2,[0x0012c938]
  0012c8d6: ldr r3,[0x0012c93c]
  0012c8d8: add r0,pc
  0012c8da: add r2,pc
  0012c8dc: ldr.w r12,[r0,#0x0]
  0012c8e0: add r3,pc
  0012c8e2: ldr r0,[r2,#0x0]
  0012c8e4: ldr r3,[r3,#0x0]
  0012c8e6: ldrb.w r12,[r12,#0x0]
  0012c8ea: add.w r2,r0,r6, lsl #0x5
  0012c8ee: add.w r2,r2,r5, lsl #0x3
  0012c8f2: cmp.w r12,#0x0
  0012c8f6: it eq
  0012c8f8: mov.eq r0,r3
  0012c8fa: add.w r3,r3,r6, lsl #0x5
  0012c8fe: cmp.w r12,#0x0
  0012c902: it eq
  0012c904: add.eq.w r2,r3,r5, lsl #0x3
  0012c908: b 0x0012c8a8
```
