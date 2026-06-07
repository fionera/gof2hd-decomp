# GameText::convertStringFromArabic
elf 0x7efe0 body 646
Sig: undefined __stdcall convertStringFromArabic(String param_1)

## decompile
```c

/* GameText::convertStringFromArabic(AbyssEngine::String) */

void GameText::convertStringFromArabic(String *param_1,undefined4 param_2,String *param_3)

{
  ushort uVar1;
  ushort uVar2;
  uint uVar3;
  uint uVar4;
  ushort *puVar5;
  ushort *puVar6;
  undefined2 *puVar7;
  uint uVar8;
  uint uVar9;
  uint *puVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  String aSStack_58 [12];
  String aSStack_4c [8];
  int local_44;
  String aSStack_40 [12];
  String aSStack_34 [8];
  int local_2c;
  int local_28;
  
  piVar12 = *(int **)(DAT_0008f2a8 + 0x8eff6);
  local_28 = *piVar12;
  AbyssEngine::String::String(aSStack_34,param_3,false);
  uVar3 = local_2c + 1;
  uVar4 = uVar3 * 2;
  if (CARRY4(uVar3,uVar3)) {
    uVar4 = 0xffffffff;
  }
  puVar5 = operator_new__(uVar4);
  puVar6 = AbyssEngine::String::operator_cast_to_unsigned_short_(aSStack_34);
  __aeabi_memcpy(puVar5,puVar6,local_2c * 2 + 2);
  iVar14 = *(int *)(param_3 + 8);
  uVar3 = iVar14 - 1;
  uVar4 = uVar3;
  do {
    if (0x7fffffff < uVar4) {
      AbyssEngine::String::String(param_1,puVar5,false);
      if (puVar5 != (ushort *)0x0) {
        operator_delete__(puVar5);
      }
      AbyssEngine::String::~String(aSStack_34);
      if (*piVar12 != local_28) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return;
    }
    if (uVar4 == 0) {
      uVar4 = 0;
LAB_0008f15c:
      iVar15 = uVar4 - 1;
      for (iVar11 = 0; iVar11 != 0x334; iVar11 = iVar11 + 0x14) {
        if (*(uint *)(DAT_0008f2ac + 0x8f176 + iVar11) == (uint)puVar5[uVar4]) {
          if (uVar4 == 0) {
            if ((iVar14 == 1) || (uVar1 = puVar5[1], uVar1 < 0x600)) {
LAB_0008f1d0:
              iVar13 = 1;
            }
            else {
              iVar13 = 1;
              if (uVar1 != 0x60c && uVar1 != 0x61f) {
LAB_0008f1ee:
                iVar13 = 4;
              }
            }
          }
          else if ((int)uVar4 < (int)uVar3) {
            uVar1 = puVar5[uVar4 + 1];
            if ((uVar1 < 0x600) || (uVar1 == 0x61f || uVar1 == 0x60c)) {
              uVar2 = puVar5[iVar15];
              if (uVar2 < 0x600) goto LAB_0008f1d0;
              iVar13 = 1;
              if ((uVar2 == 0x60c) || (uVar2 == 0x61f)) goto LAB_0008f1fe;
            }
            else {
              uVar2 = puVar5[iVar15];
              if (uVar2 < 0x600) goto LAB_0008f1ee;
            }
            iVar13 = 4;
            if (uVar2 != 0x60c && uVar2 != 0x61f) {
              if (uVar1 < 0x600) goto LAB_0008f1e0;
              iVar13 = 2;
              if ((uVar1 != 0x60c) && (uVar1 != 0x61f)) {
                iVar13 = 3;
              }
            }
          }
          else {
            uVar1 = puVar5[iVar15];
            if (uVar1 < 0x600) goto LAB_0008f1d0;
            iVar13 = 1;
            if (uVar1 == 0x60c || uVar1 == 0x61f) goto LAB_0008f1fe;
LAB_0008f1e0:
            iVar13 = 2;
          }
LAB_0008f1fe:
          puVar5[uVar4] = (ushort)*(undefined4 *)(DAT_0008f2ac + 0x8f176 + iVar13 * 4 + iVar11);
        }
      }
    }
    else {
      puVar6 = (ushort *)AbyssEngine::String::operator[](param_3,uVar4 - 1);
      uVar1 = *puVar6;
      puVar7 = (undefined2 *)AbyssEngine::String::operator[](param_3,uVar4);
      if (uVar1 == 0x644) {
        uVar2 = 2;
        switch(*puVar7) {
        case 0x622:
          uVar2 = 5;
          break;
        case 0x623:
          uVar2 = 3;
          break;
        default:
          goto switchD_0008f07c_caseD_624;
        case 0x625:
          uVar2 = 4;
          break;
        case 0x627:
          break;
        }
        puVar5[uVar4] = uVar2;
        AbyssEngine::String::String(aSStack_40,puVar5,false);
        AbyssEngine::String::SubString((uint)aSStack_4c,(uint)aSStack_40);
        AbyssEngine::String::SubString((uint)aSStack_58,(uint)aSStack_40);
        AbyssEngine::String::operator+=(aSStack_4c,aSStack_58);
        AbyssEngine::String::~String(aSStack_58);
        operator_delete__(puVar5);
        uVar8 = local_44 + 1;
        uVar9 = uVar8 * 2;
        if (CARRY4(uVar8,uVar8)) {
          uVar9 = 0xffffffff;
        }
        puVar5 = operator_new__(uVar9);
        puVar6 = AbyssEngine::String::operator_cast_to_unsigned_short_(aSStack_4c);
        __aeabi_memcpy(puVar5,puVar6,local_44 * 2 + 2);
        AbyssEngine::String::~String(aSStack_4c);
        AbyssEngine::String::~String(aSStack_40);
        uVar4 = uVar4 - 1;
      }
switchD_0008f07c_caseD_624:
      uVar9 = 0;
      do {
        if (9 < uVar9) goto LAB_0008f15c;
        iVar11 = uVar9 * 4;
        uVar9 = uVar9 + 1;
      } while (*(uint *)(DAT_0008f2b0 + 0x8f126 + iVar11) != (uint)uVar1);
      puVar10 = (uint *)(DAT_0008f2b4 + 0x8f13a);
      for (uVar9 = 0; uVar9 < 0x29; uVar9 = uVar9 + 1) {
        if (*puVar10 == (uint)puVar5[uVar4]) {
          if (((int)uVar4 < (int)uVar3) && (uVar1 = puVar5[uVar4 + 1], 0x5ff < uVar1)) {
            iVar11 = 1;
            if (uVar1 != 0x60c && uVar1 != 0x61f) {
              iVar11 = 4;
            }
          }
          else {
            iVar11 = 1;
          }
          puVar5[uVar4] = (ushort)puVar10[iVar11];
          break;
        }
        puVar10 = puVar10 + 5;
      }
    }
    uVar4 = uVar4 - 1;
  } while( true );
}

```

