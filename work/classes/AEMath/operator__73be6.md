# AbyssEngine::AEMath::operator/
elf 0x73be6 body 1
Sig: undefined __stdcall operator/(Vector * param_1, float param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0017f5fe) */
/* WARNING: Removing unreachable block (ram,0x00083bf2) */
/* WARNING: Removing unreachable block (ram,0x00083bf6) */
/* WARNING: Removing unreachable block (ram,0x00083bfa) */
/* WARNING: Removing unreachable block (ram,0x0017f606) */
/* WARNING: Removing unreachable block (ram,0x0013f212) */
/* WARNING: Removing unreachable block (ram,0x00083c0a) */
/* WARNING: Removing unreachable block (ram,0x00083c0e) */
/* WARNING: Removing unreachable block (ram,0x00083c36) */
/* WARNING: Removing unreachable block (ram,0x00083c3a) */
/* WARNING: Removing unreachable block (ram,0x00083c3e) */
/* WARNING: Removing unreachable block (ram,0x00083c4a) */
/* WARNING: Removing unreachable block (ram,0x00083c4e) */
/* WARNING: Removing unreachable block (ram,0x00083c52) */
/* WARNING: Removing unreachable block (ram,0x00083c56) */
/* WARNING: Removing unreachable block (ram,0x00083c62) */
/* WARNING: Removing unreachable block (ram,0x00083c66) */
/* WARNING: Removing unreachable block (ram,0x00083c76) */
/* WARNING: Removing unreachable block (ram,0x00083c7a) */
/* WARNING: Removing unreachable block (ram,0x00083c7e) */
/* WARNING: Removing unreachable block (ram,0x00083c90) */
/* WARNING: Removing unreachable block (ram,0x00083ca6) */
/* WARNING: Removing unreachable block (ram,0x00083cba) */
/* WARNING: Removing unreachable block (ram,0x00083cbc) */
/* WARNING: Removing unreachable block (ram,0x00083ca2) */
/* WARNING: Removing unreachable block (ram,0x000ff29e) */
/* WARNING: Removing unreachable block (ram,0x000bf286) */
/* WARNING: Removing unreachable block (ram,0x000bf28a) */
/* WARNING: Removing unreachable block (ram,0x00083bfe) */
/* WARNING: Removing unreachable block (ram,0x00083c02) */
/* WARNING: Removing unreachable block (ram,0x000bf20a) */
/* WARNING: Removing unreachable block (ram,0x00083c06) */
/* WARNING: Removing unreachable block (ram,0x000ff20e) */
/* WARNING: Removing unreachable block (ram,0x0017f60e) */
/* WARNING: Removing unreachable block (ram,0x000ff23e) */
/* WARNING: Removing unreachable block (ram,0x0013f23e) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&, float) */

void AbyssEngine::AEMath::operator/(Vector *param_1,float param_2)

{
  undefined4 in_r2;
  int unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_lr;
  bool in_ZR;
  bool in_CY;
  bool in_OV;
  undefined4 in_cr15;
  undefined4 in_s4;
  undefined4 in_s5;
  undefined8 unaff_d14;
  
  if (in_ZR) {
    coprocessor_loadlong(7,in_cr15,unaff_r8);
    if (in_OV) {
      *(undefined4 *)(unaff_r8 + -700) = unaff_lr;
      *(undefined4 *)(unaff_r8 + -0x2c0) = unaff_r10;
      *(undefined4 *)(unaff_r8 + -0x2c4) = unaff_r9;
      *(undefined4 *)(unaff_r8 + -0x2c8) = in_r2;
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (!in_CY) {
    VectorHalvingAdd(CONCAT44(in_s5,in_s4),unaff_d14,1,0);
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
