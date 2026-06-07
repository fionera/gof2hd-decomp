# LevelScript::setAutoPilotToProgrammedStation
elf 0x137b68 body 206
Sig: undefined __stdcall setAutoPilotToProgrammedStation(void)

## decompile
```c

/* LevelScript::setAutoPilotToProgrammedStation() */

void LevelScript::setAutoPilotToProgrammedStation(void)

{
  Station *this;
  int iVar1;
  SolarSystem *pSVar2;
  undefined4 uVar3;
  StarSystem *pSVar4;
  int iVar5;
  undefined4 uVar6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_00147c34 + 0x147b78);
  if (*piVar7 != 0) {
    this = (Station *)Status::getStation();
    iVar1 = Station::equals(this,(Station *)*piVar7);
    if (iVar1 == 0) {
      pSVar2 = (SolarSystem *)Status::getSystem();
      iVar1 = SolarSystem::stationIsInSystem(pSVar2,(Station *)*piVar7);
      if (iVar1 == 0) {
        Status::getSystem();
        iVar1 = SolarSystem::currentOrbitHasWarpGate();
        if (iVar1 == 0) {
          pSVar2 = (SolarSystem *)Status::getSystem();
          iVar1 = SolarSystem::getWarpGateEnumIndex(pSVar2);
          if (iVar1 < 0) {
            return;
          }
          uVar3 = Level::getPlayer();
          pSVar4 = (StarSystem *)Level::getStarSystem();
          iVar5 = StarSystem::getPlanetTargets(pSVar4);
          uVar6 = *(undefined4 *)(*(int *)(iVar5 + 4) + iVar1 * 4);
        }
        else {
          uVar3 = Level::getPlayer();
          iVar1 = Level::getLandmarks();
          uVar6 = *(undefined4 *)(*(int *)(iVar1 + 4) + 4);
        }
      }
      else {
        uVar3 = Level::getPlayer();
        pSVar4 = (StarSystem *)Level::getStarSystem();
        iVar1 = StarSystem::getPlanetTargets(pSVar4);
        pSVar2 = (SolarSystem *)Status::getSystem();
        iVar5 = Station::getIndex((Station *)*piVar7);
        iVar5 = SolarSystem::getStationEnumIndex(pSVar2,iVar5);
        uVar6 = *(undefined4 *)(*(int *)(iVar1 + 4) + iVar5 * 4);
      }
      (*(code *)(&UNK_001abc28 + DAT_001abc24))(uVar3,uVar6);
      return;
    }
    *piVar7 = 0;
  }
  return;
}

```

## target disasm
```
  00147b68: push {r4,r5,r6,r7,lr}
  00147b6a: add r7,sp,#0xc
  00147b6c: push.w r8
  00147b70: mov r4,r0
  00147b72: ldr r0,[0x00147c34]
  00147b74: add r0,pc
  00147b76: ldr r5,[r0,#0x0]
  00147b78: ldr r0,[r5,#0x0]
  00147b7a: cbz r0,0x00147b94
  00147b7c: ldr r0,[0x00147c38]
  00147b7e: add r0,pc
  00147b80: ldr r6,[r0,#0x0]
  00147b82: ldr r0,[r6,#0x0]
  00147b84: blx 0x00071c14
  00147b88: ldr r1,[r5,#0x0]
  00147b8a: blx 0x00073678
  00147b8e: cbz r0,0x00147b9a
  00147b90: movs r0,#0x0
  00147b92: str r0,[r5,#0x0]
  00147b94: pop.w r8
  00147b98: pop {r4,r5,r6,r7,pc}
  00147b9a: ldr r0,[r6,#0x0]
  00147b9c: blx 0x00071a10
  00147ba0: ldr r1,[r5,#0x0]
  00147ba2: blx 0x00076b58
  00147ba6: cbz r0,0x00147bdc
  00147ba8: ldr r0,[r4,#0x18]
  00147baa: blx 0x00072034
  00147bae: mov r8,r0
  00147bb0: ldr r0,[r4,#0x18]
  00147bb2: blx 0x000727c0
  00147bb6: blx 0x000767a4
  00147bba: mov r4,r0
  00147bbc: ldr r0,[r6,#0x0]
  00147bbe: blx 0x00071a10
  00147bc2: mov r6,r0
  00147bc4: ldr r0,[r5,#0x0]
  00147bc6: blx 0x00071824
  00147bca: mov r1,r0
  00147bcc: mov r0,r6
  00147bce: blx 0x00074644
  00147bd2: ldr r1,[r4,#0x4]
  00147bd4: ldr.w r1,[r1,r0,lsl #0x2]
  00147bd8: mov r0,r8
  00147bda: b 0x00147c28
  00147bdc: ldr r0,[r6,#0x0]
  00147bde: blx 0x00071a10
  00147be2: blx 0x00073c18
  00147be6: cbz r0,0x00147bfe
  00147be8: ldr r0,[r4,#0x18]
  00147bea: blx 0x00072034
  00147bee: mov r5,r0
  00147bf0: ldr r0,[r4,#0x18]
  00147bf2: blx 0x000725b0
  00147bf6: ldr r0,[r0,#0x4]
  00147bf8: ldr r1,[r0,#0x4]
  00147bfa: mov r0,r5
  00147bfc: b 0x00147c28
  00147bfe: ldr r0,[r6,#0x0]
  00147c00: blx 0x00071a10
  00147c04: blx 0x000767e0
  00147c08: mov r5,r0
  00147c0a: cmp r0,#0x0
  00147c0c: blt 0x00147b94
  00147c0e: ldr r0,[r4,#0x18]
  00147c10: blx 0x00072034
  00147c14: mov r6,r0
  00147c16: ldr r0,[r4,#0x18]
  00147c18: blx 0x000727c0
  00147c1c: blx 0x000767a4
  00147c20: ldr r0,[r0,#0x4]
  00147c22: ldr.w r1,[r0,r5,lsl #0x2]
  00147c26: mov r0,r6
  00147c28: pop.w r8
  00147c2c: pop.w {r4,r5,r6,r7,lr}
  00147c30: b.w 0x001abc18
  001abc18: bx pc
```
