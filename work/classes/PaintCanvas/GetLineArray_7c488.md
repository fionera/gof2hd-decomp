# PaintCanvas::GetLineArray
elf 0x7c488 body 422
Sig: undefined __thiscall GetLineArray(PaintCanvas * this, uint param_1, String * param_2, int param_3, Array * param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetLineArray(unsigned int, AbyssEngine::String const&, int,
   Array<AbyssEngine::String*>*) */

void __thiscall
AbyssEngine::PaintCanvas::GetLineArray
          (PaintCanvas *this,uint param_1,String *param_2,int param_3,Array *param_4)

{
  String *pSVar1;
  short *psVar2;
  uint uVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [8];
  int local_2c;
  int local_28;
  
  piVar4 = *(int **)(DAT_0008c684 + 0x8c4a0);
  local_28 = *piVar4;
  pSVar1 = operator_new(0xc);
  String::String(pSVar1);
  String::String(aSStack_34,param_2,false);
  String::String(aSStack_40,(char *)(DAT_0008c688 + 0x8c4c0),false);
  String::operator+=(aSStack_34,aSStack_40);
  String::~String(aSStack_40);
  uVar3 = 0;
  for (iVar7 = 0; iVar7 < local_2c; iVar7 = iVar7 + iVar6) {
    String::SubString((uint)aSStack_40,(uint)aSStack_34);
    String::String(aSStack_4c,aSStack_40,false);
    GetLine(this,param_1,aSStack_4c,param_3,pSVar1);
    String::~String(aSStack_4c);
    iVar6 = *(int *)(pSVar1 + 8);
    String::~String(aSStack_40);
    uVar3 = uVar3 + 1;
  }
  (**(code **)(*(int *)pSVar1 + 4))(pSVar1);
  ArraySetLength<AbyssEngine::String*>(uVar3,param_4);
  for (uVar5 = 0; uVar5 < uVar3; uVar5 = uVar5 + 1) {
    pSVar1 = operator_new(0xc);
    String::String(pSVar1);
    *(String **)(*(int *)(param_4 + 4) + uVar5 * 4) = pSVar1;
  }
  uVar5 = 0;
  while( true ) {
    if (uVar3 <= uVar5) break;
    String::SubString((uint)aSStack_40,(uint)aSStack_34);
    String::String(aSStack_58,aSStack_40,false);
    GetLine(this,param_1,aSStack_58,param_3,*(undefined4 *)(*(int *)(param_4 + 4) + uVar5 * 4));
    String::~String(aSStack_58);
    iVar7 = 0;
    pSVar1 = *(String **)(*(int *)(param_4 + 4) + uVar5 * 4);
    iVar6 = *(int *)(pSVar1 + 8);
    while (psVar2 = (short *)String::operator[](pSVar1,iVar7), *psVar2 == 0x20) {
      iVar7 = iVar7 + 1;
      pSVar1 = *(String **)(*(int *)(param_4 + 4) + uVar5 * 4);
    }
    iVar6 = iVar6 + 1;
    do {
      psVar2 = (short *)String::operator[](*(String **)(*(int *)(param_4 + 4) + uVar5 * 4),
                                           iVar6 + -2);
      iVar6 = iVar6 + -1;
    } while (*psVar2 == 0x20);
    String::SubString((uint)aSStack_64,*(uint *)(*(int *)(param_4 + 4) + uVar5 * 4));
    String::operator=(*(String **)(*(int *)(param_4 + 4) + uVar5 * 4),aSStack_64);
    String::~String(aSStack_64);
    String::~String(aSStack_40);
    uVar5 = uVar5 + 1;
  }
  String::~String(aSStack_34);
  if (*piVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008c488: push {r4,r5,r6,r7,lr}
  0008c48a: add r7,sp,#0xc
  0008c48c: push {r8,r9,r10,r11}
  0008c490: sub sp,#0x54
  0008c492: str r3,[sp,#0x10]
  0008c494: mov r4,r2
  0008c496: strd r0,r1,[sp,#0x8]
  0008c49a: ldr r0,[0x0008c684]
  0008c49c: add r0,pc
  0008c49e: ldr r6,[r0,#0x0]
  0008c4a0: ldr r0,[r6,#0x0]
  0008c4a2: str r0,[sp,#0x50]
  0008c4a4: movs r0,#0xc
  0008c4a6: blx 0x0006eb24
  0008c4aa: mov r5,r0
  0008c4ac: blx 0x0006efbc
  0008c4b0: add r0,sp,#0x44
  0008c4b2: mov r1,r4
  0008c4b4: movs r2,#0x0
  0008c4b6: blx 0x0006f028
  0008c4ba: ldr r1,[0x0008c688]
  0008c4bc: add r1,pc
  0008c4be: add r0,sp,#0x38
  0008c4c0: movs r2,#0x0
  0008c4c2: blx 0x0006ee18
  0008c4c6: add r0,sp,#0x44
  0008c4c8: add r1,sp,#0x38
  0008c4ca: str r6,[sp,#0x4]
  0008c4cc: blx 0x0006ef5c
  0008c4d0: add r6,sp,#0x38
  0008c4d2: mov r0,r6
  0008c4d4: blx 0x0006ee30
  0008c4d8: add.w r8,sp,#0x44
  0008c4dc: add.w r9,sp,#0x2c
  0008c4e0: movs r4,#0x0
  0008c4e2: mov.w r11,#0x0
  0008c4e6: b 0x0008c51e
  0008c4e8: mov r0,r6
  0008c4ea: mov r1,r8
  0008c4ec: mov r2,r11
  0008c4ee: blx 0x0006f604
  0008c4f2: mov r0,r9
  0008c4f4: mov r1,r6
  0008c4f6: movs r2,#0x0
  0008c4f8: blx 0x0006f028
  0008c4fc: str r5,[sp,#0x0]
  0008c4fe: mov r2,r9
  0008c500: ldrd r0,r1,[sp,#0x8]
  0008c504: ldr r3,[sp,#0x10]
  0008c506: blx 0x0006fdfc
  0008c50a: mov r0,r9
  0008c50c: blx 0x0006ee30
  0008c510: mov r0,r6
  0008c512: ldr.w r10,[r5,#0x8]
  0008c516: blx 0x0006ee30
  0008c51a: add r11,r10
  0008c51c: adds r4,#0x1
  0008c51e: ldr r3,[sp,#0x4c]
  0008c520: cmp r11,r3
  0008c522: blt 0x0008c4e8
  0008c524: ldr r0,[r5,#0x0]
  0008c526: ldr r1,[r0,#0x4]
  0008c528: mov r0,r5
  0008c52a: blx r1
  0008c52c: ldr.w r10,[r7,#0x8]
  0008c530: mov r0,r4
  0008c532: mov r1,r10
  0008c534: blx 0x0006fe08
  0008c538: movs r6,#0x0
  0008c53a: b 0x0008c552
  0008c53c: movs r0,#0xc
  0008c53e: blx 0x0006eb24
  0008c542: mov r5,r0
  0008c544: blx 0x0006efbc
  0008c548: ldr.w r0,[r10,#0x4]
  0008c54c: str.w r5,[r0,r6,lsl #0x2]
  0008c550: adds r6,#0x1
  0008c552: cmp r6,r4
  0008c554: bcc 0x0008c53c
  0008c556: add r6,sp,#0x38
  0008c558: add r1,sp,#0x44
  0008c55a: add r5,sp,#0x20
  0008c55c: mov.w r8,#0x0
  0008c560: mov.w r9,#0x0
  0008c564: b 0x0008c60e
  0008c566: ldr r3,[sp,#0x4c]
  0008c568: mov r0,r6
  0008c56a: mov r2,r8
  0008c56c: blx 0x0006f604
  0008c570: mov r0,r5
  0008c572: mov r1,r6
  0008c574: movs r2,#0x0
  0008c576: blx 0x0006f028
  0008c57a: ldr.w r0,[r10,#0x4]
  0008c57e: ldr.w r0,[r0,r9,lsl #0x2]
  0008c582: str r0,[sp,#0x0]
  0008c584: mov r2,r5
  0008c586: ldrd r0,r1,[sp,#0x8]
  0008c58a: ldr r3,[sp,#0x10]
  0008c58c: blx 0x0006fdfc
  0008c590: mov r0,r5
  0008c592: blx 0x0006ee30
  0008c596: ldr.w r0,[r10,#0x4]
  0008c59a: movs r5,#0x0
  0008c59c: ldr.w r0,[r0,r9,lsl #0x2]
  0008c5a0: ldr r6,[r0,#0x8]
  0008c5a2: add r8,r6
  0008c5a4: b 0x0008c5b0
  0008c5a6: ldr.w r0,[r10,#0x4]
  0008c5aa: adds r5,#0x1
  0008c5ac: ldr.w r0,[r0,r9,lsl #0x2]
  0008c5b0: mov r1,r5
  0008c5b2: blx 0x0006f3b8
  0008c5b6: ldrh r0,[r0,#0x0]
  0008c5b8: cmp r0,#0x20
  0008c5ba: beq 0x0008c5a6
  0008c5bc: adds r6,#0x1
  0008c5be: ldr.w r0,[r10,#0x4]
  0008c5c2: ldr.w r0,[r0,r9,lsl #0x2]
  0008c5c6: subs r1,r6,#0x2
  0008c5c8: blx 0x0006f3b8
  0008c5cc: ldrh r0,[r0,#0x0]
  0008c5ce: subs r6,#0x1
  0008c5d0: cmp r0,#0x20
  0008c5d2: beq 0x0008c5be
  0008c5d4: ldr.w r0,[r10,#0x4]
  0008c5d8: ldr.w r1,[r0,r9,lsl #0x2]
  0008c5dc: add.w r11,sp,#0x14
  0008c5e0: mov r2,r5
  0008c5e2: mov r3,r6
  0008c5e4: mov r0,r11
  0008c5e6: blx 0x0006f604
  0008c5ea: ldr.w r0,[r10,#0x4]
  0008c5ee: ldr.w r0,[r0,r9,lsl #0x2]
  0008c5f2: mov r1,r11
  0008c5f4: blx 0x0006f2b0
  0008c5f8: mov r0,r11
  0008c5fa: blx 0x0006ee30
  0008c5fe: add r6,sp,#0x38
  0008c600: mov r0,r6
  0008c602: blx 0x0006ee30
  0008c606: add.w r9,r9,#0x1
  0008c60a: add r1,sp,#0x44
  0008c60c: add r5,sp,#0x20
  0008c60e: cmp r9,r4
  0008c610: bcc 0x0008c566
  0008c612: add r0,sp,#0x44
  0008c614: blx 0x0006ee30
  0008c618: ldr r0,[sp,#0x50]
  0008c61a: ldr r1,[sp,#0x4]
  0008c61c: ldr r1,[r1,#0x0]
  0008c61e: subs r0,r1,r0
  0008c620: ittt eq
  0008c622: add.eq sp,#0x54
  0008c624: pop.eq.w {r8,r9,r10,r11}
  0008c628: pop.eq {r4,r5,r6,r7,pc}
  0008c62a: blx 0x0006e824
```
