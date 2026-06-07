# FileRead::loadWanted
elf 0x120490 body 476
Sig: undefined __stdcall loadWanted(void)

## decompile
```c

/* FileRead::loadWanted() */

void FileRead::loadWanted(void)

{
  uint uVar1;
  String *pSVar2;
  int iVar3;
  Array<Wanted*> *this;
  Wanted *pWVar4;
  int *piVar5;
  uint *extraout_r2;
  uint uVar6;
  int *piVar7;
  uint uVar8;
  char cStack_7d;
  String aSStack_7c [12];
  int local_70;
  int iStack_6c;
  int local_68;
  int iStack_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int iStack_48;
  int local_44;
  int local_40;
  int local_3c;
  uint local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar7 = *(int **)(DAT_001306a4 + 0x1304a2);
  local_28 = *piVar7;
  pSVar2 = (String *)AbyssEngine::String::String(aSStack_34,(char *)(DAT_001306a8 + 0x1304ae),false)
  ;
  iVar3 = AEFile::FileExist(pSVar2);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar3 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_001306ac + 0x1304cc),(char *)&local_38,extraout_r2);
    this = operator_new(0xc);
    Array<Wanted*>::Array(this);
    ArraySetLength<Wanted*>(0x19,(Array *)this);
    AbyssEngine::String::String(aSStack_34);
    uVar1 = 0;
    while (uVar8 = uVar1, uVar8 < *(uint *)this) {
      AEFile::ReadSwitched(aSStack_34,local_38);
      AbyssEngine::String::ConvertFromUTF8(aSStack_34);
      AEFile::ReadSwitched(&local_3c,local_38);
      AEFile::ReadSwitched(&local_40,local_38);
      AEFile::ReadSwitched(&local_44,local_38);
      AEFile::ReadSwitched(&local_54,local_38);
      AEFile::ReadSwitched(&iStack_48,local_38);
      AEFile::ReadSwitched(&local_4c,local_38);
      AEFile::ReadSwitched(&local_50,local_38);
      AEFile::ReadSwitched(&local_5c,local_38);
      AEFile::ReadSwitched(&local_60,local_38);
      AEFile::ReadSwitched(&iStack_64,local_38);
      AEFile::ReadSwitched(&local_68,local_38);
      AEFile::ReadSwitched(&iStack_6c,local_38);
      AEFile::ReadSwitched(&local_70,local_38);
      pWVar4 = operator_new(0x54);
      iVar3 = local_3c;
      AbyssEngine::String::String(aSStack_7c,aSStack_34,false);
      Wanted::Wanted(pWVar4,iVar3,aSStack_7c,local_40,local_44,local_54 == 1,iStack_48,local_4c,
                     local_50,local_5c,local_60,iStack_64,local_68,iStack_6c,local_70);
      *(Wanted **)(*(int *)(this + 4) + uVar8 * 4) = pWVar4;
      AbyssEngine::String::~String(aSStack_7c);
      AEFile::ReadSwitched(&local_58,local_38);
      uVar1 = uVar8 + 1;
      if (0 < local_58) {
        piVar5 = operator_new__(0x14);
        for (uVar6 = 0; uVar6 < 5; uVar6 = uVar6 + 1) {
          AEFile::Read(&cStack_7d,local_38);
          piVar5[uVar6] = (int)cStack_7d;
        }
        Wanted::setImageParts(*(Wanted **)(*(int *)(this + 4) + uVar8 * 4),piVar5);
      }
    }
    AEFile::Close(local_38);
    AbyssEngine::String::~String(aSStack_34);
  }
  iVar3 = *piVar7 - local_28;
  if (iVar3 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
}

```

