# RecordHandler::readWanted
elf 0xce6c8 body 436
Sig: undefined __thiscall readWanted(RecordHandler * this, uint param_1)

## decompile
```c

/* RecordHandler::readWanted(unsigned int) */

void __thiscall RecordHandler::readWanted(RecordHandler *this,uint param_1)

{
  String *pSVar1;
  Wanted *this_00;
  int *piVar2;
  int iVar3;
  uint uVar4;
  code *pcVar5;
  int *piVar6;
  int *piVar7;
  String aSStack_84 [12];
  int local_78;
  int iStack_74;
  int local_70;
  int iStack_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  bool bStack_51;
  int local_50;
  int iStack_4c;
  int local_48;
  String aSStack_44 [12];
  int local_38;
  int local_34;
  int local_30;
  bool bStack_2a;
  bool bStack_29;
  int local_28;
  
  piVar7 = *(int **)(DAT_000de8a4 + 0xde6dc);
  local_28 = *piVar7;
  bStack_29 = false;
  bStack_2a = false;
  local_30 = -1;
  local_38 = -1;
  local_34 = -1;
  AEFile::Read(&bStack_29,param_1);
  AEFile::Read(&bStack_2a,param_1);
  pcVar5 = *(code **)(DAT_000de8a8 + 0xde710);
  (*pcVar5)(&local_30,param_1);
  (*pcVar5)(&local_34,param_1);
  (*pcVar5)(&local_38,param_1);
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_44);
  local_48 = 0;
  local_50 = 0;
  iStack_4c = 0;
  bStack_51 = true;
  local_58 = 0;
  local_60 = 0;
  local_5c = 0;
  local_68 = 0;
  local_64 = 0;
  local_70 = 0;
  iStack_6c = 0;
  local_78 = 0;
  iStack_74 = 0;
  AEFile::Read(pSVar1,param_1,true);
  AEFile::Read(&local_48,param_1);
  AEFile::Read(&iStack_4c,param_1);
  AEFile::Read(&local_50,param_1);
  AEFile::Read(&bStack_51,param_1);
  AEFile::Read(&local_58,param_1);
  AEFile::Read(&local_5c,param_1);
  AEFile::Read(&local_60,param_1);
  AEFile::Read(&local_64,param_1);
  AEFile::Read(&local_68,param_1);
  AEFile::Read(&iStack_6c,param_1);
  AEFile::Read(&local_70,param_1);
  AEFile::Read(&iStack_74,param_1);
  AEFile::Read(&local_78,param_1);
  this_00 = operator_new(0x54);
  iVar3 = local_48;
  AbyssEngine::String::String(aSStack_84,aSStack_44,false);
  Wanted::Wanted(this_00,iVar3,aSStack_84,iStack_4c,local_50,bStack_51,local_58,local_5c,local_60,
                 local_64,local_68,iStack_6c,local_70,iStack_74,local_78);
  AbyssEngine::String::~String(aSStack_84);
  piVar2 = operator_new__(0x14);
  piVar6 = piVar2;
  for (uVar4 = 0; uVar4 < 5; uVar4 = uVar4 + 1) {
    AEFile::Read(piVar6,param_1);
    piVar6 = piVar6 + 1;
  }
  Wanted::setImageParts(this_00,piVar2);
  Wanted::setActive(this_00,bStack_29);
  Wanted::setTerminated(SUB41(this_00,0));
  Wanted::setCurrentLocation(this_00,local_30);
  Wanted::setTravelsTo(this_00,local_34);
  Wanted::setLastSeen(this_00,local_38);
  AbyssEngine::String::~String(aSStack_44);
  iVar3 = *piVar7 - local_28;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  000de6c8: push {r4,r5,r6,r7,lr}
  000de6ca: add r7,sp,#0xc
  000de6cc: push {r8,r9,r10,r11}
  000de6d0: sub sp,#0x9c
  000de6d2: ldr r0,[0x000de8a4]
  000de6d4: movs r5,#0x0
  000de6d6: mov r4,r1
  000de6d8: add r0,pc
  000de6da: ldr.w r8,[r0,#0x0]
  000de6de: ldr.w r0,[r8,#0x0]
  000de6e2: str r0,[sp,#0x98]
  000de6e4: mov.w r0,#0xffffffff
  000de6e8: strb.w r5,[r7,#-0x21]
  000de6ec: strb.w r5,[r7,#-0x22]
  000de6f0: str r0,[sp,#0x90]
  000de6f2: strd r0,r0,[sp,#0x88]
  000de6f6: sub.w r0,r7,#0x21
  000de6fa: blx 0x00074854
  000de6fe: sub.w r0,r7,#0x22
  000de702: mov r1,r4
  000de704: blx 0x00074854
  000de708: ldr r0,[0x000de8a8]
  000de70a: mov r1,r4
  000de70c: add r0,pc
  000de70e: ldr r6,[r0,#0x0]
  000de710: add r0,sp,#0x90
  000de712: blx r6
  000de714: add r0,sp,#0x8c
  000de716: mov r1,r4
  000de718: blx r6
  000de71a: add r0,sp,#0x88
  000de71c: mov r1,r4
  000de71e: blx r6
  000de720: add r0,sp,#0x7c
  000de722: blx 0x0006efbc
  000de726: movs r1,#0x1
  000de728: str r5,[sp,#0x78]
  000de72a: strd r5,r5,[sp,#0x70]
  000de72e: strb.w r1,[r7,#-0x49]
  000de732: str r5,[sp,#0x68]
  000de734: strd r5,r5,[sp,#0x60]
  000de738: strd r5,r5,[sp,#0x58]
  000de73c: strd r5,r5,[sp,#0x50]
  000de740: strd r5,r5,[sp,#0x48]
  000de744: mov r1,r4
  000de746: movs r2,#0x1
  000de748: blx 0x00074824
  000de74c: add r0,sp,#0x78
  000de74e: mov r1,r4
  000de750: blx 0x00074818
  000de754: add r0,sp,#0x74
  000de756: mov r1,r4
  000de758: blx 0x00074818
  000de75c: add r0,sp,#0x70
  000de75e: mov r1,r4
  000de760: blx 0x00074818
  000de764: sub.w r0,r7,#0x49
  000de768: mov r1,r4
  000de76a: blx 0x00074854
  000de76e: add r0,sp,#0x68
  000de770: mov r1,r4
  000de772: blx 0x00074818
  000de776: add r0,sp,#0x64
  000de778: mov r1,r4
  000de77a: blx 0x00074818
  000de77e: add r0,sp,#0x60
  000de780: mov r1,r4
  000de782: blx 0x00074818
  000de786: add r0,sp,#0x5c
  000de788: mov r1,r4
  000de78a: blx 0x00074818
  000de78e: add r0,sp,#0x58
  000de790: mov r1,r4
  000de792: blx 0x00074818
  000de796: add r0,sp,#0x54
  000de798: mov r1,r4
  000de79a: blx 0x00074818
  000de79e: add r0,sp,#0x50
  000de7a0: mov r1,r4
  000de7a2: blx 0x00074818
  000de7a6: add r0,sp,#0x4c
  000de7a8: mov r1,r4
  000de7aa: blx 0x00074818
  000de7ae: add r0,sp,#0x48
  000de7b0: mov r1,r4
  000de7b2: blx 0x00074818
  000de7b6: movs r0,#0x54
  000de7b8: blx 0x0006eb24
  000de7bc: str r0,[sp,#0x38]
  000de7be: ldr r0,[sp,#0x78]
  000de7c0: str.w r8,[sp,#0x30]
  000de7c4: str r0,[sp,#0x34]
  000de7c6: add r0,sp,#0x3c
  000de7c8: add r1,sp,#0x7c
  000de7ca: movs r2,#0x0
  000de7cc: blx 0x0006f028
  000de7d0: add.w lr,sp,#0x50
  000de7d4: ldrd r0,r3,[sp,#0x70]
  000de7d8: ldrb.w r1,[r7,#-0x49]
  000de7dc: ldrd r5,r2,[sp,#0x64]
  000de7e0: ldrd r12,r6,[sp,#0x5c]
  000de7e4: ldm.w lr,{r9,r11,lr}
  000de7e8: ldrd r10,r8,[sp,#0x48]
  000de7ec: strd r8,r10,[sp,#0x24]
  000de7f0: stm sp,{r0,r1,r2,r5,r6,r12,lr}
  000de7f4: add r2,sp,#0x3c
  000de7f6: strd r11,r9,[sp,#0x1c]
  000de7fa: ldr.w r8,[sp,#0x38]
  000de7fe: ldr r1,[sp,#0x34]
  000de800: mov r0,r8
  000de802: blx 0x000749f8
  000de806: add r0,sp,#0x3c
  000de808: blx 0x0006ee30
  000de80c: movs r0,#0x14
  000de80e: blx 0x0006ec08
  000de812: mov r10,r0
  000de814: movs r5,#0x0
  000de816: mov r6,r0
  000de818: b 0x000de826
  000de81a: mov r0,r6
  000de81c: mov r1,r4
  000de81e: blx 0x00074818
  000de822: adds r6,#0x4
  000de824: adds r5,#0x1
  000de826: cmp r5,#0x5
  000de828: bcc 0x000de81a
  000de82a: mov r0,r8
  000de82c: mov r1,r10
  000de82e: blx 0x00074a04
  000de832: ldrb.w r1,[r7,#-0x21]
  000de836: mov r0,r8
  000de838: blx 0x00073a20
  000de83c: ldrb.w r1,[r7,#-0x22]
  000de840: mov r0,r8
  000de842: blx 0x00074a10
  000de846: ldr r1,[sp,#0x90]
  000de848: mov r0,r8
  000de84a: blx 0x000737bc
  000de84e: ldr r1,[sp,#0x8c]
  000de850: mov r0,r8
  000de852: blx 0x000737ec
  000de856: ldr r1,[sp,#0x88]
  000de858: mov r0,r8
  000de85a: blx 0x000737c8
  000de85e: add r0,sp,#0x7c
  000de860: blx 0x0006ee30
  000de864: ldr r0,[sp,#0x98]
  000de866: ldr r1,[sp,#0x30]
  000de868: ldr r1,[r1,#0x0]
  000de86a: subs r0,r1,r0
  000de86c: itttt eq
  000de86e: mov.eq r0,r8
  000de870: add.eq sp,#0x9c
  000de872: pop.eq.w {r8,r9,r10,r11}
  000de876: pop.eq {r4,r5,r6,r7,pc}
  000de878: blx 0x0006e824
```
