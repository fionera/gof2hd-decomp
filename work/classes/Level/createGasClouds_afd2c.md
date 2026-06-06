# Level::createGasClouds
elf 0xafd2c body 602
Sig: undefined __thiscall createGasClouds(Level * this)

## decompile
```c

/* Level::createGasClouds() */

void __thiscall Level::createGasClouds(Level *this)

{
  uint uVar1;
  bool bVar2;
  byte bVar3;
  undefined4 uVar4;
  Station *pSVar5;
  int *piVar6;
  int iVar7;
  Array<KIPlayer*> *this_00;
  undefined4 *puVar8;
  int iVar9;
  int iVar10;
  AEGeometry *this_01;
  PlayerGasCloud *this_02;
  uint uVar11;
  Galaxy *this_03;
  int *piVar12;
  int *piVar13;
  code *pcVar14;
  uint in_fpscr;
  float fVar15;
  float fVar16;
  float fVar17;
  Vector aVStack_54 [12];
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  int local_3c;
  
  piVar12 = *(int **)(DAT_000bffc0 + 0xbfd46);
  local_3c = *piVar12;
  this_03 = (Galaxy *)**(undefined4 **)(DAT_000bffc4 + 0xbfd48);
  pSVar5 = (Station *)Status::getStation();
  piVar6 = (int *)Galaxy::getPlasmaProbabilities(this_03,pSVar5);
  iVar7 = Status::getShip();
  iVar7 = Ship::getFirstEquipmentOfSort(iVar7);
  if ((iVar7 != 0) && (iVar7 = Status::inAlienOrbit(), iVar7 == 0)) {
    Status::getSystem();
    iVar7 = SolarSystem::getIndex();
    if ((iVar7 != 10) && (*piVar6 == 0xcc)) {
      Status::getSystem();
      iVar7 = SolarSystem::getRoutes();
      if (iVar7 == 0) goto LAB_000bff6a;
    }
    local_48 = 0;
    local_44 = 0;
    local_40 = 0;
    this_00 = operator_new(0xc);
    Array<KIPlayer*>::Array(this_00);
    *(Array<KIPlayer*> **)(this + 0xf4) = this_00;
    iVar7 = Status::getCurrentCampaignMission();
    bVar2 = false;
    if (iVar7 == 0x8e) {
      pSVar5 = (Station *)Status::getStation();
      iVar7 = Station::getIndex(pSVar5);
      bVar2 = iVar7 == 0x4f;
    }
    piVar13 = *(int **)(DAT_000bffcc + 0xbfdea);
    iVar7 = AbyssEngine::AERandom::nextInt(*piVar13);
    fVar15 = (float)VectorSignedToFloat(piVar6[1],(byte)(in_fpscr >> 0x16) & 3);
    fVar17 = (float)VectorSignedToFloat(iVar7 + 4,(byte)(in_fpscr >> 0x16) & 3);
    fVar16 = DAT_000bffa4;
    if (bVar2) {
      fVar16 = 3.0;
    }
    fVar16 = fVar16 + (fVar15 / DAT_000bffa0) * fVar17;
    ArraySetLength<KIPlayer*>((uint)(0.0 < fVar16) * (int)fVar16,*(Array **)(this + 0xf4));
    fVar15 = DAT_000bffb4;
    fVar16 = DAT_000bffb0;
    iVar7 = DAT_000bffac;
    uVar4 = DAT_000bffa8;
    pcVar14 = *(code **)(DAT_000bffd0 + 0xbfe46);
    puVar8 = *(undefined4 **)(DAT_000bffd4 + 0xbfe5c);
    for (uVar11 = 0; uVar11 < **(uint **)(this + 0xf4); uVar11 = uVar11 + 1) {
      iVar9 = *piVar6;
      do {
        do {
          iVar10 = (*pcVar14)(*piVar13,uVar4);
          local_48 = VectorSignedToFloat(iVar10 + iVar7,(byte)(in_fpscr >> 0x16) & 3);
          iVar10 = (*pcVar14)(*piVar13,uVar4);
          local_44 = VectorSignedToFloat(iVar10 + iVar7,(byte)(in_fpscr >> 0x16) & 3);
          iVar10 = (*pcVar14)(*piVar13,uVar4);
          local_40 = VectorSignedToFloat(iVar10 + iVar7,(byte)(in_fpscr >> 0x16) & 3);
          fVar17 = (float)AbyssEngine::AEMath::VectorLength((Vector *)&local_48);
          uVar1 = in_fpscr & 0xfffffff | (uint)(fVar17 < fVar16) << 0x1f |
                  (uint)(fVar17 == fVar16) << 0x1e;
          in_fpscr = uVar1 | (uint)(NAN(fVar17) || NAN(fVar16)) << 0x1c;
          bVar3 = (byte)(uVar1 >> 0x18);
        } while ((bool)(bVar3 >> 6 & 1) || bVar3 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1));
        AbyssEngine::AEMath::operator-(aVStack_54,(Vector *)&local_48);
        fVar17 = (float)AbyssEngine::AEMath::VectorLength(aVStack_54);
        uVar1 = in_fpscr & 0xfffffff | (uint)(fVar17 < fVar15) << 0x1f |
                (uint)(fVar17 == fVar15) << 0x1e;
        in_fpscr = uVar1 | (uint)(NAN(fVar17) || NAN(fVar15)) << 0x1c;
        bVar3 = (byte)(uVar1 >> 0x18);
      } while ((bool)(bVar3 >> 6 & 1) || bVar3 >> 7 != ((byte)(in_fpscr >> 0x1c) & 1));
      if (uVar11 == 0 && bVar2) {
        local_44 = 0;
        local_48 = DAT_000bffbc;
        local_40 = DAT_000bffb8;
      }
      this_01 = operator_new(0xc0);
      AEGeometry::AEGeometry(this_01,0x37d1,(PaintCanvas *)*puVar8,false);
      this_02 = operator_new(0x16c);
      PlayerGasCloud::PlayerGasCloud
                (this_02,iVar9,*(ParticleSystemManager **)(this + 0x94),this_01,(Vector *)&local_48)
      ;
      *(PlayerGasCloud **)(*(int *)(*(int *)(this + 0xf4) + 4) + uVar11 * 4) = this_02;
      (**(code **)(**(int **)(*(int *)(*(int *)(this + 0xf4) + 4) + uVar11 * 4) + 0x14))();
    }
  }
LAB_000bff6a:
  if (*piVar12 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000bfd2c: push {r4,r5,r6,r7,lr}
  000bfd2e: add r7,sp,#0xc
  000bfd30: push {r7,r8,r9,r10,r11}
  000bfd34: vpush {d8,d9}
  000bfd38: sub sp,#0x48
  000bfd3a: mov r8,r0
  000bfd3c: ldr r0,[0x000bffc0]
  000bfd3e: ldr r1,[0x000bffc4]
  000bfd40: ldr r2,[0x000bffc8]
  000bfd42: add r0,pc
  000bfd44: add r1,pc
  000bfd46: add r2,pc
  000bfd48: ldr r5,[r0,#0x0]
  000bfd4a: ldr r0,[r1,#0x0]
  000bfd4c: ldr r6,[r2,#0x0]
  000bfd4e: ldr r1,[r5,#0x0]
  000bfd50: ldr r4,[r0,#0x0]
  000bfd52: ldr r0,[r6,#0x0]
  000bfd54: str r1,[sp,#0x44]
  000bfd56: blx 0x00071c14
  000bfd5a: mov r1,r0
  000bfd5c: mov r0,r4
  000bfd5e: blx 0x00073e70
  000bfd62: str r0,[sp,#0x14]
  000bfd64: ldr r0,[r6,#0x0]
  000bfd66: blx 0x00071a58
  000bfd6a: movs r1,#0x21
  000bfd6c: blx 0x0007228c
  000bfd70: cmp r0,#0x0
  000bfd72: beq.w 0x000bff6a
  000bfd76: ldr r0,[r6,#0x0]
  000bfd78: blx 0x000723d0
  000bfd7c: cmp r0,#0x0
  000bfd7e: bne.w 0x000bff6a
  000bfd82: ldr r0,[r6,#0x0]
  000bfd84: blx 0x00071a10
  000bfd88: blx 0x00071a7c
  000bfd8c: cmp r0,#0xa
  000bfd8e: beq 0x000bfda8
  000bfd90: ldr r0,[sp,#0x14]
  000bfd92: ldr r0,[r0,#0x0]
  000bfd94: cmp r0,#0xcc
  000bfd96: bne 0x000bfda8
  000bfd98: ldr r0,[r6,#0x0]
  000bfd9a: blx 0x00071a10
  000bfd9e: blx 0x00071aac
  000bfda2: cmp r0,#0x0
  000bfda4: beq.w 0x000bff6a
  000bfda8: movs r0,#0x0
  000bfdaa: strd r0,r0,[sp,#0x38]
  000bfdae: str r0,[sp,#0x40]
  000bfdb0: movs r0,#0xc
  000bfdb2: blx 0x0006eb24
  000bfdb6: mov r4,r0
  000bfdb8: blx 0x00073d08
  000bfdbc: str.w r4,[r8,#0xf4]
  000bfdc0: ldr r0,[r6,#0x0]
  000bfdc2: str r5,[sp,#0x8]
  000bfdc4: blx 0x00071770
  000bfdc8: cmp r0,#0x8e
  000bfdca: mov.w r4,#0x0
  000bfdce: bne 0x000bfde2
  000bfdd0: ldr r0,[r6,#0x0]
  000bfdd2: blx 0x00071c14
  000bfdd6: blx 0x00071824
  000bfdda: subs r0,#0x4f
  000bfddc: clz r0,r0
  000bfde0: lsrs r4,r0,#0x5
  000bfde2: ldr r0,[0x000bffcc]
  000bfde4: movs r1,#0x4
  000bfde6: add r0,pc
  000bfde8: ldr.w r9,[r0,#0x0]
  000bfdec: ldr.w r0,[r9,#0x0]
  000bfdf0: blx 0x00071848
  000bfdf4: ldr r1,[sp,#0x14]
  000bfdf6: adds r0,#0x4
  000bfdf8: vldr.32 s2,[pc,#0x1a4]
  000bfdfc: cmp r4,#0x0
  000bfdfe: vmov s6,r0
  000bfe02: vldr.32 s4,[pc,#0x1a0]
  000bfe06: vldr.32 s0,[r1,#0x4]
  000bfe0a: vcvt.f32.s32 s0,s0
  000bfe0e: vcvt.f32.s32 s6,s6
  000bfe12: str r4,[sp,#0x10]
  000bfe14: vdiv.f32 s0,s0,s2
  000bfe18: vmov.f32 s2,0x40400000
  000bfe1c: it ne
  000bfe1e: vmov.ne.f32 s4,s2
  000bfe22: ldr.w r1,[r8,#0xf4]
  000bfe26: vmla.f32 s4,s0,s6
  000bfe2a: vcvt.u32.f32 s0,s4
  000bfe2e: vmov r0,s0
  000bfe32: blx 0x00073d14
  000bfe36: ldr r0,[0x000bffd0]
  000bfe38: add.w r10,r8,#0xc8
  000bfe3c: str.w r8,[sp,#0x24]
  000bfe40: add r4,sp,#0x38
  000bfe42: add r0,pc
  000bfe44: ldr r6,[0x000bffa8]
  000bfe46: ldr r5,[0x000bffac]
  000bfe48: add.w r8,sp,#0x2c
  000bfe4c: ldr.w r11,[r0,#0x0]
  000bfe50: movs r2,#0x0
  000bfe52: ldr r0,[0x000bffd4]
  000bfe54: vldr.32 s16,[pc,#0x158]
  000bfe58: add r0,pc
  000bfe5a: vldr.32 s18,[pc,#0x158]
  000bfe5e: ldr r0,[r0,#0x0]
  000bfe60: str r0,[sp,#0xc]
  000bfe62: b 0x000bff5a
  000bfe64: ldr r0,[sp,#0x14]
  000bfe66: str r2,[sp,#0x28]
  000bfe68: ldr r0,[r0,#0x0]
  000bfe6a: str r0,[sp,#0x1c]
  000bfe6c: ldr.w r0,[r9,#0x0]
  000bfe70: mov r1,r6
  000bfe72: blx r11
  000bfe74: add r0,r5
  000bfe76: mov r1,r6
  000bfe78: vmov s0,r0
  000bfe7c: vcvt.f32.s32 s0,s0
  000bfe80: ldr.w r0,[r9,#0x0]
  000bfe84: vstr.32 s0,[sp,#0x38]
  000bfe88: blx r11
  000bfe8a: add r0,r5
  000bfe8c: mov r1,r6
  000bfe8e: vmov s0,r0
  000bfe92: vcvt.f32.s32 s0,s0
  000bfe96: ldr.w r0,[r9,#0x0]
  000bfe9a: vstr.32 s0,[sp,#0x3c]
  000bfe9e: blx r11
  000bfea0: add r0,r5
  000bfea2: vmov s0,r0
  000bfea6: mov r0,r4
  000bfea8: vcvt.f32.s32 s0,s0
  000bfeac: vstr.32 s0,[sp,#0x40]
  000bfeb0: blx 0x0006ec44
  000bfeb4: vmov s0,r0
  000bfeb8: vcmpe.f32 s0,s16
  000bfebc: vmrs apsr,fpscr
  000bfec0: ble 0x000bfe6c
  000bfec2: mov r0,r8
  000bfec4: mov r1,r4
  000bfec6: mov r2,r10
  000bfec8: blx 0x0006ec38
  000bfecc: mov r0,r8
  000bfece: blx 0x0006ec44
  000bfed2: vmov s0,r0
  000bfed6: vcmpe.f32 s0,s18
  000bfeda: vmrs apsr,fpscr
  000bfede: ble 0x000bfe6c
  000bfee0: ldr r0,[sp,#0x28]
  000bfee2: cmp r0,#0x0
  000bfee4: it ne
  000bfee6: mov.ne r0,#0x1
  000bfee8: ldr r1,[sp,#0x10]
  000bfeea: eor r1,r1,#0x1
  000bfeee: orrs r0,r1
  000bfef0: bne 0x000bfefe
  000bfef2: movs r0,#0x0
  000bfef4: str r0,[sp,#0x3c]
  000bfef6: ldr r0,[0x000bffbc]
  000bfef8: str r0,[sp,#0x38]
  000bfefa: ldr r0,[0x000bffb8]
  000bfefc: str r0,[sp,#0x40]
  000bfefe: movs r0,#0xc0
  000bff00: blx 0x0006eb24
  000bff04: mov r1,r0
  000bff06: ldr r0,[sp,#0xc]
  000bff08: ldr r2,[r0,#0x0]
  000bff0a: str r1,[sp,#0x20]
  000bff0c: mov r0,r1
  000bff0e: movw r1,#0x37d1
  000bff12: movs r3,#0x0
  000bff14: blx 0x0007207c
  000bff18: mov.w r0,#0x16c
  000bff1c: blx 0x0006eb24
  000bff20: mov r1,r0
  000bff22: ldr r0,[sp,#0x24]
  000bff24: ldr.w r2,[r0,#0x94]
  000bff28: str r4,[sp,#0x0]
  000bff2a: mov r0,r1
  000bff2c: str r1,[sp,#0x18]
  000bff2e: ldrd r1,r3,[sp,#0x1c]
  000bff32: blx 0x00073e7c
  000bff36: ldr r1,[sp,#0x24]
  000bff38: ldr r3,[sp,#0x28]
  000bff3a: ldr r2,[sp,#0x18]
  000bff3c: ldr.w r0,[r1,#0xf4]
  000bff40: ldr r0,[r0,#0x4]
  000bff42: str.w r2,[r0,r3,lsl #0x2]
  000bff46: ldr.w r0,[r1,#0xf4]
  000bff4a: ldr r0,[r0,#0x4]
  000bff4c: ldr.w r0,[r0,r3,lsl #0x2]
  000bff50: ldr r2,[r0,#0x0]
  000bff52: ldr r2,[r2,#0x14]
  000bff54: blx r2
  000bff56: ldr r2,[sp,#0x28]
  000bff58: adds r2,#0x1
  000bff5a: ldr r0,[sp,#0x24]
  000bff5c: ldr.w r0,[r0,#0xf4]
  000bff60: ldr r0,[r0,#0x0]
  000bff62: cmp r2,r0
  000bff64: bcc.w 0x000bfe64
  000bff68: ldr r5,[sp,#0x8]
  000bff6a: ldr r0,[sp,#0x44]
  000bff6c: ldr r1,[r5,#0x0]
  000bff6e: subs r0,r1,r0
  000bff70: itttt eq
  000bff72: add.eq sp,#0x48
  000bff74: vpop.eq {d8,d9}
  000bff78: add.eq sp,#0x4
  000bff7a: pop.eq.w {r8,r9,r10,r11}
  000bff7e: it eq
  000bff80: pop.eq {r4,r5,r6,r7,pc}
  000bff82: blx 0x0006e824
```
