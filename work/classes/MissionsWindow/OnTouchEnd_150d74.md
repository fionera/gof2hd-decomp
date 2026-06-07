# MissionsWindow::OnTouchEnd
elf 0x150d74 body 992
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* MissionsWindow::OnTouchEnd(int, int) */

void MissionsWindow::OnTouchEnd(int param_1,int param_2)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint *puVar5;
  String *pSVar6;
  Item *pIVar7;
  Mission *pMVar8;
  Agent *pAVar9;
  StarMap *this;
  uint uVar10;
  int in_r2;
  uint uVar11;
  EVP_PKEY_CTX *ctx;
  uint uVar12;
  undefined4 *puVar13;
  ChoiceWindow *this_00;
  int *piVar14;
  
  piVar14 = *(int **)(DAT_001610f0 + 0x160d88);
  iVar2 = *piVar14;
  if (*(int *)(param_1 + 0x40) == 1) {
    uVar3 = WantedWindow::OnTouchEnd(*(int *)(param_1 + 0x10),param_2);
    if (**(int **)(param_1 + 0x10) == 0) {
      *(undefined4 *)(param_1 + 0x40) = 0;
      **(int **)(param_1 + 0x10) = 1;
    }
    goto LAB_00161144;
  }
  if (*(char *)(param_1 + 0x20) == '\0') {
LAB_00160e4e:
    if (*(char *)(param_1 + 0x22) == '\0') {
      iVar4 = Status::wantedBoardAccessible();
      if (iVar4 != 0) {
        for (uVar12 = 0; uVar12 < **(uint **)(param_1 + 0x14); uVar12 = uVar12 + 1) {
          iVar4 = TouchButton::OnTouchEnd
                            (*(int *)((*(uint **)(param_1 + 0x14))[1] + uVar12 * 4),param_2);
          if (iVar4 != 0) {
            *(uint *)(param_1 + 0x40) = uVar12;
          }
        }
      }
      ScrollTouchWindow::OnTouchEnd(*(int *)param_1,param_2);
      ScrollTouchWindow::OnTouchEnd(*(int *)(param_1 + 4),param_2);
      if ((*(int *)(param_1 + 0x24) == 0) ||
         (iVar4 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x24),param_2), iVar4 == 0)) {
        if ((*(int *)(param_1 + 0x2c) != 0) &&
           (iVar4 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x2c),param_2), iVar4 != 0)) {
          this_00 = *(ChoiceWindow **)(param_1 + 0xc);
          pSVar6 = (String *)GameText::getText(**(int **)(DAT_001611a4 + 0x160f38));
          ChoiceWindow::set(this_00,pSVar6,true);
          *(undefined1 *)(param_1 + 0x20) = 1;
        }
        if ((*(int *)(param_1 + 0x28) != 0) &&
           (iVar4 = TouchButton::OnTouchEnd(*(int *)(param_1 + 0x28),param_2), iVar4 != 0)) {
          puVar5 = *(uint **)(DAT_001611a8 + 0x160f66);
          iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar5);
          ctx = *(EVP_PKEY_CTX **)(iVar4 + 0x10);
          *(EVP_PKEY_CTX **)(param_1 + 8) = ctx;
          if (ctx == (EVP_PKEY_CTX *)0x0) {
            this = operator_new(0x1e8);
            pMVar8 = (Mission *)Status::getFreelanceMission();
            StarMap::StarMap(this,true,pMVar8,false,-1);
            goto LAB_00161118;
          }
          Status::getFreelanceMission();
          goto LAB_00160f80;
        }
        puVar13 = *(undefined4 **)(DAT_001611ac + 0x160f9e);
        iVar4 = Layout::OnTouchEnd((Layout *)*puVar13,param_2,in_r2);
        if (iVar4 != 0) {
          Layout::resetWindowDimensions((Layout *)*puVar13);
          uVar3 = 1;
          goto LAB_00161144;
        }
        iVar4 = Layout::helpPressed((Layout *)*puVar13);
        if (iVar4 != 0) {
          uVar3 = *puVar13;
          pSVar6 = (String *)GameText::getText(**(int **)(DAT_001611b0 + 0x160fe0));
          AbyssEngine::String::String((String *)&stack0xffffffcc,pSVar6,false);
          Layout::initHelpWindow(uVar3,(String *)&stack0xffffffcc);
          AbyssEngine::String::~String((String *)&stack0xffffffcc);
        }
      }
      else {
        puVar5 = *(uint **)(DAT_0016119c + 0x160ef6);
        iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar5);
        ctx = *(EVP_PKEY_CTX **)(iVar4 + 0x10);
        *(EVP_PKEY_CTX **)(param_1 + 8) = ctx;
        if (ctx == (EVP_PKEY_CTX *)0x0) {
          this = operator_new(0x1e8);
          pMVar8 = (Mission *)Status::getCampaignMission();
          StarMap::StarMap(this,true,pMVar8,false,-1);
LAB_00161118:
          iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar5);
          *(StarMap **)(iVar4 + 0x10) = this;
          iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule(*puVar5);
          *(undefined4 *)(param_1 + 8) = *(undefined4 *)(iVar4 + 0x10);
        }
        else {
          Status::getCampaignMission();
LAB_00160f80:
          StarMap::init(ctx);
        }
        iVar4 = DAT_001611a0;
        *(undefined1 *)(param_1 + 0x22) = 1;
        Layout::resetWindowDimensions((Layout *)**(undefined4 **)(iVar4 + 0x16113c));
      }
    }
    else {
      iVar4 = StarMap::OnTouchEnd(*(int *)(param_1 + 8),param_2);
      if (iVar4 != 0) {
        if (**(char **)(DAT_0016117c + 0x160e6c) == '\0') {
          uVar12 = **(uint **)(DAT_00161190 + 0x160fbc);
          uVar10 = **(uint **)(DAT_00161194 + 0x160fbe);
          *(undefined4 *)(param_1 + 0x30) = 0;
          *(undefined4 *)(param_1 + 0x34) = 0;
        }
        else {
          if (**(char **)(DAT_00161180 + 0x160e7a) == '\0') {
            uVar12 = 1000;
            uVar10 = 0x514;
            if (**(char **)(DAT_00161184 + 0x161008) == '\0') {
              uVar12 = 500;
              uVar10 = 0x28a;
            }
            uVar11 = uVar10 >> 1;
          }
          else {
            uVar12 = 0x2bf;
            uVar11 = 0x1c9;
            uVar10 = 0x392;
          }
          iVar4 = **(int **)(DAT_0016118c + 0x16102c);
          *(uint *)(param_1 + 0x30) = (**(int **)(DAT_00161188 + 0x16102a) >> 1) - uVar11;
          *(uint *)(param_1 + 0x34) = (iVar4 >> 1) - (uVar12 >> 1);
        }
        *(uint *)(param_1 + 0x38) = uVar10;
        *(uint *)(param_1 + 0x3c) = uVar12;
        uVar3 = 0;
        *(undefined1 *)(param_1 + 0x22) = 0;
        goto LAB_00161144;
      }
    }
  }
  else {
    iVar4 = ChoiceWindow::OnTouchEnd(*(int *)(param_1 + 0xc),param_2);
    if (iVar4 == 1) {
      uVar3 = 0;
      *(undefined1 *)(param_1 + 0x20) = 0;
      goto LAB_00161144;
    }
    if (iVar4 != 0) goto LAB_00160e4e;
    puVar13 = *(undefined4 **)(DAT_00161174 + 0x160dde);
    Status::getFreelanceMission();
    iVar4 = Mission::getType();
    if (iVar4 == 0) {
LAB_00160e10:
      Status::getShip();
      puVar5 = (uint *)Ship::getCargo();
      if (puVar5 != (uint *)0x0) {
        for (uVar12 = 0; uVar12 < *puVar5; uVar12 = uVar12 + 1) {
          iVar4 = Item::isUnsaleable(*(Item **)(puVar5[1] + uVar12 * 4));
          if ((iVar4 != 0) &&
             (iVar4 = Item::getIndex(*(Item **)(puVar5[1] + uVar12 * 4)), iVar4 == 0x74)) {
            pIVar7 = (Item *)Status::getShip();
            Ship::removeCargo(pIVar7);
            *(undefined1 *)(param_1 + 0x23) = 1;
            break;
          }
        }
      }
    }
    else {
      Status::getFreelanceMission();
      iVar4 = Mission::getType();
      if (iVar4 == 3) goto LAB_00160e10;
      Status::getFreelanceMission();
      iVar4 = Mission::getType();
      if (iVar4 == 5) goto LAB_00160e10;
      Status::getFreelanceMission();
      iVar4 = Mission::getType();
      if (iVar4 == 0xb) {
        Status::setPassengers((Status *)*puVar13,0);
      }
    }
    pMVar8 = (Mission *)Status::getFreelanceMission();
    pAVar9 = (Agent *)Mission::getAgent(pMVar8);
    iVar4 = Agent::isGenericAgent(pAVar9);
    if (iVar4 == 0) {
      pMVar8 = (Mission *)Status::getFreelanceMission();
      pAVar9 = (Agent *)Mission::getAgent(pMVar8);
      Agent::setOfferAccepted(pAVar9,false);
    }
    Status::setFreelanceMission((Mission *)*puVar13);
    uVar1 = *(undefined1 *)(param_1 + 0x23);
    init((EVP_PKEY_CTX *)param_1);
    *(undefined1 *)(param_1 + 0x23) = uVar1;
  }
  uVar3 = 0;
