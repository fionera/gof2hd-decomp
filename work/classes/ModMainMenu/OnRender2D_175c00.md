# ModMainMenu::OnRender2D
elf 0x175c00 body 414
Sig: undefined __thiscall OnRender2D(ModMainMenu * this)

## decompile
```c

/* ModMainMenu::OnRender2D() */

void __thiscall ModMainMenu::OnRender2D(ModMainMenu *this)

{
  float fVar1;
  float fVar2;
  String *pSVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  uint uVar8;
  String *pSVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  int *piVar13;
  uint *puVar14;
  uint in_fpscr;
  
  AbyssEngine::PaintCanvas::Begin2d();
  AbyssEngine::PaintCanvas::SetColor(*(uint *)(this + 4));
  CutScene::render2D();
  if (this[0x28] == (ModMainMenu)0x0) {
    MenuTouchWindow::draw();
  }
  else {
    if (*(int *)(this + 0x24) < 0xf3c) {
      VectorSignedToFloat(*(int *)(this + 0x24),(byte)(in_fpscr >> 0x16) & 3);
    }
    AbyssEngine::PaintCanvas::SetColor(*(uint *)(this + 4));
    puVar14 = *(uint **)(DAT_00185dac + 0x185c78);
    AbyssEngine::PaintCanvas::DrawImage2D(*puVar14,*(int *)(this + 0x20),0,'\0','D');
    if (0xf3b < *(int *)(this + 0x24)) {
      uVar11 = *puVar14;
      AbyssEngine::ApplicationManager::GetSystemTimeMillis();
      fVar2 = (float)__aeabi_l2f();
      fVar1 = DAT_00185da8;
      AbyssEngine::AEMath::Sinf(fVar2 * DAT_00185da8);
      AbyssEngine::ApplicationManager::GetSystemTimeMillis();
      fVar2 = (float)__aeabi_l2f();
      AbyssEngine::AEMath::Sinf(fVar2 * fVar1);
      AbyssEngine::PaintCanvas::SetColor((uchar)uVar11,0xff,0xff,0xff);
      puVar7 = *(undefined4 **)(DAT_00185db4 + 0x185d16);
      piVar13 = *(int **)(DAT_00185db8 + 0x185d18);
      pSVar3 = (String *)*puVar7;
      uVar11 = *puVar14;
      iVar4 = GameText::getText(*piVar13);
      pSVar9 = (String *)*puVar7;
      uVar8 = *puVar14;
      iVar12 = **(int **)(DAT_00185dbc + 0x185d3c);
      GameText::getText(*piVar13);
      iVar5 = AbyssEngine::PaintCanvas::GetTextWidth(uVar8,pSVar9);
      iVar10 = **(int **)(DAT_00185dc0 + 0x185d60);
      iVar6 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar14);
      AbyssEngine::PaintCanvas::DrawString
                (uVar11,pSVar3,iVar4,(iVar12 >> 1) - (iVar5 >> 1),
                 (bool)((char)(iVar10 >> 1) + (char)(iVar6 >> 1) + '\n'));
    }
  }
  (*(code *)(DAT_001ac774 + 0x1ac778))(*(undefined4 *)(this + 4));
  return;
}

```

