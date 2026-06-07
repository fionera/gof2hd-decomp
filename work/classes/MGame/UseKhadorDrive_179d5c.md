# MGame::UseKhadorDrive
elf 0x179d5c body 572
Sig: undefined __stdcall UseKhadorDrive(void)

## decompile
```c

/* MGame::UseKhadorDrive() */

void MGame::UseKhadorDrive(void)

{
  int in_r0;
  int iVar1;
  Station *this;
  int iVar2;
  undefined4 uVar3;
  StarMap *this_00;
  Engine *this_01;
  undefined4 uVar4;
  String *pSVar5;
  int *piVar6;
  
  iVar1 = PlayerEgo::isChargingDrive();
  if (iVar1 != 0) {
    return;
  }
  piVar6 = *(int **)(DAT_00189f9c + 0x189d7e);
  Status::getMission();
  iVar1 = Mission::isEmpty();
  if (((((iVar1 == 0) && (iVar1 = Status::getCurrentCampaignMission(), iVar1 != 0x4e)) &&
       (iVar1 = Mission::getType(), iVar1 != 0xb)) &&
      ((iVar1 = Mission::getType(), iVar1 != 0 && (iVar1 = Mission::getType(), iVar1 != 0xbd)))) &&
     ((iVar1 = Mission::getType(), iVar1 != 0xd &&
      ((iVar1 = Mission::getType(), iVar1 != 0xab && (iVar1 = Mission::getType(), iVar1 != 0xac)))))
     ) {
LAB_00189dfc:
    uVar4 = *(undefined4 *)(in_r0 + 0x74);
    uVar3 = Level::getPlayer();
    (*(code *)(DAT_001ac7b4 + 0x1ac7b8))(uVar4,0x15,uVar3,0);
    return;
  }
  iVar1 = Status::getCurrentCampaignMission();
  if ((iVar1 == 0x41) && (iVar1 = Status::inAlienOrbit(), iVar1 == 0)) {
    this = (Station *)Status::getStation();
    iVar1 = Station::getIndex(this);
    Status::getCampaignMission();
    iVar2 = Mission::getTargetStation();
    if (iVar1 == iVar2) goto LAB_00189dfc;
  }
  PlayerEgo::resetGunDelay();
  iVar1 = Status::getCurrentCampaignMission();
  if (iVar1 == 0x4e) {
    **(undefined4 **)(DAT_00189fac + 0x189e36) = *(undefined4 *)(*piVar6 + 0x78);
    *(undefined1 *)(in_r0 + 0xdd) = 1;
    startChargingJumpDrive();
    *(undefined1 *)(in_r0 + 0x5d) = 0;
    resumeSounds();
    *(undefined1 *)(in_r0 + 0xd6) = 0;
    Hud::closeHudMenu();
    (*(code *)(DAT_001ac804 + 0x1ac808))(*piVar6,1);
    return;
  }
  iVar1 = Status::inAlienOrbit();
  if (iVar1 == 0) {
    iVar1 = PlayerEgo::hasVolatileGoods();
    if (iVar1 != 0) {
      pSVar5 = *(String **)(in_r0 + 0x94);
      GameText::getText(**(int **)(DAT_00189fa8 + 0x189ed2));
      ChoiceWindow::set(pSVar5);
      *(undefined1 *)(in_r0 + 0x5d) = 1;
      *(undefined1 *)(in_r0 + 0xce) = 1;
      *(undefined1 *)(in_r0 + 0x111) = 1;
      *(undefined1 *)(in_r0 + 0xd6) = 0;
      Hud::closeHudMenu();
      pauseSounds();
      *(undefined1 *)(in_r0 + 0x17c) = 0;
      return;
    }
    if (*(int *)(in_r0 + 0x90) == 0) {
      this_00 = operator_new(0x1e8);
      StarMap::StarMap(this_00,false,(Mission *)0x0,false,-1);
      *(StarMap **)(in_r0 + 0x90) = this_00;
    }
    this_01 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
    AbyssEngine::Engine::SetPostEffect(this_01,DAT_00189f98,false);
    StarMap::initLights(*(StarMap **)(in_r0 + 0x90));
    *(undefined1 *)(in_r0 + 0xdd) = 1;
    StarMap::setJumpMapMode(SUB41(*(undefined4 *)(in_r0 + 0x90),0),true);
    iVar1 = Status::inAlienOrbit();
    if (iVar1 == 0) {
      StarMap::askForJumpIntoAlienWorld(*(StarMap **)(in_r0 + 0x90));
    }
    *(undefined1 *)(in_r0 + 0x5d) = 1;
    *(undefined1 *)(in_r0 + 199) = 1;
    pauseSounds();
    uVar3 = *(undefined4 *)(in_r0 + 0x74);
    *(undefined1 *)(in_r0 + 0xd6) = 0;
  }
  else {
    iVar1 = Status::getCurrentCampaignMission();
    if (iVar1 == 0x50) {
      *(undefined4 *)(*piVar6 + 0x84) = 100;
    }
    uVar3 = Galaxy::getStation(**(int **)(DAT_00189fa0 + 0x189e8e));
    **(undefined4 **)(DAT_00189fa4 + 0x189e9a) = uVar3;
    *(undefined1 *)(in_r0 + 0xdd) = 1;
    startChargingJumpDrive();
    *(undefined1 *)(in_r0 + 0x5d) = 0;
    resumeSounds();
    uVar3 = *(undefined4 *)(in_r0 + 0x74);
    *(undefined1 *)(in_r0 + 0xd6) = 0;
  }
  (*(code *)(DAT_001ac814 + 0x1ac818))(uVar3);
  return;
}

```

