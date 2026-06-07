# PlayerEgo::setDockingCamera
elf 0x9aa18 body 318
Sig: undefined __thiscall setDockingCamera(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::setDockingCamera() */

void __thiscall PlayerEgo::setDockingCamera(PlayerEgo *this)

{
  bool bVar1;
  int *piVar2;
  int iVar3;
  AEGeometry *pAVar4;
  Matrix *pMVar5;
  uint uVar6;
  uint *puVar7;
  float fVar8;
  SingleLevel *pSVar9;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  
  if (this[10].field_10 == (int *)0x0) {
    puVar7 = *(uint **)(DAT_000aab84 + 0xaaa36);
    AbyssEngine::PaintCanvas::CameraCreate((PaintCanvas *)*puVar7,(uint *)&this[10].m_pLevel);
    uVar6 = *puVar7;
    iVar3 = Status::inAlienOrbit();
    fVar8 = DAT_000aab68;
    if (iVar3 != 0) {
      fVar8 = DAT_000aab6c;
    }
    AbyssEngine::PaintCanvas::CameraSetPerspective(uVar6,fVar8,extraout_s1,DAT_000aab6c);
    pAVar4 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar4,(PaintCanvas *)*puVar7);
    this[10].field_10 = (int *)pAVar4;
    AEGeometry::setRotationOrder(pAVar4,2);
    piVar2 = DAT_000aab7c;
    pSVar9 = (SingleLevel *)this[0xf].field_8;
    bVar1 = (float)pSVar9 == 0.0;
    if (bVar1) {
      pSVar9 = this[0xf].m_pLevel;
    }
    if ((bVar1 && (bVar1 && (float)pSVar9 == 0.0)) && ((float)this[0xf].field_10 == 0.0)) {
      this[0xf].m_pLevel = DAT_000aab78;
      this[0xf].field_10 = piVar2;
    }
    AEGeometry::translate((Vector *)this[10].field_10);
    pAVar4 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar4,(PaintCanvas *)*puVar7);
    this[0xb].field_10 = (int *)pAVar4;
    AEGeometry::translate(extraout_s0,extraout_s1_00,extraout_s2);
    AEGeometry::addChild((uint)this[10].field_10);
    pAVar4 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar4,(PaintCanvas *)*puVar7);
    this[10].field_14 = (int *)pAVar4;
    AEGeometry::addChild((uint)pAVar4);
  }
  AEGeometry::setPosition((AEGeometry *)this[10].field_14,(Vector *)&this[9].field_4);
  pMVar5 = (Matrix *)this[10].field_14;
  AEGeometry::getMatrix();
  AEGeometry::setMatrix(pMVar5);
  (*(code *)(DAT_001ab2e4 + 0x1ab2e8))(**(undefined4 **)(DAT_000aab8c + 0xaab44),this[10].m_pLevel);
  return;
}

