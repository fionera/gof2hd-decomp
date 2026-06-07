# MGame::successCheck
elf 0x17ff58 body 1334
Sig: undefined __thiscall successCheck(MGame * this)

## decompile
```c

/* MGame::successCheck() */

void __thiscall MGame::successCheck(MGame *this)

{
  int iVar1;
  int iVar2;
  DialogueWindow *pDVar3;
  Mission *pMVar4;
  Mission *this_00;
  Agent *pAVar5;
  String *pSVar6;
  undefined4 uVar7;
  void *pvVar8;
  uint *puVar9;
  StarSystem *this_01;
  Route *this_02;
  Route *pRVar10;
  Station *this_03;
  Level *pLVar11;
  int *piVar12;
  int *piVar13;
  void *extraout_r1;
  int extraout_r2;
  int extraout_r2_00;
  uint in_r3;
  uint extraout_r3;
  void *extraout_r3_00;
  void *extraout_r3_01;
  void *__arg;
  undefined4 uVar14;
  uint uVar15;
  code *pcVar16;
  KIPlayer *this_04;
  PlayerFixedObject *this_05;
  int *piVar17;
  undefined4 *puVar18;
  float fVar19;
  undefined8 uVar20;
  String aSStack_6c [12];
  String aSStack_60 [12];
  undefined1 auStack_54 [12];
  String aSStack_48 [12];
  float local_3c;
  float local_38;
  float local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  
  piVar17 = *(int **)(DAT_001902f8 + 0x18ff6e);
  local_24 = *piVar17;
  if ((this->field_4C < (int)(uint)((uint)this->field_48 < 0x1389)) ||
     ((char)this[1].field_14 != '\0')) {
    Status::getCampaignMission();
    iVar1 = Mission::getType();
    in_r3 = extraout_r3;
    if (iVar1 != 0xaa) goto LAB_00190270;
  }
  puVar18 = *(undefined4 **)(DAT_00190300 + 0x18ffa8);
  iVar1 = Status::missionCompleted(SUB41(*puVar18,0),false,(ulonglong)in_r3 << 0x20);
  iVar2 = Level::checkObjective((int)this->field_78);
  if ((iVar1 == 0) && (iVar2 == 0)) goto LAB_00190270;
  Status::getMission();
  iVar2 = Mission::getType();
  if (iVar2 != 5) {
    Status::getMission();
    iVar2 = Mission::getType();
    if (iVar2 != 3) {
      pMVar4 = (Mission *)Status::getMission();
      iVar2 = Mission::isCampaignMission(pMVar4);
      if (iVar2 == 0) {
        Status::incMissionCount((Status *)*puVar18);
      }
      pMVar4 = (Mission *)Status::getMission();
      iVar2 = Mission::isCampaignMission(pMVar4);
      if (iVar2 != 0) {
        pMVar4 = (Mission *)Status::getMission();
        iVar2 = Mission::isCampaignMission(pMVar4);
        if (iVar2 != 0) {
          iVar2 = Status::getCurrentCampaignMission();
          iVar2 = DialogueWindow::hasSuccessDialogue(iVar2);
          if (iVar2 != 0) goto LAB_0019020c;
        }
        iVar1 = Status::getCurrentCampaignMission();
        if (0x2d < iVar1) {
          pMVar4 = (Mission *)Status::getMission();
          iVar1 = Mission::isCampaignMission(pMVar4);
          if (iVar1 != 0) {
            iVar1 = Status::getCurrentCampaignMission();
            iVar1 = DialogueWindow::hasSuccessDialogue(iVar1);
            if (iVar1 == 0) {
              Status::nextCampaignMission(SUB41(*puVar18,0));
              Level::removeObjectives((Level *)this->field_78);
              Status::setMission((Mission *)*puVar18);
            }
          }
        }
        goto LAB_00190270;
      }
LAB_0019020c:
      if ((DialogueWindow *)this->field_8C == (DialogueWindow *)0x0) {
        pDVar3 = operator_new(0x74);
        DialogueWindow::DialogueWindow(pDVar3);
        pLVar11 = (Level *)this->field_78;
        this->field_8C = (int)pDVar3;
        if (pLVar11 != (Level *)0x0) goto LAB_0019029e;
      }
      else {
        iVar2 = DialogueWindow::hasLevel((DialogueWindow *)this->field_8C);
        if ((iVar2 == 0) && (pLVar11 = (Level *)this->field_78, pLVar11 != (Level *)0x0)) {
          pDVar3 = (DialogueWindow *)this->field_8C;
LAB_0019029e:
          DialogueWindow::setLevel(pDVar3,pLVar11);
        }
      }
      pMVar4 = (Mission *)this->field_8C;
      if (iVar1 == 0) {
        iVar1 = Status::getMission();
      }
      DialogueWindow::set(pMVar4,iVar1,1);
      *(undefined2 *)((int)&this->field_5C + 1) = 0x101;
      pauseSounds();
      pMVar4 = (Mission *)Status::getMission();
      iVar1 = Mission::isCampaignMission(pMVar4);
      if ((iVar1 == 0) || (iVar1 = Status::getCurrentCampaignMission(), iVar1 != 0x26)) {
        pMVar4 = (Mission *)Status::getMission();
        iVar1 = Mission::isCampaignMission(pMVar4);
        if ((iVar1 == 0) || (iVar1 = Status::getCurrentCampaignMission(), iVar1 != 0x38)) {
          pMVar4 = (Mission *)Status::getMission();
          iVar1 = Mission::isCampaignMission(pMVar4);
          if ((iVar1 == 0) || (iVar1 = Status::getCurrentCampaignMission(), iVar1 != 0x3f)) {
            pMVar4 = (Mission *)Status::getMission();
            iVar1 = Mission::isCampaignMission(pMVar4);
            if ((iVar1 == 0) || (iVar1 = Status::getCurrentCampaignMission(), iVar1 != 0x49)) {
              this_03 = (Station *)Status::getStation();
              iVar1 = Station::getIndex(this_03);
              if ((iVar1 == 0x70) && (iVar1 = Status::getCurrentCampaignMission(), iVar1 == 0x8f)) {
                **(undefined1 **)(DAT_0019050c + 0x1904aa) = 1;
              }
            }
            else {
              puVar9 = (uint *)Level::getEnemies();
              for (uVar15 = 0; uVar15 < *puVar9; uVar15 = uVar15 + 1) {
                this_05 = *(PlayerFixedObject **)(puVar9[1] + uVar15 * 4);
                if ((this_05[0x40] != (PlayerFixedObject)0x0) &&
                   (iVar1 = KIPlayer::isDead(), iVar1 == 0)) {
                  Player::setHitpoints(*(int *)(this_05 + 4));
                  PlayerFixedObject::setMoving(this_05,true);
                }
              }
            }
          }
          else {
            puVar9 = (uint *)Level::getEnemies();
            for (uVar15 = 0; uVar15 < *puVar9; uVar15 = uVar15 + 1) {
              iVar1 = *(int *)(puVar9[1] + uVar15 * 4);
              if (*(int *)(iVar1 + 0x28) == 8) {
                Player::removeAllGuns(*(Player **)(iVar1 + 4));
              }
            }
          }
        }
        else {
          this_01 = (StarSystem *)Level::getStarSystem();
          StarSystem::getPlanets(this_01);
          fVar19 = (float)AEGeometry::getPosition();
          AbyssEngine::AEMath::operator*((Vector *)&local_3c,fVar19);
          local_30 = (int)local_3c;
          local_2c = (int)local_38;
          local_28 = (int)local_34;
          this_02 = operator_new(0x18);
          Route::Route(this_02,&local_30,3);
          uVar20 = Level::getEnemies();
          pvVar8 = (void *)((ulonglong)uVar20 >> 0x20);
          iVar1 = extraout_r2;
          __arg = extraout_r3_00;
          for (uVar15 = 0; uVar15 < *(uint *)uVar20; uVar15 = uVar15 + 1) {
            this_04 = *(KIPlayer **)(((uint *)uVar20)[1] + uVar15 * 4);
            if (*(int *)(this_04 + 0x28) == 1) {
              pRVar10 = (Route *)Route::clone((__fn *)this_02,pvVar8,iVar1,__arg);
              KIPlayer::setRoute(this_04,pRVar10);
              pvVar8 = extraout_r1;
              iVar1 = extraout_r2_00;
              __arg = extraout_r3_01;
            }
          }
          pvVar8 = (void *)Route::~Route(this_02);
          operator_delete(pvVar8);
        }
      }
      else {
        puVar9 = (uint *)Level::getEnemies();
        for (uVar15 = 0; uVar15 < *puVar9; uVar15 = uVar15 + 1) {
          iVar1 = *(int *)(puVar9[1] + uVar15 * 4);
          if ((*(char *)(iVar1 + 0x40) != '\0') && (iVar2 = KIPlayer::isDead(), iVar2 == 0)) {
            Player::setHitpoints(*(int *)(iVar1 + 4));
          }
        }
      }
      goto LAB_00190270;
    }
  }
  if ((DialogueWindow *)this->field_8C == (DialogueWindow *)0x0) {
    pDVar3 = operator_new(0x74);
    DialogueWindow::DialogueWindow(pDVar3);
    pLVar11 = (Level *)this->field_78;
    this->field_8C = (int)pDVar3;
    if (pLVar11 != (Level *)0x0) goto LAB_00190020;
  }
  else {
    iVar1 = DialogueWindow::hasLevel((DialogueWindow *)this->field_8C);
    if ((iVar1 == 0) && (pLVar11 = (Level *)this->field_78, pLVar11 != (Level *)0x0)) {
      pDVar3 = (DialogueWindow *)this->field_8C;
LAB_00190020:
      DialogueWindow::setLevel(pDVar3,pLVar11);
    }
  }
  pcVar16 = *(code **)(DAT_00190304 + 0x19002e);
  pMVar4 = (Mission *)(*pcVar16)(*puVar18);
  this_00 = (Mission *)(*pcVar16)(*puVar18);
  pAVar5 = (Agent *)Mission::getAgent(this_00);
  iVar1 = Agent::getStation(pAVar5);
  Mission::setTargetStation(pMVar4,iVar1);
  pMVar4 = (Mission *)this->field_8C;
  iVar1 = (*pcVar16)(*puVar18);
  DialogueWindow::set(pMVar4,iVar1,1);
  pMVar4 = (Mission *)(*pcVar16)(*puVar18);
  Mission::setType(pMVar4,0xb);
  PlayerEgo::setTurretMode(SUB41(this->m_pPlayerEgo,0));
  LevelScript::resetCamera((Level *)this->field_7C);
  PlayerEgo::setFreeLookMode(this->m_pPlayerEgo,false);
  TargetFollowCamera::enableFirstPersonCam((TargetFollowCamera *)this[1].field_2C,false);
  PlayerEgo::hideShipForFirstPersonCameraView(SUB41(this->m_pPlayerEgo,0));
  *(undefined1 *)((int)&this[1].field_48 + 1) = 1;
  iVar1 = (*pcVar16)(*puVar18);
  Mission::setStatusValue(iVar1);
  pMVar4 = (Mission *)(*pcVar16)(*puVar18);
  Mission::setWon(pMVar4,false);
  pSVar6 = (String *)GameText::getText(**(int **)(DAT_00190308 + 0x1900c2));
  AbyssEngine::String::String((String *)&local_3c,pSVar6,false);
  uVar14 = *puVar18;
  AbyssEngine::String::String(aSStack_48,(String *)&local_3c,false);
  Status::getMission();
  Mission::getTargetStationName();
  uVar7 = AbyssEngine::String::String(aSStack_60,(char *)(DAT_0019030c + 0x1900f8),false);
  Status::replaceHash(&local_30,uVar14,aSStack_48,auStack_54,uVar7);
  AbyssEngine::String::operator=((String *)&local_3c,(String *)&local_30);
  pcVar16 = *(code **)(DAT_00190310 + 0x19011a);
  (*pcVar16)(&local_30);
  (*pcVar16)(aSStack_60);
  (*pcVar16)(auStack_54);
  (*pcVar16)(aSStack_48);
  pMVar4 = (Mission *)Status::getMission();
  pAVar5 = (Agent *)Mission::getAgent(pMVar4);
  AbyssEngine::String::String(aSStack_6c,(String *)&local_3c,false);
  Agent::setMissionString(pAVar5,aSStack_6c);
  AbyssEngine::String::~String(aSStack_6c);
  Status::setMission((Mission *)*puVar18);
  PlayerEgo::setRoute((Route *)this->m_pPlayerEgo);
  iVar1 = PlayerEgo::goingToWaypoint();
  if (iVar1 != 0) {
    PlayerEgo::setAutoPilot((KIPlayer *)this->m_pPlayerEgo);
  }
  PlayerEgo::removeRoute();
  Level::setPlayerRoute((Route *)this->field_78);
  piVar12 = this->field_78;
  if ((Objective *)piVar12[10] != (Objective *)0x0) {
    pvVar8 = (void *)Objective::~Objective((Objective *)piVar12[10]);
    operator_delete(pvVar8);
    piVar12 = this->field_78;
  }
  piVar13 = piVar12 + 0xb;
  piVar12[10] = 0;
  if ((Objective *)*piVar13 != (Objective *)0x0) {
    pvVar8 = (void *)Objective::~Objective((Objective *)*piVar13);
    operator_delete(pvVar8);
    piVar13 = this->field_78 + 0xb;
  }
  *piVar13 = 0;
  *(undefined2 *)((int)&this->field_5C + 1) = 0x101;
  pauseSounds();
  AbyssEngine::String::~String((String *)&local_3c);
LAB_00190270:
  iVar1 = *piVar17 - local_24;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  0018ff58: push {r4,r5,r6,r7,lr}
  0018ff5a: add r7,sp,#0xc
  0018ff5c: push {r8,r9,r10}
  0018ff60: sub sp,#0x58
  0018ff62: mov r4,r0
  0018ff64: ldr r0,[0x001902f8]
  0018ff66: movw r2,#0x1389
  0018ff6a: add r0,pc
  0018ff6c: ldr.w r8,[r0,#0x0]
  0018ff70: ldr.w r0,[r8,#0x0]
  0018ff74: str r0,[sp,#0x54]
  0018ff76: ldrd r0,r1,[r4,#0x48]
  0018ff7a: subs r0,r0,r2
  0018ff7c: sbcs r0,r1,#0x0
  0018ff80: blt 0x0018ff88
  0018ff82: ldrb.w r0,[r4,#0xdc]
  0018ff86: cbz r0,0x0018ff9e
  0018ff88: ldr r0,[0x001902fc]
  0018ff8a: add r0,pc
  0018ff8c: ldr r0,[r0,#0x0]
  0018ff8e: ldr r0,[r0,#0x0]
  0018ff90: blx 0x0007285c
  0018ff94: blx 0x00072874
  0018ff98: cmp r0,#0xaa
  0018ff9a: bne.w 0x0019026e
  0018ff9e: ldr r0,[0x00190300]
  0018ffa0: movs r6,#0x0
  0018ffa2: ldr r1,[r4,#0x7c]
  0018ffa4: add r0,pc
  0018ffa6: ldrd r1,r2,[r1,#0x8]
  0018ffaa: ldr.w r9,[r0,#0x0]
  0018ffae: ldr.w r0,[r9,#0x0]
  0018ffb2: strd r1,r2,[sp,#0x0]
  0018ffb6: movs r1,#0x0
  0018ffb8: movs r2,#0x0
  0018ffba: blx 0x0007552c
  0018ffbe: mov r5,r0
  0018ffc0: ldrd r0,r1,[r4,#0x78]
  0018ffc4: ldr r1,[r1,#0x8]
  0018ffc6: blx 0x0007864c
  0018ffca: cbnz r5,0x0018ffd2
  0018ffcc: cmp r0,#0x0
  0018ffce: beq.w 0x00190270
  0018ffd2: ldr.w r0,[r9,#0x0]
  0018ffd6: blx 0x00072850
  0018ffda: blx 0x00072874
  0018ffde: cmp r0,#0x5
  0018ffe0: beq 0x0018fff4
  0018ffe2: ldr.w r0,[r9,#0x0]
  0018ffe6: blx 0x00072850
  0018ffea: blx 0x00072874
  0018ffee: cmp r0,#0x3
  0018fff0: bne.w 0x001901cc
  0018fff4: ldr.w r0,[r4,#0x8c]
  0018fff8: cbz r0,0x0019000a
  0018fffa: blx 0x00078658
  0018fffe: cbnz r0,0x00190024
  00190000: ldr r1,[r4,#0x78]
  00190002: cbz r1,0x00190024
  00190004: ldr.w r0,[r4,#0x8c]
  00190008: b 0x00190020
  0019000a: movs r0,#0x74
  0019000c: blx 0x0006eb24
  00190010: mov r5,r0
  00190012: blx 0x00075544
  00190016: ldr r1,[r4,#0x78]
  00190018: str.w r5,[r4,#0x8c]
  0019001c: cbz r1,0x00190024
  0019001e: mov r0,r5
  00190020: blx 0x00078664
  00190024: ldr r1,[0x00190304]
  00190026: ldr.w r0,[r9,#0x0]
  0019002a: add r1,pc
  0019002c: ldr r6,[r1,#0x0]
  0019002e: blx r6
  00190030: mov r5,r0
  00190032: ldr.w r0,[r9,#0x0]
  00190036: blx r6
  00190038: blx 0x00073438
  0019003c: blx 0x00071ac4
  00190040: mov r1,r0
  00190042: mov r0,r5
  00190044: blx 0x00078670
  00190048: ldr.w r0,[r9,#0x0]
  0019004c: ldr.w r5,[r4,#0x8c]
  00190050: blx r6
  00190052: mov r1,r0
  00190054: mov r0,r5
  00190056: movs r2,#0x1
  00190058: mov.w r3,#0xffffffff
  0019005c: mov.w r10,#0x1
  00190060: blx 0x00075550
  00190064: ldr.w r0,[r9,#0x0]
  00190068: blx r6
  0019006a: movs r1,#0xb
  0019006c: blx 0x00076c24
  00190070: ldr r0,[r4,#0x58]
  00190072: movs r1,#0x0
  00190074: blx 0x00072b08
  00190078: ldrd r1,r0,[r4,#0x78]
  0019007c: blx 0x0007246c
  00190080: ldr r0,[r4,#0x58]
  00190082: movs r1,#0x0
  00190084: blx 0x00076b94
  00190088: ldr.w r0,[r4,#0xf4]
  0019008c: movs r1,#0x0
  0019008e: blx 0x00076ba0
  00190092: ldr r0,[r4,#0x58]
  00190094: movs r1,#0x0
  00190096: blx 0x00076bac
  0019009a: strb.w r10,[r4,#0x111]
  0019009e: ldr.w r0,[r9,#0x0]
  001900a2: blx r6
  001900a4: mov.w r1,#0xffffffff
  001900a8: blx 0x00072898
  001900ac: ldr.w r0,[r9,#0x0]
  001900b0: blx r6
  001900b2: movs r1,#0x0
  001900b4: blx 0x00073984
  001900b8: ldr r0,[0x00190308]
  001900ba: movw r1,#0x323
  001900be: add r0,pc
  001900c0: ldr r0,[r0,#0x0]
  001900c2: ldr r0,[r0,#0x0]
  001900c4: blx 0x00072f70
  001900c8: add r6,sp,#0x3c
  001900ca: mov r1,r0
  001900cc: movs r2,#0x0
  001900ce: mov r0,r6
  001900d0: blx 0x0006f028
  001900d4: ldr.w r5,[r9,#0x0]
  001900d8: add r0,sp,#0x30
  001900da: mov r1,r6
  001900dc: movs r2,#0x0
  001900de: blx 0x0006f028
  001900e2: ldr.w r0,[r9,#0x0]
  001900e6: blx 0x00072850
  001900ea: mov r1,r0
  001900ec: add r0,sp,#0x24
  001900ee: blx 0x00074b54
  001900f2: ldr r1,[0x0019030c]
  001900f4: add r1,pc
  001900f6: add r0,sp,#0x18
  001900f8: movs r2,#0x0
  001900fa: blx 0x0006ee18
  001900fe: str r0,[sp,#0x0]
  00190100: add r0,sp,#0x48
  00190102: add r2,sp,#0x30
  00190104: add r3,sp,#0x24
  00190106: mov r1,r5
  00190108: blx 0x000739b4
  0019010c: add r0,sp,#0x3c
  0019010e: add r1,sp,#0x48
  00190110: blx 0x0006f2b0
  00190114: ldr r0,[0x00190310]
  00190116: add r0,pc
  00190118: ldr r5,[r0,#0x0]
  0019011a: add r0,sp,#0x48
  0019011c: blx r5
  0019011e: add r0,sp,#0x18
  00190120: blx r5
  00190122: add r0,sp,#0x24
  00190124: blx r5
  00190126: add r0,sp,#0x30
  00190128: blx r5
  0019012a: ldr.w r0,[r9,#0x0]
  0019012e: blx 0x00072850
  00190132: blx 0x00073438
  00190136: mov r5,r0
  00190138: add r0,sp,#0xc
  0019013a: add r1,sp,#0x3c
  0019013c: movs r2,#0x0
  0019013e: blx 0x0006f028
  00190142: add r1,sp,#0xc
  00190144: mov r0,r5
  00190146: blx 0x000749d4
  0019014a: add r0,sp,#0xc
  0019014c: blx 0x0006ee30
  00190150: ldr r0,[0x00190314]
  00190152: add r0,pc
  00190154: ldr r0,[r0,#0x0]
  00190156: ldr r1,[r0,#0x0]
  00190158: ldr.w r0,[r9,#0x0]
  0019015c: blx 0x00073c3c
  00190160: ldr r0,[r4,#0x58]
  00190162: movs r1,#0x0
  00190164: blx 0x00073cc0
  00190168: ldr r0,[r4,#0x58]
  0019016a: blx 0x000783e8
  0019016e: cbz r0,0x00190178
  00190170: ldr r0,[r4,#0x58]
  00190172: movs r1,#0x0
  00190174: blx 0x000728d4
  00190178: ldr r0,[r4,#0x58]
  0019017a: blx 0x000783f4
  0019017e: ldr r0,[r4,#0x78]
  00190180: movs r1,#0x0
  00190182: blx 0x00074170
  00190186: ldr r1,[r4,#0x78]
  00190188: ldr r0,[r1,#0x28]
  0019018a: cbz r0,0x00190196
  0019018c: blx 0x00073b34
  00190190: blx 0x0006eb48
  00190194: ldr r1,[r4,#0x78]
  00190196: ldr.w r0,[r1,#0x2c]!
  0019019a: movs r5,#0x0
  0019019c: cmp r0,#0x0
  0019019e: str.w r5,[r1,#-0x4]
  001901a2: beq 0x001901b2
  001901a4: blx 0x00073b34
  001901a8: blx 0x0006eb48
  001901ac: ldr r0,[r4,#0x78]
  001901ae: add.w r1,r0,#0x2c
  001901b2: movw r0,#0x101
  001901b6: str r5,[r1,#0x0]
  001901b8: strh.w r0,[r4,#0x5d]
  001901bc: mov r0,r4
  001901be: blx 0x0007822c
  001901c2: add r0,sp,#0x3c
  001901c4: blx 0x0006ee30
  001901c8: movs r6,#0x1
  001901ca: b 0x00190270
  001901cc: ldr.w r0,[r9,#0x0]
  001901d0: blx 0x00072850
  001901d4: blx 0x0007372c
  001901d8: cbnz r0,0x001901e2
  001901da: ldr.w r0,[r9,#0x0]
  001901de: blx 0x00075190
  001901e2: ldr.w r0,[r9,#0x0]
  001901e6: blx 0x00072850
  001901ea: blx 0x0007372c
  001901ee: cbz r0,0x0019020c
  001901f0: ldr.w r0,[r9,#0x0]
  001901f4: blx 0x00072850
  001901f8: blx 0x0007372c
  001901fc: cbz r0,0x00190226
  001901fe: ldr.w r0,[r9,#0x0]
  00190202: blx 0x00071770
  00190206: blx 0x0007867c
  0019020a: cbz r0,0x00190226
  0019020c: ldr.w r0,[r4,#0x8c]
  00190210: cbz r0,0x00190288
  00190212: blx 0x00078658
  00190216: cmp r0,#0x0
  00190218: bne 0x001902a2
  0019021a: ldr r1,[r4,#0x78]
  0019021c: cmp r1,#0x0
  0019021e: beq 0x001902a2
  00190220: ldr.w r0,[r4,#0x8c]
  00190224: b 0x0019029e
  00190226: ldr.w r0,[r9,#0x0]
  0019022a: blx 0x00071770
  0019022e: cmp r0,#0x2e
  00190230: blt 0x0019026e
  00190232: ldr.w r0,[r9,#0x0]
  00190236: blx 0x00072850
  0019023a: blx 0x0007372c
  0019023e: cbz r0,0x0019026e
  00190240: ldr.w r0,[r9,#0x0]
  00190244: blx 0x00071770
  00190248: blx 0x0007867c
  0019024c: cbnz r0,0x0019026e
  0019024e: ldr.w r0,[r9,#0x0]
  00190252: movs r1,#0x1
  00190254: blx 0x00075070
  00190258: ldr r0,[r4,#0x78]
  0019025a: blx 0x000783dc
  0019025e: ldr r0,[0x00190510]
  00190260: add r0,pc
  00190262: ldr r0,[r0,#0x0]
  00190264: ldr r1,[r0,#0x0]
  00190266: ldr.w r0,[r9,#0x0]
  0019026a: blx 0x00073c3c
  0019026e: movs r6,#0x0
  00190270: ldr r0,[sp,#0x54]
  00190272: ldr.w r1,[r8,#0x0]
  00190276: subs r0,r1,r0
  00190278: itttt eq
  0019027a: mov.eq r0,r6
  0019027c: add.eq sp,#0x58
  0019027e: pop.eq.w {r8,r9,r10}
  00190282: pop.eq {r4,r5,r6,r7,pc}
  00190284: blx 0x0006e824
  00190288: movs r0,#0x74
  0019028a: blx 0x0006eb24
  0019028e: mov r6,r0
  00190290: blx 0x00075544
  00190294: ldr r1,[r4,#0x78]
  00190296: str.w r6,[r4,#0x8c]
  0019029a: cbz r1,0x001902a2
  0019029c: mov r0,r6
  0019029e: blx 0x00078664
  001902a2: ldr.w r6,[r4,#0x8c]
  001902a6: cbnz r5,0x001902b2
  001902a8: ldr.w r0,[r9,#0x0]
  001902ac: blx 0x00072850
  001902b0: mov r5,r0
  001902b2: mov r0,r6
  001902b4: mov r1,r5
  001902b6: movs r2,#0x1
  001902b8: mov.w r3,#0xffffffff
  001902bc: blx 0x00075550
  001902c0: movw r0,#0x101
  001902c4: strh.w r0,[r4,#0x5d]
  001902c8: mov r0,r4
  001902ca: blx 0x0007822c
  001902ce: ldr.w r0,[r9,#0x0]
  001902d2: blx 0x00072850
  001902d6: blx 0x0007372c
  001902da: cbz r0,0x0019033e
  001902dc: ldr.w r0,[r9,#0x0]
  001902e0: blx 0x00071770
  001902e4: cmp r0,#0x26
  001902e6: bne 0x0019033e
  001902e8: ldr r0,[r4,#0x78]
  001902ea: blx 0x00071eb4
  001902ee: mov r4,r0
  001902f0: ldr r0,[0x00190504]
  001902f2: movs r5,#0x0
  001902f4: mov r9,r0
  001902f6: b 0x00190336
  00190318: ldr r0,[r4,#0x4]
  0019031a: ldr.w r6,[r0,r5,lsl #0x2]
  0019031e: ldrb.w r0,[r6,#0x40]
  00190322: cbz r0,0x00190334
  00190324: mov r0,r6
  00190326: blx 0x00071ec0
  0019032a: cbnz r0,0x00190334
  0019032c: ldr r0,[r6,#0x4]
  0019032e: mov r1,r9
  00190330: blx 0x000727e4
  00190334: adds r5,#0x1
  00190336: ldr r0,[r4,#0x0]
  00190338: cmp r5,r0
  0019033a: bcc 0x00190318
  0019033c: b 0x001901c8
  0019033e: ldr.w r0,[r9,#0x0]
  00190342: blx 0x00072850
  00190346: blx 0x0007372c
  0019034a: cmp r0,#0x0
  0019034c: beq 0x001903e8
  0019034e: ldr.w r0,[r9,#0x0]
  00190352: blx 0x00071770
  00190356: cmp r0,#0x38
  00190358: bne 0x001903e8
  0019035a: ldr r0,[r4,#0x78]
  0019035c: blx 0x000727c0
  00190360: blx 0x00073ccc
  00190364: ldr r0,[r0,#0x4]
  00190366: add r5,sp,#0x48
  00190368: ldr r1,[r0,#0x8]
  0019036a: mov r0,r5
  0019036c: blx 0x000720b8
  00190370: ldr r2,[0x00190508]
  00190372: add r0,sp,#0x3c
  00190374: mov r1,r5
  00190376: blx 0x0006ec74
  0019037a: vldr.32 s0,[sp,#0x3c]
  0019037e: movs r0,#0x18
  00190380: vldr.32 s2,[sp,#0x40]
  00190384: vldr.32 s4,[sp,#0x44]
  00190388: vcvt.s32.f32 s0,s0
  0019038c: vcvt.s32.f32 s2,s2
  00190390: vcvt.s32.f32 s4,s4
  00190394: vstr.32 s0,[sp,#0x48]
  00190398: vstr.32 s2,[sp,#0x4c]
  0019039c: vstr.32 s4,[sp,#0x50]
  001903a0: blx 0x0006eb24
  001903a4: mov r9,r0
  001903a6: add r1,sp,#0x48
  001903a8: movs r2,#0x3
  001903aa: blx 0x00073eb8
  001903ae: ldr r0,[r4,#0x78]
  001903b0: blx 0x00071eb4
  001903b4: mov r4,r0
  001903b6: movs r5,#0x0
  001903b8: b 0x001903d6
  001903ba: ldr r0,[r4,#0x4]
  001903bc: ldr.w r6,[r0,r5,lsl #0x2]
  001903c0: ldr r0,[r6,#0x28]
  001903c2: cmp r0,#0x1
  001903c4: bne 0x001903d4
  001903c6: mov r0,r9
  001903c8: blx 0x00073f3c
  001903cc: mov r1,r0
  001903ce: mov r0,r6
  001903d0: blx 0x00073f60
  001903d4: adds r5,#0x1
  001903d6: ldr r0,[r4,#0x0]
  001903d8: cmp r5,r0
  001903da: bcc 0x001903ba
  001903dc: mov r0,r9
  001903de: blx 0x00072310
  001903e2: blx 0x0006eb48
  001903e6: b 0x001901c8
  001903e8: ldr.w r0,[r9,#0x0]
  001903ec: blx 0x00072850
  001903f0: blx 0x0007372c
  001903f4: cbz r0,0x0019042a
  001903f6: ldr.w r0,[r9,#0x0]
  001903fa: blx 0x00071770
  001903fe: cmp r0,#0x3f
  00190400: bne 0x0019042a
  00190402: ldr r0,[r4,#0x78]
  00190404: blx 0x00071eb4
  00190408: mov r4,r0
  0019040a: movs r5,#0x0
  0019040c: b 0x00190422
  0019040e: ldr r0,[r4,#0x4]
  00190410: ldr.w r0,[r0,r5,lsl #0x2]
  00190414: ldr r1,[r0,#0x28]
  00190416: cmp r1,#0x8
  00190418: itt eq
  0019041a: ldr.eq r0,[r0,#0x4]
  0019041c: blx.eq 0x00076bb8
  00190420: adds r5,#0x1
  00190422: ldr r0,[r4,#0x0]
  00190424: cmp r5,r0
  00190426: bcc 0x0019040e
  00190428: b 0x001901c8
  0019042a: ldr.w r0,[r9,#0x0]
  0019042e: blx 0x00072850
  00190432: blx 0x0007372c
  00190436: cbz r0,0x00190482
  00190438: ldr.w r0,[r9,#0x0]
  0019043c: blx 0x00071770
  00190440: cmp r0,#0x49
  00190442: bne 0x00190482
  00190444: ldr r0,[r4,#0x78]
  00190446: blx 0x00071eb4
  0019044a: mov r4,r0
  0019044c: ldr r0,[0x00190504]
  0019044e: movs r5,#0x0
  00190450: mov r9,r0
  00190452: b 0x0019047a
  00190454: ldr r0,[r4,#0x4]
  00190456: ldr.w r6,[r0,r5,lsl #0x2]
  0019045a: ldrb.w r0,[r6,#0x40]
  0019045e: cbz r0,0x00190478
  00190460: mov r0,r6
  00190462: blx 0x00071ec0
  00190466: cbnz r0,0x00190478
  00190468: ldr r0,[r6,#0x4]
  0019046a: mov r1,r9
  0019046c: blx 0x000727e4
  00190470: mov r0,r6
  00190472: movs r1,#0x1
  00190474: blx 0x00073f54
  00190478: adds r5,#0x1
  0019047a: ldr r0,[r4,#0x0]
  0019047c: cmp r5,r0
  0019047e: bcc 0x00190454
  00190480: b 0x001901c8
  00190482: ldr.w r0,[r9,#0x0]
  00190486: blx 0x00071c14
  0019048a: blx 0x00071824
  0019048e: cmp r0,#0x70
  00190490: bne.w 0x001901c8
  00190494: ldr.w r0,[r9,#0x0]
  00190498: blx 0x00071770
  0019049c: movs r6,#0x1
  0019049e: cmp r0,#0x8f
  001904a0: bne.w 0x00190270
  001904a4: ldr r0,[0x0019050c]
  001904a6: add r0,pc
  001904a8: ldr r0,[r0,#0x0]
  001904aa: strb r6,[r0,#0x0]
  001904ac: b 0x00190270
```
