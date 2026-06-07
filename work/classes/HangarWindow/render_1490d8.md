# HangarWindow::render
elf 0x1490d8 body 8732
Sig: undefined __stdcall render(void)

## decompile
```c

/* HangarWindow::render() */

void HangarWindow::render(void)

{
  char cVar1;
  byte bVar2;
  HangarWindow *in_r0;
  int *piVar3;
  int iVar4;
  uint *puVar5;
  float fVar6;
  float fVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  String *pSVar12;
  Item *pIVar13;
  Array *pAVar14;
  Ship *pSVar15;
  Layout *this;
  String *pSVar16;
  int iVar17;
  undefined4 *puVar18;
  uint *puVar19;
  int iVar20;
  int *piVar21;
  uint uVar22;
  uint uVar23;
  BluePrint *pBVar24;
  code *pcVar25;
  ImageFactory *pIVar26;
  char *pcVar27;
  float *pfVar28;
  ChoiceWindow *this_00;
  undefined4 uVar29;
  int iVar30;
  HangarWindow *pHVar31;
  String *pSVar32;
  uint uVar33;
  TouchButton *pTVar34;
  int iVar35;
  ListItem *this_01;
  int iVar36;
  int *piVar37;
  int *piVar38;
  int iVar39;
  int iVar40;
  int *piVar41;
  uint in_fpscr;
  int iVar42;
  int iVar43;
  float fVar44;
  float fVar45;
  float fVar46;
  float fVar47;
  bool bVar48;
  int local_168;
  String aSStack_128 [12];
  String aSStack_11c [12];
  String aSStack_110 [12];
  String aSStack_104 [12];
  String aSStack_f8 [12];
  String aSStack_ec [12];
  String aSStack_e0 [12];
  String aSStack_d4 [12];
  String aSStack_c8 [12];
  String aSStack_bc [12];
  String aSStack_b0 [12];
  String aSStack_a4 [12];
  String aSStack_98 [12];
  String aSStack_8c [12];
  String aSStack_80 [12];
  String aSStack_74 [12];
  String aSStack_68 [12];
  String aSStack_5c [12];
  String aSStack_50 [12];
  int local_44;
  
  piVar3 = *(int **)(DAT_00159414 + 0x1590f0);
  puVar19 = *(uint **)(DAT_00159418 + 0x1590f2);
  local_44 = *piVar3;
  AbyssEngine::PaintCanvas::SetColor(*puVar19);
  iVar4 = AbyssEngine::ApplicationManager::GetApplicationModule
                    (**(uint **)(DAT_0015941c + 0x15910e));
  cVar1 = '\0';
  if (iVar4 != 0) {
    cVar1 = *(char *)(iVar4 + 0x18);
  }
  if (iVar4 == 0 || cVar1 == '\0') {
    iVar4 = *(int *)(in_r0 + 0x58);
    if (iVar4 != 0) goto LAB_0015a83c;
    piVar37 = *(int **)(DAT_00159420 + 0x159138);
    Layout::drawBG();
    iVar4 = HangarList::getCurrentTab(*(HangarList **)(in_r0 + 0x14));
    puVar5 = (uint *)HangarList::getCurrentTabItems(*(HangarList **)(in_r0 + 0x14));
    if (puVar5 != (uint *)0x0) {
      fVar6 = (float)getRelativeScrollStartPos(in_r0);
      fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0xd8),(byte)(in_fpscr >> 0x16) & 3
                                         );
      fVar7 = (float)getRelativeScrollHeight(in_r0);
      piVar21 = *(int **)(DAT_00159424 + 0x159184);
      iVar8 = *piVar37;
      iVar43 = (int)(fVar7 * fVar47);
      iVar17 = *piVar21;
      iVar42 = (int)(fVar6 * fVar47);
      if (0 < iVar43) {
        iVar17 = (iVar17 - *(int *)(iVar8 + 0x48)) - *(int *)(iVar8 + 0x2c);
      }
      if (iVar4 == 0) {
        iVar30 = *(int *)(iVar8 + 0x4c) << 1;
      }
      else if (iVar4 == 4 || iVar4 == 1) {
        iVar30 = *(int *)(iVar8 + 0x50);
      }
      else {
        iVar30 = 0;
      }
      iVar17 = iVar17 + (*(int *)(iVar8 + 0x28) + *(int *)(in_r0 + 0xf4)) * -2;
      iVar20 = *(int *)(iVar8 + 0x2cc);
      iVar8 = *(int *)(iVar8 + 0x4c);
      if ((**(char **)(DAT_00159428 + 0x1591f2) != '\0') &&
         (**(char **)(DAT_0015942c + 0x159208) == '\0')) {
        iVar9 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar19);
        iVar10 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar19);
        iVar11 = __aeabi_idiv(**(undefined4 **)(DAT_00159430 + 0x159232));
        iVar35 = 0;
        for (iVar39 = 0; iVar39 <= iVar11; iVar39 = iVar39 + 1) {
          AbyssEngine::PaintCanvas::DrawImage2D
                    ((PaintCanvas *)*puVar19,*(uint *)(in_r0 + 0xf0),
                     (*(int *)(*piVar37 + 0x28) - iVar9) + *(int *)(in_r0 + 0xf4),iVar35,'\x01');
          iVar36 = *piVar37;
          if (iVar43 < 1) {
            iVar40 = 0;
          }
          else {
            iVar40 = *(int *)(iVar36 + 0x48) + *(int *)(iVar36 + 0x2c);
          }
          AbyssEngine::PaintCanvas::DrawImage2D
                    ((PaintCanvas *)*puVar19,*(uint *)(in_r0 + 0xf0),
                     *(int *)(in_r0 + 0xf4) + *(int *)(iVar36 + 0x28) + iVar17 + iVar40,iVar35,'\0')
          ;
          iVar35 = iVar35 + iVar10;
        }
      }
      pHVar31 = in_r0 + 0x24;
      iVar8 = iVar8 + iVar20 + iVar30;
      for (iVar20 = 0; fVar6 = DAT_00159434, iVar20 != 0x18; iVar20 = iVar20 + 1) {
        if ((in_r0[0xd0] == (HangarWindow)0x0) &&
           (pTVar34 = *(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + iVar20 * 4),
           pTVar34 != (TouchButton *)0x0)) {
          TouchButton::setVisible(pTVar34,false);
        }
      }
      iVar20 = iVar30 + -2;
      piVar41 = *(int **)(DAT_00159438 + 0x159304);
      for (uVar22 = 0; uVar22 < *puVar5; uVar22 = uVar22 + 1) {
        iVar9 = *piVar37;
        iVar9 = (*(int *)(iVar9 + 0x70) + *(int *)(in_r0 + 0x10c)) * uVar22 +
                *(int *)(in_r0 + 0xb4) + *(int *)(iVar9 + 0x20) + *(int *)(iVar9 + 0xc);
        if ((-1 < iVar9) && (iVar9 <= *piVar41)) {
          this_01 = *(ListItem **)(puVar5[1] + uVar22 * 4);
          iVar10 = ::ListItem::isSelectable(this_01);
          if (iVar10 != 0) {
            if ((*(ListItem **)(in_r0 + 0x68) == this_01) &&
               (iVar10 = ::ListItem::isTextButton(this_01), iVar10 == 0)) {
              if ((iVar4 == 0) && (-1 < *(int *)(this_01 + 0x3c))) {
                iVar10 = *piVar37;
                iVar11 = *(int *)(in_r0 + 0xf4);
                pSVar16 = aSStack_50;
                iVar35 = *(int *)(iVar10 + 0x28);
                uVar29 = *(undefined4 *)(iVar10 + 0x70);
                AbyssEngine::String::String(pSVar16,(char *)(DAT_0015966c + 0x15946c),false);
                Layout::drawBox(iVar10,10,iVar11 + iVar35,iVar9,iVar17,uVar29,pSVar16);
              }
              else {
                iVar10 = *piVar37;
                iVar11 = *(int *)(in_r0 + 0xf4);
                pSVar16 = aSStack_5c;
                iVar35 = *(int *)(iVar10 + 0x28);
                uVar29 = *(undefined4 *)(iVar10 + 0x70);
                AbyssEngine::String::String(pSVar16,(char *)(DAT_00159444 + 0x1593fc),false);
                Layout::drawBox(iVar10,4,iVar11 + iVar35,iVar9,iVar17,uVar29,pSVar16);
              }
            }
            else {
              if ((iVar4 != 0) || (*(int *)(this_01 + 0x3c) < 0)) {
                iVar10 = *piVar37;
                iVar11 = *(int *)(in_r0 + 0xf4);
                iVar35 = *(int *)(iVar10 + 0x28);
                uVar29 = *(undefined4 *)(iVar10 + 0x70);
                AbyssEngine::String::String(aSStack_74,(char *)(DAT_0015943c + 0x159382),false);
                Layout::drawBox(iVar10,3,iVar11 + iVar35,iVar9,iVar17,uVar29,aSStack_74);
                AbyssEngine::String::~String(aSStack_74);
                goto LAB_0015948e;
              }
              iVar10 = *piVar37;
              iVar11 = *(int *)(in_r0 + 0xf4);
              pSVar16 = aSStack_68;
              iVar35 = *(int *)(iVar10 + 0x28);
              uVar29 = *(undefined4 *)(iVar10 + 0x70);
              AbyssEngine::String::String(pSVar16,(char *)(DAT_00159440 + 0x1593c4),false);
              Layout::drawBox(iVar10,9,iVar11 + iVar35,iVar9,iVar17,uVar29,pSVar16);
            }
            AbyssEngine::String::~String(pSVar16);
          }
LAB_0015948e:
          AbyssEngine::PaintCanvas::SetColor(*puVar19);
          AbyssEngine::String::String(aSStack_80);
          iVar10 = ::ListItem::isItem(this_01);
          cVar1 = (char)iVar9;
          if (iVar10 == 0) {
            iVar10 = ::ListItem::isShip(this_01);
            if (iVar10 != 0) {
              iVar10 = **(int **)(DAT_00159674 + 0x159514);
              Ship::getIndex();
              pSVar12 = (String *)GameText::getText(iVar10);
              AbyssEngine::String::operator=(aSStack_80,pSVar12);
              AbyssEngine::PaintCanvas::SetColor(*puVar19);
              if ((iVar4 == 1) && (in_r0[0x11d] != (HangarWindow)0x0)) {
                pSVar12 = (String *)**(undefined4 **)(DAT_0015967c + 0x159558);
                uVar33 = *puVar19;
                Ship::getPrice(*(Ship **)(this_01 + 0xc));
                Layout::formatCredits((int)aSStack_8c);
                AbyssEngine::PaintCanvas::DrawString
                          (uVar33,pSVar12,(int)aSStack_8c,
                           iVar8 + *(int *)(*piVar37 + 0x28) + *(int *)(in_r0 + 0xf4),
                           (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
              }
              else {
                uVar33 = *puVar19;
                if ((iVar4 == 1) || (1 < uVar22)) {
                  Ship::getPrice(*(Ship **)(this_01 + 0xc));
                  pSVar15 = (Ship *)Status::getShip();
                  Ship::getPrice(pSVar15);
                  Status::getCredits();
                  AbyssEngine::PaintCanvas::SetColor(uVar33);
                  pSVar12 = (String *)**(undefined4 **)(DAT_00159c74 + 0x159a32);
                  uVar33 = *puVar19;
                  Ship::getPrice(*(Ship **)(this_01 + 0xc));
                  pSVar15 = (Ship *)Status::getShip();
                  Ship::getPrice(pSVar15);
                  Layout::formatCredits((int)aSStack_8c);
                  AbyssEngine::PaintCanvas::DrawString
                            (uVar33,pSVar12,(int)aSStack_8c,
                             iVar8 + *(int *)(*piVar37 + 0x28) + *(int *)(in_r0 + 0xf4),
                             (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
                }
                else {
                  pSVar12 = (String *)**(undefined4 **)(DAT_001599ec + 0x159918);
                  Ship::getPrice(*(Ship **)(this_01 + 0xc));
                  Layout::formatCredits((int)aSStack_8c);
                  AbyssEngine::PaintCanvas::DrawString
                            (uVar33,pSVar12,(int)aSStack_8c,
                             iVar8 + *(int *)(*piVar37 + 0x28) + *(int *)(in_r0 + 0xf4),
                             (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
                }
              }
              AbyssEngine::String::~String(aSStack_8c);
              pIVar26 = (ImageFactory *)**(undefined4 **)(DAT_00159c78 + 0x159a9e);
              iVar10 = Ship::getIndex();
              ImageFactory::drawShip
                        (pIVar26,iVar10,*(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + iVar30,
                         *(int *)(in_r0 + 0x118) + iVar9);
              AbyssEngine::PaintCanvas::SetColor(*puVar19);
LAB_00159ad6:
              local_168 = 0;
              goto LAB_00159ada;
            }
            iVar10 = ::ListItem::isSlot(this_01);
            if (iVar10 != 0) {
              pSVar12 = (String *)GameText::getText(**(int **)(DAT_00159680 + 0x1595b6));
              AbyssEngine::String::operator=(aSStack_80,pSVar12);
              if ((iVar4 == 4) && (uVar22 == *puVar5 - 1)) {
                TouchButton::setPosition
                          (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x5c),
                           *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + iVar17 / 2,
                           *(int *)(in_r0 + 0x114) + iVar9,'\x14');
                TouchButton::setVisible
                          (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x5c),true);
                TouchButton::draw();
                AbyssEngine::String::String(aSStack_8c,(char *)(DAT_00159684 + 0x159622),false);
                AbyssEngine::String::operator=(aSStack_80,aSStack_8c);
                AbyssEngine::String::~String(aSStack_8c);
              }
              goto LAB_00159ad6;
            }
            iVar10 = ::ListItem::isBluePrint(this_01);
            if (iVar10 != 0) {
              iVar10 = **(int **)(DAT_001599c8 + 0x15969a);
              BluePrint::getIndex(*(BluePrint **)(this_01 + 8));
              pSVar12 = (String *)GameText::getText(iVar10);
              AbyssEngine::String::operator=(aSStack_80,pSVar12);
              fVar7 = (float)BluePrint::getCompletionRate(*(BluePrint **)(this_01 + 8));
              uVar33 = in_fpscr & 0xfffffff | (uint)(fVar7 < 0.0) << 0x1f |
                       (uint)(fVar7 == 0.0) << 0x1e;
              in_fpscr = uVar33 | (uint)NAN(fVar7) << 0x1c;
              bVar2 = (byte)(uVar33 >> 0x18);
              if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
                fVar7 = (float)BluePrint::getCompletionRate(*(BluePrint **)(this_01 + 8));
                AbyssEngine::PaintCanvas::DrawImage2D
                          (*puVar19,*(int *)(in_r0 + 0x78),
                           *(int *)(*piVar37 + 0x28) + iVar8 + 2 + *(int *)(in_r0 + 0xf4));
                fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0xdc),
                                                    (byte)(in_fpscr >> 0x16) & 3);
                AbyssEngine::PaintCanvas::DrawRegion2D
                          ((PaintCanvas *)*puVar19,*(uint *)(in_r0 + 0x7c),0,0,(int)(fVar7 * fVar47)
                           ,*(int *)(in_r0 + 0xe0),(float)(int)(fVar7 * fVar47),0,0,0,
                           *(int *)(*piVar37 + 0x28) + iVar8 + 3 + *(int *)(in_r0 + 0xf4));
                fVar47 = (float)VectorSignedToFloat(*(int *)(in_r0 + 0xdc) + -4,
                                                    (byte)(in_fpscr >> 0x16) & 3);
                AbyssEngine::PaintCanvas::DrawImage2D
                          (*puVar19,*(int *)(in_r0 + 0x74),
                           *(int *)(in_r0 + 0xf4) +
                           *(int *)(*piVar37 + 0x28) + iVar8 + 5 + (int)(fVar7 * fVar47),
                           cVar1 + '\x02' + (char)(*(int *)(*piVar37 + 0x70) / 2),'\x11');
                AbyssEngine::PaintCanvas::SetColor(*puVar19);
                AbyssEngine::String::String(aSStack_e0,(int)(fVar7 * fVar6));
                AbyssEngine::String::String(aSStack_ec,(char *)(DAT_001599d0 + 0x1597d0),false);
                AbyssEngine::operator+(aSStack_a4,aSStack_e0);
                AbyssEngine::operator+(aSStack_98,aSStack_a4);
                AbyssEngine::String::String(aSStack_f8,(char *)(DAT_001599d4 + 0x1597fc),false);
                AbyssEngine::operator+(aSStack_8c,aSStack_98);
                pcVar25 = *(code **)(DAT_001599d8 + 0x15981c);
                (*pcVar25)(aSStack_f8);
                (*pcVar25)(aSStack_98);
                (*pcVar25)(aSStack_a4);
                (*pcVar25)(aSStack_ec);
                (*pcVar25)(aSStack_e0);
                iVar10 = *piVar37;
                AbyssEngine::PaintCanvas::DrawString
                          (*puVar19,(String *)**(undefined4 **)(DAT_001599dc + 0x159838),
                           (int)aSStack_8c,
                           iVar8 + 2 + *(int *)(iVar10 + 0x28) + *(int *)(in_r0 + 0xf4) +
                           *(int *)(in_r0 + 0xdc) + *(int *)(iVar10 + 0x2c),
                           (bool)(cVar1 + (char)(*(int *)(iVar10 + 0x70) / 2) +
                                  (char)*(undefined4 *)(iVar10 + 0x260) +
                                 (char)*(undefined4 *)(in_r0 + 0x114)));
                AbyssEngine::PaintCanvas::SetColor(*puVar19);
                AbyssEngine::String::~String(aSStack_8c);
              }
              pIVar26 = (ImageFactory *)**(undefined4 **)(DAT_001599e0 + 0x159892);
              iVar10 = BluePrint::getIndex(*(BluePrint **)(this_01 + 8));
              iVar35 = **(int **)(DAT_001599e4 + 0x1598a4);
              iVar11 = BluePrint::getIndex(*(BluePrint **)(this_01 + 8));
              iVar11 = Item::getType(*(Item **)(*(int *)(iVar35 + 4) + iVar11 * 4));
              ImageFactory::drawItem
                        (pIVar26,iVar10,iVar11,
                         *(int *)(*piVar37 + 0x28) + iVar30 + *(int *)(in_r0 + 0xf4),
                         *(int *)(in_r0 + 0x118) + iVar9);
              if (this_01[0x45] != (ListItem)0x0) {
                AbyssEngine::PaintCanvas::SetColor(*puVar19);
              }
              goto LAB_00159ad6;
            }
            iVar10 = ::ListItem::isPendingProduct(this_01);
            if (iVar10 != 0) {
              iVar10 = *(int *)(*(int *)(this_01 + 0x18) + 0x10);
              if (iVar10 < 2) {
                AbyssEngine::String::String(aSStack_98,(char *)(DAT_0015a800 + 0x15a48e),false);
              }
              else {
                AbyssEngine::String::String(aSStack_104,iVar10);
                AbyssEngine::String::String(aSStack_110,(char *)(DAT_001599f0 + 0x1599ae),false);
                AbyssEngine::operator+(aSStack_98,aSStack_104);
              }
              piVar38 = *(int **)(DAT_0015a804 + 0x15a4a4);
              GameText::getText(*piVar38);
              AbyssEngine::operator+(aSStack_8c,aSStack_98);
              AbyssEngine::String::operator=(aSStack_80,aSStack_8c);
              AbyssEngine::String::~String(aSStack_8c);
              AbyssEngine::String::~String(aSStack_98);
              if (iVar10 >= 2) {
                AbyssEngine::String::~String(aSStack_110);
                AbyssEngine::String::~String(aSStack_104);
              }
              AbyssEngine::PaintCanvas::SetColor(*puVar19);
              uVar33 = *puVar19;
              pSVar32 = (String *)**(undefined4 **)(DAT_0015a80c + 0x15a4f6);
              pSVar12 = (String *)GameText::getText(*piVar38);
              AbyssEngine::String::String(aSStack_a4,(char *)(DAT_0015a810 + 0x15a510),false);
              AbyssEngine::operator+(aSStack_98,pSVar12);
              AbyssEngine::operator+(aSStack_8c,aSStack_98);
              AbyssEngine::PaintCanvas::DrawString
                        (uVar33,pSVar32,(int)aSStack_8c,
                         iVar8 + *(int *)(*piVar37 + 0x28) + *(int *)(in_r0 + 0xf4),
                         (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
              pcVar25 = *(code **)(DAT_0015a814 + 0x15a56c);
              (*pcVar25)(aSStack_8c);
              (*pcVar25)(aSStack_98);
              (*pcVar25)(aSStack_a4);
              AbyssEngine::PaintCanvas::SetColor(*puVar19);
              iVar11 = *(int *)(*(int *)(this_01 + 0x18) + 0x14);
              pIVar26 = (ImageFactory *)**(undefined4 **)(DAT_0015a81c + 0x15a590);
              iVar10 = Item::getType(*(Item **)(*(int *)(**(int **)(DAT_0015a818 + 0x15a58c) + 4) +
                                               iVar11 * 4));
              ImageFactory::drawItem
                        (pIVar26,iVar11,iVar10,
                         iVar30 + *(int *)(*piVar37 + 0x28) + *(int *)(in_r0 + 0xf4),
                         *(int *)(in_r0 + 0x118) + iVar9);
              goto LAB_00159ad6;
            }
            iVar10 = ::ListItem::isMoveToCargoButton(this_01);
            if (iVar10 == 0) {
              iVar10 = ::ListItem::isSellButton(this_01);
              if (iVar10 == 0) {
                iVar10 = *piVar37;
                iVar11 = *(int *)(iVar10 + 0x1c);
                iVar39 = *(int *)(in_r0 + 0xf4);
                iVar35 = *(int *)(iVar10 + 0x28);
                iVar36 = *(int *)(iVar10 + 0x70);
                AbyssEngine::String::String(aSStack_11c,*(String **)(this_01 + 0x1c),false);
                Layout::drawBox(iVar10,0,iVar39 + iVar35,(iVar9 + iVar36) - iVar11,iVar17,iVar11,
                                aSStack_11c);
                AbyssEngine::String::~String(aSStack_11c);
              }
              else {
                TouchButton::setPosition
                          (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x14),
                           *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28),iVar9,'\x11');
                TouchButton::setVisible
                          (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x14),true);
                TouchButton::draw();
              }
            }
            else {
              TouchButton::setPosition
                        (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x18),
                         *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28),iVar9,'\x11');
              TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x18),true);
              TouchButton::draw();
            }
          }
          else {
            piVar38 = *(int **)(DAT_00159670 + 0x1594b2);
            iVar10 = *piVar38;
            Item::getIndex(*(Item **)(this_01 + 0x10));
            pSVar12 = (String *)GameText::getText(iVar10);
            AbyssEngine::String::operator=(aSStack_80,pSVar12);
            if (iVar4 == 4 || iVar4 == 1) {
              if (this_01 == *(ListItem **)(in_r0 + 0x68)) {
                iVar10 = HangarList::getCurrentTab(*(HangarList **)(in_r0 + 0x14));
                if ((iVar10 == 1) ||
                   ((iVar10 = HangarList::getCurrentTab(*(HangarList **)(in_r0 + 0x14)), iVar10 == 4
                    && (iVar10 = ::ListItem::isItem(this_01), iVar10 != 0)))) {
                  if (iVar4 == 1) {
                    iVar10 = Item::getStationAmount(*(Item **)(this_01 + 0x10));
                    iVar11 = Item::getAmount();
                  }
                  else {
                    iVar10 = Item::getAmount();
                    pBVar24 = *(BluePrint **)(in_r0 + 0x80);
                    iVar11 = Item::getIndex(*(Item **)(this_01 + 0x10));
                    iVar11 = BluePrint::getCurrentAmount(pBVar24,iVar11);
                  }
                  if (0 < iVar11) {
                    TouchButton::setPosition
                              (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x20),
                               *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28),
                               *(int *)(in_r0 + 0x114) + iVar9,'\x11');
                    TouchButton::setVisible
                              (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x20),true);
                    TouchButton::draw();
                  }
                  if (0 < iVar10) {
                    TouchButton::setPosition
                              (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x24),
                               *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + iVar17,
                               *(int *)(in_r0 + 0x114) + iVar9,'\x12');
                    TouchButton::setVisible
                              (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x24),true);
                    TouchButton::draw();
                  }
                  local_168 = TouchButton::getWidth();
                  local_168 = local_168 + *(int *)(*piVar37 + 0x2c);
                }
                else {
                  local_168 = 0;
                }
                AbyssEngine::PaintCanvas::SetColor(*puVar19);
                puVar18 = *(undefined4 **)(DAT_0015a09c + 0x159d4c);
                pSVar12 = (String *)*puVar18;
                uVar33 = *puVar19;
                if (iVar4 == 1) {
                  iVar10 = GameText::getText(*piVar38);
                  iVar35 = *(int *)(in_r0 + 0xf4);
                  iVar36 = *(int *)(in_r0 + 0x100);
                  iVar39 = *(int *)(*piVar37 + 0x28);
                  pSVar32 = (String *)*puVar18;
                  uVar23 = *puVar19;
                  GameText::getText(*piVar38);
                  iVar11 = AbyssEngine::PaintCanvas::GetTextWidth(uVar23,pSVar32);
                  AbyssEngine::PaintCanvas::DrawString
                            (uVar33,pSVar12,iVar10,(iVar35 + iVar39 + iVar36) - iVar11 / 2,
                             (bool)(cVar1 + '\x02'));
                  pSVar12 = (String *)*puVar18;
                  uVar33 = *puVar19;
                  iVar10 = GameText::getText(*piVar38);
                  iVar39 = *(int *)(in_r0 + 0xf4);
                  iVar35 = *(int *)(in_r0 + 0x100);
                  pSVar32 = (String *)*puVar18;
                  uVar23 = *puVar19;
                  GameText::getText(*piVar38);
                  iVar11 = AbyssEngine::PaintCanvas::GetTextWidth(uVar23,pSVar32);
                  AbyssEngine::PaintCanvas::DrawString
                            (uVar33,pSVar12,iVar10,((iVar17 + iVar39) - iVar35) - iVar11 / 2,
                             (bool)(cVar1 + '\x02'));
                  pIVar13 = *(Item **)(this_01 + 0x10);
                  goto LAB_00159e26;
                }
                iVar10 = GameText::getText(*piVar38);
                iVar40 = *(int *)(*piVar37 + 0x28);
                iVar35 = *(int *)(in_r0 + 0xf4);
                iVar39 = *(int *)(in_r0 + 0x100);
                iVar36 = *(int *)(*piVar37 + 0x254);
                pSVar32 = (String *)*puVar18;
                uVar23 = *puVar19;
                GameText::getText(*piVar38);
                iVar11 = AbyssEngine::PaintCanvas::GetTextWidth(uVar23,pSVar32);
                AbyssEngine::PaintCanvas::DrawString
                          (uVar33,pSVar12,iVar10,((iVar35 + iVar40 + iVar39) - iVar36) - iVar11 / 2,
                           (bool)(cVar1 + '\x02'));
                pSVar12 = (String *)*puVar18;
                uVar33 = *puVar19;
                iVar10 = GameText::getText(*piVar38);
                iVar39 = *(int *)(in_r0 + 0xf4);
                iVar35 = *(int *)(in_r0 + 0x100);
                iVar36 = *(int *)(*piVar37 + 600);
                pSVar32 = (String *)*puVar18;
                uVar23 = *puVar19;
                GameText::getText(*piVar38);
                iVar11 = AbyssEngine::PaintCanvas::GetTextWidth(uVar23,pSVar32);
                AbyssEngine::PaintCanvas::DrawString
                          (uVar33,pSVar12,iVar10,
                           (((iVar17 + iVar39) - iVar35) + iVar36) - iVar11 / 2,
                           (bool)(cVar1 + '\x02'));
LAB_00159f14:
                iVar11 = Item::getAmount();
                pBVar24 = *(BluePrint **)(in_r0 + 0x80);
                iVar10 = Item::getIndex(*(Item **)(this_01 + 0x10));
                iVar35 = BluePrint::getCurrentAmount(pBVar24,iVar10);
                iVar10 = Item::getBlueprintAmount(*(Item **)(this_01 + 0x10));
                iVar10 = iVar10 + iVar35;
              }
              else {
                pIVar13 = *(Item **)(this_01 + 0x10);
                local_168 = 0;
                if (iVar4 != 1) goto LAB_00159f14;
LAB_00159e26:
                iVar11 = Item::getStationAmount(pIVar13);
                iVar10 = Item::getAmount();
              }
              AbyssEngine::String::String(aSStack_98,iVar11);
              AbyssEngine::String::String(aSStack_a4,(char *)(DAT_0015a0a0 + 0x159f52),false);
              AbyssEngine::operator+(aSStack_8c,aSStack_98);
              AbyssEngine::String::~String(aSStack_a4);
              AbyssEngine::String::~String(aSStack_98);
              if (iVar4 != 1) {
                AbyssEngine::String::String(aSStack_bc,iVar10);
                AbyssEngine::String::String(aSStack_c8,(char *)(DAT_0015a0a4 + 0x159f9a),false);
                AbyssEngine::operator+(aSStack_b0,aSStack_bc);
                pBVar24 = *(BluePrint **)(in_r0 + 0x80);
                iVar35 = Item::getIndex(*(Item **)(this_01 + 0x10));
                iVar35 = BluePrint::getTotalAmount(pBVar24,iVar35);
                AbyssEngine::String::String(aSStack_d4,iVar35);
                AbyssEngine::operator+(aSStack_a4,aSStack_b0);
              }
              else {
                AbyssEngine::String::String(aSStack_a4,iVar10);
              }
              AbyssEngine::String::String(aSStack_e0,(char *)(DAT_0015a0a8 + 0x159fe4),false);
              AbyssEngine::operator+(aSStack_98,aSStack_a4);
              AbyssEngine::String::~String(aSStack_e0);
              AbyssEngine::String::~String(aSStack_a4);
              if (iVar4 != 1) {
                pcVar25 = *(code **)(DAT_0015a0ac + 0x15a00c);
                (*pcVar25)(aSStack_d4);
                (*pcVar25)(aSStack_b0);
                (*pcVar25)(aSStack_c8);
                (*pcVar25)(aSStack_bc);
              }
              AbyssEngine::PaintCanvas::SetColor(*puVar19);
              if (0 < iVar11) {
                iVar36 = *(int *)(in_r0 + 0xf4);
                pSVar12 = (String *)**(undefined4 **)(DAT_0015a0b0 + 0x15a034);
                iVar39 = *(int *)(in_r0 + 0x100);
                iVar40 = *(int *)(in_r0 + 0x108);
                uVar33 = *puVar19;
                iVar35 = *(int *)(*piVar37 + 0x28);
                iVar11 = AbyssEngine::PaintCanvas::GetTextWidth(uVar33,pSVar12);
                AbyssEngine::PaintCanvas::DrawString
                          (uVar33,pSVar12,(int)aSStack_8c,
                           (iVar36 + iVar35 + iVar39 + iVar40) - iVar11,
                           (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
              }
              if ((iVar4 == 4) || (0 < iVar10)) {
                if (iVar4 == 4) {
                  iVar10 = *(int *)(*piVar37 + 0x25c);
                }
                else {
                  iVar10 = 0;
                }
                iVar35 = *(int *)(in_r0 + 0xf4);
                iVar39 = *(int *)(in_r0 + 0x100);
                iVar36 = *(int *)(in_r0 + 0x104);
                pSVar12 = (String *)**(undefined4 **)(DAT_0015a454 + 0x15a0c0);
                uVar33 = *puVar19;
                iVar11 = AbyssEngine::PaintCanvas::GetTextWidth(uVar33,pSVar12);
                AbyssEngine::PaintCanvas::DrawString
                          (uVar33,pSVar12,(int)aSStack_98,
                           (((iVar17 + iVar10 + iVar35) - iVar39) + iVar36) - iVar11,
                           (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
              }
              AbyssEngine::String::~String(aSStack_98);
              AbyssEngine::String::~String(aSStack_8c);
            }
            else {
              local_168 = 0;
            }
            uVar33 = *puVar19;
            if (iVar4 == 1) {
              Item::getSinglePrice(*(Item **)(this_01 + 0x10));
              Status::getCredits();
              AbyssEngine::PaintCanvas::SetColor(uVar33);
            }
            else {
              AbyssEngine::PaintCanvas::SetColor(uVar33);
            }
            if (in_r0[0x11d] == (HangarWindow)0x0) {
              pSVar12 = (String *)**(undefined4 **)(DAT_0015a464 + 0x15a160);
              uVar33 = *puVar19;
              Item::getSinglePrice(*(Item **)(this_01 + 0x10));
              Layout::formatCredits((int)aSStack_8c);
              AbyssEngine::PaintCanvas::DrawString
                        (uVar33,pSVar12,(int)aSStack_8c,
                         iVar8 + *(int *)(*piVar37 + 0x28) + *(int *)(in_r0 + 0xf4),
                         (bool)(cVar1 + (char)(*(int *)(*piVar37 + 0x70) / 2) + '\x01'));
              AbyssEngine::String::~String(aSStack_8c);
            }
            pIVar26 = (ImageFactory *)**(undefined4 **)(DAT_0015a468 + 0x15a1ba);
            iVar10 = Item::getIndex(*(Item **)(this_01 + 0x10));
            iVar11 = Item::getType(*(Item **)(this_01 + 0x10));
            ImageFactory::drawItem
                      (pIVar26,iVar10,iVar11,
                       *(int *)(*piVar37 + 0x28) + iVar30 + *(int *)(in_r0 + 0xf4),
                       *(int *)(in_r0 + 0x118) + iVar9);
            if ((iVar4 == 1) &&
               (iVar11 = *(int *)(**(int **)(DAT_0015a46c + 0x15a200) + 0x54),
               iVar10 = Item::getIndex(*(Item **)(this_01 + 0x10)),
               *(char *)(*(int *)(iVar11 + 4) + iVar10) == '\0')) {
              iVar10 = *piVar37;
              AbyssEngine::PaintCanvas::DrawImage2D
                        (*puVar19,*(int *)(in_r0 + 0xec),
                         *(int *)(iVar10 + 0x28) + iVar20 + *(int *)(in_r0 + 0xf4) +
                         *(int *)(iVar10 + 0x2cc),
                         cVar1 + (char)*(undefined4 *)(in_r0 + 0x118) +
                         (char)*(undefined4 *)(iVar10 + 0x2d0) + 0xfe,'\x11');
            }
            else {
              if (*(int *)(this_01 + 0x3c) < 0) {
                if (iVar4 == 1) {
                  iVar10 = Status::getShip();
                  iVar11 = Item::getIndex(*(Item **)(this_01 + 0x10));
                  iVar10 = Ship::hasEquipment(iVar10,iVar11);
                  if (iVar10 != 0) goto LAB_0015a222;
                }
                if (iVar4 == 4) {
                  pIVar13 = *(Item **)(this_01 + 0x10);
                  Status::getStation();
                  pAVar14 = (Array *)Station::getItems();
                  iVar10 = Item::isInList(pIVar13,pAVar14);
                  if (iVar10 != 0) {
                    iVar10 = *piVar37;
                    AbyssEngine::PaintCanvas::DrawImage2D
                              (*puVar19,*(int *)(in_r0 + 0xe8),
                               *(int *)(iVar10 + 0x28) + iVar20 + *(int *)(in_r0 + 0xf4) +
                               *(int *)(iVar10 + 0x2cc),
                               cVar1 + (char)*(undefined4 *)(in_r0 + 0x118) +
                               (char)*(undefined4 *)(iVar10 + 0x2d0) + 0xfe,'\x11');
                  }
                  goto LAB_00159ada;
                }
              }
              else {
LAB_0015a222:
                iVar10 = *piVar37;
                AbyssEngine::PaintCanvas::DrawImage2D
                          (*puVar19,*(int *)(in_r0 + 0xe8),
                           *(int *)(iVar10 + 0x28) + iVar20 + *(int *)(in_r0 + 0xf4) +
                           *(int *)(iVar10 + 0x2cc),
                           cVar1 + (char)*(undefined4 *)(in_r0 + 0x118) +
                           (char)*(undefined4 *)(iVar10 + 0x2d0) + 0xfe,'\x11');
              }
              if ((iVar4 == 3 || iVar4 == 0) && (iVar10 = Item::getAmount(), 1 < iVar10)) {
                AbyssEngine::String::String(aSStack_a4,(char *)(DAT_0015a470 + 0x15a27a),false);
                iVar10 = Item::getAmount();
                AbyssEngine::String::String(aSStack_e0,iVar10);
                AbyssEngine::operator+(aSStack_98,aSStack_a4);
                AbyssEngine::String::String(aSStack_ec,(char *)(DAT_0015a474 + 0x15a2aa),false);
                AbyssEngine::operator+(aSStack_8c,aSStack_98);
                AbyssEngine::String::operator+=(aSStack_80,aSStack_8c);
                pcVar25 = *(code **)(DAT_0015a478 + 0x15a2cc);
                (*pcVar25)(aSStack_8c);
                (*pcVar25)(aSStack_ec);
                (*pcVar25)(aSStack_98);
                (*pcVar25)(aSStack_e0);
                (*pcVar25)(aSStack_a4);
              }
            }
LAB_00159ada:
            AbyssEngine::PaintCanvas::DrawString
                      (*puVar19,(String *)**(undefined4 **)(DAT_00159c7c + 0x159ae2),(int)aSStack_80
                       ,*(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + iVar8,
                       (bool)(cVar1 + '\x02'));
            if (this_01 == *(ListItem **)(in_r0 + 0x68)) {
              iVar10 = ::ListItem::isShip(this_01);
              if (iVar10 == 0) {
                if (iVar4 == 2) {
                  iVar10 = ::ListItem::isBluePrint(this_01);
                  if (iVar10 != 0) {
                    TouchButton::setPosition
                              (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x1c),
                               *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + iVar17,
                               *(int *)(in_r0 + 0x114) + iVar9,'\x12');
                    TouchButton::setVisible
                              (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x1c),true);
                    TouchButton::draw();
                    local_168 = TouchButton::getWidth();
                    local_168 = local_168 + *(int *)(*piVar37 + 0x2c);
                  }
                }
                else if ((iVar4 == 0) && (iVar10 = ::ListItem::isItem(this_01), iVar10 != 0)) {
                  iVar10 = Status::getShip();
                  Item::getSort(*(Item **)(this_01 + 0x10));
                  pIVar13 = (Item *)Ship::getFirstEquipmentOfSort(iVar10);
                  if (pIVar13 == (Item *)0x0) {
                    bVar2 = 0;
LAB_0015a658:
                    bVar48 = false;
                  }
                  else {
                    bVar2 = Item::canBeInstalledMultipleTimes(*(Item **)(this_01 + 0x10));
                    iVar10 = Item::getType(pIVar13);
                    bVar2 = bVar2 ^ 1;
                    if (iVar10 != 3) goto LAB_0015a658;
                    iVar10 = Item::getIndex(pIVar13);
                    iVar11 = Item::getIndex(*(Item **)(this_01 + 0x10));
                    bVar48 = iVar10 == iVar11;
                  }
                  if (*(int *)(this_01 + 0x3c) < 0) {
                    pSVar15 = (Ship *)Status::getShip();
                    iVar10 = Item::getType(*(Item **)(this_01 + 0x10));
                    iVar10 = Ship::getFreeSlots(pSVar15,iVar10);
                    if (((~bVar2 & iVar10 == 0) != 0) ||
                       ((bVar48 &&
                        (iVar10 = Item::canBeInstalledMultipleTimes(*(Item **)(this_01 + 0x10)),
                        iVar10 == 0)))) goto LAB_0015a6d0;
                  }
                  iVar10 = 3 - (*(int *)(this_01 + 0x3c) >> 0x1f);
                  TouchButton::setPosition
                            (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + iVar10 * 4),
                             *(int *)(in_r0 + 0xf4) + iVar17 + *(int *)(*piVar37 + 0x28) +
                             *(int *)(*piVar37 + 0x2c) * -2,*(int *)(in_r0 + 0x114) + iVar9,'\x12');
                  TouchButton::setVisible
                            (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + iVar10 * 4),true);
                  TouchButton::draw();
                  local_168 = TouchButton::getWidth();
                  local_168 = local_168 + *(int *)(*piVar37 + 0x2c) * 2;
                }
              }
              else {
                if (iVar4 != 0) {
                  if ((iVar4 == 3) &&
                     (iVar11 = *(int *)(*(int *)(in_r0 + 0x68) + 0xc), iVar10 = Status::getShip(),
                     iVar11 == iVar10)) goto LAB_0015a6d0;
                  TouchButton::setPosition
                            (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 4),
                             *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + iVar17,
                             *(int *)(in_r0 + 0x114) + iVar9,'\x12');
                  TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 4),true)
                  ;
                  TouchButton::draw();
                  local_168 = TouchButton::getWidth();
                  local_168 = local_168 + *(int *)(*piVar37 + 0x2c);
                }
                if ((iVar4 == 1) && (in_r0[0x11d] != (HangarWindow)0x0)) {
                  TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 8),true)
                  ;
                  TouchButton::setPosition
                            (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x28),
                             *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28),
                             *(int *)(in_r0 + 0x114) + iVar9,'\x11');
                  TouchButton::setVisible
                            (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x28),true);
                  TouchButton::draw();
                }
              }
LAB_0015a6d0:
              iVar10 = ::ListItem::isItem(this_01);
              if ((((iVar10 != 0) || (iVar10 = ::ListItem::isShip(this_01), iVar10 != 0)) ||
                  (iVar10 = ::ListItem::isBluePrint(this_01), iVar10 != 0)) ||
                 (iVar10 = ::ListItem::isPendingProduct(this_01), iVar10 != 0)) {
                TouchButton::setPosition
                          ((TouchButton *)**(undefined4 **)(*(int *)(in_r0 + 0x24) + 4),
                           *(int *)(in_r0 + 0xf4) + *(int *)(*piVar37 + 0x28) + (iVar17 - local_168)
                           ,*(int *)(in_r0 + 0x114) + iVar9,'\x12');
                TouchButton::setVisible((TouchButton *)**(undefined4 **)(*(int *)pHVar31 + 4),true);
                TouchButton::draw();
              }
            }
          }
          AbyssEngine::String::~String(aSStack_80);
        }
      }
      iVar4 = iVar43;
      iVar8 = iVar43;
      if (iVar43 < 1) {
        iVar4 = iVar42 + -1;
        iVar8 = iVar42;
      }
      if (iVar4 < 0 == (iVar43 < 1 && SBORROW4(iVar8,1))) {
        this = (Layout *)*piVar37;
        Layout::drawScrollBar
                  (this,((*piVar21 - *(int *)(this + 0x48)) - *(int *)(this + 0x28)) -
                        *(int *)(in_r0 + 0xf4),*(int *)(this + 0x20) + *(int *)(this + 0xc),
                   *(int *)(in_r0 + 0xd8),iVar42,iVar43);
      }
    }
    iVar4 = *piVar37;
    pSVar12 = (String *)GameText::getText(**(int **)(DAT_0015a820 + 0x15a7dc));
    AbyssEngine::String::String(aSStack_128,pSVar12,false);
    Layout::drawHeader(iVar4,aSStack_128);
    AbyssEngine::String::~String(aSStack_128);
    for (uVar22 = 0; uVar22 < **(uint **)(in_r0 + 4); uVar22 = uVar22 + 1) {
      TouchButton::draw();
    }
  }
  iVar4 = *(int *)(in_r0 + 0x58);
LAB_0015a83c:
  if (iVar4 == 1) {
    *(undefined4 *)(in_r0 + 0x58) = 0;
    render();
    *(undefined4 *)(in_r0 + 0x58) = 1;
    ListItemWindow::draw(*(ListItemWindow **)(in_r0 + 0x18));
  }
  piVar37 = *(int **)(DAT_0015aa7c + 0x15a860);
  Layout::drawFooter((Layout *)*piVar37);
  pHVar31 = in_r0 + 0x24;
  TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x2c),true);
  iVar4 = *(int *)(DAT_0015aa80 + 0x15a884);
  TouchButton::setAlwaysPressed
            (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x2c),*(char *)(iVar4 + 0x4e) == '\0'
            );
  pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x2c);
  Status::getCredits();
  Layout::formatCredits((int)aSStack_80);
  TouchButton::setText(pSVar12);
  AbyssEngine::String::~String(aSStack_80);
  TouchButton::draw();
  if (in_r0[0x3c] != (HangarWindow)0x0) {
    ChoiceWindow::draw();
    fVar6 = DAT_0015ae90;
    if (in_r0[0xae] == (HangarWindow)0x0) {
      if (in_r0[0xb0] != (HangarWindow)0x0) {
        iVar17 = *piVar37;
        iVar30 = 0;
        iVar8 = (*(int **)(in_r0 + 0x20))[1];
        iVar43 = *(int *)(iVar17 + 8);
        iVar42 = *(int *)(iVar17 + 0x2c);
        iVar17 = *(int *)(iVar17 + 0x28) + **(int **)(in_r0 + 0x20);
        for (uVar22 = 0; uVar22 < 5; uVar22 = uVar22 + 1) {
          pTVar34 = *(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + uVar22 * 4 + 0x48);
          if ((((uVar22 == 0) || ((uVar22 == 1 && (*(char *)(iVar4 + 0x49) != '\0')))) ||
              ((uVar22 == 2 && (*(char *)(iVar4 + 0x4a) != '\0')))) ||
             ((uVar22 == 3 && (*(char *)(iVar4 + 0x4b) != '\0')))) {
            bVar48 = false;
          }
          else {
            bVar48 = uVar22 != 4 || *(char *)(iVar4 + 0x4c) == '\0';
          }
          TouchButton::setVisible(pTVar34,bVar48);
          iVar20 = (*(int *)(*piVar37 + 0x34) + *(int *)(*piVar37 + 0x30)) * iVar30 +
                   iVar8 + iVar43 + iVar42 * 2;
          TouchButton::setPosition(pTVar34,iVar17,iVar20);
          TouchButton::draw();
          if (bVar48) {
            iVar9 = TouchButton::getWidth();
            iVar10 = *(int *)(*piVar37 + 0x2c);
            uVar29 = TouchButton::getHeight(pTVar34);
            AbyssEngine::PaintCanvas::SetColor(*puVar19);
            fVar7 = (float)VectorSignedToFloat(uVar29,(byte)(in_fpscr >> 0x16) & 3);
            fVar47 = (float)VectorSignedToFloat(iVar20,(byte)(in_fpscr >> 0x16) & 3);
            puVar18 = *(undefined4 **)(DAT_0015ae98 + 0x15ad68);
            pSVar12 = (String *)*puVar18;
            uVar33 = *puVar19;
            iVar10 = iVar9 + iVar17 + iVar10;
            iVar20 = GameText::getText(**(int **)(DAT_0015ae94 + 0x15ad66));
            bVar48 = SUB41((int)(fVar47 + fVar7 * fVar6),0);
            AbyssEngine::PaintCanvas::DrawString(uVar33,pSVar12,iVar20,iVar10,bVar48);
            AbyssEngine::PaintCanvas::SetColor(*puVar19);
            pSVar12 = (String *)*puVar18;
            uVar33 = *puVar19;
            if (uVar22 == 0) {
              AbyssEngine::String::String(aSStack_80,(char *)(DAT_0015aeb0 + 0x15ae48),false);
              AbyssEngine::PaintCanvas::DrawString
                        (uVar33,pSVar12,(int)aSStack_80,iVar10,
                         (bool)((char)*(undefined4 *)(*piVar37 + 4) + bVar48));
              AbyssEngine::String::~String(aSStack_80);
            }
            else {
              AbyssEngine::String::String(aSStack_98,(char *)(DAT_0015aea0 + 0x15adc8),false);
              Layout::formatCredits((int)aSStack_a4);
              AbyssEngine::operator+(aSStack_8c,aSStack_98);
              AbyssEngine::String::String(aSStack_e0,(char *)(DAT_0015aea8 + 0x15adf0),false);
              AbyssEngine::operator+(aSStack_80,aSStack_8c);
              AbyssEngine::PaintCanvas::DrawString
                        (uVar33,pSVar12,(int)aSStack_80,iVar10,
                         (bool)((char)*(undefined4 *)(*piVar37 + 4) + bVar48));
              pcVar25 = *(code **)(DAT_0015aeac + 0x15ae26);
              (*pcVar25)(aSStack_80);
              (*pcVar25)(aSStack_e0);
              (*pcVar25)(aSStack_8c);
              (*pcVar25)(aSStack_a4);
              (*pcVar25)(aSStack_98);
            }
            iVar30 = iVar30 + 1;
          }
        }
      }
    }
    else {
      iVar8 = AbyssEngine::ApplicationManager::GetApplicationData();
      fVar6 = DAT_0015aa90;
      if (in_r0[0x11f] == (HangarWindow)0x0) {
        iVar17 = *(int *)(*(int *)(in_r0 + 0x20) + 4);
        iVar42 = *(int *)(*piVar37 + 8);
        iVar43 = *(int *)(*piVar37 + 0x2c);
        TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x30),true)
        ;
        pSVar12 = *(String **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x30);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015af0c + 0x15aef6),false);
        TouchButton::setText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x30);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015af40 + 0x15af28),false);
        TouchButton::setSplitText(pSVar12);
        iVar17 = iVar17 + iVar42 + iVar43 * 5;
        AbyssEngine::String::~String(aSStack_80);
        piVar21 = *(int **)(DAT_0015afb4 + 0x15af64);
        TouchButton::setPosition
                  (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x30),*piVar21 / 2,iVar17,
                   '\x14');
        TouchButton::draw();
        TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x34),true);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x34);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015afb8 + 0x15af9c),false);
        TouchButton::setText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x34);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015afe8 + 0x15afd0),false);
        TouchButton::setSplitText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        TouchButton::setPosition
                  (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x34),*piVar21 / 2,
                   *(int *)(*piVar37 + 0x30) + iVar17 + *(int *)(*piVar37 + 0x34),'\x14');
        TouchButton::draw();
        TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x38),true);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x38);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015b3cc + 0x15b03c),false);
        TouchButton::setText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x38);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015b3d0 + 0x15b064),false);
        TouchButton::setSplitText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        TouchButton::setPosition
                  (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x38),*piVar21 / 2,
                   iVar17 + (*(int *)(*piVar37 + 0x34) + *(int *)(*piVar37 + 0x30)) * 2,'\x14');
        TouchButton::draw();
        TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x3c),true);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x3c);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015b3d4 + 0x15b0ca),false);
        TouchButton::setText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x3c);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015b3d8 + 0x15b0f2),false);
        TouchButton::setSplitText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        TouchButton::setPosition
                  (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x3c),*piVar21 / 2,
                   (*(int *)(*piVar37 + 0x34) + *(int *)(*piVar37 + 0x30)) * 3 + iVar17,'\x14');
        TouchButton::draw();
        TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x40),true);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x40);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015b3dc + 0x15b15c),false);
        TouchButton::setText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x40);
        AbyssEngine::String::String
                  (aSStack_80,(char *)**(undefined4 **)(DAT_0015b3e0 + 0x15b184),false);
        TouchButton::setSplitText(pSVar12);
        AbyssEngine::String::~String(aSStack_80);
        TouchButton::setPosition
                  (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + 0x40),*piVar21 / 2,
                   iVar17 + (*(int *)(*piVar37 + 0x34) + *(int *)(*piVar37 + 0x30)) * 4,'\x14');
        TouchButton::draw();
        uVar22 = 5;
      }
      else {
        pcVar27 = (char *)(DAT_0015aa88 + 0x15a918);
        uVar22 = 0;
        piVar21 = *(int **)(DAT_0015aa8c + 0x15a91c);
        piVar41 = *(int **)(DAT_0015aa94 + 0x15a936);
        while( true ) {
          if (4 < uVar22) break;
          AbyssEngine::String::String(aSStack_80,pcVar27,false);
          AbyssEngine::String::String(aSStack_8c,pcVar27,false);
          TouchButton::setVisible
                    (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + uVar22 * 4 + 0x30),true);
          switch(uVar22 & 0x7fffffff) {
          case 0:
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aa98 + 0x15a978),false);
            AbyssEngine::String::operator=(aSStack_80,aSStack_98);
            AbyssEngine::String::~String(aSStack_98);
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aa9c + 0x15a996),false);
            AbyssEngine::String::operator=(aSStack_8c,aSStack_98);
            break;
          case 1:
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aaa0 + 0x15a9da),false);
            AbyssEngine::String::operator=(aSStack_80,aSStack_98);
            AbyssEngine::String::~String(aSStack_98);
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aaa4 + 0x15a9f8),false);
            AbyssEngine::String::operator=(aSStack_8c,aSStack_98);
            break;
          case 2:
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aaa8 + 0x15aa12),false);
            AbyssEngine::String::operator=(aSStack_80,aSStack_98);
            AbyssEngine::String::~String(aSStack_98);
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aaac + 0x15aa30),false);
            AbyssEngine::String::operator=(aSStack_8c,aSStack_98);
            break;
          case 3:
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aab0 + 0x15aa4a),false);
            AbyssEngine::String::operator=(aSStack_80,aSStack_98);
            AbyssEngine::String::~String(aSStack_98);
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015aab4 + 0x15aa68),false);
            AbyssEngine::String::operator=(aSStack_8c,aSStack_98);
            break;
          case 4:
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015ae80 + 0x15aabe),false);
            AbyssEngine::String::operator=(aSStack_80,aSStack_98);
            AbyssEngine::String::~String(aSStack_98);
            AbyssEngine::String::String
                      (aSStack_98,(char *)**(undefined4 **)(DAT_0015ae84 + 0x15aadc),false);
            AbyssEngine::String::operator=(aSStack_8c,aSStack_98);
            break;
          default:
            AbyssEngine::String::String(aSStack_98,pcVar27,false);
            AbyssEngine::String::operator=(aSStack_80,aSStack_98);
            pSVar16 = (String *)AbyssEngine::String::~String(aSStack_98);
            AbyssEngine::String::String(pSVar16,pcVar27,false);
            AbyssEngine::String::operator=(aSStack_8c,aSStack_98);
          }
          AbyssEngine::String::~String(aSStack_98);
          iVar17 = __aeabi_uidiv(uVar22 & 0xff,3);
          fVar7 = (float)VectorSignedToFloat(*(int *)(in_r0 + 300),(byte)(in_fpscr >> 0x16) & 3);
          fVar47 = (float)VectorSignedToFloat(*(int *)(*piVar37 + 0x20) * -3 +
                                              (*piVar21 / 2 - *(int *)(in_r0 + 0x124) / 2),
                                              (byte)(in_fpscr >> 0x16) & 3);
          fVar44 = (float)VectorSignedToFloat(iVar17 * (*(int *)(in_r0 + 300) +
                                                       *(int *)(in_r0 + 0x124)),
                                              (byte)(in_fpscr >> 0x16) & 3);
          iVar17 = (*(int *)(in_r0 + 0x128) + *(int *)(in_r0 + 0x120)) *
                   (uVar22 + iVar17 * -3 & 0xff) +
                   ((*piVar41 / 2 - *(int *)(in_r0 + 0x120)) - *(int *)(in_r0 + 0x128));
          iVar42 = (int)(fVar47 + fVar7 * -0.5 + fVar44);
          TouchButton::setPosition
                    (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + uVar22 * 4 + 0x30),
                     iVar17,iVar42,'D');
          TouchButton::replaceTextKeepSize
                    (*(TouchButton **)(*(int *)(*(int *)pHVar31 + 4) + uVar22 * 4 + 0x30),aSStack_80
                    );
          TouchButton::setSplitText(*(String **)(*(int *)(*(int *)pHVar31 + 4) + uVar22 * 4 + 0x30))
          ;
          TouchButton::draw();
          AbyssEngine::PaintCanvas::DrawImage2D
                    (*puVar19,*(int *)(*(int *)(in_r0 + 0x30) + uVar22 * 4),iVar17,
                     (char)iVar42 - (char)*(undefined4 *)(*piVar37 + 0x2c),'\x11');
          if ((uVar22 == 4) && (**(char **)(DAT_0015ae88 + 0x15abee) != '\0')) {
            fVar44 = (float)VectorSignedToFloat(iVar17,(byte)(in_fpscr >> 0x16) & 3);
            fVar45 = (float)VectorSignedToFloat(iVar42,(byte)(in_fpscr >> 0x16) & 3);
            fVar47 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x124),
                                                (byte)(in_fpscr >> 0x16) & 3);
            fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x120),
                                               (byte)(in_fpscr >> 0x16) & 3);
            pfVar28 = (float *)&DAT_0015b6a8;
            if (**(char **)(DAT_0015ae8c + 0x15abfe) == '\0') {
              pfVar28 = (float *)&DAT_0015b6ac;
            }
            AbyssEngine::PaintCanvas::DrawImage2D
                      (*puVar19,*(int *)(in_r0 + 0x34),(int)(fVar44 + *pfVar28 * fVar7),
                       (uchar)(int)(fVar45 + fVar47 * fVar6),'\x11');
          }
          AbyssEngine::String::~String(aSStack_8c);
          AbyssEngine::String::~String(aSStack_80);
          uVar22 = uVar22 + 1;
        }
      }
      if ((((*(char *)(iVar4 + 0x4b) == '\0') || (*(char *)(iVar4 + 0x4a) == '\0')) ||
          (*(char *)(iVar4 + 0x49) == '\0')) ||
         (((*(char *)(iVar4 + 0x4d) == '\0' || (*(char *)(iVar4 + 0x4c) == '\0')) ||
          (*(char *)(iVar8 + 0x15) != '\0')))) {
        iVar17 = *(int *)(in_r0 + 0x120);
        iVar20 = *(int *)(in_r0 + 0x124);
        iVar4 = *(int *)(in_r0 + 0x128);
        iVar43 = *(int *)(in_r0 + 300);
        piVar21 = *(int **)(DAT_0015b3e4 + 0x15b208);
        piVar41 = *(int **)(DAT_0015b3e8 + 0x15b20e);
        iVar8 = *piVar21;
        iVar42 = *piVar41;
        iVar30 = *(int *)(*piVar37 + 0x20);
        TouchButton::setVisible(*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x44),true)
        ;
        fVar6 = (float)VectorSignedToFloat(iVar43,(byte)(in_fpscr >> 0x16) & 3);
        fVar7 = (float)VectorSignedToFloat((iVar42 / 2 - iVar20 / 2) + iVar30 * -3,
                                           (byte)(in_fpscr >> 0x16) & 3);
        fVar47 = (float)VectorSignedToFloat(iVar43 + iVar20,(byte)(in_fpscr >> 0x16) & 3);
        iVar42 = (int)(fVar7 + fVar6 * -0.5 + fVar47);
        pTVar34 = *(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x44);
        if (in_r0[0x11f] == (HangarWindow)0x0) {
          GameText::getText(**(int **)(DAT_0015b6b0 + 0x15b400));
          TouchButton::setText(pTVar34);
          TouchButton::setPosition
                    (*(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x44),*piVar21 / 2,
                     (*(int *)(*piVar37 + 0x30) + *(int *)(*piVar37 + 0x34)) * uVar22 +
                     *(int *)(in_r0 + 0x48),'\x14');
          TouchButton::draw();
        }
        else {
          iVar4 = iVar8 / 2 + iVar17 + iVar4;
          TouchButton::setPosition(pTVar34,iVar4,iVar42,'D');
          pTVar34 = *(TouchButton **)(*(int *)(*(int *)(in_r0 + 0x24) + 4) + 0x44);
          pSVar12 = (String *)GameText::getText(**(int **)(DAT_0015b3ec + 0x15b2b8));
          TouchButton::replaceTextKeepSize(pTVar34,pSVar12);
          TouchButton::draw();
          AbyssEngine::PaintCanvas::DrawImage2D
                    (*puVar19,*(int *)(*(int *)(in_r0 + 0x30) + 0x14),iVar4,
                     (char)iVar42 - (char)*(undefined4 *)(*piVar37 + 0x2c),'\x11');
          iVar4 = *(int *)(in_r0 + 0x124);
          pfVar28 = (float *)&DAT_0015b6a8;
          fVar6 = (float)VectorSignedToFloat(*(int *)(in_r0 + 300),(byte)(in_fpscr >> 0x16) & 3);
          fVar7 = (float)VectorSignedToFloat((*piVar41 / 2 - iVar4 / 2) +
                                             *(int *)(*piVar37 + 0x20) * -3,
                                             (byte)(in_fpscr >> 0x16) & 3);
          fVar44 = (float)VectorSignedToFloat(*(int *)(in_r0 + 300) + iVar4,
                                              (byte)(in_fpscr >> 0x16) & 3);
          fVar47 = (float)VectorSignedToFloat(iVar4,(byte)(in_fpscr >> 0x16) & 3);
          fVar45 = (float)VectorSignedToFloat(*(int *)(in_r0 + 0x120),(byte)(in_fpscr >> 0x16) & 3);
          fVar46 = (float)VectorSignedToFloat(*piVar21 / 2 + *(int *)(in_r0 + 0x120) +
                                              *(int *)(in_r0 + 0x128),(byte)(in_fpscr >> 0x16) & 3);
          fVar6 = (float)VectorSignedToFloat((int)(fVar7 + fVar6 * -0.5 + fVar44),
                                             (byte)(in_fpscr >> 0x16) & 3);
          if (**(char **)(DAT_0015b3f4 + 0x15b35c) == '\0') {
            pfVar28 = (float *)&DAT_0015b6ac;
          }
          AbyssEngine::PaintCanvas::DrawImage2D
                    (*puVar19,*(int *)(in_r0 + 0x38),(int)(fVar46 + *pfVar28 * fVar45),
                     (uchar)(int)(fVar6 + fVar47 * DAT_0015b3f0),'\x11');
        }
        pSVar12 = *(String **)(*(int *)(*(int *)pHVar31 + 4) + 0x44);
        GameText::getText(**(int **)(DAT_0015b6b4 + 0x15b450));
        TouchButton::setSplitText(pSVar12);
        uVar22 = uVar22 + 1;
      }
      if (in_r0[0x11f] == (HangarWindow)0x0) {
        iVar17 = *(int *)(*piVar37 + 0x30);
        iVar4 = *(int *)(*piVar37 + 0x34);
        this_00 = *(ChoiceWindow **)(in_r0 + 0x20);
        iVar8 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar19);
        ChoiceWindow::setHeight(this_00,(iVar4 + iVar17) * (uVar22 + 1) + iVar8 * 2);
      }
    }
  }
  if (*piVar3 == local_44) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001590d8: push {r4,r5,r6,r7,lr}
  001590da: add r7,sp,#0xc
  001590dc: push {r7,r8,r9,r10,r11}
  001590e0: vpush {d8,d9,d10}
  001590e4: sub sp,#0x178
  001590e6: mov r11,r0
  001590e8: ldr r0,[0x00159414]
  001590ea: ldr r1,[0x00159418]
  001590ec: add r0,pc
  001590ee: add r1,pc
  001590f0: ldr r0,[r0,#0x0]
  001590f2: ldr r2,[r1,#0x0]
  001590f4: str r2,[sp,#0x8c]
  001590f6: str r0,[sp,#0x70]
  001590f8: ldr r1,[r0,#0x0]
  001590fa: ldr r0,[r2,#0x0]
  001590fc: str r1,[sp,#0x174]
  001590fe: mov.w r1,#0xffffffff
  00159102: blx 0x0006fac0
  00159106: ldr r0,[0x0015941c]
  00159108: movs r1,#0x5
  0015910a: add r0,pc
  0015910c: ldr.w r8,[r0,#0x0]
  00159110: ldr.w r0,[r8,#0x0]
  00159114: blx 0x00075bd4
  00159118: cmp r0,#0x0
  0015911a: str.w r11,[sp,#0x88]
  0015911e: itt ne
  00159120: ldrb.ne r0,[r0,#0x18]
  00159122: cmp.ne r0,#0x0
  00159124: bne 0x001591cc
  00159126: mov r6,r11
  00159128: ldr.w r0,[r6,#0x58]!
  0015912c: cmp r0,#0x0
  0015912e: bne.w 0x0015a83c
  00159132: ldr r0,[0x00159420]
  00159134: add r0,pc
  00159136: ldr.w r10,[r0,#0x0]
  0015913a: ldr.w r0,[r10,#0x0]
  0015913e: blx 0x00071d40
  00159142: ldr.w r0,[r11,#0x14]
  00159146: blx 0x00076f48
  0015914a: mov r5,r0
  0015914c: ldr.w r0,[r11,#0x14]
  00159150: blx 0x00076f3c
  00159154: cmp r0,#0x0
  00159156: beq.w 0x0015a7d0
  0015915a: str r0,[sp,#0x64]
  0015915c: mov r0,r11
  0015915e: blx 0x00076fe4
  00159162: vldr.32 s0,[r11,#0xd8]
  00159166: mov r4,r0
  00159168: mov r0,r11
  0015916a: vcvt.f32.s32 s16,s0
  0015916e: blx 0x00076ff0
  00159172: vmov s0,r0
  00159176: ldr r0,[0x00159424]
  00159178: vmov s2,r4
  0015917c: vmul.f32 s0,s0,s16
  00159180: add r0,pc
  00159182: vmul.f32 s2,s2,s16
  00159186: ldr r4,[r0,#0x0]
  00159188: ldr.w r0,[r10,#0x0]
  0015918c: vcvt.s32.f32 s4,s0
  00159190: ldr r1,[r4,#0x0]
  00159192: vcvt.s32.f32 s0,s2
  00159196: vmov r2,s4
  0015919a: str r2,[sp,#0x7c]
  0015919c: cmp r2,#0x1
  0015919e: itttt ge
  001591a0: ldr.ge r2,[r0,#0x2c]
  001591a2: ldr.ge r3,[r0,#0x48]
  001591a4: sub.ge r1,r1,r3
  001591a6: sub.ge r1,r1,r2
  001591a8: ldr r2,[r0,#0x28]
  001591aa: cmp r5,#0x0
  001591ac: ldr.w r3,[r11,#0xf4]
  001591b0: str.w r10,[sp,#0x84]
  001591b4: add r2,r3
  001591b6: str r5,[sp,#0x6c]
  001591b8: strd r6,r8,[sp,#0x40]
  001591bc: str r4,[sp,#0x38]
  001591be: beq 0x001591d4
  001591c0: cmp r5,#0x4
  001591c2: it ne
  001591c4: cmp.ne r5,#0x1
  001591c6: bne 0x001591da
  001591c8: ldr r6,[r0,#0x50]
  001591ca: b 0x001591dc
  001591cc: add.w r6,r11,#0x58
  001591d0: b.w 0x0015a83a
  001591d4: ldr r3,[r0,#0x4c]
  001591d6: lsls r6,r3,#0x1
  001591d8: b 0x001591dc
  001591da: movs r6,#0x0
  001591dc: vmov r3,s0
  001591e0: sub.w r1,r1,r2, lsl #0x1
  001591e4: str r1,[sp,#0x80]
  001591e6: str r3,[sp,#0x48]
  001591e8: ldr r1,[0x00159428]
  001591ea: ldr.w r2,[r0,#0x2cc]
  001591ee: add r1,pc
  001591f0: ldr r0,[r0,#0x4c]
  001591f2: str r0,[sp,#0x68]
  001591f4: adds r0,r2,r6
  001591f6: ldr r1,[r1,#0x0]
  001591f8: str r0,[sp,#0x5c]
  001591fa: str r6,[sp,#0x60]
  001591fc: ldrb r0,[r1,#0x0]
  001591fe: cmp r0,#0x0
  00159200: beq 0x0015929e
  00159202: ldr r0,[0x0015942c]
  00159204: add r0,pc
  00159206: ldr r0,[r0,#0x0]
  00159208: ldrb r0,[r0,#0x0]
  0015920a: cmp r0,#0x0
  0015920c: bne 0x0015929e
  0015920e: ldr r5,[sp,#0x88]
  00159210: ldr r4,[sp,#0x8c]
  00159212: ldr.w r1,[r5,#0xf0]
  00159216: ldr r0,[r4,#0x0]
  00159218: blx 0x00072d84
  0015921c: str r0,[sp,#0x78]
  0015921e: ldr r0,[r4,#0x0]
  00159220: ldr.w r1,[r5,#0xf0]
  00159224: blx 0x00072d90
  00159228: mov r1,r0
  0015922a: ldr r0,[0x00159430]
  0015922c: str r1,[sp,#0x74]
  0015922e: add r0,pc
  00159230: ldr r0,[r0,#0x0]
  00159232: ldr r0,[r0,#0x0]
  00159234: blx 0x0007198c
  00159238: ldr.w r8,[sp,#0x88]
  0015923c: mov r10,r0
  0015923e: mov.w r9,#0x0
  00159242: mov.w r11,#0x0
  00159246: b 0x0015929a
  00159248: ldr r5,[sp,#0x84]
  0015924a: movs r6,#0x1
  0015924c: ldr r2,[r5,#0x0]
  0015924e: ldrd r1,r3,[r8,#0xf0]
  00159252: ldr r4,[sp,#0x8c]
  00159254: ldr r2,[r2,#0x28]
  00159256: str r6,[sp,#0x0]
  00159258: ldr r6,[sp,#0x78]
  0015925a: ldr r0,[r4,#0x0]
  0015925c: subs r2,r2,r6
  0015925e: add r2,r3
  00159260: mov r3,r9
  00159262: blx 0x00074e3c
  00159266: ldr r5,[r5,#0x0]
  00159268: ldrd r1,r2,[r8,#0xf0]
  0015926c: ldr r0,[r4,#0x0]
  0015926e: ldr r4,[sp,#0x7c]
  00159270: ldr r3,[r5,#0x28]
  00159272: cmp r4,#0x1
  00159274: blt 0x0015927e
  00159276: ldr r4,[r5,#0x2c]
  00159278: ldr r5,[r5,#0x48]
  0015927a: add r5,r4
  0015927c: b 0x00159280
  0015927e: movs r5,#0x0
  00159280: ldr r4,[sp,#0x80]
  00159282: movs r6,#0x0
  00159284: str r6,[sp,#0x0]
  00159286: add r3,r4
  00159288: add r2,r3
  0015928a: mov r3,r9
  0015928c: add r2,r5
  0015928e: blx 0x00074e3c
  00159292: ldr r0,[sp,#0x74]
  00159294: add.w r11,r11,#0x1
  00159298: add r9,r0
  0015929a: cmp r11,r10
  0015929c: ble 0x00159248
  0015929e: ldr.w r11,[sp,#0x88]
  001592a2: movs r4,#0x0
  001592a4: ldr.w r8,[sp,#0x8c]
  001592a8: ldr.w r10,[sp,#0x84]
  001592ac: add.w r6,r11,#0x24
  001592b0: ldr r5,[sp,#0x60]
  001592b2: ldr r0,[sp,#0x68]
  001592b4: ldr r1,[sp,#0x5c]
  001592b6: add r0,r1
  001592b8: str r0,[sp,#0x68]
  001592ba: b 0x001592d4
  001592bc: ldrb.w r0,[r11,#0xd0]
  001592c0: cbnz r0,0x001592d2
  001592c2: ldr r0,[r6,#0x0]
  001592c4: ldr r0,[r0,#0x4]
  001592c6: ldr.w r0,[r0,r4,lsl #0x2]
  001592ca: cbz r0,0x001592d2
  001592cc: movs r1,#0x0
  001592ce: blx 0x00074e48
  001592d2: adds r4,#0x1
  001592d4: cmp r4,#0x18
  001592d6: bne 0x001592bc
  001592d8: ldr r0,[sp,#0x68]
  001592da: movs r4,#0x0
  001592dc: str r6,[sp,#0x58]
  001592de: adds r1,r0,#0x5
  001592e0: str r1,[sp,#0x2c]
  001592e2: adds r1,r0,#0x3
  001592e4: adds r0,#0x2
  001592e6: str r0,[sp,#0x3c]
  001592e8: subs r0,r5,#0x2
  001592ea: str r0,[sp,#0x4c]
  001592ec: ldr r0,[sp,#0x80]
  001592ee: str r1,[sp,#0x30]
  001592f0: vldr.32 s16,[pc,#0x140]
  001592f4: add.w r0,r0,r0, lsr #0x1f
  001592f8: asrs r0,r0,#0x1
  001592fa: str r0,[sp,#0x34]
  001592fc: ldr r0,[0x00159438]
  001592fe: ldr r5,[sp,#0x64]
  00159300: add r0,pc
  00159302: ldr.w r12,[r0,#0x0]
  00159306: str.w r12,[sp,#0x54]
  0015930a: ldr r0,[r5,#0x0]
  0015930c: cmp r4,r0
  0015930e: bcs.w 0x0015a78c
  00159312: ldr.w r0,[r10,#0x0]
  00159316: ldr.w r2,[r11,#0x10c]
  0015931a: ldr.w r1,[r11,#0xb4]
  0015931e: ldr r3,[r0,#0xc]
  00159320: ldr r6,[r0,#0x20]
  00159322: ldr r0,[r0,#0x70]
  00159324: add r0,r2
  00159326: adds r2,r6,r3
  00159328: add r1,r2
  0015932a: mla r6,r0,r4,r1
  0015932e: cmp r6,#0x0
  00159330: blt.w 0x0015a74a
  00159334: ldr.w r0,[r12,#0x0]
  00159338: cmp r6,r0
  0015933a: bgt.w 0x0015a74a
  0015933e: ldr r0,[r5,#0x4]
  00159340: str r4,[sp,#0x5c]
  00159342: ldr.w r9,[r0,r4,lsl #0x2]
  00159346: mov r0,r9
  00159348: blx 0x00076ffc
  0015934c: cmp r0,#0x0
  0015934e: str r6,[sp,#0x74]
  00159350: beq.w 0x0015948e
  00159354: ldr.w r0,[r11,#0x68]
  00159358: cmp r0,r9
  0015935a: bne 0x00159364
  0015935c: mov r0,r9
  0015935e: blx 0x00077008
  00159362: cbz r0,0x001593dc
  00159364: ldr r0,[sp,#0x6c]
  00159366: cbz r0,0x001593a0
  00159368: ldr.w r4,[r10,#0x0]
  0015936c: add r6,sp,#0x144
  0015936e: ldr.w r8,[r11,#0xf4]
  00159372: movs r2,#0x0
  00159374: mov r0,r6
  00159376: ldr.w r10,[r4,#0x28]
  0015937a: ldr r5,[r4,#0x70]
  0015937c: ldr r1,[0x0015943c]
  0015937e: add r1,pc
  00159380: blx 0x0006ee18
  00159384: ldr r0,[sp,#0x80]
  00159386: add.w r2,r8,r10
  0015938a: movs r1,#0x3
  0015938c: stm sp,{r0,r5,r6}
  00159390: mov r0,r4
  00159392: ldr r3,[sp,#0x74]
  00159394: blx 0x0007462c
  00159398: mov r0,r6
  0015939a: blx 0x0006ee30
  0015939e: b 0x0015948a
  001593a0: ldr.w r0,[r9,#0x3c]
  001593a4: cmp r0,#0x0
  001593a6: blt 0x00159368
  001593a8: ldr.w r4,[r10,#0x0]
  001593ac: mov r8,r9
  001593ae: ldr.w r9,[r11,#0xf4]
  001593b2: add r6,sp,#0x150
  001593b4: movs r2,#0x0
  001593b6: ldr.w r10,[r4,#0x28]
  001593ba: mov r0,r6
  001593bc: ldr r5,[r4,#0x70]
  001593be: ldr r1,[0x00159440]
  001593c0: add r1,pc
  001593c2: blx 0x0006ee18
  001593c6: ldr r0,[sp,#0x80]
  001593c8: add.w r2,r9,r10
  001593cc: movs r1,#0x9
  001593ce: stm sp,{r0,r5,r6}
  001593d2: mov r0,r4
  001593d4: ldr r3,[sp,#0x74]
  001593d6: blx 0x0007462c
  001593da: b 0x00159482
  001593dc: ldr r0,[sp,#0x6c]
  001593de: cbz r0,0x00159448
  001593e0: ldr.w r4,[r10,#0x0]
  001593e4: mov r8,r9
  001593e6: ldr.w r9,[r11,#0xf4]
  001593ea: add r6,sp,#0x15c
  001593ec: movs r2,#0x0
  001593ee: ldr.w r10,[r4,#0x28]
  001593f2: mov r0,r6
  001593f4: ldr r5,[r4,#0x70]
  001593f6: ldr r1,[0x00159444]
  001593f8: add r1,pc
  001593fa: blx 0x0006ee18
  001593fe: ldr r0,[sp,#0x80]
  00159400: add.w r2,r9,r10
  00159404: movs r1,#0x4
  00159406: stm sp,{r0,r5,r6}
  0015940a: mov r0,r4
  0015940c: ldr r3,[sp,#0x74]
  0015940e: blx 0x0007462c
  00159412: b 0x00159482
  00159448: ldr.w r0,[r9,#0x3c]
  0015944c: cmp r0,#0x0
  0015944e: blt 0x001593e0
  00159450: ldr.w r4,[r10,#0x0]
  00159454: mov r8,r9
  00159456: ldr.w r9,[r11,#0xf4]
  0015945a: add r6,sp,#0x168
  0015945c: movs r2,#0x0
  0015945e: ldr.w r10,[r4,#0x28]
  00159462: mov r0,r6
  00159464: ldr r5,[r4,#0x70]
  00159466: ldr r1,[0x0015966c]
  00159468: add r1,pc
  0015946a: blx 0x0006ee18
  0015946e: ldr r0,[sp,#0x80]
  00159470: add.w r2,r9,r10
  00159474: movs r1,#0xa
  00159476: stm sp,{r0,r5,r6}
  0015947a: mov r0,r4
  0015947c: ldr r3,[sp,#0x74]
  0015947e: blx 0x0007462c
  00159482: mov r0,r6
  00159484: blx 0x0006ee30
  00159488: mov r9,r8
  0015948a: ldr.w r8,[sp,#0x8c]
  0015948e: ldr.w r0,[r8,#0x0]
  00159492: mov.w r1,#0xffffffff
  00159496: blx 0x0006fac0
  0015949a: add r0,sp,#0x138
  0015949c: blx 0x0006efbc
  001594a0: mov r0,r9
  001594a2: str.w r9,[sp,#0x78]
  001594a6: blx 0x000768e8
  001594aa: cbz r0,0x00159504
  001594ac: ldr r0,[0x00159670]
  001594ae: add r0,pc
  001594b0: ldr.w r10,[r0,#0x0]
  001594b4: ldr r0,[sp,#0x78]
  001594b6: ldr.w r4,[r10,#0x0]
  001594ba: ldr r0,[r0,#0x10]
  001594bc: blx 0x000718d8
  001594c0: addw r1,r0,#0x4fa
  001594c4: mov r0,r4
  001594c6: blx 0x00072f70
  001594ca: mov r1,r0
  001594cc: add r0,sp,#0x138
  001594ce: blx 0x0006f2b0
  001594d2: ldr r6,[sp,#0x6c]
  001594d4: cmp r6,#0x4
  001594d6: it ne
  001594d8: cmp.ne r6,#0x1
  001594da: bne.w 0x001598f6
  001594de: ldr.w r8,[sp,#0x88]
  001594e2: ldr r4,[sp,#0x78]
  001594e4: ldr.w r9,[sp,#0x8c]
  001594e8: ldr.w r0,[r8,#0x68]
  001594ec: cmp r4,r0
  001594ee: beq.w 0x0015963a
  001594f2: movs r1,#0x0
  001594f4: ldr r0,[r4,#0x10]
  001594f6: str r1,[sp,#0x50]
  001594f8: ldr r1,[sp,#0x6c]
  001594fa: cmp r1,#0x1
  001594fc: beq.w 0x00159e26
  00159500: b.w 0x00159f14
  00159504: ldr r0,[sp,#0x78]
  00159506: blx 0x00076204
  0015950a: cmp r0,#0x0
  0015950c: beq 0x001595a6
  0015950e: ldr r0,[0x00159674]
  00159510: add r0,pc
  00159512: ldr r0,[r0,#0x0]
  00159514: ldr r4,[r0,#0x0]
  00159516: ldr r0,[sp,#0x78]
  00159518: ldr r0,[r0,#0xc]
  0015951a: blx 0x000719c8
  0015951e: addw r1,r0,#0x391
  00159522: mov r0,r4
  00159524: blx 0x00072f70
  00159528: mov r1,r0
  0015952a: add r0,sp,#0x138
  0015952c: blx 0x0006f2b0
  00159530: ldr r0,[sp,#0x8c]
  00159532: ldr r0,[r0,#0x0]
  00159534: ldr r1,[0x00159678]
  00159536: blx 0x0006fac0
  0015953a: ldr r0,[sp,#0x6c]
  0015953c: ldr.w r9,[sp,#0x88]
  00159540: cmp r0,#0x1
  00159542: bne.w 0x001598fe
  00159546: ldrb.w r0,[r9,#0x11d]
  0015954a: cmp r0,#0x0
  0015954c: beq.w 0x001598fe
  00159550: ldr r0,[0x0015967c]
  00159552: ldr r4,[sp,#0x84]
  00159554: add r0,pc
  00159556: ldr r0,[r0,#0x0]
  00159558: ldr r6,[r4,#0x0]
  0015955a: ldr.w r8,[r0,#0x0]
  0015955e: ldr r0,[sp,#0x8c]
  00159560: ldr r5,[r0,#0x0]
  00159562: ldr r0,[sp,#0x78]
  00159564: ldr r0,[r0,#0xc]
  00159566: blx 0x00071b00
  0015956a: add.w r10,sp,#0x12c
  0015956e: mov r2,r0
  00159570: mov r0,r10
  00159572: mov r1,r6
  00159574: blx 0x00074e54
  00159578: ldr r0,[r4,#0x0]
  0015957a: ldr.w r1,[r9,#0xf4]
  0015957e: ldr r2,[r0,#0x28]
  00159580: ldr r0,[r0,#0x70]
  00159582: add.w r0,r0,r0, lsr #0x1f
  00159586: ldr r3,[sp,#0x74]
  00159588: add.w r0,r3,r0, asr #0x1
  0015958c: adds r0,#0x1
  0015958e: str r0,[sp,#0x0]
  00159590: movs r0,#0x0
  00159592: str r0,[sp,#0x4]
  00159594: ldr r0,[sp,#0x68]
  00159596: add r0,r2
  00159598: mov r2,r10
  0015959a: adds r3,r0,r1
  0015959c: mov r0,r5
  0015959e: mov r1,r8
  001595a0: blx 0x0006fe14
  001595a4: b 0x0015995e
  001595a6: ldr r0,[sp,#0x78]
  001595a8: blx 0x00076210
  001595ac: cmp r0,#0x0
  001595ae: beq 0x00159688
  001595b0: ldr r0,[0x00159680]
  001595b2: add r0,pc
  001595b4: ldr r0,[r0,#0x0]
  001595b6: ldr r0,[r0,#0x0]
  001595b8: movs r1,#0xae
  001595ba: blx 0x00072f70
  001595be: mov r1,r0
  001595c0: add r0,sp,#0x138
  001595c2: blx 0x0006f2b0
  001595c6: ldr r0,[sp,#0x6c]
  001595c8: cmp r0,#0x4
  001595ca: bne.w 0x00159ad6
  001595ce: ldr r0,[sp,#0x64]
  001595d0: ldr r1,[sp,#0x5c]
  001595d2: ldr r0,[r0,#0x0]
  001595d4: subs r0,#0x1
  001595d6: cmp r1,r0
  001595d8: bne.w 0x00159ad6
  001595dc: ldr r2,[sp,#0x88]
  001595de: ldr r3,[sp,#0x84]
  001595e0: ldr r6,[sp,#0x74]
  001595e2: ldr r0,[r2,#0x24]
  001595e4: ldr r3,[r3,#0x0]
  001595e6: ldr.w r1,[r2,#0xf4]
  001595ea: ldr.w r2,[r2,#0x114]
  001595ee: ldr r0,[r0,#0x4]
  001595f0: add r2,r6
  001595f2: ldr r3,[r3,#0x28]
  001595f4: ldr r6,[sp,#0x34]
  001595f6: ldr r0,[r0,#0x5c]
  001595f8: add r3,r6
  001595fa: add r1,r3
  001595fc: movs r3,#0x14
  001595fe: blx 0x00076d80
  00159602: ldr r0,[sp,#0x58]
  00159604: ldr r0,[r0,#0x0]
  00159606: ldr r0,[r0,#0x4]
  00159608: ldr r0,[r0,#0x5c]
  0015960a: movs r1,#0x1
  0015960c: blx 0x00074e48
  00159610: ldr r0,[sp,#0x58]
  00159612: ldr r0,[r0,#0x0]
  00159614: ldr r0,[r0,#0x4]
  00159616: ldr r0,[r0,#0x5c]
  00159618: blx 0x000746e0
  0015961c: ldr r1,[0x00159684]
  0015961e: add r1,pc
  00159620: add r4,sp,#0x12c
  00159622: movs r2,#0x0
  00159624: mov r0,r4
  00159626: blx 0x0006ee18
  0015962a: add r0,sp,#0x138
  0015962c: mov r1,r4
  0015962e: blx 0x0006f2b0
  00159632: mov r0,r4
  00159634: blx 0x0006ee30
  00159638: b 0x00159ad6
  0015963a: ldr.w r0,[r8,#0x14]
  0015963e: blx 0x00076f48
  00159642: cmp r0,#0x1
  00159644: bne.w 0x00159962
  00159648: ldr r4,[sp,#0x78]
  0015964a: ldr r1,[sp,#0x6c]
  0015964c: ldr r6,[sp,#0x88]
  0015964e: ldr r0,[r4,#0x10]
  00159650: cmp r1,#0x1
  00159652: bne.w 0x00159c88
  00159656: blx 0x00077014
  0015965a: ldr r5,[sp,#0x84]
  0015965c: mov r9,r0
  0015965e: ldr r0,[r4,#0x10]
  00159660: blx 0x0007183c
  00159664: ldr.w r8,[sp,#0x8c]
  00159668: b 0x00159ca8
  00159688: ldr r0,[sp,#0x78]
  0015968a: blx 0x000768f4
  0015968e: cmp r0,#0x0
  00159690: beq.w 0x00159982
  00159694: ldr r0,[0x001599c8]
  00159696: add r0,pc
  00159698: ldr r0,[r0,#0x0]
  0015969a: ldr r4,[r0,#0x0]
  0015969c: ldr r0,[sp,#0x78]
  0015969e: ldr r0,[r0,#0x8]
  001596a0: blx 0x000733fc
  001596a4: addw r1,r0,#0x4fa
  001596a8: mov r0,r4
  001596aa: blx 0x00072f70
  001596ae: mov r1,r0
  001596b0: add r0,sp,#0x138
  001596b2: blx 0x0006f2b0
  001596b6: ldr r0,[sp,#0x78]
  001596b8: ldr r0,[r0,#0x8]
  001596ba: blx 0x00077020
  001596be: vmov s0,r0
  001596c2: vcmpe.f32 s0,#0
  001596c6: vmrs apsr,fpscr
  001596ca: ble.w 0x0015988c
  001596ce: ldr r0,[sp,#0x78]
  001596d0: ldr r0,[r0,#0x8]
  001596d2: blx 0x00077020
  001596d6: ldr.w r8,[sp,#0x8c]
  001596da: ldrd r10,r9,[sp,#0x84]
  001596de: mov r4,r0
  001596e0: ldr.w r0,[r10,#0x0]
  001596e4: ldr r5,[sp,#0x3c]
  001596e6: ldr.w r6,[r9,#0xf4]
  001596ea: ldr r2,[r0,#0x28]
  001596ec: ldr r3,[r0,#0x70]
  001596ee: add r2,r5
  001596f0: ldr.w r1,[r9,#0x78]
  001596f4: ldr.w r0,[r8,#0x0]
  001596f8: add r2,r6
  001596fa: add.w r3,r3,r3, lsr #0x1f
  001596fe: ldr r6,[sp,#0x74]
  00159700: add.w r3,r6,r3, asr #0x1
  00159704: adds r3,#0x1
  00159706: blx 0x00072dc0
  0015970a: ldr.w r0,[r10,#0x0]
  0015970e: vmov s18,r4
  00159712: vldr.32 s0,[r9,#0xdc]
  00159716: vcvt.f32.s32 s0,s0
  0015971a: ldr r2,[r0,#0x28]
  0015971c: ldr r4,[sp,#0x30]
  0015971e: ldr.w r6,[r9,#0xf4]
  00159722: add r2,r4
  00159724: ldr r3,[r0,#0x70]
  00159726: add r2,r6
  00159728: ldr r6,[sp,#0x74]
  0015972a: ldr.w r1,[r9,#0x7c]
  0015972e: ldr.w r0,[r8,#0x0]
  00159732: adds r4,r6,#0x2
  00159734: ldr.w r5,[r9,#0xe0]
  00159738: vmul.f32 s0,s18,s0
  0015973c: str r2,[sp,#0x14]
  0015973e: vcvt.s32.f32 s0,s0
  00159742: movs r2,#0x0
  00159744: strd r2,r2,[sp,#0xc]
  00159748: strd r5,r2,[sp,#0x4]
  0015974c: add.w r2,r3,r3, lsr #0x1f
  00159750: movs r3,#0x0
  00159752: add.w r2,r4,r2, asr #0x1
  00159756: str r2,[sp,#0x18]
  00159758: movs r2,#0x0
  0015975a: vstr.32 s0,[sp]
  0015975e: blx 0x00072d9c
  00159762: ldr.w r0,[r9,#0xdc]
  00159766: ldr.w r1,[r9,#0x74]
  0015976a: subs r0,#0x4
  0015976c: ldr.w r2,[r9,#0xf4]
  00159770: vmov s0,r0
  00159774: vcvt.f32.s32 s0,s0
  00159778: ldr.w r3,[r10,#0x0]
  0015977c: ldr.w r0,[r8,#0x0]
  00159780: ldr r6,[r3,#0x28]
  00159782: ldr r3,[r3,#0x70]
  00159784: vmul.f32 s0,s18,s0
  00159788: movs r5,#0x11
  0015978a: add.w r3,r3,r3, lsr #0x1f
  0015978e: str r5,[sp,#0x0]
  00159790: movs r5,#0x14
  00159792: str r5,[sp,#0x4]
  00159794: vcvt.s32.f32 s0,s0
  00159798: ldr r5,[sp,#0x2c]
  0015979a: add.w r3,r4,r3, asr #0x1
  0015979e: add r6,r5
  001597a0: vmov r5,s0
  001597a4: add r6,r5
  001597a6: add r2,r6
  001597a8: blx 0x00071d70
  001597ac: ldr.w r0,[r8,#0x0]
  001597b0: ldr r1,[0x001599cc]
  001597b2: blx 0x0006fac0
  001597b6: vmul.f32 s0,s18,s16
  001597ba: vcvt.s32.f32 s0,s0
  001597be: vmov r1,s0
  001597c2: add r4,sp,#0xd8
  001597c4: mov r0,r4
  001597c6: blx 0x0006f658
  001597ca: ldr r1,[0x001599d0]
  001597cc: add r1,pc
  001597ce: add r5,sp,#0xcc
  001597d0: movs r2,#0x0
  001597d2: mov r0,r5
  001597d4: blx 0x0006ee18
  001597d8: add r6,sp,#0x114
  001597da: mov r1,r4
  001597dc: mov r2,r5
  001597de: mov r0,r6
  001597e0: blx 0x0006ef98
  001597e4: ldr r0,[sp,#0x78]
  001597e6: ldr r0,[r0,#0x8]
  001597e8: add r4,sp,#0x120
  001597ea: add.w r2,r0,#0x14
  001597ee: mov r1,r6
  001597f0: mov r0,r4
  001597f2: blx 0x0006ef98
  001597f6: ldr r1,[0x001599d4]
  001597f8: add r1,pc
  001597fa: add r5,sp,#0xc0
  001597fc: movs r2,#0x0
  001597fe: mov r0,r5
  00159800: blx 0x0006ee18
  00159804: add.w r11,sp,#0x12c
  00159808: mov r1,r4
  0015980a: mov r2,r5
  0015980c: mov r0,r11
  0015980e: blx 0x0006ef98
  00159812: ldr r1,[0x001599d8]
  00159814: mov r0,r5
  00159816: mov r5,r4
  00159818: add r1,pc
  0015981a: ldr r4,[r1,#0x0]
  0015981c: blx r4
  0015981e: mov r0,r5
  00159820: blx r4
  00159822: mov r0,r6
  00159824: blx r4
  00159826: add r0,sp,#0xcc
  00159828: blx r4
  0015982a: add r0,sp,#0xd8
  0015982c: blx r4
  0015982e: ldr r0,[0x001599dc]
  00159830: ldr.w r2,[r10,#0x0]
  00159834: add r0,pc
  00159836: ldr.w r12,[r9,#0xdc]
  0015983a: ldr.w lr,[r9,#0xf4]
  0015983e: ldr r0,[r0,#0x0]
  00159840: ldr.w r5,[r9,#0x114]
  00159844: ldr.w r4,[r2,#0x260]
  00159848: ldr r1,[r0,#0x0]
  0015984a: ldr.w r0,[r8,#0x0]
  0015984e: ldrd r3,r9,[r2,#0x28]
  00159852: ldr r2,[r2,#0x70]
  00159854: add.w r2,r2,r2, lsr #0x1f
  00159858: ldr r6,[sp,#0x74]
  0015985a: add.w r2,r6,r2, asr #0x1
  0015985e: add r2,r4
  00159860: add r2,r5
  00159862: str r2,[sp,#0x0]
  00159864: movs r2,#0x0
  00159866: str r2,[sp,#0x4]
  00159868: ldr r2,[sp,#0x3c]
  0015986a: add r2,r3
  0015986c: add r2,lr
  0015986e: add r2,r12
  00159870: add.w r3,r2,r9
  00159874: mov r2,r11
  00159876: blx 0x0006fe14
  0015987a: ldr.w r0,[r8,#0x0]
  0015987e: mov.w r1,#0xffffffff
  00159882: blx 0x0006fac0
  00159886: mov r0,r11
  00159888: blx 0x0006ee30
  0015988c: ldr r0,[0x001599e0]
  0015988e: add r0,pc
  00159890: ldr r0,[r0,#0x0]
  00159892: ldr r4,[r0,#0x0]
  00159894: ldr r0,[sp,#0x78]
  00159896: ldr r0,[r0,#0x8]
  00159898: blx 0x000733fc
  0015989c: mov r5,r0
  0015989e: ldr r0,[0x001599e4]
  001598a0: add r0,pc
  001598a2: ldr r0,[r0,#0x0]
  001598a4: ldr r6,[r0,#0x0]
  001598a6: ldr r0,[sp,#0x78]
  001598a8: ldr r0,[r0,#0x8]
  001598aa: blx 0x000733fc
  001598ae: ldr r1,[r6,#0x4]
  001598b0: ldr.w r0,[r1,r0,lsl #0x2]
  001598b4: blx 0x000718fc
  001598b8: mov r2,r0
  001598ba: ldr r0,[sp,#0x84]
  001598bc: ldr r3,[sp,#0x88]
  001598be: ldr r6,[sp,#0x74]
  001598c0: ldr r0,[r0,#0x0]
  001598c2: ldr.w r1,[r3,#0xf4]
  001598c6: ldr.w r3,[r3,#0x118]
  001598ca: ldr r0,[r0,#0x28]
  001598cc: add r3,r6
  001598ce: str r3,[sp,#0x0]
  001598d0: ldr r3,[sp,#0x60]
  001598d2: add r0,r3
  001598d4: adds r3,r0,r1
  001598d6: mov r0,r4
  001598d8: mov r1,r5
  001598da: blx 0x00076978
  001598de: ldr r0,[sp,#0x78]
  001598e0: ldrb.w r0,[r0,#0x45]
  001598e4: cmp r0,#0x0
  001598e6: beq.w 0x00159ad6
  001598ea: ldr r0,[sp,#0x8c]
  001598ec: ldr r0,[r0,#0x0]
  001598ee: ldr r1,[0x001599e8]
  001598f0: blx 0x0006fac0
  001598f4: b 0x00159ad6
  001598f6: movs r0,#0x0
  001598f8: str r0,[sp,#0x50]
  001598fa: b.w 0x0015a114
  001598fe: ldr r0,[sp,#0x8c]
  00159900: ldr.w r8,[r0,#0x0]
  00159904: ldr r0,[sp,#0x6c]
  00159906: cmp r0,#0x1
  00159908: beq 0x001599f4
  0015990a: ldr r0,[sp,#0x5c]
  0015990c: cmp r0,#0x2
  0015990e: bcs 0x001599f4
  00159910: ldr r0,[0x001599ec]
  00159912: ldr r6,[sp,#0x84]
  00159914: add r0,pc
  00159916: ldr r0,[r0,#0x0]
  00159918: ldr r5,[r6,#0x0]
  0015991a: ldr r4,[r0,#0x0]
  0015991c: ldr r0,[sp,#0x78]
  0015991e: ldr r0,[r0,#0xc]
  00159920: blx 0x00071b00
  00159924: add.w r10,sp,#0x12c
  00159928: mov r2,r0
  0015992a: mov r0,r10
  0015992c: mov r1,r5
  0015992e: blx 0x00074e54
  00159932: ldr r0,[r6,#0x0]
  00159934: ldr.w r1,[r9,#0xf4]
  00159938: ldr r2,[r0,#0x28]
  0015993a: ldr r0,[r0,#0x70]
  0015993c: add.w r0,r0,r0, lsr #0x1f
  00159940: ldr r3,[sp,#0x74]
  00159942: add.w r0,r3,r0, asr #0x1
  00159946: adds r0,#0x1
  00159948: str r0,[sp,#0x0]
  0015994a: movs r0,#0x0
  0015994c: str r0,[sp,#0x4]
  0015994e: ldr r0,[sp,#0x68]
  00159950: add r0,r2
  00159952: mov r2,r10
  00159954: adds r3,r0,r1
  00159956: mov r0,r8
  00159958: mov r1,r4
  0015995a: blx 0x0006fe14
  0015995e: mov r0,r10
  00159960: b 0x00159a94
  00159962: ldr r0,[sp,#0x88]
  00159964: ldr r0,[r0,#0x14]
  00159966: blx 0x00076f48
  0015996a: cmp r0,#0x4
  0015996c: bne 0x0015997a
  0015996e: ldr r0,[sp,#0x78]
  00159970: blx 0x000768e8
  00159974: cmp r0,#0x0
  00159976: bne.w 0x00159648
  0015997a: movs r0,#0x0
  0015997c: ldr.w r8,[sp,#0x8c]
  00159980: b 0x00159d38
  00159982: ldr r0,[sp,#0x78]
  00159984: blx 0x00076900
  00159988: cmp r0,#0x0
  0015998a: beq.w 0x0015a40e
  0015998e: ldr r0,[sp,#0x78]
  00159990: ldr r0,[r0,#0x18]
  00159992: ldr r1,[r0,#0x10]
  00159994: cmp r1,#0x2
  00159996: blt.w 0x0015a484
  0015999a: add r0,sp,#0xb4
  0015999c: blx 0x0006f658
  001599a0: ldr.w r9,[sp,#0x8c]
  001599a4: add r5,sp,#0x12c
  001599a6: add r4,sp,#0xa8
  001599a8: ldr r1,[0x001599f0]
  001599aa: add r1,pc
  001599ac: mov r0,r4
  001599ae: movs r2,#0x0
  001599b0: blx 0x0006ee18
  001599b4: add r0,sp,#0x120
  001599b6: add r1,sp,#0xb4
  001599b8: mov r2,r4
  001599ba: blx 0x0006ef98
  001599be: mov.w r8,#0x1
  001599c2: b.w 0x0015a49c
  001599f4: ldr r0,[sp,#0x78]
  001599f6: ldr r0,[r0,#0xc]
  001599f8: blx 0x00071b00
  001599fc: mov r4,r0
  001599fe: ldr r0,[0x00159c6c]
  00159a00: add r0,pc
  00159a02: ldr r6,[r0,#0x0]
  00159a04: ldr r0,[r6,#0x0]
  00159a06: blx 0x00071a58
  00159a0a: blx 0x00071b00
  00159a0e: mov r5,r0
  00159a10: ldr r0,[r6,#0x0]
  00159a12: blx 0x000733d8
  00159a16: subs r1,r4,r5
  00159a18: cmp r1,r0
  00159a1a: ldr r1,[0x00159c70]
  00159a1c: mov r0,r8
  00159a1e: itt gt
  00159a20: movw.gt r1,#0x5bff
  00159a24: movt.gt r1,#0xa35b
  00159a28: blx 0x0006fac0
  00159a2c: ldr r0,[0x00159c74]
  00159a2e: add r0,pc
  00159a30: ldr r0,[r0,#0x0]
  00159a32: ldr.w r8,[r0,#0x0]
  00159a36: ldr r0,[sp,#0x8c]
  00159a38: ldr.w r10,[r0,#0x0]
  00159a3c: ldr r0,[sp,#0x84]
  00159a3e: ldr r5,[r0,#0x0]
  00159a40: ldr r0,[sp,#0x78]
  00159a42: ldr r0,[r0,#0xc]
  00159a44: blx 0x00071b00
  00159a48: add.w r9,sp,#0x12c
  00159a4c: mov r4,r0
  00159a4e: ldr r0,[r6,#0x0]
  00159a50: blx 0x00071a58
  00159a54: blx 0x00071b00
  00159a58: subs r2,r4,r0
  00159a5a: mov r0,r9
  00159a5c: mov r1,r5
  00159a5e: blx 0x00074e54
  00159a62: ldr r0,[sp,#0x84]
  00159a64: ldr r1,[sp,#0x88]
  00159a66: ldr r0,[r0,#0x0]
  00159a68: ldr.w r1,[r1,#0xf4]
  00159a6c: ldr r2,[r0,#0x28]
  00159a6e: ldr r0,[r0,#0x70]
  00159a70: add.w r0,r0,r0, lsr #0x1f
  00159a74: ldr r3,[sp,#0x74]
  00159a76: add.w r0,r3,r0, asr #0x1
  00159a7a: adds r0,#0x1
  00159a7c: str r0,[sp,#0x0]
  00159a7e: movs r0,#0x0
  00159a80: str r0,[sp,#0x4]
  00159a82: ldr r0,[sp,#0x68]
  00159a84: add r0,r2
  00159a86: mov r2,r9
  00159a88: adds r3,r0,r1
  00159a8a: mov r0,r10
  00159a8c: mov r1,r8
  00159a8e: blx 0x0006fe14
  00159a92: mov r0,r9
  00159a94: blx 0x0006ee30
  00159a98: ldr r0,[0x00159c78]
  00159a9a: add r0,pc
  00159a9c: ldr r0,[r0,#0x0]
  00159a9e: ldr r4,[r0,#0x0]
  00159aa0: ldr r0,[sp,#0x78]
  00159aa2: ldr r0,[r0,#0xc]
  00159aa4: blx 0x000719c8
  00159aa8: mov r1,r0
  00159aaa: ldr r0,[sp,#0x84]
  00159aac: ldr r3,[sp,#0x88]
  00159aae: ldr r6,[sp,#0x74]
  00159ab0: ldr r0,[r0,#0x0]
  00159ab2: ldr.w r2,[r3,#0xf4]
  00159ab6: ldr.w r3,[r3,#0x118]
  00159aba: ldr r0,[r0,#0x28]
  00159abc: add r3,r6
  00159abe: ldr r6,[sp,#0x60]
  00159ac0: add r0,r6
  00159ac2: add r2,r0
  00159ac4: mov r0,r4
  00159ac6: blx 0x000765e8
  00159aca: ldr r0,[sp,#0x8c]
  00159acc: ldr r0,[r0,#0x0]
  00159ace: mov.w r1,#0xffffffff
  00159ad2: blx 0x0006fac0
  00159ad6: movs r0,#0x0
  00159ad8: str r0,[sp,#0x50]
  00159ada: ldr r0,[0x00159c7c]
  00159adc: ldr r1,[sp,#0x84]
  00159ade: add r0,pc
  00159ae0: ldr r2,[r1,#0x0]
  00159ae2: ldr r1,[sp,#0x88]
  00159ae4: ldr r0,[r0,#0x0]
  00159ae6: ldr r2,[r2,#0x28]
  00159ae8: ldr.w r3,[r1,#0xf4]
  00159aec: ldr r1,[r0,#0x0]
  00159aee: ldr r0,[sp,#0x8c]
  00159af0: ldr r0,[r0,#0x0]
  00159af2: ldr r6,[sp,#0x74]
  00159af4: adds r6,#0x2
  00159af6: str r6,[sp,#0x0]
  00159af8: movs r6,#0x0
  00159afa: str r6,[sp,#0x4]
  00159afc: ldr r6,[sp,#0x68]
  00159afe: add r2,r6
  00159b00: add r3,r2
  00159b02: add r2,sp,#0x138
  00159b04: blx 0x0006fe14
  00159b08: ldr r0,[sp,#0x88]
  00159b0a: ldr r1,[sp,#0x78]
  00159b0c: ldr r0,[r0,#0x68]
  00159b0e: cmp r1,r0
  00159b10: bne.w 0x0015a734
  00159b14: ldr r0,[sp,#0x78]
  00159b16: blx 0x00076204
  00159b1a: cmp r0,#0x0
  00159b1c: beq 0x00159bf0
  00159b1e: ldr r0,[sp,#0x6c]
  00159b20: ldrd r5,r4,[sp,#0x84]
  00159b24: cbz r0,0x00159b96
  00159b26: cmp r0,#0x3
  00159b28: bne 0x00159b44
  00159b2a: ldr r0,[0x00159c80]
  00159b2c: ldr r1,[r4,#0x68]
  00159b2e: add r0,pc
  00159b30: ldr r0,[r0,#0x0]
  00159b32: ldr r4,[r1,#0xc]
  00159b34: ldr r0,[r0,#0x0]
  00159b36: blx 0x00071a58
  00159b3a: cmp r4,r0
  00159b3c: ldrd r5,r4,[sp,#0x84]
  00159b40: beq.w 0x0015a6d0
  00159b44: ldr r0,[r4,#0x24]
  00159b46: ldr r3,[r5,#0x0]
  00159b48: ldr.w r2,[r4,#0x114]
  00159b4c: ldr r6,[sp,#0x74]
  00159b4e: ldr r0,[r0,#0x4]
  00159b50: add r2,r6
  00159b52: ldr r3,[r3,#0x28]
  00159b54: ldr r6,[sp,#0x80]
  00159b56: ldr.w r1,[r4,#0xf4]
  00159b5a: add r3,r6
  00159b5c: ldr r0,[r0,#0x4]
  00159b5e: add r1,r3
  00159b60: movs r3,#0x12
  00159b62: blx 0x00076d80
  00159b66: ldr r0,[sp,#0x58]
  00159b68: ldr r0,[r0,#0x0]
  00159b6a: ldr r0,[r0,#0x4]
  00159b6c: ldr r0,[r0,#0x4]
  00159b6e: movs r1,#0x1
  00159b70: blx 0x00074e48
  00159b74: ldr r0,[sp,#0x58]
  00159b76: ldr r0,[r0,#0x0]
  00159b78: ldr r0,[r0,#0x4]
  00159b7a: ldr r0,[r0,#0x4]
  00159b7c: blx 0x000746e0
  00159b80: ldr r0,[sp,#0x58]
  00159b82: ldr r0,[r0,#0x0]
  00159b84: ldr r0,[r0,#0x4]
  00159b86: ldr r0,[r0,#0x4]
  00159b88: blx 0x00074da0
  00159b8c: ldr r1,[r5,#0x0]
  00159b8e: ldr r1,[r1,#0x2c]
  00159b90: add r0,r1
  00159b92: str r0,[sp,#0x50]
  00159b94: ldr r0,[sp,#0x6c]
  00159b96: cmp r0,#0x1
  00159b98: bne.w 0x0015a6d0
  00159b9c: ldrb.w r0,[r4,#0x11d]
  00159ba0: cmp r0,#0x0
  00159ba2: beq.w 0x0015a6d0
  00159ba6: ldr r0,[sp,#0x58]
  00159ba8: ldr r0,[r0,#0x0]
  00159baa: ldr r0,[r0,#0x4]
  00159bac: ldr r0,[r0,#0x8]
  00159bae: movs r1,#0x1
  00159bb0: blx 0x00074e48
  00159bb4: ldr r0,[r4,#0x24]
  00159bb6: ldr r3,[r5,#0x0]
  00159bb8: ldr.w r1,[r4,#0xf4]
  00159bbc: ldr r0,[r0,#0x4]
  00159bbe: ldr r3,[r3,#0x28]
  00159bc0: ldr.w r2,[r4,#0x114]
  00159bc4: ldr r0,[r0,#0x28]
  00159bc6: add r1,r3
  00159bc8: ldr r6,[sp,#0x74]
  00159bca: add r2,r6
  00159bcc: movs r3,#0x11
  00159bce: blx 0x00076d80
  00159bd2: ldr r0,[sp,#0x58]
  00159bd4: ldr r0,[r0,#0x0]
  00159bd6: ldr r0,[r0,#0x4]
  00159bd8: ldr r0,[r0,#0x28]
  00159bda: movs r1,#0x1
  00159bdc: blx 0x00074e48
  00159be0: ldr r0,[sp,#0x58]
  00159be2: ldr r0,[r0,#0x0]
  00159be4: ldr r0,[r0,#0x4]
  00159be6: ldr r0,[r0,#0x28]
  00159be8: blx 0x000746e0
  00159bec: b.w 0x0015a6d0
  00159bf0: ldr r0,[sp,#0x6c]
  00159bf2: cmp r0,#0x2
  00159bf4: beq.w 0x0015a3ac
  00159bf8: cmp r0,#0x0
  00159bfa: bne.w 0x0015a6d0
  00159bfe: ldr r0,[sp,#0x78]
  00159c00: blx 0x000768e8
  00159c04: cmp r0,#0x0
  00159c06: beq.w 0x0015a6d0
  00159c0a: ldr r0,[0x00159c84]
  00159c0c: add r0,pc
  00159c0e: ldr r6,[r0,#0x0]
  00159c10: ldr r0,[r6,#0x0]
  00159c12: blx 0x00071a58
  00159c16: ldrd r10,r9,[sp,#0x84]
  00159c1a: mov r4,r0
  00159c1c: ldr r0,[sp,#0x78]
  00159c1e: ldr r0,[r0,#0x10]
  00159c20: blx 0x00071908
  00159c24: mov r1,r0
  00159c26: mov r0,r4
  00159c28: blx 0x0007228c
  00159c2c: mov r4,r0
  00159c2e: cmp r0,#0x0
  00159c30: beq.w 0x0015a654
  00159c34: ldr r0,[sp,#0x78]
  00159c36: ldr r0,[r0,#0x10]
  00159c38: blx 0x0007702c
  00159c3c: mov r5,r0
  00159c3e: mov r0,r4
  00159c40: blx 0x000718fc
  00159c44: eor r8,r5,#0x1
  00159c48: cmp r0,#0x3
  00159c4a: bne.w 0x0015a658
  00159c4e: mov r0,r4
  00159c50: blx 0x000718d8
  00159c54: mov r4,r0
  00159c56: ldr r0,[sp,#0x78]
  00159c58: ldr r0,[r0,#0x10]
  00159c5a: blx 0x000718d8
  00159c5e: subs r0,r4,r0
  00159c60: clz r0,r0
  00159c64: lsrs r5,r0,#0x5
  00159c66: b.w 0x0015a65a
  00159c88: blx 0x0007183c
  00159c8c: ldr r5,[sp,#0x84]
  00159c8e: mov r9,r0
  00159c90: mov r0,r4
  00159c92: ldr.w r4,[r6,#0x80]
  00159c96: ldr r0,[r0,#0x10]
  00159c98: blx 0x000718d8
  00159c9c: mov r1,r0
  00159c9e: ldr.w r8,[sp,#0x8c]
  00159ca2: mov r0,r4
  00159ca4: blx 0x00077038
  00159ca8: cmp r0,#0x1
  00159caa: blt 0x00159ce4
  00159cac: ldr r0,[r6,#0x24]
  00159cae: ldr r3,[r5,#0x0]
  00159cb0: ldr.w r1,[r6,#0xf4]
  00159cb4: ldr r0,[r0,#0x4]
  00159cb6: ldr r3,[r3,#0x28]
  00159cb8: ldr.w r2,[r6,#0x114]
  00159cbc: ldr r0,[r0,#0x20]
  00159cbe: add r1,r3
  00159cc0: ldr r4,[sp,#0x74]
  00159cc2: add r2,r4
  00159cc4: movs r3,#0x11
  00159cc6: blx 0x00076d80
  00159cca: ldr r0,[sp,#0x58]
  00159ccc: ldr r0,[r0,#0x0]
  00159cce: ldr r0,[r0,#0x4]
  00159cd0: ldr r0,[r0,#0x20]
  00159cd2: movs r1,#0x1
  00159cd4: blx 0x00074e48
  00159cd8: ldr r0,[sp,#0x58]
  00159cda: ldr r0,[r0,#0x0]
  00159cdc: ldr r0,[r0,#0x4]
  00159cde: ldr r0,[r0,#0x20]
  00159ce0: blx 0x000746e0
  00159ce4: cmp.w r9,#0x1
  00159ce8: blt 0x00159d26
  00159cea: ldr r0,[r6,#0x24]
  00159cec: ldr r3,[r5,#0x0]
  00159cee: ldr.w r1,[r6,#0xf4]
  00159cf2: ldr.w r2,[r6,#0x114]
  00159cf6: ldr r6,[sp,#0x74]
  00159cf8: ldr r0,[r0,#0x4]
  00159cfa: add r2,r6
  00159cfc: ldr r3,[r3,#0x28]
  00159cfe: ldr r6,[sp,#0x80]
  00159d00: ldr r0,[r0,#0x24]
  00159d02: add r3,r6
  00159d04: add r1,r3
  00159d06: movs r3,#0x12
  00159d08: blx 0x00076d80
  00159d0c: ldr r0,[sp,#0x58]
  00159d0e: ldr r0,[r0,#0x0]
  00159d10: ldr r0,[r0,#0x4]
  00159d12: ldr r0,[r0,#0x24]
  00159d14: movs r1,#0x1
  00159d16: blx 0x00074e48
  00159d1a: ldr r0,[sp,#0x58]
  00159d1c: ldr r0,[r0,#0x0]
  00159d1e: ldr r0,[r0,#0x4]
  00159d20: ldr r0,[r0,#0x24]
  00159d22: blx 0x000746e0
  00159d26: ldr r0,[sp,#0x58]
  00159d28: ldr r0,[r0,#0x0]
  00159d2a: ldr r0,[r0,#0x4]
  00159d2c: ldr r0,[r0,#0x24]
  00159d2e: blx 0x00074da0
  00159d32: ldr r1,[r5,#0x0]
  00159d34: ldr r1,[r1,#0x2c]
  00159d36: add r0,r1
  00159d38: str r0,[sp,#0x50]
  00159d3a: ldr.w r0,[r8,#0x0]
  00159d3e: mov.w r1,#0xffffffff
  00159d42: blx 0x0006fac0
  00159d46: ldr r0,[0x0015a09c]
  00159d48: add r0,pc
  00159d4a: ldr r1,[r0,#0x0]
  00159d4c: str r1,[sp,#0x28]
  00159d4e: ldr.w r0,[r10,#0x0]
  00159d52: ldr r1,[r1,#0x0]
  00159d54: str r1,[sp,#0x24]
  00159d56: ldr r1,[sp,#0x8c]
  00159d58: ldr r1,[r1,#0x0]
  00159d5a: str r1,[sp,#0x20]
  00159d5c: ldr r1,[sp,#0x6c]
  00159d5e: cmp r1,#0x1
  00159d60: bne 0x00159e38
  00159d62: movs r1,#0x88
  00159d64: blx 0x00072f70
  00159d68: str r0,[sp,#0x1c]
  00159d6a: ldr r0,[sp,#0x84]
  00159d6c: ldr r1,[sp,#0x88]
  00159d6e: ldr r0,[r0,#0x0]
  00159d70: ldr.w r5,[r1,#0xf4]
  00159d74: ldr.w r11,[r1,#0x100]
  00159d78: ldr.w r8,[r0,#0x28]
  00159d7c: ldr r0,[sp,#0x28]
  00159d7e: ldr r6,[r0,#0x0]
  00159d80: ldr r0,[sp,#0x8c]
  00159d82: ldr r4,[r0,#0x0]
  00159d84: ldr.w r0,[r10,#0x0]
  00159d88: movs r1,#0x88
  00159d8a: blx 0x00072f70
  00159d8e: mov r2,r0
  00159d90: mov r0,r4
  00159d92: mov r1,r6
  00159d94: blx 0x0006faa8
  00159d98: ldr r2,[sp,#0x74]
  00159d9a: add.w r1,r5,r8
  00159d9e: add r1,r11
  00159da0: add.w r9,r2,#0x2
  00159da4: movs r2,#0x0
  00159da6: add.w r0,r0,r0, lsr #0x1f
  00159daa: str.w r9,[sp,#0x0]
  00159dae: str r2,[sp,#0x4]
  00159db0: sub.w r3,r1,r0, asr #0x1
  00159db4: ldr r1,[sp,#0x24]
  00159db6: ldrd r2,r0,[sp,#0x1c]
  00159dba: blx 0x0006fe14
  00159dbe: ldr r0,[sp,#0x28]
  00159dc0: ldr r0,[r0,#0x0]
  00159dc2: str r0,[sp,#0x24]
  00159dc4: ldr r0,[sp,#0x8c]
  00159dc6: ldr r0,[r0,#0x0]
  00159dc8: str r0,[sp,#0x20]
  00159dca: ldr.w r0,[r10,#0x0]
  00159dce: movs r1,#0xb7
  00159dd0: blx 0x00072f70
  00159dd4: mov r11,r0
  00159dd6: ldr r0,[sp,#0x88]
  00159dd8: ldr.w r6,[r0,#0xf4]
  00159ddc: ldr.w r5,[r0,#0x100]
  00159de0: ldr r0,[sp,#0x28]
  00159de2: ldr r4,[r0,#0x0]
  00159de4: ldr r0,[sp,#0x8c]
  00159de6: ldr.w r8,[r0,#0x0]
  00159dea: ldr.w r0,[r10,#0x0]
  00159dee: movs r1,#0xb7
  00159df0: blx 0x00072f70
  00159df4: mov r2,r0
  00159df6: mov r0,r8
  00159df8: mov r1,r4
  00159dfa: blx 0x0006faa8
  00159dfe: ldr r1,[sp,#0x80]
  00159e00: add r1,r6
  00159e02: subs r1,r1,r5
  00159e04: movs r2,#0x0
  00159e06: add.w r0,r0,r0, lsr #0x1f
  00159e0a: str r2,[sp,#0x4]
  00159e0c: mov r2,r11
  00159e0e: str.w r9,[sp,#0x0]
  00159e12: sub.w r3,r1,r0, asr #0x1
  00159e16: ldrd r0,r1,[sp,#0x20]
  00159e1a: blx 0x0006fe14
  00159e1e: ldr r4,[sp,#0x78]
  00159e20: ldr r0,[r4,#0x10]
  00159e22: ldrd r8,r9,[sp,#0x88]
  00159e26: blx 0x00077014
  00159e2a: add r6,sp,#0x114
  00159e2c: mov r5,r0
  00159e2e: ldr r0,[r4,#0x10]
  00159e30: blx 0x0007183c
  00159e34: mov r10,r0
  00159e36: b 0x00159f3e
  00159e38: movs r1,#0xb7
  00159e3a: blx 0x00072f70
  00159e3e: str r0,[sp,#0x1c]
  00159e40: ldr r0,[sp,#0x84]
  00159e42: ldr r1,[r0,#0x0]
  00159e44: ldr r0,[sp,#0x88]
  00159e46: ldr.w r11,[r1,#0x28]
  00159e4a: ldr.w r4,[r0,#0xf4]
  00159e4e: ldr.w r5,[r0,#0x100]
  00159e52: ldr r0,[sp,#0x28]
  00159e54: ldr.w r9,[r1,#0x254]
  00159e58: ldr r6,[r0,#0x0]
  00159e5a: ldr r0,[sp,#0x8c]
  00159e5c: ldr.w r8,[r0,#0x0]
  00159e60: ldr.w r0,[r10,#0x0]
  00159e64: movs r1,#0xb7
  00159e66: blx 0x00072f70
  00159e6a: mov r2,r0
  00159e6c: mov r0,r8
  00159e6e: mov r1,r6
  00159e70: blx 0x0006faa8
  00159e74: add.w r1,r4,r11
  00159e78: ldr r2,[sp,#0x74]
  00159e7a: add r1,r5
  00159e7c: sub.w r1,r1,r9
  00159e80: add.w r9,r2,#0x2
  00159e84: movs r2,#0x0
  00159e86: add.w r0,r0,r0, lsr #0x1f
  00159e8a: str.w r9,[sp,#0x0]
  00159e8e: str r2,[sp,#0x4]
  00159e90: sub.w r3,r1,r0, asr #0x1
  00159e94: ldr r1,[sp,#0x24]
  00159e96: ldrd r2,r0,[sp,#0x1c]
  00159e9a: blx 0x0006fe14
  00159e9e: ldr r0,[sp,#0x28]
  00159ea0: ldr r0,[r0,#0x0]
  00159ea2: str r0,[sp,#0x24]
  00159ea4: ldr r0,[sp,#0x8c]
  00159ea6: ldr r0,[r0,#0x0]
  00159ea8: str r0,[sp,#0x20]
  00159eaa: ldr.w r0,[r10,#0x0]
  00159eae: movw r1,#0x10f
  00159eb2: blx 0x00072f70
  00159eb6: str r0,[sp,#0x1c]
  00159eb8: ldr r0,[sp,#0x84]
  00159eba: ldr r1,[sp,#0x88]
  00159ebc: ldr r0,[r0,#0x0]
  00159ebe: ldr.w r6,[r1,#0xf4]
  00159ec2: ldr.w r5,[r1,#0x100]
  00159ec6: ldr r1,[sp,#0x28]
  00159ec8: ldr.w r11,[r0,#0x258]
  00159ecc: ldr r0,[sp,#0x8c]
  00159ece: ldr r4,[r1,#0x0]
  00159ed0: ldr.w r8,[r0,#0x0]
  00159ed4: ldr.w r0,[r10,#0x0]
  00159ed8: movw r1,#0x10f
  00159edc: blx 0x00072f70
  00159ee0: mov r2,r0
  00159ee2: mov r0,r8
  00159ee4: mov r1,r4
  00159ee6: blx 0x0006faa8
  00159eea: ldr r1,[sp,#0x80]
  00159eec: add r1,r6
  00159eee: subs r1,r1,r5
  00159ef0: add r1,r11
  00159ef2: movs r2,#0x0
  00159ef4: add.w r0,r0,r0, lsr #0x1f
  00159ef8: str.w r9,[sp,#0x0]
  00159efc: str r2,[sp,#0x4]
  00159efe: sub.w r3,r1,r0, asr #0x1
  00159f02: ldr r1,[sp,#0x24]
  00159f04: ldrd r2,r0,[sp,#0x1c]
  00159f08: blx 0x0006fe14
  00159f0c: ldr r4,[sp,#0x78]
  00159f0e: ldr r0,[r4,#0x10]
  00159f10: ldrd r8,r9,[sp,#0x88]
  00159f14: blx 0x0007183c
  00159f18: add r6,sp,#0x114
  00159f1a: mov r5,r0
  00159f1c: mov r0,r4
  00159f1e: ldr.w r4,[r8,#0x80]
  00159f22: ldr r0,[r0,#0x10]
  00159f24: blx 0x000718d8
  00159f28: mov r1,r0
  00159f2a: mov r0,r4
  00159f2c: blx 0x00077038
  00159f30: mov r4,r0
  00159f32: ldr r0,[sp,#0x78]
  00159f34: ldr r0,[r0,#0x10]
  00159f36: blx 0x00076f78
  00159f3a: add.w r10,r0,r4
  00159f3e: add.w r11,sp,#0x120
  00159f42: mov r1,r5
  00159f44: mov r0,r11
  00159f46: blx 0x0006f658
  00159f4a: add r4,sp,#0x12c
  00159f4c: ldr r1,[0x0015a0a0]
  00159f4e: add r1,pc
  00159f50: mov r0,r6
  00159f52: movs r2,#0x0
  00159f54: blx 0x0006ee18
  00159f58: mov r0,r4
  00159f5a: mov r1,r11
  00159f5c: mov r2,r6
  00159f5e: blx 0x0006ef98
  00159f62: mov r0,r6
  00159f64: blx 0x0006ee30
  00159f68: mov r0,r11
  00159f6a: blx 0x0006ee30
  00159f6e: ldr r0,[sp,#0x6c]
  00159f70: cmp r0,#0x1
  00159f72: bne 0x00159f84
  00159f74: mov r0,r6
  00159f76: mov r1,r10
  00159f78: blx 0x0006f658
  00159f7c: add r4,sp,#0xd8
  00159f7e: mov.w r8,#0x0
  00159f82: b 0x00159fde
  00159f84: add r0,sp,#0xfc
  00159f86: mov r1,r10
  00159f88: blx 0x0006f658
  00159f8c: add r4,sp,#0xd8
  00159f8e: strd r5,r10,[sp,#0x24]
  00159f92: mov r10,r9
  00159f94: ldr r1,[0x0015a0a4]
  00159f96: add r1,pc
  00159f98: add.w r9,sp,#0xf0
  00159f9c: movs r2,#0x0
  00159f9e: mov r0,r9
  00159fa0: blx 0x0006ee18
  00159fa4: add r0,sp,#0x108
  00159fa6: add r1,sp,#0xfc
  00159fa8: mov r2,r9
  00159faa: blx 0x0006ef98
  00159fae: ldr r0,[sp,#0x78]
  00159fb0: ldr.w r5,[r8,#0x80]
  00159fb4: ldr r0,[r0,#0x10]
  00159fb6: blx 0x000718d8
  00159fba: mov r1,r0
  00159fbc: mov r0,r5
  00159fbe: blx 0x00077044
  00159fc2: mov r1,r0
  00159fc4: add r0,sp,#0xe4
  00159fc6: blx 0x0006f658
  00159fca: add r1,sp,#0x108
  00159fcc: add r2,sp,#0xe4
  00159fce: mov r0,r6
  00159fd0: blx 0x0006ef98
  00159fd4: mov r9,r10
  00159fd6: ldrd r5,r10,[sp,#0x24]
  00159fda: mov.w r8,#0x1
  00159fde: ldr r1,[0x0015a0a8]
  00159fe0: add r1,pc
  00159fe2: mov r0,r4
  00159fe4: movs r2,#0x0
  00159fe6: blx 0x0006ee18
  00159fea: mov r0,r11
  00159fec: mov r1,r6
  00159fee: mov r2,r4
  00159ff0: blx 0x0006ef98
  00159ff4: mov r0,r4
  00159ff6: blx 0x0006ee30
  00159ffa: mov r0,r6
  00159ffc: blx 0x0006ee30
  0015a000: cmp.w r8,#0x0
  0015a004: beq 0x0015a01c
  0015a006: ldr r0,[0x0015a0ac]
  0015a008: add r0,pc
  0015a00a: ldr r4,[r0,#0x0]
  0015a00c: add r0,sp,#0xe4
  0015a00e: blx r4
  0015a010: add r0,sp,#0x108
  0015a012: blx r4
  0015a014: add r0,sp,#0xf0
  0015a016: blx r4
  0015a018: add r0,sp,#0xfc
  0015a01a: blx r4
  0015a01c: ldr.w r0,[r9,#0x0]
  0015a020: mov.w r1,#0xffffffff
  0015a024: blx 0x0006fac0
  0015a028: cmp r5,#0x1
  0015a02a: blt 0x0015a082
  0015a02c: ldr r0,[0x0015a0b0]
  0015a02e: ldr r1,[sp,#0x84]
  0015a030: add r0,pc
  0015a032: ldr r2,[sp,#0x88]
  0015a034: ldr r0,[r0,#0x0]
  0015a036: ldr r1,[r1,#0x0]
  0015a038: ldr.w r9,[r2,#0xf4]
  0015a03c: ldr r5,[r0,#0x0]
  0015a03e: ldr r0,[sp,#0x8c]
  0015a040: ldr.w r8,[r2,#0x100]
  0015a044: ldr.w r11,[r2,#0x108]
  0015a048: ldr r6,[r0,#0x0]
  0015a04a: ldr r4,[r1,#0x28]
  0015a04c: add r2,sp,#0x12c
  0015a04e: mov r0,r6
  0015a050: mov r1,r5
  0015a052: blx 0x0006faa8
  0015a056: ldr r1,[sp,#0x84]
  0015a058: ldr r1,[r1,#0x0]
  0015a05a: ldr r1,[r1,#0x70]
  0015a05c: add.w r1,r1,r1, lsr #0x1f
  0015a060: ldr r2,[sp,#0x74]
  0015a062: add.w r1,r2,r1, asr #0x1
  0015a066: add r2,sp,#0x12c
  0015a068: adds r1,#0x1
  0015a06a: str r1,[sp,#0x0]
  0015a06c: movs r1,#0x0
  0015a06e: str r1,[sp,#0x4]
  0015a070: add.w r1,r9,r4
  0015a074: add r1,r8
  0015a076: add r1,r11
  0015a078: subs r3,r1,r0
  0015a07a: mov r0,r6
  0015a07c: mov r1,r5
  0015a07e: blx 0x0006fe14
  0015a082: ldr r6,[sp,#0x6c]
  0015a084: cmp r6,#0x4
  0015a086: beq 0x0015a08e
  0015a088: cmp.w r10,#0x1
  0015a08c: blt 0x0015a108
  0015a08e: cmp r6,#0x4
  0015a090: bne 0x0015a0b4
  0015a092: ldr r0,[sp,#0x84]
  0015a094: ldr r0,[r0,#0x0]
  0015a096: ldr.w r8,[r0,#0x25c]
  0015a09a: b 0x0015a0b8
  0015a0b4: mov.w r8,#0x0
  0015a0b8: ldr r1,[sp,#0x88]
  0015a0ba: ldr r0,[0x0015a454]
  0015a0bc: add r0,pc
  0015a0be: ldr.w r9,[r1,#0xf4]
  0015a0c2: ldrd r10,r11,[r1,#0x100]
  0015a0c6: ldr r0,[r0,#0x0]
  0015a0c8: ldr r4,[r0,#0x0]
  0015a0ca: ldr r0,[sp,#0x8c]
  0015a0cc: ldr r5,[r0,#0x0]
  0015a0ce: add r2,sp,#0x120
  0015a0d0: mov r0,r5
  0015a0d2: mov r1,r4
  0015a0d4: blx 0x0006faa8
  0015a0d8: ldr r1,[sp,#0x84]
  0015a0da: ldr r1,[r1,#0x0]
  0015a0dc: ldr r1,[r1,#0x70]
  0015a0de: add.w r1,r1,r1, lsr #0x1f
  0015a0e2: ldr r2,[sp,#0x74]
  0015a0e4: add.w r1,r2,r1, asr #0x1
  0015a0e8: add r2,sp,#0x120
  0015a0ea: adds r1,#0x1
  0015a0ec: str r1,[sp,#0x0]
  0015a0ee: movs r1,#0x0
  0015a0f0: str r1,[sp,#0x4]
  0015a0f2: ldr r1,[sp,#0x80]
  0015a0f4: add r1,r8
  0015a0f6: add r1,r9
  0015a0f8: sub.w r1,r1,r10
  0015a0fc: add r1,r11
  0015a0fe: subs r3,r1,r0
  0015a100: mov r0,r5
  0015a102: mov r1,r4
  0015a104: blx 0x0006fe14
  0015a108: add r0,sp,#0x120
  0015a10a: blx 0x0006ee30
  0015a10e: add r0,sp,#0x12c
  0015a110: blx 0x0006ee30
  0015a114: ldr r0,[sp,#0x8c]
  0015a116: cmp r6,#0x1
  0015a118: ldr r4,[r0,#0x0]
  0015a11a: bne 0x0015a148
  0015a11c: ldr r0,[sp,#0x78]
  0015a11e: ldr r0,[r0,#0x10]
  0015a120: blx 0x00071944
  0015a124: mov r5,r0
  0015a126: ldr r0,[0x0015a458]
  0015a128: add r0,pc
  0015a12a: ldr r0,[r0,#0x0]
  0015a12c: ldr r0,[r0,#0x0]
  0015a12e: blx 0x000733d8
  0015a132: ldr r1,[0x0015a45c]
  0015a134: cmp r5,r0
  0015a136: mov r0,r4
  0015a138: itt gt
  0015a13a: movw.gt r1,#0x5bff
  0015a13e: movt.gt r1,#0xa35b
  0015a142: blx 0x0006fac0
  0015a146: b 0x0015a150
  0015a148: ldr r1,[0x0015a460]
  0015a14a: mov r0,r4
  0015a14c: blx 0x0006fac0
  0015a150: ldr r4,[sp,#0x88]
  0015a152: ldrb.w r0,[r4,#0x11d]
  0015a156: cbnz r0,0x0015a1b4
  0015a158: ldr r0,[0x0015a464]
  0015a15a: ldr r5,[sp,#0x84]
  0015a15c: add r0,pc
  0015a15e: ldr r0,[r0,#0x0]
  0015a160: ldr r6,[r5,#0x0]
  0015a162: ldr.w r8,[r0,#0x0]
  0015a166: ldr r0,[sp,#0x8c]
  0015a168: ldr.w r9,[r0,#0x0]
  0015a16c: ldr r0,[sp,#0x78]
  0015a16e: ldr r0,[r0,#0x10]
  0015a170: blx 0x00071944
  0015a174: add.w r10,sp,#0x12c
  0015a178: mov r2,r0
  0015a17a: mov r0,r10
  0015a17c: mov r1,r6
  0015a17e: blx 0x00074e54
  0015a182: ldr r0,[r5,#0x0]
  0015a184: ldr.w r1,[r4,#0xf4]
  0015a188: ldr r2,[r0,#0x28]
  0015a18a: ldr r0,[r0,#0x70]
  0015a18c: add.w r0,r0,r0, lsr #0x1f
  0015a190: ldr r3,[sp,#0x74]
  0015a192: add.w r0,r3,r0, asr #0x1
  0015a196: adds r0,#0x1
  0015a198: str r0,[sp,#0x0]
  0015a19a: movs r0,#0x0
  0015a19c: str r0,[sp,#0x4]
  0015a19e: ldr r0,[sp,#0x68]
  0015a1a0: add r0,r2
  0015a1a2: mov r2,r10
  0015a1a4: adds r3,r0,r1
  0015a1a6: mov r0,r9
  0015a1a8: mov r1,r8
  0015a1aa: blx 0x0006fe14
  0015a1ae: mov r0,r10
  0015a1b0: blx 0x0006ee30
  0015a1b4: ldr r0,[0x0015a468]
  0015a1b6: add r0,pc
  0015a1b8: ldr r0,[r0,#0x0]
  0015a1ba: ldr r4,[r0,#0x0]
  0015a1bc: ldr r0,[sp,#0x78]
  0015a1be: ldr r0,[r0,#0x10]
  0015a1c0: blx 0x000718d8
  0015a1c4: mov r5,r0
  0015a1c6: ldr r0,[sp,#0x78]
  0015a1c8: ldr r0,[r0,#0x10]
  0015a1ca: blx 0x000718fc
  0015a1ce: mov r2,r0
  0015a1d0: ldr r0,[sp,#0x84]
  0015a1d2: ldr r3,[sp,#0x88]
  0015a1d4: ldr r6,[sp,#0x74]
  0015a1d6: ldr r0,[r0,#0x0]
  0015a1d8: ldr.w r1,[r3,#0xf4]
  0015a1dc: ldr.w r3,[r3,#0x118]
  0015a1e0: ldr r0,[r0,#0x28]
  0015a1e2: add r3,r6
  0015a1e4: str r3,[sp,#0x0]
  0015a1e6: ldr r3,[sp,#0x60]
  0015a1e8: add r0,r3
  0015a1ea: adds r3,r0,r1
  0015a1ec: mov r0,r4
  0015a1ee: mov r1,r5
  0015a1f0: blx 0x00076978
  0015a1f4: ldr r1,[sp,#0x6c]
  0015a1f6: cmp r1,#0x1
  0015a1f8: bne 0x0015a218
  0015a1fa: ldr r0,[0x0015a46c]
  0015a1fc: add r0,pc
  0015a1fe: ldr r0,[r0,#0x0]
  0015a200: ldr r0,[r0,#0x0]
  0015a202: ldr r4,[r0,#0x54]
  0015a204: ldr r0,[sp,#0x78]
  0015a206: ldr r0,[r0,#0x10]
  0015a208: blx 0x000718d8
  0015a20c: ldr r1,[r4,#0x4]
  0015a20e: ldrb r0,[r1,r0]
  0015a210: ldr r1,[sp,#0x6c]
  0015a212: cmp r0,#0x0
  0015a214: beq.w 0x0015a370
  0015a218: ldr r0,[sp,#0x78]
  0015a21a: ldr r0,[r0,#0x3c]
  0015a21c: cmp.w r0,#0xffffffff
  0015a220: ble 0x0015a2e4
  0015a222: ldr r0,[sp,#0x84]
  0015a224: ldr r2,[r0,#0x0]
  0015a226: ldr r0,[sp,#0x88]
  0015a228: ldr.w r1,[r0,#0xe8]
  0015a22c: ldr.w r3,[r0,#0xf4]
  0015a230: ldr.w r12,[r0,#0x118]
  0015a234: ldr r0,[sp,#0x8c]
  0015a236: ldr r0,[r0,#0x0]
  0015a238: ldrd r5,r4,[r2,#0x2cc]
  0015a23c: ldr r2,[r2,#0x28]
  0015a23e: movs r6,#0x11
  0015a240: str r6,[sp,#0x0]
  0015a242: movs r6,#0x22
  0015a244: str r6,[sp,#0x4]
  0015a246: ldr r6,[sp,#0x4c]
  0015a248: add r2,r6
  0015a24a: add r2,r3
  0015a24c: ldr r3,[sp,#0x74]
  0015a24e: add r2,r5
  0015a250: add r3,r12
  0015a252: add r3,r4
  0015a254: subs r3,#0x2
  0015a256: blx 0x00071d70
  0015a25a: ldr r0,[sp,#0x6c]
  0015a25c: cmp r0,#0x3
  0015a25e: it ne
  0015a260: cmp.ne r0,#0x0
  0015a262: bne.w 0x00159ada
  0015a266: ldr r0,[sp,#0x78]
  0015a268: ldr r0,[r0,#0x10]
  0015a26a: blx 0x0007183c
  0015a26e: cmp r0,#0x2
  0015a270: blt.w 0x00159ada
  0015a274: ldr r1,[0x0015a470]
  0015a276: add r1,pc
  0015a278: add.w r9,sp,#0x114
  0015a27c: movs r2,#0x0
  0015a27e: mov r0,r9
  0015a280: blx 0x0006ee18
  0015a284: ldr r0,[sp,#0x78]
  0015a286: ldr r0,[r0,#0x10]
  0015a288: blx 0x0007183c
  0015a28c: mov r1,r0
  0015a28e: add r5,sp,#0x12c
  0015a290: add r6,sp,#0xd8
  0015a292: mov r0,r6
  0015a294: blx 0x0006f658
  0015a298: add r4,sp,#0x120
  0015a29a: mov r1,r9
  0015a29c: mov r2,r6
  0015a29e: mov r0,r4
  0015a2a0: blx 0x0006ef98
  0015a2a4: ldr r1,[0x0015a474]
  0015a2a6: add r1,pc
  0015a2a8: add r6,sp,#0xcc
  0015a2aa: movs r2,#0x0
  0015a2ac: mov r0,r6
  0015a2ae: blx 0x0006ee18
  0015a2b2: mov r0,r5
  0015a2b4: mov r1,r4
  0015a2b6: mov r2,r6
  0015a2b8: blx 0x0006ef98
  0015a2bc: add r0,sp,#0x138
  0015a2be: mov r1,r5
  0015a2c0: blx 0x0006ef5c
  0015a2c4: ldr r0,[0x0015a478]
  0015a2c6: mov r8,r4
  0015a2c8: add r0,pc
  0015a2ca: ldr r4,[r0,#0x0]
  0015a2cc: mov r0,r5
  0015a2ce: blx r4
  0015a2d0: mov r0,r6
  0015a2d2: blx r4
  0015a2d4: mov r0,r8
  0015a2d6: blx r4
  0015a2d8: add r0,sp,#0xd8
  0015a2da: blx r4
  0015a2dc: mov r0,r9
  0015a2de: blx r4
  0015a2e0: b.w 0x00159ada
  0015a2e4: cmp r1,#0x1
  0015a2e6: bne 0x0015a30e
  0015a2e8: ldr r0,[0x0015a47c]
  0015a2ea: add r0,pc
  0015a2ec: ldr r0,[r0,#0x0]
  0015a2ee: ldr r0,[r0,#0x0]
  0015a2f0: blx 0x00071a58
  0015a2f4: mov r4,r0
  0015a2f6: ldr r0,[sp,#0x78]
  0015a2f8: ldr r0,[r0,#0x10]
  0015a2fa: blx 0x000718d8
  0015a2fe: mov r1,r0
  0015a300: mov r0,r4
  0015a302: movs r2,#0x1
  0015a304: blx 0x00073978
  0015a308: ldr r1,[sp,#0x6c]
  0015a30a: cmp r0,#0x0
  0015a30c: bne 0x0015a222
  0015a30e: cmp r1,#0x4
  0015a310: bne 0x0015a25a
  0015a312: ldr r0,[0x0015a480]
  0015a314: ldr r1,[sp,#0x78]
  0015a316: add r0,pc
  0015a318: ldr r0,[r0,#0x0]
  0015a31a: ldr r4,[r1,#0x10]
  0015a31c: ldr r0,[r0,#0x0]
  0015a31e: blx 0x00071c14
  0015a322: blx 0x00071830
  0015a326: mov r1,r0
  0015a328: mov r0,r4
  0015a32a: blx 0x00077050
  0015a32e: cmp r0,#0x0
  0015a330: beq.w 0x00159ada
  0015a334: ldr r0,[sp,#0x84]
  0015a336: ldr r2,[r0,#0x0]
  0015a338: ldr r0,[sp,#0x88]
  0015a33a: ldr.w r1,[r0,#0xe8]
  0015a33e: ldr.w r3,[r0,#0xf4]
  0015a342: ldr.w r12,[r0,#0x118]
  0015a346: ldr r0,[sp,#0x8c]
  0015a348: ldr r0,[r0,#0x0]
  0015a34a: ldrd r5,r4,[r2,#0x2cc]
  0015a34e: ldr r2,[r2,#0x28]
  0015a350: movs r6,#0x11
  0015a352: str r6,[sp,#0x0]
  0015a354: movs r6,#0x22
  0015a356: str r6,[sp,#0x4]
  0015a358: ldr r6,[sp,#0x4c]
  0015a35a: add r2,r6
  0015a35c: add r2,r3
  0015a35e: ldr r3,[sp,#0x74]
  0015a360: add r2,r5
  0015a362: add r3,r12
  0015a364: add r3,r4
  0015a366: subs r3,#0x2
  0015a368: blx 0x00071d70
  0015a36c: b.w 0x00159ada
  0015a370: ldr r0,[sp,#0x84]
  0015a372: ldr r2,[r0,#0x0]
  0015a374: ldr r0,[sp,#0x88]
  0015a376: ldr.w r1,[r0,#0xec]
  0015a37a: ldr.w r3,[r0,#0xf4]
  0015a37e: ldr.w r12,[r0,#0x118]
  0015a382: ldr r0,[sp,#0x8c]
  0015a384: ldr r0,[r0,#0x0]
  0015a386: ldrd r5,r4,[r2,#0x2cc]
  0015a38a: ldr r2,[r2,#0x28]
  0015a38c: movs r6,#0x11
  0015a38e: str r6,[sp,#0x0]
  0015a390: movs r6,#0x22
  0015a392: str r6,[sp,#0x4]
  0015a394: ldr r6,[sp,#0x4c]
  0015a396: add r2,r6
  0015a398: add r2,r3
  0015a39a: ldr r3,[sp,#0x74]
  0015a39c: add r2,r5
  0015a39e: add r3,r12
  0015a3a0: add r3,r4
  0015a3a2: subs r3,#0x2
  0015a3a4: blx 0x00071d70
  0015a3a8: b.w 0x00159ada
  0015a3ac: ldr r0,[sp,#0x78]
  0015a3ae: blx 0x000768f4
  0015a3b2: ldrd r3,r2,[sp,#0x84]
  0015a3b6: cmp r0,#0x0
  0015a3b8: beq.w 0x0015a6d0
  0015a3bc: ldr r0,[r2,#0x24]
  0015a3be: ldr r3,[r3,#0x0]
  0015a3c0: ldr.w r1,[r2,#0xf4]
  0015a3c4: ldr.w r2,[r2,#0x114]
  0015a3c8: ldr r6,[sp,#0x74]
  0015a3ca: ldr r0,[r0,#0x4]
  0015a3cc: add r2,r6
  0015a3ce: ldr r3,[r3,#0x28]
  0015a3d0: ldr r6,[sp,#0x80]
  0015a3d2: ldr r0,[r0,#0x1c]
  0015a3d4: add r3,r6
  0015a3d6: add r1,r3
  0015a3d8: movs r3,#0x12
  0015a3da: blx 0x00076d80
  0015a3de: ldr r0,[sp,#0x58]
  0015a3e0: ldr r0,[r0,#0x0]
  0015a3e2: ldr r0,[r0,#0x4]
  0015a3e4: ldr r0,[r0,#0x1c]
  0015a3e6: movs r1,#0x1
  0015a3e8: blx 0x00074e48
  0015a3ec: ldr r0,[sp,#0x58]
  0015a3ee: ldr r0,[r0,#0x0]
  0015a3f0: ldr r0,[r0,#0x4]
  0015a3f2: ldr r0,[r0,#0x1c]
  0015a3f4: blx 0x000746e0
  0015a3f8: ldr r0,[sp,#0x58]
  0015a3fa: ldr r0,[r0,#0x0]
  0015a3fc: ldr r0,[r0,#0x4]
  0015a3fe: ldr r0,[r0,#0x1c]
  0015a400: blx 0x00074da0
  0015a404: ldr r1,[sp,#0x84]
  0015a406: ldr r1,[r1,#0x0]
  0015a408: ldr r1,[r1,#0x2c]
  0015a40a: add r0,r1
  0015a40c: b 0x0015a6ce
  0015a40e: ldr r0,[sp,#0x78]
  0015a410: blx 0x0007705c
  0015a414: cmp r0,#0x0
  0015a416: beq.w 0x0015a5ce
  0015a41a: ldr r1,[sp,#0x88]
  0015a41c: ldr r2,[sp,#0x84]
  0015a41e: ldr r0,[r1,#0x24]
  0015a420: ldr r2,[r2,#0x0]
  0015a422: ldr.w r1,[r1,#0xf4]
  0015a426: ldr r0,[r0,#0x4]
  0015a428: ldr r2,[r2,#0x28]
  0015a42a: ldr r0,[r0,#0x18]
  0015a42c: add r1,r2
  0015a42e: ldr r2,[sp,#0x74]
  0015a430: movs r3,#0x11
  0015a432: blx 0x00076d80
  0015a436: ldr r0,[sp,#0x58]
  0015a438: ldr r0,[r0,#0x0]
  0015a43a: ldr r0,[r0,#0x4]
  0015a43c: ldr r0,[r0,#0x18]
  0015a43e: movs r1,#0x1
  0015a440: blx 0x00074e48
  0015a444: ldr r0,[sp,#0x58]
  0015a446: ldr r0,[r0,#0x0]
  0015a448: ldr r0,[r0,#0x4]
  0015a44a: ldr r0,[r0,#0x18]
  0015a44c: blx 0x000746e0
  0015a450: b 0x0015a734
  0015a484: ldr r1,[0x0015a800]
  0015a486: mov.w r8,#0x0
  0015a48a: add r1,pc
  0015a48c: add r0,sp,#0x120
  0015a48e: movs r2,#0x0
  0015a490: blx 0x0006ee18
  0015a494: ldr.w r9,[sp,#0x8c]
  0015a498: add r5,sp,#0x12c
  0015a49a: add r4,sp,#0xa8
  0015a49c: ldr r0,[0x0015a804]
  0015a49e: ldr r1,[sp,#0x78]
  0015a4a0: add r0,pc
  0015a4a2: ldr r1,[r1,#0x18]
  0015a4a4: ldr r6,[r0,#0x0]
  0015a4a6: ldr r1,[r1,#0x14]
  0015a4a8: ldr r0,[r6,#0x0]
  0015a4aa: addw r1,r1,#0x4fa
  0015a4ae: blx 0x00072f70
  0015a4b2: mov r2,r0
  0015a4b4: add r1,sp,#0x120
  0015a4b6: mov r0,r5
  0015a4b8: blx 0x0006ef98
  0015a4bc: add r0,sp,#0x138
  0015a4be: mov r1,r5
  0015a4c0: blx 0x0006f2b0
  0015a4c4: mov r0,r5
  0015a4c6: blx 0x0006ee30
  0015a4ca: add r0,sp,#0x120
  0015a4cc: blx 0x0006ee30
  0015a4d0: cmp.w r8,#0x0
  0015a4d4: beq 0x0015a4e2
  0015a4d6: mov r0,r4
  0015a4d8: blx 0x0006ee30
  0015a4dc: add r0,sp,#0xb4
  0015a4de: blx 0x0006ee30
  0015a4e2: ldr.w r0,[r9,#0x0]
  0015a4e6: ldr r1,[0x0015a808]
  0015a4e8: blx 0x0006fac0
  0015a4ec: ldr r0,[0x0015a80c]
  0015a4ee: ldr.w r8,[sp,#0x8c]
  0015a4f2: add r0,pc
  0015a4f4: ldr r0,[r0,#0x0]
  0015a4f6: ldr.w r5,[r8,#0x0]
  0015a4fa: ldr.w r9,[r0,#0x0]
  0015a4fe: ldr r0,[r6,#0x0]
  0015a500: movw r1,#0x113
  0015a504: blx 0x00072f70
  0015a508: ldr r1,[0x0015a810]
  0015a50a: mov r6,r0
  0015a50c: add r1,pc
  0015a50e: add.w r10,sp,#0x114
  0015a512: movs r2,#0x0
  0015a514: mov r0,r10
  0015a516: blx 0x0006ee18
  0015a51a: add.w r11,sp,#0x12c
  0015a51e: add r4,sp,#0x120
  0015a520: mov r1,r6
  0015a522: mov r2,r10
  0015a524: mov r0,r4
  0015a526: blx 0x0006ef98
  0015a52a: ldr r0,[sp,#0x78]
  0015a52c: ldr r2,[r0,#0x18]
  0015a52e: mov r0,r11
  0015a530: mov r1,r4
  0015a532: blx 0x0006ef98
  0015a536: ldr r0,[sp,#0x84]
  0015a538: ldr r1,[sp,#0x88]
  0015a53a: ldr r0,[r0,#0x0]
  0015a53c: ldr.w r1,[r1,#0xf4]
  0015a540: ldr r2,[r0,#0x28]
  0015a542: ldr r0,[r0,#0x70]
  0015a544: add.w r0,r0,r0, lsr #0x1f
  0015a548: ldr r3,[sp,#0x74]
  0015a54a: add.w r0,r3,r0, asr #0x1
  0015a54e: adds r0,#0x1
  0015a550: str r0,[sp,#0x0]
  0015a552: movs r0,#0x0
  0015a554: str r0,[sp,#0x4]
  0015a556: ldr r0,[sp,#0x68]
  0015a558: add r0,r2
  0015a55a: mov r2,r11
  0015a55c: adds r3,r0,r1
  0015a55e: mov r0,r5
  0015a560: mov r1,r9
  0015a562: blx 0x0006fe14
  0015a566: ldr r0,[0x0015a814]
  0015a568: add r0,pc
  0015a56a: ldr r4,[r0,#0x0]
  0015a56c: mov r0,r11
  0015a56e: blx r4
  0015a570: add r0,sp,#0x120
  0015a572: blx r4
  0015a574: mov r0,r10
  0015a576: blx r4
  0015a578: ldr.w r0,[r8,#0x0]
  0015a57c: mov.w r1,#0xffffffff
  0015a580: blx 0x0006fac0
  0015a584: ldr r0,[0x0015a818]
  0015a586: ldr r1,[0x0015a81c]
  0015a588: add r0,pc
  0015a58a: ldr r2,[sp,#0x78]
  0015a58c: add r1,pc
  0015a58e: ldr r0,[r0,#0x0]
  0015a590: ldr r2,[r2,#0x18]
  0015a592: ldr r1,[r1,#0x0]
  0015a594: ldr r0,[r0,#0x0]
  0015a596: ldr r4,[r2,#0x14]
  0015a598: ldr r5,[r1,#0x0]
  0015a59a: ldr r0,[r0,#0x4]
  0015a59c: ldr.w r0,[r0,r4,lsl #0x2]
  0015a5a0: blx 0x000718fc
  0015a5a4: ldr r1,[sp,#0x88]
  0015a5a6: mov r2,r0
  0015a5a8: ldr r3,[sp,#0x84]
  0015a5aa: ldr r6,[sp,#0x74]
  0015a5ac: ldr.w r0,[r1,#0xf4]
  0015a5b0: ldr.w r1,[r1,#0x118]
  0015a5b4: ldr r3,[r3,#0x0]
  0015a5b6: add r1,r6
  0015a5b8: ldr r3,[r3,#0x28]
  0015a5ba: str r1,[sp,#0x0]
  0015a5bc: ldr r1,[sp,#0x60]
  0015a5be: add r1,r3
  0015a5c0: adds r3,r1,r0
  0015a5c2: mov r0,r5
  0015a5c4: mov r1,r4
  0015a5c6: blx 0x00076978
  0015a5ca: b.w 0x00159ad6
  0015a5ce: ldr r0,[sp,#0x78]
  0015a5d0: blx 0x00077068
  0015a5d4: cbz r0,0x0015a60e
  0015a5d6: ldr r1,[sp,#0x88]
  0015a5d8: ldr r2,[sp,#0x84]
  0015a5da: ldr r0,[r1,#0x24]
  0015a5dc: ldr r2,[r2,#0x0]
  0015a5de: ldr.w r1,[r1,#0xf4]
  0015a5e2: ldr r0,[r0,#0x4]
  0015a5e4: ldr r2,[r2,#0x28]
  0015a5e6: ldr r0,[r0,#0x14]
  0015a5e8: add r1,r2
  0015a5ea: ldr r2,[sp,#0x74]
  0015a5ec: movs r3,#0x11
  0015a5ee: blx 0x00076d80
  0015a5f2: ldr r0,[sp,#0x58]
  0015a5f4: ldr r0,[r0,#0x0]
  0015a5f6: ldr r0,[r0,#0x4]
  0015a5f8: ldr r0,[r0,#0x14]
  0015a5fa: movs r1,#0x1
  0015a5fc: blx 0x00074e48
  0015a600: ldr r0,[sp,#0x58]
  0015a602: ldr r0,[r0,#0x0]
  0015a604: ldr r0,[r0,#0x4]
  0015a606: ldr r0,[r0,#0x14]
  0015a608: blx 0x000746e0
  0015a60c: b 0x0015a734
  0015a60e: ldr r0,[sp,#0x84]
  0015a610: ldr r4,[r0,#0x0]
  0015a612: ldr r0,[sp,#0x88]
  0015a614: ldr r5,[r4,#0x1c]
  0015a616: ldr.w r8,[r0,#0xf4]
  0015a61a: ldr r0,[sp,#0x78]
  0015a61c: ldr r6,[r4,#0x28]
  0015a61e: ldr.w r9,[r4,#0x70]
  0015a622: ldr r1,[r0,#0x1c]
  0015a624: add r0,sp,#0x9c
  0015a626: movs r2,#0x0
  0015a628: blx 0x0006f028
  0015a62c: ldr r0,[sp,#0x80]
  0015a62e: add.w r2,r8,r6
  0015a632: strd r0,r5,[sp,#0x0]
  0015a636: movs r1,#0x0
  0015a638: ldr r0,[sp,#0x74]
  0015a63a: add.w r10,sp,#0x9c
  0015a63e: str.w r10,[sp,#0x8]
  0015a642: add r0,r9
  0015a644: subs r3,r0,r5
  0015a646: mov r0,r4
  0015a648: blx 0x0007462c
  0015a64c: mov r0,r10
  0015a64e: blx 0x0006ee30
  0015a652: b 0x0015a734
  0015a654: mov.w r8,#0x0
  0015a658: movs r5,#0x0
  0015a65a: ldr r0,[sp,#0x78]
  0015a65c: ldr r0,[r0,#0x3c]
  0015a65e: cmp.w r0,#0xffffffff
  0015a662: ble 0x0015a750
  0015a664: ldr r3,[sp,#0x78]
  0015a666: movs r4,#0x3
  0015a668: ldr.w r0,[r9,#0x24]
  0015a66c: ldr.w r2,[r9,#0x114]
  0015a670: ldr r3,[r3,#0x3c]
  0015a672: ldr r5,[sp,#0x74]
  0015a674: ldr.w r6,[r10,#0x0]
  0015a678: ldr r0,[r0,#0x4]
  0015a67a: add r2,r5
  0015a67c: ldr.w r1,[r9,#0xf4]
  0015a680: add.w r4,r4,r3, lsr #0x1f
  0015a684: ldrd r5,r6,[r6,#0x28]
  0015a688: ldr r3,[sp,#0x80]
  0015a68a: ldr.w r0,[r0,r4,lsl #0x2]
  0015a68e: add r3,r5
  0015a690: add r1,r3
  0015a692: sub.w r1,r1,r6, lsl #0x1
  0015a696: movs r3,#0x12
  0015a698: blx 0x00076d80
  0015a69c: ldr r5,[sp,#0x58]
  0015a69e: ldr r0,[r5,#0x0]
  0015a6a0: ldr r0,[r0,#0x4]
  0015a6a2: ldr.w r0,[r0,r4,lsl #0x2]
  0015a6a6: movs r1,#0x1
  0015a6a8: blx 0x00074e48
  0015a6ac: ldr r0,[r5,#0x0]
  0015a6ae: ldr r0,[r0,#0x4]
  0015a6b0: ldr.w r0,[r0,r4,lsl #0x2]
  0015a6b4: blx 0x000746e0
  0015a6b8: ldr r0,[r5,#0x0]
  0015a6ba: ldr r0,[r0,#0x4]
  0015a6bc: ldr.w r0,[r0,r4,lsl #0x2]
  0015a6c0: blx 0x00074da0
  0015a6c4: ldr.w r1,[r10,#0x0]
  0015a6c8: ldr r1,[r1,#0x2c]
  0015a6ca: add.w r0,r0,r1, lsl #0x1
  0015a6ce: str r0,[sp,#0x50]
  0015a6d0: ldr r0,[sp,#0x78]
  0015a6d2: blx 0x000768e8
  0015a6d6: cbnz r0,0x0015a6f0
  0015a6d8: ldr r0,[sp,#0x78]
  0015a6da: blx 0x00076204
  0015a6de: cbnz r0,0x0015a6f0
  0015a6e0: ldr r0,[sp,#0x78]
  0015a6e2: blx 0x000768f4
  0015a6e6: cbnz r0,0x0015a6f0
  0015a6e8: ldr r0,[sp,#0x78]
  0015a6ea: blx 0x00076900
  0015a6ee: cbz r0,0x0015a734
  0015a6f0: ldr r2,[sp,#0x88]
  0015a6f2: ldr r3,[sp,#0x84]
  0015a6f4: ldr r6,[sp,#0x74]
  0015a6f6: ldr r0,[r2,#0x24]
  0015a6f8: ldr r3,[r3,#0x0]
  0015a6fa: ldr.w r1,[r2,#0xf4]
  0015a6fe: ldr.w r2,[r2,#0x114]
  0015a702: ldr r0,[r0,#0x4]
  0015a704: add r2,r6
  0015a706: ldr r6,[sp,#0x80]
  0015a708: ldr r5,[sp,#0x50]
  0015a70a: ldr r3,[r3,#0x28]
  0015a70c: subs r6,r6,r5
  0015a70e: ldr r0,[r0,#0x0]
  0015a710: add r3,r6
  0015a712: add r1,r3
  0015a714: movs r3,#0x12
  0015a716: blx 0x00076d80
  0015a71a: ldr r0,[sp,#0x58]
  0015a71c: ldr r0,[r0,#0x0]
  0015a71e: ldr r0,[r0,#0x4]
  0015a720: ldr r0,[r0,#0x0]
  0015a722: movs r1,#0x1
  0015a724: blx 0x00074e48
  0015a728: ldr r0,[sp,#0x58]
  0015a72a: ldr r0,[r0,#0x0]
  0015a72c: ldr r0,[r0,#0x4]
  0015a72e: ldr r0,[r0,#0x0]
  0015a730: blx 0x000746e0
  0015a734: add r0,sp,#0x138
  0015a736: blx 0x0006ee30
  0015a73a: ldr.w r8,[sp,#0x8c]
  0015a73e: ldrd r10,r11,[sp,#0x84]
  0015a742: ldr r5,[sp,#0x64]
  0015a744: ldr.w r12,[sp,#0x54]
  0015a748: ldr r4,[sp,#0x5c]
  0015a74a: adds r4,#0x1
  0015a74c: b.w 0x0015930a
  0015a750: ldr r0,[r6,#0x0]
  0015a752: blx 0x00071a58
  0015a756: mov r4,r0
  0015a758: ldr r0,[sp,#0x78]
  0015a75a: ldr r0,[r0,#0x10]
  0015a75c: blx 0x000718fc
  0015a760: mov r1,r0
  0015a762: mov r0,r4
  0015a764: blx 0x000733e4
  0015a768: clz r0,r0
  0015a76c: mvn.w r1,r8
  0015a770: lsrs r0,r0,#0x5
  0015a772: tst r1,r0
  0015a774: bne 0x0015a6d0
  0015a776: cmp r5,#0x0
  0015a778: beq.w 0x0015a664
  0015a77c: ldr r0,[sp,#0x78]
  0015a77e: ldr r0,[r0,#0x10]
  0015a780: blx 0x0007702c
  0015a784: cmp r0,#0x0
  0015a786: bne.w 0x0015a664
  0015a78a: b 0x0015a6d0
  0015a78c: ldr r0,[sp,#0x7c]
  0015a78e: ldrd r6,r8,[sp,#0x40]
  0015a792: cmp r0,#0x0
  0015a794: itt le
  0015a796: ldr.le r0,[sp,#0x48]
  0015a798: cmp.le r0,#0x1
  0015a79a: blt 0x0015a7d0
  0015a79c: ldr.w r0,[r10,#0x0]
  0015a7a0: ldr r1,[sp,#0x38]
  0015a7a2: ldr.w r12,[r11,#0xf4]
  0015a7a6: ldr r4,[r0,#0x28]
  0015a7a8: ldr r2,[r1,#0x0]
  0015a7aa: ldr r1,[r0,#0x48]
  0015a7ac: ldr.w lr,[r0,#0xc]
  0015a7b0: subs r1,r2,r1
  0015a7b2: ldr.w r9,[r0,#0x20]
  0015a7b6: subs r1,r1,r4
  0015a7b8: ldr.w r3,[r11,#0xd8]
  0015a7bc: ldr r5,[sp,#0x48]
  0015a7be: sub.w r1,r1,r12
  0015a7c2: add.w r2,r9,lr
  0015a7c6: str r5,[sp,#0x0]
  0015a7c8: ldr r5,[sp,#0x7c]
  0015a7ca: str r5,[sp,#0x4]
  0015a7cc: blx 0x00075b5c
  0015a7d0: ldr r0,[0x0015a820]
  0015a7d2: movs r1,#0xa7
  0015a7d4: ldr.w r4,[r10,#0x0]
  0015a7d8: add r0,pc
  0015a7da: ldr r0,[r0,#0x0]
  0015a7dc: ldr r0,[r0,#0x0]
  0015a7de: blx 0x00072f70
  0015a7e2: add r5,sp,#0x90
  0015a7e4: mov r1,r0
  0015a7e6: movs r2,#0x0
  0015a7e8: mov r0,r5
  0015a7ea: blx 0x0006f028
  0015a7ee: mov r0,r4
  0015a7f0: mov r1,r5
  0015a7f2: blx 0x000746d4
  0015a7f6: add r0,sp,#0x90
  0015a7f8: blx 0x0006ee30
  0015a7fc: movs r4,#0x0
  0015a7fe: b 0x0015a830
  0015a824: ldr r0,[r0,#0x4]
  0015a826: ldr.w r0,[r0,r4,lsl #0x2]
  0015a82a: blx 0x000746e0
  0015a82e: adds r4,#0x1
  0015a830: ldr.w r0,[r11,#0x4]
  0015a834: ldr r1,[r0,#0x0]
  0015a836: cmp r4,r1
  0015a838: bcc 0x0015a824
  0015a83a: ldr r0,[r6,#0x0]
  0015a83c: cmp r0,#0x1
  0015a83e: bne 0x0015a85a
  0015a840: movs r0,#0x0
  0015a842: str.w r0,[r11,#0x58]
  0015a846: mov r0,r11
  0015a848: blx 0x00075598
  0015a84c: ldr.w r0,[r11,#0x18]
  0015a850: movs r1,#0x1
  0015a852: str.w r1,[r11,#0x58]
  0015a856: blx 0x00077074
  0015a85a: ldr r0,[0x0015aa7c]
  0015a85c: add r0,pc
  0015a85e: ldr.w r10,[r0,#0x0]
  0015a862: ldr.w r0,[r10,#0x0]
  0015a866: blx 0x00075b8c
  0015a86a: mov r9,r11
  0015a86c: movs r1,#0x1
  0015a86e: ldr.w r0,[r9,#0x24]!
  0015a872: ldr r0,[r0,#0x4]
  0015a874: ldr r0,[r0,#0x2c]
  0015a876: blx 0x00074e48
  0015a87a: ldr r0,[0x0015aa80]
  0015a87c: ldr.w r1,[r9,#0x0]
  0015a880: add r0,pc
  0015a882: ldr r2,[r0,#0x0]
  0015a884: ldr r0,[r1,#0x4]
  0015a886: str r2,[sp,#0x7c]
  0015a888: ldrb.w r1,[r2,#0x4e]
  0015a88c: ldr r0,[r0,#0x2c]
  0015a88e: clz r1,r1
  0015a892: lsrs r1,r1,#0x5
  0015a894: blx 0x00075acc
  0015a898: ldr.w r0,[r9,#0x0]
  0015a89c: ldr r1,[0x0015aa84]
  0015a89e: ldr.w r5,[r10,#0x0]
  0015a8a2: add r1,pc
  0015a8a4: ldr r0,[r0,#0x4]
  0015a8a6: ldr r1,[r1,#0x0]
  0015a8a8: ldr r4,[r0,#0x2c]
  0015a8aa: ldr r0,[r1,#0x0]
  0015a8ac: blx 0x000733d8
  0015a8b0: add r6,sp,#0x138
  0015a8b2: mov r2,r0
  0015a8b4: mov r1,r5
  0015a8b6: mov r0,r6
  0015a8b8: blx 0x00074e54
  0015a8bc: mov r0,r4
  0015a8be: mov r1,r6
  0015a8c0: blx 0x00075460
  0015a8c4: add r0,sp,#0x138
  0015a8c6: blx 0x0006ee30
  0015a8ca: ldr.w r0,[r11,#0x24]
  0015a8ce: ldr r0,[r0,#0x4]
  0015a8d0: ldr r0,[r0,#0x2c]
  0015a8d2: blx 0x000746e0
  0015a8d6: ldrb.w r0,[r11,#0x3c]
  0015a8da: cmp r0,#0x0
  0015a8dc: beq.w 0x0015b4a0
  0015a8e0: ldr.w r0,[r11,#0x20]
  0015a8e4: blx 0x000746ec
  0015a8e8: ldrb.w r0,[r11,#0xae]
  0015a8ec: str.w r9,[sp,#0x84]
  0015a8f0: cmp r0,#0x0
  0015a8f2: beq.w 0x0015ac74
  0015a8f6: ldr.w r0,[r8,#0x0]
  0015a8fa: blx 0x00071704
  0015a8fe: str r0,[sp,#0x68]
  0015a900: ldrb.w r0,[r11,#0x11f]
  0015a904: cmp r0,#0x0
  0015a906: beq.w 0x0015aecc
  0015a90a: ldr r4,[0x0015aa88]
  0015a90c: vmov.f32 s16,0xbf000000
  0015a910: ldr r0,[0x0015aa8c]
  0015a912: add r6,sp,#0x138
  0015a914: add r4,pc
  0015a916: add r5,sp,#0x12c
  0015a918: add r0,pc
  0015a91a: add.w r8,sp,#0x120
  0015a91e: vldr.32 s18,[pc,#0x170]
  0015a922: mov.w r9,#0x0
  0015a926: ldr r0,[r0,#0x0]
  0015a928: mov.w r11,#0x11
  0015a92c: str r0,[sp,#0x78]
  0015a92e: ldr r0,[0x0015aa94]
  0015a930: str r4,[sp,#0x6c]
  0015a932: add r0,pc
  0015a934: ldr r0,[r0,#0x0]
  0015a936: str r0,[sp,#0x74]
  0015a938: b 0x0015ac66
  0015a93a: mov r0,r6
  0015a93c: mov r1,r4
  0015a93e: movs r2,#0x0
  0015a940: blx 0x0006ee18
  0015a944: mov r0,r5
  0015a946: mov r1,r4
  0015a948: movs r2,#0x0
  0015a94a: blx 0x0006ee18
  0015a94e: ldr r0,[sp,#0x84]
  0015a950: ldr r0,[r0,#0x0]
  0015a952: ldr r0,[r0,#0x4]
  0015a954: add.w r0,r0,r9, lsl #0x2
  0015a958: ldr r0,[r0,#0x30]
  0015a95a: movs r1,#0x1
  0015a95c: blx 0x00074e48
  0015a960: bic r0,r9,#0x80000000
  0015a964: cmp r0,#0x4
  0015a966: bhi 0x0015a9aa
  0015a968: tbb [pc,r0]
  0015a972: ldr r0,[0x0015aa98]
  0015a974: add r0,pc
  0015a976: ldr r0,[r0,#0x0]
  0015a978: ldr r1,[r0,#0x0]
  0015a97a: mov r0,r8
  0015a97c: movs r2,#0x0
  0015a97e: blx 0x0006ee18
  0015a982: mov r0,r6
  0015a984: mov r1,r8
  0015a986: blx 0x0006f2b0
  0015a98a: mov r0,r8
  0015a98c: blx 0x0006ee30
  0015a990: ldr r0,[0x0015aa9c]
  0015a992: add r0,pc
  0015a994: ldr r0,[r0,#0x0]
  0015a996: ldr r1,[r0,#0x0]
  0015a998: mov r0,r8
  0015a99a: movs r2,#0x0
  0015a99c: blx 0x0006ee18
  0015a9a0: mov r0,r5
  0015a9a2: mov r1,r8
  0015a9a4: blx 0x0006f2b0
  0015a9a8: b 0x0015aaee
  0015a9aa: mov r0,r8
  0015a9ac: mov r1,r4
  0015a9ae: movs r2,#0x0
  0015a9b0: blx 0x0006ee18
  0015a9b4: mov r0,r6
  0015a9b6: mov r1,r8
  0015a9b8: blx 0x0006f2b0
  0015a9bc: mov r0,r8
  0015a9be: blx 0x0006ee30
  0015a9c2: mov r1,r4
  0015a9c4: movs r2,#0x0
  0015a9c6: blx 0x0006ee18
  0015a9ca: mov r0,r5
  0015a9cc: mov r1,r8
  0015a9ce: blx 0x0006f2b0
  0015a9d2: b 0x0015aaee
  0015a9d4: ldr r0,[0x0015aaa0]
  0015a9d6: add r0,pc
  0015a9d8: ldr r0,[r0,#0x0]
  0015a9da: ldr r1,[r0,#0x0]
  0015a9dc: mov r0,r8
  0015a9de: movs r2,#0x0
  0015a9e0: blx 0x0006ee18
  0015a9e4: mov r0,r6
  0015a9e6: mov r1,r8
  0015a9e8: blx 0x0006f2b0
  0015a9ec: mov r0,r8
  0015a9ee: blx 0x0006ee30
  0015a9f2: ldr r0,[0x0015aaa4]
  0015a9f4: add r0,pc
  0015a9f6: ldr r0,[r0,#0x0]
  0015a9f8: ldr r1,[r0,#0x0]
  0015a9fa: mov r0,r8
  0015a9fc: movs r2,#0x0
  0015a9fe: blx 0x0006ee18
  0015aa02: mov r0,r5
  0015aa04: mov r1,r8
  0015aa06: blx 0x0006f2b0
  0015aa0a: b 0x0015aaee
  0015aa0c: ldr r0,[0x0015aaa8]
  0015aa0e: add r0,pc
  0015aa10: ldr r0,[r0,#0x0]
  0015aa12: ldr r1,[r0,#0x0]
  0015aa14: mov r0,r8
  0015aa16: movs r2,#0x0
  0015aa18: blx 0x0006ee18
  0015aa1c: mov r0,r6
  0015aa1e: mov r1,r8
  0015aa20: blx 0x0006f2b0
  0015aa24: mov r0,r8
  0015aa26: blx 0x0006ee30
  0015aa2a: ldr r0,[0x0015aaac]
  0015aa2c: add r0,pc
  0015aa2e: ldr r0,[r0,#0x0]
  0015aa30: ldr r1,[r0,#0x0]
  0015aa32: mov r0,r8
  0015aa34: movs r2,#0x0
  0015aa36: blx 0x0006ee18
  0015aa3a: mov r0,r5
  0015aa3c: mov r1,r8
  0015aa3e: blx 0x0006f2b0
  0015aa42: b 0x0015aaee
  0015aa44: ldr r0,[0x0015aab0]
  0015aa46: add r0,pc
  0015aa48: ldr r0,[r0,#0x0]
  0015aa4a: ldr r1,[r0,#0x0]
  0015aa4c: mov r0,r8
  0015aa4e: movs r2,#0x0
  0015aa50: blx 0x0006ee18
  0015aa54: mov r0,r6
  0015aa56: mov r1,r8
  0015aa58: blx 0x0006f2b0
  0015aa5c: mov r0,r8
  0015aa5e: blx 0x0006ee30
  0015aa62: ldr r0,[0x0015aab4]
  0015aa64: add r0,pc
  0015aa66: ldr r0,[r0,#0x0]
  0015aa68: ldr r1,[r0,#0x0]
  0015aa6a: mov r0,r8
  0015aa6c: movs r2,#0x0
  0015aa6e: blx 0x0006ee18
  0015aa72: mov r0,r5
  0015aa74: mov r1,r8
  0015aa76: blx 0x0006f2b0
  0015aa7a: b 0x0015aaee
  0015aab8: ldr r0,[0x0015ae80]
  0015aaba: add r0,pc
  0015aabc: ldr r0,[r0,#0x0]
  0015aabe: ldr r1,[r0,#0x0]
  0015aac0: mov r0,r8
  0015aac2: movs r2,#0x0
  0015aac4: blx 0x0006ee18
  0015aac8: mov r0,r6
  0015aaca: mov r1,r8
  0015aacc: blx 0x0006f2b0
  0015aad0: mov r0,r8
  0015aad2: blx 0x0006ee30
  0015aad6: ldr r0,[0x0015ae84]
  0015aad8: add r0,pc
  0015aada: ldr r0,[r0,#0x0]
  0015aadc: ldr r1,[r0,#0x0]
  0015aade: mov r0,r8
  0015aae0: movs r2,#0x0
  0015aae2: blx 0x0006ee18
  0015aae6: mov r0,r5
  0015aae8: mov r1,r8
  0015aaea: blx 0x0006f2b0
  0015aaee: mov r0,r8
  0015aaf0: blx 0x0006ee30
  0015aaf4: uxtb.w r0,r9
  0015aaf8: movs r1,#0x3
  0015aafa: blx 0x0006ec2c
  0015aafe: ldr r2,[sp,#0x78]
  0015ab00: ldr r3,[sp,#0x88]
  0015ab02: ldr.w r1,[r10,#0x0]
  0015ab06: ldr r2,[r2,#0x0]
  0015ab08: ldrd lr,r6,[r3,#0x120]
  0015ab0c: ldrd r4,r5,[r3,#0x128]
  0015ab10: vmov s0,r5
  0015ab14: ldr.w r12,[r3,#0x24]
  0015ab18: add.w r2,r2,r2, lsr #0x1f
  0015ab1c: add.w r3,r6,r6, lsr #0x1f
  0015ab20: vcvt.f32.s32 s0,s0
  0015ab24: ldr r1,[r1,#0x20]
  0015ab26: asrs r2,r2,#0x1
  0015ab28: sub.w r2,r2,r3, asr #0x1
  0015ab2c: add.w r3,r4,lr
  0015ab30: sub.w r1,r1,r1, lsl #0x2
  0015ab34: add r1,r2
  0015ab36: vmov s2,r1
  0015ab3a: adds r1,r5,r6
  0015ab3c: muls r1,r0
  0015ab3e: vcvt.f32.s32 s2,s2
  0015ab42: add.w r0,r0,r0, lsl #0x1
  0015ab46: sub.w r0,r9,r0
  0015ab4a: uxtb r0,r0
  0015ab4c: vmov s4,r1
  0015ab50: vmla.f32 s2,s0,s16
  0015ab54: vcvt.f32.s32 s4,s4
  0015ab58: ldr r1,[sp,#0x74]
  0015ab5a: ldr.w r2,[r12,#0x4]
  0015ab5e: ldr r1,[r1,#0x0]
  0015ab60: add.w r1,r1,r1, lsr #0x1f
  0015ab64: vadd.f32 s0,s2,s4
  0015ab68: rsb r1,lr,r1, asr #0x1
  0015ab6c: subs r1,r1,r4
  0015ab6e: mla r6,r3,r0,r1
  0015ab72: add.w r0,r2,r9, lsl #0x2
  0015ab76: vcvt.s32.f32 s20,s0
  0015ab7a: ldr r0,[r0,#0x30]
  0015ab7c: vmov r4,s20
  0015ab80: mov r1,r6
  0015ab82: mov r2,r4
  0015ab84: movs r3,#0x44
  0015ab86: blx 0x00076d80
  0015ab8a: add r5,sp,#0x12c
  0015ab8c: ldr r0,[sp,#0x84]
  0015ab8e: ldr r0,[r0,#0x0]
  0015ab90: ldr r0,[r0,#0x4]
  0015ab92: add.w r0,r0,r9, lsl #0x2
  0015ab96: ldr r0,[r0,#0x30]
  0015ab98: add r1,sp,#0x138
  0015ab9a: blx 0x00077080
  0015ab9e: ldr r0,[sp,#0x84]
  0015aba0: ldr r0,[r0,#0x0]
  0015aba2: ldr r0,[r0,#0x4]
  0015aba4: add.w r0,r0,r9, lsl #0x2
  0015aba8: ldr r0,[r0,#0x30]
  0015abaa: mov r1,r5
  0015abac: blx 0x0007708c
  0015abb0: ldr r0,[sp,#0x84]
  0015abb2: ldr r0,[r0,#0x0]
  0015abb4: ldr r0,[r0,#0x4]
  0015abb6: add.w r0,r0,r9, lsl #0x2
  0015abba: ldr r0,[r0,#0x30]
  0015abbc: blx 0x000746e0
  0015abc0: ldr r0,[sp,#0x88]
  0015abc2: ldr.w r2,[r10,#0x0]
  0015abc6: ldr r0,[r0,#0x30]
  0015abc8: ldr r2,[r2,#0x2c]
  0015abca: ldr.w r1,[r0,r9,lsl #0x2]
  0015abce: ldr r0,[sp,#0x8c]
  0015abd0: ldr r0,[r0,#0x0]
  0015abd2: movs r3,#0x44
  0015abd4: str.w r11,[sp,#0x0]
  0015abd8: str r3,[sp,#0x4]
  0015abda: subs r3,r4,r2
  0015abdc: mov r2,r6
  0015abde: blx 0x00071d70
  0015abe2: cmp.w r9,#0x4
  0015abe6: bne 0x0015ac52
  0015abe8: ldr r0,[0x0015ae88]
  0015abea: add r0,pc
  0015abec: ldr r0,[r0,#0x0]
  0015abee: ldrb r0,[r0,#0x0]
  0015abf0: cbz r0,0x0015ac52
  0015abf2: ldr r0,[0x0015ae8c]
  0015abf4: vmov s4,r6
  0015abf8: ldr r1,[sp,#0x88]
  0015abfa: add r0,pc
  0015abfc: ldr r0,[r0,#0x0]
  0015abfe: vldr.32 s0,[r1,#0x120]
  0015ac02: vldr.32 s2,[r1,#0x124]
  0015ac06: vcvt.f32.s32 s4,s4
  0015ac0a: vcvt.f32.s32 s6,s20
  0015ac0e: vcvt.f32.s32 s2,s2
  0015ac12: vcvt.f32.s32 s0,s0
  0015ac16: ldrb r0,[r0,#0x0]
  0015ac18: cmp r0,#0x0
  0015ac1a: adr.w r0,0x15b6a8
  0015ac1e: it eq
  0015ac20: add.eq r0,#0x4
  0015ac22: ldr r1,[r1,#0x34]
  0015ac24: vldr.32 s8,[r0]
  0015ac28: ldr r0,[sp,#0x8c]
  0015ac2a: vmla.f32 s6,s2,s18
  0015ac2e: vmla.f32 s4,s8,s0
  0015ac32: ldr r0,[r0,#0x0]
  0015ac34: movs r2,#0x11
  0015ac36: mov.w r11,#0x11
  0015ac3a: str r2,[sp,#0x4]
  0015ac3c: vcvt.s32.f32 s0,s6
  0015ac40: vcvt.s32.f32 s2,s4
  0015ac44: str r2,[sp,#0x0]
  0015ac46: vmov r3,s0
  0015ac4a: vmov r2,s2
  0015ac4e: blx 0x00071d70
  0015ac52: mov r0,r5
  0015ac54: blx 0x0006ee30
  0015ac58: add r6,sp,#0x138
  0015ac5a: mov r0,r6
  0015ac5c: blx 0x0006ee30
  0015ac60: add.w r9,r9,#0x1
  0015ac64: ldr r4,[sp,#0x6c]
  0015ac66: cmp.w r9,#0x4
  0015ac6a: bls.w 0x0015a93a
  0015ac6e: ldr.w r11,[sp,#0x88]
  0015ac72: b 0x0015b1d2
  0015ac74: ldrb.w r0,[r11,#0xb0]
  0015ac78: cmp r0,#0x0
  0015ac7a: beq.w 0x0015b4a0
  0015ac7e: ldr.w r1,[r10,#0x0]
  0015ac82: movs r5,#0x0
  0015ac84: ldr.w r0,[r11,#0x20]
  0015ac88: mov.w r11,#0x0
  0015ac8c: vldr.32 s16,[pc,#0x200]
  0015ac90: ldrd r2,r0,[r0,#0x0]
  0015ac94: ldr r3,[r1,#0x8]
  0015ac96: ldrd r6,r1,[r1,#0x28]
  0015ac9a: add r0,r3
  0015ac9c: str.w r10,[sp,#0x80]
  0015aca0: add.w r8,r6,r2
  0015aca4: add.w r0,r0,r1, lsl #0x1
  0015aca8: str r0,[sp,#0x78]
  0015acaa: str.w r8,[sp,#0x6c]
  0015acae: cmp.w r11,#0x4
  0015acb2: bhi.w 0x0015b4a0
  0015acb6: ldr.w r0,[r9,#0x0]
  0015acba: cmp.w r11,#0x0
  0015acbe: ldr r0,[r0,#0x4]
  0015acc0: add.w r0,r0,r11, lsl #0x2
  0015acc4: ldr r4,[r0,#0x48]
  0015acc6: beq 0x0015acf4
  0015acc8: ldr r1,[sp,#0x7c]
  0015acca: cmp.w r11,#0x1
  0015acce: bne 0x0015acd6
  0015acd0: ldrb.w r0,[r1,#0x49]
  0015acd4: cbnz r0,0x0015acf4
  0015acd6: cmp.w r11,#0x2
  0015acda: bne 0x0015ace2
  0015acdc: ldrb.w r0,[r1,#0x4a]
  0015ace0: cbnz r0,0x0015acf4
  0015ace2: cmp.w r11,#0x3
  0015ace6: bne.w 0x0015aeb4
  0015acea: ldrb.w r0,[r1,#0x4b]
  0015acee: cmp r0,#0x0
  0015acf0: beq.w 0x0015aeb4
  0015acf4: movs r6,#0x0
  0015acf6: mov r0,r4
  0015acf8: mov r1,r6
  0015acfa: blx 0x00074e48
  0015acfe: ldr.w r0,[r10,#0x0]
  0015ad02: ldrd r1,r0,[r0,#0x30]
  0015ad06: str r5,[sp,#0x88]
  0015ad08: add r0,r1
  0015ad0a: ldr r1,[sp,#0x78]
  0015ad0c: mla r5,r0,r5,r1
  0015ad10: mov r0,r4
  0015ad12: mov r1,r8
  0015ad14: mov r2,r5
  0015ad16: blx 0x00076498
  0015ad1a: mov r0,r4
  0015ad1c: blx 0x000746e0
  0015ad20: cmp r6,#0x0
  0015ad22: beq.w 0x0015ae3a
  0015ad26: mov r0,r4
  0015ad28: blx 0x00074da0
  0015ad2c: mov r1,r10
  0015ad2e: mov r10,r0
  0015ad30: ldr r0,[r1,#0x0]
  0015ad32: ldr.w r9,[r0,#0x2c]
  0015ad36: mov r0,r4
  0015ad38: blx 0x00076f0c
  0015ad3c: ldr r6,[sp,#0x8c]
  0015ad3e: mov r4,r0
  0015ad40: mov.w r1,#0xffffffff
  0015ad44: ldr r0,[r6,#0x0]
  0015ad46: blx 0x0006fac0
  0015ad4a: vmov s0,r4
  0015ad4e: cmp.w r11,#0x0
  0015ad52: vmov s2,r5
  0015ad56: vcvt.f32.s32 s0,s0
  0015ad5a: vcvt.f32.s32 s2,s2
  0015ad5e: ldr r0,[0x0015ae94]
  0015ad60: ldr r1,[0x0015ae98]
  0015ad62: add r0,pc
  0015ad64: add r1,pc
  0015ad66: ldr r0,[r0,#0x0]
  0015ad68: ldr r4,[r1,#0x0]
  0015ad6a: vmla.f32 s2,s0,s16
  0015ad6e: ldr r0,[r0,#0x0]
  0015ad70: vcvt.s32.f32 s0,s2
  0015ad74: ldr r1,[r4,#0x0]
  0015ad76: str r1,[sp,#0x74]
  0015ad78: add.w r1,r10,r8
  0015ad7c: ldr r5,[r6,#0x0]
  0015ad7e: add.w r8,r1,r9
  0015ad82: ite ne
  0015ad84: add.ne.w r1,r11,#0x70
  0015ad88: movw.eq r1,#0xd48
  0015ad8c: vmov r6,s0
  0015ad90: blx 0x00072f70
  0015ad94: ldr r1,[sp,#0x74]
  0015ad96: mov r2,r0
  0015ad98: movs r0,#0x0
  0015ad9a: mov r3,r8
  0015ad9c: str r0,[sp,#0x4]
  0015ad9e: mov r0,r5
  0015ada0: str r6,[sp,#0x0]
  0015ada2: blx 0x0006fe14
  0015ada6: ldr r5,[sp,#0x8c]
  0015ada8: ldr r1,[0x0015ae9c]
  0015adaa: ldr r0,[r5,#0x0]
  0015adac: blx 0x0006fac0
  0015adb0: ldr.w r10,[r4,#0x0]
  0015adb4: cmp.w r11,#0x0
  0015adb8: ldr.w r9,[r5,#0x0]
  0015adbc: beq 0x0015ae3e
  0015adbe: ldr r1,[0x0015aea0]
  0015adc0: add r5,sp,#0x120
  0015adc2: movs r2,#0x0
  0015adc4: add r1,pc
  0015adc6: mov r0,r5
  0015adc8: blx 0x0006ee18
  0015adcc: ldr r0,[0x0015aea4]
  0015adce: ldr r1,[sp,#0x80]
  0015add0: add r0,pc
  0015add2: ldr r1,[r1,#0x0]
  0015add4: ldr.w r2,[r0,r11,lsl #0x2]
  0015add8: add r4,sp,#0x114
  0015adda: mov r0,r4
  0015addc: blx 0x00074e54
  0015ade0: add r0,sp,#0x12c
  0015ade2: mov r1,r5
  0015ade4: mov r2,r4
  0015ade6: blx 0x0006ef98
  0015adea: ldr r1,[0x0015aea8]
  0015adec: add r1,pc
  0015adee: add r5,sp,#0xd8
  0015adf0: movs r2,#0x0
  0015adf2: mov r0,r5
  0015adf4: blx 0x0006ee18
  0015adf8: add r4,sp,#0x138
  0015adfa: add r1,sp,#0x12c
  0015adfc: mov r2,r5
  0015adfe: mov r0,r4
  0015ae00: blx 0x0006ef98
  0015ae04: ldr r0,[sp,#0x80]
  0015ae06: ldr r0,[r0,#0x0]
  0015ae08: ldr r0,[r0,#0x4]
  0015ae0a: add r0,r6
  0015ae0c: str r0,[sp,#0x0]
  0015ae0e: movs r0,#0x0
  0015ae10: mov r1,r10
  0015ae12: str r0,[sp,#0x4]
  0015ae14: mov r0,r9
  0015ae16: mov r2,r4
  0015ae18: mov r3,r8
  0015ae1a: blx 0x0006fe14
  0015ae1e: mov r0,r4
  0015ae20: ldr r1,[0x0015aeac]
  0015ae22: add r1,pc
  0015ae24: ldr r4,[r1,#0x0]
  0015ae26: blx r4
  0015ae28: add r0,sp,#0xd8
  0015ae2a: blx r4
  0015ae2c: add r0,sp,#0x12c
  0015ae2e: blx r4
  0015ae30: add r0,sp,#0x114
  0015ae32: blx r4
  0015ae34: add r0,sp,#0x120
  0015ae36: blx r4
  0015ae38: b 0x0015ae6c
  0015ae3a: ldr r5,[sp,#0x88]
  0015ae3c: b 0x0015ae78
  0015ae3e: ldr r1,[0x0015aeb0]
  0015ae40: add r4,sp,#0x138
  0015ae42: movs r2,#0x0
  0015ae44: add r1,pc
  0015ae46: mov r0,r4
  0015ae48: blx 0x0006ee18
  0015ae4c: ldr r0,[sp,#0x80]
  0015ae4e: ldr r0,[r0,#0x0]
  0015ae50: ldr r0,[r0,#0x4]
  0015ae52: add r0,r6
  0015ae54: str r0,[sp,#0x0]
  0015ae56: movs r0,#0x0
  0015ae58: mov r1,r10
  0015ae5a: str r0,[sp,#0x4]
  0015ae5c: mov r0,r9
  0015ae5e: mov r2,r4
  0015ae60: mov r3,r8
  0015ae62: blx 0x0006fe14
  0015ae66: mov r0,r4
  0015ae68: blx 0x0006ee30
  0015ae6c: ldr r5,[sp,#0x88]
  0015ae6e: ldrd r10,r9,[sp,#0x80]
  0015ae72: ldr.w r8,[sp,#0x6c]
  0015ae76: adds r5,#0x1
  0015ae78: add.w r11,r11,#0x1
  0015ae7c: b 0x0015acae
  0015aeb4: ldrb.w r0,[r1,#0x4c]
  0015aeb8: subs.w r1,r11,#0x4
  0015aebc: it ne
  0015aebe: mov.ne r1,#0x1
  0015aec0: clz r0,r0
  0015aec4: lsrs r0,r0,#0x5
  0015aec6: orr.w r6,r1,r0
  0015aeca: b 0x0015acf6
  0015aecc: ldrd r0,r1,[r11,#0x20]
  0015aed0: ldr r1,[r1,#0x4]
  0015aed2: ldr.w r2,[r10,#0x0]
  0015aed6: ldr r0,[r0,#0x4]
  0015aed8: str r0,[sp,#0x88]
  0015aeda: ldr r0,[r1,#0x30]
  0015aedc: movs r1,#0x1
  0015aede: ldr r6,[r2,#0x8]
  0015aee0: ldr.w r9,[r2,#0x2c]
  0015aee4: blx 0x00074e48
  0015aee8: ldr.w r0,[r11,#0x24]
  0015aeec: add r5,sp,#0x138
  0015aeee: ldr r1,[0x0015af0c]
  0015aef0: movs r2,#0x0
  0015aef2: add r1,pc
  0015aef4: ldr r0,[r0,#0x4]
  0015aef6: ldr r1,[r1,#0x0]
  0015aef8: ldr r4,[r0,#0x30]
  0015aefa: mov r0,r5
  0015aefc: ldr r1,[r1,#0x0]
  0015aefe: blx 0x0006ee18
  0015af02: mov r0,r4
  0015af04: mov r1,r5
  0015af06: blx 0x00075460
  0015af0a: b 0x0015af10
  0015af10: add r0,sp,#0x138
  0015af12: blx 0x0006ee30
  0015af16: ldr.w r8,[sp,#0x84]
  0015af1a: add r5,sp,#0x138
  0015af1c: movs r2,#0x0
  0015af1e: ldr.w r0,[r8,#0x0]
  0015af22: ldr r1,[0x0015af40]
  0015af24: add r1,pc
  0015af26: ldr r0,[r0,#0x4]
  0015af28: ldr r1,[r1,#0x0]
  0015af2a: ldr r4,[r0,#0x30]
  0015af2c: mov r0,r5
  0015af2e: ldr r1,[r1,#0x0]
  0015af30: blx 0x0006ee18
  0015af34: mov r0,r4
  0015af36: mov r1,r5
  0015af38: blx 0x0007708c
  0015af3c: b 0x0015af44
  0015af44: ldr r0,[sp,#0x88]
  0015af46: add.w r1,r9,r9, lsl #0x2
  0015af4a: add r0,r6
  0015af4c: add.w r9,r0,r1
  0015af50: add r0,sp,#0x138
  0015af52: blx 0x0006ee30
  0015af56: ldr r0,[0x0015afb4]
  0015af58: mov r2,r9
  0015af5a: ldr.w r1,[r8,#0x0]
  0015af5e: movs r3,#0x14
  0015af60: add r0,pc
  0015af62: ldr r5,[r0,#0x0]
  0015af64: ldr r0,[r1,#0x4]
  0015af66: ldr r1,[r5,#0x0]
  0015af68: ldr r0,[r0,#0x30]
  0015af6a: add.w r1,r1,r1, lsr #0x1f
  0015af6e: asrs r1,r1,#0x1
  0015af70: blx 0x00076d80
  0015af74: ldr.w r0,[r8,#0x0]
  0015af78: ldr r0,[r0,#0x4]
  0015af7a: ldr r0,[r0,#0x30]
  0015af7c: blx 0x000746e0
  0015af80: ldr.w r0,[r8,#0x0]
  0015af84: movs r1,#0x1
  0015af86: ldr r0,[r0,#0x4]
  0015af88: ldr r0,[r0,#0x34]
  0015af8a: blx 0x00074e48
  0015af8e: ldr.w r0,[r8,#0x0]
  0015af92: add r6,sp,#0x138
  0015af94: ldr r1,[0x0015afb8]
  0015af96: movs r2,#0x0
  0015af98: add r1,pc
  0015af9a: ldr r0,[r0,#0x4]
  0015af9c: ldr r1,[r1,#0x0]
  0015af9e: ldr r4,[r0,#0x34]
  0015afa0: mov r0,r6
  0015afa2: ldr r1,[r1,#0x0]
  0015afa4: blx 0x0006ee18
  0015afa8: mov r0,r4
  0015afaa: mov r1,r6
  0015afac: blx 0x00075460
  0015afb0: b 0x0015afbc
  0015afbc: add r0,sp,#0x138
  0015afbe: blx 0x0006ee30
  0015afc2: ldr.w r0,[r8,#0x0]
  0015afc6: add r6,sp,#0x138
  0015afc8: ldr r1,[0x0015afe8]
  0015afca: movs r2,#0x0
  0015afcc: add r1,pc
  0015afce: ldr r0,[r0,#0x4]
  0015afd0: ldr r1,[r1,#0x0]
  0015afd2: ldr r4,[r0,#0x34]
  0015afd4: mov r0,r6
  0015afd6: ldr r1,[r1,#0x0]
  0015afd8: blx 0x0006ee18
  0015afdc: mov r0,r4
  0015afde: mov r1,r6
  0015afe0: blx 0x0007708c
  0015afe4: b 0x0015afec
  0015afec: add r0,sp,#0x138
  0015afee: blx 0x0006ee30
  0015aff2: ldr.w r0,[r8,#0x0]
  0015aff6: ldr.w r1,[r10,#0x0]
  0015affa: ldr r0,[r0,#0x4]
  0015affc: ldrd r2,r1,[r1,#0x30]
  0015b000: ldr r3,[r5,#0x0]
  0015b002: add r2,r9
  0015b004: ldr r0,[r0,#0x34]
  0015b006: add r2,r1
  0015b008: add.w r1,r3,r3, lsr #0x1f
  0015b00c: movs r3,#0x14
  0015b00e: asrs r1,r1,#0x1
  0015b010: blx 0x00076d80
  0015b014: ldr.w r0,[r8,#0x0]
  0015b018: ldr r0,[r0,#0x4]
  0015b01a: ldr r0,[r0,#0x34]
  0015b01c: blx 0x000746e0
  0015b020: ldr.w r0,[r8,#0x0]
  0015b024: movs r1,#0x1
  0015b026: ldr r0,[r0,#0x4]
  0015b028: ldr r0,[r0,#0x38]
  0015b02a: blx 0x00074e48
  0015b02e: ldr.w r0,[r8,#0x0]
  0015b032: add r6,sp,#0x138
  0015b034: ldr r1,[0x0015b3cc]
  0015b036: movs r2,#0x0
  0015b038: add r1,pc
  0015b03a: ldr r0,[r0,#0x4]
  0015b03c: ldr r1,[r1,#0x0]
  0015b03e: ldr r4,[r0,#0x38]
  0015b040: mov r0,r6
  0015b042: ldr r1,[r1,#0x0]
  0015b044: blx 0x0006ee18
  0015b048: mov r0,r4
  0015b04a: mov r1,r6
  0015b04c: blx 0x00075460
  0015b050: add r0,sp,#0x138
  0015b052: blx 0x0006ee30
  0015b056: ldr.w r0,[r8,#0x0]
  0015b05a: add r6,sp,#0x138
  0015b05c: ldr r1,[0x0015b3d0]
  0015b05e: movs r2,#0x0
  0015b060: add r1,pc
  0015b062: ldr r0,[r0,#0x4]
  0015b064: ldr r1,[r1,#0x0]
  0015b066: ldr r4,[r0,#0x38]
  0015b068: mov r0,r6
  0015b06a: ldr r1,[r1,#0x0]
  0015b06c: blx 0x0006ee18
  0015b070: mov r0,r4
  0015b072: mov r1,r6
  0015b074: blx 0x0007708c
  0015b078: add r0,sp,#0x138
  0015b07a: blx 0x0006ee30
  0015b07e: ldr.w r0,[r8,#0x0]
  0015b082: ldr.w r1,[r10,#0x0]
  0015b086: ldr r0,[r0,#0x4]
  0015b088: ldrd r2,r1,[r1,#0x30]
  0015b08c: ldr r3,[r5,#0x0]
  0015b08e: add r1,r2
  0015b090: ldr r0,[r0,#0x38]
  0015b092: add.w r2,r9,r1, lsl #0x1
  0015b096: add.w r1,r3,r3, lsr #0x1f
  0015b09a: movs r3,#0x14
  0015b09c: asrs r1,r1,#0x1
  0015b09e: blx 0x00076d80
  0015b0a2: ldr.w r0,[r8,#0x0]
  0015b0a6: ldr r0,[r0,#0x4]
  0015b0a8: ldr r0,[r0,#0x38]
  0015b0aa: blx 0x000746e0
  0015b0ae: ldr.w r0,[r8,#0x0]
  0015b0b2: movs r1,#0x1
  0015b0b4: ldr r0,[r0,#0x4]
  0015b0b6: ldr r0,[r0,#0x3c]
  0015b0b8: blx 0x00074e48
  0015b0bc: ldr.w r0,[r8,#0x0]
  0015b0c0: add r6,sp,#0x138
  0015b0c2: ldr r1,[0x0015b3d4]
  0015b0c4: movs r2,#0x0
  0015b0c6: add r1,pc
  0015b0c8: ldr r0,[r0,#0x4]
  0015b0ca: ldr r1,[r1,#0x0]
  0015b0cc: ldr r4,[r0,#0x3c]
  0015b0ce: mov r0,r6
  0015b0d0: ldr r1,[r1,#0x0]
  0015b0d2: blx 0x0006ee18
  0015b0d6: mov r0,r4
  0015b0d8: mov r1,r6
  0015b0da: blx 0x00075460
  0015b0de: add r0,sp,#0x138
  0015b0e0: blx 0x0006ee30
  0015b0e4: ldr.w r0,[r8,#0x0]
  0015b0e8: add r6,sp,#0x138
  0015b0ea: ldr r1,[0x0015b3d8]
  0015b0ec: movs r2,#0x0
  0015b0ee: add r1,pc
  0015b0f0: ldr r0,[r0,#0x4]
  0015b0f2: ldr r1,[r1,#0x0]
  0015b0f4: ldr r4,[r0,#0x3c]
  0015b0f6: mov r0,r6
  0015b0f8: ldr r1,[r1,#0x0]
  0015b0fa: blx 0x0006ee18
  0015b0fe: mov r0,r4
  0015b100: mov r1,r6
  0015b102: blx 0x0007708c
  0015b106: add r0,sp,#0x138
  0015b108: blx 0x0006ee30
  0015b10c: ldr.w r0,[r8,#0x0]
  0015b110: ldr.w r1,[r10,#0x0]
  0015b114: ldr r0,[r0,#0x4]
  0015b116: ldrd r2,r1,[r1,#0x30]
  0015b11a: ldr r3,[r5,#0x0]
  0015b11c: add r1,r2
  0015b11e: ldr r0,[r0,#0x3c]
  0015b120: add.w r1,r1,r1, lsl #0x1
  0015b124: add.w r2,r1,r9
  0015b128: add.w r1,r3,r3, lsr #0x1f
  0015b12c: movs r3,#0x14
  0015b12e: asrs r1,r1,#0x1
  0015b130: blx 0x00076d80
  0015b134: ldr.w r0,[r8,#0x0]
  0015b138: ldr r0,[r0,#0x4]
  0015b13a: ldr r0,[r0,#0x3c]
  0015b13c: blx 0x000746e0
  0015b140: ldr.w r0,[r8,#0x0]
  0015b144: movs r1,#0x1
  0015b146: ldr r0,[r0,#0x4]
  0015b148: ldr r0,[r0,#0x40]
  0015b14a: blx 0x00074e48
  0015b14e: ldr.w r0,[r8,#0x0]
  0015b152: add r6,sp,#0x138
  0015b154: ldr r1,[0x0015b3dc]
  0015b156: movs r2,#0x0
  0015b158: add r1,pc
  0015b15a: ldr r0,[r0,#0x4]
  0015b15c: ldr r1,[r1,#0x0]
  0015b15e: ldr r4,[r0,#0x40]
  0015b160: mov r0,r6
  0015b162: ldr r1,[r1,#0x0]
  0015b164: blx 0x0006ee18
  0015b168: mov r0,r4
  0015b16a: mov r1,r6
  0015b16c: blx 0x00075460
  0015b170: add r0,sp,#0x138
  0015b172: blx 0x0006ee30
  0015b176: ldr.w r0,[r8,#0x0]
  0015b17a: add r6,sp,#0x138
  0015b17c: ldr r1,[0x0015b3e0]
  0015b17e: movs r2,#0x0
  0015b180: add r1,pc
  0015b182: ldr r0,[r0,#0x4]
  0015b184: ldr r1,[r1,#0x0]
  0015b186: ldr r4,[r0,#0x40]
  0015b188: mov r0,r6
  0015b18a: ldr r1,[r1,#0x0]
  0015b18c: blx 0x0006ee18
  0015b190: mov r0,r4
  0015b192: mov r1,r6
  0015b194: blx 0x0007708c
  0015b198: add r0,sp,#0x138
  0015b19a: blx 0x0006ee30
  0015b19e: ldr.w r0,[r8,#0x0]
  0015b1a2: ldr.w r1,[r10,#0x0]
  0015b1a6: ldr r0,[r0,#0x4]
  0015b1a8: ldrd r2,r1,[r1,#0x30]
  0015b1ac: ldr r3,[r5,#0x0]
  0015b1ae: add r1,r2
  0015b1b0: ldr r0,[r0,#0x40]
  0015b1b2: add.w r2,r9,r1, lsl #0x2
  0015b1b6: add.w r1,r3,r3, lsr #0x1f
  0015b1ba: movs r3,#0x14
  0015b1bc: asrs r1,r1,#0x1
  0015b1be: blx 0x00076d80
  0015b1c2: ldr.w r0,[r8,#0x0]
  0015b1c6: ldr r0,[r0,#0x4]
  0015b1c8: ldr r0,[r0,#0x40]
  0015b1ca: blx 0x000746e0
  0015b1ce: mov.w r9,#0x5
  0015b1d2: ldr r1,[sp,#0x7c]
  0015b1d4: ldrb.w r0,[r1,#0x4b]
  0015b1d8: cbz r0,0x0015b1fc
  0015b1da: ldrb.w r0,[r1,#0x4a]
  0015b1de: cbz r0,0x0015b1fc
  0015b1e0: ldrb.w r0,[r1,#0x49]
  0015b1e4: cbz r0,0x0015b1fc
  0015b1e6: ldrb.w r0,[r1,#0x4d]
  0015b1ea: cbz r0,0x0015b1fc
  0015b1ec: ldrb.w r0,[r1,#0x4c]
  0015b1f0: cbz r0,0x0015b1fc
  0015b1f2: ldr r0,[sp,#0x68]
  0015b1f4: ldrb r0,[r0,#0x15]
  0015b1f6: cmp r0,#0x0
  0015b1f8: beq.w 0x0015b46a
  0015b1fc: ldr r0,[0x0015b3e4]
  0015b1fe: ldr r1,[0x0015b3e8]
  0015b200: ldr.w r2,[r11,#0x24]
  0015b204: add r0,pc
  0015b206: ldr.w r3,[r11,#0x120]
  0015b20a: add r1,pc
  0015b20c: str r3,[sp,#0x78]
  0015b20e: mov r3,r10
  0015b210: ldr.w r8,[r11,#0x124]
  0015b214: ldrd r10,r5,[r11,#0x128]
  0015b218: ldr r4,[r0,#0x0]
  0015b21a: ldr r0,[r2,#0x4]
  0015b21c: ldr r6,[r1,#0x0]
  0015b21e: ldr r1,[r3,#0x0]
  0015b220: ldr r0,[r0,#0x44]
  0015b222: ldr r2,[r4,#0x0]
  0015b224: str r4,[sp,#0x88]
  0015b226: str r6,[sp,#0x7c]
  0015b228: ldr r4,[r6,#0x0]
  0015b22a: ldr r6,[r1,#0x20]
  0015b22c: movs r1,#0x1
  0015b22e: str r3,[sp,#0x80]
  0015b230: str r2,[sp,#0x74]
  0015b232: blx 0x00074e48
  0015b236: add.w r0,r4,r4, lsr #0x1f
  0015b23a: add.w r1,r8,r8, lsr #0x1f
  0015b23e: vmov s0,r5
  0015b242: asrs r0,r0,#0x1
  0015b244: vmov.f32 s16,0xbf000000
  0015b248: sub.w r0,r0,r1, asr #0x1
  0015b24c: sub.w r1,r6,r6, lsl #0x2
  0015b250: add r0,r1
  0015b252: vcvt.f32.s32 s0,s0
  0015b256: vmov s2,r0
  0015b25a: add.w r0,r5,r8
  0015b25e: vcvt.f32.s32 s2,s2
  0015b262: vmov s4,r0
  0015b266: vcvt.f32.s32 s4,s4
  0015b26a: ldr.w r0,[r11,#0x24]
  0015b26e: ldrb.w r1,[r11,#0x11f]
  0015b272: vmla.f32 s2,s0,s16
  0015b276: ldr r0,[r0,#0x4]
  0015b278: cmp r1,#0x0
  0015b27a: vadd.f32 s0,s2,s4
  0015b27e: vcvt.s32.f32 s0,s0
  0015b282: ldr r6,[r0,#0x44]
  0015b284: beq.w 0x0015b3f8
  0015b288: vmov r4,s0
  0015b28c: ldr r0,[sp,#0x74]
  0015b28e: ldr r1,[sp,#0x78]
  0015b290: movs r3,#0x44
  0015b292: add.w r0,r0,r0, lsr #0x1f
  0015b296: rsb r0,r1,r0, asr #0x1
  0015b29a: add r1,r10
  0015b29c: sub.w r0,r0,r10
  0015b2a0: add.w r5,r0,r1, lsl #0x1
  0015b2a4: mov r0,r6
  0015b2a6: mov r1,r5
  0015b2a8: mov r2,r4
  0015b2aa: blx 0x00076d80
  0015b2ae: ldr r0,[0x0015b3ec]
  0015b2b0: ldr.w r1,[r11,#0x24]
  0015b2b4: add r0,pc
  0015b2b6: ldr r0,[r0,#0x0]
  0015b2b8: ldr r1,[r1,#0x4]
  0015b2ba: ldr r0,[r0,#0x0]
  0015b2bc: ldr r6,[r1,#0x44]
  0015b2be: movs r1,#0x76
  0015b2c0: blx 0x00072f70
  0015b2c4: mov r1,r0
  0015b2c6: mov r0,r6
  0015b2c8: blx 0x00077080
  0015b2cc: ldr.w r0,[r11,#0x24]
  0015b2d0: ldr r0,[r0,#0x4]
  0015b2d2: ldr r0,[r0,#0x44]
  0015b2d4: blx 0x000746e0
  0015b2d8: ldr.w r10,[sp,#0x80]
  0015b2dc: movs r3,#0x11
  0015b2de: ldr.w r1,[r11,#0x30]
  0015b2e2: ldr.w r8,[sp,#0x8c]
  0015b2e6: ldr.w r2,[r10,#0x0]
  0015b2ea: ldr r1,[r1,#0x14]
  0015b2ec: ldr.w r0,[r8,#0x0]
  0015b2f0: ldr r2,[r2,#0x2c]
  0015b2f2: str r3,[sp,#0x0]
  0015b2f4: movs r3,#0x44
  0015b2f6: str r3,[sp,#0x4]
  0015b2f8: subs r3,r4,r2
  0015b2fa: mov r2,r5
  0015b2fc: blx 0x00071d70
  0015b300: ldr r0,[sp,#0x7c]
  0015b302: add.w r12,r11,#0x120
  0015b306: ldr.w r5,[r10,#0x0]
  0015b30a: vldr.32 s10,[pc,#0xe4]
  0015b30e: ldr r4,[r0,#0x0]
  0015b310: ldm.w r12,{r0,r3,r12}
  0015b314: ldr.w r6,[r11,#0x12c]
  0015b318: add.w r4,r4,r4, lsr #0x1f
  0015b31c: ldr.w r1,[r11,#0x38]
  0015b320: add.w r2,r3,r3, lsr #0x1f
  0015b324: asrs r4,r4,#0x1
  0015b326: vmov s0,r6
  0015b32a: sub.w r2,r4,r2, asr #0x1
  0015b32e: adr.w r4,0x15b6a8
  0015b332: vcvt.f32.s32 s0,s0
  0015b336: ldr r5,[r5,#0x20]
  0015b338: sub.w r5,r5,r5, lsl #0x2
  0015b33c: add r2,r5
  0015b33e: add.w r5,r12,r0
  0015b342: vmov s2,r2
  0015b346: adds r2,r6,r3
  0015b348: vcvt.f32.s32 s2,s2
  0015b34c: vmov s4,r2
  0015b350: vcvt.f32.s32 s4,s4
  0015b354: ldr r2,[0x0015b3f4]
  0015b356: ldr r6,[sp,#0x88]
  0015b358: add r2,pc
  0015b35a: vmla.f32 s2,s0,s16
  0015b35e: ldr r2,[r2,#0x0]
  0015b360: ldr r6,[r6,#0x0]
  0015b362: ldrb r2,[r2,#0x0]
  0015b364: add.w r6,r6,r6, lsr #0x1f
  0015b368: rsb r6,r0,r6, asr #0x1
  0015b36c: cmp r2,#0x0
  0015b36e: vadd.f32 s0,s2,s4
  0015b372: mov.w r2,#0x11
  0015b376: vmov s2,r3
  0015b37a: vmov s4,r0
  0015b37e: sub.w r0,r6,r12
  0015b382: vcvt.f32.s32 s2,s2
  0015b386: add.w r0,r0,r5, lsl #0x1
  0015b38a: vcvt.f32.s32 s4,s4
  0015b38e: vmov s6,r0
  0015b392: vcvt.s32.f32 s0,s0
  0015b396: vcvt.f32.s32 s6,s6
  0015b39a: vcvt.f32.s32 s0,s0
  0015b39e: it eq
  0015b3a0: add.eq r4,#0x4
  0015b3a2: vldr.32 s8,[r4]
  0015b3a6: ldr.w r0,[r8,#0x0]
  0015b3aa: vmla.f32 s6,s8,s4
  0015b3ae: str r2,[sp,#0x4]
  0015b3b0: vmla.f32 s0,s2,s10
  0015b3b4: vcvt.s32.f32 s2,s6
  0015b3b8: vcvt.s32.f32 s0,s0
  0015b3bc: str r2,[sp,#0x0]
  0015b3be: vmov r2,s2
  0015b3c2: vmov r3,s0
  0015b3c6: blx 0x00071d70
  0015b3ca: b 0x0015b448
  0015b3f8: ldr r0,[0x0015b6b0]
  0015b3fa: movs r1,#0x76
  0015b3fc: add r0,pc
  0015b3fe: ldr r0,[r0,#0x0]
  0015b400: ldr r0,[r0,#0x0]
  0015b402: blx 0x00072f70
  0015b406: mov r1,r0
  0015b408: mov r0,r6
  0015b40a: blx 0x00075460
  0015b40e: ldr.w r10,[sp,#0x80]
  0015b412: ldr.w r1,[r11,#0x24]
  0015b416: ldr.w r2,[r11,#0x48]
  0015b41a: ldr.w r0,[r10,#0x0]
  0015b41e: ldrd r3,r0,[r0,#0x30]
  0015b422: ldr r6,[sp,#0x88]
  0015b424: add r3,r0
  0015b426: ldr r1,[r1,#0x4]
  0015b428: mla r2,r3,r9,r2
  0015b42c: ldr r6,[r6,#0x0]
  0015b42e: ldr r0,[r1,#0x44]
  0015b430: movs r3,#0x14
  0015b432: add.w r1,r6,r6, lsr #0x1f
  0015b436: asrs r1,r1,#0x1
  0015b438: blx 0x00076d80
  0015b43c: ldr.w r0,[r11,#0x24]
  0015b440: ldr r0,[r0,#0x4]
  0015b442: ldr r0,[r0,#0x44]
  0015b444: blx 0x000746e0
  0015b448: ldr r0,[0x0015b6b4]
  0015b44a: ldr r1,[sp,#0x84]
  0015b44c: add r0,pc
  0015b44e: ldr r1,[r1,#0x0]
  0015b450: ldr r0,[r0,#0x0]
  0015b452: ldr r1,[r1,#0x4]
  0015b454: ldr r0,[r0,#0x0]
  0015b456: ldr r4,[r1,#0x44]
  0015b458: movs r1,#0x77
  0015b45a: blx 0x00072f70
  0015b45e: mov r1,r0
  0015b460: mov r0,r4
  0015b462: blx 0x0007708c
  0015b466: add.w r9,r9,#0x1
  0015b46a: ldrb.w r0,[r11,#0x11f]
  0015b46e: cbnz r0,0x0015b4a0
  0015b470: ldr.w r0,[r10,#0x0]
  0015b474: add.w r3,r9,#0x1
  0015b478: ldr r1,[0x0015b6b8]
  0015b47a: ldrd r2,r0,[r0,#0x30]
  0015b47e: add r1,pc
  0015b480: ldr.w r4,[r11,#0x20]
  0015b484: add r0,r2
  0015b486: ldr r1,[r1,#0x0]
  0015b488: mul r5,r0,r3
  0015b48c: ldr r0,[sp,#0x8c]
  0015b48e: ldr r1,[r1,#0x0]
  0015b490: ldr r0,[r0,#0x0]
  0015b492: blx 0x0006fe20
  0015b496: add.w r1,r5,r0, lsl #0x1
  0015b49a: mov r0,r4
  0015b49c: blx 0x00076d98
  0015b4a0: ldr r0,[sp,#0x174]
  0015b4a2: ldr r1,[sp,#0x70]
  0015b4a4: ldr r1,[r1,#0x0]
  0015b4a6: subs r0,r1,r0
  0015b4a8: itttt eq
  0015b4aa: add.eq sp,#0x178
  0015b4ac: vpop.eq {d8,d9,d10}
  0015b4b0: add.eq sp,#0x4
  0015b4b2: pop.eq.w {r8,r9,r10,r11}
  0015b4b6: it eq
  0015b4b8: pop.eq {r4,r5,r6,r7,pc}
  0015b4ba: blx 0x0006e824
```
