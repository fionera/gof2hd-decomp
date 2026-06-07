# ScrollTouchWindow::drawTextBG
elf 0x17448c body 164
Sig: undefined __thiscall drawTextBG(ScrollTouchWindow * this)

## decompile
```c

/* ScrollTouchWindow::drawTextBG() */

void __thiscall ScrollTouchWindow::drawTextBG(ScrollTouchWindow *this)

{
  float fVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  String aSStack_34 [12];
  int local_28;
  
  piVar8 = *(int **)(DAT_00184540 + 0x1844a0);
  piVar12 = *(int **)(DAT_00184544 + 0x1844a2);
  iVar11 = *piVar8;
  local_28 = *piVar12;
  uVar3 = *(undefined4 *)(this + 0x14);
  iVar10 = *(int *)(this + 0x18);
  iVar4 = *(int *)(this + 0x1c);
  iVar6 = *(int *)(iVar11 + 0x2c);
  fVar1 = (float)ScrollTouchBox::getRelativeScrollHeight(*(ScrollTouchBox **)this);
  iVar5 = *piVar8;
  if (fVar1 <= 0.0) {
    iVar9 = *(int *)(iVar5 + 0x2c);
    iVar5 = iVar9;
  }
  else {
    iVar9 = *(int *)(iVar5 + 0x48) + *(int *)(iVar5 + 0x2c) * 2;
    iVar5 = *(int *)(iVar5 + 0x2c);
  }
  iVar7 = *(int *)(this + 0x20);
  uVar2 = AbyssEngine::String::String(aSStack_34,(char *)(DAT_00184548 + 0x1844f8),false);
  Layout::drawBox(iVar11,5,uVar3,iVar6 + iVar10,iVar4 - iVar9,iVar7 + iVar5 * -2,uVar2);
  AbyssEngine::String::~String(aSStack_34);
  if (*piVar12 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0018448c: push {r4,r5,r6,r7,lr}
  0018448e: add r7,sp,#0xc
  00184490: push {r8,r9,r10,r11}
  00184494: sub sp,#0x24
  00184496: mov r5,r0
  00184498: ldr r0,[0x00184540]
  0018449a: ldr r1,[0x00184544]
  0018449c: add r0,pc
  0018449e: add r1,pc
  001844a0: ldr r6,[r0,#0x0]
  001844a2: ldr.w r11,[r1,#0x0]
  001844a6: ldr.w r10,[r6,#0x0]
  001844aa: ldr.w r0,[r11,#0x0]
  001844ae: str r0,[sp,#0x20]
  001844b0: ldr r1,[r5,#0x14]
  001844b2: ldr r0,[r5,#0x0]
  001844b4: str r1,[sp,#0x10]
  001844b6: ldrd r8,r1,[r5,#0x18]
  001844ba: ldr.w r4,[r10,#0x2c]
  001844be: str r1,[sp,#0xc]
  001844c0: blx 0x00077e30
  001844c4: vmov s0,r0
  001844c8: ldr r1,[r6,#0x0]
  001844ca: add.w r9,r4,r8
  001844ce: vcmpe.f32 s0,#0
  001844d2: vmrs apsr,fpscr
  001844d6: ble 0x001844e6
  001844d8: ldr r0,[r1,#0x2c]
  001844da: ldr r1,[r1,#0x48]
  001844dc: add.w r6,r1,r0, lsl #0x1
  001844e0: lsl.w r8,r0,#0x1
  001844e4: b 0x001844ec
  001844e6: ldr r6,[r1,#0x2c]
  001844e8: lsl.w r8,r6,#0x1
  001844ec: ldr r4,[r5,#0x20]
  001844ee: add r0,sp,#0x14
  001844f0: ldr r1,[0x00184548]
  001844f2: movs r2,#0x0
  001844f4: add r1,pc
  001844f6: blx 0x0006ee18
  001844fa: ldr r2,[sp,#0xc]
  001844fc: sub.w r1,r4,r8
  00184500: mov r3,r9
  00184502: subs r2,r2,r6
  00184504: strd r2,r1,[sp,#0x0]
  00184508: ldr r2,[sp,#0x10]
  0018450a: movs r1,#0x5
  0018450c: str r0,[sp,#0x8]
  0018450e: mov r0,r10
  00184510: blx 0x0007462c
  00184514: add r0,sp,#0x14
  00184516: blx 0x0006ee30
  0018451a: ldr r0,[sp,#0x20]
  0018451c: ldr.w r1,[r11,#0x0]
  00184520: subs r0,r1,r0
  00184522: ittt eq
  00184524: add.eq sp,#0x24
  00184526: pop.eq.w {r8,r9,r10,r11}
  0018452a: pop.eq {r4,r5,r6,r7,pc}
  0018452c: blx 0x0006e824
```
