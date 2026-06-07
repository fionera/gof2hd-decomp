# Hud::drawOrbitInformation
elf 0x166018 body 506
Sig: undefined __stdcall drawOrbitInformation(void)

## decompile
```c

/* Hud::drawOrbitInformation() */

void Hud::drawOrbitInformation(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  SolarSystem *this;
  int iVar4;
  code *pcVar5;
  String *pSVar6;
  uint uVar7;
  int *piVar8;
  int *piVar9;
  int *piVar10;
  uint *puVar11;
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar10 = *(int **)(DAT_00176254 + 0x17602c);
  piVar9 = *(int **)(DAT_00176258 + 0x17602e);
  local_28 = *piVar10;
  iVar1 = Status::inAlienOrbit();
  if (iVar1 == 0) {
    puVar11 = *(uint **)(DAT_0017625c + 0x176052);
    AbyssEngine::PaintCanvas::SetColor(*puVar11);
    iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar11);
    piVar8 = *(int **)(DAT_00176260 + 0x17606e);
    iVar1 = iVar1 + *(int *)(*piVar8 + 0x21c);
    Status::getSystem();
    iVar2 = SolarSystem::hasNoOwner();
    if (iVar2 == 0) {
      AbyssEngine::PaintCanvas::DrawImage2D(*puVar11,*(int *)(in_r0 + 0x1c4),3);
    }
    uVar7 = *puVar11;
    puVar3 = *(undefined4 **)(DAT_00176264 + 0x1760a6);
    pSVar6 = (String *)*puVar3;
    Status::getStation();
    Station::getName();
    AbyssEngine::PaintCanvas::DrawString
              (uVar7,pSVar6,(int)aSStack_34,iVar1,SUB41(*(undefined4 *)(*piVar8 + 0x220),0));
    AbyssEngine::String::~String(aSStack_34);
    AbyssEngine::PaintCanvas::SetColor(*puVar11);
    iVar2 = Status::getCurrentCampaignMission();
    if (0xf < iVar2) {
      this = (SolarSystem *)Status::getSystem();
      iVar2 = SolarSystem::getSecurityLevel(this);
      Status::getSystem();
      iVar4 = SolarSystem::getIndex();
      if ((iVar4 == 0x1a) && (1 < *(int *)(*piVar9 + 0x114))) {
        iVar2 = 3;
      }
      uVar7 = *puVar11;
      pSVar6 = (String *)*puVar3;
      Status::getSystem();
      SolarSystem::getName();
      AbyssEngine::String::String(aSStack_4c,aSStack_58,false);
      AbyssEngine::String::String(aSStack_64,(char *)(DAT_00176268 + 0x17614a),false);
      AbyssEngine::operator+(aSStack_40,aSStack_4c);
      piVar9 = *(int **)(DAT_0017626c + 0x176160);
      GameText::getText(*piVar9);
      AbyssEngine::operator+(aSStack_34,aSStack_40);
      AbyssEngine::PaintCanvas::DrawString
                (uVar7,pSVar6,(int)aSStack_34,iVar1,SUB41(*(undefined4 *)(*piVar8 + 0x224),0));
      pcVar5 = *(code **)(DAT_00176270 + 0x176198);
      (*pcVar5)(aSStack_34);
      (*pcVar5)(aSStack_40);
      (*pcVar5)(aSStack_64);
      (*pcVar5)(aSStack_4c);
      (*pcVar5)(aSStack_58);
      iVar4 = DAT_00176274 + 0x1761ba + iVar2 * 0xc;
      AbyssEngine::PaintCanvas::SetColor
                ((uchar)*puVar11,*(uchar *)(DAT_00176274 + 0x1761ba + iVar2 * 0xc),
                 *(uchar *)(iVar4 + 4),*(uchar *)(iVar4 + 8));
      uVar7 = *puVar11;
      pSVar6 = (String *)*puVar3;
      iVar2 = GameText::getText(*piVar9);
      AbyssEngine::PaintCanvas::DrawString
                (uVar7,pSVar6,iVar2,iVar1,SUB41(*(undefined4 *)(*piVar8 + 0x228),0));
    }
  }
  if (*piVar10 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00176018: push {r4,r5,r6,r7,lr}
  0017601a: add r7,sp,#0xc
  0017601c: push {r8,r9,r10,r11}
  00176020: sub sp,#0x54
  00176022: mov r5,r0
  00176024: ldr r0,[0x00176254]
  00176026: ldr r1,[0x00176258]
  00176028: add r0,pc
  0017602a: add r1,pc
  0017602c: ldr.w r10,[r0,#0x0]
  00176030: ldr.w r9,[r1,#0x0]
  00176034: ldr.w r1,[r10,#0x0]
  00176038: ldr.w r0,[r9,#0x0]
  0017603c: str r1,[sp,#0x50]
  0017603e: blx 0x000723d0
  00176042: cmp r0,#0x0
  00176044: bne.w 0x001761fc
  00176048: ldr r0,[0x0017625c]
  0017604a: mov.w r1,#0xffffffff
  0017604e: add r0,pc
  00176050: ldr.w r11,[r0,#0x0]
  00176054: ldr.w r0,[r11,#0x0]
  00176058: blx 0x0006fac0
  0017605c: ldr.w r1,[r5,#0x1c4]
  00176060: ldr.w r0,[r11,#0x0]
  00176064: blx 0x00072d84
  00176068: ldr r1,[0x00176260]
  0017606a: add r1,pc
  0017606c: ldr.w r8,[r1,#0x0]
  00176070: ldr.w r1,[r8,#0x0]
  00176074: ldr.w r2,[r1,#0x21c]
  00176078: ldr.w r1,[r9,#0x0]
  0017607c: add r0,r2
  0017607e: str r0,[sp,#0x10]
  00176080: mov r0,r1
  00176082: blx 0x00071a10
  00176086: blx 0x00074680
  0017608a: cbnz r0,0x0017609c
  0017608c: ldr.w r1,[r5,#0x1c4]
  00176090: movs r2,#0x3
  00176092: ldr.w r0,[r11,#0x0]
  00176096: movs r3,#0x3
  00176098: blx 0x00072dc0
  0017609c: ldr r0,[0x00176264]
  0017609e: ldr.w r6,[r11,#0x0]
  001760a2: add r0,pc
  001760a4: ldr r0,[r0,#0x0]
  001760a6: str r0,[sp,#0xc]
  001760a8: ldr r5,[r0,#0x0]
  001760aa: ldr.w r0,[r9,#0x0]
  001760ae: blx 0x00071c14
  001760b2: add r4,sp,#0x44
  001760b4: mov r1,r0
  001760b6: mov r0,r4
  001760b8: blx 0x000736a8
  001760bc: ldr.w r0,[r8,#0x0]
  001760c0: ldr.w r0,[r0,#0x220]
  001760c4: movs r1,#0x0
  001760c6: mov r2,r4
  001760c8: strd r0,r1,[sp,#0x0]
  001760cc: mov r0,r6
  001760ce: ldr r3,[sp,#0x10]
  001760d0: mov r1,r5
  001760d2: blx 0x0006fe14
  001760d6: add r0,sp,#0x44
  001760d8: blx 0x0006ee30
  001760dc: ldr.w r0,[r11,#0x0]
  001760e0: ldr r1,[0x00176250]
  001760e2: blx 0x0006fac0
  001760e6: ldr.w r0,[r9,#0x0]
  001760ea: blx 0x00071770
  001760ee: cmp r0,#0x10
  001760f0: blt.w 0x001761fc
  001760f4: ldr.w r0,[r9,#0x0]
  001760f8: blx 0x00071a10
  001760fc: blx 0x00073ef4
  00176100: mov r5,r0
  00176102: ldr.w r0,[r9,#0x0]
  00176106: blx 0x00071a10
  0017610a: blx 0x00071a7c
  0017610e: mov r1,r0
  00176110: ldr.w r0,[r9,#0x0]
  00176114: cmp r1,#0x1a
  00176116: bne 0x00176122
  00176118: ldr.w r1,[r0,#0x114]
  0017611c: cmp r1,#0x1
  0017611e: it gt
  00176120: mov.gt r5,#0x3
  00176122: ldr r1,[sp,#0xc]
  00176124: ldr.w r6,[r11,#0x0]
  00176128: ldr.w r9,[r1,#0x0]
  0017612c: blx 0x00071a10
  00176130: add r4,sp,#0x20
  00176132: mov r1,r0
  00176134: mov r0,r4
  00176136: blx 0x00071cc8
  0017613a: add r0,sp,#0x2c
  0017613c: mov r1,r4
  0017613e: movs r2,#0x0
  00176140: blx 0x0006f028
  00176144: ldr r1,[0x00176268]
  00176146: add r1,pc
  00176148: add r0,sp,#0x14
  0017614a: movs r2,#0x0
  0017614c: blx 0x0006ee18
  00176150: add r0,sp,#0x38
  00176152: add r1,sp,#0x2c
  00176154: add r2,sp,#0x14
  00176156: blx 0x0006ef98
  0017615a: ldr r0,[0x0017626c]
  0017615c: add r0,pc
  0017615e: ldr r4,[r0,#0x0]
  00176160: ldr r0,[r4,#0x0]
  00176162: movs r1,#0x89
  00176164: blx 0x00072f70
  00176168: mov r2,r0
  0017616a: add r0,sp,#0x44
  0017616c: add r1,sp,#0x38
  0017616e: blx 0x0006ef98
  00176172: ldr.w r0,[r8,#0x0]
  00176176: str r4,[sp,#0x8]
  00176178: ldr.w r0,[r0,#0x224]
  0017617c: movs r1,#0x0
  0017617e: add r2,sp,#0x44
  00176180: strd r0,r1,[sp,#0x0]
  00176184: mov r1,r9
  00176186: ldr.w r9,[sp,#0x10]
  0017618a: mov r0,r6
  0017618c: mov r3,r9
  0017618e: blx 0x0006fe14
  00176192: ldr r0,[0x00176270]
  00176194: add r0,pc
  00176196: ldr r4,[r0,#0x0]
  00176198: add r0,sp,#0x44
  0017619a: blx r4
  0017619c: add r0,sp,#0x38
  0017619e: blx r4
  001761a0: add r0,sp,#0x14
  001761a2: blx r4
  001761a4: add r0,sp,#0x2c
  001761a6: blx r4
  001761a8: add r0,sp,#0x20
  001761aa: blx r4
  001761ac: ldr r0,[0x00176274]
  001761ae: add.w r2,r5,r5, lsl #0x1
  001761b2: mov r6,r5
  001761b4: movs r5,#0xff
  001761b6: add r0,pc
  001761b8: str r5,[sp,#0x0]
  001761ba: add.w r3,r0,r2, lsl #0x2
  001761be: ldrb.w r1,[r0,r2,lsl #0x2]
  001761c2: ldrb r2,[r3,#0x4]
  001761c4: ldr.w r0,[r11,#0x0]
  001761c8: ldrb r3,[r3,#0x8]
  001761ca: blx 0x0006ff28
  001761ce: ldr r0,[sp,#0xc]
  001761d0: add.w r1,r6,#0x192
  001761d4: ldr.w r5,[r11,#0x0]
  001761d8: ldr r4,[r0,#0x0]
  001761da: ldr r0,[sp,#0x8]
  001761dc: ldr r0,[r0,#0x0]
  001761de: blx 0x00072f70
  001761e2: mov r2,r0
  001761e4: ldr.w r0,[r8,#0x0]
  001761e8: mov r1,r4
  001761ea: mov r3,r9
  001761ec: ldr.w r0,[r0,#0x228]
  001761f0: str r0,[sp,#0x0]
  001761f2: movs r0,#0x0
  001761f4: str r0,[sp,#0x4]
  001761f6: mov r0,r5
  001761f8: blx 0x0006fe14
  001761fc: ldr r0,[sp,#0x50]
  001761fe: ldr.w r1,[r10,#0x0]
  00176202: subs r0,r1,r0
  00176204: ittt eq
  00176206: add.eq sp,#0x54
  00176208: pop.eq.w {r8,r9,r10,r11}
  0017620c: pop.eq {r4,r5,r6,r7,pc}
  0017620e: blx 0x0006e824
```
