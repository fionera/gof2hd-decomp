# PlayerEgo::revive
elf 0xa14a4 body 230
Sig: undefined __thiscall revive(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::revive() */

void __thiscall PlayerEgo::revive(PlayerEgo *this)

{
  void *pvVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  Player *this_00;
  int *piVar5;
  float fVar6;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 local_20;
  int local_1c;
  
  piVar5 = *(int **)(DAT_000b1590 + 0xb14ba);
  local_1c = *piVar5;
  ParticleSystemManager::enableSystemEmit((int)this->m_pLevel->field_74,SUB41(this[0x15].field_8,0))
  ;
  ParticleSystemManager::enableSystemRender
            ((int)this->m_pLevel->field_74,SUB41(this[0x15].field_8,0));
  if ((Explosion *)this[3].field_20 != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion((Explosion *)this[3].field_20);
    operator_delete(pvVar1);
  }
  this[3].field_20 = 0;
  TargetFollowCamera::setActive((TargetFollowCamera *)this[3].field_1C,true);
  fVar6 = (float)Player::setActive(SUB41(this->m_pPlayer,0));
  piVar3 = *(int **)(DAT_000b1594 + 0xb150a);
  puVar2 = (undefined4 *)*piVar3;
  fVar6 = (float)FModSound::play((int)puVar2,(Vector *)*puVar2,(Vector *)0x0,fVar6);
  FModSound::play(*piVar3,(Vector *)this->field_1C,(Vector *)0x0,fVar6);
  setExhaustVisible(this,true);
  iVar4 = this->m_pPlayer;
  Player::getMaxHitpoints();
  Player::setHitpoints(iVar4);
  this_00 = (Player *)this->m_pPlayer;
  Player::getMaxArmorHP(this_00);
  Player::setArmorHP((int)this_00);
  setPosition(extraout_s0,extraout_s1,extraout_s2);
  local_28 = 0;
  uStack_24 = 0;
  local_20 = 0x3f800000;
  AEGeometry::setDirection((Vector *)this->field_8,(Vector *)&local_28);
  this[0x15].field_4 = (int *)0x0;
  if (*piVar5 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b14a4: push {r4,r5,r6,r7,lr}
  000b14a6: add r7,sp,#0xc
  000b14a8: push.w r8
  000b14ac: sub sp,#0x20
  000b14ae: mov r4,r0
  000b14b0: ldr r0,[0x000b1590]
  000b14b2: movs r2,#0x0
  000b14b4: movs r6,#0x0
  000b14b6: add r0,pc
  000b14b8: ldr.w r8,[r0,#0x0]
  000b14bc: ldr.w r0,[r8,#0x0]
  000b14c0: str r0,[sp,#0x1c]
  000b14c2: ldr r0,[r4,#0xc]
  000b14c4: ldr.w r1,[r4,#0x2fc]
  000b14c8: ldr r0,[r0,#0x74]
  000b14ca: blx 0x000723c4
  000b14ce: ldr r0,[r4,#0xc]
  000b14d0: movs r2,#0x0
  000b14d2: ldr.w r1,[r4,#0x2fc]
  000b14d6: ldr r0,[r0,#0x74]
  000b14d8: blx 0x00072a24
  000b14dc: ldr.w r0,[r4,#0x8c]
  000b14e0: cbz r0,0x000b14ea
  000b14e2: blx 0x00072334
  000b14e6: blx 0x0006eb48
  000b14ea: ldr.w r0,[r4,#0x88]
  000b14ee: movs r1,#0x1
  000b14f0: str.w r6,[r4,#0x8c]
  000b14f4: blx 0x00072acc
  000b14f8: ldr r0,[r4,#0x0]
  000b14fa: movs r1,#0x1
  000b14fc: blx 0x00072580
  000b1500: ldr r0,[0x000b1594]
  000b1502: movs r2,#0x0
  000b1504: movs r3,#0x0
  000b1506: add r0,pc
  000b1508: ldr r5,[r0,#0x0]
  000b150a: ldr r0,[r5,#0x0]
  000b150c: ldr r1,[r0,#0x0]
  000b150e: str r6,[sp,#0x0]
  000b1510: blx 0x00071548
  000b1514: ldr r1,[r4,#0x1c]
  000b1516: movs r2,#0x0
  000b1518: ldr r0,[r5,#0x0]
  000b151a: movs r3,#0x0
  000b151c: str r6,[sp,#0x0]
  000b151e: blx 0x00071548
  000b1522: mov r0,r4
  000b1524: movs r1,#0x1
  000b1526: blx 0x00072af0
  000b152a: ldr r5,[r4,#0x0]
  000b152c: mov r0,r5
  000b152e: blx 0x00072910
  000b1532: mov r1,r0
  000b1534: mov r0,r5
  000b1536: blx 0x000727e4
  000b153a: ldr r5,[r4,#0x0]
  000b153c: mov r0,r5
  000b153e: blx 0x00072934
  000b1542: mov r1,r0
  000b1544: mov r0,r5
  000b1546: blx 0x00072c7c
  000b154a: ldr r3,[0x000b158c]
  000b154c: mov r0,r4
  000b154e: movs r1,#0x0
  000b1550: movs r2,#0x0
  000b1552: blx 0x000725bc
  000b1556: ldr r0,[r4,#0x8]
  000b1558: mov.w r1,#0x3f800000
  000b155c: strd r6,r6,[sp,#0x10]
  000b1560: add r2,sp,#0x4
  000b1562: str r1,[sp,#0x18]
  000b1564: strd r6,r1,[sp,#0x4]
  000b1568: add r1,sp,#0x10
  000b156a: str r6,[sp,#0xc]
  000b156c: blx 0x000726ac
  000b1570: str.w r6,[r4,#0x2f8]
  000b1574: ldr r0,[sp,#0x1c]
  000b1576: ldr.w r1,[r8,#0x0]
  000b157a: subs r0,r1,r0
  000b157c: ittt eq
  000b157e: add.eq sp,#0x20
  000b1580: pop.eq.w r8
  000b1584: pop.eq {r4,r5,r6,r7,pc}
  000b1586: blx 0x0006e824
```
