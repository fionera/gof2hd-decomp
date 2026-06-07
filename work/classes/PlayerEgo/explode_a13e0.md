# PlayerEgo::explode
elf 0xa13e0 body 172
Sig: undefined __thiscall explode(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::explode() */

void __thiscall PlayerEgo::explode(PlayerEgo *this)

{
  Explosion *this_00;
  int iVar1;
  code *pcVar2;
  undefined4 *puVar3;
  
  ParticleSystemManager::enableSystemEmit((int)this->m_pLevel->field_74,SUB41(this[0x15].field_8,0))
  ;
  if (this[3].field_20 == 0) {
    TargetFollowCamera::setActive((TargetFollowCamera *)this[3].field_1C,false);
    this_00 = operator_new(0x68);
    Explosion::Explosion(this_00,0);
    iVar1 = this->m_pPlayer;
    this[3].field_20 = (int)this_00;
    Player::setActive(SUB41(iVar1,0));
    puVar3 = *(undefined4 **)(DAT_000b149c + 0xb1430);
    pcVar2 = *(code **)(DAT_000b14a0 + 0xb1432);
    (*pcVar2)((undefined4 *)*puVar3,*(undefined4 *)*puVar3);
    (*pcVar2)(*puVar3,this->field_1C);
    (*pcVar2)(*puVar3,0x1b);
    (*pcVar2)(*puVar3,0x23);
    (*pcVar2)(*puVar3,0x8d5);
    (*pcVar2)(*puVar3,0x8d4);
    (*pcVar2)(*puVar3,0x8cc);
    (*pcVar2)(*puVar3,0x447);
    (*pcVar2)(*puVar3,0x448);
    (*pcVar2)(*puVar3,0x449);
    (*(code *)(DAT_001abbe4 + 0x1abbe8))(this,0);
    return;
  }
  return;
}

```

## target disasm
```
  000b13e0: push {r4,r5,r6,r7,lr}
  000b13e2: add r7,sp,#0xc
  000b13e4: push.w r11
  000b13e8: mov r4,r0
  000b13ea: ldr r0,[r0,#0xc]
  000b13ec: ldr.w r1,[r4,#0x2fc]
  000b13f0: movs r2,#0x1
  000b13f2: ldr r0,[r0,#0x74]
  000b13f4: blx 0x000723c4
  000b13f8: ldr.w r0,[r4,#0x8c]
  000b13fc: cbz r0,0x000b1404
  000b13fe: pop.w r11
  000b1402: pop {r4,r5,r6,r7,pc}
  000b1404: ldr.w r0,[r4,#0x88]
  000b1408: movs r1,#0x0
  000b140a: blx 0x00072acc
  000b140e: movs r0,#0x68
  000b1410: blx 0x0006eb24
  000b1414: mov r5,r0
  000b1416: movs r1,#0x0
  000b1418: blx 0x000727f0
  000b141c: ldr r0,[r4,#0x0]
  000b141e: movs r1,#0x0
  000b1420: str.w r5,[r4,#0x8c]
  000b1424: blx 0x00072580
  000b1428: ldr r0,[0x000b149c]
  000b142a: ldr r2,[0x000b14a0]
  000b142c: add r0,pc
  000b142e: add r2,pc
  000b1430: ldr r6,[r0,#0x0]
  000b1432: ldr r5,[r2,#0x0]
  000b1434: ldr r0,[r6,#0x0]
  000b1436: ldr r1,[r0,#0x0]
  000b1438: blx r5
  000b143a: ldr r1,[r4,#0x1c]
  000b143c: ldr r0,[r6,#0x0]
  000b143e: blx r5
  000b1440: ldr r0,[r6,#0x0]
  000b1442: movs r1,#0x1b
  000b1444: blx r5
  000b1446: ldr r0,[r6,#0x0]
  000b1448: movs r1,#0x23
  000b144a: blx r5
  000b144c: ldr r0,[r6,#0x0]
  000b144e: movw r1,#0x8d5
  000b1452: blx r5
  000b1454: ldr r0,[r6,#0x0]
  000b1456: movw r1,#0x8d4
  000b145a: blx r5
  000b145c: ldr r0,[r6,#0x0]
  000b145e: movw r1,#0x8cc
  000b1462: blx r5
  000b1464: ldr r0,[r6,#0x0]
  000b1466: movw r1,#0x447
  000b146a: blx r5
  000b146c: ldr r0,[r6,#0x0]
  000b146e: mov.w r1,#0x448
  000b1472: blx r5
  000b1474: ldr r0,[r6,#0x0]
  000b1476: movw r1,#0x449
  000b147a: blx r5
  000b147c: mov r0,r4
  000b147e: movs r1,#0x0
  000b1480: pop.w r11
  000b1484: pop.w {r4,r5,r6,r7,lr}
  000b1488: b.w 0x001abbd8
```
