# Hud::hudEventMedal
elf 0x1625a0 body 324
Sig: undefined __thiscall hudEventMedal(Hud * this, int param_1, int param_2)

## decompile
```c

/* Hud::hudEventMedal(int, int) */

void __thiscall Hud::hudEventMedal(Hud *this,int param_1,int param_2)

{
  String *pSVar1;
  int iVar2;
  ListItem *this_00;
  String *this_01;
  int iVar3;
  code *pcVar4;
  String *this_02;
  int *piVar5;
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar5 = *(int **)(DAT_00172740 + 0x1725b6);
  local_24 = *piVar5;
  pSVar1 = (String *)GameText::getText(**(int **)(DAT_00172744 + 0x1725bc));
  AbyssEngine::String::String(aSStack_54,(char *)(DAT_00172748 + 0x1725d8),false);
  AbyssEngine::operator+(aSStack_48,pSVar1);
  if (99 < param_2) {
    param_2 = 100;
  }
  AbyssEngine::String::String(aSStack_60,param_2);
  AbyssEngine::operator+(aSStack_3c,aSStack_48);
  AbyssEngine::String::String(aSStack_6c,(char *)(DAT_0017274c + 0x172604),false);
  AbyssEngine::operator+(aSStack_30,aSStack_3c);
  this_02 = (String *)(this + 0x1e0);
  AbyssEngine::String::operator=(this_02,aSStack_30);
  pcVar4 = *(code **)(DAT_00172750 + 0x172626);
  (*pcVar4)(aSStack_30);
  (*pcVar4)(aSStack_6c);
  (*pcVar4)(aSStack_3c);
  (*pcVar4)(aSStack_60);
  (*pcVar4)(aSStack_48);
  (*pcVar4)(aSStack_54);
  AbyssEngine::String::String(aSStack_78,this_02,false);
  iVar2 = sameHudEventAsBefore(this,aSStack_78);
  AbyssEngine::String::~String(aSStack_78);
  if (iVar2 == 0) {
    this_00 = operator_new(0x48);
    this_01 = operator_new(0xc);
    AbyssEngine::String::String(this_01,this_02,false);
    ::ListItem::ListItem(this_00,this_01,3);
    addToEventQueue(this,this_00);
    iVar2 = AbyssEngine::PaintCanvas::GetTextWidth
                      (**(uint **)(DAT_00172758 + 0x172690),
                       (String *)**(undefined4 **)(DAT_00172754 + 0x17268e));
    iVar3 = **(int **)(DAT_0017275c + 0x1726aa);
    *(undefined4 *)(this + 0x1d8) = 0;
    this[0x1de] = (Hud)0x1;
    this[0x1ec] = (Hud)((iVar3 / 2 - *(int *)(this + 0x4e8)) + *(int *)(this + 0x4f0) * -2 < iVar2);
  }
  if (*piVar5 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001725a0: push {r4,r5,r6,r7,lr}
  001725a2: add r7,sp,#0xc
  001725a4: push {r8,r9,r10}
  001725a8: sub sp,#0x58
  001725aa: mov r9,r0
  001725ac: ldr r0,[0x00172740]
  001725ae: mov r5,r2
  001725b0: ldr r2,[0x00172744]
  001725b2: add r0,pc
  001725b4: addw r1,r1,#0x5e3
  001725b8: add r2,pc
  001725ba: ldr.w r10,[r0,#0x0]
  001725be: ldr r2,[r2,#0x0]
  001725c0: ldr.w r0,[r10,#0x0]
  001725c4: str r0,[sp,#0x54]
  001725c6: ldr r0,[r2,#0x0]
  001725c8: blx 0x00072f70
  001725cc: ldr r1,[0x00172748]
  001725ce: add r4,sp,#0x24
  001725d0: mov r6,r0
  001725d2: movs r2,#0x0
  001725d4: add r1,pc
  001725d6: mov r0,r4
  001725d8: blx 0x0006ee18
  001725dc: add r0,sp,#0x30
  001725de: mov r1,r6
  001725e0: mov r2,r4
  001725e2: blx 0x0006ef98
  001725e6: cmp r5,#0x64
  001725e8: it ge
  001725ea: mov.ge r5,#0x64
  001725ec: add r0,sp,#0x18
  001725ee: mov r1,r5
  001725f0: blx 0x0006f658
  001725f4: add r0,sp,#0x3c
  001725f6: add r1,sp,#0x30
  001725f8: add r2,sp,#0x18
  001725fa: blx 0x0006ef98
  001725fe: ldr r1,[0x0017274c]
  00172600: add r1,pc
  00172602: add r0,sp,#0xc
  00172604: movs r2,#0x0
  00172606: blx 0x0006ee18
  0017260a: add r0,sp,#0x48
  0017260c: add r1,sp,#0x3c
  0017260e: add r2,sp,#0xc
  00172610: blx 0x0006ef98
  00172614: add.w r5,r9,#0x1e0
  00172618: add r1,sp,#0x48
  0017261a: mov r0,r5
  0017261c: blx 0x0006f2b0
  00172620: ldr r0,[0x00172750]
  00172622: add r0,pc
  00172624: ldr r4,[r0,#0x0]
  00172626: add r0,sp,#0x48
  00172628: blx r4
  0017262a: add r0,sp,#0xc
  0017262c: blx r4
  0017262e: add r0,sp,#0x3c
  00172630: blx r4
  00172632: add r0,sp,#0x18
  00172634: blx r4
  00172636: add r0,sp,#0x30
  00172638: blx r4
  0017263a: add r0,sp,#0x24
  0017263c: blx r4
  0017263e: mov r4,sp
  00172640: mov r1,r5
  00172642: mov r0,r4
  00172644: movs r2,#0x0
  00172646: blx 0x0006f028
  0017264a: mov r0,r9
  0017264c: mov r1,r4
  0017264e: blx 0x00077a94
  00172652: mov r6,r0
  00172654: mov r0,sp
  00172656: blx 0x0006ee30
  0017265a: cbnz r6,0x001726ce
  0017265c: movs r0,#0x48
  0017265e: blx 0x0006eb24
  00172662: mov r8,r0
  00172664: movs r0,#0xc
  00172666: blx 0x0006eb24
  0017266a: mov r6,r0
  0017266c: mov r1,r5
  0017266e: movs r2,#0x0
  00172670: blx 0x0006f028
  00172674: mov r0,r8
  00172676: mov r1,r6
  00172678: movs r2,#0x3
  0017267a: blx 0x0007618c
  0017267e: mov r0,r9
  00172680: mov r1,r8
  00172682: blx 0x00077aa0
  00172686: ldr r0,[0x00172754]
  00172688: ldr r1,[0x00172758]
  0017268a: add r0,pc
  0017268c: add r1,pc
  0017268e: ldr r0,[r0,#0x0]
  00172690: ldr r2,[r1,#0x0]
  00172692: ldr r1,[r0,#0x0]
  00172694: ldr r0,[r2,#0x0]
  00172696: mov r2,r5
  00172698: blx 0x0006faa8
  0017269c: ldr r1,[0x0017275c]
  0017269e: movs r2,#0x0
  001726a0: ldr.w r3,[r9,#0x4e8]
  001726a4: movs r5,#0x1
  001726a6: add r1,pc
  001726a8: ldr.w r6,[r9,#0x4f0]
  001726ac: ldr r1,[r1,#0x0]
  001726ae: ldr r1,[r1,#0x0]
  001726b0: str.w r2,[r9,#0x1d8]
  001726b4: strb.w r5,[r9,#0x1de]
  001726b8: add.w r1,r1,r1, lsr #0x1f
  001726bc: rsb r1,r3,r1, asr #0x1
  001726c0: sub.w r1,r1,r6, lsl #0x1
  001726c4: cmp r0,r1
  001726c6: it gt
  001726c8: mov.gt r2,#0x1
  001726ca: strb.w r2,[r9,#0x1ec]
  001726ce: ldr r0,[sp,#0x54]
  001726d0: ldr.w r1,[r10,#0x0]
  001726d4: subs r0,r1,r0
  001726d6: ittt eq
  001726d8: add.eq sp,#0x58
  001726da: pop.eq.w {r8,r9,r10}
  001726de: pop.eq {r4,r5,r6,r7,pc}
  001726e0: blx 0x0006e824
```
