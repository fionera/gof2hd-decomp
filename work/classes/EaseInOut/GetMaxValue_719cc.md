# EaseInOut::GetMaxValue
elf 0x719cc body 18
Sig: undefined __stdcall GetMaxValue(void)

## decompile
```c

/* AbyssEngine::EaseInOut::GetMaxValue() */

float AbyssEngine::EaseInOut::GetMaxValue(void)

{
  float *in_r0;
  
  return *in_r0 + in_r0[1];
}

```

## target disasm
```
  000819cc: vldr.32 s0,[r0]
  000819d0: vldr.32 s2,[r0,#0x4]
  000819d4: vadd.f32 s0,s0,s2
  000819d8: vmov r0,s0
  000819dc: bx lr
```
