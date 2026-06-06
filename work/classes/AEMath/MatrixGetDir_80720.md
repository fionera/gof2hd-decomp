# AbyssEngine::AEMath::MatrixGetDir
elf 0x80720 body 14
Sig: undefined __stdcall MatrixGetDir(Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::MatrixGetDir(AbyssEngine::AEMath::Matrix const&) */

void AbyssEngine::AEMath::MatrixGetDir(Matrix *param_1)

{
  int in_r1;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(in_r1 + 0x18);
  uVar1 = *(undefined4 *)(in_r1 + 0x28);
  *(undefined4 *)param_1 = *(undefined4 *)(in_r1 + 8);
  *(undefined4 *)(param_1 + 4) = uVar2;
  *(undefined4 *)(param_1 + 8) = uVar1;
  return;
}

```
## target disasm
```
  00090720: ldr r3,[r1,#0x18]
  00090722: ldr r2,[r1,#0x8]
  00090724: ldr r1,[r1,#0x28]
  00090726: strd r2,r3,[r0,#0x0]
  0009072a: str r1,[r0,#0x8]
  0009072c: bx lr
```
