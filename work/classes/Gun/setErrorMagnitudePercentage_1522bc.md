# Gun::setErrorMagnitudePercentage
elf 0x1522bc body 14
Sig: undefined __thiscall setErrorMagnitudePercentage(Gun * this, int param_1)

## decompile
```c

/* Gun::setErrorMagnitudePercentage(int) */

void __thiscall Gun::setErrorMagnitudePercentage(Gun *this,int param_1)

{
  uint in_fpscr;
  undefined4 uVar1;
  
  uVar1 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0xfc) = uVar1;
  return;
}

```

## target disasm
```
  001622bc: vmov s0,r1
  001622c0: vcvt.f32.s32 s0,s0
  001622c4: vstr.32 s0,[r0,#0xfc]
  001622c8: bx lr
```
