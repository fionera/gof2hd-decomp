# Level::createFighterTurrets
elf 0xbc2dc body 250
Sig: undefined __thiscall createFighterTurrets(Level * this)

## decompile
```c

/* Level::createFighterTurrets() */

void __thiscall Level::createFighterTurrets(Level *this)

{
  int *piVar1;
  int iVar2;
  uint *puVar3;
  PlayerTurret *this_00;
  int iVar4;
  Player *pPVar5;
  KIPlayer *pKVar6;
  uint uVar7;
  
  piVar1 = *(int **)(DAT_000cc3e8 + 0xcc2ec);
  iVar2 = *piVar1;
  puVar3 = *(uint **)(this + 0xf8);
  if (puVar3 != (uint *)0x0) {
    for (uVar7 = 0; uVar7 < *puVar3; uVar7 = uVar7 + 1) {
      pKVar6 = *(KIPlayer **)(puVar3[1] + uVar7 * 4);
      if (pKVar6 != (KIPlayer *)0x0) {
        if (*(int *)(pKVar6 + 0x7c) == 0x2d) {
          this_00 = (PlayerTurret *)createStaticObject(this,(Waypoint *)0x0,0x1a74,true);
          Player::setVulnerable(SUB41(*(undefined4 *)(this_00 + 4),0));
          pPVar5 = *(Player **)(this_00 + 4);
          iVar4 = Player::getMaxHitpoints();
          Player::setMaxHitpoints(pPVar5,iVar4);
          PlayerTurret::setHost(this_00,pKVar6,(Vector *)&stack0xffffffcc);
          *(PlayerTurret **)(pKVar6 + 0x10) = this_00;
          *(undefined4 *)(this_00 + 0x28) = 8;
        }
        else {
          if (*(int *)(pKVar6 + 0x7c) != 0x33) goto LAB_000cc3b2;
          this_00 = (PlayerTurret *)createStaticObject(this,(Waypoint *)0x0,0x1a74,true);
          Player::setVulnerable(SUB41(*(undefined4 *)(this_00 + 4),0));
          pPVar5 = *(Player **)(this_00 + 4);
          iVar4 = Player::getMaxHitpoints();
          Player::setMaxHitpoints(pPVar5,iVar4);
          PlayerTurret::setHost(this_00,pKVar6,(Vector *)&stack0xffffffcc);
          *(PlayerTurret **)(pKVar6 + 0x10) = this_00;
          *(undefined4 *)(this_00 + 0x28) = 0;
        }
        *(KIPlayer *)(this_00 + 0x74) = (KIPlayer)0x1;
        ArrayAdd<KIPlayer*>((KIPlayer *)this_00,*(Array **)(this + 0xf8));
      }
LAB_000cc3b2:
      puVar3 = *(uint **)(this + 0xf8);
    }
  }
  if (*piVar1 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000cc2dc: push {r4,r5,r6,r7,lr}
  000cc2de: add r7,sp,#0xc
  000cc2e0: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000cc2e4: mov r9,r0
  000cc2e6: ldr r0,[0x000cc3e8]
  000cc2e8: add r0,pc
  000cc2ea: ldr r0,[r0,#0x0]
  000cc2ec: str r0,[sp,#0x0]
  000cc2ee: ldr r0,[r0,#0x0]
  000cc2f0: str r0,[sp,#0x10]
  000cc2f2: ldr.w r0,[r9,#0xf8]
  000cc2f6: cmp r0,#0x0
  000cc2f8: beq 0x000cc3c0
  000cc2fa: add.w r8,sp,#0x4
  000cc2fe: mov.w r11,#0x0
  000cc302: mov.w r10,#0x0
  000cc306: b 0x000cc3ba
  000cc308: ldr r0,[r0,#0x4]
  000cc30a: ldr.w r6,[r0,r10,lsl #0x2]
  000cc30e: cmp r6,#0x0
  000cc310: beq 0x000cc3b2
  000cc312: ldr r0,[r6,#0x7c]
  000cc314: cmp r0,#0x2d
  000cc316: beq 0x000cc360
  000cc318: cmp r0,#0x33
  000cc31a: bne 0x000cc3b2
  000cc31c: mov r0,r9
  000cc31e: movs r1,#0x0
  000cc320: movw r2,#0x1a74
  000cc324: movs r3,#0x1
  000cc326: blx 0x00073f6c
  000cc32a: mov r5,r0
  000cc32c: ldr r0,[r0,#0x4]
  000cc32e: movs r1,#0x0
  000cc330: blx 0x0007294c
  000cc334: ldr r0,[r6,#0x4]
  000cc336: ldr r4,[r5,#0x4]
  000cc338: blx 0x00072910
  000cc33c: mov r1,r0
  000cc33e: mov r0,r4
  000cc340: blx 0x00073f78
  000cc344: ldr r0,[0x000cc3e4]
  000cc346: mov r1,r6
  000cc348: mov r2,r8
  000cc34a: strd r11,r0,[sp,#0x4]
  000cc34e: ldr r0,[0x000cc3e0]
  000cc350: str r0,[sp,#0xc]
  000cc352: mov r0,r5
  000cc354: blx 0x000741d0
  000cc358: str r5,[r6,#0x10]
  000cc35a: str.w r11,[r5,#0x28]
  000cc35e: b 0x000cc3a2
  000cc360: mov r0,r9
  000cc362: movs r1,#0x0
  000cc364: movw r2,#0x1a74
  000cc368: movs r3,#0x1
  000cc36a: blx 0x00073f6c
  000cc36e: mov r5,r0
  000cc370: ldr r0,[r0,#0x4]
  000cc372: movs r1,#0x0
  000cc374: blx 0x0007294c
  000cc378: ldr r0,[r6,#0x4]
  000cc37a: ldr r4,[r5,#0x4]
  000cc37c: blx 0x00072910
  000cc380: mov r1,r0
  000cc382: mov r0,r4
  000cc384: blx 0x00073f78
  000cc388: ldr r0,[0x000cc3dc]
  000cc38a: mov r1,r6
  000cc38c: mov r2,r8
  000cc38e: strd r11,r0,[sp,#0x4]
  000cc392: ldr r0,[0x000cc3d8]
  000cc394: str r0,[sp,#0xc]
  000cc396: mov r0,r5
  000cc398: blx 0x000741d0
  000cc39c: movs r0,#0x8
  000cc39e: str r5,[r6,#0x10]
  000cc3a0: str r0,[r5,#0x28]
  000cc3a2: movs r0,#0x1
  000cc3a4: strb.w r0,[r5,#0x74]
  000cc3a8: mov r0,r5
  000cc3aa: ldr.w r1,[r9,#0xf8]
  000cc3ae: blx 0x000740f8
  000cc3b2: ldr.w r0,[r9,#0xf8]
  000cc3b6: add.w r10,r10,#0x1
  000cc3ba: ldr r1,[r0,#0x0]
  000cc3bc: cmp r10,r1
  000cc3be: bcc 0x000cc308
  000cc3c0: ldr r0,[sp,#0x10]
  000cc3c2: ldr r1,[sp,#0x0]
  000cc3c4: ldr r1,[r1,#0x0]
  000cc3c6: subs r0,r1,r0
  000cc3c8: ittt eq
  000cc3ca: add.eq sp,#0x14
  000cc3cc: pop.eq.w {r8,r9,r10,r11}
  000cc3d0: pop.eq {r4,r5,r6,r7,pc}
  000cc3d2: blx 0x0006e824
```
