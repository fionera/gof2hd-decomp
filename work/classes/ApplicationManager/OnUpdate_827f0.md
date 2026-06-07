# ApplicationManager::OnUpdate
elf 0x827f0 body 3814
Sig: undefined __stdcall OnUpdate(longlong param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnUpdate(long long) */

void AbyssEngine::ApplicationManager::OnUpdate(longlong param_1)

{
  ApplicationManager *pAVar1;
  longlong lVar2;
  char cVar3;
  char cVar4;
  ApplicationManager *this;
  int *piVar5;
  uint uVar6;
  char *pcVar7;
  undefined4 *puVar8;
  uint *puVar9;
  int *piVar10;
  uint uVar11;
  uint uVar12;
  undefined4 *puVar13;
  uint in_r2;
  int in_r3;
  int iVar14;
  char *pcVar15;
  undefined4 uVar16;
  char *pcVar17;
  String *pSVar18;
  int iVar19;
  undefined4 uVar20;
  int *piVar21;
  String *pSVar22;
  int iVar23;
  uint *puVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  int *piVar27;
  code *pcVar28;
  uint uVar29;
  float fVar30;
  longlong lVar31;
  undefined4 uVar32;
  String aSStack_dc [12];
  String aSStack_d0 [12];
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
  
  this = (ApplicationManager *)param_1;
  piVar27 = *(int **)(DAT_00092900 + 0x92806);
  local_28 = *piVar27;
  Engine::PreUpdate();
  if (**(char **)(DAT_00092904 + 0x9281c) != '\0') {
    CheckForOrientationChange(this);
  }
  if ((*(int *)(this + 0xac) != 0) && (this[0xb0] != (ApplicationManager)0x0)) {
    AESoundRessource::checkLooping();
  }
  switch(*(undefined4 *)(this + 0x3c)) {
  case 0:
    iVar19 = **(int **)(DAT_00092908 + 0x9285a);
    if (0 < iVar19) {
      **(int **)(DAT_00092908 + 0x9285a) = iVar19 + -1;
      Engine::ClearBuffer(*(Engine **)(this + 0xa8),0);
      if (*piVar27 != local_28) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*(undefined4 *)(this + 0xa8));
      }
      (*(code *)(DAT_001ab614 + 0x1ab618))();
      return;
    }
    piVar5 = *(int **)(this + 0x60);
    if (piVar5 == (int *)0x0) {
LAB_00093244:
      piVar5 = *(int **)(this + 0x18);
      if (piVar5 == (int *)0x0) break;
    }
    else {
      *(undefined4 *)(this + 0x60) = 0;
      *(int **)(this + 0x18) = piVar5;
      if (*(int *)(this + 0x20) != 0) {
        iVar19 = (**(code **)(*piVar5 + 0x44))();
        if (iVar19 != 0) {
          (**(code **)(this + 0x20))(*(undefined4 *)this,0xffffffff,*(undefined4 *)(this + 0x24));
        }
        goto LAB_00093244;
      }
    }
    iVar19 = (**(code **)(*piVar5 + 8))();
    if ((*(int *)(this + 0x20) != 0) &&
       (iVar14 = (**(code **)(**(int **)(this + 0x18) + 0x44))(), iVar14 != 0)) {
      (**(code **)(this + 0x20))(*(undefined4 *)this,iVar19,*(undefined4 *)(this + 0x24));
    }
    iVar14 = DAT_000939c8;
    if (iVar19 == 0) {
      puVar8 = (undefined4 *)(DAT_000939c4 + 0x9375c);
      *(undefined4 *)(this + 0xa0) = 0;
      *(undefined4 *)(this + 0xa4) = 0;
      *(undefined4 *)(this + 0x3c) = 5;
      puVar8 = (undefined4 *)*puVar8;
      puVar13 = *(undefined4 **)(iVar14 + 0x93762);
      *(undefined4 *)(this + 0x68) = 0;
      *(undefined4 *)(this + 0x6c) = 0;
      *(uint *)(this + 0x70) = in_r2;
      *(int *)(this + 0x74) = in_r3;
      *(uint *)(this + 0x78) = in_r2 - 1;
      *(uint *)(this + 0x7c) = in_r3 - (uint)(in_r2 == 0);
      *(undefined4 *)(this + 0x80) = 0;
      *(undefined4 *)(this + 0x84) = 0;
      *puVar8 = 0;
      *puVar13 = 0;
    }
    else {
      uVar6 = *(uint *)(this + 0x68);
      uVar11 = *(uint *)(this + 0x70);
      iVar19 = *(int *)(this + 0x74);
      *(uint *)(this + 0x68) = uVar6 + (in_r2 - uVar11);
      *(uint *)(this + 0x6c) =
           *(int *)(this + 0x6c) + ((in_r3 - iVar19) - (uint)(in_r2 < uVar11)) +
           (uint)CARRY4(uVar6,in_r2 - uVar11);
      *(uint *)(this + 0x70) = in_r2;
      *(int *)(this + 0x74) = in_r3;
      *(uint *)(this + 0x78) = uVar11;
      *(int *)(this + 0x7c) = iVar19;
    }
    break;
  case 1:
    if (*(int **)(this + 0x18) != (int *)0x0) {
      (**(code **)(**(int **)(this + 0x18) + 0xc))();
      Engine::ResetLightParam(*(Engine **)(this + 0xa8));
      **(undefined4 **)(DAT_00092910 + 0x928b2) = **(undefined4 **)(DAT_0009290c + 0x928b0);
      *(undefined4 *)(this + 0x3c) = 0;
      *(undefined4 *)(this + 0x18) = 0;
    }
    break;
  case 4:
    uVar6 = *(uint *)(this + 0x68);
    *(undefined4 *)(this + 0xa0) = 0;
    *(undefined4 *)(this + 0xa4) = 0;
    *(undefined4 *)(this + 0x3c) = *(undefined4 *)(this + 0x40);
    *(uint *)(this + 0x68) = uVar6 + 1;
    *(uint *)(this + 0x6c) = *(int *)(this + 0x6c) + (uint)(0xfffffffe < uVar6);
    *(uint *)(this + 0x70) = in_r2;
    *(int *)(this + 0x74) = in_r3;
    *(uint *)(this + 0x78) = in_r2 - 1;
    *(uint *)(this + 0x7c) = in_r3 - (uint)(in_r2 == 0);
    *(undefined4 *)(this + 0x80) = 0;
    *(undefined4 *)(this + 0x84) = 0;
    break;
  case 5:
    pcVar7 = *(char **)(DAT_00092920 + 0x9291a);
    if (*pcVar7 != '\0') {
      puVar24 = *(uint **)(DAT_0009295c + 0x9292c);
      puVar9 = *(uint **)(DAT_00092960 + 0x9292e);
      uVar6 = *puVar24;
      *puVar24 = uVar6 + 1;
      puVar24[1] = puVar24[1] + (uint)(0xfffffffe < uVar6);
      uVar29 = *(uint *)(this + 0x70);
      iVar19 = *(int *)(this + 0x74);
      uVar11 = *(uint *)(this + 0x78);
      iVar14 = *(int *)(this + 0x7c);
      uVar6 = *puVar9;
      uVar12 = uVar29 - uVar11;
      *puVar9 = uVar12 + uVar6;
      puVar9[1] = ((iVar19 - iVar14) - (uint)(uVar29 < uVar11)) +
                  puVar9[1] + (uint)CARRY4(uVar12,uVar6);
    }
    if (*(int *)(this + 0x18) != 0) {
      pAVar1 = this + 0x70;
      piVar5 = *(int **)(DAT_000929a4 + 0x9297a);
      piVar10 = *(int **)(DAT_000929a8 + 0x92980);
      iVar14 = *piVar10;
      iVar19 = (*(int *)pAVar1 - *(int *)(this + 0x78)) + *piVar5;
      *piVar5 = iVar19;
      *piVar10 = iVar14 + 1;
      if (1000 < iVar19) {
        piVar21 = *(int **)(DAT_000929c0 + 0x929b2);
        *piVar10 = 0;
        *piVar5 = iVar19 + -1000;
        *piVar21 = iVar14 + 1;
      }
      pcVar15 = *(char **)(DAT_000929d0 + 0x929ca);
      if (*pcVar15 != '\0') {
        iVar19 = Engine::IsPostEffectActivated(*(Engine **)(this + 0xa8));
        if (iVar19 != 0) {
          **(undefined1 **)(DAT_000929f4 + 0x929e6) = 1;
          PaintCanvas::StartDraw2FBO(*(PaintCanvas **)this);
        }
        if (*pcVar15 != '\0') {
          PaintCanvas::CheckNUseRefractFBO(SUB41(*(undefined4 *)this,0));
        }
      }
      (**(code **)(**(int **)(this + 0x18) + 0x30))();
      iVar19 = *(int *)(this + 0xa8);
      *(undefined4 *)(iVar19 + 0x68) = 0;
      *(undefined4 *)(iVar19 + 0x6c) = 0;
      *(undefined4 *)(iVar19 + 0x58) = 0;
      *(undefined4 *)(iVar19 + 0x5c) =
           *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      *(undefined4 *)(iVar19 + 0x60) =
           *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      *(undefined4 *)(iVar19 + 100) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc)
      ;
      *(undefined4 *)(*(int *)this + 4) = 0;
      (**(code **)(**(int **)(this + 0x18) + 0x34))();
      pcVar17 = *(char **)(DAT_00092a40 + 0x92a3a);
      if (((*pcVar17 != '\0') && (*pcVar15 != '\0')) &&
         (iVar19 = Engine::IsPostEffectActivated(*(Engine **)(this + 0xa8)), iVar19 != 0)) {
        *pcVar17 = '\0';
        PaintCanvas::StopDraw2FBO(*(PaintCanvas **)this);
      }
      if ((*(code **)(this + 0x28) == (code *)0x0) ||
         (iVar19 = (**(code **)(this + 0x28))(*(undefined4 *)this,*(undefined4 *)(this + 0x2c)),
         iVar19 == 0)) {
        (**(code **)(**(int **)(this + 0x18) + 0x38))();
      }
      iVar19 = *(int *)(this + 0xa8);
      if (*(char *)(iVar19 + 0x74) != '\0') {
        *(undefined1 *)(iVar19 + 0xfc) = 0;
        *(undefined1 *)(iVar19 + 0xfe) = 0;
        PaintCanvas::SetColor((uchar)*(undefined4 *)this,'\0',0xff,'\0');
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_00092ad4 + 0x92ac0),false);
        String::String(aSStack_4c,**(int **)(DAT_00092ad8 + 0x92ac8));
        operator+(aSStack_34,aSStack_40);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,true);
        pcVar28 = *(code **)(DAT_00092b34 + 0x92b00);
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_00092b38 + 0x92b22),false);
        String::String(aSStack_4c,*(int *)(*(int *)(this + 0xa8) + 0x58));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 + '\n'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_00092b98 + 0x92b88),false);
        String::String(aSStack_4c,*(int *)(*(int *)(this + 0xa8) + 0x5c));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\x02' + '\x0f'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_00092bfc + 0x92bec),false);
        String::String(aSStack_4c,*(int *)(*(int *)(this + 0xa8) + 100));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\x03' + '\x14'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_0009302c + 0x92c50),false);
        String::String(aSStack_4c,*(int *)(*(int *)(this + 0xa8) + 0x60));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\x04' + '\x19'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_00093030 + 0x92cae),false);
        String::String(aSStack_4c,*(int *)(*(int *)this + 4));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\x05' + '\x1e'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_58,(char *)(DAT_00093034 + 0x92d0c),false);
        String::String(aSStack_64,*(int *)(this + 0xb4));
        operator+(aSStack_4c,aSStack_58);
        String::String(aSStack_70,(char *)(DAT_00093038 + 0x92d28),false);
        operator+(aSStack_40,aSStack_4c);
        String::String(aSStack_7c,*(int *)(this + 0xb8));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\x06' + '#'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_7c);
        (*pcVar28)(aSStack_40);
        (*pcVar28)(aSStack_70);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_64);
        (*pcVar28)(aSStack_58);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_40,(char *)(DAT_0009303c + 0x92da6),false);
        String::String(aSStack_4c,*(int *)(*(int *)(this + 0xa8) + 0x68));
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\a' + '#'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        String::String(aSStack_58,(char *)(DAT_00093040 + 0x92e04),false);
        String::String(aSStack_64,
                       (int)(*(int *)(*(int *)(this + 0xa8) + 0x70) +
                            ((uint)(*(int *)(*(int *)(this + 0xa8) + 0x70) >> 0x1f) >> 0xc)) >> 0x14
                      );
        operator+(aSStack_4c,aSStack_58);
        String::String(aSStack_70,(char *)(DAT_00093044 + 0x92e2a),false);
        operator+(aSStack_40,aSStack_4c);
        String::String(aSStack_7c,
                       (int)(**(int **)(DAT_00093048 + 0x92e40) +
                            ((uint)(**(int **)(DAT_00093048 + 0x92e40) >> 0x1f) >> 0x16)) >> 10);
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\b' + '#'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_7c);
        (*pcVar28)(aSStack_40);
        (*pcVar28)(aSStack_70);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_64);
        (*pcVar28)(aSStack_58);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        fVar30 = (float)String::String(aSStack_40,(char *)(DAT_0009304c + 0x92eb0),false);
        String::String(aSStack_4c,fVar30);
        operator+(aSStack_34,aSStack_40);
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,5,(bool)(cVar3 * '\t' + '#'));
        (*pcVar28)(aSStack_34);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_40);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        iVar19 = PaintCanvas::GetWidth();
        iVar23 = *(int *)(DAT_00093054 + 0x92f20);
        iVar14 = PaintCanvas::GetTextWidth(*(uint *)this,*(String **)(*(int *)(this + 0xa8) + 0x78))
        ;
        PaintCanvas::DrawString(uVar6,pSVar22,iVar23,(iVar19 - iVar14) / 2,true);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        iVar19 = PaintCanvas::GetWidth();
        iVar23 = *(int *)(DAT_00093058 + 0x92f6a);
        iVar14 = PaintCanvas::GetTextWidth(*(uint *)this,*(String **)(*(int *)(this + 0xa8) + 0x78))
        ;
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,iVar23,(iVar19 - iVar14) / 2,(bool)(cVar3 + '\n'));
        if (*pcVar15 == '\0') {
          iVar19 = *(int *)(this + 0xa8);
        }
        else {
          uVar11 = *(uint *)this;
          pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
          String::String(aSStack_34,(char *)(DAT_0009305c + 0x92fb8),false);
          iVar19 = PaintCanvas::GetWidth();
          uVar6 = *(uint *)this;
          pSVar18 = *(String **)(*(int *)(this + 0xa8) + 0x78);
          String::String(aSStack_40,(char *)(DAT_00093060 + 0x92fd4),false);
          iVar14 = PaintCanvas::GetTextWidth(uVar6,pSVar18);
          PaintCanvas::DrawString(uVar11,pSVar22,(int)aSStack_34,iVar19 - iVar14,true);
          String::~String(aSStack_40);
          String::~String(aSStack_34);
          cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
          cVar3 = cVar3 + '\n';
          uVar6 = 0;
          while( true ) {
            iVar19 = *(int *)(this + 0xa8);
            if (*(uint *)(iVar19 + 0x3d8) <= uVar6) break;
            iVar14 = *(int *)(*(int *)(iVar19 + 0x3dc) + uVar6 * 4);
            if (iVar14 != 0) {
              pSVar22 = *(String **)(iVar19 + 0x78);
              uVar11 = *(uint *)this;
              String::String(aSStack_4c,iVar14);
              String::String(aSStack_58,(char *)(DAT_0009336c + 0x93088),false);
              operator+(aSStack_40,aSStack_4c);
              ShaderBaseStruct::GetShaderName();
              operator+(aSStack_34,aSStack_40);
              iVar19 = PaintCanvas::GetWidth();
              PaintCanvas::DrawString(uVar11,pSVar22,(int)aSStack_34,iVar19 + -300,(bool)cVar3);
              (*pcVar28)(aSStack_34);
              (*pcVar28)(aSStack_64);
              (*pcVar28)(aSStack_40);
              (*pcVar28)(aSStack_58);
              (*pcVar28)(aSStack_4c);
              cVar4 = PaintCanvas::GetTextHeight(*(uint *)this);
              cVar3 = cVar4 + cVar3 + '\x05';
            }
            uVar6 = uVar6 + 1;
          }
        }
        *(undefined1 *)(iVar19 + 0xfe) = 1;
        *(undefined1 *)(iVar19 + 0xfc) = 1;
      }
      if (**(char **)(DAT_00093788 + 0x93398) != '\0') {
        uVar32 = 0xff;
        PaintCanvas::SetColor((uchar)*(undefined4 *)this,'\0',0xff,'\0');
        uVar26 = **(undefined4 **)(DAT_0009378c + 0x933ba);
        uVar16 = (*(undefined4 **)(DAT_0009378c + 0x933ba))[1];
        uVar11 = **(uint **)(DAT_00093790 + 0x933be);
        uVar6 = (*(uint **)(DAT_00093790 + 0x933be))[1];
        uVar20 = **(undefined4 **)(DAT_00093794 + 0x933c2);
        uVar25 = (*(undefined4 **)(DAT_00093794 + 0x933c2))[1];
        String::String(aSStack_70,(char *)(DAT_00093798 + 0x933dc),false);
        iVar19 = FUN_001aae54(uVar20,uVar25,1000,0,uVar32);
        lVar31 = FUN_001aae54(uVar26,uVar16,iVar19,iVar19 >> 0x1f);
        lVar2 = (ulonglong)uVar11 * 1000;
        iVar14 = FUN_001aae54((int)lVar2,uVar6 * 1000 + (int)((ulonglong)lVar2 >> 0x20),uVar20,
                              uVar25);
        String::String(aSStack_7c,iVar14);
        operator+(aSStack_64,aSStack_70);
        String::String(aSStack_88,(char *)(DAT_0009379c + 0x9342c),false);
        operator+(aSStack_58,aSStack_64);
        String::String(aSStack_94,iVar19);
        operator+(aSStack_4c,aSStack_58);
        String::String(aSStack_a0,(char *)(DAT_000937a0 + 0x9345c),false);
        operator+(aSStack_40,aSStack_4c);
        String::String(aSStack_ac,lVar31);
        operator+(aSStack_34,aSStack_40);
        pcVar28 = *(code **)(DAT_000937a4 + 0x93486);
        (*pcVar28)(aSStack_ac);
        (*pcVar28)(aSStack_40);
        (*pcVar28)(aSStack_a0);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_94);
        (*pcVar28)(aSStack_58);
        (*pcVar28)(aSStack_88);
        (*pcVar28)(aSStack_64);
        (*pcVar28)(aSStack_7c);
        (*pcVar28)(aSStack_70);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        iVar19 = PaintCanvas::GetWidth();
        iVar14 = PaintCanvas::GetTextWidth(*(uint *)this,*(String **)(*(int *)(this + 0xa8) + 0x78))
        ;
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,iVar19 / 2 - iVar14 / 2,true);
        String::~String(aSStack_34);
      }
      if (*pcVar7 != '\0') {
        iVar19 = *(int *)(this + 0xa8);
        puVar24 = *(uint **)(DAT_000937a8 + 0x93512);
        uVar12 = *(int *)(iVar19 + 0x5c) + *(int *)(iVar19 + 0x6c);
        uVar29 = *puVar24;
        uVar6 = *(int *)(iVar19 + 0x58) + *(int *)(iVar19 + 0x68);
        uVar11 = uVar29 + uVar6;
        *puVar24 = uVar11 + uVar12;
        puVar24[1] = puVar24[1] + ((int)uVar6 >> 0x1f) + (uint)CARRY4(uVar29,uVar6) +
                     ((int)uVar12 >> 0x1f) + (uint)CARRY4(uVar11,uVar12);
        uVar25 = 0xff;
        PaintCanvas::SetColor((uchar)*(undefined4 *)this,'\0',0xff,'\0');
        uVar16 = **(undefined4 **)(DAT_000937ac + 0x9354e);
        uVar20 = (*(undefined4 **)(DAT_000937ac + 0x9354e))[1];
        String::String(aSStack_88,(char *)(DAT_000937b0 + 0x9355a),false);
        iVar19 = FUN_001aae54(uVar16,uVar20,1000,0,uVar25);
        String::String(aSStack_94,**(longlong **)(DAT_000937b4 + 0x93572));
        operator+(aSStack_7c,aSStack_88);
        String::String(aSStack_a0,(char *)(DAT_000937b8 + 0x9358c),false);
        operator+(aSStack_70,aSStack_7c);
        String::String(aSStack_ac,**(longlong **)(DAT_000937bc + 0x935a2));
        operator+(aSStack_64,aSStack_70);
        String::String(aSStack_b8,(char *)(DAT_000937c0 + 0x935bc),false);
        operator+(aSStack_58,aSStack_64);
        String::String(aSStack_c4,iVar19);
        operator+(aSStack_4c,aSStack_58);
        String::String(aSStack_d0,(char *)(DAT_000937c4 + 0x935e4),false);
        operator+(aSStack_40,aSStack_4c);
        String::String(aSStack_dc,*(longlong *)puVar24);
        operator+(aSStack_34,aSStack_40);
        pcVar28 = *(code **)(DAT_000937c8 + 0x9360e);
        (*pcVar28)(aSStack_dc);
        (*pcVar28)(aSStack_40);
        (*pcVar28)(aSStack_d0);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_c4);
        (*pcVar28)(aSStack_58);
        (*pcVar28)(aSStack_b8);
        (*pcVar28)(aSStack_64);
        (*pcVar28)(aSStack_ac);
        (*pcVar28)(aSStack_70);
        (*pcVar28)(aSStack_a0);
        (*pcVar28)(aSStack_7c);
        (*pcVar28)(aSStack_94);
        (*pcVar28)(aSStack_88);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        iVar19 = PaintCanvas::GetWidth();
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,iVar19 / 2,true);
        iVar19 = PaintCanvas::GetWidth();
        String::String(aSStack_58,iVar19);
        String::String(aSStack_64,(char *)(DAT_000939cc + 0x93686),false);
        operator+(aSStack_4c,aSStack_58);
        iVar19 = PaintCanvas::GetHeight();
        String::String(aSStack_70,iVar19);
        operator+(aSStack_40,aSStack_4c);
        String::operator=(aSStack_34,aSStack_40);
        (*pcVar28)(aSStack_40);
        (*pcVar28)(aSStack_70);
        (*pcVar28)(aSStack_4c);
        (*pcVar28)(aSStack_64);
        (*pcVar28)(aSStack_58);
        uVar6 = *(uint *)this;
        pSVar22 = *(String **)(*(int *)(this + 0xa8) + 0x78);
        iVar19 = PaintCanvas::GetWidth();
        cVar3 = PaintCanvas::GetTextHeight(*(uint *)this);
        PaintCanvas::DrawString(uVar6,pSVar22,(int)aSStack_34,iVar19 / 2,(bool)(cVar3 + '\n'));
        String::~String(aSStack_34);
      }
      uVar6 = *(uint *)pAVar1;
      iVar19 = *(int *)(this + 0x74);
      *(uint *)(this + 0x78) = uVar6;
      *(int *)(this + 0x7c) = iVar19;
      *(uint *)pAVar1 = in_r2;
      *(int *)(this + 0x74) = in_r3;
      uVar11 = *(uint *)(this + 0x68);
      *(uint *)(this + 0x68) = (in_r2 - uVar6) + uVar11;
      *(uint *)(this + 0x6c) =
           ((in_r3 - iVar19) - (uint)(in_r2 < uVar6)) + *(int *)(this + 0x6c) +
           (uint)CARRY4(in_r2 - uVar6,uVar11);
    }
    break;
  case 6:
    pAVar1 = this + 0x68;
    uVar6 = *(uint *)pAVar1;
    iVar19 = *(int *)(this + 0x6c);
    if (uVar6 == 0 && iVar19 == 0) {
      iVar19 = PaintCanvas::WarmUpTexture();
      if (iVar19 == 0) {
        uVar6 = *(uint *)pAVar1;
        iVar19 = *(int *)(this + 0x6c);
      }
      else {
        *(undefined4 *)(this + 0x3c) = 5;
        uVar6 = 0;
        *(undefined4 *)pAVar1 = 0;
        *(undefined4 *)(this + 0x6c) = 0;
        *(undefined4 *)(this + 0xa0) = 0;
        *(undefined4 *)(this + 0xa4) = 0;
        *(uint *)(this + 0x70) = in_r2;
        *(int *)(this + 0x74) = in_r3;
        *(uint *)(this + 0x78) = in_r2 - 1;
        *(uint *)(this + 0x7c) = in_r3 - (uint)(in_r2 == 0);
        iVar19 = 0;
        *(undefined4 *)(this + 0x80) = 0;
        *(undefined4 *)(this + 0x84) = 0;
      }
    }
    uVar12 = *(uint *)(this + 0x70);
    iVar14 = *(int *)(this + 0x74);
    *(uint *)(this + 0x70) = in_r2;
    *(int *)(this + 0x74) = in_r3;
    *(uint *)(this + 0x78) = uVar12;
    *(int *)(this + 0x7c) = iVar14;
    uVar11 = uVar6 + (in_r2 - uVar12);
    iVar19 = iVar19 + ((in_r3 - iVar14) - (uint)(in_r2 < uVar12)) +
             (uint)CARRY4(uVar6,in_r2 - uVar12);
    if ((int)(-(uint)(500 < uVar11) - iVar19) < 0 !=
        (SBORROW4(0,iVar19) != SBORROW4(-iVar19,(uint)(500 < uVar11)))) {
      iVar19 = 0;
      uVar11 = 0;
    }
    *(uint *)pAVar1 = uVar11;
    *(int *)(this + 0x6c) = iVar19;
    break;
  case 7:
    puVar24 = *(uint **)(DAT_00093370 + 0x9316c);
    if ((*puVar24 < *(uint *)(*(int *)this + 0x10)) ||
       (**(uint **)(DAT_00093374 + 0x9317c) < *(uint *)(*(int *)this + 0x24))) {
      if ((*puVar24 == 0) &&
         ((*(int *)(this + 0x20) != 0 &&
          (iVar19 = (**(code **)(**(int **)(this + 0x18) + 0x44))(), iVar19 != 0)))) {
        (**(code **)(this + 0x20))(*(undefined4 *)this,0,*(undefined4 *)(this + 0x24));
      }
      uVar6 = *puVar24;
      if (uVar6 < *(uint *)(*(int *)this + 0x10)) {
        iVar19 = *(int *)(*(int *)(*(int *)this + 0x14) + uVar6 * 4);
        if ((iVar19 != 0) && (*(char *)(iVar19 + 0x15) == '\0')) {
          pcVar7 = (char *)String::GetAEChar();
          TextureCreateFromFileIntern
                    (*(Engine **)(this + 0xa8),pcVar7,(_func_void_Image_ptr_void_ptr *)0x0,
                     (void *)0x0,(uint *)aSStack_34,*(float *)(iVar19 + 0x10),
                     (AELoadedTexture *)*(float *)(iVar19 + 0x10),SUB41(iVar19,0));
          operator_delete__(pcVar7);
          uVar6 = *puVar24;
        }
        *puVar24 = uVar6 + 1;
      }
      puVar24 = *(uint **)(DAT_00093378 + 0x931e2);
      for (iVar19 = 10; iVar19 != 0; iVar19 = iVar19 + -1) {
        if (*puVar24 < *(uint *)(*(PaintCanvas **)this + 0x24)) {
          PaintCanvas::MeshConvertToVBO(*(PaintCanvas **)this,*puVar24);
          *puVar24 = *puVar24 + 1;
        }
      }
      iVar19 = *(int *)(this + 0x6c);
      uVar11 = *(uint *)(this + 0x68);
      uVar6 = *(uint *)(this + 0x70);
      iVar14 = *(int *)(this + 0x74);
    }
    else {
      *(uint *)(this + 0x78) = in_r2 - 1;
      uVar11 = 0;
      iVar19 = 0;
      *(int *)(this + 0x74) = in_r3;
      *(undefined4 *)(this + 0x6c) = 0;
      *(uint *)(this + 0x7c) = in_r3 - (uint)(in_r2 == 0);
      *(undefined4 *)(this + 0x80) = 0;
      *(undefined4 *)(this + 0x84) = 0;
      *(undefined4 *)(this + 0xa0) = 0;
      *(undefined4 *)(this + 0xa4) = 0;
      *(undefined4 *)(this + 0x3c) = 5;
      *(undefined4 *)(this + 0x68) = 0;
      *(uint *)(this + 0x70) = in_r2;
      uVar6 = in_r2;
      iVar14 = in_r3;
    }
    *(uint *)(this + 0x78) = uVar6;
    *(int *)(this + 0x7c) = iVar14;
    *(uint *)(this + 0x70) = in_r2;
    *(int *)(this + 0x74) = in_r3;
    uVar12 = uVar11 + (in_r2 - uVar6);
    iVar19 = iVar19 + ((in_r3 - iVar14) - (uint)(in_r2 < uVar6)) +
                      (uint)CARRY4(uVar11,in_r2 - uVar6);
    if ((int)(-(uint)(500 < uVar12) - iVar19) < 0 !=
        (SBORROW4(0,iVar19) != SBORROW4(-iVar19,(uint)(500 < uVar12)))) {
      iVar19 = 0;
      uVar12 = 0;
    }
    *(uint *)(this + 0x68) = uVar12;
    *(int *)(this + 0x6c) = iVar19;
  }
  Engine::SwapBuffer(*(Engine **)(this + 0xa8));
  if (*piVar27 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000927f0: push {r4,r5,r6,r7,lr}
  000927f2: add r7,sp,#0xc
  000927f4: push {r8,r9,r10,r11}
  000927f8: sub sp,#0xec
  000927fa: mov r9,r0
  000927fc: ldr r0,[0x00092900]
  000927fe: mov r8,r3
  00092800: mov r10,r2
  00092802: add r0,pc
  00092804: ldr.w r11,[r0,#0x0]
  00092808: ldr.w r0,[r11,#0x0]
  0009280c: str r0,[sp,#0xe8]
  0009280e: ldr.w r0,[r9,#0xa8]
  00092812: blx 0x00070438
  00092816: ldr r0,[0x00092904]
  00092818: add r0,pc
  0009281a: ldr r0,[r0,#0x0]
  0009281c: ldrb r0,[r0,#0x0]
  0009281e: cbz r0,0x00092826
  00092820: mov r0,r9
  00092822: blx 0x00070444
  00092826: ldr.w r0,[r9,#0xac]
  0009282a: cbz r0,0x00092836
  0009282c: ldrb.w r1,[r9,#0xb0]
  00092830: cbz r1,0x00092836
  00092832: blx 0x00070450
  00092836: ldr.w r0,[r9,#0x3c]
  0009283a: cmp r0,#0x7
  0009283c: bhi.w 0x00093732
  00092840: tbh [pc,r0]
  00092854: ldr r0,[0x00092908]
  00092856: add r0,pc
  00092858: ldr r0,[r0,#0x0]
  0009285a: ldr r1,[r0,#0x0]
  0009285c: cmp r1,#0x1
  0009285e: blt.w 0x00093218
  00092862: subs r1,#0x1
  00092864: str r1,[r0,#0x0]
  00092866: ldr.w r0,[r9,#0xa8]
  0009286a: movs r1,#0x0
  0009286c: blx 0x0006fe38
  00092870: ldr.w r0,[r9,#0xa8]
  00092874: ldr r1,[sp,#0xe8]
  00092876: ldr.w r2,[r11,#0x0]
  0009287a: subs r1,r2,r1
  0009287c: itttt eq
  0009287e: add.eq sp,#0xec
  00092880: pop.eq.w {r8,r9,r10,r11}
  00092884: pop.eq.w {r4,r5,r6,r7,lr}
  00092888: b.eq.w 0x001ab608
  0009288c: blx 0x0006e824
  00092890: ldr.w r0,[r9,#0x18]
  00092894: cmp r0,#0x0
  00092896: beq.w 0x00093732
  0009289a: ldr r1,[r0,#0x0]
  0009289c: ldr r1,[r1,#0xc]
  0009289e: blx r1
  000928a0: ldr.w r0,[r9,#0xa8]
  000928a4: blx 0x0006ed94
  000928a8: ldr r0,[0x0009290c]
  000928aa: ldr r1,[0x00092910]
  000928ac: add r0,pc
  000928ae: add r1,pc
  000928b0: ldr r0,[r0,#0x0]
  000928b2: ldr r1,[r1,#0x0]
  000928b4: ldr r0,[r0,#0x0]
  000928b6: str r0,[r1,#0x0]
  000928b8: movs r0,#0x0
  000928ba: str.w r0,[r9,#0x3c]
  000928be: str.w r0,[r9,#0x18]
  000928c2: b.w 0x00093732
  000928c6: ldrd r1,r2,[r9,#0x68]
  000928ca: subs.w r3,r10,#0x1
  000928ce: ldr.w r0,[r9,#0x40]
  000928d2: mov.w r6,#0x0
  000928d6: strd r6,r6,[r9,#0xa0]
  000928da: sbc r5,r8,#0x0
  000928de: str.w r0,[r9,#0x3c]
  000928e2: adds r0,r1,#0x1
  000928e4: adc r1,r2,#0x0
  000928e8: add.w r2,r9,#0x68
  000928ec: stm r2,{r0,r1,r10}
  000928f0: strd r8,r3,[r9,#0x74]
  000928f4: strd r5,r6,[r9,#0x7c]
  000928f8: str.w r6,[r9,#0x84]
  000928fc: b.w 0x00093732
  00092914: ldr r0,[0x00092920]
  00092916: add r0,pc
  00092918: ldr r5,[r0,#0x0]
  0009291a: ldrb r0,[r5,#0x0]
  0009291c: cbz r0,0x00092964
  0009291e: b 0x00092924
  00092924: ldr r0,[0x0009295c]
  00092926: ldr r1,[0x00092960]
  00092928: add r0,pc
  0009292a: add r1,pc
  0009292c: ldr r0,[r0,#0x0]
  0009292e: ldr r1,[r1,#0x0]
  00092930: ldrd r2,r3,[r0,#0x0]
  00092934: adds r2,#0x1
  00092936: adc r3,r3,#0x0
  0009293a: strd r2,r3,[r0,#0x0]
  0009293e: ldrd r12,r2,[r9,#0x70]
  00092942: ldrd r3,r6,[r9,#0x78]
  00092946: ldrd r0,lr,[r1,#0x0]
  0009294a: subs.w r3,r12,r3
  0009294e: sbcs r2,r6
  00092950: adds r3,r3,r0
  00092952: adc.w r0,r2,lr
  00092956: strd r3,r0,[r1,#0x0]
  0009295a: b 0x00092964
  00092964: ldr.w r0,[r9,#0x18]
  00092968: cmp r0,#0x0
  0009296a: beq.w 0x00093732
  0009296e: str r5,[sp,#0x1c]
  00092970: mov r3,r9
  00092972: ldr r0,[0x000929a4]
  00092974: ldr r1,[0x000929a8]
  00092976: add r0,pc
  00092978: ldr.w r12,[r3,#0x70]!
  0009297c: add r1,pc
  0009297e: str r3,[sp,#0x18]
  00092980: ldr r0,[r0,#0x0]
  00092982: ldr r1,[r1,#0x0]
  00092984: ldr.w r2,[r3,#0x8]!
  00092988: ldr r6,[r0,#0x0]
  0009298a: ldr r5,[r1,#0x0]
  0009298c: sub.w r2,r12,r2
  00092990: add r2,r6
  00092992: str r3,[sp,#0x14]
  00092994: adds r3,r5,#0x1
  00092996: cmp.w r2,#0x3e8
  0009299a: str r2,[r0,#0x0]
  0009299c: str r3,[r1,#0x0]
  0009299e: ble 0x000929c4
  000929a0: b 0x000929ac
  000929ac: ldr r5,[0x000929c0]
  000929ae: add r5,pc
  000929b0: ldr r6,[r5,#0x0]
  000929b2: movs r5,#0x0
  000929b4: str r5,[r1,#0x0]
  000929b6: sub.w r1,r2,#0x3e8
  000929ba: str r1,[r0,#0x0]
  000929bc: str r3,[r6,#0x0]
  000929be: b 0x000929c4
  000929c4: ldr r0,[0x000929d0]
  000929c6: add r0,pc
  000929c8: ldr r4,[r0,#0x0]
  000929ca: ldrb r0,[r4,#0x0]
  000929cc: cbz r0,0x00092a06
  000929ce: b 0x000929d4
  000929d4: ldr.w r0,[r9,#0xa8]
  000929d8: blx 0x0006ee78
  000929dc: cbz r0,0x000929f8
  000929de: ldr r0,[0x000929f4]
  000929e0: movs r1,#0x1
  000929e2: add r0,pc
  000929e4: ldr r0,[r0,#0x0]
  000929e6: strb r1,[r0,#0x0]
  000929e8: ldr.w r0,[r9,#0x0]
  000929ec: blx 0x00070468
  000929f0: b 0x000929f8
  000929f8: ldrb r0,[r4,#0x0]
  000929fa: cbz r0,0x00092a06
  000929fc: ldr.w r0,[r9,#0x0]
  00092a00: movs r1,#0x1
  00092a02: blx 0x00070474
  00092a06: ldr.w r0,[r9,#0x18]
  00092a0a: ldr r1,[r0,#0x0]
  00092a0c: ldr r1,[r1,#0x30]
  00092a0e: blx r1
  00092a10: ldr.w r0,[r9,#0xa8]
  00092a14: movs r1,#0x0
  00092a16: vmov.i32 q8,#0x0
  00092a1a: strd r1,r1,[r0,#0x68]
  00092a1e: adds r0,#0x58
  00092a20: vst1.64 {d16,d17},[r0]
  00092a24: ldr.w r0,[r9,#0x0]
  00092a28: str r1,[r0,#0x4]
  00092a2a: ldr.w r0,[r9,#0x18]
  00092a2e: ldr r1,[r0,#0x0]
  00092a30: ldr r1,[r1,#0x34]
  00092a32: blx r1
  00092a34: ldr r0,[0x00092a40]
  00092a36: add r0,pc
  00092a38: ldr r5,[r0,#0x0]
  00092a3a: ldrb r0,[r5,#0x0]
  00092a3c: cbz r0,0x00092a5e
  00092a3e: b 0x00092a44
  00092a44: ldrb r0,[r4,#0x0]
  00092a46: cbz r0,0x00092a5e
  00092a48: ldr.w r0,[r9,#0xa8]
  00092a4c: blx 0x0006ee78
  00092a50: cbz r0,0x00092a5e
  00092a52: movs r0,#0x0
  00092a54: strb r0,[r5,#0x0]
  00092a56: ldr.w r0,[r9,#0x0]
  00092a5a: blx 0x00070480
  00092a5e: ldr.w r2,[r9,#0x28]
  00092a62: cbz r2,0x00092a70
  00092a64: ldr.w r0,[r9,#0x0]
  00092a68: ldr.w r1,[r9,#0x2c]
  00092a6c: blx r2
  00092a6e: cbnz r0,0x00092a7a
  00092a70: ldr.w r0,[r9,#0x18]
  00092a74: ldr r1,[r0,#0x0]
  00092a76: ldr r1,[r1,#0x38]
  00092a78: blx r1
  00092a7a: ldr.w r0,[r9,#0xa8]
  00092a7e: str.w r10,[sp,#0x30]
  00092a82: ldrb.w r1,[r0,#0x74]
  00092a86: strd r8,r11,[sp,#0x20]
  00092a8a: cmp r1,#0x0
  00092a8c: beq.w 0x00093392
  00092a90: movs r1,#0x0
  00092a92: movs r2,#0xff
  00092a94: strb.w r1,[r0,#0xfc]
  00092a98: movs r3,#0x0
  00092a9a: strb.w r1,[r0,#0xfe]
  00092a9e: movs r1,#0xff
  00092aa0: ldr.w r0,[r9,#0x0]
  00092aa4: str r1,[sp,#0x0]
  00092aa6: movs r1,#0x0
  00092aa8: blx 0x0006ff28
  00092aac: ldr.w r0,[r9,#0xa8]
  00092ab0: movs r2,#0x0
  00092ab2: ldr.w r5,[r9,#0x0]
  00092ab6: ldr r6,[r0,#0x78]
  00092ab8: add r0,sp,#0xd0
  00092aba: ldr r1,[0x00092ad4]
  00092abc: add r1,pc
  00092abe: blx 0x0006ee18
  00092ac2: ldr r0,[0x00092ad8]
  00092ac4: add r0,pc
  00092ac6: ldr r0,[r0,#0x0]
  00092ac8: ldr r1,[r0,#0x0]
  00092aca: add r0,sp,#0xc4
  00092acc: blx 0x0006f658
  00092ad0: b 0x00092adc
  00092adc: add r0,sp,#0xdc
  00092ade: add r1,sp,#0xd0
  00092ae0: add r2,sp,#0xc4
  00092ae2: blx 0x0006ef98
  00092ae6: movs r0,#0x0
  00092ae8: movs r1,#0x5
  00092aea: add r2,sp,#0xdc
  00092aec: strd r1,r0,[sp,#0x0]
  00092af0: mov r0,r5
  00092af2: mov r1,r6
  00092af4: movs r3,#0x5
  00092af6: blx 0x0006fe14
  00092afa: ldr r0,[0x00092b34]
  00092afc: add r0,pc
  00092afe: ldr.w r11,[r0,#0x0]
  00092b02: add r0,sp,#0xdc
  00092b04: blx r11
  00092b06: add r0,sp,#0xc4
  00092b08: blx r11
  00092b0a: add r0,sp,#0xd0
  00092b0c: blx r11
  00092b0e: ldr.w r0,[r9,#0xa8]
  00092b12: movs r2,#0x0
  00092b14: ldr.w r5,[r9,#0x0]
  00092b18: ldr r6,[r0,#0x78]
  00092b1a: add r0,sp,#0xd0
  00092b1c: ldr r1,[0x00092b38]
  00092b1e: add r1,pc
  00092b20: blx 0x0006ee18
  00092b24: ldr.w r0,[r9,#0xa8]
  00092b28: ldr r1,[r0,#0x58]
  00092b2a: add r0,sp,#0xc4
  00092b2c: blx 0x0006f658
  00092b30: b 0x00092b3c
  00092b3c: add r0,sp,#0xdc
  00092b3e: add r1,sp,#0xd0
  00092b40: add r2,sp,#0xc4
  00092b42: blx 0x0006ef98
  00092b46: ldr.w r1,[r9,#0xa8]
  00092b4a: ldr.w r0,[r9,#0x0]
  00092b4e: ldr r1,[r1,#0x78]
  00092b50: blx 0x0006fe20
  00092b54: movs r1,#0x0
  00092b56: adds r0,#0xa
  00092b58: add r2,sp,#0xdc
  00092b5a: strd r0,r1,[sp,#0x0]
  00092b5e: mov r0,r5
  00092b60: mov r1,r6
  00092b62: movs r3,#0x5
  00092b64: blx 0x0006fe14
  00092b68: add r0,sp,#0xdc
  00092b6a: blx r11
  00092b6c: add r0,sp,#0xc4
  00092b6e: blx r11
  00092b70: add r0,sp,#0xd0
  00092b72: blx r11
  00092b74: ldr.w r0,[r9,#0xa8]
  00092b78: movs r2,#0x0
  00092b7a: ldr.w r5,[r9,#0x0]
  00092b7e: ldr r6,[r0,#0x78]
  00092b80: add r0,sp,#0xd0
  00092b82: ldr r1,[0x00092b98]
  00092b84: add r1,pc
  00092b86: blx 0x0006ee18
  00092b8a: ldr.w r0,[r9,#0xa8]
  00092b8e: ldr r1,[r0,#0x5c]
  00092b90: add r0,sp,#0xc4
  00092b92: blx 0x0006f658
  00092b96: b 0x00092b9c
  00092b9c: add r0,sp,#0xdc
  00092b9e: add r1,sp,#0xd0
  00092ba0: add r2,sp,#0xc4
  00092ba2: blx 0x0006ef98
  00092ba6: ldr.w r1,[r9,#0xa8]
  00092baa: ldr.w r0,[r9,#0x0]
  00092bae: ldr r1,[r1,#0x78]
  00092bb0: blx 0x0006fe20
  00092bb4: movs r2,#0xf
  00092bb6: add.w r0,r2,r0, lsl #0x1
  00092bba: movs r1,#0x0
  00092bbc: add r2,sp,#0xdc
  00092bbe: strd r0,r1,[sp,#0x0]
  00092bc2: mov r0,r5
  00092bc4: mov r1,r6
  00092bc6: movs r3,#0x5
  00092bc8: blx 0x0006fe14
  00092bcc: add r0,sp,#0xdc
  00092bce: blx r11
  00092bd0: add r0,sp,#0xc4
  00092bd2: blx r11
  00092bd4: add r0,sp,#0xd0
  00092bd6: blx r11
  00092bd8: ldr.w r0,[r9,#0xa8]
  00092bdc: movs r2,#0x0
  00092bde: ldr.w r5,[r9,#0x0]
  00092be2: ldr r6,[r0,#0x78]
  00092be4: add r0,sp,#0xd0
  00092be6: ldr r1,[0x00092bfc]
  00092be8: add r1,pc
  00092bea: blx 0x0006ee18
  00092bee: ldr.w r0,[r9,#0xa8]
  00092bf2: ldr r1,[r0,#0x64]
  00092bf4: add r0,sp,#0xc4
  00092bf6: blx 0x0006f658
  00092bfa: b 0x00092c00
  00092c00: add r0,sp,#0xdc
  00092c02: add r1,sp,#0xd0
  00092c04: add r2,sp,#0xc4
  00092c06: blx 0x0006ef98
  00092c0a: ldr.w r1,[r9,#0xa8]
  00092c0e: ldr.w r0,[r9,#0x0]
  00092c12: ldr r1,[r1,#0x78]
  00092c14: blx 0x0006fe20
  00092c18: add.w r0,r0,r0, lsl #0x1
  00092c1c: movs r1,#0x0
  00092c1e: adds r0,#0x14
  00092c20: add r2,sp,#0xdc
  00092c22: strd r0,r1,[sp,#0x0]
  00092c26: mov r0,r5
  00092c28: mov r1,r6
  00092c2a: movs r3,#0x5
  00092c2c: blx 0x0006fe14
  00092c30: add r0,sp,#0xdc
  00092c32: blx r11
  00092c34: add r0,sp,#0xc4
  00092c36: blx r11
  00092c38: add r0,sp,#0xd0
  00092c3a: blx r11
  00092c3c: ldr.w r0,[r9,#0xa8]
  00092c40: movs r2,#0x0
  00092c42: ldr.w r5,[r9,#0x0]
  00092c46: ldr r6,[r0,#0x78]
  00092c48: add r0,sp,#0xd0
  00092c4a: ldr r1,[0x0009302c]
  00092c4c: add r1,pc
  00092c4e: blx 0x0006ee18
  00092c52: ldr.w r0,[r9,#0xa8]
  00092c56: ldr r1,[r0,#0x60]
  00092c58: add r0,sp,#0xc4
  00092c5a: blx 0x0006f658
  00092c5e: add r0,sp,#0xdc
  00092c60: add r1,sp,#0xd0
  00092c62: add r2,sp,#0xc4
  00092c64: blx 0x0006ef98
  00092c68: ldr.w r1,[r9,#0xa8]
  00092c6c: ldr.w r0,[r9,#0x0]
  00092c70: ldr r1,[r1,#0x78]
  00092c72: blx 0x0006fe20
  00092c76: movs r2,#0x19
  00092c78: add.w r0,r2,r0, lsl #0x2
  00092c7c: movs r1,#0x0
  00092c7e: add r2,sp,#0xdc
  00092c80: strd r0,r1,[sp,#0x0]
  00092c84: mov r0,r5
  00092c86: mov r1,r6
  00092c88: movs r3,#0x5
  00092c8a: blx 0x0006fe14
  00092c8e: add r0,sp,#0xdc
  00092c90: blx r11
  00092c92: add r0,sp,#0xc4
  00092c94: blx r11
  00092c96: add r0,sp,#0xd0
  00092c98: blx r11
  00092c9a: ldr.w r0,[r9,#0xa8]
  00092c9e: movs r2,#0x0
  00092ca0: ldr.w r5,[r9,#0x0]
  00092ca4: ldr r6,[r0,#0x78]
  00092ca6: add r0,sp,#0xd0
  00092ca8: ldr r1,[0x00093030]
  00092caa: add r1,pc
  00092cac: blx 0x0006ee18
  00092cb0: ldr.w r0,[r9,#0x0]
  00092cb4: ldr r1,[r0,#0x4]
  00092cb6: add r0,sp,#0xc4
  00092cb8: blx 0x0006f658
  00092cbc: add r0,sp,#0xdc
  00092cbe: add r1,sp,#0xd0
  00092cc0: add r2,sp,#0xc4
  00092cc2: blx 0x0006ef98
  00092cc6: ldr.w r1,[r9,#0xa8]
  00092cca: ldr.w r0,[r9,#0x0]
  00092cce: ldr r1,[r1,#0x78]
  00092cd0: blx 0x0006fe20
  00092cd4: add.w r0,r0,r0, lsl #0x2
  00092cd8: movs r1,#0x0
  00092cda: adds r0,#0x1e
  00092cdc: add r2,sp,#0xdc
  00092cde: strd r0,r1,[sp,#0x0]
  00092ce2: mov r0,r5
  00092ce4: mov r1,r6
  00092ce6: movs r3,#0x5
  00092ce8: blx 0x0006fe14
  00092cec: add r0,sp,#0xdc
  00092cee: blx r11
  00092cf0: add r0,sp,#0xc4
  00092cf2: blx r11
  00092cf4: add r0,sp,#0xd0
  00092cf6: blx r11
  00092cf8: ldr.w r0,[r9,#0xa8]
  00092cfc: movs r2,#0x0
  00092cfe: ldr.w r5,[r9,#0x0]
  00092d02: ldr r6,[r0,#0x78]
  00092d04: add r0,sp,#0xb8
  00092d06: ldr r1,[0x00093034]
  00092d08: add r1,pc
  00092d0a: blx 0x0006ee18
  00092d0e: ldr.w r1,[r9,#0xb4]
  00092d12: add r0,sp,#0xac
  00092d14: blx 0x0006f658
  00092d18: add r0,sp,#0xc4
  00092d1a: add r1,sp,#0xb8
  00092d1c: add r2,sp,#0xac
  00092d1e: blx 0x0006ef98
  00092d22: ldr r1,[0x00093038]
  00092d24: add r1,pc
  00092d26: add r0,sp,#0xa0
  00092d28: movs r2,#0x0
  00092d2a: blx 0x0006ee18
  00092d2e: add r0,sp,#0xd0
  00092d30: add r1,sp,#0xc4
  00092d32: add r2,sp,#0xa0
  00092d34: blx 0x0006ef98
  00092d38: ldr.w r1,[r9,#0xb8]
  00092d3c: add r0,sp,#0x94
  00092d3e: blx 0x0006f658
  00092d42: add r0,sp,#0xdc
  00092d44: add r1,sp,#0xd0
  00092d46: add r2,sp,#0x94
  00092d48: blx 0x0006ef98
  00092d4c: ldr.w r1,[r9,#0xa8]
  00092d50: ldr.w r0,[r9,#0x0]
  00092d54: ldr r1,[r1,#0x78]
  00092d56: blx 0x0006fe20
  00092d5a: add.w r0,r0,r0, lsl #0x1
  00092d5e: movs r2,#0x23
  00092d60: movs r1,#0x0
  00092d62: movs r3,#0x5
  00092d64: add.w r0,r2,r0, lsl #0x1
  00092d68: add r2,sp,#0xdc
  00092d6a: strd r0,r1,[sp,#0x0]
  00092d6e: mov r0,r5
  00092d70: mov r1,r6
  00092d72: blx 0x0006fe14
  00092d76: add r0,sp,#0xdc
  00092d78: blx r11
  00092d7a: add r0,sp,#0x94
  00092d7c: blx r11
  00092d7e: add r0,sp,#0xd0
  00092d80: blx r11
  00092d82: add r0,sp,#0xa0
  00092d84: blx r11
  00092d86: add r0,sp,#0xc4
  00092d88: blx r11
  00092d8a: add r0,sp,#0xac
  00092d8c: blx r11
  00092d8e: add r0,sp,#0xb8
  00092d90: blx r11
  00092d92: ldr.w r0,[r9,#0xa8]
  00092d96: movs r2,#0x0
  00092d98: ldr.w r5,[r9,#0x0]
  00092d9c: ldr r6,[r0,#0x78]
  00092d9e: add r0,sp,#0xd0
  00092da0: ldr r1,[0x0009303c]
  00092da2: add r1,pc
  00092da4: blx 0x0006ee18
  00092da8: ldr.w r0,[r9,#0xa8]
  00092dac: ldr r1,[r0,#0x68]
  00092dae: add r0,sp,#0xc4
  00092db0: blx 0x0006f658
  00092db4: add r0,sp,#0xdc
  00092db6: add r1,sp,#0xd0
  00092db8: add r2,sp,#0xc4
  00092dba: blx 0x0006ef98
  00092dbe: ldr.w r1,[r9,#0xa8]
  00092dc2: ldr.w r0,[r9,#0x0]
  00092dc6: ldr r1,[r1,#0x78]
  00092dc8: blx 0x0006fe20
  00092dcc: rsb r0,r0,r0, lsl #0x3
  00092dd0: movs r1,#0x0
  00092dd2: adds r0,#0x23
  00092dd4: add r2,sp,#0xdc
  00092dd6: strd r0,r1,[sp,#0x0]
  00092dda: mov r0,r5
  00092ddc: mov r1,r6
  00092dde: movs r3,#0x5
  00092de0: blx 0x0006fe14
  00092de4: add r0,sp,#0xdc
  00092de6: blx r11
  00092de8: add r0,sp,#0xc4
  00092dea: blx r11
  00092dec: add r0,sp,#0xd0
  00092dee: blx r11
  00092df0: ldr.w r0,[r9,#0xa8]
  00092df4: movs r2,#0x0
  00092df6: ldr.w r5,[r9,#0x0]
  00092dfa: ldr r6,[r0,#0x78]
  00092dfc: add r0,sp,#0xb8
  00092dfe: ldr r1,[0x00093040]
  00092e00: add r1,pc
  00092e02: blx 0x0006ee18
  00092e06: ldr.w r0,[r9,#0xa8]
  00092e0a: ldr r0,[r0,#0x70]
  00092e0c: asrs r1,r0,#0x1f
  00092e0e: add.w r0,r0,r1, lsr #0xc
  00092e12: asrs r1,r0,#0x14
  00092e14: add r0,sp,#0xac
  00092e16: blx 0x0006f658
  00092e1a: add r0,sp,#0xc4
  00092e1c: add r1,sp,#0xb8
  00092e1e: add r2,sp,#0xac
  00092e20: blx 0x0006ef98
  00092e24: ldr r1,[0x00093044]
  00092e26: add r1,pc
  00092e28: add r0,sp,#0xa0
  00092e2a: movs r2,#0x0
  00092e2c: blx 0x0006ee18
  00092e30: add r0,sp,#0xd0
  00092e32: add r1,sp,#0xc4
  00092e34: add r2,sp,#0xa0
  00092e36: blx 0x0006ef98
  00092e3a: ldr r0,[0x00093048]
  00092e3c: add r0,pc
  00092e3e: ldr r0,[r0,#0x0]
  00092e40: ldr r0,[r0,#0x0]
  00092e42: asrs r1,r0,#0x1f
  00092e44: add.w r0,r0,r1, lsr #0x16
  00092e48: asrs r1,r0,#0xa
  00092e4a: add r0,sp,#0x94
  00092e4c: blx 0x0006f658
  00092e50: add r0,sp,#0xdc
  00092e52: add r1,sp,#0xd0
  00092e54: add r2,sp,#0x94
  00092e56: blx 0x0006ef98
  00092e5a: ldr.w r1,[r9,#0xa8]
  00092e5e: ldr.w r0,[r9,#0x0]
  00092e62: ldr r1,[r1,#0x78]
  00092e64: blx 0x0006fe20
  00092e68: movs r2,#0x23
  00092e6a: add.w r0,r2,r0, lsl #0x3
  00092e6e: movs r1,#0x0
  00092e70: add r2,sp,#0xdc
  00092e72: strd r0,r1,[sp,#0x0]
  00092e76: mov r0,r5
  00092e78: mov r1,r6
  00092e7a: movs r3,#0x5
  00092e7c: blx 0x0006fe14
  00092e80: add r0,sp,#0xdc
  00092e82: blx r11
  00092e84: add r0,sp,#0x94
  00092e86: blx r11
  00092e88: add r0,sp,#0xd0
  00092e8a: blx r11
  00092e8c: add r0,sp,#0xa0
  00092e8e: blx r11
  00092e90: add r0,sp,#0xc4
  00092e92: blx r11
  00092e94: add r0,sp,#0xac
  00092e96: blx r11
  00092e98: add r0,sp,#0xb8
  00092e9a: blx r11
  00092e9c: ldr.w r0,[r9,#0xa8]
  00092ea0: movs r2,#0x0
  00092ea2: ldr.w r5,[r9,#0x0]
  00092ea6: ldr r6,[r0,#0x78]
  00092ea8: add r0,sp,#0xd0
  00092eaa: ldr r1,[0x0009304c]
  00092eac: add r1,pc
  00092eae: blx 0x0006ee18
  00092eb2: ldr r0,[0x00093050]
  00092eb4: add r0,pc
  00092eb6: ldr r0,[r0,#0x0]
  00092eb8: ldr r1,[r0,#0x0]
  00092eba: add r0,sp,#0xc4
  00092ebc: blx 0x0006f5d4
  00092ec0: add r0,sp,#0xdc
  00092ec2: add r1,sp,#0xd0
  00092ec4: add r2,sp,#0xc4
  00092ec6: blx 0x0006ef98
  00092eca: ldr.w r1,[r9,#0xa8]
  00092ece: ldr.w r0,[r9,#0x0]
  00092ed2: str r4,[sp,#0x2c]
  00092ed4: ldr r1,[r1,#0x78]
  00092ed6: blx 0x0006fe20
  00092eda: add.w r0,r0,r0, lsl #0x3
  00092ede: movs r1,#0x0
  00092ee0: adds r0,#0x23
  00092ee2: add r2,sp,#0xdc
  00092ee4: strd r0,r1,[sp,#0x0]
  00092ee8: mov r0,r5
  00092eea: mov r1,r6
  00092eec: movs r3,#0x5
  00092eee: mov.w r10,#0x5
  00092ef2: blx 0x0006fe14
  00092ef6: add r0,sp,#0xdc
  00092ef8: blx r11
  00092efa: add r0,sp,#0xc4
  00092efc: blx r11
  00092efe: add r0,sp,#0xd0
  00092f00: blx r11
  00092f02: ldr.w r0,[r9,#0xa8]
  00092f06: ldr.w r5,[r9,#0x0]
  00092f0a: ldr.w r8,[r0,#0x78]
  00092f0e: mov r0,r5
  00092f10: blx 0x0006fafc
  00092f14: ldr r2,[0x00093054]
  00092f16: mov r4,r0
  00092f18: ldr.w r1,[r9,#0xa8]
  00092f1c: add r2,pc
  00092f1e: ldr.w r0,[r9,#0x0]
  00092f22: ldr r6,[r2,#0x0]
  00092f24: ldr r1,[r1,#0x78]
  00092f26: mov r2,r6
  00092f28: blx 0x0006faa8
  00092f2c: subs r0,r4,r0
  00092f2e: movs r1,#0x0
  00092f30: str r1,[sp,#0x4]
  00092f32: mov r1,r8
  00092f34: add.w r0,r0,r0, lsr #0x1f
  00092f38: mov r2,r6
  00092f3a: str.w r10,[sp,#0x0]
  00092f3e: asrs r3,r0,#0x1
  00092f40: mov r0,r5
  00092f42: blx 0x0006fe14
  00092f46: ldr.w r0,[r9,#0xa8]
  00092f4a: ldr.w r10,[r9,#0x0]
  00092f4e: ldr.w r8,[r0,#0x78]
  00092f52: mov r0,r10
  00092f54: blx 0x0006fafc
  00092f58: ldr.w r1,[r9,#0xa8]
  00092f5c: mov r4,r0
  00092f5e: ldr.w r0,[r9,#0x0]
  00092f62: ldr r1,[r1,#0x78]
  00092f64: ldr r2,[0x00093058]
  00092f66: add r2,pc
  00092f68: ldr r6,[r2,#0x0]
  00092f6a: mov r2,r6
  00092f6c: blx 0x0006faa8
  00092f70: ldr.w r1,[r9,#0xa8]
  00092f74: mov r5,r0
  00092f76: ldr.w r0,[r9,#0x0]
  00092f7a: ldr r1,[r1,#0x78]
  00092f7c: blx 0x0006fe20
  00092f80: adds r0,#0xa
  00092f82: str r0,[sp,#0x0]
  00092f84: movs r0,#0x0
  00092f86: mov r1,r8
  00092f88: str r0,[sp,#0x4]
  00092f8a: subs r0,r4,r5
  00092f8c: mov r2,r6
  00092f8e: add.w r0,r0,r0, lsr #0x1f
  00092f92: asrs r3,r0,#0x1
  00092f94: mov r0,r10
  00092f96: blx 0x0006fe14
  00092f9a: ldr r0,[sp,#0x2c]
  00092f9c: ldrb r0,[r0,#0x0]
  00092f9e: cmp r0,#0x0
  00092fa0: beq.w 0x0009337c
  00092fa4: ldr.w r0,[r9,#0xa8]
  00092fa8: movs r2,#0x0
  00092faa: ldr.w r8,[r9,#0x0]
  00092fae: ldr r4,[r0,#0x78]
  00092fb0: add r0,sp,#0xdc
  00092fb2: ldr r1,[0x0009305c]
  00092fb4: add r1,pc
  00092fb6: blx 0x0006ee18
  00092fba: ldr.w r0,[r9,#0x0]
  00092fbe: blx 0x0006fafc
  00092fc2: mov r10,r0
  00092fc4: ldr.w r0,[r9,#0xa8]
  00092fc8: ldr.w r6,[r9,#0x0]
  00092fcc: ldr r5,[r0,#0x78]
  00092fce: ldr r1,[0x00093060]
  00092fd0: add r1,pc
  00092fd2: add r0,sp,#0xd0
  00092fd4: movs r2,#0x0
  00092fd6: blx 0x0006ee18
  00092fda: add r2,sp,#0xd0
  00092fdc: mov r0,r6
  00092fde: mov r1,r5
  00092fe0: blx 0x0006faa8
  00092fe4: movs r2,#0x0
  00092fe6: movs r1,#0x5
  00092fe8: strd r1,r2,[sp,#0x0]
  00092fec: sub.w r3,r10,r0
  00092ff0: add r2,sp,#0xdc
  00092ff2: mov r0,r8
  00092ff4: mov r1,r4
  00092ff6: blx 0x0006fe14
  00092ffa: add r0,sp,#0xd0
  00092ffc: blx 0x0006ee30
  00093000: add r5,sp,#0xdc
  00093002: mov r0,r5
  00093004: blx 0x0006ee30
  00093008: ldr.w r1,[r9,#0xa8]
  0009300c: ldr.w r0,[r9,#0x0]
  00093010: ldr r1,[r1,#0x78]
  00093012: blx 0x0006fe20
  00093016: ldr.w r10,[sp,#0x30]
  0009301a: add.w r6,r0,#0xa
  0009301e: add r2,sp,#0xc4
  00093020: mov.w r8,#0x0
  00093024: str.w r11,[sp,#0x28]
  00093028: b 0x0009310c
  00093064: ldr.w r1,[r0,#0x3dc]
  00093068: ldr.w r1,[r1,r8,lsl #0x2]
  0009306c: cmp r1,#0x0
  0009306e: beq 0x00093108
  00093070: ldr r0,[r0,#0x78]
  00093072: mov r11,r5
  00093074: str r0,[sp,#0x2c]
  00093076: mov r0,r2
  00093078: ldr.w r10,[r9,#0x0]
  0009307c: mov r4,r2
  0009307e: blx 0x0006f658
  00093082: ldr r1,[0x0009336c]
  00093084: add r1,pc
  00093086: add r5,sp,#0xb8
  00093088: movs r2,#0x0
  0009308a: mov r0,r5
  0009308c: blx 0x0006ee18
  00093090: add r0,sp,#0xd0
  00093092: mov r1,r4
  00093094: mov r2,r5
  00093096: blx 0x0006ef98
  0009309a: ldr.w r0,[r9,#0xa8]
  0009309e: ldr.w r0,[r0,#0x514]
  000930a2: ldr.w r1,[r0,r8,lsl #0x2]
  000930a6: add r4,sp,#0xac
  000930a8: mov r0,r4
  000930aa: blx 0x0007048c
  000930ae: add r1,sp,#0xd0
  000930b0: mov r0,r11
  000930b2: mov r2,r4
  000930b4: blx 0x0006ef98
  000930b8: ldr.w r0,[r9,#0x0]
  000930bc: blx 0x0006fafc
  000930c0: mov r5,r11
  000930c2: mov r11,r6
  000930c4: movs r1,#0x0
  000930c6: str r6,[sp,#0x0]
  000930c8: str r1,[sp,#0x4]
  000930ca: sub.w r3,r0,#0x12c
  000930ce: ldr r1,[sp,#0x2c]
  000930d0: mov r0,r10
  000930d2: mov r2,r5
  000930d4: blx 0x0006fe14
  000930d8: ldr r4,[sp,#0x28]
  000930da: mov r0,r5
  000930dc: blx r4
  000930de: add r0,sp,#0xac
  000930e0: blx r4
  000930e2: add r0,sp,#0xd0
  000930e4: blx r4
  000930e6: add r0,sp,#0xb8
  000930e8: blx r4
  000930ea: add r6,sp,#0xc4
  000930ec: mov r0,r6
  000930ee: blx r4
  000930f0: ldr.w r1,[r9,#0xa8]
  000930f4: ldr.w r0,[r9,#0x0]
  000930f8: ldr r1,[r1,#0x78]
  000930fa: blx 0x0006fe20
  000930fe: add r0,r11
  00093100: ldr.w r10,[sp,#0x30]
  00093104: mov r2,r6
  00093106: adds r6,r0,#0x5
  00093108: add.w r8,r8,#0x1
  0009310c: ldr.w r0,[r9,#0xa8]
  00093110: ldr.w r1,[r0,#0x3d8]
  00093114: cmp r8,r1
  00093116: bcc 0x00093064
  00093118: b 0x00093384
  0009311a: mov r5,r9
  0009311c: ldr.w r0,[r5,#0x68]!
  00093120: ldr r1,[r5,#0x4]
  00093122: orrs.w r2,r0,r1
  00093126: bne.w 0x00093338
  0009312a: ldr.w r0,[r9,#0x0]
  0009312e: blx 0x00070498
  00093132: cmp r0,#0x0
  00093134: beq.w 0x00093334
  00093138: subs.w r1,r10,#0x1
  0009313c: mov.w r0,#0x5
  00093140: sbc r2,r8,#0x0
  00093144: str.w r0,[r9,#0x3c]
  00093148: movs r0,#0x0
  0009314a: strd r0,r0,[r5,#0x0]
  0009314e: strd r0,r0,[r9,#0xa0]
  00093152: strd r10,r8,[r9,#0x70]
  00093156: strd r1,r2,[r9,#0x78]
  0009315a: movs r1,#0x0
  0009315c: strd r0,r0,[r9,#0x80]
  00093160: b 0x00093338
  00093162: ldr r0,[0x00093370]
  00093164: ldr.w r1,[r9,#0x0]
  00093168: add r0,pc
  0009316a: ldr r6,[r0,#0x0]
  0009316c: ldr r2,[r1,#0x10]
  0009316e: ldr r0,[r6,#0x0]
  00093170: cmp r0,r2
  00093172: bcc 0x00093184
  00093174: ldr r2,[0x00093374]
  00093176: ldr r1,[r1,#0x24]
  00093178: add r2,pc
  0009317a: ldr r2,[r2,#0x0]
  0009317c: ldr r2,[r2,#0x0]
  0009317e: cmp r2,r1
  00093180: bcs.w 0x000932c6
  00093184: cmp r0,#0x0
  00093186: beq.w 0x0009329e
  0009318a: ldr.w r1,[r9,#0x0]
  0009318e: ldr r0,[r6,#0x0]
  00093190: ldr r2,[r1,#0x10]
  00093192: cmp r0,r2
  00093194: bcs 0x000931da
  00093196: ldr r1,[r1,#0x14]
  00093198: ldr.w r5,[r1,r0,lsl #0x2]
  0009319c: cbz r5,0x000931d6
  0009319e: ldrb r1,[r5,#0x15]
  000931a0: cbnz r1,0x000931d6
  000931a2: adds r0,r5,#0x4
  000931a4: blx 0x0006ef68
  000931a8: mov r4,r8
  000931aa: mov r8,r0
  000931ac: ldr.w r0,[r9,#0xa8]
  000931b0: movs r1,#0x1
  000931b2: vldr.32 s0,[r5,#0x10]
  000931b6: movs r2,#0x0
  000931b8: strd r5,r1,[sp,#0x8]
  000931bc: add r1,sp,#0xdc
  000931be: str r1,[sp,#0x0]
  000931c0: mov r1,r8
  000931c2: movs r3,#0x0
  000931c4: vstr.32 s0,[sp,#0x4]
  000931c8: blx 0x0006f220
  000931cc: mov r0,r8
  000931ce: mov r8,r4
  000931d0: blx 0x0006ebfc
  000931d4: ldr r0,[r6,#0x0]
  000931d6: adds r0,#0x1
  000931d8: str r0,[r6,#0x0]
  000931da: ldr r0,[0x00093378]
  000931dc: movs r5,#0xa
  000931de: add r0,pc
  000931e0: ldr r6,[r0,#0x0]
  000931e2: b 0x000931fc
  000931e4: ldr.w r0,[r9,#0x0]
  000931e8: ldr r1,[r6,#0x0]
  000931ea: ldr r2,[r0,#0x24]
  000931ec: cmp r1,r2
  000931ee: bcs 0x000931fa
  000931f0: blx 0x000704a4
  000931f4: ldr r0,[r6,#0x0]
  000931f6: adds r0,#0x1
  000931f8: str r0,[r6,#0x0]
  000931fa: subs r5,#0x1
  000931fc: cmp r5,#0x0
  000931fe: bne 0x000931e4
  00093200: mov r3,r9
  00093202: ldr.w r5,[r3,#0x70]!
  00093206: add.w r2,r3,#0x8
  0009320a: mov r0,r3
  0009320c: ldr.w r12,[r3,#-0x4]
  00093210: ldr.w r1,[r0,#-0x8]!
  00093214: ldr r6,[r3,#0x4]
  00093216: b 0x00093302
  00093218: ldr.w r0,[r9,#0x60]
  0009321c: cbz r0,0x00093244
  0009321e: movs r1,#0x0
  00093220: str.w r1,[r9,#0x60]
  00093224: ldr.w r1,[r9,#0x20]
  00093228: str.w r0,[r9,#0x18]
  0009322c: cbz r1,0x0009324e
  0009322e: ldr r1,[r0,#0x0]
  00093230: ldr r1,[r1,#0x44]
  00093232: blx r1
  00093234: cbz r0,0x00093244
  00093236: ldr.w r0,[r9,#0x0]
  0009323a: mov.w r1,#0xffffffff
  0009323e: ldrd r3,r2,[r9,#0x20]
  00093242: blx r3
  00093244: ldr.w r0,[r9,#0x18]
  00093248: cmp r0,#0x0
  0009324a: beq.w 0x00093732
  0009324e: ldr r1,[r0,#0x0]
  00093250: ldr r1,[r1,#0x8]
  00093252: blx r1
  00093254: mov r5,r0
  00093256: ldr.w r0,[r9,#0x20]
  0009325a: cbz r0,0x00093274
  0009325c: ldr.w r0,[r9,#0x18]
  00093260: ldr r1,[r0,#0x0]
  00093262: ldr r1,[r1,#0x44]
  00093264: blx r1
  00093266: cbz r0,0x00093274
  00093268: ldr.w r0,[r9,#0x0]
  0009326c: mov r1,r5
  0009326e: ldrd r3,r2,[r9,#0x20]
  00093272: blx r3
  00093274: cmp r5,#0x0
  00093276: beq.w 0x00093750
  0009327a: add.w r3,r9,#0x68
  0009327e: add.w r12,r9,#0x68
  00093282: ldmia r3,{r0,r1,r2,r3}
  00093284: subs.w r6,r10,r2
  00093288: sbc.w r5,r8,r3
  0009328c: adds r0,r0,r6
  0009328e: adcs r1,r5
  00093290: stm r12,{r0,r1,r10}
  00093294: strd r8,r2,[r9,#0x74]
  00093298: str.w r3,[r9,#0x7c]
  0009329c: b 0x00093732
  0009329e: ldr.w r0,[r9,#0x20]
  000932a2: cmp r0,#0x0
  000932a4: beq.w 0x0009318a
  000932a8: ldr.w r0,[r9,#0x18]
  000932ac: ldr r1,[r0,#0x0]
  000932ae: ldr r1,[r1,#0x44]
  000932b0: blx r1
  000932b2: cmp r0,#0x0
  000932b4: beq.w 0x0009318a
  000932b8: ldr.w r0,[r9,#0x0]
  000932bc: movs r1,#0x0
  000932be: ldrd r3,r2,[r9,#0x20]
  000932c2: blx r3
  000932c4: b 0x0009318a
  000932c6: mov r2,r9
  000932c8: subs.w r0,r10,#0x1
  000932cc: str.w r0,[r2,#0x78]!
  000932d0: sbc r0,r8,#0x0
  000932d4: movs r1,#0x0
  000932d6: mov.w r12,#0x0
  000932da: str.w r8,[r2,#-0x4]
  000932de: mov r3,r2
  000932e0: str.w r1,[r2,#-0xc]
  000932e4: mov r5,r10
  000932e6: strd r0,r1,[r2,#0x4]
  000932ea: movs r0,#0x5
  000932ec: str r1,[r2,#0xc]
  000932ee: mov r6,r8
  000932f0: strd r1,r1,[r2,#0x28]
  000932f4: str.w r0,[r2,#-0x3c]
  000932f8: mov r0,r2
  000932fa: str.w r1,[r0,#-0x10]!
  000932fe: str.w r10,[r3,#-0x8]!
  00093302: strd r5,r6,[r2,#0x0]
  00093306: subs.w r2,r10,r5
  0009330a: strd r10,r8,[r3,#0x0]
  0009330e: sbc.w r3,r8,r6
  00093312: adds r1,r1,r2
  00093314: adc.w r2,r12,r3
  00093318: movs r3,#0x0
  0009331a: rsbs.w r6,r1,#0x1f4
  0009331e: sbcs.w r6,r3,r2
  00093322: it lt
  00093324: mov.lt r3,#0x1
  00093326: cmp r3,#0x0
  00093328: itt ne
  0009332a: mov.ne r2,#0x0
  0009332c: mov.ne r1,#0x0
  0009332e: strd r1,r2,[r0,#0x0]
  00093332: b 0x00093732
  00093334: ldrd r0,r1,[r5,#0x0]
  00093338: ldrd r2,r3,[r9,#0x70]
  0009333c: strd r10,r8,[r9,#0x70]
  00093340: strd r2,r3,[r9,#0x78]
  00093344: subs.w r2,r10,r2
  00093348: sbc.w r3,r8,r3
  0009334c: adds r0,r0,r2
  0009334e: adcs r1,r3
  00093350: movs r2,#0x0
  00093352: rsbs.w r3,r0,#0x1f4
  00093356: sbcs.w r3,r2,r1
  0009335a: it lt
  0009335c: mov.lt r2,#0x1
  0009335e: cmp r2,#0x0
  00093360: itt ne
  00093362: mov.ne r1,#0x0
  00093364: mov.ne r0,#0x0
  00093366: strd r0,r1,[r5,#0x0]
  0009336a: b 0x00093732
  0009337c: ldr.w r0,[r9,#0xa8]
  00093380: ldr.w r10,[sp,#0x30]
  00093384: movs r1,#0x1
  00093386: strb.w r1,[r0,#0xfe]
  0009338a: strb.w r1,[r0,#0xfc]
  0009338e: ldrd r8,r11,[sp,#0x20]
  00093392: ldr r0,[0x00093788]
  00093394: add r0,pc
  00093396: ldr r0,[r0,#0x0]
  00093398: ldrb r0,[r0,#0x0]
  0009339a: cmp r0,#0x0
  0009339c: beq.w 0x000934fc
  000933a0: ldr.w r0,[r9,#0x0]
  000933a4: movs r1,#0xff
  000933a6: str r1,[sp,#0x0]
  000933a8: movs r1,#0x0
  000933aa: movs r2,#0xff
  000933ac: movs r3,#0x0
  000933ae: blx 0x0006ff28
  000933b2: ldr r0,[0x0009378c]
  000933b4: ldr r1,[0x00093790]
  000933b6: add r0,pc
  000933b8: ldr r2,[0x00093794]
  000933ba: add r1,pc
  000933bc: ldr r0,[r0,#0x0]
  000933be: add r2,pc
  000933c0: ldr r1,[r1,#0x0]
  000933c2: ldrd r8,r4,[r0,#0x0]
  000933c6: ldrd r11,r0,[r1,#0x0]
  000933ca: ldr r2,[r2,#0x0]
  000933cc: str r0,[sp,#0x2c]
  000933ce: add r0,sp,#0xa0
  000933d0: ldrd r5,r6,[r2,#0x0]
  000933d4: movs r2,#0x0
  000933d6: ldr r1,[0x00093798]
  000933d8: add r1,pc
  000933da: blx 0x0006ee18
  000933de: mov r0,r5
  000933e0: mov r1,r6
  000933e2: mov.w r2,#0x3e8
  000933e6: movs r3,#0x0
  000933e8: bl 0x001aae54
  000933ec: mov r10,r0
  000933ee: asrs r3,r0,#0x1f
  000933f0: mov r0,r8
  000933f2: mov r1,r4
  000933f4: mov r2,r10
  000933f6: bl 0x001aae54
  000933fa: mov.w r3,#0x3e8
  000933fe: mov r8,r0
  00093400: mov r4,r1
  00093402: umull r0,r1,r11,r3
  00093406: ldr r2,[sp,#0x2c]
  00093408: mla r1,r2,r3,r1
  0009340c: mov r2,r5
  0009340e: mov r3,r6
  00093410: bl 0x001aae54
  00093414: mov r1,r0
  00093416: add r0,sp,#0x94
  00093418: blx 0x0006f658
  0009341c: add r0,sp,#0xac
  0009341e: add r1,sp,#0xa0
  00093420: add r2,sp,#0x94
  00093422: blx 0x0006ef98
  00093426: ldr r1,[0x0009379c]
  00093428: add r1,pc
  0009342a: add r0,sp,#0x88
  0009342c: movs r2,#0x0
  0009342e: blx 0x0006ee18
  00093432: ldr.w r11,[sp,#0x24]
  00093436: add r0,sp,#0xb8
  00093438: add r1,sp,#0xac
  0009343a: add r2,sp,#0x88
  0009343c: blx 0x0006ef98
  00093440: add r0,sp,#0x7c
  00093442: mov r1,r10
  00093444: blx 0x0006f658
  00093448: add r0,sp,#0xc4
  0009344a: add r1,sp,#0xb8
  0009344c: add r2,sp,#0x7c
  0009344e: blx 0x0006ef98
  00093452: ldr.w r10,[sp,#0x30]
  00093456: ldr r1,[0x000937a0]
  00093458: add r1,pc
  0009345a: add r0,sp,#0x70
  0009345c: movs r2,#0x0
  0009345e: blx 0x0006ee18
  00093462: add r0,sp,#0xd0
  00093464: add r1,sp,#0xc4
  00093466: add r2,sp,#0x70
  00093468: blx 0x0006ef98
  0009346c: add r0,sp,#0x64
  0009346e: mov r2,r8
  00093470: mov r3,r4
  00093472: blx 0x0006f5bc
  00093476: add r0,sp,#0xdc
  00093478: add r1,sp,#0xd0
  0009347a: add r2,sp,#0x64
  0009347c: blx 0x0006ef98
  00093480: ldr r0,[0x000937a4]
  00093482: add r0,pc
  00093484: ldr r5,[r0,#0x0]
  00093486: add r0,sp,#0x64
  00093488: blx r5
  0009348a: add r0,sp,#0xd0
  0009348c: blx r5
  0009348e: add r0,sp,#0x70
  00093490: blx r5
  00093492: add r0,sp,#0xc4
  00093494: blx r5
  00093496: add r0,sp,#0x7c
  00093498: blx r5
  0009349a: add r0,sp,#0xb8
  0009349c: blx r5
  0009349e: add r0,sp,#0x88
  000934a0: blx r5
  000934a2: add r0,sp,#0xac
  000934a4: blx r5
  000934a6: add r0,sp,#0x94
  000934a8: blx r5
  000934aa: add r0,sp,#0xa0
  000934ac: blx r5
  000934ae: ldr.w r0,[r9,#0xa8]
  000934b2: ldr.w r5,[r9,#0x0]
  000934b6: ldr.w r8,[r0,#0x78]
  000934ba: mov r0,r5
  000934bc: blx 0x0006fafc
  000934c0: ldr.w r1,[r9,#0xa8]
  000934c4: mov r6,r0
  000934c6: ldr.w r0,[r9,#0x0]
  000934ca: ldr r1,[r1,#0x78]
  000934cc: add r2,sp,#0xdc
  000934ce: blx 0x0006faa8
  000934d2: movs r1,#0x0
  000934d4: movs r2,#0x5
  000934d6: strd r2,r1,[sp,#0x0]
  000934da: add.w r1,r6,r6, lsr #0x1f
  000934de: add.w r0,r0,r0, lsr #0x1f
  000934e2: add r2,sp,#0xdc
  000934e4: asrs r1,r1,#0x1
  000934e6: sub.w r3,r1,r0, asr #0x1
  000934ea: mov r0,r5
  000934ec: mov r1,r8
  000934ee: blx 0x0006fe14
  000934f2: add r0,sp,#0xdc
  000934f4: blx 0x0006ee30
  000934f8: ldr.w r8,[sp,#0x20]
  000934fc: ldr r0,[sp,#0x1c]
  000934fe: ldrb r0,[r0,#0x0]
  00093500: cmp r0,#0x0
  00093502: beq.w 0x0009370e
  00093506: ldr r0,[0x000937a8]
  00093508: mov r4,r8
  0009350a: ldr.w r1,[r9,#0xa8]
  0009350e: add r0,pc
  00093510: ldr.w r8,[r0,#0x0]
  00093514: ldrd r0,r2,[r1,#0x58]
  00093518: ldrd r1,r3,[r1,#0x68]
  0009351c: add r2,r3
  0009351e: ldrd r6,r5,[r8,#0x0]
  00093522: movs r3,#0x0
  00093524: add r0,r1
  00093526: adds r1,r6,r0
  00093528: adc.w r0,r5,r0, asr #0x1f
  0009352c: adds r1,r1,r2
  0009352e: adc.w r0,r0,r2, asr #0x1f
  00093532: movs r2,#0xff
  00093534: strd r1,r0,[r8,#0x0]
  00093538: movs r1,#0xff
  0009353a: ldr.w r0,[r9,#0x0]
  0009353e: str r1,[sp,#0x0]
  00093540: movs r1,#0x0
  00093542: blx 0x0006ff28
  00093546: ldr r0,[0x000937ac]
  00093548: movs r2,#0x0
  0009354a: add r0,pc
  0009354c: ldr r0,[r0,#0x0]
  0009354e: ldrd r5,r6,[r0,#0x0]
  00093552: add r0,sp,#0x88
  00093554: ldr r1,[0x000937b0]
  00093556: add r1,pc
  00093558: blx 0x0006ee18
  0009355c: mov r0,r5
  0009355e: mov r1,r6
  00093560: mov.w r2,#0x3e8
  00093564: movs r3,#0x0
  00093566: bl 0x001aae54
  0009356a: mov r5,r0
  0009356c: ldr r0,[0x000937b4]
  0009356e: add r0,pc
  00093570: ldr r0,[r0,#0x0]
  00093572: ldrd r2,r3,[r0,#0x0]
  00093576: add r0,sp,#0x7c
  00093578: blx 0x0006f5bc
  0009357c: add r0,sp,#0x94
  0009357e: add r1,sp,#0x88
  00093580: add r2,sp,#0x7c
  00093582: blx 0x0006ef98
  00093586: ldr r1,[0x000937b8]
  00093588: add r1,pc
  0009358a: add r0,sp,#0x70
  0009358c: movs r2,#0x0
  0009358e: blx 0x0006ee18
  00093592: add r0,sp,#0xa0
  00093594: add r1,sp,#0x94
  00093596: add r2,sp,#0x70
  00093598: blx 0x0006ef98
  0009359c: ldr r0,[0x000937bc]
  0009359e: add r0,pc
  000935a0: ldr r0,[r0,#0x0]
  000935a2: ldrd r2,r3,[r0,#0x0]
  000935a6: add r0,sp,#0x64
  000935a8: blx 0x0006f5bc
  000935ac: add r0,sp,#0xac
  000935ae: add r1,sp,#0xa0
  000935b0: add r2,sp,#0x64
  000935b2: blx 0x0006ef98
  000935b6: ldr r1,[0x000937c0]
  000935b8: add r1,pc
  000935ba: add r0,sp,#0x58
  000935bc: movs r2,#0x0
  000935be: blx 0x0006ee18
  000935c2: add r0,sp,#0xb8
  000935c4: add r1,sp,#0xac
  000935c6: add r2,sp,#0x58
  000935c8: blx 0x0006ef98
  000935cc: add r0,sp,#0x4c
  000935ce: mov r1,r5
  000935d0: blx 0x0006f658
  000935d4: add r0,sp,#0xc4
  000935d6: add r1,sp,#0xb8
  000935d8: add r2,sp,#0x4c
  000935da: blx 0x0006ef98
  000935de: ldr r1,[0x000937c4]
  000935e0: add r1,pc
  000935e2: add r0,sp,#0x40
  000935e4: movs r2,#0x0
  000935e6: blx 0x0006ee18
  000935ea: add r0,sp,#0xd0
  000935ec: add r1,sp,#0xc4
  000935ee: add r2,sp,#0x40
  000935f0: blx 0x0006ef98
  000935f4: ldrd r2,r3,[r8,#0x0]
  000935f8: add r0,sp,#0x34
  000935fa: blx 0x0006f5bc
  000935fe: add r0,sp,#0xdc
  00093600: add r1,sp,#0xd0
  00093602: add r2,sp,#0x34
  00093604: blx 0x0006ef98
  00093608: ldr r0,[0x000937c8]
  0009360a: add r0,pc
  0009360c: ldr r6,[r0,#0x0]
  0009360e: add r0,sp,#0x34
  00093610: blx r6
  00093612: add r0,sp,#0xd0
  00093614: blx r6
  00093616: add r0,sp,#0x40
  00093618: blx r6
  0009361a: add r0,sp,#0xc4
  0009361c: blx r6
  0009361e: add r0,sp,#0x4c
  00093620: blx r6
  00093622: add r0,sp,#0xb8
  00093624: blx r6
  00093626: add r0,sp,#0x58
  00093628: blx r6
  0009362a: add r0,sp,#0xac
  0009362c: blx r6
  0009362e: add r0,sp,#0x64
  00093630: blx r6
  00093632: add r0,sp,#0xa0
  00093634: blx r6
  00093636: add r0,sp,#0x70
  00093638: blx r6
  0009363a: add r0,sp,#0x94
  0009363c: blx r6
  0009363e: add r0,sp,#0x7c
  00093640: blx r6
  00093642: add r0,sp,#0x88
  00093644: blx r6
  00093646: ldr.w r0,[r9,#0xa8]
  0009364a: ldr.w r5,[r9,#0x0]
  0009364e: ldr.w r8,[r0,#0x78]
  00093652: mov r0,r5
  00093654: blx 0x0006fafc
  00093658: add.w r0,r0,r0, lsr #0x1f
  0009365c: movs r1,#0x0
  0009365e: movs r2,#0x5
  00093660: strd r2,r1,[sp,#0x0]
  00093664: add r2,sp,#0xdc
  00093666: asrs r3,r0,#0x1
  00093668: mov r0,r5
  0009366a: mov r1,r8
  0009366c: blx 0x0006fe14
  00093670: ldr.w r0,[r9,#0x0]
  00093674: blx 0x0006fafc
  00093678: mov r1,r0
  0009367a: add r0,sp,#0xb8
  0009367c: blx 0x0006f658
  00093680: ldr r1,[0x000939cc]
  00093682: add r1,pc
  00093684: add r0,sp,#0xac
  00093686: movs r2,#0x0
  00093688: blx 0x0006ee18
  0009368c: add r0,sp,#0xc4
  0009368e: add r1,sp,#0xb8
  00093690: add r2,sp,#0xac
  00093692: blx 0x0006ef98
  00093696: ldr.w r0,[r9,#0x0]
  0009369a: blx 0x0006fb08
  0009369e: mov r1,r0
  000936a0: add r0,sp,#0xa0
  000936a2: blx 0x0006f658
  000936a6: add r0,sp,#0xd0
  000936a8: add r1,sp,#0xc4
  000936aa: add r2,sp,#0xa0
  000936ac: blx 0x0006ef98
  000936b0: add r0,sp,#0xdc
  000936b2: add r1,sp,#0xd0
  000936b4: blx 0x0006f2b0
  000936b8: add r0,sp,#0xd0
  000936ba: blx r6
  000936bc: add r0,sp,#0xa0
  000936be: blx r6
  000936c0: add r0,sp,#0xc4
  000936c2: blx r6
  000936c4: add r0,sp,#0xac
  000936c6: blx r6
  000936c8: add r0,sp,#0xb8
  000936ca: blx r6
  000936cc: ldr.w r0,[r9,#0xa8]
  000936d0: ldr.w r5,[r9,#0x0]
  000936d4: ldr.w r8,[r0,#0x78]
  000936d8: mov r0,r5
  000936da: blx 0x0006fafc
  000936de: ldr.w r1,[r9,#0xa8]
  000936e2: mov r6,r0
  000936e4: ldr.w r0,[r9,#0x0]
  000936e8: ldr r1,[r1,#0x78]
  000936ea: blx 0x0006fe20
  000936ee: movs r1,#0x0
  000936f0: adds r0,#0xa
  000936f2: strd r0,r1,[sp,#0x0]
  000936f6: add.w r0,r6,r6, lsr #0x1f
  000936fa: add r2,sp,#0xdc
  000936fc: mov r1,r8
  000936fe: asrs r3,r0,#0x1
  00093700: mov r0,r5
  00093702: blx 0x0006fe14
  00093706: add r0,sp,#0xdc
  00093708: blx 0x0006ee30
  0009370c: mov r8,r4
  0009370e: ldr r2,[sp,#0x18]
  00093710: ldr r3,[sp,#0x14]
  00093712: ldrd r0,r1,[r2,#0x0]
  00093716: strd r0,r1,[r3,#0x0]
  0009371a: subs.w r0,r10,r0
  0009371e: strd r10,r8,[r2,#0x0]
  00093722: sbc.w r1,r8,r1
  00093726: ldrd r2,r3,[r9,#0x68]
  0009372a: adds r0,r0,r2
  0009372c: adcs r1,r3
  0009372e: strd r0,r1,[r9,#0x68]
  00093732: ldr.w r0,[r9,#0xa8]
  00093736: blx 0x0007045c
  0009373a: ldr r0,[sp,#0xe8]
  0009373c: ldr.w r1,[r11,#0x0]
  00093740: subs r0,r1,r0
  00093742: ittt eq
  00093744: add.eq sp,#0xec
  00093746: pop.eq.w {r8,r9,r10,r11}
  0009374a: pop.eq {r4,r5,r6,r7,pc}
  0009374c: blx 0x0006e824
  00093750: ldr r0,[0x000939c4]
  00093752: movs r3,#0x0
  00093754: ldr r1,[0x000939c8]
  00093756: movs r6,#0x5
  00093758: add r0,pc
  0009375a: strd r3,r3,[r9,#0xa0]
  0009375e: add r1,pc
  00093760: str.w r6,[r9,#0x3c]
  00093764: ldr r0,[r0,#0x0]
  00093766: subs.w r2,r10,#0x1
  0009376a: ldr r1,[r1,#0x0]
  0009376c: sbc r6,r8,#0x0
  00093770: strd r3,r3,[r9,#0x68]
  00093774: strd r10,r8,[r9,#0x70]
  00093778: strd r2,r6,[r9,#0x78]
  0009377c: strd r3,r3,[r9,#0x80]
  00093780: str r3,[r0,#0x0]
  00093782: str r3,[r1,#0x0]
  00093784: b 0x00093732
  001ab608: bx pc
```
