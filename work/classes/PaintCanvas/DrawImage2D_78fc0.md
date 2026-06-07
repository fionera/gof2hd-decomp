# PaintCanvas::DrawImage2D
elf 0x78fc0 body 686
Sig: undefined __thiscall DrawImage2D(PaintCanvas * this, uint param_1, int param_2, int param_3, int param_4, int param_5, uchar param_6, uchar param_7, uchar param_8)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int, int, int, unsigned char, unsigned
   char, unsigned char) */

void __thiscall
AbyssEngine::PaintCanvas::DrawImage2D
          (PaintCanvas *this,uint param_1,int param_2,int param_3,int param_4,int param_5,
          uchar param_6,uchar param_7,uchar param_8)

{
  uchar uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  Image2D *pIVar5;
  int iVar6;
  undefined4 *puVar7;
  int *piVar8;
  uint in_fpscr;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  int local_94;
  int local_90;
  float local_88 [3];
  float local_7c;
  float local_74;
  undefined4 local_70;
  float fStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined4 local_50;
  int local_4c;
  
  uVar1 = param_8;
  _param_8 = (uint)param_8;
  piVar8 = *(int **)(DAT_00089280 + 0x88fda);
  local_4c = *piVar8;
  if (param_1 < *(uint *)(this + 0x14c)) {
    pIVar5 = *(Image2D **)(*(int *)(this + 0x150) + param_1 * 4);
    if ((PaintCanvas *)(uint)(byte)pIVar5[0x14] != (PaintCanvas *)0x0) {
      RestoreImage2D((PaintCanvas *)(uint)(byte)pIVar5[0x14],pIVar5);
      pIVar5 = *(Image2D **)(*(int *)(this + 0x150) + param_1 * 4);
    }
    SetTexture((uint)this,*(uint *)(pIVar5 + 4));
    local_90 = param_2;
    if ((param_6 & 7) == 4) {
      local_94 = GetWidth();
      local_94 = local_94 >> 1;
    }
    else if ((param_6 & 7) == 2) {
      local_90 = -param_2;
      local_94 = GetWidth();
    }
    else {
      local_94 = 0;
    }
    iVar2 = param_4;
    if ((param_6 & 8) != 0) {
      iVar2 = GetWidth();
      iVar2 = iVar2 - (param_4 + param_2);
    }
    fVar9 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    iVar6 = *(int *)(**(int **)(*(int *)(this + 0x150) + param_1 * 4) + 4);
    fVar10 = *(float *)(iVar6 + 0xc);
    iVar2 = param_5;
    if ((param_6 & 0x80) != 0) {
      iVar2 = GetHeight();
      iVar2 = iVar2 - (param_5 + param_3);
      iVar6 = *(int *)(**(int **)(*(int *)(this + 0x150) + param_1 * 4) + 4);
    }
    fVar11 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
    fVar12 = *(float *)(iVar6 + 0x1c);
    iVar2 = param_3;
    if ((param_6 & 0x70) == 0x40) {
      iVar6 = GetHeight();
      iVar6 = iVar6 >> 1;
    }
    else if ((param_6 & 0x70) == 0x20) {
      iVar6 = GetHeight();
      iVar2 = -param_3;
    }
    else {
      iVar6 = 0;
    }
    if ((param_7 & 7) == 4) {
      if ((param_6 & 8) == 0) {
        iVar3 = -(param_4 >> 1);
      }
      else {
        iVar3 = GetWidth();
        iVar3 = (param_4 + param_2) - iVar3 >> 1;
      }
    }
    else if ((param_7 & 7) == 2) {
      if ((param_6 & 8) == 0) {
        iVar3 = -param_4;
      }
      else {
        iVar3 = GetWidth();
        iVar3 = (param_4 + param_2) - iVar3;
      }
    }
    else {
      iVar3 = 0;
    }
    fVar9 = fVar9 / fVar10;
    fVar11 = fVar11 / fVar12;
    if ((param_7 & 0x70) == 0x40) {
      if ((param_6 & 0x80) == 0) {
        iVar4 = -(param_5 >> 1);
      }
      else {
        iVar4 = GetHeight();
        iVar4 = (param_5 + param_3) - iVar4 >> 1;
      }
    }
    else if ((param_7 & 0x70) == 0x20) {
      if ((param_6 & 0x80) == 0) {
        iVar4 = -param_5;
      }
      else {
        iVar4 = GetHeight();
        iVar4 = (param_5 + param_3) - iVar4;
      }
    }
    else {
      iVar4 = 0;
    }
    uStack_58 = DAT_00089278;
    uStack_60 = DAT_00089270;
    uStack_68 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_64 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    local_70 = 0;
    puVar7 = (undefined4 *)((uint)local_88 | 4);
    *puVar7 = 0;
    puVar7[1] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    puVar7[2] = uStack_68;
    puVar7[3] = uStack_64;
    local_7c = (float)VectorSignedToFloat(local_90 + local_94 + iVar3,(byte)(in_fpscr >> 0x16) & 3);
    fStack_6c = (float)VectorSignedToFloat(iVar4 + iVar6 + iVar2,(byte)(in_fpscr >> 0x16) & 3);
    local_50 = 0x3f800000;
    local_88[0] = fVar9;
    if ((uVar1 & 1) != 0) {
      local_88[0] = -fVar9;
      fVar9 = (float)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
      local_7c = fVar9 + local_7c;
    }
    local_74 = fVar11;
    if ((int)(_param_8 << 0x1e) < 0) {
      local_74 = -fVar11;
      fVar9 = (float)VectorSignedToFloat(param_5,(byte)(in_fpscr >> 0x16) & 3);
      fStack_6c = fVar9 + fStack_6c;
    }
    SetWorldViewMatrix((Matrix *)this);
    glDisable(0xb44);
    MeshDraw(*(Engine **)(this + 0x34),
             (Mesh *)**(undefined4 **)(*(int *)(this + 0x150) + param_1 * 4));
    glEnable(0xb44);
  }
  if (*piVar8 == local_4c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00088fc0: push {r4,r5,r6,r7,lr}
  00088fc2: add r7,sp,#0xc
  00088fc4: push {r7,r8,r9,r10,r11}
  00088fc8: vpush {d8,d9,d10,d11}
  00088fcc: sub sp,#0x50
  00088fce: mov r5,r0
  00088fd0: ldr r0,[0x00089280]
  00088fd2: mov r4,r3
  00088fd4: mov r9,r2
  00088fd6: add r0,pc
  00088fd8: mov r11,r1
  00088fda: ldr r6,[r0,#0x0]
  00088fdc: ldr r0,[r6,#0x0]
  00088fde: str r0,[sp,#0x4c]
  00088fe0: ldr.w r0,[r5,#0x14c]
  00088fe4: cmp r0,r1
  00088fe6: bls.w 0x00089252
  00088fea: ldr.w r0,[r5,#0x150]
  00088fee: ldr.w r8,[r7,#0x10]
  00088ff2: ldr.w r1,[r0,r11,lsl #0x2]
  00088ff6: ldrb r0,[r1,#0x14]
  00088ff8: cbz r0,0x00089006
  00088ffa: blx 0x0006faf0
  00088ffe: ldr.w r0,[r5,#0x150]
  00089002: ldr.w r1,[r0,r11,lsl #0x2]
  00089006: ldr r1,[r1,#0x4]
  00089008: mov r0,r5
  0008900a: mov.w r2,#0xffffffff
  0008900e: blx 0x0006fa6c
  00089012: and r0,r8,#0x7
  00089016: str r6,[sp,#0xc]
  00089018: cmp r0,#0x4
  0008901a: beq 0x00089030
  0008901c: cmp r0,#0x2
  0008901e: bne 0x0008903a
  00089020: rsb.w r0,r9,#0x0
  00089024: str r0,[sp,#0x8]
  00089026: mov r0,r5
  00089028: blx 0x0006fafc
  0008902c: str r0,[sp,#0x4]
  0008902e: b 0x00089040
  00089030: mov r0,r5
  00089032: blx 0x0006fafc
  00089036: asrs r0,r0,#0x1
  00089038: b 0x0008903c
  0008903a: movs r0,#0x0
  0008903c: strd r0,r9,[sp,#0x4]
  00089040: ldr r6,[r7,#0x8]
  00089042: ands r0,r8,#0x8
  00089046: str r0,[sp,#0x0]
  00089048: mov r0,r6
  0008904a: beq 0x0008905a
  0008904c: mov r0,r5
  0008904e: add.w r10,r6,r9
  00089052: blx 0x0006fafc
  00089056: sub.w r0,r0,r10
  0008905a: ldr.w r1,[r5,#0x150]
  0008905e: vmov s0,r0
  00089062: ldr r2,[r7,#0xc]
  00089064: ands r10,r8,#0x80
  00089068: ldr.w r1,[r1,r11,lsl #0x2]
  0008906c: mov r0,r2
  0008906e: ldr r1,[r1,#0x0]
  00089070: vcvt.f32.s32 s16,s0
  00089074: ldr r1,[r1,#0x4]
  00089076: vldr.32 s18,[r1,#0xc]
  0008907a: beq 0x00089094
  0008907c: mov r0,r5
  0008907e: blx 0x0006fb08
  00089082: ldr.w r1,[r5,#0x150]
  00089086: ldr r2,[r7,#0xc]
  00089088: ldr.w r1,[r1,r11,lsl #0x2]
  0008908c: add r2,r4
  0008908e: subs r0,r0,r2
  00089090: ldr r1,[r1,#0x0]
  00089092: ldr r1,[r1,#0x4]
  00089094: vmov s0,r0
  00089098: ldr r2,[r7,#0x14]
  0008909a: and r0,r8,#0x70
  0008909e: vcvt.f32.s32 s20,s0
  000890a2: vldr.32 s22,[r1,#0x1c]
  000890a6: cmp r0,#0x40
  000890a8: beq 0x000890c0
  000890aa: cmp r0,#0x20
  000890ac: bne 0x000890d0
  000890ae: mov r0,r5
  000890b0: rsb.w r8,r4,#0x0
  000890b4: blx 0x0006fb08
  000890b8: mov r12,r8
  000890ba: ldr r2,[r7,#0x14]
  000890bc: mov r8,r0
  000890be: b 0x000890d6
  000890c0: mov r0,r5
  000890c2: mov r8,r2
  000890c4: blx 0x0006fb08
  000890c8: mov r2,r8
  000890ca: asr.w r8,r0, asr #0x1
  000890ce: b 0x000890d4
  000890d0: mov.w r8,#0x0
  000890d4: mov r12,r4
  000890d6: and r0,r2,#0x7
  000890da: cmp r0,#0x4
  000890dc: beq 0x00089100
  000890de: cmp r0,#0x2
  000890e0: bne 0x00089120
  000890e2: ldr r0,[sp,#0x0]
  000890e4: cbz r0,0x00089124
  000890e6: add.w r0,r6,r9
  000890ea: str r0,[sp,#0x0]
  000890ec: mov r0,r5
  000890ee: mov r6,r12
  000890f0: mov r9,r2
  000890f2: blx 0x0006fafc
  000890f6: ldr r1,[sp,#0x0]
  000890f8: mov r12,r6
  000890fa: mov r2,r9
  000890fc: subs r6,r1,r0
  000890fe: b 0x0008912e
  00089100: ldr r0,[sp,#0x0]
  00089102: cbz r0,0x00089128
  00089104: add.w r0,r6,r9
  00089108: str r0,[sp,#0x0]
  0008910a: mov r0,r5
  0008910c: mov r6,r12
  0008910e: mov r9,r2
  00089110: blx 0x0006fafc
  00089114: ldr r1,[sp,#0x0]
  00089116: mov r2,r9
  00089118: mov r12,r6
  0008911a: subs r0,r1,r0
  0008911c: asrs r6,r0,#0x1
  0008911e: b 0x0008912e
  00089120: movs r6,#0x0
  00089122: b 0x0008912e
  00089124: rsbs r6,r6
  00089126: b 0x0008912e
  00089128: movs r0,#0x0
  0008912a: sub.w r6,r0,r6, asr #0x1
  0008912e: vdiv.f32 s18,s16,s18
  00089132: ldr.w r9,[r7,#0x18]
  00089136: and r0,r2,#0x70
  0008913a: cmp r0,#0x40
  0008913c: vdiv.f32 s16,s20,s22
  00089140: beq 0x00089162
  00089142: cmp r0,#0x20
  00089144: bne 0x00089180
  00089146: ldr r0,[r7,#0xc]
  00089148: cmp.w r10,#0x0
  0008914c: beq 0x00089184
  0008914e: add.w r10,r0,r4
  00089152: mov r0,r5
  00089154: mov r4,r12
  00089156: blx 0x0006fb08
  0008915a: mov r12,r4
  0008915c: sub.w r0,r10,r0
  00089160: b 0x00089190
  00089162: cmp.w r10,#0x0
  00089166: beq 0x00089188
  00089168: ldr r0,[r7,#0xc]
  0008916a: add.w r10,r0,r4
  0008916e: mov r0,r5
  00089170: mov r4,r12
  00089172: blx 0x0006fb08
  00089176: sub.w r0,r10,r0
  0008917a: mov r12,r4
  0008917c: asrs r0,r0,#0x1
  0008917e: b 0x00089190
  00089180: movs r0,#0x0
  00089182: b 0x00089190
  00089184: rsbs r0,r0
  00089186: b 0x00089190
  00089188: ldr r1,[r7,#0xc]
  0008918a: movs r0,#0x0
  0008918c: sub.w r0,r0,r1, asr #0x1
  00089190: adr r1,[0x89270]
  00089192: vmov.i32 q8,#0x0
  00089196: add r2,sp,#0x10
  00089198: vld1.64 {d18,d19},[r1]
  0008919c: add.w r1,r2,#0x18
  000891a0: tst r9,#0x1
  000891a4: vst1.64 {d16,d17},[r1]
  000891a8: orr r1,r2,#0x4
  000891ac: vst1.32 {d16,d17},[r1]
  000891b0: ldrd r3,r1,[sp,#0x4]
  000891b4: add r1,r3
  000891b6: add.w r3,r8,r12
  000891ba: add r1,r6
  000891bc: add r0,r3
  000891be: vmov s0,r1
  000891c2: vcvt.f32.s32 s2,s0
  000891c6: vmov s0,r0
  000891ca: add.w r0,r2,#0x28
  000891ce: vcvt.f32.s32 s0,s0
  000891d2: vst1.64 {d18,d19},[r0]
  000891d6: mov.w r0,#0x3f800000
  000891da: str r0,[sp,#0x48]
  000891dc: vstr.32 s18,[sp,#0x10]
  000891e0: vstr.32 s16,[sp,#0x24]
  000891e4: vstr.32 s0,[sp,#0x2c]
  000891e8: vstr.32 s2,[sp,#0x1c]
  000891ec: beq 0x00089208
  000891ee: ldr r0,[r7,#0x8]
  000891f0: vneg.f32 s6,s18
  000891f4: vmov s4,r0
  000891f8: vcvt.f32.s32 s4,s4
  000891fc: vstr.32 s6,[sp,#0x10]
  00089200: vadd.f32 s2,s4,s2
  00089204: vstr.32 s2,[sp,#0x1c]
  00089208: ldr r6,[sp,#0xc]
  0008920a: lsls.w r0,r9,#0x1e
  0008920e: bpl 0x0008922a
  00089210: ldr r0,[r7,#0xc]
  00089212: vneg.f32 s4,s16
  00089216: vmov s2,r0
  0008921a: vcvt.f32.s32 s2,s2
  0008921e: vstr.32 s4,[sp,#0x24]
  00089222: vadd.f32 s0,s2,s0
  00089226: vstr.32 s0,[sp,#0x2c]
  0008922a: add r1,sp,#0x10
  0008922c: mov r0,r5
  0008922e: blx 0x0006f55c
  00089232: movw r0,#0xb44
  00089236: blx 0x0006edc4
  0008923a: ldr.w r1,[r5,#0x150]
  0008923e: ldr r0,[r5,#0x34]
  00089240: ldr.w r1,[r1,r11,lsl #0x2]
  00089244: ldr r1,[r1,#0x0]
  00089246: blx 0x0006f568
  0008924a: movw r0,#0xb44
  0008924e: blx 0x0006edb8
  00089252: ldr r0,[sp,#0x4c]
  00089254: ldr r1,[r6,#0x0]
  00089256: subs r0,r1,r0
  00089258: itttt eq
  0008925a: add.eq sp,#0x50
  0008925c: vpop.eq {d8,d9,d10,d11}
  00089260: add.eq sp,#0x4
  00089262: pop.eq.w {r8,r9,r10,r11}
  00089266: it eq
  00089268: pop.eq {r4,r5,r6,r7,pc}
  0008926a: blx 0x0006e824
```
