# AbyssEngine::AEMath::operator<=
elf 0x73d46 body 68
Sig: undefined __stdcall operator<=(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

bool AbyssEngine::AEMath::operator<=(Vector *param_1,Vector *param_2)

{
  if (*(float *)param_2 < *(float *)param_1) {
    return false;
  }
  if (*(float *)(param_1 + 4) <= *(float *)(param_2 + 4)) {
    return *(float *)(param_1 + 8) <= *(float *)(param_2 + 8);
  }
  return false;
}

```
## target disasm
```
  00083d46: vldr.32 s0,[r1]
  00083d4a: mov r2,r0
  00083d4c: vldr.32 s2,[r0]
  00083d50: vcmpe.f32 s2,s0
  00083d54: vmrs apsr,fpscr
  00083d58: itt hi
  00083d5a: mov.hi r0,#0x0
  00083d5c: bx.hi lr
  00083d5e: vldr.32 s0,[r1,#0x4]
  00083d62: movs r0,#0x0
  00083d64: vldr.32 s2,[r2,#0x4]
  00083d68: vcmpe.f32 s2,s0
  00083d6c: vmrs apsr,fpscr
  00083d70: it hi
  00083d72: bx.hi lr
  00083d74: vldr.32 s0,[r1,#0x8]
  00083d78: vldr.32 s2,[r2,#0x8]
  00083d7c: vcmpe.f32 s2,s0
  00083d80: vmrs apsr,fpscr
  00083d84: it ls
  00083d86: mov.ls r0,#0x1
  00083d88: bx lr
```
