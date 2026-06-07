# SpaceLounge::OnTouchEnd
elf 0x171a48 body 1104
Sig: undefined __thiscall OnTouchEnd(SpaceLounge * this, int param_1, int param_2)

## decompile
```c

/* SpaceLounge::OnTouchEnd(int, int) */

void __thiscall SpaceLounge::OnTouchEnd(SpaceLounge *this,int param_1,int param_2)

{
  int iVar1;
  String *pSVar2;
  String *this_00;
  Matrix *pMVar3;
  float *pfVar4;
  uint uVar5;
  float *pfVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  int *piVar9;
  uint in_fpscr;
  float fVar10;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float fVar11;
  float extraout_s2;
  String aSStack_478 [924];
  Matrix aMStack_dc [60];
  undefined4 auStack_a0 [5];
  undefined4 uStack_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined4 uStack_68;
  String aSStack_60 [12];
  int iStack_54;
  
  piVar9 = *(int **)(DAT_00181d80 + 0x181a64);
  iStack_54 = *piVar9;
  this[0xb2] = (SpaceLounge)0x0;
  if ((this[0x1b] != (SpaceLounge)0x0) || (this[0x19] != (SpaceLounge)0x0)) {
    iVar1 = ChoiceWindow::OnTouchEnd(*(int *)(this + 8),param_1);
    if (iVar1 == 1) {
      this[0x19] = (SpaceLounge)0x0;
    }
    else if (iVar1 == 0) {
LAB_00181a9a:
      onKeyPress(this,0x10000);
    }
    goto LAB_00181aac;
  }
  if (this[0x34] != (SpaceLounge)0x0) {
    iVar1 = StarMap::OnTouchEnd(*(int *)(this + 4),param_1);
    if (iVar1 != 0) {
      CutScene::resetCamera();
      this[0x34] = (SpaceLounge)0x0;
    }
    goto LAB_00181aac;
  }
  puVar8 = *(undefined4 **)(DAT_00181d84 + 0x181b00);
  iVar1 = Layout::OnTouchEnd((Layout *)*puVar8,param_1,param_2);
  if (iVar1 != 0) {
    if (this[0x1c] == (SpaceLounge)0x0) {
      if (*(int *)(this + 0x14) != 0) {
        if ((-1 < *(int *)(this + 0x20)) &&
           (iVar1 = Agent::isGenericAgent
                              (*(Agent **)
                                (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4)),
           iVar1 != 0)) {
          Agent::setEvent(*(Agent **)
                           (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4),1);
        }
        *(undefined4 *)(this + 0x14) = 0;
        this[0x36] = (SpaceLounge)0x0;
      }
    }
    else {
      Layout::resetWindowDimensions((Layout *)*puVar8);
      this[0x1c] = (SpaceLounge)0x0;
    }
    goto LAB_00181aac;
  }
  if (this[0x1c] == (SpaceLounge)0x0) {
    switch(*(undefined4 *)(this + 0x14)) {
    case 0:
      if (this[0xbd] == (SpaceLounge)0x0) {
        Status::getSystem();
        iVar1 = SolarSystem::getRace();
        uStack_70 = DAT_00183758;
        uStack_78 = DAT_00183750;
        uStack_84 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        uStack_80 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        uStack_7c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        puVar8 = (undefined4 *)(DAT_001830f0 + 0x182db0 + iVar1 * 0xc);
        VectorSignedToFloat(*puVar8,(byte)(in_fpscr >> 0x16) & 3);
        uVar7 = puVar8[2];
        fVar11 = (float)VectorSignedToFloat(puVar8[1],(byte)(in_fpscr >> 0x16) & 3);
        puVar8 = (undefined4 *)((uint)auStack_a0 | 4);
        *puVar8 = 0;
        puVar8[1] = uStack_84;
        puVar8[2] = uStack_80;
        puVar8[3] = uStack_7c;
        local_88 = 0;
        fVar10 = (float)VectorSignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
        auStack_a0[0] = 0x3f800000;
        uStack_8c = 0x3f800000;
        uStack_68 = 0x3f800000;
        AbyssEngine::AEMath::MatrixSetTranslation(aMStack_dc,fVar10,extraout_s1,fVar11);
        AbyssEngine::AEMath::MatrixSetRotation(aMStack_dc,extraout_s0,extraout_s1_00,extraout_s2);
        uVar5 = **(uint **)(DAT_001830f8 + 0x182e32);
        pMVar3 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
        AbyssEngine::PaintCanvas::CameraSetLocal(uVar5,pMVar3);
        if (*(int *)(this + 0x48) != 0) {
          AbyssEngine::EaseInOutMatrix::SetRange(*(int *)(this + 0x48));
        }
        this[0xbd] = (SpaceLounge)0x1;
        *(undefined4 *)(this + 0x104) = 0;
        goto LAB_00181aac;
      }
      if (*(uint **)(this + 0x24) != (uint *)0x0) {
        fVar10 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
        fVar11 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
        *(undefined4 *)(this + 0x88) = 0xffffffff;
        *(undefined4 *)(this + 0x20) = 0xffffffff;
        for (uVar5 = 0; uVar5 < **(uint **)(this + 0x24); uVar5 = uVar5 + 1) {
          pfVar4 = *(float **)(*(int *)(*(int *)(this + 0x40) + 4) + uVar5 * 8);
          if (((((int)((uint)(*pfVar4 < fVar11) << 0x1f) < 0) &&
               (pfVar6 = *(float **)(*(int *)(*(int *)(this + 0x40) + 4) + uVar5 * 8 + 4),
               fVar11 < *pfVar6)) && (fVar10 < pfVar4[1])) &&
             ((int)((uint)(pfVar6[1] < fVar10) << 0x1f) < 0)) {
            *(uint *)(this + 0x20) = uVar5;
            goto LAB_00181a9a;
          }
        }
      }
      break;
    case 1:
switchD_00181bba_caseD_1:
      onKeyPress(this,0x10000);
      break;
    case 2:
      for (uVar5 = 0; uVar5 < **(uint **)(this + 0x5c); uVar5 = uVar5 + 1) {
        iVar1 = TouchButton::OnTouchEnd(*(int *)((*(uint **)(this + 0x5c))[1] + uVar5 * 4),param_1);
        if (iVar1 != 0) {
          Agent::getRace(*(Agent **)
                          (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
          if ((uVar5 & 0x7fffffff) < 5) {
                    /* WARNING: Could not recover jumptable at 0x00181cc2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&DAT_00181cc6 + (ushort)(&DAT_00181cc6)[uVar5]))();
            return;
          }
          iVar1 = Agent::isGenericAgent
                            (*(Agent **)
                              (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4));
          if (iVar1 != 0) {
            Agent::setEvent(*(Agent **)
                             (*(int *)(*(int *)(this + 0x24) + 4) + *(int *)(this + 0x20) * 4),1);
          }
        }
      }
      break;
    case 3:
      iVar1 = TouchButton::OnTouchEnd(**(int **)(*(int *)(this + 0x5c) + 4),param_1);
      if (iVar1 != 0) goto switchD_00181bba_caseD_1;
    }
    ScrollTouchWindow::OnTouchEnd(*(int *)(this + 0x60),param_1);
    iVar1 = Layout::helpPressed((Layout *)*puVar8);
    if (iVar1 == 0) goto LAB_00181aac;
    uVar7 = *puVar8;
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_001830e8 + 0x182d6c));
    AbyssEngine::String::String(aSStack_478,pSVar2,false);
    Layout::initHelpWindow(uVar7,aSStack_478);
    this_00 = aSStack_478;
  }
  else {
    ListItemWindow::OnTouchEnd(*(int *)(this + 0xc),param_1);
    iVar1 = Layout::helpPressed((Layout *)*puVar8);
    if (iVar1 == 0) goto LAB_00181aac;
    uVar7 = *puVar8;
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_00181d88 + 0x181b4c));
    AbyssEngine::String::String(aSStack_60,pSVar2,false);
    Layout::initHelpWindow(uVar7,aSStack_60);
    this_00 = aSStack_60;
  }
  AbyssEngine::String::~String(this_00);
LAB_00181aac:
  iVar1 = *piVar9 - iStack_54;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  00181a48: push {r4,r5,r6,r7,lr}
  00181a4a: add r7,sp,#0xc
  00181a4c: push {r7,r8,r9,r10,r11}
  00181a50: vpush {d8,d9,d10,d11,d12}
  00181a54: sub.w sp,sp,#0x4c0
  00181a58: mov r9,r0
  00181a5a: ldr r0,[0x00181d80]
  00181a5c: mov r11,r9
  00181a5e: mov r5,r1
  00181a60: add r0,pc
  00181a62: mov r6,r2
  00181a64: movs r1,#0x0
  00181a66: ldr.w r10,[r0,#0x0]
  00181a6a: ldr.w r0,[r10,#0x0]
  00181a6e: str.w r0,[r7,#-0x4c]
  00181a72: ldrb.w r0,[r11,#0x1b]!
  00181a76: cmp r0,#0x0
  00181a78: strb.w r1,[r11,#0x97]
  00181a7c: bne 0x00181a86
  00181a7e: mov r4,r9
  00181a80: ldrb.w r0,[r4,#0x19]!
  00181a84: cbz r0,0x00181ad0
  00181a86: ldr.w r0,[r9,#0x8]
  00181a8a: mov r1,r5
  00181a8c: mov r2,r6
  00181a8e: blx 0x00074770
  00181a92: movs r5,#0x0
  00181a94: cmp r0,#0x1
  00181a96: beq 0x00181aa8
  00181a98: cbnz r0,0x00181aac
  00181a9a: mov r0,r9
  00181a9c: mov.w r1,#0x10000
  00181aa0: blx 0x00077d64
  00181aa4: movs r5,#0x0
  00181aa6: b 0x00181aac
  00181aa8: strb.w r5,[r9,#0x19]
  00181aac: ldr.w r0,[r7,#-0x4c]
  00181ab0: ldr.w r1,[r10,#0x0]
  00181ab4: subs r0,r1,r0
  00181ab6: itttt eq
  00181ab8: mov.eq r0,r5
  00181aba: add.eq.w sp,sp,#0x4c0
  00181abe: vpop.eq {d8,d9,d10,d11,d12}
  00181ac2: add.eq sp,#0x4
  00181ac4: itt eq
  00181ac6: pop.eq.w {r8,r9,r10,r11}
  00181aca: pop.eq {r4,r5,r6,r7,pc}
  00181acc: blx 0x0006e824
  00181ad0: ldrb.w r0,[r9,#0x34]
  00181ad4: cbz r0,0x00181af6
  00181ad6: ldr.w r0,[r9,#0x4]
  00181ada: mov r1,r5
  00181adc: mov r2,r6
  00181ade: blx 0x000751e4
  00181ae2: movs r5,#0x0
  00181ae4: cmp r0,#0x0
  00181ae6: beq 0x00181aac
  00181ae8: ldr.w r0,[r9,#0x44]
  00181aec: blx 0x00072070
  00181af0: strb.w r5,[r9,#0x34]
  00181af4: b 0x00181aac
  00181af6: ldr r0,[0x00181d84]
  00181af8: mov r1,r5
  00181afa: mov r2,r6
  00181afc: add r0,pc
  00181afe: ldr.w r8,[r0,#0x0]
  00181b02: ldr.w r0,[r8,#0x0]
  00181b06: blx 0x0007477c
  00181b0a: ldrb.w r1,[r9,#0x1c]
  00181b0e: cbz r0,0x00181b22
  00181b10: cbz r1,0x00181b70
  00181b12: ldr.w r0,[r8,#0x0]
  00181b16: blx 0x00074dc4
  00181b1a: movs r5,#0x0
  00181b1c: strb.w r5,[r9,#0x1c]
  00181b20: b 0x00181aac
  00181b22: cmp r1,#0x0
  00181b24: beq 0x00181bac
  00181b26: ldr.w r0,[r9,#0xc]
  00181b2a: mov r1,r5
  00181b2c: mov r2,r6
  00181b2e: blx 0x00077134
  00181b32: ldr.w r0,[r8,#0x0]
  00181b36: blx 0x000747b8
  00181b3a: cmp r0,#0x0
  00181b3c: beq 0x00181aa4
  00181b3e: ldr r0,[0x00181d88]
  00181b40: movw r1,#0x283
  00181b44: ldr.w r4,[r8,#0x0]
  00181b48: add r0,pc
  00181b4a: ldr r0,[r0,#0x0]
  00181b4c: ldr r0,[r0,#0x0]
  00181b4e: blx 0x00072f70
  00181b52: sub.w r5,r7,#0x58
  00181b56: mov r1,r0
  00181b58: movs r2,#0x0
  00181b5a: mov r0,r5
  00181b5c: blx 0x0006f028
  00181b60: mov r0,r4
  00181b62: mov r1,r5
  00181b64: blx 0x000747c4
  00181b68: sub.w r0,r7,#0x58
  00181b6c: b.w 0x00182d88
  00181b70: ldr.w r0,[r9,#0x14]
  00181b74: cmp r0,#0x0
  00181b76: beq 0x00181c50
  00181b78: ldr.w r0,[r9,#0x20]
  00181b7c: cmp r0,#0x0
  00181b7e: blt 0x00181ba0
  00181b80: ldr.w r1,[r9,#0x24]
  00181b84: ldr r1,[r1,#0x4]
  00181b86: ldr.w r0,[r1,r0,lsl #0x2]
  00181b8a: blx 0x00071b9c
  00181b8e: cbz r0,0x00181ba0
  00181b90: ldrd r0,r1,[r9,#0x20]
  00181b94: ldr r1,[r1,#0x4]
  00181b96: ldr.w r0,[r1,r0,lsl #0x2]
  00181b9a: movs r1,#0x1
  00181b9c: blx 0x000749bc
  00181ba0: movs r5,#0x0
  00181ba2: str.w r5,[r9,#0x14]
  00181ba6: strb.w r5,[r9,#0x36]
  00181baa: b 0x00181aac
  00181bac: ldr.w r0,[r9,#0x14]
  00181bb0: cmp r0,#0x3
  00181bb2: bhi.w 0x00182d44
  00181bb6: str.w r10,[sp,#0x8c]
  00181bba: tbh [pc,r0]
  00181bc6: ldrb.w r0,[r9,#0xbd]
  00181bca: cmp r0,#0x0
  00181bcc: beq.w 0x00182d90
  00181bd0: ldr.w r0,[r9,#0x24]
  00181bd4: cmp r0,#0x0
  00181bd6: beq.w 0x00182d44
  00181bda: vmov s0,r6
  00181bde: mov.w r1,#0xffffffff
  00181be2: vmov s2,r5
  00181be6: vcvt.f32.s32 s0,s0
  00181bea: vcvt.f32.s32 s2,s2
  00181bee: str.w r1,[r9,#0x88]
  00181bf2: str.w r1,[r9,#0x20]
  00181bf6: ldr r1,[r0,#0x0]
  00181bf8: movs r0,#0x0
  00181bfa: b 0x00181c48
  00181bfc: ldr.w r2,[r9,#0x40]
  00181c00: ldr r3,[r2,#0x4]
  00181c02: ldr.w r2,[r3,r0,lsl #0x3]
  00181c06: vldr.32 s4,[r2]
  00181c0a: vcmpe.f32 s4,s2
  00181c0e: vmrs apsr,fpscr
  00181c12: bpl 0x00181c46
  00181c14: add.w r3,r3,r0, lsl #0x3
  00181c18: ldr r3,[r3,#0x4]
  00181c1a: vldr.32 s4,[r3]
  00181c1e: vcmpe.f32 s4,s2
  00181c22: vmrs apsr,fpscr
  00181c26: ble 0x00181c46
  00181c28: vldr.32 s4,[r2,#0x4]
  00181c2c: vcmpe.f32 s4,s0
  00181c30: vmrs apsr,fpscr
  00181c34: ble 0x00181c46
  00181c36: vldr.32 s4,[r3,#0x4]
  00181c3a: vcmpe.f32 s4,s0
  00181c3e: vmrs apsr,fpscr
  00181c42: bmi.w 0x00182ed0
  00181c46: adds r0,#0x1
  00181c48: cmp r0,r1
  00181c4a: bcc 0x00181bfc
  00181c4c: b.w 0x00182d44
  00181c50: movs r5,#0x1
  00181c52: b 0x00181aac
  00181c54: ldr r0,[0x00181d8c]
  00181c56: vmov.f32 s16,0x3f800000
  00181c5a: vmov.f32 s18,0x40000000
  00181c5e: movs r2,#0x0
  00181c60: add r0,pc
  00181c62: str r0,[sp,#0x74]
  00181c64: ldr r0,[0x00181d90]
  00181c66: vmov.f32 s20,0x41200000
  00181c6a: vmov.f32 s22,0x40a00000
  00181c6e: add r0,pc
  00181c70: ldr r0,[r0,#0x0]
  00181c72: str r0,[sp,#0x88]
  00181c74: ldr r0,[0x00181d94]
  00181c76: str r4,[sp,#0x70]
  00181c78: add r0,pc
  00181c7a: ldr r0,[r0,#0x0]
  00181c7c: str r0,[sp,#0x78]
  00181c7e: add r0,sp,#0x7c
  00181c80: stm r0,{r5,r6,r8}
  00181c84: ldr.w r0,[r9,#0x5c]
  00181c88: ldr r1,[r0,#0x0]
  00181c8a: cmp r2,r1
  00181c8c: bcs.w 0x00182d44
  00181c90: ldr r0,[r0,#0x4]
  00181c92: mov r1,r5
  00181c94: mov r4,r2
  00181c96: ldr.w r0,[r0,r2,lsl #0x2]
  00181c9a: mov r2,r6
  00181c9c: blx 0x00074788
  00181ca0: cmp r0,#0x0
  00181ca2: beq.w 0x00182416
  00181ca6: ldrd r0,r1,[r9,#0x20]
  00181caa: ldr r1,[r1,#0x4]
  00181cac: ldr.w r0,[r1,r0,lsl #0x2]
  00181cb0: str r0,[sp,#0x90]
  00181cb2: blx 0x00071bd8
  00181cb6: bic r0,r4,#0x80000000
  00181cba: str r4,[sp,#0x94]
  00181cbc: cmp r0,#0x4
  00181cbe: bhi.w 0x001823f4
  00181cc2: tbh [pc,r0]
  001823f4: ldrd r0,r1,[r9,#0x20]
  001823f8: ldr r1,[r1,#0x4]
  001823fa: ldr.w r0,[r1,r0,lsl #0x2]
  001823fe: blx 0x00071b9c
  00182402: ldr r4,[sp,#0x94]
  00182404: cbz r0,0x00182416
  00182406: ldrd r0,r1,[r9,#0x20]
  0018240a: ldr r1,[r1,#0x4]
  0018240c: ldr.w r0,[r1,r0,lsl #0x2]
  00182410: movs r1,#0x1
  00182412: blx 0x000749bc
  00182416: adds r4,#0x1
  00182418: mov r2,r4
  0018241a: b.w 0x00181c84
  00182d28: ldr.w r0,[r9,#0x5c]
  00182d2c: mov r1,r5
  00182d2e: mov r2,r6
  00182d30: ldr r0,[r0,#0x4]
  00182d32: ldr r0,[r0,#0x0]
  00182d34: blx 0x00074788
  00182d38: cbz r0,0x00182d44
  00182d3a: mov r0,r9
  00182d3c: mov.w r1,#0x10000
  00182d40: blx 0x00077d64
  00182d44: ldr.w r0,[r9,#0x60]
  00182d48: mov r1,r5
  00182d4a: mov r2,r6
  00182d4c: blx 0x00075bc8
  00182d50: ldr.w r0,[r8,#0x0]
  00182d54: blx 0x000747b8
  00182d58: cmp r0,#0x0
  00182d5a: beq.w 0x00181aa4
  00182d5e: ldr r0,[0x001830e8]
  00182d60: movw r1,#0x273
  00182d64: ldr.w r4,[r8,#0x0]
  00182d68: add r0,pc
  00182d6a: ldr r0,[r0,#0x0]
  00182d6c: ldr r0,[r0,#0x0]
  00182d6e: blx 0x00072f70
  00182d72: add r5,sp,#0x98
  00182d74: mov r1,r0
  00182d76: movs r2,#0x0
  00182d78: mov r0,r5
  00182d7a: blx 0x0006f028
  00182d7e: mov r0,r4
  00182d80: mov r1,r5
  00182d82: blx 0x000747c4
  00182d86: add r0,sp,#0x98
  00182d88: blx 0x0006ee30
  00182d8c: b.w 0x00181aa4
  00182d90: ldr r0,[0x001830ec]
  00182d92: add r0,pc
  00182d94: ldr r0,[r0,#0x0]
  00182d96: ldr r0,[r0,#0x0]
  00182d98: blx 0x00071a10
  00182d9c: blx 0x00071950
  00182da0: mov r5,r0
  00182da2: ldr r0,[0x001830f0]
  00182da4: add.w r1,r5,r5, lsl #0x1
  00182da8: vmov.i32 q8,#0x0
  00182dac: add r0,pc
  00182dae: sub.w r8,r7,#0x98
  00182db2: add.w r0,r0,r1, lsl #0x2
  00182db6: mov r1,r8
  00182db8: vldr.32 s0,[r0]
  00182dbc: vldr.32 s2,[r0,#0x4]
  00182dc0: vcvt.f32.s32 s0,s0
  00182dc4: vldr.32 s4,[r0,#0x8]
  00182dc8: vcvt.f32.s32 s2,s2
  00182dcc: adr.w r0,0x183750
  00182dd0: vld1.64 {d18,d19},[r0]
  00182dd4: orr r0,r8,#0x4
  00182dd8: vst1.32 {d16,d17},[r0]
  00182ddc: add.w r0,r8,#0x18
  00182de0: vst1.64 {d16,d17},[r0]
  00182de4: add.w r0,r8,#0x28
  00182de8: vmov r2,s0
  00182dec: vmov r3,s2
  00182df0: vst1.64 {d18,d19},[r0]
  00182df4: vcvt.f32.s32 s0,s4
  00182df8: mov.w r0,#0x3f800000
  00182dfc: str.w r0,[r7,#-0x98]
  00182e00: str.w r0,[r7,#-0x84]
  00182e04: str.w r0,[r7,#-0x60]
  00182e08: sub.w r0,r7,#0xd4
  00182e0c: vstr.32 s0,[sp]
  00182e10: blx 0x0006f820
  00182e14: ldr r0,[0x001830f4]
  00182e16: mov r1,r8
  00182e18: movs r2,#0x0
  00182e1a: add r0,pc
  00182e1c: ldr.w r3,[r0,r5,lsl #0x2]
  00182e20: sub.w r0,r7,#0xd4
  00182e24: movs r5,#0x0
  00182e26: str r5,[sp,#0x0]
  00182e28: blx 0x00072094
  00182e2c: ldr r0,[0x001830f8]
  00182e2e: add r0,pc
  00182e30: ldr r0,[r0,#0x0]
  00182e32: ldr r6,[r0,#0x0]
  00182e34: mov r0,r6
  00182e36: blx 0x000717f4
  00182e3a: mov r1,r0
  00182e3c: mov r0,r6
  00182e3e: mov r2,r8
  00182e40: blx 0x000720a0
  00182e44: ldr.w r0,[r9,#0x48]
  00182e48: cbz r0,0x00182ec2
  00182e4a: ldr.w r1,[r7,#-0x98]
  00182e4e: sub.w r3,r7,#0x6c
  00182e52: str r1,[sp,#0x84]
  00182e54: ldr.w r1,[r7,#-0x94]
  00182e58: str r1,[sp,#0x90]
  00182e5a: ldr.w r1,[r7,#-0x90]
  00182e5e: str r1,[sp,#0x88]
  00182e60: ldr.w r1,[r7,#-0x8c]
  00182e64: str r1,[sp,#0x80]
  00182e66: ldrd r12,lr,[r7,#-0x88]
  00182e6a: ldrd r8,r4,[r7,#-0x80]
  00182e6e: ldrd r10,r11,[r7,#-0x78]
  00182e72: str r0,[sp,#0x94]
  00182e74: ldr.w r6,[r7,#-0x70]
  00182e78: ldmia r3,{r0,r1,r2,r3}
  00182e7a: str r1,[sp,#0x60]
  00182e7c: str r2,[sp,#0x64]
  00182e7e: str r3,[sp,#0x68]
  00182e80: strd r12,lr,[sp,#0x40]
  00182e84: strd r8,r4,[sp,#0x48]
  00182e88: strd r10,r11,[sp,#0x50]
  00182e8c: strd r6,r0,[sp,#0x58]
  00182e90: str r0,[sp,#0x20]
  00182e92: ldr r0,[sp,#0x80]
  00182e94: str r1,[sp,#0x24]
  00182e96: str r2,[sp,#0x28]
  00182e98: str r3,[sp,#0x2c]
  00182e9a: ldr r1,[sp,#0x84]
  00182e9c: ldr r2,[sp,#0x90]
  00182e9e: ldr r3,[sp,#0x88]
  00182ea0: str r1,[sp,#0x30]
  00182ea2: str r2,[sp,#0x34]
  00182ea4: str r3,[sp,#0x38]
  00182ea6: str r0,[sp,#0x3c]
  00182ea8: stm sp,{r0,r12,lr}
  00182eac: strd r8,r4,[sp,#0xc]
  00182eb0: str.w r10,[sp,#0x14]
  00182eb4: ldr.w r10,[sp,#0x8c]
  00182eb8: strd r11,r6,[sp,#0x18]
  00182ebc: ldr r0,[sp,#0x94]
  00182ebe: blx 0x0006f130
  00182ec2: movs r0,#0x1
  00182ec4: strb.w r0,[r9,#0xbd]
  00182ec8: str.w r5,[r9,#0x104]
  00182ecc: b.w 0x00181aac
  00182ed0: str.w r0,[r9,#0x20]
  00182ed4: b.w 0x00181a9a
```
