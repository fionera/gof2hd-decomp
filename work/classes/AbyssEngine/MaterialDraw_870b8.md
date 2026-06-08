# AbyssEngine::MaterialDraw
elf 0x870b8 body 400
Sig: undefined __stdcall MaterialDraw(PaintCanvas * param_1, Engine * param_2, Material * param_3, bool param_4)

## decompile
```c

/* AbyssEngine::MaterialDraw(AbyssEngine::PaintCanvas*, AbyssEngine::Engine*,
   AbyssEngine::Material*, bool) */

void AbyssEngine::MaterialDraw(PaintCanvas *param_1,Engine *param_2,Material *param_3,bool param_4)

{
  double dVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  uint in_fpscr;
  uint uVar6;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float fVar7;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar8;
  float extraout_s3;
  double dVar9;
  double dVar10;
  Matrix aMStack_90 [60];
  int local_54;
  
  piVar5 = *(int **)(DAT_00097254 + 0x970d4);
  local_54 = *piVar5;
  if (param_1 != (PaintCanvas *)0x0 && param_3 != (Material *)0x0) {
    if (param_4) {
      Engine::SetTexturesExt
                ((uint)param_2,*(undefined4 *)param_3,*(undefined4 *)(param_3 + 4),
                 *(undefined4 *)(param_3 + 8),0xffffffff);
    }
    Engine::SetAddData(param_2,*(void **)(param_3 + 0x24),*(int *)(param_3 + 0x28));
    uVar6 = in_fpscr & 0xfffffff | (uint)(*(float *)(param_3 + 0x68) == -10.0) << 0x1e;
    fVar7 = extraout_s1;
    fVar8 = extraout_s2;
    if ((byte)(uVar6 >> 0x1e) == 0) {
      Engine::LightSetLightColorAmbient
                (*(float *)(param_3 + 0x68),extraout_s1,extraout_s2,(uint)param_2);
      fVar7 = extraout_s1_00;
      fVar8 = extraout_s2_00;
    }
    dVar1 = DAT_00097248;
    iVar4 = 0;
    for (uVar3 = 0; uVar3 < *(uint *)(param_3 + 0x44); uVar3 = uVar3 + 1) {
      AEMath::operator*(aMStack_90,(Matrix *)(*(int *)(param_3 + 0x60) + iVar4));
      PaintCanvas::SetWorldViewMatrix((Matrix *)param_1);
      Engine::SetModelMatrix((Matrix *)param_2);
      Engine::SetUVMatrix(param_2,(Matrix *)(*(int *)(param_3 + 0x3c) + iVar4));
      uVar2 = *(uint *)(*(int *)(param_3 + 0x54) + uVar3 * 4);
      VectorUnsignedToFloat(uVar2 >> 0x18,(byte)(uVar6 >> 0x16) & 3);
      dVar9 = (double)VectorUnsignedToFloat((uVar2 & 0xffffff) >> 0x10,(byte)(uVar6 >> 0x16) & 3);
      VectorUnsignedToFloat((uVar2 & 0xffff) >> 8,(byte)(uVar6 >> 0x16) & 3);
      dVar10 = (double)VectorUnsignedToFloat(uVar2 & 0xff,(byte)(uVar6 >> 0x16) & 3);
      Engine::SetColor((float)(dVar10 / dVar1),extraout_s1_01,(float)(dVar9 / dVar1),extraout_s3);
      MeshDraw(param_2,*(Mesh **)(*(int *)(param_3 + 0x48) + uVar3 * 4));
      iVar4 = iVar4 + 0x3c;
      fVar7 = extraout_s1_02;
      fVar8 = extraout_s2_01;
    }
    if (*(float *)(param_3 + 0x68) != -10.0) {
      Engine::LightSetLightColorAmbient(*(float *)(param_3 + 0x68),fVar7,fVar8,(uint)param_2);
    }
    *(undefined4 *)(param_3 + 0x2c) = 0;
    *(undefined4 *)(param_3 + 0x5c) = 0;
    *(undefined4 *)(param_3 + 0x38) = 0;
    *(undefined4 *)(param_3 + 0x44) = 0;
    *(undefined4 *)(param_3 + 0x50) = 0;
  }
  if (*piVar5 != local_54) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000970b8: push {r4,r5,r6,r7,lr}
  000970ba: add r7,sp,#0xc
  000970bc: push {r7,r8,r9,r10,r11}
  000970c0: vpush {d8,d9,d10,d11,d12}
  000970c4: sub sp,#0x48
  000970c6: mov r8,r0
  000970c8: ldr r0,[0x00097254]
  000970ca: cmp.w r8,#0x0
  000970ce: mov r4,r2
  000970d0: add r0,pc
  000970d2: mov r11,r1
  000970d4: ldr.w r9,[r0,#0x0]
  000970d8: ldr.w r0,[r9,#0x0]
  000970dc: str r0,[sp,#0x44]
  000970de: it ne
  000970e0: cmp.ne r4,#0x0
  000970e2: beq.w 0x0009722a
  000970e6: cbz r3,0x000970f8
  000970e8: ldm.w r4,{r1,r2,r3}
  000970ec: mov.w r0,#0xffffffff
  000970f0: str r0,[sp,#0x0]
  000970f2: mov r0,r11
  000970f4: blx 0x00070864
  000970f8: ldrd r1,r2,[r4,#0x24]
  000970fc: mov r0,r11
  000970fe: blx 0x00070870
  00097102: vmov.f32 s16,0xc1200000
  00097106: vldr.32 s0,[r4,#0x68]
  0009710a: vcmpe.f32 s0,s16
  0009710e: vmrs apsr,fpscr
  00097112: beq 0x00097136
  00097114: vmov r1,s0
  00097118: vldr.32 s22,[r11,#0x268]
  0009711c: vldr.32 s20,[r11,#0x26c]
  00097120: mov.w r0,#0x4000
  00097124: vldr.32 s18,[r11,#0x270]
  00097128: ldrd r2,r3,[r4,#0x6c]
  0009712c: str r0,[sp,#0x0]
  0009712e: mov r0,r11
  00097130: blx 0x0007087c
  00097134: b 0x00097142
  00097136: vldr.32 s18,[pc,#0x118]
  0009713a: vmov.f32 s20,s18
  0009713e: vmov.f32 s22,s18
  00097142: add.w r10,sp,#0x8
  00097146: vldr.64 d12,[pc,#0x100]
  0009714a: movs r6,#0x0
  0009714c: movs r5,#0x0
  0009714e: b 0x000971f2
  00097150: ldr r1,[r4,#0x60]
  00097152: ldr r0,[r4,#0x30]
  00097154: add r1,r6
  00097156: adds r2,r0,r6
  00097158: mov r0,r10
  0009715a: blx 0x0006f82c
  0009715e: mov r0,r8
  00097160: mov r1,r10
  00097162: blx 0x0006f55c
  00097166: ldr r0,[r4,#0x30]
  00097168: adds r1,r0,r6
  0009716a: mov r0,r11
  0009716c: blx 0x0006ffb8
  00097170: ldr r0,[r4,#0x3c]
  00097172: adds r1,r0,r6
  00097174: mov r0,r11
  00097176: blx 0x0006ffc4
  0009717a: ldr r0,[r4,#0x54]
  0009717c: ldr.w r0,[r0,r5,lsl #0x2]
  00097180: lsrs r1,r0,#0x18
  00097182: vmov s0,r1
  00097186: vcvt.f64.u32 d16,s0
  0009718a: ubfx r1,r0,#0x10,#0x8
  0009718e: vmov s0,r1
  00097192: vdiv.f64 d16,d16,d12
  00097196: vcvt.f64.u32 d17,s0
  0009719a: ubfx r1,r0,#0x8,#0x8
  0009719e: uxtb r0,r0
  000971a0: vmov s0,r1
  000971a4: vdiv.f64 d17,d17,d12
  000971a8: vcvt.f64.u32 d18,s0
  000971ac: vdiv.f64 d18,d18,d12
  000971b0: vmov s0,r0
  000971b4: mov r0,r11
  000971b6: vcvt.f32.f64 s2,d17
  000971ba: vcvt.f64.u32 d19,s0
  000971be: vdiv.f64 d19,d19,d12
  000971c2: vcvt.f32.f64 s0,d16
  000971c6: vcvt.f32.f64 s4,d18
  000971ca: vmov r1,s0
  000971ce: vmov r2,s2
  000971d2: vmov r3,s4
  000971d6: vcvt.f32.f64 s0,d19
  000971da: vstr.32 s0,[sp]
  000971de: blx 0x0006fad8
  000971e2: ldr r0,[r4,#0x48]
  000971e4: ldr.w r1,[r0,r5,lsl #0x2]
  000971e8: mov r0,r11
  000971ea: blx 0x0006f568
  000971ee: adds r6,#0x3c
  000971f0: adds r5,#0x1
  000971f2: ldr r0,[r4,#0x44]
  000971f4: cmp r5,r0
  000971f6: bcc 0x00097150
  000971f8: vldr.32 s0,[r4,#0x68]
  000971fc: vcmpe.f32 s0,s16
  00097200: vmrs apsr,fpscr
  00097204: beq 0x0009721e
  00097206: vmov r1,s22
  0009720a: mov.w r0,#0x4000
  0009720e: vmov r2,s20
  00097212: str r0,[sp,#0x0]
  00097214: vmov r3,s18
  00097218: mov r0,r11
  0009721a: blx 0x0007087c
  0009721e: movs r0,#0x0
  00097220: str r0,[r4,#0x2c]
  00097222: str r0,[r4,#0x5c]
  00097224: str r0,[r4,#0x38]
  00097226: str r0,[r4,#0x44]
  00097228: str r0,[r4,#0x50]
  0009722a: ldr r0,[sp,#0x44]
  0009722c: ldr.w r1,[r9,#0x0]
  00097230: subs r0,r1,r0
  00097232: itttt eq
  00097234: add.eq sp,#0x48
  00097236: vpop.eq {d8,d9,d10,d11,d12}
  0009723a: add.eq sp,#0x4
  0009723c: pop.eq.w {r8,r9,r10,r11}
  00097240: it eq
  00097242: pop.eq {r4,r5,r6,r7,pc}
  00097244: blx 0x0006e824
```
