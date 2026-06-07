# PlayerEgo::setLevel
elf 0x9b0b8 body 146
Sig: undefined __thiscall setLevel(PlayerEgo * this, Level * param_1)

## decompile
```c

/* PlayerEgo::setLevel(Level*) */

void __thiscall PlayerEgo::setLevel(PlayerEgo *this,Level *param_1)

{
  undefined4 uVar1;
  int *piVar2;
  SingleLevel *pSVar3;
  int iVar4;
  undefined4 uVar5;
  
  this->m_pLevel = (SingleLevel *)param_1;
  uVar5 = *(undefined4 *)(param_1 + 0x74);
  uVar1 = AEGeometry::getMatrix();
  piVar2 = (int *)ParticleSystemManager::addSystem(uVar5,uVar1,9,0);
  pSVar3 = this->m_pLevel;
  this[0x15].field_8 = piVar2;
  ParticleSystemManager::enableSystemEmit((int)pSVar3->field_74,SUB41(piVar2,0));
  iVar4 = Status::getCurrentCampaignMission();
  if (iVar4 < 2) {
    iVar4 = this->m_pLevel->field_78;
    uVar1 = AEGeometry::getMatrix();
    pSVar3 = (SingleLevel *)ParticleSystemManager::addSystem(iVar4,uVar1,0xf,0);
    this[0x15].m_pLevel = pSVar3;
    ParticleSystemManager::enableSystemEmit(this->m_pLevel->field_78,SUB41(pSVar3,0));
    piVar2 = this->m_pLevel->field_84;
    uVar1 = AEGeometry::getMatrix();
    piVar2 = (int *)ParticleSystemManager::addSystem(piVar2,uVar1,0x2a,0);
    pSVar3 = this->m_pLevel;
    this[0x15].field_10 = piVar2;
    (*(code *)(DAT_001aba84 + 0x1aba88))(pSVar3->field_84,piVar2,0);
    return;
  }
  return;
}

```

## target disasm
```
  000ab0b8: push {r4,r5,r7,lr}
  000ab0ba: add r7,sp,#0x8
  000ab0bc: mov r4,r0
  000ab0be: ldr r0,[r0,#0x8]
  000ab0c0: str r1,[r4,#0xc]
  000ab0c2: ldr r5,[r1,#0x74]
  000ab0c4: blx 0x000721cc
  000ab0c8: mov r1,r0
  000ab0ca: mov r0,r5
  000ab0cc: movs r2,#0x9
  000ab0ce: movs r3,#0x0
  000ab0d0: blx 0x000724d8
  000ab0d4: mov r1,r0
  000ab0d6: ldr r0,[r4,#0xc]
  000ab0d8: str.w r1,[r4,#0x2fc]
  000ab0dc: movs r2,#0x0
  000ab0de: ldr r0,[r0,#0x74]
  000ab0e0: blx 0x000723c4
  000ab0e4: ldr r0,[0x000ab14c]
  000ab0e6: add r0,pc
  000ab0e8: ldr r0,[r0,#0x0]
  000ab0ea: ldr r0,[r0,#0x0]
  000ab0ec: blx 0x00071770
  000ab0f0: cmp r0,#0x1
  000ab0f2: it gt
  000ab0f4: pop.gt {r4,r5,r7,pc}
  000ab0f6: ldrd r0,r1,[r4,#0x8]
  000ab0fa: ldr r5,[r1,#0x78]
  000ab0fc: blx 0x000721cc
  000ab100: mov r1,r0
  000ab102: mov r0,r5
  000ab104: movs r2,#0xf
  000ab106: movs r3,#0x0
  000ab108: blx 0x000724d8
  000ab10c: mov r1,r0
  000ab10e: ldr r0,[r4,#0xc]
  000ab110: str.w r1,[r4,#0x300]
  000ab114: movs r2,#0x0
  000ab116: ldr r0,[r0,#0x78]
  000ab118: blx 0x000723c4
  000ab11c: ldrd r0,r1,[r4,#0x8]
  000ab120: ldr.w r5,[r1,#0x84]
  000ab124: blx 0x000721cc
  000ab128: mov r1,r0
  000ab12a: mov r0,r5
  000ab12c: movs r2,#0x2a
  000ab12e: movs r3,#0x0
  000ab130: blx 0x000724d8
  000ab134: mov r1,r0
  000ab136: ldr r0,[r4,#0xc]
  000ab138: str.w r1,[r4,#0x304]
  000ab13c: movs r2,#0x0
  000ab13e: ldr.w r0,[r0,#0x84]
  000ab142: pop.w {r4,r5,r7,lr}
  000ab146: b.w 0x001aba78
```
