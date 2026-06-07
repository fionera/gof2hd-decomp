# HangarWindow::update
elf 0x148a7e body 488
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* HangarWindow::update(int) */

void HangarWindow::update(int param_1)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  bool bVar4;
  int in_r1;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint in_fpscr;
  float fVar8;
  float fVar9;
  
  if (*(char *)(param_1 + 0xc) != '\0') {
    *(int *)(param_1 + 8) = in_r1;
    if (*(int *)(param_1 + 0x58) == 1) {
      (*(code *)(DAT_001ac4b4 + 0x1ac4b8))(*(undefined4 *)(param_1 + 0x18));
      return;
    }
    uVar2 = HangarList::getCurrentTab(*(HangarList **)(param_1 + 0x14));
    for (uVar7 = 0; uVar7 < **(uint **)(param_1 + 4); uVar7 = uVar7 + 1) {
      bVar4 = true;
      if ((uVar2 != uVar7) && (uVar2 != 3 || uVar7 != 0)) {
        bVar4 = uVar7 == 2 && uVar2 == 4;
      }
      TouchButton::setAlwaysPressed
                (*(TouchButton **)((*(uint **)(param_1 + 4))[1] + uVar7 * 4),bVar4);
    }
    if (*(char *)(param_1 + 0xd0) == '\0') {
      fVar8 = *(float *)(param_1 + 0xc4) * *(float *)(param_1 + 200);
      fVar9 = -(*(float *)(param_1 + 0xc4) * *(float *)(param_1 + 200));
      if (0.0 < fVar8) {
        fVar9 = fVar8;
      }
      *(float *)(param_1 + 200) = fVar8;
      uVar2 = in_fpscr & 0xfffffff | (uint)(fVar9 < 1.0) << 0x1f | (uint)(fVar9 == 1.0) << 0x1e;
      in_fpscr = uVar2 | (uint)NAN(fVar9) << 0x1c;
      bVar1 = (byte)(uVar2 >> 0x18);
      if (!(bool)(bVar1 >> 6 & 1) && bVar1 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        fVar9 = (float)VectorSignedToFloat(*(undefined4 *)(param_1 + 0xb4),
                                           (byte)(in_fpscr >> 0x16) & 3);
        *(int *)(param_1 + 0xb4) = (int)(fVar8 + fVar9);
      }
    }
    if (0 < *(int *)(param_1 + 0xb4)) {
      fVar9 = (float)VectorSignedToFloat(-*(int *)(param_1 + 0xb4),(byte)(in_fpscr >> 0x16) & 3);
      *(undefined4 *)(param_1 + 0xc4) = 0x3f800000;
      *(float *)(param_1 + 200) = fVar9 * 0.5;
    }
    iVar3 = HangarList::getCurrentTabItems(*(HangarList **)(param_1 + 0x14));
    if (iVar3 != 0) {
      iVar3 = *(int *)(param_1 + 0xd8) - *(int *)(param_1 + 0xd4);
      if (iVar3 < 0) {
        if (*(int *)(param_1 + 0xb4) < iVar3) {
          fVar9 = (float)VectorSignedToFloat(iVar3 - *(int *)(param_1 + 0xb4),
                                             (byte)(in_fpscr >> 0x16) & 3);
          *(undefined4 *)(param_1 + 0xc4) = 0x3f800000;
          *(float *)(param_1 + 200) = fVar9 * 0.5;
        }
      }
      else {
        *(undefined4 *)(param_1 + 200) = 0;
        *(undefined4 *)(param_1 + 0xb4) = 0;
      }
    }
    if ((*(char *)(param_1 + 0x88) != '\0') &&
       ((iVar3 = TouchButton::isTouched
                           (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x20)),
        iVar3 != 0 ||
        (iVar3 = TouchButton::isTouched
                           (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x24)),
        iVar3 != 0)))) {
      iVar6 = 200;
      iVar5 = *(int *)(param_1 + 0x70) + in_r1;
      iVar3 = *(int *)(param_1 + 0x6c) + in_r1;
      *(int *)(param_1 + 0x6c) = iVar3;
      *(int *)(param_1 + 0x70) = iVar5;
      if (0x5dc < iVar3) {
        iVar6 = 0x1e;
      }
      if ((iVar6 < iVar5) &&
         ((*(undefined4 *)(param_1 + 0x70) = 0, *(char *)(param_1 + 0x88) != '\0' ||
          (*(char *)(param_1 + 0x89) != '\0')))) {
        iVar3 = TouchButton::isTouched
                          (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x24));
        if ((iVar3 == 0) ||
           (iVar3 = TouchButton::isVisible
                              (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x24)),
           iVar3 == 0)) {
          iVar3 = TouchButton::isTouched
                            (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x20));
          if ((iVar3 != 0) &&
             (iVar3 = TouchButton::isVisible
                                (*(TouchButton **)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x20)),
             iVar3 != 0)) {
            iVar3 = 1;
            if (4000 < *(int *)(param_1 + 0x6c)) {
              iVar3 = 5;
            }
            for (; iVar3 != 0; iVar3 = iVar3 + -1) {
              transaction((HangarWindow *)param_1,false);
            }
          }
        }
        else {
          iVar3 = 1;
          if (4000 < *(int *)(param_1 + 0x6c)) {
            iVar3 = 5;
          }
          for (; iVar3 != 0; iVar3 = iVar3 + -1) {
            transaction((HangarWindow *)param_1,true);
          }
        }
      }
    }
  }
  return;
}

