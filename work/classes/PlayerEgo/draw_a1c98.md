# PlayerEgo::draw
elf 0xa1c98 body 576
Sig: undefined __stdcall draw(bool param_1)

## decompile
```c

/* PlayerEgo::draw(bool) */

void PlayerEgo::draw(bool param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  char cVar3;
  PlayerEgo *this;
  int iVar4;
  Matrix *pMVar5;
  SingleLevel *pSVar6;
  int in_r1;
  undefined4 *puVar7;
  int *piVar8;
  float *pfVar9;
  uint *puVar10;
  int *piVar11;
  undefined4 local_58 [5];
  undefined4 local_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined4 local_20;
  int local_1c;
  
  this = (PlayerEgo *)(uint)param_1;
  piVar11 = *(int **)(DAT_000b1ee8 + 0xb1cac);
  local_1c = *piVar11;
  if (this[0xb].field_8 != (int *)0x0) goto LAB_000b1cb6;
  if ((this[0xd].field_14 != (int *)0x0) && (*(char *)&this[0xb].field_14 == '\0')) {
    if (*piVar11 == local_1c) {
      (*(code *)(DAT_001abc74 + 0x1abc78))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  if (this[0xd].field_10 != (int *)0x0) {
    if (*piVar11 == local_1c) {
      (*(code *)(DAT_001abc54 + 0x1abc58))();
      return;
    }
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  if (((((((char)this[0xd].field_20 != '\0') ||
         (iVar4 = isDead(this), uVar2 = DAT_000b1ee0, uVar1 = DAT_000b1ed8, iVar4 != 0)) ||
        ((char)this[1].m_pPlayer != '\0')) ||
       ((in_r1 == 0 || (*(char *)&this[0xc].field_10 != '\0')))) ||
      ((*(char *)((int)&this[0xd].field_18 + 2) != '\0' ||
       (*(char *)((int)&this[0xd].field_18 + 1) != '\0')))) ||
     ((*(char *)((int)&this[0x17].field_18 + 2) != '\0' && (*(char *)&this[0xb].field_14 == '\0'))))
  {
LAB_000b1d1e:
    if ((*(char *)&this[9].field_14 != '\0') ||
       ((*(char *)((int)&this[0x17].field_18 + 2) != '\0' && (2 < (int)this[0xc].field_14 - 1U)))) {
      if (*piVar11 - local_1c == 0) {
        (*(code *)(DAT_001abc64 + 0x1abc68))();
        return;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail(*piVar11 - local_1c);
    }
    goto LAB_000b1cb6;
  }
  cVar3 = *(char *)&this[0xb].field_14;
  if (*(char *)&this[9].field_14 != '\0') {
    if (cVar3 == '\0') goto LAB_000b1d1e;
    cVar3 = '\x01';
  }
  uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uStack_38 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uStack_34 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar7 = (undefined4 *)((uint)local_58 | 4);
  *puVar7 = 0;
  puVar7[1] = uStack_3c;
  puVar7[2] = uStack_38;
  puVar7[3] = uStack_34;
  local_40 = 0;
  uStack_30 = uVar1;
  uStack_28 = uVar2;
  local_58[0] = 0x3f800000;
  local_44 = 0x3f800000;
  local_20 = 0x3f800000;
  if (cVar3 == '\0') {
    pMVar5 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)local_58,pMVar5);
  }
  puVar10 = *(uint **)(DAT_000b1eec + 0xb1dd8);
  AbyssEngine::PaintCanvas::SetColor(*puVar10);
  if (*(char *)&this[0xb].field_14 == '\0') {
LAB_000b1e12:
    if ((char)this->m_pLevel->field_30 == '\0') {
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar10,this[0xf].field_20,(int)**(float **)(DAT_000b1f00 + 0xb1e6e),
                 (uchar)(int)(*(float **)(DAT_000b1f00 + 0xb1e6e))[1],'\x11');
      this[0x10].m_pLevel = (SingleLevel *)0x0;
    }
    else {
      AbyssEngine::PaintCanvas::DrawImage2D
                (*puVar10,this[0x10].m_pPlayer,(int)**(float **)(DAT_000b1efc + 0xb1e24),
                 (uchar)(int)(*(float **)(DAT_000b1efc + 0xb1e24))[1],'\x11');
      pSVar6 = (SingleLevel *)((int)this[8].field_14 + (int)this[0x10].m_pLevel);
      this[0x10].m_pLevel = pSVar6;
      if (200 < (int)pSVar6) {
        *(undefined1 *)&this->m_pLevel->field_30 = 0;
      }
    }
  }
  else {
    iVar4 = Status::getShip();
    iVar4 = Ship::getFirstEquipmentOfSort(iVar4);
    if (iVar4 == 0) goto LAB_000b1e12;
    iVar4 = Radar::isPlasmaInRange((Radar *)this->field_14);
    if (iVar4 == 0) {
      pfVar9 = *(float **)(DAT_000b1ef8 + 0xb1ea2);
      piVar8 = this[0x10].field_4;
    }
    else {
      pfVar9 = *(float **)(DAT_000b1ef4 + 0xb1e0c);
      piVar8 = this[0x10].field_8;
    }
    AbyssEngine::PaintCanvas::DrawImage2D
              (*puVar10,(int)piVar8,(int)*pfVar9,(uchar)(int)pfVar9[1],'\x11');
  }
  drawThrottle(this);
LAB_000b1cb6:
  if (*piVar11 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000b1c98: push {r4,r5,r6,r7,lr}
  000b1c9a: add r7,sp,#0xc
  000b1c9c: push.w r11
  000b1ca0: sub sp,#0x48
  000b1ca2: mov r4,r0
  000b1ca4: ldr r0,[0x000b1ee8]
  000b1ca6: mov r5,r1
  000b1ca8: add r0,pc
  000b1caa: ldr r6,[r0,#0x0]
  000b1cac: ldr r0,[r6,#0x0]
  000b1cae: str r0,[sp,#0x44]
  000b1cb0: ldr.w r0,[r4,#0x194]
  000b1cb4: cbz r0,0x000b1cca
  000b1cb6: ldr r0,[sp,#0x44]
  000b1cb8: ldr r1,[r6,#0x0]
  000b1cba: subs r0,r1,r0
  000b1cbc: ittt eq
  000b1cbe: add.eq sp,#0x48
  000b1cc0: pop.eq.w r11
  000b1cc4: pop.eq {r4,r5,r6,r7,pc}
  000b1cc6: blx 0x0006e824
  000b1cca: ldr.w r0,[r4,#0x1e8]
  000b1cce: cbz r0,0x000b1cd6
  000b1cd0: ldrb.w r1,[r4,#0x1a0]
  000b1cd4: cbz r1,0x000b1d42
  000b1cd6: ldr.w r0,[r4,#0x1e4]
  000b1cda: cbz r0,0x000b1cf6
  000b1cdc: ldr r1,[sp,#0x44]
  000b1cde: ldr r2,[r6,#0x0]
  000b1ce0: subs r1,r2,r1
  000b1ce2: itttt eq
  000b1ce4: add.eq sp,#0x48
  000b1ce6: pop.eq.w r11
  000b1cea: pop.eq.w {r4,r5,r6,r7,lr}
  000b1cee: b.eq.w 0x001abc48
  000b1cf2: blx 0x0006e824
  000b1cf6: ldrb.w r0,[r4,#0x1f4]
  000b1cfa: cbnz r0,0x000b1d1e
  000b1cfc: mov r0,r4
  000b1cfe: blx 0x000724b4
  000b1d02: cbnz r0,0x000b1d1e
  000b1d04: ldrb.w r0,[r4,#0x24]
  000b1d08: cbnz r0,0x000b1d1e
  000b1d0a: cbz r5,0x000b1d1e
  000b1d0c: ldrb.w r0,[r4,#0x1c0]
  000b1d10: cbnz r0,0x000b1d1e
  000b1d12: ldrb.w r0,[r4,#0x1ee]
  000b1d16: cbnz r0,0x000b1d1e
  000b1d18: ldrb.w r0,[r4,#0x1ed]
  000b1d1c: cbz r0,0x000b1d70
  000b1d1e: ldrb.w r0,[r4,#0x158]
  000b1d22: cbz r0,0x000b1d5c
  000b1d24: ldr r0,[sp,#0x44]
  000b1d26: ldr r1,[r6,#0x0]
  000b1d28: subs r0,r1,r0
  000b1d2a: itttt eq
  000b1d2c: mov.eq r0,r4
  000b1d2e: add.eq sp,#0x48
  000b1d30: pop.eq.w r11
  000b1d34: pop.eq.w {r4,r5,r6,r7,lr}
  000b1d38: it eq
  000b1d3a: b.eq.w 0x001abc58
  000b1d3e: blx 0x0006e824
  000b1d42: ldr r1,[sp,#0x44]
  000b1d44: ldr r2,[r6,#0x0]
  000b1d46: subs r1,r2,r1
  000b1d48: itttt eq
  000b1d4a: add.eq sp,#0x48
  000b1d4c: pop.eq.w r11
  000b1d50: pop.eq.w {r4,r5,r6,r7,lr}
  000b1d54: b.eq.w 0x001abc68
  000b1d58: blx 0x0006e824
  000b1d5c: ldrb.w r0,[r4,#0x356]
  000b1d60: cmp r0,#0x0
  000b1d62: beq 0x000b1cb6
  000b1d64: ldr.w r0,[r4,#0x1c4]
  000b1d68: subs r0,#0x1
  000b1d6a: cmp r0,#0x3
  000b1d6c: bcc 0x000b1cb6
  000b1d6e: b 0x000b1d24
  000b1d70: ldrb.w r0,[r4,#0x356]
  000b1d74: cbz r0,0x000b1d7e
  000b1d76: ldrb.w r0,[r4,#0x1a0]
  000b1d7a: cmp r0,#0x0
  000b1d7c: beq 0x000b1d1e
  000b1d7e: ldrb.w r1,[r4,#0x158]
  000b1d82: ldrb.w r0,[r4,#0x1a0]
  000b1d86: cbz r1,0x000b1d8e
  000b1d88: cmp r0,#0x0
  000b1d8a: beq 0x000b1d1e
  000b1d8c: movs r0,#0x1
  000b1d8e: adr r1,[0xb1ed8]
  000b1d90: vmov.i32 q8,#0x0
  000b1d94: add r2,sp,#0x8
  000b1d96: vld1.64 {d18,d19},[r1]
  000b1d9a: orr r1,r2,#0x4
  000b1d9e: cmp r0,#0x0
  000b1da0: vst1.32 {d16,d17},[r1]
  000b1da4: add.w r1,r2,#0x18
  000b1da8: vst1.64 {d16,d17},[r1]
  000b1dac: add.w r1,r2,#0x28
  000b1db0: vst1.64 {d18,d19},[r1]
  000b1db4: mov.w r1,#0x3f800000
  000b1db8: str r1,[sp,#0x8]
  000b1dba: str r1,[sp,#0x1c]
  000b1dbc: str r1,[sp,#0x40]
  000b1dbe: bne 0x000b1dce
  000b1dc0: ldr r0,[r4,#0x8]
  000b1dc2: blx 0x000721cc
  000b1dc6: mov r1,r0
  000b1dc8: add r0,sp,#0x8
  000b1dca: blx 0x0006f148
  000b1dce: ldr r0,[0x000b1eec]
  000b1dd0: mov.w r1,#0xffffffff
  000b1dd4: add r0,pc
  000b1dd6: ldr r5,[r0,#0x0]
  000b1dd8: ldr r0,[r5,#0x0]
  000b1dda: blx 0x0006fac0
  000b1dde: ldrb.w r0,[r4,#0x1a0]
  000b1de2: cbz r0,0x000b1e12
  000b1de4: ldr r0,[0x000b1ef0]
  000b1de6: add r0,pc
  000b1de8: ldr r0,[r0,#0x0]
  000b1dea: ldr r0,[r0,#0x0]
  000b1dec: blx 0x00071a58
  000b1df0: movs r1,#0x23
  000b1df2: blx 0x0007228c
  000b1df6: cbz r0,0x000b1e12
  000b1df8: ldr r0,[r4,#0x14]
  000b1dfa: blx 0x00072d78
  000b1dfe: mov r1,r0
  000b1e00: ldr r0,[r5,#0x0]
  000b1e02: cmp r1,#0x0
  000b1e04: beq 0x000b1e9c
  000b1e06: ldr r1,[0x000b1ef4]
  000b1e08: add r1,pc
  000b1e0a: ldr r2,[r1,#0x0]
  000b1e0c: ldr.w r1,[r4,#0x248]
  000b1e10: b 0x000b1ea6
  000b1e12: ldr r0,[r4,#0xc]
  000b1e14: ldrb.w r1,[r0,#0x30]
  000b1e18: ldr r0,[r5,#0x0]
  000b1e1a: cbz r1,0x000b1e66
  000b1e1c: ldr r1,[0x000b1efc]
  000b1e1e: movs r3,#0x11
  000b1e20: add r1,pc
  000b1e22: ldr r2,[r1,#0x0]
  000b1e24: ldr.w r1,[r4,#0x240]
  000b1e28: vldr.32 s0,[r2]
  000b1e2c: vldr.32 s2,[r2,#0x4]
  000b1e30: movs r2,#0x44
  000b1e32: vcvt.s32.f32 s0,s0
  000b1e36: vcvt.s32.f32 s2,s2
  000b1e3a: strd r3,r2,[sp,#0x0]
  000b1e3e: vmov r2,s0
  000b1e42: vmov r3,s2
  000b1e46: blx 0x00071d70
  000b1e4a: ldr.w r0,[r4,#0x134]
  000b1e4e: ldr.w r1,[r4,#0x24c]
  000b1e52: add r0,r1
  000b1e54: str.w r0,[r4,#0x24c]
  000b1e58: cmp r0,#0xc9
  000b1e5a: ittt ge
  000b1e5c: ldr.ge r0,[r4,#0xc]
  000b1e5e: mov.ge r1,#0x0
  000b1e60: strb.ge.w r1,[r0,#0x30]
  000b1e64: b 0x000b1eca
  000b1e66: ldr r1,[0x000b1f00]
  000b1e68: movs r3,#0x11
  000b1e6a: add r1,pc
  000b1e6c: ldr r2,[r1,#0x0]
  000b1e6e: ldr.w r1,[r4,#0x23c]
  000b1e72: vldr.32 s0,[r2]
  000b1e76: vldr.32 s2,[r2,#0x4]
  000b1e7a: movs r2,#0x44
  000b1e7c: vcvt.s32.f32 s0,s0
  000b1e80: vcvt.s32.f32 s2,s2
  000b1e84: strd r3,r2,[sp,#0x0]
  000b1e88: vmov r2,s0
  000b1e8c: vmov r3,s2
  000b1e90: blx 0x00071d70
  000b1e94: movs r0,#0x0
  000b1e96: str.w r0,[r4,#0x24c]
  000b1e9a: b 0x000b1eca
  000b1e9c: ldr r1,[0x000b1ef8]
  000b1e9e: add r1,pc
  000b1ea0: ldr r2,[r1,#0x0]
  000b1ea2: ldr.w r1,[r4,#0x244]
  000b1ea6: vldr.32 s0,[r2]
  000b1eaa: movs r3,#0x11
  000b1eac: vldr.32 s2,[r2,#0x4]
  000b1eb0: movs r2,#0x44
  000b1eb2: vcvt.s32.f32 s0,s0
  000b1eb6: vcvt.s32.f32 s2,s2
  000b1eba: strd r3,r2,[sp,#0x0]
  000b1ebe: vmov r2,s0
  000b1ec2: vmov r3,s2
  000b1ec6: blx 0x00071d70
  000b1eca: mov r0,r4
  000b1ecc: blx 0x00072d60
  000b1ed0: b 0x000b1cb6
  001abc48: bx pc
  001abc58: bx pc
  001abc68: bx pc
```
