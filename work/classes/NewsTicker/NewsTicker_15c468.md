# NewsTicker::NewsTicker
elf 0x15c468 body 814
Sig: undefined __thiscall NewsTicker(NewsTicker * this, int param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* NewsTicker::NewsTicker(int, int, int, int, int) */

void __thiscall
NewsTicker::NewsTicker(NewsTicker *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  FileRead *this_00;
  Array *this_01;
  void *pvVar2;
  Array<NewsItem*> *this_02;
  int iVar3;
  int iVar4;
  NewsItem *pNVar5;
  String *pSVar6;
  undefined4 uVar7;
  int iVar8;
  __fn *__fn;
  undefined4 extraout_r1;
  void *extraout_r2;
  void *extraout_r2_00;
  void *pvVar9;
  int extraout_r3;
  int extraout_r3_00;
  int iVar10;
  int extraout_r3_01;
  int extraout_r3_02;
  int extraout_r3_03;
  uint uVar11;
  NewsItem *pNVar12;
  int *piVar13;
  int *piVar14;
  undefined4 *puVar15;
  String *this_03;
  int *piVar16;
  uint *puVar17;
  bool bVar18;
  uint in_fpscr;
  longlong lVar19;
  undefined8 uVar20;
  void *in_stack_ffffff80;
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  this_03 = (String *)(this + 0x14);
  piVar13 = *(int **)(DAT_0016c7fc + 0x16c480);
  local_28 = *piVar13;
  AbyssEngine::String::String(this_03);
  iVar10 = DAT_0016c818;
  *(int *)(this + 4) = param_1;
  *(int *)(this + 8) = param_2;
  *(int *)(this + 0xc) = param_3;
  AbyssEngine::String::String(aSStack_34,(char *)(iVar10 + 0x16c49c),false);
  AbyssEngine::String::operator=(this_03,aSStack_34);
  AbyssEngine::String::~String(aSStack_34);
  *(undefined4 *)(this + 0x10) = 0;
  this_00 = operator_new(1);
  FileRead::FileRead(this_00);
  this_01 = (Array *)FileRead::loadTicker();
  pvVar2 = (void *)FileRead::~FileRead(this_00);
  operator_delete(pvVar2);
  this_02 = operator_new(0xc);
  Array<NewsItem*>::Array(this_02);
  piVar14 = *(int **)(DAT_0016c81c + 0x16c4f0);
  iVar3 = AbyssEngine::AERandom::nextInt(*piVar14);
  iVar10 = extraout_r3;
  for (uVar11 = 0; uVar11 < *(uint *)this_01; uVar11 = uVar11 + 1) {
    pNVar12 = *(NewsItem **)(*(int *)(this_01 + 4) + uVar11 * 4);
    if ((((0 < *(int *)(pNVar12 + 0x10)) && (*(int *)(pNVar12 + 0x10) <= param_5)) &&
        (param_5 <= *(int *)(pNVar12 + 0x14))) &&
       ((__fn *)(uint)*(byte *)(*(int *)(pNVar12 + 8) + param_4) != (__fn *)0x0)) {
      pNVar12 = (NewsItem *)
                NewsItem::clone(pNVar12,(__fn *)(uint)*(byte *)(*(int *)(pNVar12 + 8) + param_4),
                                (void *)param_4,iVar10,in_stack_ffffff80);
      ArrayAdd<NewsItem*>(pNVar12,(Array *)this_02);
      iVar10 = extraout_r3_00;
    }
  }
  iVar8 = 0;
  pvVar2 = (void *)0x0;
  iVar10 = 0;
  piVar16 = *(int **)(DAT_0016c820 + 0x16c54a);
  do {
    pvVar9 = pvVar2;
    iVar4 = iVar10;
    iVar1 = iVar10;
    do {
      do {
        iVar10 = iVar1;
        if (iVar3 + 2 <= iVar8 || (void *)0x63 < pvVar2) {
          if (iVar10 != 0) {
            pNVar12 = (NewsItem *)
                      NewsItem::clone(*(NewsItem **)
                                       (*(__fn **)(this_01 + 4) + *(int *)(*piVar16 + 0x174) * 4),
                                      *(__fn **)(this_01 + 4),pvVar9,iVar4,in_stack_ffffff80);
            ArrayAdd<NewsItem*>(pNVar12,(Array *)this_02);
          }
          AbyssEngine::String::String(aSStack_34,(char *)(DAT_0016c824 + 0x16c644),false);
          piVar14 = *(int **)(DAT_0016c828 + 0x16c658);
          for (uVar11 = 0; uVar11 < *(uint *)this_02; uVar11 = uVar11 + 1) {
            iVar10 = *(int *)(*(int *)(this_02 + 4) + uVar11 * 4);
            pSVar6 = (String *)GameText::getText(*piVar14);
            AbyssEngine::String::String(aSStack_4c,pSVar6,false);
            replaceTokens(aSStack_40,extraout_r1,aSStack_4c);
            AbyssEngine::String::~String(aSStack_4c);
            if (*(char *)(iVar10 + 4) != '\0') {
              if (*(char *)(iVar10 + 0x18) == '\0') {
                AbyssEngine::String::operator=(aSStack_40,(String *)(*piVar16 + 0x168));
              }
              else {
                AbyssEngine::String::operator=((String *)(*piVar16 + 0x168),aSStack_40);
              }
            }
            AbyssEngine::operator+(aSStack_58,aSStack_40);
            AbyssEngine::String::operator+=(this_03,aSStack_58);
            AbyssEngine::String::~String(aSStack_58);
            AbyssEngine::String::~String(aSStack_40);
          }
          puVar15 = *(undefined4 **)(DAT_0016c82c + 0x16c6ec);
          puVar17 = *(uint **)(DAT_0016c830 + 0x16c6ee);
          iVar10 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar17,(String *)*puVar15);
          *(int *)(this + 0x10) = iVar10;
          if (iVar10 < param_3) {
            AbyssEngine::String::String(aSStack_40,this_03,false);
            AbyssEngine::String::operator+=(this_03,aSStack_40);
            AbyssEngine::String::~String(aSStack_40);
            uVar7 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar17,(String *)*puVar15);
            *(undefined4 *)(this + 0x10) = uVar7;
          }
          iVar10 = GameText::getLanguage();
          if (iVar10 == 9) {
            param_3 = -*(int *)(this + 0x10);
          }
          uVar7 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
          *(undefined4 *)(this + 0x2c) = 0;
          this[0x28] = (NewsTicker)0x0;
          *(undefined4 *)this = uVar7;
          ArrayReleaseClasses<NewsItem*>((Array *)this_02);
          pvVar2 = (void *)Array<NewsItem*>::~Array(this_02);
          operator_delete(pvVar2);
          ArrayReleaseClasses<NewsItem*>(this_01);
          pvVar2 = (void *)Array<NewsItem*>::~Array((Array<NewsItem*> *)this_01);
          operator_delete(pvVar2);
          AbyssEngine::String::~String(aSStack_34);
          iVar10 = *piVar13 - local_28;
          if (iVar10 != 0) {
                    /* WARNING: Subroutine does not return */
            __stack_chk_fail(iVar10);
          }
          return;
        }
        iVar4 = AbyssEngine::AERandom::nextInt(*piVar14);
        pNVar12 = *(NewsItem **)(*(int *)(this_01 + 4) + iVar4 * 4);
        Status::getSystem();
        iVar4 = SolarSystem::getIndex();
      } while ((0x15 < iVar4) &&
              (pvVar9 = extraout_r2, iVar4 = extraout_r3_01, iVar1 = iVar10, *(int *)pNVar12 == 0xd)
              );
      if ((((*(int *)(pNVar12 + 0x14) < 0xa1) ||
           ((param_5 < *(int *)(pNVar12 + 0x10) ||
            (iVar4 = AbyssEngine::AERandom::nextInt(*piVar14), 0x31 < iVar4)))) ||
          (*(char *)(*(int *)(pNVar12 + 8) + param_4) == '\0')) || (pNVar12[0x18] != (NewsItem)0x0))
      goto LAB_0016c618;
      __fn = (__fn *)param_4;
      pvVar9 = extraout_r2_00;
      iVar4 = extraout_r3_02;
      if (pNVar12[4] == (NewsItem)0x0) goto LAB_0016c600;
      lVar19 = Status::getPlayingTime();
      lVar19 = lVar19 - *(longlong *)(*piVar16 + 0x160);
      iVar4 = (int)((ulonglong)lVar19 >> 0x20);
      bVar18 = DAT_0016c814 < (uint)lVar19;
      pvVar9 = (void *)(-(uint)bVar18 - iVar4);
      iVar1 = 1;
    } while ((int)pvVar9 < 0 == (SBORROW4(0,iVar4) != SBORROW4(-iVar4,(uint)bVar18)));
    uVar20 = Status::getPlayingTime();
    __fn = (__fn *)((ulonglong)uVar20 >> 0x20);
    pvVar9 = (void *)*piVar16;
    *(undefined8 *)((int)pvVar9 + 0x160) = uVar20;
    *(undefined4 *)((int)pvVar9 + 0x174) = *(undefined4 *)pNVar12;
    iVar4 = extraout_r3_03;
LAB_0016c600:
    pNVar5 = (NewsItem *)NewsItem::clone(pNVar12,__fn,pvVar9,iVar4,in_stack_ffffff80);
    ArrayAdd<NewsItem*>(pNVar5,(Array *)this_02);
    pNVar12[0x18] = (NewsItem)0x1;
    iVar8 = iVar8 + 1;
LAB_0016c618:
    pvVar2 = (void *)((int)pvVar2 + 1);
  } while( true );
}

