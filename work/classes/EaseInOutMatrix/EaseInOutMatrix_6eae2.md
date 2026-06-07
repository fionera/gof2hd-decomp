# EaseInOutMatrix::EaseInOutMatrix
elf 0x6eae2 body 214
Sig: undefined __thiscall EaseInOutMatrix(EaseInOutMatrix * this, Matrix param_1, Matrix param_2, int param_3)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(AbyssEngine::AEMath::Matrix,
   AbyssEngine::AEMath::Matrix, int) */

EaseInOutMatrix * __thiscall
AbyssEngine::EaseInOutMatrix::EaseInOutMatrix
          (EaseInOutMatrix *this,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
          undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
          undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16,
          undefined4 param_17,undefined4 param_18,undefined4 param_19,undefined4 param_20,
          undefined4 param_21,undefined4 param_22,undefined4 param_23,undefined4 param_24,
          undefined4 param_25,undefined4 param_26,undefined4 param_27,undefined4 param_28,
          undefined4 param_29,undefined4 param_30,undefined4 param_31,undefined4 param_32)

{
  int iVar1;
  uint in_fpscr;
  undefined4 uVar2;
  
  iVar1 = AEMath::Matrix::Matrix((Matrix *)this);
  Quaternion::Quaternion((Quaternion *)(iVar1 + 0x3c));
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  Quaternion::Quaternion((Quaternion *)(this + 0x58));
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  AEMath::Matrix::Matrix((Matrix *)(this + 0x78));
  AEMath::Matrix::Matrix((Matrix *)(this + 0xb4));
  SetRange(this,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,param_10,param_11,
           param_12,param_13,param_14,param_15,param_16,param_17,param_18,param_19,param_20,param_21
           ,param_22,param_23,param_24,param_25,param_26,param_27,param_28,param_29,param_30,
           param_31);
  uVar2 = VectorSignedToFloat(param_32,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0xf0) = uVar2;
  return this;
}

```

## target disasm
```
  0007eae2: push {r4,r5,r6,r7,lr}
  0007eae4: add r7,sp,#0xc
  0007eae6: push {r8,r9,r10,r11}
  0007eaea: sub sp,#0x84
  0007eaec: mov r8,r3
  0007eaee: mov r9,r2
  0007eaf0: mov r4,r1
  0007eaf2: mov r5,r0
  0007eaf4: blx 0x0006f118
  0007eaf8: adds r0,#0x3c
  0007eafa: str r0,[sp,#0x74]
  0007eafc: blx 0x0006f124
  0007eb00: add.w r0,r5,#0x58
  0007eb04: movs r6,#0x0
  0007eb06: strd r6,r6,[r5,#0x4c]
  0007eb0a: str r6,[r5,#0x54]
  0007eb0c: str r0,[sp,#0x70]
  0007eb0e: blx 0x0006f124
  0007eb12: strd r4,r9,[sp,#0x78]
  0007eb16: str.w r8,[sp,#0x80]
  0007eb1a: strd r6,r6,[r5,#0x68]
  0007eb1e: str r6,[r5,#0x70]
  0007eb20: add.w r0,r5,#0x78
  0007eb24: blx 0x0006f118
  0007eb28: add.w r0,r5,#0xb4
  0007eb2c: blx 0x0006f118
  0007eb30: add.w r10,r7,#0x1c
  0007eb34: add.w r12,r7,#0x8
  0007eb38: ldrd r8,r11,[r7,#0x40]
  0007eb3c: ldrd r4,lr,[r7,#0x38]
  0007eb40: ldm.w r10,{r6,r9,r10}
  0007eb44: ldm.w r12,{r0,r1,r2,r3,r12}
  0007eb48: strd r4,lr,[sp,#0x30]
  0007eb4c: strd r8,r11,[sp,#0x38]
  0007eb50: stm sp,{r0,r1,r2,r3,r12}
  0007eb54: add r0,sp,#0x14
  0007eb56: add r3,sp,#0x78
  0007eb58: stm r0,{r6,r9,r10}
  0007eb5c: ldr r0,[r7,#0x48]
  0007eb5e: str r0,[sp,#0x40]
  0007eb60: ldr r0,[r7,#0x4c]
  0007eb62: str r0,[sp,#0x44]
  0007eb64: ldr r0,[r7,#0x50]
  0007eb66: str r0,[sp,#0x48]
  0007eb68: ldr r0,[r7,#0x54]
  0007eb6a: str r0,[sp,#0x4c]
  0007eb6c: ldr r0,[r7,#0x58]
  0007eb6e: str r0,[sp,#0x50]
  0007eb70: ldr r0,[r7,#0x5c]
  0007eb72: str r0,[sp,#0x54]
  0007eb74: ldr r0,[r7,#0x60]
  0007eb76: str r0,[sp,#0x58]
  0007eb78: ldr r0,[r7,#0x64]
  0007eb7a: str r0,[sp,#0x5c]
  0007eb7c: ldr r0,[r7,#0x28]
  0007eb7e: str r0,[sp,#0x20]
  0007eb80: ldr r0,[r7,#0x2c]
  0007eb82: str r0,[sp,#0x24]
  0007eb84: ldr r0,[r7,#0x30]
  0007eb86: str r0,[sp,#0x28]
  0007eb88: ldr r0,[r7,#0x68]
  0007eb8a: str r0,[sp,#0x60]
  0007eb8c: ldr r0,[r7,#0x6c]
  0007eb8e: str r0,[sp,#0x64]
  0007eb90: ldr r0,[r7,#0x70]
  0007eb92: str r0,[sp,#0x68]
  0007eb94: ldr r0,[r7,#0x34]
  0007eb96: str r0,[sp,#0x2c]
  0007eb98: mov r0,r5
  0007eb9a: ldmia r3,{r1,r2,r3}
  0007eb9c: blx 0x0006f130
  0007eba0: ldr r0,[r7,#0x74]
  0007eba2: vmov s0,r0
  0007eba6: mov r0,r5
  0007eba8: vcvt.f32.s32 s0,s0
  0007ebac: vstr.32 s0,[r5,#0xf0]
  0007ebb0: add sp,#0x84
  0007ebb2: pop.w {r8,r9,r10,r11}
  0007ebb6: pop {r4,r5,r6,r7,pc}
```
