# FileRead::loadTicker
elf 0x121144 body 286
Sig: undefined __stdcall loadTicker(void)

## decompile
```c

/* FileRead::loadTicker() */

void FileRead::loadTicker(void)

{
  String *pSVar1;
  int iVar2;
  Array<NewsItem*> *this;
  bool *pbVar3;
  NewsItem *this_00;
  uint *extraout_r2;
  uint uVar4;
  int *piVar5;
  code *pcVar6;
  code *pcVar7;
  int local_48;
  int iStack_44;
  int local_40;
  uint local_3c;
  String local_38 [4];
  undefined1 auStack_34 [4];
  undefined1 auStack_30 [4];
  undefined1 auStack_2c [4];
  int local_28;
  
  piVar5 = *(int **)(DAT_00131280 + 0x131156);
  local_28 = *piVar5;
  pSVar1 = (String *)AbyssEngine::String::String(local_38,(char *)(DAT_00131284 + 0x131166),false);
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String(local_38);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_00131288 + 0x131186),(char *)&local_3c,extraout_r2);
    this = operator_new(0xc);
    Array<NewsItem*>::Array(this);
    ArraySetLength<NewsItem*>(0x3b,(Array *)this);
    pcVar6 = *(code **)(DAT_0013128c + 0x1311b4);
    pcVar7 = *(code **)(DAT_00131290 + 0x1311bc);
    for (uVar4 = 0; uVar4 < 0x3b; uVar4 = uVar4 + 1) {
      (*pcVar6)(&local_40);
      (*pcVar7)(local_38,local_3c);
      (*pcVar7)(auStack_34,local_3c);
      (*pcVar7)(auStack_30,local_3c);
      (*pcVar7)(auStack_2c,local_3c);
      (*pcVar6)(&iStack_44,local_3c);
      (*pcVar6)(&local_48,local_3c);
      pbVar3 = operator_new__(4);
      for (iVar2 = 0; iVar2 != 4; iVar2 = iVar2 + 1) {
        pbVar3[iVar2] = *(int *)(local_38 + iVar2 * 4) != 0;
      }
      this_00 = operator_new(0x1c);
      NewsItem::NewsItem(this_00,uVar4,local_40 != 0,pbVar3,4,iStack_44,local_48);
      *(NewsItem **)(*(int *)(this + 4) + uVar4 * 4) = this_00;
    }
    AEFile::Close(local_3c);
  }
  iVar2 = *piVar5 - local_28;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  00131144: push {r4,r5,r6,r7,lr}
  00131146: add r7,sp,#0xc
  00131148: push {r8,r9,r10,r11}
  0013114c: sub sp,#0x44
  0013114e: ldr r0,[0x00131280]
  00131150: movs r2,#0x0
  00131152: add r0,pc
  00131154: ldr.w r8,[r0,#0x0]
  00131158: ldr.w r0,[r8,#0x0]
  0013115c: ldr r1,[0x00131284]
  0013115e: str r0,[sp,#0x40]
  00131160: add r0,sp,#0x30
  00131162: add r1,pc
  00131164: blx 0x0006ee18
  00131168: blx 0x0006fff4
  0013116c: add r5,sp,#0x30
  0013116e: mov r4,r0
  00131170: mov r0,r5
  00131172: blx 0x0006ee30
  00131176: cmp r4,#0x0
  00131178: beq 0x00131248
  0013117a: str.w r8,[sp,#0x10]
  0013117e: add r1,sp,#0x2c
  00131180: ldr r0,[0x00131288]
  00131182: add r0,pc
  00131184: blx 0x0006ecd4
  00131188: movs r0,#0xc
  0013118a: blx 0x0006eb24
  0013118e: mov r6,r0
  00131190: blx 0x00076384
  00131194: movs r0,#0x3b
  00131196: mov r1,r6
  00131198: blx 0x00076390
  0013119c: add.w r0,r5,#0xc
  001311a0: str r0,[sp,#0x1c]
  001311a2: add.w r0,r5,#0x8
  001311a6: str r0,[sp,#0x18]
  001311a8: adds r0,r5,#0x4
  001311aa: str r0,[sp,#0x14]
  001311ac: ldr r0,[0x0013128c]
  001311ae: movs r4,#0x0
  001311b0: add r0,pc
  001311b2: ldr.w r9,[r0,#0x0]
  001311b6: ldr r0,[0x00131290]
  001311b8: add r0,pc
  001311ba: ldr.w r10,[r0,#0x0]
  001311be: b 0x00131236
  001311c0: add r0,sp,#0x28
  001311c2: blx r9
  001311c4: ldr r1,[sp,#0x2c]
  001311c6: mov r0,r5
  001311c8: blx r10
  001311ca: ldr r1,[sp,#0x2c]
  001311cc: ldr r0,[sp,#0x14]
  001311ce: blx r10
  001311d0: ldr r1,[sp,#0x2c]
  001311d2: ldr r0,[sp,#0x18]
  001311d4: blx r10
  001311d6: ldr r1,[sp,#0x2c]
  001311d8: ldr r0,[sp,#0x1c]
  001311da: blx r10
  001311dc: ldr r1,[sp,#0x2c]
  001311de: add r0,sp,#0x24
  001311e0: blx r9
  001311e2: ldr r1,[sp,#0x2c]
  001311e4: add r0,sp,#0x20
  001311e6: blx r9
  001311e8: movs r0,#0x4
  001311ea: blx 0x0006ec08
  001311ee: mov r11,r0
  001311f0: movs r0,#0x0
  001311f2: b 0x00131204
  001311f4: ldr.w r1,[r5,r0,lsl #0x2]
  001311f8: cmp r1,#0x0
  001311fa: it ne
  001311fc: mov.ne r1,#0x1
  001311fe: strb.w r1,[r11,r0]
  00131202: adds r0,#0x1
  00131204: cmp r0,#0x4
  00131206: bne 0x001311f4
  00131208: movs r0,#0x1c
  0013120a: blx 0x0006eb24
  0013120e: mov r8,r0
  00131210: ldr r2,[sp,#0x28]
  00131212: ldrd r1,r0,[sp,#0x20]
  00131216: movs r3,#0x4
  00131218: cmp r2,#0x0
  0013121a: strd r3,r0,[sp,#0x0]
  0013121e: mov r0,r8
  00131220: str r1,[sp,#0x8]
  00131222: it ne
  00131224: mov.ne r2,#0x1
  00131226: mov r1,r4
  00131228: mov r3,r11
  0013122a: blx 0x0007639c
  0013122e: ldr r0,[r6,#0x4]
  00131230: str.w r8,[r0,r4,lsl #0x2]
  00131234: adds r4,#0x1
  00131236: ldr r1,[sp,#0x2c]
  00131238: cmp r4,#0x3b
  0013123a: bcc 0x001311c0
  0013123c: mov r0,r1
  0013123e: blx 0x0006ece0
  00131242: ldr.w r8,[sp,#0x10]
  00131246: b 0x0013124a
  00131248: movs r6,#0x0
  0013124a: ldr r0,[sp,#0x40]
  0013124c: ldr.w r1,[r8,#0x0]
  00131250: subs r0,r1,r0
  00131252: itttt eq
  00131254: mov.eq r0,r6
  00131256: add.eq sp,#0x44
  00131258: pop.eq.w {r8,r9,r10,r11}
  0013125c: pop.eq {r4,r5,r6,r7,pc}
  0013125e: blx 0x0006e824
```
