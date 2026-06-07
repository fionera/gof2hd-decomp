# FileInterfaceAndroid::OpenRead
elf 0x6dfe0 body 430
Sig: undefined __thiscall OpenRead(FileInterfaceAndroid * this, String param_1, int param_2, bool param_3, int param_4, int param_5, uint param_6)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* FileInterfaceAndroid::OpenRead(AbyssEngine::String, int, bool, int, int, unsigned int) */

void __thiscall
FileInterfaceAndroid::OpenRead
          (FileInterfaceAndroid *this,undefined4 param_2,undefined4 param_3,int param_4,int param_5,
          undefined4 param_6,int param_7)

{
  ushort uVar1;
  ushort *puVar2;
  undefined4 uVar3;
  zip_file *pzVar4;
  zip_file *pzVar5;
  FileInterfaceAndroid *pFVar6;
  int *piVar7;
  undefined4 *puVar8;
  int *piVar9;
  int iVar10;
  undefined4 uVar11;
  ushort *puVar12;
  int iVar13;
  int *piVar14;
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar14 = *(int **)(_LAB_0007e1e0 + 0x7dff6);
  local_28 = *piVar14;
  puVar2 = (ushort *)AbyssEngine::String::GetAEWChar();
  if (this[4] != (FileInterfaceAndroid)0x0) {
    uVar1 = *puVar2;
    AbyssEngine::String::String(aSStack_34,(char *)(_LAB_0007e1e4 + 0x7e01e),false);
    puVar12 = puVar2 + 1;
    if (uVar1 != 0x2f) {
      puVar12 = puVar2;
    }
    AbyssEngine::String::String(aSStack_40,puVar12,false);
    AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
    AbyssEngine::String::~String(aSStack_40);
    AbyssEngine::String::String(aSStack_40,(char *)(_LAB_0007e1e8 + 0x7e044),false);
    AbyssEngine::String::String(aSStack_4c,puVar12,false);
    AbyssEngine::String::operator+=(aSStack_40,aSStack_4c);
    AbyssEngine::String::~String(aSStack_4c);
    uVar3 = AbyssEngine::String::GetAEChar();
    iVar10 = param_4;
    fprintf(*(int *)(_LAB_0007e1f0 + 0x7e07a) + 0xa8,_LAB_0007e1ec + 0x7e078,uVar3,param_3,param_4,
            param_5,param_6,param_7);
    uVar11 = **(undefined4 **)(_LAB_0007e1f4 + 0x7e090);
    uVar3 = AbyssEngine::String::GetAEChar();
    pzVar4 = (zip_file *)zip_fopen(uVar11,uVar3,0);
    iVar13 = **(int **)(_LAB_0007e1f8 + 0x7e0aa);
    if (iVar13 == 0) {
      pzVar5 = (zip_file *)0x0;
    }
    else {
      uVar3 = AbyssEngine::String::GetAEChar();
      pzVar5 = (zip_file *)zip_fopen(iVar13,uVar3,0);
    }
    if (pzVar4 == (zip_file *)0x0) {
      if (pzVar5 == (zip_file *)0x0) {
        if (*(char **)(this + 0x30) != (char *)0x0) {
          AbyssEngine::String::String(aSStack_4c,*(char **)(this + 0x30),false);
          AbyssEngine::String::String(aSStack_64,puVar12,false);
          AbyssEngine::operator+(aSStack_58,aSStack_4c);
          AbyssEngine::String::~String(aSStack_64);
          uVar3 = AbyssEngine::String::GetAEChar();
          iVar10 = fopen(uVar3,_LAB_0007e1fc + 0x7e128);
          if (iVar10 != 0) {
            piVar7 = operator_new(0x38);
            iVar13 = _LAB_0007e204;
            puVar8 = (undefined4 *)(_LAB_0007e200 + 0x7e140);
            piVar7[2] = iVar10;
            piVar7[3] = 0;
            piVar7[4] = 0;
            piVar9 = (int *)*puVar8;
            iVar10 = *(int *)(iVar13 + 0x7e146);
            *(undefined1 *)(piVar7 + 5) = 0;
            *piVar7 = iVar10 + 8;
            *piVar9 = *piVar9 + 1;
          }
          AbyssEngine::String::~String(aSStack_58);
          AbyssEngine::String::~String(aSStack_4c);
        }
      }
      else {
        pFVar6 = operator_new(0x38);
        FileInterfaceAndroid(pFVar6,pzVar5,SUB41(param_4,0),param_7,iVar10,param_5);
      }
    }
    else {
      pFVar6 = operator_new(0x38);
      FileInterfaceAndroid(pFVar6,pzVar4,SUB41(param_4,0),param_7,iVar10,param_5);
    }
    AbyssEngine::String::~String(aSStack_40);
    AbyssEngine::String::~String(aSStack_34);
  }
  iVar10 = *piVar14 - local_28;
  if (iVar10 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar10);
}

