# AbyssEngine::AEMath::MatrixSetTranslation
elf 0x80e60 body 24
Sig: undefined __stdcall MatrixSetTranslation(Matrix * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::MatrixSetTranslation(AbyssEngine::AEMath::Matrix&,
   AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::MatrixSetTranslation(Matrix *param_1,Vector *param_2)

{
  undefined4 *in_r2;
  
  *(undefined4 *)(param_2 + 0xc) = *in_r2;
  *(undefined4 *)(param_2 + 0x1c) = in_r2[1];
  *(undefined4 *)(param_2 + 0x2c) = in_r2[2];
  __aeabi_memcpy(param_1,param_2,0x3c);
  return;
}

```
## target disasm
```
  00090e60: push {r7,lr}
  00090e62: mov r7,sp
  00090e64: ldr r3,[r2,#0x0]
  00090e66: str r3,[r1,#0xc]
  00090e68: ldr r3,[r2,#0x4]
  00090e6a: str r3,[r1,#0x1c]
  00090e6c: ldr r2,[r2,#0x8]
  00090e6e: str r2,[r1,#0x2c]
  00090e70: movs r2,#0x3c
  00090e72: blx 0x0006f1e4
  00090e76: pop {r7,pc}
```
