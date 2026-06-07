# SpaceLounge::updateScreenPositions
elf 0x170478 body 650
Sig: undefined __thiscall updateScreenPositions(SpaceLounge * this)

## decompile
```c

/* SpaceLounge::updateScreenPositions() */

void __thiscall SpaceLounge::updateScreenPositions(SpaceLounge *this)

{
  undefined8 uVar1;
  undefined8 uVar2;
  float fVar3;
  int iVar4;
  uint *puVar5;
  undefined4 *puVar6;
  code *pcVar7;
  undefined4 uVar8;
  int *piVar9;
  int iVar10;
  int *piVar11;
  uint uVar12;
  uint uVar13;
  float fVar14;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  Matrix aMStack_148 [60];
  Matrix aMStack_10c [12];
  float local_100 [2];
  float local_f8;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 uStack_e4;
  undefined4 uStack_e0;
  undefined4 uStack_dc;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined4 local_c8;
  Vector aVStack_c0 [64];
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 local_68;
  float local_64;
  undefined4 local_60;
  int local_5c;
  
  piVar11 = *(int **)(DAT_00180728 + 0x18048e);
  local_5c = *piVar11;
  if (*(int *)(this + 0x24) != 0) {
    iVar4 = Level::getEnemies();
    local_68 = 0;
    local_64 = 0.0;
    local_60 = 0;
    local_70 = 0;
    uStack_6c = 0;
    puVar5 = *(uint **)(DAT_0018072c + 0x1804b2);
    local_80 = 0;
    uStack_7c = 0;
    local_78 = 0;
    uStack_74 = 0;
    uVar12 = *puVar5;
    AbyssEngine::PaintCanvas::CameraGetCurrent();
    AbyssEngine::PaintCanvas::CameraGetLocal(uVar12);
    fVar14 = (float)AbyssEngine::AEMath::MatrixGetRight((Matrix *)local_100);
    AbyssEngine::AEMath::operator*(fVar14,aVStack_c0);
    AbyssEngine::AEMath::Vector::operator=((Vector *)&local_80,aVStack_c0);
    fVar3 = DAT_00180720;
    fVar14 = DAT_0018071c;
    uVar2 = DAT_00180710;
    uVar1 = DAT_00180708;
    puVar6 = (undefined4 *)((uint)local_100 | 4);
    uVar15 = 0;
    uVar16 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar17 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar18 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    pcVar7 = *(code **)(DAT_00180730 + 0x180524);
    for (uVar12 = 0; uVar12 < **(uint **)(this + 0x24); uVar12 = uVar12 + 1) {
      (**(code **)(**(int **)(*(int *)(iVar4 + 4) + uVar12 * 4) + 0x28))(aVStack_c0);
      (*pcVar7)((Vector *)&uStack_74,aVStack_c0);
      AbyssEngine::AEMath::operator-(aVStack_c0,(Vector *)&uStack_74);
      (*pcVar7)((Vector *)&local_68,aVStack_c0);
      AbyssEngine::PaintCanvas::GetScreenPosition((Vector *)*puVar5,(Vector *)&local_68);
      AbyssEngine::AEMath::operator+(aVStack_c0,(Vector *)&uStack_74);
      (*pcVar7)((Vector *)&local_68,aVStack_c0);
      local_64 = local_64 + fVar14;
      AbyssEngine::PaintCanvas::GetScreenPosition((Vector *)*puVar5,(Vector *)&local_68);
      uVar13 = *puVar5;
      AbyssEngine::PaintCanvas::CameraGetCurrent();
      uVar8 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar13);
      __aeabi_memcpy(aVStack_c0,uVar8,0x3c);
      AbyssEngine::AEMath::MatrixGetPosition(aMStack_148);
      AbyssEngine::AEMath::MatrixGetUp(aMStack_10c);
      AbyssEngine::AEMath::MatrixGetLookAt
                ((AEMath *)local_100,(Vector *)aMStack_148,(Vector *)&uStack_74,
                 (Vector *)aMStack_10c);
      AbyssEngine::AEMath::Matrix::operator=((Matrix *)aVStack_c0,(Matrix *)local_100);
      AEGeometry::setMatrix
                (*(Matrix **)
                  (*(int *)(*(int *)(iVar4 + 4) + (**(int **)(this + 0x24) + uVar12) * 4) + 8));
      (*pcVar7)(this + 0x4c,(Vector *)&uStack_74);
      AbyssEngine::AEMath::MatrixGetDir((Matrix *)local_100);
      *(float *)(this + 0x4c) = *(float *)(this + 0x4c) - local_100[0] * fVar3;
      AbyssEngine::AEMath::MatrixGetDir((Matrix *)local_100);
      *(float *)(this + 0x54) = *(float *)(this + 0x54) - local_f8 * fVar3;
      piVar9 = *(int **)(*(int *)(iVar4 + 4) + (**(int **)(this + 0x24) + uVar12) * 4);
      (**(code **)(*piVar9 + 0x44))(piVar9,this + 0x4c);
      Status::getSystem();
      iVar10 = SolarSystem::getRace();
      if (iVar10 == 0) {
        local_100[0] = 1.0;
        *puVar6 = uVar15;
        puVar6[1] = uVar16;
        puVar6[2] = uVar17;
        puVar6[3] = uVar18;
        local_ec = 0x3f800000;
        uStack_d8 = uVar1;
        uStack_d0 = uVar2;
        local_c8 = 0x3f800000;
        local_e8 = uVar15;
        uStack_e4 = uVar16;
        uStack_e0 = uVar17;
        uStack_dc = uVar18;
        AbyssEngine::AEMath::MatrixSetRotation(aMStack_148,extraout_s0,extraout_s1,extraout_s2);
        AbyssEngine::AEMath::Matrix::operator*=((Matrix *)aVStack_c0,(Matrix *)local_100);
      }
      AEGeometry::setMatrix(*(Matrix **)(*(int *)(*(int *)(iVar4 + 4) + uVar12 * 4) + 8));
      piVar9 = *(int **)(*(int *)(iVar4 + 4) + uVar12 * 4);
      (**(code **)(*piVar9 + 0x44))(piVar9,(Vector *)&uStack_74);
    }
  }
  if (*piVar11 == local_5c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00180478: push {r4,r5,r6,r7,lr}
  0018047a: add r7,sp,#0xc
  0018047c: push {r7,r8,r9,r10,r11}
  00180480: vpush {d8,d9,d10,d11,d12,d13}
  00180484: sub sp,#0x118
  00180486: mov r4,r0
  00180488: ldr r0,[0x00180728]
  0018048a: add r0,pc
  0018048c: ldr r1,[r0,#0x0]
  0018048e: ldr r0,[r1,#0x0]
  00180490: str r0,[sp,#0x114]
  00180492: ldr r0,[r4,#0x24]
  00180494: cmp r0,#0x0
  00180496: beq.w 0x001806e6
  0018049a: ldr r0,[r4,#0x44]
  0018049c: str r1,[sp,#0x4]
  0018049e: ldr r0,[r0,#0x0]
  001804a0: blx 0x00071eb4
  001804a4: str r0,[sp,#0x20]
  001804a6: movs r1,#0x0
  001804a8: ldr r0,[0x0018072c]
  001804aa: strd r1,r1,[sp,#0x108]
  001804ae: add r0,pc
  001804b0: str r1,[sp,#0x110]
  001804b2: strd r1,r1,[sp,#0x100]
  001804b6: ldr r0,[r0,#0x0]
  001804b8: strd r1,r1,[sp,#0xf0]
  001804bc: strd r1,r1,[sp,#0xf8]
  001804c0: ldr r6,[r0,#0x0]
  001804c2: str r0,[sp,#0x18]
  001804c4: mov r0,r6
  001804c6: blx 0x000717f4
  001804ca: mov r1,r0
  001804cc: mov r0,r6
  001804ce: blx 0x0006ff1c
  001804d2: add r5,sp,#0x70
  001804d4: mov r1,r0
  001804d6: str r4,[sp,#0x24]
  001804d8: mov r0,r5
  001804da: blx 0x0006f4e4
  001804de: add r4,sp,#0xb0
  001804e0: ldr r1,[0x00180718]
  001804e2: mov r2,r5
  001804e4: mov r0,r4
  001804e6: blx 0x00072658
  001804ea: add r0,sp,#0xf0
  001804ec: mov r1,r4
  001804ee: blx 0x0006eb3c
  001804f2: add.w r0,r5,#0x28
  001804f6: str r0,[sp,#0x10]
  001804f8: add.w r0,r5,#0x18
  001804fc: ldr.w r11,[sp,#0x24]
  00180500: str r0,[sp,#0xc]
  00180502: orr r0,r5,#0x4
  00180506: str r0,[sp,#0x8]
  00180508: add.w r0,r11,#0x4c
  0018050c: str r0,[sp,#0x14]
  0018050e: vmov.i32 q6,#0x0
  00180512: ldr r0,[0x00180730]
  00180514: add.w r8,sp,#0xfc
  00180518: vldr.32 s16,[pc,#0x200]
  0018051c: mov.w r9,#0x0
  00180520: add r0,pc
  00180522: vldr.32 s18,[pc,#0x1fc]
  00180526: ldr r0,[r0,#0x0]
  00180528: str r0,[sp,#0x1c]
  0018052a: adr r0,[0x180708]
  0018052c: vld1.64 {d10,d11},[r0]
  00180530: b 0x001806d8
  00180532: ldr r0,[sp,#0x20]
  00180534: ldr r0,[r0,#0x4]
  00180536: ldr.w r1,[r0,r9,lsl #0x2]
  0018053a: ldr r0,[r1,#0x0]
  0018053c: ldr r2,[r0,#0x28]
  0018053e: mov r0,r4
  00180540: blx r2
  00180542: ldr r6,[sp,#0x1c]
  00180544: mov r0,r8
  00180546: mov r1,r4
  00180548: blx r6
  0018054a: mov r5,r6
  0018054c: add r6,sp,#0xf0
  0018054e: mov r0,r4
  00180550: mov r1,r8
  00180552: mov r2,r6
  00180554: blx 0x0006ec38
  00180558: add.w r10,sp,#0x108
  0018055c: mov r1,r4
  0018055e: mov r0,r10
  00180560: blx r5
  00180562: ldr.w r0,[r11,#0x40]
  00180566: mov r6,r5
  00180568: ldr r5,[sp,#0x18]
  0018056a: mov r1,r10
  0018056c: mov r11,r8
  0018056e: ldr r0,[r0,#0x4]
  00180570: ldr.w r2,[r0,r9,lsl #0x3]
  00180574: ldr r0,[r5,#0x0]
  00180576: blx 0x000729c4
  0018057a: add r2,sp,#0xf0
  0018057c: mov r0,r4
  0018057e: mov r1,r8
  00180580: blx 0x0006f1cc
  00180584: mov r0,r10
  00180586: mov r1,r4
  00180588: blx r6
  0018058a: vldr.32 s0,[sp,#0x10c]
  0018058e: mov r1,r10
  00180590: ldr r0,[sp,#0x24]
  00180592: vadd.f32 s0,s0,s16
  00180596: vstr.32 s0,[sp,#0x10c]
  0018059a: ldr r0,[r0,#0x40]
  0018059c: ldr r0,[r0,#0x4]
  0018059e: add.w r0,r0,r9, lsl #0x3
  001805a2: ldr r2,[r0,#0x4]
  001805a4: ldr r0,[r5,#0x0]
  001805a6: blx 0x000729c4
  001805aa: ldr.w r10,[r5,#0x0]
  001805ae: mov r0,r10
  001805b0: blx 0x000717f4
  001805b4: mov r1,r0
  001805b6: mov r0,r10
  001805b8: ldr.w r10,[sp,#0x20]
  001805bc: blx 0x0006ff1c
  001805c0: mov r1,r0
  001805c2: mov r0,r4
  001805c4: movs r2,#0x3c
  001805c6: blx 0x0006f1e4
  001805ca: add r6,sp,#0x28
  001805cc: mov r1,r4
  001805ce: mov r0,r6
  001805d0: blx 0x0006f16c
  001805d4: add.w r8,sp,#0x64
  001805d8: mov r1,r4
  001805da: mov r0,r8
  001805dc: blx 0x0006f4d8
  001805e0: add r5,sp,#0x70
  001805e2: mov r3,r8
  001805e4: mov r1,r6
  001805e6: mov r2,r11
  001805e8: mov r0,r5
  001805ea: mov r8,r11
  001805ec: blx 0x000725e0
  001805f0: mov r0,r4
  001805f2: mov r1,r5
  001805f4: blx 0x0006f148
  001805f8: ldr r0,[sp,#0x24]
  001805fa: ldr.w r1,[r10,#0x4]
  001805fe: ldr r0,[r0,#0x24]
  00180600: ldr r0,[r0,#0x0]
  00180602: add r0,r9
  00180604: ldr.w r0,[r1,r0,lsl #0x2]
  00180608: mov r1,r4
  0018060a: ldr r0,[r0,#0x8]
  0018060c: blx 0x000721e4
  00180610: ldr r6,[sp,#0x14]
  00180612: mov r1,r11
  00180614: ldr r2,[sp,#0x1c]
  00180616: mov r0,r6
  00180618: blx r2
  0018061a: mov r0,r5
  0018061c: mov r1,r4
  0018061e: blx 0x0006f4c0
  00180622: ldr r0,[sp,#0x24]
  00180624: mov r1,r4
  00180626: vldr.32 s0,[sp,#0x70]
  0018062a: ldr.w r11,[sp,#0x24]
  0018062e: vldr.32 s2,[r0,#0x4c]
  00180632: vmls.f32 s2,s0,s18
  00180636: ldr r0,[sp,#0x24]
  00180638: vstr.32 s2,[r0,#0x4c]
  0018063c: mov r0,r5
  0018063e: blx 0x0006f4c0
  00180642: vldr.32 s0,[sp,#0x78]
  00180646: vldr.32 s2,[r11,#0x54]
  0018064a: vmls.f32 s2,s0,s18
  0018064e: vstr.32 s2,[r11,#0x54]
  00180652: ldr.w r0,[r11,#0x24]
  00180656: ldr.w r1,[r10,#0x4]
  0018065a: ldr r0,[r0,#0x0]
  0018065c: add r0,r9
  0018065e: ldr.w r0,[r1,r0,lsl #0x2]
  00180662: ldr r1,[r0,#0x0]
  00180664: ldr r2,[r1,#0x44]
  00180666: mov r1,r6
  00180668: blx r2
  0018066a: ldr r0,[0x00180734]
  0018066c: add r0,pc
  0018066e: ldr r0,[r0,#0x0]
  00180670: ldr r0,[r0,#0x0]
  00180672: blx 0x00071a10
  00180676: blx 0x00071950
  0018067a: cbnz r0,0x001806b6
  0018067c: ldr r0,[sp,#0x8]
  0018067e: mov.w r1,#0x3f800000
  00180682: str r1,[sp,#0x70]
  00180684: add r5,sp,#0x70
  00180686: ldr r3,[0x00180724]
  00180688: movs r2,#0x0
  0018068a: vst1.32 {d12,d13},[r0]
  0018068e: ldr r0,[sp,#0xc]
  00180690: str r1,[sp,#0x84]
  00180692: vst1.64 {d12,d13},[r0]
  00180696: ldr r0,[sp,#0x10]
  00180698: vst1.32 {d10,d11},[r0]
  0018069c: movs r0,#0x0
  0018069e: str r0,[sp,#0x0]
  001806a0: add r0,sp,#0x28
  001806a2: str r1,[sp,#0xa8]
  001806a4: mov r1,r5
  001806a6: blx 0x00072094
  001806aa: mov r0,r4
  001806ac: mov r1,r5
  001806ae: ldr.w r11,[sp,#0x24]
  001806b2: blx 0x0006f4a8
  001806b6: ldr r6,[sp,#0x20]
  001806b8: mov r1,r4
  001806ba: ldr r0,[r6,#0x4]
  001806bc: ldr.w r0,[r0,r9,lsl #0x2]
  001806c0: ldr r0,[r0,#0x8]
  001806c2: blx 0x000721e4
  001806c6: ldr r0,[r6,#0x4]
  001806c8: ldr.w r0,[r0,r9,lsl #0x2]
  001806cc: ldr r1,[r0,#0x0]
  001806ce: ldr r2,[r1,#0x44]
  001806d0: mov r1,r8
  001806d2: blx r2
  001806d4: add.w r9,r9,#0x1
  001806d8: ldr.w r0,[r11,#0x24]
  001806dc: ldr r0,[r0,#0x0]
  001806de: cmp r9,r0
  001806e0: bcc.w 0x00180532
  001806e4: ldr r1,[sp,#0x4]
  001806e6: ldr r0,[sp,#0x114]
  001806e8: ldr r1,[r1,#0x0]
  001806ea: subs r0,r1,r0
  001806ec: itttt eq
  001806ee: add.eq sp,#0x118
  001806f0: vpop.eq {d8,d9,d10,d11,d12,d13}
  001806f4: add.eq sp,#0x4
  001806f6: pop.eq.w {r8,r9,r10,r11}
  001806fa: it eq
  001806fc: pop.eq {r4,r5,r6,r7,pc}
  001806fe: blx 0x0006e824
```
