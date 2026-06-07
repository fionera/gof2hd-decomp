# ModStation::showDlcMenu
elf 0xdc1bc body 194
Sig: undefined __thiscall showDlcMenu(ModStation * this)

## decompile
```c

/* ModStation::showDlcMenu() */

void __thiscall ModStation::showDlcMenu(ModStation *this)

{
  int iVar1;
  float unaff_r4;
  float unaff_r5;
  MenuTouchWindow *this_00;
  uint uVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  
  piVar3 = *(int **)(DAT_000ec28c + 0xec1cc);
  iVar1 = *piVar3;
  this_00 = (MenuTouchWindow *)this->field_50;
  if (this_00 == (MenuTouchWindow *)0x0) {
    this_00 = operator_new(0x240);
    MenuTouchWindow::MenuTouchWindow(this_00,2);
    this->field_50 = (int)this_00;
  }
  *(undefined1 *)((int)&this->m_nStarMapWindowOpen + 2) = 1;
  iVar4 = *(int *)(DAT_000ec290 + 0xec1fc);
  iVar5 = *(int *)(DAT_000ec294 + 0xec204);
  for (uVar2 = 0; uVar2 < **(uint **)(this_00 + 4); uVar2 = uVar2 + 1) {
    if (uVar2 < 10) {
      TouchButton::getPosition();
      *(int *)(iVar4 + uVar2 * 4) = (int)unaff_r4;
      TouchButton::getPosition();
      *(int *)(iVar5 + uVar2 * 4) = (int)unaff_r5;
      this_00 = (MenuTouchWindow *)this->field_50;
    }
  }
  **(uint **)(DAT_000ec298 + 0xec25a) = **(uint **)(this_00 + 4);
  *(undefined1 *)((int)&this->field_64 + 2) = 0;
  MenuTouchWindow::callDlcMenu(this_00);
  if (*piVar3 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ec1bc: push {r4,r5,r6,r7,lr}
  000ec1be: add r7,sp,#0xc
  000ec1c0: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000ec1c4: mov r4,r0
  000ec1c6: ldr r0,[0x000ec28c]
  000ec1c8: add r0,pc
  000ec1ca: ldr.w r8,[r0,#0x0]
  000ec1ce: ldr.w r0,[r8,#0x0]
  000ec1d2: str r0,[sp,#0x10]
  000ec1d4: ldr r5,[r4,#0x50]
  000ec1d6: cbnz r5,0x000ec1ea
  000ec1d8: mov.w r0,#0x240
  000ec1dc: blx 0x0006eb24
  000ec1e0: mov r5,r0
  000ec1e2: movs r1,#0x2
  000ec1e4: blx 0x00075268
  000ec1e8: str r5,[r4,#0x50]
  000ec1ea: movs r0,#0x1
  000ec1ec: add.w r9,sp,#0x4
  000ec1f0: strb.w r0,[r4,#0x62]
  000ec1f4: movs r6,#0x0
  000ec1f6: ldr r0,[0x000ec290]
  000ec1f8: add r0,pc
  000ec1fa: ldr.w r10,[r0,#0x0]
  000ec1fe: ldr r0,[0x000ec294]
  000ec200: add r0,pc
  000ec202: ldr.w r11,[r0,#0x0]
  000ec206: b 0x000ec24c
  000ec208: cmp r6,#0x9
  000ec20a: bhi 0x000ec24a
  000ec20c: ldr r0,[r0,#0x4]
  000ec20e: ldr.w r1,[r0,r6,lsl #0x2]
  000ec212: mov r0,r9
  000ec214: blx 0x00074dd0
  000ec218: vldr.32 s0,[sp,#0x4]
  000ec21c: vcvt.s32.f32 s0,s0
  000ec220: vmov r0,s0
  000ec224: str.w r0,[r10,r6,lsl #0x2]
  000ec228: ldr r0,[r4,#0x50]
  000ec22a: ldr r0,[r0,#0x4]
  000ec22c: ldr r0,[r0,#0x4]
  000ec22e: ldr.w r1,[r0,r6,lsl #0x2]
  000ec232: mov r0,r9
  000ec234: blx 0x00074dd0
  000ec238: vldr.32 s0,[sp,#0x8]
  000ec23c: vcvt.s32.f32 s0,s0
  000ec240: vmov r0,s0
  000ec244: str.w r0,[r11,r6,lsl #0x2]
  000ec248: ldr r5,[r4,#0x50]
  000ec24a: adds r6,#0x1
  000ec24c: ldr r0,[r5,#0x4]
  000ec24e: ldr r1,[r0,#0x0]
  000ec250: cmp r6,r1
  000ec252: bcc 0x000ec208
  000ec254: ldr r0,[0x000ec298]
  000ec256: add r0,pc
  000ec258: ldr r0,[r0,#0x0]
  000ec25a: str r1,[r0,#0x0]
  000ec25c: movs r0,#0x0
  000ec25e: strb.w r0,[r4,#0x66]
  000ec262: mov r0,r5
  000ec264: blx 0x000756b8
  000ec268: ldr r0,[sp,#0x10]
  000ec26a: ldr.w r1,[r8,#0x0]
  000ec26e: subs r0,r1,r0
  000ec270: ittt eq
  000ec272: add.eq sp,#0x14
  000ec274: pop.eq.w {r8,r9,r10,r11}
  000ec278: pop.eq {r4,r5,r6,r7,pc}
  000ec27a: blx 0x0006e824
```
