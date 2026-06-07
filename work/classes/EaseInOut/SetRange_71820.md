# EaseInOut::SetRange
elf 0x71820 body 26
Sig: undefined __stdcall SetRange(float param_1, float param_2)

## decompile
```c

/* AbyssEngine::EaseInOut::SetRange(float, float) */

void AbyssEngine::EaseInOut::SetRange(float param_1,float param_2)

{
  float fVar1;
  float *in_r0;
  float in_r1;
  float in_r2;
  
  fVar1 = DAT_0008183c;
  *in_r0 = in_r1;
  in_r0[2] = fVar1;
  in_r0[1] = in_r2 - in_r1;
  (*(code *)(DAT_001ab0f4 + 0x1ab0f8))();
  return;
}

```

## target disasm
```
  00081820: vmov s2,r2
  00081824: ldr r2,[0x0008183c]
  00081826: vmov s0,r1
  0008182a: str r1,[r0,#0x0]
  0008182c: str r2,[r0,#0x8]
  0008182e: vsub.f32 s0,s2,s0
  00081832: vstr.32 s0,[r0,#0x4]
  00081836: b.w 0x001ab0e8
```
