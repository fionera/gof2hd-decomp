# Hud::updateSecondaryWeaponString
elf 0x161478 body 234
Sig: undefined __thiscall updateSecondaryWeaponString(Hud * this)

## decompile
```c

/* Hud::updateSecondaryWeaponString() */

void __thiscall Hud::updateSecondaryWeaponString(Hud *this)

{
  String *pSVar1;
  int iVar2;
  code *pcVar3;
  int iVar4;
  int *piVar5;
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar5 = *(int **)(DAT_001715a4 + 0x17148a);
  local_1c = *piVar5;
  if (*(Item **)(this + 600) != (Item *)0x0) {
    iVar2 = **(int **)(DAT_001715a8 + 0x1714a0);
    Item::getIndex(*(Item **)(this + 600));
    pSVar1 = (String *)GameText::getText(iVar2);
    AbyssEngine::String::String(aSStack_4c,(char *)(DAT_001715ac + 0x1714bc),false);
    AbyssEngine::operator+(aSStack_40,pSVar1);
    iVar2 = Item::getAmount();
    AbyssEngine::String::String(aSStack_58,iVar2);
    AbyssEngine::operator+(aSStack_34,aSStack_40);
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_001715b0 + 0x1714ea),false);
    AbyssEngine::operator+(aSStack_28,aSStack_34);
    AbyssEngine::String::operator=((String *)(this + 0x3b4),aSStack_28);
    pcVar3 = *(code **)(DAT_001715b4 + 0x17150c);
    (*pcVar3)(aSStack_28);
    (*pcVar3)(aSStack_64);
    (*pcVar3)(aSStack_34);
    (*pcVar3)(aSStack_58);
    (*pcVar3)(aSStack_40);
    (*pcVar3)(aSStack_4c);
    iVar4 = **(int **)(DAT_001715c0 + 0x171532);
    iVar2 = AbyssEngine::PaintCanvas::GetTextWidth
                      (**(uint **)(DAT_001715bc + 0x171530),
                       (String *)**(undefined4 **)(DAT_001715b8 + 0x17152e));
    *(int *)(this + 0x3c0) = (iVar4 >> 1) - (iVar2 >> 1);
  }
  if (*piVar5 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00171478: push {r4,r5,r6,r7,lr}
  0017147a: add r7,sp,#0xc
  0017147c: push.w r8
  00171480: sub sp,#0x50
  00171482: mov r4,r0
  00171484: ldr r0,[0x001715a4]
  00171486: add r0,pc
  00171488: ldr.w r8,[r0,#0x0]
  0017148c: ldr.w r0,[r8,#0x0]
  00171490: str r0,[sp,#0x4c]
  00171492: ldr.w r0,[r4,#0x258]
  00171496: cmp r0,#0x0
  00171498: beq 0x0017154c
  0017149a: ldr r1,[0x001715a8]
  0017149c: add r1,pc
  0017149e: ldr r1,[r1,#0x0]
  001714a0: ldr r5,[r1,#0x0]
  001714a2: blx 0x000718d8
  001714a6: addw r1,r0,#0x4fa
  001714aa: mov r0,r5
  001714ac: blx 0x00072f70
  001714b0: ldr r1,[0x001715ac]
  001714b2: add r6,sp,#0x1c
  001714b4: mov r5,r0
  001714b6: movs r2,#0x0
  001714b8: add r1,pc
  001714ba: mov r0,r6
  001714bc: blx 0x0006ee18
  001714c0: add r0,sp,#0x28
  001714c2: mov r1,r5
  001714c4: mov r2,r6
  001714c6: blx 0x0006ef98
  001714ca: ldr.w r0,[r4,#0x258]
  001714ce: blx 0x0007183c
  001714d2: mov r1,r0
  001714d4: add r0,sp,#0x10
  001714d6: blx 0x0006f658
  001714da: add r0,sp,#0x34
  001714dc: add r1,sp,#0x28
  001714de: add r2,sp,#0x10
  001714e0: blx 0x0006ef98
  001714e4: ldr r1,[0x001715b0]
  001714e6: add r1,pc
  001714e8: add r0,sp,#0x4
  001714ea: movs r2,#0x0
  001714ec: blx 0x0006ee18
  001714f0: add r0,sp,#0x40
  001714f2: add r1,sp,#0x34
  001714f4: add r2,sp,#0x4
  001714f6: blx 0x0006ef98
  001714fa: add.w r5,r4,#0x3b4
  001714fe: add r1,sp,#0x40
  00171500: mov r0,r5
  00171502: blx 0x0006f2b0
  00171506: ldr r0,[0x001715b4]
  00171508: add r0,pc
  0017150a: ldr r6,[r0,#0x0]
  0017150c: add r0,sp,#0x40
  0017150e: blx r6
  00171510: add r0,sp,#0x4
  00171512: blx r6
  00171514: add r0,sp,#0x34
  00171516: blx r6
  00171518: add r0,sp,#0x10
  0017151a: blx r6
  0017151c: add r0,sp,#0x28
  0017151e: blx r6
  00171520: add r0,sp,#0x1c
  00171522: blx r6
  00171524: ldr r0,[0x001715b8]
  00171526: ldr r1,[0x001715bc]
  00171528: ldr r2,[0x001715c0]
  0017152a: add r0,pc
  0017152c: add r1,pc
  0017152e: add r2,pc
  00171530: ldr r0,[r0,#0x0]
  00171532: ldr r3,[r1,#0x0]
  00171534: ldr r1,[r2,#0x0]
  00171536: mov r2,r5
  00171538: ldr r6,[r1,#0x0]
  0017153a: ldr r1,[r0,#0x0]
  0017153c: ldr r0,[r3,#0x0]
  0017153e: blx 0x0006faa8
  00171542: asrs r1,r6,#0x1
  00171544: sub.w r0,r1,r0, asr #0x1
  00171548: str.w r0,[r4,#0x3c0]
  0017154c: ldr r0,[sp,#0x4c]
  0017154e: ldr.w r1,[r8,#0x0]
  00171552: subs r0,r1,r0
  00171554: ittt eq
  00171556: add.eq sp,#0x50
  00171558: pop.eq.w r8
  0017155c: pop.eq {r4,r5,r6,r7,pc}
  0017155e: blx 0x0006e824
```
