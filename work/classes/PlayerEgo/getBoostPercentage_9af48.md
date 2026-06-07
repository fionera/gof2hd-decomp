# PlayerEgo::getBoostPercentage
elf 0x9af48 body 88
Sig: undefined __stdcall getBoostPercentage(void)

## decompile
```c

/* PlayerEgo::getBoostPercentage() */

float PlayerEgo::getBoostPercentage(void)

{
  bool bVar1;
  int in_r0;
  undefined4 uVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  
  uVar2 = __aeabi_idiv(*(undefined4 *)(in_r0 + 0xcc),6);
  fVar4 = (float)VectorSignedToFloat(uVar2,(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 0x138),(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = fVar3 / fVar4;
  if ((-1 < (int)((uint)(fVar3 < 1.0) << 0x1f)) &&
     (fVar4 = 6.0 - fVar3, bVar1 = 5.0 < fVar3, fVar3 = 1.0, bVar1)) {
    fVar3 = fVar4;
  }
  return fVar3;
}

```

## target disasm
```
  000aaf48: push {r4,r6,r7,lr}
  000aaf4a: add r7,sp,#0x8
  000aaf4c: mov r4,r0
  000aaf4e: ldr.w r0,[r0,#0xcc]
  000aaf52: movs r1,#0x6
  000aaf54: blx 0x0007198c
  000aaf58: vldr.32 s0,[r4,#0x138]
  000aaf5c: vmov s2,r0
  000aaf60: vcvt.f32.s32 s2,s2
  000aaf64: vcvt.f32.s32 s0,s0
  000aaf68: vdiv.f32 s2,s0,s2
  000aaf6c: vmov.f32 s0,0x3f800000
  000aaf70: vcmpe.f32 s2,s0
  000aaf74: vmrs apsr,fpscr
  000aaf78: bpl 0x000aaf80
  000aaf7a: vmov.f32 s0,s2
  000aaf7e: b 0x000aaf9a
  000aaf80: vmov.f32 s6,0x40a00000
  000aaf84: vmov.f32 s4,0x40c00000
  000aaf88: vcmpe.f32 s2,s6
  000aaf8c: vmrs apsr,fpscr
  000aaf90: vsub.f32 s4,s4,s2
  000aaf94: it gt
  000aaf96: vmov.gt.f32 s0,s4
  000aaf9a: vmov r0,s0
  000aaf9e: pop {r4,r6,r7,pc}
```
