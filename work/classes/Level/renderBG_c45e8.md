# Level::renderBG
elf 0xc45e8 body 1260
Sig: undefined __stdcall renderBG(int param_1)

## decompile
```c

/* Level::renderBG(int) */

void Level::renderBG(int param_1)

{
  bool bVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 in_r1;
  undefined4 extraout_r1;
  uint uVar5;
  code *pcVar6;
  int iVar7;
  undefined4 *puVar8;
  Matrix *pMVar9;
  uint *puVar10;
  uint in_fpscr;
  float fVar11;
  float fVar12;
  float extraout_s1;
  float extraout_s1_00;
  undefined4 extraout_s1_01;
  float extraout_s1_02;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar13;
  float fVar14;
  longlong lVar15;
  Vector aVStack_c4 [12];
  Vector aVStack_b8 [12];
  Vector aVStack_ac [12];
  Vector aVStack_a0 [12];
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88 [5];
  undefined4 local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined4 local_50;
  int local_4c;
  
  piVar2 = *(int **)(DAT_000d46fc + 0xd4602);
  puVar10 = *(uint **)(DAT_000d4700 + 0xd4604);
  local_4c = *piVar2;
  AbyssEngine::PaintCanvas::SetColor(*puVar10);
  AbyssEngine::PaintCanvas::BeginBG();
  uVar5 = *puVar10;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
  AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
  pMVar9 = (Matrix *)(param_1 + 0x1d0);
  AbyssEngine::AEMath::Matrix::operator=(pMVar9,(Matrix *)local_88);
  *(undefined4 *)(param_1 + 0x1ec) = 0;
  *(undefined4 *)(param_1 + 0x1dc) = 0;
  *(undefined4 *)(param_1 + 0x1fc) = 0;
  iVar3 = Status::inAlienOrbit();
  fVar11 = extraout_s1;
  fVar13 = extraout_s2;
  if (iVar3 == 0) {
    Status::getSystem();
    iVar3 = SolarSystem::getIndex();
    fVar11 = extraout_s1_00;
    fVar13 = extraout_s2_00;
    if (iVar3 == 0x1b) {
      local_90 = 0;
      local_94 = 0x3f800000;
      local_8c = 0;
      StarSystem::getLightDirection();
      pcVar6 = *(code **)(DAT_000d4708 + 0xd46a0);
      (*pcVar6)(aVStack_a0,local_88);
      AbyssEngine::AEMath::VectorCross((Vector *)local_88,(Vector *)&local_94);
      (*pcVar6)(aVStack_ac,(Vector *)local_88);
      AbyssEngine::AEMath::VectorCross(aVStack_c4,aVStack_ac);
      (*pcVar6)(aVStack_b8,aVStack_c4);
      AbyssEngine::AEMath::operator-((AEMath *)local_88,aVStack_b8);
      AbyssEngine::AEMath::Vector::operator=((Vector *)&local_94,(Vector *)local_88);
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)local_88,(Vector *)(param_1 + 0x20c),(Vector *)&local_94,aVStack_a0);
      goto LAB_000d4724;
    }
  }
  AbyssEngine::AEMath::MatrixSetRotation
            ((Matrix *)local_88,*(float *)(param_1 + 0x1ac),fVar11,fVar13);
LAB_000d4724:
  AbyssEngine::AEMath::Matrix::operator*=(pMVar9,(Matrix *)(param_1 + 0x20c));
  AbyssEngine::PaintCanvas::SetWorldViewMatrix((Matrix *)*puVar10);
  AbyssEngine::PaintCanvas::SetTexture(*puVar10,*(uint *)(param_1 + 0x19c));
  AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar10,0);
  AbyssEngine::PaintCanvas::DrawMesh((PaintCanvas *)*puVar10,*(uint *)(param_1 + 8));
  AbyssEngine::PaintCanvas::SetTexture(*puVar10,*(uint *)(param_1 + 0x198));
  AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar10,2);
  AbyssEngine::PaintCanvas::DrawMesh((PaintCanvas *)*puVar10,*(uint *)(param_1 + 4));
  if (*(int *)(param_1 + 0x1b4) != -1) {
    AbyssEngine::PaintCanvas::SetTexture(*puVar10,*(uint *)(param_1 + 0x1b8));
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar10,1);
    uVar5 = *puVar10;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
    AbyssEngine::AEMath::Matrix::operator=(pMVar9,(Matrix *)local_88);
    *(undefined4 *)(param_1 + 0x1ec) = 0;
    *(undefined4 *)(param_1 + 0x1dc) = 0;
    *(undefined4 *)(param_1 + 0x1fc) = 0;
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
    AbyssEngine::PaintCanvas::DrawTransform(*puVar10,*(Matrix **)(param_1 + 0x1b4));
  }
  StarSystem::render(*(StarSystem **)(param_1 + 0xec));
  iVar3 = Status::inSupernovaSystem();
  if ((iVar3 != 0) && (*(int *)(param_1 + 0xc) != -1)) {
    iVar3 = Status::getCurrentCampaignMission();
    AbyssEngine::PaintCanvas::SetTexture(*puVar10,*(uint *)(param_1 + 0x1a0));
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar10,2);
    fVar11 = 1.0;
    fVar13 = (float)VectorSignedToFloat(in_r1,(byte)(in_fpscr >> 0x16) & 3);
    if (0x6a < iVar3) {
      fVar11 = 1.5;
    }
    uVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    bVar1 = (bool)__aeabi_f2lz(fVar11 * fVar13);
    AbyssEngine::Transform::Update(CONCAT44(extraout_r1,uVar4),bVar1);
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
    AbyssEngine::PaintCanvas::DrawTransform(*puVar10,*(Matrix **)(param_1 + 0x10));
    uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    AbyssEngine::Transform::Update((ulonglong)uVar5,bVar1);
    AbyssEngine::PaintCanvas::DrawTransform(*puVar10,*(Matrix **)(param_1 + 0x18));
  }
  if (*(int *)(param_1 + 0x1bc) != -1) {
    AbyssEngine::PaintCanvas::SetTexture(*puVar10,*(uint *)(param_1 + 0x1c0));
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar10,2);
    uVar5 = *puVar10;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
    AbyssEngine::AEMath::Matrix::operator=(pMVar9,(Matrix *)local_88);
    iVar3 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    iVar7 = *(int *)(iVar3 + 0x110);
    lVar15 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar10);
    AbyssEngine::Transform::Update(lVar15,SUB41(in_r1,0));
    if (*(int *)(iVar3 + 0x110) < iVar7) {
      puVar8 = *(undefined4 **)(DAT_000d4b04 + 0xd4930);
      pcVar6 = *(code **)(DAT_000d4b08 + 0xd4932);
      uVar4 = (*pcVar6)(*puVar8,0x10000);
      fVar14 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
      uVar4 = (*pcVar6)(*puVar8,0x10000);
      fVar13 = DAT_000d4afc;
      fVar11 = DAT_000d4af8;
      fVar12 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
      uVar4 = (*pcVar6)(fVar12 * DAT_000d4af8,extraout_s1_01,fVar14 * DAT_000d4af8,*puVar8,0x10000);
      fVar12 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
      AbyssEngine::AEMath::MatrixSetRotation
                ((Matrix *)local_88,fVar12 * fVar11 * fVar13,extraout_s1_02,extraout_s2_01);
    }
    AbyssEngine::AEMath::Matrix::operator*=(pMVar9,(Matrix *)(param_1 + 0x248));
    *(undefined4 *)(param_1 + 0x1dc) = 0;
    *(undefined4 *)(param_1 + 0x1ec) = 0;
    *(undefined4 *)(param_1 + 0x1fc) = 0;
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
    AbyssEngine::PaintCanvas::DrawTransform(*puVar10,*(Matrix **)(param_1 + 0x1bc));
  }
  if ((*(char *)(param_1 + 0x289) != '\0') &&
     (1.0 <= *(float *)(*(int *)(DAT_000d4b0c + 0xd49d8) + 0x28))) {
    uVar5 = *puVar10;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
    AbyssEngine::AEMath::MatrixGetInverse((Matrix *)local_88);
    AbyssEngine::AEMath::Matrix::operator=(pMVar9,(Matrix *)local_88);
    *(undefined4 *)(param_1 + 0x1ec) = 0;
    *(undefined4 *)(param_1 + 0x1dc) = 0;
    *(undefined4 *)(param_1 + 0x1fc) = 0;
    AbyssEngine::PaintCanvas::SetWorldViewMatrix((Matrix *)*puVar10);
    AbyssEngine::PaintCanvas::SetColor(*puVar10);
    uStack_58 = DAT_000d4af0;
    uStack_60 = DAT_000d4ae8;
    uStack_6c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_68 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_64 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar8 = (undefined4 *)((uint)local_88 | 4);
    pMVar9 = *(Matrix **)(*puVar10 + 0x34);
    *puVar8 = 0;
    puVar8[1] = uStack_6c;
    puVar8[2] = uStack_68;
    puVar8[3] = uStack_64;
    local_70 = 0;
    local_88[0] = 0x3f800000;
    local_74 = 0x3f800000;
    local_50 = 0x3f800000;
    AbyssEngine::Engine::SetModelMatrix(pMVar9);
    AbyssEngine::PaintCanvas::SetTexture(*puVar10,*(uint *)(param_1 + 0x1c4));
    AbyssEngine::PaintCanvas::SetBlendMode((PaintCanvas *)*puVar10,8);
    AbyssEngine::Engine::LightSetLight(*(Engine **)(*puVar10 + 0x34),0x4000);
    bVar1 = SUB41(DAT_000d4b00,0);
    AbyssEngine::Engine::GlEnable(*(uint *)(*puVar10 + 0x34),bVar1);
    AbyssEngine::PaintCanvas::DrawMesh((PaintCanvas *)*puVar10,*(uint *)(param_1 + 0x1cc));
    AbyssEngine::Engine::GlEnable(*(uint *)(*puVar10 + 0x34),bVar1);
    AbyssEngine::Engine::LightEnable(SUB41(*(undefined4 *)(*puVar10 + 0x34),0));
  }
  AbyssEngine::PaintCanvas::EndBG((PaintCanvas *)*puVar10);
  if (*piVar2 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000d45e8: push {r4,r5,r6,r7,lr}
  000d45ea: add r7,sp,#0xc
  000d45ec: push {r7,r8,r9,r10,r11}
  000d45f0: vpush {d8,d9,d10,d11}
  000d45f4: sub sp,#0x88
  000d45f6: str r1,[sp,#0x4]
  000d45f8: mov r4,r0
  000d45fa: ldr r0,[0x000d46fc]
  000d45fc: ldr r1,[0x000d4700]
  000d45fe: add r0,pc
  000d4600: add r1,pc
  000d4602: ldr r0,[r0,#0x0]
  000d4604: ldr.w r11,[r1,#0x0]
  000d4608: str r0,[sp,#0x8]
  000d460a: ldr r1,[r0,#0x0]
  000d460c: ldr.w r0,[r11,#0x0]
  000d4610: str r1,[sp,#0x84]
  000d4612: mov.w r1,#0xffffffff
  000d4616: blx 0x0006fac0
  000d461a: ldr.w r0,[r11,#0x0]
  000d461e: blx 0x00074410
  000d4622: ldr.w r5,[r11,#0x0]
  000d4626: mov r0,r5
  000d4628: blx 0x000717f4
  000d462c: mov r1,r0
  000d462e: mov r0,r5
  000d4630: blx 0x0006ff1c
  000d4634: add r5,sp,#0x48
  000d4636: mov r1,r0
  000d4638: mov r0,r5
  000d463a: blx 0x0006fb50
  000d463e: add.w r10,r4,#0x1d0
  000d4642: mov r1,r5
  000d4644: mov r0,r10
  000d4646: blx 0x0006f148
  000d464a: ldr r0,[0x000d4704]
  000d464c: add r0,pc
  000d464e: ldr.w r9,[r0,#0x0]
  000d4652: movs r0,#0x0
  000d4654: str.w r0,[r4,#0x1ec]
  000d4658: str.w r0,[r4,#0x1dc]
  000d465c: str.w r0,[r4,#0x1fc]
  000d4660: ldr.w r0,[r9,#0x0]
  000d4664: blx 0x000723d0
  000d4668: cmp r0,#0x0
  000d466a: bne 0x000d470c
  000d466c: ldr.w r0,[r9,#0x0]
  000d4670: blx 0x00071a10
  000d4674: blx 0x00071a7c
  000d4678: cmp r0,#0x1b
  000d467a: bne 0x000d470c
  000d467c: movs r0,#0x0
  000d467e: mov.w r1,#0x3f800000
  000d4682: str r0,[sp,#0x40]
  000d4684: add.w r8,sp,#0x48
  000d4688: str r1,[sp,#0x3c]
  000d468a: str r0,[sp,#0x44]
  000d468c: mov r0,r8
  000d468e: ldr.w r1,[r4,#0xec]
  000d4692: blx 0x000727cc
  000d4696: ldr r0,[0x000d4708]
  000d4698: add r6,sp,#0x30
  000d469a: mov r1,r8
  000d469c: add r0,pc
  000d469e: ldr r5,[r0,#0x0]
  000d46a0: mov r0,r6
  000d46a2: blx r5
  000d46a4: add.w r8,sp,#0x48
  000d46a8: add r1,sp,#0x3c
  000d46aa: mov r2,r6
  000d46ac: mov r0,r8
  000d46ae: blx 0x0006ec8c
  000d46b2: add r6,sp,#0x24
  000d46b4: mov r1,r8
  000d46b6: mov r0,r6
  000d46b8: blx r5
  000d46ba: add r0,sp,#0xc
  000d46bc: add r2,sp,#0x30
  000d46be: mov r1,r6
  000d46c0: blx 0x0006ec8c
  000d46c4: add.w r8,sp,#0x18
  000d46c8: add r1,sp,#0xc
  000d46ca: mov r0,r8
  000d46cc: blx r5
  000d46ce: add r6,sp,#0x48
  000d46d0: mov r1,r8
  000d46d2: mov r0,r6
  000d46d4: blx 0x0006f4cc
  000d46d8: add.w r8,sp,#0x3c
  000d46dc: mov r1,r6
  000d46de: mov r0,r8
  000d46e0: blx 0x0006eb3c
  000d46e4: add r0,sp,#0x24
  000d46e6: add.w r5,r4,#0x20c
  000d46ea: str r0,[sp,#0x0]
  000d46ec: add r0,sp,#0x48
  000d46ee: add r3,sp,#0x30
  000d46f0: mov r1,r5
  000d46f2: mov r2,r8
  000d46f4: blx 0x000701bc
  000d46f8: b 0x000d4724
  000d470c: ldrd r2,r3,[r4,#0x1a4]
  000d4710: add.w r5,r4,#0x20c
  000d4714: vldr.32 s0,[r4,#0x1ac]
  000d4718: add r0,sp,#0x48
  000d471a: mov r1,r5
  000d471c: vstr.32 s0,[sp]
  000d4720: blx 0x00072094
  000d4724: mov r0,r10
  000d4726: mov r1,r5
  000d4728: blx 0x0006f4a8
  000d472c: ldr.w r0,[r11,#0x0]
  000d4730: mov r1,r10
  000d4732: blx 0x0006f55c
  000d4736: ldr.w r1,[r4,#0x19c]
  000d473a: mov.w r2,#0xffffffff
  000d473e: ldr.w r0,[r11,#0x0]
  000d4742: blx 0x0006fa6c
  000d4746: ldr.w r0,[r11,#0x0]
  000d474a: movs r1,#0x0
  000d474c: movs r6,#0x0
  000d474e: blx 0x0006febc
  000d4752: ldr r1,[r4,#0x8]
  000d4754: ldr.w r0,[r11,#0x0]
  000d4758: blx 0x0007441c
  000d475c: ldr.w r1,[r4,#0x198]
  000d4760: mov.w r2,#0xffffffff
  000d4764: ldr.w r0,[r11,#0x0]
  000d4768: blx 0x0006fa6c
  000d476c: ldr.w r0,[r11,#0x0]
  000d4770: movs r1,#0x2
  000d4772: blx 0x0006febc
  000d4776: ldr r1,[r4,#0x4]
  000d4778: ldr.w r0,[r11,#0x0]
  000d477c: blx 0x0007441c
  000d4780: ldr.w r0,[r4,#0x1b4]
  000d4784: adds r0,#0x1
  000d4786: beq 0x000d47ea
  000d4788: ldr.w r1,[r4,#0x1b8]
  000d478c: mov.w r2,#0xffffffff
  000d4790: ldr.w r0,[r11,#0x0]
  000d4794: blx 0x0006fa6c
  000d4798: ldr.w r0,[r11,#0x0]
  000d479c: movs r1,#0x1
  000d479e: blx 0x0006febc
  000d47a2: ldr.w r5,[r11,#0x0]
  000d47a6: mov r0,r5
  000d47a8: blx 0x000717f4
  000d47ac: mov r1,r0
  000d47ae: mov r0,r5
  000d47b0: blx 0x0006ff1c
  000d47b4: add r5,sp,#0x48
  000d47b6: mov r1,r0
  000d47b8: mov r0,r5
  000d47ba: blx 0x0006fb50
  000d47be: mov r0,r10
  000d47c0: mov r1,r5
  000d47c2: blx 0x0006f148
  000d47c6: add r5,sp,#0x48
  000d47c8: mov r1,r10
  000d47ca: str.w r6,[r4,#0x1ec]
  000d47ce: mov r0,r5
  000d47d0: str.w r6,[r4,#0x1dc]
  000d47d4: str.w r6,[r4,#0x1fc]
  000d47d8: blx 0x0006fb50
  000d47dc: ldr.w r1,[r4,#0x1b4]
  000d47e0: mov r2,r5
  000d47e2: ldr.w r0,[r11,#0x0]
  000d47e6: blx 0x0007306c
  000d47ea: ldr.w r0,[r4,#0xec]
  000d47ee: blx 0x00074428
  000d47f2: ldr.w r0,[r9,#0x0]
  000d47f6: blx 0x00071878
  000d47fa: cmp r0,#0x0
  000d47fc: beq 0x000d48a6
  000d47fe: ldr r0,[r4,#0xc]
  000d4800: adds r0,#0x1
  000d4802: beq 0x000d48a6
  000d4804: ldr.w r0,[r9,#0x0]
  000d4808: blx 0x00071770
  000d480c: mov r5,r0
  000d480e: ldr.w r0,[r11,#0x0]
  000d4812: ldr.w r1,[r4,#0x1a0]
  000d4816: mov.w r2,#0xffffffff
  000d481a: blx 0x0006fa6c
  000d481e: ldr.w r0,[r11,#0x0]
  000d4822: movs r1,#0x2
  000d4824: blx 0x0006febc
  000d4828: ldr r0,[sp,#0x4]
  000d482a: vmov.f32 s2,0x3fc00000
  000d482e: vmov.f32 s0,0x3f800000
  000d4832: cmp r5,#0x6a
  000d4834: vmov s4,r0
  000d4838: vcvt.f32.s32 s4,s4
  000d483c: it gt
  000d483e: vmov.gt.f32 s0,s2
  000d4842: ldr r1,[r4,#0x10]
  000d4844: ldr.w r0,[r11,#0x0]
  000d4848: vmul.f32 s16,s0,s4
  000d484c: blx 0x00072088
  000d4850: mov r5,r0
  000d4852: vmov r0,s16
  000d4856: blx 0x0006ebb4
  000d485a: mov r6,r0
  000d485c: movs r0,#0x0
  000d485e: str r0,[sp,#0x0]
  000d4860: mov r0,r5
  000d4862: mov r2,r6
  000d4864: mov r3,r1
  000d4866: mov r8,r1
  000d4868: blx 0x0006f7cc
  000d486c: add.w r9,sp,#0x48
  000d4870: mov r1,r10
  000d4872: mov r0,r9
  000d4874: blx 0x0006fb50
  000d4878: ldr r1,[r4,#0x10]
  000d487a: mov r2,r9
  000d487c: ldr.w r0,[r11,#0x0]
  000d4880: blx 0x0007306c
  000d4884: ldr r1,[r4,#0x18]
  000d4886: ldr.w r0,[r11,#0x0]
  000d488a: blx 0x00072088
  000d488e: movs r1,#0x0
  000d4890: mov r2,r6
  000d4892: mov r3,r8
  000d4894: str r1,[sp,#0x0]
  000d4896: blx 0x0006f7cc
  000d489a: ldr r1,[r4,#0x18]
  000d489c: mov r2,r9
  000d489e: ldr.w r0,[r11,#0x0]
  000d48a2: blx 0x0007306c
  000d48a6: ldr.w r0,[r4,#0x1bc]
  000d48aa: adds r0,#0x1
  000d48ac: beq.w 0x000d49c6
  000d48b0: ldr.w r1,[r4,#0x1c0]
  000d48b4: mov.w r2,#0xffffffff
  000d48b8: ldr.w r0,[r11,#0x0]
  000d48bc: blx 0x0006fa6c
  000d48c0: ldr.w r0,[r11,#0x0]
  000d48c4: movs r1,#0x2
  000d48c6: blx 0x0006febc
  000d48ca: ldr.w r5,[r11,#0x0]
  000d48ce: mov r0,r5
  000d48d0: blx 0x000717f4
  000d48d4: mov r1,r0
  000d48d6: mov r0,r5
  000d48d8: blx 0x0006ff1c
  000d48dc: add r5,sp,#0x48
  000d48de: mov r1,r0
  000d48e0: mov r0,r5
  000d48e2: blx 0x0006fb50
  000d48e6: mov r0,r10
  000d48e8: mov r1,r5
  000d48ea: blx 0x0006f148
  000d48ee: ldr.w r1,[r4,#0x1bc]
  000d48f2: ldr.w r0,[r11,#0x0]
  000d48f6: blx 0x00072088
  000d48fa: mov r5,r0
  000d48fc: ldr.w r6,[r0,#0x110]
  000d4900: ldr.w r0,[r11,#0x0]
  000d4904: ldr.w r1,[r4,#0x1bc]
  000d4908: blx 0x00072088
  000d490c: ldr r2,[sp,#0x4]
  000d490e: mov.w r8,#0x0
  000d4912: str.w r8,[sp,#0x0]
  000d4916: asrs r3,r2,#0x1f
  000d4918: blx 0x0006f7cc
  000d491c: ldr.w r0,[r5,#0x110]
  000d4920: add.w r9,r4,#0x248
  000d4924: cmp r0,r6
  000d4926: bge 0x000d499a
  000d4928: ldr r0,[0x000d4b04]
  000d492a: ldr r1,[0x000d4b08]
  000d492c: add r0,pc
  000d492e: add r1,pc
  000d4930: ldr r6,[r0,#0x0]
  000d4932: ldr r5,[r1,#0x0]
  000d4934: mov.w r1,#0x10000
  000d4938: ldr r0,[r6,#0x0]
  000d493a: blx r5
  000d493c: ldr r1,[r6,#0x0]
  000d493e: vmov s0,r0
  000d4942: vcvt.f32.s32 s16,s0
  000d4946: mov r0,r1
  000d4948: mov.w r1,#0x10000
  000d494c: blx r5
  000d494e: vmov s0,r0
  000d4952: vldr.32 s18,[pc,#0x1a4]
  000d4956: mov.w r1,#0x10000
  000d495a: vcvt.f32.s32 s0,s0
  000d495e: ldr r0,[r6,#0x0]
  000d4960: vmul.f32 s2,s16,s18
  000d4964: vldr.32 s16,[pc,#0x194]
  000d4968: vmul.f32 s0,s0,s18
  000d496c: vmul.f32 s20,s2,s16
  000d4970: vmul.f32 s22,s0,s16
  000d4974: blx r5
  000d4976: vmov s0,r0
  000d497a: add r0,sp,#0x48
  000d497c: vmov r2,s20
  000d4980: mov r1,r9
  000d4982: vcvt.f32.s32 s0,s0
  000d4986: vmov r3,s22
  000d498a: vmul.f32 s0,s0,s18
  000d498e: vmul.f32 s0,s0,s16
  000d4992: vstr.32 s0,[sp]
  000d4996: blx 0x00072094
  000d499a: mov r0,r10
  000d499c: mov r1,r9
  000d499e: blx 0x0006f4a8
  000d49a2: add r5,sp,#0x48
  000d49a4: mov r1,r10
  000d49a6: str.w r8,[r4,#0x1dc]
  000d49aa: mov r0,r5
  000d49ac: str.w r8,[r4,#0x1ec]
  000d49b0: str.w r8,[r4,#0x1fc]
  000d49b4: blx 0x0006fb50
  000d49b8: ldr.w r1,[r4,#0x1bc]
  000d49bc: mov r2,r5
  000d49be: ldr.w r0,[r11,#0x0]
  000d49c2: blx 0x0007306c
  000d49c6: ldrb.w r0,[r4,#0x289]
  000d49ca: cmp r0,#0x0
  000d49cc: beq 0x000d4ac0
  000d49ce: ldr r0,[0x000d4b0c]
  000d49d0: vmov.f32 s2,0x3f800000
  000d49d4: add r0,pc
  000d49d6: ldr r0,[r0,#0x0]
  000d49d8: vldr.32 s0,[r0,#0x28]
  000d49dc: vcmpe.f32 s0,s2
  000d49e0: vmrs apsr,fpscr
  000d49e4: blt 0x000d4ac0
  000d49e6: ldr.w r5,[r11,#0x0]
  000d49ea: mov r0,r5
  000d49ec: blx 0x000717f4
  000d49f0: mov r1,r0
  000d49f2: mov r0,r5
  000d49f4: blx 0x0006ff1c
  000d49f8: add r6,sp,#0x48
  000d49fa: mov r1,r0
  000d49fc: mov r0,r6
  000d49fe: blx 0x0006fb50
  000d4a02: mov r0,r10
  000d4a04: mov r1,r6
  000d4a06: blx 0x0006f148
  000d4a0a: movs r0,#0x0
  000d4a0c: mov r1,r10
  000d4a0e: str.w r0,[r4,#0x1ec]
  000d4a12: str.w r0,[r4,#0x1dc]
  000d4a16: str.w r0,[r4,#0x1fc]
  000d4a1a: ldr.w r0,[r11,#0x0]
  000d4a1e: blx 0x0006f55c
  000d4a22: ldr.w r0,[r11,#0x0]
  000d4a26: mov.w r1,#0xffffffff
  000d4a2a: blx 0x0006fac0
  000d4a2e: ldr.w r0,[r11,#0x0]
  000d4a32: adr r1,[0xd4ae8]
  000d4a34: vmov.i32 q8,#0x0
  000d4a38: vld1.64 {d18,d19},[r1]
  000d4a3c: orr r1,r6,#0x4
  000d4a40: ldr r0,[r0,#0x34]
  000d4a42: vst1.32 {d16,d17},[r1]
  000d4a46: add.w r1,r6,#0x18
  000d4a4a: vst1.64 {d16,d17},[r1]
  000d4a4e: add.w r1,r6,#0x28
  000d4a52: vst1.64 {d18,d19},[r1]
  000d4a56: mov.w r1,#0x3f800000
  000d4a5a: str r1,[sp,#0x48]
  000d4a5c: str r1,[sp,#0x5c]
  000d4a5e: str r1,[sp,#0x80]
  000d4a60: mov r1,r6
  000d4a62: blx 0x0006ffb8
  000d4a66: ldrd r1,r2,[r4,#0x1c4]
  000d4a6a: ldr.w r0,[r11,#0x0]
  000d4a6e: blx 0x0006fa6c
  000d4a72: ldr.w r0,[r11,#0x0]
  000d4a76: movs r1,#0x8
  000d4a78: blx 0x0006febc
  000d4a7c: ldr.w r0,[r11,#0x0]
  000d4a80: mov.w r1,#0x4000
  000d4a84: ldr r0,[r0,#0x34]
  000d4a86: blx 0x0006fe5c
  000d4a8a: ldr.w r0,[r11,#0x0]
  000d4a8e: movs r2,#0x1
  000d4a90: ldr r5,[0x000d4b00]
  000d4a92: ldr r0,[r0,#0x34]
  000d4a94: mov r1,r5
  000d4a96: blx 0x0006eddc
  000d4a9a: ldr.w r1,[r4,#0x1cc]
  000d4a9e: ldr.w r0,[r11,#0x0]
  000d4aa2: blx 0x0007441c
  000d4aa6: ldr.w r0,[r11,#0x0]
  000d4aaa: mov r1,r5
  000d4aac: movs r2,#0x0
  000d4aae: ldr r0,[r0,#0x34]
  000d4ab0: blx 0x0006eddc
  000d4ab4: ldr.w r0,[r11,#0x0]
  000d4ab8: movs r1,#0x0
  000d4aba: ldr r0,[r0,#0x34]
  000d4abc: blx 0x0006fe50
  000d4ac0: ldr.w r0,[r11,#0x0]
  000d4ac4: blx 0x00074434
  000d4ac8: ldr r0,[sp,#0x84]
  000d4aca: ldr r1,[sp,#0x8]
  000d4acc: ldr r1,[r1,#0x0]
  000d4ace: subs r0,r1,r0
  000d4ad0: itttt eq
  000d4ad2: add.eq sp,#0x88
  000d4ad4: vpop.eq {d8,d9,d10,d11}
  000d4ad8: add.eq sp,#0x4
  000d4ada: pop.eq.w {r8,r9,r10,r11}
  000d4ade: it eq
  000d4ae0: pop.eq {r4,r5,r6,r7,pc}
  000d4ae2: blx 0x0006e824
```
