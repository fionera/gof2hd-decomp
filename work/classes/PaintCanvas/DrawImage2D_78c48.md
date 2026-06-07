# PaintCanvas::DrawImage2D
elf 0x78c48 body 312
Sig: undefined __thiscall DrawImage2D(PaintCanvas * this, uint param_1, int param_2, int param_3, uchar param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawImage2D(unsigned int, int, int, unsigned char) */

void __thiscall
AbyssEngine::PaintCanvas::DrawImage2D
          (PaintCanvas *this,uint param_1,int param_2,int param_3,uchar param_4)

{
  uchar uVar1;
  Image2D *pIVar2;
  undefined4 *puVar3;
  int *piVar4;
  uint in_fpscr;
  float fVar5;
  undefined4 local_60 [3];
  float local_54;
  undefined4 local_4c;
  undefined4 local_48;
  float fStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined4 local_28;
  int local_24;
  
  uVar1 = param_4;
  _param_4 = (uint)param_4;
  piVar4 = *(int **)(DAT_00088d98 + 0x88c5e);
  local_24 = *piVar4;
  if (param_1 < *(uint *)(this + 0x14c)) {
    pIVar2 = *(Image2D **)(*(int *)(this + 0x150) + param_1 * 4);
    if ((PaintCanvas *)(uint)(byte)pIVar2[0x14] != (PaintCanvas *)0x0) {
      RestoreImage2D((PaintCanvas *)(uint)(byte)pIVar2[0x14],pIVar2);
      pIVar2 = *(Image2D **)(*(int *)(this + 0x150) + param_1 * 4);
    }
    SetTexture((uint)this,*(uint *)(pIVar2 + 4));
    uStack_30 = DAT_00088d88;
    uStack_38 = DAT_00088d80;
    uStack_40 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uStack_3c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    local_54 = (float)VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
    fStack_44 = (float)VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
    local_48 = 0;
    puVar3 = (undefined4 *)((uint)local_60 | 4);
    *puVar3 = 0;
    puVar3[1] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    puVar3[2] = uStack_40;
    puVar3[3] = uStack_3c;
    local_28 = 0x3f800000;
    local_60[0] = 0x3f800000;
    local_4c = 0x3f800000;
    if ((uVar1 & 1) != 0) {
      local_60[0] = DAT_00088d90;
      fVar5 = DAT_00088d94;
      if (param_1 < *(uint *)(this + 0x14c)) {
        fVar5 = (float)VectorUnsignedToFloat
                                 ((uint)*(ushort *)
                                         (*(int *)(*(int *)(this + 0x150) + param_1 * 4) + 0x10),
                                  (byte)(in_fpscr >> 0x16) & 3);
      }
      local_54 = fVar5 + local_54;
    }
    if ((int)(_param_4 << 0x1e) < 0) {
      local_4c = DAT_00088d90;
      fVar5 = DAT_00088d94;
      if (param_1 < *(uint *)(this + 0x14c)) {
        fVar5 = (float)VectorUnsignedToFloat
                                 ((uint)*(ushort *)
                                         (*(int *)(*(int *)(this + 0x150) + param_1 * 4) + 0x12),
                                  (byte)(in_fpscr >> 0x16) & 3);
      }
      fStack_44 = fVar5 + fStack_44;
    }
    SetWorldViewMatrix((Matrix *)this);
    glDisable(0xb44);
    MeshDraw(*(Engine **)(this + 0x34),
             (Mesh *)**(undefined4 **)(*(int *)(this + 0x150) + param_1 * 4));
    glEnable(0xb44);
  }
  if (*piVar4 == local_24) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00088c48: push {r4,r5,r6,r7,lr}
  00088c4a: add r7,sp,#0xc
  00088c4c: push {r8,r9,r10}
  00088c50: sub sp,#0x40
  00088c52: mov r5,r0
  00088c54: ldr r0,[0x00088d98]
  00088c56: mov r8,r3
  00088c58: mov r6,r2
  00088c5a: add r0,pc
  00088c5c: mov r4,r1
  00088c5e: ldr.w r9,[r0,#0x0]
  00088c62: ldr.w r0,[r9,#0x0]
  00088c66: str r0,[sp,#0x3c]
  00088c68: ldr.w r0,[r5,#0x14c]
  00088c6c: cmp r0,r1
  00088c6e: bls 0x00088d6a
  00088c70: ldr.w r0,[r5,#0x150]
  00088c74: ldr.w r10,[r7,#0x8]
  00088c78: ldr.w r1,[r0,r4,lsl #0x2]
  00088c7c: ldrb r0,[r1,#0x14]
  00088c7e: cbz r0,0x00088c8c
  00088c80: blx 0x0006faf0
  00088c84: ldr.w r0,[r5,#0x150]
  00088c88: ldr.w r1,[r0,r4,lsl #0x2]
  00088c8c: ldr r1,[r1,#0x4]
  00088c8e: mov r0,r5
  00088c90: mov.w r2,#0xffffffff
  00088c94: blx 0x0006fa6c
  00088c98: vmov s0,r6
  00088c9c: adr r0,[0x88d80]
  00088c9e: vmov.i32 q8,#0x0
  00088ca2: vld1.64 {d18,d19},[r0]
  00088ca6: vcvt.f32.s32 s2,s0
  00088caa: mov r0,sp
  00088cac: vmov s0,r8
  00088cb0: add.w r1,r0,#0x18
  00088cb4: tst r10,#0x1
  00088cb8: vcvt.f32.s32 s0,s0
  00088cbc: vst1.64 {d16,d17},[r1]
  00088cc0: orr r1,r0,#0x4
  00088cc4: add.w r0,r0,#0x28
  00088cc8: vst1.32 {d16,d17},[r1]
  00088ccc: vst1.64 {d18,d19},[r0]
  00088cd0: mov.w r0,#0x3f800000
  00088cd4: str r0,[sp,#0x38]
  00088cd6: str r0,[sp,#0x0]
  00088cd8: str r0,[sp,#0x14]
  00088cda: vstr.32 s0,[sp,#0x1c]
  00088cde: vstr.32 s2,[sp,#0xc]
  00088ce2: beq 0x00088d10
  00088ce4: ldr r0,[0x00088d90]
  00088ce6: str r0,[sp,#0x0]
  00088ce8: ldr.w r0,[r5,#0x14c]
  00088cec: cmp r0,r4
  00088cee: bls 0x00088d04
  00088cf0: ldr.w r0,[r5,#0x150]
  00088cf4: ldr.w r0,[r0,r4,lsl #0x2]
  00088cf8: ldrh r0,[r0,#0x10]
  00088cfa: vmov s4,r0
  00088cfe: vcvt.f32.u32 s4,s4
  00088d02: b 0x00088d08
  00088d04: vldr.32 s4,[pc,#0x8c]
  00088d08: vadd.f32 s2,s4,s2
  00088d0c: vstr.32 s2,[sp,#0xc]
  00088d10: lsls.w r0,r10,#0x1e
  00088d14: bpl 0x00088d42
  00088d16: ldr r0,[0x00088d90]
  00088d18: str r0,[sp,#0x14]
  00088d1a: ldr.w r0,[r5,#0x14c]
  00088d1e: cmp r0,r4
  00088d20: bls 0x00088d36
  00088d22: ldr.w r0,[r5,#0x150]
  00088d26: ldr.w r0,[r0,r4,lsl #0x2]
  00088d2a: ldrh r0,[r0,#0x12]
  00088d2c: vmov s2,r0
  00088d30: vcvt.f32.u32 s2,s2
  00088d34: b 0x00088d3a
  00088d36: vldr.32 s2,[pc,#0x5c]
  00088d3a: vadd.f32 s0,s2,s0
  00088d3e: vstr.32 s0,[sp,#0x1c]
  00088d42: mov r1,sp
  00088d44: mov r0,r5
  00088d46: blx 0x0006f55c
  00088d4a: movw r0,#0xb44
  00088d4e: blx 0x0006edc4
  00088d52: ldr.w r1,[r5,#0x150]
  00088d56: ldr r0,[r5,#0x34]
  00088d58: ldr.w r1,[r1,r4,lsl #0x2]
  00088d5c: ldr r1,[r1,#0x0]
  00088d5e: blx 0x0006f568
  00088d62: movw r0,#0xb44
  00088d66: blx 0x0006edb8
  00088d6a: ldr r0,[sp,#0x3c]
  00088d6c: ldr.w r1,[r9,#0x0]
  00088d70: subs r0,r1,r0
  00088d72: ittt eq
  00088d74: add.eq sp,#0x40
  00088d76: pop.eq.w {r8,r9,r10}
  00088d7a: pop.eq {r4,r5,r6,r7,pc}
  00088d7c: blx 0x0006e824
```
