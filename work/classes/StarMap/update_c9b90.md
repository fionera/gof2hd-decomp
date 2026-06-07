# StarMap::update
elf 0xc9b90 body 2970
Sig: undefined __thiscall update(StarMap * this, int param_1)

## decompile
```c

/* StarMap::update(int) */

void __thiscall StarMap::update(StarMap *this,int param_1)

{
  byte bVar1;
  int *piVar2;
  int iVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  Matrix *pMVar8;
  undefined4 uVar9;
  void *pvVar10;
  uint *puVar11;
  int iVar12;
  int iVar13;
  Vector *pVVar14;
  int iVar15;
  code *pcVar16;
  uint uVar17;
  AEGeometry *this_00;
  undefined4 *puVar18;
  int iVar19;
  char cVar20;
  uint in_fpscr;
  uint uVar21;
  float in_s0;
  undefined4 uVar22;
  int iVar23;
  float extraout_s0;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s2;
  float extraout_s2_00;
  int iVar24;
  int iVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  longlong lVar29;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  Vector aVStack_b0 [12];
  Vector aVStack_a4 [12];
  Vector aVStack_98 [12];
  Matrix aMStack_8c [12];
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  int local_44;
  
  piVar2 = *(int **)(DAT_000d9d30 + 0xd9ba6);
  local_44 = *piVar2;
  iVar3 = *(int *)(this + 4);
  *(int *)(this + 0x18) = param_1;
  if (((iVar3 == 0) || (this[0x138] != (StarMap)0x0)) || (this[0x139] != (StarMap)0x0)) {
    puVar11 = *(uint **)(this + 0x194);
    puVar18 = *(undefined4 **)(DAT_000d9d34 + 0xd9bdc);
    for (uVar17 = 0; uVar17 < *puVar11; uVar17 = uVar17 + 1) {
      pVVar14 = (Vector *)*puVar18;
      AEGeometry::getPosition();
      iVar3 = AbyssEngine::PaintCanvas::GetScreenPosition(pVVar14,(Vector *)&local_80);
      puVar11 = *(uint **)(this + 0x194);
      in_s0 = -1.0;
      if (iVar3 != 0) {
        in_s0 = 1.0;
      }
      *(float *)(*(int *)(puVar11[1] + uVar17 * 4) + 8) = in_s0;
    }
    iVar3 = *(int *)(this + 4);
  }
  if (((iVar3 == 3) || (this[0x138] != (StarMap)0x0)) || (this[0x139] != (StarMap)0x0)) {
    puVar11 = *(uint **)(this + 0x198);
    puVar18 = *(undefined4 **)(DAT_000d9d38 + 0xd9c58);
    for (uVar17 = 0; uVar17 < *puVar11; uVar17 = uVar17 + 1) {
      pMVar8 = (Matrix *)AEGeometry::getMatrix();
      AEGeometry::getMatrix();
      AbyssEngine::AEMath::operator*((Matrix *)&local_80,pMVar8);
      AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_ec);
      AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x84),(Vector *)&local_ec);
      iVar3 = AbyssEngine::PaintCanvas::GetScreenPosition
                        ((Vector *)*puVar18,(Vector *)(this + 0x84));
      puVar11 = *(uint **)(this + 0x198);
      in_s0 = -1.0;
      if (iVar3 != 0) {
        in_s0 = 1.0;
      }
      *(float *)(*(int *)(puVar11[1] + uVar17 * 4) + 8) = in_s0;
    }
  }
  if (*(int *)(this + 0xf8) != 0) {
    fVar4 = (float)AbyssEngine::EaseInOut::GetValue();
    fVar5 = (float)AbyssEngine::EaseInOut::GetMinValue(*(EaseInOut **)(this + 0x184));
    fVar6 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar7 = (float)AbyssEngine::EaseInOut::GetMinValue(*(EaseInOut **)(this + 0x184));
    fVar6 = (fVar4 - fVar5) / (fVar6 - fVar7);
    if (this[0x139] == (StarMap)0x0) {
      if ((this[0x138] == (StarMap)0x0) && (*(int *)(this + 4) == 0)) {
        fVar6 = DAT_000d9d3c;
      }
    }
    else {
      fVar6 = 1.0 - fVar6;
    }
    AEGeometry::setScaling
              ((float)(DAT_000d9d48 + (double)fVar6 * DAT_000d9d40),extraout_s1,fVar4 - fVar5);
    if ((*(int *)(this + 0xa4) != 0) && (-1 < *(int *)(this + 0x1c4))) {
      AEGeometry::getPosition();
      pMVar8 = (Matrix *)AEGeometry::getMatrix();
      AEGeometry::getMatrix();
      AbyssEngine::AEMath::operator*((Matrix *)&local_80,pMVar8);
      AbyssEngine::AEMath::MatrixGetPosition(aMStack_8c);
      this_00 = *(AEGeometry **)(this + 0xf8);
      fVar6 = (float)AbyssEngine::AEMath::operator-(aVStack_b0,(Vector *)aMStack_8c);
      AbyssEngine::AEMath::operator*(fVar6,aVStack_a4);
      AbyssEngine::AEMath::operator+(aVStack_98,(Vector *)&local_ec);
      AEGeometry::setPosition(this_00,aVStack_98);
    }
    puVar11 = *(uint **)(DAT_000da05c + 0xd9e2a);
    uVar17 = *puVar11;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar17);
    AbyssEngine::AEMath::MatrixGetPosition((Matrix *)&local_80);
    pVVar14 = (Vector *)(this + 0x78);
    AbyssEngine::AEMath::Vector::operator=(pVVar14,(Vector *)&local_80);
    AEGeometry::getPosition();
    AbyssEngine::AEMath::Vector::operator-=(pVVar14,(Vector *)&local_80);
    AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_ec,pVVar14);
    AbyssEngine::AEMath::Vector::operator=(pVVar14,(Vector *)&local_ec);
    *(float *)(this + 0x78) = *(float *)(this + 0x78) + 0.5;
    local_e8 = 0x3f800000;
    local_ec = 0;
    local_e4 = 0;
    AEGeometry::setDirection(*(Vector **)(this + 0xf8),pVVar14);
    lVar29 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar11);
    in_s0 = (float)AbyssEngine::Transform::Update(lVar29,SUB41(param_1,0));
  }
  if ((this[0x138] != (StarMap)0x0) || (this[0x139] != (StarMap)0x0)) {
    uVar22 = VectorSignedToFloat(param_1 * 0xf,(byte)(in_fpscr >> 0x16) & 3);
    pcVar16 = *(code **)(DAT_000da060 + 0xd9edc);
    (*pcVar16)(*(undefined4 *)(this + 0x17c),uVar22);
    (*pcVar16)(*(undefined4 *)(this + 0x180),uVar22);
    (*pcVar16)(*(undefined4 *)(this + 0x184),uVar22);
    pcVar16 = *(code **)(DAT_000da064 + 0xd9efe);
    uVar22 = (*pcVar16)(*(undefined4 *)(this + 0x17c));
    uVar9 = (*pcVar16)(*(undefined4 *)(this + 0x180));
    local_78 = (*pcVar16)(*(undefined4 *)(this + 0x184));
    local_80 = uVar22;
    uStack_7c = uVar9;
    AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x78),(Vector *)&local_80);
    puVar11 = *(uint **)(DAT_000da068 + 0xd9f26);
    uVar17 = *puVar11;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    uVar22 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar17);
    __aeabi_memcpy(&local_80,uVar22,0x3c);
    AbyssEngine::AEMath::MatrixSetTranslation
              ((Matrix *)&local_ec,*(float *)(this + 0x80),extraout_s1_00,extraout_s2);
    uVar17 = *puVar11;
    pMVar8 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraSetLocal(uVar17,pMVar8);
    fVar7 = *(float *)(this + 0x78);
    fVar4 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar28 = *(float *)(this + 0x78);
    fVar5 = (float)AbyssEngine::EaseInOut::GetMaxValue();
    fVar6 = -(fVar28 - fVar5);
    if (0.0 < fVar7 - fVar4) {
      fVar6 = fVar28 - fVar5;
    }
    if (fVar6 <= 1.0) {
      fVar7 = *(float *)(this + 0x7c);
      fVar4 = (float)AbyssEngine::EaseInOut::GetMaxValue();
      fVar28 = *(float *)(this + 0x7c);
      fVar5 = (float)AbyssEngine::EaseInOut::GetMaxValue();
      fVar6 = -(fVar28 - fVar5);
      if (0.0 < fVar7 - fVar4) {
        fVar6 = fVar28 - fVar5;
      }
      if (fVar6 <= 1.0) {
        fVar7 = *(float *)(this + 0x80);
        fVar4 = (float)AbyssEngine::EaseInOut::GetMaxValue();
        fVar28 = *(float *)(this + 0x80);
        fVar5 = (float)AbyssEngine::EaseInOut::GetMaxValue();
        fVar6 = -(fVar28 - fVar5);
        if (0.0 < fVar7 - fVar4) {
          fVar6 = fVar28 - fVar5;
        }
        if (fVar6 <= 1.0) {
          if (this[0x138] == (StarMap)0x0) {
            ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0x90));
            if (*(Array<AEGeometry*> **)(this + 0x90) != (Array<AEGeometry*> *)0x0) {
              pvVar10 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x90));
              operator_delete(pvVar10);
            }
            *(undefined4 *)(this + 0x90) = 0;
            ArrayReleaseClasses<AEGeometry*>(*(Array **)(this + 0x94));
            if (*(Array<AEGeometry*> **)(this + 0x94) != (Array<AEGeometry*> *)0x0) {
              pvVar10 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)(this + 0x94));
              operator_delete(pvVar10);
            }
            *(undefined4 *)(this + 0x94) = 0;
            if (*(void **)(this + 0x98) != (void *)0x0) {
              operator_delete__(*(void **)(this + 0x98));
            }
            *(undefined4 *)(this + 0x98) = 0;
            if (*(void **)(this + 0x9c) != (void *)0x0) {
              operator_delete__(*(void **)(this + 0x9c));
            }
            *(undefined4 *)(this + 0x9c) = 0;
            ArrayRelease<bool>(*(Array **)(this + 0x100));
            if (*(Array<bool> **)(this + 0x100) != (Array<bool> *)0x0) {
              pvVar10 = (void *)Array<bool>::~Array(*(Array<bool> **)(this + 0x100));
              operator_delete(pvVar10);
            }
            *(undefined4 *)(this + 0x100) = 0;
            if (*(AEGeometry **)(this + 0xa4) != (AEGeometry *)0x0) {
              pvVar10 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0xa4));
              operator_delete(pvVar10);
            }
            *(undefined4 *)(this + 4) = 0;
            *(undefined4 *)(this + 0xa4) = 0;
            AEGeometry::setVisible
                      (SUB41(*(undefined4 *)
                              (*(int *)(*(int *)(this + 0x68) + 4) + *(int *)(this + 0x60) * 4),0));
          }
          else {
            *(undefined4 *)(this + 4) = 3;
          }
          *(undefined2 *)(this + 0x138) = 0;
        }
      }
    }
    goto LAB_000da6a8;
  }
  FModSound::setParamValue((FModSound *)**(undefined4 **)(DAT_000da2f4 + 0xda07a),0,0x66,in_s0);
  if (*(int *)(this + 4) == 3) {
    if (this[0x13b] == (StarMap)0x0) {
      if (this[0x174] == (StarMap)0x0) {
        fVar6 = *(float *)(this + 0x168);
        fVar5 = fVar6 * *(float *)(this + 0x16c);
        fVar4 = fVar6 * *(float *)(this + 0x170);
        fVar6 = -(fVar6 * *(float *)(this + 0x16c));
        if (0.0 < fVar5) {
          fVar6 = fVar5;
        }
        *(float *)(this + 0x16c) = fVar5;
        *(float *)(this + 0x170) = fVar4;
        if (0.5 < fVar6) {
          *(float *)(this + 0x188) = fVar5 + *(float *)(this + 0x188);
        }
        fVar6 = -fVar4;
        if (0.0 < fVar4) {
          fVar6 = fVar4;
        }
        uVar17 = in_fpscr & 0xfffffff | (uint)(fVar6 < 0.5) << 0x1f | (uint)(fVar6 == 0.5) << 0x1e;
        in_fpscr = uVar17 | (uint)NAN(fVar6) << 0x1c;
        bVar1 = (byte)(uVar17 >> 0x18);
        if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
          *(float *)(this + 0x18c) = fVar4 + *(float *)(this + 0x18c);
        }
      }
    }
    else {
      iVar24 = (int)*(float *)(this + 0x18c);
      iVar25 = (int)*(float *)(this + 0x188);
      iVar15 = 0x8000 - *(int *)(*(int *)(this + 0x98) + *(int *)(this + 100) * 4);
      iVar19 = DAT_000da304 - iVar24;
      iVar23 = iVar24 + 0xc18;
      iVar3 = iVar19;
      if (DAT_000da304 < iVar24) {
        iVar3 = iVar23;
      }
      iVar12 = iVar25 - iVar15;
      iVar13 = iVar12;
      if (iVar12 < 0) {
        iVar13 = -iVar12;
      }
      if (iVar25 < iVar15) {
        iVar12 = iVar15 - iVar25;
        fVar6 = 0.25;
      }
      else {
        fVar6 = -0.25;
      }
      fVar4 = (float)VectorSignedToFloat(iVar12,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x188) = *(float *)(this + 0x188) + fVar4 * fVar6;
      if (iVar13 < 0xb) {
        this[0x13b] = (StarMap)0x0;
      }
      if (DAT_000da79c < iVar24) {
        fVar6 = -0.25;
      }
      else {
        fVar6 = 0.25;
        iVar23 = iVar19;
      }
      fVar4 = (float)VectorSignedToFloat(iVar23,(byte)(in_fpscr >> 0x16) & 3);
      *(float *)(this + 0x18c) = *(float *)(this + 0x18c) + fVar4 * fVar6;
      if (iVar13 <= param_1 * 4) {
        if (iVar3 < 0) {
          iVar3 = -iVar3;
        }
        if (iVar3 <= param_1 * 4) {
          *(int *)(this + 0x1a0) = *(int *)(this + 100);
          this[0x13b] = (StarMap)0x0;
        }
      }
    }
    fVar4 = *(float *)(this + 0x16c) + *(float *)(this + 0x170);
    fVar6 = -fVar4;
    if (0.0 < fVar4) {
      fVar6 = fVar4;
    }
    fVar4 = 10.0;
    if (fVar6 * DAT_000da7a0 < 10.0) {
      fVar4 = fVar6 * DAT_000da7a0;
    }
    fVar4 = *(float *)(this + 0x1c0) + fVar4;
    uVar17 = in_fpscr & 0xfffffff | (uint)(fVar4 < DAT_000da7a4) << 0x1f |
             (uint)(fVar4 == DAT_000da7a4) << 0x1e;
    uVar21 = uVar17 | (uint)(NAN(fVar4) || NAN(DAT_000da7a4)) << 0x1c;
    bVar1 = (byte)(uVar17 >> 0x18);
    if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(uVar21 >> 0x1c) & 1)) {
      fVar4 = DAT_000da798;
    }
    puVar11 = *(uint **)(this + 0x90);
    *(float *)(this + 0x1c0) = fVar4;
    fVar6 = extraout_s1_01;
    if (puVar11 != (uint *)0x0) {
      uVar17 = 0;
      fVar4 = (float)VectorSignedToFloat(param_1,(byte)(uVar21 >> 0x16) & 3);
      fVar4 = fVar4 * DAT_000da7a8;
      fVar5 = DAT_000da7a8;
      for (; uVar17 < *puVar11; uVar17 = uVar17 + 1) {
        if (*(int *)(puVar11[1] + uVar17 * 4) != 0) {
          AEGeometry::rotate(fVar4,fVar6,fVar5);
          puVar11 = *(uint **)(this + 0x90);
          fVar4 = extraout_s0;
          fVar6 = extraout_s1_04;
          fVar5 = extraout_s2_00;
        }
      }
    }
    fVar5 = *(float *)(this + 0x18c);
    fVar4 = DAT_000da7ac;
    if (DAT_000da7ac < fVar5) {
      fVar4 = DAT_000da7b0;
    }
    if (DAT_000da7b0 < fVar5) {
      fVar4 = DAT_000da7b0;
    }
    uVar17 = uVar21 & 0xfffffff | (uint)(fVar5 < DAT_000da7b0) << 0x1f |
             (uint)(fVar5 == DAT_000da7b0) << 0x1e;
    fVar7 = fVar4;
    if (DAT_000da7ac < fVar5) {
      fVar7 = fVar5;
    }
    bVar1 = (byte)(uVar17 >> 0x18);
    fVar28 = (float)VectorSignedToFloat((int)*(float *)(this + 0x188) % 0x10000,
                                        (byte)(uVar17 >> 0x16) & 3);
    if (!(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == (NAN(fVar5) || NAN(DAT_000da7b0))) {
      fVar7 = fVar4;
    }
    *(float *)(this + 0x188) = fVar28;
    *(float *)(this + 0x18c) = fVar7;
    AEGeometry::setRotation(*(AEGeometry **)(this + 0xa4),fVar7,fVar6,fVar28);
    goto LAB_000da6a8;
  }
  if (*(int *)(this + 4) != 0) goto LAB_000da6a8;
  if (((-1 < *(int *)(this + 0x114)) && (this[0x118] != (StarMap)0x0)) &&
     (*(int *)(this + 0x11c) < 4000)) {
    iVar3 = *(int *)(this + 0x11c) + param_1;
    *(int *)(this + 0x11c) = iVar3;
    fVar6 = extraout_s1_01;
    if (3999 < iVar3) {
      OnTouchBegin((int)this,**(int **)(DAT_000da2fc + 0xda0be) >> 1);
      iVar3 = *(int *)(this + 0x11c);
      fVar6 = extraout_s1_02;
    }
    fVar4 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::setScaling
              ((float)((double)(fVar4 / DAT_000da300) * DAT_000d9d50),fVar6,DAT_000da300);
  }
  if (this[0x174] == (StarMap)0x0) {
    fVar6 = *(float *)(this + 0x168);
    fVar5 = fVar6 * *(float *)(this + 0x16c);
    fVar4 = fVar6 * *(float *)(this + 0x170);
    fVar6 = -(fVar6 * *(float *)(this + 0x16c));
    if (0.0 < fVar5) {
      fVar6 = fVar5;
    }
    *(float *)(this + 0x16c) = fVar5;
    uVar17 = in_fpscr & 0xfffffff | (uint)(fVar6 < 0.5) << 0x1f | (uint)(fVar6 == 0.5) << 0x1e;
    uVar21 = uVar17 | (uint)NAN(fVar6) << 0x1c;
    *(float *)(this + 0x170) = fVar4;
    bVar1 = (byte)(uVar17 >> 0x18);
    if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(uVar21 >> 0x1c) & 1)) {
      fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x13c),(byte)(uVar21 >> 0x16) & 3);
      *(int *)(this + 0x13c) = (int)(fVar5 + fVar6);
    }
    fVar6 = -fVar4;
    if (0.0 < fVar4) {
      fVar6 = fVar4;
    }
    uVar17 = in_fpscr & 0xfffffff | (uint)(fVar6 < 0.5) << 0x1f | (uint)(fVar6 == 0.5) << 0x1e;
    in_fpscr = uVar17 | (uint)NAN(fVar6) << 0x1c;
    bVar1 = (byte)(uVar17 >> 0x18);
    if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
      fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x140),(byte)(in_fpscr >> 0x16) & 3)
      ;
      *(int *)(this + 0x140) = (int)(fVar4 + fVar6);
    }
  }
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x140),(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x13c),(byte)(in_fpscr >> 0x16) & 3);
  iVar3 = (int)(*(float *)(this + 0xc) + fVar4);
  iVar23 = (int)(*(float *)(this + 8) + fVar6);
  if (iVar23 < 0x79) {
    iVar15 = DAT_000da30c;
    if (iVar23 <= DAT_000da308) goto LAB_000da2d4;
  }
  else {
    iVar15 = 0x78;
LAB_000da2d4:
    fVar6 = (float)VectorSignedToFloat(iVar15 - iVar23,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 0x16c) = fVar6 * DAT_000da310;
  }
  if (iVar3 < 0x8d) {
    if (iVar3 < -400) {
      iVar3 = (DAT_000da6c8 + 100) - iVar3;
      goto LAB_000da320;
    }
  }
  else {
    iVar3 = 0x8c - iVar3;
LAB_000da320:
    fVar6 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    *(float *)(this + 0x170) = fVar6 * DAT_000da310;
  }
  puVar11 = *(uint **)(DAT_000da6cc + 0xda33a);
  uVar17 = *puVar11;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  uVar22 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar17);
  __aeabi_memcpy(&local_80,uVar22,0x3c);
  fVar6 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x13c),(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x140),(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::AEMath::MatrixSetTranslation
            ((Matrix *)&local_ec,(*(float *)(this + 8) + fVar6) * 20.0,extraout_s1_03,
             (*(float *)(this + 0xc) + fVar4) * 20.0);
  uVar17 = *puVar11;
  pMVar8 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraSetLocal(uVar17,pMVar8);
  fVar6 = DAT_000da6d0;
  fVar5 = *(float *)(this + 0x16c) + *(float *)(this + 0x170);
  fVar4 = -fVar5;
  if (0.0 < fVar5) {
    fVar4 = fVar5;
  }
  fVar5 = 10.0;
  if (fVar4 < 10.0) {
    fVar5 = fVar4;
  }
  fVar5 = *(float *)(this + 0x1c0) + fVar5;
  uVar17 = in_fpscr & 0xfffffff | (uint)(fVar5 < DAT_000da7a4) << 0x1f |
           (uint)(fVar5 == DAT_000da7a4) << 0x1e;
  uVar21 = uVar17 | (uint)(NAN(fVar5) || NAN(DAT_000da7a4)) << 0x1c;
  bVar1 = (byte)(uVar17 >> 0x18);
  if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(uVar21 >> 0x1c) & 1)) {
    fVar5 = DAT_000da6d0;
  }
  *(float *)(this + 0x1c0) = fVar5;
  if (((this[0x13a] != (StarMap)0x0) &&
      (puVar11 = (uint *)Status::getSystemVisibilities
                                   ((Status *)**(undefined4 **)(DAT_000da7c0 + 0xda40a)),
      puVar11 != (uint *)0x0)) &&
     ((uVar17 = *(uint *)(this + 0x60), uVar17 < *puVar11 &&
      ((-1 < (int)uVar17 && (*(char *)(puVar11[1] + uVar17) != '\0')))))) {
    AbyssEngine::AEMath::Vector::operator=
              ((Vector *)(this + 0x78),
               *(Vector **)(*(int *)(*(int *)(this + 0x194) + 4) + uVar17 * 4));
    fVar5 = (float)VectorSignedToFloat(**(int **)(DAT_000da7c4 + 0xda44c) >> 1,
                                       (byte)(uVar21 >> 0x16) & 3);
    fVar7 = *(float *)(this + 0x7c);
    fVar26 = *(float *)(this + 0x78);
    fVar27 = (float)VectorSignedToFloat(**(int **)(DAT_000da7c8 + 0xda44e) >> 1,
                                        (byte)(uVar21 >> 0x16) & 3);
    fVar28 = (fVar26 - fVar27) / -30.0;
    fVar4 = fVar28;
    if ((fVar27 <= fVar26) && (fVar4 = fVar6, fVar26 != fVar27)) {
      fVar4 = fVar28;
    }
    fVar6 = (fVar7 - fVar5) / -30.0;
    bVar1 = (byte)(((uint)(fVar7 == fVar5) << 0x1e) >> 0x18);
    cVar20 = -((char)((byte)(((uint)(fVar7 < fVar5) << 0x1f) >> 0x18) | bVar1) >> 7);
    if (((bool)(bVar1 >> 6) || (bool)cVar20 != (NAN(fVar7) || NAN(fVar5))) && (cVar20 == '\0')) {
      fVar6 = DAT_000da798;
    }
    iVar23 = (int)fVar4;
    iVar3 = (int)(fVar26 - fVar27);
    if (iVar3 < 0) {
      iVar3 = -iVar3;
    }
    if (iVar23 < 0) {
      iVar23 = -iVar23;
    }
    if (iVar3 < iVar23) {
      fVar4 = fVar4 * 0.5;
    }
    iVar23 = (int)fVar6;
    iVar3 = (int)(fVar7 - fVar5);
    if (iVar3 < 0) {
      iVar3 = -iVar3;
    }
    if (iVar23 < 0) {
      iVar23 = -iVar23;
    }
    if (iVar3 < iVar23) {
      fVar6 = fVar6 * 0.5;
    }
    fVar5 = -fVar4;
    if (0.0 < fVar4) {
      fVar5 = fVar4;
    }
    *(float *)(this + 0x16c) = fVar4;
    *(float *)(this + 0x170) = fVar6;
    if (fVar5 <= 2.0) {
      fVar4 = -fVar6;
      if (0.0 < fVar6) {
        fVar4 = fVar6;
      }
      if (fVar4 <= 2.0) {
        this[0x13a] = (StarMap)0x0;
        *(undefined4 *)(this + 0x19c) = *(undefined4 *)(this + 0x60);
      }
    }
  }
LAB_000da6a8:
  if (*piVar2 == local_44) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000d9b90: push {r4,r5,r6,r7,lr}
  000d9b92: add r7,sp,#0xc
  000d9b94: push {r7,r8,r9,r10,r11}
  000d9b98: vpush {d8,d9,d10}
  000d9b9c: sub sp,#0xb8
  000d9b9e: mov r9,r0
  000d9ba0: ldr r0,[0x000d9d30]
  000d9ba2: add r0,pc
  000d9ba4: ldr r0,[r0,#0x0]
  000d9ba6: str r0,[sp,#0x8]
  000d9ba8: ldr r0,[r0,#0x0]
  000d9baa: str r0,[sp,#0xb4]
  000d9bac: ldr.w r0,[r9,#0x4]
  000d9bb0: str r1,[sp,#0x4]
  000d9bb2: cmp r0,#0x0
  000d9bb4: str.w r1,[r9,#0x18]
  000d9bb8: beq 0x000d9bc6
  000d9bba: ldrb.w r1,[r9,#0x138]
  000d9bbe: cbnz r1,0x000d9bc6
  000d9bc0: ldrb.w r1,[r9,#0x139]
  000d9bc4: cbz r1,0x000d9c2c
  000d9bc6: ldr.w r1,[r9,#0x194]
  000d9bca: vmov.f32 s16,0xbf800000
  000d9bce: ldr r0,[0x000d9d34]
  000d9bd0: vmov.f32 s18,0x3f800000
  000d9bd4: add r4,sp,#0x78
  000d9bd6: movs r6,#0x0
  000d9bd8: add r0,pc
  000d9bda: ldr.w r8,[r0,#0x0]
  000d9bde: b 0x000d9c22
  000d9be0: ldr.w r0,[r9,#0x68]
  000d9be4: ldr.w r5,[r8,#0x0]
  000d9be8: ldr r0,[r0,#0x4]
  000d9bea: ldr.w r1,[r0,r6,lsl #0x2]
  000d9bee: mov r0,r4
  000d9bf0: blx 0x000720b8
  000d9bf4: ldr.w r0,[r9,#0x194]
  000d9bf8: mov r1,r4
  000d9bfa: ldr r0,[r0,#0x4]
  000d9bfc: ldr.w r2,[r0,r6,lsl #0x2]
  000d9c00: mov r0,r5
  000d9c02: blx 0x000729c4
  000d9c06: ldr.w r1,[r9,#0x194]
  000d9c0a: vmov.f32 s0,s16
  000d9c0e: cmp r0,#0x0
  000d9c10: it ne
  000d9c12: vmov.ne.f32 s0,s18
  000d9c16: ldr r0,[r1,#0x4]
  000d9c18: ldr.w r0,[r0,r6,lsl #0x2]
  000d9c1c: adds r6,#0x1
  000d9c1e: vstr.32 s0,[r0,#0x8]
  000d9c22: ldr r0,[r1,#0x0]
  000d9c24: cmp r6,r0
  000d9c26: bcc 0x000d9be0
  000d9c28: ldr.w r0,[r9,#0x4]
  000d9c2c: cmp r0,#0x3
  000d9c2e: beq 0x000d9c3e
  000d9c30: ldrb.w r0,[r9,#0x138]
  000d9c34: cbnz r0,0x000d9c3e
  000d9c36: ldrb.w r0,[r9,#0x139]
  000d9c3a: cmp r0,#0x0
  000d9c3c: beq 0x000d9cce
  000d9c3e: ldr.w r1,[r9,#0x198]
  000d9c42: vmov.f32 s16,0xbf800000
  000d9c46: ldr r0,[0x000d9d38]
  000d9c48: vmov.f32 s18,0x3f800000
  000d9c4c: add.w r6,r9,#0x84
  000d9c50: add.w r10,sp,#0x78
  000d9c54: add r0,pc
  000d9c56: add r4,sp,#0xc
  000d9c58: mov.w r11,#0x0
  000d9c5c: ldr.w r8,[r0,#0x0]
  000d9c60: b 0x000d9cc8
  000d9c62: ldr.w r0,[r9,#0xa4]
  000d9c66: blx 0x000721cc
  000d9c6a: mov r5,r0
  000d9c6c: ldr.w r0,[r9,#0x90]
  000d9c70: ldr r0,[r0,#0x4]
  000d9c72: add.w r0,r0,r11, lsl #0x2
  000d9c76: ldr r0,[r0,#0x4]
  000d9c78: blx 0x000721cc
  000d9c7c: mov r2,r0
  000d9c7e: mov r0,r10
  000d9c80: mov r1,r5
  000d9c82: blx 0x0006f82c
  000d9c86: mov r0,r4
  000d9c88: mov r1,r10
  000d9c8a: blx 0x0006f16c
  000d9c8e: mov r0,r6
  000d9c90: mov r1,r4
  000d9c92: blx 0x0006eb3c
  000d9c96: ldr.w r0,[r9,#0x198]
  000d9c9a: ldr r1,[r0,#0x4]
  000d9c9c: ldr.w r0,[r8,#0x0]
  000d9ca0: ldr.w r2,[r1,r11,lsl #0x2]
  000d9ca4: mov r1,r6
  000d9ca6: blx 0x000729c4
  000d9caa: ldr.w r1,[r9,#0x198]
  000d9cae: vmov.f32 s0,s16
  000d9cb2: cmp r0,#0x0
  000d9cb4: it ne
  000d9cb6: vmov.ne.f32 s0,s18
  000d9cba: ldr r0,[r1,#0x4]
  000d9cbc: ldr.w r0,[r0,r11,lsl #0x2]
  000d9cc0: add.w r11,r11,#0x1
  000d9cc4: vstr.32 s0,[r0,#0x8]
  000d9cc8: ldr r0,[r1,#0x0]
  000d9cca: cmp r11,r0
  000d9ccc: bcc 0x000d9c62
  000d9cce: ldr.w r0,[r9,#0xf8]
  000d9cd2: ldr.w r8,[sp,#0x4]
  000d9cd6: cmp r0,#0x0
  000d9cd8: beq.w 0x000d9eb6
  000d9cdc: ldr.w r0,[r9,#0x184]
  000d9ce0: blx 0x0007468c
  000d9ce4: ldr.w r1,[r9,#0x184]
  000d9ce8: vmov s16,r0
  000d9cec: mov r0,r1
  000d9cee: blx 0x00074698
  000d9cf2: ldr.w r1,[r9,#0x184]
  000d9cf6: vmov s18,r0
  000d9cfa: mov r0,r1
  000d9cfc: blx 0x000746a4
  000d9d00: ldr.w r1,[r9,#0x184]
  000d9d04: vmov s20,r0
  000d9d08: mov r0,r1
  000d9d0a: blx 0x00074698
  000d9d0e: vmov s0,r0
  000d9d12: ldrb.w r0,[r9,#0x139]
  000d9d16: vsub.f32 s2,s16,s18
  000d9d1a: vsub.f32 s0,s20,s0
  000d9d1e: cmp r0,#0x0
  000d9d20: vdiv.f32 s16,s2,s0
  000d9d24: beq 0x000d9d58
  000d9d26: vmov.f32 s0,0x3f800000
  000d9d2a: vsub.f32 s16,s0,s16
  000d9d2e: b 0x000d9d6e
  000d9d58: ldrb.w r0,[r9,#0x138]
  000d9d5c: cbnz r0,0x000d9d6e
  000d9d5e: ldr.w r0,[r9,#0x4]
  000d9d62: vldr.32 s0,[pc,#-0x28]
  000d9d66: cmp r0,#0x0
  000d9d68: it eq
  000d9d6a: vmov.eq.f32 s16,s0
  000d9d6e: vcvt.f64.f32 d16,s16
  000d9d72: ldr.w r0,[r9,#0xf8]
  000d9d76: vldr.64 d17,[pc,#-0x38]
  000d9d7a: vldr.64 d18,[pc,#-0x34]
  000d9d7e: vmla.f64 d18,d16,d17
  000d9d82: vcvt.f32.f64 s0,d18
  000d9d86: vmov r1,s0
  000d9d8a: mov r2,r1
  000d9d8c: mov r3,r1
  000d9d8e: blx 0x000727b4
  000d9d92: ldr.w r0,[r9,#0xa4]
  000d9d96: cmp r0,#0x0
  000d9d98: beq 0x000d9e24
  000d9d9a: ldr.w r0,[r9,#0x1c4]
  000d9d9e: cmp r0,#0x0
  000d9da0: blt 0x000d9e24
  000d9da2: ldr r0,[0x000da058]
  000d9da4: add.w r10,sp,#0xc
  000d9da8: ldr.w r1,[r9,#0x68]
  000d9dac: add r0,pc
  000d9dae: ldr r0,[r0,#0x0]
  000d9db0: ldr r1,[r1,#0x4]
  000d9db2: ldr r0,[r0,#0x0]
  000d9db4: ldr r0,[r0,#0x7c]
  000d9db6: ldr.w r1,[r1,r0,lsl #0x2]
  000d9dba: mov r0,r10
  000d9dbc: blx 0x000720b8
  000d9dc0: ldr.w r0,[r9,#0xa4]
  000d9dc4: blx 0x000721cc
  000d9dc8: mov r5,r0
  000d9dca: ldr.w r0,[r9,#0x90]
  000d9dce: ldr.w r1,[r9,#0x1c4]
  000d9dd2: ldr r0,[r0,#0x4]
  000d9dd4: ldr.w r0,[r0,r1,lsl #0x2]
  000d9dd8: blx 0x000721cc
  000d9ddc: add r6,sp,#0x78
  000d9dde: mov r2,r0
  000d9de0: mov r1,r5
  000d9de2: mov r0,r6
  000d9de4: blx 0x0006f82c
  000d9de8: add r5,sp,#0x6c
  000d9dea: mov r1,r6
  000d9dec: mov r0,r5
  000d9dee: blx 0x0006f16c
  000d9df2: add r4,sp,#0x48
  000d9df4: mov r1,r5
  000d9df6: mov r2,r10
  000d9df8: ldr.w r6,[r9,#0xf8]
  000d9dfc: mov r0,r4
  000d9dfe: blx 0x0006ec38
  000d9e02: vmov r1,s16
  000d9e06: add r5,sp,#0x54
  000d9e08: mov r2,r4
  000d9e0a: mov r0,r5
  000d9e0c: blx 0x00072658
  000d9e10: add r4,sp,#0x60
  000d9e12: mov r1,r10
  000d9e14: mov r2,r5
  000d9e16: mov r0,r4
  000d9e18: blx 0x0006f1cc
  000d9e1c: mov r0,r6
  000d9e1e: mov r1,r4
  000d9e20: blx 0x00072148
  000d9e24: ldr r0,[0x000da05c]
  000d9e26: add r0,pc
  000d9e28: ldr r6,[r0,#0x0]
  000d9e2a: ldr r4,[r6,#0x0]
  000d9e2c: mov r0,r4
  000d9e2e: blx 0x000717f4
  000d9e32: mov r1,r0
  000d9e34: mov r0,r4
  000d9e36: blx 0x0006ff1c
  000d9e3a: add r5,sp,#0x78
  000d9e3c: mov r1,r0
  000d9e3e: mov r0,r5
  000d9e40: blx 0x0006f16c
  000d9e44: add.w r4,r9,#0x78
  000d9e48: mov r1,r5
  000d9e4a: mov r0,r4
  000d9e4c: blx 0x0006eb3c
  000d9e50: add r5,sp,#0x78
  000d9e52: ldr.w r1,[r9,#0xf8]
  000d9e56: mov r0,r5
  000d9e58: blx 0x000720b8
  000d9e5c: mov r0,r4
  000d9e5e: mov r1,r5
  000d9e60: blx 0x00072694
  000d9e64: add r5,sp,#0xc
  000d9e66: mov r1,r4
  000d9e68: mov r0,r5
  000d9e6a: blx 0x0006ec80
  000d9e6e: mov r0,r4
  000d9e70: mov r1,r5
  000d9e72: blx 0x0006eb3c
  000d9e76: vmov.f32 s0,0x3f000000
  000d9e7a: vldr.32 s2,[r9,#0x78]
  000d9e7e: ldr.w r0,[r9,#0xf8]
  000d9e82: mov.w r1,#0x3f800000
  000d9e86: add r2,sp,#0xc
  000d9e88: movs r5,#0x0
  000d9e8a: vadd.f32 s0,s2,s0
  000d9e8e: vstr.32 s0,[r9,#0x78]
  000d9e92: str r1,[sp,#0x10]
  000d9e94: mov r1,r4
  000d9e96: str r5,[sp,#0xc]
  000d9e98: str r5,[sp,#0x14]
  000d9e9a: blx 0x000726ac
  000d9e9e: ldr.w r1,[r9,#0xf8]
  000d9ea2: ldr r0,[r6,#0x0]
  000d9ea4: ldr r1,[r1,#0xc]
  000d9ea6: blx 0x00072088
  000d9eaa: asr.w r3,r8, asr #0x1f
  000d9eae: mov r2,r8
  000d9eb0: str r5,[sp,#0x0]
  000d9eb2: blx 0x0006f7cc
  000d9eb6: ldrb.w r0,[r9,#0x138]
  000d9eba: cbnz r0,0x000d9ec6
  000d9ebc: ldrb.w r0,[r9,#0x139]
  000d9ec0: cmp r0,#0x0
  000d9ec2: beq.w 0x000da06c
  000d9ec6: rsb r0,r8,r8, lsl #0x4
  000d9eca: vmov s0,r0
  000d9ece: vcvt.f32.s32 s0,s0
  000d9ed2: ldr r1,[0x000da060]
  000d9ed4: ldr.w r0,[r9,#0x17c]
  000d9ed8: add r1,pc
  000d9eda: ldr r5,[r1,#0x0]
  000d9edc: vmov r4,s0
  000d9ee0: mov r1,r4
  000d9ee2: blx r5
  000d9ee4: ldr.w r0,[r9,#0x180]
  000d9ee8: mov r1,r4
  000d9eea: blx r5
  000d9eec: ldr.w r0,[r9,#0x184]
  000d9ef0: mov r1,r4
  000d9ef2: blx r5
  000d9ef4: ldr r1,[0x000da064]
  000d9ef6: ldr.w r0,[r9,#0x17c]
  000d9efa: add r1,pc
  000d9efc: ldr r6,[r1,#0x0]
  000d9efe: blx r6
  000d9f00: mov r4,r0
  000d9f02: ldr.w r0,[r9,#0x180]
  000d9f06: blx r6
  000d9f08: mov r5,r0
  000d9f0a: ldr.w r0,[r9,#0x184]
  000d9f0e: blx r6
  000d9f10: strd r4,r5,[sp,#0x78]
  000d9f14: add r1,sp,#0x78
  000d9f16: str r0,[sp,#0x80]
  000d9f18: add.w r0,r9,#0x78
  000d9f1c: blx 0x0006eb3c
  000d9f20: ldr r0,[0x000da068]
  000d9f22: add r0,pc
  000d9f24: ldr r5,[r0,#0x0]
  000d9f26: ldr r4,[r5,#0x0]
  000d9f28: mov r0,r4
  000d9f2a: blx 0x000717f4
  000d9f2e: mov r1,r0
  000d9f30: mov r0,r4
  000d9f32: blx 0x0006ff1c
  000d9f36: add r4,sp,#0x78
  000d9f38: mov r1,r0
  000d9f3a: movs r2,#0x3c
  000d9f3c: mov r0,r4
  000d9f3e: blx 0x0006f1e4
  000d9f42: ldrd r2,r3,[r9,#0x78]
  000d9f46: add r0,sp,#0xc
  000d9f48: vldr.32 s0,[r9,#0x80]
  000d9f4c: mov r1,r4
  000d9f4e: vstr.32 s0,[sp]
  000d9f52: blx 0x0006f820
  000d9f56: ldr r5,[r5,#0x0]
  000d9f58: mov r0,r5
  000d9f5a: blx 0x000717f4
  000d9f5e: mov r1,r0
  000d9f60: mov r0,r5
  000d9f62: mov r2,r4
  000d9f64: blx 0x000720a0
  000d9f68: ldr.w r0,[r9,#0x17c]
  000d9f6c: vldr.32 s16,[r9,#0x78]
  000d9f70: blx 0x000746a4
  000d9f74: mov r4,r0
  000d9f76: ldr.w r0,[r9,#0x17c]
  000d9f7a: vldr.32 s18,[r9,#0x78]
  000d9f7e: blx 0x000746a4
  000d9f82: vmov s2,r4
  000d9f86: vmov s0,r0
  000d9f8a: vsub.f32 s2,s16,s2
  000d9f8e: vsub.f32 s0,s18,s0
  000d9f92: vmov.f32 s16,0x3f800000
  000d9f96: vcmpe.f32 s2,#0
  000d9f9a: vmrs apsr,fpscr
  000d9f9e: vneg.f32 s4,s0
  000d9fa2: it gt
  000d9fa4: vmov.gt.f32 s4,s0
  000d9fa8: vcmpe.f32 s4,s16
  000d9fac: vmrs apsr,fpscr
  000d9fb0: bhi.w 0x000da6a8
  000d9fb4: ldr.w r0,[r9,#0x180]
  000d9fb8: vldr.32 s18,[r9,#0x7c]
  000d9fbc: blx 0x000746a4
  000d9fc0: mov r4,r0
  000d9fc2: ldr.w r0,[r9,#0x180]
  000d9fc6: vldr.32 s20,[r9,#0x7c]
  000d9fca: blx 0x000746a4
  000d9fce: vmov s2,r4
  000d9fd2: vmov s0,r0
  000d9fd6: vsub.f32 s2,s18,s2
  000d9fda: vsub.f32 s0,s20,s0
  000d9fde: vcmpe.f32 s2,#0
  000d9fe2: vmrs apsr,fpscr
  000d9fe6: vneg.f32 s4,s0
  000d9fea: it gt
  000d9fec: vmov.gt.f32 s4,s0
  000d9ff0: vcmpe.f32 s4,s16
  000d9ff4: vmrs apsr,fpscr
  000d9ff8: bhi.w 0x000da6a8
  000d9ffc: ldr.w r0,[r9,#0x184]
  000da000: vldr.32 s18,[r9,#0x80]
  000da004: blx 0x000746a4
  000da008: mov r4,r0
  000da00a: ldr.w r0,[r9,#0x184]
  000da00e: vldr.32 s20,[r9,#0x80]
  000da012: blx 0x000746a4
  000da016: vmov s2,r4
  000da01a: vmov s0,r0
  000da01e: vsub.f32 s2,s18,s2
  000da022: vsub.f32 s0,s20,s0
  000da026: vcmpe.f32 s2,#0
  000da02a: vmrs apsr,fpscr
  000da02e: vneg.f32 s4,s0
  000da032: it gt
  000da034: vmov.gt.f32 s4,s0
  000da038: vcmpe.f32 s4,s16
  000da03c: vmrs apsr,fpscr
  000da040: bhi.w 0x000da6a8
  000da044: ldrb.w r0,[r9,#0x138]
  000da048: cmp r0,#0x0
  000da04a: beq.w 0x000da22a
  000da04e: movs r0,#0x3
  000da050: str.w r0,[r9,#0x4]
  000da054: b 0x000da2c4
  000da06c: ldr r0,[0x000da2f4]
  000da06e: movs r1,#0x0
  000da070: ldr.w r3,[r9,#0x1c0]
  000da074: movs r2,#0x66
  000da076: add r0,pc
  000da078: ldr r0,[r0,#0x0]
  000da07a: ldr r0,[r0,#0x0]
  000da07c: blx 0x0007159c
  000da080: ldr.w r0,[r9,#0x4]
  000da084: cmp r0,#0x3
  000da086: beq.w 0x000da1ce
  000da08a: cmp r0,#0x0
  000da08c: bne.w 0x000da6a8
  000da090: ldr.w r0,[r9,#0x114]
  000da094: cmp r0,#0x0
  000da096: blt 0x000da10c
  000da098: ldrb.w r1,[r9,#0x118]
  000da09c: cbz r1,0x000da10c
  000da09e: ldr.w r1,[r9,#0x11c]
  000da0a2: cmp.w r1,#0xfa0
  000da0a6: bge 0x000da10c
  000da0a8: add r1,r8
  000da0aa: str.w r1,[r9,#0x11c]
  000da0ae: cmp.w r1,#0xfa0
  000da0b2: blt 0x000da0d6
  000da0b4: ldr r0,[0x000da2f8]
  000da0b6: ldr r1,[0x000da2fc]
  000da0b8: add r0,pc
  000da0ba: add r1,pc
  000da0bc: ldr r0,[r0,#0x0]
  000da0be: ldr r1,[r1,#0x0]
  000da0c0: ldr r0,[r0,#0x0]
  000da0c2: ldr r1,[r1,#0x0]
  000da0c4: asrs r2,r0,#0x1
  000da0c6: mov r0,r9
  000da0c8: asrs r1,r1,#0x1
  000da0ca: blx 0x00074704
  000da0ce: ldr.w r0,[r9,#0x114]
  000da0d2: ldr.w r1,[r9,#0x11c]
  000da0d6: vmov s0,r1
  000da0da: vldr.32 s2,[pc,#0x224]
  000da0de: vldr.64 d17,[pc,#-0x390]
  000da0e2: vcvt.f32.s32 s0,s0
  000da0e6: ldr.w r2,[r9,#0x68]
  000da0ea: ldr r2,[r2,#0x4]
  000da0ec: ldr.w r0,[r2,r0,lsl #0x2]
  000da0f0: vdiv.f32 s0,s0,s2
  000da0f4: vcvt.f64.f32 d16,s0
  000da0f8: vmul.f64 d16,d16,d17
  000da0fc: vcvt.f32.f64 s0,d16
  000da100: vmov r1,s0
  000da104: mov r2,r1
  000da106: mov r3,r1
  000da108: blx 0x000727b4
  000da10c: ldrb.w r0,[r9,#0x174]
  000da110: cmp r0,#0x0
  000da112: bne 0x000da194
  000da114: vldr.32 s0,[r9,#0x168]
  000da118: vldr.32 s2,[r9,#0x16c]
  000da11c: vldr.32 s6,[r9,#0x170]
  000da120: vmul.f32 s4,s0,s2
  000da124: vnmul.f32 s8,s0,s2
  000da128: vmov.f32 s2,0x3f000000
  000da12c: vmul.f32 s0,s0,s6
  000da130: vcmpe.f32 s4,#0
  000da134: vmrs apsr,fpscr
  000da138: it gt
  000da13a: vmov.gt.f32 s8,s4
  000da13e: vstr.32 s4,[r9,#0x16c]
  000da142: vcmpe.f32 s8,s2
  000da146: vstr.32 s0,[r9,#0x170]
  000da14a: vmrs apsr,fpscr
  000da14e: ble 0x000da164
  000da150: vldr.32 s6,[r9,#0x13c]
  000da154: vcvt.f32.s32 s6,s6
  000da158: vadd.f32 s4,s4,s6
  000da15c: vcvt.s32.f32 s4,s4
  000da160: vstr.32 s4,[r9,#0x13c]
  000da164: vcmpe.f32 s0,#0
  000da168: vmrs apsr,fpscr
  000da16c: vneg.f32 s4,s0
  000da170: it gt
  000da172: vmov.gt.f32 s4,s0
  000da176: vcmpe.f32 s4,s2
  000da17a: vmrs apsr,fpscr
  000da17e: ble 0x000da194
  000da180: vldr.32 s2,[r9,#0x140]
  000da184: vcvt.f32.s32 s2,s2
  000da188: vadd.f32 s0,s0,s2
  000da18c: vcvt.s32.f32 s0,s0
  000da190: vstr.32 s0,[r9,#0x140]
  000da194: vldr.32 s4,[r9,#0x13c]
  000da198: vldr.32 s6,[r9,#0x140]
  000da19c: vldr.32 s0,[r9,#0x8]
  000da1a0: vldr.32 s2,[r9,#0xc]
  000da1a4: vcvt.f32.s32 s6,s6
  000da1a8: vcvt.f32.s32 s4,s4
  000da1ac: vadd.f32 s2,s2,s6
  000da1b0: vadd.f32 s0,s0,s4
  000da1b4: vcvt.s32.f32 s2,s2
  000da1b8: vcvt.s32.f32 s0,s0
  000da1bc: vmov r0,s2
  000da1c0: vmov r1,s0
  000da1c4: cmp r1,#0x78
  000da1c6: ble.w 0x000da2cc
  000da1ca: movs r2,#0x78
  000da1cc: b 0x000da2d4
  000da1ce: ldrb.w r0,[r9,#0x13b]
  000da1d2: cmp r0,#0x0
  000da1d4: beq.w 0x000da492
  000da1d8: ldr.w r12,[r9,#0x64]
  000da1dc: ldr.w r0,[r9,#0x98]
  000da1e0: vldr.32 s0,[r9,#0x18c]
  000da1e4: vldr.32 s2,[r9,#0x188]
  000da1e8: ldr.w r0,[r0,r12,lsl #0x2]
  000da1ec: vcvt.s32.f32 s4,s0
  000da1f0: vcvt.s32.f32 s6,s2
  000da1f4: ldr r1,[0x000da304]
  000da1f6: rsb.w r4,r0,#0x8000
  000da1fa: vmov r5,s4
  000da1fe: vmov r0,s6
  000da202: subs.w lr,r1,r5
  000da206: addw r6,r5,#0xc18
  000da20a: mov r1,lr
  000da20c: it lt
  000da20e: mov.lt r1,r6
  000da210: subs r2,r0,r4
  000da212: mov r3,r2
  000da214: it mi
  000da216: rsb.mi r3,r2
  000da218: cmp r4,r0
  000da21a: ble.w 0x000da50c
  000da21e: subs r0,r4,r0
  000da220: vmov.f32 s4,0x3e800000
  000da224: vmov s6,r0
  000da228: b 0x000da514
  000da22a: ldr.w r0,[r9,#0x90]
  000da22e: blx 0x00071fec
  000da232: ldr.w r0,[r9,#0x90]
  000da236: cbz r0,0x000da240
  000da238: blx 0x000717c4
  000da23c: blx 0x0006eb48
  000da240: ldr.w r0,[r9,#0x94]
  000da244: movs r4,#0x0
  000da246: str.w r4,[r9,#0x90]
  000da24a: blx 0x00071fec
  000da24e: ldr.w r0,[r9,#0x94]
  000da252: cbz r0,0x000da25c
  000da254: blx 0x000717c4
  000da258: blx 0x0006eb48
  000da25c: ldr.w r0,[r9,#0x98]
  000da260: str.w r4,[r9,#0x94]
  000da264: cbz r0,0x000da26a
  000da266: blx 0x0006ebfc
  000da26a: ldr.w r0,[r9,#0x9c]
  000da26e: movs r4,#0x0
  000da270: str.w r4,[r9,#0x98]
  000da274: cbz r0,0x000da27a
  000da276: blx 0x0006ebfc
  000da27a: ldr.w r0,[r9,#0x100]
  000da27e: str.w r4,[r9,#0x9c]
  000da282: blx 0x00074710
  000da286: ldr.w r0,[r9,#0x100]
  000da28a: cbz r0,0x000da294
  000da28c: blx 0x00073a5c
  000da290: blx 0x0006eb48
  000da294: ldr.w r0,[r9,#0xa4]
  000da298: movs r4,#0x0
  000da29a: str.w r4,[r9,#0x100]
  000da29e: cbz r0,0x000da2a8
  000da2a0: blx 0x00071fc8
  000da2a4: blx 0x0006eb48
  000da2a8: ldr.w r1,[r9,#0x68]
  000da2ac: ldr.w r0,[r9,#0x60]
  000da2b0: str.w r4,[r9,#0x4]
  000da2b4: str.w r4,[r9,#0xa4]
  000da2b8: ldr r1,[r1,#0x4]
  000da2ba: ldr.w r0,[r1,r0,lsl #0x2]
  000da2be: movs r1,#0x1
  000da2c0: blx 0x00072d24
  000da2c4: movs r0,#0x0
  000da2c6: strh.w r0,[r9,#0x138]
  000da2ca: b 0x000da6a8
  000da2cc: ldr r2,[0x000da308]
  000da2ce: cmp r1,r2
  000da2d0: bgt 0x000da2ea
  000da2d2: ldr r2,[0x000da30c]
  000da2d4: subs r1,r2,r1
  000da2d6: vldr.32 s2,[pc,#0x38]
  000da2da: vmov s0,r1
  000da2de: vcvt.f32.s32 s0,s0
  000da2e2: vmul.f32 s0,s0,s2
  000da2e6: vstr.32 s0,[r9,#0x16c]
  000da2ea: cmp r0,#0x8d
  000da2ec: blt 0x000da314
  000da2ee: rsb.w r0,r0,#0x8c
  000da2f2: b 0x000da320
  000da314: cmn r0,#0x190
  000da318: bge 0x000da334
  000da31a: ldr r1,[0x000da6c8]
  000da31c: adds r1,#0x64
  000da31e: subs r0,r1,r0
  000da320: vmov s0,r0
  000da324: vldr.32 s2,[pc,#-0x18]
  000da328: vcvt.f32.s32 s0,s0
  000da32c: vmul.f32 s0,s0,s2
  000da330: vstr.32 s0,[r9,#0x170]
  000da334: ldr r0,[0x000da6cc]
  000da336: add r0,pc
  000da338: ldr r5,[r0,#0x0]
  000da33a: ldr r4,[r5,#0x0]
  000da33c: mov r0,r4
  000da33e: blx 0x000717f4
  000da342: mov r1,r0
  000da344: mov r0,r4
  000da346: blx 0x0006ff1c
  000da34a: add r4,sp,#0x78
  000da34c: mov r1,r0
  000da34e: movs r2,#0x3c
  000da350: mov r0,r4
  000da352: blx 0x0006f1e4
  000da356: vldr.32 s4,[r9,#0x13c]
  000da35a: vmov.f32 s8,0x41a00000
  000da35e: vldr.32 s6,[r9,#0x140]
  000da362: movs r0,#0x0
  000da364: vldr.32 s0,[r9,#0x8]
  000da368: mov r1,r4
  000da36a: vldr.32 s2,[r9,#0xc]
  000da36e: vcvt.f32.s32 s4,s4
  000da372: vcvt.f32.s32 s6,s6
  000da376: str r0,[sp,#0x0]
  000da378: add r0,sp,#0xc
  000da37a: vadd.f32 s0,s0,s4
  000da37e: vadd.f32 s2,s2,s6
  000da382: vmul.f32 s0,s0,s8
  000da386: vmul.f32 s2,s2,s8
  000da38a: vmov r2,s0
  000da38e: vmov r3,s2
  000da392: blx 0x0006f820
  000da396: ldr r5,[r5,#0x0]
  000da398: mov r0,r5
  000da39a: blx 0x000717f4
  000da39e: mov r1,r0
  000da3a0: mov r0,r5
  000da3a2: mov r2,r4
  000da3a4: blx 0x000720a0
  000da3a8: vldr.32 s0,[r9,#0x16c]
  000da3ac: vmov.f32 s6,0x41200000
  000da3b0: vldr.32 s2,[r9,#0x170]
  000da3b4: vldr.32 s4,[r9,#0x1c0]
  000da3b8: vadd.f32 s0,s0,s2
  000da3bc: vldr.32 s16,[pc,#0x310]
  000da3c0: vcmpe.f32 s0,#0
  000da3c4: vmrs apsr,fpscr
  000da3c8: vneg.f32 s2,s0
  000da3cc: it gt
  000da3ce: vmov.gt.f32 s2,s0
  000da3d2: vcmpe.f32 s2,s6
  000da3d6: vmrs apsr,fpscr
  000da3da: it mi
  000da3dc: vmov.mi.f32 s6,s2
  000da3e0: vldr.32 s2,[pc,#0x3c0]
  000da3e4: vadd.f32 s0,s4,s6
  000da3e8: vcmpe.f32 s0,s2
  000da3ec: vmrs apsr,fpscr
  000da3f0: it gt
  000da3f2: vmov.gt.f32 s0,s16
  000da3f6: ldrb.w r0,[r9,#0x13a]
  000da3fa: vstr.32 s0,[r9,#0x1c0]
  000da3fe: cmp r0,#0x0
  000da400: beq.w 0x000da6a8
  000da404: ldr r0,[0x000da7c0]
  000da406: add r0,pc
  000da408: ldr r0,[r0,#0x0]
  000da40a: ldr r0,[r0,#0x0]
  000da40c: blx 0x00071aa0
  000da410: cmp r0,#0x0
  000da412: beq.w 0x000da6a8
  000da416: ldr.w r1,[r9,#0x60]
  000da41a: ldr r2,[r0,#0x0]
  000da41c: cmp r2,r1
  000da41e: bls.w 0x000da6a8
  000da422: cmp r1,#0x0
  000da424: blt.w 0x000da6a8
  000da428: ldr r0,[r0,#0x4]
  000da42a: ldrb r0,[r0,r1]
  000da42c: cmp r0,#0x0
  000da42e: beq.w 0x000da6a8
  000da432: ldr.w r0,[r9,#0x194]
  000da436: ldr r0,[r0,#0x4]
  000da438: ldr.w r1,[r0,r1,lsl #0x2]
  000da43c: add.w r0,r9,#0x78
  000da440: blx 0x0006eb3c
  000da444: ldr r0,[0x000da7c4]
  000da446: ldr r1,[0x000da7c8]
  000da448: add r0,pc
  000da44a: add r1,pc
  000da44c: ldr r0,[r0,#0x0]
  000da44e: ldr r1,[r1,#0x0]
  000da450: ldr r0,[r0,#0x0]
  000da452: asrs r0,r0,#0x1
  000da454: vmov s0,r0
  000da458: vcvt.f32.s32 s4,s0
  000da45c: ldr r0,[r1,#0x0]
  000da45e: vldr.32 s8,[r9,#0x7c]
  000da462: vldr.32 s12,[r9,#0x78]
  000da466: asrs r0,r0,#0x1
  000da468: vmov s0,r0
  000da46c: vcvt.f32.s32 s14,s0
  000da470: vmov.f32 s0,0xc1f00000
  000da474: vsub.f32 s2,s8,s4
  000da478: vsub.f32 s6,s12,s14
  000da47c: vcmpe.f32 s12,s14
  000da480: vmrs apsr,fpscr
  000da484: vdiv.f32 s10,s6,s0
  000da488: bpl.w 0x000da6d4
  000da48c: vmov.f32 s16,s10
  000da490: b 0x000da6da
  000da492: ldrb.w r0,[r9,#0x174]
  000da496: cmp r0,#0x0
  000da498: bne 0x000da56c
  000da49a: vldr.32 s0,[r9,#0x168]
  000da49e: vldr.32 s2,[r9,#0x16c]
  000da4a2: vldr.32 s6,[r9,#0x170]
  000da4a6: vmul.f32 s4,s0,s2
  000da4aa: vnmul.f32 s8,s0,s2
  000da4ae: vmov.f32 s2,0x3f000000
  000da4b2: vmul.f32 s0,s0,s6
  000da4b6: vcmpe.f32 s4,#0
  000da4ba: vmrs apsr,fpscr
  000da4be: it gt
  000da4c0: vmov.gt.f32 s8,s4
  000da4c4: vstr.32 s4,[r9,#0x16c]
  000da4c8: vcmpe.f32 s8,s2
  000da4cc: vstr.32 s0,[r9,#0x170]
  000da4d0: vmrs apsr,fpscr
  000da4d4: vcmpe.f32 s0,#0
  000da4d8: ittt gt
  000da4da: vldr.gt.32 s6,[r9,#0x188]
  000da4de: vadd.gt.f32 s4,s4,s6
  000da4e2: vstr.gt.32 s4,[r9,#0x188]
  000da4e6: vmrs apsr,fpscr
  000da4ea: vneg.f32 s4,s0
  000da4ee: it gt
  000da4f0: vmov.gt.f32 s4,s0
  000da4f4: vcmpe.f32 s4,s2
  000da4f8: vmrs apsr,fpscr
  000da4fc: ittt gt
  000da4fe: vldr.gt.32 s2,[r9,#0x18c]
  000da502: vadd.gt.f32 s0,s0,s2
  000da506: vstr.gt.32 s0,[r9,#0x18c]
  000da50a: b 0x000da56c
  000da50c: vmov.f32 s4,0xbe800000
  000da510: vmov s6,r2
  000da514: vcvt.f32.s32 s6,s6
  000da518: cmp r3,#0xa
  000da51a: vmla.f32 s2,s6,s4
  000da51e: vstr.32 s2,[r9,#0x188]
  000da522: itt le
  000da524: mov.le r0,#0x0
  000da526: strb.le.w r0,[r9,#0x13b]
  000da52a: ldr r0,[0x000da79c]
  000da52c: cmp r5,r0
  000da52e: bgt 0x000da53a
  000da530: vmov.f32 s2,0x3e800000
  000da534: vmov s4,lr
  000da538: b 0x000da542
  000da53a: vmov.f32 s2,0xbe800000
  000da53e: vmov s4,r6
  000da542: vcvt.f32.s32 s4,s4
  000da546: cmp.w r3,r8, lsl #0x2
  000da54a: vmla.f32 s0,s4,s2
  000da54e: vstr.32 s0,[r9,#0x18c]
  000da552: bgt 0x000da56c
  000da554: lsl.w r0,r8,#0x2
  000da558: cmp r1,#0x0
  000da55a: bpl 0x000da55e
  000da55c: rsbs r1,r1
  000da55e: cmp r1,r0
  000da560: ittt le
  000da562: str.w.le r12,[r9,#0x1a0]
  000da566: mov.le r0,#0x0
  000da568: strb.le.w r0,[r9,#0x13b]
  000da56c: vldr.32 s0,[r9,#0x16c]
  000da570: vmov.f32 s6,0x41200000
  000da574: vldr.32 s2,[r9,#0x170]
  000da578: vldr.32 s4,[r9,#0x1c0]
  000da57c: vadd.f32 s0,s0,s2
  000da580: vcmpe.f32 s0,#0
  000da584: vmrs apsr,fpscr
  000da588: vneg.f32 s2,s0
  000da58c: it gt
  000da58e: vmov.gt.f32 s2,s0
  000da592: vldr.32 s0,[pc,#0x20c]
  000da596: vmul.f32 s0,s2,s0
  000da59a: vldr.32 s2,[pc,#0x208]
  000da59e: vcmpe.f32 s0,s6
  000da5a2: vmrs apsr,fpscr
  000da5a6: it mi
  000da5a8: vmov.mi.f32 s6,s0
  000da5ac: vadd.f32 s0,s4,s6
  000da5b0: vldr.32 s4,[pc,#0x1e4]
  000da5b4: vcmpe.f32 s0,s2
  000da5b8: vmrs apsr,fpscr
  000da5bc: it gt
  000da5be: vmov.gt.f32 s0,s4
  000da5c2: ldr.w r1,[r9,#0x90]
  000da5c6: vstr.32 s0,[r9,#0x1c0]
  000da5ca: cbz r1,0x000da602
  000da5cc: vmov s0,r8
  000da5d0: vldr.32 s2,[pc,#0x1d4]
  000da5d4: movs r5,#0x0
  000da5d6: vcvt.f32.s32 s0,s0
  000da5da: vmul.f32 s0,s0,s2
  000da5de: vmov r4,s0
  000da5e2: b 0x000da5fc
  000da5e4: ldr r0,[r1,#0x4]
  000da5e6: ldr.w r0,[r0,r5,lsl #0x2]
  000da5ea: cbz r0,0x000da5fa
  000da5ec: movs r1,#0x0
  000da5ee: mov r2,r4
  000da5f0: movs r3,#0x0
  000da5f2: blx 0x00072160
  000da5f6: ldr.w r1,[r9,#0x90]
  000da5fa: adds r5,#0x1
  000da5fc: ldr r0,[r1,#0x0]
  000da5fe: cmp r5,r0
  000da600: bcc 0x000da5e4
  000da602: vldr.32 s0,[pc,#0x1a8]
  000da606: movw r2,#0xffff
  000da60a: vldr.32 s4,[r9,#0x18c]
  000da60e: vldr.32 s6,[pc,#0x1a0]
  000da612: vmov.f32 s8,s0
  000da616: vcmpe.f32 s4,s0
  000da61a: vldr.32 s2,[r9,#0x188]
  000da61e: vmrs apsr,fpscr
  000da622: vcmpe.f32 s4,s6
  000da626: ldr r3,[0x000da7bc]
  000da628: it gt
  000da62a: vmov.gt.f32 s8,s6
  000da62e: vmrs apsr,fpscr
  000da632: vcmpe.f32 s4,s0
  000da636: it gt
  000da638: vmov.gt.f32 s8,s6
  000da63c: vmrs apsr,fpscr
  000da640: vmov.f32 s0,s8
  000da644: vcmpe.f32 s4,s6
  000da648: it gt
  000da64a: vmov.gt.f32 s0,s4
  000da64e: vcvt.s32.f32 s2,s2
  000da652: vmrs apsr,fpscr
  000da656: vldr.32 s4,[pc,#0x15c]
  000da65a: vmov r0,s2
  000da65e: asr.w r1,r0, asr #0x1f
  000da662: add.w r1,r0,r1, lsr #0x10
  000da666: bic.w r1,r1,r2
  000da66a: sub.w r0,r0,r1
  000da66e: vmov s2,r0
  000da672: vcvt.f32.s32 s2,s2
  000da676: it gt
  000da678: vmov.gt.f32 s0,s8
  000da67c: vldr.32 s8,[pc,#0x138]
  000da680: ldr.w r0,[r9,#0xa4]
  000da684: vmul.f32 s6,s2,s4
  000da688: vstr.32 s2,[r9,#0x188]
  000da68c: vmul.f32 s4,s0,s4
  000da690: vstr.32 s0,[r9,#0x18c]
  000da694: vmul.f32 s6,s6,s8
  000da698: vmul.f32 s4,s4,s8
  000da69c: vmov r2,s6
  000da6a0: vmov r1,s4
  000da6a4: blx 0x00073054
  000da6a8: ldr r0,[sp,#0xb4]
  000da6aa: ldr r1,[sp,#0x8]
  000da6ac: ldr r1,[r1,#0x0]
  000da6ae: subs r0,r1,r0
  000da6b0: itttt eq
  000da6b2: add.eq sp,#0xb8
  000da6b4: vpop.eq {d8,d9,d10}
  000da6b8: add.eq sp,#0x4
  000da6ba: pop.eq.w {r8,r9,r10,r11}
  000da6be: it eq
  000da6c0: pop.eq {r4,r5,r6,r7,pc}
  000da6c2: blx 0x0006e824
  000da6d4: it gt
  000da6d6: vmov.gt.f32 s16,s10
  000da6da: vdiv.f32 s0,s2,s0
  000da6de: vcmpe.f32 s8,s4
  000da6e2: vmrs apsr,fpscr
  000da6e6: bgt 0x000da6ee
  000da6e8: it pl
  000da6ea: vldr.pl.32 s0,[pc,#0xac]
  000da6ee: vcvt.s32.f32 s8,s16
  000da6f2: vcvt.s32.f32 s6,s6
  000da6f6: vmov.f32 s4,0x3f000000
  000da6fa: vmov r0,s8
  000da6fe: vmov r1,s6
  000da702: cmp r1,#0x0
  000da704: bpl 0x000da708
  000da706: rsbs r1,r1
  000da708: vmul.f32 s6,s16,s4
  000da70c: cmp r0,#0x0
  000da70e: bpl 0x000da712
  000da710: rsbs r0,r0
  000da712: cmp r0,r1
  000da714: it gt
  000da716: vmov.gt.f32 s16,s6
  000da71a: vcvt.s32.f32 s6,s0
  000da71e: vcvt.s32.f32 s2,s2
  000da722: vmov r0,s6
  000da726: vmov r1,s2
  000da72a: cmp r1,#0x0
  000da72c: bpl 0x000da730
  000da72e: rsbs r1,r1
  000da730: vneg.f32 s2,s16
  000da734: cmp r0,#0x0
  000da736: vmul.f32 s4,s0,s4
  000da73a: bpl 0x000da73e
  000da73c: rsbs r0,r0
  000da73e: cmp r0,r1
  000da740: vcmpe.f32 s16,#0
  000da744: it gt
  000da746: vmov.gt.f32 s0,s4
  000da74a: vmrs apsr,fpscr
  000da74e: vmov.f32 s4,0x40000000
  000da752: it gt
  000da754: vmov.gt.f32 s2,s16
  000da758: vstr.32 s16,[r9,#0x16c]
  000da75c: vcmpe.f32 s2,s4
  000da760: vstr.32 s0,[r9,#0x170]
  000da764: vmrs apsr,fpscr
  000da768: bhi 0x000da6a8
  000da76a: vcmpe.f32 s0,#0
  000da76e: vmrs apsr,fpscr
  000da772: vneg.f32 s2,s0
  000da776: it gt
  000da778: vmov.gt.f32 s2,s0
  000da77c: vcmpe.f32 s2,s4
  000da780: vmrs apsr,fpscr
  000da784: bhi 0x000da6a8
  000da786: ldr.w r0,[r9,#0x60]
  000da78a: movs r1,#0x0
  000da78c: strb.w r1,[r9,#0x13a]
  000da790: str.w r0,[r9,#0x19c]
  000da794: b 0x000da6a8
```
