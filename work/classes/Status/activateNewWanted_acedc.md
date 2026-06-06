# Status::activateNewWanted
elf 0xacedc body 924
Sig: undefined __stdcall activateNewWanted(void)

## decompile
```c

/* Status::activateNewWanted() */

int Status::activateNewWanted(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  FileRead *this;
  Station *this_00;
  uint uVar4;
  Station *this_01;
  uint uVar5;
  Array<int> *this_02;
  void *pvVar6;
  int extraout_r1;
  uint uVar7;
  Wanted *this_03;
  uint *puVar8;
  int iVar9;
  int *piVar10;
  uint uVar11;
  uint *puVar12;
  uint *puVar13;
  Array *this_04;
  bool bVar14;
  SystemPathFinder *local_34;
  uint local_30;
  
  puVar12 = *(uint **)(DAT_000bd28c + 0xbceec);
  puVar8 = *(uint **)*puVar12;
  if (puVar8 == (uint *)0x0) {
    iVar9 = 0;
  }
  else {
    this_04 = (Array *)0x0;
    iVar9 = 0;
    local_34 = (SystemPathFinder *)0x0;
    for (uVar11 = 0; uVar11 < *puVar8; uVar11 = uVar11 + 1) {
      this_03 = *(Wanted **)(puVar8[1] + uVar11 * 4);
      if (this_03 != (Wanted *)0x0) {
        if (uVar11 < 2) {
          local_30 = 4;
          uVar1 = 2;
        }
        else {
          __aeabi_idivmod(uVar11 - 1,6);
          iVar2 = __aeabi_idiv(extraout_r1,3);
          uVar1 = iVar2 + 2;
          local_30 = extraout_r1 / 2 + 4;
        }
        iVar2 = SolarSystem::getRace();
        iVar3 = Wanted::getBoard();
        if ((((iVar2 == iVar3) &&
             (iVar3 = *(int *)(*puVar12 + 0x1e8), iVar2 = Wanted::getRequiredMission(this_03),
             iVar2 <= iVar3)) &&
            ((1 < uVar11 ||
             (iVar3 = *(int *)(*puVar12 + 0x1e8), iVar2 = Wanted::getRequiredMission(this_03),
             iVar3 <= iVar2)))) &&
           ((((iVar2 = inAlienOrbit(), iVar2 == 0 &&
              (iVar2 = Station::getIndex(*(Station **)(*puVar12 + 0x19c)), iVar2 != 0x6c)) &&
             (iVar2 = Wanted::isActive(this_03), iVar2 == 0)) &&
            (iVar2 = Wanted::isTerminated(this_03), iVar2 == 0)))) {
          uVar7 = *puVar12;
          iVar2 = Wanted::getBoard();
          if (iVar2 < 4) {
            iVar2 = *(int *)(uVar7 + iVar2 * 4 + 4);
          }
          else {
            iVar2 = 0;
          }
          iVar3 = Wanted::getRequiredBounties(this_03);
          if (iVar3 <= iVar2) {
            Wanted::setActive(this_03,true);
            if (local_34 == (SystemPathFinder *)0x0) {
              local_34 = operator_new(1);
              SystemPathFinder::SystemPathFinder(local_34);
              this = operator_new(1);
              FileRead::FileRead(this);
              this_04 = (Array *)FileRead::loadSystemsBinary();
              pvVar6 = (void *)FileRead::~FileRead(this);
              operator_delete(pvVar6);
            }
            iVar9 = iVar9 + 1;
            do {
              this_00 = (Station *)Globals::getRandomStation();
              uVar4 = Station::getSystem(this_00);
              uVar7 = *puVar12;
              puVar13 = *(uint **)(uVar7 + 0x38);
              bVar14 = puVar13 == (uint *)0x0;
              if (!bVar14) {
                uVar7 = *puVar13;
              }
              if ((bVar14 || uVar4 <= uVar7) && (!bVar14 && uVar7 != uVar4)) goto LAB_000bd06c;
              bVar14 = false;
              while (((iVar2 = SolarSystem::getRoutes(), iVar2 == 0 || (!bVar14)) ||
                     ((uVar4 < 0x1d && ((1 << (uVar4 & 0xff) & DAT_000bd290) != 0))))) {
                if (this_00 != (Station *)0x0) {
                  pvVar6 = (void *)Station::~Station(this_00);
                  operator_delete(pvVar6);
                }
                this_00 = (Station *)Globals::getRandomStation();
                uVar4 = Station::getSystem(this_00);
                bVar14 = false;
                if ((puVar13 != (uint *)0x0) && (bVar14 = false, uVar4 < *puVar13)) {
LAB_000bd06c:
                  bVar14 = false;
                  if (*(char *)(puVar13[1] + uVar4) != '\0') {
                    bVar14 = true;
                  }
                }
              }
              this_01 = (Station *)Globals::getRandomStation();
              uVar5 = Station::getSystem(this_01);
              bVar14 = puVar13 == (uint *)0x0;
              uVar7 = uVar5;
              if (!bVar14) {
                uVar7 = *puVar13;
              }
              if ((bVar14 || uVar5 <= uVar7) && (!bVar14 && uVar7 != uVar5)) goto LAB_000bd0ec;
              bVar14 = false;
              while ((((iVar2 = SolarSystem::getRoutes(), iVar2 == 0 || (!bVar14)) ||
                      ((uVar5 < 0x1d && ((1 << (uVar5 & 0xff) & DAT_000bd290) != 0)))) ||
                     (uVar4 == uVar5))) {
                if (this_01 != (Station *)0x0) {
                  pvVar6 = (void *)Station::~Station(this_01);
                  operator_delete(pvVar6);
                }
                this_01 = (Station *)Globals::getRandomStation();
                uVar5 = Station::getSystem(this_01);
                bVar14 = false;
                if ((puVar13 != (uint *)0x0) && (bVar14 = false, uVar5 < *puVar13)) {
LAB_000bd0ec:
                  bVar14 = false;
                  if (*(char *)(puVar13[1] + uVar5) != '\0') {
                    bVar14 = true;
                  }
                }
              }
              iVar2 = Station::getIndex(this_00);
              Wanted::setLastSeen(this_03,iVar2);
              iVar2 = Station::getIndex(this_01);
              Wanted::setTravelsTo(this_03,iVar2);
              iVar2 = Station::getSystem(this_00);
              iVar3 = Station::getSystem(this_01);
              if (this_00 != (Station *)0x0) {
                pvVar6 = (void *)Station::~Station(this_00);
                operator_delete(pvVar6);
              }
              if (this_01 != (Station *)0x0) {
                pvVar6 = (void *)Station::~Station(this_01);
                operator_delete(pvVar6);
              }
              this_02 = (Array<int> *)SystemPathFinder::getSystemPath(local_34,this_04,iVar2,iVar3);
            } while (((this_02 == (Array<int> *)0x0) || (*(uint *)this_02 < uVar1)) ||
                    (local_30 < *(uint *)this_02));
            piVar10 = *(int **)(DAT_000bd298 + 0xbd1dc);
            iVar2 = AbyssEngine::AERandom::nextInt(*piVar10);
            iVar3 = *(int *)(*(int *)(this_02 + 4) + iVar2 * 4);
            iVar2 = SolarSystem::getStations(*(SolarSystem **)(*(int *)(this_04 + 4) + iVar3 * 4));
            if (iVar2 != 0) {
              iVar2 = *piVar10;
              SolarSystem::getStations(*(SolarSystem **)(*(int *)(this_04 + 4) + iVar3 * 4));
              iVar2 = AbyssEngine::AERandom::nextInt(iVar2);
              iVar3 = SolarSystem::getStations(*(SolarSystem **)(*(int *)(this_04 + 4) + iVar3 * 4))
              ;
              Wanted::setCurrentLocation(this_03,*(int *)(*(int *)(iVar3 + 4) + iVar2 * 4));
            }
            pvVar6 = (void *)Array<int>::~Array(this_02);
            operator_delete(pvVar6);
          }
        }
      }
    }
    if (local_34 != (SystemPathFinder *)0x0) {
      pvVar6 = (void *)SystemPathFinder::~SystemPathFinder(local_34);
      operator_delete(pvVar6);
    }
    if (this_04 != (Array *)0x0) {
      ArrayReleaseClasses<SolarSystem*>(this_04);
      pvVar6 = (void *)Array<SolarSystem*>::~Array((Array<SolarSystem*> *)this_04);
      operator_delete(pvVar6);
    }
  }
  return iVar9;
}

```
## target disasm
```
  000bcedc: push {r4,r5,r6,r7,lr}
  000bcede: add r7,sp,#0xc
  000bcee0: push {r8,r9,r10,r11}
  000bcee4: sub sp,#0x24
  000bcee6: ldr r0,[0x000bd28c]
  000bcee8: add r0,pc
  000bceea: ldr.w r10,[r0,#0x0]
  000bceee: ldr.w r0,[r10,#0x0]
  000bcef2: ldr r6,[r0,#0x0]
  000bcef4: cmp r6,#0x0
  000bcef6: beq.w 0x000bd26a
  000bcefa: movs r0,#0x0
  000bcefc: mov.w r9,#0x0
  000bcf00: mov.w r11,#0x0
  000bcf04: mov.w r8,#0x0
  000bcf08: str r0,[sp,#0x14]
  000bcf0a: str.w r10,[sp,#0x10]
  000bcf0e: b 0x000bd23e
  000bcf10: ldr r0,[r6,#0x4]
  000bcf12: ldr.w r5,[r0,r9,lsl #0x2]
  000bcf16: cmp r5,#0x0
  000bcf18: beq.w 0x000bd23a
  000bcf1c: cmp.w r9,#0x2
  000bcf20: bcs 0x000bcf2a
  000bcf22: movs r0,#0x4
  000bcf24: str r0,[sp,#0x18]
  000bcf26: movs r0,#0x2
  000bcf28: b 0x000bcf4c
  000bcf2a: sub.w r0,r9,#0x1
  000bcf2e: movs r1,#0x6
  000bcf30: blx 0x0006f514
  000bcf34: mov r4,r1
  000bcf36: mov r0,r1
  000bcf38: movs r1,#0x3
  000bcf3a: blx 0x0007198c
  000bcf3e: adds r0,#0x2
  000bcf40: add.w r1,r4,r4, lsr #0x1f
  000bcf44: movs r2,#0x4
  000bcf46: add.w r1,r2,r1, asr #0x1
  000bcf4a: str r1,[sp,#0x18]
  000bcf4c: str r0,[sp,#0x1c]
  000bcf4e: ldr.w r0,[r10,#0x0]
  000bcf52: ldr.w r0,[r0,#0x1a4]
  000bcf56: blx 0x00071950
  000bcf5a: mov r4,r0
  000bcf5c: mov r0,r5
  000bcf5e: blx 0x00073a08
  000bcf62: cmp r4,r0
  000bcf64: bne.w 0x000bd23a
  000bcf68: ldr.w r0,[r10,#0x0]
  000bcf6c: ldr.w r4,[r0,#0x1e8]
  000bcf70: mov r0,r5
  000bcf72: blx 0x00073a14
  000bcf76: cmp r4,r0
  000bcf78: blt.w 0x000bd23a
  000bcf7c: cmp.w r9,#0x1
  000bcf80: bhi 0x000bcf96
  000bcf82: ldr.w r0,[r10,#0x0]
  000bcf86: ldr.w r4,[r0,#0x1e8]
  000bcf8a: mov r0,r5
  000bcf8c: blx 0x00073a14
  000bcf90: cmp r4,r0
  000bcf92: bgt.w 0x000bd23a
  000bcf96: ldr.w r0,[r10,#0x0]
  000bcf9a: blx 0x000723d0
  000bcf9e: cmp r0,#0x0
  000bcfa0: bne.w 0x000bd23a
  000bcfa4: ldr.w r0,[r10,#0x0]
  000bcfa8: ldr.w r0,[r0,#0x19c]
  000bcfac: blx 0x00071824
  000bcfb0: cmp r0,#0x6c
  000bcfb2: beq.w 0x000bd23a
  000bcfb6: mov r0,r5
  000bcfb8: blx 0x00073774
  000bcfbc: cmp r0,#0x0
  000bcfbe: bne.w 0x000bd23a
  000bcfc2: mov r0,r5
  000bcfc4: blx 0x00071a04
  000bcfc8: cmp r0,#0x0
  000bcfca: bne.w 0x000bd23a
  000bcfce: mov r0,r5
  000bcfd0: ldr.w r4,[r10,#0x0]
  000bcfd4: blx 0x00073a08
  000bcfd8: cmp r0,#0x3
  000bcfda: itte le
  000bcfdc: add.le.w r0,r4,r0, lsl #0x2
  000bcfe0: ldr.le r4,[r0,#0x4]
  000bcfe2: mov.gt r4,#0x0
  000bcfe4: mov r0,r5
  000bcfe6: blx 0x00073918
  000bcfea: cmp r4,r0
  000bcfec: blt.w 0x000bd23a
  000bcff0: mov r0,r5
  000bcff2: movs r1,#0x1
  000bcff4: blx 0x00073a20
  000bcff8: ldr r0,[sp,#0x14]
  000bcffa: cbnz r0,0x000bd028
  000bcffc: movs r0,#0x1
  000bcffe: blx 0x0006eb24
  000bd002: str r0,[sp,#0x14]
  000bd004: blx 0x00073798
  000bd008: movs r0,#0x1
  000bd00a: blx 0x0006eb24
  000bd00e: mov r4,r0
  000bd010: blx 0x00072124
  000bd014: mov r0,r4
  000bd016: blx 0x0007378c
  000bd01a: mov r11,r0
  000bd01c: mov r0,r4
  000bd01e: blx 0x0007213c
  000bd022: blx 0x0006eb48
  000bd026: ldr r0,[sp,#0x14]
  000bd028: str r5,[sp,#0xc]
  000bd02a: add.w r8,r8,#0x1
  000bd02e: str r0,[sp,#0x14]
  000bd030: str r6,[sp,#0x0]
  000bd032: strd r8,r9,[sp,#0x4]
  000bd036: str.w r11,[sp,#0x20]
  000bd03a: ldr r0,[0x000bd294]
  000bd03c: add r0,pc
  000bd03e: ldr.w r9,[r0,#0x0]
  000bd042: ldr.w r0,[r9,#0x0]
  000bd046: blx 0x000737d4
  000bd04a: mov r5,r0
  000bd04c: blx 0x0007189c
  000bd050: mov r4,r0
  000bd052: ldr.w r0,[r10,#0x0]
  000bd056: ldr.w r10,[r0,#0x38]
  000bd05a: cmp.w r10,#0x0
  000bd05e: itt ne
  000bd060: ldr.ne.w r0,[r10,#0x0]
  000bd064: cmp.ne r0,r4
  000bd066: bhi 0x000bd06c
  000bd068: movs r6,#0x0
  000bd06a: b 0x000bd078
  000bd06c: ldr.w r0,[r10,#0x4]
  000bd070: ldrb r6,[r0,r4]
  000bd072: cmp r6,#0x0
  000bd074: it ne
  000bd076: mov.ne r6,#0x1
  000bd078: ldr.w r0,[r11,#0x4]
  000bd07c: ldr.w r0,[r0,r4,lsl #0x2]
  000bd080: blx 0x00071aac
  000bd084: cbz r0,0x000bd098
  000bd086: lsls r0,r6,#0x1f
  000bd088: beq 0x000bd098
  000bd08a: cmp r4,#0x1c
  000bd08c: bhi 0x000bd0c8
  000bd08e: ldr r1,[0x000bd290]
  000bd090: movs r0,#0x1
  000bd092: lsls r0,r4
  000bd094: tst r0,r1
  000bd096: beq 0x000bd0c8
  000bd098: cbz r5,0x000bd0a4
  000bd09a: mov r0,r5
  000bd09c: blx 0x0007360c
  000bd0a0: blx 0x0006eb48
  000bd0a4: ldr.w r0,[r9,#0x0]
  000bd0a8: blx 0x000737d4
  000bd0ac: mov r5,r0
  000bd0ae: blx 0x0007189c
  000bd0b2: mov r4,r0
  000bd0b4: movs r6,#0x0
  000bd0b6: cmp.w r10,#0x0
  000bd0ba: beq 0x000bd078
  000bd0bc: ldr.w r0,[r10,#0x0]
  000bd0c0: movs r6,#0x0
  000bd0c2: cmp r0,r4
  000bd0c4: bhi 0x000bd06c
  000bd0c6: b 0x000bd078
  000bd0c8: ldr.w r0,[r9,#0x0]
  000bd0cc: blx 0x000737d4
  000bd0d0: mov r6,r0
  000bd0d2: blx 0x0007189c
  000bd0d6: mov r8,r0
  000bd0d8: cmp.w r10,#0x0
  000bd0dc: itt ne
  000bd0de: ldr.ne.w r0,[r10,#0x0]
  000bd0e2: cmp.ne r0,r8
  000bd0e4: bhi 0x000bd0ec
  000bd0e6: mov.w r11,#0x0
  000bd0ea: b 0x000bd0fe
  000bd0ec: ldr.w r0,[r10,#0x4]
  000bd0f0: ldrb.w r11,[r0,r8]
  000bd0f4: cmp.w r11,#0x0
  000bd0f8: it ne
  000bd0fa: mov.ne.w r11,#0x1
  000bd0fe: ldr r0,[sp,#0x20]
  000bd100: ldr r0,[r0,#0x4]
  000bd102: ldr.w r0,[r0,r8,lsl #0x2]
  000bd106: blx 0x00071aac
  000bd10a: cbz r0,0x000bd124
  000bd10c: lsls.w r0,r11,#0x1f
  000bd110: beq 0x000bd124
  000bd112: cmp.w r8,#0x1c
  000bd116: bhi 0x000bd158
  000bd118: ldr r1,[0x000bd290]
  000bd11a: movs r0,#0x1
  000bd11c: lsl.w r0,r0,r8
  000bd120: tst r0,r1
  000bd122: beq 0x000bd158
  000bd124: cbz r6,0x000bd130
  000bd126: mov r0,r6
  000bd128: blx 0x0007360c
  000bd12c: blx 0x0006eb48
  000bd130: ldr.w r0,[r9,#0x0]
  000bd134: blx 0x000737d4
  000bd138: mov r6,r0
  000bd13a: blx 0x0007189c
  000bd13e: mov r8,r0
  000bd140: mov.w r11,#0x0
  000bd144: cmp.w r10,#0x0
  000bd148: beq 0x000bd0fe
  000bd14a: ldr.w r0,[r10,#0x0]
  000bd14e: mov.w r11,#0x0
  000bd152: cmp r0,r8
  000bd154: bhi 0x000bd0ec
  000bd156: b 0x000bd0fe
  000bd158: cmp r4,r8
  000bd15a: beq 0x000bd124
  000bd15c: mov r0,r5
  000bd15e: blx 0x00071824
  000bd162: ldr r4,[sp,#0xc]
  000bd164: mov r1,r0
  000bd166: mov r0,r4
  000bd168: blx 0x000737c8
  000bd16c: mov r0,r6
  000bd16e: blx 0x00071824
  000bd172: mov r1,r0
  000bd174: mov r0,r4
  000bd176: blx 0x000737ec
  000bd17a: mov r0,r5
  000bd17c: blx 0x0007189c
  000bd180: mov r8,r0
  000bd182: mov r0,r6
  000bd184: blx 0x0007189c
  000bd188: mov r4,r0
  000bd18a: cbz r5,0x000bd196
  000bd18c: mov r0,r5
  000bd18e: blx 0x0007360c
  000bd192: blx 0x0006eb48
  000bd196: ldr.w r10,[sp,#0x10]
  000bd19a: cmp r6,#0x0
  000bd19c: ldr.w r9,[sp,#0x8]
  000bd1a0: ldr.w r11,[sp,#0x20]
  000bd1a4: beq 0x000bd1b0
  000bd1a6: mov r0,r6
  000bd1a8: blx 0x0007360c
  000bd1ac: blx 0x0006eb48
  000bd1b0: ldr r0,[sp,#0x14]
  000bd1b2: mov r1,r11
  000bd1b4: mov r2,r8
  000bd1b6: mov r3,r4
  000bd1b8: blx 0x000737e0
  000bd1bc: mov r4,r0
  000bd1be: cmp r0,#0x0
  000bd1c0: beq.w 0x000bd03a
  000bd1c4: ldr r1,[r4,#0x0]
  000bd1c6: ldr r0,[sp,#0x1c]
  000bd1c8: cmp r1,r0
  000bd1ca: bcc.w 0x000bd03a
  000bd1ce: ldr r0,[sp,#0x18]
  000bd1d0: cmp r1,r0
  000bd1d2: bhi.w 0x000bd03a
  000bd1d6: ldr r0,[0x000bd298]
  000bd1d8: add r0,pc
  000bd1da: ldr.w r8,[r0,#0x0]
  000bd1de: ldr.w r0,[r8,#0x0]
  000bd1e2: blx 0x00071848
  000bd1e6: ldr r1,[r4,#0x4]
  000bd1e8: ldr.w r6,[r1,r0,lsl #0x2]
  000bd1ec: ldr.w r0,[r11,#0x4]
  000bd1f0: ldr.w r0,[r0,r6,lsl #0x2]
  000bd1f4: blx 0x00071a88
  000bd1f8: cbz r0,0x000bd22c
  000bd1fa: ldr.w r0,[r11,#0x4]
  000bd1fe: ldr.w r8,[r8,#0x0]
  000bd202: ldr.w r0,[r0,r6,lsl #0x2]
  000bd206: blx 0x00071a88
  000bd20a: ldr r1,[r0,#0x0]
  000bd20c: mov r0,r8
  000bd20e: blx 0x00071848
  000bd212: mov r8,r0
  000bd214: ldr.w r0,[r11,#0x4]
  000bd218: ldr.w r0,[r0,r6,lsl #0x2]
  000bd21c: blx 0x00071a88
  000bd220: ldr r0,[r0,#0x4]
  000bd222: ldr.w r1,[r0,r8,lsl #0x2]
  000bd226: ldr r0,[sp,#0xc]
  000bd228: blx 0x000737bc
  000bd22c: mov r0,r4
  000bd22e: blx 0x00070204
  000bd232: blx 0x0006eb48
  000bd236: ldrd r6,r8,[sp,#0x0]
  000bd23a: add.w r9,r9,#0x1
  000bd23e: ldr r0,[r6,#0x0]
  000bd240: cmp r9,r0
  000bd242: bcc.w 0x000bcf10
  000bd246: ldr r0,[sp,#0x14]
  000bd248: cbz r0,0x000bd252
  000bd24a: blx 0x0007381c
  000bd24e: blx 0x0006eb48
  000bd252: cmp.w r11,#0x0
  000bd256: beq 0x000bd26e
  000bd258: mov r0,r11
  000bd25a: blx 0x00073804
  000bd25e: mov r0,r11
  000bd260: blx 0x00073810
  000bd264: blx 0x0006eb48
  000bd268: b 0x000bd26e
  000bd26a: mov.w r8,#0x0
  000bd26e: mov r0,r8
  000bd270: add sp,#0x24
  000bd272: pop.w {r8,r9,r10,r11}
  000bd276: pop {r4,r5,r6,r7,pc}
```
