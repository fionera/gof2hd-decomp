# CutScene::process
elf 0x98e8c body 1894
Sig: undefined __stdcall process(int param_1)

## decompile
```c

/* CutScene::process(int) */

void CutScene::process(int param_1)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  undefined4 uVar5;
  Station *pSVar6;
  Transform *this;
  undefined1 uVar7;
  uint uVar8;
  int iVar9;
  code *pcVar10;
  uint uVar11;
  int *piVar12;
  int *piVar13;
  uint *puVar14;
  code *pcVar15;
  uint in_fpscr;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s2;
  float fVar16;
  float extraout_s2_00;
  float fVar17;
  ulonglong in_d16;
  longlong lVar18;
  undefined8 uVar19;
  ulonglong uVar20;
  Matrix aMStack_a4 [60];
  undefined1 auStack_68 [64];
  int local_28;
  
  piVar12 = *(int **)(DAT_000a9268 + 0xa8e9e);
  local_28 = *piVar12;
  if (*(char *)(param_1 + 0x5c) != '\0') {
    uVar1 = AbyssEngine::ApplicationManager::GetCurrentTimeMillis();
    uVar11 = *(uint *)(param_1 + 0x50);
    uVar8 = uVar1 - *(int *)(param_1 + 0x48);
    uVar20 = in_d16 & 0xffff0000ffff0000 | (ulonglong)uVar1 & 0xffff;
    *(uint *)(param_1 + 0x50) = uVar11 + uVar8;
    *(uint *)(param_1 + 0x54) =
         *(int *)(param_1 + 0x54) + ((int)uVar8 >> 0x1f) + (uint)CARRY4(uVar11,uVar8);
    *(uint *)(param_1 + 0x58) = uVar8;
    *(ulonglong *)(param_1 + 0x40) = uVar20;
    *(ulonglong *)(param_1 + 0x48) = uVar20;
    if (*(int *)(param_1 + 0x68) != 0) {
      TargetFollowCamera::update(*(int *)(param_1 + 0x68));
    }
    iVar2 = *(int *)(param_1 + 0x88);
    if (iVar2 == 2) {
      fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x58),
                                          (byte)(in_fpscr >> 0x16) & 3);
      puVar14 = *(uint **)(DAT_000a9288 + 0xa90c0);
      *(float *)(param_1 + 4) = *(float *)(param_1 + 4) + *(float *)(param_1 + 0x24) * fVar17;
      uVar5 = AbyssEngine::PaintCanvas::CameraGetLocal(*puVar14);
      __aeabi_memcpy(auStack_68,uVar5,0x3c);
      AbyssEngine::AEMath::MatrixSetRotation(aMStack_a4,extraout_s0,extraout_s1,extraout_s2);
      AbyssEngine::PaintCanvas::CameraSetLocal(*puVar14,*(Matrix **)(param_1 + 0x74));
      uVar19 = Status::getPlayingTime();
      if ((((int)(uint)((int)uVar19 == 0) <= (int)((ulonglong)uVar19 >> 0x20)) &&
          (puVar14 = (uint *)Level::getEnemies(), puVar14 != (uint *)0x0)) &&
         (uVar1 = *puVar14, 1 < uVar1)) {
        uVar8 = puVar14[1];
        iVar2 = *(int *)(uVar8 + uVar1 * 4 + -8);
        fVar17 = extraout_s1_00;
        if ((iVar2 != 0) && (*(int *)(iVar2 + 8) != 0)) {
          fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x58),
                                              (byte)(in_fpscr >> 0x16) & 3);
          AEGeometry::translate(fVar17 * DAT_000a9294,extraout_s1_00,fVar17 * DAT_000a9290);
          fVar17 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x58),(byte)(in_fpscr >> 0x16) & 3)
          ;
          fVar16 = (float)VectorSignedToFloat(-*(int *)(param_1 + 0x58),(byte)(in_fpscr >> 0x16) & 3
                                             );
          AEGeometry::rotate(fVar17 * DAT_000a9298,extraout_s1_01,fVar16 * DAT_000a929c);
          uVar1 = *puVar14;
          uVar8 = puVar14[1];
          fVar17 = extraout_s1_02;
        }
        iVar2 = *(int *)(uVar8 + uVar1 * 4 + -4);
        if ((iVar2 != 0) && (*(int *)(iVar2 + 8) != 0)) {
          fVar16 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0x58),
                                              (byte)(in_fpscr >> 0x16) & 3);
          AEGeometry::translate(fVar16 * DAT_000a9290,fVar17,DAT_000a9290);
          fVar17 = (float)VectorSignedToFloat(*(int *)(param_1 + 0x58),(byte)(in_fpscr >> 0x16) & 3)
          ;
          fVar16 = (float)VectorSignedToFloat(-*(int *)(param_1 + 0x58),(byte)(in_fpscr >> 0x16) & 3
                                             );
          AEGeometry::rotate(fVar17 * DAT_000a9298,extraout_s1_03,fVar16 * DAT_000a92a4);
        }
      }
    }
    else if (iVar2 == 0x17) {
      pSVar6 = (Station *)Status::getStation();
      iVar2 = Station::getIndex(pSVar6);
      if (iVar2 == 0x65) {
        uVar1 = 10;
      }
      else {
        pSVar6 = (Station *)Status::getStation();
        iVar2 = Station::getIndex(pSVar6);
        if (iVar2 == 100) {
          uVar1 = 7;
        }
        else {
          Status::getSystem();
          uVar1 = SolarSystem::getRace();
          uVar1 = uVar1 | 2;
        }
      }
      *(int *)(param_1 + 0x84) = *(int *)(param_1 + 0x58) + *(int *)(param_1 + 0x84);
      Status::getSystem();
      iVar2 = SolarSystem::getRace();
      if (iVar2 == 1) {
        AbyssEngine::PaintCanvas::FogSetParameter
                  ((PaintCanvas *)**(undefined4 **)(DAT_000a9640 + 0xa9410),0x2601,0,DAT_000a963c,
                   0x3f800000,DAT_000a9638);
      }
      else if ((uVar1 == 2) && (puVar14 = *(uint **)(param_1 + 0x38), puVar14 != (uint *)0x0)) {
        puVar3 = *(uint **)(DAT_000a9648 + 0xa943a);
        for (uVar1 = 0; uVar8 = *puVar14, uVar1 < uVar8; uVar1 = uVar1 + 1) {
          lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
          AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
          puVar14 = *(uint **)(param_1 + 0x38);
        }
        if (3000 < *(int *)(param_1 + 0x84)) {
          *(undefined4 *)(param_1 + 0x84) = 0;
          piVar13 = *(int **)(DAT_000a964c + 0xa9478);
          pcVar10 = *(code **)(DAT_000a9650 + 0xa9480);
          pcVar15 = *(code **)(DAT_000a9654 + 0xa9488);
          for (uVar1 = 0; uVar1 < uVar8; uVar1 = uVar1 + 1) {
            iVar2 = AbyssEngine::AERandom::nextInt(*piVar13);
            if (iVar2 < 0x14) {
              this = (Transform *)AbyssEngine::PaintCanvas::TransformGetTransform(*puVar3);
              iVar2 = AbyssEngine::Transform::IsRunning(this);
              if (iVar2 == 0) {
                uVar5 = (*pcVar10)(*puVar3,*(undefined4 *)
                                            (*(int *)(*(int *)(*(int *)(param_1 + 0x38) + 4) +
                                                     uVar1 * 4) + 0xc));
                (*pcVar15)(uVar5,3,0);
                uVar5 = (*pcVar10)(*puVar3,*(undefined4 *)
                                            (*(int *)(*(int *)(*(int *)(param_1 + 0x38) + 4) +
                                                     uVar1 * 4) + 0xc));
                (*pcVar15)(uVar5,1,0);
                uVar5 = (*pcVar10)(*puVar3,*(undefined4 *)
                                            (*(int *)(*(int *)(*(int *)(param_1 + 0x38) + 4) +
                                                     uVar1 * 4) + 0x14));
                (*pcVar15)(uVar5,3,0);
                uVar5 = (*pcVar10)(*puVar3,*(undefined4 *)
                                            (*(int *)(*(int *)(*(int *)(param_1 + 0x38) + 4) +
                                                     uVar1 * 4) + 0x14));
                (*pcVar15)(uVar5,1,0);
              }
            }
            uVar8 = **(uint **)(param_1 + 0x38);
          }
        }
      }
      puVar14 = *(uint **)(DAT_000a9644 + 0xa9510);
      uVar1 = *puVar14;
      Level::getEnemies();
      uVar5 = AbyssEngine::PaintCanvas::TransformGetLocal(uVar1);
      __aeabi_memcpy(auStack_68,uVar5,0x3c);
      AbyssEngine::AEMath::MatrixSetRotation
                (aMStack_a4,extraout_s0_00,extraout_s1_04,extraout_s2_00);
      uVar1 = *puVar14;
      iVar2 = Level::getEnemies();
      AbyssEngine::PaintCanvas::TransformSetLocal
                (uVar1,*(Matrix **)(*(int *)(**(int **)(iVar2 + 4) + 8) + 0xc));
      for (uVar1 = 0; puVar3 = (uint *)Level::getEnemies(), uVar1 < *puVar3; uVar1 = uVar1 + 1) {
        iVar2 = Level::getEnemies();
        if (*(int *)(*(int *)(iVar2 + 4) + uVar1 * 4) != 0) {
          uVar8 = *puVar14;
          Level::getEnemies();
          lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(uVar8);
          AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
        }
      }
    }
    else if (iVar2 == 4) {
      puVar14 = *(uint **)(DAT_000a9270 + 0xa8f14);
      for (uVar1 = 0; puVar3 = (uint *)Level::getEnemies(), uVar1 < *puVar3; uVar1 = uVar1 + 1) {
        iVar2 = Level::getEnemies();
        if (*(int *)(*(int *)(iVar2 + 4) + uVar1 * 4) != 0) {
          uVar8 = *puVar14;
          Level::getEnemies();
          lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(uVar8);
          AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
        }
      }
      Status::getSystem();
      iVar2 = SolarSystem::getRace();
      if (iVar2 == 0) {
        uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
        uVar7 = (undefined1)*(undefined4 *)(param_1 + 0x58);
        uVar20 = (ulonglong)uVar1;
      }
      else {
        Status::getSystem();
        iVar2 = SolarSystem::getRace();
        if (iVar2 == 1) {
          AbyssEngine::PaintCanvas::FogSetParameter
                    ((PaintCanvas *)*puVar14,0x2601,0,DAT_000a927c,0x3f800000,DAT_000a9278);
          AbyssEngine::PaintCanvas::FogEnable(*puVar14,1,1);
          uVar1 = *puVar14;
          Level::getEnemies();
          Level::getEnemies();
          lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(uVar1);
          AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
          if (*(int *)(param_1 + 0x30) != 0) {
            lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
            AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
            iVar4 = *(int *)(param_1 + 0x58) + *(int *)(param_1 + 0x7c);
            *(int *)(param_1 + 0x7c) = iVar4;
            iVar2 = DAT_000a9280;
            if (20000 < iVar4) {
              *(undefined4 *)(param_1 + 0x7c) = 0;
              iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(iVar2 + 0xa9014));
              if (iVar2 < 100) {
                uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
                AbyssEngine::Transform::SetAnimationState(uVar5,3,0);
                uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
                AbyssEngine::Transform::SetAnimationState(uVar5,1,0);
              }
            }
          }
          if (*(int *)(param_1 + 0x34) != 0) {
            lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
            AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
            iVar4 = *(int *)(param_1 + 0x58) + *(int *)(param_1 + 0x80);
            *(int *)(param_1 + 0x80) = iVar4;
            iVar2 = DAT_000a9284;
            if (22000 < iVar4) {
              *(undefined4 *)(param_1 + 0x80) = 0;
              iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(iVar2 + 0xa9086));
              if (iVar2 < 100) {
                uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
                AbyssEngine::Transform::SetAnimationState(uVar5,3,0);
                goto LAB_000a93b0;
              }
            }
          }
          goto LAB_000a95ac;
        }
        Status::getSystem();
        iVar2 = SolarSystem::getRace();
        if (iVar2 == 3) {
          uVar1 = *puVar14;
          Level::getEnemies();
          Level::getEnemies();
          lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(uVar1);
          AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
          if (*(int *)(param_1 + 0x28) != 0) {
            lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
            AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
          }
          lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
          AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
          iVar9 = *(int *)(param_1 + 0x7c) + *(int *)(param_1 + 0x58);
          iVar4 = *(int *)(param_1 + 0x58) + *(int *)(param_1 + 0x80);
          *(int *)(param_1 + 0x7c) = iVar9;
          *(int *)(param_1 + 0x80) = iVar4;
          iVar2 = DAT_000a9658;
          if ((1000 < iVar9) && (*(int *)(param_1 + 0x28) != 0)) {
            *(undefined4 *)(param_1 + 0x7c) = 0;
            iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(iVar2 + 0xa9342));
            if (iVar2 < 0x28) {
              uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
              AbyssEngine::Transform::SetAnimationState(uVar5,3,0);
              uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
              AbyssEngine::Transform::SetAnimationState(uVar5,1,0);
            }
            iVar4 = *(int *)(param_1 + 0x80);
          }
          iVar2 = DAT_000a965c;
          if (2000 < iVar4) {
            *(undefined4 *)(param_1 + 0x80) = 0;
            iVar2 = AbyssEngine::AERandom::nextInt(**(int **)(iVar2 + 0xa938e));
            if (iVar2 < 0x1e) {
              uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
              AbyssEngine::Transform::SetAnimationState(uVar5,3,0);
LAB_000a93b0:
              uVar5 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar14);
              iVar2 = *piVar12 - local_28;
              if (iVar2 == 0) {
                (*(code *)(DAT_001ab9d4 + 0x1ab9d8))();
                return;
              }
                    /* WARNING: Subroutine does not return */
              __stack_chk_fail(uVar5,iVar2,*piVar12);
            }
          }
          goto LAB_000a95ac;
        }
        Status::getSystem();
        iVar2 = SolarSystem::getRace();
        if (iVar2 != 2) goto LAB_000a95ac;
        uVar1 = *puVar14;
        pcVar10 = *(code **)(DAT_000a9660 + 0xa95d8);
        (*pcVar10)(*(undefined4 *)param_1);
        (*pcVar10)(*(undefined4 *)param_1);
        lVar18 = AbyssEngine::PaintCanvas::TransformGetTransform(uVar1);
        AbyssEngine::Transform::Update(lVar18,SUB41(*(undefined4 *)(param_1 + 0x58),0));
        uVar1 = *puVar14;
        (*pcVar10)(*(undefined4 *)param_1);
        (*pcVar10)(*(undefined4 *)param_1);
        uVar20 = AbyssEngine::PaintCanvas::TransformGetTransform(uVar1);
        uVar7 = (undefined1)*(undefined4 *)(param_1 + 0x58);
      }
      AbyssEngine::Transform::Update(uVar20,(bool)uVar7);
    }
  }
LAB_000a95ac:
  if (*piVar12 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000a8e8c: push {r4,r5,r6,r7,lr}
  000a8e8e: add r7,sp,#0xc
  000a8e90: push {r8,r9,r10,r11}
  000a8e94: sub sp,#0x8c
  000a8e96: mov r4,r0
  000a8e98: ldr r0,[0x000a9268]
  000a8e9a: add r0,pc
  000a8e9c: ldr.w r8,[r0,#0x0]
  000a8ea0: ldr.w r0,[r8,#0x0]
  000a8ea4: str r0,[sp,#0x88]
  000a8ea6: ldrb.w r0,[r4,#0x5c]
  000a8eaa: cmp r0,#0x0
  000a8eac: beq.w 0x000a95ac
  000a8eb0: ldr r0,[0x000a926c]
  000a8eb2: add r0,pc
  000a8eb4: ldr r0,[r0,#0x0]
  000a8eb6: ldr r0,[r0,#0x0]
  000a8eb8: blx 0x000713d4
  000a8ebc: mov r2,r0
  000a8ebe: ldr r3,[r4,#0x48]
  000a8ec0: vmov.32 d16[0x0],r2
  000a8ec4: ldrd r6,r5,[r4,#0x50]
  000a8ec8: subs r2,r2,r3
  000a8eca: ldr r0,[r4,#0x68]
  000a8ecc: vmov.32 d16[0x1],r1
  000a8ed0: adds r1,r6,r2
  000a8ed2: adc.w r3,r5,r2, asr #0x1f
  000a8ed6: cmp r0,#0x0
  000a8ed8: vmov d17,d16
  000a8edc: strd r1,r3,[r4,#0x50]
  000a8ee0: add.w r1,r4,#0x40
  000a8ee4: str r2,[r4,#0x58]
  000a8ee6: vst1.64 {d16,d17},[r1]
  000a8eea: beq 0x000a8ef2
  000a8eec: movs r1,#0x1e
  000a8eee: blx 0x000721a8
  000a8ef2: ldr.w r0,[r4,#0x88]
  000a8ef6: cmp r0,#0x2
  000a8ef8: beq.w 0x000a90aa
  000a8efc: cmp r0,#0x17
  000a8efe: beq.w 0x000a9220
  000a8f02: cmp r0,#0x4
  000a8f04: bne.w 0x000a95ac
  000a8f08: ldr r0,[0x000a9270]
  000a8f0a: mov.w r9,#0x0
  000a8f0e: movs r6,#0x0
  000a8f10: add r0,pc
  000a8f12: ldr.w r11,[r0,#0x0]
  000a8f16: b 0x000a8f4e
  000a8f18: ldr r0,[r4,#0x0]
  000a8f1a: blx 0x00071eb4
  000a8f1e: ldr r0,[r0,#0x4]
  000a8f20: ldr.w r0,[r0,r6,lsl #0x2]
  000a8f24: cbz r0,0x000a8f4c
  000a8f26: ldr r0,[r4,#0x0]
  000a8f28: ldr.w r5,[r11,#0x0]
  000a8f2c: blx 0x00071eb4
  000a8f30: ldr r0,[r0,#0x4]
  000a8f32: ldr.w r0,[r0,r6,lsl #0x2]
  000a8f36: ldr r0,[r0,#0x8]
  000a8f38: ldr r1,[r0,#0xc]
  000a8f3a: mov r0,r5
  000a8f3c: blx 0x00072088
  000a8f40: ldr r2,[r4,#0x58]
  000a8f42: str.w r9,[sp,#0x0]
  000a8f46: asrs r3,r2,#0x1f
  000a8f48: blx 0x0006f7cc
  000a8f4c: adds r6,#0x1
  000a8f4e: ldr r0,[r4,#0x0]
  000a8f50: blx 0x00071eb4
  000a8f54: ldr r0,[r0,#0x0]
  000a8f56: cmp r6,r0
  000a8f58: bcc 0x000a8f18
  000a8f5a: ldr r0,[0x000a9274]
  000a8f5c: add r0,pc
  000a8f5e: ldr r5,[r0,#0x0]
  000a8f60: ldr r0,[r5,#0x0]
  000a8f62: blx 0x00071a10
  000a8f66: blx 0x00071950
  000a8f6a: cmp r0,#0x0
  000a8f6c: beq.w 0x000a924c
  000a8f70: ldr r0,[r5,#0x0]
  000a8f72: blx 0x00071a10
  000a8f76: blx 0x00071950
  000a8f7a: cmp r0,#0x1
  000a8f7c: bne.w 0x000a92ac
  000a8f80: ldr r1,[0x000a9278]
  000a8f82: mov.w r2,#0x3f800000
  000a8f86: ldr.w r0,[r11,#0x0]
  000a8f8a: mov.w r10,#0x0
  000a8f8e: ldr r3,[0x000a927c]
  000a8f90: strd r2,r1,[sp,#0x0]
  000a8f94: movw r1,#0x2601
  000a8f98: movs r2,#0x0
  000a8f9a: blx 0x00071ff8
  000a8f9e: ldr.w r0,[r11,#0x0]
  000a8fa2: movs r1,#0x1
  000a8fa4: movs r2,#0x1
  000a8fa6: blx 0x00071fe0
  000a8faa: ldr r0,[r4,#0x0]
  000a8fac: ldr.w r9,[r11,#0x0]
  000a8fb0: blx 0x00071eb4
  000a8fb4: mov r5,r0
  000a8fb6: ldr r0,[r4,#0x0]
  000a8fb8: blx 0x00071eb4
  000a8fbc: ldr r0,[r0,#0x0]
  000a8fbe: ldr r1,[r5,#0x4]
  000a8fc0: add.w r0,r1,r0, lsl #0x2
  000a8fc4: ldr.w r0,[r0,#-0xc]
  000a8fc8: ldr r0,[r0,#0x8]
  000a8fca: ldr r1,[r0,#0xc]
  000a8fcc: mov r0,r9
  000a8fce: blx 0x00072088
  000a8fd2: ldr r2,[r4,#0x58]
  000a8fd4: str.w r10,[sp,#0x0]
  000a8fd8: asrs r3,r2,#0x1f
  000a8fda: blx 0x0006f7cc
  000a8fde: ldr r0,[r4,#0x30]
  000a8fe0: cbz r0,0x000a9046
  000a8fe2: ldr r1,[r0,#0xc]
  000a8fe4: ldr.w r0,[r11,#0x0]
  000a8fe8: blx 0x00072088
  000a8fec: ldr r2,[r4,#0x58]
  000a8fee: str.w r10,[sp,#0x0]
  000a8ff2: asrs r3,r2,#0x1f
  000a8ff4: blx 0x0006f7cc
  000a8ff8: ldr r0,[r4,#0x58]
  000a8ffa: ldr r1,[r4,#0x7c]
  000a8ffc: add r0,r1
  000a8ffe: movw r1,#0x4e21
  000a9002: cmp r0,r1
  000a9004: str r0,[r4,#0x7c]
  000a9006: blt 0x000a9046
  000a9008: ldr r0,[0x000a9280]
  000a900a: movs r1,#0x0
  000a900c: str r1,[r4,#0x7c]
  000a900e: movs r1,#0x64
  000a9010: add r0,pc
  000a9012: ldr r0,[r0,#0x0]
  000a9014: ldr r0,[r0,#0x0]
  000a9016: blx 0x00071848
  000a901a: cmp r0,#0x63
  000a901c: bgt 0x000a9046
  000a901e: ldr r0,[r4,#0x30]
  000a9020: ldr r1,[r0,#0xc]
  000a9022: ldr.w r0,[r11,#0x0]
  000a9026: blx 0x00072088
  000a902a: movs r1,#0x3
  000a902c: movs r2,#0x0
  000a902e: blx 0x0006fd18
  000a9032: ldr r1,[r4,#0x30]
  000a9034: ldr.w r0,[r11,#0x0]
  000a9038: ldr r1,[r1,#0xc]
  000a903a: blx 0x00072088
  000a903e: movs r1,#0x1
  000a9040: movs r2,#0x0
  000a9042: blx 0x0006fd18
  000a9046: ldr r0,[r4,#0x34]
  000a9048: cmp r0,#0x0
  000a904a: beq.w 0x000a95ac
  000a904e: ldr r1,[r0,#0xc]
  000a9050: ldr.w r0,[r11,#0x0]
  000a9054: blx 0x00072088
  000a9058: ldr r2,[r4,#0x58]
  000a905a: movs r5,#0x0
  000a905c: str r5,[sp,#0x0]
  000a905e: asrs r3,r2,#0x1f
  000a9060: blx 0x0006f7cc
  000a9064: ldr r0,[r4,#0x58]
  000a9066: ldr.w r1,[r4,#0x80]
  000a906a: add r0,r1
  000a906c: movw r1,#0x55f1
  000a9070: cmp r0,r1
  000a9072: str.w r0,[r4,#0x80]
  000a9076: blt.w 0x000a95ac
  000a907a: ldr r0,[0x000a9284]
  000a907c: movs r1,#0x64
  000a907e: str.w r5,[r4,#0x80]
  000a9082: add r0,pc
  000a9084: ldr r0,[r0,#0x0]
  000a9086: ldr r0,[r0,#0x0]
  000a9088: blx 0x00071848
  000a908c: cmp r0,#0x63
  000a908e: bgt.w 0x000a95ac
  000a9092: ldr r0,[r4,#0x34]
  000a9094: ldr r1,[r0,#0xc]
  000a9096: ldr.w r0,[r11,#0x0]
  000a909a: blx 0x00072088
  000a909e: movs r1,#0x3
  000a90a0: movs r2,#0x0
  000a90a2: blx 0x0006fd18
  000a90a6: ldr r1,[r4,#0x34]
  000a90a8: b 0x000a93b0
  000a90aa: vldr.32 s4,[r4,#0x58]
  000a90ae: vldr.32 s0,[r4,#0x4]
  000a90b2: vldr.32 s2,[r4,#0x24]
  000a90b6: vcvt.f32.s32 s4,s4
  000a90ba: ldr r0,[0x000a9288]
  000a90bc: add r0,pc
  000a90be: ldr r6,[r0,#0x0]
  000a90c0: vmla.f32 s0,s2,s4
  000a90c4: vstr.32 s0,[r4,#0x4]
  000a90c8: ldr r1,[r4,#0x74]
  000a90ca: ldr r0,[r6,#0x0]
  000a90cc: blx 0x0006ff1c
  000a90d0: add r5,sp,#0x48
  000a90d2: mov r1,r0
  000a90d4: movs r2,#0x3c
  000a90d6: mov r0,r5
  000a90d8: blx 0x0006f1e4
  000a90dc: ldr r3,[r4,#0x4]
  000a90de: movs r0,#0x0
  000a90e0: str r0,[sp,#0x0]
  000a90e2: add r0,sp,#0xc
  000a90e4: mov r1,r5
  000a90e6: movs r2,#0x0
  000a90e8: blx 0x00072094
  000a90ec: ldr r1,[r4,#0x74]
  000a90ee: mov r2,r5
  000a90f0: ldr r0,[r6,#0x0]
  000a90f2: blx 0x000720a0
  000a90f6: ldr r0,[0x000a928c]
  000a90f8: add r0,pc
  000a90fa: ldr r0,[r0,#0x0]
  000a90fc: ldr r0,[r0,#0x0]
  000a90fe: blx 0x00071710
  000a9102: subs r0,#0x1
  000a9104: sbcs r0,r1,#0x0
  000a9108: blt.w 0x000a95ac
  000a910c: ldr r0,[r4,#0x0]
  000a910e: blx 0x00071eb4
  000a9112: mov r5,r0
  000a9114: cmp r0,#0x0
  000a9116: beq.w 0x000a95ac
  000a911a: ldr r1,[r5,#0x0]
  000a911c: cmp r1,#0x2
  000a911e: bcc.w 0x000a95ac
  000a9122: ldr r2,[r5,#0x4]
  000a9124: add.w r0,r2,r1, lsl #0x2
  000a9128: ldr.w r0,[r0,#-0x8]
  000a912c: cbz r0,0x000a91a2
  000a912e: ldr r0,[r0,#0x8]
  000a9130: cbz r0,0x000a91a2
  000a9132: vldr.32 s0,[r4,#0x58]
  000a9136: movs r2,#0x0
  000a9138: vldr.32 s2,[pc,#0x154]
  000a913c: vcvt.f32.s32 s0,s0
  000a9140: vldr.32 s4,[pc,#0x150]
  000a9144: vmul.f32 s2,s0,s2
  000a9148: vmul.f32 s0,s0,s4
  000a914c: vmov r1,s2
  000a9150: vmov r3,s0
  000a9154: blx 0x00072154
  000a9158: ldr r0,[r4,#0x58]
  000a915a: vldr.32 s4,[pc,#0x13c]
  000a915e: vldr.32 s6,[pc,#0x13c]
  000a9162: vmov s0,r0
  000a9166: rsbs r0,r0
  000a9168: vmov s2,r0
  000a916c: vcvt.f32.s32 s0,s0
  000a9170: vcvt.f32.s32 s2,s2
  000a9174: ldrd r0,r6,[r5,#0x0]
  000a9178: add.w r0,r6,r0, lsl #0x2
  000a917c: ldr.w r0,[r0,#-0x8]
  000a9180: vmul.f32 s0,s0,s4
  000a9184: vmul.f32 s4,s2,s4
  000a9188: vmul.f32 s2,s2,s6
  000a918c: ldr r0,[r0,#0x8]
  000a918e: vmov r2,s0
  000a9192: vmov r1,s4
  000a9196: vmov r3,s2
  000a919a: blx 0x00072160
  000a919e: ldrd r1,r2,[r5,#0x0]
  000a91a2: add.w r0,r2,r1, lsl #0x2
  000a91a6: ldr.w r0,[r0,#-0x4]
  000a91aa: cmp r0,#0x0
  000a91ac: beq.w 0x000a95ac
  000a91b0: ldr r0,[r0,#0x8]
  000a91b2: cmp r0,#0x0
  000a91b4: beq.w 0x000a95ac
  000a91b8: vldr.32 s0,[r4,#0x58]
  000a91bc: movs r2,#0x0
  000a91be: vldr.32 s2,[pc,#0xd0]
  000a91c2: movs r3,#0x0
  000a91c4: vcvt.f32.s32 s0,s0
  000a91c8: vmul.f32 s0,s0,s2
  000a91cc: vmov r1,s0
  000a91d0: blx 0x00072154
  000a91d4: ldr r0,[r4,#0x58]
  000a91d6: vldr.32 s4,[pc,#0xc0]
  000a91da: vldr.32 s6,[pc,#0xc4]
  000a91de: vmov s0,r0
  000a91e2: rsbs r0,r0
  000a91e4: vldr.32 s8,[pc,#0xbc]
  000a91e8: vmov s2,r0
  000a91ec: vcvt.f32.s32 s0,s0
  000a91f0: vcvt.f32.s32 s2,s2
  000a91f4: ldrd r0,r6,[r5,#0x0]
  000a91f8: add.w r0,r6,r0, lsl #0x2
  000a91fc: ldr.w r0,[r0,#-0x4]
  000a9200: vmul.f32 s0,s0,s4
  000a9204: vmul.f32 s4,s2,s6
  000a9208: vmul.f32 s2,s2,s8
  000a920c: ldr r0,[r0,#0x8]
  000a920e: vmov r1,s0
  000a9212: vmov r2,s4
  000a9216: vmov r3,s2
  000a921a: blx 0x00072160
  000a921e: b 0x000a95ac
  000a9220: ldr r0,[0x000a92a8]
  000a9222: add r0,pc
  000a9224: ldr r5,[r0,#0x0]
  000a9226: ldr r0,[r5,#0x0]
  000a9228: blx 0x00071c14
  000a922c: blx 0x00071824
  000a9230: cmp r0,#0x65
  000a9232: bne 0x000a9238
  000a9234: movs r6,#0xa
  000a9236: b 0x000a93ea
  000a9238: ldr r0,[r5,#0x0]
  000a923a: blx 0x00071c14
  000a923e: blx 0x00071824
  000a9242: cmp r0,#0x64
  000a9244: bne.w 0x000a93dc
  000a9248: movs r6,#0x7
  000a924a: b 0x000a93ea
  000a924c: ldr r0,[r4,#0x30]
  000a924e: ldr r1,[r0,#0xc]
  000a9250: ldr.w r0,[r11,#0x0]
  000a9254: blx 0x00072088
  000a9258: ldr r2,[r4,#0x58]
  000a925a: movs r1,#0x0
  000a925c: str r1,[sp,#0x0]
  000a925e: asrs r3,r2,#0x1f
  000a9260: blx 0x0006f7cc
  000a9264: b 0x000a95ac
  000a92ac: ldr r0,[r5,#0x0]
  000a92ae: blx 0x00071a10
  000a92b2: blx 0x00071950
  000a92b6: cmp r0,#0x3
  000a92b8: bne.w 0x000a95c2
  000a92bc: ldr r0,[r4,#0x0]
  000a92be: ldr.w r9,[r11,#0x0]
  000a92c2: blx 0x00071eb4
  000a92c6: mov r5,r0
  000a92c8: ldr r0,[r4,#0x0]
  000a92ca: blx 0x00071eb4
  000a92ce: ldr r0,[r0,#0x0]
  000a92d0: ldr r1,[r5,#0x4]
  000a92d2: add.w r0,r1,r0, lsl #0x2
  000a92d6: ldr.w r0,[r0,#-0xc]
  000a92da: ldr r0,[r0,#0x8]
  000a92dc: ldr r1,[r0,#0xc]
  000a92de: mov r0,r9
  000a92e0: blx 0x00072088
  000a92e4: ldr r2,[r4,#0x58]
  000a92e6: movs r5,#0x0
  000a92e8: str r5,[sp,#0x0]
  000a92ea: asrs r3,r2,#0x1f
  000a92ec: blx 0x0006f7cc
  000a92f0: ldr r0,[r4,#0x28]
  000a92f2: cbz r0,0x000a9308
  000a92f4: ldr r1,[r0,#0xc]
  000a92f6: ldr.w r0,[r11,#0x0]
  000a92fa: blx 0x00072088
  000a92fe: ldr r2,[r4,#0x58]
  000a9300: str r5,[sp,#0x0]
  000a9302: asrs r3,r2,#0x1f
  000a9304: blx 0x0006f7cc
  000a9308: ldr r0,[r4,#0x2c]
  000a930a: ldr r1,[r0,#0xc]
  000a930c: ldr.w r0,[r11,#0x0]
  000a9310: blx 0x00072088
  000a9314: ldr r2,[r4,#0x58]
  000a9316: str r5,[sp,#0x0]
  000a9318: asrs r3,r2,#0x1f
  000a931a: blx 0x0006f7cc
  000a931e: ldr r1,[r4,#0x28]
  000a9320: ldr r0,[r4,#0x58]
  000a9322: ldrd r2,r3,[r4,#0x7c]
  000a9326: add r2,r0
  000a9328: add r0,r3
  000a932a: cmp.w r2,#0x3e8
  000a932e: strd r2,r0,[r4,#0x7c]
  000a9332: ble 0x000a9378
  000a9334: cbz r1,0x000a9378
  000a9336: ldr r0,[0x000a9658]
  000a9338: movs r1,#0x0
  000a933a: str r1,[r4,#0x7c]
  000a933c: movs r1,#0x64
  000a933e: add r0,pc
  000a9340: ldr r0,[r0,#0x0]
  000a9342: ldr r0,[r0,#0x0]
  000a9344: blx 0x00071848
  000a9348: cmp r0,#0x27
  000a934a: bgt 0x000a9374
  000a934c: ldr r0,[r4,#0x28]
  000a934e: ldr r1,[r0,#0xc]
  000a9350: ldr.w r0,[r11,#0x0]
  000a9354: blx 0x00072088
  000a9358: movs r1,#0x3
  000a935a: movs r2,#0x0
  000a935c: blx 0x0006fd18
  000a9360: ldr r1,[r4,#0x28]
  000a9362: ldr.w r0,[r11,#0x0]
  000a9366: ldr r1,[r1,#0xc]
  000a9368: blx 0x00072088
  000a936c: movs r1,#0x1
  000a936e: movs r2,#0x0
  000a9370: blx 0x0006fd18
  000a9374: ldr.w r0,[r4,#0x80]
  000a9378: cmp.w r0,#0x7d0
  000a937c: ble.w 0x000a95ac
  000a9380: ldr r0,[0x000a965c]
  000a9382: movs r1,#0x0
  000a9384: str.w r1,[r4,#0x80]
  000a9388: movs r1,#0x64
  000a938a: add r0,pc
  000a938c: ldr r0,[r0,#0x0]
  000a938e: ldr r0,[r0,#0x0]
  000a9390: blx 0x00071848
  000a9394: cmp r0,#0x1d
  000a9396: bgt.w 0x000a95ac
  000a939a: ldr r0,[r4,#0x2c]
  000a939c: ldr r1,[r0,#0xc]
  000a939e: ldr.w r0,[r11,#0x0]
  000a93a2: blx 0x00072088
  000a93a6: movs r1,#0x3
  000a93a8: movs r2,#0x0
  000a93aa: blx 0x0006fd18
  000a93ae: ldr r1,[r4,#0x2c]
  000a93b0: ldr.w r0,[r11,#0x0]
  000a93b4: ldr r1,[r1,#0xc]
  000a93b6: blx 0x00072088
  000a93ba: ldr r1,[sp,#0x88]
  000a93bc: ldr.w r2,[r8,#0x0]
  000a93c0: subs r1,r2,r1
  000a93c2: itttt eq
  000a93c4: mov.eq r1,#0x1
  000a93c6: mov.eq r2,#0x0
  000a93c8: add.eq sp,#0x8c
  000a93ca: pop.eq.w {r8,r9,r10,r11}
  000a93ce: itt eq
  000a93d0: pop.eq.w {r4,r5,r6,r7,lr}
  000a93d4: b.eq.w 0x001ab9c8
  000a93d8: blx 0x0006e824
  000a93dc: ldr r0,[r5,#0x0]
  000a93de: blx 0x00071a10
  000a93e2: blx 0x00071950
  000a93e6: orr r6,r0,#0x2
  000a93ea: ldr r0,[r4,#0x58]
  000a93ec: ldr.w r1,[r4,#0x84]
  000a93f0: add r0,r1
  000a93f2: str.w r0,[r4,#0x84]
  000a93f6: ldr r0,[r5,#0x0]
  000a93f8: blx 0x00071a10
  000a93fc: blx 0x00071950
  000a9400: cmp r0,#0x1
  000a9402: bne 0x000a9424
  000a9404: ldr r0,[0x000a9640]
  000a9406: mov.w r2,#0x3f800000
  000a940a: ldr r1,[0x000a9638]
  000a940c: add r0,pc
  000a940e: ldr r3,[0x000a963c]
  000a9410: ldr r0,[r0,#0x0]
  000a9412: ldr r0,[r0,#0x0]
  000a9414: strd r2,r1,[sp,#0x0]
  000a9418: movw r1,#0x2601
  000a941c: movs r2,#0x0
  000a941e: blx 0x00071ff8
  000a9422: b 0x000a950a
  000a9424: cmp r6,#0x2
  000a9426: bne 0x000a950a
  000a9428: ldr r1,[r4,#0x38]
  000a942a: cmp r1,#0x0
  000a942c: beq 0x000a950a
  000a942e: ldr r0,[0x000a9648]
  000a9430: mov.w r9,#0x0
  000a9434: movs r6,#0x0
  000a9436: add r0,pc
  000a9438: ldr r5,[r0,#0x0]
  000a943a: b 0x000a945a
  000a943c: ldr r0,[r1,#0x4]
  000a943e: ldr.w r1,[r0,r6,lsl #0x2]
  000a9442: ldr r0,[r5,#0x0]
  000a9444: ldr r1,[r1,#0xc]
  000a9446: blx 0x00072088
  000a944a: ldr r2,[r4,#0x58]
  000a944c: str.w r9,[sp,#0x0]
  000a9450: asrs r3,r2,#0x1f
  000a9452: blx 0x0006f7cc
  000a9456: ldr r1,[r4,#0x38]
  000a9458: adds r6,#0x1
  000a945a: ldr r0,[r1,#0x0]
  000a945c: cmp r6,r0
  000a945e: bcc 0x000a943c
  000a9460: ldr.w r1,[r4,#0x84]
  000a9464: movw r2,#0xbb8
  000a9468: cmp r1,r2
  000a946a: ble 0x000a950a
  000a946c: movs r6,#0x0
  000a946e: str.w r6,[r4,#0x84]
  000a9472: ldr r1,[0x000a964c]
  000a9474: add r1,pc
  000a9476: ldr.w r9,[r1,#0x0]
  000a947a: ldr r1,[0x000a9650]
  000a947c: add r1,pc
  000a947e: ldr.w r10,[r1,#0x0]
  000a9482: ldr r1,[0x000a9654]
  000a9484: add r1,pc
  000a9486: ldr.w r11,[r1,#0x0]
  000a948a: b 0x000a9506
  000a948c: ldr.w r0,[r9,#0x0]
  000a9490: movs r1,#0x64
  000a9492: blx 0x00071848
  000a9496: cmp r0,#0x13
  000a9498: bgt 0x000a9500
  000a949a: ldr r0,[r4,#0x38]
  000a949c: ldr r0,[r0,#0x4]
  000a949e: ldr.w r1,[r0,r6,lsl #0x2]
  000a94a2: ldr r0,[r5,#0x0]
  000a94a4: ldr r1,[r1,#0xc]
  000a94a6: blx 0x00072088
  000a94aa: blx 0x000721b4
  000a94ae: cbnz r0,0x000a9500
  000a94b0: ldr r0,[r4,#0x38]
  000a94b2: ldr r0,[r0,#0x4]
  000a94b4: ldr.w r1,[r0,r6,lsl #0x2]
  000a94b8: ldr r0,[r5,#0x0]
  000a94ba: ldr r1,[r1,#0xc]
  000a94bc: blx r10
  000a94be: movs r1,#0x3
  000a94c0: movs r2,#0x0
  000a94c2: blx r11
  000a94c4: ldr r0,[r4,#0x38]
  000a94c6: ldr r0,[r0,#0x4]
  000a94c8: ldr.w r0,[r0,r6,lsl #0x2]
  000a94cc: ldr r1,[r0,#0xc]
  000a94ce: ldr r0,[r5,#0x0]
  000a94d0: blx r10
  000a94d2: movs r1,#0x1
  000a94d4: movs r2,#0x0
  000a94d6: blx r11
  000a94d8: ldr r0,[r4,#0x38]
  000a94da: ldr r0,[r0,#0x4]
  000a94dc: ldr.w r0,[r0,r6,lsl #0x2]
  000a94e0: ldr r1,[r0,#0x14]
  000a94e2: ldr r0,[r5,#0x0]
  000a94e4: blx r10
  000a94e6: movs r1,#0x3
  000a94e8: movs r2,#0x0
  000a94ea: blx r11
  000a94ec: ldr r0,[r4,#0x38]
  000a94ee: ldr r0,[r0,#0x4]
  000a94f0: ldr.w r1,[r0,r6,lsl #0x2]
  000a94f4: ldr r0,[r5,#0x0]
  000a94f6: ldr r1,[r1,#0x14]
  000a94f8: blx r10
  000a94fa: movs r1,#0x1
  000a94fc: movs r2,#0x0
  000a94fe: blx r11
  000a9500: ldr r0,[r4,#0x38]
  000a9502: adds r6,#0x1
  000a9504: ldr r0,[r0,#0x0]
  000a9506: cmp r6,r0
  000a9508: bcc 0x000a948c
  000a950a: ldr r0,[0x000a9644]
  000a950c: add r0,pc
  000a950e: ldr.w r11,[r0,#0x0]
  000a9512: ldr r0,[r4,#0x0]
  000a9514: ldr.w r5,[r11,#0x0]
  000a9518: blx 0x00071eb4
  000a951c: ldr r0,[r0,#0x4]
  000a951e: ldr r0,[r0,#0x0]
  000a9520: ldr r0,[r0,#0x8]
  000a9522: ldr r1,[r0,#0xc]
  000a9524: mov r0,r5
  000a9526: blx 0x000720c4
  000a952a: add.w r9,sp,#0x48
  000a952e: mov r1,r0
  000a9530: movs r2,#0x3c
  000a9532: mov r0,r9
  000a9534: blx 0x0006f1e4
  000a9538: ldr r3,[r4,#0x4]
  000a953a: add r0,sp,#0xc
  000a953c: mov.w r10,#0x0
  000a9540: mov r1,r9
  000a9542: movs r2,#0x0
  000a9544: str.w r10,[sp,#0x0]
  000a9548: blx 0x00072094
  000a954c: ldr r0,[r4,#0x0]
  000a954e: ldr.w r5,[r11,#0x0]
  000a9552: blx 0x00071eb4
  000a9556: ldr r0,[r0,#0x4]
  000a9558: mov r2,r9
  000a955a: ldr r0,[r0,#0x0]
  000a955c: ldr r0,[r0,#0x8]
  000a955e: ldr r1,[r0,#0xc]
  000a9560: mov r0,r5
  000a9562: blx 0x000721c0
  000a9566: movs r6,#0x0
  000a9568: b 0x000a95a0
  000a956a: ldr r0,[r4,#0x0]
  000a956c: blx 0x00071eb4
  000a9570: ldr r0,[r0,#0x4]
  000a9572: ldr.w r0,[r0,r6,lsl #0x2]
  000a9576: cbz r0,0x000a959e
  000a9578: ldr r0,[r4,#0x0]
  000a957a: ldr.w r5,[r11,#0x0]
  000a957e: blx 0x00071eb4
  000a9582: ldr r0,[r0,#0x4]
  000a9584: ldr.w r0,[r0,r6,lsl #0x2]
  000a9588: ldr r0,[r0,#0x8]
  000a958a: ldr r1,[r0,#0xc]
  000a958c: mov r0,r5
  000a958e: blx 0x00072088
  000a9592: ldr r2,[r4,#0x58]
  000a9594: str.w r10,[sp,#0x0]
  000a9598: asrs r3,r2,#0x1f
  000a959a: blx 0x0006f7cc
  000a959e: adds r6,#0x1
  000a95a0: ldr r0,[r4,#0x0]
  000a95a2: blx 0x00071eb4
  000a95a6: ldr r0,[r0,#0x0]
  000a95a8: cmp r6,r0
  000a95aa: bcc 0x000a956a
  000a95ac: ldr r0,[sp,#0x88]
  000a95ae: ldr.w r1,[r8,#0x0]
  000a95b2: subs r0,r1,r0
  000a95b4: ittt eq
  000a95b6: add.eq sp,#0x8c
  000a95b8: pop.eq.w {r8,r9,r10,r11}
  000a95bc: pop.eq {r4,r5,r6,r7,pc}
  000a95be: blx 0x0006e824
  000a95c2: ldr r0,[r5,#0x0]
  000a95c4: blx 0x00071a10
  000a95c8: blx 0x00071950
  000a95cc: cmp r0,#0x2
  000a95ce: bne 0x000a95ac
  000a95d0: ldr r1,[0x000a9660]
  000a95d2: ldr r0,[r4,#0x0]
  000a95d4: add r1,pc
  000a95d6: ldr.w r9,[r11,#0x0]
  000a95da: ldr r5,[r1,#0x0]
  000a95dc: blx r5
  000a95de: mov r6,r0
  000a95e0: ldr r0,[r4,#0x0]
  000a95e2: blx r5
  000a95e4: ldr r0,[r0,#0x0]
  000a95e6: ldr r1,[r6,#0x4]
  000a95e8: add.w r0,r1,r0, lsl #0x2
  000a95ec: ldr.w r0,[r0,#-0x8]
  000a95f0: ldr r0,[r0,#0x8]
  000a95f2: ldr r1,[r0,#0xc]
  000a95f4: mov r0,r9
  000a95f6: blx 0x00072088
  000a95fa: ldr r2,[r4,#0x58]
  000a95fc: mov.w r10,#0x0
  000a9600: str.w r10,[sp,#0x0]
  000a9604: asrs r3,r2,#0x1f
  000a9606: blx 0x0006f7cc
  000a960a: ldr r0,[r4,#0x0]
  000a960c: ldr.w r9,[r11,#0x0]
  000a9610: blx r5
  000a9612: mov r6,r0
  000a9614: ldr r0,[r4,#0x0]
  000a9616: blx r5
  000a9618: ldr r0,[r0,#0x0]
  000a961a: ldr r1,[r6,#0x4]
  000a961c: add.w r0,r1,r0, lsl #0x2
  000a9620: ldr.w r0,[r0,#-0x4]
  000a9624: ldr r0,[r0,#0x8]
  000a9626: ldr r1,[r0,#0xc]
  000a9628: mov r0,r9
  000a962a: blx 0x00072088
  000a962e: ldr r2,[r4,#0x58]
  000a9630: str.w r10,[sp,#0x0]
  000a9634: b 0x000a925e
  001ab9c8: bx pc
```
