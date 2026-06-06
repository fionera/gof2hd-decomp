# AbyssEngine::AEMath::operator>=
elf 0x73d8a body 68
Sig: undefined __stdcall operator>=(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

bool AbyssEngine::AEMath::operator>=(Vector *param_1,Vector *param_2)

{
  if (*(float *)param_1 < *(float *)param_2) {
    return false;
  }
  if (*(float *)(param_2 + 4) <= *(float *)(param_1 + 4)) {
    return *(float *)(param_2 + 8) <= *(float *)(param_1 + 8);
  }
  return false;
}

```
## target disasm
```
  00083d8a: vldr.32 s0,[r1]
  00083d8e: mov r2,r0
  00083d90: vldr.32 s2,[r0]
  00083d94: vcmpe.f32 s2,s0
  00083d98: vmrs apsr,fpscr
  00083d9c: itt lt
  00083d9e: mov.lt r0,#0x0
  00083da0: bx.lt lr
  00083da2: vldr.32 s0,[r1,#0x4]
  00083da6: movs r0,#0x0
  00083da8: vldr.32 s2,[r2,#0x4]
  00083dac: vcmpe.f32 s2,s0
  00083db0: vmrs apsr,fpscr
  00083db4: it lt
  00083db6: bx.lt lr
  00083db8: vldr.32 s0,[r1,#0x8]
  00083dbc: vldr.32 s2,[r2,#0x8]
  00083dc0: vcmpe.f32 s2,s0
  00083dc4: vmrs apsr,fpscr
  00083dc8: it ge
  00083dca: mov.ge r0,#0x1
  00083dcc: bx lr
```
