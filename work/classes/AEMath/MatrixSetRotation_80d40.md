# AbyssEngine::AEMath::MatrixSetRotation
elf 0x80d40 body 66
Sig: undefined __stdcall MatrixSetRotation(Matrix * param_1, Vector * param_2, Vector * param_3, Vector * param_4)

## decompile
```c

/* AbyssEngine::AEMath::MatrixSetRotation(AbyssEngine::AEMath::Matrix&, AbyssEngine::AEMath::Vector
   const&, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector const&) */

void AbyssEngine::AEMath::MatrixSetRotation
               (Matrix *param_1,Vector *param_2,Vector *param_3,Vector *param_4)

{
  undefined4 *in_stack_00000000;
  
  *(undefined4 *)param_2 = *(undefined4 *)param_3;
  *(undefined4 *)(param_2 + 0x10) = *(undefined4 *)(param_3 + 4);
  *(undefined4 *)(param_2 + 0x20) = *(undefined4 *)(param_3 + 8);
  *(undefined4 *)(param_2 + 4) = *(undefined4 *)param_4;
  *(undefined4 *)(param_2 + 0x14) = *(undefined4 *)(param_4 + 4);
  *(undefined4 *)(param_2 + 0x24) = *(undefined4 *)(param_4 + 8);
  *(undefined4 *)(param_2 + 8) = *in_stack_00000000;
  *(undefined4 *)(param_2 + 0x18) = in_stack_00000000[1];
  *(undefined4 *)(param_2 + 0x28) = in_stack_00000000[2];
  __aeabi_memcpy(param_1,param_2,0x3c);
  return;
}

```
## target disasm
```
  00090d40: push {r7,lr}
  00090d42: mov r7,sp
  00090d44: ldr.w r12,[r2,#0x0]
  00090d48: str.w r12,[r1,#0x0]
  00090d4c: ldr.w r12,[r2,#0x4]
  00090d50: str.w r12,[r1,#0x10]
  00090d54: ldr r2,[r2,#0x8]
  00090d56: str r2,[r1,#0x20]
  00090d58: ldr r2,[r3,#0x0]
  00090d5a: str r2,[r1,#0x4]
  00090d5c: ldr r2,[r3,#0x4]
  00090d5e: str r2,[r1,#0x14]
  00090d60: ldr.w r12,[r7,#0x8]
  00090d64: ldr r2,[r3,#0x8]
  00090d66: str r2,[r1,#0x24]
  00090d68: ldr.w r2,[r12,#0x0]
  00090d6c: str r2,[r1,#0x8]
  00090d6e: ldr.w r2,[r12,#0x4]
  00090d72: str r2,[r1,#0x18]
  00090d74: ldr.w r2,[r12,#0x8]
  00090d78: str r2,[r1,#0x28]
  00090d7a: movs r2,#0x3c
  00090d7c: blx 0x0006f1e4
  00090d80: pop {r7,pc}
```
