# AbyssEngine::AEMath::operator!=
elf 0x8052c body 226
Sig: undefined __stdcall operator!=(Matrix * param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Matrix const&) */

bool AbyssEngine::AEMath::operator!=(Matrix *param_1,Matrix *param_2)

{
  if ((((((*(float *)param_1 == *(float *)param_2) &&
         (*(float *)(param_1 + 4) == *(float *)(param_2 + 4))) &&
        (*(float *)(param_1 + 8) == *(float *)(param_2 + 8))) &&
       ((*(float *)(param_1 + 0xc) == *(float *)(param_2 + 0xc) &&
        (*(float *)(param_1 + 0x10) == *(float *)(param_2 + 0x10))))) &&
      ((*(float *)(param_1 + 0x14) == *(float *)(param_2 + 0x14) &&
       ((*(float *)(param_1 + 0x18) == *(float *)(param_2 + 0x18) &&
        (*(float *)(param_1 + 0x1c) == *(float *)(param_2 + 0x1c))))))) &&
     ((*(float *)(param_1 + 0x20) == *(float *)(param_2 + 0x20) &&
      ((*(float *)(param_1 + 0x24) == *(float *)(param_2 + 0x24) &&
       (*(float *)(param_1 + 0x28) == *(float *)(param_2 + 0x28))))))) {
    return *(float *)(param_1 + 0x2c) != *(float *)(param_2 + 0x2c);
  }
  return true;
}

```
## target disasm
```
  0009052c: vldr.32 s0,[r1]
  00090530: vldr.32 s2,[r0]
  00090534: vcmpe.f32 s2,s0
  00090538: vmrs apsr,fpscr
  0009053c: bne 0x000905f2
  0009053e: vldr.32 s0,[r1,#0x4]
  00090542: vldr.32 s2,[r0,#0x4]
  00090546: vcmpe.f32 s2,s0
  0009054a: vmrs apsr,fpscr
  0009054e: bne 0x000905f2
  00090550: vldr.32 s0,[r1,#0x8]
  00090554: vldr.32 s2,[r0,#0x8]
  00090558: vcmpe.f32 s2,s0
  0009055c: vmrs apsr,fpscr
  00090560: bne 0x000905f2
  00090562: vldr.32 s0,[r1,#0xc]
  00090566: vldr.32 s2,[r0,#0xc]
  0009056a: vcmpe.f32 s2,s0
  0009056e: vmrs apsr,fpscr
  00090572: bne 0x000905f2
  00090574: vldr.32 s0,[r1,#0x10]
  00090578: vldr.32 s2,[r0,#0x10]
  0009057c: vcmpe.f32 s2,s0
  00090580: vmrs apsr,fpscr
  00090584: bne 0x000905f2
  00090586: vldr.32 s0,[r1,#0x14]
  0009058a: vldr.32 s2,[r0,#0x14]
  0009058e: vcmpe.f32 s2,s0
  00090592: vmrs apsr,fpscr
  00090596: bne 0x000905f2
  00090598: vldr.32 s0,[r1,#0x18]
  0009059c: vldr.32 s2,[r0,#0x18]
  000905a0: vcmpe.f32 s2,s0
  000905a4: vmrs apsr,fpscr
  000905a8: bne 0x000905f2
  000905aa: vldr.32 s0,[r1,#0x1c]
  000905ae: vldr.32 s2,[r0,#0x1c]
  000905b2: vcmpe.f32 s2,s0
  000905b6: vmrs apsr,fpscr
  000905ba: bne 0x000905f2
  000905bc: vldr.32 s0,[r1,#0x20]
  000905c0: vldr.32 s2,[r0,#0x20]
  000905c4: vcmpe.f32 s2,s0
  000905c8: vmrs apsr,fpscr
  000905cc: bne 0x000905f2
  000905ce: vldr.32 s0,[r1,#0x24]
  000905d2: vldr.32 s2,[r0,#0x24]
  000905d6: vcmpe.f32 s2,s0
  000905da: vmrs apsr,fpscr
  000905de: bne 0x000905f2
  000905e0: vldr.32 s0,[r1,#0x28]
  000905e4: vldr.32 s2,[r0,#0x28]
  000905e8: vcmpe.f32 s2,s0
  000905ec: vmrs apsr,fpscr
  000905f0: beq 0x000905f6
  000905f2: movs r0,#0x1
  000905f4: bx lr
  000905f6: vldr.32 s2,[r0,#0x2c]
  000905fa: movs r0,#0x0
  000905fc: vldr.32 s0,[r1,#0x2c]
  00090600: vcmpe.f32 s2,s0
  00090604: vmrs apsr,fpscr
  00090608: it ne
  0009060a: mov.ne r0,#0x1
  0009060c: bx lr
```
