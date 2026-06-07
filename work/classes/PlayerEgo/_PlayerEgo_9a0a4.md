# PlayerEgo::~PlayerEgo
elf 0x9a0a4 body 334
Sig: undefined __thiscall ~PlayerEgo(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::~PlayerEgo() */

PlayerEgo * __thiscall PlayerEgo::~PlayerEgo(PlayerEgo *this)

{
  void *pvVar1;
  AEGeometry *pAVar2;
  Route *this_00;
  TractorBeam *this_01;
  MiningGame *this_02;
  Explosion *pEVar3;
  EaseInOutMatrix *this_03;
  Array *pAVar4;
  
  if ((Player *)this->m_pPlayer != (Player *)0x0) {
    pvVar1 = (void *)Player::~Player((Player *)this->m_pPlayer);
    operator_delete(pvVar1);
  }
  this->m_pPlayer = 0;
  if ((AEGeometry *)this->field_4 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->field_4);
    operator_delete(pvVar1);
  }
  this->field_4 = (int *)0x0;
  if ((AEGeometry *)this->field_8 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->field_8);
    operator_delete(pvVar1);
  }
  pAVar2 = (AEGeometry *)this[6].field_4;
  this->field_8 = (int *)0x0;
  if (pAVar2 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(pAVar2);
    operator_delete(pvVar1);
  }
  this[6].field_4 = (int *)0x0;
  if ((AEGeometry *)this[1].field_4 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[1].field_4);
    operator_delete(pvVar1);
  }
  this_00 = (Route *)this[7].m_pPlayer;
  this[1].field_4 = (int *)0x0;
  if (this_00 != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(this_00);
    operator_delete(pvVar1);
  }
  pAVar2 = (AEGeometry *)this[10].field_10;
  this[7].m_pPlayer = 0;
  if (pAVar2 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(pAVar2);
    operator_delete(pvVar1);
  }
  this[10].field_10 = (int *)0x0;
  if ((AEGeometry *)this[10].field_14 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[10].field_14);
    operator_delete(pvVar1);
  }
  pAVar2 = (AEGeometry *)this[1].field_8;
  this[10].field_14 = (int *)0x0;
  if (pAVar2 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(pAVar2);
    operator_delete(pvVar1);
  }
  this[1].field_8 = (int *)0x0;
  if (this[1].m_pLevel != (SingleLevel *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[1].m_pLevel);
    operator_delete(pvVar1);
  }
  this[1].m_pLevel = (SingleLevel *)0x0;
  if ((AEGeometry *)this[1].field_10 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[1].field_10);
    operator_delete(pvVar1);
  }
  this[1].field_10 = (int *)0x0;
  if ((AEGeometry *)this[0xb].field_10 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[0xb].field_10);
    operator_delete(pvVar1);
  }
  this_01 = (TractorBeam *)this[0xc].field_4;
  this[0xb].field_10 = (int *)0x0;
  if (this_01 != (TractorBeam *)0x0) {
    pvVar1 = (void *)TractorBeam::~TractorBeam(this_01);
    operator_delete(pvVar1);
  }
  this_02 = (MiningGame *)this[0xd].field_10;
  this[0xc].field_4 = (int *)0x0;
  if (this_02 != (MiningGame *)0x0) {
    pvVar1 = (void *)MiningGame::~MiningGame(this_02);
    operator_delete(pvVar1);
  }
  pEVar3 = (Explosion *)this[3].field_20;
  this[0xd].field_10 = (int *)0x0;
  if (pEVar3 != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(pEVar3);
    operator_delete(pvVar1);
  }
  pEVar3 = (Explosion *)this[4].m_pPlayer;
  this[3].field_20 = 0;
  if (pEVar3 != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion(pEVar3);
    operator_delete(pvVar1);
  }
  this_03 = (EaseInOutMatrix *)this[0x17].field_1C;
  this[4].m_pPlayer = 0;
  if (this_03 != (EaseInOutMatrix *)0x0) {
    pvVar1 = (void *)AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix(this_03);
    operator_delete(pvVar1);
  }
  pAVar4 = (Array *)this[0xc].field_8;
  this[0x17].field_1C = 0;
  if (pAVar4 != (Array *)0x0) {
    ArrayReleaseClasses<RepairBeam*>(pAVar4);
    if ((Array<RepairBeam*> *)this[0xc].field_8 != (Array<RepairBeam*> *)0x0) {
      pvVar1 = (void *)Array<RepairBeam*>::~Array((Array<RepairBeam*> *)this[0xc].field_8);
      operator_delete(pvVar1);
    }
  }
  this[0xc].field_8 = (int *)0x0;
  return this;
}

