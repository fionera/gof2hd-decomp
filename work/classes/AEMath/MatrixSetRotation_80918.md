# AbyssEngine::AEMath::MatrixSetRotation
elf 0x80918 body 1
Sig: undefined __stdcall MatrixSetRotation(Matrix * param_1, float param_2, float param_3, float param_4)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, float, float, float) */

void AbyssEngine::AEMath::MatrixSetRotation
               (Matrix *param_1,float param_2,float param_3,float param_4)

{
  char in_NG;
  undefined1 in_ZR;
  bool in_OV;
  
  if ((bool)in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  if ((bool)in_ZR) {
    software_interrupt(0x80e92d);
  }
  if (!in_OV) {
    func_0x001cbddc();
  }
  if ((bool)in_ZR) {
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
