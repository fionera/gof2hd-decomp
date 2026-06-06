# Status::missionCompleted
elf 0xabf38 body 1320
Sig: undefined __stdcall missionCompleted(bool param_1, bool param_2, longlong param_3)

## decompile
```c

/* Status::missionCompleted(bool, bool, long long) */

Mission * Status::missionCompleted(bool param_1,bool param_2,longlong param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  Agent *this;
  uint *puVar4;
  Item *pIVar5;
  int iVar6;
  Array *pAVar7;
  uint uVar8;
  int *piVar9;
  uint uVar10;
  byte bVar11;
  BluePrint *this_00;
  byte bVar12;
  Mission *this_01;
  uint uVar13;
  uint in_stack_00000000;
  int in_stack_00000004;
  
  uVar8 = (uint)param_2;
  uVar1 = (uint)param_1;
  uVar13 = 0;
  uVar2 = uVar8 & (uint)param_3;
  do {
    if (**(uint **)(uVar1 + 0x198) <= uVar13) {
      return (Mission *)0x0;
    }
    this_01 = *(Mission **)((*(uint **)(uVar1 + 0x198))[1] + uVar13 * 4);
    iVar3 = Mission::hasWon(this_01);
    if (iVar3 != 0) {
      return (Mission *)0x0;
    }
    if ((this_01 == (Mission *)0x0) ||
       ((((iVar3 = Mission::isCampaignMission(this_01), iVar3 == 0 &&
          (iVar3 = Mission::getAgent(this_01), iVar3 == 0)) &&
         (iVar3 = Mission::getClientImage(this_01), iVar3 == 0)) &&
        ((iVar3 = Mission::getTargetStation(), iVar3 == 0 &&
         (iVar3 = Mission::getTargetStation(), iVar3 == 0)))))) goto switchD_000bbfe2_caseD_9f;
    iVar3 = Mission::getType();
    switch(iVar3) {
    case 0x96:
      iVar3 = *(int *)(uVar1 + 0x1c4);
      goto LAB_000bc172;
    case 0x97:
      iVar3 = *(int *)(uVar1 + 0x1c0);
      goto LAB_000bc172;
    case 0x98:
      puVar4 = (uint *)Ship::getEquipment(*(Ship **)(uVar1 + 400));
      for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
        pIVar5 = *(Item **)(puVar4[1] + uVar10 * 4);
        if (pIVar5 != (Item *)0x0) {
          iVar3 = Item::getIndex(pIVar5);
          iVar6 = Mission::getStatusValue();
          if (iVar3 == iVar6) {
            return this_01;
          }
        }
      }
      break;
    case 0x99:
      iVar3 = *(int *)(uVar1 + 0x1d0);
      goto LAB_000bc172;
    case 0x9a:
      iVar3 = Ship::getCurrentLoad();
      iVar6 = Mission::getStatusValue();
      goto LAB_000bc166;
    case 0x9b:
      iVar3 = *(int *)(uVar1 + 0x1d4);
LAB_000bc172:
      iVar6 = Mission::getStatusValue();
      if (iVar6 <= iVar3) {
LAB_000bc4a8:
        Mission::setWon(this_01,true);
        return this_01;
      }
      break;
    case 0x9c:
      if (uVar8 == 0) {
        iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
        iVar6 = Mission::getTargetStation();
        if (in_stack_00000004 < (int)(uint)(in_stack_00000000 < 0x2711)) break;
LAB_000bc414:
        if (iVar3 == iVar6) {
          return this_01;
        }
      }
      break;
    case 0x9d:
      puVar4 = (uint *)Ship::getEquipment(*(Ship **)(uVar1 + 400));
      for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
        pIVar5 = *(Item **)(puVar4[1] + uVar10 * 4);
        if (pIVar5 != (Item *)0x0) {
          iVar3 = Item::getType(pIVar5);
          iVar6 = Mission::getStatusValue();
          if (iVar3 == iVar6) {
            return this_01;
          }
        }
      }
      break;
    case 0x9e:
      puVar4 = (uint *)Ship::getEquipment(*(Ship **)(uVar1 + 400));
      bVar12 = 0;
      bVar11 = 0;
      for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
        pIVar5 = *(Item **)(puVar4[1] + uVar10 * 4);
        if (pIVar5 != (Item *)0x0) {
          iVar3 = Item::getType(pIVar5);
          if (iVar3 == 0) {
            bVar12 = 1;
          }
          else {
            pIVar5 = *(Item **)(puVar4[1] + uVar10 * 4);
            if (pIVar5 != (Item *)0x0) {
              iVar3 = Item::getSort(pIVar5);
              bVar11 = bVar11 | iVar3 == 10;
            }
          }
        }
      }
      if ((bool)(bVar12 & bVar11)) {
        return this_01;
      }
      break;
    case 0x9f:
    case 0xa1:
    case 0xad:
    case 0xaf:
    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xb6:
    case 0xb7:
    case 0xb9:
    case 0xba:
    case 0xbb:
    case 0xbc:
      break;
    case 0xa0:
      if (uVar8 != 0) {
        return this_01;
      }
      iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
      iVar6 = Mission::getTargetStation();
      if (((int)(uint)(in_stack_00000000 < 0x2711) <= in_stack_00000004) && (iVar3 != iVar6)) {
        return this_01;
      }
      break;
    case 0xa2:
      if (uVar8 != 0) {
        iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
        iVar6 = Mission::getTargetStation();
        if (iVar3 == iVar6) {
          iVar6 = *(int *)(uVar1 + 0x18);
          iVar3 = Mission::getStatusValue();
          this_00 = *(BluePrint **)(*(int *)(iVar6 + 4) + iVar3 * 4);
          puVar4 = (uint *)BluePrint::getIngredientList(this_00);
          BluePrint::getQuantityList(this_00);
          uVar10 = 0;
          do {
            if (*puVar4 <= uVar10) {
              return this_01;
            }
            iVar3 = Ship::hasCargo(*(int *)(uVar1 + 400),*(int *)(puVar4[1] + uVar10 * 4));
            uVar10 = uVar10 + 1;
          } while (iVar3 != 0);
        }
      }
      break;
    case 0xa3:
      uVar10 = 0;
      do {
        if (**(uint **)(uVar1 + 0x90) <= uVar10) {
          return this_01;
        }
        iVar3 = uVar10 * 4;
        uVar10 = uVar10 + 1;
      } while (*(int *)((*(uint **)(uVar1 + 0x90))[1] + iVar3) < 0);
      break;
    case 0xa4:
      uVar10 = (uint)((int)(-(uint)(10000 < in_stack_00000000) - in_stack_00000004) < 0 !=
                     (SBORROW4(0,in_stack_00000004) !=
                     SBORROW4(-in_stack_00000004,(uint)(10000 < in_stack_00000000)))) & (uVar8 ^ 1);
      goto LAB_000bc458;
    case 0xa5:
      if (uVar8 == 0) {
LAB_000bc404:
        iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
        iVar6 = Mission::getTargetStation();
        goto LAB_000bc414;
      }
      break;
    case 0xa6:
      if (uVar8 != 0) {
        iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
        iVar6 = Mission::getTargetStation();
        if (iVar3 == iVar6) {
          iVar3 = Mission::getProductionGoodIndex(this_01);
          iVar6 = Mission::getProductionGoodAmount();
          pAVar7 = (Array *)Ship::getCargo();
          iVar3 = Item::isInList(iVar3,iVar6,pAVar7);
          if (iVar3 != 0) {
            return this_01;
          }
          iVar6 = *(int *)(**(int **)(DAT_000bc4c4 + 0xbc28e) + 400);
          iVar3 = Mission::getProductionGoodIndex(this_01);
          uVar10 = Ship::hasEquipment(iVar6,iVar3);
          goto LAB_000bc458;
        }
      }
      break;
    case 0xa7:
    case 0xae:
      iVar3 = Mission::getStatusValue();
      iVar6 = Mission::getProductionGoodAmount();
LAB_000bc166:
      if (iVar6 <= iVar3) goto LAB_000bc4a8;
      break;
    case 0xa8:
      piVar9 = *(int **)(DAT_000bc4c8 + 0xbc3e4);
      iVar6 = *(int *)(*piVar9 + 0x178);
      iVar3 = Mission::getStatusValue();
      if (iVar3 <= iVar6) {
        Mission::setWon(this_01,true);
        *(undefined4 *)(*piVar9 + 0x178) = 0;
        return this_01;
      }
      break;
    case 0xa9:
      uVar10 = (uint)((int)(-(uint)(2000 < in_stack_00000000) - in_stack_00000004) < 0 !=
                     (SBORROW4(0,in_stack_00000004) !=
                     SBORROW4(-in_stack_00000004,(uint)(2000 < in_stack_00000000)))) & (uVar8 ^ 1);
LAB_000bc458:
      if (uVar10 != 0) {
        return this_01;
      }
      break;
    case 0xaa:
      iVar3 = Mission::getStatusValue();
      if (iVar3 == 1) goto LAB_000bc4a8;
      break;
    case 0xab:
      uVar10 = uVar2;
joined_r0x000bc0c8:
      if (uVar10 != 0) goto LAB_000bc404;
      break;
    case 0xac:
      if (uVar2 != 0) {
        iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
        iVar6 = Mission::getTargetStation();
        if (iVar3 == iVar6) {
          iVar6 = Mission::getProductionGoodIndex(this_01);
          iVar3 = Mission::getProductionGoodAmount();
          pAVar7 = (Array *)Ship::getCargo();
LAB_000bc454:
          uVar10 = Item::isInList(iVar6,iVar3,pAVar7);
          goto LAB_000bc458;
        }
      }
      break;
    case 0xb8:
      iVar3 = Mission::getStatusValue();
      if ((iVar3 == 0) &&
         (((piVar9 = *(int **)(DAT_000bc4cc + 0xbc2c6), *(int *)(*piVar9 + 0x1e8) != 0x5c ||
           (iVar3 = inAlienOrbit(), iVar3 != 0)) ||
          (iVar3 = Station::getIndex(*(Station **)(*piVar9 + 0x19c)), iVar3 != 0x71))))
      goto LAB_000bc4a8;
      break;
    case 0xbd:
      if (uVar8 != 0) {
        puVar4 = (uint *)Ship::getEquipment(*(Ship **)(uVar1 + 400));
        for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
          pIVar5 = *(Item **)(puVar4[1] + uVar10 * 4);
          if (pIVar5 != (Item *)0x0) {
            iVar3 = Item::getSort(pIVar5);
            iVar6 = Mission::getStatusValue();
            if (iVar3 == iVar6) {
              return this_01;
            }
          }
        }
      }
      break;
    default:
      switch(iVar3) {
      case 8:
        iVar3 = Mission::isCampaignMission(this_01);
        if ((iVar3 != 0) &&
           (*(int *)(**(int **)(DAT_000bc3f8 + 0xbc06e) + 0x1e8) == 0x8f && uVar8 == 0)) {
          iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
          iVar6 = Mission::getTargetStation();
          if ((iVar3 == iVar6) && (puVar4 = *(uint **)(uVar1 + 0x1c), puVar4 != (uint *)0x0)) {
            for (uVar10 = 0; uVar10 < *puVar4; uVar10 = uVar10 + 1) {
              iVar3 = *(int *)(puVar4[1] + uVar10 * 4);
              if ((iVar3 != 0) && (*(int *)(iVar3 + 0x14) == 0xd2)) {
                return this_01;
              }
            }
          }
        }
        if (uVar8 != 0) {
          iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
          iVar6 = Mission::getTargetStation();
          if (iVar3 == iVar6) {
            iVar6 = Mission::getProductionGoodIndex(this_01);
            iVar3 = Mission::getProductionGoodAmount();
            pAVar7 = (Array *)Ship::getCargo();
            goto LAB_000bc454;
          }
        }
        break;
      case 9:
      case 10:
      case 0xc:
        break;
      case 0xb:
switchD_000bc04e_caseD_b:
        uVar10 = uVar8;
        goto joined_r0x000bc0c8;
      case 0xd:
        if (uVar8 != 0) {
          uVar10 = (uint)*(byte *)(uVar1 + 0xf0);
          goto LAB_000bc458;
        }
        break;
      case 0xe:
        if (uVar8 != 0) {
          iVar3 = Station::getIndex(*(Station **)(uVar1 + 0x19c));
          this = (Agent *)Mission::getAgent(this_01);
          iVar6 = Agent::getStation(this);
          if (iVar3 == iVar6) {
            pAVar7 = (Array *)Ship::getCargo();
            uVar10 = Item::isInList(0x73,pAVar7);
            goto LAB_000bc458;
          }
        }
        break;
      default:
        if (iVar3 == 0) goto switchD_000bc04e_caseD_b;
      }
    }
switchD_000bbfe2_caseD_9f:
    uVar13 = uVar13 + 1;
  } while( true );
}

```
## target disasm
```
  000bbf38: push {r4,r5,r6,r7,lr}
  000bbf3a: add r7,sp,#0xc
  000bbf3c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000bbf40: ldrd r3,r5,[r7,#0x8]
  000bbf44: mov r6,r0
  000bbf46: mov.w r11,#0x0
  000bbf4a: mov r8,r1
  000bbf4c: rsbs.w r0,r3,#0x7d0
  000bbf50: eor r1,r1,#0x1
  000bbf54: sbcs.w r0,r11,r5
  000bbf58: mov.w r0,#0x0
  000bbf5c: it lt
  000bbf5e: mov.lt r0,#0x1
  000bbf60: ands r0,r1
  000bbf62: str r0,[sp,#0x8]
  000bbf64: and.w r0,r8,r2
  000bbf68: str r0,[sp,#0xc]
  000bbf6a: movw r0,#0x2710
  000bbf6e: subs r0,r0,r3
  000bbf70: sbcs.w r0,r11,r5
  000bbf74: mov.w r0,#0x0
  000bbf78: it lt
  000bbf7a: mov.lt r0,#0x1
  000bbf7c: str r6,[sp,#0x10]
  000bbf7e: ands r0,r1
  000bbf80: str r0,[sp,#0x4]
  000bbf82: ldr.w r0,[r6,#0x198]
  000bbf86: ldr r1,[r0,#0x0]
  000bbf88: cmp r11,r1
  000bbf8a: bcs.w 0x000bc49a
  000bbf8e: ldr r0,[r0,#0x4]
  000bbf90: ldr.w r10,[r0,r11,lsl #0x2]
  000bbf94: mov r0,r10
  000bbf96: blx 0x00073930
  000bbf9a: cmp r0,#0x0
  000bbf9c: bne.w 0x000bc49a
  000bbfa0: cmp.w r10,#0x0
  000bbfa4: beq.w 0x000bc45a
  000bbfa8: mov r0,r10
  000bbfaa: blx 0x0007372c
  000bbfae: cbnz r0,0x000bbfd4
  000bbfb0: mov r0,r10
  000bbfb2: blx 0x00073438
  000bbfb6: cbnz r0,0x000bbfd4
  000bbfb8: mov r0,r10
  000bbfba: blx 0x0007393c
  000bbfbe: cbnz r0,0x000bbfd4
  000bbfc0: mov r0,r10
  000bbfc2: blx 0x00073738
  000bbfc6: cbnz r0,0x000bbfd4
  000bbfc8: mov r0,r10
  000bbfca: blx 0x00073738
  000bbfce: cmp r0,#0x0
  000bbfd0: beq.w 0x000bc45a
  000bbfd4: mov r0,r10
  000bbfd6: blx 0x00072874
  000bbfda: sub.w r1,r0,#0x96
  000bbfde: cmp r1,#0x27
  000bbfe0: bhi 0x000bc046
  000bbfe2: tbh [pc,r1]
  000bc036: mov r0,r10
  000bc038: blx 0x0007288c
  000bc03c: mov r5,r0
  000bc03e: mov r0,r10
  000bc040: blx 0x00072bb0
  000bc044: b 0x000bc166
  000bc046: sub.w r1,r0,#0x8
  000bc04a: cmp r1,#0x6
  000bc04c: bhi 0x000bc0be
  000bc04e: tbh [pc,r1]
  000bc060: mov r0,r10
  000bc062: blx 0x0007372c
  000bc066: cbz r0,0x000bc080
  000bc068: ldr r0,[0x000bc3f8]
  000bc06a: add r0,pc
  000bc06c: ldr r0,[r0,#0x0]
  000bc06e: ldr r0,[r0,#0x0]
  000bc070: ldr.w r0,[r0,#0x1e8]
  000bc074: cmp r0,#0x8f
  000bc076: it eq
  000bc078: cmp.eq.w r8,#0x0
  000bc07c: beq.w 0x000bc460
  000bc080: cmp.w r8,#0x0
  000bc084: beq.w 0x000bc45a
  000bc088: ldr.w r0,[r6,#0x19c]
  000bc08c: blx 0x00071824
  000bc090: mov r5,r0
  000bc092: mov r0,r10
  000bc094: blx 0x00073738
  000bc098: cmp r5,r0
  000bc09a: bne.w 0x000bc45a
  000bc09e: mov r0,r10
  000bc0a0: blx 0x000728a4
  000bc0a4: mov r9,r0
  000bc0a6: mov r0,r10
  000bc0a8: blx 0x00072bb0
  000bc0ac: mov r5,r0
  000bc0ae: ldr.w r0,[r6,#0x190]
  000bc0b2: blx 0x00073750
  000bc0b6: mov r2,r0
  000bc0b8: mov r0,r9
  000bc0ba: mov r1,r5
  000bc0bc: b 0x000bc454
  000bc0be: cmp r0,#0x0
  000bc0c0: bne.w 0x000bc45a
  000bc0c4: cmp.w r8,#0x0
  000bc0c8: bne.w 0x000bc404
  000bc0cc: b 0x000bc45a
  000bc0ce: cmp.w r8,#0x0
  000bc0d2: beq.w 0x000bc45a
  000bc0d6: ldrb.w r0,[r6,#0xf0]
  000bc0da: b 0x000bc458
  000bc0dc: cmp.w r8,#0x0
  000bc0e0: beq.w 0x000bc45a
  000bc0e4: ldr.w r0,[r6,#0x19c]
  000bc0e8: blx 0x00071824
  000bc0ec: mov r5,r0
  000bc0ee: mov r0,r10
  000bc0f0: blx 0x00073438
  000bc0f4: blx 0x00071ac4
  000bc0f8: cmp r5,r0
  000bc0fa: bne.w 0x000bc45a
  000bc0fe: ldr.w r0,[r6,#0x190]
  000bc102: blx 0x00073750
  000bc106: mov r1,r0
  000bc108: movs r0,#0x73
  000bc10a: blx 0x00073948
  000bc10e: b 0x000bc458
  000bc110: ldr.w r4,[r6,#0x1c4]
  000bc114: b 0x000bc172
  000bc116: ldr.w r4,[r6,#0x1c0]
  000bc11a: b 0x000bc172
  000bc11c: ldr.w r0,[r6,#0x190]
  000bc120: blx 0x0007333c
  000bc124: mov r5,r0
  000bc126: movs r4,#0x0
  000bc128: b 0x000bc148
  000bc12a: ldr r0,[r5,#0x4]
  000bc12c: ldr.w r0,[r0,r4,lsl #0x2]
  000bc130: cbz r0,0x000bc146
  000bc132: blx 0x000718d8
  000bc136: mov r6,r0
  000bc138: mov r0,r10
  000bc13a: blx 0x0007288c
  000bc13e: cmp r6,r0
  000bc140: ldr r6,[sp,#0x10]
  000bc142: beq.w 0x000bc49e
  000bc146: adds r4,#0x1
  000bc148: ldr r0,[r5,#0x0]
  000bc14a: cmp r4,r0
  000bc14c: bcc 0x000bc12a
  000bc14e: b 0x000bc45a
  000bc150: ldr.w r4,[r6,#0x1d0]
  000bc154: b 0x000bc172
  000bc156: ldr.w r0,[r6,#0x190]
  000bc15a: blx 0x00072bf8
  000bc15e: mov r5,r0
  000bc160: mov r0,r10
  000bc162: blx 0x0007288c
  000bc166: cmp r5,r0
  000bc168: blt.w 0x000bc45a
  000bc16c: b 0x000bc4a8
  000bc16e: ldr.w r4,[r6,#0x1d4]
  000bc172: mov r0,r10
  000bc174: blx 0x0007288c
  000bc178: cmp r4,r0
  000bc17a: blt.w 0x000bc45a
  000bc17e: b 0x000bc4a8
  000bc180: ldr.w r0,[r6,#0x190]
  000bc184: mov r4,r8
  000bc186: blx 0x0007333c
  000bc18a: mov r5,r0
  000bc18c: mov.w r9,#0x0
  000bc190: movs r6,#0x0
  000bc192: mov.w r8,#0x0
  000bc196: b 0x000bc1c6
  000bc198: ldr r0,[r5,#0x4]
  000bc19a: ldr.w r0,[r0,r8,lsl #0x2]
  000bc19e: cbz r0,0x000bc1c2
  000bc1a0: blx 0x000718fc
  000bc1a4: cbz r0,0x000bc1be
  000bc1a6: ldr r0,[r5,#0x4]
  000bc1a8: ldr.w r0,[r0,r8,lsl #0x2]
  000bc1ac: cbz r0,0x000bc1c2
  000bc1ae: blx 0x00071908
  000bc1b2: subs r0,#0xa
  000bc1b4: clz r0,r0
  000bc1b8: lsrs r0,r0,#0x5
  000bc1ba: orrs r6,r0
  000bc1bc: b 0x000bc1c2
  000bc1be: mov.w r9,#0x1
  000bc1c2: add.w r8,r8,#0x1
  000bc1c6: ldr r0,[r5,#0x0]
  000bc1c8: cmp r8,r0
  000bc1ca: bcc 0x000bc198
  000bc1cc: and.w r0,r9,r6
  000bc1d0: ldr r6,[sp,#0x10]
  000bc1d2: mov r8,r4
  000bc1d4: lsls r0,r0,#0x1f
  000bc1d6: beq.w 0x000bc45a
  000bc1da: b 0x000bc49e
  000bc1dc: cmp.w r8,#0x0
  000bc1e0: beq.w 0x000bc45a
  000bc1e4: ldr.w r0,[r6,#0x19c]
  000bc1e8: blx 0x00071824
  000bc1ec: mov r5,r0
  000bc1ee: mov r0,r10
  000bc1f0: blx 0x00073738
  000bc1f4: cmp r5,r0
  000bc1f6: bne.w 0x000bc45a
  000bc1fa: mov r0,r10
  000bc1fc: ldr r4,[r6,#0x18]
  000bc1fe: blx 0x0007288c
  000bc202: ldr r1,[r4,#0x4]
  000bc204: ldr.w r6,[r1,r0,lsl #0x2]
  000bc208: mov r0,r6
  000bc20a: blx 0x00073954
  000bc20e: mov r5,r0
  000bc210: mov r0,r6
  000bc212: ldr r6,[sp,#0x10]
  000bc214: blx 0x00073960
  000bc218: mov r9,r0
  000bc21a: movs r4,#0x0
  000bc21c: ldr r0,[r5,#0x0]
  000bc21e: cmp r4,r0
  000bc220: bcs.w 0x000bc49e
  000bc224: ldr.w r1,[r9,#0x4]
  000bc228: ldr r3,[r5,#0x4]
  000bc22a: ldr.w r0,[r6,#0x190]
  000bc22e: ldr.w r2,[r1,r4,lsl #0x2]
  000bc232: ldr.w r1,[r3,r4,lsl #0x2]
  000bc236: blx 0x00071a70
  000bc23a: adds r4,#0x1
  000bc23c: cmp r0,#0x0
  000bc23e: bne 0x000bc21c
  000bc240: b 0x000bc45a
  000bc242: cmp.w r8,#0x0
  000bc246: beq.w 0x000bc45a
  000bc24a: ldr.w r0,[r6,#0x19c]
  000bc24e: blx 0x00071824
  000bc252: mov r5,r0
  000bc254: mov r0,r10
  000bc256: blx 0x00073738
  000bc25a: cmp r5,r0
  000bc25c: bne.w 0x000bc45a
  000bc260: mov r0,r10
  000bc262: blx 0x000728a4
  000bc266: mov r9,r0
  000bc268: mov r0,r10
  000bc26a: blx 0x00072bb0
  000bc26e: mov r5,r0
  000bc270: ldr.w r0,[r6,#0x190]
  000bc274: blx 0x00073750
  000bc278: mov r2,r0
  000bc27a: mov r0,r9
  000bc27c: mov r1,r5
  000bc27e: blx 0x0007396c
  000bc282: cmp r0,#0x0
  000bc284: bne.w 0x000bc49e
  000bc288: ldr r0,[0x000bc4c4]
  000bc28a: add r0,pc
  000bc28c: ldr r0,[r0,#0x0]
  000bc28e: ldr r0,[r0,#0x0]
  000bc290: ldr.w r5,[r0,#0x190]
  000bc294: mov r0,r10
  000bc296: blx 0x000728a4
  000bc29a: mov r1,r0
  000bc29c: mov r0,r5
  000bc29e: movs r2,#0x1
  000bc2a0: blx 0x00073978
  000bc2a4: b 0x000bc458
  000bc2a6: mov r0,r10
  000bc2a8: blx 0x0007288c
  000bc2ac: cmp r0,#0x1
  000bc2ae: bne.w 0x000bc45a
  000bc2b2: b 0x000bc4a8
  000bc2b4: mov r0,r10
  000bc2b6: blx 0x0007288c
  000bc2ba: cmp r0,#0x0
  000bc2bc: bne.w 0x000bc45a
  000bc2c0: ldr r0,[0x000bc4cc]
  000bc2c2: add r0,pc
  000bc2c4: ldr r4,[r0,#0x0]
  000bc2c6: ldr r0,[r4,#0x0]
  000bc2c8: ldr.w r1,[r0,#0x1e8]
  000bc2cc: cmp r1,#0x5c
  000bc2ce: bne.w 0x000bc4a8
  000bc2d2: blx 0x000723d0
  000bc2d6: cmp r0,#0x0
  000bc2d8: bne.w 0x000bc4a8
  000bc2dc: ldr r0,[r4,#0x0]
  000bc2de: ldr.w r0,[r0,#0x19c]
  000bc2e2: blx 0x00071824
  000bc2e6: cmp r0,#0x71
  000bc2e8: beq.w 0x000bc45a
  000bc2ec: b 0x000bc4a8
  000bc2ee: cmp.w r8,#0x0
  000bc2f2: beq.w 0x000bc45a
  000bc2f6: ldr.w r0,[r6,#0x190]
  000bc2fa: blx 0x0007333c
  000bc2fe: mov r5,r0
  000bc300: movs r4,#0x0
  000bc302: b 0x000bc322
  000bc304: ldr r0,[r5,#0x4]
  000bc306: ldr.w r0,[r0,r4,lsl #0x2]
  000bc30a: cbz r0,0x000bc320
  000bc30c: blx 0x00071908
  000bc310: mov r6,r0
  000bc312: mov r0,r10
  000bc314: blx 0x0007288c
  000bc318: cmp r6,r0
  000bc31a: ldr r6,[sp,#0x10]
  000bc31c: beq.w 0x000bc49e
  000bc320: adds r4,#0x1
  000bc322: ldr r0,[r5,#0x0]
  000bc324: cmp r4,r0
  000bc326: bcc 0x000bc304
  000bc328: b 0x000bc45a
  000bc32a: cmp.w r8,#0x0
  000bc32e: bne.w 0x000bc45a
  000bc332: ldr.w r0,[r6,#0x19c]
  000bc336: blx 0x00071824
  000bc33a: mov r5,r0
  000bc33c: mov r0,r10
  000bc33e: blx 0x00073738
  000bc342: ldr r1,[r7,#0x8]
  000bc344: movw r2,#0x2711
  000bc348: subs r1,r1,r2
  000bc34a: ldr r1,[r7,#0xc]
  000bc34c: sbcs r1,r1,#0x0
  000bc350: bge 0x000bc414
  000bc352: b 0x000bc45a
  000bc354: ldr.w r0,[r6,#0x190]
  000bc358: blx 0x0007333c
  000bc35c: mov r5,r0
  000bc35e: movs r4,#0x0
  000bc360: b 0x000bc380
  000bc362: ldr r0,[r5,#0x4]
  000bc364: ldr.w r0,[r0,r4,lsl #0x2]
  000bc368: cbz r0,0x000bc37e
  000bc36a: blx 0x000718fc
  000bc36e: mov r6,r0
  000bc370: mov r0,r10
  000bc372: blx 0x0007288c
  000bc376: cmp r6,r0
  000bc378: ldr r6,[sp,#0x10]
  000bc37a: beq.w 0x000bc49e
  000bc37e: adds r4,#0x1
  000bc380: ldr r0,[r5,#0x0]
  000bc382: cmp r4,r0
  000bc384: bcc 0x000bc362
  000bc386: b 0x000bc45a
  000bc388: cmp.w r8,#0x0
  000bc38c: bne.w 0x000bc49e
  000bc390: ldr.w r0,[r6,#0x19c]
  000bc394: blx 0x00071824
  000bc398: mov r5,r0
  000bc39a: mov r0,r10
  000bc39c: blx 0x00073738
  000bc3a0: ldr r1,[r7,#0x8]
  000bc3a2: movw r2,#0x2711
  000bc3a6: subs r1,r1,r2
  000bc3a8: ldr r1,[r7,#0xc]
  000bc3aa: sbcs r1,r1,#0x0
  000bc3ae: blt 0x000bc45a
  000bc3b0: cmp r5,r0
  000bc3b2: beq 0x000bc45a
  000bc3b4: b 0x000bc49e
  000bc3b6: ldr.w r0,[r6,#0x90]
  000bc3ba: movs r2,#0x0
  000bc3bc: ldr r1,[r0,#0x0]
  000bc3be: cmp r2,r1
  000bc3c0: bcs 0x000bc49e
  000bc3c2: ldr r3,[r0,#0x4]
  000bc3c4: ldr.w r3,[r3,r2,lsl #0x2]
  000bc3c8: adds r2,#0x1
  000bc3ca: cmp.w r3,#0xffffffff
  000bc3ce: ble 0x000bc3be
  000bc3d0: b 0x000bc45a
  000bc3d2: ldr r0,[sp,#0x4]
  000bc3d4: b 0x000bc458
  000bc3d6: cmp.w r8,#0x0
  000bc3da: beq 0x000bc404
  000bc3dc: b 0x000bc45a
  000bc3de: ldr r0,[0x000bc4c8]
  000bc3e0: add r0,pc
  000bc3e2: ldr r4,[r0,#0x0]
  000bc3e4: ldr r0,[r4,#0x0]
  000bc3e6: ldr.w r5,[r0,#0x178]
  000bc3ea: mov r0,r10
  000bc3ec: blx 0x0007288c
  000bc3f0: cmp r5,r0
  000bc3f2: blt 0x000bc45a
  000bc3f4: b 0x000bc4b2
  000bc3fc: ldr r0,[sp,#0x8]
  000bc3fe: b 0x000bc458
  000bc400: ldr r0,[sp,#0xc]
  000bc402: cbz r0,0x000bc45a
  000bc404: ldr.w r0,[r6,#0x19c]
  000bc408: blx 0x00071824
  000bc40c: mov r5,r0
  000bc40e: mov r0,r10
  000bc410: blx 0x00073738
  000bc414: cmp r5,r0
  000bc416: bne 0x000bc45a
  000bc418: b 0x000bc49e
  000bc41a: ldr r0,[sp,#0xc]
  000bc41c: cbz r0,0x000bc45a
  000bc41e: ldr.w r0,[r6,#0x19c]
  000bc422: blx 0x00071824
  000bc426: mov r5,r0
  000bc428: mov r0,r10
  000bc42a: blx 0x00073738
  000bc42e: cmp r5,r0
  000bc430: bne 0x000bc45a
  000bc432: mov r0,r10
  000bc434: blx 0x000728a4
  000bc438: mov r5,r0
  000bc43a: mov r0,r10
  000bc43c: blx 0x00072bb0
  000bc440: mov r6,r0
  000bc442: ldr r0,[sp,#0x10]
  000bc444: ldr.w r0,[r0,#0x190]
  000bc448: blx 0x00073750
  000bc44c: mov r1,r6
  000bc44e: ldr r6,[sp,#0x10]
  000bc450: mov r2,r0
  000bc452: mov r0,r5
  000bc454: blx 0x0007396c
  000bc458: cbnz r0,0x000bc49e
  000bc45a: add.w r11,r11,#0x1
  000bc45e: b 0x000bbf82
  000bc460: ldr.w r0,[r6,#0x19c]
  000bc464: blx 0x00071824
  000bc468: mov r5,r0
  000bc46a: mov r0,r10
  000bc46c: blx 0x00073738
  000bc470: cmp r5,r0
  000bc472: bne.w 0x000bc080
  000bc476: ldr r0,[r6,#0x1c]
  000bc478: cmp r0,#0x0
  000bc47a: beq.w 0x000bc080
  000bc47e: ldr r1,[r0,#0x0]
  000bc480: movs r2,#0x0
  000bc482: b 0x000bc494
  000bc484: ldr r3,[r0,#0x4]
  000bc486: ldr.w r3,[r3,r2,lsl #0x2]
  000bc48a: cbz r3,0x000bc492
  000bc48c: ldr r3,[r3,#0x14]
  000bc48e: cmp r3,#0xd2
  000bc490: beq 0x000bc49e
  000bc492: adds r2,#0x1
  000bc494: cmp r2,r1
  000bc496: bcc 0x000bc484
  000bc498: b 0x000bc080
  000bc49a: mov.w r10,#0x0
  000bc49e: mov r0,r10
  000bc4a0: add sp,#0x14
  000bc4a2: pop.w {r8,r9,r10,r11}
  000bc4a6: pop {r4,r5,r6,r7,pc}
  000bc4a8: mov r0,r10
  000bc4aa: movs r1,#0x1
  000bc4ac: blx 0x00073984
  000bc4b0: b 0x000bc49e
  000bc4b2: mov r0,r10
  000bc4b4: movs r1,#0x1
  000bc4b6: blx 0x00073984
  000bc4ba: ldr r0,[r4,#0x0]
  000bc4bc: movs r1,#0x0
  000bc4be: str.w r1,[r0,#0x178]
  000bc4c2: b 0x000bc49e
```
