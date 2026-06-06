# AEFile::Open
elf 0x708c4 body 440
Sig: undefined __stdcall Open(String * param_1, FileOpenType param_2, uint * param_3)

## decompile
```c

/* AEFile::Open(AbyssEngine::String&, AEFile::FileOpenType, unsigned int*) */

void AEFile::Open(String *param_1,int param_2,uint *param_3)

{
  undefined1 *puVar1;
  ushort *puVar2;
  int iVar3;
  short *psVar4;
  uint uVar5;
  undefined4 uVar6;
  Array *pAVar7;
  int *piVar8;
  uint uVar9;
  code *pcVar10;
  int *piVar11;
  AELowLevelFile *pAVar12;
  int *piVar13;
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar11 = *(int **)(DAT_00080ab0 + 0x808da);
  piVar13 = *(int **)(DAT_00080ab4 + 0x808de);
  local_28 = *piVar13;
  if (*piVar11 == 0) {
LAB_00080a32:
    uVar6 = 0;
  }
  else {
    puVar2 = AbyssEngine::String::operator_cast_to_unsigned_short_(param_1);
    pAVar12 = (AELowLevelFile *)0x0;
    if (param_2 == 0) {
      piVar11 = (int *)*piVar11;
      AbyssEngine::String::String(aSStack_64,puVar2,false);
      iVar3 = (**(code **)(*piVar11 + 0xc))(piVar11,aSStack_64,*(undefined4 *)(param_1 + 8),0,0);
      puVar1 = &stack0xfffffff8;
LAB_000809da:
      AbyssEngine::String::~String((String *)(puVar1 + -0x5c));
      if (iVar3 == 0) goto LAB_00080a32;
LAB_000809e0:
      pAVar12 = operator_new(8);
      *(int *)pAVar12 = *(int *)(DAT_00080ab8 + 0x809ee) + 8;
      *(int *)(pAVar12 + 4) = iVar3;
    }
    else {
      if (param_2 == 2) {
        piVar11 = (int *)*piVar11;
        AbyssEngine::String::String(aSStack_70,puVar2,false);
        iVar3 = (**(code **)(*piVar11 + 0x10))(piVar11,aSStack_70,*(undefined4 *)(param_1 + 8),0,0);
        puVar1 = &stack0xffffffec;
        goto LAB_000809da;
      }
      if (param_2 == 1) {
        piVar11 = (int *)*piVar11;
        AbyssEngine::String::String(aSStack_34,puVar2,false);
        iVar3 = (**(code **)(*piVar11 + 8))(piVar11,aSStack_34,*(undefined4 *)(param_1 + 8),0,0,0,0)
        ;
        AbyssEngine::String::~String(aSStack_34);
        if (iVar3 == 0) {
          psVar4 = (short *)AbyssEngine::String::operator[](param_1,0);
          if (*psVar4 != 0x2f) {
            AbyssEngine::String::String(aSStack_4c,(char *)(DAT_00080ac0 + 0x80950),false);
            AbyssEngine::String::String(aSStack_58,param_1,false);
            AbyssEngine::operator+(aSStack_40,aSStack_4c);
            AbyssEngine::String::operator=(param_1,aSStack_40);
            pcVar10 = *(code **)(DAT_00080ac4 + 0x80974);
            (*pcVar10)(aSStack_40);
            (*pcVar10)(aSStack_58);
            (*pcVar10)(aSStack_4c);
          }
          pAVar12 = (AELowLevelFile *)findPakFile(param_1);
          if (pAVar12 == (AELowLevelFile *)0x0) goto LAB_00080a32;
          goto LAB_000809f4;
        }
        goto LAB_000809e0;
      }
    }
LAB_000809f4:
    piVar11 = *(int **)(DAT_00080abc + 0x809fe);
    pAVar7 = (Array *)*piVar11;
    uVar5 = *(uint *)pAVar7;
    if (param_3 == (uint *)0x0) {
      if (uVar5 == 0) {
        ArrayAdd<AELowLevelFile*>(pAVar12,pAVar7);
      }
      else {
        piVar8 = *(int **)(pAVar7 + 4);
        if ((int *)*piVar8 != (int *)0x0) {
          (**(code **)(*(int *)*piVar8 + 4))();
          piVar8 = *(int **)(*piVar11 + 4);
        }
        *piVar8 = 0;
        **(undefined4 **)(*piVar11 + 4) = pAVar12;
      }
    }
    else {
      for (uVar9 = 1; uVar9 < uVar5; uVar9 = uVar9 + 1) {
        if (*(int *)(*(int *)(pAVar7 + 4) + uVar9 * 4) == 0) {
          *(AELowLevelFile **)(*(int *)(pAVar7 + 4) + uVar9 * 4) = pAVar12;
          *param_3 = uVar9;
          goto LAB_00080a64;
        }
      }
      if (uVar5 == 0) {
        ArrayAdd<AELowLevelFile*>((AELowLevelFile *)0x0,pAVar7);
        pAVar7 = (Array *)*piVar11;
      }
      ArrayAdd<AELowLevelFile*>(pAVar12,pAVar7);
      *param_3 = *(int *)*piVar11 - 1;
    }
LAB_00080a64:
    uVar6 = 1;
  }
  if (*piVar13 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

```
## target disasm
```
  000808c4: push {r4,r5,r6,r7,lr}
  000808c6: add r7,sp,#0xc
  000808c8: push {r8,r9,r10,r11}
  000808cc: sub sp,#0x5c
  000808ce: mov r11,r0
  000808d0: ldr r0,[0x00080ab0]
  000808d2: mov r4,r1
  000808d4: ldr r1,[0x00080ab4]
  000808d6: add r0,pc
  000808d8: mov r8,r2
  000808da: add r1,pc
  000808dc: ldr r5,[r0,#0x0]
  000808de: ldr.w r10,[r1,#0x0]
  000808e2: ldr r0,[r5,#0x0]
  000808e4: ldr.w r1,[r10,#0x0]
  000808e8: cmp r0,#0x0
  000808ea: str r1,[sp,#0x58]
  000808ec: beq.w 0x00080a32
  000808f0: mov r0,r11
  000808f2: blx 0x0006f3ac
  000808f6: mov r1,r0
  000808f8: movs r6,#0x0
  000808fa: cmp r4,#0x0
  000808fc: beq 0x0008098c
  000808fe: cmp r4,#0x2
  00080900: beq 0x000809b2
  00080902: cmp r4,#0x1
  00080904: bne 0x000809f4
  00080906: add.w r9,sp,#0x4c
  0008090a: movs r2,#0x0
  0008090c: ldr r4,[r5,#0x0]
  0008090e: movs r5,#0x0
  00080910: mov r0,r9
  00080912: blx 0x0006ef50
  00080916: ldr r0,[r4,#0x0]
  00080918: ldr.w r2,[r11,#0x8]
  0008091c: ldr r6,[r0,#0x8]
  0008091e: mov r0,r4
  00080920: mov r1,r9
  00080922: movs r3,#0x0
  00080924: strd r5,r5,[sp,#0x0]
  00080928: str r5,[sp,#0x8]
  0008092a: blx r6
  0008092c: mov r4,r0
  0008092e: add r0,sp,#0x4c
  00080930: blx 0x0006ee30
  00080934: cmp r4,#0x0
  00080936: bne 0x000809e0
  00080938: mov r0,r11
  0008093a: movs r1,#0x0
  0008093c: blx 0x0006f3b8
  00080940: ldrh r0,[r0,#0x0]
  00080942: cmp r0,#0x2f
  00080944: beq 0x00080980
  00080946: ldr r1,[0x00080ac0]
  00080948: add r0,sp,#0x34
  0008094a: movs r2,#0x0
  0008094c: add r1,pc
  0008094e: blx 0x0006ee18
  00080952: add r0,sp,#0x28
  00080954: mov r1,r11
  00080956: movs r2,#0x0
  00080958: blx 0x0006f028
  0008095c: add r0,sp,#0x40
  0008095e: add r1,sp,#0x34
  00080960: add r2,sp,#0x28
  00080962: blx 0x0006ef98
  00080966: add r1,sp,#0x40
  00080968: mov r0,r11
  0008096a: blx 0x0006f2b0
  0008096e: ldr r0,[0x00080ac4]
  00080970: add r0,pc
  00080972: ldr r4,[r0,#0x0]
  00080974: add r0,sp,#0x40
  00080976: blx r4
  00080978: add r0,sp,#0x28
  0008097a: blx r4
  0008097c: add r0,sp,#0x34
  0008097e: blx r4
  00080980: mov r0,r11
  00080982: blx 0x0006f3c4
  00080986: mov r6,r0
  00080988: cbnz r0,0x000809f4
  0008098a: b 0x00080a32
  0008098c: add.w r9,sp,#0x1c
  00080990: movs r2,#0x0
  00080992: ldr r4,[r5,#0x0]
  00080994: mov r0,r9
  00080996: blx 0x0006ef50
  0008099a: ldr r0,[r4,#0x0]
  0008099c: ldr.w r2,[r11,#0x8]
  000809a0: ldr r5,[r0,#0xc]
  000809a2: mov r0,r4
  000809a4: mov r1,r9
  000809a6: movs r3,#0x0
  000809a8: str r6,[sp,#0x0]
  000809aa: blx r5
  000809ac: mov r4,r0
  000809ae: add r0,sp,#0x1c
  000809b0: b 0x000809da
  000809b2: add r6,sp,#0x10
  000809b4: movs r2,#0x0
  000809b6: ldr r4,[r5,#0x0]
  000809b8: mov.w r9,#0x0
  000809bc: mov r0,r6
  000809be: blx 0x0006ef50
  000809c2: ldr r0,[r4,#0x0]
  000809c4: ldr.w r2,[r11,#0x8]
  000809c8: ldr r5,[r0,#0x10]
  000809ca: mov r0,r4
  000809cc: mov r1,r6
  000809ce: movs r3,#0x0
  000809d0: str.w r9,[sp,#0x0]
  000809d4: blx r5
  000809d6: mov r4,r0
  000809d8: add r0,sp,#0x10
  000809da: blx 0x0006ee30
  000809de: cbz r4,0x00080a32
  000809e0: movs r0,#0x8
  000809e2: blx 0x0006eb24
  000809e6: mov r6,r0
  000809e8: ldr r0,[0x00080ab8]
  000809ea: add r0,pc
  000809ec: ldr r0,[r0,#0x0]
  000809ee: adds r0,#0x8
  000809f0: str r0,[r6,#0x0]
  000809f2: str r4,[r6,#0x4]
  000809f4: ldr r0,[0x00080abc]
  000809f6: cmp.w r8,#0x0
  000809fa: add r0,pc
  000809fc: ldr r4,[r0,#0x0]
  000809fe: ldr r1,[r4,#0x0]
  00080a00: ldr r0,[r1,#0x0]
  00080a02: beq 0x00080a36
  00080a04: movs r2,#0x1
  00080a06: b 0x00080a12
  00080a08: ldr r3,[r1,#0x4]
  00080a0a: ldr.w r5,[r3,r2,lsl #0x2]
  00080a0e: cbz r5,0x00080a54
  00080a10: adds r2,#0x1
  00080a12: cmp r2,r0
  00080a14: bcc 0x00080a08
  00080a16: cbnz r0,0x00080a20
  00080a18: movs r0,#0x0
  00080a1a: blx 0x0006f3d0
  00080a1e: ldr r1,[r4,#0x0]
  00080a20: mov r0,r6
  00080a22: blx 0x0006f3d0
  00080a26: ldr r0,[r4,#0x0]
  00080a28: ldr r0,[r0,#0x0]
  00080a2a: subs r0,#0x1
  00080a2c: str.w r0,[r8,#0x0]
  00080a30: b 0x00080a64
  00080a32: movs r0,#0x0
  00080a34: b 0x00080a66
  00080a36: cbz r0,0x00080a5e
  00080a38: ldr r1,[r1,#0x4]
  00080a3a: ldr r0,[r1,#0x0]
  00080a3c: cbz r0,0x00080a48
  00080a3e: ldr r1,[r0,#0x0]
  00080a40: ldr r1,[r1,#0x4]
  00080a42: blx r1
  00080a44: ldr r0,[r4,#0x0]
  00080a46: ldr r1,[r0,#0x4]
  00080a48: movs r0,#0x0
  00080a4a: str r0,[r1,#0x0]
  00080a4c: ldr r0,[r4,#0x0]
  00080a4e: ldr r0,[r0,#0x4]
  00080a50: str r6,[r0,#0x0]
  00080a52: b 0x00080a64
  00080a54: str.w r6,[r3,r2,lsl #0x2]
  00080a58: str.w r2,[r8,#0x0]
  00080a5c: b 0x00080a64
  00080a5e: mov r0,r6
  00080a60: blx 0x0006f3d0
  00080a64: movs r0,#0x1
  00080a66: ldr r1,[sp,#0x58]
  00080a68: ldr.w r2,[r10,#0x0]
  00080a6c: subs r1,r2,r1
  00080a6e: ittt eq
  00080a70: add.eq sp,#0x5c
  00080a72: pop.eq.w {r8,r9,r10,r11}
  00080a76: pop.eq {r4,r5,r6,r7,pc}
  00080a78: blx 0x0006e824
```