```

## target disasm
```
  0016c468: push {r4,r5,r6,r7,lr}
  0016c46a: add r7,sp,#0xc
  0016c46c: push {r8,r9,r10,r11}
  0016c470: sub sp,#0x5c
  0016c472: mov r11,r0
  0016c474: ldr r0,[0x0016c7fc]
  0016c476: add.w r9,r11,#0x14
  0016c47a: mov r6,r3
  0016c47c: add r0,pc
  0016c47e: mov r4,r2
  0016c480: mov r5,r1
  0016c482: ldr.w r8,[r0,#0x0]
  0016c486: ldr.w r0,[r8,#0x0]
  0016c48a: str r0,[sp,#0x58]
  0016c48c: mov r0,r9
  0016c48e: blx 0x0006efbc
  0016c492: ldr r1,[0x0016c818]
  0016c494: strd r5,r4,[r11,#0x4]
  0016c498: add r1,pc
  0016c49a: str r6,[sp,#0x14]
  0016c49c: str.w r6,[r11,#0xc]
  0016c4a0: add r0,sp,#0x4c
  0016c4a2: movs r2,#0x0
  0016c4a4: blx 0x0006ee18
  0016c4a8: add r1,sp,#0x4c
  0016c4aa: mov r0,r9
  0016c4ac: blx 0x0006f2b0
  0016c4b0: add r0,sp,#0x4c
  0016c4b2: blx 0x0006ee30
  0016c4b6: movs r0,#0x0
  0016c4b8: str.w r0,[r11,#0x10]
  0016c4bc: movs r0,#0x1
  0016c4be: blx 0x0006eb24
  0016c4c2: mov r4,r0
  0016c4c4: blx 0x00072124
  0016c4c8: mov r0,r4
  0016c4ca: blx 0x00077848
  0016c4ce: mov r6,r0
  0016c4d0: mov r0,r4
  0016c4d2: blx 0x0007213c
  0016c4d6: blx 0x0006eb48
  0016c4da: movs r0,#0xc
  0016c4dc: blx 0x0006eb24
  0016c4e0: mov r10,r0
  0016c4e2: str.w r8,[sp,#0x10]
  0016c4e6: blx 0x00076384
  0016c4ea: ldr r0,[0x0016c81c]
  0016c4ec: add r0,pc
  0016c4ee: ldr.w r8,[r0,#0x0]
  0016c4f2: ldr.w r0,[r8,#0x0]
  0016c4f6: movs r1,#0x1
  0016c4f8: blx 0x00071848
  0016c4fc: ldr r4,[r7,#0xc]
  0016c4fe: movs r5,#0x0
  0016c500: adds r0,#0x2
  0016c502: str r0,[sp,#0x20]
  0016c504: b 0x0016c530
  0016c506: ldr r0,[r6,#0x4]
  0016c508: ldr.w r0,[r0,r5,lsl #0x2]
  0016c50c: ldr r1,[r0,#0x10]
  0016c50e: cmp r1,#0x1
  0016c510: blt 0x0016c52e
  0016c512: cmp r1,r4
  0016c514: bgt 0x0016c52e
  0016c516: ldr r1,[r0,#0x14]
  0016c518: cmp r1,r4
  0016c51a: blt 0x0016c52e
  0016c51c: ldr r1,[r0,#0x8]
  0016c51e: ldr r2,[r7,#0x8]
  0016c520: ldrb r1,[r1,r2]
  0016c522: cbz r1,0x0016c52e
  0016c524: blx 0x00077854
  0016c528: mov r1,r10
  0016c52a: blx 0x00077860
  0016c52e: adds r5,#0x1
  0016c530: ldr r0,[r6,#0x0]
  0016c532: cmp r5,r0
  0016c534: bcc 0x0016c506
  0016c536: str.w r11,[sp,#0x4]
  0016c53a: movs r1,#0x0
  0016c53c: str.w r9,[sp,#0x24]
  0016c540: movs r2,#0x0
  0016c542: ldr r0,[0x0016c820]
  0016c544: movs r3,#0x0
  0016c546: add r0,pc
  0016c548: ldr.w r9,[r0,#0x0]
  0016c54c: ldr r0,[sp,#0x20]
  0016c54e: str r1,[sp,#0xc]
  0016c550: cmp r1,r0
  0016c552: mov.w r0,#0x0
  0016c556: mov.w r1,#0x0
  0016c55a: it lt
  0016c55c: mov.lt r0,#0x1
  0016c55e: str r2,[sp,#0x8]
  0016c560: cmp r2,#0x64
  0016c562: it cc
  0016c564: mov.cc r1,#0x1
  0016c566: and.w r11,r0,r1
  0016c56a: mov r1,r3
  0016c56c: str r1,[sp,#0x1c]
  0016c56e: cmp.w r11,#0x0
  0016c572: beq 0x0016c620
  0016c574: ldr r1,[r6,#0x0]
  0016c576: ldr.w r0,[r8,#0x0]
  0016c57a: blx 0x00071848
  0016c57e: ldr r1,[r6,#0x4]
  0016c580: ldr.w r5,[r1,r0,lsl #0x2]
  0016c584: ldr.w r0,[r9,#0x0]
  0016c588: blx 0x00071a10
  0016c58c: blx 0x00071a7c
  0016c590: cmp r0,#0x16
  0016c592: blt 0x0016c59a
  0016c594: ldr r0,[r5,#0x0]
  0016c596: cmp r0,#0xd
  0016c598: beq 0x0016c56e
  0016c59a: ldr r0,[r5,#0x14]
  0016c59c: cmp r0,#0xa1
  0016c59e: blt 0x0016c616
  0016c5a0: ldr r0,[r5,#0x10]
  0016c5a2: cmp r0,r4
  0016c5a4: bgt 0x0016c616
  0016c5a6: ldr.w r0,[r8,#0x0]
  0016c5aa: movs r1,#0x64
  0016c5ac: blx 0x00071848
  0016c5b0: cmp r0,#0x31
  0016c5b2: bgt 0x0016c616
  0016c5b4: ldr r0,[r5,#0x8]
  0016c5b6: ldr r1,[r7,#0x8]
  0016c5b8: ldrb r0,[r0,r1]
  0016c5ba: cbz r0,0x0016c616
  0016c5bc: ldrb r0,[r5,#0x18]
  0016c5be: cbnz r0,0x0016c616
  0016c5c0: ldrb r0,[r5,#0x4]
  0016c5c2: cbz r0,0x0016c600
  0016c5c4: ldr.w r0,[r9,#0x0]
  0016c5c8: blx 0x00071710
  0016c5cc: mov r2,r0
  0016c5ce: ldr.w r0,[r9,#0x0]
  0016c5d2: ldrd r3,r12,[r0,#0x160]
  0016c5d6: subs.w lr,r2,r3
  0016c5da: ldr r2,[0x0016c814]
  0016c5dc: sbc.w r3,r1,r12
  0016c5e0: movs r1,#0x1
  0016c5e2: subs.w r2,r2,lr
  0016c5e6: mov.w r2,#0x0
  0016c5ea: sbcs r2,r3
  0016c5ec: bge 0x0016c56c
  0016c5ee: blx 0x00071710
  0016c5f2: ldr.w r2,[r9,#0x0]
  0016c5f6: strd r0,r1,[r2,#0x160]
  0016c5fa: ldr r0,[r5,#0x0]
  0016c5fc: str.w r0,[r2,#0x174]
  0016c600: mov r0,r5
  0016c602: blx 0x00077854
  0016c606: mov r1,r10
  0016c608: blx 0x00077860
  0016c60c: ldr r1,[sp,#0xc]
  0016c60e: movs r0,#0x1
  0016c610: strb r0,[r5,#0x18]
  0016c612: adds r1,#0x1
  0016c614: b 0x0016c618
  0016c616: ldr r1,[sp,#0xc]
  0016c618: ldr r2,[sp,#0x8]
  0016c61a: ldr r3,[sp,#0x1c]
  0016c61c: adds r2,#0x1
  0016c61e: b 0x0016c54c
  0016c620: ldr r0,[sp,#0x1c]
  0016c622: lsls r0,r0,#0x1f
  0016c624: beq 0x0016c63e
  0016c626: ldr.w r0,[r9,#0x0]
  0016c62a: ldr r1,[r6,#0x4]
  0016c62c: ldr.w r0,[r0,#0x174]
  0016c630: ldr.w r0,[r1,r0,lsl #0x2]
  0016c634: blx 0x00077854
  0016c638: mov r1,r10
  0016c63a: blx 0x00077860
  0016c63e: ldr r1,[0x0016c824]
  0016c640: add r1,pc
  0016c642: add r0,sp,#0x4c
  0016c644: movs r2,#0x0
  0016c646: blx 0x0006ee18
  0016c64a: ldr r0,[0x0016c828]
  0016c64c: add.w r11,sp,#0x34
  0016c650: add r5,sp,#0x28
  0016c652: movs r4,#0x0
  0016c654: add r0,pc
  0016c656: str.w r10,[sp,#0x18]
  0016c65a: ldr r0,[r0,#0x0]
  0016c65c: str r0,[sp,#0x20]
  0016c65e: b 0x0016c6dc
  0016c660: ldr.w r0,[r10,#0x4]
  0016c664: ldr.w r8,[r0,r4,lsl #0x2]
  0016c668: ldr r0,[sp,#0x20]
  0016c66a: ldr.w r1,[r8,#0x0]
  0016c66e: ldr r0,[r0,#0x0]
  0016c670: addw r1,r1,#0xcbe
  0016c674: blx 0x00072f70
  0016c678: mov r1,r0
  0016c67a: mov r0,r11
  0016c67c: movs r2,#0x0
  0016c67e: blx 0x0006f028
  0016c682: add r0,sp,#0x40
  0016c684: mov r2,r11
  0016c686: blx 0x0007786c
  0016c68a: mov r0,r11
  0016c68c: mov r10,r11
  0016c68e: blx 0x0006ee30
  0016c692: ldrb.w r0,[r8,#0x4]
  0016c696: cbz r0,0x0016c6b6
  0016c698: ldr.w r0,[r9,#0x0]
  0016c69c: ldrb.w r2,[r8,#0x18]
  0016c6a0: add.w r1,r0,#0x168
  0016c6a4: cbz r2,0x0016c6b0
  0016c6a6: mov r0,r1
  0016c6a8: add r1,sp,#0x40
  0016c6aa: blx 0x0006f2b0
  0016c6ae: b 0x0016c6b6
  0016c6b0: add r0,sp,#0x40
  0016c6b2: blx 0x0006f2b0
  0016c6b6: add r1,sp,#0x40
  0016c6b8: add r2,sp,#0x4c
  0016c6ba: mov r0,r5
  0016c6bc: blx 0x0006ef98
  0016c6c0: ldr r0,[sp,#0x24]
  0016c6c2: mov r1,r5
  0016c6c4: blx 0x0006ef5c
  0016c6c8: mov r0,r5
  0016c6ca: blx 0x0006ee30
  0016c6ce: add r0,sp,#0x40
  0016c6d0: blx 0x0006ee30
  0016c6d4: mov r11,r10
  0016c6d6: adds r4,#0x1
  0016c6d8: ldr.w r10,[sp,#0x18]
  0016c6dc: ldr.w r0,[r10,#0x0]
  0016c6e0: cmp r4,r0
  0016c6e2: bcc 0x0016c660
  0016c6e4: ldr r0,[0x0016c82c]
  0016c6e6: ldr r1,[0x0016c830]
  0016c6e8: add r0,pc
  0016c6ea: add r1,pc
  0016c6ec: ldr.w r8,[r0,#0x0]
  0016c6f0: ldr.w r11,[r1,#0x0]
  0016c6f4: ldr.w r1,[r8,#0x0]
  0016c6f8: ldr.w r0,[r11,#0x0]
  0016c6fc: ldr.w r9,[sp,#0x24]
  0016c700: mov r2,r9
  0016c702: blx 0x0006faa8
  0016c706: ldr r4,[sp,#0x4]
  0016c708: ldr r5,[sp,#0x14]
  0016c70a: cmp r0,r5
  0016c70c: str r0,[r4,#0x10]
  0016c70e: bge 0x0016c738
  0016c710: add r0,sp,#0x40
  0016c712: mov r1,r9
  0016c714: movs r2,#0x0
  0016c716: blx 0x0006f028
  0016c71a: add r1,sp,#0x40
  0016c71c: mov r0,r9
  0016c71e: blx 0x0006ef5c
  0016c722: add r0,sp,#0x40
  0016c724: blx 0x0006ee30
  0016c728: ldr.w r1,[r8,#0x0]
  0016c72c: ldr.w r0,[r11,#0x0]
  0016c730: mov r2,r9
  0016c732: blx 0x0006faa8
  0016c736: str r0,[r4,#0x10]
  0016c738: blx 0x0006f544
  0016c73c: cmp r0,#0x9
  0016c73e: itt eq
  0016c740: ldr.eq r0,[r4,#0x10]
  0016c742: rsb.eq r5,r0
  0016c744: movs r0,#0x0
  0016c746: vmov s0,r5
  0016c74a: vcvt.f32.s32 s0,s0
  0016c74e: str r0,[r4,#0x2c]
  0016c750: strb.w r0,[r4,#0x28]
  0016c754: mov r0,r10
  0016c756: vstr.32 s0,[r4]
  0016c75a: blx 0x00077878
  0016c75e: mov r0,r10
  0016c760: blx 0x00077884
  0016c764: blx 0x0006eb48
  0016c768: mov r0,r6
  0016c76a: blx 0x00077878
  0016c76e: mov r0,r6
  0016c770: blx 0x00077884
  0016c774: blx 0x0006eb48
  0016c778: add r0,sp,#0x4c
  0016c77a: blx 0x0006ee30
  0016c77e: ldr r0,[sp,#0x58]
  0016c780: ldr r1,[sp,#0x10]
  0016c782: ldr r1,[r1,#0x0]
  0016c784: subs r0,r1,r0
  0016c786: itttt eq
  0016c788: mov.eq r0,r4
  0016c78a: add.eq sp,#0x5c
  0016c78c: pop.eq.w {r8,r9,r10,r11}
  0016c790: pop.eq {r4,r5,r6,r7,pc}
  0016c792: blx 0x0006e824
```
