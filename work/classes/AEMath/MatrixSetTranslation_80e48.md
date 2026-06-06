# AbyssEngine::AEMath::MatrixSetTranslation
elf 0x80e48 body 24
Sig: undefined __stdcall MatrixSetTranslation(Matrix * param_1, float param_2, float param_3, float param_4)

## decompile
```c

/* AbyssEngine::AEMath::MatrixSetTranslation(AbyssEngine::AEMath::Matrix&, float, float, float) */

void AbyssEngine::AEMath::MatrixSetTranslation
               (Matrix *param_1,float param_2,float param_3,float param_4)

{
  int in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  undefined4 in_stack_00000000;
  
  *(undefined4 *)(in_r1 + 0x1c) = in_r3;
  *(undefined4 *)(in_r1 + 0xc) = in_r2;
  *(undefined4 *)(in_r1 + 0x2c) = in_stack_00000000;
  __aeabi_memcpy(param_1,in_r1,0x3c);
  return;
}

```
## target disasm
```
  00090e48: push {r7,lr}
  00090e4a: mov r7,sp
  00090e4c: vldr.32 s0,[r7,#0x8]
  00090e50: str r3,[r1,#0x1c]
  00090e52: str r2,[r1,#0xc]
  00090e54: movs r2,#0x3c
  00090e56: vstr.32 s0,[r1,#0x2c]
  00090e5a: blx 0x0006f1e4
  00090e5e: pop {r7,pc}
```
