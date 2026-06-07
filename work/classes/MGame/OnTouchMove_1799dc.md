# MGame::OnTouchMove
elf 0x1799dc body 528
Sig: undefined __thiscall OnTouchMove(MGame * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::OnTouchMove(int, int, void*) */

void __thiscall MGame::OnTouchMove(MGame *this,int param_1,int param_2,void *param_3)

{
  byte bVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint in_fpscr;
  float fVar5;
  undefined4 extraout_s1;
  undefined4 extraout_s1_00;
  undefined4 uVar7;
  undefined8 uVar6;
  float fVar8;
  float fVar9;
  float fVar10;
  undefined4 extraout_s7;
  undefined4 extraout_s7_00;
  undefined4 uVar11;
  float fVar12;
  
  if (((*(char *)((int)&this->field_5C + 1) == '\0') ||
      (((*(char *)((int)&this[1].field_94 + 2) != '\0' &&
        (iVar3 = MenuTouchWindow::isShowingMessage(), iVar3 == 0)) &&
       (iVar3 = MenuTouchWindow::isMakingScreenshot(), iVar3 == 0)))) &&
     ((((char)this[1].field_94 != '\0' && (this[1].field_30 == 0)) && (this->field_14 == 3)))) {
    freeCamTouchMove((int)this,param_1,(void *)param_2);
  }
  else {
    if (*(char *)((int)&this->field_5C + 1) != '\0') goto LAB_00189b50;
    iVar3 = Hud::touchMove((uint)this->m_pLevel,param_1,(void *)param_2);
    uVar4 = this->field_14;
    this[1].field_30 = iVar3;
    if (((uVar4 < 2) &&
        (maneuverTouchMove((int)this,uVar4,(void *)param_2), (char)this[2].field_28 != '\0')) &&
       ((*(char *)((int)&this->field_5C + 3) == '\0' &&
        ((this[1].field_30 == 0 ||
         ((this[1].field_30 == 0x20 && (*(void **)&this->field_C0 != param_3)))))))) {
      if (*(char *)((int)&this[2].field_28 + 1) == '\0') {
        fVar12 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
        fVar8 = (float)this[2].field_30;
        fVar10 = (float)VectorSignedToFloat(this[2].field_3C,(byte)(in_fpscr >> 0x16) & 3);
        fVar9 = fVar12 - fVar8;
        fVar5 = -fVar9;
        if (0.0 < fVar9) {
          fVar5 = fVar9;
        }
        if (fVar5 <= fVar10) goto LAB_00189b4a;
        uVar4 = in_fpscr & 0xfffffff | (uint)(fVar8 < fVar12) << 0x1f |
                (uint)(fVar8 == fVar12) << 0x1e;
        in_fpscr = uVar4 | (uint)(NAN(fVar8) || NAN(fVar12)) << 0x1c;
        iVar3 = -1;
        bVar1 = (byte)(uVar4 >> 0x18);
        if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
          iVar3 = 1;
        }
        iVar3 = VectorSignedToFloat(iVar3 + param_2,(byte)(in_fpscr >> 0x16) & 3);
        this[2].field_38 = 0;
        this[2].field_30 = iVar3;
        fVar5 = (float)PlayerEgo::getThrust();
        this[2].field_40 = (int)fVar5;
        uVar11 = extraout_s7_00;
        uVar7 = extraout_s1_00;
      }
      else {
        fVar12 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
        fVar5 = (float)this[2].field_40;
        uVar11 = extraout_s7;
        uVar7 = extraout_s1;
      }
      iVar3 = DAT_00189bf0;
      uVar2 = DAT_00189bec;
      fVar8 = (float)this[2].field_30;
      fVar10 = (float)VectorSignedToFloat(this[2].field_38,(byte)(in_fpscr >> 0x16) & 3);
      *(undefined1 *)((int)&this[2].field_28 + 1) = 1;
      uVar6 = FloatVectorMax(CONCAT44(uVar7,((fVar8 - fVar12) - fVar10) /
                                            *(float *)(**(int **)(iVar3 + 0x189b12) + 0x2f8) + fVar5
                                     ),CONCAT44(uVar11,uVar2),2,0x20);
      PlayerEgo::setThrust((float)uVar6);
      PlayerEgo::throttleChanged();
    }
  }
LAB_00189b4a:
  if (*(char *)((int)&this->field_5C + 1) == '\0') {
    return;
  }
LAB_00189b50:
  if (((((char)this->field_60 == '\0') && (this->field_0xc5 == '\0')) &&
      (*(char *)((int)&this[1].field_4 + 2) == '\0')) && (this->field_C4 == '\0')) {
    if (this->field_0xc7 == '\0') {
      if (*(char *)((int)&this->field_5C + 2) == '\0') {
        if (((*(char *)((int)&this[1].field_0 + 1) != '\0') &&
            (iVar3 = AbyssEngine::ApplicationManager::GetApplicationData(),
            *(char *)(iVar3 + 5) == '\0')) && (*(char *)(iVar3 + 0xc) == '\0')) {
          MenuTouchWindow::OnTouchMove((int)this->m_pDialogueWindow,param_1,(void *)param_2);
        }
      }
      else {
        DialogueWindow::OnTouchMove(this->field_8C,param_1);
      }
    }
    else if (*(char *)**(undefined4 **)(DAT_00189bf4 + 0x189b8a) == '\0') {
      StarMap::OnTouchMove((int)this->field_90,param_1);
    }
    else {
      Layout::OnTouchMove((int)**(undefined4 **)(DAT_00189bf4 + 0x189b8a),param_1);
    }
  }
  else {
    ChoiceWindow::OnTouchMove(this->field_94,param_1);
  }
  return;
}

