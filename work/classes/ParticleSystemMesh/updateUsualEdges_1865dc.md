# ParticleSystemMesh::updateUsualEdges
elf 0x1865dc body 208
Sig: undefined __stdcall updateUsualEdges(int param_1, int param_2)

## decompile
```c

/* ParticleSystemMesh::updateUsualEdges(int, int) */

void ParticleSystemMesh::updateUsualEdges(int param_1,int param_2)

{
  undefined4 in_r2;
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  uint in_fpscr;
  float fVar5;
  float extraout_s1;
  float extraout_s1_00;
  float fVar6;
  Vector aVStack_40 [12];
  undefined4 local_34;
  float local_30;
  float local_2c;
  int local_28;
  
  piVar4 = *(int **)(DAT_001966b0 + 0x1965f4);
  local_28 = *piVar4;
  uVar2 = *(undefined4 *)(param_1 + 0x58);
  uVar3 = *(undefined4 *)(param_1 + 0x9c);
  local_34 = 0;
  local_30 = 0.0;
  local_2c = 0.0;
  if ((int)((uint)*(byte *)(param_1 + 0x36) << 0x1c) < 0) {
    fVar5 = (float)VectorSignedToFloat(in_r2,(byte)(in_fpscr >> 0x16) & 3);
    fVar5 = fVar5 * DAT_001966ac;
    fVar6 = *(float *)(*(int *)(param_1 + 100) + param_2 * 0xc + 4);
  }
  else {
    fVar5 = (float)VectorSignedToFloat(in_r2,(byte)(in_fpscr >> 0x16) & 3);
    fVar6 = DAT_001966ac;
  }
  AbyssEngine::AEMath::operator*(aVStack_40,fVar5 * fVar6);
  AbyssEngine::AEMath::Vector::operator=((Vector *)&local_34,aVStack_40);
  fVar6 = extraout_s1;
  for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x9c) * 4; iVar1 = iVar1 + 1) {
    AbyssEngine::PaintCanvas::MeshTranslatePoint
              (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),
               (short)uVar2 + (short)uVar3 * (short)param_2 * 4 + (short)iVar1,local_30,fVar6,
               local_2c);
    fVar6 = extraout_s1_00;
  }
  if (*piVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001965dc: push {r4,r5,r6,r7,lr}
  001965de: add r7,sp,#0xc
  001965e0: push {r8,r9,r10,r11}
  001965e4: sub sp,#0x24
  001965e6: mov r4,r0
  001965e8: ldr r0,[0x001966b0]
  001965ea: vmov s0,r2
  001965ee: mov r5,r1
  001965f0: add r0,pc
  001965f2: ldr.w r11,[r0,#0x0]
  001965f6: ldr.w r0,[r11,#0x0]
  001965fa: str r0,[sp,#0x20]
  001965fc: movs r0,#0x0
  001965fe: ldr.w r9,[r4,#0x58]
  00196602: ldr.w r10,[r4,#0x9c]
  00196606: strd r0,r0,[sp,#0x14]
  0019660a: str r0,[sp,#0x1c]
  0019660c: ldrb.w r0,[r4,#0x36]
  00196610: lsls r0,r0,#0x1c
  00196612: bmi 0x00196628
  00196614: vcvt.f32.s32 s0,s0
  00196618: vldr.32 s2,[pc,#0x90]
  0019661c: ldr r0,[r4,#0x64]
  0019661e: add.w r1,r5,r5, lsl #0x1
  00196622: add.w r1,r0,r1, lsl #0x2
  00196626: b 0x00196646
  00196628: vcvt.f32.s32 s0,s0
  0019662c: vldr.32 s2,[pc,#0x7c]
  00196630: ldr r0,[r4,#0x64]
  00196632: add.w r1,r5,r5, lsl #0x1
  00196636: add.w r0,r0,r1, lsl #0x2
  0019663a: add.w r1,r4,#0x1c
  0019663e: vmul.f32 s0,s0,s2
  00196642: vldr.32 s2,[r0,#0x4]
  00196646: vmul.f32 s0,s0,s2
  0019664a: vmov r2,s0
  0019664e: add.w r8,sp,#0x8
  00196652: mov r0,r8
  00196654: blx 0x0006ec74
  00196658: add r0,sp,#0x14
  0019665a: mov r1,r8
  0019665c: blx 0x0006eb3c
  00196660: mul r0,r10,r5
  00196664: movs r6,#0x0
  00196666: add.w r5,r9,r0, lsl #0x2
  0019666a: b 0x0019668c
  0019666c: adds r2,r5,r6
  0019666e: ldr r3,[sp,#0x14]
  00196670: ldr r0,[r4,#0x8]
  00196672: ldr r1,[r4,#0x54]
  00196674: uxth r2,r2
  00196676: vldr.32 s0,[sp,#0x18]
  0019667a: vldr.32 s2,[sp,#0x1c]
  0019667e: vstr.32 s0,[sp]
  00196682: vstr.32 s2,[sp,#0x4]
  00196686: blx 0x00078b98
  0019668a: adds r6,#0x1
  0019668c: ldr.w r0,[r4,#0x9c]
  00196690: cmp.w r6,r0, lsl #0x2
  00196694: blt 0x0019666c
  00196696: ldr r0,[sp,#0x20]
  00196698: ldr.w r1,[r11,#0x0]
  0019669c: subs r0,r1,r0
  0019669e: ittt eq
  001966a0: add.eq sp,#0x24
  001966a2: pop.eq.w {r8,r9,r10,r11}
  001966a6: pop.eq {r4,r5,r6,r7,pc}
  001966a8: blx 0x0006e824
```
