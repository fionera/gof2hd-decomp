# Layout::reload
elf 0xd2c20 body 1008
Sig: undefined __stdcall reload(void)

## decompile
```c

/* Layout::reload() */

void Layout::reload(void)

{
  Layout *in_r0;
  int *piVar1;
  uint uVar2;
  TouchButton *pTVar3;
  String *pSVar4;
  int iVar5;
  undefined4 uVar6;
  code *pcVar7;
  uint *puVar8;
  int *piVar9;
  int *piVar10;
  uint local_30;
  uint local_2c;
  int local_28;
  
  piVar1 = *(int **)(DAT_000e3018 + 0xe2c38);
  local_28 = *piVar1;
  *(undefined4 *)(in_r0 + 0x324) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x328) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x32c) = 0xffffffff;
  *(undefined4 *)(in_r0 + 0x3a8) = 0xffffffff;
  __aeabi_memset4(in_r0 + 0x334,0x70,0xff);
  puVar8 = *(uint **)(DAT_000e302c + 0xe2c62);
  pcVar7 = *(code **)(DAT_000e3030 + 0xe2c64);
  (*pcVar7)(*puVar8,0x503,in_r0 + 0x398);
  (*pcVar7)(*puVar8,0x47e,in_r0 + 0x324);
  (*pcVar7)(*puVar8,0x4ff,in_r0 + 0x328);
  (*pcVar7)(*puVar8,0x500,in_r0 + 0x330);
  (*pcVar7)(*puVar8,0x474,in_r0 + 0x32c);
  (*pcVar7)(*puVar8,0x502,in_r0 + 0x334);
  (*pcVar7)(*puVar8,0x506,in_r0 + 0x340);
  (*pcVar7)(*puVar8,0x501,in_r0 + 0x338);
  (*pcVar7)(*puVar8,0x507,in_r0 + 0x344);
  (*pcVar7)(*puVar8,0x4fe,in_r0 + 0x33c);
  (*pcVar7)(*puVar8,0x482,in_r0 + 0x348);
  (*pcVar7)(*puVar8,0x481,in_r0 + 0x34c);
  (*pcVar7)(*puVar8,0x486,in_r0 + 0x378);
  (*pcVar7)(*puVar8,0x487,in_r0 + 0x374);
  (*pcVar7)(*puVar8,0x48b,in_r0 + 0x37c);
  (*pcVar7)(*puVar8,0x52d,in_r0 + 0x3a4);
  if (**(char **)(DAT_000e3034 + 0xe2da8) == '\0') {
    (*pcVar7)(*puVar8,0x480,in_r0 + 0x350);
    (*pcVar7)(*puVar8,0x47f,in_r0 + 0x354);
    (*pcVar7)(*puVar8,0x479,in_r0 + 0x358);
    (*pcVar7)(*puVar8,0x478,in_r0 + 0x35c);
    (*pcVar7)(*puVar8,0x489,in_r0 + 0x36c);
    uVar2 = *puVar8;
    uVar6 = 0x488;
  }
  else {
    (*pcVar7)(*puVar8,0x6bb);
    (*pcVar7)(*puVar8,0x6ba,in_r0 + 0x354);
    (*pcVar7)(*puVar8,0x6b9,in_r0 + 0x358);
    (*pcVar7)(*puVar8,0x6b8,in_r0 + 0x35c);
    (*pcVar7)(*puVar8,0x6b7,in_r0 + 0x36c);
    uVar2 = *puVar8;
    uVar6 = 0x6bc;
  }
  (*pcVar7)(uVar2,uVar6,in_r0 + 0x370);
  (*pcVar7)(*puVar8,0x530,in_r0 + 0x360);
  (*pcVar7)(*puVar8,0x531,in_r0 + 0x364);
  (*pcVar7)(*puVar8,0x52f,in_r0 + 0x368);
  (*pcVar7)(*puVar8,0x47c,in_r0 + 900);
  (*pcVar7)(*puVar8,0x47d,in_r0 + 0x380);
  (*pcVar7)(*puVar8,0x47b,in_r0 + 0x388);
  (*pcVar7)(*puVar8,0x47a,in_r0 + 0x38c);
  (*pcVar7)(*puVar8,0x484,in_r0 + 0x390);
  (*pcVar7)(*puVar8,0x483,in_r0 + 0x394);
  (*pcVar7)(*puVar8,0x50c,in_r0 + 0x3a0);
  (*pcVar7)(*puVar8,0x50d,in_r0 + 0x39c);
  pTVar3 = operator_new(200);
  piVar10 = *(int **)(DAT_000e3038 + 0xe2ec0);
  pSVar4 = (String *)GameText::getText(*piVar10);
  piVar9 = *(int **)(DAT_000e303c + 0xe2ed6);
  TouchButton::TouchButton(pTVar3,pSVar4,2,*(int *)(in_r0 + 0x28),*piVar9 + -3,'!');
  *(TouchButton **)(in_r0 + 0x3b4) = pTVar3;
  uVar6 = TouchButton::getWidth();
  *(undefined4 *)(in_r0 + 0x2f0) = uVar6;
  local_2c = 0xffffffff;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar8,0x535,&local_2c);
  uVar2 = local_2c;
  pTVar3 = operator_new(200);
  if (uVar2 == 0xffffffff) {
    pSVar4 = (String *)GameText::getText(*piVar10);
    TouchButton::TouchButton
              (pTVar3,pSVar4,2,*(int *)(in_r0 + 0x28),*piVar9 - *(int *)(in_r0 + 0x3fc),'!');
  }
  else {
    TouchButton::TouchButton
              (pTVar3,uVar2,2,*(int *)(in_r0 + 0x28),*piVar9 - *(int *)(in_r0 + 0x3fc),'!');
  }
  *(TouchButton **)(in_r0 + 0x3b8) = pTVar3;
  local_30 = 0xffffffff;
  AbyssEngine::PaintCanvas::Image2DCreate((PaintCanvas *)*puVar8,0x471,&local_30);
  pTVar3 = operator_new(200);
  TouchButton::TouchButton
            (pTVar3,local_30,1,**(int **)(DAT_000e3040 + 0xe2f8a),0,*(int *)(in_r0 + 0x3c),'\x12',
             '\x04');
  *(TouchButton **)(in_r0 + 0x3bc) = pTVar3;
  iVar5 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar8);
  *in_r0 = (Layout)0x0;
  *(undefined4 *)(in_r0 + 0x3c4) = 0;
  *(undefined4 *)(in_r0 + 0x3c8) = 0;
  *(int *)(in_r0 + 0x3ac) = iVar5 / 2 + -1;
  *(undefined4 *)(in_r0 + 0x3b0) = 0xffffffff;
  resetWindowDimensions(in_r0);
  *(undefined4 *)(in_r0 + 0x3d0) = 0;
  in_r0[0x3cc] = (Layout)0x0;
  *(undefined4 *)(in_r0 + 0x3d8) = 0;
  *(undefined2 *)(in_r0 + 0x400) = 0;
  in_r0[0x3dc] = (Layout)0x0;
  *(undefined4 *)(in_r0 + 0x404) = 0;
  *(undefined4 *)(in_r0 + 0x408) = 0;
  *(undefined2 *)(in_r0 + 0x2ec) = 0;
  *(undefined4 *)(in_r0 + 0x40d) = 0;
  *(undefined4 *)(in_r0 + 0x409) = 0;
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e2c20: push {r4,r5,r6,r7,lr}
  000e2c22: add r7,sp,#0xc
  000e2c24: push {r8,r9,r10,r11}
  000e2c28: sub sp,#0x54
  000e2c2a: mov r4,r0
  000e2c2c: ldr r0,[0x000e3018]
  000e2c2e: add.w r5,r4,#0x334
  000e2c32: movs r1,#0x70
  000e2c34: add r0,pc
  000e2c36: movs r2,#0xff
  000e2c38: ldr r0,[r0,#0x0]
  000e2c3a: str r0,[sp,#0x14]
  000e2c3c: ldr r0,[r0,#0x0]
  000e2c3e: str r0,[sp,#0x50]
  000e2c40: mov.w r0,#0xffffffff
  000e2c44: strd r0,r0,[r4,#0x324]
  000e2c48: str.w r0,[r4,#0x32c]
  000e2c4c: str.w r0,[r4,#0x3a8]
  000e2c50: mov r0,r5
  000e2c52: blx 0x00074d94
  000e2c56: ldr r0,[0x000e302c]
  000e2c58: add.w r2,r4,#0x398
  000e2c5c: ldr r1,[0x000e3030]
  000e2c5e: add r0,pc
  000e2c60: add r1,pc
  000e2c62: ldr.w r8,[r0,#0x0]
  000e2c66: ldr r6,[r1,#0x0]
  000e2c68: movw r1,#0x503
  000e2c6c: ldr.w r0,[r8,#0x0]
  000e2c70: blx r6
  000e2c72: ldr.w r0,[r8,#0x0]
  000e2c76: add.w r2,r4,#0x324
  000e2c7a: movw r1,#0x47e
  000e2c7e: blx r6
  000e2c80: ldr.w r0,[r8,#0x0]
  000e2c84: add.w r2,r4,#0x328
  000e2c88: movw r1,#0x4ff
  000e2c8c: blx r6
  000e2c8e: ldr.w r0,[r8,#0x0]
  000e2c92: add.w r2,r4,#0x330
  000e2c96: mov.w r1,#0x500
  000e2c9a: blx r6
  000e2c9c: ldr.w r0,[r8,#0x0]
  000e2ca0: add.w r2,r4,#0x32c
  000e2ca4: movw r1,#0x474
  000e2ca8: blx r6
  000e2caa: ldr.w r0,[r8,#0x0]
  000e2cae: movw r1,#0x502
  000e2cb2: mov r2,r5
  000e2cb4: blx r6
  000e2cb6: ldr.w r0,[r8,#0x0]
  000e2cba: add.w r2,r4,#0x340
  000e2cbe: movw r1,#0x506
  000e2cc2: blx r6
  000e2cc4: ldr.w r0,[r8,#0x0]
  000e2cc8: add.w r2,r4,#0x338
  000e2ccc: movw r1,#0x501
  000e2cd0: blx r6
  000e2cd2: ldr.w r0,[r8,#0x0]
  000e2cd6: add.w r2,r4,#0x344
  000e2cda: movw r1,#0x507
  000e2cde: blx r6
  000e2ce0: ldr.w r0,[r8,#0x0]
  000e2ce4: add.w r2,r4,#0x33c
  000e2ce8: movw r1,#0x4fe
  000e2cec: blx r6
  000e2cee: ldr.w r0,[r8,#0x0]
  000e2cf2: add.w r2,r4,#0x348
  000e2cf6: movw r1,#0x482
  000e2cfa: blx r6
  000e2cfc: ldr.w r0,[r8,#0x0]
  000e2d00: add.w r2,r4,#0x34c
  000e2d04: movw r1,#0x481
  000e2d08: blx r6
  000e2d0a: ldr.w r0,[r8,#0x0]
  000e2d0e: add.w r2,r4,#0x378
  000e2d12: movw r1,#0x486
  000e2d16: blx r6
  000e2d18: ldr.w r0,[r8,#0x0]
  000e2d1c: add.w r2,r4,#0x374
  000e2d20: movw r1,#0x487
  000e2d24: blx r6
  000e2d26: ldr.w r0,[r8,#0x0]
  000e2d2a: add.w r2,r4,#0x37c
  000e2d2e: movw r1,#0x48b
  000e2d32: blx r6
  000e2d34: ldr.w r0,[r8,#0x0]
  000e2d38: add.w r2,r4,#0x3a4
  000e2d3c: movw r1,#0x52d
  000e2d40: blx r6
  000e2d42: add.w r1,r4,#0x38c
  000e2d46: str r1,[sp,#0x30]
  000e2d48: add.w r1,r4,#0x380
  000e2d4c: add.w r0,r4,#0x39c
  000e2d50: str r1,[sp,#0x28]
  000e2d52: add.w r1,r4,#0x384
  000e2d56: str r0,[sp,#0x40]
  000e2d58: add.w r0,r4,#0x3a0
  000e2d5c: str r1,[sp,#0x24]
  000e2d5e: add.w r1,r4,#0x360
  000e2d62: str r0,[sp,#0x3c]
  000e2d64: add.w r0,r4,#0x390
  000e2d68: str r1,[sp,#0x18]
  000e2d6a: add.w r1,r4,#0x364
  000e2d6e: str r0,[sp,#0x34]
  000e2d70: add.w r0,r4,#0x394
  000e2d74: str r1,[sp,#0x1c]
  000e2d76: add.w r1,r4,#0x368
  000e2d7a: str r0,[sp,#0x38]
  000e2d7c: add.w r0,r4,#0x388
  000e2d80: str r0,[sp,#0x2c]
  000e2d82: add.w r10,r4,#0x358
  000e2d86: str r1,[sp,#0x20]
  000e2d88: add.w r1,r4,#0x370
  000e2d8c: str r1,[sp,#0x44]
  000e2d8e: add.w r9,r4,#0x35c
  000e2d92: ldr.w r0,[r8,#0x0]
  000e2d96: add.w r5,r4,#0x36c
  000e2d9a: ldr r1,[0x000e3034]
  000e2d9c: add.w r2,r4,#0x350
  000e2da0: add.w r11,r4,#0x354
  000e2da4: add r1,pc
  000e2da6: ldr r1,[r1,#0x0]
  000e2da8: ldrb r1,[r1,#0x0]
  000e2daa: cbz r1,0x000e2dec
  000e2dac: movw r1,#0x6bb
  000e2db0: blx r6
  000e2db2: ldr.w r0,[r8,#0x0]
  000e2db6: movw r1,#0x6ba
  000e2dba: mov r2,r11
  000e2dbc: blx r6
  000e2dbe: ldr.w r0,[r8,#0x0]
  000e2dc2: movw r1,#0x6b9
  000e2dc6: mov r2,r10
  000e2dc8: blx r6
  000e2dca: ldr.w r0,[r8,#0x0]
  000e2dce: mov.w r1,#0x6b8
  000e2dd2: mov r2,r9
  000e2dd4: blx r6
  000e2dd6: ldr.w r0,[r8,#0x0]
  000e2dda: movw r1,#0x6b7
  000e2dde: mov r2,r5
  000e2de0: blx r6
  000e2de2: ldr.w r0,[r8,#0x0]
  000e2de6: movw r1,#0x6bc
  000e2dea: b 0x000e2e2a
  000e2dec: mov.w r1,#0x480
  000e2df0: blx r6
  000e2df2: ldr.w r0,[r8,#0x0]
  000e2df6: movw r1,#0x47f
  000e2dfa: mov r2,r11
  000e2dfc: blx r6
  000e2dfe: ldr.w r0,[r8,#0x0]
  000e2e02: movw r1,#0x479
  000e2e06: mov r2,r10
  000e2e08: blx r6
  000e2e0a: ldr.w r0,[r8,#0x0]
  000e2e0e: mov.w r1,#0x478
  000e2e12: mov r2,r9
  000e2e14: blx r6
  000e2e16: ldr.w r0,[r8,#0x0]
  000e2e1a: movw r1,#0x489
  000e2e1e: mov r2,r5
  000e2e20: blx r6
  000e2e22: ldr.w r0,[r8,#0x0]
  000e2e26: mov.w r1,#0x488
  000e2e2a: ldr r2,[sp,#0x44]
  000e2e2c: blx r6
  000e2e2e: ldr.w r0,[r8,#0x0]
  000e2e32: mov.w r1,#0x530
  000e2e36: ldr r2,[sp,#0x18]
  000e2e38: blx r6
  000e2e3a: ldr.w r0,[r8,#0x0]
  000e2e3e: movw r1,#0x531
  000e2e42: ldr r2,[sp,#0x1c]
  000e2e44: blx r6
  000e2e46: ldr.w r0,[r8,#0x0]
  000e2e4a: movw r1,#0x52f
  000e2e4e: ldr r2,[sp,#0x20]
  000e2e50: blx r6
  000e2e52: ldr.w r0,[r8,#0x0]
  000e2e56: movw r1,#0x47c
  000e2e5a: ldr r2,[sp,#0x24]
  000e2e5c: blx r6
  000e2e5e: ldr.w r0,[r8,#0x0]
  000e2e62: movw r1,#0x47d
  000e2e66: ldr r2,[sp,#0x28]
  000e2e68: blx r6
  000e2e6a: ldr.w r0,[r8,#0x0]
  000e2e6e: movw r1,#0x47b
  000e2e72: ldr r2,[sp,#0x2c]
  000e2e74: blx r6
  000e2e76: ldr.w r0,[r8,#0x0]
  000e2e7a: movw r1,#0x47a
  000e2e7e: ldr r2,[sp,#0x30]
  000e2e80: blx r6
  000e2e82: ldr.w r0,[r8,#0x0]
  000e2e86: movw r1,#0x484
  000e2e8a: ldr r2,[sp,#0x34]
  000e2e8c: blx r6
  000e2e8e: ldr.w r0,[r8,#0x0]
  000e2e92: movw r1,#0x483
  000e2e96: ldr r2,[sp,#0x38]
  000e2e98: blx r6
  000e2e9a: ldr.w r0,[r8,#0x0]
  000e2e9e: movw r1,#0x50c
  000e2ea2: ldr r2,[sp,#0x3c]
  000e2ea4: blx r6
  000e2ea6: ldr.w r0,[r8,#0x0]
  000e2eaa: movw r1,#0x50d
  000e2eae: ldr r2,[sp,#0x40]
  000e2eb0: blx r6
  000e2eb2: movs r0,#0xc8
  000e2eb4: blx 0x0006eb24
  000e2eb8: mov r5,r0
  000e2eba: ldr r0,[0x000e3038]
  000e2ebc: add r0,pc
  000e2ebe: ldr.w r10,[r0,#0x0]
  000e2ec2: ldr.w r0,[r10,#0x0]
  000e2ec6: movs r1,#0xaa
  000e2ec8: blx 0x00072f70
  000e2ecc: mov r1,r0
  000e2ece: ldr r0,[0x000e303c]
  000e2ed0: ldr r3,[r4,#0x28]
  000e2ed2: add r0,pc
  000e2ed4: ldr.w r9,[r0,#0x0]
  000e2ed8: ldr.w r0,[r9,#0x0]
  000e2edc: movs r2,#0x21
  000e2ede: subs r0,#0x3
  000e2ee0: strd r0,r2,[sp,#0x0]
  000e2ee4: mov r0,r5
  000e2ee6: movs r2,#0x2
  000e2ee8: blx 0x00074578
  000e2eec: mov r0,r5
  000e2eee: str.w r5,[r4,#0x3b4]
  000e2ef2: blx 0x00074da0
  000e2ef6: str.w r0,[r4,#0x2f0]
  000e2efa: mov.w r1,#0xffffffff
  000e2efe: ldr.w r0,[r8,#0x0]
  000e2f02: add r2,sp,#0x4c
  000e2f04: str r1,[sp,#0x4c]
  000e2f06: movw r1,#0x535
  000e2f0a: blx 0x00071cf8
  000e2f0e: movs r0,#0xc8
  000e2f10: ldr r6,[sp,#0x4c]
  000e2f12: blx 0x0006eb24
  000e2f16: mov r5,r0
  000e2f18: adds r0,r6,#0x1
  000e2f1a: beq 0x000e2f3a
  000e2f1c: ldr.w r0,[r4,#0x3fc]
  000e2f20: ldr.w r1,[r9,#0x0]
  000e2f24: ldr r3,[r4,#0x28]
  000e2f26: subs r0,r1,r0
  000e2f28: movs r2,#0x21
  000e2f2a: strd r0,r2,[sp,#0x0]
  000e2f2e: mov r0,r5
  000e2f30: mov r1,r6
  000e2f32: movs r2,#0x2
  000e2f34: blx 0x00074dac
  000e2f38: b 0x000e2f60
  000e2f3a: ldr.w r0,[r10,#0x0]
  000e2f3e: movs r1,#0xab
  000e2f40: blx 0x00072f70
  000e2f44: mov r1,r0
  000e2f46: ldr.w r0,[r4,#0x3fc]
  000e2f4a: ldr r3,[r4,#0x28]
  000e2f4c: ldr.w r2,[r9,#0x0]
  000e2f50: subs r0,r2,r0
  000e2f52: movs r6,#0x21
  000e2f54: strd r0,r6,[sp,#0x0]
  000e2f58: mov r0,r5
  000e2f5a: movs r2,#0x2
  000e2f5c: blx 0x00074578
  000e2f60: str.w r5,[r4,#0x3b8]
  000e2f64: add r2,sp,#0x48
  000e2f66: ldr.w r0,[r8,#0x0]
  000e2f6a: mov.w r9,#0xffffffff
  000e2f6e: movw r1,#0x471
  000e2f72: str.w r9,[sp,#0x48]
  000e2f76: blx 0x00071cf8
  000e2f7a: movs r0,#0xc8
  000e2f7c: blx 0x0006eb24
  000e2f80: mov r5,r0
  000e2f82: ldr r0,[0x000e3040]
  000e2f84: ldr r1,[sp,#0x48]
  000e2f86: add r0,pc
  000e2f88: ldr r0,[r0,#0x0]
  000e2f8a: ldr r3,[r0,#0x0]
  000e2f8c: ldr r0,[r4,#0x3c]
  000e2f8e: movs r6,#0x0
  000e2f90: mov.w r12,#0x4
  000e2f94: movs r2,#0x12
  000e2f96: strd r6,r0,[sp,#0x0]
  000e2f9a: strd r2,r12,[sp,#0x8]
  000e2f9e: mov r0,r5
  000e2fa0: movs r2,#0x1
  000e2fa2: blx 0x00074db8
  000e2fa6: ldr r0,[0x000e3044]
  000e2fa8: str.w r5,[r4,#0x3bc]
  000e2fac: add r0,pc
  000e2fae: ldr r0,[r0,#0x0]
  000e2fb0: ldr r1,[r0,#0x0]
  000e2fb2: ldr.w r0,[r8,#0x0]
  000e2fb6: blx 0x0006fe20
  000e2fba: add.w r0,r0,r0, lsr #0x1f
  000e2fbe: strb r6,[r4,#0x0]
  000e2fc0: strd r6,r6,[r4,#0x3c4]
  000e2fc4: add.w r0,r9,r0, asr #0x1
  000e2fc8: strd r0,r9,[r4,#0x3ac]
  000e2fcc: mov r0,r4
  000e2fce: blx 0x00074dc4
  000e2fd2: str.w r6,[r4,#0x3d0]
  000e2fd6: strb.w r6,[r4,#0x3cc]
  000e2fda: str.w r6,[r4,#0x3d8]
  000e2fde: strh.w r6,[r4,#0x400]
  000e2fe2: strb.w r6,[r4,#0x3dc]
  000e2fe6: str.w r6,[r4,#0x404]
  000e2fea: str.w r6,[r4,#0x408]
  000e2fee: strh.w r6,[r4,#0x2ec]
  000e2ff2: str.w r6,[r4,#0x40d]
  000e2ff6: str.w r6,[r4,#0x409]
  000e2ffa: ldr r0,[sp,#0x50]
  000e2ffc: ldr r1,[sp,#0x14]
  000e2ffe: ldr r1,[r1,#0x0]
  000e3000: subs r0,r1,r0
  000e3002: ittt eq
  000e3004: add.eq sp,#0x54
  000e3006: pop.eq.w {r8,r9,r10,r11}
  000e300a: pop.eq {r4,r5,r6,r7,pc}
  000e300c: blx 0x0006e824
```