LAB_00161144:
  if (*piVar14 == iVar2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

```

## target disasm
```
  00160d74: push {r4,r5,r6,r7,lr}
  00160d76: add r7,sp,#0xc
  00160d78: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00160d7c: mov r11,r0
  00160d7e: ldr r0,[0x001610f0]
  00160d80: mov r5,r2
  00160d82: mov r6,r1
  00160d84: add r0,pc
  00160d86: ldr.w r9,[r0,#0x0]
  00160d8a: ldr.w r0,[r9,#0x0]
  00160d8e: str r0,[sp,#0x10]
  00160d90: ldr.w r0,[r11,#0x40]
  00160d94: cmp r0,#0x1
  00160d96: bne 0x00160dbc
  00160d98: ldr.w r0,[r11,#0x10]
  00160d9c: mov r1,r6
  00160d9e: mov r2,r5
  00160da0: blx 0x000772f0
  00160da4: ldr.w r1,[r11,#0x10]
  00160da8: ldr r2,[r1,#0x0]
  00160daa: cmp r2,#0x0
  00160dac: bne.w 0x00161144
  00160db0: movs r2,#0x0
  00160db2: str.w r2,[r11,#0x40]
  00160db6: movs r2,#0x1
  00160db8: str r2,[r1,#0x0]
  00160dba: b 0x00161144
  00160dbc: ldrb.w r0,[r11,#0x20]
  00160dc0: cmp r0,#0x0
  00160dc2: beq 0x00160e4e
  00160dc4: ldr.w r0,[r11,#0xc]
  00160dc8: mov r1,r6
  00160dca: mov r2,r5
  00160dcc: blx 0x00074770
  00160dd0: cmp r0,#0x1
  00160dd2: beq.w 0x00160f12
  00160dd6: cbnz r0,0x00160e4e
  00160dd8: ldr r0,[0x00161174]
  00160dda: add r0,pc
  00160ddc: ldr.w r8,[r0,#0x0]
  00160de0: ldr.w r0,[r8,#0x0]
  00160de4: blx 0x00073444
  00160de8: blx 0x00072874
  00160dec: cbz r0,0x00160e10
  00160dee: ldr.w r0,[r8,#0x0]
  00160df2: blx 0x00073444
  00160df6: blx 0x00072874
  00160dfa: cmp r0,#0x3
  00160dfc: beq 0x00160e10
  00160dfe: ldr.w r0,[r8,#0x0]
  00160e02: blx 0x00073444
  00160e06: blx 0x00072874
  00160e0a: cmp r0,#0x5
  00160e0c: bne.w 0x0016106a
  00160e10: ldr.w r0,[r8,#0x0]
  00160e14: blx 0x00071a58
  00160e18: blx 0x00073750
  00160e1c: mov r5,r0
  00160e1e: cmp r0,#0x0
  00160e20: beq.w 0x00161084
  00160e24: movs r6,#0x0
  00160e26: b 0x00160e46
  00160e28: ldr r0,[r5,#0x4]
  00160e2a: ldr.w r0,[r0,r6,lsl #0x2]
  00160e2e: blx 0x00074ad0
  00160e32: cbz r0,0x00160e44
  00160e34: ldr r0,[r5,#0x4]
  00160e36: ldr.w r0,[r0,r6,lsl #0x2]
  00160e3a: blx 0x000718d8
  00160e3e: cmp r0,#0x74
  00160e40: beq.w 0x00161050
  00160e44: adds r6,#0x1
  00160e46: ldr r0,[r5,#0x0]
  00160e48: cmp r6,r0
  00160e4a: bcc 0x00160e28
  00160e4c: b 0x00161084
  00160e4e: ldrb.w r0,[r11,#0x22]
  00160e52: cbz r0,0x00160e90
  00160e54: ldr.w r0,[r11,#0x8]
  00160e58: mov r1,r6
  00160e5a: mov r2,r5
  00160e5c: blx 0x000751e4
  00160e60: cmp r0,#0x0
  00160e62: beq.w 0x00161142
  00160e66: ldr r0,[0x0016117c]
  00160e68: add r0,pc
  00160e6a: ldr r0,[r0,#0x0]
  00160e6c: ldrb r0,[r0,#0x0]
  00160e6e: cmp r0,#0x0
  00160e70: beq.w 0x00160fb2
  00160e74: ldr r0,[0x00161180]
  00160e76: add r0,pc
  00160e78: ldr r0,[r0,#0x0]
  00160e7a: ldrb r0,[r0,#0x0]
  00160e7c: cmp r0,#0x0
  00160e7e: beq.w 0x00161002
  00160e82: movw r0,#0x2bf
  00160e86: movw r2,#0x1c9
  00160e8a: movw r1,#0x392
  00160e8e: b 0x00161022
  00160e90: ldr r0,[0x00161198]
  00160e92: add r0,pc
  00160e94: ldr.w r10,[r0,#0x0]
  00160e98: ldr.w r0,[r10,#0x0]
  00160e9c: blx 0x00077290
  00160ea0: cbz r0,0x00160ec6
  00160ea2: movs r4,#0x0
  00160ea4: b 0x00160ebc
  00160ea6: ldr r0,[r0,#0x4]
  00160ea8: mov r1,r6
  00160eaa: mov r2,r5
  00160eac: ldr.w r0,[r0,r4,lsl #0x2]
  00160eb0: blx 0x00074788
  00160eb4: cbz r0,0x00160eba
  00160eb6: str.w r4,[r11,#0x40]
  00160eba: adds r4,#0x1
  00160ebc: ldr.w r0,[r11,#0x14]
  00160ec0: ldr r1,[r0,#0x0]
  00160ec2: cmp r4,r1
  00160ec4: bcc 0x00160ea6
  00160ec6: ldr.w r0,[r11,#0x0]
  00160eca: mov r1,r6
  00160ecc: mov r2,r5
  00160ece: blx 0x00075bc8
  00160ed2: ldr.w r0,[r11,#0x4]
  00160ed6: mov r1,r6
  00160ed8: mov r2,r5
  00160eda: blx 0x00075bc8
  00160ede: ldr.w r0,[r11,#0x24]
  00160ee2: cbz r0,0x00160f1a
  00160ee4: mov r1,r6
  00160ee6: mov r2,r5
  00160ee8: blx 0x00074788
  00160eec: cbz r0,0x00160f1a
  00160eee: ldr r0,[0x0016119c]
  00160ef0: movs r1,#0x5
  00160ef2: add r0,pc
  00160ef4: ldr r6,[r0,#0x0]
  00160ef6: ldr r0,[r6,#0x0]
  00160ef8: blx 0x00075bd4
  00160efc: ldr r5,[r0,#0x10]
  00160efe: str.w r5,[r11,#0x8]
  00160f02: cmp r5,#0x0
  00160f04: beq.w 0x001610c8
  00160f08: ldr.w r0,[r10,#0x0]
  00160f0c: blx 0x0007285c
  00160f10: b 0x00160f80
  00160f12: movs r0,#0x0
  00160f14: strb.w r0,[r11,#0x20]
  00160f18: b 0x00161144
  00160f1a: ldr.w r0,[r11,#0x2c]
  00160f1e: cbz r0,0x00160f4e
  00160f20: mov r1,r6
  00160f22: mov r2,r5
  00160f24: blx 0x00074788
  00160f28: cbz r0,0x00160f4e
  00160f2a: ldr r0,[0x001611a4]
  00160f2c: mov.w r1,#0x1a2
  00160f30: ldr.w r8,[r11,#0xc]
  00160f34: add r0,pc
  00160f36: ldr r0,[r0,#0x0]
  00160f38: ldr r0,[r0,#0x0]
  00160f3a: blx 0x00072f70
  00160f3e: mov r1,r0
  00160f40: mov r0,r8
  00160f42: movs r2,#0x1
  00160f44: movs r4,#0x1
  00160f46: blx 0x000746f8
  00160f4a: strb.w r4,[r11,#0x20]
  00160f4e: ldr.w r0,[r11,#0x28]
  00160f52: cbz r0,0x00160f94
  00160f54: mov r1,r6
  00160f56: mov r2,r5
  00160f58: blx 0x00074788
  00160f5c: cbz r0,0x00160f94
  00160f5e: ldr r0,[0x001611a8]
  00160f60: movs r1,#0x5
  00160f62: add r0,pc
  00160f64: ldr r6,[r0,#0x0]
  00160f66: ldr r0,[r6,#0x0]
  00160f68: blx 0x00075bd4
  00160f6c: ldr r5,[r0,#0x10]
  00160f6e: str.w r5,[r11,#0x8]
  00160f72: cmp r5,#0x0
  00160f74: beq.w 0x001610f4
  00160f78: ldr.w r0,[r10,#0x0]
  00160f7c: blx 0x00073444
  00160f80: mov r2,r0
  00160f82: mov.w r0,#0xffffffff
  00160f86: str r0,[sp,#0x0]
  00160f88: mov r0,r5
  00160f8a: movs r1,#0x1
  00160f8c: movs r3,#0x0
  00160f8e: blx 0x00074554
  00160f92: b 0x00161130
  00160f94: ldr r0,[0x001611ac]
  00160f96: mov r1,r6
  00160f98: mov r2,r5
  00160f9a: add r0,pc
  00160f9c: ldr r4,[r0,#0x0]
  00160f9e: ldr r0,[r4,#0x0]
  00160fa0: blx 0x0007477c
  00160fa4: mov r1,r0
  00160fa6: ldr r0,[r4,#0x0]
  00160fa8: cbz r1,0x00160fca
  00160faa: blx 0x00074dc4
  00160fae: movs r0,#0x1
  00160fb0: b 0x00161144
  00160fb2: ldr r0,[0x00161190]
  00160fb4: movs r2,#0x0
  00160fb6: ldr r1,[0x00161194]
  00160fb8: add r0,pc
  00160fba: add r1,pc
  00160fbc: ldr r0,[r0,#0x0]
  00160fbe: ldr r1,[r1,#0x0]
  00160fc0: ldr r0,[r0,#0x0]
  00160fc2: ldr r1,[r1,#0x0]
  00160fc4: strd r2,r2,[r11,#0x30]
  00160fc8: b 0x00161040
  00160fca: blx 0x000747b8
  00160fce: cmp r0,#0x0
  00160fd0: beq.w 0x00161142
  00160fd4: ldr r0,[0x001611b0]
  00160fd6: movw r1,#0x27b
  00160fda: ldr r4,[r4,#0x0]
  00160fdc: add r0,pc
  00160fde: ldr r0,[r0,#0x0]
  00160fe0: ldr r0,[r0,#0x0]
  00160fe2: blx 0x00072f70
  00160fe6: add r5,sp,#0x4
  00160fe8: mov r1,r0
  00160fea: movs r2,#0x0
  00160fec: mov r0,r5
  00160fee: blx 0x0006f028
  00160ff2: mov r0,r4
  00160ff4: mov r1,r5
  00160ff6: blx 0x000747c4
  00160ffa: add r0,sp,#0x4
  00160ffc: blx 0x0006ee30
  00161000: b 0x00161142
  00161002: ldr r0,[0x00161184]
  00161004: add r0,pc
  00161006: ldr r0,[r0,#0x0]
  00161008: ldrb r1,[r0,#0x0]
  0016100a: mov.w r0,#0x3e8
  0016100e: cmp r1,#0x0
  00161010: movw r1,#0x514
  00161014: it eq
  00161016: mov.eq.w r0,#0x1f4
  0016101a: it eq
  0016101c: movw.eq r1,#0x28a
  00161020: lsrs r2,r1,#0x1
  00161022: ldr r3,[0x00161188]
  00161024: ldr r4,[0x0016118c]
  00161026: add r3,pc
  00161028: add r4,pc
  0016102a: ldr r3,[r3,#0x0]
  0016102c: ldr r6,[r4,#0x0]
  0016102e: ldr r3,[r3,#0x0]
  00161030: rsb r2,r2,r3, asr #0x1
  00161034: ldr r3,[r6,#0x0]
  00161036: asrs r3,r3,#0x1
  00161038: sub.w r3,r3,r0, lsr #0x1
  0016103c: strd r2,r3,[r11,#0x30]
  00161040: str.w r1,[r11,#0x38]
  00161044: str.w r0,[r11,#0x3c]
  00161048: movs r0,#0x0
  0016104a: strb.w r0,[r11,#0x22]
  0016104e: b 0x00161144
  00161050: ldr.w r0,[r8,#0x0]
  00161054: blx 0x00071a58
  00161058: ldr r1,[r5,#0x4]
  0016105a: ldr.w r1,[r1,r6,lsl #0x2]
  0016105e: blx 0x000738e8
  00161062: movs r0,#0x1
  00161064: strb.w r0,[r11,#0x23]
  00161068: b 0x00161084
  0016106a: ldr.w r0,[r8,#0x0]
  0016106e: blx 0x00073444
  00161072: blx 0x00072874
  00161076: cmp r0,#0xb
  00161078: bne 0x00161084
  0016107a: ldr.w r0,[r8,#0x0]
  0016107e: movs r1,#0x0
  00161080: blx 0x00075148
  00161084: ldr.w r0,[r8,#0x0]
  00161088: blx 0x00073444
  0016108c: blx 0x00073438
  00161090: blx 0x00071b9c
  00161094: cbnz r0,0x001610a8
  00161096: ldr.w r0,[r8,#0x0]
  0016109a: blx 0x00073444
  0016109e: blx 0x00073438
  001610a2: movs r1,#0x0
  001610a4: blx 0x00071b18
  001610a8: ldr r0,[0x00161178]
  001610aa: add r0,pc
  001610ac: ldr r0,[r0,#0x0]
  001610ae: ldr r1,[r0,#0x0]
  001610b0: ldr.w r0,[r8,#0x0]
  001610b4: blx 0x00073f90
  001610b8: mov r0,r11
  001610ba: ldrb.w r4,[r11,#0x23]
  001610be: blx 0x000753d0
  001610c2: strb.w r4,[r11,#0x23]
  001610c6: b 0x00161142
  001610c8: mov.w r0,#0x1e8
  001610cc: blx 0x0006eb24
  001610d0: mov r5,r0
  001610d2: ldr.w r0,[r10,#0x0]
  001610d6: blx 0x0007285c
  001610da: mov r2,r0
  001610dc: mov.w r0,#0xffffffff
  001610e0: movs r1,#0x1
  001610e2: str r0,[sp,#0x0]
  001610e4: mov r0,r5
  001610e6: movs r3,#0x0
  001610e8: blx 0x00075424
  001610ec: b 0x00161118
  001610f4: mov.w r0,#0x1e8
  001610f8: blx 0x0006eb24
  001610fc: mov r5,r0
  001610fe: ldr.w r0,[r10,#0x0]
  00161102: blx 0x00073444
  00161106: mov r2,r0
  00161108: mov.w r0,#0xffffffff
  0016110c: movs r1,#0x1
  0016110e: str r0,[sp,#0x0]
  00161110: mov r0,r5
  00161112: movs r3,#0x0
  00161114: blx 0x00075424
  00161118: ldr r0,[r6,#0x0]
  0016111a: movs r1,#0x5
  0016111c: blx 0x00075bd4
  00161120: str r5,[r0,#0x10]
  00161122: movs r1,#0x5
  00161124: ldr r0,[r6,#0x0]
  00161126: blx 0x00075bd4
  0016112a: ldr r0,[r0,#0x10]
  0016112c: str.w r0,[r11,#0x8]
  00161130: ldr r0,[0x001611a0]
  00161132: movs r1,#0x1
  00161134: strb.w r1,[r11,#0x22]
  00161138: add r0,pc
  0016113a: ldr r0,[r0,#0x0]
  0016113c: ldr r0,[r0,#0x0]
  0016113e: blx 0x00074dc4
  00161142: movs r0,#0x0
  00161144: ldr r1,[sp,#0x10]
  00161146: ldr.w r2,[r9,#0x0]
  0016114a: subs r1,r2,r1
  0016114c: ittt eq
  0016114e: add.eq sp,#0x14
  00161150: pop.eq.w {r8,r9,r10,r11}
  00161154: pop.eq {r4,r5,r6,r7,pc}
  00161156: blx 0x0006e824
```
