# PlayerEgo::dockToDockingPoint
elf 0x9f9f4 body 686
Sig: undefined __stdcall dockToDockingPoint(KIPlayer * param_1, Radar * param_2)

## decompile
```c

/* PlayerEgo::dockToDockingPoint(KIPlayer*, Radar*) */

void PlayerEgo::dockToDockingPoint(KIPlayer *param_1,Radar *param_2)

{
  int iVar1;
  undefined4 *puVar2;
  void *pvVar3;
  undefined4 uVar4;
  EaseInOutMatrix *pEVar5;
  int *piVar6;
  float extraout_s1;
  float extraout_s2;
  Matrix aMStack_a4 [60];
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  int local_28;
  
  piVar6 = *(int **)(DAT_000afcb0 + 0xafa08);
  local_28 = *piVar6;
  iVar1 = isDead((PlayerEgo *)param_1);
  if (iVar1 != 0) goto LAB_000afc8c;
  if (param_1[0x356] == (KIPlayer)0x0) {
    if (param_2 != (Radar *)0x0) {
      param_1[0x356] = (KIPlayer)0x1;
      *(Radar **)(param_1 + 0x1bc) = param_2;
      *(undefined4 *)(param_1 + 0x1d8) = 0x578;
      *(undefined4 *)(param_1 + 0x1dc) = 0;
      param_1[0x145] = (KIPlayer)0x1;
      *(undefined4 *)(param_1 + 0x1c4) = 0;
    }
    goto LAB_000afc8c;
  }
  if (param_2 == (Radar *)0x0) {
LAB_000afc08:
    PlayEngineSound();
    param_1[0x356] = (KIPlayer)0x0;
    param_1[0x1a1] = (KIPlayer)0x0;
    param_1[0x145] = (KIPlayer)0x0;
    *(undefined4 *)(param_1 + 0x1bc) = 0;
    local_68 = 0;
    local_64 = 0;
    local_60 = 0;
    AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x1c8),(Vector *)&local_68);
    TargetFollowCamera::setActive(*(TargetFollowCamera **)(param_1 + 0x88),true);
    TargetFollowCamera::setLookAtCam(SUB41(*(undefined4 *)(param_1 + 0x88),0));
    TargetFollowCamera::useTargetsUpVector(SUB41(*(undefined4 *)(param_1 + 0x88),0));
    Player::resetGunDelay(*(Player **)param_1,0);
    *(undefined4 *)(param_1 + 0x1c4) = 0;
    setExhaustVisible((PlayerEgo *)param_1,true);
    if (*(int *)(param_1 + 0x36c) != 0) {
      SpacePoint::giveFree();
      *(undefined4 *)(param_1 + 0x36c) = 0;
    }
  }
  else {
    getPosition();
    iVar1 = KIPlayer::getNearestNavigationPoint((Vector *)param_2,(SpacePoint *)&local_68);
    if (iVar1 == 0) {
      if (param_2[0x70] != (Radar)0x0) {
        param_2[0x8c] = (Radar)0x1;
      }
      goto LAB_000afc08;
    }
    if (param_2[0x70] != (Radar)0x0) {
      param_2[0x8c] = (Radar)0x1;
    }
    setTurretMode(SUB41(param_1,0));
    puVar2 = (undefined4 *)(DAT_000afcb4 + 0xafa68);
    param_1[0x1a1] = (KIPlayer)0x0;
    AbyssEngine::PaintCanvas::CameraSetCurrent(*(PaintCanvas **)*puVar2,**(uint **)(param_1 + 0x88))
    ;
    LevelScript::resetCamera(*(Level **)(param_1 + 0x10));
    PlayEngineSound();
    *(undefined4 *)(param_1 + 0x1c4) = 3;
    TargetFollowCamera::setLookAtCam(SUB41(*(undefined4 *)(param_1 + 0x88),0));
    TargetFollowCamera::useTargetsUpVector(SUB41(*(undefined4 *)(param_1 + 0x88),0));
    if (*(EaseInOutMatrix **)(param_1 + 0x358) != (EaseInOutMatrix *)0x0) {
      pvVar3 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix
                                 (*(EaseInOutMatrix **)(param_1 + 0x358));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(param_1 + 0x358) = 0;
    getPosition();
    iVar1 = KIPlayer::getNearestNavigationPoint((Vector *)param_2,(SpacePoint *)&local_68);
    uVar4 = AEGeometry::getMatrix();
    __aeabi_memcpy(&local_68,uVar4,0x3c);
    AbyssEngine::AEMath::MatrixSetTranslation
              (aMStack_a4,*(float *)(iVar1 + 8),extraout_s1,extraout_s2);
    pEVar5 = operator_new(0xf4);
    puVar2 = (undefined4 *)AEGeometry::getMatrix();
    AbyssEngine::EaseInOutMatrix::EaseInOutMatrix
              (pEVar5,*puVar2,puVar2[1],puVar2[2],puVar2[3],puVar2[4],puVar2[5],puVar2[6],puVar2[7],
               puVar2[8],puVar2[9],puVar2[10],puVar2[0xb],puVar2[0xc],puVar2[0xd],puVar2[0xe],
               local_68,local_64,local_60,local_5c,local_58,uStack_54,uStack_50,uStack_4c,local_48,
               uStack_44,local_40,uStack_3c,local_38,uStack_34,uStack_30,3000);
    *(EaseInOutMatrix **)(param_1 + 0x358) = pEVar5;
    setExhaustVisible((PlayerEgo *)param_1,true);
  }
  if (*(HackingGame **)(param_1 + 0x1e8) != (HackingGame *)0x0) {
    pvVar3 = (void *)HackingGame::~HackingGame(*(HackingGame **)(param_1 + 0x1e8));
    operator_delete(pvVar3);
    *(undefined4 *)(param_1 + 0x1e8) = 0;
    Hud::setHackingGameActive(*(Hud **)(param_1 + 0x220),false);
  }
LAB_000afc8c:
  if (*piVar6 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000af9f4: push {r4,r5,r6,r7,lr}
  000af9f6: add r7,sp,#0xc
  000af9f8: push {r8,r9,r10,r11}
  000af9fc: sub sp,#0x144
  000af9fe: mov r4,r0
  000afa00: ldr r0,[0x000afcb0]
  000afa02: mov r5,r1
  000afa04: add r0,pc
  000afa06: ldr.w r8,[r0,#0x0]
  000afa0a: ldr.w r0,[r8,#0x0]
  000afa0e: str r0,[sp,#0x140]
  000afa10: mov r0,r4
  000afa12: blx 0x000724b4
  000afa16: cmp r0,#0x0
  000afa18: bne.w 0x000afc8c
  000afa1c: ldrb.w r0,[r4,#0x356]
  000afa20: cmp r0,#0x0
  000afa22: beq.w 0x000afbd4
  000afa26: cmp r5,#0x0
  000afa28: beq.w 0x000afbf6
  000afa2c: add r6,sp,#0x100
  000afa2e: mov r1,r4
  000afa30: mov r0,r6
  000afa32: blx 0x0007264c
  000afa36: ldr.w r2,[r4,#0x36c]
  000afa3a: mov r0,r5
  000afa3c: mov r1,r6
  000afa3e: blx 0x00072b5c
  000afa42: ldrb.w r1,[r5,#0x70]
  000afa46: cmp r0,#0x0
  000afa48: beq.w 0x000afbfc
  000afa4c: cbz r1,0x000afa54
  000afa4e: movs r0,#0x1
  000afa50: strb.w r0,[r5,#0x8c]
  000afa54: mov r0,r4
  000afa56: movs r1,#0x0
  000afa58: movs r6,#0x0
  000afa5a: blx 0x00072b08
  000afa5e: ldr r0,[0x000afcb4]
  000afa60: ldr.w r1,[r4,#0x88]
  000afa64: add r0,pc
  000afa66: strb.w r6,[r4,#0x1a1]
  000afa6a: ldr r0,[r0,#0x0]
  000afa6c: ldr r1,[r1,#0x0]
  000afa6e: ldr r0,[r0,#0x0]
  000afa70: blx 0x0006fd9c
  000afa74: ldrd r1,r0,[r4,#0xc]
  000afa78: blx 0x0007246c
  000afa7c: mov r0,r4
  000afa7e: blx 0x00072b68
  000afa82: ldr.w r0,[r4,#0x88]
  000afa86: movs r1,#0x3
  000afa88: str.w r1,[r4,#0x1c4]
  000afa8c: movs r1,#0x1
  000afa8e: blx 0x00072670
  000afa92: ldr.w r0,[r4,#0x88]
  000afa96: movs r1,#0x0
  000afa98: blx 0x0007267c
  000afa9c: ldr.w r0,[r4,#0x358]
  000afaa0: cbz r0,0x000afaaa
  000afaa2: blx 0x00072340
  000afaa6: blx 0x0006eb48
  000afaaa: str.w r6,[r4,#0x358]
  000afaae: add r6,sp,#0x100
  000afab0: mov r1,r4
  000afab2: mov r0,r6
  000afab4: blx 0x0007264c
  000afab8: ldr.w r2,[r4,#0x36c]
  000afabc: mov r0,r5
  000afabe: mov r1,r6
  000afac0: blx 0x00072b5c
  000afac4: mov r5,r0
  000afac6: ldr r0,[r4,#0x8]
  000afac8: blx 0x000721cc
  000afacc: add r6,sp,#0x100
  000aface: mov r1,r0
  000afad0: movs r2,#0x3c
  000afad2: mov r0,r6
  000afad4: blx 0x0006f1e4
  000afad8: ldrd r2,r3,[r5,#0x0]
  000afadc: add r0,sp,#0xc4
  000afade: vldr.32 s0,[r5,#0x8]
  000afae2: mov r1,r6
  000afae4: vstr.32 s0,[sp]
  000afae8: blx 0x0006f820
  000afaec: movs r0,#0xf4
  000afaee: blx 0x0006eb24
  000afaf2: mov r5,r0
  000afaf4: ldr r0,[r4,#0x8]
  000afaf6: blx 0x000721cc
  000afafa: ldr r1,[r0,#0x0]
  000afafc: add.w lr,sp,#0x110
  000afb00: str r1,[sp,#0xbc]
  000afb02: add r2,sp,#0x130
  000afb04: ldr r1,[r0,#0x4]
  000afb06: str r1,[sp,#0xb8]
  000afb08: ldr r1,[r0,#0x8]
  000afb0a: str r1,[sp,#0xb4]
  000afb0c: ldr r1,[r0,#0xc]
  000afb0e: str r1,[sp,#0xb0]
  000afb10: ldr r1,[r0,#0x10]
  000afb12: str r1,[sp,#0xac]
  000afb14: ldr r1,[r0,#0x14]
  000afb16: str r1,[sp,#0xa8]
  000afb18: ldr r1,[r0,#0x18]
  000afb1a: str r1,[sp,#0xa4]
  000afb1c: ldr r1,[r0,#0x1c]
  000afb1e: str r1,[sp,#0xa0]
  000afb20: ldr r1,[r0,#0x20]
  000afb22: str r1,[sp,#0x9c]
  000afb24: ldr r1,[r0,#0x24]
  000afb26: str r1,[sp,#0x98]
  000afb28: ldr r1,[r0,#0x28]
  000afb2a: str r1,[sp,#0x94]
  000afb2c: ldr r1,[r0,#0x2c]
  000afb2e: str r1,[sp,#0x90]
  000afb30: ldr r1,[r0,#0x30]
  000afb32: str r1,[sp,#0x8c]
  000afb34: ldr r1,[r0,#0x34]
  000afb36: ldr r0,[r0,#0x38]
  000afb38: str r0,[sp,#0x84]
  000afb3a: ldr r0,[sp,#0x100]
  000afb3c: str r0,[sp,#0x80]
  000afb3e: ldr r0,[sp,#0x104]
  000afb40: str r0,[sp,#0x7c]
  000afb42: ldr r0,[sp,#0x108]
  000afb44: str r0,[sp,#0x78]
  000afb46: ldr r0,[sp,#0x10c]
  000afb48: str r1,[sp,#0x88]
  000afb4a: str r0,[sp,#0x74]
  000afb4c: ldm.w lr,{r9,r10,r11,lr}
  000afb50: str r5,[sp,#0xc0]
  000afb52: ldrd r5,r6,[sp,#0x120]
  000afb56: ldrd r3,r12,[sp,#0x128]
  000afb5a: ldmia r2,{r0,r1,r2}
  000afb5c: str r0,[sp,#0x60]
  000afb5e: movw r0,#0xbb8
  000afb62: str r0,[sp,#0x6c]
  000afb64: add r0,sp,#0x40
  000afb66: str r1,[sp,#0x64]
  000afb68: str r2,[sp,#0x68]
  000afb6a: stm r0,{r9,r10,r11,lr}
  000afb6e: str r5,[sp,#0x50]
  000afb70: ldr r5,[sp,#0xc0]
  000afb72: strd r6,r3,[sp,#0x54]
  000afb76: ldr r0,[sp,#0x90]
  000afb78: str r0,[sp,#0x20]
  000afb7a: ldr r0,[sp,#0x8c]
  000afb7c: str r0,[sp,#0x24]
  000afb7e: ldr r0,[sp,#0x88]
  000afb80: str r0,[sp,#0x28]
  000afb82: ldr r0,[sp,#0x84]
  000afb84: str r0,[sp,#0x2c]
  000afb86: ldr r0,[sp,#0x80]
  000afb88: str r0,[sp,#0x30]
  000afb8a: ldr r0,[sp,#0x7c]
  000afb8c: str r0,[sp,#0x34]
  000afb8e: ldr r0,[sp,#0x78]
  000afb90: str r0,[sp,#0x38]
  000afb92: ldr r0,[sp,#0x74]
  000afb94: str r0,[sp,#0x3c]
  000afb96: ldr r0,[sp,#0xb0]
  000afb98: str r0,[sp,#0x0]
  000afb9a: ldr r0,[sp,#0xac]
  000afb9c: str r0,[sp,#0x4]
  000afb9e: ldr r0,[sp,#0xa8]
  000afba0: str r0,[sp,#0x8]
  000afba2: ldr r0,[sp,#0xa4]
  000afba4: str r0,[sp,#0xc]
  000afba6: ldr r0,[sp,#0xa0]
  000afba8: str r0,[sp,#0x10]
  000afbaa: ldr r0,[sp,#0x9c]
  000afbac: str r0,[sp,#0x14]
  000afbae: ldr r0,[sp,#0x98]
  000afbb0: str r0,[sp,#0x18]
  000afbb2: ldr r0,[sp,#0x94]
  000afbb4: str r0,[sp,#0x1c]
  000afbb6: mov r0,r5
  000afbb8: str.w r12,[sp,#0x5c]
  000afbbc: ldr r1,[sp,#0xbc]
  000afbbe: ldrd r3,r2,[sp,#0xb4]
  000afbc2: blx 0x00072b74
  000afbc6: mov r0,r4
  000afbc8: movs r1,#0x1
  000afbca: str.w r5,[r4,#0x358]
  000afbce: blx 0x00072af0
  000afbd2: b 0x000afc6e
  000afbd4: cmp r5,#0x0
  000afbd6: beq 0x000afc8c
  000afbd8: movs r0,#0x1
  000afbda: movs r1,#0x0
  000afbdc: mov.w r2,#0x578
  000afbe0: strb.w r0,[r4,#0x356]
  000afbe4: str.w r5,[r4,#0x1bc]
  000afbe8: strd r2,r1,[r4,#0x1d8]
  000afbec: strb.w r0,[r4,#0x145]
  000afbf0: str.w r1,[r4,#0x1c4]
  000afbf4: b 0x000afc8c
  000afbf6: add.w r6,r4,#0x36c
  000afbfa: b 0x000afc08
  000afbfc: add.w r6,r4,#0x36c
  000afc00: cbz r1,0x000afc08
  000afc02: movs r0,#0x1
  000afc04: strb.w r0,[r5,#0x8c]
  000afc08: mov r0,r4
  000afc0a: blx 0x00072b68
  000afc0e: add.w r0,r4,#0x1c8
  000afc12: add r1,sp,#0x100
  000afc14: movs r5,#0x0
  000afc16: strb.w r5,[r4,#0x356]
  000afc1a: strb.w r5,[r4,#0x1a1]
  000afc1e: strb.w r5,[r4,#0x145]
  000afc22: str.w r5,[r4,#0x1bc]
  000afc26: strd r5,r5,[sp,#0x100]
  000afc2a: str r5,[sp,#0x108]
  000afc2c: blx 0x0006eb3c
  000afc30: ldr.w r0,[r4,#0x88]
  000afc34: movs r1,#0x1
  000afc36: blx 0x00072acc
  000afc3a: ldr.w r0,[r4,#0x88]
  000afc3e: movs r1,#0x0
  000afc40: blx 0x00072670
  000afc44: ldr.w r0,[r4,#0x88]
  000afc48: movs r1,#0x0
  000afc4a: blx 0x0007267c
  000afc4e: ldr r0,[r4,#0x0]
  000afc50: movs r1,#0x0
  000afc52: blx 0x00072ad8
  000afc56: mov r0,r4
  000afc58: movs r1,#0x1
  000afc5a: str.w r5,[r4,#0x1c4]
  000afc5e: blx 0x00072af0
  000afc62: ldr.w r0,[r4,#0x36c]
  000afc66: cbz r0,0x000afc6e
  000afc68: blx 0x00072b80
  000afc6c: str r5,[r6,#0x0]
  000afc6e: ldr.w r0,[r4,#0x1e8]
  000afc72: cbz r0,0x000afc8c
  000afc74: blx 0x0007273c
  000afc78: blx 0x0006eb48
  000afc7c: ldr.w r0,[r4,#0x220]
  000afc80: movs r1,#0x0
  000afc82: str.w r1,[r4,#0x1e8]
  000afc86: movs r1,#0x0
  000afc88: blx 0x0007270c
  000afc8c: ldr r0,[sp,#0x140]
  000afc8e: ldr.w r1,[r8,#0x0]
  000afc92: subs r0,r1,r0
  000afc94: ittt eq
  000afc96: add.eq sp,#0x144
  000afc98: pop.eq.w {r8,r9,r10,r11}
  000afc9c: pop.eq {r4,r5,r6,r7,pc}
  000afc9e: blx 0x0006e824
```
