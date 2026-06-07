# PlayerEgo::tryToStartEmergencySystem
elf 0xa106c body 132
Sig: undefined __thiscall tryToStartEmergencySystem(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::tryToStartEmergencySystem() */

undefined4 __thiscall PlayerEgo::tryToStartEmergencySystem(PlayerEgo *this)

{
  int iVar1;
  Ship *this_00;
  Item *pIVar2;
  undefined4 uVar3;
  float fVar4;
  
  if ((this[4].field_1C == 0) || (this[0x15].field_18 != (int *)0x0)) {
    uVar3 = 0;
  }
  else {
    iVar1 = Player::getHitpoints();
    uVar3 = 0;
    if (iVar1 < 2) {
      Player::setHitpoints(this->m_pPlayer);
      iVar1 = this->m_pPlayer;
      this[0x15].field_18 = (int *)this[0x15].field_1C;
      Player::setVulnerable(SUB41(iVar1,0));
      this_00 = (Ship *)Status::getShip();
      iVar1 = Status::getShip();
      pIVar2 = (Item *)Ship::getFirstEquipmentOfSort(iVar1);
      fVar4 = (float)Ship::removeEquipment(this_00,pIVar2);
      FModSound::play(**(int **)(DAT_000b10f4 + 0xb10e4),(Vector *)0x45b,(Vector *)0x0,fVar4);
      uVar3 = 1;
    }
  }
  return uVar3;
}

```

## target disasm
```
  000b106c: push {r4,r5,r6,r7,lr}
  000b106e: add r7,sp,#0xc
  000b1070: push.w r11
  000b1074: sub sp,#0x8
  000b1076: mov r5,r0
  000b1078: ldr.w r0,[r0,#0xac]
  000b107c: cbz r0,0x000b1084
  000b107e: ldr.w r0,[r5,#0x30c]
  000b1082: cbz r0,0x000b1090
  000b1084: movs r4,#0x0
  000b1086: mov r0,r4
  000b1088: add sp,#0x8
  000b108a: pop.w r11
  000b108e: pop {r4,r5,r6,r7,pc}
  000b1090: ldr r0,[r5,#0x0]
  000b1092: blx 0x000724f0
  000b1096: movs r4,#0x0
  000b1098: cmp r0,#0x1
  000b109a: bgt 0x000b1086
  000b109c: ldr r0,[r5,#0x0]
  000b109e: movs r1,#0x1
  000b10a0: blx 0x000727e4
  000b10a4: ldr r0,[r5,#0x0]
  000b10a6: ldr.w r1,[r5,#0x310]
  000b10aa: str.w r1,[r5,#0x30c]
  000b10ae: movs r1,#0x0
  000b10b0: blx 0x0007294c
  000b10b4: ldr r0,[0x000b10f0]
  000b10b6: add r0,pc
  000b10b8: ldr r6,[r0,#0x0]
  000b10ba: ldr r0,[r6,#0x0]
  000b10bc: blx 0x00071a58
  000b10c0: mov r5,r0
  000b10c2: ldr r0,[r6,#0x0]
  000b10c4: blx 0x00071a58
  000b10c8: movs r1,#0x1b
  000b10ca: blx 0x0007228c
  000b10ce: mov r1,r0
  000b10d0: mov r0,r5
  000b10d2: blx 0x00072c34
  000b10d6: ldr r0,[0x000b10f4]
  000b10d8: movw r1,#0x45b
  000b10dc: movs r2,#0x0
  000b10de: movs r3,#0x0
  000b10e0: add r0,pc
  000b10e2: str r4,[sp,#0x0]
  000b10e4: ldr r0,[r0,#0x0]
  000b10e6: ldr r0,[r0,#0x0]
  000b10e8: blx 0x00071548
  000b10ec: movs r4,#0x1
  000b10ee: b 0x000b1086
```