```

## target disasm
```
  00158a7e: push {r4,r5,r6,r7,lr}
  00158a80: add r7,sp,#0xc
  00158a82: push.w r8
  00158a86: mov r4,r0
  00158a88: ldrb r0,[r0,#0xc]
  00158a8a: mov r8,r1
  00158a8c: cmp r0,#0x0
  00158a8e: beq.w 0x00158c60
  00158a92: ldr r0,[r4,#0x58]
  00158a94: str.w r8,[r4,#0x8]
  00158a98: cmp r0,#0x1
  00158a9a: bne 0x00158aac
  00158a9c: ldr r0,[r4,#0x18]
  00158a9e: mov r1,r8
  00158aa0: pop.w r8
  00158aa4: pop.w {r4,r5,r6,r7,lr}
  00158aa8: b.w 0x001ac4a8
  00158aac: ldr r0,[r4,#0x14]
  00158aae: blx 0x00076f48
  00158ab2: mov r6,r0
  00158ab4: movs r5,#0x0
  00158ab6: b 0x00158aea
  00158ab8: ldr r0,[r0,#0x4]
  00158aba: movs r1,#0x1
  00158abc: cmp r6,r5
  00158abe: ldr.w r0,[r0,r5,lsl #0x2]
  00158ac2: beq 0x00158ae4
  00158ac4: eor r2,r6,#0x3
  00158ac8: orrs r2,r5
  00158aca: clz r2,r2
  00158ace: lsrs r2,r2,#0x5
  00158ad0: itttt eq
  00158ad2: eor.eq r1,r5,#0x2
  00158ad6: eor.eq r2,r6,#0x4
  00158ada: orr.eq r1,r2
  00158adc: clz.eq r1,r1
  00158ae0: it eq
  00158ae2: lsr.eq r1,r1,#0x5
  00158ae4: blx 0x00075acc
  00158ae8: adds r5,#0x1
  00158aea: ldr r0,[r4,#0x4]
  00158aec: ldr r1,[r0,#0x0]
  00158aee: cmp r5,r1
  00158af0: bcc 0x00158ab8
  00158af2: ldrb.w r0,[r4,#0xd0]
  00158af6: cbnz r0,0x00158b3c
  00158af8: vldr.32 s2,[r4,#0xc4]
  00158afc: vldr.32 s4,[r4,#0xc8]
  00158b00: vmul.f32 s0,s2,s4
  00158b04: vnmul.f32 s2,s2,s4
  00158b08: vmov.f32 s4,0x3f800000
  00158b0c: vcmpe.f32 s0,#0
  00158b10: vmrs apsr,fpscr
  00158b14: it gt
  00158b16: vmov.gt.f32 s2,s0
  00158b1a: vstr.32 s0,[r4,#0xc8]
  00158b1e: vcmpe.f32 s2,s4
  00158b22: vmrs apsr,fpscr
  00158b26: ble 0x00158b3c
  00158b28: vldr.32 s2,[r4,#0xb4]
  00158b2c: vcvt.f32.s32 s2,s2
  00158b30: vadd.f32 s0,s0,s2
  00158b34: vcvt.s32.f32 s0,s0
  00158b38: vstr.32 s0,[r4,#0xb4]
  00158b3c: ldr.w r0,[r4,#0xb4]
  00158b40: cmp r0,#0x1
  00158b42: blt 0x00158b62
  00158b44: rsbs r0,r0
  00158b46: vmov.f32 s0,0x3f000000
  00158b4a: vmov s2,r0
  00158b4e: mov.w r0,#0x3f800000
  00158b52: vcvt.f32.s32 s2,s2
  00158b56: str.w r0,[r4,#0xc4]
  00158b5a: vmul.f32 s0,s2,s0
  00158b5e: vstr.32 s0,[r4,#0xc8]
  00158b62: ldr r0,[r4,#0x14]
  00158b64: blx 0x00076f3c
  00158b68: cbz r0,0x00158ba8
  00158b6a: ldrd r0,r1,[r4,#0xd4]
  00158b6e: subs r0,r1,r0
  00158b70: cmp.w r0,#0xffffffff
  00158b74: ble 0x00158b82
  00158b76: movs r0,#0x0
  00158b78: str.w r0,[r4,#0xc8]
  00158b7c: str.w r0,[r4,#0xb4]
  00158b80: b 0x00158ba8
  00158b82: ldr.w r1,[r4,#0xb4]
  00158b86: cmp r0,r1
  00158b88: ble 0x00158ba8
  00158b8a: subs r0,r0,r1
  00158b8c: vmov.f32 s0,0x3f000000
  00158b90: vmov s2,r0
  00158b94: mov.w r0,#0x3f800000
  00158b98: vcvt.f32.s32 s2,s2
  00158b9c: str.w r0,[r4,#0xc4]
  00158ba0: vmul.f32 s0,s2,s0
  00158ba4: vstr.32 s0,[r4,#0xc8]
  00158ba8: ldrb.w r0,[r4,#0x88]
  00158bac: cmp r0,#0x0
  00158bae: beq 0x00158c60
  00158bb0: ldr r0,[r4,#0x24]
  00158bb2: ldr r0,[r0,#0x4]
  00158bb4: ldr r0,[r0,#0x20]
  00158bb6: blx 0x00076f60
  00158bba: cbnz r0,0x00158bca
  00158bbc: ldr r0,[r4,#0x24]
  00158bbe: ldr r0,[r0,#0x4]
  00158bc0: ldr r0,[r0,#0x24]
  00158bc2: blx 0x00076f60
  00158bc6: cmp r0,#0x0
  00158bc8: beq 0x00158c60
  00158bca: ldrd r0,r1,[r4,#0x6c]
  00158bce: movs r2,#0xc8
  00158bd0: add r1,r8
  00158bd2: movw r3,#0x5dc
  00158bd6: add r0,r8
  00158bd8: strd r0,r1,[r4,#0x6c]
  00158bdc: cmp r0,r3
  00158bde: it gt
  00158be0: mov.gt r2,#0x1e
  00158be2: cmp r1,r2
  00158be4: ble 0x00158c60
  00158be6: ldrb.w r0,[r4,#0x88]
  00158bea: movs r1,#0x0
  00158bec: str r1,[r4,#0x70]
  00158bee: cbnz r0,0x00158bf6
  00158bf0: ldrb.w r0,[r4,#0x89]
  00158bf4: cbz r0,0x00158c60
  00158bf6: ldr r0,[r4,#0x24]
  00158bf8: ldr r0,[r0,#0x4]
  00158bfa: ldr r0,[r0,#0x24]
  00158bfc: blx 0x00076f60
  00158c00: cbz r0,0x00158c2c
  00158c02: ldr r0,[r4,#0x24]
  00158c04: ldr r0,[r0,#0x4]
  00158c06: ldr r0,[r0,#0x24]
  00158c08: blx 0x00074ec0
  00158c0c: cbz r0,0x00158c2c
  00158c0e: ldr r0,[r4,#0x6c]
  00158c10: movs r5,#0x1
  00158c12: cmp.w r0,#0xfa0
  00158c16: it gt
  00158c18: mov.gt r5,#0x5
  00158c1a: b 0x00158c26
  00158c1c: mov r0,r4
  00158c1e: movs r1,#0x1
  00158c20: blx 0x00076f6c
  00158c24: subs r5,#0x1
  00158c26: cmp r5,#0x0
  00158c28: bne 0x00158c1c
  00158c2a: b 0x00158c60
  00158c2c: ldr r0,[r4,#0x24]
  00158c2e: ldr r0,[r0,#0x4]
  00158c30: ldr r0,[r0,#0x20]
  00158c32: blx 0x00076f60
  00158c36: cbz r0,0x00158c60
  00158c38: ldr r0,[r4,#0x24]
  00158c3a: ldr r0,[r0,#0x4]
  00158c3c: ldr r0,[r0,#0x20]
  00158c3e: blx 0x00074ec0
  00158c42: cbz r0,0x00158c60
  00158c44: ldr r0,[r4,#0x6c]
  00158c46: movs r5,#0x1
  00158c48: cmp.w r0,#0xfa0
  00158c4c: it gt
  00158c4e: mov.gt r5,#0x5
  00158c50: b 0x00158c5c
  00158c52: mov r0,r4
  00158c54: movs r1,#0x0
  00158c56: blx 0x00076f6c
  00158c5a: subs r5,#0x1
  00158c5c: cmp r5,#0x0
  00158c5e: bne 0x00158c52
  00158c60: pop.w r8
  00158c64: pop {r4,r5,r6,r7,pc}
```
