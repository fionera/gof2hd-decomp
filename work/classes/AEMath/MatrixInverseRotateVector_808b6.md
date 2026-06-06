# AbyssEngine::AEMath::MatrixInverseRotateVector
elf 0x808b6 body 359
Sig: undefined __stdcall MatrixInverseRotateVector(Matrix * param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0014bf32) overlaps instruction at (ram,0x0014bf30)
    */
/* WARNING: Removing unreachable block (ram,0x000908de) */
/* WARNING: Removing unreachable block (ram,0x0014c02a) */
/* WARNING: Removing unreachable block (ram,0x0014c02e) */
/* WARNING: Removing unreachable block (ram,0x0014c032) */
/* WARNING: Removing unreachable block (ram,0x0014c036) */
/* WARNING: Removing unreachable block (ram,0x0014c03a) */
/* WARNING: Removing unreachable block (ram,0x0014c046) */
/* WARNING: Removing unreachable block (ram,0x000908e2) */
/* WARNING: Removing unreachable block (ram,0x0014bf32) */
/* WARNING: Removing unreachable block (ram,0x0014bf36) */
/* WARNING: Removing unreachable block (ram,0x0014bf3a) */
/* WARNING: Removing unreachable block (ram,0x000908e6) */
/* WARNING: Removing unreachable block (ram,0x0024c0ee) */
/* WARNING: Removing unreachable block (ram,0x000908ea) */
/* WARNING: Removing unreachable block (ram,0x0030bf36) */
/* WARNING: Removing unreachable block (ram,0x0028c0f6) */
/* WARNING: Removing unreachable block (ram,0x002cbf46) */
/* WARNING: Removing unreachable block (ram,0x000908fa) */
/* WARNING: Removing unreachable block (ram,0x000908fe) */
/* WARNING: Removing unreachable block (ram,0x000cc10e) */
/* WARNING: Removing unreachable block (ram,0x000cc170) */
/* WARNING: Removing unreachable block (ram,0x000cc178) */
/* WARNING: Removing unreachable block (ram,0x000cc18e) */
/* WARNING: Removing unreachable block (ram,0x000cc18a) */
/* WARNING: Removing unreachable block (ram,0x000cc1a2) */
/* WARNING: Removing unreachable block (ram,0x000cc1b8) */
/* WARNING: Removing unreachable block (ram,0x000cc1b0) */
/* WARNING: Removing unreachable block (ram,0x000908ee) */
/* WARNING: Removing unreachable block (ram,0x000908f2) */
/* WARNING: Removing unreachable block (ram,0x000908f6) */
/* WARNING: Removing unreachable block (ram,0x008cc0fe) */
/* WARNING: Removing unreachable block (ram,0x0010bf1e) */
/* WARNING: Removing unreachable block (ram,0x00090902) */
/* WARNING: Removing unreachable block (ram,0x00090906) */
/* WARNING: Removing unreachable block (ram,0x0024c116) */
/* WARNING: Removing unreachable block (ram,0x0009090a) */
/* WARNING: Removing unreachable block (ram,0x000cbf12) */
/* WARNING: Removing unreachable block (ram,0x0009090e) */
/* WARNING: Removing unreachable block (ram,0x00090912) */
/* WARNING: Removing unreachable block (ram,0x00090916) */
/* WARNING: Removing unreachable block (ram,0x0009091a) */
/* WARNING: Removing unreachable block (ram,0x00090922) */
/* WARNING: Removing unreachable block (ram,0x00090926) */
/* WARNING: Removing unreachable block (ram,0x0009092a) */
/* WARNING: Removing unreachable block (ram,0x0014bf1a) */
/* WARNING: Removing unreachable block (ram,0x0014bf34) */
/* WARNING: Removing unreachable block (ram,0x0014bf30) */
/* WARNING: Removing unreachable block (ram,0x0020c112) */
/* WARNING: Removing unreachable block (ram,0x0034bf3e) */
/* WARNING: Removing unreachable block (ram,0x000908d6) */
/* WARNING: Removing unreachable block (ram,0x000908da) */
/* WARNING: Removing unreachable block (ram,0x0010bf26) */
/* WARNING: Removing unreachable block (ram,0xfe5aa61a) */
/* WARNING: Removing unreachable block (ram,0x000908c6) */
/* WARNING: Removing unreachable block (ram,0x0018c152) */
/* WARNING: Removing unreachable block (ram,0x0018c1b8) */
/* WARNING: Removing unreachable block (ram,0x0018c1a4) */
/* WARNING: Removing unreachable block (ram,0x0018c1b0) */
/* WARNING: Removing unreachable block (ram,0x0018c1b2) */
/* WARNING: Removing unreachable block (ram,0x0018c1bc) */
/* WARNING: Removing unreachable block (ram,0x0018c352) */
/* WARNING: Removing unreachable block (ram,0x0018c218) */
/* WARNING: Removing unreachable block (ram,0x0018c3d4) */
/* WARNING: Removing unreachable block (ram,0x0018c3fe) */
/* WARNING: Removing unreachable block (ram,0x0018c3f2) */
/* WARNING: Removing unreachable block (ram,0x000908ca) */
/* WARNING: Removing unreachable block (ram,0x000908ce) */
/* WARNING: Removing unreachable block (ram,0x000908d2) */
/* WARNING: Removing unreachable block (ram,0x0020c15e) */
/* WARNING: Removing unreachable block (ram,0x000cbf1e) */
/* WARNING: Removing unreachable block (ram,0x000cbf22) */
/* WARNING: Removing unreachable block (ram,0x000cbf4e) */
/* WARNING: Removing unreachable block (ram,0x000cbf56) */
/* WARNING: Removing unreachable block (ram,0x000cbf5a) */
/* WARNING: Removing unreachable block (ram,0x000cbf5e) */
/* WARNING: Removing unreachable block (ram,0x000cbf7e) */
/* WARNING: Removing unreachable block (ram,0x000cbf82) */
/* WARNING: Removing unreachable block (ram,0x000cbf8e) */
/* WARNING: Removing unreachable block (ram,0x000cbf92) */
/* WARNING: Removing unreachable block (ram,0x001cc156) */
/* WARNING: Type propagation algorithm not settling */
/* AbyssEngine::AEMath::MatrixInverseRotateVector(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Vector const&) */

