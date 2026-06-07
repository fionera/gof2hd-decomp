# PlayerEgo::dockToAsteroid
elf 0x9f4a0 body 200
Sig: undefined __stdcall dockToAsteroid(KIPlayer * param_1, Radar * param_2)

## decompile
```c

/* PlayerEgo::dockToAsteroid(KIPlayer*, Radar*) */

void PlayerEgo::dockToAsteroid(KIPlayer *param_1,Radar *param_2)

{
  void *pvVar1;
  float fVar2;
  Radar *in_r2;
  int *piVar3;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_000af56c + 0xaf4b4);
  local_1c = *piVar3;
  if (param_1[0x1c0] == (KIPlayer)0x0) {
    if (param_2 != (Radar *)0x0) {
      param_1[0x1c0] = (KIPlayer)0x1;
      *(undefined4 *)(param_1 + 0x1dc) = 0;
      *(Radar **)(param_1 + 0x1bc) = param_2;
      fVar2 = (float)PlayerAsteroid::getScaling((PlayerAsteroid *)param_2);
      fVar2 = fVar2 * DAT_000af568;
      param_1[0x145] = (KIPlayer)0x1;
      *(undefined4 *)(param_1 + 0x1c4) = 0;
      *(int *)(param_1 + 0x1d8) = (int)fVar2;
    }
  }
  else {
    PlayerAsteroid::setRotationEnabled(*(PlayerAsteroid **)(param_1 + 0x1bc),true);
    param_1[0x145] = (KIPlayer)0x0;
    param_1[0x1c0] = (KIPlayer)0x0;
    *(undefined4 *)(param_1 + 0x1bc) = 0;
    local_28 = 0;
    uStack_24 = 0;
    local_20 = 0;
    AbyssEngine::AEMath::Vector::operator=((Vector *)(param_1 + 0x1c8),(Vector *)&local_28);
    TargetFollowCamera::setActive(*(TargetFollowCamera **)(param_1 + 0x88),true);
    Player::resetGunDelay(*(Player **)param_1,0);
    if (*(MiningGame **)(param_1 + 0x1e4) != (MiningGame *)0x0) {
      pvVar1 = (void *)MiningGame::~MiningGame(*(MiningGame **)(param_1 + 0x1e4));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(param_1 + 0x1e4) = 0;
    Radar::unlockAsteroid(in_r2);
    *(undefined4 *)(param_1 + 0x1c4) = 0;
    setExhaustVisible((PlayerEgo *)param_1,true);
  }
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000af4a0: push {r4,r5,r6,r7,lr}
  000af4a2: add r7,sp,#0xc
  000af4a4: push.w r8
  000af4a8: sub sp,#0x10
  000af4aa: mov r4,r0
  000af4ac: ldr r0,[0x000af56c]
  000af4ae: mov r8,r2
  000af4b0: add r0,pc
  000af4b2: ldr r6,[r0,#0x0]
  000af4b4: ldr r0,[r6,#0x0]
  000af4b6: str r0,[sp,#0xc]
  000af4b8: ldrb.w r0,[r4,#0x1c0]
  000af4bc: cbz r0,0x000af51e
  000af4be: ldr.w r0,[r4,#0x1bc]
  000af4c2: movs r1,#0x1
  000af4c4: blx 0x00072ac0
  000af4c8: add.w r0,r4,#0x1c8
  000af4cc: movs r5,#0x0
  000af4ce: mov r1,sp
  000af4d0: strb.w r5,[r4,#0x145]
  000af4d4: strb.w r5,[r4,#0x1c0]
  000af4d8: str.w r5,[r4,#0x1bc]
  000af4dc: strd r5,r5,[sp,#0x0]
  000af4e0: str r5,[sp,#0x8]
  000af4e2: blx 0x0006eb3c
  000af4e6: ldr.w r0,[r4,#0x88]
  000af4ea: movs r1,#0x1
  000af4ec: blx 0x00072acc
  000af4f0: ldr r0,[r4,#0x0]
  000af4f2: movs r1,#0x0
  000af4f4: blx 0x00072ad8
  000af4f8: ldr.w r0,[r4,#0x1e4]
  000af4fc: cbz r0,0x000af506
  000af4fe: blx 0x00072328
  000af502: blx 0x0006eb48
  000af506: mov r0,r8
  000af508: str.w r5,[r4,#0x1e4]
  000af50c: blx 0x00072ae4
  000af510: mov r0,r4
  000af512: movs r1,#0x1
  000af514: str.w r5,[r4,#0x1c4]
  000af518: blx 0x00072af0
  000af51c: b 0x000af554
  000af51e: cbz r1,0x000af554
  000af520: mov.w r8,#0x1
  000af524: mov r0,r1
  000af526: movs r5,#0x0
  000af528: strb.w r8,[r4,#0x1c0]
  000af52c: str.w r5,[r4,#0x1dc]
  000af530: str.w r1,[r4,#0x1bc]
  000af534: blx 0x00072afc
  000af538: vldr.32 s0,[pc,#0x2c]
  000af53c: vmov s2,r0
  000af540: vmul.f32 s0,s2,s0
  000af544: vcvt.s32.f32 s0,s0
  000af548: strb.w r8,[r4,#0x145]
  000af54c: str.w r5,[r4,#0x1c4]
  000af550: vstr.32 s0,[r4,#0x1d8]
  000af554: ldr r0,[sp,#0xc]
  000af556: ldr r1,[r6,#0x0]
  000af558: subs r0,r1,r0
  000af55a: ittt eq
  000af55c: add.eq sp,#0x10
  000af55e: pop.eq.w r8
  000af562: pop.eq {r4,r5,r6,r7,pc}
  000af564: blx 0x0006e824
```
