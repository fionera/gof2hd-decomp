# FileRead::loadStationsBinary
elf 0x12014c body 300
Sig: undefined __thiscall loadStationsBinary(FileRead * this, SolarSystem * param_1)

## decompile
```c

/* FileRead::loadStationsBinary(SolarSystem*) */

void __thiscall FileRead::loadStationsBinary(FileRead *this,SolarSystem *param_1)

{
  Array<Station*> *this_00;
  String *pSVar1;
  int iVar2;
  uint *puVar3;
  Station *pSVar4;
  uint *extraout_r2;
  uint uVar5;
  int *piVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  String local_54 [12];
  int local_48;
  int iStack_44;
  int local_40;
  int iStack_3c;
  uint local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar6 = *(int **)(DAT_001302ac + 0x13015e);
  local_28 = *piVar6;
  this_00 = operator_new(0xc);
  Array<Station*>::Array(this_00);
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_34,(char *)(DAT_001302b0 + 0x130178),false)
  ;
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_001302b4 + 0x130194),(char *)&local_38,extraout_r2);
    puVar3 = (uint *)SolarSystem::getStations(param_1);
    ArraySetLength<Station*>(*puVar3,(Array *)this_00);
    AbyssEngine::String::String(aSStack_34);
    uVar8 = 0;
    for (uVar9 = 0; uVar9 < 0x87; uVar9 = uVar9 + 1) {
      AEFile::ReadSwitched(aSStack_34,local_38);
      AbyssEngine::String::ConvertFromUTF8(aSStack_34);
      AEFile::ReadSwitched(&iStack_3c,local_38);
      AEFile::ReadSwitched(&local_40,local_38);
      AEFile::ReadSwitched(&iStack_44,local_38);
      AEFile::ReadSwitched(&local_48,local_38);
      uVar5 = *puVar3;
      uVar7 = 0;
      while (uVar7 < uVar5) {
        if (*(uint *)(puVar3[1] + uVar7 * 4) == uVar9) {
          pSVar4 = operator_new(0x34);
          AbyssEngine::String::String(local_54,aSStack_34,false);
          Station::Station(pSVar4,local_54,iStack_3c,local_40,iStack_44,local_48);
          *(Station **)(*(int *)(this_00 + 4) + uVar8 * 4) = pSVar4;
          AbyssEngine::String::~String(local_54);
          uVar5 = *puVar3;
          uVar8 = uVar8 + 1;
        }
        uVar7 = uVar7 + 1;
        if (uVar8 == uVar5) {
          AEFile::Close(local_38);
          goto LAB_0013025a;
        }
      }
    }
    AEFile::Close(local_38);
LAB_0013025a:
    AbyssEngine::String::~String(aSStack_34);
  }
  iVar2 = *piVar6 - local_28;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0013014c: push {r4,r5,r6,r7,lr}
  0013014e: add r7,sp,#0xc
  00130150: push {r8,r9,r10,r11}
  00130154: sub sp,#0x3c
  00130156: ldr r0,[0x001302ac]
  00130158: mov r8,r1
  0013015a: add r0,pc
  0013015c: ldr r5,[r0,#0x0]
  0013015e: ldr r0,[r5,#0x0]
  00130160: str r0,[sp,#0x38]
  00130162: movs r0,#0xc
  00130164: blx 0x0006eb24
  00130168: mov r4,r0
  0013016a: blx 0x00073594
  0013016e: ldr r1,[0x001302b0]
  00130170: add r0,sp,#0x2c
  00130172: movs r2,#0x0
  00130174: add r1,pc
  00130176: blx 0x0006ee18
  0013017a: blx 0x0006fff4
  0013017e: mov r6,r0
  00130180: add r0,sp,#0x2c
  00130182: blx 0x0006ee30
  00130186: cmp r6,#0x0
  00130188: beq 0x00130250
  0013018a: str r5,[sp,#0x8]
  0013018c: add r1,sp,#0x28
  0013018e: ldr r0,[0x001302b4]
  00130190: add r0,pc
  00130192: blx 0x0006ecd4
  00130196: mov r0,r8
  00130198: blx 0x00071a88
  0013019c: mov r5,r0
  0013019e: ldr r0,[r0,#0x0]
  001301a0: mov r1,r4
  001301a2: blx 0x000735b8
  001301a6: add r0,sp,#0x2c
  001301a8: blx 0x0006efbc
  001301ac: add r6,sp,#0x24
  001301ae: add.w r11,sp,#0xc
  001301b2: mov.w r8,#0x0
  001301b6: mov.w r9,#0x0
  001301ba: b 0x00130240
  001301bc: add r0,sp,#0x2c
  001301be: blx 0x000762f4
  001301c2: add r0,sp,#0x2c
  001301c4: blx 0x00076300
  001301c8: ldr r1,[sp,#0x28]
  001301ca: mov r0,r6
  001301cc: blx 0x0007630c
  001301d0: ldr r1,[sp,#0x28]
  001301d2: add r0,sp,#0x20
  001301d4: blx 0x0007630c
  001301d8: ldr r1,[sp,#0x28]
  001301da: add r0,sp,#0x1c
  001301dc: blx 0x0007630c
  001301e0: ldr r1,[sp,#0x28]
  001301e2: add r0,sp,#0x18
  001301e4: blx 0x0007630c
  001301e8: ldr r2,[r5,#0x0]
  001301ea: movs r6,#0x0
  001301ec: cmp r6,r2
  001301ee: bcs 0x0013023a
  001301f0: ldr r0,[r5,#0x4]
  001301f2: ldr.w r0,[r0,r6,lsl #0x2]
  001301f6: cmp r0,r9
  001301f8: bne 0x00130232
  001301fa: movs r0,#0x34
  001301fc: blx 0x0006eb24
  00130200: mov r10,r0
  00130202: add r1,sp,#0x2c
  00130204: mov r0,r11
  00130206: movs r2,#0x0
  00130208: blx 0x0006f028
  0013020c: ldrd r3,r2,[sp,#0x20]
  00130210: ldrd r1,r0,[sp,#0x18]
  00130214: strd r0,r1,[sp,#0x0]
  00130218: mov r0,r10
  0013021a: mov r1,r11
  0013021c: blx 0x00076318
  00130220: ldr r0,[r4,#0x4]
  00130222: str.w r10,[r0,r8,lsl #0x2]
  00130226: mov r0,r11
  00130228: blx 0x0006ee30
  0013022c: ldr r2,[r5,#0x0]
  0013022e: add.w r8,r8,#0x1
  00130232: adds r6,#0x1
  00130234: cmp r8,r2
  00130236: bne 0x001301ec
  00130238: b 0x00130254
  0013023a: add.w r9,r9,#0x1
  0013023e: add r6,sp,#0x24
  00130240: ldr r1,[sp,#0x28]
  00130242: cmp.w r9,#0x87
  00130246: bcc 0x001301bc
  00130248: mov r0,r1
  0013024a: blx 0x0006ece0
  0013024e: b 0x0013025a
  00130250: movs r4,#0x0
  00130252: b 0x00130262
  00130254: ldr r0,[sp,#0x28]
  00130256: blx 0x0006ece0
  0013025a: add r0,sp,#0x2c
  0013025c: blx 0x0006ee30
  00130260: ldr r5,[sp,#0x8]
  00130262: ldr r0,[sp,#0x38]
  00130264: ldr r1,[r5,#0x0]
  00130266: subs r0,r1,r0
  00130268: itttt eq
  0013026a: mov.eq r0,r4
  0013026c: add.eq sp,#0x3c
  0013026e: pop.eq.w {r8,r9,r10,r11}
  00130272: pop.eq {r4,r5,r6,r7,pc}
  00130274: blx 0x0006e824
```