undefined4 AbyssEngine::AEMath::MatrixInverseRotateVector(Matrix *param_1,Vector *param_2)

{
  undefined4 uVar1;
  Matrix *pMVar2;
  undefined4 in_r2;
  int unaff_r4;
  uint unaff_r5;
  uint *unaff_r6;
  uint *unaff_r7;
  int unaff_r8;
  uint unaff_r9;
  uint unaff_r10;
  int unaff_r11;
  uint in_r12;
  uint unaff_lr;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  bool in_OV;
  undefined4 in_cr0;
  undefined4 in_cr8;
  
  if (!in_ZR) {
                    /* WARNING: Could not recover jumptable at 0x0010bf06. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    if (!in_OV) {
      uVar1 = (*(code *)(~((int)param_1 >> 0xc) & 0x10bf0e))();
      return uVar1;
    }
    *(Matrix **)(unaff_r4 + 4) = param_1;
    *(uint *)(unaff_r4 + 8) = unaff_r5;
    *(undefined4 *)(unaff_r4 + 0xc) = 0x10bf16;
                    /* WARNING: Could not recover jumptable at 0x0010bf16. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    if (in_NG) {
                    /* WARNING: Could not recover jumptable at 0x0010bf1a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar1 = (*(code *)((uint)param_1 & unaff_r8 >> 0x10))();
      return uVar1;
    }
    uVar1 = (*(code *)(&stack0x000010bf + in_CY))();
    return uVar1;
  }
  if (!in_CY) {
    pMVar2 = param_1;
    if (in_NG == in_OV) {
      pMVar2 = param_1 + -0x9cb;
      *(Matrix **)param_1 = pMVar2;
    }
    if (in_ZR) {
      pMVar2 = (Matrix *)(in_r12 ^ (int)unaff_r6 >> 0x11);
      *unaff_r6 = in_r12;
      unaff_r6[1] = unaff_lr;
      unaff_r6[2] = 0x1cbf16;
      *(Vector **)(unaff_r11 + 4) = param_2;
      *(undefined4 *)(unaff_r11 + 8) = in_r2;
      *(int *)(unaff_r11 + 0xc) = unaff_r11;
      *(uint *)(unaff_r11 + 0x10) = in_r12;
      *(uint *)(unaff_r11 + 0x14) = unaff_lr;
      *(undefined4 *)(unaff_r11 + 0x18) = 0x1cbf1a;
      *unaff_r6 = unaff_r9;
      unaff_r6[1] = unaff_r10;
      unaff_r6[2] = in_r12;
      unaff_r6[3] = unaff_lr;
      unaff_r6[4] = 0x1cbf1e;
    }
    coprocessor_function2(0,0,1,in_cr0,in_cr0,in_cr8);
    if (in_ZR || in_NG != in_OV) {
      *unaff_r6 = (uint)pMVar2;
    }
    if (in_CY && !in_ZR) {
      *(Matrix **)pMVar2 = pMVar2 + -0x9cb;
    }
    if (in_ZR) {
      *unaff_r7 = in_r12;
      unaff_r7[1] = (uint)register0x00000054;
      unaff_r7[2] = 0x1cbf32;
      *(Vector **)(unaff_r11 + 4) = param_2;
      *(undefined4 *)(unaff_r11 + 8) = in_r2;
      *(uint *)(unaff_r11 + 0xc) = in_r12;
      *(uint *)(unaff_r11 + 0x10) = unaff_lr;
      *(undefined4 *)(unaff_r11 + 0x14) = 0x1cbf36;
      *unaff_r7 = unaff_r9;
      unaff_r7[1] = unaff_r10;
      unaff_r7[2] = in_r12;
      unaff_r7[3] = (uint)register0x00000054;
      unaff_r7[4] = (uint)&UNK_001cbf3a;
    }
    if (in_ZR || in_NG != in_OV) {
      coprocessor_function(0,0,1,in_cr0,in_cr0,in_cr8);
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_NG) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  000908b6: bne 0x0010bf06
  000908ba: bcc 0x001cbf06
  000908be: bmi 0x0020bf0a
  000908c2: bpl 0x0024bf0e
  000908c6: bcc 0x0018c152
  000908ca: bmi 0x001cc156
  000908ce: beq 0x000cbf1e
  000908d2: bne 0x0020c15e
  000908d6: bvs 0x000cbf22
  000908da: bvc 0x0010bf26
  000908de: beq 0x0014c02a
  000908e2: bcs 0x0014bf32
  000908e6: bcc 0x0024c0ee
  000908ea: bpl 0x0030bf36
  000908ee: bmi 0x0028c0f6
  000908f2: bvs 0x0034bf3e
  000908f6: bne 0x008cc0fe
  000908fa: beq 0x002cbf46
  000908fe: bcc 0x000cc10e
  00090902: bmi 0x0020c112
  00090906: bne 0x0024c116
  0009090a: bcc 0x000cbf12
  0009090e: bmi 0x0010bf16
  00090912: bne 0x0014bf1a
  00090916: ldrblt r4,[r0,#0x770]!
  0009091a: stmdb sp!,{r0,r1,r8,r9,r10,r11,sp,pc}
  0009091e: stc p15,cr0,[sp,#-0x200]!
  00090922: andls r7,r1,r4, lsl #0x16
  00090926: stmdami pc!,{r3,r4,r7,r9,r10,lr}
  000cbf12: eors lr,r11,r12, lsr r0
  000cbf16: eors lr,r9,r10, lsr r0
  000cbf1a: eors lr,r7,r8, lsr r0
  000cbf1e: eors lr,r5,r6, lsr r0
  000cbf22: eors lr,r3,r4, lsr r0
  000cbf26: eors lr,r1,r2, lsr r0
  000cbf2a: eor lr,pc,r0, lsr r0
  000cbf2e: eor lr,sp,lr, lsr #32
  000cbf32: eor lr,r11,r12, lsr #32
  000cbf36: eor lr,r9,r10, lsr #32
  000cbf3a: eor lr,r7,r8, lsr #32
  000cbf3e: eor lr,r5,r6, lsr #32
  000cbf42: eor lr,r3,r4, lsr #32
  000cbf46: eor lr,r1,r2, lsr #32
  000cbf4a: ldrb lr,[pc,r0]!
  000cbf4e: ldrtmi r4,[r0],-r3,lsl #0xd
  000cbf52: pkhbtmi lr,r3,lr
  000cbf56: ands r4,r11,r0, asr #0xc
  000cbf5a: strbmi r4,[r8],-r3,lsl #0xd
  000cbf5e: ands lr,r5,r8, lsl r0
  000cbf62: ands lr,r3,r4, lsl r0
  000cbf66: ands lr,r1,r2, lsl r0
  000cbf6a: and lr,pc,r0, lsl r0
  000cbf6e: and lr,sp,lr
  000cbf72: and lr,r11,r12
  000cbf76: and lr,r9,r10
  000cbf7a: and lr,r7,r8
  000cbf7e: stmdage r0,{r0,r1,r7,r9,r10,lr}^
  000cbf82: swi 0x56f7a2
  000cbf86: and r11,r5,r12, lsl r9
  000cbf8a: ldrb lr,[pc,r0]!
  000cbf8e: strtmi r4,[r8],-r3,lsl #0xd
  000cbf92: ldcl p7,cr15,[r10,#0x288]
  0010bf06: bicvc pc,pc,r0, asr #0xc
  0010bf0a: ldrbtcc pc,[pc],pc,asr #32
  0010bf0e: stmib r4,{r0,r5,pc}^
  0010bf12: rscvs r5,r0,r1, lsl #0xc
  0010bf16: adcpls pc,sp,pc, asr #0x8
  0010bf1a: andmi pc,r0,r8, asr #0x10
  001cbf06: strge r0,[r0],#-0x9cb
  001cbf0a: eoreq r0,r12,r6, asr #0x11
  001cbf0e: stmiaeq r6,{r12,lr,pc}^
  001cbf12: stmibeq r11,{r1,r2,r11,r12,lr,pc}^
  001cbf16: stmiaeq r6,{r9,r10,r12,lr,pc}^
  001cbf1a: cdp2 p0,0x0,cr0,cr0,cr8,0x1
  001cbf1e: strle r0,[r6],#-0x8c6
  001cbf22: strhi r0,[r0],#-0x9cb
  001cbf26: eoreq r0,r12,r7, asr #0x11
  001cbf2a: stmiaeq r7,{r12,sp,pc}^
  001cbf2e: stmibeq r11,{r1,r2,r12,lr,pc}^
  001cbf32: stmiaeq r7,{r9,r10,r12,sp,pc}^
  001cbf36: cdple p0,0x0,cr0,cr0,cr8,0x1
  0020bf0a: eoreqs r0,r5,r0
  0020bf0e: eoreqs r0,sp,r0
  0020bf12: eoreqs r0,sp,r0
  0020bf16: eoreqs r0,sp,r0
  0020bf1a: eoreqs r0,sp,r0
  0020bf1e: eoreqs r0,sp,r0
  0020bf22: eoreqs r0,lr,r0
  0020bf26: stmiaeq r4,{}^
  0020bf2a: andeq r0,r0,r0
```
