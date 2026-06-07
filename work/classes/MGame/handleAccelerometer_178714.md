# MGame::handleAccelerometer
elf 0x178714 body 554
Sig: undefined __thiscall handleAccelerometer(MGame * this)

## decompile
```c

/* MGame::handleAccelerometer() */

void __thiscall MGame::handleAccelerometer(MGame *this)

{
  char cVar1;
  Engine *this_00;
  int iVar2;
  double *pdVar3;
  code *pcVar4;
  code *pcVar5;
  int iVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  double dVar10;
  float fVar11;
  
  this_00 = (Engine *)AbyssEngine::ApplicationManager::GetEngine();
  iVar2 = AbyssEngine::Engine::GetAccelValue(this_00);
  fVar7 = (float)(*(double *)(iVar2 + 8) * 2.5);
  fVar11 = 1.0;
  if (fVar7 <= 1.0) {
    fVar11 = -1.0;
    if (((int)((uint)(fVar7 < -1.0) << 0x1f) < 0) || (fVar11 = fVar7, fVar7 < 0.0)) {
      PlayerEgo::left((int)this->m_pPlayerEgo,fVar11 * fVar11);
      goto LAB_001887aa;
    }
    if (fVar7 == 0.0) goto LAB_001887aa;
  }
  PlayerEgo::right((int)this->m_pPlayerEgo,fVar11 * fVar11);
LAB_001887aa:
  pcVar5 = *(code **)(DAT_0018893c + 0x1887b4);
  (*pcVar5)(this->field_8);
  pcVar4 = *(code **)(DAT_00188940 + 0x1887bc);
  pdVar3 = (double *)(*pcVar4)();
  dVar10 = *pdVar3;
  (*pcVar5)(this->field_8);
  iVar2 = (*pcVar4)();
  fVar7 = (float)dVar10;
  iVar6 = *(int *)(DAT_00188944 + 0x1887d4);
  dVar10 = *(double *)(iVar2 + 0x10);
  fVar11 = *(float *)(iVar6 + 0x1c);
  cVar1 = *(char *)(iVar6 + 0x10);
  (*pcVar5)(this->field_8);
  iVar2 = (*pcVar4)();
  if (cVar1 == '\0') {
    fVar8 = fVar7;
    if (0.0 < *(double *)(iVar2 + 0x10)) {
      fVar8 = 1.0;
      if (fVar7 <= 1.0) {
        fVar8 = (1.0 - fVar7) + 1.0;
      }
    }
    fVar8 = fVar11 - fVar8;
    fVar11 = *(float *)(iVar6 + 0x20) - (float)dVar10;
  }
  else {
    fVar8 = fVar7;
    if (0.0 < *(double *)(iVar2 + 0x10)) {
      fVar8 = 1.0;
      if (fVar7 <= 1.0) {
        fVar8 = (1.0 - fVar7) + 1.0;
      }
    }
    fVar8 = fVar8 - fVar11;
    fVar11 = (float)dVar10 - *(float *)(iVar6 + 0x20);
  }
  fVar7 = fVar8 * 3.0;
  fVar9 = fVar11 * 3.0;
  fVar8 = fVar8 * -3.0;
  if (0.0 < fVar7) {
    fVar8 = fVar7;
  }
  fVar11 = fVar11 * -3.0;
  if (0.0 < fVar9) {
    fVar11 = fVar9;
  }
  if (fVar8 < fVar11) {
    fVar7 = fVar9;
  }
  fVar11 = 1.0;
  if (fVar7 <= 1.0) {
    fVar11 = -1.0;
    if (((int)((uint)(fVar7 < -1.0) << 0x1f) < 0) || (fVar11 = fVar7, fVar7 < 0.0)) {
      iVar2 = 0x40;
      if (0 < this[1].field_A4) {
        iVar2 = 0x44;
      }
      (*(code *)(DAT_001abb74 + 0x1abb78))
                (this->m_pPlayerEgo,*(undefined4 *)((int)&this->field_0 + iVar2),fVar11 * fVar11);
      return;
    }
    if (fVar7 == 0.0) {
      return;
    }
  }
  iVar2 = 0x40;
  if (0 < this[1].field_A4) {
    iVar2 = 0x44;
  }
  (*(code *)(DAT_001abb84 + 0x1abb88))
            (this->m_pPlayerEgo,*(undefined4 *)((int)&this->field_0 + iVar2),fVar11 * fVar11);
  return;
}

```

