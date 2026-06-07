# FileRead::loadStationsBinary
elf 0x11f970 body 288
Sig: undefined __thiscall loadStationsBinary(FileRead * this, short * param_1, int param_2)

## decompile
```c

/* FileRead::loadStationsBinary(short*, int) */

void __thiscall FileRead::loadStationsBinary(FileRead *this,short *param_1,int param_2)

{
  Array<Station*> *this_00;
  String *pSVar1;
  int iVar2;
  Station *pSVar3;
  uint *extraout_r2;
  int *piVar4;
  uint uVar5;
  int iVar6;
  String aSStack_54 [12];
  int local_48;
  int iStack_44;
  int local_40;
  int iStack_3c;
  uint local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_0012fac4 + 0x12f984);
  local_28 = *piVar4;
  this_00 = operator_new(0xc);
  Array<Station*>::Array(this_00);
  ArraySetLength<Station*>(param_2,(Array *)this_00);
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_34,(char *)(DAT_0012fac8 + 0x12f9aa),false)
  ;
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar2 == 0) {
    this_00 = (Array<Station*> *)0x0;
  }
  else {
    AEFile::OpenRead((AEFile *)(DAT_0012facc + 0x12f9c8),(char *)&local_38,extraout_r2);
    AbyssEngine::String::String(aSStack_34);
    iVar2 = 0;
    for (uVar5 = 0; uVar5 < 0x87; uVar5 = uVar5 + 1) {
      AEFile::ReadSwitched(aSStack_34,local_38);
      AbyssEngine::String::ConvertFromUTF8(aSStack_34);
      AEFile::ReadSwitched(&iStack_3c,local_38);
      AEFile::ReadSwitched(&local_40,local_38);
      AEFile::ReadSwitched(&iStack_44,local_38);
      AEFile::ReadSwitched(&local_48,local_38);
      for (iVar6 = 0; iVar6 < param_2; iVar6 = iVar6 + 1) {
        if (uVar5 == (int)param_1[iVar6]) {
          pSVar3 = operator_new(0x34);
          AbyssEngine::String::String(aSStack_54,aSStack_34,false);
          Station::Station(pSVar3,aSStack_54,iStack_3c,local_40,iStack_44,local_48);
          *(Station **)(*(int *)(this_00 + 4) + iVar2 * 4) = pSVar3;
          AbyssEngine::String::~String(aSStack_54);
          iVar2 = iVar2 + 1;
        }
      }
    }
    AEFile::Close(local_38);
    AbyssEngine::String::~String(aSStack_34);
  }
  if (*piVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(this_00);
  }
  return;
}

```