## target disasm
```
  0008efe0: push {r4,r5,r6,r7,lr}
  0008efe2: add r7,sp,#0xc
  0008efe4: push {r8,r9,r10,r11}
  0008efe8: sub sp,#0x44
  0008efea: mov r8,r0
  0008efec: ldr r0,[0x0008f2a8]
  0008efee: mov r5,r2
  0008eff0: mov r1,r2
  0008eff2: add r0,pc
  0008eff4: movs r2,#0x0
  0008eff6: ldr r4,[r0,#0x0]
  0008eff8: ldr r0,[r4,#0x0]
  0008effa: str r0,[sp,#0x40]
  0008effc: add r0,sp,#0x34
  0008effe: blx 0x0006f028
  0008f002: ldr r0,[sp,#0x3c]
  0008f004: adds r0,#0x1
  0008f006: adds r0,r0,r0
  0008f008: it cs
  0008f00a: mov.cs.w r0,#0xffffffff
  0008f00e: blx 0x0006ec08
  0008f012: mov r6,r0
  0008f014: add r0,sp,#0x34
  0008f016: strd r4,r8,[sp,#0x0]
  0008f01a: blx 0x0006f3ac
  0008f01e: mov r1,r0
  0008f020: ldr r0,[sp,#0x3c]
  0008f022: movs r2,#0x2
  0008f024: add.w r2,r2,r0, lsl #0x1
  0008f028: mov r0,r6
  0008f02a: blx 0x0006f1e4
  0008f02e: ldr.w r10,[r5,#0x8]
  0008f032: movw r8,#0x60c
  0008f036: movw r11,#0x61f
  0008f03a: str r5,[sp,#0xc]
  0008f03c: sub.w r9,r10,#0x1
  0008f040: str.w r10,[sp,#0x8]
  0008f044: mov r4,r9
  0008f046: b 0x0008f234
  0008f048: cbz r4,0x0008f08a
  0008f04a: sub.w r8,r4,#0x1
  0008f04e: ldr.w r11,[sp,#0xc]
  0008f052: mov r1,r8
  0008f054: mov r0,r11
  0008f056: blx 0x0006f3b8
  0008f05a: ldrh r5,[r0,#0x0]
  0008f05c: mov r0,r11
  0008f05e: mov r1,r4
  0008f060: blx 0x0006f3b8
  0008f064: movw r1,#0x644
  0008f068: cmp r5,r1
  0008f06a: bne 0x0008f112
  0008f06c: ldrh r0,[r0,#0x0]
  0008f06e: movw r11,#0x61f
  0008f072: subw r1,r0,#0x622
  0008f076: cmp r1,#0x5
  0008f078: bhi 0x0008f116
  0008f07a: movs r0,#0x2
  0008f07c: tbb [pc,r1]
  0008f086: movs r0,#0x5
  0008f088: b 0x0008f094
  0008f08a: movs r4,#0x0
  0008f08c: b 0x0008f15c
  0008f08e: movs r0,#0x3
  0008f090: b 0x0008f094
  0008f092: movs r0,#0x4
  0008f094: strh.w r0,[r6,r4,lsl #0x1]
  0008f098: add.w r10,sp,#0x28
  0008f09c: mov r1,r6
  0008f09e: movs r2,#0x0
  0008f0a0: mov r0,r10
  0008f0a2: blx 0x0006ef50
  0008f0a6: add r0,sp,#0x1c
  0008f0a8: mov r1,r10
  0008f0aa: movs r2,#0x0
  0008f0ac: mov r3,r8
  0008f0ae: blx 0x0006f604
  0008f0b2: ldr r3,[sp,#0x30]
  0008f0b4: add.w r11,sp,#0x10
  0008f0b8: mov r1,r10
  0008f0ba: mov r2,r4
  0008f0bc: mov r0,r11
  0008f0be: blx 0x0006f604
  0008f0c2: add r4,sp,#0x1c
  0008f0c4: mov r1,r11
  0008f0c6: mov r0,r4
  0008f0c8: blx 0x0006ef5c
  0008f0cc: mov r0,r11
  0008f0ce: blx 0x0006ee30
  0008f0d2: mov r0,r6
  0008f0d4: blx 0x0006ebfc
  0008f0d8: ldr r0,[sp,#0x24]
  0008f0da: adds r0,#0x1
  0008f0dc: adds r0,r0,r0
  0008f0de: it cs
  0008f0e0: mov.cs.w r0,#0xffffffff
  0008f0e4: blx 0x0006ec08
  0008f0e8: mov r6,r0
  0008f0ea: mov r0,r4
  0008f0ec: blx 0x0006f3ac
  0008f0f0: mov r1,r0
  0008f0f2: ldr r0,[sp,#0x24]
  0008f0f4: movs r2,#0x2
  0008f0f6: add.w r2,r2,r0, lsl #0x1
  0008f0fa: mov r0,r6
  0008f0fc: blx 0x0006f1e4
  0008f100: mov r0,r4
  0008f102: blx 0x0006ee30
  0008f106: add r0,sp,#0x28
  0008f108: blx 0x0006ee30
  0008f10c: mov r4,r8
  0008f10e: ldr.w r10,[sp,#0x8]
  0008f112: movw r11,#0x61f
  0008f116: movs r0,#0x0
  0008f118: movw r8,#0x60c
  0008f11c: cmp r0,#0x9
  0008f11e: bhi 0x0008f15c
  0008f120: ldr r1,[0x0008f2b0]
  0008f122: add r1,pc
  0008f124: ldr.w r1,[r1,r0,lsl #0x2]
  0008f128: adds r0,#0x1
  0008f12a: cmp r1,r5
  0008f12c: bne 0x0008f11c
  0008f12e: ldr r0,[0x0008f2b4]
  0008f130: movs r2,#0x0
  0008f132: ldrh.w r1,[r6,r4,lsl #0x1]
  0008f136: add r0,pc
  0008f138: b 0x0008f13e
  0008f13a: adds r0,#0x14
  0008f13c: adds r2,#0x1
  0008f13e: cmp r2,#0x28
  0008f140: bhi 0x0008f232
  0008f142: ldr r3,[r0,#0x0]
  0008f144: cmp r3,r1
  0008f146: bne 0x0008f13a
  0008f148: cmp r4,r9
  0008f14a: bge 0x0008f158
  0008f14c: add.w r1,r6,r4, lsl #0x1
  0008f150: ldrh r2,[r1,#0x2]
  0008f152: cmp.w r2,#0x600
  0008f156: bcs 0x0008f220
  0008f158: movs r1,#0x1
  0008f15a: b 0x0008f22a
  0008f15c: sub.w lr,r4,#0x1
  0008f160: add.w r12,r4,#0x1
  0008f164: movs r2,#0x0
  0008f166: cmp.w r2,#0x334
  0008f16a: beq 0x0008f232
  0008f16c: ldr r3,[0x0008f2ac]
  0008f16e: ldrh.w r0,[r6,r4,lsl #0x1]
  0008f172: add r3,pc
  0008f174: ldr r1,[r3,r2]
  0008f176: cmp r1,r0
  0008f178: bne 0x0008f208
  0008f17a: cbz r4,0x0008f1a8
  0008f17c: cmp r4,r9
  0008f17e: bge 0x0008f1c6
  0008f180: ldrh.w r1,[r6,r12,lsl #0x1]
  0008f184: cmp.w r1,#0x600
  0008f188: bcc 0x0008f192
  0008f18a: cmp r1,r11
  0008f18c: it ne
  0008f18e: cmp.ne r1,r8
  0008f190: bne 0x0008f1e4
  0008f192: ldrh.w r0,[r6,lr,lsl #0x1]
  0008f196: cmp.w r0,#0x600
  0008f19a: bcc 0x0008f1d0
  0008f19c: movs r5,#0x1
  0008f19e: cmp r0,r8
  0008f1a0: beq 0x0008f1fe
  0008f1a2: cmp r0,r11
  0008f1a4: beq 0x0008f1fe
  0008f1a6: b 0x0008f1f2
  0008f1a8: cmp.w r10,#0x1
  0008f1ac: beq 0x0008f1d0
  0008f1ae: ldrh.w r0,[r6,r12,lsl #0x1]
  0008f1b2: cmp.w r0,#0x600
  0008f1b6: bcc 0x0008f1d0
  0008f1b8: cmp r0,r8
  0008f1ba: mov.w r5,#0x1
  0008f1be: it ne
  0008f1c0: cmp.ne r0,r11
  0008f1c2: beq 0x0008f1fe
  0008f1c4: b 0x0008f1ee
  0008f1c6: ldrh.w r0,[r6,lr,lsl #0x1]
  0008f1ca: cmp.w r0,#0x600
  0008f1ce: bcs 0x0008f1d4
  0008f1d0: movs r5,#0x1
  0008f1d2: b 0x0008f1fe
  0008f1d4: cmp r0,r8
  0008f1d6: mov.w r5,#0x1
  0008f1da: it ne
  0008f1dc: cmp.ne r0,r11
  0008f1de: beq 0x0008f1fe
  0008f1e0: movs r5,#0x2
  0008f1e2: b 0x0008f1fe
  0008f1e4: ldrh.w r0,[r6,lr,lsl #0x1]
  0008f1e8: cmp.w r0,#0x600
  0008f1ec: bcs 0x0008f1f2
  0008f1ee: movs r5,#0x4
  0008f1f0: b 0x0008f1fe
  0008f1f2: cmp r0,r8
  0008f1f4: mov.w r5,#0x4
  0008f1f8: it ne
  0008f1fa: cmp.ne r0,r11
  0008f1fc: bne 0x0008f20c
  0008f1fe: add.w r0,r3,r5, lsl #0x2
  0008f202: ldr r0,[r0,r2]
  0008f204: strh.w r0,[r6,r4,lsl #0x1]
  0008f208: adds r2,#0x14
  0008f20a: b 0x0008f166
  0008f20c: cmp.w r1,#0x600
  0008f210: bcc 0x0008f1e0
  0008f212: movs r5,#0x2
  0008f214: cmp r1,r8
  0008f216: beq 0x0008f1fe
  0008f218: cmp r1,r11
  0008f21a: it ne
  0008f21c: mov.ne r5,#0x3
  0008f21e: b 0x0008f1fe
  0008f220: movs r1,#0x1
  0008f222: cmp r2,r8
  0008f224: itt ne
  0008f226: cmp.ne r2,r11
  0008f228: mov.ne r1,#0x4
  0008f22a: ldr.w r0,[r0,r1,lsl #0x2]
  0008f22e: strh.w r0,[r6,r4,lsl #0x1]
  0008f232: subs r4,#0x1
  0008f234: cmp.w r4,#0xffffffff
  0008f238: bgt.w 0x0008f048
  0008f23c: ldr r0,[sp,#0x4]
  0008f23e: mov r1,r6
  0008f240: movs r2,#0x0
  0008f242: blx 0x0006ef50
  0008f246: cmp r6,#0x0
  0008f248: ldr r4,[sp,#0x0]
  0008f24a: itt ne
  0008f24c: mov.ne r0,r6
  0008f24e: blx.ne 0x0006ebfc
  0008f252: add r0,sp,#0x34
  0008f254: blx 0x0006ee30
  0008f258: ldr r0,[sp,#0x40]
  0008f25a: ldr r1,[r4,#0x0]
  0008f25c: subs r0,r1,r0
  0008f25e: ittt eq
  0008f260: add.eq sp,#0x44
  0008f262: pop.eq.w {r8,r9,r10,r11}
  0008f266: pop.eq {r4,r5,r6,r7,pc}
  0008f268: blx 0x0006e824
```
