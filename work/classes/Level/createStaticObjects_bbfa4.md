# Level::createStaticObjects
elf 0xbbfa4 body 492
Sig: undefined __thiscall createStaticObjects(Level * this)

## decompile
```c

/* Level::createStaticObjects() */

void __thiscall Level::createStaticObjects(Level *this)

{
  int iVar1;
  Station *pSVar2;
  PlayerFixedObject *pPVar3;
  String *pSVar4;
  void *pvVar5;
  Array<KIPlayer*> *pAVar6;
  int *piVar7;
  Vector *pVVar8;
  String aSStack_4c [12];
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  Vector aVStack_34 [12];
  int local_28;
  
  piVar7 = *(int **)(DAT_000cc1e4 + 0xcbfb8);
  local_28 = *piVar7;
  iVar1 = Status::inAlienOrbit();
  if (iVar1 == 0) {
    pSVar2 = (Station *)Status::getStation();
    iVar1 = Station::getIndex(pSVar2);
    if ((iVar1 == 0x70) && (iVar1 = Status::getCurrentCampaignMission(), 0x7f < iVar1)) {
      iVar1 = Status::getCurrentCampaignMission();
      if (iVar1 < 0x83) {
        iVar1 = 0x493e;
      }
      else {
        iVar1 = Status::getCurrentCampaignMission();
        if (iVar1 < 0x87) {
          iVar1 = 0x4941;
        }
        else {
          iVar1 = Status::getCurrentCampaignMission();
          if (iVar1 < 0x8a) {
            iVar1 = 0x4944;
          }
          else {
            iVar1 = Status::getCurrentCampaignMission();
            if (iVar1 < 0x8e) {
              iVar1 = 0x4947;
            }
            else {
              iVar1 = Status::getCurrentCampaignMission();
              if (0x91 < iVar1) goto LAB_000cc0e2;
              iVar1 = 0x494a;
            }
          }
        }
      }
      pPVar3 = (PlayerFixedObject *)createStaticObject(this,(Waypoint *)0x0,iVar1,false);
      (**(code **)(*(int *)pPVar3 + 0x48))(pPVar3,DAT_000cc1dc,0,DAT_000cc1e0);
      PlayerFixedObject::setMoving(pPVar3,false);
      pVVar8 = *(Vector **)(pPVar3 + 8);
      pPVar3[0x70] = (PlayerFixedObject)0x0;
      StarSystem::getLightDirection();
      local_40 = 0;
      uStack_3c = 0x3f800000;
      local_38 = 0;
      AEGeometry::setDirection(pVVar8,aVStack_34);
      pSVar4 = (String *)GameText::getText(**(int **)(DAT_000cc1ec + 0xcc098));
      AbyssEngine::String::operator=((String *)(pPVar3 + 0x18),pSVar4);
      Player::setAlwaysFriend(*(Player **)(pPVar3 + 4),true);
      pAVar6 = *(Array<KIPlayer*> **)(this + 0xf8);
      if (pAVar6 == (Array<KIPlayer*> *)0x0) {
        pAVar6 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar6);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar6;
      }
      ArrayAdd<KIPlayer*>((KIPlayer *)pPVar3,(Array *)pAVar6);
      if (*(Array<int> **)(pPVar3 + 0x50) != (Array<int> *)0x0) {
        pvVar5 = (void *)Array<int>::~Array(*(Array<int> **)(pPVar3 + 0x50));
        operator_delete(pvVar5);
      }
      *(undefined4 *)(pPVar3 + 0x50) = 0;
    }
  }
LAB_000cc0e2:
  iVar1 = Status::getCurrentCampaignMission();
  if (((0x54 < iVar1) && (iVar1 = Status::getCurrentCampaignMission(), iVar1 != 0x87)) &&
     (iVar1 = Status::inAlienOrbit(), iVar1 == 0)) {
    pSVar2 = (Station *)Status::getStation();
    iVar1 = Station::getIndex(pSVar2);
    if (iVar1 == 0x67) {
      pPVar3 = (PlayerFixedObject *)createStaticObject(this,(Waypoint *)0x0,0x4a88,false);
      (**(code **)(*(int *)pPVar3 + 0x48))(pPVar3,0,0,0);
      PlayerFixedObject::setMoving(pPVar3,false);
      iVar1 = DAT_000cc1f0;
      pPVar3[0x70] = (PlayerFixedObject)0x1;
      pSVar4 = (String *)GameText::getText(**(int **)(iVar1 + 0xcc144));
      AbyssEngine::String::String(aSStack_4c,pSVar4,false);
      PlayerFixedObject::setName(pPVar3,aSStack_4c);
      AbyssEngine::String::~String(aSStack_4c);
      PlayerFixedObject::setDockingType(pPVar3,1);
      if (*(Array<int> **)(pPVar3 + 0x50) != (Array<int> *)0x0) {
        pvVar5 = (void *)Array<int>::~Array(*(Array<int> **)(pPVar3 + 0x50));
        operator_delete(pvVar5);
      }
      *(undefined4 *)(pPVar3 + 0x50) = 0;
      Player::setAlwaysFriend(*(Player **)(pPVar3 + 4),true);
      pAVar6 = *(Array<KIPlayer*> **)(this + 0xf8);
      if (pAVar6 == (Array<KIPlayer*> *)0x0) {
        pAVar6 = operator_new(0xc);
        Array<KIPlayer*>::Array(pAVar6);
        *(Array<KIPlayer*> **)(this + 0xf8) = pAVar6;
      }
      ArrayAdd<KIPlayer*>((KIPlayer *)pPVar3,(Array *)pAVar6);
    }
  }
  if (*piVar7 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```
