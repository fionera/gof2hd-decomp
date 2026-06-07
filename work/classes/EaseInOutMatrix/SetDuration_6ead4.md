# EaseInOutMatrix::SetDuration
elf 0x6ead4 body 14
Sig: undefined __thiscall SetDuration(EaseInOutMatrix * this, int param_1)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::SetDuration(int) */

void __thiscall AbyssEngine::EaseInOutMatrix::SetDuration(EaseInOutMatrix *this,int param_1)

{
  uint in_fpscr;
  undefined4 uVar1;
  
  uVar1 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0xf0) = uVar1;
  return;
}

```

## target disasm
```
  0007ead4: vmov s0,r1
  0007ead8: vcvt.f32.s32 s0,s0
  0007eadc: vstr.32 s0,[r0,#0xf0]
  0007eae0: bx lr
```
