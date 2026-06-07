# PlayerEgo::getCloakRate
elf 0x9afd0 body 62
Sig: undefined __thiscall getCloakRate(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getCloakRate() */

float __thiscall PlayerEgo::getCloakRate(PlayerEgo *this)

{
  int iVar1;
  uint in_fpscr;
  float fVar2;
  float fVar3;
  float fVar4;
  
  iVar1 = 0x210;
  if ((char)this[0xb].field_20 == '\0') {
    iVar1 = 0x214;
  }
  fVar2 = (float)VectorSignedToFloat(this[0xe].field_10,(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = (float)VectorSignedToFloat(*(undefined4 *)((int)&this->m_pPlayer + iVar1),
                                     (byte)(in_fpscr >> 0x16) & 3);
  fVar4 = 1.0;
  if (0.0 <= fVar2 / fVar3) {
    fVar4 = fVar2 / fVar3;
  }
  return fVar4;
}

```

## target disasm
```
  000aafd0: ldrb.w r1,[r0,#0x1ac]
  000aafd4: mov.w r2,#0x210
  000aafd8: vldr.32 s0,[r0,#0x208]
  000aafdc: cmp r1,#0x0
  000aafde: it eq
  000aafe0: mov.eq.w r2,#0x214
  000aafe4: add r0,r2
  000aafe6: vldr.32 s2,[r0]
  000aafea: vcvt.f32.s32 s0,s0
  000aafee: vcvt.f32.s32 s2,s2
  000aaff2: vdiv.f32 s0,s0,s2
  000aaff6: vcmpe.f32 s0,#0
  000aaffa: vmrs apsr,fpscr
  000aaffe: vmov.f32 s2,0x3f800000
  000ab002: it ge
  000ab004: vmov.ge.f32 s2,s0
  000ab008: vmov r0,s2
  000ab00c: bx lr
```
