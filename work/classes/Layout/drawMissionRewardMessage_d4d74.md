# Layout::drawMissionRewardMessage
elf 0xd4d74 body 566
Sig: undefined __stdcall drawMissionRewardMessage(bool param_1)

## decompile
```c

/* Layout::drawMissionRewardMessage(bool) */

void Layout::drawMissionRewardMessage(bool param_1)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  String *pSVar5;
  undefined4 *puVar6;
  int in_r1;
  int *piVar7;
  undefined4 uVar8;
  uchar uVar9;
  undefined4 uVar10;
  code *pcVar11;
  uint uVar12;
  uint *puVar13;
  undefined4 uVar14;
  int iVar15;
  int *piVar16;
  uint in_fpscr;
  undefined1 auStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  uVar1 = (uint)param_1;
  piVar7 = *(int **)(DAT_000e4fec + 0xe4d88);
  local_28 = *piVar7;
  if (*(char *)(uVar1 + 0x2ec) == '\0') goto LAB_000e4f96;
  puVar13 = *(uint **)(DAT_000e4ff0 + 0xe4d9e);
  AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar13);
  uVar2 = *(undefined4 *)(uVar1 + 0x3b0);
  AbyssEngine::PaintCanvas::SetColor(*puVar13);
  iVar3 = *(int *)(uVar1 + 0x3d0);
  if (iVar3 < 2000) {
LAB_000e4dd4:
    VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  }
  else if (5000 < iVar3) {
    iVar3 = 7000 - iVar3;
    goto LAB_000e4dd4;
  }
  AbyssEngine::PaintCanvas::SetColor(*puVar13);
  uVar4 = AbyssEngine::PaintCanvas::GetColor((PaintCanvas *)*puVar13);
  uVar14 = *(undefined4 *)(uVar1 + 1000);
  iVar3 = *(int *)(uVar1 + 0x3ec);
  uVar10 = *(undefined4 *)(uVar1 + 0x3f0);
  uVar8 = *(undefined4 *)(uVar1 + 0x3f4);
  *(undefined4 *)(uVar1 + 0x3b0) = uVar4;
  if (in_r1 != 0) {
    piVar16 = *(int **)(DAT_000e4ff4 + 0xe4e2a);
    iVar15 = *piVar16;
    uVar4 = AbyssEngine::String::String(aSStack_34,(char *)(DAT_000e4ff8 + 0xe4e38),false);
    drawBox(uVar1,2,(iVar15 >> 1) - (iVar3 >> 1),uVar10,iVar3,uVar14,uVar4);
    AbyssEngine::String::~String(aSStack_34);
    iVar15 = *piVar16;
    uVar4 = AbyssEngine::String::String(aSStack_40,(char *)(DAT_000e4ffc + 0xe4e6a),false);
    drawBox(uVar1,8,(iVar15 >> 1) - (iVar3 >> 1),uVar10,iVar3,uVar14,uVar4);
    AbyssEngine::String::~String(aSStack_40);
  }
  piVar16 = *(int **)(DAT_000e5000 + 0xe4e94);
  uVar9 = (uchar)uVar10;
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar13,*(int *)(uVar1 + 0x3a4),*piVar16 >> 1,uVar9,'\x11')
  ;
  pSVar5 = (String *)GameText::getText(**(int **)(DAT_000e5004 + 0xe4eba));
  AbyssEngine::String::String(aSStack_4c,pSVar5,false);
  iVar15 = *piVar16;
  uVar12 = *puVar13;
  puVar6 = *(undefined4 **)(DAT_000e5008 + 0xe4ede);
  pSVar5 = (String *)*puVar6;
  iVar3 = AbyssEngine::PaintCanvas::GetTextWidth(uVar12,pSVar5);
  AbyssEngine::PaintCanvas::DrawString
            (uVar12,pSVar5,(int)aSStack_4c,(iVar15 >> 1) - (iVar3 >> 1),(bool)(uVar9 + (char)uVar8))
  ;
  AbyssEngine::String::String(aSStack_64,(char *)(DAT_000e500c + 0xe4f14),false);
  formatCredits((int)auStack_70);
  AbyssEngine::operator+(aSStack_58,aSStack_64);
  AbyssEngine::String::operator=(aSStack_4c,aSStack_58);
  pcVar11 = *(code **)(DAT_000e5010 + 0xe4f3c);
  (*pcVar11)(aSStack_58);
  (*pcVar11)(auStack_70);
  (*pcVar11)(aSStack_64);
  iVar15 = *piVar16;
  uVar12 = *puVar13;
  pSVar5 = (String *)*puVar6;
  iVar3 = AbyssEngine::PaintCanvas::GetTextWidth(uVar12,pSVar5);
  AbyssEngine::PaintCanvas::DrawString
            (uVar12,pSVar5,(int)aSStack_4c,(iVar15 >> 1) - (iVar3 >> 1),
             (bool)((char)*(undefined4 *)(uVar1 + 0x3f8) + uVar9));
  AbyssEngine::PaintCanvas::SetColor(*puVar13);
  *(undefined4 *)(uVar1 + 0x3b0) = uVar2;
  AbyssEngine::String::~String(aSStack_4c);
LAB_000e4f96:
  if (*piVar7 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e4d74: push {r4,r5,r6,r7,lr}
  000e4d76: add r7,sp,#0xc
  000e4d78: push {r8,r9,r10,r11}
  000e4d7c: sub sp,#0x74
  000e4d7e: mov r4,r0
  000e4d80: ldr r0,[0x000e4fec]
  000e4d82: mov r10,r1
  000e4d84: add r0,pc
  000e4d86: ldr r1,[r0,#0x0]
  000e4d88: ldr r0,[r1,#0x0]
  000e4d8a: str r0,[sp,#0x70]
  000e4d8c: ldrb.w r0,[r4,#0x2ec]
  000e4d90: cmp r0,#0x0
  000e4d92: beq.w 0x000e4f96
  000e4d96: str r1,[sp,#0x14]
  000e4d98: ldr r0,[0x000e4ff0]
  000e4d9a: add r0,pc
  000e4d9c: ldr.w r8,[r0,#0x0]
  000e4da0: ldr.w r0,[r8,#0x0]
  000e4da4: blx 0x0006fa90
  000e4da8: str r0,[sp,#0x18]
  000e4daa: mov.w r1,#0xffffffff
  000e4dae: ldr.w r0,[r4,#0x3b0]
  000e4db2: str r0,[sp,#0x10]
  000e4db4: ldr.w r0,[r8,#0x0]
  000e4db8: blx 0x0006fac0
  000e4dbc: ldr.w r0,[r4,#0x3d0]
  000e4dc0: cmp.w r0,#0x7d0
  000e4dc4: blt 0x000e4dd4
  000e4dc6: movw r1,#0x1389
  000e4dca: cmp r0,r1
  000e4dcc: blt 0x000e4de6
  000e4dce: movw r1,#0x1b58
  000e4dd2: subs r0,r1,r0
  000e4dd4: vmov s0,r0
  000e4dd8: vldr.32 s2,[pc,#0x208]
  000e4ddc: vcvt.f32.s32 s0,s0
  000e4de0: vdiv.f32 s0,s0,s2
  000e4de4: b 0x000e4dea
  000e4de6: vmov.f32 s0,0x3f800000
  000e4dea: vldr.32 s2,[pc,#0x1fc]
  000e4dee: vmul.f32 s0,s0,s2
  000e4df2: vcvt.s32.f32 s0,s0
  000e4df6: ldr.w r0,[r8,#0x0]
  000e4dfa: vmov r1,s0
  000e4dfe: sub.w r1,r1,#0x100
  000e4e02: blx 0x0006fac0
  000e4e06: ldr.w r0,[r8,#0x0]
  000e4e0a: blx 0x0006fa90
  000e4e0e: ldrd r9,r6,[r4,#0x3e8]
  000e4e12: cmp.w r10,#0x0
  000e4e16: ldrd r5,r1,[r4,#0x3f0]
  000e4e1a: str.w r0,[r4,#0x3b0]
  000e4e1e: str r1,[sp,#0x20]
  000e4e20: beq 0x000e4e86
  000e4e22: ldr r0,[0x000e4ff4]
  000e4e24: movs r2,#0x0
  000e4e26: add r0,pc
  000e4e28: ldr.w r11,[r0,#0x0]
  000e4e2c: add r0,sp,#0x64
  000e4e2e: ldr.w r10,[r11,#0x0]
  000e4e32: ldr r1,[0x000e4ff8]
  000e4e34: add r1,pc
  000e4e36: blx 0x0006ee18
  000e4e3a: strd r6,r9,[sp,#0x0]
  000e4e3e: movs r1,#0x2
  000e4e40: str r0,[sp,#0x8]
  000e4e42: asr.w r0,r10, asr #0x1
  000e4e46: sub.w r2,r0,r6, asr #0x1
  000e4e4a: mov r0,r4
  000e4e4c: mov r3,r5
  000e4e4e: blx 0x0007462c
  000e4e52: add r0,sp,#0x64
  000e4e54: asr.w r10,r6, asr #0x1
  000e4e58: blx 0x0006ee30
  000e4e5c: ldr.w r11,[r11,#0x0]
  000e4e60: add r0,sp,#0x58
  000e4e62: ldr r1,[0x000e4ffc]
  000e4e64: movs r2,#0x0
  000e4e66: add r1,pc
  000e4e68: blx 0x0006ee18
  000e4e6c: rsb r2,r10,r11, asr #0x1
  000e4e70: strd r6,r9,[sp,#0x0]
  000e4e74: str r0,[sp,#0x8]
  000e4e76: mov r0,r4
  000e4e78: movs r1,#0x8
  000e4e7a: mov r3,r5
  000e4e7c: blx 0x0007462c
  000e4e80: add r0,sp,#0x58
  000e4e82: blx 0x0006ee30
  000e4e86: ldr r0,[0x000e5000]
  000e4e88: movs r3,#0x14
  000e4e8a: ldr.w r1,[r4,#0x3a4]
  000e4e8e: movs r6,#0x11
  000e4e90: add r0,pc
  000e4e92: ldr.w r11,[r0,#0x0]
  000e4e96: ldr.w r0,[r8,#0x0]
  000e4e9a: ldr.w r2,[r11,#0x0]
  000e4e9e: strd r6,r3,[sp,#0x0]
  000e4ea2: mov r3,r5
  000e4ea4: str r5,[sp,#0x24]
  000e4ea6: asrs r2,r2,#0x1
  000e4ea8: blx 0x00071d70
  000e4eac: ldr r0,[0x000e5004]
  000e4eae: movw r1,#0xc86
  000e4eb2: ldrb.w r2,[r4,#0x2ed]
  000e4eb6: add r0,pc
  000e4eb8: cmp r2,#0x0
  000e4eba: ldr r0,[r0,#0x0]
  000e4ebc: ldr r0,[r0,#0x0]
  000e4ebe: it eq
  000e4ec0: mov.eq r1,#0xd8
  000e4ec2: blx 0x00072f70
  000e4ec6: add.w r10,sp,#0x4c
  000e4eca: mov r1,r0
  000e4ecc: movs r2,#0x0
  000e4ece: mov r0,r10
  000e4ed0: blx 0x0006f028
  000e4ed4: ldr r0,[0x000e5008]
  000e4ed6: ldr.w r9,[r11,#0x0]
  000e4eda: add r0,pc
  000e4edc: ldr.w r6,[r8,#0x0]
  000e4ee0: ldr r0,[r0,#0x0]
  000e4ee2: str r0,[sp,#0x1c]
  000e4ee4: ldr r5,[r0,#0x0]
  000e4ee6: mov r0,r6
  000e4ee8: mov r1,r5
  000e4eea: mov r2,r10
  000e4eec: blx 0x0006faa8
  000e4ef0: ldrd r2,r1,[sp,#0x20]
  000e4ef4: add r1,r2
  000e4ef6: str r1,[sp,#0x0]
  000e4ef8: movs r1,#0x0
  000e4efa: add r2,sp,#0x4c
  000e4efc: str r1,[sp,#0x4]
  000e4efe: asr.w r1,r9, asr #0x1
  000e4f02: sub.w r3,r1,r0, asr #0x1
  000e4f06: mov r0,r6
  000e4f08: mov r1,r5
  000e4f0a: blx 0x0006fe14
  000e4f0e: ldr r1,[0x000e500c]
  000e4f10: add r1,pc
  000e4f12: add r0,sp,#0x34
  000e4f14: movs r2,#0x0
  000e4f16: blx 0x0006ee18
  000e4f1a: ldr.w r2,[r4,#0x3d4]
  000e4f1e: add r0,sp,#0x28
  000e4f20: blx 0x00074e54
  000e4f24: add r0,sp,#0x40
  000e4f26: add r1,sp,#0x34
  000e4f28: add r2,sp,#0x28
  000e4f2a: blx 0x0006ef98
  000e4f2e: add r0,sp,#0x4c
  000e4f30: add r1,sp,#0x40
  000e4f32: blx 0x0006f2b0
  000e4f36: ldr r0,[0x000e5010]
  000e4f38: add r0,pc
  000e4f3a: ldr r5,[r0,#0x0]
  000e4f3c: add r0,sp,#0x40
  000e4f3e: blx r5
  000e4f40: add r0,sp,#0x28
  000e4f42: blx r5
  000e4f44: add r0,sp,#0x34
  000e4f46: blx r5
  000e4f48: ldr r0,[sp,#0x1c]
  000e4f4a: ldr.w r10,[r11,#0x0]
  000e4f4e: ldr.w r6,[r8,#0x0]
  000e4f52: ldr r5,[r0,#0x0]
  000e4f54: add r2,sp,#0x4c
  000e4f56: mov r0,r6
  000e4f58: mov r1,r5
  000e4f5a: blx 0x0006faa8
  000e4f5e: ldr.w r1,[r4,#0x3f8]
  000e4f62: ldr r3,[sp,#0x24]
  000e4f64: movs r2,#0x0
  000e4f66: add r1,r3
  000e4f68: strd r1,r2,[sp,#0x0]
  000e4f6c: asr.w r1,r10, asr #0x1
  000e4f70: sub.w r3,r1,r0, asr #0x1
  000e4f74: add r2,sp,#0x4c
  000e4f76: mov r0,r6
  000e4f78: mov r1,r5
  000e4f7a: blx 0x0006fe14
  000e4f7e: ldr.w r0,[r8,#0x0]
  000e4f82: ldr r1,[sp,#0x18]
  000e4f84: blx 0x0006fac0
  000e4f88: ldr r0,[sp,#0x10]
  000e4f8a: str.w r0,[r4,#0x3b0]
  000e4f8e: add r0,sp,#0x4c
  000e4f90: blx 0x0006ee30
  000e4f94: ldr r1,[sp,#0x14]
  000e4f96: ldr r0,[sp,#0x70]
  000e4f98: ldr r1,[r1,#0x0]
  000e4f9a: subs r0,r1,r0
  000e4f9c: ittt eq
  000e4f9e: add.eq sp,#0x74
  000e4fa0: pop.eq.w {r8,r9,r10,r11}
  000e4fa4: pop.eq {r4,r5,r6,r7,pc}
  000e4fa6: blx 0x0006e824
```