## target disasm
```
  000cbfa4: push {r4,r5,r6,r7,lr}
  000cbfa6: add r7,sp,#0xc
  000cbfa8: push {r8,r9,r10,r11}
  000cbfac: sub sp,#0x2c
  000cbfae: mov r4,r0
  000cbfb0: ldr r0,[0x000cc1e4]
  000cbfb2: ldr r1,[0x000cc1e8]
  000cbfb4: add r0,pc
  000cbfb6: add r1,pc
  000cbfb8: ldr.w r8,[r0,#0x0]
  000cbfbc: ldr r6,[r1,#0x0]
  000cbfbe: ldr.w r1,[r8,#0x0]
  000cbfc2: ldr r0,[r6,#0x0]
  000cbfc4: str r1,[sp,#0x28]
  000cbfc6: blx 0x000723d0
  000cbfca: cmp r0,#0x0
  000cbfcc: bne.w 0x000cc0e2
  000cbfd0: ldr r0,[r6,#0x0]
  000cbfd2: blx 0x00071c14
  000cbfd6: blx 0x00071824
  000cbfda: cmp r0,#0x70
  000cbfdc: bne.w 0x000cc0e2
  000cbfe0: ldr r0,[r6,#0x0]
  000cbfe2: blx 0x00071770
  000cbfe6: cmp r0,#0x80
  000cbfe8: blt 0x000cc0e2
  000cbfea: ldr r0,[r6,#0x0]
  000cbfec: blx 0x00071770
  000cbff0: cmp r0,#0x83
  000cbff2: bge 0x000cbffa
  000cbff4: movw r2,#0x493e
  000cbff8: b 0x000cc038
  000cbffa: ldr r0,[r6,#0x0]
  000cbffc: blx 0x00071770
  000cc000: cmp r0,#0x87
  000cc002: bge 0x000cc00a
  000cc004: movw r2,#0x4941
  000cc008: b 0x000cc038
  000cc00a: ldr r0,[r6,#0x0]
  000cc038: mov r0,r4
  000cc03a: movs r1,#0x0
  000cc03c: movs r3,#0x0
  000cc03e: mov.w r11,#0x0
  000cc042: blx 0x00073f6c
  000cc046: mov r5,r0
  000cc048: ldr r0,[r0,#0x0]
  000cc04a: mov r9,r4
  000cc04c: ldr r1,[0x000cc1dc]
  000cc04e: ldr r3,[0x000cc1e0]
  000cc050: movs r2,#0x0
  000cc052: ldr r4,[r0,#0x48]
  000cc054: mov r0,r5
  000cc056: blx r4
  000cc058: mov r0,r5
  000cc05a: movs r1,#0x0
  000cc05c: mov r4,r9
  000cc05e: blx 0x00073f54
  000cc062: ldr.w r10,[r5,#0x8]
  000cc066: strb.w r11,[r5,#0x70]
  000cc06a: ldr.w r1,[r9,#0xec]
  000cc06e: add.w r9,sp,#0x1c
  000cc072: mov r0,r9
  000cc074: blx 0x000727cc
  000cc078: mov.w r0,#0x3f800000
  000cc07c: add r2,sp,#0x10
  000cc07e: strd r11,r0,[sp,#0x10]
  000cc082: mov r0,r10
  000cc084: mov r1,r9
  000cc086: str.w r11,[sp,#0x18]
  000cc08a: blx 0x000726ac
  000cc08e: ldr r0,[0x000cc1ec]
  000cc090: movw r1,#0xc87
  000cc094: add r0,pc
  000cc096: ldr r0,[r0,#0x0]
  000cc098: ldr r0,[r0,#0x0]
  000cc09a: blx 0x00072f70
  000cc09e: mov r1,r0
  000cc0a0: add.w r0,r5,#0x18
  000cc0a4: blx 0x0006f2b0
  000cc0a8: ldr r0,[r5,#0x4]
  000cc0aa: movs r1,#0x1
  000cc0ac: blx 0x00073fb4
  000cc0b0: ldr.w r1,[r4,#0xf8]
  000cc0b4: cbz r1,0x000cc0ba
  000cc0b6: mov r0,r5
  000cc0b8: b 0x000cc0ce
  000cc0ba: movs r0,#0xc
  000cc0bc: blx 0x0006eb24
  000cc0c0: mov r9,r0
  000cc0c2: blx 0x00073d08
  000cc0c6: mov r0,r5
  000cc0c8: mov r1,r9
  000cc0ca: str.w r9,[r4,#0xf8]
  000cc0ce: blx 0x000740f8
  000cc0d2: ldr r0,[r5,#0x50]
  000cc0d4: cbz r0,0x000cc0de
  000cc0d6: blx 0x00070204
  000cc0da: blx 0x0006eb48
  000cc0de: movs r0,#0x0
  000cc0e0: str r0,[r5,#0x50]
  000cc0e2: ldr r0,[r6,#0x0]
  000cc0e4: blx 0x00071770
  000cc0e8: cmp r0,#0x55
  000cc0ea: blt 0x000cc1a6
  000cc0ec: ldr r0,[r6,#0x0]
  000cc0ee: blx 0x00071770
  000cc0f2: cmp r0,#0x87
  000cc0f4: beq 0x000cc1a6
  000cc0f6: ldr r0,[r6,#0x0]
  000cc0f8: blx 0x000723d0
  000cc0fc: cmp r0,#0x0
  000cc0fe: bne 0x000cc1a6
  000cc100: ldr r0,[r6,#0x0]
  000cc102: blx 0x00071c14
  000cc106: blx 0x00071824
  000cc10a: cmp r0,#0x67
  000cc10c: bne 0x000cc1a6
  000cc10e: mov r0,r4
  000cc110: movs r1,#0x0
  000cc112: movw r2,#0x4a88
  000cc116: movs r3,#0x0
  000cc118: blx 0x00073f6c
  000cc11c: mov r5,r0
  000cc11e: ldr r0,[r0,#0x0]
  000cc120: movs r1,#0x0
  000cc122: movs r2,#0x0
  000cc124: movs r3,#0x0
  000cc126: ldr r6,[r0,#0x48]
  000cc128: mov r0,r5
  000cc12a: blx r6
  000cc12c: mov r0,r5
  000cc12e: movs r1,#0x0
  000cc130: blx 0x00073f54
  000cc134: ldr r0,[0x000cc1f0]
  000cc136: movs r1,#0x1
  000cc138: strb.w r1,[r5,#0x70]
  000cc13c: movw r1,#0xc8a
  000cc140: add r0,pc
  000cc142: ldr r0,[r0,#0x0]
  000cc144: ldr r0,[r0,#0x0]
  000cc146: blx 0x00072f70
  000cc14a: add r6,sp,#0x4
  000cc14c: mov r1,r0
  000cc14e: movs r2,#0x0
  000cc150: mov r0,r6
  000cc152: blx 0x0006f028
  000cc156: mov r0,r5
  000cc158: mov r1,r6
  000cc15a: blx 0x00073fa8
  000cc15e: add r0,sp,#0x4
  000cc160: blx 0x0006ee30
  000cc164: mov r0,r5
  000cc166: movs r1,#0x1
  000cc168: blx 0x00072748
  000cc16c: ldr r0,[r5,#0x50]
  000cc16e: cbz r0,0x000cc178
  000cc170: blx 0x00070204
  000cc174: blx 0x0006eb48
  000cc178: ldr r0,[r5,#0x4]
  000cc17a: movs r1,#0x0
  000cc17c: str r1,[r5,#0x50]
  000cc17e: movs r1,#0x1
  000cc180: blx 0x00073fb4
  000cc184: ldr.w r1,[r4,#0xf8]
  000cc188: cbz r1,0x000cc18e
  000cc18a: mov r0,r5
  000cc18c: b 0x000cc1a2
  000cc18e: movs r0,#0xc
  000cc190: blx 0x0006eb24
  000cc194: mov r6,r0
  000cc196: blx 0x00073d08
  000cc19a: mov r0,r5
  000cc19c: mov r1,r6
  000cc19e: str.w r6,[r4,#0xf8]
  000cc1a2: blx 0x000740f8
  000cc1a6: ldr r0,[sp,#0x28]
  000cc1a8: ldr.w r1,[r8,#0x0]
  000cc1ac: subs r0,r1,r0
  000cc1ae: ittt eq
  000cc1b0: add.eq sp,#0x2c
  000cc1b2: pop.eq.w {r8,r9,r10,r11}
  000cc1b6: pop.eq {r4,r5,r6,r7,pc}
  000cc1b8: blx 0x0006e824
```
