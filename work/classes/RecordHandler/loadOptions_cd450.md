# RecordHandler::loadOptions
elf 0xcd450 body 790
Sig: undefined __thiscall loadOptions(RecordHandler * this)

## decompile
```c

/* RecordHandler::loadOptions() */

void __thiscall RecordHandler::loadOptions(RecordHandler *this)

{
  int iVar1;
  uint uVar2;
  String *pSVar3;
  String *this_00;
  undefined4 *puVar4;
  int iVar5;
  RecordHandler *pRVar6;
  code *pcVar7;
  code *pcVar8;
  int *piVar9;
  int *piVar10;
  float fVar11;
  short sStack_2e;
  String aSStack_2c [8];
  int local_24;
  uint local_20;
  int local_1c;
  
  pRVar6 = this + 8;
  piVar10 = *(int **)(DAT_000dd780 + 0xdd464);
  local_1c = *piVar10;
  iVar1 = AEFile::FileExist(pRVar6);
  if (iVar1 != 0) {
    uVar2 = AEFile::OpenRead(pRVar6,&local_20);
    iVar1 = checkHash(uVar2);
    AEFile::Close(local_20);
    if (iVar1 != 0) {
      AEFile::OpenRead(pRVar6,&local_20);
      iVar1 = *(int *)(DAT_000dd784 + 0xdd4a4);
      pcVar8 = *(code **)(DAT_000dd788 + 0xdd4a8);
      (*pcVar8)(iVar1 + 0x10,local_20);
      (*pcVar8)(iVar1 + 0x11,local_20);
      puVar4 = (undefined4 *)(DAT_000dd790 + 0xdd4c8);
      *(undefined1 *)(**(int **)(DAT_000dd78c + 0xdd4be) + 0xfa) = *(undefined1 *)(iVar1 + 0x11);
      pcVar7 = (code *)*puVar4;
      (*pcVar7)(iVar1 + 0x14,local_20);
      (*pcVar7)(iVar1 + 0x18,local_20);
      (*pcVar7)(iVar1 + 0x1c,local_20);
      (*pcVar7)(iVar1,local_20);
      (*pcVar8)(iVar1 + 0xc,local_20);
      (*pcVar8)(iVar1 + 0xd,local_20);
      (*pcVar7)(iVar1 + 4,local_20);
      (*pcVar7)(iVar1 + 0x24,local_20);
      (*pcVar7)(iVar1 + 0x28,local_20);
      piVar9 = *(int **)(DAT_000dd794 + 0xdd516);
      if ((int *)*piVar9 != (int *)0x0) {
        (**(code **)(*(int *)*piVar9 + 4))();
        *piVar9 = 0;
      }
      pSVar3 = (String *)AbyssEngine::String::String(aSStack_2c);
      AEFile::Read(pSVar3,local_20,false);
      if (local_24 != 0) {
        this_00 = operator_new(0xc);
        AbyssEngine::String::String(this_00,aSStack_2c,false);
        *piVar9 = (int)this_00;
      }
      AEFile::Read(*(int **)(DAT_000dd798 + 0xdd550),local_20);
      AEFile::Read(*(bool **)(DAT_000dd79c + 0xdd55c),local_20);
      AEFile::Read((int *)(iVar1 + 0x54),local_20);
      AEFile::Read((int *)(iVar1 + 0x58),local_20);
      AEFile::Read((float *)(iVar1 + 8),local_20);
      AEFile::Read((bool *)(iVar1 + 0xe),local_20);
      AEFile::Read((bool *)(iVar1 + 0x32),local_20);
      AEFile::Read((bool *)(iVar1 + 0x33),local_20);
      AEFile::Read((bool *)(iVar1 + 0x34),local_20);
      AEFile::Read((float *)(iVar1 + 0x20),local_20);
      sStack_2e = -1;
      AEFile::Read(&sStack_2e,local_20);
      AEFile::Read(*(bool **)(DAT_000dd7a0 + 0xdd5ca),local_20);
      AEFile::Read((bool *)(iVar1 + 0x35),local_20);
      AEFile::Read((bool *)(iVar1 + 0x36),local_20);
      AEFile::Read((bool *)(iVar1 + 0x3a),local_20);
      AEFile::Read((bool *)(iVar1 + 0x3b),local_20);
      AEFile::Read((bool *)(iVar1 + 0x3c),local_20);
      AEFile::Read((bool *)(iVar1 + 0x37),local_20);
      AEFile::Read((bool *)(iVar1 + 0x3d),local_20);
      AEFile::Read((bool *)(iVar1 + 0x3e),local_20);
      AEFile::Read((bool *)(iVar1 + 0x3f),local_20);
      AEFile::Read((bool *)(iVar1 + 0x40),local_20);
      AEFile::Read((bool *)(iVar1 + 0x41),local_20);
      AEFile::Read((float *)(iVar1 + 0x44),local_20);
      AEFile::Read((bool *)(iVar1 + 0x48),local_20);
      AEFile::Read((bool *)(iVar1 + 0x38),local_20);
      AEFile::Read((bool *)(iVar1 + 0x4e),local_20);
      AEFile::Read((bool *)(iVar1 + 0x49),local_20);
      AEFile::Read((bool *)(iVar1 + 0x4a),local_20);
      AEFile::Read((bool *)(iVar1 + 0x4b),local_20);
      AEFile::Read((bool *)(iVar1 + 0x4c),local_20);
      AEFile::Read((bool *)(iVar1 + 0x4d),local_20);
      AEFile::Read((int *)(iVar1 + 0x50),local_20);
      AEFile::Read((bool *)(iVar1 + 0x60),local_20);
      AEFile::Read((bool *)(iVar1 + 0x61),local_20);
      AEFile::Close(local_20);
      iVar5 = (int)sStack_2e;
      if (-1 < iVar5) {
        if (iVar5 == 9) {
          iVar5 = 0;
          sStack_2e = 0;
        }
        GameText::setLanguage((short)**(undefined4 **)(DAT_000dd7a4 + 0xdd6d6),iVar5);
        Globals::loadFont(**(int **)(DAT_000dd7a8 + 0xdd6ec));
      }
      piVar9 = *(int **)(DAT_000dd7ac + 0xdd6f8);
      if ((FModSound *)*piVar9 != (FModSound *)0x0) {
        FModSound::setAudioLanguage((FModSound *)*piVar9,(int)sStack_2e);
        FModSound::enableCategory(*piVar9,true);
        FModSound::enableCategory(*piVar9,true);
        fVar11 = (float)FModSound::enableCategory(*piVar9,true);
        fVar11 = (float)FModSound::setVolume(*piVar9,fVar11);
        fVar11 = (float)FModSound::setVolume(*piVar9,fVar11);
        FModSound::setVolume(*piVar9,fVar11);
      }
      AEFile::Read((bool *)(iVar1 + 0x62),local_20);
      AbyssEngine::String::~String(aSStack_2c);
    }
  }
  if (*piVar10 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000dd450: push {r4,r5,r6,r7,lr}
  000dd452: add r7,sp,#0xc
  000dd454: push.w r8
  000dd458: sub sp,#0x18
  000dd45a: ldr r1,[0x000dd780]
  000dd45c: add.w r4,r0,#0x8
  000dd460: add r1,pc
  000dd462: mov r0,r4
  000dd464: ldr.w r8,[r1,#0x0]
  000dd468: ldr.w r1,[r8,#0x0]
  000dd46c: str r1,[sp,#0x14]
  000dd46e: blx 0x0006fff4
  000dd472: cmp r0,#0x0
  000dd474: beq.w 0x000dd750
  000dd478: add r1,sp,#0x10
  000dd47a: mov r0,r4
  000dd47c: blx 0x0006f34c
  000dd480: ldr r1,[sp,#0x10]
  000dd482: blx 0x00074848
  000dd486: mov r5,r0
  000dd488: ldr r0,[sp,#0x10]
  000dd48a: blx 0x0006ece0
  000dd48e: cmp r5,#0x0
  000dd490: beq.w 0x000dd750
  000dd494: add r1,sp,#0x10
  000dd496: mov r0,r4
  000dd498: blx 0x0006f34c
  000dd49c: ldr r0,[0x000dd784]
  000dd49e: ldr r2,[0x000dd788]
  000dd4a0: add r0,pc
  000dd4a2: ldr r1,[sp,#0x10]
  000dd4a4: add r2,pc
  000dd4a6: ldr r4,[r0,#0x0]
  000dd4a8: ldr r6,[r2,#0x0]
  000dd4aa: add.w r0,r4,#0x10
  000dd4ae: blx r6
  000dd4b0: ldr r1,[sp,#0x10]
  000dd4b2: add.w r0,r4,#0x11
  000dd4b6: blx r6
  000dd4b8: ldr r0,[0x000dd78c]
  000dd4ba: add r0,pc
  000dd4bc: ldr r0,[r0,#0x0]
  000dd4be: ldr r0,[r0,#0x0]
  000dd4c0: ldr r2,[0x000dd790]
  000dd4c2: ldrb r1,[r4,#0x11]
  000dd4c4: add r2,pc
  000dd4c6: strb.w r1,[r0,#0xfa]
  000dd4ca: ldr r1,[sp,#0x10]
  000dd4cc: add.w r0,r4,#0x14
  000dd4d0: ldr r5,[r2,#0x0]
  000dd4d2: blx r5
  000dd4d4: ldr r1,[sp,#0x10]
  000dd4d6: add.w r0,r4,#0x18
  000dd4da: blx r5
  000dd4dc: ldr r1,[sp,#0x10]
  000dd4de: add.w r0,r4,#0x1c
  000dd4e2: blx r5
  000dd4e4: ldr r1,[sp,#0x10]
  000dd4e6: mov r0,r4
  000dd4e8: blx r5
  000dd4ea: ldr r1,[sp,#0x10]
  000dd4ec: add.w r0,r4,#0xc
  000dd4f0: blx r6
  000dd4f2: ldr r1,[sp,#0x10]
  000dd4f4: add.w r0,r4,#0xd
  000dd4f8: blx r6
  000dd4fa: ldr r1,[sp,#0x10]
  000dd4fc: adds r0,r4,#0x4
  000dd4fe: blx r5
  000dd500: ldr r1,[sp,#0x10]
  000dd502: add.w r0,r4,#0x24
  000dd506: blx r5
  000dd508: ldr r1,[sp,#0x10]
  000dd50a: add.w r0,r4,#0x28
  000dd50e: blx r5
  000dd510: ldr r0,[0x000dd794]
  000dd512: add r0,pc
  000dd514: ldr r6,[r0,#0x0]
  000dd516: ldr r0,[r6,#0x0]
  000dd518: cbz r0,0x000dd524
  000dd51a: ldr r1,[r0,#0x0]
  000dd51c: ldr r1,[r1,#0x4]
  000dd51e: blx r1
  000dd520: movs r0,#0x0
  000dd522: str r0,[r6,#0x0]
  000dd524: add r0,sp,#0x4
  000dd526: blx 0x0006efbc
  000dd52a: ldr r1,[sp,#0x10]
  000dd52c: movs r2,#0x0
  000dd52e: blx 0x00074824
  000dd532: ldr r0,[sp,#0xc]
  000dd534: cbz r0,0x000dd548
  000dd536: movs r0,#0xc
  000dd538: blx 0x0006eb24
  000dd53c: mov r5,r0
  000dd53e: add r1,sp,#0x4
  000dd540: movs r2,#0x0
  000dd542: blx 0x0006f028
  000dd546: str r5,[r6,#0x0]
  000dd548: ldr r0,[0x000dd798]
  000dd54a: ldr r1,[sp,#0x10]
  000dd54c: add r0,pc
  000dd54e: ldr r0,[r0,#0x0]
  000dd550: blx 0x00074818
  000dd554: ldr r0,[0x000dd79c]
  000dd556: ldr r1,[sp,#0x10]
  000dd558: add r0,pc
  000dd55a: ldr r0,[r0,#0x0]
  000dd55c: blx 0x00074854
  000dd560: ldr r1,[sp,#0x10]
  000dd562: add.w r0,r4,#0x54
  000dd566: blx 0x00074818
  000dd56a: ldr r1,[sp,#0x10]
  000dd56c: add.w r0,r4,#0x58
  000dd570: blx 0x00074818
  000dd574: ldr r1,[sp,#0x10]
  000dd576: add.w r0,r4,#0x8
  000dd57a: blx 0x00074830
  000dd57e: ldr r1,[sp,#0x10]
  000dd580: add.w r0,r4,#0xe
  000dd584: blx 0x00074854
  000dd588: ldr r1,[sp,#0x10]
  000dd58a: add.w r0,r4,#0x32
  000dd58e: blx 0x00074854
  000dd592: ldr r1,[sp,#0x10]
  000dd594: add.w r0,r4,#0x33
  000dd598: blx 0x00074854
  000dd59c: ldr r1,[sp,#0x10]
  000dd59e: add.w r0,r4,#0x34
  000dd5a2: blx 0x00074854
  000dd5a6: ldr r1,[sp,#0x10]
  000dd5a8: add.w r0,r4,#0x20
  000dd5ac: blx 0x00074830
  000dd5b0: ldr r1,[sp,#0x10]
  000dd5b2: movw r0,#0xffff
  000dd5b6: strh r0,[r7,#-0x26]
  000dd5ba: sub.w r0,r7,#0x26
  000dd5be: blx 0x00074908
  000dd5c2: ldr r0,[0x000dd7a0]
  000dd5c4: ldr r1,[sp,#0x10]
  000dd5c6: add r0,pc
  000dd5c8: ldr r0,[r0,#0x0]
  000dd5ca: blx 0x00074854
  000dd5ce: ldr r1,[sp,#0x10]
  000dd5d0: add.w r0,r4,#0x35
  000dd5d4: blx 0x00074854
  000dd5d8: ldr r1,[sp,#0x10]
  000dd5da: add.w r0,r4,#0x36
  000dd5de: blx 0x00074854
  000dd5e2: ldr r1,[sp,#0x10]
  000dd5e4: add.w r0,r4,#0x3a
  000dd5e8: blx 0x00074854
  000dd5ec: ldr r1,[sp,#0x10]
  000dd5ee: add.w r0,r4,#0x3b
  000dd5f2: blx 0x00074854
  000dd5f6: ldr r1,[sp,#0x10]
  000dd5f8: add.w r0,r4,#0x3c
  000dd5fc: blx 0x00074854
  000dd600: ldr r1,[sp,#0x10]
  000dd602: add.w r0,r4,#0x37
  000dd606: blx 0x00074854
  000dd60a: ldr r1,[sp,#0x10]
  000dd60c: add.w r0,r4,#0x3d
  000dd610: blx 0x00074854
  000dd614: ldr r1,[sp,#0x10]
  000dd616: add.w r0,r4,#0x3e
  000dd61a: blx 0x00074854
  000dd61e: ldr r1,[sp,#0x10]
  000dd620: add.w r0,r4,#0x3f
  000dd624: blx 0x00074854
  000dd628: ldr r1,[sp,#0x10]
  000dd62a: add.w r0,r4,#0x40
  000dd62e: blx 0x00074854
  000dd632: ldr r1,[sp,#0x10]
  000dd634: add.w r0,r4,#0x41
  000dd638: blx 0x00074854
  000dd63c: ldr r1,[sp,#0x10]
  000dd63e: add.w r0,r4,#0x44
  000dd642: blx 0x00074830
  000dd646: ldr r1,[sp,#0x10]
  000dd648: add.w r0,r4,#0x48
  000dd64c: blx 0x00074854
  000dd650: ldr r1,[sp,#0x10]
  000dd652: add.w r0,r4,#0x38
  000dd656: blx 0x00074854
  000dd65a: ldr r1,[sp,#0x10]
  000dd65c: add.w r0,r4,#0x4e
  000dd660: blx 0x00074854
  000dd664: ldr r1,[sp,#0x10]
  000dd666: add.w r0,r4,#0x49
  000dd66a: blx 0x00074854
  000dd66e: ldr r1,[sp,#0x10]
  000dd670: add.w r0,r4,#0x4a
  000dd674: blx 0x00074854
  000dd678: ldr r1,[sp,#0x10]
  000dd67a: add.w r0,r4,#0x4b
  000dd67e: blx 0x00074854
  000dd682: ldr r1,[sp,#0x10]
  000dd684: add.w r0,r4,#0x4c
  000dd688: blx 0x00074854
  000dd68c: ldr r1,[sp,#0x10]
  000dd68e: add.w r0,r4,#0x4d
  000dd692: blx 0x00074854
  000dd696: ldr r1,[sp,#0x10]
  000dd698: add.w r0,r4,#0x50
  000dd69c: blx 0x00074818
  000dd6a0: ldr r1,[sp,#0x10]
  000dd6a2: add.w r0,r4,#0x60
  000dd6a6: blx 0x00074854
  000dd6aa: ldr r1,[sp,#0x10]
  000dd6ac: add.w r0,r4,#0x61
  000dd6b0: blx 0x00074854
  000dd6b4: ldr r0,[sp,#0x10]
  000dd6b6: blx 0x0006ece0
  000dd6ba: ldrsh.w r2,[r7,#-0x26]
  000dd6be: mov.w r0,#0xffffffff
  000dd6c2: cmp r2,r0
  000dd6c4: ble 0x000dd6f2
  000dd6c6: cmp r2,#0x9
  000dd6c8: itt eq
  000dd6ca: mov.eq r2,#0x0
  000dd6cc: strh.eq r2,[r7,#-0x26]
  000dd6d0: ldr r0,[0x000dd7a4]
  000dd6d2: add r0,pc
  000dd6d4: ldr r0,[r0,#0x0]
  000dd6d6: ldr r0,[r0,#0x0]
  000dd6d8: mov r1,r2
  000dd6da: movw r2,#0xd49
  000dd6de: blx 0x0006ffdc
  000dd6e2: ldr r0,[0x000dd7a8]
  000dd6e4: ldrsh.w r1,[r7,#-0x26]
  000dd6e8: add r0,pc
  000dd6ea: ldr r0,[r0,#0x0]
  000dd6ec: ldr r0,[r0,#0x0]
  000dd6ee: blx 0x00071d04
  000dd6f2: ldr r0,[0x000dd7ac]
  000dd6f4: add r0,pc
  000dd6f6: ldr r5,[r0,#0x0]
  000dd6f8: ldr r0,[r5,#0x0]
  000dd6fa: cbz r0,0x000dd740
  000dd6fc: ldrsh.w r1,[r7,#-0x26]
  000dd700: blx 0x00071488
  000dd704: ldr r0,[r5,#0x0]
  000dd706: ldrb r2,[r4,#0xd]
  000dd708: movs r1,#0x1
  000dd70a: blx 0x00074914
  000dd70e: ldr r0,[r5,#0x0]
  000dd710: ldrb r2,[r4,#0xc]
  000dd712: movs r1,#0x2
  000dd714: blx 0x00074914
  000dd718: ldr r0,[r5,#0x0]
  000dd71a: ldrb r2,[r4,#0xe]
  000dd71c: movs r1,#0x3
  000dd71e: blx 0x00074914
  000dd722: ldr r0,[r5,#0x0]
  000dd724: ldr r2,[r4,#0x0]
  000dd726: movs r1,#0x1
  000dd728: blx 0x00074920
  000dd72c: ldr r0,[r5,#0x0]
  000dd72e: ldr r2,[r4,#0x4]
  000dd730: movs r1,#0x2
  000dd732: blx 0x00074920
  000dd736: ldr r0,[r5,#0x0]
  000dd738: ldr r2,[r4,#0x8]
  000dd73a: movs r1,#0x3
  000dd73c: blx 0x00074920
  000dd740: ldr r1,[sp,#0x10]
  000dd742: add.w r0,r4,#0x62
  000dd746: blx 0x00074854
  000dd74a: add r0,sp,#0x4
  000dd74c: blx 0x0006ee30
  000dd750: ldr r0,[sp,#0x14]
  000dd752: ldr.w r1,[r8,#0x0]
  000dd756: subs r0,r1,r0
  000dd758: ittt eq
  000dd75a: add.eq sp,#0x18
  000dd75c: pop.eq.w r8
  000dd760: pop.eq {r4,r5,r6,r7,pc}
  000dd762: blx 0x0006e824
```
