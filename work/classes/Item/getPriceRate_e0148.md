# Item::getPriceRate
elf 0xe0148 body 36
Sig: undefined __thiscall getPriceRate(Item * this)

## decompile
```c

/* Item::getPriceRate() */

float __thiscall Item::getPriceRate(Item *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = (float)VectorSignedToFloat(*(int *)(this + 0x24) - *(int *)(this + 0x20),
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat(*(int *)(this + 0x18) - *(int *)(this + 0x20),
                                     (byte)(in_fpscr >> 0x16) & 3);
  return fVar2 / fVar1;
}

```
## target disasm
```
  000f0148: ldr r1,[r0,#0x18]
  000f014a: ldrd r2,r0,[r0,#0x20]
  000f014e: subs r0,r0,r2
  000f0150: vmov s0,r0
  000f0154: subs r0,r1,r2
  000f0156: vmov s2,r0
  000f015a: vcvt.f32.s32 s0,s0
  000f015e: vcvt.f32.s32 s2,s2
  000f0162: vdiv.f32 s0,s2,s0
  000f0166: vmov r0,s0
  000f016a: bx lr
```
