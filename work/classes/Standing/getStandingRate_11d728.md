# Standing::getStandingRate
elf 0x11d728 body 30
Sig: undefined __stdcall getStandingRate(int param_1)

## decompile
```c

/* Standing::getStandingRate(int) */

float Standing::getStandingRate(int param_1)

{
  undefined4 uVar1;
  uint in_fpscr;
  float fVar2;
  
  uVar1 = getStanding(param_1);
  fVar2 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  return fVar2 / DAT_0012d748;
}

```

## target disasm
```
  0012d728: push {r7,lr}
  0012d72a: mov r7,sp
  0012d72c: blx 0x00074fb0
  0012d730: vmov s0,r0
  0012d734: vldr.32 s2,[pc,#0x10]
  0012d738: vcvt.f32.s32 s0,s0
  0012d73c: vdiv.f32 s0,s0,s2
  0012d740: vmov r0,s0
  0012d744: pop {r7,pc}
```
