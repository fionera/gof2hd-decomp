# MGame::OnRender2D
elf 0x1808c0 body 1208
Sig: undefined __thiscall OnRender2D(MGame * this)

## decompile
```c

/* WARNING: Heritage AFTER dead removal. Example location: s1 : 0x001ac770 */
/* WARNING: Restarted to delay deadcode elimination for space: register */
/* MGame::OnRender2D() */

void __thiscall MGame::OnRender2D(MGame *this)

{
  byte bVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  float fVar4;
  int iVar5;
  float fVar6;
  String *pSVar7;
  SingleLevel *pSVar8;
  PlayerEgo *pPVar9;
  int iVar10;
  int *piVar11;
  uint *puVar12;
  Array *pAVar13;
  uint uVar14;
  uint uVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  uint in_fpscr;
  undefined8 in_d0;
  undefined8 in_d1;
  undefined8 in_d2;
  undefined8 in_d3;
  undefined8 in_d4;
  undefined8 in_d5;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined4 uVar19;
  undefined8 local_40;
  undefined4 local_38;
  int local_34;
  
  uVar3 = (ulonglong)in_d1 >> 0x20;
  uVar2 = (ulonglong)in_d0 >> 0x20;
  piVar11 = *(int **)(DAT_00190b34 + 0x1908d4);
  local_34 = *piVar11;
  local_40 = in_d6;
  local_38 = in_s14;
  if ((char)this->field_54 == '\0') goto LAB_00190a84;
  puVar12 = *(uint **)(DAT_00190b38 + 0x1908e8);
  AbyssEngine::PaintCanvas::Begin2d();
  pPVar9 = (PlayerEgo *)in_d0;
  uVar19 = (undefined4)in_d1;
  if ((*(char *)((int)&this->field_5C + 1) == '\0') ||
     (*(char *)((int)&this[1].field_0 + 1) == '\0')) {
    if (*(char *)((int)&this[1].field_48 + 1) == '\0') {
      Level::getStarSystem();
      StarSystem::render2D();
      iVar5 = LevelScript::startSequenceOver();
      if ((iVar5 != 0) ||
         (iVar5 = LevelScript::startSequence((LevelScript *)this->field_7C), iVar5 == 0)) {
        pPVar9 = this->m_pPlayerEgo;
        Radio::draw(CONCAT44(pPVar9,this->field_84),(PlayerEgo *)this->field_7C[2],
                    (LevelScript *)this->field_7C[3]);
      }
LAB_0019099c:
      if (*piVar11 != local_34) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*puVar12);
      }
      (*(code *)(DAT_001ac774 + 0x1ac778))
                (pPVar9,(int)uVar2,uVar19,(int)uVar3,(int)in_d2,(int)((ulonglong)in_d2 >> 0x20),
                 (int)in_d3,(int)((ulonglong)in_d3 >> 0x20),(int)in_d4,
                 (int)((ulonglong)in_d4 >> 0x20),(int)in_d5,(int)((ulonglong)in_d5 >> 0x20),
                 (undefined4)local_40,(int)((ulonglong)in_d6 >> 0x20),local_38);
      return;
    }
    if (this->field_0xc7 != '\0') {
      StarMap::draw();
      if (*(Layout *)**(undefined4 **)(DAT_00190b3c + 0x190962) != (Layout)0x0) {
        Layout::drawHelpWindow((Layout *)**(undefined4 **)(DAT_00190b3c + 0x190962));
      }
      goto LAB_0019099c;
    }
    Level::render2D();
    if (**(int **)(DAT_00190b40 + 0x1909cc) == 0) {
      Hud::drawPauseButton();
    }
    Status::getCampaignMission();
    iVar5 = Mission::getType();
    if (iVar5 == 0xaa) {
      iVar5 = LevelScript::getEvent();
      if (iVar5 == 0) {
        Hud::drawOrbitInformation();
      }
LAB_00190a44:
      Radio::draw(CONCAT44(this->m_pPlayerEgo,this->field_84),(PlayerEgo *)this->field_7C[2],
                  (LevelScript *)this->field_7C[3]);
LAB_00190a58:
      if (*(char *)((int)&this->field_5C + 2) != '\0') {
        DialogueWindow::draw();
      }
      *(undefined1 *)&this[1].field_48 = 0;
      *(undefined1 *)&this->field_5C = 0;
    }
    else {
      if ((*(char *)((int)&this->field_5C + 3) != '\0') || ((char)this[1].field_14 != '\0')) {
        iVar5 = Status::getCurrentCampaignMission();
        if (((7 < iVar5) &&
            ((((char)this->field_7C[4] == '\0' && ((char)this[1].field_14 == '\0')) &&
             (iVar5 = PlayerEgo::isDockingToPlanet(), iVar5 == 0)))) &&
           (iVar5 = LevelScript::getEvent(), iVar5 == 0)) {
          Hud::drawOrbitInformation();
        }
        iVar5 = LevelScript::startSequenceOver();
        if ((iVar5 != 0) ||
           (iVar5 = LevelScript::startSequence((LevelScript *)this->field_7C), iVar5 == 0))
        goto LAB_00190a44;
        goto LAB_00190a58;
      }
      if ((char)this->field_60 == '\0') {
        PlayerEgo::draw(SUB41(this->m_pPlayerEgo,0));
        iVar5 = PlayerEgo::isMining();
        if (((iVar5 == 0) && (this->field_0xc7 == '\0')) &&
           ((iVar5 = PlayerEgo::isHacking(), iVar5 == 0 ||
            (*(char *)((int)&this[1].field_0C + 1) != '\0')))) {
          Radar::draw((Player *)this->field_80,(Hud *)this->m_pPlayerEgo->m_pPlayer,
                      (int)this->m_pLevel);
        }
        if (*(char *)((int)&this->field_5C + 2) == '\0') {
          pSVar8 = this->m_pLevel;
          if (*(char *)((int)&this->field_5C + 1) == '\0') {
            iVar5 = this->field_40;
            iVar10 = iVar5 >> 0x1f;
          }
          else {
            iVar5 = 0;
            iVar10 = 0;
          }
          puVar12 = (uint *)this->field_7C;
          pPVar9 = this->m_pPlayerEgo;
          bVar1 = (byte)this->field_5C;
          uVar17 = *puVar12;
          uVar15 = puVar12[2];
          uVar14 = puVar12[3];
          nextCamId((int)this);
          Hud::draw((ulonglong)CONCAT14(bVar1,pSVar8),CONCAT44(iVar10,iVar5),
                    (PlayerEgo *)(uVar17 - uVar15),
                    (bool)(((char)((int)uVar17 >> 0x1f) - (char)uVar14) - (uVar17 < uVar15)),
                    (uint)pPVar9,(uint)bVar1);
          Radio::draw(CONCAT44(this->m_pPlayerEgo,this->field_84),(PlayerEgo *)this->field_7C[2],
                      (LevelScript *)this->field_7C[3]);
          Radar::drawCurrentLock((Hud *)this->field_80);
          Layout::drawMissionRewardMessage(SUB41(**(undefined4 **)(DAT_00190dc4 + 0x190d4a),0));
        }
        else {
          DialogueWindow::draw();
        }
        if ((((this->field_0xc5 != '\0') || (this->field_0xc6 != '\0')) ||
            (*(char *)((int)&this[1].field_4 + 2) != '\0')) || (this->field_C4 != '\0')) {
          ChoiceWindow::draw();
        }
        if (*(char *)((int)&this[1].field_0C + 2) != '\0') {
          Hud::drawMenu((int)this->m_pLevel);
        }
      }
      else if ((int)(uint)((uint)this->field_48 < 0xbb9) <= this->field_4C) {
        if (this->field_50 < 4000) {
          VectorSignedToFloat(this->field_50,(byte)(in_fpscr >> 0x16) & 3);
        }
        AbyssEngine::PaintCanvas::SetColor((uint)this->field_4);
        AbyssEngine::PaintCanvas::DrawImage2D(*puVar12,this->field_10,0,'\0','D');
        if (3999 < this->field_50) {
          uVar15 = *puVar12;
          AbyssEngine::ApplicationManager::GetSystemTimeMillis();
          fVar6 = (float)__aeabi_l2f();
          fVar4 = DAT_00190d9c;
          AbyssEngine::AEMath::Sinf(fVar6 * DAT_00190d9c);
          AbyssEngine::ApplicationManager::GetSystemTimeMillis();
          fVar6 = (float)__aeabi_l2f();
          AbyssEngine::AEMath::Sinf(fVar6 * fVar4);
          AbyssEngine::PaintCanvas::SetColor((uchar)uVar15,0xff,0xff,0xff);
          pSVar7 = (String *)GameText::getText(**(int **)(DAT_00190da4 + 0x190bf4));
          AbyssEngine::String::String((String *)&local_40,pSVar7,false);
          if (this[2].m_pPlayerEgo == (PlayerEgo *)0x0) {
            uVar15 = *puVar12;
            iVar5 = **(int **)(DAT_00190db8 + 0x190c7c);
            pSVar7 = (String *)**(undefined4 **)(DAT_00190dbc + 0x190c82);
            iVar10 = AbyssEngine::PaintCanvas::GetTextWidth(uVar15,pSVar7);
            iVar18 = **(int **)(DAT_00190dc0 + 0x190ca0);
            iVar16 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar12);
            AbyssEngine::PaintCanvas::DrawString
                      (uVar15,pSVar7,(int)&local_40,(iVar5 >> 1) - (iVar10 >> 1),
                       (bool)((char)(iVar18 >> 1) + (char)(iVar16 >> 1) + '\n'));
          }
          else {
            iVar10 = this[2].field_5C;
            iVar16 = **(int **)(DAT_00190da8 + 0x190c14);
            iVar18 = **(int **)(DAT_00190db4 + 0x190c1e);
            pAVar13 = (Array *)**(undefined4 **)(DAT_00190dac + 0x190c18);
            uVar15 = **(uint **)(DAT_00190db0 + 0x190c1a);
            iVar5 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar12);
            Globals::drawLines(uVar15,pAVar13,iVar10,iVar18 >> 1,
                               (bool)((char)(iVar16 >> 1) + (char)(iVar5 >> 1) + '\n'));
          }
          AbyssEngine::String::~String((String *)&local_40);
        }
      }
    }
    Layout::drawFade((Layout *)**(undefined4 **)(DAT_00190dc8 + 0x190a76));
  }
  else {
    if ((((*(char *)((int)&this[1].field_94 + 2) != '\0') && ((char)this[1].field_94 != '\0')) &&
        (*(char *)((int)&this->m_pDialogueWindow->field_0 + 1) == '\0')) ||
       (MenuTouchWindow::draw(), *(char *)((int)&this[1].field_94 + 2) != '\0')) {
      Level::getStarSystem();
      StarSystem::render2D();
    }
    local_40 = 0x3f0000003f000000;
    local_38 = 0;
    iVar5 = AbyssEngine::ApplicationManager::GetEngine();
    AbyssEngine::AEMath::Vector::operator=((Vector *)(iVar5 + 0x3cc),(Vector *)&local_40);
  }
  AbyssEngine::PaintCanvas::End2d();
LAB_00190a84:
  if (*piVar11 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001908c0: push {r4,r5,r6,r7,lr}
  001908c2: add r7,sp,#0xc
  001908c4: push {r7,r8,r9,r10,r11}
  001908c8: vpush {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  001908cc: mov r4,r0
  001908ce: ldr r0,[0x00190b34]
  001908d0: add r0,pc
  001908d2: ldr r6,[r0,#0x0]
  001908d4: ldr r0,[r6,#0x0]
  001908d6: str r0,[sp,#0x3c]
  001908d8: ldrb.w r0,[r4,#0x54]
  001908dc: cmp r0,#0x0
  001908de: beq.w 0x00190a84
  001908e2: ldr r0,[0x00190b38]
  001908e4: add r0,pc
  001908e6: ldr.w r8,[r0,#0x0]
  001908ea: ldr.w r0,[r8,#0x0]
  001908ee: blx 0x00071d34
  001908f2: ldrb.w r0,[r4,#0x5d]
  001908f6: cbz r0,0x00190948
  001908f8: ldrb.w r0,[r4,#0xc9]
  001908fc: cbz r0,0x00190948
  001908fe: ldrb.w r0,[r4,#0x15e]
  00190902: cbz r0,0x00190912
  00190904: ldrb.w r0,[r4,#0x15c]
  00190908: cbz r0,0x00190912
  0019090a: ldr.w r0,[r4,#0x88]
  0019090e: ldrb r0,[r0,#0x1]
  00190910: cbz r0,0x00190920
  00190912: ldr.w r0,[r4,#0x88]
  00190916: blx 0x000755d4
  0019091a: ldrb.w r0,[r4,#0x15e]
  0019091e: cbz r0,0x0019092a
  00190920: ldr r0,[r4,#0x78]
  00190922: blx 0x000727c0
  00190926: blx 0x00074458
  0019092a: mov.w r0,#0x3f000000
  0019092e: strd r0,r0,[sp,#0x30]
  00190932: movs r0,#0x0
  00190934: str r0,[sp,#0x38]
  00190936: ldr r0,[r4,#0x8]
  00190938: blx 0x0007258c
  0019093c: add.w r0,r0,#0x3cc
  00190940: add r1,sp,#0x30
  00190942: blx 0x0006eb3c
  00190946: b 0x00190a7c
  00190948: ldrb.w r0,[r4,#0x111]
  0019094c: cbz r0,0x0019096e
  0019094e: ldrb.w r0,[r4,#0xc7]
  00190952: cbz r0,0x001909c0
  00190954: ldr.w r0,[r4,#0x90]
  00190958: blx 0x000755bc
  0019095c: ldr r0,[0x00190b3c]
  0019095e: add r0,pc
  00190960: ldr r0,[r0,#0x0]
  00190962: ldr r0,[r0,#0x0]
  00190964: ldrb r1,[r0,#0x0]
  00190966: cbz r1,0x0019099c
  00190968: blx 0x000755ec
  0019096c: b 0x0019099c
  0019096e: ldr r0,[r4,#0x78]
  00190970: blx 0x000727c0
  00190974: blx 0x00074458
  00190978: ldr r0,[r4,#0x7c]
  0019097a: blx 0x00078544
  0019097e: cbnz r0,0x00190988
  00190980: ldr r0,[r4,#0x7c]
  00190982: blx 0x000782d4
  00190986: cbnz r0,0x0019099c
  00190988: ldr r5,[r4,#0x7c]
  0019098a: ldr.w r0,[r4,#0x84]
  0019098e: ldrd r2,r3,[r5,#0x8]
  00190992: ldr r1,[r4,#0x58]
  00190994: strd r1,r5,[sp,#0x0]
  00190998: blx 0x00075610
  0019099c: ldr.w r0,[r8,#0x0]
  001909a0: ldr r1,[sp,#0x3c]
  001909a2: ldr r2,[r6,#0x0]
  001909a4: subs r1,r2,r1
  001909a6: itttt eq
  001909a8: vpop.eq {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  001909ac: add.eq sp,#0x4
  001909ae: pop.eq.w {r8,r9,r10,r11}
  001909b2: pop.eq.w {r4,r5,r6,r7,lr}
  001909b6: it eq
  001909b8: b.eq.w 0x001ac768
  001909bc: blx 0x0006e824
  001909c0: ldr r0,[r4,#0x78]
  001909c2: blx 0x00072208
  001909c6: ldr r0,[0x00190b40]
  001909c8: add r0,pc
  001909ca: ldr r0,[r0,#0x0]
  001909cc: ldr r0,[r0,#0x0]
  001909ce: cbnz r0,0x001909d6
  001909d0: ldr r0,[r4,#0x74]
  001909d2: blx 0x000786c4
  001909d6: ldr r0,[0x00190b44]
  001909d8: add r0,pc
  001909da: ldr r5,[r0,#0x0]
  001909dc: ldr r0,[r5,#0x0]
  001909de: blx 0x0007285c
  001909e2: blx 0x00072874
  001909e6: cmp r0,#0xaa
  001909e8: bne 0x001909fa
  001909ea: ldr r0,[r4,#0x7c]
  001909ec: blx 0x00077320
  001909f0: cbnz r0,0x00190a44
  001909f2: ldr r0,[r4,#0x74]
  001909f4: blx 0x000786d0
  001909f8: b 0x00190a44
  001909fa: ldrb.w r0,[r4,#0x5f]
  001909fe: cbnz r0,0x00190a08
  00190a00: ldrb.w r0,[r4,#0xdc]
  00190a04: cmp r0,#0x0
  00190a06: beq 0x00190a9c
  00190a08: ldr r0,[r5,#0x0]
  00190a0a: blx 0x00071770
  00190a0e: cmp r0,#0x8
  00190a10: blt 0x00190a34
  00190a12: ldr r0,[r4,#0x7c]
  00190a14: ldrb r0,[r0,#0x10]
  00190a16: cbnz r0,0x00190a34
  00190a18: ldrb.w r0,[r4,#0xdc]
  00190a1c: cbnz r0,0x00190a34
  00190a1e: ldr r0,[r4,#0x58]
  00190a20: blx 0x000767bc
  00190a24: cbnz r0,0x00190a34
  00190a26: ldr r0,[r4,#0x7c]
  00190a28: blx 0x00077320
  00190a2c: cbnz r0,0x00190a34
  00190a2e: ldr r0,[r4,#0x74]
  00190a30: blx 0x000786d0
  00190a34: ldr r0,[r4,#0x7c]
  00190a36: blx 0x00078544
  00190a3a: cbnz r0,0x00190a44
  00190a3c: ldr r0,[r4,#0x7c]
  00190a3e: blx 0x000782d4
  00190a42: cbnz r0,0x00190a58
  00190a44: ldr r5,[r4,#0x7c]
  00190a46: ldr.w r0,[r4,#0x84]
  00190a4a: ldrd r2,r3,[r5,#0x8]
  00190a4e: ldr r1,[r4,#0x58]
  00190a50: strd r1,r5,[sp,#0x0]
  00190a54: blx 0x00075610
  00190a58: ldrb.w r0,[r4,#0x5e]
  00190a5c: cbz r0,0x00190a66
  00190a5e: ldr.w r0,[r4,#0x8c]
  00190a62: blx 0x000755e0
  00190a66: movs r0,#0x0
  00190a68: strb.w r0,[r4,#0x110]
  00190a6c: strb.w r0,[r4,#0x5c]
  00190a70: ldr r0,[0x00190dc8]
  00190a72: add r0,pc
  00190a74: ldr r0,[r0,#0x0]
  00190a76: ldr r0,[r0,#0x0]
  00190a78: blx 0x000786dc
  00190a7c: ldr.w r0,[r8,#0x0]
  00190a80: blx 0x00071d7c
  00190a84: ldr r0,[sp,#0x3c]
  00190a86: ldr r1,[r6,#0x0]
  00190a88: subs r0,r1,r0
  00190a8a: itttt eq
  00190a8c: vpop.eq {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  00190a90: add.eq sp,#0x4
  00190a92: pop.eq.w {r8,r9,r10,r11}
  00190a96: pop.eq {r4,r5,r6,r7,pc}
  00190a98: blx 0x0006e824
  00190a9c: ldrb.w r0,[r4,#0x60]
  00190aa0: cbz r0,0x00190ae0
  00190aa2: ldrd r0,r1,[r4,#0x48]
  00190aa6: movw r2,#0xbb9
  00190aaa: subs r0,r0,r2
  00190aac: sbcs r0,r1,#0x0
  00190ab0: blt 0x00190a70
  00190ab2: ldr r0,[r4,#0x50]
  00190ab4: cmp.w r0,#0xfa0
  00190ab8: bge 0x00190b48
  00190aba: vmov s0,r0
  00190abe: vldr.32 s2,[pc,#0x2d4]
  00190ac2: vcvt.f32.s32 s0,s0
  00190ac6: vdiv.f32 s0,s0,s2
  00190aca: vldr.32 s2,[pc,#0x2cc]
  00190ace: vmul.f32 s0,s0,s2
  00190ad2: vcvt.s32.f32 s0,s0
  00190ad6: vmov r0,s0
  00190ada: sub.w r1,r0,#0x100
  00190ade: b 0x00190b4c
  00190ae0: ldr r0,[r4,#0x58]
  00190ae2: movs r1,#0x1
  00190ae4: blx 0x000786e8
  00190ae8: ldr r0,[r4,#0x58]
  00190aea: blx 0x00077adc
  00190aee: cbnz r0,0x00190b1e
  00190af0: ldrb.w r0,[r4,#0xc7]
  00190af4: cbnz r0,0x00190b1e
  00190af6: ldr r0,[r4,#0x58]
  00190af8: blx 0x0007834c
  00190afc: cbz r0,0x00190b04
  00190afe: ldrb.w r0,[r4,#0xd5]
  00190b02: cbz r0,0x00190b1e
  00190b04: ldr r1,[r4,#0x58]
  00190b06: ldrb.w r3,[r4,#0x5d]
  00190b0a: ldr r2,[r4,#0x74]
  00190b0c: ldr.w r0,[r4,#0x80]
  00190b10: cmp r3,#0x0
  00190b12: ldr r1,[r1,#0x0]
  00190b14: ite eq
  00190b16: ldr.eq r3,[r4,#0x40]
  00190b18: mov.ne r3,#0x0
  00190b1a: blx 0x000786f4
  00190b1e: ldrb.w r0,[r4,#0x5e]
  00190b22: cmp r0,#0x0
  00190b24: beq.w 0x00190c60
  00190b28: ldr.w r0,[r4,#0x8c]
  00190b2c: blx 0x000755e0
  00190b30: b 0x00190d50
  00190b48: mov.w r1,#0xffffffff
  00190b4c: ldr r0,[r4,#0x4]
  00190b4e: blx 0x0006fac0
  00190b52: ldr r1,[r4,#0x10]
  00190b54: movs r2,#0x44
  00190b56: ldr.w r0,[r8,#0x0]
  00190b5a: movs r3,#0x0
  00190b5c: strd r2,r2,[sp,#0x0]
  00190b60: movs r2,#0x0
  00190b62: blx 0x00071d70
  00190b66: ldr r0,[r4,#0x50]
  00190b68: cmp.w r0,#0xfa0
  00190b6c: blt.w 0x00190a70
  00190b70: ldr r0,[0x00190da0]
  00190b72: ldr.w r10,[r8,#0x0]
  00190b76: add r0,pc
  00190b78: ldr r5,[r0,#0x0]
  00190b7a: ldr r0,[r5,#0x0]
  00190b7c: blx 0x00077ee4
  00190b80: blx 0x0006f7d8
  00190b84: vldr.32 s16,[pc,#0x214]
  00190b88: vmov s0,r0
  00190b8c: vmul.f32 s0,s0,s16
  00190b90: vmov r0,s0
  00190b94: blx 0x0006f1a8
  00190b98: mov r9,r0
  00190b9a: ldr r0,[r5,#0x0]
  00190b9c: blx 0x00077ee4
  00190ba0: blx 0x0006f7d8
  00190ba4: vmov s0,r0
  00190ba8: vmul.f32 s0,s0,s16
  00190bac: vmov r0,s0
  00190bb0: blx 0x0006f1a8
  00190bb4: vmov s4,r9
  00190bb8: movs r1,#0xff
  00190bba: vmov s0,r0
  00190bbe: movs r2,#0xff
  00190bc0: vcmpe.f32 s4,#0
  00190bc4: movs r3,#0xff
  00190bc6: vmrs apsr,fpscr
  00190bca: vneg.f32 s2,s0
  00190bce: it gt
  00190bd0: vmov.gt.f32 s2,s0
  00190bd4: vldr.32 s0,[pc,#0x1c0]
  00190bd8: vmul.f32 s0,s2,s0
  00190bdc: vcvt.u32.f32 s0,s0
  00190be0: vmov r0,s0
  00190be4: str r0,[sp,#0x0]
  00190be6: mov r0,r10
  00190be8: blx 0x0006ff28
  00190bec: ldr r0,[0x00190da4]
  00190bee: movs r1,#0xc7
  00190bf0: add r0,pc
  00190bf2: ldr r0,[r0,#0x0]
  00190bf4: ldr r0,[r0,#0x0]
  00190bf6: blx 0x00072f70
  00190bfa: mov r1,r0
  00190bfc: add r0,sp,#0x30
  00190bfe: movs r2,#0x0
  00190c00: blx 0x0006f028
  00190c04: ldr.w r0,[r4,#0x1e8]
  00190c08: cbz r0,0x00190c74
  00190c0a: ldr r0,[0x00190da8]
  00190c0c: ldr r1,[0x00190dac]
  00190c0e: ldr r2,[0x00190db0]
  00190c10: add r0,pc
  00190c12: ldr r3,[0x00190db4]
  00190c14: add r1,pc
  00190c16: add r2,pc
  00190c18: ldr r0,[r0,#0x0]
  00190c1a: add r3,pc
  00190c1c: ldr r5,[r1,#0x0]
  00190c1e: ldr r2,[r2,#0x0]
  00190c20: ldr r3,[r3,#0x0]
  00190c22: ldr r1,[r4,#0x10]
  00190c24: ldr.w r4,[r4,#0x1ec]
  00190c28: str r4,[sp,#0x2c]
  00190c2a: ldr.w r10,[r0,#0x0]
  00190c2e: ldr.w r11,[r3,#0x0]
  00190c32: ldr.w r9,[r5,#0x0]
  00190c36: ldr r4,[r2,#0x0]
  00190c38: ldr.w r0,[r8,#0x0]
  00190c3c: blx 0x00072d90
  00190c40: asr.w r2,r10, asr #0x1
  00190c44: add.w r0,r2,r0, asr #0x1
  00190c48: movs r1,#0x1
  00190c4a: adds r0,#0xa
  00190c4c: strd r0,r1,[sp,#0x0]
  00190c50: asr.w r3,r11, asr #0x1
  00190c54: ldr r2,[sp,#0x2c]
  00190c56: mov r0,r4
  00190c58: mov r1,r9
  00190c5a: blx 0x00074e9c
  00190c5e: b 0x00190cce
  00190c60: ldrb.w r0,[r4,#0x5d]
  00190c64: ldr r1,[r4,#0x74]
  00190c66: cmp r0,#0x0
  00190c68: str r1,[sp,#0x2c]
  00190c6a: beq 0x00190cd6
  00190c6c: movs r0,#0x0
  00190c6e: str r0,[sp,#0x20]
  00190c70: movs r0,#0x0
  00190c72: b 0x00190cdc
  00190c74: ldr r0,[0x00190db8]
  00190c76: ldr r1,[0x00190dbc]
  00190c78: add r0,pc
  00190c7a: ldr.w r10,[r8,#0x0]
  00190c7e: add r1,pc
  00190c80: ldr r0,[r0,#0x0]
  00190c82: ldr r1,[r1,#0x0]
  00190c84: ldr r0,[r0,#0x0]
  00190c86: ldr.w r9,[r1,#0x0]
  00190c8a: str r0,[sp,#0x2c]
  00190c8c: add r2,sp,#0x30
  00190c8e: mov r0,r10
  00190c90: mov r1,r9
  00190c92: blx 0x0006faa8
  00190c96: mov r5,r0
  00190c98: ldr r0,[0x00190dc0]
  00190c9a: ldr r1,[r4,#0x10]
  00190c9c: add r0,pc
  00190c9e: ldr r0,[r0,#0x0]
  00190ca0: ldr.w r11,[r0,#0x0]
  00190ca4: ldr.w r0,[r8,#0x0]
  00190ca8: blx 0x00072d90
  00190cac: asr.w r2,r11, asr #0x1
  00190cb0: add.w r0,r2,r0, asr #0x1
  00190cb4: movs r1,#0x0
  00190cb6: adds r0,#0xa
  00190cb8: strd r0,r1,[sp,#0x0]
  00190cbc: add r2,sp,#0x30
  00190cbe: ldr r0,[sp,#0x2c]
  00190cc0: mov r1,r9
  00190cc2: asrs r0,r0,#0x1
  00190cc4: sub.w r3,r0,r5, asr #0x1
  00190cc8: mov r0,r10
  00190cca: blx 0x0006fe14
  00190cce: add r0,sp,#0x30
  00190cd0: blx 0x0006ee30
  00190cd4: b 0x00190a70
  00190cd6: ldr r0,[r4,#0x40]
  00190cd8: str r0,[sp,#0x20]
  00190cda: asrs r0,r0,#0x1f
  00190cdc: str r0,[sp,#0x1c]
  00190cde: ldr r0,[r4,#0x7c]
  00190ce0: ldr r1,[r4,#0x58]
  00190ce2: ldr.w r11,[r4,#0x14]
  00190ce6: str r1,[sp,#0x28]
  00190ce8: ldrb.w r1,[r4,#0x5c]
  00190cec: str r1,[sp,#0x24]
  00190cee: mov r1,r11
  00190cf0: ldr.w r10,[r0,#0x0]
  00190cf4: ldrd r5,r9,[r0,#0x8]
  00190cf8: mov r0,r4
  00190cfa: blx 0x00078700
  00190cfe: asr.w r1,r10, asr #0x1f
  00190d02: subs.w r2,r10,r5
  00190d06: sbc.w r1,r1,r9
  00190d0a: strd r2,r1,[sp,#0x0]
  00190d0e: ldr r1,[sp,#0x28]
  00190d10: str r1,[sp,#0x8]
  00190d12: ldr r1,[sp,#0x24]
  00190d14: strd r1,r11,[sp,#0xc]
  00190d18: str r0,[sp,#0x14]
  00190d1a: ldr r0,[sp,#0x2c]
  00190d1c: ldrd r3,r2,[sp,#0x1c]
  00190d20: blx 0x0007870c
  00190d24: ldr r5,[r4,#0x7c]
  00190d26: ldr.w r0,[r4,#0x84]
  00190d2a: ldrd r2,r3,[r5,#0x8]
  00190d2e: ldr r1,[r4,#0x58]
  00190d30: strd r1,r5,[sp,#0x0]
  00190d34: blx 0x00075610
  00190d38: ldr r1,[r4,#0x74]
  00190d3a: ldr.w r0,[r4,#0x80]
  00190d3e: blx 0x00078718
  00190d42: ldr r0,[0x00190dc4]
  00190d44: movs r1,#0x0
  00190d46: add r0,pc
  00190d48: ldr r0,[r0,#0x0]
  00190d4a: ldr r0,[r0,#0x0]
  00190d4c: blx 0x00075640
  00190d50: ldrb.w r0,[r4,#0xc5]
  00190d54: cbnz r0,0x00190d68
  00190d56: ldrb.w r0,[r4,#0xc6]
  00190d5a: cbnz r0,0x00190d68
  00190d5c: ldrb.w r0,[r4,#0xce]
  00190d60: cbnz r0,0x00190d68
  00190d62: ldrb.w r0,[r4,#0xc4]
  00190d66: cbz r0,0x00190d70
  00190d68: ldr.w r0,[r4,#0x94]
  00190d6c: blx 0x000746ec
  00190d70: ldrb.w r0,[r4,#0xd6]
  00190d74: cmp r0,#0x0
  00190d76: beq.w 0x00190a70
  00190d7a: ldr r1,[r4,#0x40]
  00190d7c: ldr r0,[r4,#0x74]
  00190d7e: blx 0x00078724
  00190d82: b 0x00190a70
  001ac768: bx pc
  001ac76c: ldr r12,[0x1ac774]
  001ac770: add pc,r12,pc
```
