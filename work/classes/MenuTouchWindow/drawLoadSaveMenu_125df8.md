# MenuTouchWindow::drawLoadSaveMenu
elf 0x125df8 body 1108
Sig: undefined __stdcall drawLoadSaveMenu(bool param_1)

## decompile
```c

/* MenuTouchWindow::drawLoadSaveMenu(bool) */

void MenuTouchWindow::drawLoadSaveMenu(bool param_1)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  uint uVar9;
  uint *puVar10;
  int *piVar11;
  undefined4 uVar12;
  int *piVar13;
  int *piVar14;
  int iVar15;
  code *pcVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int local_5c;
  int local_58;
  String aSStack_34 [12];
  int local_28;
  
  uVar1 = (uint)param_1;
  piVar13 = *(int **)(DAT_00135f9c + 0x135e0e);
  piVar2 = *(int **)(DAT_00135fa0 + 0x135e10);
  puVar10 = *(uint **)(DAT_00135fa4 + 0x135e14);
  local_28 = *piVar2;
  iVar5 = *(int *)(*piVar13 + 0x10c);
  AbyssEngine::PaintCanvas::SetColor(*puVar10);
  iVar3 = *piVar13;
  iVar20 = *(int *)(uVar1 + 0x198);
  piVar11 = *(int **)(DAT_00135fa8 + 0x135e3a);
  iVar19 = *(int *)(iVar3 + 0x28);
  local_58 = *(int *)(iVar3 + 0x110);
  iVar17 = *piVar11;
  local_5c = *(int *)(iVar3 + 0x114);
  iVar6 = iVar17 + iVar19 * -2 + iVar20 * -2;
  if ((**(char **)(DAT_00135fac + 0x135e62) != '\0') &&
     (**(char **)(DAT_00135fb0 + 0x135e6e) == '\0')) {
    local_5c = 8;
    if (**(char **)(DAT_00135fb4 + 0x135e7e) == '\0') {
      local_58 = 0xc;
      if (**(char **)(DAT_0013627c + 0x135e94) == '\0') {
        local_5c = 4;
        local_58 = 6;
      }
    }
    else {
      local_58 = 8;
      local_5c = 5;
    }
    iVar3 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar10);
    iVar17 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar10);
    iVar19 = __aeabi_idiv(**(undefined4 **)(DAT_00136280 + 0x135ec8));
    iVar20 = 0;
    for (iVar18 = 0; iVar18 <= iVar19; iVar18 = iVar18 + 1) {
      AbyssEngine::PaintCanvas::DrawImage2D
                ((PaintCanvas *)*puVar10,*(uint *)(uVar1 + 0x11c),
                 (*(int *)(*piVar13 + 0x28) - iVar3) + *(int *)(uVar1 + 0x198),iVar20,'\x01');
      AbyssEngine::PaintCanvas::DrawImage2D
                ((PaintCanvas *)*puVar10,*(uint *)(uVar1 + 0x11c),
                 *(int *)(*piVar13 + 0x28) + iVar6 + *(int *)(uVar1 + 0x198),iVar20,'\0');
      iVar20 = iVar20 + iVar17;
    }
    iVar20 = *(int *)(uVar1 + 0x198);
    iVar3 = *piVar13;
    iVar17 = *piVar11;
    iVar19 = *(int *)(iVar3 + 0x28);
  }
  TouchButton::setPosition
            (*(TouchButton **)(uVar1 + 0xc4),(iVar17 - iVar20) - iVar19,
             (*(int *)(iVar3 + 0x70) + *(int *)(uVar1 + 0x1b4)) * *(int *)(uVar1 + 0x18c) +
             *(int *)(uVar1 + 0x194) + *(int *)(iVar3 + 0x20) + *(int *)(iVar3 + 0xc) +
             *(int *)(iVar3 + 0x108));
  piVar14 = *(int **)(DAT_00136284 + 0x135f86);
  piVar11 = *(int **)(DAT_00136288 + 0x135f8c);
  pcVar4 = (char *)(DAT_0013628c + 0x135f98);
  for (iVar3 = 0; iVar3 < *piVar14; iVar3 = iVar3 + 1) {
    iVar17 = *piVar13;
    iVar17 = (*(int *)(iVar17 + 0x70) + *(int *)(uVar1 + 0x1b4)) * iVar3 +
             *(int *)(uVar1 + 0x194) + *(int *)(iVar17 + 0x20) + *(int *)(iVar17 + 0xc);
    if ((-1 < iVar17) && (iVar17 <= *piVar11)) {
      AbyssEngine::PaintCanvas::SetColor(*puVar10);
      iVar15 = *piVar13;
      iVar19 = *(int *)(uVar1 + 0x18c);
      iVar20 = *(int *)(uVar1 + 0x198);
      iVar18 = *(int *)(iVar15 + 0x28);
      uVar12 = *(undefined4 *)(iVar15 + 0x70);
      AbyssEngine::String::String(aSStack_34,pcVar4,false);
      uVar7 = 3;
      if (iVar3 == iVar19) {
        uVar7 = 4;
      }
      Layout::drawBox(iVar15,uVar7,iVar18 + iVar20,iVar17,iVar6 + -3,uVar12,aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      puVar8 = *(undefined4 **)(DAT_00136290 + 0x136056);
      iVar20 = local_58 + iVar17;
      AbyssEngine::PaintCanvas::DrawString
                (*puVar10,(String *)*puVar8,
                 **(int **)(*(int *)(*(int *)(*(int *)(uVar1 + 0x100) + 4) + iVar3 * 4) + 4),
                 *(int *)(*piVar13 + 0x28) + *(int *)(uVar1 + 0x198) + *(int *)(*piVar13 + 0x2c),
                 SUB41(iVar20,0));
      iVar19 = *(int *)(*(int *)(*(int *)(uVar1 + 0xbc) + 4) + iVar3 * 4);
      if ((iVar19 != 0) && (uVar9 = *(uint *)(iVar19 + 0x1a0), uVar9 < 0x40)) {
        ImageFactory::drawShip
                  ((ImageFactory *)**(undefined4 **)(DAT_00136294 + 0x13609a),uVar9,
                   *(int *)(*piVar13 + 0x2c) + *(int *)(*piVar13 + 0x28) + *(int *)(uVar1 + 0x198) +
                   *(int *)(uVar1 + 0x1bc),iVar5 + iVar17);
      }
      iVar19 = *piVar13;
      pcVar16 = *(code **)(DAT_00136298 + 0x1360fe);
      (*pcVar16)(*puVar10,*puVar8,
                 *(undefined4 *)
                  (*(int *)(*(int *)(*(int *)(*(int *)(uVar1 + 0x100) + 4) + iVar3 * 4) + 4) + 4),
                 *(int *)(iVar19 + 0x28) + *(int *)(uVar1 + 0x198) + *(int *)(iVar19 + 0x2c) * 2 +
                 *(int *)(uVar1 + 0x1bc) + *(int *)(iVar19 + 0x2c4),iVar20,0);
      AbyssEngine::PaintCanvas::SetColor(*puVar10);
      iVar17 = iVar17 + local_5c;
      iVar19 = *piVar13;
      (*pcVar16)(*puVar10,*puVar8,
                 *(undefined4 *)
                  (*(int *)(*(int *)(*(int *)(*(int *)(uVar1 + 0x100) + 4) + iVar3 * 4) + 4) + 8),
                 *(int *)(iVar19 + 0x28) + *(int *)(uVar1 + 0x198) + *(int *)(iVar19 + 0x2c),
                 iVar17 + *(int *)(iVar19 + 0x70) / 2,0);
      iVar19 = *piVar13;
      (*pcVar16)(*puVar10,*puVar8,
                 *(undefined4 *)
                  (*(int *)(*(int *)(*(int *)(*(int *)(uVar1 + 0x100) + 4) + iVar3 * 4) + 4) + 0xc),
                 *(int *)(iVar19 + 0x28) + *(int *)(uVar1 + 0x198) + *(int *)(iVar19 + 0x2c) * 2 +
                 *(int *)(uVar1 + 0x1bc) + *(int *)(iVar19 + 0x2c4),
                 iVar17 + *(int *)(iVar19 + 0x70) / 2,0);
      iVar19 = *piVar13;
      (*pcVar16)(*puVar10,*puVar8,
                 *(undefined4 *)
                  (*(int *)(*(int *)(*(int *)(*(int *)(uVar1 + 0x100) + 4) + iVar3 * 4) + 4) + 0x10)
                 ,*(int *)(iVar19 + 0x28) + *(int *)(uVar1 + 0x198) + *(int *)(iVar19 + 0x2c4) +
                  (*(int *)(iVar19 + 0x2c) + *(int *)(uVar1 + 0x1b8)) * 2,iVar20,0);
      iVar19 = *piVar13;
      (*pcVar16)(*puVar10,*puVar8,
                 *(undefined4 *)
                  (*(int *)(*(int *)(*(int *)(*(int *)(uVar1 + 0x100) + 4) + iVar3 * 4) + 4) + 0x14)
                 ,*(int *)(iVar19 + 0x28) + *(int *)(uVar1 + 0x198) + *(int *)(iVar19 + 0x2c4) +
                  (*(int *)(iVar19 + 0x2c) + *(int *)(uVar1 + 0x1b8)) * 2,
                 iVar17 + *(int *)(iVar19 + 0x70) / 2,0);
      if (iVar3 == *(int *)(uVar1 + 0x18c)) {
        TouchButton::draw();
      }
    }
  }
  if (*piVar2 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00135df8: push {r4,r5,r6,r7,lr}
  00135dfa: add r7,sp,#0xc
  00135dfc: push {r8,r9,r10,r11}
  00135e00: sub sp,#0x5c
  00135e02: mov r11,r0
  00135e04: ldr r0,[0x00135f9c]
  00135e06: ldr r1,[0x00135fa0]
  00135e08: ldr r2,[0x00135fa4]
  00135e0a: add r0,pc
  00135e0c: add r1,pc
  00135e0e: ldr r5,[r0,#0x0]
  00135e10: add r2,pc
  00135e12: ldr r0,[r1,#0x0]
  00135e14: ldr r3,[r2,#0x0]
  00135e16: str r5,[sp,#0x48]
  00135e18: str r3,[sp,#0x44]
  00135e1a: ldr r1,[r5,#0x0]
  00135e1c: str r0,[sp,#0x10]
  00135e1e: ldr r2,[r0,#0x0]
  00135e20: ldr r0,[r3,#0x0]
  00135e22: str r2,[sp,#0x58]
  00135e24: ldr.w r1,[r1,#0x10c]
  00135e28: str r1,[sp,#0x14]
  00135e2a: mov.w r1,#0xffffffff
  00135e2e: blx 0x0006fac0
  00135e32: ldr r1,[0x00135fa8]
  00135e34: ldr r0,[r5,#0x0]
  00135e36: add r1,pc
  00135e38: ldr.w lr,[r11,#0x198]
  00135e3c: ldr r3,[r1,#0x0]
  00135e3e: ldr.w r12,[r0,#0x28]
  00135e42: ldr.w r1,[r0,#0x110]
  00135e46: ldr.w r8,[r3,#0x0]
  00135e4a: str r1,[sp,#0x28]
  00135e4c: ldr.w r1,[r0,#0x114]
  00135e50: sub.w r2,r8,r12, lsl #0x1
  00135e54: str r1,[sp,#0x24]
  00135e56: sub.w r1,r2,lr, lsl #0x1
  00135e5a: str r1,[sp,#0x40]
  00135e5c: ldr r2,[0x00135fac]
  00135e5e: add r2,pc
  00135e60: ldr r2,[r2,#0x0]
  00135e62: ldrb r2,[r2,#0x0]
  00135e64: cmp r2,#0x0
  00135e66: beq 0x00135f3c
  00135e68: ldr r2,[0x00135fb0]
  00135e6a: add r2,pc
  00135e6c: ldr r2,[r2,#0x0]
  00135e6e: ldrb r2,[r2,#0x0]
  00135e70: cmp r2,#0x0
  00135e72: bne 0x00135f3c
  00135e74: ldr r0,[0x00135fb4]
  00135e76: movs r1,#0x8
  00135e78: ldr r5,[sp,#0x44]
  00135e7a: add r0,pc
  00135e7c: str r3,[sp,#0x34]
  00135e7e: ldr r0,[r0,#0x0]
  00135e80: ldrb r0,[r0,#0x0]
  00135e82: cbz r0,0x00135e8e
  00135e84: movs r0,#0x8
  00135e86: str r0,[sp,#0x28]
  00135e88: movs r0,#0x5
  00135e8a: str r0,[sp,#0x24]
  00135e8c: b 0x00135ea8
  00135e8e: ldr r0,[0x0013627c]
  00135e90: add r0,pc
  00135e92: ldr r0,[r0,#0x0]
  00135e94: ldrb r0,[r0,#0x0]
  00135e96: cmp r0,#0x0
  00135e98: mov.w r0,#0xc
  00135e9c: it eq
  00135e9e: mov.eq r1,#0x4
  00135ea0: it eq
  00135ea2: mov.eq r0,#0x6
  00135ea4: strd r1,r0,[sp,#0x24]
  00135ea8: ldr.w r1,[r11,#0x11c]
  00135eac: ldr r0,[r5,#0x0]
  00135eae: blx 0x00072d84
  00135eb2: str r0,[sp,#0x3c]
  00135eb4: ldr r0,[r5,#0x0]
  00135eb6: ldr.w r1,[r11,#0x11c]
  00135eba: blx 0x00072d90
  00135ebe: mov r1,r0
  00135ec0: ldr r0,[0x00136280]
  00135ec2: str r1,[sp,#0x38]
  00135ec4: add r0,pc
  00135ec6: ldr r0,[r0,#0x0]
  00135ec8: ldr r0,[r0,#0x0]
  00135eca: blx 0x0007198c
  00135ece: ldr.w r8,[sp,#0x44]
  00135ed2: mov r9,r0
  00135ed4: movs r6,#0x0
  00135ed6: mov.w r10,#0x0
  00135eda: b 0x00135f26
  00135edc: ldr r5,[sp,#0x48]
  00135ede: movs r4,#0x1
  00135ee0: ldr.w r1,[r11,#0x11c]
  00135ee4: ldr.w r3,[r11,#0x198]
  00135ee8: ldr r2,[r5,#0x0]
  00135eea: ldr.w r0,[r8,#0x0]
  00135eee: ldr r2,[r2,#0x28]
  00135ef0: str r4,[sp,#0x0]
  00135ef2: ldr r4,[sp,#0x3c]
  00135ef4: subs r2,r2,r4
  00135ef6: add r2,r3
  00135ef8: mov r3,r6
  00135efa: blx 0x00074e3c
  00135efe: ldr r2,[r5,#0x0]
  00135f00: movs r5,#0x0
  00135f02: ldr.w r1,[r11,#0x11c]
  00135f06: ldr.w r3,[r11,#0x198]
  00135f0a: ldr r2,[r2,#0x28]
  00135f0c: str r5,[sp,#0x0]
  00135f0e: ldr r5,[sp,#0x40]
  00135f10: ldr.w r0,[r8,#0x0]
  00135f14: add r2,r5
  00135f16: add r2,r3
  00135f18: mov r3,r6
  00135f1a: blx 0x00074e3c
  00135f1e: ldr r0,[sp,#0x38]
  00135f20: add.w r10,r10,#0x1
  00135f24: add r6,r0
  00135f26: cmp r10,r9
  00135f28: ble 0x00135edc
  00135f2a: ldr r0,[sp,#0x48]
  00135f2c: ldr r1,[sp,#0x34]
  00135f2e: ldr.w lr,[r11,#0x198]
  00135f32: ldr r0,[r0,#0x0]
  00135f34: ldr.w r8,[r1,#0x0]
  00135f38: ldr.w r12,[r0,#0x28]
  00135f3c: ldr.w r9,[r0,#0xc]
  00135f40: ldr r5,[r0,#0x20]
  00135f42: ldr r1,[r0,#0x70]
  00135f44: ldr.w r3,[r11,#0x1b4]
  00135f48: ldr.w r6,[r11,#0x18c]
  00135f4c: ldr.w r2,[r11,#0x194]
  00135f50: add r1,r3
  00135f52: add.w r3,r5,r9
  00135f56: ldr.w r4,[r0,#0x108]
  00135f5a: add r2,r3
  00135f5c: ldr.w r0,[r11,#0xc4]
  00135f60: mla r1,r1,r6,r2
  00135f64: ldr.w r10,[sp,#0x48]
  00135f68: adds r2,r1,r4
  00135f6a: sub.w r1,r8,lr
  00135f6e: sub.w r1,r1,r12
  00135f72: blx 0x00076498
  00135f76: ldr r0,[sp,#0x40]
  00135f78: mov.w r8,#0x0
  00135f7c: subs r0,#0x3
  00135f7e: str r0,[sp,#0x20]
  00135f80: ldr r0,[0x00136284]
  00135f82: add r0,pc
  00135f84: ldr r5,[r0,#0x0]
  00135f86: ldr r0,[0x00136288]
  00135f88: add r0,pc
  00135f8a: ldr r0,[r0,#0x0]
  00135f8c: str r0,[sp,#0x2c]
  00135f8e: ldr r0,[0x0013628c]
  00135f90: ldr.w r9,[sp,#0x44]
  00135f94: add r0,pc
  00135f96: str r0,[sp,#0x1c]
  00135f98: str r5,[sp,#0x18]
  00135f9a: b 0x0013624a
  00135fb8: ldr.w r0,[r10,#0x0]
  00135fbc: ldr.w r2,[r11,#0x1b4]
  00135fc0: ldr.w r1,[r11,#0x194]
  00135fc4: ldr r3,[r0,#0xc]
  00135fc6: ldr r6,[r0,#0x20]
  00135fc8: ldr r0,[r0,#0x70]
  00135fca: add r0,r2
  00135fcc: adds r2,r6,r3
  00135fce: add r1,r2
  00135fd0: mla r4,r0,r8,r1
  00135fd4: cmp r4,#0x0
  00135fd6: blt.w 0x00136246
  00135fda: ldr r0,[sp,#0x2c]
  00135fdc: ldr r0,[r0,#0x0]
  00135fde: cmp r4,r0
  00135fe0: bgt.w 0x00136246
  00135fe4: ldr.w r0,[r9,#0x0]
  00135fe8: mov.w r1,#0xffffffff
  00135fec: blx 0x0006fac0
  00135ff0: ldr.w r6,[r10,#0x0]
  00135ff4: movs r2,#0x0
  00135ff6: ldr.w r0,[r11,#0x18c]
  00135ffa: str r0,[sp,#0x34]
  00135ffc: ldr.w r0,[r11,#0x198]
  00136000: str r0,[sp,#0x3c]
  00136002: ldr r0,[r6,#0x28]
  00136004: ldr r1,[sp,#0x1c]
  00136006: str r0,[sp,#0x38]
  00136008: add r0,sp,#0x4c
  0013600a: str r4,[sp,#0x40]
  0013600c: ldr r4,[r6,#0x70]
  0013600e: blx 0x0006ee18
  00136012: ldr r0,[sp,#0x20]
  00136014: movs r1,#0x3
  00136016: strd r0,r4,[sp,#0x0]
  0013601a: add r4,sp,#0x4c
  0013601c: ldr r0,[sp,#0x34]
  0013601e: str r4,[sp,#0x8]
  00136020: cmp r8,r0
  00136022: it eq
  00136024: mov.eq r1,#0x4
  00136026: ldrd r2,r0,[sp,#0x38]
  0013602a: add r2,r0
  0013602c: mov r0,r6
  0013602e: ldr r6,[sp,#0x40]
  00136030: mov r3,r6
  00136032: blx 0x0007462c
  00136036: mov r0,r4
  00136038: blx 0x0006ee30
  0013603c: ldr.w r0,[r11,#0x100]
  00136040: mov r3,r6
  00136042: ldr.w r12,[r11,#0x198]
  00136046: ldr r0,[r0,#0x4]
  00136048: ldr r1,[0x00136290]
  0013604a: ldr.w r6,[r10,#0x0]
  0013604e: ldr.w r0,[r0,r8,lsl #0x2]
  00136052: add r1,pc
  00136054: ldr r1,[r1,#0x0]
  00136056: str r1,[sp,#0x34]
  00136058: ldr r2,[r0,#0x4]
  0013605a: ldr r1,[r1,#0x0]
  0013605c: ldr.w r0,[r9,#0x0]
  00136060: ldr r2,[r2,#0x0]
  00136062: ldrd lr,r6,[r6,#0x28]
  00136066: ldr r4,[sp,#0x28]
  00136068: add r4,r3
  0013606a: add.w r3,lr,r12
  0013606e: add r3,r6
  00136070: str r4,[sp,#0x3c]
  00136072: str r4,[sp,#0x0]
  00136074: movs r4,#0x0
  00136076: str r4,[sp,#0x4]
  00136078: blx 0x0006fe14
  0013607c: ldr.w r0,[r11,#0xbc]
  00136080: ldr r0,[r0,#0x4]
  00136082: ldr.w r0,[r0,r8,lsl #0x2]
  00136086: cbz r0,0x001360b8
  00136088: ldr.w r1,[r0,#0x1a0]
  0013608c: cmp r1,#0x3f
  0013608e: bhi 0x001360b8
  00136090: ldr r0,[0x00136294]
  00136092: ldr.w r2,[r10,#0x0]
  00136096: add r0,pc
  00136098: ldr.w lr,[r11,#0x198]
  0013609c: ldr.w r12,[r11,#0x1bc]
  001360a0: ldrd r6,r2,[r2,#0x28]
  001360a4: ldr r0,[r0,#0x0]
  001360a6: add r6,lr
  001360a8: ldr r3,[sp,#0x14]
  001360aa: ldr r4,[sp,#0x40]
  001360ac: add r2,r6
  001360ae: ldr r0,[r0,#0x0]
  001360b0: add r2,r12
  001360b2: add r3,r4
  001360b4: blx 0x000765e8
  001360b8: ldr.w r0,[r11,#0x100]
  001360bc: ldr.w r1,[r10,#0x0]
  001360c0: ldr.w lr,[r11,#0x198]
  001360c4: ldr r0,[r0,#0x4]
  001360c6: ldr.w r12,[r11,#0x1bc]
  001360ca: ldr.w r0,[r0,r8,lsl #0x2]
  001360ce: ldr r0,[r0,#0x4]
  001360d0: ldrd r6,r3,[r1,#0x28]
  001360d4: ldr.w r10,[sp,#0x34]
  001360d8: add r6,lr
  001360da: ldr r4,[sp,#0x3c]
  001360dc: add.w r3,r6,r3, lsl #0x1
  001360e0: ldr.w r5,[r1,#0x2c4]
  001360e4: ldr r2,[r0,#0x4]
  001360e6: add r3,r12
  001360e8: str r4,[sp,#0x0]
  001360ea: movs r4,#0x0
  001360ec: str r4,[sp,#0x4]
  001360ee: add r3,r5
  001360f0: ldr.w r1,[r10,#0x0]
  001360f4: ldr.w r0,[r9,#0x0]
  001360f8: ldr r5,[0x00136298]
  001360fa: add r5,pc
  001360fc: ldr r6,[r5,#0x0]
  001360fe: str r6,[sp,#0x38]
  00136100: blx r6
  00136102: ldr.w r0,[r9,#0x0]
  00136106: ldr r1,[0x00136278]
  00136108: blx 0x0006fac0
  0013610c: ldr.w r0,[r11,#0x100]
  00136110: ldr r1,[sp,#0x40]
  00136112: ldr.w r12,[r11,#0x198]
  00136116: ldr r0,[r0,#0x4]
  00136118: ldr.w r0,[r0,r8,lsl #0x2]
  0013611c: ldr r0,[r0,#0x4]
  0013611e: ldr r2,[r0,#0x8]
  00136120: ldr r0,[sp,#0x24]
  00136122: adds r4,r1,r0
  00136124: ldr r0,[sp,#0x48]
  00136126: ldr r0,[r0,#0x0]
  00136128: ldrd r6,r5,[r0,#0x28]
  0013612c: ldr r0,[r0,#0x70]
  0013612e: ldr.w r1,[r10,#0x0]
  00136132: str r4,[sp,#0x30]
  00136134: add.w r0,r0,r0, lsr #0x1f
  00136138: str.w r8,[sp,#0x40]
  0013613c: add.w r3,r4,r0, asr #0x1
  00136140: str r3,[sp,#0x0]
  00136142: movs r3,#0x0
  00136144: ldr.w r0,[r9,#0x0]
  00136148: str r3,[sp,#0x4]
  0013614a: add.w r3,r6,r12
  0013614e: add r3,r5
  00136150: ldr r5,[sp,#0x38]
  00136152: blx r5
  00136154: ldr.w r0,[r11,#0x100]
  00136158: ldr r1,[sp,#0x40]
  0013615a: ldr.w lr,[r11,#0x198]
  0013615e: ldr r0,[r0,#0x4]
  00136160: ldr.w r12,[r11,#0x1bc]
  00136164: ldr.w r0,[r0,r1,lsl #0x2]
  00136168: ldr r0,[r0,#0x4]
  0013616a: ldr r2,[r0,#0xc]
  0013616c: ldr r0,[sp,#0x48]
  0013616e: ldr r0,[r0,#0x0]
  00136170: ldrd r8,r6,[r0,#0x28]
  00136174: ldr r1,[r0,#0x70]
  00136176: ldr.w r9,[r0,#0x2c4]
  0013617a: ldr r0,[sp,#0x44]
  0013617c: add.w r1,r1,r1, lsr #0x1f
  00136180: add.w r3,r4,r1, asr #0x1
  00136184: str r3,[sp,#0x0]
  00136186: movs r3,#0x0
  00136188: ldr.w r1,[r10,#0x0]
  0013618c: str r3,[sp,#0x4]
  0013618e: add.w r3,r8,lr
  00136192: add.w r3,r3,r6, lsl #0x1
  00136196: ldr r0,[r0,#0x0]
  00136198: add r3,r12
  0013619a: add r3,r9
  0013619c: blx r5
  0013619e: ldr.w r0,[r11,#0x100]
  001361a2: mov r5,r10
  001361a4: ldr.w r8,[sp,#0x40]
  001361a8: ldr.w r12,[r11,#0x198]
  001361ac: ldr r0,[r0,#0x4]
  001361ae: ldr.w lr,[r11,#0x1b8]
  001361b2: ldr.w r0,[r0,r8,lsl #0x2]
  001361b6: ldr r0,[r0,#0x4]
  001361b8: ldr r2,[r0,#0x10]
  001361ba: ldr r0,[sp,#0x48]
  001361bc: ldr r0,[r0,#0x0]
  001361be: ldr.w r9,[r0,#0x2c4]
  001361c2: ldrd r3,r6,[r0,#0x28]
  001361c6: ldr r0,[sp,#0x44]
  001361c8: add r6,lr
  001361ca: add r3,r12
  001361cc: ldr.w r1,[r10,#0x0]
  001361d0: add r3,r9
  001361d2: ldr.w r10,[sp,#0x38]
  001361d6: ldr r0,[r0,#0x0]
  001361d8: add.w r3,r3,r6, lsl #0x1
  001361dc: ldr r4,[sp,#0x3c]
  001361de: str r4,[sp,#0x0]
  001361e0: movs r4,#0x0
  001361e2: str r4,[sp,#0x4]
  001361e4: blx r10
  001361e6: ldr.w r0,[r11,#0x100]
  001361ea: ldr r3,[sp,#0x48]
  001361ec: ldr r1,[r5,#0x0]
  001361ee: ldr r0,[r0,#0x4]
  001361f0: ldr r5,[r3,#0x0]
  001361f2: ldr.w r12,[r11,#0x198]
  001361f6: ldr.w r0,[r0,r8,lsl #0x2]
  001361fa: ldr.w lr,[r11,#0x1b8]
  001361fe: ldr.w r9,[r5,#0x2c4]
  00136202: ldr r0,[r0,#0x4]
  00136204: ldr r2,[r0,#0x14]
  00136206: ldr r0,[sp,#0x44]
  00136208: ldr r0,[r0,#0x0]
  0013620a: ldrd r6,r3,[r5,#0x28]
  0013620e: ldr r5,[r5,#0x70]
  00136210: add r3,lr
  00136212: add r6,r12
  00136214: ldr r4,[sp,#0x30]
  00136216: add r6,r9
  00136218: ldr.w r9,[sp,#0x44]
  0013621c: add.w r5,r5,r5, lsr #0x1f
  00136220: add.w r3,r6,r3, lsl #0x1
  00136224: add.w r5,r4,r5, asr #0x1
  00136228: str r5,[sp,#0x0]
  0013622a: movs r5,#0x0
  0013622c: ldr r4,[sp,#0x48]
  0013622e: str r5,[sp,#0x4]
  00136230: blx r10
  00136232: ldr.w r0,[r11,#0x18c]
  00136236: mov r10,r4
  00136238: ldr r5,[sp,#0x18]
  0013623a: cmp r8,r0
  0013623c: itt eq
  0013623e: ldr.eq.w r0,[r11,#0xc4]
  00136242: blx.eq 0x000746e0
  00136246: add.w r8,r8,#0x1
  0013624a: ldr r0,[r5,#0x0]
  0013624c: cmp r8,r0
  0013624e: blt.w 0x00135fb8
  00136252: ldr r0,[sp,#0x58]
  00136254: ldr r1,[sp,#0x10]
  00136256: ldr r1,[r1,#0x0]
  00136258: subs r0,r1,r0
  0013625a: ittt eq
  0013625c: add.eq sp,#0x5c
  0013625e: pop.eq.w {r8,r9,r10,r11}
  00136262: pop.eq {r4,r5,r6,r7,pc}
  00136264: blx 0x0006e824
```
