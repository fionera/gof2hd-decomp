# PlayerEgo::getCloakRechargeRate
elf 0x9b06a body 34
Sig: undefined __thiscall getCloakRechargeRate(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getCloakRechargeRate() */

float __thiscall PlayerEgo::getCloakRechargeRate(PlayerEgo *this)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar2 = (float)VectorSignedToFloat(this[0x18].field_8,(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorSignedToFloat(this[0xe].field_14,(byte)(in_fpscr >> 0x16) & 3);
  return 1.0 - fVar1 / fVar2;
}

```

## target disasm
```
  000ab06a: vldr.32 s0,[r0,#0x20c]
  000ab06e: vldr.32 s2,[r0,#0x368]
  000ab072: vcvt.f32.s32 s2,s2
  000ab076: vcvt.f32.s32 s0,s0
  000ab07a: vdiv.f32 s0,s0,s2
  000ab07e: vmov.f32 s2,0x3f800000
  000ab082: vsub.f32 s0,s2,s0
  000ab086: vmov r0,s0
  000ab08a: bx lr
```
