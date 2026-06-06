# AbyssEngine::AEMath::MatrixInverseTransformVector
elf 0x80818 body 1
Sig: undefined __stdcall MatrixInverseTransformVector(Matrix * param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0010bfa0) */
/* WARNING: Removing unreachable block (ram,0x0010bfac) */
/* WARNING: Removing unreachable block (ram,0x0010bfa8) */
/* WARNING: Removing unreachable block (ram,0x0010bf94) */
/* WARNING: Removing unreachable block (ram,0x0010bfa4) */
/* WARNING: Removing unreachable block (ram,0x00090824) */
/* WARNING: Removing unreachable block (ram,0x0024be70) */
/* WARNING: Removing unreachable block (ram,0x00090828) */
/* WARNING: Removing unreachable block (ram,0x0009082c) */
/* WARNING: Removing unreachable block (ram,0x0010bf7c) */
/* WARNING: Removing unreachable block (ram,0x0010bf80) */
/* WARNING: Removing unreachable block (ram,0x0010bf84) */
/* WARNING: Removing unreachable block (ram,0x0010bf88) */
/* WARNING: Removing unreachable block (ram,0x0010bf8c) */
/* WARNING: Removing unreachable block (ram,0x0010bf90) */
/* WARNING: Removing unreachable block (ram,0x0010bf98) */
/* WARNING: Removing unreachable block (ram,0x00090830) */
/* WARNING: Removing unreachable block (ram,0x0020c1d4) */
/* WARNING: Removing unreachable block (ram,0x00090834) */
/* WARNING: Removing unreachable block (ram,0x0024c0d8) */
/* WARNING: Removing unreachable block (ram,0x0118c1a0) */
/* WARNING: Removing unreachable block (ram,0x00090838) */
/* WARNING: Removing unreachable block (ram,0x000cbe84) */
/* WARNING: Removing unreachable block (ram,0x000cbe88) */
/* WARNING: Removing unreachable block (ram,0x000cbe8c) */
/* WARNING: Removing unreachable block (ram,0x000cbe98) */
/* WARNING: Removing unreachable block (ram,0x000cbe9c) */
/* WARNING: Removing unreachable block (ram,0x000cbeac) */
/* WARNING: Removing unreachable block (ram,0x0009083c) */
/* WARNING: Removing unreachable block (ram,0xfe20c0c8) */
/* WARNING: Removing unreachable block (ram,0x00090840) */
/* WARNING: Removing unreachable block (ram,0x0018b1d0) */
/* WARNING: Removing unreachable block (ram,0x0018b016) */
/* WARNING: Removing unreachable block (ram,0x0018b1e0) */
/* WARNING: Removing unreachable block (ram,0x001ab8fc) */
/* WARNING: Removing unreachable block (ram,0x00090844) */
/* WARNING: Removing unreachable block (ram,0xfe1cc0d0) */
/* WARNING: Removing unreachable block (ram,0x00090848) */
/* WARNING: Removing unreachable block (ram,0x0010be94) */
/* WARNING: Removing unreachable block (ram,0x0009084c) */
/* WARNING: Removing unreachable block (ram,0xfe24c0d8) */
/* WARNING: Removing unreachable block (ram,0x00090850) */
/* WARNING: Removing unreachable block (ram,0x000cbfa0) */
/* WARNING: Removing unreachable block (ram,0x000cbfa4) */
/* WARNING: Removing unreachable block (ram,0x000cbfd0) */
/* WARNING: Removing unreachable block (ram,0x000cbfe0) */
/* WARNING: Removing unreachable block (ram,0x000cbfea) */
/* WARNING: Removing unreachable block (ram,0x000cbffa) */
/* WARNING: Removing unreachable block (ram,0x000cc00a) */
/* WARNING: Removing unreachable block (ram,0x00156240) */
/* WARNING: Removing unreachable block (ram,0x001563f0) */
/* WARNING: Removing unreachable block (ram,0x001563fe) */
/* WARNING: Removing unreachable block (ram,0x00156520) */
/* WARNING: Removing unreachable block (ram,0x00156412) */
/* WARNING: Removing unreachable block (ram,0x00156244) */
/* WARNING: Removing unreachable block (ram,0x000cc014) */
/* WARNING: Removing unreachable block (ram,0x000cc018) */
/* WARNING: Removing unreachable block (ram,0x000cc01c) */
/* WARNING: Removing unreachable block (ram,0x000cc024) */
/* WARNING: Removing unreachable block (ram,0x000cc028) */
/* WARNING: Removing unreachable block (ram,0x000cc02c) */
/* WARNING: Removing unreachable block (ram,0x000cc030) */
/* WARNING: Removing unreachable block (ram,0x000cc034) */
/* WARNING: Removing unreachable block (ram,0x00156260) */
/* WARNING: Removing unreachable block (ram,0x00156262) */
/* WARNING: Removing unreachable block (ram,0x001563e0) */
/* WARNING: Removing unreachable block (ram,0x001562cc) */
/* WARNING: Removing unreachable block (ram,0x001563e8) */
/* WARNING: Removing unreachable block (ram,0x00156526) */
/* WARNING: Removing unreachable block (ram,0x001566c8) */
/* WARNING: Removing unreachable block (ram,0x00156574) */
/* WARNING: Removing unreachable block (ram,0x00156580) */
/* WARNING: Removing unreachable block (ram,0x00156582) */
/* WARNING: Removing unreachable block (ram,0x0015658a) */
/* WARNING: Removing unreachable block (ram,0x0015658c) */
/* WARNING: Removing unreachable block (ram,0x00156632) */
/* WARNING: Removing unreachable block (ram,0x00156592) */
/* WARNING: Removing unreachable block (ram,0x001566a2) */
/* WARNING: Removing unreachable block (ram,0x0015662a) */
/* WARNING: Removing unreachable block (ram,0x001566b6) */
/* WARNING: Removing unreachable block (ram,0x001566c6) */
/* WARNING: Removing unreachable block (ram,0x001566ce) */
/* WARNING: Removing unreachable block (ram,0x001566e8) */
/* WARNING: Removing unreachable block (ram,0x001566d8) */
/* WARNING: Removing unreachable block (ram,0x000cc004) */
/* WARNING: Removing unreachable block (ram,0x000cbff4) */
/* WARNING: Removing unreachable block (ram,0x000cc038) */
/* WARNING: Removing unreachable block (ram,0x000cc0ba) */
/* WARNING: Removing unreachable block (ram,0x000cc0b6) */
/* WARNING: Removing unreachable block (ram,0x000cc0ce) */
/* WARNING: Removing unreachable block (ram,0x000cc0d6) */
/* WARNING: Removing unreachable block (ram,0x000cc0de) */
/* WARNING: Removing unreachable block (ram,0x000cc0e2) */
/* WARNING: Removing unreachable block (ram,0x000cc0ec) */
/* WARNING: Removing unreachable block (ram,0x000cc0f6) */
/* WARNING: Removing unreachable block (ram,0x000cc100) */
/* WARNING: Removing unreachable block (ram,0x000cc10e) */
/* WARNING: Removing unreachable block (ram,0x000cc170) */
/* WARNING: Removing unreachable block (ram,0x000cc178) */
/* WARNING: Removing unreachable block (ram,0x000cc18e) */
/* WARNING: Removing unreachable block (ram,0x000cc18a) */
/* WARNING: Removing unreachable block (ram,0x000cc1a2) */
/* WARNING: Removing unreachable block (ram,0x000cc1a6) */
/* WARNING: Removing unreachable block (ram,0x000cc1b8) */
/* WARNING: Removing unreachable block (ram,0x000cc1b0) */
/* WARNING: Removing unreachable block (ram,0x00090854) */
/* WARNING: Removing unreachable block (ram,0x0014bea0) */
/* WARNING: Removing unreachable block (ram,0x00090858) */
/* WARNING: Removing unreachable block (ram,0x0009085c) */
/* WARNING: Removing unreachable block (ram,0x0110c1b0) */
/* WARNING: Removing unreachable block (ram,0x00090860) */
/* WARNING: Removing unreachable block (ram,0x0014bfb0) */
/* WARNING: Removing unreachable block (ram,0x0014bfc2) */
/* WARNING: Removing unreachable block (ram,0x0014c02a) */
/* WARNING: Removing unreachable block (ram,0x0014c02e) */
/* WARNING: Removing unreachable block (ram,0x0014c032) */
/* WARNING: Removing unreachable block (ram,0x0014c036) */
/* WARNING: Removing unreachable block (ram,0x0014c03a) */
/* WARNING: Removing unreachable block (ram,0x0014c046) */
/* WARNING: Removing unreachable block (ram,0x00154e36) */
/* WARNING: Removing unreachable block (ram,0x00154e48) */
/* WARNING: Removing unreachable block (ram,0x00154e4e) */
/* WARNING: Removing unreachable block (ram,0x00154e94) */
/* WARNING: Removing unreachable block (ram,0x00154ea0) */
/* WARNING: Removing unreachable block (ram,0x00154ea4) */
/* WARNING: Removing unreachable block (ram,0x00154ea8) */
/* WARNING: Removing unreachable block (ram,0x00154f80) */
/* WARNING: Removing unreachable block (ram,0x00154fae) */
/* WARNING: Removing unreachable block (ram,0x00154f88) */
/* WARNING: Removing unreachable block (ram,0x00154f9a) */
/* WARNING: Removing unreachable block (ram,0x00154fac) */
/* WARNING: Removing unreachable block (ram,0x00154fb4) */
/* WARNING: Removing unreachable block (ram,0x00154eae) */
/* WARNING: Removing unreachable block (ram,0x00154ebe) */
/* WARNING: Removing unreachable block (ram,0x00154ee2) */
/* WARNING: Removing unreachable block (ram,0x00154eca) */
/* WARNING: Removing unreachable block (ram,0x00154ed8) */
/* WARNING: Removing unreachable block (ram,0x00154ee0) */
/* WARNING: Removing unreachable block (ram,0x00154ee8) */
/* WARNING: Removing unreachable block (ram,0x00154f14) */
/* WARNING: Removing unreachable block (ram,0x00154ef4) */
/* WARNING: Removing unreachable block (ram,0x00154f06) */
/* WARNING: Removing unreachable block (ram,0x00154f12) */
/* WARNING: Removing unreachable block (ram,0x00154f20) */
/* WARNING: Removing unreachable block (ram,0x00154f56) */
/* WARNING: Removing unreachable block (ram,0x00154f5e) */
/* WARNING: Removing unreachable block (ram,0x00090864) */
/* WARNING: Removing unreachable block (ram,0x0114c0b8) */
/* WARNING: Removing unreachable block (ram,0x00090868) */
/* WARNING: Removing unreachable block (ram,0x0030bfb4) */
/* WARNING: Removing unreachable block (ram,0x0009086c) */
/* WARNING: Removing unreachable block (ram,0xfe10c074) */
/* WARNING: Removing unreachable block (ram,0x00090870) */
/* WARNING: Removing unreachable block (ram,0x0038bfbc) */
/* WARNING: Removing unreachable block (ram,0x00090874) */
/* WARNING: Removing unreachable block (ram,0xfe0cc07c) */
/* WARNING: Removing unreachable block (ram,0x00090878) */
/* WARNING: Removing unreachable block (ram,0x002cbec4) */
/* WARNING: Removing unreachable block (ram,0x0009087c) */
/* WARNING: Removing unreachable block (ram,0xfe14c084) */
/* WARNING: Removing unreachable block (ram,0x00090880) */
/* WARNING: Removing unreachable block (ram,0x0034bfcc) */
/* WARNING: Removing unreachable block (ram,0x00090884) */
/* WARNING: Removing unreachable block (ram,0x01a4c190) */
/* WARNING: Removing unreachable block (ram,0x00090888) */
/* WARNING: Removing unreachable block (ram,0xff90c1a8) */
/* WARNING: Removing unreachable block (ram,0x0009088c) */
/* WARNING: Removing unreachable block (ram,0xffa0c0ac) */
/* WARNING: Removing unreachable block (ram,0x00090890) */
/* WARNING: Removing unreachable block (ram,0xfe90c0a0) */
/* WARNING: Removing unreachable block (ram,0x00090894) */
/* WARNING: Removing unreachable block (ram,0xfe10c0a4) */
/* WARNING: Removing unreachable block (ram,0x00090898) */
/* WARNING: Removing unreachable block (ram,0xfea0c0a8) */
/* WARNING: Removing unreachable block (ram,0x0009089c) */
/* WARNING: Removing unreachable block (ram,0x009cc174) */
/* WARNING: Removing unreachable block (ram,0x000908a0) */
/* WARNING: Removing unreachable block (ram,0x0098c17c) */
/* WARNING: Removing unreachable block (ram,0x000908a4) */
/* WARNING: Removing unreachable block (ram,0x0028c184) */
/* WARNING: Removing unreachable block (ram,0x000908a8) */
/* WARNING: Removing unreachable block (ram,0x000cbeb0) */
/* WARNING: Removing unreachable block (ram,0x000cbed0) */
/* WARNING: Removing unreachable block (ram,0x000cbed4) */
/* WARNING: Removing unreachable block (ram,0x000908ac) */
/* WARNING: Removing unreachable block (ram,0x0010beb4) */
/* WARNING: Removing unreachable block (ram,0x000908b0) */
/* WARNING: Removing unreachable block (ram,0x000908b4) */
/* WARNING: Removing unreachable block (ram,0x00090914) */
/* WARNING: Removing unreachable block (ram,0x00090918) */
/* WARNING: Removing unreachable block (ram,0x0009091c) */
/* WARNING: Removing unreachable block (ram,0x00090920) */
/* WARNING: Removing unreachable block (ram,0x00090924) */
/* WARNING: Removing unreachable block (ram,0x00090938) */
/* WARNING: Removing unreachable block (ram,0x004cc160) */
/* WARNING: Removing unreachable block (ram,0x0009093c) */
/* WARNING: Removing unreachable block (ram,0x0014beb8) */
/* WARNING: Removing unreachable block (ram,0x0014beba) */
/* WARNING: Removing unreachable block (ram,0x0014bec8) */
/* WARNING: Removing unreachable block (ram,0x0014bf34) */
/* WARNING: Removing unreachable block (ram,0x0014bf30) */
/* WARNING: Removing unreachable block (ram,0x0014cef0) */
/* WARNING: Removing unreachable block (ram,0x0014d8da) */
/* WARNING: Removing unreachable block (ram,0x0014d8f2) */
/* WARNING: Removing unreachable block (ram,0x00154f60) */
/* WARNING: Removing unreachable block (ram,0x00154f7c) */
/* WARNING: Removing unreachable block (ram,0x00154f6a) */
/* WARNING: Removing unreachable block (ram,0x0028c1c0) */
/* AbyssEngine::AEMath::MatrixInverseTransformVector(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::MatrixInverseTransformVector(Matrix *param_1,Vector *param_2)

{
  undefined4 *in_r2;
  undefined4 *unaff_r6;
  undefined4 unaff_r7;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  int in_r12;
  undefined4 unaff_lr;
  bool in_NG;
  bool in_ZR;
  bool in_CY;
  bool in_OV;
  
  if (in_NG) {
    if (!in_CY || in_ZR) {
      iRam00000000 = -(in_r12 >> 0x13);
    }
    else {
      *unaff_r6 = unaff_r11;
      unaff_r6[-1] = unaff_r7;
      unaff_r6[-2] = unaff_r6;
      unaff_r6[-3] = param_2;
    }
    if (in_ZR) {
      *in_r2 = unaff_r9;
      in_r2[1] = unaff_r10;
      in_r2[2] = unaff_r11;
      in_r2[3] = in_r12;
      in_r2[4] = register0x00000054;
      in_r2[5] = &UNK_001cbe7c;
      *(Vector **)(in_r12 + 4) = param_2;
      *(undefined4 **)(in_r12 + 8) = in_r2;
      *(undefined4 *)(in_r12 + 0xc) = unaff_r10;
      *(undefined **)(in_r12 + 0x10) = &UNK_001cbe80;
      *in_r2 = unaff_r10;
      in_r2[1] = unaff_lr;
      in_r2[2] = &UNK_001cbe84;
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (in_OV) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```
## target disasm
```
```
