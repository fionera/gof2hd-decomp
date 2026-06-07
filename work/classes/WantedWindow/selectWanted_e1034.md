# WantedWindow::selectWanted
elf 0xe1034 body 1568
Sig: undefined __thiscall selectWanted(WantedWindow * this, int param_1)

## decompile
```c

/* WantedWindow::selectWanted(int) */

void __thiscall WantedWindow::selectWanted(WantedWindow *this,int param_1)

{
  int *piVar1;
  void *pvVar2;
  int *piVar3;
  undefined4 uVar4;
  int iVar5;
  Station *this_00;
  Station *this_01;
  Station *this_02;
  String *pSVar6;
  ScrollTouchWindow *pSVar7;
  int iVar8;
  int iVar9;
  code *pcVar10;
  int *piVar11;
  undefined4 *puVar12;
  int iVar13;
  ImageFactory *this_03;
  undefined4 uVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  code *pcVar19;
  int iVar20;
  String aSStack_94 [12];
  String aSStack_88 [12];
  String aSStack_7c [12];
  undefined1 auStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar1 = *(int **)(DAT_000f13b8 + 0xf1048);
  local_28 = *piVar1;
  if (*(Array<ImagePart*> **)(this + 8) != (Array<ImagePart*> *)0x0) {
    pvVar2 = (void *)Array<ImagePart*>::~Array(*(Array<ImagePart*> **)(this + 8));
    operator_delete(pvVar2);
  }
  *(undefined4 *)(this + 8) = 0;
  if (*(ScrollTouchWindow **)(this + 0x2c) != (ScrollTouchWindow *)0x0) {
    pvVar2 = (void *)ScrollTouchWindow::~ScrollTouchWindow(*(ScrollTouchWindow **)(this + 0x2c));
    operator_delete(pvVar2);
  }
  iVar5 = DAT_000f13bc;
  *(undefined4 *)(this + 0x2c) = 0;
  this_03 = (ImageFactory *)**(undefined4 **)(iVar5 + 0xf107e);
  piVar3 = (int *)Wanted::getImageParts
                            (*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
  uVar4 = ImageFactory::loadChar(this_03,piVar3);
  *(undefined4 *)(this + 8) = uVar4;
  Wanted::getName();
  AbyssEngine::String::operator=((String *)(this + 0x54),aSStack_34);
  AbyssEngine::String::~String(aSStack_34);
  iVar5 = Wanted::isActive(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
  if (iVar5 == 0) {
    iVar5 = Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
    if (iVar5 == 0) {
      piVar3 = *(int **)(DAT_000f1774 + 0xf140a);
      pSVar6 = (String *)GameText::getText(*piVar3);
      AbyssEngine::String::operator=((String *)(this + 0x3c),pSVar6);
      pSVar6 = (String *)GameText::getText(*piVar3);
      AbyssEngine::String::operator=((String *)(this + 0x48),pSVar6);
      iVar5 = Wanted::getReward();
      AbyssEngine::String::String(aSStack_40,iVar5);
      AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f1778 + 0xf144a),false);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::String::operator=((String *)(this + 0x78),aSStack_34);
      pcVar10 = *(code **)(DAT_000f177c + 0xf146a);
      (*pcVar10)(aSStack_34);
      (*pcVar10)(aSStack_4c);
      (*pcVar10)(aSStack_40);
      pSVar6 = (String *)GameText::getText(*piVar3);
      AbyssEngine::String::operator=((String *)(this + 0x6c),pSVar6);
    }
    else {
      AbyssEngine::String::String(aSStack_34,(char *)(DAT_000f13f0 + 0xf1348),false);
      AbyssEngine::String::operator=((String *)(this + 0x3c),aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::String(aSStack_34,(char *)(DAT_000f13f4 + 0xf1366),false);
      AbyssEngine::String::operator=((String *)(this + 0x48),aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::String(aSStack_34,(char *)(DAT_000f13f8 + 0xf1384),false);
      AbyssEngine::String::operator=((String *)(this + 0x6c),aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
      AbyssEngine::String::String(aSStack_34,(char *)(DAT_000f13fc + 0xf13a2),false);
      AbyssEngine::String::operator=((String *)(this + 0x78),aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
    }
  }
  else {
    puVar12 = *(undefined4 **)(DAT_000f13c0 + 0xf10da);
    uVar14 = *puVar12;
    uVar4 = Wanted::getLastSeen(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
    pcVar10 = *(code **)(DAT_000f13c4 + 0xf10ee);
    this_00 = (Station *)(*pcVar10)(uVar14,uVar4);
    uVar14 = *puVar12;
    uVar4 = Wanted::getTravelsTo(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
    this_01 = (Station *)(*pcVar10)(uVar14,uVar4);
    uVar14 = *puVar12;
    uVar4 = Wanted::getCurrentLocation
                      (*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
    this_02 = (Station *)(*pcVar10)(uVar14,uVar4);
    uVar14 = *puVar12;
    pcVar10 = *(code **)(DAT_000f13c8 + 0xf112e);
    uVar4 = (*pcVar10)(this_00);
    pcVar19 = *(code **)(DAT_000f13cc + 0xf113a);
    (*pcVar19)(uVar14,uVar4);
    uVar14 = *puVar12;
    uVar4 = (*pcVar10)(this_01);
    (*pcVar19)(uVar14,uVar4);
    uVar14 = *puVar12;
    uVar4 = (*pcVar10)(this_02);
    (*pcVar19)(uVar14,uVar4);
    Station::getName();
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f13d0 + 0xf1178),false);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    SolarSystem::getName();
    AbyssEngine::operator+(aSStack_40,aSStack_4c);
    AbyssEngine::String::String(aSStack_7c,(char *)(DAT_000f13d4 + 0xf11a8),false);
    AbyssEngine::operator+(aSStack_34,aSStack_40);
    AbyssEngine::String::operator=((String *)(this + 0x3c),aSStack_34);
    pcVar10 = *(code **)(DAT_000f13d8 + 0xf11c8);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_7c);
    (*pcVar10)(aSStack_40);
    (*pcVar10)(auStack_70);
    (*pcVar10)(aSStack_4c);
    (*pcVar10)(aSStack_64);
    (*pcVar10)(aSStack_58);
    Station::getName();
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f13dc + 0xf11f2),false);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    SolarSystem::getName();
    AbyssEngine::operator+(aSStack_40,aSStack_4c);
    AbyssEngine::String::String(aSStack_7c,(char *)(DAT_000f13e0 + 0xf121a),false);
    AbyssEngine::operator+(aSStack_34,aSStack_40);
    AbyssEngine::String::operator=((String *)(this + 0x48),aSStack_34);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_7c);
    (*pcVar10)(aSStack_40);
    (*pcVar10)(auStack_70);
    (*pcVar10)(aSStack_4c);
    (*pcVar10)(aSStack_64);
    (*pcVar10)(aSStack_58);
    Station::getName();
    AbyssEngine::String::String(aSStack_64,(char *)(DAT_000f13e4 + 0xf125e),false);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    SolarSystem::getName();
    AbyssEngine::operator+(aSStack_40,aSStack_4c);
    AbyssEngine::String::String(aSStack_7c,(char *)(DAT_000f13e8 + 0xf1286),false);
    AbyssEngine::operator+(aSStack_34,aSStack_40);
    AbyssEngine::String::operator=((String *)(this + 0x6c),aSStack_34);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_7c);
    (*pcVar10)(aSStack_40);
    (*pcVar10)(auStack_70);
    (*pcVar10)(aSStack_4c);
    (*pcVar10)(aSStack_64);
    (*pcVar10)(aSStack_58);
    iVar5 = Wanted::getReward();
    AbyssEngine::String::String(aSStack_40,iVar5);
    AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f13ec + 0xf12d8),false);
    AbyssEngine::operator+(aSStack_34,aSStack_40);
    AbyssEngine::String::operator=((String *)(this + 0x78),aSStack_34);
    (*pcVar10)(aSStack_34);
    (*pcVar10)(aSStack_4c);
    (*pcVar10)(aSStack_40);
    if (this_00 != (Station *)0x0) {
      pvVar2 = (void *)Station::~Station(this_00);
      operator_delete(pvVar2);
    }
    if (this_01 != (Station *)0x0) {
      pvVar2 = (void *)Station::~Station(this_01);
      operator_delete(pvVar2);
    }
    if (this_02 != (Station *)0x0) {
      pvVar2 = (void *)Station::~Station(this_02);
      operator_delete(pvVar2);
    }
  }
  Wanted::isTerminated(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
  piVar3 = *(int **)(DAT_000f1780 + 0xf14a0);
  pSVar6 = (String *)GameText::getText(*piVar3);
  AbyssEngine::String::operator=((String *)(this + 0x60),pSVar6);
  iVar8 = *(int *)(this + 0x20);
  iVar9 = *(int *)(this + 0x28);
  piVar11 = *(int **)(DAT_000f1784 + 0xf14c8);
  iVar5 = *piVar11;
  iVar15 = *(int *)(iVar5 + 0x2c);
  iVar20 = *(int *)(iVar5 + 0x10);
  iVar16 = *(int *)(iVar5 + 0x24);
  iVar17 = *(int *)(iVar5 + 0x2d8);
  iVar18 = iVar8 + *(int *)(iVar5 + 0xc) + *(int *)(iVar5 + 0x20) + *(int *)(iVar5 + 0x5c) + iVar15;
  iVar5 = Wanted::isActive(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
  iVar13 = *piVar11;
  iVar16 = (((((iVar8 - iVar15) + iVar9) - iVar18) - iVar20) - iVar17) - iVar16;
  if (iVar5 != 0) {
    iVar16 = (iVar16 - *(int *)(iVar13 + 0x4c)) - *(int *)(iVar13 + 0x30);
  }
  pSVar7 = operator_new(0x24);
  iVar5 = *(int *)(iVar13 + 0x2c);
  ScrollTouchWindow::ScrollTouchWindow
            (pSVar7,*(int *)(this + 0x1c) + (*(int *)(this + 0x24) >> 1) + iVar5,
             *(int *)(iVar13 + 0x2d8) + iVar5 + iVar18,
             ((*(int *)(this + 0x24) >> 1) - iVar5) - *(int *)(iVar13 + 0x28),iVar16,false);
  iVar5 = DAT_000f1788;
  *(ScrollTouchWindow **)(this + 0x2c) = pSVar7;
  AbyssEngine::String::String(aSStack_34,(char *)(iVar5 + 0xf1568),false);
  pSVar6 = (String *)GameText::getText(*piVar3);
  AbyssEngine::String::operator=(aSStack_34,pSVar6);
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f178c + 0xf1584),false);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_4c);
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f1790 + 0xf15b0),false);
  GameText::getText(*piVar3);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_4c);
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f1794 + 0xf15e4),false);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_4c);
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000f1798 + 0xf1610),false);
  iVar5 = *piVar3;
  Wanted::getIndex(*(Wanted **)(*(int *)(*(int *)(this + 0x38) + 4) + param_1 * 4));
  GameText::getText(iVar5);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_4c);
  pSVar7 = *(ScrollTouchWindow **)(this + 0x2c);
  AbyssEngine::String::String(aSStack_88,(char *)(DAT_000f179c + 0xf165a),false);
  AbyssEngine::String::String(aSStack_94,aSStack_34,false);
  ScrollTouchWindow::setText(pSVar7,aSStack_88,aSStack_94);
  pcVar10 = *(code **)(DAT_000f17a0 + 0xf167a);
  (*pcVar10)(aSStack_94);
  (*pcVar10)(aSStack_88);
  (*pcVar10)(aSStack_34);
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000f1034: push {r4,r5,r6,r7,lr}
  000f1036: add r7,sp,#0xc
  000f1038: push {r8,r9,r10,r11}
  000f103c: sub sp,#0x94
  000f103e: mov r9,r0
  000f1040: ldr r0,[0x000f13b8]
  000f1042: mov r10,r1
  000f1044: add r0,pc
  000f1046: ldr r0,[r0,#0x0]
  000f1048: str r0,[sp,#0x14]
  000f104a: ldr r0,[r0,#0x0]
  000f104c: str r0,[sp,#0x90]
  000f104e: ldr.w r0,[r9,#0x8]
  000f1052: cbz r0,0x000f105c
  000f1054: blx 0x00075af0
  000f1058: blx 0x0006eb48
  000f105c: ldr.w r0,[r9,#0x2c]
  000f1060: movs r4,#0x0
  000f1062: str.w r4,[r9,#0x8]
  000f1066: cbz r0,0x000f1070
  000f1068: blx 0x00075afc
  000f106c: blx 0x0006eb48
  000f1070: ldr r0,[0x000f13bc]
  000f1072: ldr.w r1,[r9,#0x38]
  000f1076: str.w r4,[r9,#0x2c]
  000f107a: add r0,pc
  000f107c: ldr r1,[r1,#0x4]
  000f107e: ldr r2,[r0,#0x0]
  000f1080: ldr.w r0,[r1,r10,lsl #0x2]
  000f1084: ldr r6,[r2,#0x0]
  000f1086: blx 0x00074c50
  000f108a: mov r1,r0
  000f108c: mov r0,r6
  000f108e: blx 0x00075b14
  000f1092: str.w r0,[r9,#0x8]
  000f1096: add r6,sp,#0x84
  000f1098: ldr.w r0,[r9,#0x38]
  000f109c: ldr r0,[r0,#0x4]
  000f109e: ldr.w r1,[r0,r10,lsl #0x2]
  000f10a2: mov r0,r6
  000f10a4: blx 0x0007405c
  000f10a8: add.w r0,r9,#0x54
  000f10ac: mov r1,r6
  000f10ae: blx 0x0006f2b0
  000f10b2: add r0,sp,#0x84
  000f10b4: blx 0x0006ee30
  000f10b8: ldr.w r0,[r9,#0x38]
  000f10bc: ldr r0,[r0,#0x4]
  000f10be: ldr.w r0,[r0,r10,lsl #0x2]
  000f10c2: blx 0x00073774
  000f10c6: cmp r0,#0x0
  000f10c8: str.w r10,[sp,#0x20]
  000f10cc: beq.w 0x000f132c
  000f10d0: ldr r0,[0x000f13c0]
  000f10d2: ldr.w r1,[r9,#0x38]
  000f10d6: add r0,pc
  000f10d8: ldr r1,[r1,#0x4]
  000f10da: ldr r5,[r0,#0x0]
  000f10dc: ldr.w r0,[r1,r10,lsl #0x2]
  000f10e0: ldr r6,[r5,#0x0]
  000f10e2: blx 0x00074c2c
  000f10e6: mov r1,r0
  000f10e8: ldr r0,[0x000f13c4]
  000f10ea: add r0,pc
  000f10ec: ldr r4,[r0,#0x0]
  000f10ee: mov r0,r6
  000f10f0: blx r4
  000f10f2: mov r11,r0
  000f10f4: str r0,[sp,#0x1c]
  000f10f6: ldr.w r0,[r9,#0x38]
  000f10fa: ldr r6,[r5,#0x0]
  000f10fc: ldr r0,[r0,#0x4]
  000f10fe: ldr.w r0,[r0,r10,lsl #0x2]
  000f1102: blx 0x000737b0
  000f1106: mov r1,r0
  000f1108: mov r0,r6
  000f110a: blx r4
  000f110c: mov r8,r0
  000f110e: ldr.w r0,[r9,#0x38]
  000f1112: ldr r6,[r5,#0x0]
  000f1114: ldr r0,[r0,#0x4]
  000f1116: ldr.w r0,[r0,r10,lsl #0x2]
  000f111a: blx 0x00073780
  000f111e: mov r1,r0
  000f1120: mov r0,r6
  000f1122: blx r4
  000f1124: mov r10,r0
  000f1126: ldr r0,[0x000f13c8]
  000f1128: ldr r6,[r5,#0x0]
  000f112a: add r0,pc
  000f112c: ldr r4,[r0,#0x0]
  000f112e: mov r0,r11
  000f1130: blx r4
  000f1132: mov r1,r0
  000f1134: ldr r0,[0x000f13cc]
  000f1136: add r0,pc
  000f1138: ldr.w r11,[r0,#0x0]
  000f113c: mov r0,r6
  000f113e: blx r11
  000f1140: str r0,[sp,#0x10]
  000f1142: mov r0,r8
  000f1144: ldr r6,[r5,#0x0]
  000f1146: str.w r8,[sp,#0x18]
  000f114a: blx r4
  000f114c: mov r1,r0
  000f114e: mov r0,r6
  000f1150: blx r11
  000f1152: str r0,[sp,#0xc]
  000f1154: mov r0,r10
  000f1156: ldr.w r8,[r5,#0x0]
  000f115a: mov r6,r10
  000f115c: blx r4
  000f115e: mov r1,r0
  000f1160: mov r0,r8
  000f1162: ldr r4,[sp,#0x1c]
  000f1164: blx r11
  000f1166: mov r8,r0
  000f1168: add r0,sp,#0x60
  000f116a: mov r1,r4
  000f116c: mov r11,r4
  000f116e: blx 0x000736a8
  000f1172: ldr r1,[0x000f13d0]
  000f1174: add r1,pc
  000f1176: add r0,sp,#0x54
  000f1178: movs r2,#0x0
  000f117a: blx 0x0006ee18
  000f117e: add r0,sp,#0x6c
  000f1180: add r1,sp,#0x60
  000f1182: add r2,sp,#0x54
  000f1184: blx 0x0006ef98
  000f1188: ldr r1,[sp,#0x10]
  000f118a: add r0,sp,#0x48
  000f118c: blx 0x00071cc8
  000f1190: ldr.w r10,[sp,#0x20]
  000f1194: ldr r5,[sp,#0x18]
  000f1196: mov r4,r6
  000f1198: add r0,sp,#0x78
  000f119a: add r1,sp,#0x6c
  000f119c: add r2,sp,#0x48
  000f119e: blx 0x0006ef98
  000f11a2: ldr r1,[0x000f13d4]
  000f11a4: add r1,pc
  000f11a6: add r0,sp,#0x3c
  000f11a8: movs r2,#0x0
  000f11aa: blx 0x0006ee18
  000f11ae: add r0,sp,#0x84
  000f11b0: add r1,sp,#0x78
  000f11b2: add r2,sp,#0x3c
  000f11b4: blx 0x0006ef98
  000f11b8: add.w r0,r9,#0x3c
  000f11bc: add r1,sp,#0x84
  000f11be: blx 0x0006f2b0
  000f11c2: ldr r0,[0x000f13d8]
  000f11c4: add r0,pc
  000f11c6: ldr r6,[r0,#0x0]
  000f11c8: add r0,sp,#0x84
  000f11ca: blx r6
  000f11cc: add r0,sp,#0x3c
  000f11ce: blx r6
  000f11d0: add r0,sp,#0x78
  000f11d2: blx r6
  000f11d4: add r0,sp,#0x48
  000f11d6: blx r6
  000f11d8: add r0,sp,#0x6c
  000f11da: blx r6
  000f11dc: add r0,sp,#0x54
  000f11de: blx r6
  000f11e0: add r0,sp,#0x60
  000f11e2: blx r6
  000f11e4: add r0,sp,#0x60
  000f11e6: mov r1,r5
  000f11e8: blx 0x000736a8
  000f11ec: ldr r1,[0x000f13dc]
  000f11ee: add r1,pc
  000f11f0: add r0,sp,#0x54
  000f11f2: movs r2,#0x0
  000f11f4: blx 0x0006ee18
  000f11f8: add r0,sp,#0x6c
  000f11fa: add r1,sp,#0x60
  000f11fc: add r2,sp,#0x54
  000f11fe: blx 0x0006ef98
  000f1202: ldr r1,[sp,#0xc]
  000f1204: add r0,sp,#0x48
  000f1206: blx 0x00071cc8
  000f120a: add r0,sp,#0x78
  000f120c: add r1,sp,#0x6c
  000f120e: add r2,sp,#0x48
  000f1210: blx 0x0006ef98
  000f1214: ldr r1,[0x000f13e0]
  000f1216: add r1,pc
  000f1218: add r0,sp,#0x3c
  000f121a: movs r2,#0x0
  000f121c: blx 0x0006ee18
  000f1220: add r0,sp,#0x84
  000f1222: add r1,sp,#0x78
  000f1224: add r2,sp,#0x3c
  000f1226: blx 0x0006ef98
  000f122a: add.w r0,r9,#0x48
  000f122e: add r1,sp,#0x84
  000f1230: blx 0x0006f2b0
  000f1234: add r0,sp,#0x84
  000f1236: blx r6
  000f1238: add r0,sp,#0x3c
  000f123a: blx r6
  000f123c: add r0,sp,#0x78
  000f123e: blx r6
  000f1240: add r0,sp,#0x48
  000f1242: blx r6
  000f1244: add r0,sp,#0x6c
  000f1246: blx r6
  000f1248: add r0,sp,#0x54
  000f124a: blx r6
  000f124c: add r0,sp,#0x60
  000f124e: blx r6
  000f1250: add r0,sp,#0x60
  000f1252: mov r1,r4
  000f1254: blx 0x000736a8
  000f1258: ldr r1,[0x000f13e4]
  000f125a: add r1,pc
  000f125c: add r0,sp,#0x54
  000f125e: movs r2,#0x0
  000f1260: blx 0x0006ee18
  000f1264: add r0,sp,#0x6c
  000f1266: add r1,sp,#0x60
  000f1268: add r2,sp,#0x54
  000f126a: blx 0x0006ef98
  000f126e: add r0,sp,#0x48
  000f1270: mov r1,r8
  000f1272: blx 0x00071cc8
  000f1276: add r0,sp,#0x78
  000f1278: add r1,sp,#0x6c
  000f127a: add r2,sp,#0x48
  000f127c: blx 0x0006ef98
  000f1280: ldr r1,[0x000f13e8]
  000f1282: add r1,pc
  000f1284: add r0,sp,#0x3c
  000f1286: movs r2,#0x0
  000f1288: blx 0x0006ee18
  000f128c: add r0,sp,#0x84
  000f128e: add r1,sp,#0x78
  000f1290: add r2,sp,#0x3c
  000f1292: blx 0x0006ef98
  000f1296: add.w r0,r9,#0x6c
  000f129a: add r1,sp,#0x84
  000f129c: blx 0x0006f2b0
  000f12a0: add r0,sp,#0x84
  000f12a2: blx r6
  000f12a4: add r0,sp,#0x3c
  000f12a6: blx r6
  000f12a8: add r0,sp,#0x78
  000f12aa: blx r6
  000f12ac: add r0,sp,#0x48
  000f12ae: blx r6
  000f12b0: add r0,sp,#0x6c
  000f12b2: blx r6
  000f12b4: add r0,sp,#0x54
  000f12b6: blx r6
  000f12b8: add r0,sp,#0x60
  000f12ba: blx r6
  000f12bc: ldr.w r0,[r9,#0x38]
  000f12c0: ldr r0,[r0,#0x4]
  000f12c2: ldr.w r0,[r0,r10,lsl #0x2]
  000f12c6: blx 0x00074c44
  000f12ca: mov r1,r0
  000f12cc: add r0,sp,#0x78
  000f12ce: blx 0x0006f658
  000f12d2: ldr r1,[0x000f13ec]
  000f12d4: add r1,pc
  000f12d6: add r0,sp,#0x6c
  000f12d8: movs r2,#0x0
  000f12da: blx 0x0006ee18
  000f12de: add r0,sp,#0x84
  000f12e0: add r1,sp,#0x78
  000f12e2: add r2,sp,#0x6c
  000f12e4: blx 0x0006ef98
  000f12e8: add.w r0,r9,#0x78
  000f12ec: add r1,sp,#0x84
  000f12ee: blx 0x0006f2b0
  000f12f2: add r0,sp,#0x84
  000f12f4: blx r6
  000f12f6: add r0,sp,#0x6c
  000f12f8: blx r6
  000f12fa: add r0,sp,#0x78
  000f12fc: blx r6
  000f12fe: cmp.w r11,#0x0
  000f1302: beq 0x000f130e
  000f1304: mov r0,r11
  000f1306: blx 0x0007360c
  000f130a: blx 0x0006eb48
  000f130e: cbz r5,0x000f131a
  000f1310: mov r0,r5
  000f1312: blx 0x0007360c
  000f1316: blx 0x0006eb48
  000f131a: cmp r4,#0x0
  000f131c: beq.w 0x000f148a
  000f1320: mov r0,r4
  000f1322: blx 0x0007360c
  000f1326: blx 0x0006eb48
  000f132a: b 0x000f148a
  000f132c: ldr.w r0,[r9,#0x38]
  000f1330: ldr r0,[r0,#0x4]
  000f1332: ldr.w r0,[r0,r10,lsl #0x2]
  000f1336: blx 0x00071a04
  000f133a: cmp r0,#0x0
  000f133c: beq 0x000f1400
  000f133e: ldr r1,[0x000f13f0]
  000f1340: add r6,sp,#0x84
  000f1342: movs r2,#0x0
  000f1344: add r1,pc
  000f1346: mov r0,r6
  000f1348: blx 0x0006ee18
  000f134c: add.w r0,r9,#0x3c
  000f1350: mov r1,r6
  000f1352: blx 0x0006f2b0
  000f1356: add r0,sp,#0x84
  000f1358: blx 0x0006ee30
  000f135c: ldr r1,[0x000f13f4]
  000f135e: add r6,sp,#0x84
  000f1360: movs r2,#0x0
  000f1362: add r1,pc
  000f1364: mov r0,r6
  000f1366: blx 0x0006ee18
  000f136a: add.w r0,r9,#0x48
  000f136e: mov r1,r6
  000f1370: blx 0x0006f2b0
  000f1374: add r0,sp,#0x84
  000f1376: blx 0x0006ee30
  000f137a: ldr r1,[0x000f13f8]
  000f137c: add r6,sp,#0x84
  000f137e: movs r2,#0x0
  000f1380: add r1,pc
  000f1382: mov r0,r6
  000f1384: blx 0x0006ee18
  000f1388: add.w r0,r9,#0x6c
  000f138c: mov r1,r6
  000f138e: blx 0x0006f2b0
  000f1392: add r0,sp,#0x84
  000f1394: blx 0x0006ee30
  000f1398: ldr r1,[0x000f13fc]
  000f139a: add r6,sp,#0x84
  000f139c: movs r2,#0x0
  000f139e: add r1,pc
  000f13a0: mov r0,r6
  000f13a2: blx 0x0006ee18
  000f13a6: add.w r0,r9,#0x78
  000f13aa: mov r1,r6
  000f13ac: blx 0x0006f2b0
  000f13b0: add r0,sp,#0x84
  000f13b2: blx 0x0006ee30
  000f13b6: b 0x000f148a
  000f1400: ldr r0,[0x000f1774]
  000f1402: movw r1,#0xc9d
  000f1406: add r0,pc
  000f1408: ldr r5,[r0,#0x0]
  000f140a: ldr r0,[r5,#0x0]
  000f140c: blx 0x00072f70
  000f1410: mov r1,r0
  000f1412: add.w r0,r9,#0x3c
  000f1416: blx 0x0006f2b0
  000f141a: ldr r0,[r5,#0x0]
  000f141c: movw r1,#0xc9d
  000f1420: blx 0x00072f70
  000f1424: mov r1,r0
  000f1426: add.w r0,r9,#0x48
  000f142a: blx 0x0006f2b0
  000f142e: ldr.w r0,[r9,#0x38]
  000f1432: ldr r0,[r0,#0x4]
  000f1434: ldr.w r0,[r0,r10,lsl #0x2]
  000f1438: blx 0x00074c44
  000f143c: mov r1,r0
  000f143e: add r0,sp,#0x78
  000f1440: blx 0x0006f658
  000f1444: ldr r1,[0x000f1778]
  000f1446: add r1,pc
  000f1448: add r0,sp,#0x6c
  000f144a: movs r2,#0x0
  000f144c: blx 0x0006ee18
  000f1450: add r0,sp,#0x84
  000f1452: add r1,sp,#0x78
  000f1454: add r2,sp,#0x6c
  000f1456: blx 0x0006ef98
  000f145a: add.w r0,r9,#0x78
  000f145e: add r1,sp,#0x84
  000f1460: blx 0x0006f2b0
  000f1464: ldr r0,[0x000f177c]
  000f1466: add r0,pc
  000f1468: ldr r4,[r0,#0x0]
  000f146a: add r0,sp,#0x84
  000f146c: blx r4
  000f146e: add r0,sp,#0x6c
  000f1470: blx r4
  000f1472: add r0,sp,#0x78
  000f1474: blx r4
  000f1476: ldr r0,[r5,#0x0]
  000f1478: movw r1,#0xc9d
  000f147c: blx 0x00072f70
  000f1480: mov r1,r0
  000f1482: add.w r0,r9,#0x6c
  000f1486: blx 0x0006f2b0
  000f148a: ldr.w r0,[r9,#0x38]
  000f148e: ldr r0,[r0,#0x4]
  000f1490: ldr.w r0,[r0,r10,lsl #0x2]
  000f1494: blx 0x00071a04
  000f1498: ldr r1,[0x000f1780]
  000f149a: cmp r0,#0x0
  000f149c: add r1,pc
  000f149e: ldr r2,[r1,#0x0]
  000f14a0: movw r1,#0xc9c
  000f14a4: str r2,[sp,#0x18]
  000f14a6: ldr r2,[r2,#0x0]
  000f14a8: it ne
  000f14aa: movw.ne r1,#0xc9b
  000f14ae: mov r0,r2
  000f14b0: blx 0x00072f70
  000f14b4: mov r1,r0
  000f14b6: add.w r0,r9,#0x60
  000f14ba: blx 0x0006f2b0
  000f14be: ldr r0,[0x000f1784]
  000f14c0: ldr.w r1,[r9,#0x20]
  000f14c4: add r0,pc
  000f14c6: ldr.w r2,[r9,#0x28]
  000f14ca: str.w r9,[sp,#0x1c]
  000f14ce: ldr r4,[r0,#0x0]
  000f14d0: ldr.w r0,[r9,#0x38]
  000f14d4: ldr r3,[r4,#0x0]
  000f14d6: ldr r0,[r0,#0x4]
  000f14d8: ldr r6,[r3,#0x2c]
  000f14da: ldr.w r0,[r0,r10,lsl #0x2]
  000f14de: subs r5,r1,r6
  000f14e0: add.w r12,r5,r2
  000f14e4: ldrd r5,r11,[r3,#0xc]
  000f14e8: ldrd r2,r8,[r3,#0x20]
  000f14ec: add r1,r5
  000f14ee: ldr.w r9,[r3,#0x2d8]
  000f14f2: add r1,r2
  000f14f4: ldr r2,[r3,#0x5c]
  000f14f6: add r1,r2
  000f14f8: add.w r10,r1,r6
  000f14fc: sub.w r6,r12,r10
  000f1500: blx 0x00073774
  000f1504: sub.w r1,r6,r11
  000f1508: ldr r5,[r4,#0x0]
  000f150a: sub.w r1,r1,r9
  000f150e: cmp r0,#0x0
  000f1510: sub.w r4,r1,r8
  000f1514: beq 0x000f151e
  000f1516: ldr r1,[r5,#0x4c]
  000f1518: ldr r0,[r5,#0x30]
  000f151a: subs r1,r4,r1
  000f151c: subs r4,r1,r0
  000f151e: movs r0,#0x24
  000f1520: blx 0x0006eb24
  000f1524: ldr.w r8,[sp,#0x1c]
  000f1528: mov r6,r0
  000f152a: ldr.w r1,[r8,#0x1c]
  000f152e: ldr.w r3,[r8,#0x24]
  000f1532: ldrd r12,r0,[r5,#0x28]
  000f1536: ldr.w r2,[r5,#0x2d8]
  000f153a: add.w r1,r1,r3, asr #0x1
  000f153e: movs r5,#0x0
  000f1540: strd r4,r5,[sp,#0x0]
  000f1544: add.w r5,r0,r10
  000f1548: add r1,r0
  000f154a: rsb r0,r0,r3, asr #0x1
  000f154e: sub.w r3,r0,r12
  000f1552: add r2,r5
  000f1554: mov r0,r6
  000f1556: blx 0x00075b20
  000f155a: ldr r1,[0x000f1788]
  000f155c: add r0,sp,#0x84
  000f155e: movs r2,#0x0
  000f1560: str.w r6,[r8,#0x2c]
  000f1564: add r1,pc
  000f1566: blx 0x0006ee18
  000f156a: ldr r4,[sp,#0x18]
  000f156c: ldr r0,[r4,#0x0]
  000f156e: movw r1,#0xc97
  000f1572: blx 0x00072f70
  000f1576: mov r1,r0
  000f1578: add r0,sp,#0x84
  000f157a: blx 0x0006f2b0
  000f157e: ldr r1,[0x000f178c]
  000f1580: add r1,pc
  000f1582: add r0,sp,#0x6c
  000f1584: movs r2,#0x0
  000f1586: blx 0x0006ee18
  000f158a: add.w r2,r8,#0x3c
  000f158e: add r0,sp,#0x78
  000f1590: add r1,sp,#0x6c
  000f1592: blx 0x0006ef98
  000f1596: add r0,sp,#0x84
  000f1598: add r1,sp,#0x78
  000f159a: blx 0x0006ef5c
  000f159e: add r0,sp,#0x78
  000f15a0: blx 0x0006ee30
  000f15a4: add r0,sp,#0x6c
  000f15a6: blx 0x0006ee30
  000f15aa: ldr r1,[0x000f1790]
  000f15ac: add r1,pc
  000f15ae: add r0,sp,#0x6c
  000f15b0: movs r2,#0x0
  000f15b2: blx 0x0006ee18
  000f15b6: ldr r0,[r4,#0x0]
  000f15b8: movw r1,#0xc98
  000f15bc: blx 0x00072f70
  000f15c0: mov r2,r0
  000f15c2: add r0,sp,#0x78
  000f15c4: add r1,sp,#0x6c
  000f15c6: blx 0x0006ef98
  000f15ca: add r0,sp,#0x84
  000f15cc: add r1,sp,#0x78
  000f15ce: blx 0x0006ef5c
  000f15d2: add r0,sp,#0x78
  000f15d4: blx 0x0006ee30
  000f15d8: add r0,sp,#0x6c
  000f15da: blx 0x0006ee30
  000f15de: ldr r1,[0x000f1794]
  000f15e0: add r1,pc
  000f15e2: add r0,sp,#0x6c
  000f15e4: movs r2,#0x0
  000f15e6: blx 0x0006ee18
  000f15ea: add.w r2,r8,#0x48
  000f15ee: add r0,sp,#0x78
  000f15f0: add r1,sp,#0x6c
  000f15f2: blx 0x0006ef98
  000f15f6: add r0,sp,#0x84
  000f15f8: add r1,sp,#0x78
  000f15fa: blx 0x0006ef5c
  000f15fe: add r0,sp,#0x78
  000f1600: blx 0x0006ee30
  000f1604: add r0,sp,#0x6c
  000f1606: blx 0x0006ee30
  000f160a: ldr r1,[0x000f1798]
  000f160c: add r1,pc
  000f160e: add r0,sp,#0x6c
  000f1610: movs r2,#0x0
  000f1612: blx 0x0006ee18
  000f1616: ldr.w r0,[r8,#0x38]
  000f161a: ldr r1,[sp,#0x20]
  000f161c: ldr r6,[r4,#0x0]
  000f161e: ldr r0,[r0,#0x4]
  000f1620: ldr.w r0,[r0,r1,lsl #0x2]
  000f1624: blx 0x00074080
  000f1628: addw r1,r0,#0xc66
  000f162c: mov r0,r6
  000f162e: blx 0x00072f70
  000f1632: mov r2,r0
  000f1634: add r0,sp,#0x78
  000f1636: add r1,sp,#0x6c
  000f1638: blx 0x0006ef98
  000f163c: add r0,sp,#0x84
  000f163e: add r1,sp,#0x78
  000f1640: blx 0x0006ef5c
  000f1644: add r0,sp,#0x78
  000f1646: blx 0x0006ee30
  000f164a: add r0,sp,#0x6c
  000f164c: blx 0x0006ee30
  000f1650: ldr.w r4,[r8,#0x2c]
  000f1654: ldr r1,[0x000f179c]
  000f1656: add r1,pc
  000f1658: add r0,sp,#0x30
  000f165a: movs r2,#0x0
  000f165c: blx 0x0006ee18
  000f1660: add r0,sp,#0x24
  000f1662: add r1,sp,#0x84
  000f1664: movs r2,#0x0
  000f1666: blx 0x0006f028
  000f166a: add r1,sp,#0x30
  000f166c: add r2,sp,#0x24
  000f166e: mov r0,r4
  000f1670: blx 0x00075b2c
  000f1674: ldr r0,[0x000f17a0]
  000f1676: add r0,pc
  000f1678: ldr r4,[r0,#0x0]
  000f167a: add r0,sp,#0x24
  000f167c: blx r4
  000f167e: add r0,sp,#0x30
  000f1680: blx r4
  000f1682: add r0,sp,#0x84
  000f1684: blx r4
  000f1686: ldr r0,[sp,#0x90]
  000f1688: ldr r1,[sp,#0x14]
  000f168a: ldr r1,[r1,#0x0]
  000f168c: subs r0,r1,r0
  000f168e: ittt eq
  000f1690: add.eq sp,#0x94
  000f1692: pop.eq.w {r8,r9,r10,r11}
  000f1696: pop.eq {r4,r5,r6,r7,pc}
  000f1698: blx 0x0006e824
```
