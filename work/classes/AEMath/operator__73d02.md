# AbyssEngine::AEMath::operator>
elf 0x73d02 body 68
Sig: undefined __stdcall operator>(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

bool AbyssEngine::AEMath::operator>(Vector *param_1,Vector *param_2)

{
  if (*(float *)param_1 <= *(float *)param_2) {
    return false;
  }
  if (*(float *)(param_2 + 4) < *(float *)(param_1 + 4)) {
    return *(float *)(param_2 + 8) < *(float *)(param_1 + 8);
  }
  return false;
}

```
## target disasm
```
  00083d02: vldr.32 s0,[r1]
  00083d06: mov r2,r0
  00083d08: vldr.32 s2,[r0]
  00083d0c: vcmpe.f32 s2,s0
  00083d10: vmrs apsr,fpscr
  00083d14: itt le
  00083d16: mov.le r0,#0x0
  00083d18: bx.le lr
  00083d1a: vldr.32 s0,[r1,#0x4]
  00083d1e: movs r0,#0x0
  00083d20: vldr.32 s2,[r2,#0x4]
  00083d24: vcmpe.f32 s2,s0
  00083d28: vmrs apsr,fpscr
  00083d2c: it le
  00083d2e: bx.le lr
  00083d30: vldr.32 s0,[r1,#0x8]
  00083d34: vldr.32 s2,[r2,#0x8]
  00083d38: vcmpe.f32 s2,s0
  00083d3c: vmrs apsr,fpscr
  00083d40: it gt
  00083d42: mov.gt r0,#0x1
  00083d44: bx lr
```
