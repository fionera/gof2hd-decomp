# AbyssEngine::AEMath::MatrixGetUp
elf 0x80712 body 14
Sig: undefined __stdcall MatrixGetUp(Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::MatrixGetUp(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixGetUp(Matrix *param_1)

{
  int in_r1;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(in_r1 + 0x14);
  uVar1 = *(undefined4 *)(in_r1 + 0x24);
  *(undefined4 *)param_1 = *(undefined4 *)(in_r1 + 4);
  *(undefined4 *)(param_1 + 4) = uVar2;
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

```
## target disasm
```
  00090712: ldr r3,[r1,#0x14]
  00090714: ldr r2,[r1,#0x4]
  00090716: ldr r1,[r1,#0x24]
  00090718: strd r2,r3,[r0,#0x0]
  0009071c: str r1,[r0,#0x8]
  0009071e: bx lr
```
