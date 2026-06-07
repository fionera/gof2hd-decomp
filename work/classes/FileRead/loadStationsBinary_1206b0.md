# FileRead::loadStationsBinary
elf 0x1206b0 body 224
Sig: undefined __stdcall loadStationsBinary(void)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* FileRead::loadStationsBinary() */

void FileRead::loadStationsBinary(void)

{
  String *pSVar1;
  int iVar2;
  uint *extraout_r2;
  uint uVar3;
  int *piVar4;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  uint local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_001307a4 + 0x1306c2);
  local_28 = *piVar4;
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_34,(char *)(DAT_001307a8 + 0x1306d2),false)
  ;
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_001307ac + 0x1306ec),(char *)&local_38,extraout_r2);
    ArraySetLength<signed_char>(0x195,(Array *)0x0);
    AbyssEngine::String::String(aSStack_34);
    local_48 = 0;
    for (uVar3 = 0; uVar3 < 0x87; uVar3 = uVar3 + 1) {
      AEFile::ReadSwitched(aSStack_34,local_38);
      AEFile::ReadSwitched(&local_48,local_38);
      AEFile::ReadSwitched(&local_3c,local_38);
      AEFile::ReadSwitched(&local_40,local_38);
      AEFile::ReadSwitched(&local_44,local_38);
      *(char *)(_DAT_00000004 + local_48) = (char)local_3c;
      iVar2 = local_48 + 2;
      *(char *)(_DAT_00000004 + local_48 + 1) = (char)local_40;
      local_48 = local_48 + 3;
      *(char *)(_DAT_00000004 + iVar2) = (char)local_44;
    }
    AEFile::Close(local_38);
    AbyssEngine::String::~String(aSStack_34);
  }
  iVar2 = *piVar4 - local_28;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  001306b0: push {r4,r5,r6,r7,lr}
  001306b2: add r7,sp,#0xc
  001306b4: push {r8,r9,r10,r11}
  001306b8: sub sp,#0x24
  001306ba: ldr r0,[0x001307a4]
  001306bc: movs r2,#0x0
  001306be: add r0,pc
  001306c0: ldr.w r11,[r0,#0x0]
  001306c4: ldr.w r0,[r11,#0x0]
  001306c8: ldr r1,[0x001307a8]
  001306ca: str r0,[sp,#0x20]
  001306cc: add r0,sp,#0x14
  001306ce: add r1,pc
  001306d0: blx 0x0006ee18
  001306d4: blx 0x0006fff4
  001306d8: mov r4,r0
  001306da: add r0,sp,#0x14
  001306dc: blx 0x0006ee30
  001306e0: cmp r4,#0x0
  001306e2: beq 0x00130778
  001306e4: ldr r0,[0x001307ac]
  001306e6: add r1,sp,#0x10
  001306e8: add r0,pc
  001306ea: blx 0x0006ecd4
  001306ee: movw r0,#0x195
  001306f2: movs r1,#0x0
  001306f4: movs r6,#0x0
  001306f6: blx 0x0007633c
  001306fa: add r4,sp,#0x14
  001306fc: mov r0,r4
  001306fe: blx 0x0006efbc
  00130702: add.w r10,sp,#0xc
  00130706: add.w r8,sp,#0x8
  0013070a: mov r5,sp
  0013070c: mov.w r9,#0x4
  00130710: str r6,[sp,#0x0]
  00130712: b 0x00130766
  00130714: mov r0,r4
  00130716: blx 0x000762f4
  0013071a: ldr r1,[sp,#0x10]
  0013071c: mov r0,r5
  0013071e: blx 0x0007630c
  00130722: ldr r1,[sp,#0x10]
  00130724: mov r0,r10
  00130726: blx 0x0007630c
  0013072a: ldr r1,[sp,#0x10]
  0013072c: mov r0,r8
  0013072e: blx 0x0007630c
  00130732: ldr r1,[sp,#0x10]
  00130734: add r0,sp,#0x4
  00130736: blx 0x0007630c
  0013073a: ldr r0,[sp,#0x0]
  0013073c: adds r6,#0x1
  0013073e: ldr r2,[sp,#0xc]
  00130740: adds r1,r0,#0x1
  00130742: str r1,[sp,#0x0]
  00130744: ldr.w r1,[r9,#0x0]
  00130748: strb r2,[r1,r0]
  0013074a: ldr r0,[sp,#0x0]
  0013074c: ldr r2,[sp,#0x8]
  0013074e: adds r1,r0,#0x1
  00130750: str r1,[sp,#0x0]
  00130752: ldr.w r1,[r9,#0x0]
  00130756: strb r2,[r1,r0]
  00130758: ldr r0,[sp,#0x0]
  0013075a: ldr r2,[sp,#0x4]
  0013075c: adds r1,r0,#0x1
  0013075e: str r1,[sp,#0x0]
  00130760: ldr.w r1,[r9,#0x0]
  00130764: strb r2,[r1,r0]
  00130766: ldr r1,[sp,#0x10]
  00130768: cmp r6,#0x87
  0013076a: bcc 0x00130714
  0013076c: mov r0,r1
  0013076e: blx 0x0006ece0
  00130772: add r0,sp,#0x14
  00130774: blx 0x0006ee30
  00130778: ldr r0,[sp,#0x20]
  0013077a: ldr.w r1,[r11,#0x0]
  0013077e: subs r0,r1,r0
  00130780: itttt eq
  00130782: mov.eq r0,#0x0
  00130784: add.eq sp,#0x24
  00130786: pop.eq.w {r8,r9,r10,r11}
  0013078a: pop.eq {r4,r5,r6,r7,pc}
  0013078c: blx 0x0006e824
```
