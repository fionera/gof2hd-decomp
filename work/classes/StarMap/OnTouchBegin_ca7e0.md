# StarMap::OnTouchBegin
elf 0xca7e0 body 778
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* StarMap::OnTouchBegin(int, int) */

undefined4 StarMap::OnTouchBegin(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  byte bVar3;
  int iVar4;
  uint *puVar5;
  void *pvVar6;
  Array<Station*> *this;
  FileRead *this_00;
  undefined4 uVar7;
  int in_r2;
  Array *pAVar8;
  undefined4 *puVar9;
  int *piVar10;
  int iVar11;
  uint uVar12;
  uint in_fpscr;
  float fVar13;
  float fVar14;
  float fVar15;
  float extraout_s0;
  float fVar16;
  float fVar17;
  
  if (*(char *)(param_1 + 0xa9) == '\0') {
    if ((*(char *)(param_1 + 0x138) == '\0') && (*(char *)(param_1 + 0x139) == '\0')) {
      piVar10 = *(int **)(DAT_000daafc + 0xda826);
      Layout::OnTouchBegin(*piVar10,param_2);
      if (((*(char *)(param_1 + 0xa8) == '\0') || (*(char *)(param_1 + 0x13a) == '\0')) &&
         (*(char *)(param_1 + 0x13b) == '\0')) {
        TouchButton::OnTouchBegin(*(int *)(param_1 + 0x4c),param_2);
        iVar4 = *piVar10;
        if (((*(int *)(iVar4 + 0xc) < in_r2) &&
            (in_r2 < **(int **)(DAT_000dab00 + 0xda862) - *(int *)(iVar4 + 0x10))) &&
           ((*(char *)(param_1 + 0x118) == '\0' || (3999 < *(int *)(param_1 + 0x11c))))) {
          piVar10 = *(int **)(DAT_000dab04 + 0xda882);
          fVar13 = (float)FModSound::stop(*piVar10);
          uVar12 = 0;
          FModSound::play(*piVar10,(Vector *)0x66,(Vector *)0x0,fVar13);
          fVar13 = (float)VectorSignedToFloat(in_r2,(byte)(in_fpscr >> 0x16) & 3);
          fVar17 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
          iVar4 = *(int *)(param_1 + 0x60);
          *(float *)(param_1 + 0x15c) = fVar17;
          *(float *)(param_1 + 0x160) = fVar13;
          *(float *)(param_1 + 0x144) = fVar17;
          *(float *)(param_1 + 0x148) = fVar13;
          *(undefined4 *)(param_1 + 0x150) = 0;
          *(undefined4 *)(param_1 + 0x154) = 0;
          *(undefined1 *)(param_1 + 0x174) = 1;
          *(undefined1 *)(param_1 + 0x13a) = 0;
          if (*(int *)(param_1 + 4) == 0) {
            *(undefined4 *)(param_1 + 0x1d0) = 0;
            *(undefined4 *)(param_1 + 0x60) = 0xffffffff;
            puVar9 = *(undefined4 **)(DAT_000dab08 + 0xda972);
            for (; uVar12 < **(uint **)(param_1 + 0x68); uVar12 = uVar12 + 1) {
              puVar5 = (uint *)Status::getSystemVisibilities((Status *)*puVar9);
              if (((puVar5 != (uint *)0x0) && (uVar12 < *puVar5)) &&
                 (*(char *)(puVar5[1] + uVar12) != '\0')) {
                AbyssEngine::AEMath::Vector::operator=
                          ((Vector *)(param_1 + 0x78),
                           *(Vector **)(*(int *)(*(int *)(param_1 + 0x194) + 4) + uVar12 * 4));
                fVar14 = *(float *)(param_1 + 0x80);
                uVar1 = in_fpscr & 0xfffffff;
                uVar2 = uVar1 | (uint)(fVar14 < 0.0) << 0x1f | (uint)(fVar14 == 0.0) << 0x1e;
                in_fpscr = uVar2 | (uint)NAN(fVar14) << 0x1c;
                bVar3 = (byte)(uVar2 >> 0x18);
                if (!(bool)(bVar3 >> 6 & 1) && bVar3 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
                  fVar16 = *(float *)(param_1 + 0x78) - fVar17;
                  fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x1c8),
                                                      (byte)(in_fpscr >> 0x16) & 3);
                  fVar14 = -fVar16;
                  if (0.0 < fVar16) {
                    fVar14 = fVar16;
                  }
                  in_fpscr = uVar1;
                  if (fVar14 < fVar15) {
                    fVar16 = *(float *)(param_1 + 0x7c) - fVar13;
                    fVar14 = -fVar16;
                    if (0.0 < fVar16) {
                      fVar14 = fVar16;
                    }
                    if (fVar14 < fVar15) {
                      *(uint *)(param_1 + 0x60) = uVar12;
                      if (*(Array **)(param_1 + 0x58) != (Array *)0x0) {
                        ArrayReleaseClasses<Station*>(*(Array **)(param_1 + 0x58));
                        if (*(Array<Station*> **)(param_1 + 0x58) != (Array<Station*> *)0x0) {
                          pvVar6 = (void *)Array<Station*>::~Array
                                                     (*(Array<Station*> **)(param_1 + 0x58));
                          operator_delete(pvVar6);
                        }
                        *(undefined4 *)(param_1 + 0x58) = 0;
                      }
                      this = operator_new(0xc);
                      Array<Station*>::Array(this);
                      *(Array<Station*> **)(param_1 + 0x58) = this;
                      this_00 = operator_new(1);
                      FileRead::FileRead(this_00);
                      uVar7 = FileRead::loadStationsBinary
                                        (this_00,*(SolarSystem **)
                                                  (*(int *)(*(int *)(param_1 + 0x54) + 4) +
                                                  *(int *)(param_1 + 0x60) * 4));
                      *(undefined4 *)(param_1 + 0x58) = uVar7;
                      pvVar6 = (void *)FileRead::~FileRead(this_00);
                      operator_delete(pvVar6);
                      if (iVar4 != *(int *)(param_1 + 0x60)) {
                        FModSound::play(*piVar10,(Vector *)0x67,(Vector *)0x0,extraout_s0);
                      }
                      pAVar8 = *(Array **)(param_1 + 0x50);
                      iVar11 = *(int *)(param_1 + 0x54);
                      Status::getSystem();
                      iVar4 = SolarSystem::getIndex();
                      iVar4 = SystemPathFinder::getJumpDistance(pAVar8,iVar11,iVar4);
                      *(int *)(param_1 + 0x1d0) = iVar4;
                      if (iVar4 == 0) {
                        Status::getSystem();
                        iVar4 = SolarSystem::getIndex();
                        if (iVar4 != *(int *)(param_1 + 0x60)) {
                          *(undefined4 *)(param_1 + 0x1d0) = 4;
                          iVar4 = SolarSystem::getRoutes();
                          if (iVar4 == 0) {
                            *(undefined1 *)(param_1 + 0x1d4) = 1;
                          }
                        }
                      }
                      iVar4 = Status::hardCoreMode();
                      if (iVar4 == 0) {
                        return 0;
                      }
                      *(int *)(param_1 + 0x1d0) = *(int *)(param_1 + 0x1d0) << 1;
                      return 0;
                    }
                  }
                }
              }
            }
          }
          else if (*(int *)(param_1 + 4) == 3) {
            for (uVar12 = 0; uVar12 < **(uint **)(param_1 + 0x198); uVar12 = uVar12 + 1) {
              AbyssEngine::AEMath::Vector::operator=
                        ((Vector *)(param_1 + 0x78),
                         *(Vector **)((*(uint **)(param_1 + 0x198))[1] + uVar12 * 4));
              fVar14 = *(float *)(param_1 + 0x80);
              uVar1 = in_fpscr & 0xfffffff;
              uVar2 = uVar1 | (uint)(fVar14 < 0.0) << 0x1f | (uint)(fVar14 == 0.0) << 0x1e;
              in_fpscr = uVar2 | (uint)NAN(fVar14) << 0x1c;
              bVar3 = (byte)(uVar2 >> 0x18);
              if (!(bool)(bVar3 >> 6 & 1) && bVar3 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
                fVar16 = *(float *)(param_1 + 0x78) - fVar17;
                fVar15 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x1c8),
                                                    (byte)(in_fpscr >> 0x16) & 3);
                fVar14 = -fVar16;
                if (0.0 < fVar16) {
                  fVar14 = fVar16;
                }
                in_fpscr = uVar1;
                if (fVar14 < fVar15) {
                  fVar16 = *(float *)(param_1 + 0x7c) - fVar13;
                  fVar14 = -fVar16;
                  if (0.0 < fVar16) {
                    fVar14 = fVar16;
                  }
                  if (fVar14 < fVar15) {
                    *(uint *)(param_1 + 100) = uVar12;
                    FModSound::play(*piVar10,(Vector *)0x68,(Vector *)0x0,fVar15);
                    return 0;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else {
    ChoiceWindow::OnTouchBegin(*(int *)(param_1 + 0x5c),param_2);
  }
  return 0;
}

```

## target disasm
```
  000da7e0: push {r4,r5,r6,r7,lr}
  000da7e2: add r7,sp,#0xc
  000da7e4: push {r8,r9,r10}
  000da7e8: vpush {d7,d8,d9}
  000da7ec: mov r4,r0
  000da7ee: ldrb.w r0,[r0,#0xa9]
  000da7f2: mov r6,r2
  000da7f4: mov r5,r1
  000da7f6: cbz r0,0x000da804
  000da7f8: ldr r0,[r4,#0x5c]
  000da7fa: mov r1,r5
  000da7fc: mov r2,r6
  000da7fe: blx 0x0007471c
  000da802: b 0x000da810
  000da804: ldrb.w r0,[r4,#0x138]
  000da808: cbnz r0,0x000da810
  000da80a: ldrb.w r0,[r4,#0x139]
  000da80e: cbz r0,0x000da81c
  000da810: movs r0,#0x0
  000da812: vpop {d7,d8,d9}
  000da816: pop.w {r8,r9,r10}
  000da81a: pop {r4,r5,r6,r7,pc}
  000da81c: ldr r0,[0x000daafc]
  000da81e: mov r1,r5
  000da820: mov r2,r6
  000da822: add r0,pc
  000da824: ldr.w r8,[r0,#0x0]
  000da828: ldr.w r0,[r8,#0x0]
  000da82c: blx 0x00074728
  000da830: ldrb.w r0,[r4,#0xa8]
  000da834: cbz r0,0x000da83e
  000da836: ldrb.w r0,[r4,#0x13a]
  000da83a: cmp r0,#0x0
  000da83c: bne 0x000da810
  000da83e: ldrb.w r0,[r4,#0x13b]
  000da842: cmp r0,#0x0
  000da844: bne 0x000da810
  000da846: ldr r0,[r4,#0x4c]
  000da848: mov r1,r5
  000da84a: mov r2,r6
  000da84c: blx 0x00074734
  000da850: ldr.w r0,[r8,#0x0]
  000da854: ldr r1,[r0,#0xc]
  000da856: cmp r1,r6
  000da858: bge 0x000da810
  000da85a: ldr r1,[0x000dab00]
  000da85c: ldr r0,[r0,#0x10]
  000da85e: add r1,pc
  000da860: ldr r1,[r1,#0x0]
  000da862: ldr r1,[r1,#0x0]
  000da864: subs r0,r1,r0
  000da866: cmp r0,r6
  000da868: ble 0x000da810
  000da86a: ldrb.w r0,[r4,#0x118]
  000da86e: cbz r0,0x000da87a
  000da870: ldr.w r0,[r4,#0x11c]
  000da874: cmp.w r0,#0xfa0
  000da878: blt 0x000da810
  000da87a: ldr r0,[0x000dab04]
  000da87c: movs r1,#0x66
  000da87e: add r0,pc
  000da880: ldr.w r8,[r0,#0x0]
  000da884: ldr.w r0,[r8,#0x0]
  000da888: blx 0x000724a8
  000da88c: ldr.w r0,[r8,#0x0]
  000da890: mov.w r10,#0x0
  000da894: movs r1,#0x66
  000da896: movs r2,#0x0
  000da898: movs r3,#0x0
  000da89a: str.w r10,[sp,#0x0]
  000da89e: blx 0x00071548
  000da8a2: vmov s0,r6
  000da8a6: movs r1,#0x1
  000da8a8: vcvt.f32.s32 s16,s0
  000da8ac: vmov s0,r5
  000da8b0: vcvt.f32.s32 s18,s0
  000da8b4: ldr r0,[r4,#0x4]
  000da8b6: ldr.w r9,[r4,#0x60]
  000da8ba: cmp r0,#0x0
  000da8bc: vstr.32 s18,[r4,#0x15c]
  000da8c0: vstr.32 s16,[r4,#0x160]
  000da8c4: vstr.32 s18,[r4,#0x144]
  000da8c8: vstr.32 s16,[r4,#0x148]
  000da8cc: strd r10,r10,[r4,#0x150]
  000da8d0: strb.w r1,[r4,#0x174]
  000da8d4: strb.w r10,[r4,#0x13a]
  000da8d8: beq 0x000da95e
  000da8da: cmp r0,#0x3
  000da8dc: bne 0x000da810
  000da8de: add.w r5,r4,#0x78
  000da8e2: movs r6,#0x0
  000da8e4: b 0x000da952
  000da8e6: ldr r0,[r0,#0x4]
  000da8e8: ldr.w r1,[r0,r6,lsl #0x2]
  000da8ec: mov r0,r5
  000da8ee: blx 0x0006eb3c
  000da8f2: vldr.32 s0,[r4,#0x80]
  000da8f6: vcmpe.f32 s0,#0
  000da8fa: vmrs apsr,fpscr
  000da8fe: ble 0x000da950
  000da900: vldr.32 s0,[r4,#0x78]
  000da904: vldr.32 s2,[r4,#0x1c8]
  000da908: vsub.f32 s4,s0,s18
  000da90c: vcvt.f32.s32 s0,s2
  000da910: vcmpe.f32 s4,#0
  000da914: vmrs apsr,fpscr
  000da918: vneg.f32 s6,s4
  000da91c: it gt
  000da91e: vmov.gt.f32 s6,s4
  000da922: vcmpe.f32 s6,s0
  000da926: vmrs apsr,fpscr
  000da92a: bpl 0x000da950
  000da92c: vldr.32 s2,[r4,#0x7c]
  000da930: vsub.f32 s2,s2,s16
  000da934: vcmpe.f32 s2,#0
  000da938: vmrs apsr,fpscr
  000da93c: vneg.f32 s4,s2
  000da940: it gt
  000da942: vmov.gt.f32 s4,s2
  000da946: vcmpe.f32 s4,s0
  000da94a: vmrs apsr,fpscr
  000da94e: bmi 0x000daa06
  000da950: adds r6,#0x1
  000da952: ldr.w r0,[r4,#0x198]
  000da956: ldr r1,[r0,#0x0]
  000da958: cmp r6,r1
  000da95a: bcc 0x000da8e6
  000da95c: b 0x000da810
  000da95e: mov.w r0,#0xffffffff
  000da962: str.w r10,[r4,#0x1d0]
  000da966: str r0,[r4,#0x60]
  000da968: add.w r5,r4,#0x78
  000da96c: ldr r0,[0x000dab08]
  000da96e: add r0,pc
  000da970: ldr r6,[r0,#0x0]
  000da972: b 0x000da9fc
  000da974: ldr r0,[r6,#0x0]
  000da976: blx 0x00071aa0
  000da97a: cbz r0,0x000da9f8
  000da97c: ldr r1,[r0,#0x0]
  000da97e: cmp r1,r10
  000da980: bls 0x000da9f8
  000da982: ldr r0,[r0,#0x4]
  000da984: ldrb.w r0,[r0,r10]
  000da988: cbz r0,0x000da9f8
  000da98a: ldr.w r0,[r4,#0x194]
  000da98e: ldr r0,[r0,#0x4]
  000da990: ldr.w r1,[r0,r10,lsl #0x2]
  000da994: mov r0,r5
  000da996: blx 0x0006eb3c
  000da99a: vldr.32 s0,[r4,#0x80]
  000da99e: vcmpe.f32 s0,#0
  000da9a2: vmrs apsr,fpscr
  000da9a6: ble 0x000da9f8
  000da9a8: vldr.32 s0,[r4,#0x78]
  000da9ac: vldr.32 s2,[r4,#0x1c8]
  000da9b0: vsub.f32 s4,s0,s18
  000da9b4: vcvt.f32.s32 s0,s2
  000da9b8: vcmpe.f32 s4,#0
  000da9bc: vmrs apsr,fpscr
  000da9c0: vneg.f32 s6,s4
  000da9c4: it gt
  000da9c6: vmov.gt.f32 s6,s4
  000da9ca: vcmpe.f32 s6,s0
  000da9ce: vmrs apsr,fpscr
  000da9d2: bpl 0x000da9f8
  000da9d4: vldr.32 s2,[r4,#0x7c]
  000da9d8: vsub.f32 s2,s2,s16
  000da9dc: vcmpe.f32 s2,#0
  000da9e0: vmrs apsr,fpscr
  000da9e4: vneg.f32 s4,s2
  000da9e8: it gt
  000da9ea: vmov.gt.f32 s4,s2
  000da9ee: vcmpe.f32 s4,s0
  000da9f2: vmrs apsr,fpscr
  000da9f6: bmi 0x000daa1c
  000da9f8: add.w r10,r10,#0x1
  000da9fc: ldr r0,[r4,#0x68]
  000da9fe: ldr r0,[r0,#0x0]
  000daa00: cmp r10,r0
  000daa02: bcc 0x000da974
  000daa04: b 0x000da810
  000daa06: str r6,[r4,#0x64]
  000daa08: movs r1,#0x0
  000daa0a: ldr.w r0,[r8,#0x0]
  000daa0e: movs r2,#0x0
  000daa10: str r1,[sp,#0x0]
  000daa12: movs r1,#0x68
  000daa14: movs r3,#0x0
  000daa16: blx 0x00071548
  000daa1a: b 0x000da810
  000daa1c: ldr r0,[r4,#0x58]
  000daa1e: str.w r10,[r4,#0x60]
  000daa22: cbz r0,0x000daa38
  000daa24: blx 0x000735dc
  000daa28: ldr r0,[r4,#0x58]
  000daa2a: cbz r0,0x000daa34
  000daa2c: blx 0x000735e8
  000daa30: blx 0x0006eb48
  000daa34: movs r0,#0x0
  000daa36: str r0,[r4,#0x58]
  000daa38: movs r0,#0xc
  000daa3a: blx 0x0006eb24
  000daa3e: mov r5,r0
  000daa40: blx 0x00073594
  000daa44: movs r0,#0x1
  000daa46: str r5,[r4,#0x58]
  000daa48: blx 0x0006eb24
  000daa4c: mov r5,r0
  000daa4e: blx 0x00072124
  000daa52: ldr r0,[r4,#0x54]
  000daa54: ldr r1,[r4,#0x60]
  000daa56: ldr r0,[r0,#0x4]
  000daa58: ldr.w r1,[r0,r1,lsl #0x2]
  000daa5c: mov r0,r5
  000daa5e: blx 0x00073690
  000daa62: str r0,[r4,#0x58]
  000daa64: mov r0,r5
  000daa66: blx 0x0007213c
  000daa6a: blx 0x0006eb48
  000daa6e: ldr r0,[r4,#0x60]
  000daa70: cmp r9,r0
  000daa72: beq 0x000daa86
  000daa74: ldr.w r0,[r8,#0x0]
  000daa78: movs r1,#0x0
  000daa7a: str r1,[sp,#0x0]
  000daa7c: movs r1,#0x67
  000daa7e: movs r2,#0x0
  000daa80: movs r3,#0x0
  000daa82: blx 0x00071548
  000daa86: ldrd r5,r8,[r4,#0x50]
  000daa8a: ldr r0,[r6,#0x0]
  000daa8c: blx 0x00071a10
  000daa90: blx 0x00071a7c
  000daa94: ldr r3,[r4,#0x60]
  000daa96: mov r2,r0
  000daa98: mov r0,r5
  000daa9a: mov r1,r8
  000daa9c: blx 0x00074740
  000daaa0: cmp r0,#0x0
  000daaa2: str.w r0,[r4,#0x1d0]
  000daaa6: bne 0x000daad2
  000daaa8: ldr r0,[r6,#0x0]
  000daaaa: blx 0x00071a10
  000daaae: blx 0x00071a7c
  000daab2: ldr r1,[r4,#0x60]
  000daab4: cmp r0,r1
  000daab6: beq 0x000daad2
  000daab8: ldr r0,[r4,#0x54]
  000daaba: movs r2,#0x4
  000daabc: str.w r2,[r4,#0x1d0]
  000daac0: ldr r0,[r0,#0x4]
  000daac2: ldr.w r0,[r0,r1,lsl #0x2]
  000daac6: blx 0x00071aac
  000daaca: cbnz r0,0x000daad2
  000daacc: movs r0,#0x1
  000daace: strb.w r0,[r4,#0x1d4]
  000daad2: ldr r0,[r6,#0x0]
  000daad4: blx 0x0007192c
  000daad8: cmp r0,#0x0
  000daada: beq.w 0x000da810
  000daade: ldr.w r0,[r4,#0x1d0]
  000daae2: lsls r0,r0,#0x1
  000daae4: str.w r0,[r4,#0x1d0]
  000daae8: b 0x000da810
```
