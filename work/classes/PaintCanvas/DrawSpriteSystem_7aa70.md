# PaintCanvas::DrawSpriteSystem
elf 0x7aa70 body 396
Sig: undefined __stdcall DrawSpriteSystem(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawSpriteSystem(unsigned int) */

void AbyssEngine::PaintCanvas::DrawSpriteSystem(uint param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  Engine *pEVar6;
  uint in_r1;
  int *piVar7;
  Matrix *pMVar8;
  float extraout_s0;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  Matrix aMStack_144 [60];
  AEMath aAStack_108 [64];
  undefined4 local_c8;
  uint local_c4;
  uint local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 uStack_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined4 local_90;
  undefined4 local_88 [5];
  undefined4 local_74;
  undefined4 local_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined4 local_50;
  int local_4c;
  
  uVar2 = DAT_0008ac08;
  uVar1 = DAT_0008ac00;
  piVar7 = *(int **)(DAT_0008ac18 + 0x8aa88);
  local_4c = *piVar7;
  if ((in_r1 < *(uint *)(param_1 + 0x180)) && (*(int *)(*(int *)(param_1 + 0x184) + in_r1 * 4) != 0)
     ) {
    uVar9 = 0;
    uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar11 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    puVar3 = (undefined4 *)((uint)local_88 | 4);
    *puVar3 = 0;
    puVar3[1] = uVar10;
    puVar3[2] = uVar11;
    puVar3[3] = uVar12;
    local_70 = 0;
    uStack_60 = uVar1;
    uStack_58 = uVar2;
    local_88[0] = 0x3f800000;
    local_74 = 0x3f800000;
    local_50 = 0x3f800000;
    uStack_6c = uVar10;
    uStack_68 = uVar11;
    uStack_64 = uVar12;
    if (*(uint *)(param_1 + 0x170) < *(uint *)(param_1 + 0x164)) {
      if (*(char *)param_1 == '\0') {
        pMVar8 = (Matrix *)&local_c8;
      }
      else {
        uStack_ac = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
        uStack_a8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
        uStack_a4 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
        puVar3 = (undefined4 *)((uint)&local_c8 | 4);
        *puVar3 = 0;
        puVar3[1] = uStack_ac;
        puVar3[2] = uStack_a8;
        puVar3[3] = uStack_a4;
        local_b0 = 0;
        uStack_a0 = uVar1;
        uStack_98 = uVar2;
        local_c8 = 0x3f800000;
        local_b4 = 0x3f800000;
        local_90 = 0x3f800000;
        AEMath::MatrixIdentity(aAStack_108,(Matrix *)&local_c8);
        iVar4 = Engine::GetGravValue(*(Engine **)(param_1 + 0x34));
        uVar5 = AEMath::Sinf((float)(*(double *)(iVar4 + 8) * DAT_0008ac10));
        local_c8 = AEMath::Cosf(extraout_s0);
        local_c4 = uVar5 ^ 0x80000000;
        local_b8 = uVar5;
        local_b4 = local_c8;
        __aeabi_memcpy((Matrix *)aAStack_108,
                       *(int *)(*(int *)(param_1 + 0x168) + *(int *)(param_1 + 0x170) * 4) + 0xc,
                       0x3c);
        AEMath::Matrix::operator*=((Matrix *)aAStack_108,(Matrix *)&local_c8);
        pMVar8 = aMStack_144;
      }
      AEMath::MatrixGetInverse(pMVar8);
      AEMath::Matrix::operator=((Matrix *)local_88,pMVar8);
    }
    pEVar6 = *(Engine **)(param_1 + 0x34);
    puVar3 = (undefined4 *)((uint)&local_c8 | 4);
    *puVar3 = uVar9;
    puVar3[1] = uVar10;
    puVar3[2] = uVar11;
    puVar3[3] = uVar12;
    uStack_a0 = uVar1;
    uStack_98 = uVar2;
    local_c8 = 0x3f800000;
    local_b4 = 0x3f800000;
    local_90 = 0x3f800000;
    local_b0 = uVar9;
    uStack_ac = uVar10;
    uStack_a8 = uVar11;
    uStack_a4 = uVar12;
    SpriteSystemDraw(pEVar6,(Matrix *)&local_c8,(Matrix *)local_88,
                     *(SpriteSystem **)(*(int *)(param_1 + 0x184) + in_r1 * 4));
  }
  if (*piVar7 != local_4c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008aa70: push {r4,r5,r6,r7,lr}
  0008aa72: add r7,sp,#0xc
  0008aa74: push {r7,r8,r9,r10,r11}
  0008aa78: vpush {d8,d9,d10,d11}
  0008aa7c: sub sp,#0x100
  0008aa7e: mov r5,r0
  0008aa80: ldr r0,[0x0008ac18]
  0008aa82: mov r4,r1
  0008aa84: add r0,pc
  0008aa86: ldr r6,[r0,#0x0]
  0008aa88: ldr r0,[r6,#0x0]
  0008aa8a: str r0,[sp,#0xfc]
  0008aa8c: ldr.w r0,[r5,#0x180]
  0008aa90: cmp r0,r1
  0008aa92: bls.w 0x0008abe0
  0008aa96: ldr.w r0,[r5,#0x184]
  0008aa9a: ldr.w r0,[r0,r4,lsl #0x2]
  0008aa9e: cmp r0,#0x0
  0008aaa0: beq.w 0x0008abe0
  0008aaa4: adr r0,[0x8ac00]
  0008aaa6: vmov.i32 q4,#0x0
  0008aaaa: add r1,sp,#0xc0
  0008aaac: vld1.64 {d10,d11},[r0]
  0008aab0: orr r0,r1,#0x4
  0008aab4: mov.w r11,#0x3f800000
  0008aab8: vst1.32 {d8,d9},[r0]
  0008aabc: add.w r0,r1,#0x18
  0008aac0: vst1.64 {d8,d9},[r0]
  0008aac4: add.w r0,r1,#0x28
  0008aac8: vst1.64 {d10,d11},[r0]
  0008aacc: str.w r11,[sp,#0xc0]
  0008aad0: str.w r11,[sp,#0xd4]
  0008aad4: str.w r11,[sp,#0xf8]
  0008aad8: ldr.w r1,[r5,#0x164]
  0008aadc: ldr.w r0,[r5,#0x170]
  0008aae0: cmp r0,r1
  0008aae2: bcs 0x0008abaa
  0008aae4: ldrb r1,[r5,#0x0]
  0008aae6: cmp r1,#0x0
  0008aae8: beq 0x0008ab8c
  0008aaea: add.w r8,sp,#0x80
  0008aaee: vmov.i32 q8,#0x0
  0008aaf2: orr r0,r8,#0x4
  0008aaf6: mov r1,r8
  0008aaf8: vst1.32 {d16,d17},[r0]
  0008aafc: add.w r0,r8,#0x18
  0008ab00: vst1.64 {d16,d17},[r0]
  0008ab04: add.w r0,r8,#0x28
  0008ab08: vst1.64 {d10,d11},[r0]
  0008ab0c: mov.w r0,#0x3f800000
  0008ab10: str r0,[sp,#0x80]
  0008ab12: str r0,[sp,#0x94]
  0008ab14: str r0,[sp,#0xb8]
  0008ab16: add r0,sp,#0x40
  0008ab18: blx 0x0006fb2c
  0008ab1c: ldr r0,[r5,#0x34]
  0008ab1e: blx 0x0006fb38
  0008ab22: vldr.64 d17,[r0,#0x8]
  0008ab26: vldr.64 d16,[pc,#0xe8]
  0008ab2a: ldr r0,[r5,#0x30]
  0008ab2c: vmul.f64 d16,d17,d16
  0008ab30: cmp r0,#0x1
  0008ab32: vcvt.f32.f64 s0,d16
  0008ab36: vneg.f32 s2,s0
  0008ab3a: it eq
  0008ab3c: vmov.eq.f32 s2,s0
  0008ab40: vmov r10,s2
  0008ab44: mov r0,r10
  0008ab46: blx 0x0006f1a8
  0008ab4a: mov r9,r0
  0008ab4c: mov r0,r10
  0008ab4e: blx 0x0006f49c
  0008ab52: str r0,[sp,#0x80]
  0008ab54: movs r2,#0x3c
  0008ab56: str r0,[sp,#0x94]
  0008ab58: eor r0,r9,#0x80000000
  0008ab5c: str r0,[sp,#0x84]
  0008ab5e: str.w r9,[sp,#0x90]
  0008ab62: add.w r9,sp,#0x40
  0008ab66: ldr.w r0,[r5,#0x168]
  0008ab6a: ldr.w r1,[r5,#0x170]
  0008ab6e: ldr.w r0,[r0,r1,lsl #0x2]
  0008ab72: add.w r1,r0,#0xc
  0008ab76: mov r0,r9
  0008ab78: blx 0x0006f1e4
  0008ab7c: mov r0,r9
  0008ab7e: mov r1,r8
  0008ab80: blx 0x0006f4a8
  0008ab84: add.w r8,sp,#0x4
  0008ab88: mov r1,r9
  0008ab8a: b 0x0008ab9c
  0008ab8c: ldr.w r1,[r5,#0x168]
  0008ab90: add.w r8,sp,#0x80
  0008ab94: ldr.w r0,[r1,r0,lsl #0x2]
  0008ab98: add.w r1,r0,#0xc
  0008ab9c: mov r0,r8
  0008ab9e: blx 0x0006fb50
  0008aba2: add r0,sp,#0xc0
  0008aba4: mov r1,r8
  0008aba6: blx 0x0006f148
  0008abaa: add r1,sp,#0x80
  0008abac: ldr r0,[r5,#0x34]
  0008abae: orr r2,r1,#0x4
  0008abb2: vst1.32 {d8,d9},[r2]
  0008abb6: add.w r2,r1,#0x18
  0008abba: vst1.64 {d8,d9},[r2]
  0008abbe: add.w r2,r1,#0x28
  0008abc2: vst1.64 {d10,d11},[r2]
  0008abc6: str.w r11,[sp,#0x80]
  0008abca: str.w r11,[sp,#0x94]
  0008abce: str.w r11,[sp,#0xb8]
  0008abd2: ldr.w r2,[r5,#0x184]
  0008abd6: ldr.w r3,[r2,r4,lsl #0x2]
  0008abda: add r2,sp,#0xc0
  0008abdc: blx 0x0006fce8
  0008abe0: ldr r0,[sp,#0xfc]
  0008abe2: ldr r1,[r6,#0x0]
  0008abe4: subs r0,r1,r0
  0008abe6: itttt eq
  0008abe8: add.eq sp,#0x100
  0008abea: vpop.eq {d8,d9,d10,d11}
  0008abee: add.eq sp,#0x4
  0008abf0: pop.eq.w {r8,r9,r10,r11}
  0008abf4: it eq
  0008abf6: pop.eq {r4,r5,r6,r7,pc}
  0008abf8: blx 0x0006e824
```
