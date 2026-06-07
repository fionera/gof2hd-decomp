# ImageFactory::reload
elf 0x11c5d0 body 202
Sig: undefined __stdcall reload(void)

## decompile
```c

/* ImageFactory::reload() */

void ImageFactory::reload(void)

{
  undefined4 *in_r0;
  uint *puVar1;
  void *pvVar2;
  Sprite *this;
  int iVar3;
  int iVar4;
  code *pcVar5;
  uint *puVar6;
  
  puVar1 = operator_new__(0x18);
  puVar6 = *(uint **)(DAT_0012c6a8 + 0x12c5ec);
  pcVar5 = *(code **)(DAT_0012c6ac + 0x12c5ee);
  (*pcVar5)(*puVar6,0x4fa,puVar1);
  (*pcVar5)(*puVar6,0x4fb,puVar1 + 1);
  (*pcVar5)(*puVar6,0x4f7,puVar1 + 2);
  (*pcVar5)(*puVar6,0x4f8,puVar1 + 3);
  (*pcVar5)(*puVar6,0x4f9,puVar1 + 4);
  (*pcVar5)(*puVar6,0x4fc,puVar1 + 5);
  if ((Sprite *)*in_r0 != (Sprite *)0x0) {
    pvVar2 = (void *)Sprite::~Sprite((Sprite *)*in_r0);
    operator_delete(pvVar2);
  }
  *in_r0 = 0;
  this = operator_new(0x40);
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar6);
  iVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar6);
  Sprite::Sprite(this,puVar1,6,iVar3,iVar4);
  *in_r0 = this;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar6,0x485,in_r0 + 1);
  (*(code *)(DAT_001ac394 + 0x1ac398))(*puVar6,0x511,in_r0 + 2);
  return;
}

```

## target disasm
```
  0012c5d0: push {r4,r5,r6,r7,lr}
  0012c5d2: add r7,sp,#0xc
  0012c5d4: push {r6,r7,r8,r9,r11}
  0012c5d8: mov r8,r0
  0012c5da: movs r0,#0x18
  0012c5dc: blx 0x0006ec08
  0012c5e0: mov r5,r0
  0012c5e2: ldr r0,[0x0012c6a8]
  0012c5e4: ldr r1,[0x0012c6ac]
  0012c5e6: mov r2,r5
  0012c5e8: add r0,pc
  0012c5ea: add r1,pc
  0012c5ec: ldr r6,[r0,#0x0]
  0012c5ee: ldr r4,[r1,#0x0]
  0012c5f0: movw r1,#0x4fa
  0012c5f4: ldr r0,[r6,#0x0]
  0012c5f6: blx r4
  0012c5f8: ldr r0,[r6,#0x0]
  0012c5fa: adds r2,r5,#0x4
  0012c5fc: movw r1,#0x4fb
  0012c600: blx r4
  0012c602: ldr r0,[r6,#0x0]
  0012c604: add.w r2,r5,#0x8
  0012c608: movw r1,#0x4f7
  0012c60c: blx r4
  0012c60e: ldr r0,[r6,#0x0]
  0012c610: add.w r2,r5,#0xc
  0012c614: mov.w r1,#0x4f8
  0012c618: blx r4
  0012c61a: ldr r0,[r6,#0x0]
  0012c61c: add.w r2,r5,#0x10
  0012c620: movw r1,#0x4f9
  0012c624: blx r4
  0012c626: ldr r0,[r6,#0x0]
  0012c628: add.w r2,r5,#0x14
  0012c62c: movw r1,#0x4fc
  0012c630: blx r4
  0012c632: ldr.w r0,[r8,#0x0]
  0012c636: cbz r0,0x0012c640
  0012c638: blx 0x0007600c
  0012c63c: blx 0x0006eb48
  0012c640: movs r0,#0x0
  0012c642: str.w r0,[r8,#0x0]
  0012c646: movs r0,#0x40
  0012c648: blx 0x0006eb24
  0012c64c: mov r9,r0
  0012c64e: ldr r1,[r5,#0x0]
  0012c650: ldr r0,[r6,#0x0]
  0012c652: blx 0x00072d84
  0012c656: mov r4,r0
  0012c658: ldr r1,[r5,#0x0]
  0012c65a: ldr r0,[r6,#0x0]
  0012c65c: blx 0x00072d90
  0012c660: str r0,[sp,#0x0]
  0012c662: mov r0,r9
  0012c664: mov r1,r5
  0012c666: movs r2,#0x6
  0012c668: mov r3,r4
  0012c66a: blx 0x00076018
  0012c66e: mov r4,r8
  0012c670: add.w r2,r8,#0x4
  0012c674: str.w r9,[r4],#0x8
  0012c678: movw r1,#0x485
  0012c67c: ldr r0,[r6,#0x0]
  0012c67e: blx 0x00071cf8
  0012c682: ldr r0,[r6,#0x0]
  0012c684: movw r1,#0x511
  0012c688: mov r2,r4
  0012c68a: add sp,#0x8
  0012c68c: pop.w {r8,r9,r11}
  0012c690: pop.w {r4,r5,r6,r7,lr}
  0012c694: b.w 0x001ac388
  001ac388: bx pc
```
