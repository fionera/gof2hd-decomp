# AbyssEngine::AEMath::MatrixMultiply
elf 0x80688 body 10
Sig: undefined __stdcall MatrixMultiply(Matrix * param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::AEMath::MatrixMultiply(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixMultiply(Matrix *param_1,Matrix *param_2)

{
  operator*(param_1,param_2);
  return;
}

```
## target disasm
```
  00090688: push {r7,lr}
  0009068a: mov r7,sp
  0009068c: blx 0x0006f82c
  00090690: pop {r7,pc}
```