```

## target disasm
```
  000aa0a4: push {r4,r5,r7,lr}
  000aa0a6: add r7,sp,#0x8
  000aa0a8: mov r4,r0
  000aa0aa: ldr r0,[r0,#0x0]
  000aa0ac: cbz r0,0x000aa0b6
  000aa0ae: blx 0x00072304
  000aa0b2: blx 0x0006eb48
  000aa0b6: ldr r0,[r4,#0x4]
  000aa0b8: movs r5,#0x0
  000aa0ba: str r5,[r4,#0x0]
  000aa0bc: cbz r0,0x000aa0c6
  000aa0be: blx 0x00071fc8
  000aa0c2: blx 0x0006eb48
  000aa0c6: ldr r0,[r4,#0x8]
  000aa0c8: str r5,[r4,#0x4]
  000aa0ca: cbz r0,0x000aa0d4
  000aa0cc: blx 0x00071fc8
  000aa0d0: blx 0x0006eb48
  000aa0d4: ldr.w r0,[r4,#0xdc]
  000aa0d8: movs r5,#0x0
  000aa0da: str r5,[r4,#0x8]
  000aa0dc: cbz r0,0x000aa0e6
  000aa0de: blx 0x00071fc8
  000aa0e2: blx 0x0006eb48
  000aa0e6: ldr r0,[r4,#0x28]
  000aa0e8: str.w r5,[r4,#0xdc]
  000aa0ec: cbz r0,0x000aa0f6
  000aa0ee: blx 0x00071fc8
  000aa0f2: blx 0x0006eb48
  000aa0f6: ldr.w r0,[r4,#0xfc]
  000aa0fa: movs r5,#0x0
  000aa0fc: str r5,[r4,#0x28]
  000aa0fe: cbz r0,0x000aa108
  000aa100: blx 0x00072310
  000aa104: blx 0x0006eb48
  000aa108: ldr.w r0,[r4,#0x178]
  000aa10c: str.w r5,[r4,#0xfc]
  000aa110: cbz r0,0x000aa11a
  000aa112: blx 0x00071fc8
  000aa116: blx 0x0006eb48
  000aa11a: ldr.w r0,[r4,#0x17c]
  000aa11e: movs r5,#0x0
  000aa120: str.w r5,[r4,#0x178]
  000aa124: cbz r0,0x000aa12e
  000aa126: blx 0x00071fc8
  000aa12a: blx 0x0006eb48
  000aa12e: ldr r0,[r4,#0x2c]
  000aa130: str.w r5,[r4,#0x17c]
  000aa134: cbz r0,0x000aa13e
  000aa136: blx 0x00071fc8
  000aa13a: blx 0x0006eb48
  000aa13e: ldr r0,[r4,#0x30]
  000aa140: movs r5,#0x0
  000aa142: str r5,[r4,#0x2c]
  000aa144: cbz r0,0x000aa14e
  000aa146: blx 0x00071fc8
  000aa14a: blx 0x0006eb48
  000aa14e: ldr r0,[r4,#0x34]
  000aa150: str r5,[r4,#0x30]
  000aa152: cbz r0,0x000aa15c
  000aa154: blx 0x00071fc8
  000aa158: blx 0x0006eb48
  000aa15c: ldr.w r0,[r4,#0x19c]
  000aa160: movs r5,#0x0
  000aa162: str r5,[r4,#0x34]
  000aa164: cbz r0,0x000aa16e
  000aa166: blx 0x00071fc8
  000aa16a: blx 0x0006eb48
  000aa16e: ldr.w r0,[r4,#0x1b4]
  000aa172: str.w r5,[r4,#0x19c]
  000aa176: cbz r0,0x000aa180
  000aa178: blx 0x0007231c
  000aa17c: blx 0x0006eb48
  000aa180: ldr.w r0,[r4,#0x1e4]
  000aa184: movs r5,#0x0
  000aa186: str.w r5,[r4,#0x1b4]
  000aa18a: cbz r0,0x000aa194
  000aa18c: blx 0x00072328
  000aa190: blx 0x0006eb48
  000aa194: ldr.w r0,[r4,#0x8c]
  000aa198: str.w r5,[r4,#0x1e4]
  000aa19c: cbz r0,0x000aa1a6
  000aa19e: blx 0x00072334
  000aa1a2: blx 0x0006eb48
  000aa1a6: ldr.w r0,[r4,#0x90]
  000aa1aa: movs r5,#0x0
  000aa1ac: str.w r5,[r4,#0x8c]
  000aa1b0: cbz r0,0x000aa1ba
  000aa1b2: blx 0x00072334
  000aa1b6: blx 0x0006eb48
  000aa1ba: ldr.w r0,[r4,#0x358]
  000aa1be: str.w r5,[r4,#0x90]
  000aa1c2: cbz r0,0x000aa1cc
  000aa1c4: blx 0x00072340
  000aa1c8: blx 0x0006eb48
  000aa1cc: ldr.w r0,[r4,#0x1b8]
  000aa1d0: movs r5,#0x0
  000aa1d2: str.w r5,[r4,#0x358]
  000aa1d6: cbz r0,0x000aa1ea
  000aa1d8: blx 0x0007234c
  000aa1dc: ldr.w r0,[r4,#0x1b8]
  000aa1e0: cbz r0,0x000aa1ea
  000aa1e2: blx 0x00072358
  000aa1e6: blx 0x0006eb48
  000aa1ea: mov r0,r4
  000aa1ec: str.w r5,[r4,#0x1b8]
  000aa1f0: pop {r4,r5,r7,pc}
```
