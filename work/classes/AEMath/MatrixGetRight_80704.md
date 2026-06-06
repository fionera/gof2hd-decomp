# AbyssEngine::AEMath::MatrixGetRight
elf 0x80704 body 14
Sig: undefined __stdcall MatrixGetRight(Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::MatrixGetRight(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixGetRight(Matrix *param_1)

{
  undefined4 *in_r1;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = in_r1[4];
  uVar1 = in_r1[8];
  *(undefined4 *)param_1 = *in_r1;
  *(undefined4 *)(param_1 + 4) = uVar2;
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

```
## target disasm
```
  00090704: ldr r3,[r1,#0x10]
  00090706: ldr r2,[r1,#0x0]
  00090708: ldr r1,[r1,#0x20]
  0009070a: strd r2,r3,[r0,#0x0]
  0009070e: str r1,[r0,#0x8]
  00090710: bx lr
```
