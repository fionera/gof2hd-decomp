# AbyssEngine::AEMath::operator<
elf 0x73cbe body 68
Sig: undefined __stdcall operator<(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

bool AbyssEngine::AEMath::operator<(Vector *param_1,Vector *param_2)

{
  if (-1 < (int)((uint)(*(float *)param_1 < *(float *)param_2) << 0x1f)) {
    return false;
  }
  if ((int)((uint)(*(float *)(param_1 + 4) < *(float *)(param_2 + 4)) << 0x1f) < 0) {
    return (int)((uint)(*(float *)(param_1 + 8) < *(float *)(param_2 + 8)) << 0x1f) < 0;
  }
  return false;
}

```
## target disasm
```
  00083cbe: vldr.32 s0,[r1]
  00083cc2: mov r2,r0
  00083cc4: vldr.32 s2,[r0]
  00083cc8: vcmpe.f32 s2,s0
  00083ccc: vmrs apsr,fpscr
  00083cd0: itt pl
  00083cd2: mov.pl r0,#0x0
  00083cd4: bx.pl lr
  00083cd6: vldr.32 s0,[r1,#0x4]
  00083cda: movs r0,#0x0
  00083cdc: vldr.32 s2,[r2,#0x4]
  00083ce0: vcmpe.f32 s2,s0
  00083ce4: vmrs apsr,fpscr
  00083ce8: it pl
  00083cea: bx.pl lr
  00083cec: vldr.32 s0,[r1,#0x8]
  00083cf0: vldr.32 s2,[r2,#0x8]
  00083cf4: vcmpe.f32 s2,s0
  00083cf8: vmrs apsr,fpscr
  00083cfc: it mi
  00083cfe: mov.mi r0,#0x1
  00083d00: bx lr
```
