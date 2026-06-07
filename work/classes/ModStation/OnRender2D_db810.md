# ModStation::OnRender2D
elf 0xdb810 body 1540
Sig: undefined __thiscall OnRender2D(ModStation * this)

## decompile
```c

/* ModStation::OnRender2D() */

void __thiscall ModStation::OnRender2D(ModStation *this)

{
  int iVar1;
  uint uVar2;
  Station *pSVar3;
  undefined4 uVar4;
  int *piVar5;
  undefined4 *puVar6;
  int *piVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  String *pSVar11;
  int *piVar12;
  String *pSVar13;
  int iVar14;
  int *piVar15;
  code *pcVar16;
  bool bVar17;
  uint in_fpscr;
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
  
  piVar5 = *(int **)(DAT_000eba2c + 0xeb822);
  local_28 = *piVar5;
  AbyssEngine::PaintCanvas::Begin2d();
  AbyssEngine::PaintCanvas::SetColor((uint)this->field_4);
  if ((char)this->field_24 == '\0') goto LAB_000eb8dc;
  if (*(char *)((int)&this->field_64 + 2) == '\0') {
    if (*(char *)((int)&this->field_64 + 1) == '\0') {
      if ((char)this->field_64 == '\0') {
        if (*(char *)((int)&this->field_64 + 3) == '\0') {
          if ((char)this->field_68 == '\0') {
            if (*(char *)((int)&this->m_nStarMapWindowOpen + 2) == '\0') {
              if ((char)this->m_nStarMapWindowOpen == '\0') {
                piVar7 = *(int **)(DAT_000eba44 + 0xeb976);
                Layout::drawFooterStation();
                iVar10 = *piVar7;
                Status::getStation();
                Station::getName();
                AbyssEngine::String::String(aSStack_40,aSStack_4c,false);
                pSVar3 = (Station *)Status::getStation();
                iVar1 = Station::getIndex(pSVar3);
                if (iVar1 != 0x65) {
                  AbyssEngine::String::String(aSStack_64,(char *)(DAT_000ebe18 + 0xeba6a),false);
                  GameText::getText(**(int **)(DAT_000ebe1c + 0xeba76));
                  AbyssEngine::operator+(aSStack_58,aSStack_64);
                }
                else {
                  AbyssEngine::String::String(aSStack_58,(char *)(DAT_000eba4c + 0xeb9be),false);
                }
                AbyssEngine::operator+(aSStack_34,aSStack_40);
                Layout::drawHeader(iVar10,aSStack_34);
                AbyssEngine::String::~String(aSStack_34);
                AbyssEngine::String::~String(aSStack_58);
                if (iVar1 != 0x65) {
                  AbyssEngine::String::~String(aSStack_64);
                }
                AbyssEngine::String::~String(aSStack_40);
                AbyssEngine::String::~String(aSStack_4c);
                if (*(char *)((int)&this->m_nStarMapWindowOpen + 1) != '\0') {
                  iVar1 = this[1].field_10;
                  piVar12 = this[1].field_14;
                  piVar8 = this[1].field_18;
                  piVar15 = this[1].field_1C;
                  iVar10 = *piVar7;
                  uVar4 = AbyssEngine::String::String
                                    (aSStack_70,(char *)(DAT_000ebe20 + 0xebae2),false);
                  Layout::drawBox(iVar10,2,iVar1,piVar12,piVar8,piVar15,uVar4);
                  AbyssEngine::String::~String(aSStack_70);
                  Status::getSystem();
                  iVar1 = SolarSystem::hasNoOwner();
                  if (iVar1 == 0) {
                    AbyssEngine::PaintCanvas::DrawImage2D
                              ((uint)this->field_4,this->field_4C,*(int *)(*piVar7 + 0x28));
                  }
                  Status::getSystem();
                  SolarSystem::getName();
                  AbyssEngine::String::String(aSStack_58,aSStack_7c,false);
                  AbyssEngine::String::String(aSStack_88,(char *)(DAT_000ebe24 + 0xebb4c),false);
                  AbyssEngine::operator+(aSStack_4c,aSStack_58);
                  piVar8 = *(int **)(DAT_000ebf08 + 0xebb62);
                  GameText::getText(*piVar8);
                  AbyssEngine::operator+(aSStack_40,aSStack_4c);
                  pcVar16 = *(code **)(DAT_000ebf10 + 0xebb7c);
                  (*pcVar16)(aSStack_4c);
                  (*pcVar16)(aSStack_88);
                  (*pcVar16)(aSStack_58);
                  (*pcVar16)(aSStack_7c);
                  puVar6 = *(undefined4 **)(DAT_000ebf18 + 0xebb98);
                  iVar1 = AbyssEngine::PaintCanvas::GetTextWidth
                                    (**(uint **)(DAT_000ebf14 + 0xebb96),(String *)*puVar6);
                  piVar12 = this[1].field_18;
                  iVar9 = *(int *)(*piVar7 + 0x28);
                  iVar14 = *(int *)(*piVar7 + 0x4c);
                  iVar10 = AbyssEngine::PaintCanvas::GetImage2DWidth((uint)this->field_4);
                  if ((int)piVar12 + ((iVar9 * -2 - iVar14) - iVar10) < iVar1) {
                    Status::getSystem();
                    SolarSystem::getName();
                    AbyssEngine::String::operator=(aSStack_40,aSStack_4c);
                    AbyssEngine::String::~String(aSStack_4c);
                  }
                  piVar12 = this->field_4;
                  pSVar11 = (String *)*puVar6;
                  iVar9 = *(int *)(*piVar7 + 0x28);
                  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth((uint)piVar12);
                  iVar10 = *piVar7;
                  AbyssEngine::PaintCanvas::DrawString
                            ((uint)piVar12,pSVar11,(int)aSStack_40,
                             iVar1 + iVar9 + *(int *)(iVar10 + 0x4c),
                             (bool)((char)*(undefined4 *)(iVar10 + 0xc) +
                                    (char)*(undefined4 *)(iVar10 + 0x20) +
                                   (char)*(undefined4 *)(iVar10 + 0x2c) * '\x02'));
                  piVar12 = this->field_4;
                  pSVar13 = (String *)*puVar6;
                  pSVar11 = (String *)GameText::getText(*piVar8);
                  AbyssEngine::String::String(aSStack_7c,(char *)(DAT_000ebf1c + 0xebc48),false);
                  AbyssEngine::operator+(aSStack_58,pSVar11);
                  Status::getStation();
                  iVar1 = Station::getTecLevel();
                  AbyssEngine::String::String(aSStack_94,iVar1);
                  AbyssEngine::String::String(aSStack_88,aSStack_94,false);
                  AbyssEngine::operator+(aSStack_4c,aSStack_58);
                  iVar9 = *(int *)(*piVar7 + 0x28);
                  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth((uint)this->field_4);
                  iVar10 = *piVar7;
                  AbyssEngine::PaintCanvas::DrawString
                            ((uint)piVar12,pSVar13,(int)aSStack_4c,
                             iVar1 + iVar9 + *(int *)(iVar10 + 0x4c),
                             (bool)((char)*(undefined4 *)(iVar10 + 0xc) +
                                    (char)*(undefined4 *)(iVar10 + 0x20) +
                                    (char)*(undefined4 *)(iVar10 + 4) +
                                   (char)*(undefined4 *)(iVar10 + 0x2c) * '\x04'));
                  (*pcVar16)(aSStack_4c);
                  (*pcVar16)(aSStack_88);
                  (*pcVar16)(aSStack_94);
                  (*pcVar16)(aSStack_58);
                  (*pcVar16)(aSStack_7c);
                  if (*(char *)(*piVar7 + 0x286) != '\0') {
                    Status::getSystem();
                    iVar1 = SolarSystem::hasNoOwner();
                    if (iVar1 == 0) {
                      piVar12 = this->field_4;
                      iVar1 = *piVar8;
                      pSVar11 = (String *)*puVar6;
                      Status::getSystem();
                      SolarSystem::getRace();
                      iVar1 = GameText::getText(iVar1);
                      iVar14 = *(int *)(*piVar7 + 0x28);
                      iVar10 = AbyssEngine::PaintCanvas::GetImage2DWidth((uint)this->field_4);
                      iVar9 = *piVar7;
                      AbyssEngine::PaintCanvas::DrawString
                                ((uint)piVar12,pSVar11,iVar1,
                                 iVar10 + iVar14 + *(int *)(iVar9 + 0x4c),
                                 (bool)((char)*(undefined4 *)(iVar9 + 0xc) +
                                        (char)*(undefined4 *)(iVar9 + 0x20) +
                                        (char)*(undefined4 *)(iVar9 + 4) * '\x02' +
                                       (char)*(undefined4 *)(iVar9 + 0x2c) * '\x06'));
                    }
                  }
                  for (uVar2 = 0; uVar2 < 5; uVar2 = uVar2 + 1) {
                    TouchButton::draw();
                  }
                  TouchButton::draw();
                  TouchButton::draw();
                  pSVar3 = (Station *)Status::getStation();
                  iVar1 = Station::getIndex(pSVar3);
                  if (iVar1 != 0x65) {
                    pSVar3 = (Station *)Status::getStation();
                    iVar1 = Station::getIndex(pSVar3);
                    if (iVar1 != 0x6c) {
                      Status::getSystem();
                      piVar8 = (int *)SolarSystem::getIndex();
                      bVar17 = piVar8 != (int *)0x19;
                      if (bVar17) {
                        piVar8 = this->field_1C;
                      }
                      if (bVar17 && piVar8 != (int *)0x0) {
                        NewsTicker::draw();
                      }
                    }
                  }
                  Layout::drawMissionRewardMessage(SUB41(*piVar7,0));
                  AbyssEngine::String::~String(aSStack_40);
                }
                iVar1 = this->field_34;
                bVar17 = 2999 < (uint)this->field_30;
                if ((int)(-(uint)bVar17 - iVar1) < 0 ==
                    (SBORROW4(0,iVar1) != SBORROW4(-iVar1,(uint)bVar17))) {
                  piVar8 = this->field_4;
                  pSVar11 = (String *)**(undefined4 **)(DAT_000ebf24 + 0xebde0);
                  iVar1 = GameText::getText(**(int **)(DAT_000ebf20 + 0xebdda));
                  AbyssEngine::PaintCanvas::DrawString
                            ((uint)piVar8,pSVar11,iVar1,
                             *(int *)(*piVar7 + 0x28) + (int)this[1].field_18,
                             SUB41(*(undefined4 *)(*piVar7 + 0x18),0));
                }
                else {
                  *(undefined1 *)&this[1].field_20 = 0;
                }
              }
              else {
                Radio::draw((ulonglong)(uint)this->field_54,(PlayerEgo *)this[1].field_24,
                            (LevelScript *)(this[1].field_24 >> 0x1f));
                iVar1 = Radio::lastMessageShown();
                if (iVar1 != 0) {
                  AbyssEngine::PaintCanvas::DrawImage2D
                            (**(uint **)(DAT_000eba34 + 0xeb942),this->field_5C,0,
                             (uchar)(int)(float)this[1].field_78,'\x14');
                  ScrollTouchBox::draw();
                }
                iVar1 = DAT_000eba38;
                uVar2 = this[1].field_24;
                iVar10 = DAT_000eba40;
                if ((int)uVar2 < DAT_000eba3c) {
                  if ((int)uVar2 < DAT_000eba38) {
                    iVar10 = 12000;
                    goto LAB_000eb9d0;
                  }
                  if (0x7fffffff < uVar2) {
                    VectorSignedToFloat(uVar2 + 6000,(byte)(in_fpscr >> 0x16) & 3);
                    goto LAB_000eb9e2;
                  }
                }
                else {
LAB_000eb9d0:
                  VectorSignedToFloat(uVar2 + iVar10,(byte)(in_fpscr >> 0x16) & 3);
LAB_000eb9e2:
                  AbyssEngine::PaintCanvas::SetColor((uint)this->field_4);
                  AbyssEngine::PaintCanvas::FillRectangle
                            (**(int **)(DAT_000eba5c + 0xeba04),0,0,
                             **(int **)(DAT_000eba58 + 0xeba02));
                }
                if (iVar1 < this[1].field_24) {
                  CutScene::render2D();
                }
              }
            }
            else {
              MenuTouchWindow::draw();
            }
          }
          else {
            StatusWindow::draw();
          }
        }
        else {
          StarMap::draw();
        }
      }
      else {
        MissionsWindow::draw((MissionsWindow *)this->m_pDialogueWindow);
      }
    }
    else {
      SpaceLounge::draw();
    }
  }
  else {
    HangarWindow::render();
  }
  if ((*(char *)((int)&this->field_68 + 2) != '\0') ||
     (*(char *)((int)&this->m_nStarMapWindowOpen + 3) != '\0')) {
    ChoiceWindow::draw();
  }
  if (*(char *)((int)&this->field_68 + 1) != '\0') {
    DialogueWindow::draw();
  }
  puVar6 = *(undefined4 **)(DAT_000eba30 + 0xeb8c0);
  if (*(Layout *)*puVar6 != (Layout)0x0) {
    Layout::drawHelpWindow((Layout *)*puVar6);
  }
  AbyssEngine::PaintCanvas::End2d();
  if ((*(char *)*puVar6 == '\0') &&
     (((*(char *)((int)&this->field_64 + 2) == '\0' ||
       (HangarWindow::render3D(), *(char *)*puVar6 == '\0')) &&
      (*(char *)((int)&this->field_64 + 1) != '\0')))) {
    SpaceLounge::draw3DShip();
  }
  AbyssEngine::PaintCanvas::SwapBuffer();
LAB_000eb8dc:
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000eb810: push {r4,r5,r6,r7,lr}
  000eb812: add r7,sp,#0xc
  000eb814: push {r8,r9,r10,r11}
  000eb818: sub sp,#0x8c
  000eb81a: mov r4,r0
  000eb81c: ldr r0,[0x000eba2c]
  000eb81e: add r0,pc
  000eb820: ldr r5,[r0,#0x0]
  000eb822: ldr r0,[r5,#0x0]
  000eb824: str r0,[sp,#0x88]
  000eb826: ldr r0,[r4,#0x4]
  000eb828: blx 0x00071d34
  000eb82c: ldr r0,[r4,#0x4]
  000eb82e: mov.w r1,#0xffffffff
  000eb832: blx 0x0006fac0
  000eb836: ldrb.w r0,[r4,#0x24]
  000eb83a: cmp r0,#0x0
  000eb83c: beq 0x000eb8dc
  000eb83e: ldrb.w r0,[r4,#0x66]
  000eb842: cbz r0,0x000eb84c
  000eb844: ldr r0,[r4,#0x78]
  000eb846: blx 0x00075598
  000eb84a: b 0x000eb894
  000eb84c: ldrb.w r0,[r4,#0x65]
  000eb850: cbz r0,0x000eb85a
  000eb852: ldr r0,[r4,#0x74]
  000eb854: blx 0x000755a4
  000eb858: b 0x000eb894
  000eb85a: ldrb.w r0,[r4,#0x64]
  000eb85e: cbz r0,0x000eb86a
  000eb860: ldr.w r0,[r4,#0x80]
  000eb864: blx 0x000755b0
  000eb868: b 0x000eb894
  000eb86a: ldrb.w r0,[r4,#0x67]
  000eb86e: cbz r0,0x000eb878
  000eb870: ldr r0,[r4,#0x10]
  000eb872: blx 0x000755bc
  000eb876: b 0x000eb894
  000eb878: ldrb.w r0,[r4,#0x68]
  000eb87c: cbz r0,0x000eb886
  000eb87e: ldr r0,[r4,#0x7c]
  000eb880: blx 0x000755c8
  000eb884: b 0x000eb894
  000eb886: ldrb.w r0,[r4,#0x62]
  000eb88a: cmp r0,#0x0
  000eb88c: beq 0x000eb914
  000eb88e: ldr r0,[r4,#0x50]
  000eb890: blx 0x000755d4
  000eb894: ldrb.w r0,[r4,#0x6a]
  000eb898: cbz r0,0x000eb8a0
  000eb89a: ldr.w r0,[r4,#0x88]
  000eb89e: b 0x000eb8a8
  000eb8a0: ldrb.w r0,[r4,#0x63]
  000eb8a4: cbz r0,0x000eb8ac
  000eb8a6: ldr r0,[r4,#0x70]
  000eb8a8: blx 0x000746ec
  000eb8ac: ldrb.w r0,[r4,#0x69]
  000eb8b0: cbz r0,0x000eb8ba
  000eb8b2: ldr.w r0,[r4,#0x84]
  000eb8b6: blx 0x000755e0
  000eb8ba: ldr r0,[0x000eba30]
  000eb8bc: add r0,pc
  000eb8be: ldr r6,[r0,#0x0]
  000eb8c0: ldr r0,[r6,#0x0]
  000eb8c2: ldrb r1,[r0,#0x0]
  000eb8c4: cbz r1,0x000eb8ca
  000eb8c6: blx 0x000755ec
  000eb8ca: ldr r0,[r4,#0x4]
  000eb8cc: blx 0x00071d7c
  000eb8d0: ldr r0,[r6,#0x0]
  000eb8d2: ldrb r0,[r0,#0x0]
  000eb8d4: cbz r0,0x000eb8f0
  000eb8d6: ldr r0,[r4,#0x4]
  000eb8d8: blx 0x00071d88
  000eb8dc: ldr r0,[sp,#0x88]
  000eb8de: ldr r1,[r5,#0x0]
  000eb8e0: subs r0,r1,r0
  000eb8e2: ittt eq
  000eb8e4: add.eq sp,#0x8c
  000eb8e6: pop.eq.w {r8,r9,r10,r11}
  000eb8ea: pop.eq {r4,r5,r6,r7,pc}
  000eb8ec: blx 0x0006e824
  000eb8f0: ldrb.w r0,[r4,#0x66]
  000eb8f4: cbz r0,0x000eb904
  000eb8f6: ldr r0,[r4,#0x78]
  000eb8f8: blx 0x000755f8
  000eb8fc: ldr r0,[r6,#0x0]
  000eb8fe: ldrb r0,[r0,#0x0]
  000eb900: cmp r0,#0x0
  000eb902: bne 0x000eb8d6
  000eb904: ldrb.w r0,[r4,#0x65]
  000eb908: cmp r0,#0x0
  000eb90a: beq 0x000eb8d6
  000eb90c: ldr r0,[r4,#0x74]
  000eb90e: blx 0x00075604
  000eb912: b 0x000eb8d6
  000eb914: ldrb.w r0,[r4,#0x60]
  000eb918: cbz r0,0x000eb970
  000eb91a: ldr.w r2,[r4,#0xb4]
  000eb91e: movs r1,#0x0
  000eb920: ldr r0,[r4,#0x54]
  000eb922: strd r1,r1,[sp,#0x0]
  000eb926: asrs r3,r2,#0x1f
  000eb928: blx 0x00075610
  000eb92c: ldr r0,[r4,#0x54]
  000eb92e: blx 0x00075064
  000eb932: cbz r0,0x000eb960
  000eb934: ldr r0,[0x000eba34]
  000eb936: movs r2,#0x44
  000eb938: vldr.32 s0,[r4,#0x108]
  000eb93c: movs r3,#0x14
  000eb93e: add r0,pc
  000eb940: ldr r1,[r4,#0x5c]
  000eb942: ldr r0,[r0,#0x0]
  000eb944: ldr r0,[r0,#0x0]
  000eb946: vcvt.s32.f32 s0,s0
  000eb94a: strd r3,r2,[sp,#0x0]
  000eb94e: movs r2,#0x0
  000eb950: vmov r3,s0
  000eb954: blx 0x00071d70
  000eb958: ldr.w r0,[r4,#0x98]
  000eb95c: blx 0x0007561c
  000eb960: ldr r6,[0x000eba38]
  000eb962: ldr r1,[0x000eba3c]
  000eb964: ldr.w r0,[r4,#0xb4]
  000eb968: cmp r0,r1
  000eb96a: blt 0x000eb9c6
  000eb96c: ldr r1,[0x000eba40]
  000eb96e: b 0x000eb9d0
  000eb970: ldr r0,[0x000eba44]
  000eb972: add r0,pc
  000eb974: ldr r6,[r0,#0x0]
  000eb976: ldr r0,[r6,#0x0]
  000eb978: blx 0x00075628
  000eb97c: ldr r0,[0x000eba48]
  000eb97e: str r6,[sp,#0x18]
  000eb980: add r0,pc
  000eb982: ldr.w r8,[r6,#0x0]
  000eb986: ldr.w r9,[r0,#0x0]
  000eb98a: ldr.w r0,[r9,#0x0]
  000eb98e: blx 0x00071c14
  000eb992: add r6,sp,#0x64
  000eb994: mov r1,r0
  000eb996: mov r0,r6
  000eb998: blx 0x000736a8
  000eb99c: add r0,sp,#0x70
  000eb99e: mov r1,r6
  000eb9a0: movs r2,#0x0
  000eb9a2: blx 0x0006f028
  000eb9a6: ldr.w r0,[r9,#0x0]
  000eb9aa: blx 0x00071c14
  000eb9ae: blx 0x00071824
  000eb9b2: cmp r0,#0x65
  000eb9b4: bne 0x000eba64
  000eb9b6: ldr r1,[0x000eba4c]
  000eb9b8: movs r6,#0x0
  000eb9ba: add r1,pc
  000eb9bc: add r0,sp,#0x58
  000eb9be: movs r2,#0x0
  000eb9c0: blx 0x0006ee18
  000eb9c4: b 0x000eba8a
  000eb9c6: cmp r0,r6
  000eb9c8: bge.w 0x000ebe28
  000eb9cc: movw r1,#0x2ee0
  000eb9d0: add r0,r1
  000eb9d2: vldr.32 s2,[pc,#0x7c]
  000eb9d6: vmov s0,r0
  000eb9da: vcvt.f32.s32 s0,s0
  000eb9de: vdiv.f32 s0,s0,s2
  000eb9e2: vldr.32 s2,[pc,#0x70]
  000eb9e6: ldr r0,[r4,#0x4]
  000eb9e8: vmul.f32 s0,s0,s2
  000eb9ec: vcvt.s32.f32 s0,s0
  000eb9f0: vmov r1,s0
  000eb9f4: blx 0x0006fac0
  000eb9f8: ldr r0,[0x000eba58]
  000eb9fa: ldr r1,[0x000eba5c]
  000eb9fc: ldr r2,[0x000eba60]
  000eb9fe: add r0,pc
  000eba00: add r1,pc
  000eba02: add r2,pc
  000eba04: ldr r0,[r0,#0x0]
  000eba06: ldr r1,[r1,#0x0]
  000eba08: ldr r2,[r2,#0x0]
  000eba0a: ldr r3,[r0,#0x0]
  000eba0c: ldr r0,[r1,#0x0]
  000eba0e: ldr r1,[r2,#0x0]
  000eba10: movs r2,#0x0
  000eba12: str r1,[sp,#0x0]
  000eba14: movs r1,#0x0
  000eba16: blx 0x00074de8
  000eba1a: ldr.w r0,[r4,#0xb4]
  000eba1e: cmp r0,r6
  000eba20: ble.w 0x000eb894
  000eba24: ldr r0,[r4,#0x14]
  000eba26: blx 0x00075634
  000eba2a: b 0x000eb894
  000eba64: ldr r1,[0x000ebe18]
  000eba66: add r1,pc
  000eba68: add r0,sp,#0x4c
  000eba6a: movs r2,#0x0
  000eba6c: blx 0x0006ee18
  000eba70: ldr r0,[0x000ebe1c]
  000eba72: add r0,pc
  000eba74: ldr r0,[r0,#0x0]
  000eba76: ldr r0,[r0,#0x0]
  000eba78: movs r1,#0x88
  000eba7a: blx 0x00072f70
  000eba7e: mov r2,r0
  000eba80: add r0,sp,#0x58
  000eba82: add r1,sp,#0x4c
  000eba84: blx 0x0006ef98
  000eba88: movs r6,#0x1
  000eba8a: add r0,sp,#0x7c
  000eba8c: add r1,sp,#0x70
  000eba8e: add r2,sp,#0x58
  000eba90: blx 0x0006ef98
  000eba94: add r1,sp,#0x7c
  000eba96: mov r0,r8
  000eba98: blx 0x000746d4
  000eba9c: add r0,sp,#0x7c
  000eba9e: blx 0x0006ee30
  000ebaa2: add r0,sp,#0x58
  000ebaa4: blx 0x0006ee30
  000ebaa8: cbz r6,0x000ebab0
  000ebaaa: add r0,sp,#0x4c
  000ebaac: blx 0x0006ee30
  000ebab0: add r0,sp,#0x70
  000ebab2: blx 0x0006ee30
  000ebab6: add r0,sp,#0x64
  000ebab8: blx 0x0006ee30
  000ebabc: ldrb.w r0,[r4,#0x61]
  000ebac0: cmp r0,#0x0
  000ebac2: beq.w 0x000ebdc0
  000ebac6: str.w r9,[sp,#0x14]
  000ebaca: movs r2,#0x0
  000ebacc: ldrd r8,r9,[r4,#0xa0]
  000ebad0: ldrd r6,r11,[r4,#0xa8]
  000ebad4: ldr r0,[sp,#0x18]
  000ebad6: ldr.w r10,[r0,#0x0]
  000ebada: add r0,sp,#0x40
  000ebadc: ldr r1,[0x000ebe20]
  000ebade: add r1,pc
  000ebae0: blx 0x0006ee18
  000ebae4: strd r6,r11,[sp,#0x0]
  000ebae8: movs r1,#0x2
  000ebaea: str r0,[sp,#0x8]
  000ebaec: mov r0,r10
  000ebaee: mov r2,r8
  000ebaf0: mov r3,r9
  000ebaf2: blx 0x0007462c
  000ebaf6: add r0,sp,#0x40
  000ebaf8: blx 0x0006ee30
  000ebafc: ldr.w r8,[sp,#0x14]
  000ebb00: ldr.w r0,[r8,#0x0]
  000ebb04: blx 0x00071a10
  000ebb08: blx 0x00074680
  000ebb0c: cbnz r0,0x000ebb2a
  000ebb0e: ldr r0,[sp,#0x18]
  000ebb10: ldr r0,[r0,#0x0]
  000ebb12: ldr.w r12,[r0,#0xc]
  000ebb16: ldr r6,[r0,#0x20]
  000ebb18: ldrd r2,r3,[r0,#0x28]
  000ebb1c: add r6,r12
  000ebb1e: ldr r0,[r4,#0x4]
  000ebb20: ldr r1,[r4,#0x4c]
  000ebb22: add.w r3,r6,r3, lsl #0x1
  000ebb26: blx 0x00072dc0
  000ebb2a: ldr.w r0,[r8,#0x0]
  000ebb2e: blx 0x00071a10
  000ebb32: add r6,sp,#0x34
  000ebb34: mov r1,r0
  000ebb36: mov r0,r6
  000ebb38: blx 0x00071cc8
  000ebb3c: add r0,sp,#0x58
  000ebb3e: mov r1,r6
  000ebb40: movs r2,#0x0
  000ebb42: blx 0x0006f028
  000ebb46: ldr r1,[0x000ebe24]
  000ebb48: add r1,pc
  000ebb4a: add r0,sp,#0x28
  000ebb4c: movs r2,#0x0
  000ebb4e: blx 0x0006ee18
  000ebb52: add r0,sp,#0x64
  000ebb54: add r1,sp,#0x58
  000ebb56: add r2,sp,#0x28
  000ebb58: blx 0x0006ef98
  000ebb5c: ldr r0,[0x000ebf08]
  000ebb5e: add r0,pc
  000ebb60: ldr r0,[r0,#0x0]
  000ebb62: str r0,[sp,#0xc]
  000ebb64: ldr r0,[r0,#0x0]
  000ebb66: movs r1,#0x89
  000ebb68: blx 0x00072f70
  000ebb6c: mov r2,r0
  000ebb6e: add r0,sp,#0x70
  000ebb70: add r1,sp,#0x64
  000ebb72: blx 0x0006ef98
  000ebb76: ldr r0,[0x000ebf10]
  000ebb78: add r0,pc
  000ebb7a: ldr.w r11,[r0,#0x0]
  000ebb7e: add r0,sp,#0x64
  000ebb80: blx r11
  000ebb82: add r0,sp,#0x28
  000ebb84: blx r11
  000ebb86: add r0,sp,#0x58
  000ebb88: blx r11
  000ebb8a: add r0,sp,#0x34
  000ebb8c: blx r11
  000ebb8e: ldr r0,[0x000ebf14]
  000ebb90: ldr r1,[0x000ebf18]
  000ebb92: add r0,pc
  000ebb94: add r1,pc
  000ebb96: ldr r0,[r0,#0x0]
  000ebb98: ldr r1,[r1,#0x0]
  000ebb9a: str r1,[sp,#0x10]
  000ebb9c: ldr r0,[r0,#0x0]
  000ebb9e: ldr r1,[r1,#0x0]
  000ebba0: add r2,sp,#0x70
  000ebba2: blx 0x0006faa8
  000ebba6: mov r8,r0
  000ebba8: ldr r0,[sp,#0x18]
  000ebbaa: ldr r1,[r4,#0x4c]
  000ebbac: ldr.w r9,[r4,#0xa8]
  000ebbb0: ldr r2,[r0,#0x0]
  000ebbb2: ldr r0,[r4,#0x4]
  000ebbb4: ldr r6,[r2,#0x28]
  000ebbb6: ldr.w r10,[r2,#0x4c]
  000ebbba: blx 0x00072d84
  000ebbbe: sub.w r1,r9,r6, lsl #0x1
  000ebbc2: ldr r6,[sp,#0x18]
  000ebbc4: sub.w r1,r1,r10
  000ebbc8: subs r0,r1,r0
  000ebbca: cmp r8,r0
  000ebbcc: ble 0x000ebbec
  000ebbce: ldr r0,[sp,#0x14]
  000ebbd0: ldr r0,[r0,#0x0]
  000ebbd2: blx 0x00071a10
  000ebbd6: mov r1,r0
  000ebbd8: add r0,sp,#0x64
  000ebbda: blx 0x00071cc8
  000ebbde: add r0,sp,#0x70
  000ebbe0: add r1,sp,#0x64
  000ebbe2: blx 0x0006f2b0
  000ebbe6: add r0,sp,#0x64
  000ebbe8: blx 0x0006ee30
  000ebbec: ldr r0,[r6,#0x0]
  000ebbee: ldr r2,[sp,#0x10]
  000ebbf0: ldr.w r10,[r4,#0x4]
  000ebbf4: ldr r1,[r4,#0x4c]
  000ebbf6: ldr.w r8,[r2,#0x0]
  000ebbfa: ldr.w r9,[r0,#0x28]
  000ebbfe: mov r0,r10
  000ebc00: blx 0x00072d84
  000ebc04: ldr r1,[sp,#0x18]
  000ebc06: ldr r1,[r1,#0x0]
  000ebc08: ldr r2,[r1,#0xc]
  000ebc0a: ldr r3,[r1,#0x20]
  000ebc0c: ldr r6,[r1,#0x2c]
  000ebc0e: ldr r1,[r1,#0x4c]
  000ebc10: add r2,r3
  000ebc12: add r0,r9
  000ebc14: mov.w r12,#0x0
  000ebc18: add.w r2,r2,r6, lsl #0x1
  000ebc1c: adds r3,r0,r1
  000ebc1e: strd r2,r12,[sp,#0x0]
  000ebc22: add r2,sp,#0x70
  000ebc24: mov r0,r10
  000ebc26: mov r1,r8
  000ebc28: blx 0x0006fe14
  000ebc2c: ldr r0,[sp,#0x10]
  000ebc2e: ldr.w r8,[r4,#0x4]
  000ebc32: ldr.w r9,[r0,#0x0]
  000ebc36: ldr r0,[sp,#0xc]
  000ebc38: ldr r0,[r0,#0x0]
  000ebc3a: movs r1,#0x85
  000ebc3c: blx 0x00072f70
  000ebc40: ldr r1,[0x000ebf1c]
  000ebc42: mov r6,r0
  000ebc44: add r1,pc
  000ebc46: add r0,sp,#0x34
  000ebc48: movs r2,#0x0
  000ebc4a: blx 0x0006ee18
  000ebc4e: add r0,sp,#0x58
  000ebc50: add r2,sp,#0x34
  000ebc52: mov r1,r6
  000ebc54: blx 0x0006ef98
  000ebc58: ldr r0,[sp,#0x14]
  000ebc5a: ldr r0,[r0,#0x0]
  000ebc5c: blx 0x00071c14
  000ebc60: ldr r6,[sp,#0x18]
  000ebc62: blx 0x00071890
  000ebc66: mov r1,r0
  000ebc68: add r0,sp,#0x1c
  000ebc6a: blx 0x0006f658
  000ebc6e: add r0,sp,#0x28
  000ebc70: add r1,sp,#0x1c
  000ebc72: movs r2,#0x0
  000ebc74: blx 0x0006f028
  000ebc78: add r0,sp,#0x64
  000ebc7a: add r1,sp,#0x58
  000ebc7c: add r2,sp,#0x28
  000ebc7e: blx 0x0006ef98
  000ebc82: ldr r0,[r6,#0x0]
  000ebc84: ldr r1,[r4,#0x4c]
  000ebc86: ldr.w r10,[r0,#0x28]
  000ebc8a: ldr r0,[r4,#0x4]
  000ebc8c: blx 0x00072d84
  000ebc90: ldr r1,[r6,#0x0]
  000ebc92: ldr.w r12,[r1,#0x4]
  000ebc96: ldr r3,[r1,#0xc]
  000ebc98: ldr r6,[r1,#0x20]
  000ebc9a: ldr r2,[r1,#0x2c]
  000ebc9c: ldr r1,[r1,#0x4c]
  000ebc9e: add r3,r6
  000ebca0: mov.w lr,#0x0
  000ebca4: add r3,r12
  000ebca6: add r0,r10
  000ebca8: add.w r2,r3,r2, lsl #0x2
  000ebcac: adds r3,r0,r1
  000ebcae: ldr r6,[sp,#0x18]
  000ebcb0: mov r0,r8
  000ebcb2: strd r2,lr,[sp,#0x0]
  000ebcb6: add r2,sp,#0x64
  000ebcb8: mov r1,r9
  000ebcba: blx 0x0006fe14
  000ebcbe: add r0,sp,#0x64
  000ebcc0: blx r11
  000ebcc2: add r0,sp,#0x28
  000ebcc4: blx r11
  000ebcc6: add r0,sp,#0x1c
  000ebcc8: blx r11
  000ebcca: add r0,sp,#0x58
  000ebccc: blx r11
  000ebcce: add r0,sp,#0x34
  000ebcd0: blx r11
  000ebcd2: ldr r0,[r6,#0x0]
  000ebcd4: ldrb.w r0,[r0,#0x286]
  000ebcd8: cbz r0,0x000ebd52
  000ebcda: ldr r0,[sp,#0x14]
  000ebcdc: ldr r0,[r0,#0x0]
  000ebcde: blx 0x00071a10
  000ebce2: blx 0x00074680
  000ebce6: cbnz r0,0x000ebd52
  000ebce8: ldr r0,[sp,#0xc]
  000ebcea: ldr.w r8,[r4,#0x4]
  000ebcee: ldr r6,[r0,#0x0]
  000ebcf0: ldr r0,[sp,#0x10]
  000ebcf2: ldr.w r9,[r0,#0x0]
  000ebcf6: ldr r0,[sp,#0x14]
  000ebcf8: ldr r0,[r0,#0x0]
  000ebcfa: blx 0x00071a10
  000ebcfe: blx 0x00071950
  000ebd02: add.w r1,r0,#0x196
  000ebd06: mov r0,r6
  000ebd08: blx 0x00072f70
  000ebd0c: mov r11,r0
  000ebd0e: ldr r0,[sp,#0x18]
  000ebd10: ldr r1,[r4,#0x4c]
  000ebd12: ldr r0,[r0,#0x0]
  000ebd14: ldr.w r10,[r0,#0x28]
  000ebd18: ldr r0,[r4,#0x4]
  000ebd1a: blx 0x00072d84
  000ebd1e: ldr r1,[sp,#0x18]
  000ebd20: ldr r1,[r1,#0x0]
  000ebd22: ldr.w r12,[r1,#0x4]
  000ebd26: ldr r3,[r1,#0xc]
  000ebd28: ldr r6,[r1,#0x20]
  000ebd2a: ldr r2,[r1,#0x2c]
  000ebd2c: ldr r1,[r1,#0x4c]
  000ebd2e: add r3,r6
  000ebd30: add.w r2,r2,r2, lsl #0x1
  000ebd34: add.w r3,r3,r12, lsl #0x1
  000ebd38: add r0,r10
  000ebd3a: add.w r2,r3,r2, lsl #0x1
  000ebd3e: adds r3,r0,r1
  000ebd40: mov.w lr,#0x0
  000ebd44: mov r0,r8
  000ebd46: strd r2,lr,[sp,#0x0]
  000ebd4a: mov r1,r9
  000ebd4c: mov r2,r11
  000ebd4e: blx 0x0006fe14
  000ebd52: movs r6,#0x0
  000ebd54: b 0x000ebd66
  000ebd56: ldr.w r0,[r4,#0x8c]
  000ebd5a: ldr r0,[r0,#0x4]
  000ebd5c: ldr.w r0,[r0,r6,lsl #0x2]
  000ebd60: blx 0x000746e0
  000ebd64: adds r6,#0x1
  000ebd66: cmp r6,#0x5
  000ebd68: bcc 0x000ebd56
  000ebd6a: ldr.w r0,[r4,#0x90]
  000ebd6e: blx 0x000746e0
  000ebd72: ldr.w r0,[r4,#0x94]
  000ebd76: blx 0x000746e0
  000ebd7a: ldr r0,[sp,#0x14]
  000ebd7c: ldr r0,[r0,#0x0]
  000ebd7e: blx 0x00071c14
  000ebd82: blx 0x00071824
  000ebd86: cmp r0,#0x65
  000ebd88: beq 0x000ebdb0
  000ebd8a: ldr r0,[sp,#0x14]
  000ebd8c: ldr r0,[r0,#0x0]
  000ebd8e: blx 0x00071c14
  000ebd92: blx 0x00071824
  000ebd96: cmp r0,#0x6c
  000ebd98: beq 0x000ebdb0
  000ebd9a: ldr r0,[sp,#0x14]
  000ebd9c: ldr r0,[r0,#0x0]
  000ebd9e: blx 0x00071a10
  000ebda2: blx 0x00071a7c
  000ebda6: cmp r0,#0x19
  000ebda8: itt ne
  000ebdaa: ldr.ne r0,[r4,#0x1c]
  000ebdac: cmp.ne r0,#0x0
  000ebdae: bne 0x000ebe56
  000ebdb0: ldr r0,[sp,#0x18]
  000ebdb2: ldr r0,[r0,#0x0]
  000ebdb4: movs r1,#0x1
  000ebdb6: blx 0x00075640
  000ebdba: add r0,sp,#0x70
  000ebdbc: blx 0x0006ee30
  000ebdc0: ldrd r0,r1,[r4,#0x30]
  000ebdc4: movw r2,#0xbb7
  000ebdc8: movs r6,#0x0
  000ebdca: subs r0,r2,r0
  000ebdcc: sbcs.w r0,r6,r1
  000ebdd0: blt 0x000ebe50
  000ebdd2: ldr r0,[0x000ebf20]
  000ebdd4: ldr r1,[0x000ebf24]
  000ebdd6: add r0,pc
  000ebdd8: ldrb.w r2,[r4,#0xb0]
  000ebddc: add r1,pc
  000ebdde: ldr.w r8,[r4,#0x4]
  000ebde2: ldr r0,[r0,#0x0]
  000ebde4: cmp r2,#0x0
  000ebde6: ldr r1,[r1,#0x0]
  000ebde8: ldr r0,[r0,#0x0]
  000ebdea: ldr.w r9,[r1,#0x0]
  000ebdee: mov.w r1,#0x36
  000ebdf2: it eq
  000ebdf4: mov.eq r1,#0x32
  000ebdf6: blx 0x00072f70
  000ebdfa: mov r2,r0
  000ebdfc: ldr r0,[sp,#0x18]
  000ebdfe: ldr.w r1,[r4,#0xa8]
  000ebe02: ldr r0,[r0,#0x0]
  000ebe04: ldr r3,[r0,#0x18]
  000ebe06: ldr r0,[r0,#0x28]
  000ebe08: strd r3,r6,[sp,#0x0]
  000ebe0c: adds r3,r0,r1
  000ebe0e: mov r0,r8
  000ebe10: mov r1,r9
  000ebe12: blx 0x0006fe14
  000ebe16: b 0x000eb894
  000ebe28: cmp.w r0,#0xffffffff
  000ebe2c: bgt.w 0x000eba1a
  000ebe30: movw r1,#0x1770
  000ebe34: add r0,r1
  000ebe36: vldr.32 s2,[pc,#0xd4]
  000ebe3a: vmov s0,r0
  000ebe3e: vcvt.f32.s32 s0,s0
  000ebe42: vdiv.f32 s0,s0,s2
  000ebe46: vmov.f32 s2,0x3f800000
  000ebe4a: vadd.f32 s0,s0,s2
  000ebe4e: b 0x000eb9e2
  000ebe50: strb.w r6,[r4,#0xb0]
  000ebe54: b 0x000eb894
  000ebe56: blx 0x0007564c
  000ebe5a: b 0x000ebdb0
```