## target disasm
```
  00130490: push {r4,r5,r6,r7,lr}
  00130492: add r7,sp,#0xc
  00130494: push {r8,r9,r10,r11}
  00130498: sub sp,#0xa4
  0013049a: ldr r0,[0x001306a4]
  0013049c: movs r2,#0x0
  0013049e: add r0,pc
  001304a0: ldr r5,[r0,#0x0]
  001304a2: ldr r0,[r5,#0x0]
  001304a4: ldr r1,[0x001306a8]
  001304a6: str r0,[sp,#0xa0]
  001304a8: add r0,sp,#0x94
  001304aa: add r1,pc
  001304ac: blx 0x0006ee18
  001304b0: blx 0x0006fff4
  001304b4: mov r4,r0
  001304b6: add r0,sp,#0x94
  001304b8: blx 0x0006ee30
  001304bc: cmp r4,#0x0
  001304be: beq.w 0x00130652
  001304c2: str r5,[sp,#0x30]
  001304c4: add r1,sp,#0x90
  001304c6: ldr r0,[0x001306ac]
  001304c8: add r0,pc
  001304ca: blx 0x0006ecd4
  001304ce: movs r0,#0xc
  001304d0: blx 0x0006eb24
  001304d4: mov r10,r0
  001304d6: blx 0x000748e4
  001304da: movs r0,#0x19
  001304dc: mov r1,r10
  001304de: blx 0x000748f0
  001304e2: add r5,sp,#0x94
  001304e4: mov r0,r5
  001304e6: blx 0x0006efbc
  001304ea: add.w r9,sp,#0x8c
  001304ee: add r4,sp,#0x4c
  001304f0: mov.w r8,#0x0
  001304f4: b 0x00130634
  001304f6: mov r0,r5
  001304f8: blx 0x000762f4
  001304fc: mov r0,r5
  001304fe: blx 0x00076300
  00130502: ldr r1,[sp,#0x90]
  00130504: mov r0,r9
  00130506: blx 0x0007630c
  0013050a: ldr r1,[sp,#0x90]
  0013050c: add r0,sp,#0x88
  0013050e: blx 0x0007630c
  00130512: ldr r1,[sp,#0x90]
  00130514: add r0,sp,#0x84
  00130516: blx 0x0007630c
  0013051a: ldr r1,[sp,#0x90]
  0013051c: add r0,sp,#0x74
  0013051e: blx 0x0007630c
  00130522: ldr r1,[sp,#0x90]
  00130524: add r0,sp,#0x80
  00130526: blx 0x0007630c
  0013052a: ldr r1,[sp,#0x90]
  0013052c: add r0,sp,#0x7c
  0013052e: blx 0x0007630c
  00130532: ldr r1,[sp,#0x90]
  00130534: add r0,sp,#0x78
  00130536: blx 0x0007630c
  0013053a: ldr r1,[sp,#0x90]
  0013053c: add r0,sp,#0x6c
  0013053e: blx 0x0007630c
  00130542: ldr r1,[sp,#0x90]
  00130544: add r0,sp,#0x68
  00130546: blx 0x0007630c
  0013054a: ldr r1,[sp,#0x90]
  0013054c: add r0,sp,#0x64
  0013054e: blx 0x0007630c
  00130552: ldr r1,[sp,#0x90]
  00130554: add r0,sp,#0x60
  00130556: blx 0x0007630c
  0013055a: ldr r1,[sp,#0x90]
  0013055c: add r0,sp,#0x5c
  0013055e: blx 0x0007630c
  00130562: ldr r1,[sp,#0x90]
  00130564: add r0,sp,#0x58
  00130566: blx 0x0007630c
  0013056a: movs r0,#0x54
  0013056c: blx 0x0006eb24
  00130570: ldr.w r8,[sp,#0x8c]
  00130574: mov r9,r0
  00130576: mov r0,r4
  00130578: mov r1,r5
  0013057a: movs r2,#0x0
  0013057c: blx 0x0006f028
  00130580: ldr r0,[sp,#0x88]
  00130582: str r6,[sp,#0x44]
  00130584: mov r6,r10
  00130586: str r0,[sp,#0x40]
  00130588: ldr r1,[sp,#0x74]
  0013058a: ldr r2,[sp,#0x84]
  0013058c: ldrd r0,r3,[sp,#0x7c]
  00130590: str r0,[sp,#0x3c]
  00130592: ldr r0,[sp,#0x78]
  00130594: str r0,[sp,#0x38]
  00130596: ldr r0,[sp,#0x6c]
  00130598: str r0,[sp,#0x34]
  0013059a: ldr.w r12,[sp,#0x68]
  0013059e: ldrd r10,lr,[sp,#0x60]
  001305a2: ldrd r0,r11,[sp,#0x58]
  001305a6: strd r10,r11,[sp,#0x20]
  001305aa: str r0,[sp,#0x28]
  001305ac: subs r0,r1,#0x1
  001305ae: clz r0,r0
  001305b2: add r1,sp,#0x14
  001305b4: lsrs r0,r0,#0x5
  001305b6: strd r2,r0,[sp,#0x0]
  001305ba: mov r2,r4
  001305bc: ldr r0,[sp,#0x3c]
  001305be: str r0,[sp,#0xc]
  001305c0: ldr r0,[sp,#0x38]
  001305c2: str r0,[sp,#0x10]
  001305c4: ldr r0,[sp,#0x34]
  001305c6: str r3,[sp,#0x8]
  001305c8: stm r1,{r0,r12,lr}
  001305cc: mov r0,r9
  001305ce: mov r1,r8
  001305d0: ldr r3,[sp,#0x40]
  001305d2: blx 0x000749f8
  001305d6: mov r10,r6
  001305d8: ldr r0,[r6,#0x4]
  001305da: ldr r6,[sp,#0x44]
  001305dc: str.w r9,[r0,r6,lsl #0x2]
  001305e0: mov r0,r4
  001305e2: blx 0x0006ee30
  001305e6: ldr r1,[sp,#0x90]
  001305e8: add r0,sp,#0x70
  001305ea: blx 0x0007630c
  001305ee: add.w r9,sp,#0x8c
  001305f2: ldr r0,[sp,#0x70]
  001305f4: add.w r8,r6,#0x1
  001305f8: cmp r0,#0x1
  001305fa: blt 0x00130634
  001305fc: movs r0,#0x14
  001305fe: blx 0x0006ec08
  00130602: mov r11,r4
  00130604: mov r6,r0
  00130606: movs r4,#0x0
  00130608: b 0x0013061e
  0013060a: ldr r1,[sp,#0x90]
  0013060c: sub.w r0,r7,#0x75
  00130610: blx 0x00076324
  00130614: ldrsb.w r0,[r7,#-0x75]
  00130618: str.w r0,[r6,r4,lsl #0x2]
  0013061c: adds r4,#0x1
  0013061e: cmp r4,#0x5
  00130620: bcc 0x0013060a
  00130622: ldr.w r0,[r10,#0x4]
  00130626: ldr r1,[sp,#0x44]
  00130628: ldr.w r0,[r0,r1,lsl #0x2]
  0013062c: mov r1,r6
  0013062e: blx 0x00074a04
  00130632: mov r4,r11
  00130634: ldr.w r0,[r10,#0x0]
  00130638: mov r6,r8
  0013063a: ldr r1,[sp,#0x90]
  0013063c: cmp r8,r0
  0013063e: bcc.w 0x001304f6
  00130642: mov r0,r1
  00130644: blx 0x0006ece0
  00130648: ldr r5,[sp,#0x30]
  0013064a: add r0,sp,#0x94
  0013064c: blx 0x0006ee30
  00130650: b 0x00130656
  00130652: mov.w r10,#0x0
  00130656: ldr r0,[sp,#0xa0]
  00130658: ldr r1,[r5,#0x0]
  0013065a: subs r0,r1,r0
  0013065c: itttt eq
  0013065e: mov.eq r0,r10
  00130660: add.eq sp,#0xa4
  00130662: pop.eq.w {r8,r9,r10,r11}
  00130666: pop.eq {r4,r5,r6,r7,pc}
  00130668: blx 0x0006e824
```
