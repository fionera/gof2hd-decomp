# AbyssEngine::AEMath::operator==
elf 0x73c3a body 1
Sig: undefined __stdcall operator==(Vector * param_1, Vector * param_2)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x000ff29e) */
/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

bool AbyssEngine::AEMath::operator==(Vector *param_1,Vector *param_2)

{
  uint in_r2;
  undefined4 unaff_r4;
  int in_r12;
  bool in_NG;
  bool in_ZR;
  char in_OV;
  undefined4 in_cr2;
  undefined4 in_cr4;
  undefined4 in_cr15;
  
  if (in_ZR) {
    if (in_NG) {
      *(undefined4 *)param_2 = unaff_r4;
    }
    coprocessor_store(7,in_cr15,in_r12 + 0x2d0);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  coprocessor_load(6,in_cr4,param_1 + 8);
  if (in_NG != (bool)in_OV) {
    software_interrupt(0x1cfa10);
  }
  if (in_NG) {
    param_1 = (Vector *)0x0;
    in_r2 = (uint)bRam00000000;
  }
  coprocessor_load(0,in_cr2,in_r2);
  if (in_NG != (bool)in_OV) {
    software_interrupt(0x18fa10);
  }
  coprocessor_load(7,in_cr4,param_2 + 0x1c0);
  if (in_NG != (bool)in_OV) {
    software_interrupt(0x8fa10);
  }
  if (in_NG) {
    param_1 = param_1 + -(int)param_2;
  }
  if ((*(float *)param_1 == *(float *)param_2) &&
     (*(float *)(param_1 + 4) == *(float *)(param_2 + 4))) {
    return *(float *)(param_1 + 8) != *(float *)(param_2 + 8);
  }
  return true;
}

```
## target disasm
```
```
