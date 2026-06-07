# ImageFactory::drawChar
elf 0x11c940 body 110
Sig: undefined __thiscall drawChar(ImageFactory * this, Array * param_1, int param_2, int param_3, bool param_4)

## decompile
```c

/* ImageFactory::drawChar(Array<ImagePart*>*, int, int, bool) */

void __thiscall
ImageFactory::drawChar(ImageFactory *this,Array *param_1,int param_2,int param_3,bool param_4)

{
  ImagePart *this_00;
  uint uVar1;
  uint *puVar2;
  
  puVar2 = *(uint **)(DAT_0012c9b0 + 0x12c956);
  AbyssEngine::PaintCanvas::SetColor(*puVar2);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar2,*(int *)(this + 4),param_2);
  for (uVar1 = 0; uVar1 < *(uint *)param_1; uVar1 = uVar1 + 1) {
    this_00 = *(ImagePart **)(*(int *)(param_1 + 4) + uVar1 * 4);
    if (this_00 != (ImagePart *)0x0) {
      ImagePart::draw(this_00,param_2,param_3,param_4);
    }
  }
  (*(code *)(DAT_001ac074 + 0x1ac078))(*puVar2,*(undefined4 *)(this + 8),param_2,param_3);
  return;
}

```

## target disasm
```
  0012c940: push {r4,r5,r6,r7,lr}
  0012c942: add r7,sp,#0xc
  0012c944: push {r7,r8,r9,r10,r11}
  0012c948: mov r8,r0
  0012c94a: ldr r0,[0x0012c9b0]
  0012c94c: mov r6,r1
  0012c94e: mov.w r1,#0xffffffff
  0012c952: add r0,pc
  0012c954: mov r9,r3
  0012c956: mov r11,r2
  0012c958: ldr.w r10,[r0,#0x0]
  0012c95c: ldr.w r0,[r10,#0x0]
  0012c960: blx 0x0006fac0
  0012c964: ldr.w r1,[r8,#0x4]
  0012c968: mov r2,r11
  0012c96a: ldr.w r0,[r10,#0x0]
  0012c96e: mov r3,r9
  0012c970: blx 0x00072dc0
  0012c974: ldr r4,[r7,#0x8]
  0012c976: movs r5,#0x0
  0012c978: b 0x0012c98e
  0012c97a: ldr r0,[r6,#0x4]
  0012c97c: ldr.w r0,[r0,r5,lsl #0x2]
  0012c980: cbz r0,0x0012c98c
  0012c982: mov r1,r11
  0012c984: mov r2,r9
  0012c986: mov r3,r4
  0012c988: blx 0x00076054
  0012c98c: adds r5,#0x1
  0012c98e: ldr r0,[r6,#0x0]
  0012c990: cmp r5,r0
  0012c992: bcc 0x0012c97a
  0012c994: ldr.w r1,[r8,#0x8]
  0012c998: mov r2,r11
  0012c99a: ldr.w r0,[r10,#0x0]
  0012c99e: mov r3,r9
  0012c9a0: add sp,#0x4
  0012c9a2: pop.w {r8,r9,r10,r11}
  0012c9a6: pop.w {r4,r5,r6,r7,lr}
  0012c9aa: b.w 0x001ac068
```