## target disasm
```
  0012f970: push {r4,r5,r6,r7,lr}
  0012f972: add r7,sp,#0xc
  0012f974: push {r8,r9,r10,r11}
  0012f978: sub sp,#0x44
  0012f97a: ldr r0,[0x0012fac4]
  0012f97c: mov r5,r2
  0012f97e: mov r11,r1
  0012f980: add r0,pc
  0012f982: ldr.w r8,[r0,#0x0]
  0012f986: ldr.w r0,[r8,#0x0]
  0012f98a: str r0,[sp,#0x40]
  0012f98c: movs r0,#0xc
  0012f98e: blx 0x0006eb24
  0012f992: mov r6,r0
  0012f994: blx 0x00073594
  0012f998: mov r0,r5
  0012f99a: mov r1,r6
  0012f99c: blx 0x000735b8
  0012f9a0: ldr r1,[0x0012fac8]
  0012f9a2: add r0,sp,#0x34
  0012f9a4: movs r2,#0x0
  0012f9a6: add r1,pc
  0012f9a8: blx 0x0006ee18
  0012f9ac: blx 0x0006fff4
  0012f9b0: mov r4,r0
  0012f9b2: add r0,sp,#0x34
  0012f9b4: blx 0x0006ee30
  0012f9b8: cmp r4,#0x0
  0012f9ba: beq 0x0012fa78
  0012f9bc: strd r8,r6,[sp,#0xc]
  0012f9c0: add r1,sp,#0x30
  0012f9c2: ldr r0,[0x0012facc]
  0012f9c4: add r0,pc
  0012f9c6: blx 0x0006ecd4
  0012f9ca: add r0,sp,#0x34
  0012f9cc: blx 0x0006efbc
  0012f9d0: add.w r9,sp,#0x2c
  0012f9d4: add r6,sp,#0x14
  0012f9d6: movs r4,#0x0
  0012f9d8: mov.w r8,#0x0
  0012f9dc: b 0x0012fa5e
  0012f9de: add r0,sp,#0x34
  0012f9e0: blx 0x000762f4
  0012f9e4: add r0,sp,#0x34
  0012f9e6: blx 0x00076300
  0012f9ea: ldr r1,[sp,#0x30]
  0012f9ec: mov r0,r9
  0012f9ee: blx 0x0007630c
  0012f9f2: ldr r1,[sp,#0x30]
  0012f9f4: add r0,sp,#0x28
  0012f9f6: blx 0x0007630c
  0012f9fa: ldr r1,[sp,#0x30]
  0012f9fc: add r0,sp,#0x24
  0012f9fe: blx 0x0007630c
  0012fa02: ldr r1,[sp,#0x30]
  0012fa04: add r0,sp,#0x20
  0012fa06: blx 0x0007630c
  0012fa0a: mov.w r9,#0x0
  0012fa0e: b 0x0012fa52
  0012fa10: ldrsh.w r0,[r11,r9,lsl #0x1]
  0012fa14: cmp r8,r0
  0012fa16: bne 0x0012fa4e
  0012fa18: movs r0,#0x34
  0012fa1a: blx 0x0006eb24
  0012fa1e: mov r10,r0
  0012fa20: add r1,sp,#0x34
  0012fa22: mov r0,r6
  0012fa24: movs r2,#0x0
  0012fa26: blx 0x0006f028
  0012fa2a: ldrd r3,r2,[sp,#0x28]
  0012fa2e: ldrd r1,r0,[sp,#0x20]
  0012fa32: strd r0,r1,[sp,#0x0]
  0012fa36: mov r0,r10
  0012fa38: mov r1,r6
  0012fa3a: blx 0x00076318
  0012fa3e: ldr r0,[sp,#0x10]
  0012fa40: ldr r0,[r0,#0x4]
  0012fa42: str.w r10,[r0,r4,lsl #0x2]
  0012fa46: mov r0,r6
  0012fa48: blx 0x0006ee30
  0012fa4c: adds r4,#0x1
  0012fa4e: add.w r9,r9,#0x1
  0012fa52: cmp r9,r5
  0012fa54: blt 0x0012fa10
  0012fa56: add.w r8,r8,#0x1
  0012fa5a: add.w r9,sp,#0x2c
  0012fa5e: ldr r1,[sp,#0x30]
  0012fa60: cmp.w r8,#0x87
  0012fa64: bcc 0x0012f9de
  0012fa66: mov r0,r1
  0012fa68: blx 0x0006ece0
  0012fa6c: add r0,sp,#0x34
  0012fa6e: blx 0x0006ee30
  0012fa72: ldrd r8,r0,[sp,#0xc]
  0012fa76: b 0x0012fa7a
  0012fa78: movs r0,#0x0
  0012fa7a: ldr r2,[sp,#0x40]
  0012fa7c: ldr.w r1,[r8,#0x0]
  0012fa80: subs r1,r1,r2
  0012fa82: ittt eq
  0012fa84: add.eq sp,#0x44
  0012fa86: pop.eq.w {r8,r9,r10,r11}
  0012fa8a: pop.eq {r4,r5,r6,r7,pc}
  0012fa8c: blx 0x0006e824
```
