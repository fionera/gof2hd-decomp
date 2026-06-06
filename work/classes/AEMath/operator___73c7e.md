# AbyssEngine::AEMath::operator!=
elf 0x73c7e body 64
Sig: undefined __stdcall operator!=(Vector * param_1, Vector * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Vector const&) */

bool AbyssEngine::AEMath::operator!=(Vector *param_1,Vector *param_2)

{
  if ((*(float *)param_1 == *(float *)param_2) &&
     (*(float *)(param_1 + 4) == *(float *)(param_2 + 4))) {
    return *(float *)(param_1 + 8) != *(float *)(param_2 + 8);
  }
  return true;
}

```
## target disasm
```
  00083c7e: vldr.32 s0,[r1]
  00083c82: vldr.32 s2,[r0]
  00083c86: vcmpe.f32 s2,s0
  00083c8a: vmrs apsr,fpscr
  00083c8e: bne 0x00083ca2
  00083c90: vldr.32 s0,[r1,#0x4]
  00083c94: vldr.32 s2,[r0,#0x4]
  00083c98: vcmpe.f32 s2,s0
  00083c9c: vmrs apsr,fpscr
  00083ca0: beq 0x00083ca6
  00083ca2: movs r0,#0x1
  00083ca4: bx lr
  00083ca6: vldr.32 s2,[r0,#0x8]
  00083caa: movs r0,#0x0
  00083cac: vldr.32 s0,[r1,#0x8]
  00083cb0: vcmpe.f32 s2,s0
  00083cb4: vmrs apsr,fpscr
  00083cb8: it ne
  00083cba: mov.ne r0,#0x1
  00083cbc: bx lr
```
