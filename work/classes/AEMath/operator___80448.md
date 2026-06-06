# AbyssEngine::AEMath::operator==
elf 0x80448 body 228
Sig: undefined __stdcall operator==(Matrix * param_1, Matrix * param_2)

## decompile
```c

/* AbyssEngine::AEMath::TEMPNAMEPLACEHOLDERVALUE(AbyssEngine::AEMath::Matrix const&,
   AbyssEngine::AEMath::Matrix const&) */

bool AbyssEngine::AEMath::operator==(Matrix *param_1,Matrix *param_2)

{
  bool bVar1;
  
  if ((((((*(float *)param_1 == *(float *)param_2) &&
         (*(float *)(param_1 + 4) == *(float *)(param_2 + 4))) &&
        (*(float *)(param_1 + 8) == *(float *)(param_2 + 8))) &&
       ((*(float *)(param_1 + 0xc) == *(float *)(param_2 + 0xc) &&
        (*(float *)(param_1 + 0x10) == *(float *)(param_2 + 0x10))))) &&
      ((*(float *)(param_1 + 0x14) == *(float *)(param_2 + 0x14) &&
       ((*(float *)(param_1 + 0x18) == *(float *)(param_2 + 0x18) &&
        (*(float *)(param_1 + 0x1c) == *(float *)(param_2 + 0x1c))))))) &&
     ((*(float *)(param_1 + 0x20) == *(float *)(param_2 + 0x20) &&
      (*(float *)(param_1 + 0x24) == *(float *)(param_2 + 0x24))))) {
    bVar1 = false;
    if (*(float *)(param_1 + 0x28) == *(float *)(param_2 + 0x28)) {
      bVar1 = *(float *)(param_1 + 0x2c) == *(float *)(param_2 + 0x2c);
    }
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}

```
## target disasm
```
  00090448: vldr.32 s0,[r1]
  0009044c: vldr.32 s2,[r0]
  00090450: vcmpe.f32 s2,s0
  00090454: vmrs apsr,fpscr
  00090458: bne 0x000904fc
  0009045a: vldr.32 s0,[r1,#0x4]
  0009045e: vldr.32 s2,[r0,#0x4]
  00090462: vcmpe.f32 s2,s0
  00090466: vmrs apsr,fpscr
  0009046a: bne 0x000904fc
  0009046c: vldr.32 s0,[r1,#0x8]
  00090470: vldr.32 s2,[r0,#0x8]
  00090474: vcmpe.f32 s2,s0
  00090478: vmrs apsr,fpscr
  0009047c: bne 0x000904fc
  0009047e: vldr.32 s0,[r1,#0xc]
  00090482: vldr.32 s2,[r0,#0xc]
  00090486: vcmpe.f32 s2,s0
  0009048a: vmrs apsr,fpscr
  0009048e: bne 0x000904fc
  00090490: vldr.32 s0,[r1,#0x10]
  00090494: vldr.32 s2,[r0,#0x10]
  00090498: vcmpe.f32 s2,s0
  0009049c: vmrs apsr,fpscr
  000904a0: bne 0x000904fc
  000904a2: vldr.32 s0,[r1,#0x14]
  000904a6: vldr.32 s2,[r0,#0x14]
  000904aa: vcmpe.f32 s2,s0
  000904ae: vmrs apsr,fpscr
  000904b2: bne 0x000904fc
  000904b4: vldr.32 s0,[r1,#0x18]
  000904b8: vldr.32 s2,[r0,#0x18]
  000904bc: vcmpe.f32 s2,s0
  000904c0: vmrs apsr,fpscr
  000904c4: bne 0x000904fc
  000904c6: vldr.32 s0,[r1,#0x1c]
  000904ca: vldr.32 s2,[r0,#0x1c]
  000904ce: vcmpe.f32 s2,s0
  000904d2: vmrs apsr,fpscr
  000904d6: bne 0x000904fc
  000904d8: vldr.32 s0,[r1,#0x20]
  000904dc: vldr.32 s2,[r0,#0x20]
  000904e0: vcmpe.f32 s2,s0
  000904e4: vmrs apsr,fpscr
  000904e8: bne 0x000904fc
  000904ea: vldr.32 s0,[r1,#0x24]
  000904ee: vldr.32 s2,[r0,#0x24]
  000904f2: vcmpe.f32 s2,s0
  000904f6: vmrs apsr,fpscr
  000904fa: beq 0x00090502
  000904fc: movs r2,#0x0
  000904fe: mov r0,r2
  00090500: bx lr
  00090502: vldr.32 s0,[r1,#0x28]
  00090506: movs r2,#0x0
  00090508: vldr.32 s2,[r0,#0x28]
  0009050c: vcmpe.f32 s2,s0
  00090510: vmrs apsr,fpscr
  00090514: bne 0x000904fe
  00090516: vldr.32 s0,[r1,#0x2c]
  0009051a: vldr.32 s2,[r0,#0x2c]
  0009051e: vcmpe.f32 s2,s0
  00090522: vmrs apsr,fpscr
  00090526: it eq
  00090528: mov.eq r2,#0x1
  0009052a: b 0x000904fe
```