## target disasm
```
  00189d5c: push {r4,r5,r6,r7,lr}
  00189d5e: add r7,sp,#0xc
  00189d60: push.w r8
  00189d64: sub sp,#0x8
  00189d66: mov r4,r0
  00189d68: ldr r0,[r0,#0x58]
  00189d6a: blx 0x00077a7c
  00189d6e: cbz r0,0x00189d78
  00189d70: add sp,#0x8
  00189d72: pop.w r8
  00189d76: pop {r4,r5,r6,r7,pc}
  00189d78: ldr r0,[0x00189f9c]
  00189d7a: add r0,pc
  00189d7c: ldr r6,[r0,#0x0]
  00189d7e: ldr r0,[r6,#0x0]
  00189d80: blx 0x00072850
  00189d84: mov r5,r0
  00189d86: blx 0x00072868
  00189d8a: cbnz r0,0x00189dd0
  00189d8c: ldr r0,[r6,#0x0]
  00189d8e: blx 0x00071770
  00189d92: cmp r0,#0x4e
  00189d94: beq 0x00189dd0
  00189d96: mov r0,r5
  00189d98: blx 0x00072874
  00189d9c: cmp r0,#0xb
  00189d9e: beq 0x00189dd0
  00189da0: mov r0,r5
  00189da2: blx 0x00072874
  00189da6: cbz r0,0x00189dd0
  00189da8: mov r0,r5
  00189daa: blx 0x00072874
  00189dae: cmp r0,#0xbd
  00189db0: beq 0x00189dd0
  00189db2: mov r0,r5
  00189db4: blx 0x00072874
  00189db8: cmp r0,#0xd
  00189dba: beq 0x00189dd0
  00189dbc: mov r0,r5
  00189dbe: blx 0x00072874
  00189dc2: cmp r0,#0xab
  00189dc4: beq 0x00189dd0
  00189dc6: mov r0,r5
  00189dc8: blx 0x00072874
  00189dcc: cmp r0,#0xac
  00189dce: bne 0x00189dfc
  00189dd0: ldr r0,[r6,#0x0]
  00189dd2: blx 0x00071770
  00189dd6: cmp r0,#0x41
  00189dd8: bne 0x00189e1a
  00189dda: ldr r0,[r6,#0x0]
  00189ddc: blx 0x000723d0
  00189de0: cbnz r0,0x00189e1a
  00189de2: ldr r0,[r6,#0x0]
  00189de4: blx 0x00071c14
  00189de8: blx 0x00071824
  00189dec: mov r5,r0
  00189dee: ldr r0,[r6,#0x0]
  00189df0: blx 0x0007285c
  00189df4: blx 0x00073738
  00189df8: cmp r5,r0
  00189dfa: bne 0x00189e1a
  00189dfc: ldrd r5,r0,[r4,#0x74]
  00189e00: blx 0x00072034
  00189e04: mov r2,r0
  00189e06: mov r0,r5
  00189e08: movs r1,#0x15
  00189e0a: movs r3,#0x0
  00189e0c: add sp,#0x8
  00189e0e: pop.w r8
  00189e12: pop.w {r4,r5,r6,r7,lr}
  00189e16: b.w 0x001ac7a8
  00189e1a: ldr r0,[r4,#0x58]
  00189e1c: blx 0x00072ca0
  00189e20: ldr r0,[r6,#0x0]
  00189e22: blx 0x00071770
  00189e26: mov r1,r0
  00189e28: ldr r0,[r6,#0x0]
  00189e2a: cmp r1,#0x4e
  00189e2c: bne 0x00189e6c
  00189e2e: ldr r1,[0x00189fac]
  00189e30: ldr r0,[r0,#0x78]
  00189e32: add r1,pc
  00189e34: ldr r1,[r1,#0x0]
  00189e36: str r0,[r1,#0x0]
  00189e38: movs r0,#0x1
  00189e3a: strb.w r0,[r4,#0xdd]
  00189e3e: mov r0,r4
  00189e40: blx 0x00078280
  00189e44: movs r5,#0x0
  00189e46: mov r0,r4
  00189e48: strb.w r5,[r4,#0x5d]
  00189e4c: blx 0x0007828c
  00189e50: ldr r0,[r4,#0x74]
  00189e52: strb.w r5,[r4,#0xd6]
  00189e56: blx 0x00077a04
  00189e5a: ldr r0,[r6,#0x0]
  00189e5c: movs r1,#0x1
  00189e5e: add sp,#0x8
  00189e60: pop.w r8
  00189e64: pop.w {r4,r5,r6,r7,lr}
  00189e68: b.w 0x001ac7f8
  00189e6c: blx 0x000723d0
  00189e70: cbz r0,0x00189ebc
  00189e72: ldr r0,[r6,#0x0]
  00189e74: blx 0x00071770
  00189e78: ldr r2,[r6,#0x0]
  00189e7a: cmp r0,#0x50
  00189e7c: itte eq
  00189e7e: mov.eq r1,#0x64
  00189e80: str.w.eq r1,[r2,#0x84]
  00189e84: ldr.ne.w r1,[r2,#0x84]
  00189e88: ldr r0,[0x00189fa0]
  00189e8a: add r0,pc
  00189e8c: ldr r0,[r0,#0x0]
  00189e8e: ldr r0,[r0,#0x0]
  00189e90: blx 0x00071c44
  00189e94: ldr r1,[0x00189fa4]
  00189e96: add r1,pc
  00189e98: ldr r1,[r1,#0x0]
  00189e9a: str r0,[r1,#0x0]
  00189e9c: movs r0,#0x1
  00189e9e: strb.w r0,[r4,#0xdd]
  00189ea2: mov r0,r4
  00189ea4: blx 0x00078280
  00189ea8: movs r5,#0x0
  00189eaa: mov r0,r4
  00189eac: strb.w r5,[r4,#0x5d]
  00189eb0: blx 0x0007828c
  00189eb4: ldr r0,[r4,#0x74]
  00189eb6: strb.w r5,[r4,#0xd6]
  00189eba: b 0x00189f7c
  00189ebc: ldr r0,[r4,#0x58]
  00189ebe: blx 0x00077b24
  00189ec2: cbz r0,0x00189f06
  00189ec4: ldr r0,[0x00189fa8]
  00189ec6: mov.w r1,#0x264
  00189eca: ldr.w r5,[r4,#0x94]
  00189ece: add r0,pc
  00189ed0: ldr r0,[r0,#0x0]
  00189ed2: ldr r0,[r0,#0x0]
  00189ed4: blx 0x00072f70
  00189ed8: mov r1,r0
  00189eda: mov r0,r5
  00189edc: blx 0x00074794
  00189ee0: movs r0,#0x1
  00189ee2: movs r5,#0x0
  00189ee4: strb.w r0,[r4,#0x5d]
  00189ee8: strb.w r0,[r4,#0xce]
  00189eec: strb.w r0,[r4,#0x111]
  00189ef0: ldr r0,[r4,#0x74]
  00189ef2: strb.w r5,[r4,#0xd6]
  00189ef6: blx 0x00077a04
  00189efa: mov r0,r4
  00189efc: blx 0x0007822c
  00189f00: strb.w r5,[r4,#0x17c]
  00189f04: b 0x00189d70
  00189f06: ldr.w r0,[r4,#0x90]
  00189f0a: cbnz r0,0x00189f2c
  00189f0c: mov.w r0,#0x1e8
  00189f10: blx 0x0006eb24
  00189f14: mov r5,r0
  00189f16: mov.w r0,#0xffffffff
  00189f1a: movs r1,#0x0
  00189f1c: str r0,[sp,#0x0]
  00189f1e: mov r0,r5
  00189f20: movs r2,#0x0
  00189f22: movs r3,#0x0
  00189f24: blx 0x00075424
  00189f28: str.w r5,[r4,#0x90]
  00189f2c: ldr r0,[r4,#0x8]
  00189f2e: blx 0x0007258c
  00189f32: ldr r1,[0x00189f98]
  00189f34: movs r2,#0x0
  00189f36: mov.w r8,#0x0
  00189f3a: blx 0x000707f8
  00189f3e: ldr.w r0,[r4,#0x90]
  00189f42: blx 0x00078298
  00189f46: ldr.w r0,[r4,#0x90]
  00189f4a: movs r5,#0x1
  00189f4c: movs r1,#0x1
  00189f4e: movs r2,#0x1
  00189f50: strb.w r5,[r4,#0xdd]
  00189f54: blx 0x00075430
  00189f58: ldr r0,[r6,#0x0]
  00189f5a: blx 0x000723d0
  00189f5e: cbnz r0,0x00189f68
  00189f60: ldr.w r0,[r4,#0x90]
  00189f64: blx 0x000782a4
  00189f68: mov r0,r4
  00189f6a: strb.w r5,[r4,#0x5d]
  00189f6e: strb.w r5,[r4,#0xc7]
  00189f72: blx 0x0007822c
  00189f76: ldr r0,[r4,#0x74]
  00189f78: strb.w r8,[r4,#0xd6]
  00189f7c: add sp,#0x8
  00189f7e: pop.w r8
  00189f82: pop.w {r4,r5,r6,r7,lr}
  00189f86: b.w 0x001ac808
  001ac7a8: bx pc
  001ac7ac: ldr r12,[0x1ac7b4]
  001ac7b0: add pc,r12,pc
  001ac7f8: bx pc
  001ac808: bx pc
```
