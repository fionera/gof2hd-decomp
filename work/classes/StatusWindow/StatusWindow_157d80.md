# StatusWindow::StatusWindow
elf 0x157d80 body 640
Sig: undefined __thiscall StatusWindow(StatusWindow * this)

## decompile
```c

/* StatusWindow::StatusWindow() */

StatusWindow * __thiscall StatusWindow::StatusWindow(StatusWindow *this)

{
  Array *pAVar1;
  TouchButton *pTVar2;
  int *piVar3;
  String *pSVar4;
  int iVar5;
  undefined4 uVar6;
  byte *pbVar7;
  uint uVar8;
  int *piVar9;
  int iVar10;
  uint *puVar11;
  int *piVar12;
  code *pcVar13;
  int iVar14;
  
  pAVar1 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(Array **)(this + 4) = pAVar1;
  ArraySetLength<TouchButton*>(2,pAVar1);
  pTVar2 = operator_new(200);
  piVar3 = *(int **)(DAT_00168024 + 0x167db0);
  pSVar4 = (String *)GameText::getText(*piVar3);
  piVar9 = *(int **)(DAT_00168028 + 0x167dc4);
  piVar12 = *(int **)(DAT_0016802c + 0x167dc6);
  iVar14 = *piVar9;
  iVar5 = Layout::getHelpButtonOffset();
  TouchButton::TouchButton(pTVar2,pSVar4,3,iVar14 - iVar5,0,'\x12');
  *(TouchButton **)(*(int *)(*(int *)(this + 4) + 4) + 4) = pTVar2;
  pTVar2 = operator_new(200);
  pSVar4 = (String *)GameText::getText(*piVar3);
  iVar10 = *piVar9;
  iVar5 = Layout::getHelpButtonOffset();
  iVar14 = TouchButton::getWidth();
  TouchButton::TouchButton
            (pTVar2,pSVar4,3,((iVar10 - iVar5) - iVar14) + *(int *)(*piVar12 + 0x38),0,'\x12');
  pbVar7 = *(byte **)(DAT_00168030 + 0x167e42);
  **(undefined4 **)(*(int *)(this + 4) + 4) = pTVar2;
  uVar8 = (uint)*pbVar7;
  *(uint *)(this + 0x30) = uVar8;
  TouchButton::setAlwaysPressed
            (*(TouchButton **)(*(int *)(*(int *)(this + 4) + 4) + uVar8 * 4),true);
  uVar6 = *(undefined4 *)(*piVar12 + 0x84);
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x78) = uVar6;
  *(undefined4 *)(this + 0x34) = 0xffffffff;
  pAVar1 = operator_new(0xc);
  Array<TouchButton*>::Array();
  *(undefined4 *)this = 0x2d;
  *(Array **)(this + 8) = pAVar1;
  ArraySetLength<TouchButton*>(0x2d,pAVar1);
  iVar5 = Achievements::getMedals((Achievements *)**(undefined4 **)(DAT_00168034 + 0x167e9c));
  for (iVar14 = 0; iVar14 < *(int *)this; iVar14 = iVar14 + 1) {
    pTVar2 = operator_new(200);
    iVar10 = *(int *)(iVar5 + iVar14 * 4);
    pSVar4 = (String *)GameText::getText(*piVar3);
    TouchButton::TouchButton(pTVar2,iVar14,iVar10,pSVar4,0,0,'D');
    *(TouchButton **)(*(int *)(*(int *)(this + 8) + 4) + iVar14 * 4) = pTVar2;
  }
  reInit();
  puVar11 = *(uint **)(DAT_00168038 + 0x167f08);
  pcVar13 = *(code **)(DAT_0016803c + 0x167f0a);
  (*pcVar13)(*puVar11,0x48e,this + 0x24);
  (*pcVar13)(*puVar11,0x48f,this + 0x28);
  (*pcVar13)(*puVar11,0x48d,this + 0x2c);
  iVar5 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar11);
  *(int *)(this + 0x70) = iVar5 / 2;
  uVar6 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar11);
  *(undefined4 *)(this + 0x45) = 0;
  *(undefined4 *)(this + 0x49) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x4d) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x51) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x40) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x44) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x74) = uVar6;
  piVar3 = operator_new__(0xc);
  *(int **)(this + 0x68) = piVar3;
  iVar5 = *piVar12;
  *piVar3 = *(int *)(iVar5 + 0x1c) * 3 + *(int *)(iVar5 + 0x2d8) + *(int *)(iVar5 + 0x2c) * 8 +
            *(int *)(this + 100) + *(int *)(iVar5 + 4) * 7;
  if (*pbVar7 == 0) {
    iVar10 = *(int *)(this + 0x78);
    iVar14 = __aeabi_idiv(*(undefined4 *)this,3);
    iVar10 = iVar10 * iVar14;
  }
  else {
    iVar10 = *(int *)(this + 0x78);
    iVar14 = __aeabi_idiv(*(undefined4 *)this,3);
    iVar10 = iVar14 * iVar10 + *(int *)(iVar5 + 0x1c) + *(int *)(iVar5 + 0x2c);
  }
  iVar14 = DAT_00168040;
  piVar3[1] = iVar10 + 10;
  piVar9 = *(int **)(iVar14 + 0x167fd8);
  *(int *)(this + 0x58) = piVar3[*(int *)(this + 0x30)];
  *(int *)(this + 0x5c) =
       (((*piVar9 - *(int *)(iVar5 + 0x10)) - *(int *)(iVar5 + 0xc)) - *(int *)(iVar5 + 0x20)) -
       *(int *)(iVar5 + 0x24);
  return this;
}

```

