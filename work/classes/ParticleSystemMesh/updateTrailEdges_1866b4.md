# ParticleSystemMesh::updateTrailEdges
elf 0x1866b4 body 354
Sig: undefined __stdcall updateTrailEdges(int param_1, int param_2)

## decompile
```c

/* ParticleSystemMesh::updateTrailEdges(int, int) */

void ParticleSystemMesh::updateTrailEdges(int param_1,int param_2)

{
  ushort uVar1;
  int *piVar2;
  int iVar3;
  short sVar4;
  undefined4 in_r2;
  int iVar5;
  int iVar6;
  uint in_fpscr;
  float fVar7;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  Vector local_40 [4];
  float local_3c;
  float local_38;
  Vector local_34 [4];
  float local_30;
  float local_2c;
  int local_28;
  
  iVar5 = 0;
  piVar2 = *(int **)(DAT_0019681c + 0x1966cc);
  local_28 = *piVar2;
  iVar3 = *(int *)(param_1 + 0x98);
  fVar7 = (float)VectorSignedToFloat(in_r2,(byte)(in_fpscr >> 0x16) & 3);
  iVar6 = *(int *)(param_1 + 0x58) + param_2 * *(int *)(param_1 + 0x9c) * 4;
  fVar7 = fVar7 * DAT_00196818;
  for (; iVar5 < iVar3; iVar5 = iVar5 + 1) {
    AbyssEngine::AEMath::operator*(local_34,fVar7);
    uVar1 = (ushort)iVar6;
    AbyssEngine::PaintCanvas::MeshTranslatePoint
              (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),uVar1,-local_2c,extraout_s1,
               -local_30);
    sVar4 = 4;
    if (*(char *)(param_1 + 0x74) == '\0') {
      sVar4 = 1;
    }
    fVar7 = (float)AbyssEngine::PaintCanvas::MeshTranslatePoint
                             (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),sVar4 + uVar1
                              ,local_30,extraout_s1_00,local_2c);
    if ((*(int *)(*(int *)(param_1 + 0x68) + param_2 * 4) != -2) ||
       (-1 < (int)((uint)*(byte *)(param_1 + 0x35) << 0x18))) {
      AbyssEngine::AEMath::operator*(local_40,fVar7);
      AbyssEngine::PaintCanvas::MeshTranslatePoint
                (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),uVar1 + 2,-local_38,
                 extraout_s1_01,-local_3c);
      sVar4 = 4;
      if (*(char *)(param_1 + 0x74) == '\0') {
        sVar4 = 1;
      }
      fVar7 = (float)AbyssEngine::PaintCanvas::MeshTranslatePoint
                               (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),
                                sVar4 + uVar1 + 2,local_3c,extraout_s1_02,local_38);
      iVar3 = 8;
      if (*(char *)(param_1 + 0x74) == '\0') {
        iVar3 = 4;
      }
      iVar6 = iVar6 + iVar3;
    }
    iVar3 = *(int *)(param_1 + 0x98);
  }
  if (*piVar2 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001966b4: push {r4,r5,r6,r7,lr}
  001966b6: add r7,sp,#0xc
  001966b8: push {r8,r9,r10,r11}
  001966bc: sub sp,#0x2c
  001966be: mov r5,r0
  001966c0: ldr r0,[0x0019681c]
  001966c2: vmov s0,r2
  001966c6: mov r8,r1
  001966c8: add r0,pc
  001966ca: vldr.32 s2,[pc,#0x14c]
  001966ce: mov.w r10,#0x0
  001966d2: ldr r0,[r0,#0x0]
  001966d4: str r0,[sp,#0xc]
  001966d6: ldr r0,[r0,#0x0]
  001966d8: str r0,[sp,#0x28]
  001966da: ldrd r0,r6,[r5,#0x98]
  001966de: ldr r1,[r5,#0x58]
  001966e0: mul r2,r8,r6
  001966e4: ldr r3,[r5,#0x64]
  001966e6: vcvt.f32.s32 s0,s0
  001966ea: mul r6,r8,r0
  001966ee: add.w r11,r1,r2, lsl #0x2
  001966f2: add.w r1,r6,r6, lsl #0x1
  001966f6: vmul.f32 s0,s0,s2
  001966fa: add.w r6,r3,r1, lsl #0x3
  001966fe: vmov r4,s0
  00196702: b 0x001967fa
  00196704: add r0,sp,#0x1c
  00196706: mov r1,r6
  00196708: mov r2,r4
  0019670a: blx 0x0006ec74
  0019670e: vldr.32 s0,[sp,#0x24]
  00196712: uxth.w r2,r11
  00196716: vldr.32 s2,[sp,#0x20]
  0019671a: vneg.f32 s0,s0
  0019671e: ldr r3,[sp,#0x1c]
  00196720: vneg.f32 s2,s2
  00196724: ldr r0,[r5,#0x8]
  00196726: ldr r1,[r5,#0x54]
  00196728: eor r3,r3,#0x80000000
  0019672c: vstr.32 s0,[sp,#0x4]
  00196730: vstr.32 s2,[sp]
  00196734: blx 0x00078b98
  00196738: ldrb.w r2,[r5,#0x74]
  0019673c: vldr.32 s0,[sp,#0x20]
  00196740: vldr.32 s2,[sp,#0x24]
  00196744: cmp r2,#0x0
  00196746: mov.w r2,#0x4
  0019674a: ldr r0,[r5,#0x8]
  0019674c: ldr r1,[r5,#0x54]
  0019674e: ldr r3,[sp,#0x1c]
  00196750: vstr.32 s0,[sp]
  00196754: vstr.32 s2,[sp,#0x4]
  00196758: it eq
  0019675a: mov.eq r2,#0x1
  0019675c: add r2,r11
  0019675e: uxth r2,r2
  00196760: blx 0x00078b98
  00196764: ldr r0,[r5,#0x68]
  00196766: add.w r1,r6,#0xc
  0019676a: ldr.w r0,[r0,r8,lsl #0x2]
  0019676e: adds r0,#0x2
  00196770: bne 0x0019677a
  00196772: ldrb.w r0,[r5,#0x35]
  00196776: lsls r0,r0,#0x18
  00196778: bmi 0x001967f0
  0019677a: add r0,sp,#0x10
  0019677c: mov r2,r4
  0019677e: blx 0x0006ec74
  00196782: vldr.32 s0,[sp,#0x18]
  00196786: add.w r9,r11,#0x2
  0019678a: vldr.32 s2,[sp,#0x14]
  0019678e: vneg.f32 s0,s0
  00196792: ldr r3,[sp,#0x10]
  00196794: vneg.f32 s2,s2
  00196798: ldr r0,[r5,#0x8]
  0019679a: ldr r1,[r5,#0x54]
  0019679c: eor r3,r3,#0x80000000
  001967a0: uxth.w r2,r9
  001967a4: vstr.32 s0,[sp,#0x4]
  001967a8: vstr.32 s2,[sp]
  001967ac: blx 0x00078b98
  001967b0: ldrb.w r2,[r5,#0x74]
  001967b4: vldr.32 s0,[sp,#0x14]
  001967b8: vldr.32 s2,[sp,#0x18]
  001967bc: cmp r2,#0x0
  001967be: mov.w r2,#0x4
  001967c2: ldr r0,[r5,#0x8]
  001967c4: ldr r1,[r5,#0x54]
  001967c6: ldr r3,[sp,#0x10]
  001967c8: vstr.32 s0,[sp]
  001967cc: vstr.32 s2,[sp,#0x4]
  001967d0: it eq
  001967d2: mov.eq r2,#0x1
  001967d4: add r2,r9
  001967d6: uxth r2,r2
  001967d8: blx 0x00078b98
  001967dc: ldrb.w r0,[r5,#0x74]
  001967e0: adds r6,#0x18
  001967e2: cmp r0,#0x0
  001967e4: mov.w r0,#0x8
  001967e8: it eq
  001967ea: mov.eq r0,#0x4
  001967ec: add r11,r0
  001967ee: b 0x001967f2
  001967f0: mov r6,r1
  001967f2: ldr.w r0,[r5,#0x98]
  001967f6: add.w r10,r10,#0x1
  001967fa: cmp r10,r0
  001967fc: blt.w 0x00196704
  00196800: ldr r0,[sp,#0x28]
  00196802: ldr r1,[sp,#0xc]
  00196804: ldr r1,[r1,#0x0]
  00196806: subs r0,r1,r0
  00196808: ittt eq
  0019680a: add.eq sp,#0x2c
  0019680c: pop.eq.w {r8,r9,r10,r11}
  00196810: pop.eq {r4,r5,r6,r7,pc}
  00196812: blx 0x0006e824
```
