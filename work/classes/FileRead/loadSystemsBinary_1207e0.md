# FileRead::loadSystemsBinary
elf 0x1207e0 body 622
Sig: undefined __stdcall loadSystemsBinary(void)

## decompile
```c

/* FileRead::loadSystemsBinary() */

void FileRead::loadSystemsBinary(void)

{
  String *pSVar1;
  int iVar2;
  Array<SolarSystem*> *this;
  uint uVar3;
  int *piVar4;
  Array *pAVar5;
  Array *pAVar6;
  Array *pAVar7;
  SolarSystem *pSVar8;
  uint *extraout_r2;
  int iVar9;
  int *piVar10;
  uint uVar11;
  uint uVar12;
  int *piVar13;
  String aSStack_68 [12];
  uint local_5c;
  int local_58;
  int local_54;
  int iStack_50;
  int local_4c;
  int iStack_48;
  int local_44;
  int iStack_40;
  int local_3c;
  uint local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar10 = *(int **)(DAT_00130aa0 + 0x1307f2);
  local_28 = *piVar10;
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_34,(char *)(DAT_00130aa4 + 0x1307fe),false)
  ;
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_00130aa8 + 0x13081a),(char *)&local_38,extraout_r2);
    this = operator_new(0xc);
    Array<SolarSystem*>::Array(this);
    ArraySetLength<SolarSystem*>(0x22,(Array *)this);
    AbyssEngine::String::String(aSStack_34);
    uVar12 = 0;
    while( true ) {
      if (0x21 < uVar12) break;
      AEFile::ReadSwitched(aSStack_34,local_38);
      AbyssEngine::String::ConvertFromUTF8(aSStack_34);
      AEFile::ReadSwitched(&local_3c,local_38);
      AEFile::ReadSwitched(&local_58,local_38);
      iVar2 = local_58;
      AEFile::ReadSwitched(&iStack_40,local_38);
      AEFile::ReadSwitched(&local_44,local_38);
      AEFile::ReadSwitched(&iStack_48,local_38);
      AEFile::ReadSwitched(&local_4c,local_38);
      AEFile::ReadSwitched(&iStack_50,local_38);
      AEFile::ReadSwitched(&local_54,local_38);
      AEFile::ReadSwitched((int *)&local_5c,local_38);
      uVar11 = local_5c;
      uVar3 = (uint)((ulonglong)local_5c * 4);
      if ((int)((ulonglong)local_5c * 4 >> 0x20) != 0) {
        uVar3 = 0xffffffff;
      }
      piVar4 = operator_new__(uVar3);
      piVar13 = piVar4;
      for (iVar9 = 0; iVar9 < (int)uVar11; iVar9 = iVar9 + 1) {
        AEFile::ReadSwitched(piVar13,local_38);
        piVar13 = piVar13 + 1;
        uVar11 = local_5c;
      }
      AEFile::ReadSwitched((int *)&local_5c,local_38);
      if ((int)local_5c < 1) {
        pAVar5 = (Array *)0x0;
      }
      else {
        pAVar5 = operator_new(0xc);
        Array<int>::Array();
        ArraySetLength<int>(local_5c,pAVar5);
        iVar9 = 0;
        for (uVar11 = 0; uVar11 < *(uint *)pAVar5; uVar11 = uVar11 + 1) {
          AEFile::ReadSwitched((int *)(*(int *)(pAVar5 + 4) + iVar9),local_38);
          iVar9 = iVar9 + 4;
        }
      }
      AEFile::ReadSwitched((int *)&local_5c,local_38);
      if ((int)local_5c < 1) {
        pAVar6 = (Array *)0x0;
      }
      else {
        pAVar6 = operator_new(0xc);
        Array<int>::Array();
        ArraySetLength<int>(local_5c,pAVar6);
        iVar9 = 0;
        for (uVar11 = 0; uVar11 < *(uint *)pAVar6; uVar11 = uVar11 + 1) {
          AEFile::ReadSwitched((int *)(*(int *)(pAVar6 + 4) + iVar9),local_38);
          iVar9 = iVar9 + 4;
        }
      }
      AEFile::ReadSwitched((int *)&local_5c,local_38);
      if ((int)local_5c < 1) {
        pAVar7 = (Array *)0x0;
      }
      else {
        pAVar7 = operator_new(0xc);
        Array<int>::Array();
        ArraySetLength<int>(local_5c,pAVar7);
        iVar9 = 0;
        for (uVar11 = 0; uVar11 < *(uint *)pAVar7; uVar11 = uVar11 + 1) {
          AEFile::ReadSwitched((int *)(*(int *)(pAVar7 + 4) + iVar9),local_38);
          iVar9 = iVar9 + 4;
        }
      }
      pSVar8 = operator_new(0x48);
      AbyssEngine::String::String(aSStack_68,aSStack_34,false);
      SolarSystem::SolarSystem
                (pSVar8,uVar12,aSStack_68,local_3c,iVar2 == 1,iStack_40,local_44,iStack_48,local_4c,
                 iStack_50,local_54,piVar4,pAVar5,pAVar6,pAVar7);
      *(SolarSystem **)(*(int *)(this + 4) + uVar12 * 4) = pSVar8;
      AbyssEngine::String::~String(aSStack_68);
      operator_delete__(piVar4);
      uVar12 = uVar12 + 1;
    }
    AEFile::Close(local_38);
    AbyssEngine::String::~String(aSStack_34);
  }
  iVar2 = *piVar10 - local_28;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  001307e0: push {r4,r5,r6,r7,lr}
  001307e2: add r7,sp,#0xc
  001307e4: push {r8,r9,r10,r11}
  001307e8: sub sp,#0x84
  001307ea: ldr r0,[0x00130aa0]
  001307ec: movs r2,#0x0
  001307ee: add r0,pc
  001307f0: ldr r5,[r0,#0x0]
  001307f2: ldr r0,[r5,#0x0]
  001307f4: ldr r1,[0x00130aa4]
  001307f6: str r0,[sp,#0x80]
  001307f8: add r0,sp,#0x74
  001307fa: add r1,pc
  001307fc: blx 0x0006ee18
  00130800: blx 0x0006fff4
  00130804: mov r4,r0
  00130806: add r0,sp,#0x74
  00130808: blx 0x0006ee30
  0013080c: cmp r4,#0x0
  0013080e: beq.w 0x00130a34
  00130812: ldr r0,[0x00130aa8]
  00130814: add r1,sp,#0x70
  00130816: add r0,pc
  00130818: blx 0x0006ecd4
  0013081c: movs r0,#0xc
  0013081e: blx 0x0006eb24
  00130822: mov r10,r0
  00130824: blx 0x00076348
  00130828: movs r0,#0x22
  0013082a: mov r1,r10
  0013082c: blx 0x00076354
  00130830: add r4,sp,#0x74
  00130832: mov r0,r4
  00130834: blx 0x0006efbc
  00130838: movs r6,#0x0
  0013083a: b 0x00130a12
  0013083c: mov r0,r4
  0013083e: blx 0x000762f4
  00130842: mov r0,r4
  00130844: blx 0x00076300
  00130848: ldr r1,[sp,#0x70]
  0013084a: mov r0,r8
  0013084c: blx 0x0007630c
  00130850: ldr r1,[sp,#0x70]
  00130852: mov r0,r9
  00130854: blx 0x0007630c
  00130858: ldr r4,[sp,#0x50]
  0013085a: ldr r1,[sp,#0x70]
  0013085c: mov r0,r11
  0013085e: blx 0x0007630c
  00130862: ldr r1,[sp,#0x70]
  00130864: add r0,sp,#0x64
  00130866: blx 0x0007630c
  0013086a: ldr r1,[sp,#0x70]
  0013086c: add r0,sp,#0x60
  0013086e: blx 0x0007630c
  00130872: ldr r1,[sp,#0x70]
  00130874: add r0,sp,#0x5c
  00130876: blx 0x0007630c
  0013087a: ldr r1,[sp,#0x70]
  0013087c: add r0,sp,#0x58
  0013087e: blx 0x0007630c
  00130882: ldr r1,[sp,#0x70]
  00130884: add r0,sp,#0x54
  00130886: blx 0x0007630c
  0013088a: ldr r1,[sp,#0x70]
  0013088c: add r0,sp,#0x4c
  0013088e: blx 0x0007630c
  00130892: str r6,[sp,#0x3c]
  00130894: movs r0,#0x4
  00130896: strd r5,r4,[sp,#0x34]
  0013089a: ldr r5,[sp,#0x4c]
  0013089c: umull r0,r1,r5,r0
  001308a0: cmp r1,#0x0
  001308a2: it ne
  001308a4: mov.ne r1,#0x1
  001308a6: cmp r1,#0x0
  001308a8: it ne
  001308aa: mov.ne.w r0,#0xffffffff
  001308ae: blx 0x0006ec08
  001308b2: movs r4,#0x0
  001308b4: mov r8,r0
  001308b6: mov r6,r0
  001308b8: b 0x001308c8
  001308ba: ldr r1,[sp,#0x70]
  001308bc: mov r0,r6
  001308be: blx 0x0007630c
  001308c2: ldr r5,[sp,#0x4c]
  001308c4: adds r6,#0x4
  001308c6: adds r4,#0x1
  001308c8: cmp r4,r5
  001308ca: blt 0x001308ba
  001308cc: ldr r1,[sp,#0x70]
  001308ce: add r0,sp,#0x4c
  001308d0: blx 0x0007630c
  001308d4: str.w r8,[sp,#0x30]
  001308d8: ldr r0,[sp,#0x4c]
  001308da: cmp r0,#0x1
  001308dc: blt 0x00130912
  001308de: movs r0,#0xc
  001308e0: blx 0x0006eb24
  001308e4: mov r9,r0
  001308e6: blx 0x000701f8
  001308ea: ldr r0,[sp,#0x4c]
  001308ec: mov r1,r9
  001308ee: blx 0x00071a4c
  001308f2: movs r4,#0x0
  001308f4: movs r5,#0x0
  001308f6: b 0x00130908
  001308f8: ldr.w r0,[r9,#0x4]
  001308fc: ldr r1,[sp,#0x70]
  001308fe: add r0,r4
  00130900: blx 0x0007630c
  00130904: adds r4,#0x4
  00130906: adds r5,#0x1
  00130908: ldr.w r0,[r9,#0x0]
  0013090c: cmp r5,r0
  0013090e: bcc 0x001308f8
  00130910: b 0x00130916
  00130912: mov.w r9,#0x0
  00130916: ldr r1,[sp,#0x70]
  00130918: add r0,sp,#0x4c
  0013091a: blx 0x0007630c
  0013091e: ldr r0,[sp,#0x4c]
  00130920: cmp r0,#0x1
  00130922: blt 0x00130958
  00130924: movs r0,#0xc
  00130926: blx 0x0006eb24
  0013092a: mov r8,r0
  0013092c: blx 0x000701f8
  00130930: ldr r0,[sp,#0x4c]
  00130932: mov r1,r8
  00130934: blx 0x00071a4c
  00130938: movs r4,#0x0
  0013093a: movs r5,#0x0
  0013093c: b 0x0013094e
  0013093e: ldr.w r0,[r8,#0x4]
  00130942: ldr r1,[sp,#0x70]
  00130944: add r0,r4
  00130946: blx 0x0007630c
  0013094a: adds r4,#0x4
  0013094c: adds r5,#0x1
  0013094e: ldr.w r0,[r8,#0x0]
  00130952: cmp r5,r0
  00130954: bcc 0x0013093e
  00130956: b 0x0013095c
  00130958: mov.w r8,#0x0
  0013095c: ldr r1,[sp,#0x70]
  0013095e: add r0,sp,#0x4c
  00130960: blx 0x0007630c
  00130964: ldr r0,[sp,#0x4c]
  00130966: cmp r0,#0x1
  00130968: blt 0x0013099e
  0013096a: movs r0,#0xc
  0013096c: blx 0x0006eb24
  00130970: mov r11,r0
  00130972: blx 0x000701f8
  00130976: ldr r0,[sp,#0x4c]
  00130978: mov r1,r11
  0013097a: blx 0x00071a4c
  0013097e: movs r4,#0x0
  00130980: movs r5,#0x0
  00130982: b 0x00130994
  00130984: ldr.w r0,[r11,#0x4]
  00130988: ldr r1,[sp,#0x70]
  0013098a: add r0,r4
  0013098c: blx 0x0007630c
  00130990: adds r4,#0x4
  00130992: adds r5,#0x1
  00130994: ldr.w r0,[r11,#0x0]
  00130998: cmp r5,r0
  0013099a: bcc 0x00130984
  0013099c: b 0x001309a2
  0013099e: mov.w r11,#0x0
  001309a2: movs r0,#0x48
  001309a4: blx 0x0006eb24
  001309a8: mov r6,r0
  001309aa: add r0,sp,#0x40
  001309ac: add r1,sp,#0x74
  001309ae: movs r2,#0x0
  001309b0: blx 0x0006f028
  001309b4: ldr r3,[sp,#0x6c]
  001309b6: ldrd r1,lr,[sp,#0x64]
  001309ba: ldrd r5,r2,[sp,#0x5c]
  001309be: ldr r0,[sp,#0x30]
  001309c0: ldrd r12,r4,[sp,#0x54]
  001309c4: strd r9,r8,[sp,#0x20]
  001309c8: mov r8,r0
  001309ca: ldr r0,[sp,#0x38]
  001309cc: str.w r11,[sp,#0x28]
  001309d0: subs r0,#0x1
  001309d2: clz r0,r0
  001309d6: lsrs r0,r0,#0x5
  001309d8: strd r0,lr,[sp,#0x0]
  001309dc: add r0,sp,#0x8
  001309de: stmia r0!,{r1,r2,r5}
  001309e0: add r5,sp,#0x40
  001309e2: mov r0,r6
  001309e4: strd r4,r12,[sp,#0x14]
  001309e8: mov r4,r6
  001309ea: ldr r6,[sp,#0x3c]
  001309ec: mov r2,r5
  001309ee: str.w r8,[sp,#0x1c]
  001309f2: mov r1,r6
  001309f4: blx 0x00076360
  001309f8: ldr.w r0,[r10,#0x4]
  001309fc: str.w r4,[r0,r6,lsl #0x2]
  00130a00: mov r0,r5
  00130a02: blx 0x0006ee30
  00130a06: mov r0,r8
  00130a08: blx 0x0006ebfc
  00130a0c: adds r6,#0x1
  00130a0e: ldr r5,[sp,#0x34]
  00130a10: add r4,sp,#0x74
  00130a12: ldr r1,[sp,#0x70]
  00130a14: add.w r8,sp,#0x6c
  00130a18: add.w r9,sp,#0x50
  00130a1c: add.w r11,sp,#0x68
  00130a20: cmp r6,#0x22
  00130a22: bcc.w 0x0013083c
  00130a26: mov r0,r1
  00130a28: blx 0x0006ece0
  00130a2c: add r0,sp,#0x74
  00130a2e: blx 0x0006ee30
  00130a32: b 0x00130a38
  00130a34: mov.w r10,#0x0
  00130a38: ldr r0,[sp,#0x80]
  00130a3a: ldr r1,[r5,#0x0]
  00130a3c: subs r0,r1,r0
  00130a3e: itttt eq
  00130a40: mov.eq r0,r10
  00130a42: add.eq sp,#0x84
  00130a44: pop.eq.w {r8,r9,r10,r11}
  00130a48: pop.eq {r4,r5,r6,r7,pc}
  00130a4a: blx 0x0006e824
```
