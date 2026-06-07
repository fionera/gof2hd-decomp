# ParticleSystemMesh::updateSingle
elf 0x186870 body 362
Sig: undefined __stdcall updateSingle(int param_1, float param_2)

## decompile
```c

/* ParticleSystemMesh::updateSingle(int, float) */

undefined1 * ParticleSystemMesh::updateSingle(int param_1,float param_2)

{
  int in_r1;
  float in_r2;
  int iVar1;
  code *pcVar2;
  int *piVar3;
  uint in_fpscr;
  undefined4 uVar4;
  float fVar5;
  float extraout_s1;
  float extraout_s1_00;
  float fVar6;
  float extraout_s2;
  float extraout_s2_00;
  float fVar7;
  undefined4 in_s14;
  undefined1 *local_70;
  undefined1 auStack_64 [12];
  undefined1 auStack_58 [12];
  Matrix aMStack_4c [12];
  undefined1 auStack_40 [8];
  undefined4 uStack_38;
  int local_34;
  
  piVar3 = *(int **)(DAT_001969dc + 0x19688a);
  _uStack_38 = CONCAT44(*piVar3,in_s14);
  iVar1 = (int)*(char *)(*(int *)(param_1 + 0x6c) + in_r1);
  local_70 = (undefined1 *)param_2;
  if ((int)((uint)*(byte *)(param_1 + 0x35) << 0x18) < 0) {
    updateTrailEdges(param_1,in_r1);
    if (*(int *)(*(int *)(param_1 + 0x68) + in_r1 * 4) == -2) {
      if (*(char *)(param_1 + 0x90) == '\0') goto LAB_001969c0;
      AbyssEngine::AEMath::MatrixGetRight(aMStack_4c);
      uVar4 = 0xbf800000;
      if (*(char *)(param_1 + 0x4c) == '\0') {
        uVar4 = 0x3f800000;
      }
      pcVar2 = *(code **)(DAT_001969e0 + 0x1968fa);
      (*pcVar2)(auStack_40,aMStack_4c,uVar4);
      AbyssEngine::AEMath::MatrixGetUp(aMStack_4c);
      uVar4 = VectorSignedToFloat(*(undefined4 *)
                                   (*(int *)(DAT_001969e4 + 0x196912) + iVar1 * 0xa0 + 0x44),
                                  (byte)(in_fpscr >> 0x16) & 3);
      (*pcVar2)(auStack_58,auStack_40,uVar4);
      local_70 = auStack_64;
      (*pcVar2)(local_70,aMStack_4c,uVar4);
      finishCurrentTrailParticle(param_1,iVar1);
    }
  }
  else {
    updateUsualEdges(param_1,in_r1);
  }
  fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(param_1 + 0x68) + in_r1 * 4),
                                     (byte)(in_fpscr >> 0x16) & 3);
  *(int *)(*(int *)(param_1 + 0x68) + in_r1 * 4) = (int)(fVar5 + in_r2);
  fVar5 = (float)updateSingleColor(param_1);
  if (*(int *)(*(int *)(DAT_001969e8 + 0x196978) + iVar1 * 0xa0 + 0x28) <
      *(int *)(*(int *)(param_1 + 0x68) + in_r1 * 4)) {
    *(undefined4 *)(*(int *)(param_1 + 0x68) + in_r1 * 4) = 0xffffffff;
    fVar7 = extraout_s2;
    fVar6 = extraout_s1;
    for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x9c) * 4; iVar1 = iVar1 + 1) {
      local_70 = (undefined1 *)0x0;
      fVar5 = (float)AbyssEngine::PaintCanvas::MeshSetPoint
                               (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),fVar5
                                ,fVar6,fVar7);
      fVar7 = extraout_s2_00;
      fVar6 = extraout_s1_00;
    }
  }
LAB_001969c0:
  if (*piVar3 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return local_70;
}

```

