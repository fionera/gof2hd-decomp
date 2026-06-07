# LevelScript::~LevelScript
elf 0x137c3c body 220
Sig: undefined __thiscall ~LevelScript(LevelScript * this)

## decompile
```c

/* LevelScript::~LevelScript() */

LevelScript * __thiscall LevelScript::~LevelScript(LevelScript *this)

{
  void *pvVar1;
  PlayerEgo *this_00;
  AEGeometry *this_01;
  
  if ((AEGeometry *)this[1].field_10 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[1].field_10);
    operator_delete(pvVar1);
  }
  this[1].field_10 = 0;
  if ((AEGeometry *)this[1].field_C != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this[1].field_C);
    operator_delete(pvVar1);
  }
  this_00 = this->m_pPlayerEgo;
  this[1].field_C = 0;
  if (this_00 != (PlayerEgo *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this_00);
    operator_delete(pvVar1);
  }
  this->m_pPlayerEgo = (PlayerEgo *)0x0;
  if ((AEGeometry *)this->m_pAsteroids != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->m_pAsteroids);
    operator_delete(pvVar1);
  }
  this->m_pAsteroids = (int *)0x0;
  if ((AEGeometry *)this->m_pExtra != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->m_pExtra);
    operator_delete(pvVar1);
  }
  this->m_pExtra = (int *)0x0;
  if ((AEGeometry *)this->m_nParticleSystem0 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->m_nParticleSystem0);
    operator_delete(pvVar1);
  }
  this->m_nParticleSystem0 = 0;
  if ((AEGeometry *)this->m_pGeometriesMission2 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->m_pGeometriesMission2);
    operator_delete(pvVar1);
  }
  this->m_pGeometriesMission2 = (int *)0x0;
  if ((AEGeometry *)this->m_pExplosion != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->m_pExplosion);
    operator_delete(pvVar1);
  }
  this->m_pExplosion = (int *)0x0;
  if ((AEGeometry *)this->m_pMission != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry((AEGeometry *)this->m_pMission);
    operator_delete(pvVar1);
  }
  this->m_pMission = (int *)0x0;
  if ((Explosion *)this->m_nParticleSystem1 != (Explosion *)0x0) {
    pvVar1 = (void *)Explosion::~Explosion((Explosion *)this->m_nParticleSystem1);
    operator_delete(pvVar1);
  }
  this_01 = (AEGeometry *)this[1].field_0;
  this->m_nParticleSystem1 = 0;
  if (this_01 != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(this_01);
    operator_delete(pvVar1);
  }
  this[1].field_0 = (int *)0x0;
  return this;
}

```

## target disasm
```
  00147c3c: push {r4,r5,r7,lr}
  00147c3e: add r7,sp,#0x8
  00147c40: mov r4,r0
  00147c42: ldr.w r0,[r0,#0xdc]
  00147c46: cbz r0,0x00147c50
  00147c48: blx 0x00071fc8
  00147c4c: blx 0x0006eb48
  00147c50: ldr.w r0,[r4,#0xd8]
  00147c54: movs r5,#0x0
  00147c56: str.w r5,[r4,#0xdc]
  00147c5a: cbz r0,0x00147c64
  00147c5c: blx 0x00071fc8
  00147c60: blx 0x0006eb48
  00147c64: ldr.w r0,[r4,#0xb8]
  00147c68: str.w r5,[r4,#0xd8]
  00147c6c: cbz r0,0x00147c76
  00147c6e: blx 0x00071fc8
  00147c72: blx 0x0006eb48
  00147c76: ldr.w r0,[r4,#0xbc]
  00147c7a: movs r5,#0x0
  00147c7c: str.w r5,[r4,#0xb8]
  00147c80: cbz r0,0x00147c8a
  00147c82: blx 0x00071fc8
  00147c86: blx 0x0006eb48
  00147c8a: ldr.w r0,[r4,#0xc0]
  00147c8e: str.w r5,[r4,#0xbc]
  00147c92: cbz r0,0x00147c9c
  00147c94: blx 0x00071fc8
  00147c98: blx 0x0006eb48
  00147c9c: ldr.w r0,[r4,#0xc4]
  00147ca0: movs r5,#0x0
  00147ca2: str.w r5,[r4,#0xc0]
  00147ca6: cbz r0,0x00147cb0
  00147ca8: blx 0x00071fc8
  00147cac: blx 0x0006eb48
  00147cb0: ldr.w r0,[r4,#0xac]
  00147cb4: str.w r5,[r4,#0xc4]
  00147cb8: cbz r0,0x00147cc2
  00147cba: blx 0x00071fc8
  00147cbe: blx 0x0006eb48
  00147cc2: ldr.w r0,[r4,#0xb0]
  00147cc6: movs r5,#0x0
  00147cc8: str.w r5,[r4,#0xac]
  00147ccc: cbz r0,0x00147cd6
  00147cce: blx 0x00071fc8
  00147cd2: blx 0x0006eb48
  00147cd6: ldr.w r0,[r4,#0xb4]
  00147cda: str.w r5,[r4,#0xb0]
  00147cde: cbz r0,0x00147ce8
  00147ce0: blx 0x00071fc8
  00147ce4: blx 0x0006eb48
  00147ce8: ldr.w r0,[r4,#0xc8]
  00147cec: movs r5,#0x0
  00147cee: str.w r5,[r4,#0xb4]
  00147cf2: cbz r0,0x00147cfc
  00147cf4: blx 0x00072334
  00147cf8: blx 0x0006eb48
  00147cfc: ldr.w r0,[r4,#0xcc]
  00147d00: str.w r5,[r4,#0xc8]
  00147d04: cbz r0,0x00147d0e
  00147d06: blx 0x00071fc8
  00147d0a: blx 0x0006eb48
  00147d0e: movs r0,#0x0
  00147d10: str.w r0,[r4,#0xcc]
  00147d14: mov r0,r4
  00147d16: pop {r4,r5,r7,pc}
```
