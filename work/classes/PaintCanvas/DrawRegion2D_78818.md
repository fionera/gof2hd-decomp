# PaintCanvas::DrawRegion2D
elf 0x78818 body 538
Sig: undefined __thiscall DrawRegion2D(PaintCanvas * this, uint param_1, float param_2, int param_3, int param_4, int param_5, int param_6, float param_7, float param_8)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawRegion2D(unsigned int, float, int, int, int, int, float, float) */

void __thiscall
AbyssEngine::PaintCanvas::DrawRegion2D
          (PaintCanvas *this,uint param_1,float param_2,int param_3,int param_4,int param_5,
          int param_6,float param_7,float param_8)

{
  uint uVar1;
  int *piVar2;
  Image2D *pIVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  code *pcVar6;
  uint in_fpscr;
  float fVar7;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s2;
  float extraout_s2_00;
  float fVar8;
  undefined4 in_stack_00000008;
  float in_stack_0000000c;
  float in_stack_00000010;
  undefined1 auStack_1f0 [60];
  Matrix aMStack_1b4 [60];
  undefined4 local_178 [5];
  undefined4 local_164;
  undefined4 local_160;
  undefined4 uStack_15c;
  undefined4 uStack_158;
  undefined4 uStack_154;
  undefined8 uStack_150;
  undefined8 uStack_148;
  undefined4 local_140;
  undefined4 local_138 [5];
  undefined4 local_124;
  undefined4 local_120;
  undefined4 uStack_11c;
  undefined4 uStack_118;
  undefined4 uStack_114;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined4 local_100;
  undefined4 local_f8;
  uint uStack_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  uint local_e8;
  undefined4 uStack_e4;
  undefined4 local_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  undefined4 local_d0;
  undefined8 local_cc;
  undefined8 uStack_c4;
  undefined4 local_b8 [3];
  float local_ac;
  undefined4 local_a4;
  undefined4 local_a0;
  float fStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined4 local_80;
  undefined4 local_78 [3];
  float local_6c;
  undefined4 local_64;
  undefined4 local_60;
  float fStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined4 local_40;
  int local_3c;
  
  piVar2 = *(int **)(DAT_00088a58 + 0x88832);
  local_3c = *piVar2;
  if (param_1 < *(uint *)(this + 0x14c)) {
    pIVar3 = *(Image2D **)(*(int *)(this + 0x150) + param_1 * 4);
    if ((PaintCanvas *)(uint)(byte)pIVar3[0x14] != (PaintCanvas *)0x0) {
      RestoreImage2D((PaintCanvas *)(uint)(byte)pIVar3[0x14],pIVar3);
      pIVar3 = *(Image2D **)(*(int *)(this + 0x150) + param_1 * 4);
    }
    SetTexture((uint)this,*(uint *)(pIVar3 + 4));
    uStack_48 = DAT_00088a50;
    uStack_50 = DAT_00088a48;
    uStack_c4 = DAT_00088a40;
    local_cc = DAT_00088a38;
    uStack_15c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uStack_158 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_154 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    fVar7 = (float)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
    puVar5 = (undefined4 *)((uint)local_b8 | 4);
    puVar4 = (undefined4 *)((uint)local_78 | 4);
    *puVar5 = 0;
    puVar5[1] = uStack_15c;
    puVar5[2] = uStack_158;
    puVar5[3] = uStack_154;
    *puVar4 = 0;
    puVar4[1] = uStack_15c;
    puVar4[2] = uStack_158;
    puVar4[3] = uStack_154;
    local_60 = 0;
    local_a0 = 0;
    uStack_90 = uStack_50;
    uStack_88 = uStack_48;
    local_e0 = 0;
    puVar4 = (undefined4 *)((uint)local_138 | 4);
    local_ec = 0;
    local_f0 = 0;
    *puVar4 = 0;
    puVar4[1] = uStack_15c;
    puVar4[2] = uStack_158;
    puVar4[3] = uStack_154;
    local_120 = 0;
    uStack_110 = uStack_50;
    uStack_108 = uStack_48;
    puVar4 = (undefined4 *)((uint)local_178 | 4);
    *puVar4 = 0;
    puVar4[1] = uStack_15c;
    puVar4[2] = uStack_158;
    puVar4[3] = uStack_154;
    local_160 = 0;
    local_78[0] = 0x3f800000;
    local_64 = 0x3f800000;
    local_b8[0] = 0x3f800000;
    local_a4 = 0x3f800000;
    local_138[0] = 0x3f800000;
    local_124 = 0x3f800000;
    local_40 = 0x3f800000;
    local_80 = 0x3f800000;
    local_100 = 0x3f800000;
    uStack_150 = uStack_50;
    uStack_148 = uStack_48;
    fVar8 = (float)VectorSignedToFloat(param_5,(byte)(in_fpscr >> 0x16) & 3);
    local_ac = (float)VectorSignedToFloat(param_6,(byte)(in_fpscr >> 0x16) & 3);
    fStack_9c = (float)VectorSignedToFloat(in_stack_00000008,(byte)(in_fpscr >> 0x16) & 3);
    local_6c = (float)VectorSignedToFloat(-param_4,(byte)(in_fpscr >> 0x16) & 3);
    fStack_5c = (float)VectorSignedToFloat(-param_5,(byte)(in_fpscr >> 0x16) & 3);
    local_178[0] = 0x3f800000;
    local_164 = 0x3f800000;
    local_ac = local_ac + fVar7 * in_stack_0000000c;
    fStack_9c = fStack_9c + fVar8 * in_stack_00000010;
    local_140 = 0x3f800000;
    local_6c = local_6c * in_stack_0000000c;
    fStack_5c = fStack_5c * in_stack_00000010;
    uStack_11c = uStack_15c;
    uStack_118 = uStack_158;
    uStack_114 = uStack_154;
    uStack_dc = uStack_15c;
    uStack_d8 = uStack_158;
    uStack_d4 = uStack_154;
    uStack_98 = uStack_158;
    uStack_94 = uStack_154;
    uStack_58 = uStack_158;
    uStack_54 = uStack_154;
    uVar1 = AEMath::Sinf(local_6c);
    local_f8 = AEMath::Cosf(extraout_s0);
    local_d0 = 0x3f800000;
    uStack_f4 = uVar1 ^ 0x80000000;
    local_e8 = uVar1;
    uStack_e4 = local_f8;
    AEMath::MatrixSetScaling(aMStack_1b4,extraout_s0_00,extraout_s1,extraout_s2);
    pcVar6 = *(code **)(DAT_00088a5c + 0x889c2);
    (*pcVar6)(aMStack_1b4,&local_f8,local_78);
    AEMath::Matrix::operator=((Matrix *)local_138,aMStack_1b4);
    (*pcVar6)(auStack_1f0,local_b8,(Matrix *)local_138);
    (*pcVar6)(aMStack_1b4,auStack_1f0,local_178);
    AEMath::Matrix::operator=((Matrix *)local_138,aMStack_1b4);
    SetWorldViewMatrix((Matrix *)this);
    glDisable(0xb44);
    MeshDraw(*(Engine **)(this + 0x34),
             (Mesh *)**(undefined4 **)(*(int *)(this + 0x150) + param_1 * 4));
    glEnable(0xb44);
    param_2 = extraout_s0_01;
    param_7 = extraout_s1_00;
    param_8 = extraout_s2_00;
  }
  if (*piVar2 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_2,param_7,param_8);
  }
  return;
}