```

## target disasm
```
  001899dc: push {r4,r5,r6,r7,lr}
  001899de: add r7,sp,#0xc
  001899e0: push.w r8
  001899e4: vpush {d8}
  001899e8: mov r6,r0
  001899ea: ldrb.w r0,[r0,#0x5d]
  001899ee: mov r8,r3
  001899f0: mov r4,r2
  001899f2: mov r5,r1
  001899f4: cbz r0,0x00189a10
  001899f6: ldrb.w r0,[r6,#0x15e]
  001899fa: cbz r0,0x00189a30
  001899fc: ldr.w r0,[r6,#0x88]
  00189a00: blx 0x000781e4
  00189a04: cbnz r0,0x00189a30
  00189a06: ldr.w r0,[r6,#0x88]
  00189a0a: blx 0x000781f0
  00189a0e: cbnz r0,0x00189a30
  00189a10: ldrb.w r0,[r6,#0x15c]
  00189a14: cbz r0,0x00189a30
  00189a16: ldr.w r0,[r6,#0xf8]
  00189a1a: cbnz r0,0x00189a30
  00189a1c: ldr r0,[r6,#0x14]
  00189a1e: cmp r0,#0x3
  00189a20: bne 0x00189a30
  00189a22: mov r0,r6
  00189a24: mov r1,r5
  00189a26: mov r2,r4
  00189a28: mov r3,r8
  00189a2a: blx 0x00078238
  00189a2e: b 0x00189b4a
  00189a30: ldrb.w r0,[r6,#0x5d]
  00189a34: cmp r0,#0x0
  00189a36: bne.w 0x00189b50
  00189a3a: ldr r0,[r6,#0x74]
  00189a3c: mov r1,r5
  00189a3e: mov r2,r4
  00189a40: mov r3,r8
  00189a42: blx 0x00078244
  00189a46: ldr r1,[r6,#0x14]
  00189a48: str.w r0,[r6,#0xf8]
  00189a4c: cmp r1,#0x1
  00189a4e: bhi 0x00189b4a
  00189a50: mov r0,r6
  00189a52: mov r2,r4
  00189a54: blx 0x00078250
  00189a58: ldrb.w r0,[r6,#0x1b8]
  00189a5c: cmp r0,#0x0
  00189a5e: beq 0x00189b4a
  00189a60: ldrb.w r0,[r6,#0x5f]
  00189a64: cmp r0,#0x0
  00189a66: bne 0x00189b4a
  00189a68: ldr.w r0,[r6,#0xf8]
  00189a6c: cbz r0,0x00189a7a
  00189a6e: cmp r0,#0x20
  00189a70: bne 0x00189b4a
  00189a72: ldr.w r0,[r6,#0xc0]
  00189a76: cmp r0,r8
  00189a78: beq 0x00189b4a
  00189a7a: ldrb.w r0,[r6,#0x1b9]
  00189a7e: vmov s0,r4
  00189a82: cbz r0,0x00189a8e
  00189a84: vcvt.f32.s32 s16,s0
  00189a88: vldr.32 s0,[r6,#0x1d0]
  00189a8c: b 0x00189af2
  00189a8e: vcvt.f32.s32 s16,s0
  00189a92: vldr.32 s0,[r6,#0x1c0]
  00189a96: vldr.32 s2,[r6,#0x1cc]
  00189a9a: vcvt.f32.s32 s2,s2
  00189a9e: vsub.f32 s4,s16,s0
  00189aa2: vcmpe.f32 s4,#0
  00189aa6: vmrs apsr,fpscr
  00189aaa: vneg.f32 s6,s4
  00189aae: it gt
  00189ab0: vmov.gt.f32 s6,s4
  00189ab4: vcmpe.f32 s6,s2
  00189ab8: vmrs apsr,fpscr
  00189abc: ble 0x00189b4a
  00189abe: vcmpe.f32 s0,s16
  00189ac2: mov.w r0,#0xffffffff
  00189ac6: vmrs apsr,fpscr
  00189aca: mov.w r1,#0x0
  00189ace: it gt
  00189ad0: mov.gt r0,#0x1
  00189ad2: add r0,r4
  00189ad4: vmov s0,r0
  00189ad8: vcvt.f32.s32 s0,s0
  00189adc: ldr r0,[r6,#0x58]
  00189ade: str.w r1,[r6,#0x1c8]
  00189ae2: vstr.32 s0,[r6,#0x1c0]
  00189ae6: blx 0x0007825c
  00189aea: vmov s0,r0
  00189aee: str.w r0,[r6,#0x1d0]
  00189af2: vldr.32 s2,[r6,#0x1c0]
  00189af6: movs r1,#0x1
  00189af8: vldr.32 s4,[r6,#0x1c8]
  00189afc: vsub.f32 s2,s2,s16
  00189b00: vldr.32 s6,[pc,#0xe8]
  00189b04: vcvt.f32.s32 s4,s4
  00189b08: ldr r0,[0x00189bf0]
  00189b0a: strb.w r1,[r6,#0x1b9]
  00189b0e: add r0,pc
  00189b10: ldr r0,[r0,#0x0]
  00189b12: vsub.f32 s2,s2,s4
  00189b16: ldr r0,[r0,#0x0]
  00189b18: vldr.32 s4,[r0,#0x2f8]
  00189b1c: vdiv.f32 s2,s2,s4
  00189b20: vadd.f32 s0,s2,s0
  00189b24: vmov.f32 s4,0x3f800000
  00189b28: vmax.f32 d0,d0,d3
  00189b2c: vcmpe.f32 s0,s4
  00189b30: vmrs apsr,fpscr
  00189b34: it mi
  00189b36: vmov.mi.f32 s4,s0
  00189b3a: ldr r0,[r6,#0x58]
  00189b3c: vmov r1,s4
  00189b40: blx 0x00078268
  00189b44: ldr r0,[r6,#0x58]
  00189b46: blx 0x00078274
  00189b4a: ldrb.w r0,[r6,#0x5d]
  00189b4e: cbz r0,0x00189b74
  00189b50: ldrb.w r0,[r6,#0x60]
  00189b54: cbnz r0,0x00189b68
  00189b56: ldrb.w r0,[r6,#0xc5]
  00189b5a: cbnz r0,0x00189b68
  00189b5c: ldrb.w r0,[r6,#0xce]
  00189b60: cbnz r0,0x00189b68
  00189b62: ldrb.w r0,[r6,#0xc4]
  00189b66: cbz r0,0x00189b7e
  00189b68: ldr.w r0,[r6,#0x94]
  00189b6c: mov r1,r5
  00189b6e: mov r2,r4
  00189b70: blx 0x0007474c
  00189b74: vpop {d8}
  00189b78: pop.w r8
  00189b7c: pop {r4,r5,r6,r7,pc}
  00189b7e: ldrb.w r0,[r6,#0xc7]
  00189b82: cbz r0,0x00189b9a
  00189b84: ldr r0,[0x00189bf4]
  00189b86: add r0,pc
  00189b88: ldr r0,[r0,#0x0]
  00189b8a: ldr r0,[r0,#0x0]
  00189b8c: ldrb r1,[r0,#0x0]
  00189b8e: cbz r1,0x00189bae
  00189b90: mov r1,r5
  00189b92: mov r2,r4
  00189b94: blx 0x00074758
  00189b98: b 0x00189b74
  00189b9a: ldrb.w r0,[r6,#0x5e]
  00189b9e: cbz r0,0x00189bbc
  00189ba0: ldr.w r0,[r6,#0x8c]
  00189ba4: mov r1,r5
  00189ba6: mov r2,r4
  00189ba8: blx 0x000750d0
  00189bac: b 0x00189b74
  00189bae: ldr.w r0,[r6,#0x90]
  00189bb2: mov r1,r5
  00189bb4: mov r2,r4
  00189bb6: blx 0x000750e8
  00189bba: b 0x00189b74
  00189bbc: ldrb.w r0,[r6,#0xc9]
  00189bc0: cmp r0,#0x0
  00189bc2: beq 0x00189b74
  00189bc4: ldr r0,[0x00189bf8]
  00189bc6: add r0,pc
  00189bc8: ldr r0,[r0,#0x0]
  00189bca: ldr r0,[r0,#0x0]
  00189bcc: blx 0x00071704
  00189bd0: ldrb r1,[r0,#0x5]
  00189bd2: cmp r1,#0x0
  00189bd4: bne 0x00189b74
  00189bd6: ldrb r0,[r0,#0xc]
  00189bd8: cmp r0,#0x0
  00189bda: bne 0x00189b74
  00189bdc: ldr.w r0,[r6,#0x88]
  00189be0: mov r1,r5
  00189be2: mov r2,r4
  00189be4: mov r3,r8
  00189be6: blx 0x00075118
  00189bea: b 0x00189b74
```
