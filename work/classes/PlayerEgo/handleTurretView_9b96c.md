# PlayerEgo::handleTurretView
elf 0x9b96c body 614
Sig: undefined __thiscall handleTurretView(PlayerEgo * this, int param_1)

## decompile
```c

/* PlayerEgo::handleTurretView(int) */

void __thiscall PlayerEgo::handleTurretView(PlayerEgo *this,int param_1)

{
  Matrix *pMVar1;
  int *piVar2;
  int iVar3;
  code *pcVar4;
  int iVar5;
  undefined4 *puVar6;
  uint *puVar7;
  int *piVar8;
  uint in_fpscr;
  float fVar9;
  float fVar10;
  float fVar11;
  Matrix aMStack_d8 [60];
  float local_9c;
  float local_98;
  float local_94;
  Matrix aMStack_90 [12];
  float local_84;
  float local_80;
  float local_7c;
  Matrix aMStack_78 [60];
  int local_3c;
  
  piVar8 = *(int **)(DAT_000abbdc + 0xab984);
  local_3c = *piVar8;
  if (((this[9].field_18 == (int *)0x0) || (*(char *)((int)&this[0xd].field_18 + 1) != '\0')) &&
     ((*(char *)((int)&this[0x17].field_18 + 2) == '\0' || (this[0xc].field_14 != (int *)0x1)))) {
    fVar9 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::moveForward((float)this[5].field_8 * fVar9 * (float)this[5].field_4);
  }
  pcVar4 = *(code **)(DAT_000abbe0 + 0xab9d0);
  pMVar1 = (Matrix *)(*pcVar4)(this->field_8);
  (*pcVar4)(this[10].field_10);
  AbyssEngine::AEMath::operator*(aMStack_78,pMVar1);
  pMVar1 = (Matrix *)(*pcVar4)(this[0xb].field_10);
  AbyssEngine::AEMath::Matrix::operator*=(aMStack_78,pMVar1);
  if ((*(char *)&this[0x16].field_14 != '\0') || ((char)this[8].field_1C != '\0')) {
    AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_84);
    AbyssEngine::AEMath::MatrixGetUp(aMStack_90);
    AbyssEngine::AEMath::MatrixGetDir(aMStack_d8);
    AbyssEngine::AEMath::operator-((Vector *)&local_9c,(Vector *)&local_84);
    if (*(char *)&this[0x16].field_14 != '\0') {
      piVar2 = (int *)((int)this[0x16].field_10 + param_1);
      this[0x16].field_10 = piVar2;
      if (1000 < (int)piVar2) {
        *(undefined1 *)&this[0x16].field_14 = 0;
      }
      puVar6 = *(undefined4 **)(DAT_000abbe4 + 0xaba54);
      pcVar4 = *(code **)(DAT_000abbe8 + 0xaba56);
      iVar3 = (*pcVar4)(*puVar6,0xc);
      fVar9 = DAT_000abbd4;
      fVar10 = (float)VectorSignedToFloat(iVar3 + -6,(byte)(in_fpscr >> 0x16) & 3);
      local_84 = local_84 + fVar10 * DAT_000abbd4;
      iVar3 = (*pcVar4)(*puVar6,0xc);
      fVar10 = (float)VectorSignedToFloat(iVar3 + -6,(byte)(in_fpscr >> 0x16) & 3);
      local_80 = local_80 + fVar10 * fVar9;
      iVar3 = (*pcVar4)(*puVar6,0xc);
      fVar10 = (float)VectorSignedToFloat(iVar3 + -6,(byte)(in_fpscr >> 0x16) & 3);
      local_7c = local_7c + fVar10 * fVar9;
    }
    if ((char)this[8].field_1C != '\0') {
      fVar9 = (float)getBoostPercentage();
      piVar2 = this[5].field_14;
      puVar6 = *(undefined4 **)(DAT_000abbec + 0xabac8);
      if ((int)piVar2 < 3) {
        piVar2 = (int *)0x2;
      }
      pcVar4 = *(code **)(DAT_000abbf0 + 0xabace);
      if (7 < (int)piVar2) {
        piVar2 = (int *)0x8;
      }
      iVar5 = (int)piVar2 << 1;
      iVar3 = (*pcVar4)(*puVar6,iVar5);
      fVar10 = DAT_000abbd8;
      fVar11 = (float)VectorSignedToFloat(iVar3 - (int)piVar2,(byte)(in_fpscr >> 0x16) & 3);
      local_9c = local_9c + fVar9 * fVar11 * DAT_000abbd8;
      iVar3 = (*pcVar4)(*puVar6,iVar5);
      fVar11 = (float)VectorSignedToFloat(iVar3 - (int)piVar2,(byte)(in_fpscr >> 0x16) & 3);
      local_98 = local_98 + fVar9 * fVar11 * fVar10;
      iVar3 = (*pcVar4)(*puVar6,iVar5);
      fVar11 = (float)VectorSignedToFloat(iVar3 - (int)piVar2,(byte)(in_fpscr >> 0x16) & 3);
      local_94 = local_94 + fVar9 * fVar11 * fVar10;
    }
    AbyssEngine::AEMath::MatrixGetLookAt
              ((AEMath *)aMStack_d8,(Vector *)&local_84,(Vector *)&local_9c,(Vector *)aMStack_90);
    AbyssEngine::AEMath::Matrix::operator=(aMStack_78,aMStack_d8);
  }
  puVar7 = *(uint **)(DAT_000abbf4 + 0xabb72);
  AbyssEngine::PaintCanvas::CameraSetLocal(*puVar7,(Matrix *)this[10].m_pLevel);
  piVar2 = this[8].field_14;
  this[7].field_4 = (int *)0x0;
  this[7].field_8 = (int *)0x0;
  roll(this,(int)piVar2);
  pMVar1 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
  AbyssEngine::PaintCanvas::TransformGetLocal(*puVar7);
  AbyssEngine::AEMath::operator*(aMStack_d8,pMVar1);
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this->m_pPlayer + 4),aMStack_d8);
  if (*piVar8 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ab96c: push {r4,r5,r6,r7,lr}
  000ab96e: add r7,sp,#0xc
  000ab970: push {r7,r8,r9,r10,r11}
  000ab974: vpush {d8,d9}
  000ab978: sub sp,#0xa0
  000ab97a: mov r4,r0
  000ab97c: ldr r0,[0x000abbdc]
  000ab97e: mov r8,r1
  000ab980: add r0,pc
  000ab982: ldr.w r10,[r0,#0x0]
  000ab986: ldr.w r0,[r10,#0x0]
  000ab98a: str r0,[sp,#0x9c]
  000ab98c: ldr.w r0,[r4,#0x15c]
  000ab990: cbz r0,0x000ab998
  000ab992: ldrb.w r0,[r4,#0x1ed]
  000ab996: cbz r0,0x000ab9c8
  000ab998: ldrb.w r0,[r4,#0x356]
  000ab99c: cbz r0,0x000ab9a6
  000ab99e: ldr.w r0,[r4,#0x1c4]
  000ab9a2: cmp r0,#0x1
  000ab9a4: beq 0x000ab9c8
  000ab9a6: vmov s0,r8
  000ab9aa: vcvt.f32.s32 s0,s0
  000ab9ae: vldr.32 s4,[r4,#0xbc]
  000ab9b2: vldr.32 s2,[r4,#0xb8]
  000ab9b6: ldr r0,[r4,#0x8]
  000ab9b8: vmul.f32 s0,s4,s0
  000ab9bc: vmul.f32 s0,s0,s2
  000ab9c0: vmov r1,s0
  000ab9c4: blx 0x000725c8
  000ab9c8: ldr r1,[0x000abbe0]
  000ab9ca: ldr r0,[r4,#0x8]
  000ab9cc: add r1,pc
  000ab9ce: ldr r5,[r1,#0x0]
  000ab9d0: blx r5
  000ab9d2: mov r6,r0
  000ab9d4: ldr.w r0,[r4,#0x178]
  000ab9d8: blx r5
  000ab9da: add.w r9,sp,#0x60
  000ab9de: mov r2,r0
  000ab9e0: mov r1,r6
  000ab9e2: mov r0,r9
  000ab9e4: blx 0x0006f82c
  000ab9e8: ldr.w r0,[r4,#0x19c]
  000ab9ec: blx r5
  000ab9ee: mov r1,r0
  000ab9f0: mov r0,r9
  000ab9f2: blx 0x0006f4a8
  000ab9f6: ldrb.w r0,[r4,#0x32c]
  000ab9fa: cbnz r0,0x000aba06
  000ab9fc: ldrb.w r0,[r4,#0x13c]
  000aba00: cmp r0,#0x0
  000aba02: beq.w 0x000abb66
  000aba06: add.w r9,sp,#0x54
  000aba0a: add r6,sp,#0x60
  000aba0c: mov r0,r9
  000aba0e: mov r1,r6
  000aba10: blx 0x0006f16c
  000aba14: add r0,sp,#0x48
  000aba16: mov r1,r6
  000aba18: blx 0x0006f4d8
  000aba1c: mov r5,sp
  000aba1e: mov r1,r6
  000aba20: mov r0,r5
  000aba22: blx 0x0006f4c0
  000aba26: add r0,sp,#0x3c
  000aba28: mov r1,r9
  000aba2a: mov r2,r5
  000aba2c: blx 0x0006ec38
  000aba30: ldrb.w r0,[r4,#0x32c]
  000aba34: cbz r0,0x000abab0
  000aba36: ldr.w r0,[r4,#0x328]
  000aba3a: add r0,r8
  000aba3c: str.w r0,[r4,#0x328]
  000aba40: cmp.w r0,#0x3e8
  000aba44: itt gt
  000aba46: mov.gt r0,#0x0
  000aba48: strb.gt.w r0,[r4,#0x32c]
  000aba4c: ldr r0,[0x000abbe4]
  000aba4e: ldr r1,[0x000abbe8]
  000aba50: add r0,pc
  000aba52: add r1,pc
  000aba54: ldr r6,[r0,#0x0]
  000aba56: ldr r5,[r1,#0x0]
  000aba58: movs r1,#0xc
  000aba5a: ldr r0,[r6,#0x0]
  000aba5c: blx r5
  000aba5e: subs r0,#0x6
  000aba60: vldr.32 s16,[pc,#0x170]
  000aba64: movs r1,#0xc
  000aba66: vmov s0,r0
  000aba6a: vcvt.f32.s32 s0,s0
  000aba6e: vldr.32 s2,[sp,#0x54]
  000aba72: ldr r0,[r6,#0x0]
  000aba74: vmla.f32 s2,s0,s16
  000aba78: vstr.32 s2,[sp,#0x54]
  000aba7c: blx r5
  000aba7e: subs r0,#0x6
  000aba80: movs r1,#0xc
  000aba82: vmov s0,r0
  000aba86: vcvt.f32.s32 s0,s0
  000aba8a: vldr.32 s2,[sp,#0x58]
  000aba8e: ldr r0,[r6,#0x0]
  000aba90: vmla.f32 s2,s0,s16
  000aba94: vstr.32 s2,[sp,#0x58]
  000aba98: blx r5
  000aba9a: subs r0,#0x6
  000aba9c: vmov s0,r0
  000abaa0: vcvt.f32.s32 s0,s0
  000abaa4: vldr.32 s2,[sp,#0x5c]
  000abaa8: vmla.f32 s2,s0,s16
  000abaac: vstr.32 s2,[sp,#0x5c]
  000abab0: ldrb.w r0,[r4,#0x13c]
  000abab4: cmp r0,#0x0
  000abab6: beq 0x000abb50
  000abab8: mov r0,r4
  000ababa: blx 0x000725d4
  000ababe: mov r8,r0
  000abac0: ldr r0,[0x000abbec]
  000abac2: ldr r1,[0x000abbf0]
  000abac4: add r0,pc
  000abac6: ldr.w r6,[r4,#0xc8]
  000abaca: add r1,pc
  000abacc: ldr.w r11,[r0,#0x0]
  000abad0: cmp r6,#0x2
  000abad2: it le
  000abad4: mov.le r6,#0x2
  000abad6: ldr.w r9,[r1,#0x0]
  000abada: cmp r6,#0x8
  000abadc: it ge
  000abade: mov.ge r6,#0x8
  000abae0: ldr.w r0,[r11,#0x0]
  000abae4: lsls r5,r6,#0x1
  000abae6: mov r1,r5
  000abae8: blx r9
  000abaea: subs r0,r0,r6
  000abaec: vmov s16,r8
  000abaf0: vldr.32 s18,[pc,#0xe4]
  000abaf4: mov r1,r5
  000abaf6: vmov s0,r0
  000abafa: vcvt.f32.s32 s0,s0
  000abafe: vldr.32 s2,[sp,#0x3c]
  000abb02: ldr.w r0,[r11,#0x0]
  000abb06: vmul.f32 s0,s16,s0
  000abb0a: vmla.f32 s2,s0,s18
  000abb0e: vstr.32 s2,[sp,#0x3c]
  000abb12: blx r9
  000abb14: subs r0,r0,r6
  000abb16: mov r1,r5
  000abb18: vmov s0,r0
  000abb1c: vcvt.f32.s32 s0,s0
  000abb20: vldr.32 s2,[sp,#0x40]
  000abb24: ldr.w r0,[r11,#0x0]
  000abb28: vmul.f32 s0,s16,s0
  000abb2c: vmla.f32 s2,s0,s18
  000abb30: vstr.32 s2,[sp,#0x40]
  000abb34: blx r9
  000abb36: subs r0,r0,r6
  000abb38: vmov s0,r0
  000abb3c: vcvt.f32.s32 s0,s0
  000abb40: vldr.32 s2,[sp,#0x44]
  000abb44: vmul.f32 s0,s16,s0
  000abb48: vmla.f32 s2,s0,s18
  000abb4c: vstr.32 s2,[sp,#0x44]
  000abb50: mov r5,sp
  000abb52: add r1,sp,#0x54
  000abb54: add r2,sp,#0x3c
  000abb56: add r3,sp,#0x48
  000abb58: mov r0,r5
  000abb5a: blx 0x000725e0
  000abb5e: add r0,sp,#0x60
  000abb60: mov r1,r5
  000abb62: blx 0x0006f148
  000abb66: ldr r0,[0x000abbf4]
  000abb68: add r2,sp,#0x60
  000abb6a: ldr.w r1,[r4,#0x174]
  000abb6e: add r0,pc
  000abb70: ldr r6,[r0,#0x0]
  000abb72: ldr r0,[r6,#0x0]
  000abb74: blx 0x000720a0
  000abb78: ldr.w r1,[r4,#0x134]
  000abb7c: movs r0,#0x0
  000abb7e: strd r0,r0,[r4,#0x100]
  000abb82: mov r0,r4
  000abb84: blx 0x000725ec
  000abb88: ldr r1,[r4,#0x8]
  000abb8a: ldr r0,[r6,#0x0]
  000abb8c: ldr r1,[r1,#0xc]
  000abb8e: blx 0x000720c4
  000abb92: ldr r1,[r4,#0x4]
  000abb94: mov r5,r0
  000abb96: ldr r0,[r6,#0x0]
  000abb98: ldr r1,[r1,#0xc]
  000abb9a: blx 0x000720c4
  000abb9e: mov r6,sp
  000abba0: mov r2,r0
  000abba2: mov r0,r6
  000abba4: mov r1,r5
  000abba6: blx 0x0006f82c
  000abbaa: ldr r0,[r4,#0x0]
  000abbac: mov r1,r6
  000abbae: adds r0,#0x4
  000abbb0: blx 0x0006f148
  000abbb4: ldr r0,[sp,#0x9c]
  000abbb6: ldr.w r1,[r10,#0x0]
  000abbba: subs r0,r1,r0
  000abbbc: itttt eq
  000abbbe: add.eq sp,#0xa0
  000abbc0: vpop.eq {d8,d9}
  000abbc4: add.eq sp,#0x4
  000abbc6: pop.eq.w {r8,r9,r10,r11}
  000abbca: it eq
  000abbcc: pop.eq {r4,r5,r6,r7,pc}
  000abbce: blx 0x0006e824
```