```

## target disasm
```
  0007dfe0: push {r4,r5,r6,r7,lr}
  0007dfe2: add r7,sp,#0xc
  0007dfe4: push {r8,r9,r10,r11}
  0007dfe8: sub sp,#0x54
  0007dfea: mov r10,r0
  0007dfec: ldr r0,[0x0007e1e0]
  0007dfee: mov r8,r3
  0007dff0: mov r9,r2
  0007dff2: add r0,pc
  0007dff4: ldr.w r11,[r0,#0x0]
  0007dff8: ldr.w r0,[r11,#0x0]
  0007dffc: str r0,[sp,#0x50]
  0007dffe: mov r0,r1
  0007e000: blx 0x0006ef44
  0007e004: mov r6,r0
  0007e006: ldrb.w r0,[r10,#0x4]
  0007e00a: cmp r0,#0x0
  0007e00c: beq 0x0007e0c2
  0007e00e: mov r5,r6
  0007e010: add r0,sp,#0x44
  0007e012: ldrh.w r4,[r5],#0x2
  0007e016: movs r2,#0x0
  0007e018: ldr r1,[0x0007e1e4]
  0007e01a: add r1,pc
  0007e01c: blx 0x0006ee18
  0007e020: cmp r4,#0x2f
  0007e022: it ne
  0007e024: mov.ne r5,r6
  0007e026: add r0,sp,#0x38
  0007e028: mov r1,r5
  0007e02a: movs r2,#0x0
  0007e02c: blx 0x0006ef50
  0007e030: add r0,sp,#0x44
  0007e032: add r1,sp,#0x38
  0007e034: blx 0x0006ef5c
  0007e038: add r0,sp,#0x38
  0007e03a: blx 0x0006ee30
  0007e03e: ldr r1,[0x0007e1e8]
  0007e040: add r1,pc
  0007e042: add r0,sp,#0x38
  0007e044: movs r2,#0x0
  0007e046: blx 0x0006ee18
  0007e04a: add r0,sp,#0x2c
  0007e04c: mov r1,r5
  0007e04e: movs r2,#0x0
  0007e050: blx 0x0006ef50
  0007e054: add r0,sp,#0x38
  0007e056: add r1,sp,#0x2c
  0007e058: blx 0x0006ef5c
  0007e05c: add r0,sp,#0x2c
  0007e05e: blx 0x0006ee30
  0007e062: add r0,sp,#0x44
  0007e064: blx 0x0006ef68
  0007e068: mov r2,r0
  0007e06a: ldrd r3,r0,[r7,#0x8]
  0007e06e: ldr r1,[0x0007e1ec]
  0007e070: ldr r4,[0x0007e1f0]
  0007e072: ldr r6,[r7,#0x10]
  0007e074: add r1,pc
  0007e076: add r4,pc
  0007e078: strd r0,r6,[sp,#0x8]
  0007e07c: ldr r0,[r4,#0x0]
  0007e07e: strd r8,r3,[sp,#0x0]
  0007e082: mov r3,r9
  0007e084: adds r0,#0xa8
  0007e086: blx 0x0006ef74
  0007e08a: ldr r0,[0x0007e1f4]
  0007e08c: add r0,pc
  0007e08e: ldr r0,[r0,#0x0]
  0007e090: ldr r4,[r0,#0x0]
  0007e092: add r0,sp,#0x44
  0007e094: blx 0x0006ef68
  0007e098: mov r1,r0
  0007e09a: mov r0,r4
  0007e09c: movs r2,#0x0
  0007e09e: blx 0x0006ef80
  0007e0a2: mov r4,r0
  0007e0a4: ldr r0,[0x0007e1f8]
  0007e0a6: add r0,pc
  0007e0a8: ldr r0,[r0,#0x0]
  0007e0aa: ldr r6,[r0,#0x0]
  0007e0ac: cbz r6,0x0007e0c6
  0007e0ae: add r0,sp,#0x38
  0007e0b0: blx 0x0006ef68
  0007e0b4: mov r1,r0
  0007e0b6: mov r0,r6
  0007e0b8: movs r2,#0x0
  0007e0ba: blx 0x0006ef80
  0007e0be: mov r6,r0
  0007e0c0: b 0x0007e0c8
  0007e0c2: movs r5,#0x0
  0007e0c4: b 0x0007e176
  0007e0c6: movs r6,#0x0
  0007e0c8: cbz r4,0x0007e0de
  0007e0ca: movs r0,#0x38
  0007e0cc: blx 0x0006eb24
  0007e0d0: mov r5,r0
  0007e0d2: ldr r3,[r7,#0x10]
  0007e0d4: mov r1,r4
  0007e0d6: mov r2,r8
  0007e0d8: blx 0x0006ef8c
  0007e0dc: b 0x0007e16a
  0007e0de: cbz r6,0x0007e0f4
  0007e0e0: movs r0,#0x38
  0007e0e2: blx 0x0006eb24
  0007e0e6: mov r5,r0
  0007e0e8: ldr r3,[r7,#0x10]
  0007e0ea: mov r1,r6
  0007e0ec: mov r2,r8
  0007e0ee: blx 0x0006ef8c
  0007e0f2: b 0x0007e16a
  0007e0f4: ldr.w r1,[r10,#0x30]
  0007e0f8: cbz r1,0x0007e158
  0007e0fa: add r0,sp,#0x2c
  0007e0fc: movs r2,#0x0
  0007e0fe: blx 0x0006ee18
  0007e102: add r0,sp,#0x14
  0007e104: mov r1,r5
  0007e106: movs r2,#0x0
  0007e108: blx 0x0006ef50
  0007e10c: add r0,sp,#0x20
  0007e10e: add r1,sp,#0x2c
  0007e110: add r2,sp,#0x14
  0007e112: blx 0x0006ef98
  0007e116: add r0,sp,#0x14
  0007e118: blx 0x0006ee30
  0007e11c: add r0,sp,#0x20
  0007e11e: blx 0x0006ef68
  0007e122: ldr r1,[0x0007e1fc]
  0007e124: add r1,pc
  0007e126: blx 0x0006e944
  0007e12a: mov r4,r0
  0007e12c: cbz r0,0x0007e15c
  0007e12e: movs r0,#0x38
  0007e130: blx 0x0006eb24
  0007e134: mov r5,r0
  0007e136: ldr r0,[0x0007e200]
  0007e138: ldr r1,[0x0007e204]
  0007e13a: movs r2,#0x0
  0007e13c: add r0,pc
  0007e13e: strd r4,r2,[r5,#0x8]
  0007e142: add r1,pc
  0007e144: str r2,[r5,#0x10]
  0007e146: ldr r0,[r0,#0x0]
  0007e148: ldr r1,[r1,#0x0]
  0007e14a: strb r2,[r5,#0x14]
  0007e14c: adds r1,#0x8
  0007e14e: str r1,[r5,#0x0]
  0007e150: ldr r1,[r0,#0x0]
  0007e152: adds r1,#0x1
  0007e154: str r1,[r0,#0x0]
  0007e156: b 0x0007e15e
  0007e158: movs r5,#0x0
  0007e15a: b 0x0007e16a
  0007e15c: movs r5,#0x0
  0007e15e: add r0,sp,#0x20
  0007e160: blx 0x0006ee30
  0007e164: add r0,sp,#0x2c
  0007e166: blx 0x0006ee30
  0007e16a: add r0,sp,#0x38
  0007e16c: blx 0x0006ee30
  0007e170: add r0,sp,#0x44
  0007e172: blx 0x0006ee30
  0007e176: ldr r0,[sp,#0x50]
  0007e178: ldr.w r1,[r11,#0x0]
  0007e17c: subs r0,r1,r0
  0007e17e: itttt eq
  0007e180: mov.eq r0,r5
  0007e182: add.eq sp,#0x54
  0007e184: pop.eq.w {r8,r9,r10,r11}
  0007e188: pop.eq {r4,r5,r6,r7,pc}
  0007e18a: blx 0x0006e824
```
