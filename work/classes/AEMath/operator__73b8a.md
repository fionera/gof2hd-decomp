# AbyssEngine::AEMath::operator*
elf 0x73b8a body 1
Sig: undefined __stdcall operator*(float param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0017f422) */
/* WARNING: Removing unreachable block (ram,0x000ff1f6) */
/* WARNING: Removing unreachable block (ram,0x00083b92) */
/* WARNING: Removing unreachable block (ram,0x000ff1e2) */
/* WARNING: Removing unreachable block (ram,0x000ff1e6) */
/* WARNING: Removing unreachable block (ram,0x00a3d522) */
/* WARNING: Removing unreachable block (ram,0x000ff1ea) */
/* WARNING: Removing unreachable block (ram,0x000ff1ee) */
/* WARNING: Removing unreachable block (ram,0x000ff1f2) */
/* WARNING: Removing unreachable block (ram,0x000ff1fa) */
/* WARNING: Removing unreachable block (ram,0x000ff1fe) */
/* WARNING: Removing unreachable block (ram,0x0013f1c2) */
/* WARNING: Removing unreachable block (ram,0x0013f1c6) */
/* WARNING: Removing unreachable block (ram,0x0013f1ca) */
/* WARNING: Removing unreachable block (ram,0x0013f1b6) */
/* WARNING: Removing unreachable block (ram,0x018cda06) */
/* WARNING: Removing unreachable block (ram,0x004bf3fe) */
/* WARNING: Removing unreachable block (ram,0x0017f5fe) */
/* WARNING: Removing unreachable block (ram,0x00083bae) */
/* WARNING: Removing unreachable block (ram,0x00083bb2) */
/* WARNING: Removing unreachable block (ram,0x00083be2) */
/* WARNING: Removing unreachable block (ram,0x00083be6) */
/* WARNING: Removing unreachable block (ram,0x000bf232) */
/* WARNING: Removing unreachable block (ram,0x000bf236) */
/* WARNING: Removing unreachable block (ram,0x000bf23a) */
/* WARNING: Removing unreachable block (ram,0x00083bea) */
/* WARNING: Removing unreachable block (ram,0x00083bee) */
/* WARNING: Removing unreachable block (ram,0x000ff23a) */
/* WARNING: Removing unreachable block (ram,0x000ff23e) */
/* WARNING: Removing unreachable block (ram,0x000ff242) */
/* WARNING: Removing unreachable block (ram,0x00083bf2) */
/* WARNING: Removing unreachable block (ram,0x00083bf6) */
/* WARNING: Removing unreachable block (ram,0x00083bfa) */
/* WARNING: Removing unreachable block (ram,0x0017f606) */
/* WARNING: Removing unreachable block (ram,0x0017f60e) */
/* WARNING: Removing unreachable block (ram,0x000bf286) */
/* WARNING: Removing unreachable block (ram,0x000bf28a) */
/* WARNING: Removing unreachable block (ram,0x00083bfe) */
/* WARNING: Removing unreachable block (ram,0x00083c02) */
/* WARNING: Removing unreachable block (ram,0x00083c06) */
/* WARNING: Removing unreachable block (ram,0x000ff20e) */
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
/* WARNING: Removing unreachable block (ram,0x0013f212) */
/* WARNING: Removing unreachable block (ram,0x000bf20a) */
/* WARNING: Removing unreachable block (ram,0x0013f23e) */
/* WARNING: Removing unreachable block (ram,0x000ff1c6) */
/* WARNING: Removing unreachable block (ram,0x000ff1ba) */
/* WARNING: Removing unreachable block (ram,0x00083b96) */
/* WARNING: Removing unreachable block (ram,0x0013f1e6) */
/* WARNING: Removing unreachable block (ram,0x00083b9a) */
/* WARNING: Removing unreachable block (ram,0x00083b9e) */
/* WARNING: Removing unreachable block (ram,0x00083ba2) */
/* WARNING: Removing unreachable block (ram,0x0017f432) */
/* WARNING: Removing unreachable block (ram,0x0017f436) */
/* WARNING: Removing unreachable block (ram,0x0017f43a) */
/* WARNING: Removing unreachable block (ram,0x0017f43e) */
/* WARNING: Removing unreachable block (ram,0x00083ba6) */
/* WARNING: Removing unreachable block (ram,0x00083baa) */
/* WARNING: Removing unreachable block (ram,0x000ff1b2) */
/* WARNING: Removing unreachable block (ram,0x0093d4ee) */
/* WARNING: Removing unreachable block (ram,0x000ff1b6) */
/* WARNING: Removing unreachable block (ram,0x000ff1be) */
/* WARNING: Removing unreachable block (ram,0x000ff1c2) */
/* WARNING: Removing unreachable block (ram,0x000ff1ca) */
/* WARNING: Removing unreachable block (ram,0x000ff1ce) */
/* WARNING: Removing unreachable block (ram,0x000ff1d2) */
/* WARNING: Removing unreachable block (ram,0x000ff1d6) */
/* WARNING: Removing unreachable block (ram,0x000ff1da) */
/* WARNING: Removing unreachable block (ram,0x000bf1ae) */
/* WARNING: Removing unreachable block (ram,0xfecbf1ba) */
/* WARNING: Removing unreachable block (ram,0x000bf1b2) */
/* WARNING: Removing unreachable block (ram,0x000bf1b6) */
/* WARNING: Removing unreachable block (ram,0x000bf1ba) */
/* WARNING: Removing unreachable block (ram,0x000bf1be) */
/* WARNING: Removing unreachable block (ram,0x000bf1c6) */
/* WARNING: Removing unreachable block (ram,0x000bf1ca) */
/* WARNING: Removing unreachable block (ram,0x000bf1ce) */
/* WARNING: Removing unreachable block (ram,0x0017f426) */
/* WARNING: Removing unreachable block (ram,0x0017f42a) */
/* WARNING: Removing unreachable block (ram,0x0017f42e) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(float, AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::operator*(float param_1,Vector *param_2)

{
  undefined4 *unaff_r8;
  undefined4 unaff_r11;
  undefined4 unaff_lr;
  bool in_ZR;
  bool in_OV;
  
  if (in_ZR) {
    if (in_OV) {
      *unaff_r8 = unaff_lr;
      unaff_r8[-1] = register0x00000054;
      unaff_r8[-2] = unaff_r11;
      unaff_r8[-3] = param_2;
    }
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
