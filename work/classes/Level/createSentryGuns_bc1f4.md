# Level::createSentryGuns
elf 0xbc1f4 body 208
Sig: undefined __thiscall createSentryGuns(Level * this)

## decompile
```c

/* Level::createSentryGuns() */

void __thiscall Level::createSentryGuns(Level *this)

{
  undefined4 uVar1;
  int iVar2;
  Array<KIPlayer*> *pAVar3;
  undefined4 uVar4;
  uint uVar5;
  KIPlayer *pKVar6;
  
  iVar2 = Status::getShip();
  iVar2 = Ship::getFirstEquipmentOfSort(iVar2);
  if (iVar2 != 0) {
    pAVar3 = operator_new(0xc);
    Array<KIPlayer*>::Array(pAVar3);
    *(Array<KIPlayer*> **)(this + 0xb0) = pAVar3;
    ArraySetLength<KIPlayer*>(9,(Array *)pAVar3);
    if (*(int *)(this + 0xf8) == 0) {
      pAVar3 = operator_new(0xc);
      Array<KIPlayer*>::Array(pAVar3);
      *(Array<KIPlayer*> **)(this + 0xf8) = pAVar3;
    }
    uVar1 = DAT_000cc2d4;
    for (uVar5 = 0; uVar5 < **(uint **)(this + 0xb0); uVar5 = uVar5 + 1) {
      iVar2 = __aeabi_uidiv(uVar5,3);
      uVar4 = createStaticObject(this,(Waypoint *)0x0,iVar2 + 0x49c0,true);
      *(undefined4 *)(*(int *)(*(int *)(this + 0xb0) + 4) + uVar5 * 4) = uVar4;
      pKVar6 = *(KIPlayer **)(*(int *)(*(int *)(this + 0xb0) + 4) + uVar5 * 4);
      Player::setRadius(*(Player **)(pKVar6 + 4),800);
      Player::setAlwaysFriend(*(Player **)(pKVar6 + 4),true);
      Player::setMaxHitpoints(*(Player **)(pKVar6 + 4),100);
      (**(code **)(*(int *)pKVar6 + 0x48))(pKVar6,uVar1,uVar1,uVar1);
      KIPlayer::setActive(SUB41(pKVar6,0));
      ArrayAdd<KIPlayer*>(pKVar6,*(Array **)(this + 0xf8));
    }
  }
  return;
}

```
## target disasm
```
  000cc1f4: push {r4,r5,r6,r7,lr}
  000cc1f6: add r7,sp,#0xc
  000cc1f8: push {r8,r9,r10}
  000cc1fc: mov r9,r0
  000cc1fe: ldr r0,[0x000cc2d8]
  000cc200: add r0,pc
  000cc202: ldr r0,[r0,#0x0]
  000cc204: ldr r0,[r0,#0x0]
  000cc206: blx 0x00071a58
  000cc20a: movs r1,#0x27
  000cc20c: blx 0x0007228c
  000cc210: cmp r0,#0x0
  000cc212: beq 0x000cc2be
  000cc214: movs r0,#0xc
  000cc216: blx 0x0006eb24
  000cc21a: mov r5,r0
  000cc21c: blx 0x00073d08
  000cc220: movs r0,#0x9
  000cc222: mov r1,r5
  000cc224: str.w r5,[r9,#0xb0]
  000cc228: blx 0x00073d14
  000cc22c: ldr.w r0,[r9,#0xf8]
  000cc230: cbnz r0,0x000cc242
  000cc232: movs r0,#0xc
  000cc234: blx 0x0006eb24
  000cc238: mov r5,r0
  000cc23a: blx 0x00073d08
  000cc23e: str.w r5,[r9,#0xf8]
  000cc242: ldr r0,[0x000cc2d4]
  000cc244: movs r5,#0x0
  000cc246: movw r8,#0x49c0
  000cc24a: mov r10,r0
  000cc24c: b 0x000cc2b4
  000cc24e: mov r0,r5
  000cc250: movs r1,#0x3
  000cc252: blx 0x0006ec2c
  000cc256: add.w r2,r0,r8
  000cc25a: mov r0,r9
  000cc25c: movs r1,#0x0
  000cc25e: movs r3,#0x1
  000cc260: blx 0x00073f6c
  000cc264: ldr.w r1,[r9,#0xb0]
  000cc268: ldr r1,[r1,#0x4]
  000cc26a: str.w r0,[r1,r5,lsl #0x2]
  000cc26e: mov.w r1,#0x320
  000cc272: ldr.w r0,[r9,#0xb0]
  000cc276: ldr r0,[r0,#0x4]
  000cc278: ldr.w r6,[r0,r5,lsl #0x2]
  000cc27c: ldr r0,[r6,#0x4]
  000cc27e: blx 0x000730d8
  000cc282: ldr r0,[r6,#0x4]
  000cc284: movs r1,#0x1
  000cc286: blx 0x00073fb4
  000cc28a: ldr r0,[r6,#0x4]
  000cc28c: movs r1,#0x64
  000cc28e: blx 0x00073f78
  000cc292: ldr r0,[r6,#0x0]
  000cc294: mov r1,r10
  000cc296: mov r2,r10
  000cc298: mov r3,r10
  000cc29a: ldr r4,[r0,#0x48]
  000cc29c: mov r0,r6
  000cc29e: blx r4
  000cc2a0: mov r0,r6
  000cc2a2: movs r1,#0x0
  000cc2a4: blx 0x000732f4
  000cc2a8: ldr.w r1,[r9,#0xf8]
  000cc2ac: mov r0,r6
  000cc2ae: blx 0x000740f8
  000cc2b2: adds r5,#0x1
  000cc2b4: ldr.w r0,[r9,#0xb0]
  000cc2b8: ldr r0,[r0,#0x0]
  000cc2ba: cmp r5,r0
  000cc2bc: bcc 0x000cc24e
  000cc2be: pop.w {r8,r9,r10}
  000cc2c2: pop {r4,r5,r6,r7,pc}
```
