# AbyssEngine::ImageFontDrawString
elf 0x71bc0 body 1366
Sig: undefined __stdcall ImageFontDrawString(ImageFont * param_1, ushort * param_2, uint param_3, int param_4, int param_5, PaintCanvas * param_6, Engine * param_7, bool param_8)

## decompile
```c

/* AbyssEngine::ImageFontDrawString(AbyssEngine::ImageFont*, unsigned short const*, unsigned int,
   int, int, AbyssEngine::PaintCanvas*, AbyssEngine::Engine*, bool) */

void AbyssEngine::ImageFontDrawString
               (ImageFont *param_1,ushort *param_2,uint param_3,int param_4,int param_5,
               PaintCanvas *param_6,Engine *param_7,bool param_8)

{
  bool bVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  ushort uVar11;
  int iVar12;
  float *pfVar13;
  int iVar14;
  uint uVar15;
  ushort uVar16;
  float *pfVar17;
  int iVar18;
  uint in_fpscr;
  float fVar19;
  float fVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  undefined4 uVar24;
  undefined3 in_stack_0000000d;
  undefined4 local_90 [3];
  undefined4 local_84;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined4 local_58;
  int local_54;
  
  piVar4 = *(int **)(DAT_00081fb0 + 0x81be4);
  local_54 = *piVar4;
  bVar1 = param_2 != (ushort *)0x0 && param_1 != (ImageFont *)0x0;
  if (param_7[0xfc] == (Engine)0x0) {
    if ((bVar1) && (iVar5 = ImageFontGetWidth(param_1,param_2,param_3), -1 < iVar5 + param_4)) {
      iVar5 = (int)*(short *)(param_1 + 0x12);
      iVar6 = ImageFontGetHeight(param_1);
      iVar7 = Engine::GetDisplayWidth();
      iVar8 = iVar7;
      if (param_4 <= iVar7) {
        iVar5 = iVar5 + param_5;
        iVar8 = iVar6 + iVar5;
      }
      iVar6 = iVar7 - param_4;
      if (iVar7 >= param_4) {
        iVar6 = iVar8;
      }
      if ((iVar6 < 0 == (iVar7 < param_4 && SBORROW4(iVar7,param_4))) &&
         (iVar8 = Engine::GetDisplayHeight(), uVar3 = DAT_00082128, uVar2 = DAT_00082120,
         iVar5 <= iVar8)) {
        uStack_70 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        uStack_6c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        puVar9 = (undefined4 *)((uint)local_90 | 4);
        local_84 = VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
        iVar8 = -1;
        uVar21 = VectorSignedToFloat(iVar5 + -2,(byte)(in_fpscr >> 0x16) & 3);
        *puVar9 = 0;
        puVar9[1] = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        puVar9[2] = uStack_70;
        puVar9[3] = uStack_6c;
        uVar16 = 0;
        local_78 = 0;
        uStack_68 = uVar2;
        uStack_60 = uVar3;
        local_58 = 0x3f800000;
        local_90[0] = 0x3f800000;
        local_7c = 0x3f800000;
        iVar5 = param_3 - 1;
        uStack_74 = uVar21;
        if (param_6[0x1c] != (PaintCanvas)0x0 || _param_8 != 0) {
          iVar8 = 1;
          iVar5 = 0;
        }
        for (; uVar16 < param_3; uVar16 = uVar16 + 1) {
          uVar11 = 0;
          do {
            uVar15 = (uint)uVar11;
            if (*(ushort *)param_1 <= uVar15) goto LAB_0008210e;
            uVar11 = uVar11 + 1;
          } while (*(ushort *)(*(int *)(param_1 + 4) + uVar15 * 2) != param_2[iVar5]);
          local_84 = VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
          iVar6 = (int)*(float *)(*(int *)(*(int *)(*(int *)(param_1 + 0xc) + uVar15 * 4) + 4) + 0xc
                                 );
          uStack_74 = uVar21;
          if ((-1 < param_4 + iVar6) && (iVar7 = Engine::GetDisplayWidth(), param_4 <= iVar7)) {
            PaintCanvas::SetWorldViewMatrix((Matrix *)param_6);
            MeshDraw(param_7,*(Mesh **)(*(int *)(param_1 + 0xc) + uVar15 * 4));
          }
          iVar12 = *(short *)(param_1 + 0x10) + iVar6;
          iVar7 = iVar12;
          if (param_2[iVar5] == 0x20) {
            iVar7 = iVar12 + -2;
          }
          if (iVar6 != 0xb) {
            iVar7 = iVar12;
          }
          param_4 = param_4 + iVar7;
LAB_0008210e:
          iVar5 = iVar5 + iVar8;
        }
      }
    }
  }
  else if ((bVar1) && (iVar5 = ImageFontGetWidth(param_1,param_2,param_3), -1 < iVar5 + param_4)) {
    iVar5 = (int)*(short *)(param_1 + 0x12);
    iVar6 = ImageFontGetHeight(param_1);
    iVar7 = Engine::GetDisplayWidth();
    iVar8 = iVar7;
    if (param_4 <= iVar7) {
      iVar5 = iVar5 + param_5;
      iVar8 = iVar6 + iVar5;
    }
    iVar6 = iVar7 - param_4;
    if (iVar7 >= param_4) {
      iVar6 = iVar8;
    }
    if ((iVar6 < 0 == (iVar7 < param_4 && SBORROW4(iVar7,param_4))) &&
       (iVar8 = Engine::GetDisplayHeight(), iVar5 <= iVar8)) {
      iVar8 = -1;
      if (param_6[0x1c] != (PaintCanvas)0x0 || _param_8 != 0) {
        iVar8 = 1;
      }
      iVar6 = param_3 - 1;
      if (param_6[0x1c] != (PaintCanvas)0x0 || _param_8 != 0) {
        iVar6 = 0;
      }
      if (((**(char **)(DAT_00081fb4 + 0x81c5c) != '\0') &&
          (iVar7 = GameText::getLanguage(), iVar7 == 9)) &&
         (iVar7 = GameText::isNonArabicString(param_2,param_3), iVar7 != 0)) {
        iVar6 = 0;
        iVar8 = 1;
      }
      uVar3 = DAT_00082128;
      uVar2 = DAT_00082120;
      uVar21 = 0;
      uVar22 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
      uVar23 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
      uVar24 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
      uVar16 = 0;
      fVar20 = (float)VectorSignedToFloat(iVar5 + -2,(byte)(in_fpscr >> 0x16) & 3);
      puVar9 = (undefined4 *)((uint)local_90 | 4);
      for (; uVar16 < param_3; uVar16 = uVar16 + 1) {
        uVar11 = 0;
        do {
          uVar15 = (uint)uVar11;
          if (*(ushort *)param_1 <= uVar15) goto LAB_00081f52;
          uVar11 = uVar11 + 1;
        } while (*(ushort *)(*(int *)(param_1 + 4) + uVar15 * 2) != param_2[iVar6]);
        iVar5 = (int)*(float *)(*(int *)(*(int *)(*(int *)(param_1 + 0xc) + uVar15 * 4) + 4) + 0xc);
        if ((-1 < param_4 + iVar5) && (iVar7 = Engine::GetDisplayWidth(), param_4 <= iVar7)) {
          iVar18 = *(int *)(param_6 + 8);
          iVar7 = *(int *)(param_6 + 0xc);
          iVar12 = *(int *)(*(int *)(param_1 + 0xc) + uVar15 * 4);
          fVar19 = (float)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
          iVar14 = *(int *)(iVar18 + 4);
          pfVar13 = *(float **)(iVar12 + 4);
          pfVar17 = (float *)(iVar14 + iVar7 * 0x30);
          *pfVar17 = *pfVar13 + fVar19;
          *(float *)((iVar7 * 0x30 | 4U) + iVar14) = pfVar13[1] + fVar20;
          *(float *)((iVar7 * 0x30 | 0xcU) + iVar14) = pfVar13[3] + fVar19;
          pfVar17[4] = pfVar13[4] + fVar20;
          pfVar17[6] = pfVar13[6] + fVar19;
          pfVar17[7] = pfVar13[7] + fVar20;
          pfVar17[9] = pfVar13[9] + fVar19;
          pfVar17[10] = pfVar13[10] + fVar20;
          puVar10 = *(undefined4 **)(iVar12 + 8);
          iVar12 = *(int *)(iVar18 + 8);
          *(undefined4 *)(iVar12 + iVar7 * 0x20) = *puVar10;
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 4U)) = puVar10[1];
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 8U)) = puVar10[2];
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 0xcU)) = puVar10[3];
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 0x10U)) = puVar10[4];
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 0x14U)) = puVar10[5];
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 0x18U)) = puVar10[6];
          *(undefined4 *)(iVar12 + (iVar7 << 5 | 0x1cU)) = puVar10[7];
          iVar12 = *(int *)(iVar18 + 0xc);
          *(undefined4 *)(iVar12 + iVar7 * 0x40) = *(undefined4 *)(param_7 + 0xd0);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 4U)) = *(undefined4 *)(param_7 + 0xd4);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 8U)) = *(undefined4 *)(param_7 + 0xd8);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0xcU)) = *(undefined4 *)(param_7 + 0xdc);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x10U)) = *(undefined4 *)(param_7 + 0xd0);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x14U)) = *(undefined4 *)(param_7 + 0xd4);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x18U)) = *(undefined4 *)(param_7 + 0xd8);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x1cU)) = *(undefined4 *)(param_7 + 0xdc);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x20U)) = *(undefined4 *)(param_7 + 0xd0);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x24U)) = *(undefined4 *)(param_7 + 0xd4);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x28U)) = *(undefined4 *)(param_7 + 0xd8);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x2cU)) = *(undefined4 *)(param_7 + 0xdc);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x30U)) = *(undefined4 *)(param_7 + 0xd0);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x34U)) = *(undefined4 *)(param_7 + 0xd4);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x38U)) = *(undefined4 *)(param_7 + 0xd8);
          *(undefined4 *)(iVar12 + (iVar7 << 6 | 0x3cU)) = *(undefined4 *)(param_7 + 0xdc);
          *(int *)(param_6 + 0xc) = iVar7 + 1;
          if (0x62 < iVar7) {
            *(short *)(iVar18 + 0x28) = (short)(iVar7 + 1) * 6;
            local_90[0] = 0x3f800000;
            *puVar9 = uVar21;
            puVar9[1] = uVar22;
            puVar9[2] = uVar23;
            puVar9[3] = uVar24;
            local_7c = 0x3f800000;
            uStack_68 = uVar2;
            uStack_60 = uVar3;
            local_58 = 0x3f800000;
            local_78 = uVar21;
            uStack_74 = uVar22;
            uStack_70 = uVar23;
            uStack_6c = uVar24;
            PaintCanvas::SetWorldViewMatrix((Matrix *)param_6);
            MeshDraw(param_7,*(Mesh **)(param_6 + 8));
            *(undefined4 *)(param_6 + 0xc) = 0;
          }
        }
        iVar12 = *(short *)(param_1 + 0x10) + iVar5;
        iVar7 = iVar12;
        if (param_2[iVar6] == 0x20) {
          iVar7 = iVar12 + -2;
        }
        if (iVar5 != 0xb) {
          iVar7 = iVar12;
        }
        param_4 = param_4 + iVar7;
LAB_00081f52:
        iVar6 = iVar6 + iVar8;
      }
      iVar5 = *(int *)(param_6 + 0xc);
      if (0 < iVar5) {
        uStack_74 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        uStack_70 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        uStack_6c = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        *(short *)(*(int *)(param_6 + 8) + 0x28) = ((short)iVar5 + (short)(iVar5 << 1)) * 2;
        *puVar9 = 0;
        puVar9[1] = uStack_74;
        puVar9[2] = uStack_70;
        puVar9[3] = uStack_6c;
        local_78 = 0;
        uStack_68 = uVar2;
        uStack_60 = uVar3;
        local_90[0] = 0x3f800000;
        local_7c = 0x3f800000;
        local_58 = 0x3f800000;
        PaintCanvas::SetWorldViewMatrix((Matrix *)param_6);
        MeshDraw(param_7,*(Mesh **)(param_6 + 8));
        *(undefined4 *)(param_6 + 0xc) = 0;
      }
    }
  }
  if (*piVar4 == local_54) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00081bc0: push {r4,r5,r6,r7,lr}
  00081bc2: add r7,sp,#0xc
  00081bc4: push {r7,r8,r9,r10,r11}
  00081bc8: vpush {d10,d11,d12,d13}
  00081bcc: vpush {d8}
  00081bd0: sub sp,#0x60
  00081bd2: str r2,[sp,#0x1c]
  00081bd4: mov r4,r0
  00081bd6: ldr r0,[0x00081fb0]
  00081bd8: mov r5,r1
  00081bda: cmp r1,#0x0
  00081bdc: ldr.w r9,[r7,#0x8]
  00081be0: add r0,pc
  00081be2: mov r10,r3
  00081be4: ldr r0,[r0,#0x0]
  00081be6: str r0,[sp,#0x18]
  00081be8: ldr r0,[r0,#0x0]
  00081bea: ldrd r2,r8,[r7,#0x10]
  00081bee: str r0,[sp,#0x5c]
  00081bf0: mov r0,r1
  00081bf2: mov r1,r4
  00081bf4: it ne
  00081bf6: mov.ne r0,#0x1
  00081bf8: cmp r4,#0x0
  00081bfa: it ne
  00081bfc: mov.ne r1,#0x1
  00081bfe: ldrb.w r2,[r2,#0xfc]
  00081c02: ands r0,r1
  00081c04: cmp r2,#0x0
  00081c06: beq.w 0x00081fb8
  00081c0a: cmp r0,#0x0
  00081c0c: beq.w 0x00081fea
  00081c10: ldr r2,[sp,#0x1c]
  00081c12: mov r0,r4
  00081c14: mov r1,r5
  00081c16: blx 0x0006f52c
  00081c1a: add r0,r10
  00081c1c: cmp r0,#0x0
  00081c1e: blt.w 0x00081fea
  00081c22: mov r0,r4
  00081c24: ldrsh.w r6,[r4,#0x12]
  00081c28: blx 0x0006f538
  00081c2c: mov r11,r0
  00081c2e: ldr r0,[r7,#0x10]
  00081c30: blx 0x0006e8cc
  00081c34: cmp r0,r10
  00081c36: ittt ge
  00081c38: add.ge r6,r9
  00081c3a: add.ge.w r0,r11,r6
  00081c3e: cmp.ge r0,#0x0
  00081c40: blt.w 0x00081fea
  00081c44: ldr r0,[r7,#0x10]
  00081c46: blx 0x0006e8d8
  00081c4a: cmp r6,r0
  00081c4c: bgt.w 0x00081fea
  00081c50: ldr r0,[0x00081fb4]
  00081c52: mov.w r9,#0xffffffff
  00081c56: ldr r1,[r7,#0xc]
  00081c58: add r0,pc
  00081c5a: ldrb r1,[r1,#0x1c]
  00081c5c: ldr r0,[r0,#0x0]
  00081c5e: orrs.w r1,r1,r8
  00081c62: it ne
  00081c64: mov.ne.w r9,#0x1
  00081c68: ldr r1,[sp,#0x1c]
  00081c6a: ldrb r0,[r0,#0x0]
  00081c6c: sub.w r8,r1,#0x1
  00081c70: it ne
  00081c72: mov.ne.w r8,#0x0
  00081c76: cbz r0,0x00081c94
  00081c78: blx 0x0006f544
  00081c7c: cmp r0,#0x9
  00081c7e: bne 0x00081c94
  00081c80: ldr r1,[sp,#0x1c]
  00081c82: mov r0,r5
  00081c84: blx 0x0006f550
  00081c88: cmp r0,#0x0
  00081c8a: itt ne
  00081c8c: mov.ne.w r8,#0x0
  00081c90: mov.ne.w r9,#0x1
  00081c94: subs r0,r6,#0x2
  00081c96: vmov.i32 q6,#0x0
  00081c9a: mov.w r11,#0x0
  00081c9e: vmov s0,r0
  00081ca2: add r0,sp,#0x20
  00081ca4: add.w r1,r0,#0x28
  00081ca8: vcvt.f32.s32 s16,s0
  00081cac: str r1,[sp,#0xc]
  00081cae: add.w r1,r0,#0x18
  00081cb2: orr r0,r0,#0x4
  00081cb6: str r0,[sp,#0x4]
  00081cb8: adr.w r0,0x82120
  00081cbc: vld1.64 {d10,d11},[r0]
  00081cc0: str r1,[sp,#0x8]
  00081cc2: str.w r9,[sp,#0x14]
  00081cc6: b 0x00081f58
  00081cc8: ldrh r0,[r4,#0x0]
  00081cca: movs r1,#0x0
  00081ccc: uxth r6,r1
  00081cce: cmp r6,r0
  00081cd0: bcs.w 0x00081f52
  00081cd4: ldr r2,[r4,#0x4]
  00081cd6: adds r1,#0x1
  00081cd8: ldrh.w r3,[r5,r8,lsl #0x1]
  00081cdc: ldrh.w r2,[r2,r6,lsl #0x1]
  00081ce0: cmp r2,r3
  00081ce2: bne 0x00081ccc
  00081ce4: ldr r0,[r4,#0xc]
  00081ce6: ldr.w r0,[r0,r6,lsl #0x2]
  00081cea: ldr r0,[r0,#0x4]
  00081cec: vldr.32 s0,[r0,#0xc]
  00081cf0: vcvt.s32.f32 s0,s0
  00081cf4: vmov r9,s0
  00081cf8: add.w r0,r10,r9
  00081cfc: cmp r0,#0x0
  00081cfe: blt.w 0x00081f32
  00081d02: ldr r0,[r7,#0x10]
  00081d04: blx 0x0006e8cc
  00081d08: cmp r10,r0
  00081d0a: bgt.w 0x00081f32
  00081d0e: ldr r2,[r4,#0xc]
  00081d10: vmov s0,r10
  00081d14: ldr r0,[r7,#0xc]
  00081d16: ldrd lr,r1,[r0,#0x8]
  00081d1a: movs r0,#0x4
  00081d1c: ldr.w r2,[r2,r6,lsl #0x2]
  00081d20: add.w r6,r1,r1, lsl #0x1
  00081d24: cmp r1,#0x63
  00081d26: str r2,[sp,#0x10]
  00081d28: vcvt.f32.s32 s0,s0
  00081d2c: ldr.w r3,[lr,#0x4]
  00081d30: orr.w r0,r0,r6, lsl #0x4
  00081d34: ldr r2,[r2,#0x4]
  00081d36: add.w r12,r3,r6, lsl #0x4
  00081d3a: add r0,r3
  00081d3c: vldr.32 s2,[r2]
  00081d40: vadd.f32 s2,s2,s0
  00081d44: vstr.32 s2,[r12]
  00081d48: vldr.32 s2,[r2,#0x4]
  00081d4c: vadd.f32 s2,s2,s16
  00081d50: vstr.32 s2,[r0]
  00081d54: mov.w r0,#0xc
  00081d58: vldr.32 s2,[r2,#0xc]
  00081d5c: orr.w r0,r0,r6, lsl #0x4
  00081d60: add r0,r3
  00081d62: lsl.w r6,r1,#0x5
  00081d66: vadd.f32 s2,s2,s0
  00081d6a: vstr.32 s2,[r0]
  00081d6e: vldr.32 s2,[r2,#0x10]
  00081d72: ldr r0,[sp,#0x10]
  00081d74: vadd.f32 s2,s2,s16
  00081d78: str.w lr,[sp,#0x10]
  00081d7c: vstr.32 s2,[r12,#0x10]
  00081d80: vldr.32 s2,[r2,#0x18]
  00081d84: vadd.f32 s2,s2,s0
  00081d88: vstr.32 s2,[r12,#0x18]
  00081d8c: vldr.32 s2,[r2,#0x1c]
  00081d90: vadd.f32 s2,s2,s16
  00081d94: vstr.32 s2,[r12,#0x1c]
  00081d98: vldr.32 s2,[r2,#0x24]
  00081d9c: vadd.f32 s0,s2,s0
  00081da0: vstr.32 s0,[r12,#0x24]
  00081da4: vldr.32 s0,[r2,#0x28]
  00081da8: vadd.f32 s0,s0,s16
  00081dac: vstr.32 s0,[r12,#0x28]
  00081db0: mov.w r12,#0x8
  00081db4: ldr r0,[r0,#0x8]
  00081db6: ldr.w r2,[lr,#0x8]
  00081dba: ldr r3,[r0,#0x0]
  00081dbc: str r3,[r2,r6]
  00081dbe: mov.w r6,#0x4
  00081dc2: orr.w r6,r6,r1, lsl #0x5
  00081dc6: ldr r3,[r0,#0x4]
  00081dc8: str r3,[r2,r6]
  00081dca: orr.w r6,r12,r1, lsl #0x5
  00081dce: ldr r3,[r0,#0x8]
  00081dd0: str r3,[r2,r6]
  00081dd2: mov.w r6,#0xc
  00081dd6: orr.w r6,r6,r1, lsl #0x5
  00081dda: ldr r3,[r0,#0xc]
  00081ddc: str r3,[r2,r6]
  00081dde: mov.w r6,#0x10
  00081de2: orr.w r6,r6,r1, lsl #0x5
  00081de6: ldr r3,[r0,#0x10]
  00081de8: str r3,[r2,r6]
  00081dea: mov.w r6,#0x14
  00081dee: orr.w r6,r6,r1, lsl #0x5
  00081df2: ldr r3,[r0,#0x14]
  00081df4: str r3,[r2,r6]
  00081df6: mov.w r6,#0x18
  00081dfa: orr.w r6,r6,r1, lsl #0x5
  00081dfe: ldr r3,[r0,#0x18]
  00081e00: str r3,[r2,r6]
  00081e02: mov.w r3,#0x1c
  00081e06: orr.w r3,r3,r1, lsl #0x5
  00081e0a: ldr r0,[r0,#0x1c]
  00081e0c: str r0,[r2,r3]
  00081e0e: lsl.w r3,r1,#0x6
  00081e12: ldr r6,[r7,#0x10]
  00081e14: ldr.w r2,[lr,#0xc]
  00081e18: ldr.w r0,[r6,#0xd0]
  00081e1c: str r0,[r2,r3]
  00081e1e: mov.w r0,#0x4
  00081e22: orr.w r0,r0,r1, lsl #0x6
  00081e26: ldr.w r3,[r6,#0xd4]
  00081e2a: str r3,[r2,r0]
  00081e2c: orr.w r0,r12,r1, lsl #0x6
  00081e30: ldr.w r3,[r6,#0xd8]
  00081e34: str r3,[r2,r0]
  00081e36: mov.w r0,#0xc
  00081e3a: orr.w r0,r0,r1, lsl #0x6
  00081e3e: ldr.w r3,[r6,#0xdc]
  00081e42: str r3,[r2,r0]
  00081e44: mov.w r0,#0x10
  00081e48: orr.w r0,r0,r1, lsl #0x6
  00081e4c: ldr.w r3,[r6,#0xd0]
  00081e50: str r3,[r2,r0]
  00081e52: mov.w r0,#0x14
  00081e56: orr.w r0,r0,r1, lsl #0x6
  00081e5a: ldr.w r3,[r6,#0xd4]
  00081e5e: str r3,[r2,r0]
  00081e60: mov.w r3,#0x18
  00081e64: orr.w r3,r3,r1, lsl #0x6
  00081e68: ldr.w r0,[r6,#0xd8]
  00081e6c: str r0,[r2,r3]
  00081e6e: mov.w r3,#0x1c
  00081e72: orr.w r3,r3,r1, lsl #0x6
  00081e76: ldr.w r0,[r6,#0xdc]
  00081e7a: str r0,[r2,r3]
  00081e7c: mov.w r3,#0x20
  00081e80: orr.w r3,r3,r1, lsl #0x6
  00081e84: ldr.w r0,[r6,#0xd0]
  00081e88: str r0,[r2,r3]
  00081e8a: mov.w r3,#0x24
  00081e8e: orr.w r3,r3,r1, lsl #0x6
  00081e92: ldr.w r0,[r6,#0xd4]
  00081e96: str r0,[r2,r3]
  00081e98: mov.w r3,#0x28
  00081e9c: orr.w r3,r3,r1, lsl #0x6
  00081ea0: ldr.w r0,[r6,#0xd8]
  00081ea4: str r0,[r2,r3]
  00081ea6: mov.w r3,#0x2c
  00081eaa: orr.w r3,r3,r1, lsl #0x6
  00081eae: ldr.w r0,[r6,#0xdc]
  00081eb2: str r0,[r2,r3]
  00081eb4: mov.w r3,#0x30
  00081eb8: orr.w r3,r3,r1, lsl #0x6
  00081ebc: ldr.w r0,[r6,#0xd0]
  00081ec0: str r0,[r2,r3]
  00081ec2: mov.w r3,#0x34
  00081ec6: orr.w r3,r3,r1, lsl #0x6
  00081eca: ldr.w r0,[r6,#0xd4]
  00081ece: str r0,[r2,r3]
  00081ed0: mov.w r3,#0x38
  00081ed4: orr.w r3,r3,r1, lsl #0x6
  00081ed8: ldr.w r0,[r6,#0xd8]
  00081edc: str r0,[r2,r3]
  00081ede: mov.w r3,#0x3c
  00081ee2: orr.w r3,r3,r1, lsl #0x6
  00081ee6: ldr.w r0,[r6,#0xdc]
  00081eea: str r0,[r2,r3]
  00081eec: add.w r2,r1,#0x1
  00081ef0: ldr r0,[r7,#0xc]
  00081ef2: str r2,[r0,#0xc]
  00081ef4: blt 0x00081f32
  00081ef6: add.w r0,r2,r2, lsl #0x1
  00081efa: ldr r1,[sp,#0x10]
  00081efc: ldr r6,[r7,#0xc]
  00081efe: lsls r0,r0,#0x1
  00081f00: strh r0,[r1,#0x28]
  00081f02: mov.w r1,#0x3f800000
  00081f06: ldr r0,[sp,#0x4]
  00081f08: str r1,[sp,#0x20]
  00081f0a: vst1.32 {d12,d13},[r0]
  00081f0e: ldr r0,[sp,#0x8]
  00081f10: str r1,[sp,#0x34]
  00081f12: vst1.64 {d12,d13},[r0]
  00081f16: ldr r0,[sp,#0xc]
  00081f18: vst1.32 {d10,d11},[r0]
  00081f1c: mov r0,r6
  00081f1e: str r1,[sp,#0x58]
  00081f20: add r1,sp,#0x20
  00081f22: blx 0x0006f55c
  00081f26: ldr r1,[r6,#0x8]
  00081f28: ldr r0,[r7,#0x10]
  00081f2a: blx 0x0006f568
  00081f2e: movs r0,#0x0
  00081f30: str r0,[r6,#0xc]
  00081f32: ldrsh.w r0,[r4,#0x10]
  00081f36: ldrh.w r1,[r5,r8,lsl #0x1]
  00081f3a: add r0,r9
  00081f3c: cmp r1,#0x20
  00081f3e: mov r1,r0
  00081f40: it eq
  00081f42: sub.eq r1,#0x2
  00081f44: cmp.w r9,#0xb
  00081f48: it ne
  00081f4a: mov.ne r1,r0
  00081f4c: ldr.w r9,[sp,#0x14]
  00081f50: add r10,r1
  00081f52: add r8,r9
  00081f54: add.w r11,r11,#0x1
  00081f58: ldr r1,[sp,#0x1c]
  00081f5a: uxth.w r0,r11
  00081f5e: cmp r0,r1
  00081f60: bcc.w 0x00081cc8
  00081f64: ldr r0,[r7,#0xc]
  00081f66: ldr r0,[r0,#0xc]
  00081f68: cmp r0,#0x0
  00081f6a: ble 0x00081fea
  00081f6c: ldr r4,[r7,#0xc]
  00081f6e: add.w r0,r0,r0, lsl #0x1
  00081f72: vmov.i32 q8,#0x0
  00081f76: lsls r0,r0,#0x1
  00081f78: ldr r1,[r4,#0x8]
  00081f7a: strh r0,[r1,#0x28]
  00081f7c: add r1,sp,#0x20
  00081f7e: ldr r0,[sp,#0x4]
  00081f80: vst1.32 {d16,d17},[r0]
  00081f84: ldr r0,[sp,#0x8]
  00081f86: vst1.64 {d16,d17},[r0]
  00081f8a: ldr r0,[sp,#0xc]
  00081f8c: vst1.64 {d10,d11},[r0]
  00081f90: mov.w r0,#0x3f800000
  00081f94: str r0,[sp,#0x20]
  00081f96: str r0,[sp,#0x34]
  00081f98: str r0,[sp,#0x58]
  00081f9a: mov r0,r4
  00081f9c: blx 0x0006f55c
  00081fa0: ldr r1,[r4,#0x8]
  00081fa2: ldr r0,[r7,#0x10]
  00081fa4: blx 0x0006f568
  00081fa8: movs r0,#0x0
  00081faa: str r0,[r4,#0xc]
  00081fac: b 0x00081fea
  00081fb8: cbz r0,0x00081fea
  00081fba: ldr r2,[sp,#0x1c]
  00081fbc: mov r0,r4
  00081fbe: mov r1,r5
  00081fc0: blx 0x0006f52c
  00081fc4: add r0,r10
  00081fc6: cmp r0,#0x0
  00081fc8: blt 0x00081fea
  00081fca: mov r0,r4
  00081fcc: ldrsh.w r6,[r4,#0x12]
  00081fd0: blx 0x0006f538
  00081fd4: mov r11,r0
  00081fd6: ldr r0,[r7,#0x10]
  00081fd8: blx 0x0006e8cc
  00081fdc: cmp r0,r10
  00081fde: ittt ge
  00081fe0: add.ge r6,r9
  00081fe2: add.ge.w r0,r11,r6
  00081fe6: cmp.ge r0,#0x0
  00081fe8: bge 0x0008200c
  00081fea: ldr r0,[sp,#0x5c]
  00081fec: ldr r1,[sp,#0x18]
  00081fee: ldr r1,[r1,#0x0]
  00081ff0: subs r0,r1,r0
  00081ff2: itttt eq
  00081ff4: add.eq sp,#0x60
  00081ff6: vpop.eq {d8}
  00081ffa: vpop.eq {d10,d11,d12,d13}
  00081ffe: add.eq sp,#0x4
  00082000: itt eq
  00082002: pop.eq.w {r8,r9,r10,r11}
  00082006: pop.eq {r4,r5,r6,r7,pc}
  00082008: blx 0x0006e824
  0008200c: ldr r0,[r7,#0x10]
  0008200e: blx 0x0006e8d8
  00082012: cmp r6,r0
  00082014: bgt 0x00081fea
  00082016: subs r1,r6,#0x2
  00082018: adr r0,[0x82120]
  0008201a: vmov.i32 q8,#0x0
  0008201e: add r2,sp,#0x20
  00082020: vmov s0,r10
  00082024: vld1.64 {d18,d19},[r0]
  00082028: orr r0,r2,#0x4
  0008202c: vmov s2,r1
  00082030: vcvt.f32.s32 s0,s0
  00082034: mov.w r12,#0xffffffff
  00082038: vcvt.f32.s32 s16,s2
  0008203c: vst1.32 {d16,d17},[r0]
  00082040: add.w r0,r2,#0x18
  00082044: mov.w r9,#0x0
  00082048: vst1.64 {d16,d17},[r0]
  0008204c: add.w r0,r2,#0x28
  00082050: vst1.64 {d18,d19},[r0]
  00082054: mov.w r0,#0x3f800000
  00082058: str r0,[sp,#0x58]
  0008205a: str r0,[sp,#0x20]
  0008205c: str r0,[sp,#0x34]
  0008205e: ldr r0,[r7,#0xc]
  00082060: vstr.32 s0,[sp,#0x2c]
  00082064: vstr.32 s16,[sp,#0x3c]
  00082068: ldrb r0,[r0,#0x1c]
  0008206a: orrs.w r0,r0,r8
  0008206e: it ne
  00082070: mov.ne.w r12,#0x1
  00082074: ldr r0,[sp,#0x1c]
  00082076: sub.w r6,r0,#0x1
  0008207a: it ne
  0008207c: mov.ne r6,#0x0
  0008207e: str.w r12,[sp,#0x14]
  00082082: b 0x00082114
  00082084: ldrh r0,[r4,#0x0]
  00082086: movs r1,#0x0
  00082088: uxth.w r11,r1
  0008208c: cmp r11,r0
  0008208e: bcs 0x0008210e
  00082090: ldr r2,[r4,#0x4]
  00082092: adds r1,#0x1
  00082094: ldrh.w r3,[r5,r6,lsl #0x1]
  00082098: ldrh.w r2,[r2,r11,lsl #0x1]
  0008209c: cmp r2,r3
  0008209e: bne 0x00082088
  000820a0: vmov s0,r10
  000820a4: vcvt.f32.s32 s0,s0
  000820a8: vstr.32 s16,[sp,#0x3c]
  000820ac: vstr.32 s0,[sp,#0x2c]
  000820b0: ldr r0,[r4,#0xc]
  000820b2: ldr.w r0,[r0,r11,lsl #0x2]
  000820b6: ldr r0,[r0,#0x4]
  000820b8: vldr.32 s0,[r0,#0xc]
  000820bc: vcvt.s32.f32 s0,s0
  000820c0: vmov r8,s0
  000820c4: add.w r0,r10,r8
  000820c8: cmp r0,#0x0
  000820ca: blt 0x000820f2
  000820cc: ldr r0,[r7,#0x10]
  000820ce: blx 0x0006e8cc
  000820d2: ldr.w r12,[sp,#0x14]
  000820d6: cmp r10,r0
  000820d8: bgt 0x000820f2
  000820da: ldr r0,[r7,#0xc]
  000820dc: add r1,sp,#0x20
  000820de: blx 0x0006f55c
  000820e2: ldr r0,[r4,#0xc]
  000820e4: ldr.w r1,[r0,r11,lsl #0x2]
  000820e8: ldr r0,[r7,#0x10]
  000820ea: blx 0x0006f568
  000820ee: ldr.w r12,[sp,#0x14]
  000820f2: ldrsh.w r0,[r4,#0x10]
  000820f6: ldrh.w r1,[r5,r6,lsl #0x1]
  000820fa: add r0,r8
  000820fc: cmp r1,#0x20
  000820fe: mov r1,r0
  00082100: it eq
  00082102: sub.eq r1,#0x2
  00082104: cmp.w r8,#0xb
  00082108: it ne
  0008210a: mov.ne r1,r0
  0008210c: add r10,r1
  0008210e: add r6,r12
  00082110: add.w r9,r9,#0x1
  00082114: ldr r1,[sp,#0x1c]
  00082116: uxth.w r0,r9
  0008211a: cmp r0,r1
  0008211c: bcc 0x00082084
  0008211e: b 0x00081fea
```
