# ModStation::resetIdleCamForHangar
elf 0xd8cf0 body 320
Sig: undefined __stdcall resetIdleCamForHangar(void)

## decompile
```c

/* ModStation::resetIdleCamForHangar() */

void ModStation::resetIdleCamForHangar(void)

{
  int in_r0;
  EaseInOut *pEVar1;
  Station *pSVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint *puVar6;
  int *piVar7;
  undefined4 in_s0;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  undefined4 in_s1;
  undefined8 uVar8;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s2;
  Matrix aMStack_58 [60];
  int local_1c;
  
  uVar8 = CONCAT44(in_s1,in_s0);
  piVar7 = *(int **)(DAT_000e8e44 + 0xe8d02);
  local_1c = *piVar7;
  if (*(int *)(in_r0 + 0x14) != 0) {
    uVar8 = CutScene::resetCamera();
  }
  if (*(int *)(in_r0 + 0x13c) == 0) {
    pEVar1 = operator_new(0x10);
    uVar8 = AbyssEngine::EaseInOut::EaseInOut(pEVar1,extraout_s0,extraout_s1);
    *(EaseInOut **)(in_r0 + 0x13c) = pEVar1;
  }
  else {
    uVar8 = AbyssEngine::EaseInOut::SetRange((float)uVar8,(float)((ulonglong)uVar8 >> 0x20));
  }
  if (*(int *)(in_r0 + 0x140) == 0) {
    pEVar1 = operator_new(0x10);
    uVar8 = AbyssEngine::EaseInOut::EaseInOut(pEVar1,extraout_s0_00,extraout_s1_00);
    *(EaseInOut **)(in_r0 + 0x140) = pEVar1;
  }
  else {
    uVar8 = AbyssEngine::EaseInOut::SetRange((float)uVar8,(float)((ulonglong)uVar8 >> 0x20));
  }
  if (*(int *)(in_r0 + 0x144) == 0) {
    pEVar1 = operator_new(0x10);
    AbyssEngine::EaseInOut::EaseInOut(pEVar1,extraout_s0_01,extraout_s1_01);
    *(EaseInOut **)(in_r0 + 0x144) = pEVar1;
  }
  else {
    AbyssEngine::EaseInOut::SetRange((float)uVar8,(float)((ulonglong)uVar8 >> 0x20));
  }
  puVar6 = *(uint **)(DAT_000e8e48 + 0xe8d90);
  uVar5 = *puVar6;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
  AbyssEngine::AEMath::MatrixSetTranslation
            (aMStack_58,*(float *)(in_r0 + 0x138),extraout_s1_02,extraout_s2);
  pSVar2 = (Station *)Status::getStation();
  iVar3 = Station::getIndex(pSVar2);
  if (iVar3 == 0x65) {
    iVar3 = 8;
  }
  else {
    pSVar2 = (Station *)Status::getStation();
    iVar3 = Station::getIndex(pSVar2);
    if (iVar3 == 100) {
      iVar3 = 7;
    }
    else {
      Status::getSystem();
      iVar3 = SolarSystem::getRace();
    }
  }
  uVar5 = *puVar6;
  AbyssEngine::PaintCanvas::CameraGetCurrent();
  uVar4 = AbyssEngine::PaintCanvas::CameraGetLocal(uVar5);
  AbyssEngine::AEMath::MatrixSetRotation
            (aMStack_58,uVar4,*(undefined4 *)(DAT_000e8e50 + 0xe8e06 + iVar3 * 4),
             *(undefined4 *)(DAT_000e8e54 + 0xe8e08 + iVar3 * 4),0,2);
  if (*piVar7 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e8cf0: push {r4,r5,r6,r7,lr}
  000e8cf2: add r7,sp,#0xc
  000e8cf4: push.w r8
  000e8cf8: sub sp,#0x48
  000e8cfa: mov r4,r0
  000e8cfc: ldr r0,[0x000e8e44]
  000e8cfe: add r0,pc
  000e8d00: ldr.w r8,[r0,#0x0]
  000e8d04: ldr.w r0,[r8,#0x0]
  000e8d08: str r0,[sp,#0x44]
  000e8d0a: ldr r0,[r4,#0x14]
  000e8d0c: cbz r0,0x000e8d12
  000e8d0e: blx 0x00072070
  000e8d12: ldr.w r0,[r4,#0x13c]
  000e8d16: cbz r0,0x000e8d24
  000e8d18: ldr.w r1,[r4,#0x130]
  000e8d1c: mov r2,r1
  000e8d1e: blx 0x0006f4fc
  000e8d22: b 0x000e8d3a
  000e8d24: movs r0,#0x10
  000e8d26: blx 0x0006eb24
  000e8d2a: ldr.w r1,[r4,#0x130]
  000e8d2e: mov r5,r0
  000e8d30: mov r2,r1
  000e8d32: blx 0x00074ecc
  000e8d36: str.w r5,[r4,#0x13c]
  000e8d3a: ldr.w r0,[r4,#0x140]
  000e8d3e: cbz r0,0x000e8d4c
  000e8d40: ldr.w r1,[r4,#0x134]
  000e8d44: mov r2,r1
  000e8d46: blx 0x0006f4fc
  000e8d4a: b 0x000e8d62
  000e8d4c: movs r0,#0x10
  000e8d4e: blx 0x0006eb24
  000e8d52: ldr.w r1,[r4,#0x134]
  000e8d56: mov r5,r0
  000e8d58: mov r2,r1
  000e8d5a: blx 0x00074ecc
  000e8d5e: str.w r5,[r4,#0x140]
  000e8d62: ldr.w r0,[r4,#0x144]
  000e8d66: cbz r0,0x000e8d74
  000e8d68: ldr.w r1,[r4,#0x138]
  000e8d6c: mov r2,r1
  000e8d6e: blx 0x0006f4fc
  000e8d72: b 0x000e8d8a
  000e8d74: movs r0,#0x10
  000e8d76: blx 0x0006eb24
  000e8d7a: ldr.w r1,[r4,#0x138]
  000e8d7e: mov r5,r0
  000e8d80: mov r2,r1
  000e8d82: blx 0x00074ecc
  000e8d86: str.w r5,[r4,#0x144]
  000e8d8a: ldr r0,[0x000e8e48]
  000e8d8c: add r0,pc
  000e8d8e: ldr r6,[r0,#0x0]
  000e8d90: ldr r5,[r6,#0x0]
  000e8d92: mov r0,r5
  000e8d94: blx 0x000717f4
  000e8d98: mov r1,r0
  000e8d9a: mov r0,r5
  000e8d9c: blx 0x0006ff1c
  000e8da0: ldrd r2,r3,[r4,#0x130]
  000e8da4: mov r1,r0
  000e8da6: vldr.32 s0,[r4,#0x138]
  000e8daa: add r0,sp,#0x8
  000e8dac: vstr.32 s0,[sp]
  000e8db0: blx 0x0006f820
  000e8db4: ldr r0,[0x000e8e4c]
  000e8db6: add r0,pc
  000e8db8: ldr r4,[r0,#0x0]
  000e8dba: ldr r0,[r4,#0x0]
  000e8dbc: blx 0x00071c14
  000e8dc0: blx 0x00071824
  000e8dc4: cmp r0,#0x65
  000e8dc6: bne 0x000e8dcc
  000e8dc8: movs r4,#0x8
  000e8dca: b 0x000e8dea
  000e8dcc: ldr r0,[r4,#0x0]
  000e8dce: blx 0x00071c14
  000e8dd2: blx 0x00071824
  000e8dd6: cmp r0,#0x64
  000e8dd8: bne 0x000e8dde
  000e8dda: movs r4,#0x7
  000e8ddc: b 0x000e8dea
  000e8dde: ldr r0,[r4,#0x0]
  000e8de0: blx 0x00071a10
  000e8de4: blx 0x00071950
  000e8de8: mov r4,r0
  000e8dea: ldr r5,[r6,#0x0]
  000e8dec: mov r0,r5
  000e8dee: blx 0x000717f4
  000e8df2: mov r1,r0
  000e8df4: mov r0,r5
  000e8df6: blx 0x0006ff1c
  000e8dfa: mov r1,r0
  000e8dfc: ldr r0,[0x000e8e50]
  000e8dfe: ldr r3,[0x000e8e54]
  000e8e00: movs r6,#0x0
  000e8e02: add r0,pc
  000e8e04: add r3,pc
  000e8e06: ldr.w r2,[r0,r4,lsl #0x2]
  000e8e0a: movs r0,#0x2
  000e8e0c: ldr.w r3,[r3,r4,lsl #0x2]
  000e8e10: strd r6,r0,[sp,#0x0]
  000e8e14: add r0,sp,#0x8
  000e8e16: blx 0x00073060
  000e8e1a: ldr r0,[sp,#0x44]
  000e8e1c: ldr.w r1,[r8,#0x0]
  000e8e20: subs r0,r1,r0
  000e8e22: ittt eq
  000e8e24: add.eq sp,#0x48
  000e8e26: pop.eq.w r8
  000e8e2a: pop.eq {r4,r5,r6,r7,pc}
  000e8e2c: blx 0x0006e824
```