## target disasm
```
  00185c00: push {r4,r5,r6,r7,lr}
  00185c02: add r7,sp,#0xc
  00185c04: push {r7,r8,r9,r10,r11}
  00185c08: vpush {d6,d7,d8}
  00185c0c: mov r4,r0
  00185c0e: ldr r0,[r0,#0x4]
  00185c10: blx 0x00071d34
  00185c14: ldr r0,[r4,#0x4]
  00185c16: mov.w r1,#0xffffffff
  00185c1a: blx 0x0006fac0
  00185c1e: ldr r0,[r4,#0x1c]
  00185c20: blx 0x00075634
  00185c24: ldrb.w r0,[r4,#0x28]
  00185c28: cbz r0,0x00185c5a
  00185c2a: ldr r0,[r4,#0x24]
  00185c2c: movw r1,#0xf3b
  00185c30: cmp r0,r1
  00185c32: bgt 0x00185c62
  00185c34: vmov s0,r0
  00185c38: vldr.32 s2,[pc,#0x164]
  00185c3c: vcvt.f32.s32 s0,s0
  00185c40: vdiv.f32 s0,s0,s2
  00185c44: vldr.32 s2,[pc,#0x15c]
  00185c48: vmul.f32 s0,s0,s2
  00185c4c: vcvt.s32.f32 s0,s0
  00185c50: vmov r0,s0
  00185c54: sub.w r1,r0,#0x100
  00185c58: b 0x00185c66
  00185c5a: ldr r0,[r4,#0x18]
  00185c5c: blx 0x000755d4
  00185c60: b 0x00185d8a
  00185c62: mov.w r1,#0xffffffff
  00185c66: ldr r0,[r4,#0x4]
  00185c68: blx 0x0006fac0
  00185c6c: ldr r0,[0x00185dac]
  00185c6e: movs r2,#0x44
  00185c70: ldr r1,[r4,#0x20]
  00185c72: movs r3,#0x0
  00185c74: add r0,pc
  00185c76: ldr.w r11,[r0,#0x0]
  00185c7a: ldr.w r0,[r11,#0x0]
  00185c7e: strd r2,r2,[sp,#0x0]
  00185c82: movs r2,#0x0
  00185c84: blx 0x00071d70
  00185c88: ldr r0,[r4,#0x24]
  00185c8a: movw r1,#0xf3c
  00185c8e: cmp r0,r1
  00185c90: blt 0x00185d8a
  00185c92: ldr r0,[0x00185db0]
  00185c94: ldr.w r8,[r11,#0x0]
  00185c98: add r0,pc
  00185c9a: ldr r5,[r0,#0x0]
  00185c9c: ldr r0,[r5,#0x0]
  00185c9e: blx 0x00077ee4
  00185ca2: blx 0x0006f7d8
  00185ca6: vldr.32 s16,[pc,#0x100]
  00185caa: vmov s0,r0
  00185cae: vmul.f32 s0,s0,s16
  00185cb2: vmov r0,s0
  00185cb6: blx 0x0006f1a8
  00185cba: mov r6,r0
  00185cbc: ldr r0,[r5,#0x0]
  00185cbe: blx 0x00077ee4
  00185cc2: blx 0x0006f7d8
  00185cc6: vmov s0,r0
  00185cca: vmul.f32 s0,s0,s16
  00185cce: vmov r0,s0
  00185cd2: blx 0x0006f1a8
  00185cd6: vmov s4,r6
  00185cda: movs r1,#0xff
  00185cdc: vmov s0,r0
  00185ce0: movs r2,#0xff
  00185ce2: vcmpe.f32 s4,#0
  00185ce6: movs r3,#0xff
  00185ce8: vmrs apsr,fpscr
  00185cec: vneg.f32 s2,s0
  00185cf0: it gt
  00185cf2: vmov.gt.f32 s2,s0
  00185cf6: vldr.32 s0,[pc,#0xac]
  00185cfa: vmul.f32 s0,s2,s0
  00185cfe: vcvt.u32.f32 s0,s0
  00185d02: vmov r0,s0
  00185d06: str r0,[sp,#0x0]
  00185d08: mov r0,r8
  00185d0a: blx 0x0006ff28
  00185d0e: ldr r0,[0x00185db4]
  00185d10: ldr r1,[0x00185db8]
  00185d12: add r0,pc
  00185d14: add r1,pc
  00185d16: ldr r5,[r0,#0x0]
  00185d18: ldr.w r9,[r1,#0x0]
  00185d1c: movs r1,#0xc7
  00185d1e: ldr r0,[r5,#0x0]
  00185d20: str r0,[sp,#0xc]
  00185d22: ldr.w r0,[r11,#0x0]
  00185d26: str r0,[sp,#0x8]
  00185d28: ldr.w r0,[r9,#0x0]
  00185d2c: blx 0x00072f70
  00185d30: mov r10,r0
  00185d32: ldr r0,[0x00185dbc]
  00185d34: movs r1,#0xc7
  00185d36: ldr r6,[r5,#0x0]
  00185d38: add r0,pc
  00185d3a: ldr.w r5,[r11,#0x0]
  00185d3e: ldr r0,[r0,#0x0]
  00185d40: ldr.w r8,[r0,#0x0]
  00185d44: ldr.w r0,[r9,#0x0]
  00185d48: blx 0x00072f70
  00185d4c: mov r2,r0
  00185d4e: mov r0,r5
  00185d50: mov r1,r6
  00185d52: blx 0x0006faa8
  00185d56: mov r5,r0
  00185d58: ldr r0,[0x00185dc0]
  00185d5a: ldr r1,[r4,#0x20]
  00185d5c: add r0,pc
  00185d5e: ldr r0,[r0,#0x0]
  00185d60: ldr r6,[r0,#0x0]
  00185d62: ldr.w r0,[r11,#0x0]
  00185d66: blx 0x00072d90
  00185d6a: asrs r1,r6,#0x1
  00185d6c: add.w r0,r1,r0, asr #0x1
  00185d70: adds r0,#0xa
  00185d72: str r0,[sp,#0x0]
  00185d74: movs r0,#0x0
  00185d76: mov r2,r10
  00185d78: str r0,[sp,#0x4]
  00185d7a: asr.w r0,r8, asr #0x1
  00185d7e: sub.w r3,r0,r5, asr #0x1
  00185d82: ldrd r0,r1,[sp,#0x8]
  00185d86: blx 0x0006fe14
  00185d8a: ldr r0,[r4,#0x4]
  00185d8c: vpop {d6,d7,d8}
  00185d90: add sp,#0x4
  00185d92: pop.w {r8,r9,r10,r11}
  00185d96: pop.w {r4,r5,r6,r7,lr}
  00185d9a: b.w 0x001ac768
```