```

## target disasm
```
  000aaa18: push {r4,r5,r6,r7,lr}
  000aaa1a: add r7,sp,#0xc
  000aaa1c: push.w r8
  000aaa20: sub sp,#0x8
  000aaa22: mov r4,r0
  000aaa24: ldr.w r0,[r0,#0x178]
  000aaa28: cmp r0,#0x0
  000aaa2a: bne 0x000aab1c
  000aaa2c: ldr r0,[0x000aab84]
  000aaa2e: add.w r1,r4,#0x174
  000aaa32: add r0,pc
  000aaa34: ldr.w r8,[r0,#0x0]
  000aaa38: ldr.w r0,[r8,#0x0]
  000aaa3c: blx 0x00072058
  000aaa40: ldr r0,[0x000aab88]
  000aaa42: ldr.w r5,[r4,#0x174]
  000aaa46: add r0,pc
  000aaa48: ldr.w r6,[r8,#0x0]
  000aaa4c: ldr r0,[r0,#0x0]
  000aaa4e: ldr r0,[r0,#0x0]
  000aaa50: blx 0x000723d0
  000aaa54: vldr.32 s0,[pc,#0x110]
  000aaa58: cmp r0,#0x0
  000aaa5a: vldr.32 s2,[pc,#0x110]
  000aaa5e: mov r0,r6
  000aaa60: ldr r2,[0x000aab70]
  000aaa62: mov r1,r5
  000aaa64: ldr r3,[0x000aab74]
  000aaa66: it ne
  000aaa68: vmov.ne.f32 s0,s2
  000aaa6c: vstr.32 s0,[sp]
  000aaa70: blx 0x00072004
  000aaa74: movs r0,#0xc0
  000aaa76: blx 0x0006eb24
  000aaa7a: ldr.w r1,[r8,#0x0]
  000aaa7e: mov r5,r0
  000aaa80: blx 0x000720f4
  000aaa84: mov r0,r5
  000aaa86: movs r1,#0x2
  000aaa88: str.w r5,[r4,#0x178]
  000aaa8c: blx 0x00072100
  000aaa90: vldr.32 s0,[r4,#0x224]
  000aaa94: add.w r1,r4,#0x224
  000aaa98: vcmpe.f32 s0,#0
  000aaa9c: vmrs apsr,fpscr
  000aaaa0: ittt eq
  000aaaa2: vldr.eq.32 s0,[r4,#0x228]
  000aaaa6: vcmpe.eq.f32 s0,#0
  000aaaaa: vmrs.eq apsr,fpscr
  000aaaae: bne 0x000aaac6
  000aaab0: vldr.32 s0,[r4,#0x22c]
  000aaab4: vcmpe.f32 s0,#0
  000aaab8: vmrs apsr,fpscr
  000aaabc: ittt eq
  000aaabe: ldr.eq r0,[0x000aab78]
  000aaac0: ldr.eq r2,[0x000aab7c]
  000aaac2: strd.eq r0,r2,[r4,#0x228]
  000aaac6: ldr.w r0,[r4,#0x178]
  000aaaca: blx 0x00072460
  000aaace: movs r0,#0xc0
  000aaad0: blx 0x0006eb24
  000aaad4: ldr.w r1,[r8,#0x0]
  000aaad8: mov r5,r0
  000aaada: blx 0x000720f4
  000aaade: ldr r2,[0x000aab80]
  000aaae0: mov r0,r5
  000aaae2: ldr r3,[0x000aab7c]
  000aaae4: movs r1,#0x0
  000aaae6: str.w r5,[r4,#0x19c]
  000aaaea: blx 0x00072154
  000aaaee: ldr.w r1,[r4,#0x19c]
  000aaaf2: ldr.w r0,[r4,#0x178]
  000aaaf6: ldr r1,[r1,#0xc]
  000aaaf8: blx 0x000720e8
  000aaafc: movs r0,#0xc0
  000aaafe: blx 0x0006eb24
  000aab02: ldr.w r1,[r8,#0x0]
  000aab06: mov r5,r0
  000aab08: blx 0x000720f4
  000aab0c: ldr.w r0,[r4,#0x178]
  000aab10: str.w r5,[r4,#0x17c]
  000aab14: ldr r1,[r0,#0xc]
  000aab16: mov r0,r5
  000aab18: blx 0x000720e8
  000aab1c: ldr.w r0,[r4,#0x17c]
  000aab20: add.w r1,r4,#0x148
  000aab24: blx 0x00072148
  000aab28: ldr r0,[r4,#0x8]
  000aab2a: ldr.w r5,[r4,#0x17c]
  000aab2e: blx 0x000721cc
  000aab32: mov r1,r0
  000aab34: mov r0,r5
  000aab36: blx 0x000721e4
  000aab3a: ldr r0,[0x000aab8c]
  000aab3c: ldr.w r1,[r4,#0x174]
  000aab40: add r0,pc
  000aab42: ldr r0,[r0,#0x0]
  000aab44: ldr r0,[r0,#0x0]
  000aab46: add sp,#0x8
  000aab48: pop.w r8
  000aab4c: pop.w {r4,r5,r6,r7,lr}
  000aab50: b.w 0x001ab2d8
  001ab2d8: bx pc
```
