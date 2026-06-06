# AbyssEngine::AEMath::MatrixRotateVector
elf 0x807b6 body 463
Sig: undefined __stdcall MatrixRotateVector(Matrix * param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0014bf32) overlaps instruction at (ram,0x0014bf30)
    */
/* WARNING: Removing unreachable block (ram,0x001cbe2a) */
/* WARNING: Removing unreachable block (ram,0x0014be32) */
/* WARNING: Removing unreachable block (ram,0x000907d6) */
/* WARNING: Removing unreachable block (ram,0x000907da) */
/* WARNING: Removing unreachable block (ram,0x001cbe26) */
/* WARNING: Removing unreachable block (ram,0x001cbe3a) */
/* WARNING: Removing unreachable block (ram,0x001cbe3e) */
/* WARNING: Removing unreachable block (ram,0x001cbe42) */
/* WARNING: Removing unreachable block (ram,0x001cbe46) */
/* WARNING: Removing unreachable block (ram,0x000907de) */
/* WARNING: Removing unreachable block (ram,0x000907e2) */
/* WARNING: Removing unreachable block (ram,0x000907e6) */
/* WARNING: Removing unreachable block (ram,0x0028bfee) */
/* WARNING: Removing unreachable block (ram,0x001cc016) */
/* WARNING: Removing unreachable block (ram,0x001cc006) */
/* WARNING: Removing unreachable block (ram,0x001cbff6) */
/* WARNING: Removing unreachable block (ram,0x000907ea) */
/* WARNING: Removing unreachable block (ram,0x0014be36) */
/* WARNING: Removing unreachable block (ram,0x000907ee) */
/* WARNING: Removing unreachable block (ram,0x001cbffa) */
/* WARNING: Removing unreachable block (ram,0x001cbffe) */
/* WARNING: Removing unreachable block (ram,0x001cc002) */
/* WARNING: Removing unreachable block (ram,0x001cc01a) */
/* WARNING: Removing unreachable block (ram,0x0020bf0a) */
/* WARNING: Removing unreachable block (ram,0x0020bf0e) */
/* WARNING: Removing unreachable block (ram,0x0020bf12) */
/* WARNING: Removing unreachable block (ram,0x0020bf16) */
/* WARNING: Removing unreachable block (ram,0x0020bf1a) */
/* WARNING: Removing unreachable block (ram,0x0020bf1e) */
/* WARNING: Removing unreachable block (ram,0x0020bf22) */
/* WARNING: Removing unreachable block (ram,0x0020bf26) */
/* WARNING: Removing unreachable block (ram,0x0020bf2e) */
/* WARNING: Removing unreachable block (ram,0x0020bf32) */
/* WARNING: Removing unreachable block (ram,0x000908be) */
/* WARNING: Removing unreachable block (ram,0x000908c2) */
/* WARNING: Removing unreachable block (ram,0x0024bf0e) */
/* WARNING: Removing unreachable block (ram,0x000908ba) */
/* WARNING: Removing unreachable block (ram,0x001cbf06) */
/* WARNING: Removing unreachable block (ram,0x001cbf0a) */
/* WARNING: Removing unreachable block (ram,0x001cbf1a) */
/* WARNING: Removing unreachable block (ram,0x001cbf1e) */
/* WARNING: Removing unreachable block (ram,0x001cbf22) */
/* WARNING: Removing unreachable block (ram,0x0014c02a) */
/* WARNING: Removing unreachable block (ram,0x0014c02e) */
/* WARNING: Removing unreachable block (ram,0xfe5aa61a) */
/* WARNING: Removing unreachable block (ram,0x000908ea) */
/* WARNING: Removing unreachable block (ram,0x0030bf36) */
/* WARNING: Removing unreachable block (ram,0x000908de) */
/* WARNING: Removing unreachable block (ram,0x000908e2) */
/* WARNING: Removing unreachable block (ram,0x0014bf32) */
/* WARNING: Removing unreachable block (ram,0x0014bf36) */
/* WARNING: Removing unreachable block (ram,0x000908e6) */
/* WARNING: Removing unreachable block (ram,0x0024c0ee) */
/* WARNING: Removing unreachable block (ram,0x002cbf46) */
/* WARNING: Removing unreachable block (ram,0x0020c112) */
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
/* WARNING: Removing unreachable block (ram,0x00090902) */
/* WARNING: Removing unreachable block (ram,0x00090906) */
/* WARNING: Removing unreachable block (ram,0x0024c116) */
/* WARNING: Removing unreachable block (ram,0x000908ee) */
/* WARNING: Removing unreachable block (ram,0x000908f2) */
/* WARNING: Removing unreachable block (ram,0x000908f6) */
/* WARNING: Removing unreachable block (ram,0x008cc0fe) */
/* WARNING: Removing unreachable block (ram,0x0009090a) */
/* WARNING: Removing unreachable block (ram,0x0009090e) */
/* WARNING: Removing unreachable block (ram,0x00090912) */
/* WARNING: Removing unreachable block (ram,0x00090916) */
/* WARNING: Removing unreachable block (ram,0x0009091a) */
/* WARNING: Removing unreachable block (ram,0x00090922) */
/* WARNING: Removing unreachable block (ram,0x00090926) */
/* WARNING: Removing unreachable block (ram,0x0014bf1a) */
/* WARNING: Removing unreachable block (ram,0x0014bf34) */
/* WARNING: Removing unreachable block (ram,0x0014bf30) */
/* WARNING: Removing unreachable block (ram,0x000cbf12) */
/* WARNING: Removing unreachable block (ram,0x0009092a) */
/* WARNING: Removing unreachable block (ram,0x0028c0f6) */
/* WARNING: Removing unreachable block (ram,0x0014bf3a) */
/* WARNING: Removing unreachable block (ram,0x0014c032) */
/* WARNING: Removing unreachable block (ram,0x0014c036) */
/* WARNING: Removing unreachable block (ram,0x0014c03a) */
/* WARNING: Removing unreachable block (ram,0x0014c046) */
/* WARNING: Removing unreachable block (ram,0x000908d6) */
/* WARNING: Removing unreachable block (ram,0x000908da) */
/* WARNING: Removing unreachable block (ram,0x0010bf26) */
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
/* WARNING: Removing unreachable block (ram,0x001cbf26) */
/* WARNING: Removing unreachable block (ram,0x001cbf36) */
/* WARNING: Removing unreachable block (ram,0x001cbf3a) */
/* WARNING: Removing unreachable block (ram,0x00090816) */
/* WARNING: Removing unreachable block (ram,0x000908b2) */
/* WARNING: Removing unreachable block (ram,0x000908b6) */
/* WARNING: Removing unreachable block (ram,0x0010bf06) */
/* WARNING: Removing unreachable block (ram,0x0010bf0a) */
/* WARNING: Removing unreachable block (ram,0x0010bf0e) */
/* WARNING: Removing unreachable block (ram,0x0010bf12) */
/* WARNING: Removing unreachable block (ram,0x0010bf16) */
/* WARNING: Removing unreachable block (ram,0x0010bf1a) */
/* WARNING: Removing unreachable block (ram,0x0010bf1e) */
/* WARNING: Removing unreachable block (ram,0x00090812) */
/* WARNING: Removing unreachable block (ram,0x0014be1a) */
/* WARNING: Removing unreachable block (ram,0x0014be30) */
/* WARNING: Removing unreachable block (ram,0x0014be34) */
/* WARNING: Removing unreachable block (ram,0x0014be38) */
/* WARNING: Removing unreachable block (ram,0x008872c0) */
/* WARNING: Removing unreachable block (ram,0x0014be40) */
/* WARNING: Removing unreachable block (ram,0x000907f2) */
/* WARNING: Removing unreachable block (ram,0x0034bf3e) */
/* WARNING: Removing unreachable block (ram,0x000907f6) */
/* WARNING: Removing unreachable block (ram,0x008cbffe) */
/* WARNING: Removing unreachable block (ram,0x000907fa) */
/* WARNING: Removing unreachable block (ram,0x0024be46) */
/* WARNING: Removing unreachable block (ram,0x000907fe) */
/* WARNING: Removing unreachable block (ram,0x000cc00e) */
/* WARNING: Removing unreachable block (ram,0x000cc012) */
/* WARNING: Removing unreachable block (ram,0x00090802) */
/* WARNING: Removing unreachable block (ram,0x0024c012) */
/* WARNING: Removing unreachable block (ram,0x00090806) */
/* WARNING: Removing unreachable block (ram,0x0094c016) */
/* WARNING: Removing unreachable block (ram,0x0009080a) */
/* WARNING: Removing unreachable block (ram,0x0009080e) */
/* WARNING: Removing unreachable block (ram,0x000cbe12) */
/* WARNING: Removing unreachable block (ram,0x002cbf2a) */
/* WARNING: Removing unreachable block (ram,0x001cbe56) */
/* WARNING: Removing unreachable block (ram,0x001cbe5a) */
/* WARNING: Removing unreachable block (ram,0x000907d2) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* AbyssEngine::AEMath::MatrixRotateVector(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::MatrixRotateVector(Matrix *param_1,Vector *param_2)

{
  undefined4 *in_r2;
  int in_r3;
  int unaff_r4;
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *unaff_r5;
  undefined4 *unaff_r6;
  int unaff_r7;
  undefined4 unaff_r8;
  int *unaff_r9;
  int *piVar3;
  int unaff_r10;
  int unaff_r11;
  byte *pbVar4;
  int *in_r12;
  undefined4 unaff_lr;
  int iVar5;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  char in_OV;
  bool bVar6;
  undefined4 in_cr2;
  undefined4 in_cr10;
  undefined4 in_cr11;
  undefined4 in_cr12;
  undefined4 in_cr15;
  
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_CY) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_NG) {
    if (in_CY) {
      coprocessor_loadlong(0xf,in_cr11,param_1);
    }
    if (in_NG) {
      unaff_r9[-1] = (int)unaff_r5;
      unaff_r9[-2] = in_r3;
      unaff_r9[-3] = (int)param_1;
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_ZR) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_CY) {
    pbVar4 = (byte *)(unaff_r11 + unaff_r4 * 0x1000);
    puVar1 = (undefined4 *)(uint)*pbVar4;
    if (in_NG == (bool)in_OV) {
      *unaff_r6 = unaff_lr;
      unaff_r6[-1] = unaff_r10;
      unaff_r6[-2] = unaff_r9;
      unaff_r6[-3] = in_r2;
    }
    coprocessor_function(6,0xe,7,in_cr15,in_cr10,in_cr2);
    if (in_NG) {
      puVar2 = (undefined4 *)((int)puVar1 - (int)unaff_r9);
      *puVar1 = unaff_lr;
      puVar1 = (undefined4 *)((int)puVar2 - unaff_r10);
      *puVar2 = unaff_lr;
      *puVar1 = unaff_lr;
      *(undefined4 *)((int)puVar1 - (int)pbVar4) = unaff_lr;
    }
    if ((int)((uint)param_2 & (uint)&stack0x00000000) < 0 == (bool)in_OV) {
      *(uint *)param_1 = (uint)param_2 & (uint)&stack0x00000000;
      *(int *)(param_1 + -4) = unaff_r10;
      *(int **)(param_1 + -8) = unaff_r9;
      *(undefined4 **)(param_1 + -0xc) = in_r2;
    }
    coprocessor_function(6,0xd,7,in_cr15,in_cr12,in_cr2);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_NG) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_OV == '\x01') {
    unaff_r10 = unaff_r6[-1];
    unaff_r9 = (int *)unaff_r6[-2];
    unaff_r7 = unaff_r6[-3];
    param_2 = (Vector *)unaff_r6[-4];
    param_1 = (Matrix *)unaff_r6[-5];
  }
  iVar5 = (int)unaff_r5 - (int)in_r2;
  bVar6 = iVar5 == 0;
  piVar3 = unaff_r9;
  if ((int)in_r2 <= (int)unaff_r5) {
    iVar5 = *unaff_r9;
    unaff_r10 = unaff_r9[-1];
    piVar3 = (int *)unaff_r9[-2];
    unaff_r7 = unaff_r9[-3];
    param_2 = (Vector *)unaff_r9[-4];
    param_1 = (Matrix *)unaff_r9[-5];
  }
  *in_r2 = 0xcbe32;
  in_r2[-1] = iVar5;
  in_r2[-2] = register0x00000054;
  in_r2[-3] = in_r12;
  in_r2[-4] = unaff_r10;
  in_r2[-5] = piVar3;
  in_r2[-6] = unaff_r8;
  in_r2[-7] = unaff_r7;
  in_r2[-8] = unaff_r5;
  in_r2[-9] = param_2;
  in_r2[-10] = param_1;
  if (unaff_r5 < in_r2 || bVar6) {
    *in_r12 = 0xcbe36 - ((int)param_2 >> ((uint)param_1 & 0xff));
    in_r12[-1] = unaff_r10;
    in_r12[-2] = (int)piVar3;
    in_r12[-3] = unaff_r7;
    in_r12[-4] = (int)param_2;
    in_r12[-5] = (int)param_1;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
  000907b6: bne 0x0010be06
  000907ba: bcc 0x0020be06
  000907be: bpl 0x0010be0a
  000907c2: bne 0x0030bf0e
  000907c6: bcc 0x0018c052
  000907ca: bpl 0x0020c056
  000907ce: beq 0x000cbe1e
  000907d2: bne 0x0090c05e
  000907d6: bmi 0x000cbe22
  000907da: bvc 0x001cbe26
  000907de: beq 0x002cbf2a
  000907e2: bcs 0x0014be32
  000907e6: bcc 0x0028bfee
  000907ea: bvs 0x0014be36
  000907ee: bpl 0x001cbff6
  000907f2: bcs 0x0034bf3e
  000907f6: bne 0x008cbffe
  000907fa: beq 0x0024be46
  000907fe: bcc 0x000cc00e
  00090802: bpl 0x0024c012
  00090806: bne 0x0094c016
  0009080a: bpl 0x000cbe12
  0009080e: bcc 0x0010be16
  00090812: bne 0x0014be1a
  00090816: ldc p7,cr4,[r1,#0x1c0]
  0009081a: vldr.32 s8,[r1,#0x10]
  0009081e: vldr.32 s14,[r1,#0x1c]
  00090822: vldr.32 s10,[r1,#0x14]
  00090826: vmul.f32 s13,s8,s12
  0009082a: vldr.32 s9,[r2,#0x1c]
  0009082e: vmul.f32 s3,s14,s2
  00090832: vmul.f32 s6,s14,s10
  00090836: vldr.32 s14,[r1,#0x18]
  0009083a: vmul.f32 s0,s2,s0
  0009083e: vldr.32 s10,[r1,#0x214]
  00090842: vmul.f32 s6,s2,s6
  00090846: vldr.32 s8,[r1,#0x210]
  0009084a: vmul.f32 s2,s2,s2
  0009084e: vldr.32 s13,[r2,#0x218]
  00090852: vldr.32 s0,[r1]
  00090856: vnmls.f32 s5,s0,s4
  0009085a: vnmla.f32 s9,s6,s6
  0009085e: vldr.32 s7,[r2,#0x104]
  00090862: vnmls.f32 s4,s6,s4
  00090866: vldr.32 s15,[r1,#0x108]
  0009086a: vmla.f32 s2,s0,s18
  0009086e: vldr.32 s11,[r1,#0x204]
  00090872: vmla.f32 s12,s0,s22
  00090876: vldr.32 s8,[r1,#0x200]
  0009087a: vmla.f32 s2,s0,s16
  0009087e: vldr.32 s13,[r1,#0x208]
  00090882: vmla.f32 s11,s2,s20
  00090886: vmls.f32 s9,s12,s13
  0009088a: vmls.f32 s6,s13,s3
  0009088e: vmls.f32 s14,s5,s11
  00090892: vmla.f32 s10,s5,s3
  00090896: vmla.f32 s8,s5,s2
  0009089a: vadd.f32 s12,s9,s11
  0009089e: vadd.f32 s2,s10,s9
  000908a2: vadd.f32 s0,s12,s7
  000908a6: vstr.32 s4,[r0,#0x1c]
  000908aa: vstr.32 s2,[r0]
  000908ae: vstr.32 s0,[r0,#0x4]
  000908b2: ldrbmi r2,[r0,-r2,lsl #0x14]!
  000cbe12: ldrht lr,[r11],r12
  000cbe16: ldrht lr,[r9],r10
  000cbe1a: ldrht lr,[r7],r8
  000cbe1e: ldmdage r6,{r0,r1,r7,r9,r10,lr}
  000cbe22: subs lr,r5,r2
  000cbe26: ldmdage r9,{r0,r1,r7,r9,r10,lr}
  000cbe2a: stmda r2,{r0,r1,r5,r7,r8,r9,r10,r12,sp,lr,pc}
  000cbe2e: sub lr,pc,r1, asr r0
  000cbe32: stmdals r12,{r0,r1,r7,r9,r10,lr}
  000cbe36: add lr,pc,r12, lsr #0x1
  000cbe3a: adc lr,r7,r8, lsr #0x1
  000cbe3e: adc lr,r5,r6, lsr #0x1
  000cbe42: ldmdage r12,{r0,r1,r7,r9,r10,lr}
  000cbe46: adc lr,r1,r6, asr #32
  000cbe4a: adds lr,pc,r0, lsr #0x1
  000cc00e: stmiacs r10,{r4,r5,r7,r8,r9,r11,sp,lr,pc}
  0010be0a: and lr,r2,r12, lsl #0x1d
  0010be0e: ldclcs p15,cr11,[r0,#0x0]!
  0010be12: stmdbmi r9,{r0,r3,r5}
  0014be32: strtmi r0,[r1],-r8,lsr #0xa
  0018c052: ldrb r4,[r11,r4,lsl #0xc]!
  0018c056: stmdage r6!,{r2,r9,r10,lr}
  0018c05a: cdp p6,0xe,cr15,cr10,cr2,0x7
  0018c05e: strmi lr,[r4],-r9
  0018c062: strmi lr,[r4],-r10
  0018c066: strmi lr,[r4],-r11
  0018c06a: strmi lr,[r4],-r12
  0018c06e: ands lr,r1,sp
  0018c072: stmdage r0!,{r2,r9,r10,lr}
  0018c076: cdp p6,0xd,cr15,cr12,cr2,0x7
  001cbe26: strge r0,[r0],#-0x9cc
  001cbe2a: eoreq r0,r12,r0, asr #0x11
  001cbe2e: stmiaeq r0,{r12,lr,pc}^
  001cbe32: stmibeq r12,{r1,r2,r11,r12,sp,pc}^
  001cbe36: stmiaeq r0,{r9,r10,r12,lr,pc}^
  001cbe3a: cdp2 p0,0x0,cr0,cr0,cr8,0x1
  001cbe3e: strlt r0,[r6],#-0x8c0
  001cbe42: strhi r0,[r0],#-0x9cc
  001cbe46: eoreq r0,r12,r1, asr #0x11
  001cbe4a: stmiaeq r1,{r12,sp,pc}^
  001cbe4e: stmibeq r12,{r1,r2,r12,sp,pc}^
  001cbe52: stmiaeq r1,{r9,r10,r12,sp,pc}^
  001cbe56: cdple p0,0x0,cr0,cr0,cr8,0x1
  001cbff6: stmiaeq r12,{r9,r10,r12,lr,pc}^
  001cbffa: cdp2 p0,0x0,cr0,cr0,cr8,0x1
  001cbffe: strls r0,[r6],#-0x8cc
  001cc002: strhi r0,[r0],#-0x9cb
  001cc006: eoreq r0,r12,sp, asr #0x11
  001cc00a: stmiaeq sp,{r12,sp,pc}^
  001cc00e: stmibeq r11,{r1,r2,r12,pc}^
  001cc012: stmiaeq sp,{r9,r10,r12,sp,pc}^
  001cc016: cdple p0,0x0,cr0,cr0,cr8,0x1
```
