# AbyssEngine::AEMath::MatrixIsEqual
elf 0x80700 body 14
Sig: undefined __stdcall MatrixIsEqual(Matrix * param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::AEMath::MatrixIsEqual(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixIsEqual(Matrix *param_1,Matrix *param_2)

{
  (*(code *)(DAT_001ab4a4 + 0x1ab4a8))(param_1);
  return;
}

```
## target disasm
```
  00090700: b.w 0x001ab498
  001ab498: bx pc
  001ab49c: ldr r12,[0x1ab4a4]
  001ab4a0: add pc,r12,pc
```