## target disasm
```
  00196870: push {r4,r5,r6,r7,lr}
  00196872: add r7,sp,#0xc
  00196874: push {r7,r8,r9,r10,r11}
  00196878: vpush {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  0019687c: mov r4,r0
  0019687e: ldr r0,[0x001969dc]
  00196880: vmov s16,r2
  00196884: mov r5,r1
  00196886: add r0,pc
  00196888: ldr.w r11,[r0,#0x0]
  0019688c: ldr.w r0,[r11,#0x0]
  00196890: str r0,[sp,#0x3c]
  00196892: ldr r0,[r4,#0x6c]
  00196894: ldrb.w r1,[r4,#0x35]
  00196898: vcvt.s32.f32 s0,s16
  0019689c: ldrsb r6,[r0,r5]
  0019689e: lsls r0,r1,#0x18
  001968a0: vmov r2,s0
  001968a4: bmi 0x001968b0
  001968a6: mov r0,r4
  001968a8: mov r1,r5
  001968aa: blx 0x00078ba4
  001968ae: b 0x00196946
  001968b0: mov r0,r4
  001968b2: mov r1,r5
  001968b4: blx 0x00078bb0
  001968b8: ldr r0,[r4,#0x68]
  001968ba: ldr.w r0,[r0,r5,lsl #0x2]
  001968be: adds r0,#0x2
  001968c0: bne 0x00196946
  001968c2: ldrb.w r0,[r4,#0x90]
  001968c6: cmp r0,#0x0
  001968c8: beq 0x001969c0
  001968ca: add.w r10,sp,#0x24
  001968ce: ldr r1,[r4,#0x18]
  001968d0: mov r0,r10
  001968d2: blx 0x0006f4e4
  001968d6: vmov.f32 s2,0x3f800000
  001968da: ldrb.w r0,[r4,#0x4c]
  001968de: vmov.f32 s0,0xbf800000
  001968e2: add.w r9,sp,#0x30
  001968e6: cmp r0,#0x0
  001968e8: mov r1,r10
  001968ea: it eq
  001968ec: vmov.eq.f32 s0,s2
  001968f0: ldr r0,[0x001969e0]
  001968f2: vmov r2,s0
  001968f6: add r0,pc
  001968f8: ldr.w r8,[r0,#0x0]
  001968fc: mov r0,r9
  001968fe: blx r8
  00196900: ldr r1,[r4,#0x18]
  00196902: add r0,sp,#0x24
  00196904: blx 0x0006f4d8
  00196908: ldr r0,[0x001969e4]
  0019690a: add.w r1,r6,r6, lsl #0x2
  0019690e: add r0,pc
  00196910: ldr r0,[r0,#0x0]
  00196912: add.w r0,r0,r1, lsl #0x5
  00196916: mov r1,r9
  00196918: vldr.32 s0,[r0,#0x44]
  0019691c: add r0,sp,#0x18
  0019691e: vcvt.f32.s32 s0,s0
  00196922: vmov r10,s0
  00196926: mov r2,r10
  00196928: blx r8
  0019692a: add.w r9,sp,#0xc
  0019692e: add r1,sp,#0x24
  00196930: mov r2,r10
  00196932: mov r0,r9
  00196934: blx r8
  00196936: add r3,sp,#0x18
  00196938: mov r0,r4
  0019693a: mov r1,r6
  0019693c: mov r2,r5
  0019693e: str.w r9,[sp,#0x0]
  00196942: blx 0x00078b80
  00196946: ldr r0,[r4,#0x68]
  00196948: add.w r1,r0,r5, lsl #0x2
  0019694c: vldr.32 s0,[r1]
  00196950: vcvt.f32.s32 s0,s0
  00196954: vadd.f32 s0,s0,s16
  00196958: vcvt.s32.f32 s0,s0
  0019695c: vmov r1,s0
  00196960: str.w r1,[r0,r5,lsl #0x2]
  00196964: mov r0,r4
  00196966: mov r1,r5
  00196968: blx 0x00078bbc
  0019696c: ldr r1,[0x001969e8]
  0019696e: add.w r2,r6,r6, lsl #0x2
  00196972: ldr r0,[r4,#0x68]
  00196974: add r1,pc
  00196976: ldr r1,[r1,#0x0]
  00196978: ldr.w r3,[r0,r5,lsl #0x2]
  0019697c: add.w r1,r1,r2, lsl #0x5
  00196980: ldr r1,[r1,#0x28]
  00196982: cmp r3,r1
  00196984: ble 0x001969c0
  00196986: ldr.w r2,[r4,#0x9c]
  0019698a: mov.w r3,#0xffffffff
  0019698e: ldr r1,[r4,#0x58]
  00196990: movs r6,#0x0
  00196992: str.w r3,[r0,r5,lsl #0x2]
  00196996: muls r2,r5
  00196998: movs r5,#0x0
  0019699a: add.w r8,r1,r2, lsl #0x2
  0019699e: b 0x001969b6
  001969a0: add.w r2,r8,r5
  001969a4: ldr r0,[r4,#0x8]
  001969a6: ldr r1,[r4,#0x54]
  001969a8: movs r3,#0x0
  001969aa: uxth r2,r2
  001969ac: strd r6,r6,[sp,#0x0]
  001969b0: blx 0x000777f4
  001969b4: adds r5,#0x1
  001969b6: ldr.w r0,[r4,#0x9c]
  001969ba: cmp.w r5,r0, lsl #0x2
  001969be: blt 0x001969a0
  001969c0: ldr r0,[sp,#0x3c]
  001969c2: ldr.w r1,[r11,#0x0]
  001969c6: subs r0,r1,r0
  001969c8: itttt eq
  001969ca: vpop.eq {d0,d1,d2,d3,d4,d5,d6,d7,d8}
  001969ce: add.eq sp,#0x4
  001969d0: pop.eq.w {r8,r9,r10,r11}
  001969d4: pop.eq {r4,r5,r6,r7,pc}
  001969d6: blx 0x0006e824
```
