# Player::damage
elf 0xa31c0 body 1192
Sig: undefined __stdcall damage(int param_1, bool param_2, int param_3)

## decompile
```c

/* Player::damage(int, bool, int) */

void Player::damage(int param_1,bool param_2,int param_3)

{
  float fVar1;
  int iVar2;
  SolarSystem *this;
  Ship *pSVar3;
  uint uVar4;
  Item *pIVar5;
  Standing *pSVar6;
  PlayerEgo *pPVar7;
  int *piVar8;
  Standing *pSVar9;
  String *pSVar10;
  Player *this_00;
  uint uVar11;
  int in_r3;
  int *piVar12;
  int iVar13;
  int iVar14;
  bool bVar15;
  uint in_fpscr;
  uint uVar16;
  float fVar17;
  undefined4 uVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  
  uVar11 = (uint)param_2;
  if (*(char *)(param_1 + 0xc2) == '\0') {
    return;
  }
  if (*(char *)(param_1 + 0xc0) == '\0') {
    return;
  }
  if (*(int *)(param_1 + 0x78) < 1) {
    return;
  }
  if (param_3 == 0) {
    if (*(int *)(param_1 + 0xd0) != 0) {
      if ((((*(char *)(param_1 + 0xec) == '\0') &&
           (1 < *(int *)(*(int *)(param_1 + 0xd0) + 0x28) - 9U)) &&
          (iVar2 = Status::getSystem(), iVar2 != 0)) &&
         ((*(char *)(param_1 + 0x5c) == '\0' || (*(char *)(param_1 + 0xe0) != '\0')))) {
        iVar2 = *(int *)(param_1 + 0xd0);
        if (*(char *)(iVar2 + 0x42) != '\0') {
          if (uVar11 != 0) {
            *(uint *)(param_1 + 0x6c) = *(int *)(param_1 + 0x6c) + uVar11;
            Level::attackWanted(*(Level **)(iVar2 + 0x54),*(int *)(iVar2 + 0x48));
          }
          goto LAB_000b3488;
        }
      }
      else {
        iVar2 = *(int *)(param_1 + 0xd0);
      }
      if ((((iVar2 != 0) && (*(char *)(param_1 + 0xec) == '\0')) &&
          ((1 < *(int *)(iVar2 + 0x28) - 9U &&
           ((iVar2 = KIPlayer::isWingMan(), iVar2 == 0 && (iVar2 = Status::getSystem(), iVar2 != 0))
           )))) && ((*(char *)(param_1 + 0x5c) == '\0' || (*(char *)(param_1 + 0xe0) != '\0')))) {
        iVar13 = *(int *)(*(int *)(param_1 + 0xd0) + 0x28);
        Status::getSystem();
        iVar2 = SolarSystem::getRace();
        if (iVar13 != iVar2) {
          iVar13 = *(int *)(*(int *)(param_1 + 0xd0) + 0x28);
          this = (SolarSystem *)Status::getSystem();
          iVar2 = SolarSystem::getAttackRace(this);
          if (iVar13 != iVar2) goto LAB_000b342a;
        }
        *(uint *)(param_1 + 0x6c) = *(int *)(param_1 + 0x6c) + uVar11;
        iVar2 = Status::hardCoreMode();
        fVar21 = 0.5;
        fVar17 = DAT_000b3518;
        if (iVar2 != 0) {
          fVar17 = DAT_000b351c;
        }
        fVar19 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x84),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar20 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x6c),
                                            (byte)(in_fpscr >> 0x16) & 3);
        fVar1 = DAT_000b367c;
        if (iVar2 != 0) {
          fVar21 = 0.25;
          fVar1 = DAT_000b3680;
        }
        uVar16 = in_fpscr & 0xfffffff;
        if (fVar17 * fVar19 < fVar20) {
          Level::friendTurnedEnemy(*(int *)(*(int *)(param_1 + 0xd0) + 0x54));
          pSVar3 = (Ship *)Status::getShip();
          pSVar9 = (Standing *)Status::getStanding();
          iVar2 = Ship::getSignatureRace(pSVar3);
          if (-1 < iVar2) {
            uVar4 = Ship::getSignatureRace(pSVar3);
            bVar15 = uVar4 == *(uint *)(*(int *)(param_1 + 0xd0) + 0x28);
            if (bVar15) {
              uVar4 = (uint)*(byte *)(*(int *)(param_1 + 0xd0) + 0x42);
            }
            if (bVar15 && uVar4 == 0) {
              pIVar5 = (Item *)Ship::getFirstEquipmentOfSort((int)pSVar3);
              Ship::removeEquipment(pSVar3,pIVar5);
              pSVar6 = (Standing *)Status::getStanding();
              iVar2 = Ship::getSignatureRace(pSVar3);
              Standing::applyDelict(pSVar6,iVar2,100);
              Standing::setPlayerSignatureRace(pSVar9,-1);
              pPVar7 = (PlayerEgo *)Level::getPlayer();
              iVar2 = PlayerEgo::getHUD(pPVar7);
              iVar13 = Level::getPlayer();
              Hud::hudEvent(iVar2,(PlayerEgo *)&DAT_0000001f,iVar13);
            }
          }
        }
        fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x84),
                                            (byte)(uVar16 >> 0x16) & 3);
        fVar19 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x6c),
                                            (byte)(uVar16 >> 0x16) & 3);
        uVar16 = uVar16 & 0xfffffff;
        if (fVar21 * fVar17 < fVar19) {
          pSVar3 = (Ship *)Status::getShip();
          pSVar9 = (Standing *)Status::getStanding();
          iVar2 = Ship::getSignatureRace(pSVar3);
          if (((-1 < iVar2) && (*(int *)(*(int *)(param_1 + 0xd0) + 0x28) < 4)) &&
             (*(char *)(*(int *)(param_1 + 0xd0) + 0x42) == '\0')) {
            pIVar5 = (Item *)Ship::getFirstEquipmentOfSort((int)pSVar3);
            Ship::removeEquipment(pSVar3,pIVar5);
            pSVar6 = (Standing *)Status::getStanding();
            iVar2 = Ship::getSignatureRace(pSVar3);
            Standing::applyDelict(pSVar6,iVar2,100);
            Standing::setPlayerSignatureRace(pSVar9,-1);
            pPVar7 = (PlayerEgo *)Level::getPlayer();
            iVar2 = PlayerEgo::getHUD(pPVar7);
            iVar13 = Level::getPlayer();
            Hud::hudEvent(iVar2,(PlayerEgo *)&DAT_0000001f,iVar13);
          }
          fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x84),
                                              (byte)(uVar16 >> 0x16) & 3);
          fVar19 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x6c),
                                              (byte)(uVar16 >> 0x16) & 3);
          *(undefined1 *)(param_1 + 0xe0) = 1;
        }
        in_fpscr = uVar16 & 0xfffffff;
        if (fVar1 * fVar17 < fVar19) {
          Level::alarmAllFriends
                    (*(Level **)(*(int *)(param_1 + 0xd0) + 0x54),
                     *(int *)(*(int *)(param_1 + 0xd0) + 0x28),true);
        }
        goto LAB_000b3488;
      }
    }
LAB_000b342a:
    piVar12 = *(int **)(DAT_000b3688 + 0xb3430);
    iVar2 = Status::inBlackMarketSystem();
    if (((iVar2 != 0) && (iVar2 = *(int *)(param_1 + 0xd0), iVar2 != 0)) &&
       (*(int *)(iVar2 + 0x28) == 8)) {
      *(undefined1 *)(param_1 + 0xe0) = 1;
      Level::alarmAllFriends(*(Level **)(iVar2 + 0x54),8,true);
      piVar8 = (int *)Level::getEnemies();
      if (piVar8 != (int *)0x0) {
        iVar2 = *piVar8;
        iVar13 = 0;
        while (iVar2 != iVar13) {
          iVar14 = *(int *)(piVar8[1] + iVar13 * 4);
          iVar13 = iVar13 + 1;
          if (*(int *)(iVar14 + 0x28) == 8) {
            *(undefined1 *)(iVar14 + 0x25) = 1;
          }
        }
      }
      *(undefined2 *)(*piVar12 + 0x110) = 0x100;
    }
  }
LAB_000b3488:
  iVar2 = (int)*(float *)(param_1 + 0x88) - uVar11;
  if (iVar2 < 0) {
    *(undefined4 *)(param_1 + 0x88) = 0;
    iVar2 = iVar2 + *(int *)(param_1 + 0x8c);
    if (iVar2 < 0) {
      *(undefined4 *)(param_1 + 0x8c) = 0;
      *(undefined1 *)(param_1 + 0x66) = 1;
      *(int *)(param_1 + 0x78) = iVar2 + *(int *)(param_1 + 0x78);
    }
    else {
      *(undefined1 *)(param_1 + 0x65) = 1;
      *(int *)(param_1 + 0x8c) = iVar2;
    }
  }
  else {
    uVar18 = VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    *(undefined1 *)(param_1 + 100) = 1;
    *(undefined4 *)(param_1 + 0x88) = uVar18;
  }
  iVar2 = *(int *)(param_1 + 0xd0);
  if (((iVar2 != 0) && (*(char *)(iVar2 + 0x3c) == '\0')) &&
     ((*(char *)(iVar2 + 0x3d) == '\0' && (*(char *)(iVar2 + 0x42) != '\0')))) {
    iVar14 = *(int *)(param_1 + 0x78);
    iVar13 = __aeabi_idiv(*(undefined4 *)(param_1 + 0x84),3);
    if (iVar13 <= iVar14) goto LAB_000b34f8;
    Level::almostKillWanted(*(int *)(iVar2 + 0x54));
  }
  iVar14 = *(int *)(param_1 + 0x78);
LAB_000b34f8:
  if (iVar14 < 1) {
    *(undefined4 *)(param_1 + 0x78) = 0;
    if (param_3 == 0) {
      iVar2 = *(int *)(param_1 + 0xd0);
      if ((((iVar2 != 0) && (*(char *)(iVar2 + 0x3c) == '\0')) && (*(char *)(iVar2 + 0x3d) == '\0'))
         && (iVar2 = Status::inBlackMarketSystem(), iVar2 == 0)) {
        if (*(char *)(*(int *)(param_1 + 0xd0) + 0x42) == '\0') {
          pSVar9 = (Standing *)Status::getStanding();
          Standing::applyKill(pSVar9,*(int *)(*(int *)(param_1 + 0xd0) + 0x28));
        }
        iVar2 = Status::getCampaignMission();
        iVar13 = *(int *)(param_1 + 0xd0);
        pSVar10 = (String *)GameText::getText(**(int **)(DAT_000b3690 + 0xb3572));
        iVar13 = AbyssEngine::String::Compare((String *)(iVar13 + 0x18),pSVar10);
        if (in_r3 == 0xb3 && iVar13 == 0) {
          Mission::getStatusValue();
          Mission::setStatusValue(iVar2);
        }
        if (*(char *)(*(int *)(param_1 + 0xd0) + 0x42) != '\0') {
          Level::killWanted(*(int *)(*(int *)(param_1 + 0xd0) + 0x54));
        }
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x44) = 1;
    }
  }
  fVar17 = DAT_000b3684;
  *(undefined1 *)(param_1 + 0xc1) = 1;
  *(float *)(param_1 + 0x60) = *(float *)(param_1 + 0x60) + fVar17;
  updateDamageRate((Player *)param_1);
  if ((*(int *)(param_1 + 0xd0) != 0) &&
     (iVar2 = *(int *)(*(int *)(param_1 + 0xd0) + 0x10), iVar2 != 0)) {
    this_00 = *(Player **)(iVar2 + 4);
    this_00[0xc2] = (Player)0x1;
    damage(this_00,uVar11);
    *(undefined1 *)(*(int *)(*(int *)(*(int *)(param_1 + 0xd0) + 0x10) + 4) + 0xc2) = 0;
  }
  return;
}

```
## target disasm
```
  000b31c0: push {r4,r5,r6,r7,lr}
  000b31c2: add r7,sp,#0xc
  000b31c4: push {r7,r8,r9,r10,r11}
  000b31c8: vpush {d7,d8,d9}
  000b31cc: mov r4,r0
  000b31ce: ldrb.w r0,[r0,#0xc2]
  000b31d2: mov r8,r3
  000b31d4: mov r11,r2
  000b31d6: mov r9,r1
  000b31d8: cmp r0,#0x0
  000b31da: beq.w 0x000b35ee
  000b31de: ldrb.w r0,[r4,#0xc0]
  000b31e2: cmp r0,#0x0
  000b31e4: beq.w 0x000b35ee
  000b31e8: ldr r0,[r4,#0x78]
  000b31ea: cmp r0,#0x1
  000b31ec: blt.w 0x000b35ee
  000b31f0: cmp.w r11,#0x0
  000b31f4: bne.w 0x000b3488
  000b31f8: ldr.w r0,[r4,#0xd0]
  000b31fc: cmp r0,#0x0
  000b31fe: beq.w 0x000b342a
  000b3202: ldrb.w r1,[r4,#0xec]
  000b3206: cbnz r1,0x000b324c
  000b3208: ldr r0,[r0,#0x28]
  000b320a: subs r0,#0x9
  000b320c: cmp r0,#0x2
  000b320e: bcc 0x000b324c
  000b3210: ldr r0,[0x000b3510]
  000b3212: add r0,pc
  000b3214: ldr r0,[r0,#0x0]
  000b3216: ldr r0,[r0,#0x0]
  000b3218: blx 0x00071a10
  000b321c: cbz r0,0x000b324c
  000b321e: ldrb.w r0,[r4,#0x5c]
  000b3222: cbz r0,0x000b322a
  000b3224: ldrb.w r0,[r4,#0xe0]
  000b3228: cbz r0,0x000b324c
  000b322a: ldr.w r0,[r4,#0xd0]
  000b322e: ldrb.w r1,[r0,#0x42]
  000b3232: cbz r1,0x000b3250
  000b3234: cmp.w r9,#0x1
  000b3238: blt.w 0x000b3488
  000b323c: ldr r1,[r4,#0x6c]
  000b323e: add r1,r9
  000b3240: str r1,[r4,#0x6c]
  000b3242: ldr r1,[r0,#0x48]
  000b3244: ldr r0,[r0,#0x54]
  000b3246: blx 0x00072ec8
  000b324a: b 0x000b3488
  000b324c: ldr.w r0,[r4,#0xd0]
  000b3250: cmp r0,#0x0
  000b3252: beq.w 0x000b342a
  000b3256: ldrb.w r1,[r4,#0xec]
  000b325a: cmp r1,#0x0
  000b325c: bne.w 0x000b342a
  000b3260: ldr r1,[r0,#0x28]
  000b3262: subs r1,#0x9
  000b3264: cmp r1,#0x2
  000b3266: bcc.w 0x000b342a
  000b326a: blx 0x00072ed4
  000b326e: cmp r0,#0x0
  000b3270: bne.w 0x000b342a
  000b3274: ldr r0,[0x000b3514]
  000b3276: add r0,pc
  000b3278: ldr r6,[r0,#0x0]
  000b327a: ldr r0,[r6,#0x0]
  000b327c: blx 0x00071a10
  000b3280: cmp r0,#0x0
  000b3282: beq.w 0x000b342a
  000b3286: ldrb.w r0,[r4,#0x5c]
  000b328a: cbz r0,0x000b3296
  000b328c: ldrb.w r0,[r4,#0xe0]
  000b3290: cmp r0,#0x0
  000b3292: beq.w 0x000b342a
  000b3296: ldr.w r0,[r4,#0xd0]
  000b329a: ldr r5,[r0,#0x28]
  000b329c: ldr r0,[r6,#0x0]
  000b329e: blx 0x00071a10
  000b32a2: blx 0x00071950
  000b32a6: cmp r5,r0
  000b32a8: beq 0x000b32c0
  000b32aa: ldr.w r0,[r4,#0xd0]
  000b32ae: ldr r5,[r0,#0x28]
  000b32b0: ldr r0,[r6,#0x0]
  000b32b2: blx 0x00071a10
  000b32b6: blx 0x00072f34
  000b32ba: cmp r5,r0
  000b32bc: bne.w 0x000b342a
  000b32c0: ldr r0,[r4,#0x6c]
  000b32c2: add r0,r9
  000b32c4: str r0,[r4,#0x6c]
  000b32c6: ldr r0,[r6,#0x0]
  000b32c8: blx 0x0007192c
  000b32cc: vldr.32 s0,[pc,#0x248]
  000b32d0: cmp r0,#0x0
  000b32d2: vldr.32 s2,[pc,#0x248]
  000b32d6: vmov.f32 s18,0x3f000000
  000b32da: vldr.32 s6,[r4,#0x84]
  000b32de: vldr.32 s4,[r4,#0x6c]
  000b32e2: it ne
  000b32e4: vmov.ne.f32 s0,s2
  000b32e8: vcvt.f32.s32 s2,s6
  000b32ec: vldr.32 s16,[pc,#0x38c]
  000b32f0: vmov.f32 s6,0x3e800000
  000b32f4: cmp r0,#0x0
  000b32f6: vcvt.f32.s32 s4,s4
  000b32fa: vmul.f32 s0,s0,s2
  000b32fe: vldr.32 s2,[pc,#0x380]
  000b3302: itt ne
  000b3304: vmov.ne.f32 s16,s2
  000b3308: vmov.ne.f32 s18,s6
  000b330c: vcmpe.f32 s0,s4
  000b3310: vmrs apsr,fpscr
  000b3314: bpl 0x000b3356
  000b3316: ldr.w r0,[r4,#0xd0]
  000b331a: ldr r1,[r0,#0x28]
  000b331c: ldr r0,[r0,#0x54]
  000b331e: blx 0x00072ee0
  000b3322: ldr r0,[r6,#0x0]
  000b3324: blx 0x00071a58
  000b3328: mov r10,r0
  000b332a: ldr r0,[r6,#0x0]
  000b332c: blx 0x00071b84
  000b3330: mov r5,r0
  000b3332: mov r0,r10
  000b3334: blx 0x00072f40
  000b3338: cmp r0,#0x0
  000b333a: blt 0x000b3356
  000b333c: mov r0,r10
  000b333e: blx 0x00072f40
  000b3342: ldr.w r1,[r4,#0xd0]
  000b3346: ldr r2,[r1,#0x28]
  000b3348: cmp r0,r2
  000b334a: itt eq
  000b334c: ldrb.eq.w r0,[r1,#0x42]
  000b3350: cmp.eq r0,#0x0
  000b3352: beq.w 0x000b3620
  000b3356: vldr.32 s0,[r4,#0x84]
  000b335a: vldr.32 s2,[r4,#0x6c]
  000b335e: vcvt.f32.s32 s0,s0
  000b3362: vcvt.f32.s32 s2,s2
  000b3366: vmul.f32 s4,s18,s0
  000b336a: vcmpe.f32 s4,s2
  000b336e: vmrs apsr,fpscr
  000b3372: bpl 0x000b340c
  000b3374: ldr r0,[r6,#0x0]
  000b3376: blx 0x00071a58
  000b337a: mov r10,r0
  000b337c: ldr r0,[r6,#0x0]
  000b337e: blx 0x00071b84
  000b3382: mov r5,r0
  000b3384: mov r0,r10
  000b3386: blx 0x00072f40
  000b338a: cmp r0,#0x0
  000b338c: blt 0x000b33f6
  000b338e: ldr.w r0,[r4,#0xd0]
  000b3392: ldr r1,[r0,#0x28]
  000b3394: cmp r1,#0x3
  000b3396: bgt 0x000b33f6
  000b3398: ldrb.w r0,[r0,#0x42]
  000b339c: cbnz r0,0x000b33f6
  000b339e: mov r0,r10
  000b33a0: movs r1,#0x1d
  000b33a2: blx 0x0007228c
  000b33a6: mov r1,r0
  000b33a8: mov r0,r10
  000b33aa: blx 0x00072c34
  000b33ae: ldr r0,[r6,#0x0]
  000b33b0: blx 0x00071b84
  000b33b4: mov r6,r0
  000b33b6: mov r0,r10
  000b33b8: blx 0x00072f40
  000b33bc: mov r1,r0
  000b33be: mov r0,r6
  000b33c0: movs r2,#0x64
  000b33c2: blx 0x00072f4c
  000b33c6: mov r0,r5
  000b33c8: mov.w r1,#0xffffffff
  000b33cc: blx 0x00072f58
  000b33d0: ldr.w r0,[r4,#0xd0]
  000b33d4: ldr r0,[r0,#0x54]
  000b33d6: blx 0x00072034
  000b33da: blx 0x00072f10
  000b33de: mov r5,r0
  000b33e0: ldr.w r0,[r4,#0xd0]
  000b33e4: ldr r0,[r0,#0x54]
  000b33e6: blx 0x00072034
  000b33ea: mov r2,r0
  000b33ec: mov r0,r5
  000b33ee: movs r1,#0x1f
  000b33f0: movs r3,#0x0
  000b33f2: blx 0x00072418
  000b33f6: vldr.32 s2,[r4,#0x6c]
  000b33fa: movs r0,#0x1
  000b33fc: vldr.32 s0,[r4,#0x84]
  000b3400: vcvt.f32.s32 s0,s0
  000b3404: vcvt.f32.s32 s2,s2
  000b3408: strb.w r0,[r4,#0xe0]
  000b340c: vmul.f32 s0,s16,s0
  000b3410: vcmpe.f32 s0,s2
  000b3414: vmrs apsr,fpscr
  000b3418: bpl 0x000b3488
  000b341a: ldr.w r0,[r4,#0xd0]
  000b341e: movs r2,#0x1
  000b3420: ldr r1,[r0,#0x28]
  000b3422: ldr r0,[r0,#0x54]
  000b3424: blx 0x00072eec
  000b3428: b 0x000b3488
  000b342a: ldr r0,[0x000b3688]
  000b342c: add r0,pc
  000b342e: ldr r5,[r0,#0x0]
  000b3430: ldr r0,[r5,#0x0]
  000b3432: blx 0x00071998
  000b3436: cbz r0,0x000b3488
  000b3438: ldr.w r0,[r4,#0xd0]
  000b343c: cbz r0,0x000b3488
  000b343e: ldr r1,[r0,#0x28]
  000b3440: cmp r1,#0x8
  000b3442: bne 0x000b3488
  000b3444: mov.w r10,#0x1
  000b3448: movs r1,#0x8
  000b344a: strb.w r10,[r4,#0xe0]
  000b344e: movs r2,#0x1
  000b3450: ldr r0,[r0,#0x54]
  000b3452: blx 0x00072eec
  000b3456: ldr.w r0,[r4,#0xd0]
  000b345a: ldr r0,[r0,#0x54]
  000b345c: blx 0x00071eb4
  000b3460: cbz r0,0x000b347e
  000b3462: ldr r1,[r0,#0x0]
  000b3464: movs r2,#0x0
  000b3466: b 0x000b347a
  000b3468: ldr r3,[r0,#0x4]
  000b346a: ldr.w r3,[r3,r2,lsl #0x2]
  000b346e: adds r2,#0x1
  000b3470: ldr r6,[r3,#0x28]
  000b3472: cmp r6,#0x8
  000b3474: it eq
  000b3476: strb.eq.w r10,[r3,#0x25]
  000b347a: cmp r1,r2
  000b347c: bne 0x000b3468
  000b347e: ldr r0,[r5,#0x0]
  000b3480: mov.w r1,#0x100
  000b3484: strh.w r1,[r0,#0x110]
  000b3488: vldr.32 s0,[r4,#0x88]
  000b348c: vcvt.s32.f32 s0,s0
  000b3490: vmov r0,s0
  000b3494: sub.w r0,r0,r9
  000b3498: cmp.w r0,#0xffffffff
  000b349c: ble 0x000b34b2
  000b349e: vmov s0,r0
  000b34a2: movs r0,#0x1
  000b34a4: vcvt.f32.s32 s0,s0
  000b34a8: strb.w r0,[r4,#0x64]
  000b34ac: vstr.32 s0,[r4,#0x88]
  000b34b0: b 0x000b34e0
  000b34b2: ldr.w r2,[r4,#0x8c]
  000b34b6: movs r1,#0x0
  000b34b8: str.w r1,[r4,#0x88]
  000b34bc: add r0,r2
  000b34be: cmp.w r0,#0xffffffff
  000b34c2: ble 0x000b34d0
  000b34c4: movs r1,#0x1
  000b34c6: strb.w r1,[r4,#0x65]
  000b34ca: str.w r0,[r4,#0x8c]
  000b34ce: b 0x000b34e0
  000b34d0: str.w r1,[r4,#0x8c]
  000b34d4: movs r2,#0x1
  000b34d6: ldr r1,[r4,#0x78]
  000b34d8: strb.w r2,[r4,#0x66]
  000b34dc: add r0,r1
  000b34de: str r0,[r4,#0x78]
  000b34e0: ldr.w r5,[r4,#0xd0]
  000b34e4: cbz r5,0x000b34f6
  000b34e6: ldrb.w r0,[r5,#0x3c]
  000b34ea: cbnz r0,0x000b34f6
  000b34ec: ldrb.w r0,[r5,#0x3d]
  000b34f0: cmp r0,#0x0
  000b34f2: beq.w 0x000b35fa
  000b34f6: ldr r6,[r4,#0x78]
  000b34f8: cmp r6,#0x0
  000b34fa: bgt 0x000b35ac
  000b34fc: movs r0,#0x0
  000b34fe: cmp.w r11,#0x0
  000b3502: str r0,[r4,#0x78]
  000b3504: beq 0x000b3520
  000b3506: movs r0,#0x1
  000b3508: strb.w r0,[r4,#0x44]
  000b350c: b 0x000b35ac
  000b3520: ldr.w r0,[r4,#0xd0]
  000b3524: cmp r0,#0x0
  000b3526: beq 0x000b35ac
  000b3528: ldrb.w r1,[r0,#0x3c]
  000b352c: cbnz r1,0x000b35ac
  000b352e: ldrb.w r0,[r0,#0x3d]
  000b3532: cbnz r0,0x000b35ac
  000b3534: ldr r0,[0x000b368c]
  000b3536: add r0,pc
  000b3538: ldr r5,[r0,#0x0]
  000b353a: ldr r0,[r5,#0x0]
  000b353c: blx 0x00071998
  000b3540: cbnz r0,0x000b35ac
  000b3542: ldr.w r0,[r4,#0xd0]
  000b3546: ldrb.w r0,[r0,#0x42]
  000b354a: cbnz r0,0x000b355c
  000b354c: ldr r0,[r5,#0x0]
  000b354e: blx 0x00071b84
  000b3552: ldr.w r1,[r4,#0xd0]
  000b3556: ldr r1,[r1,#0x28]
  000b3558: blx 0x00072f64
  000b355c: ldr r0,[r5,#0x0]
  000b355e: blx 0x0007285c
  000b3562: mov r6,r0
  000b3564: ldr r0,[0x000b3690]
  000b3566: mov.w r1,#0x680
  000b356a: ldr.w r5,[r4,#0xd0]
  000b356e: add r0,pc
  000b3570: ldr r0,[r0,#0x0]
  000b3572: ldr r0,[r0,#0x0]
  000b3574: blx 0x00072f70
  000b3578: mov r1,r0
  000b357a: add.w r0,r5,#0x18
  000b357e: blx 0x0006f664
  000b3582: cmp.w r8,#0xb3
  000b3586: it eq
  000b3588: cmp.eq r0,#0x0
  000b358a: bne 0x000b359a
  000b358c: mov r0,r6
  000b358e: blx 0x0007288c
  000b3592: adds r1,r0,#0x1
  000b3594: mov r0,r6
  000b3596: blx 0x00072898
  000b359a: ldr.w r0,[r4,#0xd0]
  000b359e: ldrb.w r1,[r0,#0x42]
  000b35a2: cbz r1,0x000b35ac
  000b35a4: ldr r1,[r0,#0x48]
  000b35a6: ldr r0,[r0,#0x54]
  000b35a8: blx 0x00072f7c
  000b35ac: vldr.32 s0,[pc,#0xd4]
  000b35b0: movs r0,#0x1
  000b35b2: vldr.32 s2,[r4,#0x60]
  000b35b6: strb.w r0,[r4,#0xc1]
  000b35ba: mov r0,r4
  000b35bc: vadd.f32 s0,s2,s0
  000b35c0: vstr.32 s0,[r4,#0x60]
  000b35c4: blx 0x00072df0
  000b35c8: ldr.w r0,[r4,#0xd0]
  000b35cc: cbz r0,0x000b35ee
  000b35ce: ldr r0,[r0,#0x10]
  000b35d0: cbz r0,0x000b35ee
  000b35d2: ldr r0,[r0,#0x4]
  000b35d4: movs r1,#0x1
  000b35d6: strb.w r1,[r0,#0xc2]
  000b35da: mov r1,r9
  000b35dc: blx 0x00072ab4
  000b35e0: ldr.w r0,[r4,#0xd0]
  000b35e4: movs r1,#0x0
  000b35e6: ldr r0,[r0,#0x10]
  000b35e8: ldr r0,[r0,#0x4]
  000b35ea: strb.w r1,[r0,#0xc2]
  000b35ee: vpop {d7,d8,d9}
  000b35f2: add sp,#0x4
  000b35f4: pop.w {r8,r9,r10,r11}
  000b35f8: pop {r4,r5,r6,r7,pc}
  000b35fa: ldrb.w r0,[r5,#0x42]
  000b35fe: cmp r0,#0x0
  000b3600: beq.w 0x000b34f6
  000b3604: ldr.w r0,[r4,#0x84]
  000b3608: movs r1,#0x3
  000b360a: ldr r6,[r4,#0x78]
  000b360c: blx 0x0007198c
  000b3610: cmp r6,r0
  000b3612: bge.w 0x000b34f8
  000b3616: ldr r1,[r5,#0x48]
  000b3618: ldr r0,[r5,#0x54]
  000b361a: blx 0x00072f88
  000b361e: b 0x000b34f6
  000b3620: mov r0,r10
  000b3622: movs r1,#0x1d
  000b3624: blx 0x0007228c
  000b3628: mov r1,r0
  000b362a: mov r0,r10
  000b362c: blx 0x00072c34
  000b3630: ldr r0,[r6,#0x0]
  000b3632: blx 0x00071b84
  000b3636: str r0,[sp,#0x4]
  000b3638: mov r0,r10
  000b363a: blx 0x00072f40
  000b363e: mov r1,r0
  000b3640: ldr r0,[sp,#0x4]
  000b3642: movs r2,#0x64
  000b3644: blx 0x00072f4c
  000b3648: mov r0,r5
  000b364a: mov.w r1,#0xffffffff
  000b364e: blx 0x00072f58
  000b3652: ldr.w r0,[r4,#0xd0]
  000b3656: ldr r0,[r0,#0x54]
  000b3658: blx 0x00072034
  000b365c: blx 0x00072f10
  000b3660: mov r5,r0
  000b3662: ldr.w r0,[r4,#0xd0]
  000b3666: ldr r0,[r0,#0x54]
  000b3668: blx 0x00072034
  000b366c: mov r2,r0
  000b366e: mov r0,r5
  000b3670: movs r1,#0x1f
  000b3672: movs r3,#0x0
  000b3674: blx 0x00072418
  000b3678: b 0x000b3356
```