## target disasm
```
  00167d80: push {r4,r5,r6,r7,lr}
  00167d82: add r7,sp,#0xc
  00167d84: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00167d88: mov r9,r0
  00167d8a: movs r0,#0xc
  00167d8c: blx 0x0006eb24
  00167d90: mov r5,r0
  00167d92: blx 0x00074f38
  00167d96: movs r0,#0x2
  00167d98: mov r1,r5
  00167d9a: str.w r5,[r9,#0x4]
  00167d9e: blx 0x00074f44
  00167da2: movs r0,#0xc8
  00167da4: blx 0x0006eb24
  00167da8: mov r10,r0
  00167daa: ldr r0,[0x00168024]
  00167dac: add r0,pc
  00167dae: ldr r0,[r0,#0x0]
  00167db0: str r0,[sp,#0x18]
  00167db2: ldr r0,[r0,#0x0]
  00167db4: movs r1,#0xa8
  00167db6: blx 0x00072f70
  00167dba: mov r8,r0
  00167dbc: ldr r0,[0x00168028]
  00167dbe: ldr r1,[0x0016802c]
  00167dc0: add r0,pc
  00167dc2: add r1,pc
  00167dc4: ldr r4,[r0,#0x0]
  00167dc6: ldr r5,[r1,#0x0]
  00167dc8: ldr r6,[r4,#0x0]
  00167dca: ldr r0,[r5,#0x0]
  00167dcc: blx 0x00075ac0
  00167dd0: subs r3,r6,r0
  00167dd2: movs r1,#0x12
  00167dd4: movs r2,#0x0
  00167dd6: mov r0,r10
  00167dd8: strd r2,r1,[sp,#0x0]
  00167ddc: mov r1,r8
  00167dde: movs r2,#0x3
  00167de0: blx 0x00074578
  00167de4: ldr.w r0,[r9,#0x4]
  00167de8: ldr r0,[r0,#0x4]
  00167dea: str.w r10,[r0,#0x4]
  00167dee: movs r0,#0xc8
  00167df0: blx 0x0006eb24
  00167df4: mov r11,r0
  00167df6: ldr r0,[sp,#0x18]
  00167df8: ldr r0,[r0,#0x0]
  00167dfa: movw r1,#0x241
  00167dfe: blx 0x00072f70
  00167e02: mov r8,r0
  00167e04: ldr r4,[r4,#0x0]
  00167e06: ldr r0,[r5,#0x0]
  00167e08: blx 0x00075ac0
  00167e0c: mov r6,r0
  00167e0e: ldr.w r0,[r9,#0x4]
  00167e12: ldr r0,[r0,#0x4]
  00167e14: ldr r0,[r0,#0x4]
  00167e16: blx 0x00074da0
  00167e1a: ldr r1,[r5,#0x0]
  00167e1c: ldr r1,[r1,#0x38]
  00167e1e: movs r2,#0x12
  00167e20: mov.w r10,#0x0
  00167e24: strd r10,r2,[sp,#0x0]
  00167e28: subs r2,r4,r6
  00167e2a: subs r0,r2,r0
  00167e2c: movs r2,#0x3
  00167e2e: adds r3,r0,r1
  00167e30: mov r0,r11
  00167e32: mov r1,r8
  00167e34: blx 0x00074578
  00167e38: ldr.w r0,[r9,#0x4]
  00167e3c: ldr r1,[0x00168030]
  00167e3e: add r1,pc
  00167e40: ldr r0,[r0,#0x4]
  00167e42: ldr r1,[r1,#0x0]
  00167e44: str.w r11,[r0,#0x0]
  00167e48: ldr.w r0,[r9,#0x4]
  00167e4c: str r1,[sp,#0x10]
  00167e4e: ldrb r1,[r1,#0x0]
  00167e50: str.w r1,[r9,#0x30]
  00167e54: ldr r0,[r0,#0x4]
  00167e56: ldr.w r0,[r0,r1,lsl #0x2]
  00167e5a: movs r1,#0x1
  00167e5c: blx 0x00075acc
  00167e60: ldr r0,[r5,#0x0]
  00167e62: str r5,[sp,#0x14]
  00167e64: ldr.w r0,[r0,#0x84]
  00167e68: str.w r10,[r9,#0x10]
  00167e6c: str.w r0,[r9,#0x78]
  00167e70: mov.w r0,#0xffffffff
  00167e74: str.w r0,[r9,#0x34]
  00167e78: movs r0,#0xc
  00167e7a: blx 0x0006eb24
  00167e7e: mov r5,r0
  00167e80: blx 0x00074f38
  00167e84: movs r0,#0x2d
  00167e86: mov r1,r5
  00167e88: str.w r0,[r9,#0x0]
  00167e8c: movs r0,#0x2d
  00167e8e: str.w r5,[r9,#0x8]
  00167e92: blx 0x00074f44
  00167e96: ldr r0,[0x00168034]
  00167e98: add r0,pc
  00167e9a: ldr r0,[r0,#0x0]
  00167e9c: ldr r0,[r0,#0x0]
  00167e9e: blx 0x00074ac4
  00167ea2: mov r8,r0
  00167ea4: mov.w r10,#0x0
  00167ea8: mov.w r11,#0x0
  00167eac: b 0x00167eee
  00167eae: movs r0,#0xc8
  00167eb0: blx 0x0006eb24
  00167eb4: mov r6,r0
  00167eb6: ldr r0,[sp,#0x18]
  00167eb8: ldr.w r4,[r8,r11,lsl #0x2]
  00167ebc: addw r5,r11,#0x5e3
  00167ec0: ldr r0,[r0,#0x0]
  00167ec2: mov r1,r5
  00167ec4: blx 0x00072f70
  00167ec8: subw r5,r5,#0x5e3
  00167ecc: mov r3,r0
  00167ece: movs r0,#0x44
  00167ed0: strd r10,r10,[sp,#0x0]
  00167ed4: str r0,[sp,#0x8]
  00167ed6: mov r0,r6
  00167ed8: mov r1,r5
  00167eda: mov r2,r4
  00167edc: blx 0x0007774c
  00167ee0: ldr.w r0,[r9,#0x8]
  00167ee4: ldr r0,[r0,#0x4]
  00167ee6: str.w r6,[r0,r11,lsl #0x2]
  00167eea: add.w r11,r5,#0x1
  00167eee: ldr.w r0,[r9,#0x0]
  00167ef2: cmp r11,r0
  00167ef4: blt 0x00167eae
  00167ef6: mov r0,r9
  00167ef8: blx 0x000753c4
  00167efc: ldr r0,[0x00168038]
  00167efe: add.w r2,r9,#0x24
  00167f02: ldr r1,[0x0016803c]
  00167f04: add r0,pc
  00167f06: add r1,pc
  00167f08: ldr r4,[r0,#0x0]
  00167f0a: ldr r5,[r1,#0x0]
  00167f0c: movw r1,#0x48e
  00167f10: ldr r0,[r4,#0x0]
  00167f12: blx r5
  00167f14: ldr r0,[r4,#0x0]
  00167f16: add.w r2,r9,#0x28
  00167f1a: movw r1,#0x48f
  00167f1e: blx r5
  00167f20: ldr r0,[r4,#0x0]
  00167f22: add.w r2,r9,#0x2c
  00167f26: movw r1,#0x48d
  00167f2a: blx r5
  00167f2c: ldr.w r1,[r9,#0x24]
  00167f30: ldr r0,[r4,#0x0]
  00167f32: blx 0x00072d84
  00167f36: add.w r0,r0,r0, lsr #0x1f
  00167f3a: ldr.w r1,[r9,#0x24]
  00167f3e: asrs r0,r0,#0x1
  00167f40: str.w r0,[r9,#0x70]
  00167f44: ldr r0,[r4,#0x0]
  00167f46: blx 0x00072d90
  00167f4a: vmov.i32 q8,#0x0
  00167f4e: add.w r1,r9,#0x45
  00167f52: vst1.8 {d16,d17},[r1]
  00167f56: add.w r1,r9,#0x38
  00167f5a: vst1.32 {d16,d17},[r1]
  00167f5e: str.w r0,[r9,#0x74]
  00167f62: movs r0,#0xc
  00167f64: blx 0x0006ec08
  00167f68: str.w r0,[r9,#0x68]
  00167f6c: mov r5,r0
  00167f6e: ldr r0,[sp,#0x14]
  00167f70: ldr.w r6,[r9,#0x64]
  00167f74: ldr r4,[r0,#0x0]
  00167f76: ldr r1,[r4,#0x1c]
  00167f78: ldr r2,[r4,#0x2c]
  00167f7a: ldr.w r3,[r4,#0x2d8]
  00167f7e: add.w r1,r1,r1, lsl #0x1
  00167f82: ldr r0,[r4,#0x4]
  00167f84: add r1,r3
  00167f86: add.w r1,r1,r2, lsl #0x1
  00167f8a: add r1,r6
  00167f8c: add r1,r0
  00167f8e: add r0,r2
  00167f90: add.w r0,r0,r0, lsl #0x1
  00167f94: add.w r0,r1,r0, lsl #0x1
  00167f98: str r0,[r5,#0x0]
  00167f9a: ldr r0,[sp,#0x10]
  00167f9c: ldrb r0,[r0,#0x0]
  00167f9e: cbz r0,0x00167fba
  00167fa0: ldr.w r0,[r9,#0x0]
  00167fa4: movs r1,#0x3
  00167fa6: ldr.w r6,[r9,#0x78]
  00167faa: blx 0x0007198c
  00167fae: ldr r1,[r4,#0x1c]
  00167fb0: ldr r2,[r4,#0x2c]
  00167fb2: add r1,r2
  00167fb4: mla r0,r0,r6,r1
  00167fb8: b 0x00167fca
  00167fba: ldr.w r0,[r9,#0x0]
  00167fbe: movs r1,#0x3
  00167fc0: ldr.w r6,[r9,#0x78]
  00167fc4: blx 0x0007198c
  00167fc8: muls r0,r6
  00167fca: ldr r1,[0x00168040]
  00167fcc: adds r0,#0xa
  00167fce: str r0,[r5,#0x4]
  00167fd0: ldr.w r0,[r9,#0x30]
  00167fd4: add r1,pc
  00167fd6: ldr r1,[r1,#0x0]
  00167fd8: ldr.w r0,[r5,r0,lsl #0x2]
  00167fdc: str.w r0,[r9,#0x58]
  00167fe0: ldrd r0,r2,[r4,#0xc]
  00167fe4: ldrd r3,r6,[r4,#0x20]
  00167fe8: ldr r1,[r1,#0x0]
  00167fea: subs r1,r1,r2
  00167fec: subs r0,r1,r0
  00167fee: subs r0,r0,r3
  00167ff0: subs r0,r0,r6
  00167ff2: str.w r0,[r9,#0x5c]
  00167ff6: mov r0,r9
  00167ff8: add sp,#0x1c
  00167ffa: pop.w {r8,r9,r10,r11}
  00167ffe: pop {r4,r5,r6,r7,pc}
```
