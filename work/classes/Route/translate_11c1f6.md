# Route::translate
elf 0x11c1f6 body 56
Sig: undefined __stdcall translate(Vector * param_1)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0012c202) */
/* WARNING: Removing unreachable block (ram,0x001e784e) */
/* WARNING: Removing unreachable block (ram,0x001e7852) */
/* WARNING: Removing unreachable block (ram,0x001e7856) */
/* WARNING: Removing unreachable block (ram,0x001e785a) */
/* WARNING: Removing unreachable block (ram,0x001e7862) */
/* WARNING: Removing unreachable block (ram,0x001e786e) */
/* WARNING: Removing unreachable block (ram,0x0012c206) */
/* WARNING: Removing unreachable block (ram,0x0012c20a) */
/* WARNING: Removing unreachable block (ram,0x0012c20e) */
/* Route::translate(AbyssEngine::AEMath::Vector const&) */

void Route::translate(Vector *param_1)

{
  undefined4 in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined4 *unaff_r4;
  int unaff_r5;
  undefined4 unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 in_r12;
  undefined4 unaff_lr;
  bool in_ZR;
  bool in_OV;
  undefined4 in_cr15;
  
  if (!in_ZR) {
    coprocessor_loadlong2(7,in_cr15,unaff_r5 + -0x3f0);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *unaff_r4 = &UNK_0016784e;
  unaff_r4[-1] = unaff_lr;
  unaff_r4[-2] = register0x00000054;
  unaff_r4[-3] = in_r12;
  unaff_r4[-4] = unaff_r10;
  unaff_r4[-5] = unaff_r9;
  unaff_r4[-6] = unaff_r8;
  unaff_r4[-7] = in_r3;
  unaff_r4[-8] = in_r2;
  unaff_r4[-9] = in_r1;
  if (!in_OV) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## target disasm
```
  0012c1f6: andcs r6,r0,#0xc00000
  0012c1fa: beq 0x00167846
  0012c1fe: bne 0x001a784a
  0012c202: bcs 0x001e784e
  0012c206: addmis r6,r1,#0x10000
  0012c20a: ldrbmi r11,[r0,-r8,lsl #0x1e]!
  001e784e: swics 0x736568
  001e7852: rsbvc r6,r9,r3, ror r8
  001e7856: smcvs 0x3
  001e785a: swipl 0x6f6772
  001e785e: swipl 0x323030
  001e7862: ldrbvs r6,[r6,#-0x96e]!
  001e7866: vnmulvs.f16 s13,s2,s25
  001e786a: strbtvs r6,[pc],#-0xc5f
```