```

## target disasm
```
  00088818: push {r4,r5,r6,r7,lr}
  0008881a: add r7,sp,#0xc
  0008881c: push {r7,r8,r9,r10,r11}
  00088820: vpush {d8,d9}
  00088824: sub sp,#0x1c8
  00088826: mov r10,r0
  00088828: ldr r0,[0x00088a58]
  0008882a: mov r9,r1
  0008882c: mov r6,r3
  0008882e: add r0,pc
  00088830: ldr r1,[r0,#0x0]
  00088832: ldr r0,[r1,#0x0]
  00088834: str r0,[sp,#0x1c4]
  00088836: ldr.w r0,[r10,#0x14c]
  0008883a: cmp r0,r9
  0008883c: bls.w 0x00088a16
  00088840: strd r2,r1,[sp,#0x8]
  00088844: ldr.w r0,[r10,#0x150]
  00088848: ldr.w r11,[r7,#0x10]
  0008884c: ldr.w r8,[r7,#0x8]
  00088850: ldr.w r1,[r0,r9,lsl #0x2]
  00088854: vldr.32 s16,[r7,#0x18]
  00088858: vldr.32 s18,[r7,#0x14]
  0008885c: ldrb r0,[r1,#0x14]
  0008885e: cbz r0,0x0008886c
  00088860: blx 0x0006faf0
  00088864: ldr.w r0,[r10,#0x150]
  00088868: ldr.w r1,[r0,r9,lsl #0x2]
  0008886c: ldr r1,[r1,#0x4]
  0008886e: mov r0,r10
  00088870: mov.w r2,#0xffffffff
  00088874: blx 0x0006fa6c
  00088878: adr r1,[0x88a38]
  0008887a: add r2,sp,#0x108
  0008887c: vld1.64 {d18,d19},[r1]
  00088880: adr r1,[0x88a48]
  00088882: mov r5,r2
  00088884: rsbs r0,r6
  00088886: vld1.64 {d20,d21},[r1]
  0008888a: add.w r1,r2,#0x18
  0008888e: vmov.i32 q8,#0x0
  00088892: add.w r12,sp,#0x188
  00088896: ldr r2,[r7,#0xc]
  00088898: vmov s0,r6
  0008889c: vmov s10,r0
  000888a0: add.w r0,r12,#0x18
  000888a4: vcvt.f32.s32 s0,s0
  000888a8: add r4,sp,#0x148
  000888aa: vmov s4,r2
  000888ae: rsb.w r2,r8,#0x0
  000888b2: orr r3,r4,#0x4
  000888b6: vmov s2,r11
  000888ba: vmov s8,r2
  000888be: orr r2,r12,#0x4
  000888c2: vst1.32 {d16,d17},[r3]
  000888c6: add.w r11,sp,#0xc8
  000888ca: vmov s6,r8
  000888ce: mov.w r6,#0x3f800000
  000888d2: vst1.32 {d16,d17},[r2]
  000888d6: vst1.64 {d16,d17},[r0]
  000888da: add.w r0,r12,#0x28
  000888de: vst1.64 {d20,d21},[r0]
  000888e2: add.w r0,r4,#0x18
  000888e6: vst1.64 {d16,d17},[r0]
  000888ea: add.w r0,r4,#0x28
  000888ee: vst1.64 {d20,d21},[r0]
  000888f2: add.w r0,r5,#0x2c
  000888f6: vst1.64 {d16,d17},[r1]
  000888fa: orr r1,r11,#0x4
  000888fe: vst1.32 {d18,d19},[r0]
  00088902: movs r0,#0x0
  00088904: str r0,[sp,#0x114]
  00088906: str r0,[sp,#0x110]
  00088908: add.w r0,r11,#0x18
  0008890c: vst1.32 {d16,d17},[r1]
  00088910: add r1,sp,#0x88
  00088912: vst1.64 {d16,d17},[r0]
  00088916: add.w r0,r11,#0x28
  0008891a: vst1.64 {d20,d21},[r0]
  0008891e: orr r0,r1,#0x4
  00088922: vst1.32 {d16,d17},[r0]
  00088926: add.w r0,r1,#0x18
  0008892a: vst1.64 {d16,d17},[r0]
  0008892e: add.w r0,r1,#0x28
  00088932: str r6,[sp,#0x188]
  00088934: str r6,[sp,#0x19c]
  00088936: str r6,[sp,#0x148]
  00088938: str r6,[sp,#0x15c]
  0008893a: str r6,[sp,#0xc8]
  0008893c: str r6,[sp,#0xdc]
  0008893e: str r6,[sp,#0x1c0]
  00088940: str r6,[sp,#0x180]
  00088942: str r6,[sp,#0x100]
  00088944: vst1.64 {d20,d21},[r0]
  00088948: vcvt.f32.s32 s6,s6
  0008894c: vcvt.f32.s32 s4,s4
  00088950: vcvt.f32.s32 s2,s2
  00088954: vcvt.f32.s32 s10,s10
  00088958: vcvt.f32.s32 s8,s8
  0008895c: ldr r5,[sp,#0x8]
  0008895e: str r6,[sp,#0x88]
  00088960: str r6,[sp,#0x9c]
  00088962: vmla.f32 s4,s0,s18
  00088966: mov r0,r5
  00088968: vmla.f32 s2,s6,s16
  0008896c: str r6,[sp,#0xc0]
  0008896e: vmul.f32 s0,s10,s18
  00088972: vmul.f32 s6,s8,s16
  00088976: vstr.32 s4,[sp,#0x154]
  0008897a: vstr.32 s2,[sp,#0x164]
  0008897e: vstr.32 s0,[sp,#0x194]
  00088982: vstr.32 s6,[sp,#0x1a4]
  00088986: blx 0x0006f1a8
  0008898a: mov r8,r0
  0008898c: eor r4,r0,#0x80000000
  00088990: mov r0,r5
  00088992: blx 0x0006f49c
  00088996: vmov r2,s18
  0008899a: strd r0,r4,[sp,#0x108]
  0008899e: vmov r3,s16
  000889a2: strd r8,r0,[sp,#0x118]
  000889a6: add.w r8,sp,#0x88
  000889aa: add r0,sp,#0x4c
  000889ac: str r6,[sp,#0x130]
  000889ae: mov r1,r8
  000889b0: str r6,[sp,#0x0]
  000889b2: blx 0x0006f814
  000889b6: ldr r0,[0x00088a5c]
  000889b8: add r4,sp,#0x4c
  000889ba: add r1,sp,#0x108
  000889bc: add r2,sp,#0x188
  000889be: add r0,pc
  000889c0: ldr r5,[r0,#0x0]
  000889c2: mov r0,r4
  000889c4: blx r5
  000889c6: mov r0,r11
  000889c8: mov r1,r4
  000889ca: blx 0x0006f148
  000889ce: add r4,sp,#0x10
  000889d0: add r1,sp,#0x148
  000889d2: mov r2,r11
  000889d4: mov r0,r4
  000889d6: blx r5
  000889d8: add r6,sp,#0x4c
  000889da: mov r1,r4
  000889dc: mov r2,r8
  000889de: mov r0,r6
  000889e0: blx r5
  000889e2: mov r0,r11
  000889e4: mov r1,r6
  000889e6: blx 0x0006f148
  000889ea: mov r0,r10
  000889ec: mov r1,r11
  000889ee: blx 0x0006f55c
  000889f2: movw r0,#0xb44
  000889f6: blx 0x0006edc4
  000889fa: ldr.w r1,[r10,#0x150]
  000889fe: ldr.w r0,[r10,#0x34]
  00088a02: ldr.w r1,[r1,r9,lsl #0x2]
  00088a06: ldr r1,[r1,#0x0]
  00088a08: blx 0x0006f568
  00088a0c: movw r0,#0xb44
  00088a10: blx 0x0006edb8
  00088a14: ldr r1,[sp,#0xc]
  00088a16: ldr r0,[sp,#0x1c4]
  00088a18: ldr r1,[r1,#0x0]
  00088a1a: subs r0,r1,r0
  00088a1c: itttt eq
  00088a1e: add.eq sp,#0x1c8
  00088a20: vpop.eq {d8,d9}
  00088a24: add.eq sp,#0x4
  00088a26: pop.eq.w {r8,r9,r10,r11}
  00088a2a: it eq
  00088a2c: pop.eq {r4,r5,r6,r7,pc}
  00088a2e: blx 0x0006e824
```
