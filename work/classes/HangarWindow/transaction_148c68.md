# HangarWindow::transaction
elf 0x148c68 body 820
Sig: undefined __thiscall transaction(HangarWindow * this, bool param_1)

## decompile
```c

/* HangarWindow::transaction(bool) */

void __thiscall HangarWindow::transaction(HangarWindow *this,bool param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  Ship *pSVar4;
  undefined4 uVar5;
  uint *puVar6;
  int iVar7;
  int *piVar8;
  String *pSVar9;
  int *piVar10;
  BluePrint *this_00;
  code *pcVar11;
  Item *pIVar12;
  int *piVar13;
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar13 = *(int **)(DAT_00158fe4 + 0x158c7c);
  local_28 = *piVar13;
  uVar1 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
  if (uVar1 < 2) {
    iVar2 = Item::isUnsaleable(*(Item **)(*(int *)(this + 0x68) + 0x10));
    if (iVar2 == 0) {
      uVar1 = Item::transaction(*(Item **)(*(int *)(this + 0x68) + 0x10),param_1,
                                *(int *)(this + 0xa8),(bool)this[0x11d]);
      uVar3 = Item::getIndex(*(Item **)(*(int *)(this + 0x68) + 0x10));
      piVar10 = *(int **)(DAT_00158fec + 0x158d60);
      puVar6 = *(uint **)(*piVar10 + 0x54);
      if (uVar3 < *puVar6) {
        iVar2 = Item::getIndex(*(Item **)(*(int *)(this + 0x68) + 0x10));
        *(undefined1 *)(puVar6[1] + iVar2) = 1;
      }
      if ((uVar1 < 0x80000000) || (!param_1)) {
        if ((uVar1 == 0) && (param_1)) {
          iVar2 = Status::getCredits();
          iVar7 = Item::getSinglePrice(*(Item **)(*(int *)(this + 0x68) + 0x10));
          if (iVar2 < iVar7) {
            if (this[0x11d] != (HangarWindow)0x0) goto LAB_00158f86;
            piVar8 = *(int **)(DAT_00158ff0 + 0x158e06);
            iVar2 = *piVar10;
            pSVar9 = (String *)GameText::getText(*piVar8);
            AbyssEngine::String::String(aSStack_40,pSVar9,false);
            Item::getSinglePrice(*(Item **)(*(int *)(this + 0x68) + 0x10));
            Status::getCredits();
            Layout::formatCredits((int)aSStack_58);
            AbyssEngine::String::String(aSStack_4c,aSStack_58,false);
            uVar5 = AbyssEngine::String::String(aSStack_64,(char *)(DAT_00158ff8 + 0x158e50),false);
            Status::replaceHash(aSStack_34,iVar2,aSStack_40,aSStack_4c,uVar5);
            pcVar11 = *(code **)(DAT_00158ffc + 0x158e6a);
            (*pcVar11)(aSStack_64);
            (*pcVar11)(aSStack_4c);
            (*pcVar11)(aSStack_58);
            (*pcVar11)(aSStack_40);
            AbyssEngine::String::String(aSStack_70,(char *)(DAT_00159000 + 0x158e80),false);
            GameText::getText(*piVar8);
            AbyssEngine::operator+(aSStack_58,aSStack_70);
            AbyssEngine::String::operator+=(aSStack_34,aSStack_58);
            AbyssEngine::String::~String(aSStack_58);
            AbyssEngine::String::~String(aSStack_70);
            ChoiceWindow::set(*(ChoiceWindow **)(this + 0x20),aSStack_34,true);
            this[0x3c] = (HangarWindow)0x1;
            this[0xaf] = (HangarWindow)0x1;
            TouchButton::resetTouch(*(TouchButton **)(*(int *)(*(int *)(this + 0x24) + 4) + 0x20));
            TouchButton::resetTouch(*(TouchButton **)(*(int *)(*(int *)(this + 0x24) + 4) + 0x24));
            AbyssEngine::String::~String(aSStack_34);
          }
        }
        else if ((0 < (int)uVar1) && (!param_1)) {
          *(int *)(this + 0xa8) = *(int *)(this + 0xa8) + -1;
          pSVar4 = (Ship *)Status::getShip();
          Ship::changeLoad(pSVar4,-1);
        }
      }
      else {
        *(int *)(this + 0xa8) = *(int *)(this + 0xa8) + 1;
        pSVar4 = (Ship *)Status::getShip();
        Ship::changeLoad(pSVar4,1);
        iVar2 = ::ListItem::getIndex(*(ListItem **)(this + 0x68));
        if ((0x83 < iVar2) &&
           (iVar2 = ::ListItem::getIndex(*(ListItem **)(this + 0x68)), iVar2 < 0x9a)) {
          iVar7 = *(int *)(*piVar10 + 0xac);
          iVar2 = ::ListItem::getIndex(*(ListItem **)(this + 0x68));
          *(undefined1 *)(iVar2 + *(int *)(iVar7 + 4) + -0x84) = 1;
        }
      }
      if (this[0x11d] == (HangarWindow)0x0) {
        Status::changeCredits(*piVar10);
      }
    }
    else {
      pSVar9 = *(String **)(this + 0x20);
      GameText::getText(**(int **)(DAT_00158fe8 + 0x158cac));
      ChoiceWindow::set(pSVar9);
      this[0x88] = (HangarWindow)0x0;
      this[0x3c] = (HangarWindow)0x1;
    }
  }
  else if (uVar1 == 4) {
    if (param_1) {
      iVar2 = Item::getBlueprintAmount(*(Item **)(*(int *)(this + 0x68) + 0x10));
      this_00 = *(BluePrint **)(this + 0x80);
      iVar7 = Item::getIndex(*(Item **)(*(int *)(this + 0x68) + 0x10));
      iVar7 = BluePrint::getRemainingAmount(this_00,iVar7);
      if (iVar2 < iVar7) {
        iVar2 = Item::transactionBlueprint(SUB41(*(undefined4 *)(*(int *)(this + 0x68) + 0x10),0),0)
        ;
        if (iVar2 < 0) {
          *(int *)(this + 0xa8) = *(int *)(this + 0xa8) + 1;
        }
        else if (iVar2 != 0) {
          *(int *)(this + 0x94) = *(int *)(this + 0x94) + 1;
          pSVar4 = (Ship *)Status::getShip();
          Ship::changeLoad(pSVar4,-1);
        }
      }
    }
    Status::getShip();
    puVar6 = (uint *)Ship::getCargo();
    if (puVar6 != (uint *)0x0) {
      for (uVar1 = 0; uVar1 < *puVar6; uVar1 = uVar1 + 1) {
        iVar2 = Item::getIndex(*(Item **)(puVar6[1] + uVar1 * 4));
        iVar7 = Item::getIndex(*(Item **)(*(int *)(this + 0x68) + 0x10));
        if (iVar2 == iVar7) {
          pIVar12 = *(Item **)(puVar6[1] + uVar1 * 4);
          iVar2 = Item::getAmount();
          Item::setAmount(pIVar12,iVar2);
          pIVar12 = *(Item **)(puVar6[1] + uVar1 * 4);
          iVar2 = Item::getBlueprintAmount(*(Item **)(*(int *)(this + 0x68) + 0x10));
          Item::setBlueprintAmount(pIVar12,iVar2);
        }
      }
    }
  }
LAB_00158f86:
  if (*piVar13 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00158c68: push {r4,r5,r6,r7,lr}
  00158c6a: add r7,sp,#0xc
  00158c6c: push {r8,r9,r10,r11}
  00158c70: sub sp,#0x54
  00158c72: mov r8,r0
  00158c74: ldr r0,[0x00158fe4]
  00158c76: mov r6,r1
  00158c78: add r0,pc
  00158c7a: ldr.w r10,[r0,#0x0]
  00158c7e: ldr.w r0,[r10,#0x0]
  00158c82: str r0,[sp,#0x50]
  00158c84: ldr.w r0,[r8,#0x14]
  00158c88: blx 0x00076f48
  00158c8c: cmp r0,#0x1
  00158c8e: bhi 0x00158cc8
  00158c90: ldr.w r0,[r8,#0x68]
  00158c94: ldr r0,[r0,#0x10]
  00158c96: blx 0x00074ad0
  00158c9a: cmp r0,#0x0
  00158c9c: beq 0x00158d3a
  00158c9e: ldr r0,[0x00158fe8]
  00158ca0: movw r1,#0x143
  00158ca4: ldr.w r5,[r8,#0x20]
  00158ca8: add r0,pc
  00158caa: ldr r0,[r0,#0x0]
  00158cac: ldr r0,[r0,#0x0]
  00158cae: blx 0x00072f70
  00158cb2: mov r1,r0
  00158cb4: mov r0,r5
  00158cb6: blx 0x00074794
  00158cba: movs r0,#0x0
  00158cbc: strb.w r0,[r8,#0x88]
  00158cc0: movs r0,#0x1
  00158cc2: strb.w r0,[r8,#0x3c]
  00158cc6: b 0x00158f86
  00158cc8: cmp r0,#0x4
  00158cca: bne.w 0x00158f86
  00158cce: cmp r6,#0x0
  00158cd0: beq.w 0x00158f1c
  00158cd4: ldr.w r0,[r8,#0x68]
  00158cd8: ldr r0,[r0,#0x10]
  00158cda: blx 0x00076f78
  00158cde: mov r5,r0
  00158ce0: ldr.w r0,[r8,#0x68]
  00158ce4: ldr.w r6,[r8,#0x80]
  00158ce8: ldr r0,[r0,#0x10]
  00158cea: blx 0x000718d8
  00158cee: mov r1,r0
  00158cf0: mov r0,r6
  00158cf2: blx 0x00076f84
  00158cf6: cmp r5,r0
  00158cf8: bge.w 0x00158f1c
  00158cfc: ldr.w r0,[r8,#0x68]
  00158d00: movs r1,#0x0
  00158d02: ldr.w r2,[r8,#0xa8]
  00158d06: ldr r0,[r0,#0x10]
  00158d08: blx 0x00076f90
  00158d0c: cmp.w r0,#0xffffffff
  00158d10: ble.w 0x00158f12
  00158d14: cmp r0,#0x0
  00158d16: beq.w 0x00158f1c
  00158d1a: ldr r0,[0x00159004]
  00158d1c: ldr.w r1,[r8,#0x94]
  00158d20: add r0,pc
  00158d22: adds r1,#0x1
  00158d24: str.w r1,[r8,#0x94]
  00158d28: ldr r0,[r0,#0x0]
  00158d2a: ldr r0,[r0,#0x0]
  00158d2c: blx 0x00071a58
  00158d30: mov.w r1,#0xffffffff
  00158d34: blx 0x00076f9c
  00158d38: b 0x00158f1c
  00158d3a: ldr.w r0,[r8,#0x68]
  00158d3e: mov r1,r6
  00158d40: ldr.w r2,[r8,#0xa8]
  00158d44: ldrb.w r3,[r8,#0x11d]
  00158d48: ldr r0,[r0,#0x10]
  00158d4a: blx 0x00076fa8
  00158d4e: mov r11,r0
  00158d50: ldr.w r0,[r8,#0x68]
  00158d54: ldr r0,[r0,#0x10]
  00158d56: blx 0x000718d8
  00158d5a: ldr r1,[0x00158fec]
  00158d5c: add r1,pc
  00158d5e: ldr r5,[r1,#0x0]
  00158d60: ldr r1,[r5,#0x0]
  00158d62: ldr r4,[r1,#0x54]
  00158d64: ldr r1,[r4,#0x0]
  00158d66: cmp r0,r1
  00158d68: bcs 0x00158d7a
  00158d6a: ldr.w r0,[r8,#0x68]
  00158d6e: ldr r0,[r0,#0x10]
  00158d70: blx 0x000718d8
  00158d74: ldr r1,[r4,#0x4]
  00158d76: movs r2,#0x1
  00158d78: strb r2,[r1,r0]
  00158d7a: cmp.w r11,#0xffffffff
  00158d7e: bgt 0x00158dce
  00158d80: cbz r6,0x00158dce
  00158d82: ldr.w r0,[r8,#0xa8]
  00158d86: adds r0,#0x1
  00158d88: str.w r0,[r8,#0xa8]
  00158d8c: ldr r0,[r5,#0x0]
  00158d8e: blx 0x00071a58
  00158d92: movs r1,#0x1
  00158d94: blx 0x00076f9c
  00158d98: ldr.w r0,[r8,#0x68]
  00158d9c: blx 0x000768dc
  00158da0: cmp r0,#0x84
  00158da2: blt.w 0x00158f02
  00158da6: ldr.w r0,[r8,#0x68]
  00158daa: blx 0x000768dc
  00158dae: cmp r0,#0x99
  00158db0: bgt.w 0x00158f02
  00158db4: ldr r1,[r5,#0x0]
  00158db6: ldr.w r0,[r8,#0x68]
  00158dba: ldr.w r4,[r1,#0xac]
  00158dbe: blx 0x000768dc
  00158dc2: ldr r1,[r4,#0x4]
  00158dc4: add r0,r1
  00158dc6: movs r1,#0x1
  00158dc8: strb.w r1,[r0,#-0x84]
  00158dcc: b 0x00158f02
  00158dce: cmp.w r11,#0x0
  00158dd2: bne.w 0x00158ee2
  00158dd6: cmp r6,#0x0
  00158dd8: beq.w 0x00158ee2
  00158ddc: ldr r0,[r5,#0x0]
  00158dde: blx 0x000733d8
  00158de2: mov r6,r0
  00158de4: ldr.w r0,[r8,#0x68]
  00158de8: ldr r0,[r0,#0x10]
  00158dea: blx 0x00071944
  00158dee: cmp r6,r0
  00158df0: bge.w 0x00158f02
  00158df4: ldrb.w r0,[r8,#0x11d]
  00158df8: cmp r0,#0x0
  00158dfa: bne.w 0x00158f86
  00158dfe: ldr r0,[0x00158ff0]
  00158e00: movs r1,#0xcb
  00158e02: add r0,pc
  00158e04: ldr r4,[r0,#0x0]
  00158e06: ldr r0,[r5,#0x0]
  00158e08: str r0,[sp,#0x4]
  00158e0a: ldr r0,[r4,#0x0]
  00158e0c: blx 0x00072f70
  00158e10: mov r1,r0
  00158e12: add r0,sp,#0x38
  00158e14: movs r2,#0x0
  00158e16: blx 0x0006f028
  00158e1a: ldr r0,[0x00158ff4]
  00158e1c: ldr.w r1,[r8,#0x68]
  00158e20: add r0,pc
  00158e22: ldr r0,[r0,#0x0]
  00158e24: ldr.w r9,[r0,#0x0]
  00158e28: ldr r0,[r1,#0x10]
  00158e2a: blx 0x00071944
  00158e2e: mov r6,r0
  00158e30: ldr r0,[r5,#0x0]
  00158e32: blx 0x000733d8
  00158e36: subs r2,r6,r0
  00158e38: add r0,sp,#0x20
  00158e3a: mov r1,r9
  00158e3c: blx 0x00074e54
  00158e40: add r0,sp,#0x2c
  00158e42: add r1,sp,#0x20
  00158e44: movs r2,#0x0
  00158e46: blx 0x0006f028
  00158e4a: ldr r1,[0x00158ff8]
  00158e4c: add r1,pc
  00158e4e: add r0,sp,#0x14
  00158e50: movs r2,#0x0
  00158e52: blx 0x0006ee18
  00158e56: ldr r1,[sp,#0x4]
  00158e58: add r2,sp,#0x38
  00158e5a: str r0,[sp,#0x0]
  00158e5c: add r0,sp,#0x44
  00158e5e: add r3,sp,#0x2c
  00158e60: blx 0x000739b4
  00158e64: ldr r0,[0x00158ffc]
  00158e66: add r0,pc
  00158e68: ldr r6,[r0,#0x0]
  00158e6a: add r0,sp,#0x14
  00158e6c: blx r6
  00158e6e: add r0,sp,#0x2c
  00158e70: blx r6
  00158e72: add r0,sp,#0x20
  00158e74: blx r6
  00158e76: add r0,sp,#0x38
  00158e78: blx r6
  00158e7a: ldr r1,[0x00159000]
  00158e7c: add r1,pc
  00158e7e: add r0,sp,#0x8
  00158e80: movs r2,#0x0
  00158e82: blx 0x0006ee18
  00158e86: ldr r0,[r4,#0x0]
  00158e88: movs r1,#0x7c
  00158e8a: blx 0x00072f70
  00158e8e: mov r2,r0
  00158e90: add r0,sp,#0x20
  00158e92: add r1,sp,#0x8
  00158e94: blx 0x0006ef98
  00158e98: add r0,sp,#0x44
  00158e9a: add r1,sp,#0x20
  00158e9c: blx 0x0006ef5c
  00158ea0: add r0,sp,#0x20
  00158ea2: blx 0x0006ee30
  00158ea6: add r0,sp,#0x8
  00158ea8: blx 0x0006ee30
  00158eac: ldr.w r0,[r8,#0x20]
  00158eb0: add r1,sp,#0x44
  00158eb2: movs r2,#0x1
  00158eb4: movs r4,#0x1
  00158eb6: blx 0x000746f8
  00158eba: ldr.w r0,[r8,#0x24]
  00158ebe: strb.w r4,[r8,#0x3c]
  00158ec2: strb.w r4,[r8,#0xaf]
  00158ec6: ldr r0,[r0,#0x4]
  00158ec8: ldr r0,[r0,#0x20]
  00158eca: blx 0x00076fb4
  00158ece: ldr.w r0,[r8,#0x24]
  00158ed2: ldr r0,[r0,#0x4]
  00158ed4: ldr r0,[r0,#0x24]
  00158ed6: blx 0x00076fb4
  00158eda: add r0,sp,#0x44
  00158edc: blx 0x0006ee30
  00158ee0: b 0x00158f02
  00158ee2: cmp.w r11,#0x1
  00158ee6: blt 0x00158f02
  00158ee8: cbnz r6,0x00158f02
  00158eea: ldr.w r0,[r8,#0xa8]
  00158eee: subs r0,#0x1
  00158ef0: str.w r0,[r8,#0xa8]
  00158ef4: ldr r0,[r5,#0x0]
  00158ef6: blx 0x00071a58
  00158efa: mov.w r1,#0xffffffff
  00158efe: blx 0x00076f9c
  00158f02: ldrb.w r0,[r8,#0x11d]
  00158f06: cbnz r0,0x00158f86
  00158f08: ldr r0,[r5,#0x0]
  00158f0a: mov r1,r11
  00158f0c: blx 0x0007171c
  00158f10: b 0x00158f86
  00158f12: ldr.w r0,[r8,#0xa8]
  00158f16: adds r0,#0x1
  00158f18: str.w r0,[r8,#0xa8]
  00158f1c: ldr r0,[0x00159008]
  00158f1e: add r0,pc
  00158f20: ldr r0,[r0,#0x0]
  00158f22: ldr r0,[r0,#0x0]
  00158f24: blx 0x00071a58
  00158f28: blx 0x00073750
  00158f2c: mov r5,r0
  00158f2e: cbz r0,0x00158f86
  00158f30: movs r4,#0x0
  00158f32: b 0x00158f80
  00158f34: ldr r0,[r5,#0x4]
  00158f36: ldr.w r0,[r0,r4,lsl #0x2]
  00158f3a: blx 0x000718d8
  00158f3e: mov r6,r0
  00158f40: ldr.w r0,[r8,#0x68]
  00158f44: ldr r0,[r0,#0x10]
  00158f46: blx 0x000718d8
  00158f4a: cmp r6,r0
  00158f4c: bne 0x00158f7e
  00158f4e: ldr r0,[r5,#0x4]
  00158f50: ldr.w r1,[r8,#0x68]
  00158f54: ldr.w r6,[r0,r4,lsl #0x2]
  00158f58: ldr r0,[r1,#0x10]
  00158f5a: blx 0x0007183c
  00158f5e: mov r1,r0
  00158f60: mov r0,r6
  00158f62: blx 0x00076fc0
  00158f66: ldr r0,[r5,#0x4]
  00158f68: ldr.w r1,[r8,#0x68]
  00158f6c: ldr.w r6,[r0,r4,lsl #0x2]
  00158f70: ldr r0,[r1,#0x10]
  00158f72: blx 0x00076f78
  00158f76: mov r1,r0
  00158f78: mov r0,r6
  00158f7a: blx 0x00076234
  00158f7e: adds r4,#0x1
  00158f80: ldr r0,[r5,#0x0]
  00158f82: cmp r4,r0
  00158f84: bcc 0x00158f34
  00158f86: ldr r0,[sp,#0x50]
  00158f88: ldr.w r1,[r10,#0x0]
  00158f8c: subs r0,r1,r0
  00158f8e: ittt eq
  00158f90: add.eq sp,#0x54
  00158f92: pop.eq.w {r8,r9,r10,r11}
  00158f96: pop.eq {r4,r5,r6,r7,pc}
  00158f98: blx 0x0006e824
```
