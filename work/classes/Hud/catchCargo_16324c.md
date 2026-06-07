# Hud::catchCargo
elf 0x16324c body 882
Sig: undefined __stdcall catchCargo(int param_1, int param_2, bool param_3, bool param_4, bool param_5, bool param_6, bool param_7)

## decompile
```c

/* Hud::catchCargo(int, int, bool, bool, bool, bool, bool) */

void Hud::catchCargo(int param_1,int param_2,bool param_3,bool param_4,bool param_5,bool param_6,
                    bool param_7)

{
  char cVar1;
  String *pSVar2;
  undefined4 uVar3;
  ListItem *pLVar4;
  String *pSVar5;
  int iVar6;
  uint uVar7;
  int *piVar8;
  int iVar9;
  code *pcVar10;
  undefined4 *puVar11;
  String *this;
  int *piVar12;
  undefined4 uVar13;
  undefined3 in_stack_00000001;
  undefined3 in_stack_00000005;
  undefined3 in_stack_00000009;
  int in_stack_0000000c;
  String aSStack_c4 [12];
  String aSStack_b8 [12];
  String aSStack_ac [12];
  String aSStack_a0 [12];
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  uVar7 = (uint)param_3;
  piVar12 = *(int **)(DAT_00173618 + 0x173262);
  local_28 = *piVar12;
  *(undefined4 *)(param_1 + 0x1d0) = 0;
  *(bool *)(param_1 + 0x235) = param_4;
  if (_param_5 == 0) {
    if (param_4) {
      pSVar2 = (String *)GameText::getText(**(int **)(DAT_001736b8 + 0x17338a));
      AbyssEngine::String::operator=((String *)(param_1 + 500),pSVar2);
      pLVar4 = operator_new(0x48);
      pSVar5 = operator_new(0xc);
      AbyssEngine::String::String(pSVar5,(String *)(param_1 + 500),false);
      ::ListItem::ListItem(pLVar4,pSVar5,1);
      iVar9 = *piVar12 - local_28;
      if (iVar9 != 0) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(iVar9,*piVar12);
      }
      (*(code *)(DAT_001ac654 + 0x1ac658))();
      return;
    }
    if (uVar7 == 0) goto LAB_001734a6;
    cVar1 = '\0';
    if (in_stack_0000000c != 0) {
      cVar1 = *(char *)(param_1 + 0x26c);
    }
    if (in_stack_0000000c != 0 && cVar1 != '\0') {
      AbyssEngine::String::String(aSStack_a0,*(int *)(param_1 + 0x52c));
      AbyssEngine::String::String(aSStack_ac,(char *)(DAT_001736bc + 0x1734cc),false);
      AbyssEngine::operator+(aSStack_94,aSStack_a0);
      AbyssEngine::String::String(aSStack_88,aSStack_94,false);
      piVar8 = *(int **)(DAT_001736c0 + 0x1734f0);
      GameText::getText(*piVar8);
      AbyssEngine::operator+(aSStack_34,aSStack_88);
      pcVar10 = *(code **)(DAT_001736c4 + 0x17350c);
      (*pcVar10)(aSStack_88);
      (*pcVar10)(aSStack_94);
      (*pcVar10)(aSStack_ac);
      (*pcVar10)(aSStack_a0);
      AbyssEngine::String::String(aSStack_b8,aSStack_34,false);
      iVar9 = sameHudEventAsBeforeAggregate(param_1,aSStack_b8);
      AbyssEngine::String::~String(aSStack_b8);
      if (-1 < iVar9) {
        *(undefined4 *)(param_1 + 0x268) = 2000;
        iVar6 = *(int *)(param_1 + 0x52c) + uVar7;
        *(int *)(param_1 + 0x52c) = iVar6;
        AbyssEngine::String::String(aSStack_ac,iVar6);
        AbyssEngine::String::String(aSStack_c4,(char *)(DAT_001736c8 + 0x173558),false);
        AbyssEngine::operator+(aSStack_a0,aSStack_ac);
        AbyssEngine::String::String(aSStack_94,aSStack_a0,false);
        GameText::getText(*piVar8);
        AbyssEngine::operator+(aSStack_88,aSStack_94);
        AbyssEngine::String::operator=
                  (*(String **)
                    (*(int *)(*(int *)(*(int *)(param_1 + 0x264) + 4) + iVar9 * 4) + 0x1c),
                   aSStack_88);
        (*pcVar10)(aSStack_88);
        (*pcVar10)(aSStack_94);
        (*pcVar10)(aSStack_a0);
        (*pcVar10)(aSStack_c4);
        (*pcVar10)(aSStack_ac);
        (*pcVar10)(aSStack_34);
        goto LAB_001734a6;
      }
      AbyssEngine::String::~String(aSStack_34);
    }
    *(uint *)(param_1 + 0x52c) = uVar7;
    AbyssEngine::String::String(aSStack_a0,uVar7);
    AbyssEngine::String::String(aSStack_ac,(char *)(DAT_001736cc + 0x173404),false);
    AbyssEngine::operator+(aSStack_94,aSStack_a0);
    AbyssEngine::String::String(aSStack_88,aSStack_94,false);
    GameText::getText(**(int **)(DAT_001736d0 + 0x173424));
    AbyssEngine::operator+(aSStack_34,aSStack_88);
    AbyssEngine::String::operator=((String *)(param_1 + 500),aSStack_34);
    pcVar10 = *(code **)(DAT_001736d4 + 0x17344a);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_88);
    (*pcVar10)(aSStack_94);
    (*pcVar10)(aSStack_ac);
    (*pcVar10)(aSStack_a0);
    pLVar4 = operator_new(0x48);
    pSVar5 = operator_new(0xc);
    AbyssEngine::String::String(pSVar5,(String *)(param_1 + 500),false);
    ::ListItem::ListItem(pLVar4,pSVar5);
    *(int *)(pLVar4 + 0x2c) = param_2;
    if ((_param_7 == 0) || (_param_6 != 0)) {
      *(undefined4 *)(pLVar4 + 0x30) = 2;
    }
    if (_param_7 != 0) {
      pLVar4[0x24] = (ListItem)0x1;
    }
  }
  else {
    piVar8 = *(int **)(DAT_0017361c + 0x17328c);
    pSVar2 = (String *)GameText::getText(*piVar8);
    this = (String *)(param_1 + 500);
    AbyssEngine::String::operator=(this,pSVar2);
    puVar11 = *(undefined4 **)(DAT_00173620 + 0x1732a8);
    uVar13 = *puVar11;
    AbyssEngine::String::String(aSStack_40,this,false);
    iVar9 = *piVar8;
    Status::getMission();
    Mission::getType();
    pSVar2 = (String *)GameText::getText(iVar9);
    AbyssEngine::String::String(aSStack_4c,pSVar2,false);
    uVar3 = AbyssEngine::String::String(aSStack_58,(char *)(DAT_00173624 + 0x1732e0),false);
    Status::replaceHash(aSStack_34,uVar13,aSStack_40,aSStack_4c,uVar3);
    AbyssEngine::String::operator=(this,aSStack_34);
    pcVar10 = *(code **)(DAT_00173628 + 0x173302);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_58);
    (*pcVar10)(aSStack_4c);
    (*pcVar10)(aSStack_40);
    uVar13 = *puVar11;
    AbyssEngine::String::String(aSStack_64,this,false);
    AbyssEngine::String::String(aSStack_70,1);
    uVar3 = AbyssEngine::String::String(aSStack_7c,(char *)(DAT_001736b4 + 0x17332c),false);
    Status::replaceHash(aSStack_34,uVar13,aSStack_64,aSStack_70,uVar3);
    AbyssEngine::String::operator=(this,aSStack_34);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_7c);
    (*pcVar10)(aSStack_70);
    (*pcVar10)(aSStack_64);
    pLVar4 = operator_new(0x48);
    pSVar5 = operator_new(0xc);
    AbyssEngine::String::String(pSVar5,this,false);
    ::ListItem::ListItem(pLVar4,pSVar5);
    *(int *)(pLVar4 + 0x2c) = param_2;
  }
  addToEventQueue((Hud *)param_1,pLVar4);
LAB_001734a6:
  if (*piVar12 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0017324c: push {r4,r5,r6,r7,lr}
  0017324e: add r7,sp,#0xc
  00173250: push {r8,r9,r10,r11}
  00173254: sub sp,#0xb4
  00173256: mov r11,r0
  00173258: ldr r0,[0x00173618]
  0017325a: mov r4,r1
  0017325c: mov r6,r2
  0017325e: add r0,pc
  00173260: ldr.w r8,[r0,#0x0]
  00173264: ldrd r9,r0,[r7,#0x10]
  00173268: ldrd r1,r10,[r7,#0x8]
  0017326c: ldr.w r2,[r8,#0x0]
  00173270: str r2,[sp,#0xb0]
  00173272: movs r2,#0x0
  00173274: cmp r1,#0x0
  00173276: str.w r2,[r11,#0x1d0]
  0017327a: strb.w r3,[r11,#0x235]
  0017327e: beq 0x0017337e
  00173280: ldr r0,[0x0017361c]
  00173282: mov r10,r4
  00173284: movw r1,#0x219
  00173288: add r0,pc
  0017328a: ldr r4,[r0,#0x0]
  0017328c: ldr r0,[r4,#0x0]
  0017328e: blx 0x00072f70
  00173292: add.w r6,r11,#0x1f4
  00173296: mov r1,r0
  00173298: mov r0,r6
  0017329a: blx 0x0006f2b0
  0017329e: ldr r0,[0x00173620]
  001732a0: mov r1,r6
  001732a2: movs r2,#0x0
  001732a4: add r0,pc
  001732a6: ldr r5,[r0,#0x0]
  001732a8: add r0,sp,#0x98
  001732aa: ldr.w r9,[r5,#0x0]
  001732ae: blx 0x0006f028
  001732b2: ldr r4,[r4,#0x0]
  001732b4: ldr r0,[r5,#0x0]
  001732b6: blx 0x00072850
  001732ba: blx 0x00072874
  001732be: movw r1,#0x56f
  001732c2: cmp r0,#0x3
  001732c4: it eq
  001732c6: movw.eq r1,#0x56e
  001732ca: mov r0,r4
  001732cc: blx 0x00072f70
  001732d0: mov r1,r0
  001732d2: add r0,sp,#0x8c
  001732d4: movs r2,#0x0
  001732d6: blx 0x0006f028
  001732da: ldr r1,[0x00173624]
  001732dc: add r1,pc
  001732de: add r0,sp,#0x80
  001732e0: movs r2,#0x0
  001732e2: blx 0x0006ee18
  001732e6: str r0,[sp,#0x0]
  001732e8: add r0,sp,#0xa4
  001732ea: add r2,sp,#0x98
  001732ec: add r3,sp,#0x8c
  001732ee: mov r1,r9
  001732f0: blx 0x000739b4
  001732f4: add r1,sp,#0xa4
  001732f6: mov r0,r6
  001732f8: blx 0x0006f2b0
  001732fc: ldr r0,[0x00173628]
  001732fe: add r0,pc
  00173300: ldr r4,[r0,#0x0]
  00173302: add r0,sp,#0xa4
  00173304: blx r4
  00173306: add r0,sp,#0x80
  00173308: blx r4
  0017330a: add r0,sp,#0x8c
  0017330c: blx r4
  0017330e: add r0,sp,#0x98
  00173310: blx r4
  00173312: add r0,sp,#0x74
  00173314: mov r1,r6
  00173316: movs r2,#0x0
  00173318: ldr r5,[r5,#0x0]
  0017331a: blx 0x0006f028
  0017331e: add r0,sp,#0x68
  00173320: movs r1,#0x1
  00173322: blx 0x0006f658
  00173326: ldr r1,[0x001736b4]
  00173328: add r1,pc
  0017332a: add r0,sp,#0x5c
  0017332c: movs r2,#0x0
  0017332e: blx 0x0006ee18
  00173332: str r0,[sp,#0x0]
  00173334: add r0,sp,#0xa4
  00173336: add r2,sp,#0x74
  00173338: add r3,sp,#0x68
  0017333a: mov r1,r5
  0017333c: blx 0x000739b4
  00173340: add r1,sp,#0xa4
  00173342: mov r0,r6
  00173344: blx 0x0006f2b0
  00173348: add r0,sp,#0xa4
  0017334a: blx r4
  0017334c: add r0,sp,#0x5c
  0017334e: blx r4
  00173350: add r0,sp,#0x68
  00173352: blx r4
  00173354: add r0,sp,#0x74
  00173356: blx r4
  00173358: movs r0,#0x48
  0017335a: blx 0x0006eb24
  0017335e: mov r5,r0
  00173360: movs r0,#0xc
  00173362: blx 0x0006eb24
  00173366: mov r4,r0
  00173368: mov r1,r6
  0017336a: movs r2,#0x0
  0017336c: blx 0x0006f028
  00173370: mov r0,r5
  00173372: mov r1,r4
  00173374: blx 0x00076174
  00173378: str.w r10,[r5,#0x2c]
  0017337c: b 0x0017349e
  0017337e: cbz r3,0x001733e0
  00173380: ldr r0,[0x001736b8]
  00173382: mov.w r1,#0x142
  00173386: add r0,pc
  00173388: ldr r0,[r0,#0x0]
  0017338a: ldr r0,[r0,#0x0]
  0017338c: blx 0x00072f70
  00173390: add.w r6,r11,#0x1f4
  00173394: mov r1,r0
  00173396: mov r0,r6
  00173398: blx 0x0006f2b0
  0017339c: movs r0,#0x48
  0017339e: blx 0x0006eb24
  001733a2: mov r5,r0
  001733a4: movs r0,#0xc
  001733a6: blx 0x0006eb24
  001733aa: mov r4,r0
  001733ac: mov r1,r6
  001733ae: movs r2,#0x0
  001733b0: blx 0x0006f028
  001733b4: mov r0,r5
  001733b6: mov r1,r4
  001733b8: movs r2,#0x1
  001733ba: blx 0x0007618c
  001733be: ldr r0,[sp,#0xb0]
  001733c0: ldr.w r1,[r8,#0x0]
  001733c4: subs r0,r1,r0
  001733c6: itttt eq
  001733c8: mov.eq r0,r11
  001733ca: mov.eq r1,r5
  001733cc: add.eq sp,#0xb4
  001733ce: pop.eq.w {r8,r9,r10,r11}
  001733d2: itt eq
  001733d4: pop.eq.w {r4,r5,r6,r7,lr}
  001733d8: b.eq.w 0x001ac648
  001733dc: blx 0x0006e824
  001733e0: cmp r6,#0x1
  001733e2: blt 0x001734a6
  001733e4: cmp r0,#0x0
  001733e6: str r4,[sp,#0x10]
  001733e8: itt ne
  001733ea: ldrb.ne.w r0,[r11,#0x26c]
  001733ee: cmp.ne r0,#0x0
  001733f0: bne 0x001734bc
  001733f2: add r0,sp,#0x38
  001733f4: mov r1,r6
  001733f6: str.w r6,[r11,#0x52c]
  001733fa: blx 0x0006f658
  001733fe: ldr r1,[0x001736cc]
  00173400: add r1,pc
  00173402: add r0,sp,#0x2c
  00173404: movs r2,#0x0
  00173406: blx 0x0006ee18
  0017340a: add r0,sp,#0x44
  0017340c: add r1,sp,#0x38
  0017340e: add r2,sp,#0x2c
  00173410: blx 0x0006ef98
  00173414: add r0,sp,#0x50
  00173416: add r1,sp,#0x44
  00173418: movs r2,#0x0
  0017341a: blx 0x0006f028
  0017341e: ldr r0,[0x001736d0]
  00173420: add r0,pc
  00173422: ldr r0,[r0,#0x0]
  00173424: ldr r0,[r0,#0x0]
  00173426: addw r1,r4,#0x4fa
  0017342a: blx 0x00072f70
  0017342e: mov r2,r0
  00173430: add r0,sp,#0xa4
  00173432: add r1,sp,#0x50
  00173434: blx 0x0006ef98
  00173438: add.w r6,r11,#0x1f4
  0017343c: add r1,sp,#0xa4
  0017343e: mov r0,r6
  00173440: blx 0x0006f2b0
  00173444: ldr r0,[0x001736d4]
  00173446: add r0,pc
  00173448: ldr r4,[r0,#0x0]
  0017344a: add r0,sp,#0xa4
  0017344c: blx r4
  0017344e: add r0,sp,#0x50
  00173450: blx r4
  00173452: add r0,sp,#0x44
  00173454: blx r4
  00173456: add r0,sp,#0x2c
  00173458: blx r4
  0017345a: add r0,sp,#0x38
  0017345c: blx r4
  0017345e: movs r0,#0x48
  00173460: blx 0x0006eb24
  00173464: mov r5,r0
  00173466: movs r0,#0xc
  00173468: blx 0x0006eb24
  0017346c: mov r4,r0
  0017346e: mov r1,r6
  00173470: movs r2,#0x0
  00173472: blx 0x0006f028
  00173476: mov r0,r5
  00173478: mov r1,r4
  0017347a: blx 0x00076174
  0017347e: ldr r0,[sp,#0x10]
  00173480: cmp.w r9,#0x0
  00173484: str r0,[r5,#0x2c]
  00173486: beq 0x0017348e
  00173488: cmp.w r10,#0x0
  0017348c: beq 0x00173492
  0017348e: movs r0,#0x2
  00173490: str r0,[r5,#0x30]
  00173492: cmp.w r9,#0x0
  00173496: itt ne
  00173498: mov.ne r0,#0x1
  0017349a: strb.ne.w r0,[r5,#0x24]
  0017349e: mov r0,r11
  001734a0: mov r1,r5
  001734a2: blx 0x00077aa0
  001734a6: ldr r0,[sp,#0xb0]
  001734a8: ldr.w r1,[r8,#0x0]
  001734ac: subs r0,r1,r0
  001734ae: ittt eq
  001734b0: add.eq sp,#0xb4
  001734b2: pop.eq.w {r8,r9,r10,r11}
  001734b6: pop.eq {r4,r5,r6,r7,pc}
  001734b8: blx 0x0006e824
  001734bc: ldr.w r1,[r11,#0x52c]
  001734c0: add r0,sp,#0x38
  001734c2: blx 0x0006f658
  001734c6: ldr r1,[0x001736bc]
  001734c8: add r1,pc
  001734ca: add r0,sp,#0x2c
  001734cc: movs r2,#0x0
  001734ce: blx 0x0006ee18
  001734d2: add r0,sp,#0x44
  001734d4: add r1,sp,#0x38
  001734d6: add r2,sp,#0x2c
  001734d8: blx 0x0006ef98
  001734dc: add r0,sp,#0x50
  001734de: add r1,sp,#0x44
  001734e0: movs r2,#0x0
  001734e2: blx 0x0006f028
  001734e6: ldr r0,[0x001736c0]
  001734e8: addw r5,r4,#0x4fa
  001734ec: add r0,pc
  001734ee: ldr r0,[r0,#0x0]
  001734f0: str r0,[sp,#0xc]
  001734f2: ldr r0,[r0,#0x0]
  001734f4: mov r1,r5
  001734f6: blx 0x00072f70
  001734fa: mov r2,r0
  001734fc: add r0,sp,#0xa4
  001734fe: add r1,sp,#0x50
  00173500: blx 0x0006ef98
  00173504: str r5,[sp,#0x8]
  00173506: ldr r0,[0x001736c4]
  00173508: add r0,pc
  0017350a: ldr r4,[r0,#0x0]
  0017350c: add r0,sp,#0x50
  0017350e: blx r4
  00173510: add r0,sp,#0x44
  00173512: blx r4
  00173514: add r0,sp,#0x2c
  00173516: blx r4
  00173518: add r0,sp,#0x38
  0017351a: blx r4
  0017351c: add r0,sp,#0x20
  0017351e: add r1,sp,#0xa4
  00173520: movs r2,#0x0
  00173522: blx 0x0006f028
  00173526: add r1,sp,#0x20
  00173528: mov r0,r11
  0017352a: blx 0x00077ac4
  0017352e: mov r5,r0
  00173530: add r0,sp,#0x20
  00173532: blx 0x0006ee30
  00173536: cmp r5,#0x0
  00173538: blt 0x001735b2
  0017353a: ldr.w r0,[r11,#0x52c]
  0017353e: mov.w r1,#0x7d0
  00173542: str.w r1,[r11,#0x268]
  00173546: adds r1,r0,r6
  00173548: str.w r1,[r11,#0x52c]
  0017354c: add r0,sp,#0x2c
  0017354e: blx 0x0006f658
  00173552: ldr r1,[0x001736c8]
  00173554: add r1,pc
  00173556: add r0,sp,#0x14
  00173558: movs r2,#0x0
  0017355a: blx 0x0006ee18
  0017355e: add r0,sp,#0x38
  00173560: add r1,sp,#0x2c
  00173562: add r2,sp,#0x14
  00173564: blx 0x0006ef98
  00173568: add r0,sp,#0x44
  0017356a: add r1,sp,#0x38
  0017356c: movs r2,#0x0
  0017356e: blx 0x0006f028
  00173572: ldr r0,[sp,#0xc]
  00173574: ldr r0,[r0,#0x0]
  00173576: ldr r1,[sp,#0x8]
  00173578: blx 0x00072f70
  0017357c: mov r2,r0
  0017357e: add r0,sp,#0x50
  00173580: add r1,sp,#0x44
  00173582: blx 0x0006ef98
  00173586: ldr.w r0,[r11,#0x264]
  0017358a: ldr r0,[r0,#0x4]
  0017358c: ldr.w r0,[r0,r5,lsl #0x2]
  00173590: ldr r0,[r0,#0x1c]
  00173592: add r1,sp,#0x50
  00173594: blx 0x0006f2b0
  00173598: add r0,sp,#0x50
  0017359a: blx r4
  0017359c: add r0,sp,#0x44
  0017359e: blx r4
  001735a0: add r0,sp,#0x38
  001735a2: blx r4
  001735a4: add r0,sp,#0x14
  001735a6: blx r4
  001735a8: add r0,sp,#0x2c
  001735aa: blx r4
  001735ac: add r0,sp,#0xa4
  001735ae: blx r4
  001735b0: b 0x001734a6
  001735b2: add r0,sp,#0xa4
  001735b4: blx 0x0006ee30
  001735b8: ldr r4,[sp,#0x10]
  001735ba: b 0x001733f2
  001ac648: bx pc
```
