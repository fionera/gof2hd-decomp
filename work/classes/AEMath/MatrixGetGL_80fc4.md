# AbyssEngine::AEMath::MatrixGetGL
elf 0x80fc4 body 64
Sig: undefined __stdcall MatrixGetGL(Matrix * param_1, float * param_2)

## decompile
```c

/* AbyssEngine::AEMath::MatrixGetGL(AbyssEngine::AEMath::Matrix const&, float*) */

void AbyssEngine::AEMath::MatrixGetGL(Matrix *param_1,float *param_2)

{
  float fVar1;
  
  *param_2 = *(float *)param_1;
  param_2[4] = *(float *)(param_1 + 4);
  param_2[8] = *(float *)(param_1 + 8);
  param_2[0xc] = *(float *)(param_1 + 0xc);
  param_2[1] = *(float *)(param_1 + 0x10);
  param_2[5] = *(float *)(param_1 + 0x14);
  param_2[9] = *(float *)(param_1 + 0x18);
  param_2[0xd] = *(float *)(param_1 + 0x1c);
  param_2[2] = *(float *)(param_1 + 0x20);
  param_2[6] = *(float *)(param_1 + 0x24);
  param_2[10] = *(float *)(param_1 + 0x28);
  fVar1 = *(float *)(param_1 + 0x2c);
  param_2[3] = 0.0;
  param_2[0xe] = fVar1;
  param_2[7] = 0.0;
  param_2[0xb] = 0.0;
  param_2[0xf] = 1.0;
  return;
}

```
## target disasm
```
  00090fc4: ldr r2,[r0,#0x0]
  00090fc6: str r2,[r1,#0x0]
  00090fc8: ldr r2,[r0,#0x4]
  00090fca: str r2,[r1,#0x10]
  00090fcc: ldr r2,[r0,#0x8]
  00090fce: str r2,[r1,#0x20]
  00090fd0: ldr r2,[r0,#0xc]
  00090fd2: str r2,[r1,#0x30]
  00090fd4: ldr r2,[r0,#0x10]
  00090fd6: str r2,[r1,#0x4]
  00090fd8: ldr r2,[r0,#0x14]
  00090fda: str r2,[r1,#0x14]
  00090fdc: ldr r2,[r0,#0x18]
  00090fde: str r2,[r1,#0x24]
  00090fe0: ldr r2,[r0,#0x1c]
  00090fe2: str r2,[r1,#0x34]
  00090fe4: ldr r2,[r0,#0x20]
  00090fe6: str r2,[r1,#0x8]
  00090fe8: ldr r2,[r0,#0x24]
  00090fea: str r2,[r1,#0x18]
  00090fec: ldr r2,[r0,#0x28]
  00090fee: str r2,[r1,#0x28]
  00090ff0: movs r2,#0x0
  00090ff2: ldr r0,[r0,#0x2c]
  00090ff4: str r2,[r1,#0xc]
  00090ff6: str r0,[r1,#0x38]
  00090ff8: mov.w r0,#0x3f800000
  00090ffc: str r2,[r1,#0x1c]
  00090ffe: str r2,[r1,#0x2c]
  00091000: str r0,[r1,#0x3c]
  00091002: bx lr
```
