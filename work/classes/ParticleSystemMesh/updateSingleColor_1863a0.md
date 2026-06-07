# ParticleSystemMesh::updateSingleColor
elf 0x1863a0 body 560
Sig: undefined __stdcall updateSingleColor(int param_1)

## decompile
```c

/* ParticleSystemMesh::updateSingleColor(int) */

void ParticleSystemMesh::updateSingleColor(int param_1)

{
  int iVar1;
  int iVar2;
  float *in_r1;
  float *pfVar3;
  uint uVar4;
  float unaff_r4;
  float unaff_r5;
  int iVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  uint in_fpscr;
  float fVar9;
  float in_s1;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float in_s3;
  float extraout_s3;
  float extraout_s3_00;
  float extraout_s3_01;
  float extraout_s3_02;
  float extraout_s3_03;
  float extraout_s3_04;
  float extraout_s3_05;
  float fVar10;
  
  piVar7 = *(int **)(DAT_001965d4 + 0x1963b2);
  iVar1 = *piVar7;
  iVar8 = *(int *)(param_1 + 0x58);
  iVar6 = *(int *)(param_1 + 0x9c);
  if ((int)((uint)*(byte *)(param_1 + 0x35) << 0x18) < 0) {
    pfVar3 = in_r1;
    if (in_r1 == (float *)0x0) {
      pfVar3 = *(float **)(param_1 + 0x48);
    }
    if (*(int *)(*(int *)(param_1 + 0x68) + (int)pfVar3 * 4 + -4) == -1) {
      uVar4 = 0xff;
      if (*(char *)(param_1 + 0x45) == '\0') {
        uVar4 = 0xffffff00;
      }
      uVar4 = *(uint *)(*(int *)(DAT_001965d8 + 0x196408) +
                        *(char *)(*(int *)(param_1 + 0x6c) + (int)in_r1) * 0xa0 + 0x38) & uVar4;
      fVar9 = (float)VectorUnsignedToFloat((uVar4 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
      VectorUnsignedToFloat(uVar4 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
      fVar10 = (float)VectorUnsignedToFloat((uVar4 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
      VectorUnsignedToFloat(uVar4 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
      unaff_r5 = fVar9 * DAT_001965d0;
      unaff_r4 = fVar10 * DAT_001965d0;
      goto LAB_00196470;
    }
  }
  IParticleSystem::interpolateColor
            (param_1,in_r1,(float *)&stack0xffffffd8,(float *)&stack0xffffffd4,
             (float *)&stack0xffffffd0);
  in_s1 = extraout_s1;
  in_s3 = extraout_s3;
LAB_00196470:
  iVar8 = iVar8 + iVar6 * (int)in_r1 * 4;
  iVar6 = iVar8;
  for (iVar5 = 0; iVar2 = *(int *)(param_1 + 0x9c), iVar5 < iVar2; iVar5 = iVar5 + 1) {
    AbyssEngine::PaintCanvas::MeshSetColor
              (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),(short)iVar6 + 2,unaff_r5,
               in_s1,unaff_r4,in_s3);
    AbyssEngine::PaintCanvas::MeshSetColor
              (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),(short)iVar6 + 3,unaff_r5,
               extraout_s1_00,unaff_r4,extraout_s3_00);
    iVar6 = iVar6 + 4;
    in_s1 = extraout_s1_01;
    in_s3 = extraout_s3_01;
  }
  if ((int)((uint)*(byte *)(param_1 + 0x35) << 0x18) < 0) {
    iVar6 = 0;
    if ((float *)(*(int *)(param_1 + 0x48) + -1) != in_r1) {
      iVar6 = (int)in_r1 + 1;
    }
    if (*(int *)(*(int *)(param_1 + 0x68) + iVar6 * 4) != -1) {
      iVar6 = *(int *)(param_1 + 0x58) + iVar2 * iVar6 * 4;
      for (iVar8 = 0; iVar8 < iVar2; iVar8 = iVar8 + 1) {
        AbyssEngine::PaintCanvas::MeshSetColor
                  (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),(ushort)iVar6,unaff_r5,
                   in_s1,unaff_r4,in_s3);
        AbyssEngine::PaintCanvas::MeshSetColor
                  (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),(ushort)iVar6 + 1,
                   unaff_r5,extraout_s1_04,unaff_r4,extraout_s3_04);
        iVar6 = iVar6 + 4;
        iVar2 = *(int *)(param_1 + 0x9c);
        in_s1 = extraout_s1_05;
        in_s3 = extraout_s3_05;
      }
    }
  }
  else {
    for (iVar6 = 0; iVar6 < iVar2; iVar6 = iVar6 + 1) {
      AbyssEngine::PaintCanvas::MeshSetColor
                (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),(ushort)iVar8,unaff_r5,
                 in_s1,unaff_r4,in_s3);
      AbyssEngine::PaintCanvas::MeshSetColor
                (*(PaintCanvas **)(param_1 + 8),*(uint *)(param_1 + 0x54),(ushort)iVar8 + 1,unaff_r5
                 ,extraout_s1_02,unaff_r4,extraout_s3_02);
      iVar8 = iVar8 + 4;
      iVar2 = *(int *)(param_1 + 0x9c);
      in_s1 = extraout_s1_03;
      in_s3 = extraout_s3_03;
    }
  }
  if (*piVar7 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001963a0: push {r4,r5,r6,r7,lr}
  001963a2: add r7,sp,#0xc
  001963a4: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10}
  001963a8: mov r4,r0
  001963aa: ldr r0,[0x001965d4]
  001963ac: mov r10,r1
  001963ae: add r0,pc
  001963b0: ldr.w r8,[r0,#0x0]
  001963b4: ldr.w r0,[r8,#0x0]
  001963b8: str r0,[sp,#0x1c]
  001963ba: ldrb.w r0,[r4,#0x35]
  001963be: ldr.w r9,[r4,#0x58]
  001963c2: ldr.w r6,[r4,#0x9c]
  001963c6: lsls r0,r0,#0x18
  001963c8: bpl 0x001963e2
  001963ca: mov r1,r10
  001963cc: ldr r0,[r4,#0x68]
  001963ce: cmp.w r10,#0x0
  001963d2: it eq
  001963d4: ldr.eq r1,[r4,#0x48]
  001963d6: add.w r0,r0,r1, lsl #0x2
  001963da: ldr.w r0,[r0,#-0x4]
  001963de: adds r0,#0x1
  001963e0: beq 0x001963f8
  001963e2: add r0,sp,#0xc
  001963e4: add r1,sp,#0x10
  001963e6: add r2,sp,#0x18
  001963e8: add r3,sp,#0x14
  001963ea: strd r1,r0,[sp,#0x0]
  001963ee: mov r0,r4
  001963f0: mov r1,r10
  001963f2: blx 0x00078910
  001963f6: b 0x00196470
  001963f8: ldr r0,[r4,#0x6c]
  001963fa: ldr r1,[0x001965d8]
  001963fc: ldrb.w r2,[r4,#0x45]
  00196400: ldrsb.w r0,[r0,r10]
  00196404: add r1,pc
  00196406: cmp r2,#0x0
  00196408: ldr r1,[r1,#0x0]
  0019640a: vldr.32 s8,[pc,#0x1c4]
  0019640e: add.w r0,r0,r0, lsl #0x2
  00196412: add.w r0,r1,r0, lsl #0x5
  00196416: mov.w r1,#0xff
  0019641a: ldr r0,[r0,#0x38]
  0019641c: it eq
  0019641e: mvn.eq r1,#0xff
  00196422: ands r0,r1
  00196424: ubfx r2,r0,#0x10,#0x8
  00196428: lsrs r1,r0,#0x18
  0019642a: vmov s2,r1
  0019642e: vmov s0,r2
  00196432: vcvt.f32.u32 s0,s0
  00196436: ubfx r2,r0,#0x8,#0x8
  0019643a: uxtb r0,r0
  0019643c: vcvt.f32.u32 s2,s2
  00196440: vmov s4,r2
  00196444: vmov s6,r0
  00196448: vcvt.f32.u32 s4,s4
  0019644c: vcvt.f32.u32 s6,s6
  00196450: vmul.f32 s0,s0,s8
  00196454: vmul.f32 s2,s2,s8
  00196458: vmul.f32 s4,s4,s8
  0019645c: vmul.f32 s6,s6,s8
  00196460: vstr.32 s0,[sp,#0x14]
  00196464: vstr.32 s2,[sp,#0x18]
  00196468: vstr.32 s4,[sp,#0x10]
  0019646c: vstr.32 s6,[sp,#0xc]
  00196470: mul r0,r6,r10
  00196474: movs r5,#0x0
  00196476: add.w r9,r9,r0, lsl #0x2
  0019647a: mov r6,r9
  0019647c: b 0x001964ce
  0019647e: adds r2,r6,#0x2
  00196480: ldr r3,[sp,#0x18]
  00196482: ldr r0,[r4,#0x8]
  00196484: ldr r1,[r4,#0x54]
  00196486: uxth r2,r2
  00196488: vldr.32 s0,[sp,#0x14]
  0019648c: vldr.32 s2,[sp,#0x10]
  00196490: vldr.32 s4,[sp,#0xc]
  00196494: vstr.32 s0,[sp]
  00196498: vstr.32 s2,[sp,#0x4]
  0019649c: vstr.32 s4,[sp,#0x8]
  001964a0: blx 0x00077df4
  001964a4: adds r2,r6,#0x3
  001964a6: ldr r0,[r4,#0x8]
  001964a8: ldr r1,[r4,#0x54]
  001964aa: ldr r3,[sp,#0x18]
  001964ac: uxth r2,r2
  001964ae: vldr.32 s0,[sp,#0x14]
  001964b2: vldr.32 s2,[sp,#0x10]
  001964b6: vldr.32 s4,[sp,#0xc]
  001964ba: vstr.32 s0,[sp]
  001964be: vstr.32 s2,[sp,#0x4]
  001964c2: vstr.32 s4,[sp,#0x8]
  001964c6: blx 0x00077df4
  001964ca: adds r6,#0x4
  001964cc: adds r5,#0x1
  001964ce: ldr.w r0,[r4,#0x9c]
  001964d2: cmp r5,r0
  001964d4: blt 0x0019647e
  001964d6: ldrb.w r1,[r4,#0x35]
  001964da: lsls r1,r1,#0x18
  001964dc: bmi 0x00196540
  001964de: movs r5,#0x0
  001964e0: b 0x0019653a
  001964e2: ldr r3,[sp,#0x18]
  001964e4: uxth.w r2,r9
  001964e8: ldr r0,[r4,#0x8]
  001964ea: ldr r1,[r4,#0x54]
  001964ec: vldr.32 s0,[sp,#0x14]
  001964f0: vldr.32 s2,[sp,#0x10]
  001964f4: vldr.32 s4,[sp,#0xc]
  001964f8: vstr.32 s0,[sp]
  001964fc: vstr.32 s2,[sp,#0x4]
  00196500: vstr.32 s4,[sp,#0x8]
  00196504: blx 0x00077df4
  00196508: add.w r2,r9,#0x1
  0019650c: ldr r0,[r4,#0x8]
  0019650e: ldr r1,[r4,#0x54]
  00196510: ldr r3,[sp,#0x18]
  00196512: uxth r2,r2
  00196514: vldr.32 s0,[sp,#0x14]
  00196518: vldr.32 s2,[sp,#0x10]
  0019651c: vldr.32 s4,[sp,#0xc]
  00196520: vstr.32 s0,[sp]
  00196524: vstr.32 s2,[sp,#0x4]
  00196528: vstr.32 s4,[sp,#0x8]
  0019652c: blx 0x00077df4
  00196530: add.w r9,r9,#0x4
  00196534: ldr.w r0,[r4,#0x9c]
  00196538: adds r5,#0x1
  0019653a: cmp r5,r0
  0019653c: blt 0x001964e2
  0019653e: b 0x001965ba
  00196540: ldr r1,[r4,#0x48]
  00196542: ldr r2,[r4,#0x68]
  00196544: subs r1,#0x1
  00196546: subs.w r1,r1,r10
  0019654a: it ne
  0019654c: add.ne.w r1,r10,#0x1
  00196550: ldr.w r2,[r2,r1,lsl #0x2]
  00196554: adds r2,#0x1
  00196556: beq 0x001965ba
  00196558: muls r1,r0
  0019655a: ldr r2,[r4,#0x58]
  0019655c: movs r6,#0x0
  0019655e: add.w r5,r2,r1, lsl #0x2
  00196562: b 0x001965b6
  00196564: ldr r3,[sp,#0x18]
  00196566: uxth r2,r5
  00196568: ldr r0,[r4,#0x8]
  0019656a: ldr r1,[r4,#0x54]
  0019656c: vldr.32 s0,[sp,#0x14]
  00196570: vldr.32 s2,[sp,#0x10]
  00196574: vldr.32 s4,[sp,#0xc]
  00196578: vstr.32 s0,[sp]
  0019657c: vstr.32 s2,[sp,#0x4]
  00196580: vstr.32 s4,[sp,#0x8]
  00196584: blx 0x00077df4
  00196588: adds r2,r5,#0x1
  0019658a: ldr r0,[r4,#0x8]
  0019658c: ldr r1,[r4,#0x54]
  0019658e: ldr r3,[sp,#0x18]
  00196590: uxth r2,r2
  00196592: vldr.32 s0,[sp,#0x14]
  00196596: vldr.32 s2,[sp,#0x10]
  0019659a: vldr.32 s4,[sp,#0xc]
  0019659e: vstr.32 s0,[sp]
  001965a2: vstr.32 s2,[sp,#0x4]
  001965a6: vstr.32 s4,[sp,#0x8]
  001965aa: blx 0x00077df4
  001965ae: adds r5,#0x4
  001965b0: ldr.w r0,[r4,#0x9c]
  001965b4: adds r6,#0x1
  001965b6: cmp r6,r0
  001965b8: blt 0x00196564
  001965ba: ldr r0,[sp,#0x1c]
  001965bc: ldr.w r1,[r8,#0x0]
  001965c0: subs r0,r1,r0
  001965c2: ittt eq
  001965c4: add.eq sp,#0x20
  001965c6: pop.eq.w {r8,r9,r10}
  001965ca: pop.eq {r4,r5,r6,r7,pc}
  001965cc: blx 0x0006e824
```