## target disasm
```
  00188714: push {r4,r5,r6,r7,lr}
  00188716: add r7,sp,#0xc
  00188718: push {r8,r9,r11}
  0018871c: vpush {d8,d9,d10}
  00188720: mov r8,r0
  00188722: ldr r0,[r0,#0x8]
  00188724: blx 0x0007258c
  00188728: blx 0x0006ff88
  0018872c: vmov.f64 d16,0x4004000000000000
  00188730: vldr.64 d17,[r0,#0x8]
  00188734: vmov.f32 s2,0x3f800000
  00188738: vmul.f64 d16,d17,d16
  0018873c: vcvt.f32.f64 s0,d16
  00188740: vcmpe.f32 s0,s2
  00188744: vmrs apsr,fpscr
  00188748: ble 0x0018876c
  0018874a: vmul.f32 s0,s2,s2
  0018874e: ldr.w r1,[r8,#0x16c]
  00188752: movs r3,#0x40
  00188754: ldr.w r0,[r8,#0x58]
  00188758: cmp r1,#0x0
  0018875a: it gt
  0018875c: mov.gt r3,#0x44
  0018875e: ldr.w r1,[r8,r3]
  00188762: vmov r2,s0
  00188766: blx 0x000729d0
  0018876a: b 0x001887aa
  0018876c: vmov.f32 s2,0xbf800000
  00188770: vcmpe.f32 s0,s2
  00188774: vmrs apsr,fpscr
  00188778: bmi 0x0018878a
  0018877a: vmov.f32 s2,s0
  0018877e: vcmpe.f32 s0,#0
  00188782: vmrs apsr,fpscr
  00188786: bpl.w 0x00188928
  0018878a: vmul.f32 s0,s2,s2
  0018878e: ldr.w r1,[r8,#0x16c]
  00188792: movs r3,#0x40
  00188794: ldr.w r0,[r8,#0x58]
  00188798: cmp r1,#0x0
  0018879a: it gt
  0018879c: mov.gt r3,#0x44
  0018879e: ldr.w r1,[r8,r3]
  001887a2: vmov r2,s0
  001887a6: blx 0x000729dc
  001887aa: ldr r1,[0x0018893c]
  001887ac: ldr.w r0,[r8,#0x8]
  001887b0: add r1,pc
  001887b2: ldr r6,[r1,#0x0]
  001887b4: blx r6
  001887b6: ldr r1,[0x00188940]
  001887b8: add r1,pc
  001887ba: ldr r4,[r1,#0x0]
  001887bc: blx r4
  001887be: vldr.64 d8,[r0]
  001887c2: ldr.w r0,[r8,#0x8]
  001887c6: blx r6
  001887c8: blx r4
  001887ca: ldr r1,[0x00188944]
  001887cc: vcvt.f32.f64 s20,d8
  001887d0: add r1,pc
  001887d2: ldr.w r9,[r1,#0x0]
  001887d6: vldr.64 d16,[r0,#0x10]
  001887da: ldr.w r0,[r8,#0x8]
  001887de: vldr.32 s18,[r9,#0x1c]
  001887e2: ldrb.w r5,[r9,#0x10]
  001887e6: vcvt.f32.f64 s16,d16
  001887ea: blx r6
  001887ec: blx r4
  001887ee: vldr.64 d16,[r0,#0x10]
  001887f2: cmp r5,#0x0
  001887f4: vcmpe.f64 d16,#0
  001887f8: beq 0x0018881c
  001887fa: vmrs apsr,fpscr
  001887fe: ble 0x0018883e
  00188800: vmov.f32 s0,0x3f800000
  00188804: vcmpe.f32 s20,s0
  00188808: vmrs apsr,fpscr
  0018880c: ittt le
  0018880e: vmov.le.f32 s0,0x3f800000
  00188812: vsub.le.f32 s2,s0,s20
  00188816: vadd.le.f32 s0,s2,s0
  0018881a: b 0x00188842
  0018881c: vmrs apsr,fpscr
  00188820: ble 0x00188854
  00188822: vmov.f32 s0,0x3f800000
  00188826: vcmpe.f32 s20,s0
  0018882a: vmrs apsr,fpscr
  0018882e: ittt le
  00188830: vmov.le.f32 s0,0x3f800000
  00188834: vsub.le.f32 s2,s0,s20
  00188838: vadd.le.f32 s0,s2,s0
  0018883c: b 0x00188858
  0018883e: vmov.f32 s0,s20
  00188842: vsub.f32 s4,s0,s18
  00188846: vldr.32 s0,[r9,#0x20]
  0018884a: vmov.f32 s2,0x40400000
  0018884e: vsub.f32 s8,s16,s0
  00188852: b 0x00188868
  00188854: vmov.f32 s0,s20
  00188858: vsub.f32 s4,s18,s0
  0018885c: vldr.32 s0,[r9,#0x20]
  00188860: vmov.f32 s2,0x40400000
  00188864: vsub.f32 s8,s0,s16
  00188868: vmov.f32 s6,0xc0400000
  0018886c: vmul.f32 s0,s4,s2
  00188870: vmul.f32 s10,s8,s2
  00188874: vmul.f32 s4,s4,s6
  00188878: vmul.f32 s6,s8,s6
  0018887c: vmov.f32 s2,0x3f800000
  00188880: vcmpe.f32 s0,#0
  00188884: vmrs apsr,fpscr
  00188888: vcmpe.f32 s10,#0
  0018888c: it gt
  0018888e: vmov.gt.f32 s4,s0
  00188892: vmrs apsr,fpscr
  00188896: it gt
  00188898: vmov.gt.f32 s6,s10
  0018889c: vcmpe.f32 s6,s4
  001888a0: vmrs apsr,fpscr
  001888a4: it gt
  001888a6: vmov.gt.f32 s0,s10
  001888aa: vcmpe.f32 s0,s2
  001888ae: vmrs apsr,fpscr
  001888b2: ble 0x001888e0
  001888b4: vmul.f32 s0,s2,s2
  001888b8: ldr.w r1,[r8,#0x16c]
  001888bc: movs r3,#0x40
  001888be: ldr.w r0,[r8,#0x58]
  001888c2: cmp r1,#0x0
  001888c4: it gt
  001888c6: mov.gt r3,#0x44
  001888c8: ldr.w r1,[r8,r3]
  001888cc: vmov r2,s0
  001888d0: vpop {d8,d9,d10}
  001888d4: pop.w {r8,r9,r11}
  001888d8: pop.w {r4,r5,r6,r7,lr}
  001888dc: b.w 0x001abb78
  001888e0: vmov.f32 s2,0xbf800000
  001888e4: vcmpe.f32 s0,s2
  001888e8: vmrs apsr,fpscr
  001888ec: bmi 0x001888fc
  001888ee: vmov.f32 s2,s0
  001888f2: vcmpe.f32 s0,#0
  001888f6: vmrs apsr,fpscr
  001888fa: bpl 0x0018892e
  001888fc: vmul.f32 s0,s2,s2
  00188900: ldr.w r1,[r8,#0x16c]
  00188904: movs r3,#0x40
  00188906: ldr.w r0,[r8,#0x58]
  0018890a: cmp r1,#0x0
  0018890c: it gt
  0018890e: mov.gt r3,#0x44
  00188910: ldr.w r1,[r8,r3]
  00188914: vmov r2,s0
  00188918: vpop {d8,d9,d10}
  0018891c: pop.w {r8,r9,r11}
  00188920: pop.w {r4,r5,r6,r7,lr}
  00188924: b.w 0x001abb68
  00188928: bgt.w 0x0018874a
  0018892c: b 0x001887aa
  0018892e: bgt 0x001888b4
  00188930: vpop {d8,d9,d10}
  00188934: pop.w {r8,r9,r11}
  00188938: pop {r4,r5,r6,r7,pc}
  001abb68: bx pc
  001abb78: bx pc
```
