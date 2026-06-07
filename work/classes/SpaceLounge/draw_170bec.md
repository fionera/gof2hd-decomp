# SpaceLounge::draw
elf 0x170bec body 300
Sig: undefined __stdcall draw(void)

## decompile
```c

/* SpaceLounge::draw() */

void SpaceLounge::draw(void)

{
  SpaceLounge *in_r0;
  int iVar1;
  int iVar2;
  String *pSVar3;
  Layout *this;
  uint uVar4;
  undefined4 uVar5;
  uint *puVar6;
  undefined4 *puVar7;
  int *piVar8;
  
  piVar8 = *(int **)(DAT_00180d24 + 0x180bfc);
  iVar1 = *piVar8;
  if (in_r0[0x1c] != (SpaceLounge)0x0) {
    iVar2 = ListItemWindow::shows3DShip(*(ListItemWindow **)(in_r0 + 0xc));
    if (iVar2 != 0) {
      puVar6 = *(uint **)(DAT_00180d28 + 0x180c1c);
      AbyssEngine::PaintCanvas::SetColor((uchar)*puVar6,'\0','\0','\0');
      uVar4 = *puVar6;
      iVar2 = AbyssEngine::PaintCanvas::GetWidth();
      AbyssEngine::PaintCanvas::GetHeight();
      AbyssEngine::PaintCanvas::FillRectangle(uVar4,0,0,iVar2);
      AbyssEngine::PaintCanvas::SetColor(*puVar6);
    }
    ListItemWindow::draw(*(ListItemWindow **)(in_r0 + 0xc));
    if (*piVar8 == iVar1) {
      (*(code *)(DAT_001ac6a4 + 0x1ac6a8))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(**(undefined4 **)(DAT_00180d2c + 0x180c5a));
  }
  if (in_r0[0x34] != (SpaceLounge)0x0) {
    if (*piVar8 == iVar1) {
      (*(code *)(DAT_001ac274 + 0x1ac278))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(*(undefined4 *)(in_r0 + 4));
  }
  puVar7 = *(undefined4 **)(DAT_00180d30 + 0x180ca4);
  uVar5 = *puVar7;
  pSVar3 = (String *)GameText::getText(**(int **)(DAT_00180d34 + 0x180ca6));
  AbyssEngine::String::String((String *)&stack0xffffffd0,pSVar3,false);
  Layout::drawHeader(uVar5,(String *)&stack0xffffffd0);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  drawLounge(in_r0);
  this = (Layout *)*puVar7;
  if ((*(uint *)(in_r0 + 0x14) & 0xfffffffe) == 2) {
    Layout::drawFooterNoBackButton(this);
  }
  else {
    Layout::drawFooter(this);
  }
  if (((in_r0[0x19] != (SpaceLounge)0x0) || (in_r0[0x1a] != (SpaceLounge)0x0)) ||
     (in_r0[0x1b] != (SpaceLounge)0x0)) {
    ChoiceWindow::draw();
  }
  if (*piVar8 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00180bec: push {r4,r5,r6,r7,lr}
  00180bee: add r7,sp,#0xc
  00180bf0: push {r2,r3,r4,r5,r6,r7,r8,r9,r11}
  00180bf4: mov r4,r0
  00180bf6: ldr r0,[0x00180d24]
  00180bf8: add r0,pc
  00180bfa: ldr.w r8,[r0,#0x0]
  00180bfe: ldr.w r0,[r8,#0x0]
  00180c02: str r0,[sp,#0x14]
  00180c04: ldrb r0,[r4,#0x1c]
  00180c06: cbz r0,0x00180c78
  00180c08: ldr r0,[r4,#0xc]
  00180c0a: blx 0x00077dc4
  00180c0e: cbz r0,0x00180c4e
  00180c10: ldr r0,[0x00180d28]
  00180c12: movs r1,#0xff
  00180c14: movs r2,#0x0
  00180c16: movs r3,#0x0
  00180c18: add r0,pc
  00180c1a: ldr r6,[r0,#0x0]
  00180c1c: ldr r0,[r6,#0x0]
  00180c1e: str r1,[sp,#0x0]
  00180c20: movs r1,#0x0
  00180c22: blx 0x0006ff28
  00180c26: ldr r5,[r6,#0x0]
  00180c28: mov r0,r5
  00180c2a: blx 0x0006fafc
  00180c2e: mov r9,r0
  00180c30: ldr r0,[r6,#0x0]
  00180c32: blx 0x0006fb08
  00180c36: str r0,[sp,#0x0]
  00180c38: mov r0,r5
  00180c3a: movs r1,#0x0
  00180c3c: movs r2,#0x0
  00180c3e: mov r3,r9
  00180c40: blx 0x00074de8
  00180c44: ldr r0,[r6,#0x0]
  00180c46: mov.w r1,#0xffffffff
  00180c4a: blx 0x0006fac0
  00180c4e: ldr r0,[r4,#0xc]
  00180c50: blx 0x00077074
  00180c54: ldr r0,[0x00180d2c]
  00180c56: add r0,pc
  00180c58: ldr r0,[r0,#0x0]
  00180c5a: ldr r0,[r0,#0x0]
  00180c5c: ldr r1,[sp,#0x14]
  00180c5e: ldr.w r2,[r8,#0x0]
  00180c62: subs r1,r2,r1
  00180c64: itttt eq
  00180c66: add.eq sp,#0x18
  00180c68: pop.eq.w {r8,r9,r11}
  00180c6c: pop.eq.w {r4,r5,r6,r7,lr}
  00180c70: b.eq.w 0x001ac698
  00180c74: blx 0x0006e824
  00180c78: ldrb.w r0,[r4,#0x34]
  00180c7c: cbz r0,0x00180c9c
  00180c7e: ldr r0,[r4,#0x4]
  00180c80: ldr r1,[sp,#0x14]
  00180c82: ldr.w r2,[r8,#0x0]
  00180c86: subs r1,r2,r1
  00180c88: itttt eq
  00180c8a: add.eq sp,#0x18
  00180c8c: pop.eq.w {r8,r9,r11}
  00180c90: pop.eq.w {r4,r5,r6,r7,lr}
  00180c94: b.eq.w 0x001ac268
  00180c98: blx 0x0006e824
  00180c9c: ldr r0,[0x00180d30]
  00180c9e: ldr r1,[0x00180d34]
  00180ca0: add r0,pc
  00180ca2: add r1,pc
  00180ca4: ldr r6,[r0,#0x0]
  00180ca6: ldr r0,[r1,#0x0]
  00180ca8: mov.w r1,#0x18e
  00180cac: ldr r5,[r6,#0x0]
  00180cae: ldr r0,[r0,#0x0]
  00180cb0: blx 0x00072f70
  00180cb4: add.w r9,sp,#0x8
  00180cb8: mov r1,r0
  00180cba: movs r2,#0x0
  00180cbc: mov r0,r9
  00180cbe: blx 0x0006f028
  00180cc2: mov r0,r5
  00180cc4: mov r1,r9
  00180cc6: blx 0x000746d4
  00180cca: add r0,sp,#0x8
  00180ccc: blx 0x0006ee30
  00180cd0: mov r0,r4
  00180cd2: blx 0x00077dd0
  00180cd6: ldr r1,[r4,#0x14]
  00180cd8: ldr r0,[r6,#0x0]
  00180cda: bic r1,r1,#0x1
  00180cde: cmp r1,#0x2
  00180ce0: bne 0x00180ce8
  00180ce2: blx 0x00077ddc
  00180ce6: b 0x00180cec
  00180ce8: blx 0x00075b8c
  00180cec: ldrb r0,[r4,#0x19]
  00180cee: cbnz r0,0x00180cf8
  00180cf0: ldrb r0,[r4,#0x1a]
  00180cf2: cbnz r0,0x00180cf8
  00180cf4: ldrb r0,[r4,#0x1b]
  00180cf6: cbz r0,0x00180cfe
  00180cf8: ldr r0,[r4,#0x8]
  00180cfa: blx 0x000746ec
  00180cfe: ldr r0,[sp,#0x14]
  00180d00: ldr.w r1,[r8,#0x0]
  00180d04: subs r0,r1,r0
  00180d06: ittt eq
  00180d08: add.eq sp,#0x18
  00180d0a: pop.eq.w {r8,r9,r11}
  00180d0e: pop.eq {r4,r5,r6,r7,pc}
  00180d10: blx 0x0006e824
  001ac268: bx pc
  001ac698: bx pc
```
