# Hud::hudEvent
elf 0x1627a0 body 2010
Sig: undefined __stdcall hudEvent(int param_1, PlayerEgo * param_2, int param_3)

## decompile
```c

/* Hud::hudEvent(int, PlayerEgo*, int) */

void Hud::hudEvent(int param_1,PlayerEgo *param_2,int param_3)

{
  undefined1 uVar1;
  undefined2 uVar2;
  Station *this;
  undefined4 *puVar3;
  int iVar4;
  ListItem *this_00;
  undefined1 *puVar5;
  String *pSVar6;
  Vector *pVVar7;
  int iVar8;
  int in_r3;
  code *pcVar9;
  int *piVar10;
  String *pSVar11;
  int *piVar12;
  float fVar13;
  String aSStack_90 [12];
  String aSStack_84 [12];
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar12 = *(int **)(DAT_00172afc + 0x1727b6);
  local_24 = *piVar12;
  switch(param_2) {
  case (PlayerEgo *)0x1:
    if (*(char *)(param_1 + 0x221) == '\0') goto LAB_00172f98;
    piVar10 = *(int **)(DAT_00172b40 + 0x172a38);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172b44 + 0x172a4a),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172e02;
  case (PlayerEgo *)0x2:
    if (*(char *)(param_1 + 0x221) == '\0') goto LAB_00172f98;
    piVar10 = *(int **)(DAT_00172b48 + 0x172a88);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172b4c + 0x172a9a),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172e02;
  case (PlayerEgo *)0x3:
    if ((*(char *)(param_1 + 0x21e) == '\0') ||
       (iVar4 = PlayerEgo::readyToBoost((PlayerEgo *)param_3), iVar4 == 0)) goto LAB_00172f98;
    puVar3 = (undefined4 *)(DAT_00172b50 + 0x172ae6);
    break;
  case (PlayerEgo *)0x4:
    if (*(char *)(param_1 + 0x21e) == '\0') goto LAB_00172f98;
    puVar3 = (undefined4 *)(&UNK_00172afa + DAT_00172b54);
    break;
  case (PlayerEgo *)0x5:
    piVar10 = *(int **)(DAT_00172b04 + 0x172854);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172b08 + 0x172866),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172c6c;
  case (PlayerEgo *)0x6:
    piVar10 = *(int **)(DAT_00172e54 + 0x172b62);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172e58 + 0x172b74),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    pcVar9 = *(code **)(DAT_00172e5c + 0x172ba4);
    (*pcVar9)(aSStack_30);
    (*pcVar9)(aSStack_3c);
    fVar13 = (float)(*pcVar9)(aSStack_48);
    pVVar7 = (Vector *)0x1d;
    puVar3 = (undefined4 *)(DAT_00172e60 + 0x172bbc);
    goto LAB_00172c88;
  case (PlayerEgo *)0x7:
    puVar3 = (undefined4 *)(DAT_00172e64 + 0x172bc6);
    break;
  case (PlayerEgo *)0x8:
    puVar3 = (undefined4 *)(DAT_00172e68 + 0x172bd0);
    break;
  case (PlayerEgo *)0x9:
    puVar3 = (undefined4 *)(DAT_00172e6c + 0x172bda);
    break;
  case (PlayerEgo *)0xa:
    piVar10 = *(int **)(DAT_00172b0c + 0x17289c);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_54,(char *)(DAT_00172b10 + 0x1728ae),false);
    AbyssEngine::operator+(aSStack_48,pSVar11);
    Status::getStation();
    Station::getName();
    AbyssEngine::String::String(aSStack_60,aSStack_6c,false);
    AbyssEngine::operator+(aSStack_3c,aSStack_48);
    this = (Station *)Status::getStation();
    iVar4 = Station::getIndex(this);
    if (iVar4 != 0x65) {
      AbyssEngine::String::String(aSStack_84,(char *)(DAT_001731d8 + 0x17305a),false);
      GameText::getText(*piVar10);
      AbyssEngine::operator+(aSStack_78,aSStack_84);
    }
    else {
      AbyssEngine::String::String(aSStack_78,(char *)(DAT_00172b18 + 0x1728fc),false);
    }
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    AbyssEngine::String::~String(aSStack_30);
    AbyssEngine::String::~String(aSStack_78);
    if (iVar4 != 0x65) {
      AbyssEngine::String::~String(aSStack_84);
    }
    pcVar9 = *(code **)(DAT_001731dc + 0x1730a2);
    (*pcVar9)(aSStack_3c);
    (*pcVar9)(aSStack_60);
    (*pcVar9)(aSStack_6c);
    (*pcVar9)(aSStack_48);
    pSVar6 = aSStack_54;
    goto LAB_00172c7c;
  case (PlayerEgo *)0xb:
    piVar10 = *(int **)(DAT_00172e70 + 0x172be4);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172e74 + 0x172bf6),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172c6c;
  case (PlayerEgo *)0xc:
    piVar10 = *(int **)(DAT_00172b1c + 0x17290e);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172b20 + 0x172920),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172c6c;
  case (PlayerEgo *)0xd:
    piVar10 = *(int **)(DAT_00172b24 + 0x172958);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172b28 + 0x17296a),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172c6c;
  case (PlayerEgo *)0xe:
    piVar10 = *(int **)(DAT_00172e78 + 0x172c2e);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172e7c + 0x172c40),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172c6c;
  case (PlayerEgo *)0xf:
    piVar10 = *(int **)(DAT_00172b2c + 0x1729a2);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172b30 + 0x1729b4),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
LAB_00172c6c:
    pcVar9 = *(code **)(DAT_00172e80 + 0x172c72);
    (*pcVar9)(aSStack_30);
    (*pcVar9)(aSStack_3c);
    pSVar6 = aSStack_48;
LAB_00172c7c:
    fVar13 = (float)(*pcVar9)(pSVar6);
    pVVar7 = (Vector *)0x1c;
    puVar3 = (undefined4 *)(DAT_00172e84 + 0x172c8a);
LAB_00172c88:
    FModSound::play(*(int *)*puVar3,pVVar7,(Vector *)0x0,fVar13);
    goto switchD_001727c6_default;
  case (PlayerEgo *)0x10:
    puVar3 = (undefined4 *)(DAT_00172e88 + 0x172ca0);
    break;
  case (PlayerEgo *)0x11:
    puVar3 = (undefined4 *)(DAT_00172b34 + 0x1729ec);
    break;
  case (PlayerEgo *)0x12:
    puVar3 = (undefined4 *)(DAT_00172e8c + 0x172caa);
    break;
  case (PlayerEgo *)0x13:
    pSVar11 = (String *)(param_1 + 0x100);
    goto LAB_00172eea;
  case (PlayerEgo *)0x14:
    puVar3 = (undefined4 *)(DAT_00172e90 + 0x172cbe);
    break;
  case (PlayerEgo *)0x15:
    puVar3 = (undefined4 *)(DAT_00172e94 + 0x172cc8);
    break;
  case (PlayerEgo *)0x16:
    puVar3 = (undefined4 *)(DAT_00172e98 + 0x172cd2);
    break;
  case (PlayerEgo *)0x17:
    puVar3 = (undefined4 *)(DAT_00172e9c + 0x172cdc);
    break;
  case (PlayerEgo *)0x18:
    puVar3 = (undefined4 *)(DAT_00172ea0 + 0x172ce6);
    break;
  case (PlayerEgo *)0x19:
    *(undefined4 *)(param_1 + 0x464) = 0;
    uVar1 = 1;
    goto LAB_00172cf2;
  case (PlayerEgo *)0x1a:
    uVar1 = 0;
LAB_00172cf2:
    *(undefined1 *)(param_1 + 0x277) = uVar1;
    goto LAB_00172f98;
  case (PlayerEgo *)0x1b:
    puVar3 = (undefined4 *)(DAT_00172ea4 + 0x172d02);
    break;
  case (PlayerEgo *)0x1c:
    *(undefined4 *)(param_1 + 0x464) = 0;
    uVar1 = 1;
    goto LAB_00172d04;
  case (PlayerEgo *)0x1d:
    uVar1 = 0;
LAB_00172d04:
    *(undefined1 *)(param_1 + 0x276) = uVar1;
    goto LAB_00172f98;
  case (PlayerEgo *)0x1e:
    AbyssEngine::String::String(aSStack_60,(char *)(DAT_00172ea8 + 0x172d14),false);
    AbyssEngine::String::String(aSStack_6c,in_r3);
    AbyssEngine::operator+(aSStack_54,aSStack_60);
    AbyssEngine::String::String(aSStack_78,(char *)(DAT_00172eac + 0x172d2e),false);
    AbyssEngine::operator+(aSStack_48,aSStack_54);
    AbyssEngine::String::String(aSStack_3c,aSStack_48,false);
    GameText::getText(**(int **)(DAT_00172eb0 + 0x172d4e));
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_0017302a;
  case (PlayerEgo *)0x1f:
    puVar3 = (undefined4 *)(DAT_00172eb4 + 0x172d78);
    break;
  case (PlayerEgo *)0x20:
    piVar10 = *(int **)(DAT_00172eb8 + 0x172d80);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172ebc + 0x172d92),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
    goto LAB_00172e02;
  case (PlayerEgo *)0x21:
    piVar10 = *(int **)(DAT_00172ec0 + 0x172dc6);
    pSVar11 = (String *)GameText::getText(*piVar10);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_00172ec4 + 0x172dd8),false);
    AbyssEngine::operator+(aSStack_3c,pSVar11);
    GameText::getText(*piVar10);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
LAB_00172e02:
    pcVar9 = *(code **)(DAT_00172ec8 + 0x172e08);
    (*pcVar9)(aSStack_30);
    (*pcVar9)(aSStack_3c);
    (*pcVar9)(aSStack_48);
    goto switchD_001727c6_default;
  case (PlayerEgo *)0x22:
    puVar3 = (undefined4 *)(DAT_00172b38 + 0x172a00);
    break;
  case (PlayerEgo *)0x23:
    *(undefined4 *)(param_1 + 0x468) = 0;
    uVar2 = 1;
    *(undefined1 *)(param_1 + 0x27a) = 1;
    goto LAB_00172e26;
  case (PlayerEgo *)0x24:
  case (PlayerEgo *)0x26:
  case (PlayerEgo *)0x28:
  case (PlayerEgo *)0x2a:
    pSVar11 = (String *)GameText::getText(**(int **)(DAT_00172b00 + 0x172832));
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),pSVar11);
    *(undefined1 *)(param_1 + 0x278) = 0;
    goto switchD_001727c6_default;
  case (PlayerEgo *)0x25:
    *(undefined4 *)(param_1 + 0x468) = 0;
    *(undefined1 *)(param_1 + 0x27a) = 1;
    uVar2 = 0x101;
    goto LAB_00172e26;
  case (PlayerEgo *)0x27:
    *(undefined4 *)(param_1 + 0x468) = 0;
    *(undefined1 *)(param_1 + 0x27a) = 0;
    uVar2 = 1;
LAB_00172e26:
    *(undefined2 *)(param_1 + 0x278) = uVar2;
    goto LAB_00172f98;
  case (PlayerEgo *)0x29:
    *(undefined4 *)(param_1 + 0x468) = 0;
    *(undefined1 *)(param_1 + 0x27a) = 0;
    *(undefined2 *)(param_1 + 0x278) = 0x101;
    goto switchD_001727c6_default;
  case (PlayerEgo *)0x2b:
    puVar3 = (undefined4 *)(DAT_00172ecc + 0x172e4a);
    break;
  case (PlayerEgo *)0x2c:
    puVar3 = (undefined4 *)((int)&DAT_00172e54 + DAT_00172ed0);
    break;
  case (PlayerEgo *)0x2d:
    puVar3 = (undefined4 *)(DAT_00172b3c + 0x172a26);
    break;
  case (PlayerEgo *)0x2e:
    puVar3 = (undefined4 *)(DAT_001731e0 + 0x172ede);
    break;
  case (PlayerEgo *)0x2f:
    AbyssEngine::String::String(aSStack_60,(char *)(DAT_001731e4 + 0x172fd2),false);
    AbyssEngine::String::String(aSStack_6c,in_r3);
    AbyssEngine::operator+(aSStack_54,aSStack_60);
    AbyssEngine::String::String(aSStack_78,(char *)(DAT_001731e8 + 0x172fec),false);
    AbyssEngine::operator+(aSStack_48,aSStack_54);
    AbyssEngine::String::String(aSStack_3c,aSStack_48,false);
    GameText::getText(**(int **)(DAT_001731ec + 0x17300c));
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),aSStack_30);
LAB_0017302a:
    pcVar9 = *(code **)(DAT_001731f0 + 0x173030);
    (*pcVar9)(aSStack_30);
    (*pcVar9)(aSStack_3c);
    (*pcVar9)(aSStack_48);
    (*pcVar9)(aSStack_78);
    (*pcVar9)(aSStack_54);
    (*pcVar9)(aSStack_6c);
    (*pcVar9)(aSStack_60);
    clearQueue((Hud *)param_1);
  default:
    goto switchD_001727c6_default;
  }
  pSVar11 = (String *)GameText::getText(*(int *)*puVar3);
LAB_00172eea:
  AbyssEngine::String::operator=((String *)(param_1 + 0x1e0),pSVar11);
switchD_001727c6_default:
  pSVar11 = (String *)(param_1 + 0x1e0);
  AbyssEngine::String::String(aSStack_90,pSVar11,false);
  iVar4 = sameHudEventAsBefore((Hud *)param_1,aSStack_90);
  AbyssEngine::String::~String(aSStack_90);
  if (iVar4 == 0) {
    this_00 = operator_new(0x48);
    puVar5 = (undefined1 *)((int)&param_2[-1].field_8 + 1);
    if ((puVar5 < (undefined1 *)0x15) && ((1 << ((uint)puVar5 & 0xff) & DAT_001731d4) != 0)) {
      pSVar6 = operator_new(0xc);
      AbyssEngine::String::String(pSVar6,pSVar11,false);
      ::ListItem::ListItem(this_00,pSVar6,1);
    }
    else {
      pSVar6 = operator_new(0xc);
      AbyssEngine::String::String(pSVar6,pSVar11,false);
      ::ListItem::ListItem(this_00,pSVar6);
    }
    addToEventQueue((Hud *)param_1,this_00);
    iVar4 = AbyssEngine::PaintCanvas::GetTextWidth
                      (**(uint **)(DAT_001731f8 + 0x172f5a),
                       (String *)**(undefined4 **)(DAT_001731f4 + 0x172f58));
    iVar8 = **(int **)(DAT_001731fc + 0x172f74);
    *(undefined4 *)(param_1 + 0x1d8) = 0;
    *(undefined1 *)(param_1 + 0x1de) = 1;
    *(bool *)(param_1 + 0x1ec) =
         (iVar8 / 2 - *(int *)(param_1 + 0x4e8)) + *(int *)(param_1 + 0x4f0) * -2 < iVar4;
  }
LAB_00172f98:
  if (*piVar12 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001727a0: push {r4,r5,r6,r7,lr}
  001727a2: add r7,sp,#0xc
  001727a4: push {r8,r9,r10}
  001727a8: sub sp,#0x78
  001727aa: mov r9,r0
  001727ac: ldr r0,[0x00172afc]
  001727ae: mov r5,r3
  001727b0: mov r8,r1
  001727b2: add r0,pc
  001727b4: ldr.w r10,[r0,#0x0]
  001727b8: ldr.w r0,[r10,#0x0]
  001727bc: str r0,[sp,#0x74]
  001727be: subs r0,r1,#0x1
  001727c0: cmp r0,#0x2e
  001727c2: bhi.w 0x00172eee
  001727c6: tbh [pc,r0]
  00172828: ldr r0,[0x00172b00]
  0017282a: mov.w r1,#0xc80
  0017282e: add r0,pc
  00172830: ldr r0,[r0,#0x0]
  00172832: ldr r0,[r0,#0x0]
  00172834: blx 0x00072f70
  00172838: mov r1,r0
  0017283a: add.w r0,r9,#0x1e0
  0017283e: blx 0x0006f2b0
  00172842: movs r0,#0x0
  00172844: strb.w r0,[r9,#0x278]
  00172848: b 0x00172eee
  0017284a: ldr r0,[0x00172b04]
  0017284c: movw r1,#0x23b
  00172850: add r0,pc
  00172852: ldr r6,[r0,#0x0]
  00172854: ldr r0,[r6,#0x0]
  00172856: blx 0x00072f70
  0017285a: ldr r1,[0x00172b08]
  0017285c: add r5,sp,#0x50
  0017285e: mov r4,r0
  00172860: movs r2,#0x0
  00172862: add r1,pc
  00172864: mov r0,r5
  00172866: blx 0x0006ee18
  0017286a: add r0,sp,#0x5c
  0017286c: mov r1,r4
  0017286e: mov r2,r5
  00172870: blx 0x0006ef98
  00172874: ldr r0,[r6,#0x0]
  00172876: movs r1,#0x26
  00172878: blx 0x00072f70
  0017287c: mov r2,r0
  0017287e: add r0,sp,#0x68
  00172880: add r1,sp,#0x5c
  00172882: blx 0x0006ef98
  00172886: add.w r0,r9,#0x1e0
  0017288a: add r1,sp,#0x68
  0017288c: blx 0x0006f2b0
  00172890: b 0x00172c6c
  00172892: ldr r0,[0x00172b0c]
  00172894: movw r1,#0x222
  00172898: add r0,pc
  0017289a: ldr r6,[r0,#0x0]
  0017289c: ldr r0,[r6,#0x0]
  0017289e: blx 0x00072f70
  001728a2: ldr r1,[0x00172b10]
  001728a4: add r5,sp,#0x44
  001728a6: mov r4,r0
  001728a8: movs r2,#0x0
  001728aa: add r1,pc
  001728ac: mov r0,r5
  001728ae: blx 0x0006ee18
  001728b2: add r0,sp,#0x50
  001728b4: mov r1,r4
  001728b6: mov r2,r5
  001728b8: blx 0x0006ef98
  001728bc: ldr r0,[0x00172b14]
  001728be: add r0,pc
  001728c0: ldr r4,[r0,#0x0]
  001728c2: ldr r0,[r4,#0x0]
  001728c4: blx 0x00071c14
  001728c8: mov r1,r0
  001728ca: add r0,sp,#0x2c
  001728cc: blx 0x000736a8
  001728d0: add r0,sp,#0x38
  001728d2: add r1,sp,#0x2c
  001728d4: movs r2,#0x0
  001728d6: blx 0x0006f028
  001728da: add r0,sp,#0x5c
  001728dc: add r1,sp,#0x50
  001728de: add r2,sp,#0x38
  001728e0: blx 0x0006ef98
  001728e4: ldr r0,[r4,#0x0]
  001728e6: blx 0x00071c14
  001728ea: blx 0x00071824
  001728ee: cmp r0,#0x65
  001728f0: bne.w 0x00173054
  001728f4: ldr r1,[0x00172b18]
  001728f6: movs r4,#0x0
  001728f8: add r1,pc
  001728fa: add r0,sp,#0x20
  001728fc: movs r2,#0x0
  001728fe: blx 0x0006ee18
  00172902: b 0x00173074
  00172904: ldr r0,[0x00172b1c]
  00172906: movw r1,#0x222
  0017290a: add r0,pc
  0017290c: ldr r6,[r0,#0x0]
  0017290e: ldr r0,[r6,#0x0]
  00172910: blx 0x00072f70
  00172914: ldr r1,[0x00172b20]
  00172916: add r5,sp,#0x50
  00172918: mov r4,r0
  0017291a: movs r2,#0x0
  0017291c: add r1,pc
  0017291e: mov r0,r5
  00172920: blx 0x0006ee18
  00172924: add r0,sp,#0x5c
  00172926: mov r1,r4
  00172928: mov r2,r5
  0017292a: blx 0x0006ef98
  0017292e: ldr r0,[r6,#0x0]
  00172930: movw r1,#0x223
  00172934: blx 0x00072f70
  00172938: mov r2,r0
  0017293a: add r0,sp,#0x68
  0017293c: add r1,sp,#0x5c
  0017293e: blx 0x0006ef98
  00172942: add.w r0,r9,#0x1e0
  00172946: add r1,sp,#0x68
  00172948: blx 0x0006f2b0
  0017294c: b 0x00172c6c
  0017294e: ldr r0,[0x00172b24]
  00172950: movw r1,#0x222
  00172954: add r0,pc
  00172956: ldr r6,[r0,#0x0]
  00172958: ldr r0,[r6,#0x0]
  0017295a: blx 0x00072f70
  0017295e: ldr r1,[0x00172b28]
  00172960: add r5,sp,#0x50
  00172962: mov r4,r0
  00172964: movs r2,#0x0
  00172966: add r1,pc
  00172968: mov r0,r5
  0017296a: blx 0x0006ee18
  0017296e: add r0,sp,#0x5c
  00172970: mov r1,r4
  00172972: mov r2,r5
  00172974: blx 0x0006ef98
  00172978: ldr r0,[r6,#0x0]
  0017297a: mov.w r1,#0x224
  0017297e: blx 0x00072f70
  00172982: mov r2,r0
  00172984: add r0,sp,#0x68
  00172986: add r1,sp,#0x5c
  00172988: blx 0x0006ef98
  0017298c: add.w r0,r9,#0x1e0
  00172990: add r1,sp,#0x68
  00172992: blx 0x0006f2b0
  00172996: b 0x00172c6c
  00172998: ldr r0,[0x00172b2c]
  0017299a: movw r1,#0x222
  0017299e: add r0,pc
  001729a0: ldr r6,[r0,#0x0]
  001729a2: ldr r0,[r6,#0x0]
  001729a4: blx 0x00072f70
  001729a8: ldr r1,[0x00172b30]
  001729aa: add r5,sp,#0x50
  001729ac: mov r4,r0
  001729ae: movs r2,#0x0
  001729b0: add r1,pc
  001729b2: mov r0,r5
  001729b4: blx 0x0006ee18
  001729b8: add r0,sp,#0x5c
  001729ba: mov r1,r4
  001729bc: mov r2,r5
  001729be: blx 0x0006ef98
  001729c2: ldr r0,[r6,#0x0]
  001729c4: movw r1,#0x221
  001729c8: blx 0x00072f70
  001729cc: mov r2,r0
  001729ce: add r0,sp,#0x68
  001729d0: add r1,sp,#0x5c
  001729d2: blx 0x0006ef98
  001729d6: add.w r0,r9,#0x1e0
  001729da: add r1,sp,#0x68
  001729dc: blx 0x0006f2b0
  001729e0: b 0x00172c6c
  001729e2: ldr r0,[0x00172b34]
  001729e4: mov.w r1,#0x134
  001729e8: add r0,pc
  001729ea: b 0x00172edc
  001729ec: movs r0,#0x0
  001729ee: str.w r0,[r9,#0x464]
  001729f2: movs r0,#0x1
  001729f4: b 0x00172d04
  001729f6: ldr r0,[0x00172b38]
  001729f8: movw r1,#0xc7f
  001729fc: add r0,pc
  001729fe: b 0x00172edc
  00172a00: movs r0,#0x0
  00172a02: str.w r0,[r9,#0x468]
  00172a06: movs r0,#0x1
  00172a08: strb.w r0,[r9,#0x27a]
  00172a0c: b 0x00172e26
  00172a0e: movs r0,#0x0
  00172a10: str.w r0,[r9,#0x468]
  00172a14: strb.w r0,[r9,#0x27a]
  00172a18: movs r0,#0x1
  00172a1a: b 0x00172e26
  00172a1c: ldr r0,[0x00172b3c]
  00172a1e: movw r1,#0xc82
  00172a22: add r0,pc
  00172a24: b 0x00172edc
  00172a26: ldrb.w r0,[r9,#0x221]
  00172a2a: cmp r0,#0x0
  00172a2c: beq.w 0x00172f98
  00172a30: ldr r0,[0x00172b40]
  00172a32: movs r1,#0x25
  00172a34: add r0,pc
  00172a36: ldr r6,[r0,#0x0]
  00172a38: ldr r0,[r6,#0x0]
  00172a3a: blx 0x00072f70
  00172a3e: ldr r1,[0x00172b44]
  00172a40: add r5,sp,#0x50
  00172a42: mov r4,r0
  00172a44: movs r2,#0x0
  00172a46: add r1,pc
  00172a48: mov r0,r5
  00172a4a: blx 0x0006ee18
  00172a4e: add r0,sp,#0x5c
  00172a50: mov r1,r4
  00172a52: mov r2,r5
  00172a54: blx 0x0006ef98
  00172a58: ldr r0,[r6,#0x0]
  00172a5a: movs r1,#0x26
  00172a5c: blx 0x00072f70
  00172a60: mov r2,r0
  00172a62: add r0,sp,#0x68
  00172a64: add r1,sp,#0x5c
  00172a66: blx 0x0006ef98
  00172a6a: add.w r0,r9,#0x1e0
  00172a6e: add r1,sp,#0x68
  00172a70: blx 0x0006f2b0
  00172a74: b 0x00172e02
  00172a76: ldrb.w r0,[r9,#0x221]
  00172a7a: cmp r0,#0x0
  00172a7c: beq.w 0x00172f98
  00172a80: ldr r0,[0x00172b48]
  00172a82: movs r1,#0x25
  00172a84: add r0,pc
  00172a86: ldr r6,[r0,#0x0]
  00172a88: ldr r0,[r6,#0x0]
  00172a8a: blx 0x00072f70
  00172a8e: ldr r1,[0x00172b4c]
  00172a90: add r5,sp,#0x50
  00172a92: mov r4,r0
  00172a94: movs r2,#0x0
  00172a96: add r1,pc
  00172a98: mov r0,r5
  00172a9a: blx 0x0006ee18
  00172a9e: add r0,sp,#0x5c
  00172aa0: mov r1,r4
  00172aa2: mov r2,r5
  00172aa4: blx 0x0006ef98
  00172aa8: ldr r0,[r6,#0x0]
  00172aaa: movs r1,#0x27
  00172aac: blx 0x00072f70
  00172ab0: mov r2,r0
  00172ab2: add r0,sp,#0x68
  00172ab4: add r1,sp,#0x5c
  00172ab6: blx 0x0006ef98
  00172aba: add.w r0,r9,#0x1e0
  00172abe: add r1,sp,#0x68
  00172ac0: blx 0x0006f2b0
  00172ac4: b 0x00172e02
  00172ac6: ldrb.w r0,[r9,#0x21e]
  00172aca: cmp r0,#0x0
  00172acc: beq.w 0x00172f98
  00172ad0: mov r0,r2
  00172ad2: blx 0x00077aac
  00172ad6: cmp r0,#0x0
  00172ad8: beq.w 0x00172f98
  00172adc: ldr r0,[0x00172b50]
  00172ade: mov.w r1,#0x13a
  00172ae2: add r0,pc
  00172ae4: b 0x00172edc
  00172ae6: ldrb.w r0,[r9,#0x21e]
  00172aea: cmp r0,#0x0
  00172aec: beq.w 0x00172f98
  00172af0: ldr r0,[0x00172b54]
  00172af2: movw r1,#0x13b
  00172af6: add r0,pc
  00172af8: b 0x00172edc
  00172b58: ldr r0,[0x00172e54]
  00172b5a: movw r1,#0x23b
  00172b5e: add r0,pc
  00172b60: ldr r6,[r0,#0x0]
  00172b62: ldr r0,[r6,#0x0]
  00172b64: blx 0x00072f70
  00172b68: ldr r1,[0x00172e58]
  00172b6a: add r5,sp,#0x50
  00172b6c: mov r4,r0
  00172b6e: movs r2,#0x0
  00172b70: add r1,pc
  00172b72: mov r0,r5
  00172b74: blx 0x0006ee18
  00172b78: add r0,sp,#0x5c
  00172b7a: mov r1,r4
  00172b7c: mov r2,r5
  00172b7e: blx 0x0006ef98
  00172b82: ldr r0,[r6,#0x0]
  00172b84: movs r1,#0x27
  00172b86: blx 0x00072f70
  00172b8a: mov r2,r0
  00172b8c: add r0,sp,#0x68
  00172b8e: add r1,sp,#0x5c
  00172b90: blx 0x0006ef98
  00172b94: add.w r0,r9,#0x1e0
  00172b98: add r1,sp,#0x68
  00172b9a: blx 0x0006f2b0
  00172b9e: ldr r0,[0x00172e5c]
  00172ba0: add r0,pc
  00172ba2: ldr r4,[r0,#0x0]
  00172ba4: add r0,sp,#0x68
  00172ba6: blx r4
  00172ba8: add r0,sp,#0x5c
  00172baa: blx r4
  00172bac: add r0,sp,#0x50
  00172bae: blx r4
  00172bb0: ldr r0,[0x00172e60]
  00172bb2: movs r1,#0x0
  00172bb4: str r1,[sp,#0x0]
  00172bb6: movs r1,#0x1d
  00172bb8: add r0,pc
  00172bba: b 0x00172c88
  00172bbc: ldr r0,[0x00172e64]
  00172bbe: movw r1,#0x229
  00172bc2: add r0,pc
  00172bc4: b 0x00172edc
  00172bc6: ldr r0,[0x00172e68]
  00172bc8: movw r1,#0x21b
  00172bcc: add r0,pc
  00172bce: b 0x00172edc
  00172bd0: ldr r0,[0x00172e6c]
  00172bd2: mov.w r1,#0x21c
  00172bd6: add r0,pc
  00172bd8: b 0x00172edc
  00172bda: ldr r0,[0x00172e70]
  00172bdc: movw r1,#0x222
  00172be0: add r0,pc
  00172be2: ldr r6,[r0,#0x0]
  00172be4: ldr r0,[r6,#0x0]
  00172be6: blx 0x00072f70
  00172bea: ldr r1,[0x00172e74]
  00172bec: add r5,sp,#0x50
  00172bee: mov r4,r0
  00172bf0: movs r2,#0x0
  00172bf2: add r1,pc
  00172bf4: mov r0,r5
  00172bf6: blx 0x0006ee18
  00172bfa: add r0,sp,#0x5c
  00172bfc: mov r1,r4
  00172bfe: mov r2,r5
  00172c00: blx 0x0006ef98
  00172c04: ldr r0,[r6,#0x0]
  00172c06: movw r1,#0x226
  00172c0a: blx 0x00072f70
  00172c0e: mov r2,r0
  00172c10: add r0,sp,#0x68
  00172c12: add r1,sp,#0x5c
  00172c14: blx 0x0006ef98
  00172c18: add.w r0,r9,#0x1e0
  00172c1c: add r1,sp,#0x68
  00172c1e: blx 0x0006f2b0
  00172c22: b 0x00172c6c
  00172c24: ldr r0,[0x00172e78]
  00172c26: movw r1,#0x222
  00172c2a: add r0,pc
  00172c2c: ldr r6,[r0,#0x0]
  00172c2e: ldr r0,[r6,#0x0]
  00172c30: blx 0x00072f70
  00172c34: ldr r1,[0x00172e7c]
  00172c36: add r5,sp,#0x50
  00172c38: mov r4,r0
  00172c3a: movs r2,#0x0
  00172c3c: add r1,pc
  00172c3e: mov r0,r5
  00172c40: blx 0x0006ee18
  00172c44: add r0,sp,#0x5c
  00172c46: mov r1,r4
  00172c48: mov r2,r5
  00172c4a: blx 0x0006ef98
  00172c4e: ldr r0,[r6,#0x0]
  00172c50: movw r1,#0x225
  00172c54: blx 0x00072f70
  00172c58: mov r2,r0
  00172c5a: add r0,sp,#0x68
  00172c5c: add r1,sp,#0x5c
  00172c5e: blx 0x0006ef98
  00172c62: add.w r0,r9,#0x1e0
  00172c66: add r1,sp,#0x68
  00172c68: blx 0x0006f2b0
  00172c6c: ldr r0,[0x00172e80]
  00172c6e: add r0,pc
  00172c70: ldr r4,[r0,#0x0]
  00172c72: add r0,sp,#0x68
  00172c74: blx r4
  00172c76: add r0,sp,#0x5c
  00172c78: blx r4
  00172c7a: add r0,sp,#0x50
  00172c7c: blx r4
  00172c7e: ldr r0,[0x00172e84]
  00172c80: movs r1,#0x0
  00172c82: str r1,[sp,#0x0]
  00172c84: movs r1,#0x1c
  00172c86: add r0,pc
  00172c88: ldr r0,[r0,#0x0]
  00172c8a: ldr r0,[r0,#0x0]
  00172c8c: movs r2,#0x0
  00172c8e: movs r3,#0x0
  00172c90: blx 0x00071548
  00172c94: b 0x00172eee
  00172c96: ldr r0,[0x00172e88]
  00172c98: movw r1,#0x133
  00172c9c: add r0,pc
  00172c9e: b 0x00172edc
  00172ca0: ldr r0,[0x00172e8c]
  00172ca2: movw r1,#0x135
  00172ca6: add r0,pc
  00172ca8: b 0x00172edc
  00172caa: add.w r0,r9,#0x1e0
  00172cae: add.w r1,r9,#0x100
  00172cb2: b 0x00172eea
  00172cb4: ldr r0,[0x00172e90]
  00172cb6: movw r1,#0x21d
  00172cba: add r0,pc
  00172cbc: b 0x00172edc
  00172cbe: ldr r0,[0x00172e94]
  00172cc0: movw r1,#0x20d
  00172cc4: add r0,pc
  00172cc6: b 0x00172edc
  00172cc8: ldr r0,[0x00172e98]
  00172cca: movw r1,#0x21e
  00172cce: add r0,pc
  00172cd0: b 0x00172edc
  00172cd2: ldr r0,[0x00172e9c]
  00172cd4: movw r1,#0x21f
  00172cd8: add r0,pc
  00172cda: b 0x00172edc
  00172cdc: ldr r0,[0x00172ea0]
  00172cde: mov.w r1,#0x220
  00172ce2: add r0,pc
  00172ce4: b 0x00172edc
  00172ce6: movs r0,#0x0
  00172ce8: str.w r0,[r9,#0x464]
  00172cec: movs r0,#0x1
  00172cee: b 0x00172cf2
  00172cf0: movs r0,#0x0
  00172cf2: strb.w r0,[r9,#0x277]
  00172cf6: b 0x00172f98
  00172cf8: ldr r0,[0x00172ea4]
  00172cfa: mov.w r1,#0x142
  00172cfe: add r0,pc
  00172d00: b 0x00172edc
  00172d02: movs r0,#0x0
  00172d04: strb.w r0,[r9,#0x276]
  00172d08: b 0x00172f98
  00172d0a: ldr r1,[0x00172ea8]
  00172d0c: add r0,sp,#0x38
  00172d0e: movs r2,#0x0
  00172d10: add r1,pc
  00172d12: blx 0x0006ee18
  00172d16: add r0,sp,#0x2c
  00172d18: mov r1,r5
  00172d1a: blx 0x0006f658
  00172d1e: add r0,sp,#0x44
  00172d20: add r1,sp,#0x38
  00172d22: add r2,sp,#0x2c
  00172d24: blx 0x0006ef98
  00172d28: ldr r1,[0x00172eac]
  00172d2a: add r1,pc
  00172d2c: add r0,sp,#0x20
  00172d2e: movs r2,#0x0
  00172d30: blx 0x0006ee18
  00172d34: add r0,sp,#0x50
  00172d36: add r1,sp,#0x44
  00172d38: add r2,sp,#0x20
  00172d3a: blx 0x0006ef98
  00172d3e: add r0,sp,#0x5c
  00172d40: add r1,sp,#0x50
  00172d42: movs r2,#0x0
  00172d44: blx 0x0006f028
  00172d48: ldr r0,[0x00172eb0]
  00172d4a: add r0,pc
  00172d4c: ldr r0,[r0,#0x0]
  00172d4e: ldr r0,[r0,#0x0]
  00172d50: movw r1,#0x574
  00172d54: blx 0x00072f70
  00172d58: mov r2,r0
  00172d5a: add r0,sp,#0x68
  00172d5c: add r1,sp,#0x5c
  00172d5e: blx 0x0006ef98
  00172d62: add.w r0,r9,#0x1e0
  00172d66: add r1,sp,#0x68
  00172d68: blx 0x0006f2b0
  00172d6c: b 0x0017302a
  00172d6e: ldr r0,[0x00172eb4]
  00172d70: mov.w r1,#0x144
  00172d74: add r0,pc
  00172d76: b 0x00172edc
  00172d78: ldr r0,[0x00172eb8]
  00172d7a: movs r1,#0xda
  00172d7c: add r0,pc
  00172d7e: ldr r6,[r0,#0x0]
  00172d80: ldr r0,[r6,#0x0]
  00172d82: blx 0x00072f70
  00172d86: ldr r1,[0x00172ebc]
  00172d88: add r5,sp,#0x50
  00172d8a: mov r4,r0
  00172d8c: movs r2,#0x0
  00172d8e: add r1,pc
  00172d90: mov r0,r5
  00172d92: blx 0x0006ee18
  00172d96: add r0,sp,#0x5c
  00172d98: mov r1,r4
  00172d9a: mov r2,r5
  00172d9c: blx 0x0006ef98
  00172da0: ldr r0,[r6,#0x0]
  00172da2: movs r1,#0x26
  00172da4: blx 0x00072f70
  00172da8: mov r2,r0
  00172daa: add r0,sp,#0x68
  00172dac: add r1,sp,#0x5c
  00172dae: blx 0x0006ef98
  00172db2: add.w r0,r9,#0x1e0
  00172db6: add r1,sp,#0x68
  00172db8: blx 0x0006f2b0
  00172dbc: b 0x00172e02
  00172dbe: ldr r0,[0x00172ec0]
  00172dc0: movs r1,#0xda
  00172dc2: add r0,pc
  00172dc4: ldr r6,[r0,#0x0]
  00172dc6: ldr r0,[r6,#0x0]
  00172dc8: blx 0x00072f70
  00172dcc: ldr r1,[0x00172ec4]
  00172dce: add r5,sp,#0x50
  00172dd0: mov r4,r0
  00172dd2: movs r2,#0x0
  00172dd4: add r1,pc
  00172dd6: mov r0,r5
  00172dd8: blx 0x0006ee18
  00172ddc: add r0,sp,#0x5c
  00172dde: mov r1,r4
  00172de0: mov r2,r5
  00172de2: blx 0x0006ef98
  00172de6: ldr r0,[r6,#0x0]
  00172de8: movs r1,#0x27
  00172dea: blx 0x00072f70
  00172dee: mov r2,r0
  00172df0: add r0,sp,#0x68
  00172df2: add r1,sp,#0x5c
  00172df4: blx 0x0006ef98
  00172df8: add.w r0,r9,#0x1e0
  00172dfc: add r1,sp,#0x68
  00172dfe: blx 0x0006f2b0
  00172e02: ldr r0,[0x00172ec8]
  00172e04: add r0,pc
  00172e06: ldr r4,[r0,#0x0]
  00172e08: add r0,sp,#0x68
  00172e0a: blx r4
  00172e0c: add r0,sp,#0x5c
  00172e0e: blx r4
  00172e10: add r0,sp,#0x50
  00172e12: blx r4
  00172e14: b 0x00172eee
  00172e16: movs r0,#0x0
  00172e18: str.w r0,[r9,#0x468]
  00172e1c: movs r0,#0x1
  00172e1e: strb.w r0,[r9,#0x27a]
  00172e22: movw r0,#0x101
  00172e26: strh.w r0,[r9,#0x278]
  00172e2a: b 0x00172f98
  00172e2c: movs r0,#0x0
  00172e2e: str.w r0,[r9,#0x468]
  00172e32: strb.w r0,[r9,#0x27a]
  00172e36: movw r0,#0x101
  00172e3a: strh.w r0,[r9,#0x278]
  00172e3e: b 0x00172eee
  00172e40: ldr r0,[0x00172ecc]
  00172e42: movw r1,#0xc83
  00172e46: add r0,pc
  00172e48: b 0x00172edc
  00172e4a: ldr r0,[0x00172ed0]
  00172e4c: movw r1,#0xc81
  00172e50: add r0,pc
  00172e52: b 0x00172edc
  00172ed4: ldr r0,[0x001731e0]
  00172ed6: mov.w r1,#0x13c
  00172eda: add r0,pc
  00172edc: ldr r0,[r0,#0x0]
  00172ede: ldr r0,[r0,#0x0]
  00172ee0: blx 0x00072f70
  00172ee4: mov r1,r0
  00172ee6: add.w r0,r9,#0x1e0
  00172eea: blx 0x0006f2b0
  00172eee: add.w r6,r9,#0x1e0
  00172ef2: add r4,sp,#0x8
  00172ef4: movs r2,#0x0
  00172ef6: mov r0,r4
  00172ef8: mov r1,r6
  00172efa: blx 0x0006f028
  00172efe: mov r0,r9
  00172f00: mov r1,r4
  00172f02: blx 0x00077a94
  00172f06: mov r4,r0
  00172f08: add r0,sp,#0x8
  00172f0a: blx 0x0006ee30
  00172f0e: cmp r4,#0x0
  00172f10: bne 0x00172f98
  00172f12: movs r0,#0x48
  00172f14: blx 0x0006eb24
  00172f18: mov r5,r0
  00172f1a: sub.w r0,r8,#0x1b
  00172f1e: cmp r0,#0x14
  00172f20: bhi 0x00172fae
  00172f22: ldr r1,[0x001731d4]
  00172f24: movs r2,#0x1
  00172f26: lsl.w r0,r2,r0
  00172f2a: tst r0,r1
  00172f2c: beq 0x00172fae
  00172f2e: movs r0,#0xc
  00172f30: blx 0x0006eb24
  00172f34: mov r4,r0
  00172f36: mov r1,r6
  00172f38: movs r2,#0x0
  00172f3a: blx 0x0006f028
  00172f3e: mov r0,r5
  00172f40: mov r1,r4
  00172f42: movs r2,#0x1
  00172f44: blx 0x0007618c
  00172f48: mov r0,r9
  00172f4a: mov r1,r5
  00172f4c: blx 0x00077aa0
  00172f50: ldr r0,[0x001731f4]
  00172f52: ldr r1,[0x001731f8]
  00172f54: add r0,pc
  00172f56: add r1,pc
  00172f58: ldr r0,[r0,#0x0]
  00172f5a: ldr r2,[r1,#0x0]
  00172f5c: ldr r1,[r0,#0x0]
  00172f5e: ldr r0,[r2,#0x0]
  00172f60: mov r2,r6
  00172f62: blx 0x0006faa8
  00172f66: ldr r1,[0x001731fc]
  00172f68: movs r2,#0x0
  00172f6a: ldr.w r3,[r9,#0x4e8]
  00172f6e: movs r5,#0x1
  00172f70: add r1,pc
  00172f72: ldr.w r6,[r9,#0x4f0]
  00172f76: ldr r1,[r1,#0x0]
  00172f78: ldr r1,[r1,#0x0]
  00172f7a: str.w r2,[r9,#0x1d8]
  00172f7e: strb.w r5,[r9,#0x1de]
  00172f82: add.w r1,r1,r1, lsr #0x1f
  00172f86: rsb r1,r3,r1, asr #0x1
  00172f8a: sub.w r1,r1,r6, lsl #0x1
  00172f8e: cmp r0,r1
  00172f90: it gt
  00172f92: mov.gt r2,#0x1
  00172f94: strb.w r2,[r9,#0x1ec]
  00172f98: ldr r0,[sp,#0x74]
  00172f9a: ldr.w r1,[r10,#0x0]
  00172f9e: subs r0,r1,r0
  00172fa0: ittt eq
  00172fa2: add.eq sp,#0x78
  00172fa4: pop.eq.w {r8,r9,r10}
  00172fa8: pop.eq {r4,r5,r6,r7,pc}
  00172faa: blx 0x0006e824
  00172fae: movs r0,#0xc
  00172fb0: blx 0x0006eb24
  00172fb4: mov r4,r0
  00172fb6: mov r1,r6
  00172fb8: movs r2,#0x0
  00172fba: blx 0x0006f028
  00172fbe: mov r0,r5
  00172fc0: mov r1,r4
  00172fc2: blx 0x00076174
  00172fc6: b 0x00172f48
  00172fc8: ldr r1,[0x001731e4]
  00172fca: add r0,sp,#0x38
  00172fcc: movs r2,#0x0
  00172fce: add r1,pc
  00172fd0: blx 0x0006ee18
  00172fd4: add r0,sp,#0x2c
  00172fd6: mov r1,r5
  00172fd8: blx 0x0006f658
  00172fdc: add r0,sp,#0x44
  00172fde: add r1,sp,#0x38
  00172fe0: add r2,sp,#0x2c
  00172fe2: blx 0x0006ef98
  00172fe6: ldr r1,[0x001731e8]
  00172fe8: add r1,pc
  00172fea: add r0,sp,#0x20
  00172fec: movs r2,#0x0
  00172fee: blx 0x0006ee18
  00172ff2: add r0,sp,#0x50
  00172ff4: add r1,sp,#0x44
  00172ff6: add r2,sp,#0x20
  00172ff8: blx 0x0006ef98
  00172ffc: add r0,sp,#0x5c
  00172ffe: add r1,sp,#0x50
  00173000: movs r2,#0x0
  00173002: blx 0x0006f028
  00173006: ldr r0,[0x001731ec]
  00173008: add r0,pc
  0017300a: ldr r0,[r0,#0x0]
  0017300c: ldr r0,[r0,#0x0]
  0017300e: movw r1,#0x5c4
  00173012: blx 0x00072f70
  00173016: mov r2,r0
  00173018: add r0,sp,#0x68
  0017301a: add r1,sp,#0x5c
  0017301c: blx 0x0006ef98
  00173020: add.w r0,r9,#0x1e0
  00173024: add r1,sp,#0x68
  00173026: blx 0x0006f2b0
  0017302a: ldr r0,[0x001731f0]
  0017302c: add r0,pc
  0017302e: ldr r4,[r0,#0x0]
  00173030: add r0,sp,#0x68
  00173032: blx r4
  00173034: add r0,sp,#0x5c
  00173036: blx r4
  00173038: add r0,sp,#0x50
  0017303a: blx r4
  0017303c: add r0,sp,#0x20
  0017303e: blx r4
  00173040: add r0,sp,#0x44
  00173042: blx r4
  00173044: add r0,sp,#0x2c
  00173046: blx r4
  00173048: add r0,sp,#0x38
  0017304a: blx r4
  0017304c: mov r0,r9
  0017304e: blx 0x00077ab8
  00173052: b 0x00172eee
  00173054: ldr r1,[0x001731d8]
  00173056: add r1,pc
  00173058: add r0,sp,#0x14
  0017305a: movs r2,#0x0
  0017305c: blx 0x0006ee18
  00173060: ldr r0,[r6,#0x0]
  00173062: movs r1,#0x88
  00173064: blx 0x00072f70
  00173068: mov r2,r0
  0017306a: add r0,sp,#0x20
  0017306c: add r1,sp,#0x14
  0017306e: blx 0x0006ef98
  00173072: movs r4,#0x1
  00173074: add r0,sp,#0x68
  00173076: add r1,sp,#0x5c
  00173078: add r2,sp,#0x20
  0017307a: blx 0x0006ef98
  0017307e: add.w r0,r9,#0x1e0
  00173082: add r1,sp,#0x68
  00173084: blx 0x0006f2b0
  00173088: add r0,sp,#0x68
  0017308a: blx 0x0006ee30
  0017308e: add r0,sp,#0x20
  00173090: blx 0x0006ee30
  00173094: cbz r4,0x0017309c
  00173096: add r0,sp,#0x14
  00173098: blx 0x0006ee30
  0017309c: ldr r0,[0x001731dc]
  0017309e: add r0,pc
  001730a0: ldr r4,[r0,#0x0]
  001730a2: add r0,sp,#0x5c
  001730a4: blx r4
  001730a6: add r0,sp,#0x38
  001730a8: blx r4
  001730aa: add r0,sp,#0x2c
  001730ac: blx r4
  001730ae: add r0,sp,#0x50
  001730b0: blx r4
  001730b2: add r0,sp,#0x44
  001730b4: b 0x00172c7c
```
