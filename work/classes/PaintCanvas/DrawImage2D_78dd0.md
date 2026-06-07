# PaintCanvas::DrawImage2D
elf 0x78dd0 body 462
Sig: undefined __stdcall DrawImage2D(uint param_1, int param_2, int param_3, uchar param_4, uchar param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int, unsigned char, unsigned char) */

void AbyssEngine::PaintCanvas::DrawImage2D
               (uint param_1,int param_2,int param_3,uchar param_4,uchar param_5)

{
  PaintCanvas *this;
  int *piVar1;
  undefined4 *puVar2;
  Image2D *pIVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  uint in_fpscr;
  double dVar8;
  uint in_stack_00000004;
  int local_70;
  int local_6c;
  undefined4 local_68 [3];
  undefined4 local_5c;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined4 local_30;
  int local_28;
  
  uVar4 = (uint)param_4;
  piVar6 = *(int **)(DAT_00088fb0 + 0x88de6);
  local_28 = *piVar6;
  if ((uint)param_2 < *(uint *)(param_1 + 0x14c)) {
    pIVar3 = *(Image2D **)(*(int *)(param_1 + 0x150) + param_2 * 4);
    this = (PaintCanvas *)(uint)(byte)pIVar3[0x14];
    if (this != (PaintCanvas *)0x0) {
      RestoreImage2D(this,pIVar3);
    }
    if ((param_5 & 7) == 4) {
      local_6c = GetWidth();
      local_6c = local_6c >> 1;
    }
    else if ((param_5 & 7) == 2) {
      local_6c = GetWidth();
      param_3 = -param_3;
    }
    else {
      local_6c = 0;
    }
    if ((param_5 & 0x70) == 0x40) {
      local_70 = GetHeight();
      local_70 = local_70 >> 1;
    }
    else if ((param_5 & 0x70) == 0x20) {
      local_70 = GetHeight();
      uVar4 = -uVar4;
    }
    else {
      local_70 = 0;
    }
    if ((in_stack_00000004 & 7) == 4) {
      dVar8 = (double)VectorSignedToFloat((int)*(float *)(*(int *)(**(int **)(*(int *)(param_1 +
                                                                                      0x150) +
                                                                             param_2 * 4) + 4) + 0xc
                                                         ),(byte)(in_fpscr >> 0x16) & 3);
      iVar7 = (int)(longlong)(dVar8 * -0.5);
    }
    else if ((in_stack_00000004 & 7) == 2) {
      iVar7 = -(int)*(float *)(*(int *)(**(int **)(*(int *)(param_1 + 0x150) + param_2 * 4) + 4) +
                              0xc);
    }
    else {
      iVar7 = 0;
    }
    if ((in_stack_00000004 & 0x70) == 0x20) {
      piVar1 = *(int **)(*(int *)(param_1 + 0x150) + param_2 * 4);
      iVar5 = -(int)*(float *)(*(int *)(*piVar1 + 4) + 0x1c);
    }
    else if ((in_stack_00000004 & 0x70) == 0x40) {
      piVar1 = *(int **)(*(int *)(param_1 + 0x150) + param_2 * 4);
      dVar8 = (double)VectorSignedToFloat((int)*(float *)(*(int *)(*piVar1 + 4) + 0x1c),
                                          (byte)(in_fpscr >> 0x16) & 3);
      iVar5 = (int)(longlong)(dVar8 * -0.5);
    }
    else {
      iVar5 = 0;
      piVar1 = *(int **)(*(int *)(param_1 + 0x150) + param_2 * 4);
    }
    SetTexture(param_1,piVar1[1]);
    uStack_38 = DAT_00088fa8;
    uStack_40 = DAT_00088fa0;
    uStack_48 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_44 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    local_5c = VectorSignedToFloat(local_6c + param_3 + iVar7,(byte)(in_fpscr >> 0x16) & 3);
    puVar2 = (undefined4 *)((uint)local_68 | 4);
    uStack_4c = VectorSignedToFloat(local_70 + uVar4 + iVar5,(byte)(in_fpscr >> 0x16) & 3);
    local_50 = 0;
    *puVar2 = 0;
    puVar2[1] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    puVar2[2] = uStack_48;
    puVar2[3] = uStack_44;
    local_30 = 0x3f800000;
    local_68[0] = 0x3f800000;
    local_54 = 0x3f800000;
    SetWorldViewMatrix((Matrix *)param_1);
    MeshDraw(*(Engine **)(param_1 + 0x34),
             (Mesh *)**(undefined4 **)(*(int *)(param_1 + 0x150) + param_2 * 4));
  }
  if (*piVar6 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00088dd0: push {r4,r5,r6,r7,lr}
  00088dd2: add r7,sp,#0xc
  00088dd4: push {r8,r9,r10,r11}
  00088dd8: sub sp,#0x4c
  00088dda: mov r5,r0
  00088ddc: ldr r0,[0x00088fb0]
  00088dde: mov r11,r3
  00088de0: mov r8,r2
  00088de2: add r0,pc
  00088de4: mov r4,r1
  00088de6: ldr.w r9,[r0,#0x0]
  00088dea: ldr.w r0,[r9,#0x0]
  00088dee: str r0,[sp,#0x48]
  00088df0: ldr.w r0,[r5,#0x14c]
  00088df4: cmp r0,r1
  00088df6: bls.w 0x00088f88
  00088dfa: ldr.w r0,[r5,#0x150]
  00088dfe: ldr.w r10,[r7,#0x8]
  00088e02: ldr.w r1,[r0,r4,lsl #0x2]
  00088e06: ldrb r0,[r1,#0x14]
  00088e08: cbz r0,0x00088e0e
  00088e0a: blx 0x0006faf0
  00088e0e: and r0,r10,#0x7
  00088e12: cmp r0,#0x4
  00088e14: beq 0x00088e28
  00088e16: cmp r0,#0x2
  00088e18: bne 0x00088e32
  00088e1a: mov r0,r5
  00088e1c: blx 0x0006fafc
  00088e20: str r0,[sp,#0x4]
  00088e22: rsb.w r8,r8,#0x0
  00088e26: b 0x00088e36
  00088e28: mov r0,r5
  00088e2a: blx 0x0006fafc
  00088e2e: asrs r0,r0,#0x1
  00088e30: b 0x00088e34
  00088e32: movs r0,#0x0
  00088e34: str r0,[sp,#0x4]
  00088e36: ldr r6,[r7,#0xc]
  00088e38: and r0,r10,#0x70
  00088e3c: cmp r0,#0x40
  00088e3e: beq 0x00088e52
  00088e40: cmp r0,#0x20
  00088e42: bne 0x00088e5c
  00088e44: mov r0,r5
  00088e46: blx 0x0006fb08
  00088e4a: str r0,[sp,#0x0]
  00088e4c: rsb.w r11,r11,#0x0
  00088e50: b 0x00088e60
  00088e52: mov r0,r5
  00088e54: blx 0x0006fb08
  00088e58: asrs r0,r0,#0x1
  00088e5a: b 0x00088e5e
  00088e5c: movs r0,#0x0
  00088e5e: str r0,[sp,#0x0]
  00088e60: and r0,r6,#0x7
  00088e64: cmp r0,#0x4
  00088e66: beq 0x00088e8a
  00088e68: cmp r0,#0x2
  00088e6a: bne 0x00088eb4
  00088e6c: ldr.w r0,[r5,#0x150]
  00088e70: ldr.w r0,[r0,r4,lsl #0x2]
  00088e74: ldr r0,[r0,#0x0]
  00088e76: ldr r0,[r0,#0x4]
  00088e78: vldr.32 s0,[r0,#0xc]
  00088e7c: vcvt.s32.f32 s0,s0
  00088e80: vmov r0,s0
  00088e84: rsb.w r10,r0,#0x0
  00088e88: b 0x00088eb8
  00088e8a: ldr.w r0,[r5,#0x150]
  00088e8e: vmov.f64 d17,-0x4020000000000000
  00088e92: ldr.w r0,[r0,r4,lsl #0x2]
  00088e96: ldr r0,[r0,#0x0]
  00088e98: ldr r0,[r0,#0x4]
  00088e9a: vldr.32 s0,[r0,#0xc]
  00088e9e: vcvt.s32.f32 s0,s0
  00088ea2: vcvt.f64.s32 d16,s0
  00088ea6: vmul.f64 d16,d16,d17
  00088eaa: vcvt.s32.f64 s0,d16
  00088eae: vmov r10,s0
  00088eb2: b 0x00088eb8
  00088eb4: mov.w r10,#0x0
  00088eb8: and r0,r6,#0x70
  00088ebc: cmp r0,#0x20
  00088ebe: beq 0x00088eee
  00088ec0: cmp r0,#0x40
  00088ec2: bne 0x00088f0a
  00088ec4: ldr.w r0,[r5,#0x150]
  00088ec8: vmov.f64 d17,-0x4020000000000000
  00088ecc: ldr.w r0,[r0,r4,lsl #0x2]
  00088ed0: ldr r1,[r0,#0x0]
  00088ed2: ldr r1,[r1,#0x4]
  00088ed4: vldr.32 s0,[r1,#0x1c]
  00088ed8: vcvt.s32.f32 s0,s0
  00088edc: vcvt.f64.s32 d16,s0
  00088ee0: vmul.f64 d16,d16,d17
  00088ee4: vcvt.s32.f64 s0,d16
  00088ee8: vmov r6,s0
  00088eec: b 0x00088f14
  00088eee: ldr.w r0,[r5,#0x150]
  00088ef2: ldr.w r0,[r0,r4,lsl #0x2]
  00088ef6: ldr r1,[r0,#0x0]
  00088ef8: ldr r1,[r1,#0x4]
  00088efa: vldr.32 s0,[r1,#0x1c]
  00088efe: vcvt.s32.f32 s0,s0
  00088f02: vmov r1,s0
  00088f06: rsbs r6,r1
  00088f08: b 0x00088f14
  00088f0a: ldr.w r0,[r5,#0x150]
  00088f0e: movs r6,#0x0
  00088f10: ldr.w r0,[r0,r4,lsl #0x2]
  00088f14: ldr r1,[r0,#0x4]
  00088f16: mov r0,r5
  00088f18: mov.w r2,#0xffffffff
  00088f1c: blx 0x0006fa6c
  00088f20: adr r0,[0x88fa0]
  00088f22: ldr r2,[sp,#0x4]
  00088f24: vld1.64 {d18,d19},[r0]
  00088f28: vmov.i32 q8,#0x0
  00088f2c: add r2,r8
  00088f2e: add r1,sp,#0x8
  00088f30: ldr r0,[sp,#0x0]
  00088f32: add r2,r10
  00088f34: add r0,r11
  00088f36: vmov s0,r2
  00088f3a: add r0,r6
  00088f3c: add.w r2,r1,#0x18
  00088f40: vcvt.f32.s32 s0,s0
  00088f44: vmov s2,r0
  00088f48: orr r0,r1,#0x4
  00088f4c: vcvt.f32.s32 s2,s2
  00088f50: vst1.64 {d16,d17},[r2]
  00088f54: vst1.32 {d16,d17},[r0]
  00088f58: add.w r0,r1,#0x28
  00088f5c: vst1.64 {d18,d19},[r0]
  00088f60: mov.w r0,#0x3f800000
  00088f64: str r0,[sp,#0x40]
  00088f66: str r0,[sp,#0x8]
  00088f68: str r0,[sp,#0x1c]
  00088f6a: mov r0,r5
  00088f6c: vstr.32 s2,[sp,#0x24]
  00088f70: vstr.32 s0,[sp,#0x14]
  00088f74: blx 0x0006f55c
  00088f78: ldr.w r1,[r5,#0x150]
  00088f7c: ldr r0,[r5,#0x34]
  00088f7e: ldr.w r1,[r1,r4,lsl #0x2]
  00088f82: ldr r1,[r1,#0x0]
  00088f84: blx 0x0006f568
  00088f88: ldr r0,[sp,#0x48]
  00088f8a: ldr.w r1,[r9,#0x0]
  00088f8e: subs r0,r1,r0
  00088f90: ittt eq
  00088f92: add.eq sp,#0x4c
  00088f94: pop.eq.w {r8,r9,r10,r11}
  00088f98: pop.eq {r4,r5,r6,r7,pc}
  00088f9a: blx 0x0006e824
```
