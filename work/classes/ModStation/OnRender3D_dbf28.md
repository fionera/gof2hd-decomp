# ModStation::OnRender3D
elf 0xdbf28 body 192
Sig: undefined __thiscall OnRender3D(ModStation * this)

## decompile
```c

/* ModStation::OnRender3D() */

void __thiscall ModStation::OnRender3D(ModStation *this)

{
  uint *puVar1;
  
  if ((char)this->field_24 == '\0') {
    return;
  }
  puVar1 = *(uint **)(DAT_000ebfe8 + 0xebf40);
  AbyssEngine::PaintCanvas::ClearBuffer(*puVar1);
  if (((((this->field_14 == (int *)0x0) || (*(char *)((int)&this->field_64 + 2) != '\0')) ||
       ((char)this->field_64 != '\0')) ||
      (((char)this->field_68 != '\0' || (*(char *)((int)&this->field_64 + 3) != '\0')))) ||
     ((*(char *)((int)&this->field_64 + 1) != '\0' ||
      (*(char *)((int)&this->m_nStarMapWindowOpen + 2) != '\0')))) {
    if (*(char *)((int)&this->field_64 + 3) == '\0') {
      if (*(char *)((int)&this->field_64 + 1) != '\0') {
        SpaceLounge::OnRenderBG((SpaceLounge *)this->field_74);
      }
    }
    else {
      StarMap::renderBG();
    }
  }
  else {
    CutScene::renderBG();
  }
  AbyssEngine::PaintCanvas::Begin3d();
  if (*(char *)((int)&this->field_64 + 1) == '\0') {
    if (*(char *)((int)&this->field_64 + 3) == '\0') {
      if ((char)this->field_64 == '\0') {
        if ((*(char *)((int)&this->field_64 + 2) == '\0') && (this->field_14 != (int *)0x0)) {
          CutScene::render3D();
        }
      }
      else {
        MissionsWindow::render3D();
      }
    }
    else {
      StarMap::render();
    }
  }
  else {
    SpaceLounge::OnRender3D();
  }
  (*(code *)(DAT_001ab924 + 0x1ab928))(*puVar1);
  return;
}

```

## target disasm
```
  000ebf28: push {r4,r5,r6,r7,lr}
  000ebf2a: add r7,sp,#0xc
  000ebf2c: push.w r11
  000ebf30: mov r4,r0
  000ebf32: ldrb.w r0,[r0,#0x24]
  000ebf36: cbz r0,0x000ebf82
  000ebf38: ldr r0,[0x000ebfe8]
  000ebf3a: movs r1,#0x0
  000ebf3c: add r0,pc
  000ebf3e: ldr r5,[r0,#0x0]
  000ebf40: ldr r0,[r5,#0x0]
  000ebf42: blx 0x00071d94
  000ebf46: ldr r0,[r4,#0x14]
  000ebf48: cbz r0,0x000ebf70
  000ebf4a: ldrb.w r1,[r4,#0x66]
  000ebf4e: cbnz r1,0x000ebf70
  000ebf50: ldrb.w r1,[r4,#0x64]
  000ebf54: cbnz r1,0x000ebf70
  000ebf56: ldrb.w r1,[r4,#0x68]
  000ebf5a: cbnz r1,0x000ebf70
  000ebf5c: ldrb.w r1,[r4,#0x67]
  000ebf60: cbnz r1,0x000ebf70
  000ebf62: mov r6,r4
  000ebf64: ldrb.w r1,[r6,#0x65]!
  000ebf68: cbnz r1,0x000ebf70
  000ebf6a: ldrb.w r1,[r4,#0x62]
  000ebf6e: cbz r1,0x000ebfe2
  000ebf70: ldrb.w r0,[r4,#0x67]
  000ebf74: cbz r0,0x000ebf88
  000ebf76: ldr r0,[r4,#0x10]
  000ebf78: blx 0x00075658
  000ebf7c: add.w r6,r4,#0x65
  000ebf80: b 0x000ebf96
  000ebf82: pop.w r11
  000ebf86: pop {r4,r5,r6,r7,pc}
  000ebf88: mov r6,r4
  000ebf8a: ldrb.w r0,[r6,#0x65]!
  000ebf8e: cbz r0,0x000ebf96
  000ebf90: ldr r0,[r4,#0x74]
  000ebf92: blx 0x00075664
  000ebf96: ldr r0,[r5,#0x0]
  000ebf98: blx 0x00071da0
  000ebf9c: ldrb r0,[r6,#0x0]
  000ebf9e: cbz r0,0x000ebfa8
  000ebfa0: ldr r0,[r4,#0x74]
  000ebfa2: blx 0x00075670
  000ebfa6: b 0x000ebfd4
  000ebfa8: ldrb.w r0,[r4,#0x67]
  000ebfac: cbz r0,0x000ebfb6
  000ebfae: ldr r0,[r4,#0x10]
  000ebfb0: blx 0x0007567c
  000ebfb4: b 0x000ebfd4
  000ebfb6: ldrb.w r0,[r4,#0x64]
  000ebfba: cbz r0,0x000ebfc6
  000ebfbc: ldr.w r0,[r4,#0x80]
  000ebfc0: blx 0x00075688
  000ebfc4: b 0x000ebfd4
  000ebfc6: ldrb.w r0,[r4,#0x66]
  000ebfca: cbnz r0,0x000ebfd4
  000ebfcc: ldr r0,[r4,#0x14]
  000ebfce: cbz r0,0x000ebfd4
  000ebfd0: blx 0x00075694
  000ebfd4: ldr r0,[r5,#0x0]
  000ebfd6: pop.w r11
  000ebfda: pop.w {r4,r5,r6,r7,lr}
  000ebfde: b.w 0x001ab918
  000ebfe2: blx 0x000756a0
  000ebfe6: b 0x000ebf96
```
