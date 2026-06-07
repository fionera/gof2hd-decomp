# EaseInOut::EaseInOut
elf 0x71810 body 14
Sig: undefined __thiscall EaseInOut(EaseInOut * this, float param_1, float param_2)

## decompile
```c

/* AbyssEngine::EaseInOut::EaseInOut(float, float) */

EaseInOut * __thiscall
AbyssEngine::EaseInOut::EaseInOut(EaseInOut *this,float param_1,float param_2)

{
  SetRange(param_1,param_2);
  return this;
}

```

## target disasm
```
  00081810: push {r4,r6,r7,lr}
  00081812: add r7,sp,#0x8
  00081814: mov r4,r0
  00081816: blx 0x0006f4fc
  0008181a: mov r0,r4
  0008181c: pop {r4,r6,r7,pc}
```
