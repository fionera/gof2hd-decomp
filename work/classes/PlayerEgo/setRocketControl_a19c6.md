# PlayerEgo::setRocketControl
elf 0xa19c6 body 82
Sig: undefined __thiscall setRocketControl(PlayerEgo * this, Gun * param_1, AEGeometry * param_2)

## decompile
```c

/* PlayerEgo::setRocketControl(Gun*, AEGeometry*) */

void __thiscall PlayerEgo::setRocketControl(PlayerEgo *this,Gun *param_1,AEGeometry *param_2)

{
  SingleLevel *pSVar1;
  Matrix *pMVar2;
  int *piVar3;
  ParticleSystemManager *this_00;
  
  pSVar1 = this->m_pLevel;
  this[0xb].field_8 = (int *)param_1;
  piVar3 = pSVar1->field_64;
  this_00 = (ParticleSystemManager *)pSVar1->field_88;
  if (param_1 == (Gun *)0x0) {
    ParticleSystemManager::systemSetMatrix(this_00,(int)piVar3,(Matrix *)(this->m_pPlayer + 4));
    this[0xb].m_pLevel = (SingleLevel *)0x0;
    return;
  }
  pMVar2 = (Matrix *)AEGeometry::getReferenceMatrix();
  ParticleSystemManager::systemSetMatrix(this_00,(int)piVar3,pMVar2);
  (*(code *)(&UNK_001abc28 + DAT_001abc24))(this,0);
  return;
}

```

## target disasm
```
  000b19c6: push {r4,r5,r6,r7,lr}
  000b19c8: add r7,sp,#0xc
  000b19ca: push.w r11
  000b19ce: mov r4,r0
  000b19d0: ldr r0,[r0,#0xc]
  000b19d2: str.w r1,[r4,#0x194]
  000b19d6: cmp r1,#0x0
  000b19d8: ldr r5,[r0,#0x64]
  000b19da: ldr.w r6,[r0,#0x88]
  000b19de: beq 0x000b1a00
  000b19e0: mov r0,r2
  000b19e2: blx 0x00072d00
  000b19e6: mov r2,r0
  000b19e8: mov r0,r6
  000b19ea: mov r1,r5
  000b19ec: blx 0x00072d0c
  000b19f0: mov r0,r4
  000b19f2: movs r1,#0x0
  000b19f4: pop.w r11
  000b19f8: pop.w {r4,r5,r6,r7,lr}
  000b19fc: b.w 0x001abc18
  000b1a00: ldr r0,[r4,#0x0]
  000b1a02: mov r1,r5
  000b1a04: adds r2,r0,#0x4
  000b1a06: mov r0,r6
  000b1a08: blx 0x00072d0c
  000b1a0c: movs r0,#0x0
  000b1a0e: str.w r0,[r4,#0x198]
  000b1a12: pop.w r11
  000b1a16: pop {r4,r5,r6,r7,pc}
```
