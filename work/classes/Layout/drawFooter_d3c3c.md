# Layout::drawFooter
elf 0xd3c3c body 770
Sig: undefined __thiscall drawFooter(Layout * this, bool param_1, bool param_2)

## decompile
```c

/* Layout::drawFooter(bool, bool) */

void __thiscall Layout::drawFooter(Layout *this,bool param_1,bool param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  String *pSVar5;
  uint uVar6;
  uint *puVar7;
  undefined4 *puVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar1 = *(int **)(DAT_000e3f94 + 0xe3c54);
  puVar7 = *(uint **)(DAT_000e3f98 + 0xe3c56);
  local_28 = *piVar1;
  AbyssEngine::PaintCanvas::SetColor(*puVar7);
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
  iVar3 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar7,*(int *)(this + 0x334),*(int *)(this + 0x2dc),
             (char)*(undefined4 *)(this + 0x2e0) + (char)*(undefined4 *)(this + 0x2e8),'\x11');
  piVar11 = *(int **)(DAT_000e3f9c + 0xe3cae);
  AbyssEngine::PaintCanvas::DrawImage2D
            (*puVar7,*(int *)(this + 0x33c),*(int *)(this + 0x2dc) + iVar3);
  iVar2 = iVar3 + iVar2;
  drawBGPattern(this,*(uint *)(this + 0x338),iVar2 + *(int *)(this + 0x2dc),
                (*(int *)(this + 0x2e0) + *(int *)(this + 0x2e8)) - *(int *)(*piVar11 + 0x10),
                *(int *)(this + 0x2e4) + iVar2 * -2,*(int *)(*piVar11 + 0x10));
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)*puVar7,*(uint *)(this + 0x33c),
             (*(int *)(this + 0x2dc) - iVar2) + *(int *)(this + 0x2e4),
             (*(int *)(this + 0x2e8) + *(int *)(this + 0x2e0)) - *(int *)(*piVar11 + 0x10),'\x01');
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)*puVar7,*(uint *)(this + 0x334),
             (*(int *)(this + 0x2dc) - iVar3) + *(int *)(this + 0x2e4),
             (*(int *)(this + 0x2e8) + *(int *)(this + 0x2e0)) - *(int *)(*piVar11 + 0x10),'\x01');
  TouchButton::setVisible(*(TouchButton **)(this + 0x3b4),!param_1 && param_2);
  if ((param_1) || (param_2)) {
    TouchButton::draw();
  }
  Status::getShip();
  iVar2 = Ship::getCurrentLoad();
  Status::getShip();
  iVar3 = Ship::getMaxLoad();
  if (iVar3 < iVar2) {
    AbyssEngine::PaintCanvas::SetColor(*puVar7);
  }
  Status::getShip();
  iVar2 = Ship::getCurrentLoad();
  AbyssEngine::String::String(aSStack_58,iVar2);
  AbyssEngine::String::String(aSStack_64,(char *)(DAT_000e3fa4 + 0xe3dc8),false);
  AbyssEngine::operator+(aSStack_4c,aSStack_58);
  Status::getShip();
  iVar2 = Ship::getMaxLoad();
  AbyssEngine::String::String(aSStack_70,iVar2);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::String(aSStack_7c,(char *)(DAT_000e3fa8 + 0xe3dfa),false);
  AbyssEngine::operator+(aSStack_34,aSStack_40);
  pcVar4 = *(code **)(DAT_000e3fac + 0xe3e12);
  (*pcVar4)(aSStack_7c);
  (*pcVar4)(aSStack_40);
  (*pcVar4)(aSStack_70);
  (*pcVar4)(aSStack_4c);
  (*pcVar4)(aSStack_64);
  (*pcVar4)(aSStack_58);
  iVar10 = *(int *)(this + 0x2dc);
  iVar3 = *(int *)(this + 0x2e4);
  uVar6 = *puVar7;
  puVar8 = *(undefined4 **)(DAT_000e3fb0 + 0xe3e34);
  pSVar5 = (String *)*puVar8;
  iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(uVar6,pSVar5);
  AbyssEngine::PaintCanvas::DrawString
            (uVar6,pSVar5,(int)aSStack_34,(iVar10 + iVar3 / 2) - iVar2 / 2,
             (bool)(((char)*(undefined4 *)(this + 0x2e8) + (char)*(undefined4 *)(this + 0x2e0)) -
                   (char)*(undefined4 *)(this + 0x14)));
  AbyssEngine::PaintCanvas::SetColor(*puVar7);
  Status::getCredits();
  formatCredits((int)aSStack_40);
  iVar2 = *(int *)(this + 0x2dc);
  iVar3 = *(int *)(this + 0x2e4);
  pSVar5 = (String *)*puVar8;
  uVar6 = *puVar7;
  if (param_1) {
    iVar9 = *(int *)(*piVar11 + 0x74);
    iVar10 = AbyssEngine::PaintCanvas::GetTextWidth(uVar6,pSVar5);
    AbyssEngine::PaintCanvas::DrawString
              (uVar6,pSVar5,(int)aSStack_40,((iVar3 + iVar2) - iVar9) - iVar10 / 2,
               (bool)(((char)*(undefined4 *)(this + 0x2e8) + (char)*(undefined4 *)(this + 0x2e0)) -
                     (char)*(undefined4 *)(this + 0x14)));
  }
  else {
    iVar10 = AbyssEngine::PaintCanvas::GetTextWidth(uVar6,pSVar5);
    AbyssEngine::PaintCanvas::DrawString
              (uVar6,pSVar5,(int)aSStack_40,(iVar3 + iVar2 + -10) - iVar10,
               (bool)(((char)*(undefined4 *)(this + 0x2e8) + (char)*(undefined4 *)(this + 0x2e0)) -
                     (char)*(undefined4 *)(this + 0x14)));
  }
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_34);
  if (*piVar1 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000e3c3c: push {r4,r5,r6,r7,lr}
  000e3c3e: add r7,sp,#0xc
  000e3c40: push {r8,r9,r10,r11}
  000e3c44: sub sp,#0x74
  000e3c46: strd r2,r1,[sp,#0x14]
  000e3c4a: mov r4,r0
  000e3c4c: ldr r0,[0x000e3f94]
  000e3c4e: ldr r1,[0x000e3f98]
  000e3c50: add r0,pc
  000e3c52: add r1,pc
  000e3c54: ldr r0,[r0,#0x0]
  000e3c56: ldr.w r8,[r1,#0x0]
  000e3c5a: str r0,[sp,#0x10]
  000e3c5c: ldr r1,[r0,#0x0]
  000e3c5e: ldr.w r0,[r8,#0x0]
  000e3c62: str r1,[sp,#0x70]
  000e3c64: ldr.w r1,[r4,#0x3b0]
  000e3c68: blx 0x0006fac0
  000e3c6c: ldr.w r1,[r4,#0x33c]
  000e3c70: ldr.w r0,[r8,#0x0]
  000e3c74: blx 0x00072d84
  000e3c78: mov r9,r0
  000e3c7a: ldr.w r0,[r8,#0x0]
  000e3c7e: ldr.w r1,[r4,#0x334]
  000e3c82: blx 0x00072d84
  000e3c86: ldrd r2,r3,[r4,#0x2dc]
  000e3c8a: mov r10,r0
  000e3c8c: ldr.w r6,[r4,#0x2e8]
  000e3c90: mov.w r12,#0x21
  000e3c94: ldr.w r1,[r4,#0x334]
  000e3c98: movs r5,#0x11
  000e3c9a: ldr.w r0,[r8,#0x0]
  000e3c9e: add r3,r6
  000e3ca0: strd r5,r12,[sp,#0x0]
  000e3ca4: blx 0x00071d70
  000e3ca8: ldr r0,[0x000e3f9c]
  000e3caa: add r0,pc
  000e3cac: ldr.w r11,[r0,#0x0]
  000e3cb0: ldr.w r2,[r11,#0x0]
  000e3cb4: ldrd r12,r6,[r4,#0x2dc]
  000e3cb8: ldr.w r5,[r4,#0x2e8]
  000e3cbc: ldr r3,[r2,#0x10]
  000e3cbe: add.w r2,r12,r10
  000e3cc2: add r6,r5
  000e3cc4: ldr.w r1,[r4,#0x33c]
  000e3cc8: ldr.w r0,[r8,#0x0]
  000e3ccc: subs r3,r6,r3
  000e3cce: blx 0x00072dc0
  000e3cd2: ldr.w r0,[r11,#0x0]
  000e3cd6: add.w r6,r10,r9
  000e3cda: ldrd r12,r3,[r4,#0x2dc]
  000e3cde: ldrd r1,r5,[r4,#0x2e4]
  000e3ce2: add r3,r5
  000e3ce4: ldr r0,[r0,#0x10]
  000e3ce6: sub.w r2,r1,r6, lsl #0x1
  000e3cea: ldr.w r1,[r4,#0x338]
  000e3cee: subs r3,r3,r0
  000e3cf0: strd r2,r0,[sp,#0x0]
  000e3cf4: add.w r2,r6,r12
  000e3cf8: mov r0,r4
  000e3cfa: blx 0x00074e00
  000e3cfe: ldr.w r0,[r11,#0x0]
  000e3d02: mov.w r9,#0x1
  000e3d06: ldr.w r12,[r0,#0x10]
  000e3d0a: ldrd r2,lr,[r4,#0x2dc]
  000e3d0e: ldrd r3,r5,[r4,#0x2e4]
  000e3d12: subs r2,r2,r6
  000e3d14: ldr.w r1,[r4,#0x33c]
  000e3d18: add r2,r3
  000e3d1a: add.w r3,r5,lr
  000e3d1e: ldr.w r0,[r8,#0x0]
  000e3d22: sub.w r3,r3,r12
  000e3d26: str.w r9,[sp,#0x0]
  000e3d2a: blx 0x00074e3c
  000e3d2e: ldr.w r1,[r11,#0x0]
  000e3d32: str.w r11,[sp,#0xc]
  000e3d36: mov r11,r8
  000e3d38: ldr.w r0,[r8,#0x0]
  000e3d3c: ldr.w r12,[r1,#0x10]
  000e3d40: ldrd r2,r6,[r4,#0x2dc]
  000e3d44: ldrd r5,r3,[r4,#0x2e4]
  000e3d48: add r3,r6
  000e3d4a: sub.w r2,r2,r10
  000e3d4e: ldr.w r1,[r4,#0x334]
  000e3d52: add r2,r5
  000e3d54: sub.w r3,r3,r12
  000e3d58: str.w r9,[sp,#0x0]
  000e3d5c: ldr r6,[sp,#0x14]
  000e3d5e: blx 0x00074e3c
  000e3d62: ldr r0,[sp,#0x18]
  000e3d64: eor r5,r0,#0x1
  000e3d68: ldr.w r0,[r4,#0x3b4]
  000e3d6c: and.w r1,r5,r6
  000e3d70: blx 0x00074e48
  000e3d74: cbnz r5,0x000e3d7c
  000e3d76: ldr.w r0,[r4,#0x3b8]
  000e3d7a: b 0x000e3d82
  000e3d7c: cbz r6,0x000e3d86
  000e3d7e: ldr.w r0,[r4,#0x3b4]
  000e3d82: blx 0x000746e0
  000e3d86: ldr r0,[0x000e3fa0]
  000e3d88: add r0,pc
  000e3d8a: ldr r6,[r0,#0x0]
  000e3d8c: ldr r0,[r6,#0x0]
  000e3d8e: blx 0x00071a58
  000e3d92: blx 0x00072bf8
  000e3d96: mov r5,r0
  000e3d98: ldr r0,[r6,#0x0]
  000e3d9a: blx 0x00071a58
  000e3d9e: blx 0x00072c04
  000e3da2: cmp r5,r0
  000e3da4: ble 0x000e3db0
  000e3da6: ldr.w r0,[r11,#0x0]
  000e3daa: ldr r1,[0x000e3f90]
  000e3dac: blx 0x0006fac0
  000e3db0: ldr r0,[r6,#0x0]
  000e3db2: blx 0x00071a58
  000e3db6: blx 0x00072bf8
  000e3dba: mov r1,r0
  000e3dbc: add r0,sp,#0x40
  000e3dbe: blx 0x0006f658
  000e3dc2: ldr r1,[0x000e3fa4]
  000e3dc4: add r1,pc
  000e3dc6: add r0,sp,#0x34
  000e3dc8: movs r2,#0x0
  000e3dca: blx 0x0006ee18
  000e3dce: add r0,sp,#0x4c
  000e3dd0: add r1,sp,#0x40
  000e3dd2: add r2,sp,#0x34
  000e3dd4: blx 0x0006ef98
  000e3dd8: ldr r0,[r6,#0x0]
  000e3dda: blx 0x00071a58
  000e3dde: blx 0x00072c04
  000e3de2: mov r1,r0
  000e3de4: add r0,sp,#0x28
  000e3de6: blx 0x0006f658
  000e3dea: add r0,sp,#0x58
  000e3dec: add r1,sp,#0x4c
  000e3dee: add r2,sp,#0x28
  000e3df0: blx 0x0006ef98
  000e3df4: ldr r1,[0x000e3fa8]
  000e3df6: add r1,pc
  000e3df8: add r0,sp,#0x1c
  000e3dfa: movs r2,#0x0
  000e3dfc: blx 0x0006ee18
  000e3e00: add r0,sp,#0x64
  000e3e02: add r1,sp,#0x58
  000e3e04: add r2,sp,#0x1c
  000e3e06: str r6,[sp,#0x14]
  000e3e08: blx 0x0006ef98
  000e3e0c: ldr r0,[0x000e3fac]
  000e3e0e: add r0,pc
  000e3e10: ldr r5,[r0,#0x0]
  000e3e12: add r0,sp,#0x1c
  000e3e14: blx r5
  000e3e16: add r0,sp,#0x58
  000e3e18: blx r5
  000e3e1a: add r0,sp,#0x28
  000e3e1c: blx r5
  000e3e1e: add r0,sp,#0x4c
  000e3e20: blx r5
  000e3e22: add r0,sp,#0x34
  000e3e24: blx r5
  000e3e26: add r0,sp,#0x40
  000e3e28: blx r5
  000e3e2a: ldr r0,[0x000e3fb0]
  000e3e2c: ldr.w r10,[r4,#0x2dc]
  000e3e30: add r0,pc
  000e3e32: ldr.w r9,[r4,#0x2e4]
  000e3e36: ldr.w r6,[r11,#0x0]
  000e3e3a: ldr.w r8,[r0,#0x0]
  000e3e3e: ldr.w r5,[r8,#0x0]
  000e3e42: add r2,sp,#0x64
  000e3e44: mov r0,r6
  000e3e46: mov r1,r5
  000e3e48: blx 0x0006faa8
  000e3e4c: ldr r1,[r4,#0x14]
  000e3e4e: ldr.w r2,[r4,#0x2e8]
  000e3e52: ldr.w r3,[r4,#0x2e0]
  000e3e56: add r2,r3
  000e3e58: mov.w r12,#0x0
  000e3e5c: subs r1,r2,r1
  000e3e5e: strd r1,r12,[sp,#0x0]
  000e3e62: add.w r1,r9,r9, lsr #0x1f
  000e3e66: add.w r0,r0,r0, lsr #0x1f
  000e3e6a: add r2,sp,#0x64
  000e3e6c: add.w r1,r10,r1, asr #0x1
  000e3e70: sub.w r3,r1,r0, asr #0x1
  000e3e74: mov r0,r6
  000e3e76: mov r1,r5
  000e3e78: blx 0x0006fe14
  000e3e7c: ldr.w r1,[r4,#0x3b0]
  000e3e80: ldr.w r0,[r11,#0x0]
  000e3e84: blx 0x0006fac0
  000e3e88: ldr r0,[sp,#0x14]
  000e3e8a: ldr r0,[r0,#0x0]
  000e3e8c: blx 0x000733d8
  000e3e90: mov r2,r0
  000e3e92: add r0,sp,#0x58
  000e3e94: blx 0x00074e54
  000e3e98: ldr.w r0,[r4,#0x2dc]
  000e3e9c: ldr.w r1,[r4,#0x2e4]
  000e3ea0: ldr.w r5,[r8,#0x0]
  000e3ea4: add.w r8,r1,r0
  000e3ea8: ldr r0,[sp,#0x18]
  000e3eaa: ldr.w r6,[r11,#0x0]
  000e3eae: cbz r0,0x000e3eee
  000e3eb0: ldr r0,[sp,#0xc]
  000e3eb2: ldr r0,[r0,#0x0]
  000e3eb4: ldr.w r9,[r0,#0x74]
  000e3eb8: add r2,sp,#0x58
  000e3eba: mov r0,r6
  000e3ebc: mov r1,r5
  000e3ebe: blx 0x0006faa8
  000e3ec2: ldr r1,[r4,#0x14]
  000e3ec4: ldr.w r2,[r4,#0x2e8]
  000e3ec8: ldr.w r3,[r4,#0x2e0]
  000e3ecc: add r2,r3
  000e3ece: movs r4,#0x0
  000e3ed0: subs r1,r2,r1
  000e3ed2: add.w r0,r0,r0, lsr #0x1f
  000e3ed6: strd r1,r4,[sp,#0x0]
  000e3eda: sub.w r1,r8,r9
  000e3ede: sub.w r3,r1,r0, asr #0x1
  000e3ee2: add r2,sp,#0x58
  000e3ee4: mov r0,r6
  000e3ee6: mov r1,r5
  000e3ee8: blx 0x0006fe14
  000e3eec: b 0x000e3f1c
  000e3eee: add r2,sp,#0x58
  000e3ef0: mov r0,r6
  000e3ef2: mov r1,r5
  000e3ef4: blx 0x0006faa8
  000e3ef8: ldr r1,[r4,#0x14]
  000e3efa: ldr.w r2,[r4,#0x2e8]
  000e3efe: ldr.w r3,[r4,#0x2e0]
  000e3f02: add r2,r3
  000e3f04: movs r4,#0x0
  000e3f06: subs r1,r2,r1
  000e3f08: strd r1,r4,[sp,#0x0]
  000e3f0c: sub.w r1,r8,#0xa
  000e3f10: add r2,sp,#0x58
  000e3f12: subs r3,r1,r0
  000e3f14: mov r0,r6
  000e3f16: mov r1,r5
  000e3f18: blx 0x0006fe14
  000e3f1c: add r0,sp,#0x58
  000e3f1e: blx 0x0006ee30
  000e3f22: add r0,sp,#0x64
  000e3f24: blx 0x0006ee30
  000e3f28: ldr r0,[sp,#0x70]
  000e3f2a: ldr r1,[sp,#0x10]
  000e3f2c: ldr r1,[r1,#0x0]
  000e3f2e: subs r0,r1,r0
  000e3f30: ittt eq
  000e3f32: add.eq sp,#0x74
  000e3f34: pop.eq.w {r8,r9,r10,r11}
  000e3f38: pop.eq {r4,r5,r6,r7,pc}
  000e3f3a: blx 0x0006e824
```
