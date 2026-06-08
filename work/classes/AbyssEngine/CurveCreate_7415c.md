# AbyssEngine::CurveCreate
elf 0x7415c body 56
Sig: undefined __stdcall CurveCreate(void * * param_1, ushort param_2, Curve * * param_3)

## decompile
```c

/* AbyssEngine::CurveCreate(void**, unsigned short, AbyssEngine::Curve**) */

undefined4 AbyssEngine::CurveCreate(void **param_1,ushort param_2,Curve **param_3)

{
  Curve *pCVar1;
  void *pvVar2;
  
  pCVar1 = operator_new(8);
  *(undefined4 *)(pCVar1 + 4) = 0;
  *param_3 = pCVar1;
  *(ushort *)pCVar1 = param_2;
  pvVar2 = operator_new__((uint)param_2 << 2);
  *(void **)(pCVar1 + 4) = pvVar2;
  __aeabi_memcpy4(pvVar2,param_1,(uint)param_2 << 2);
  return 1;
}

```

## target disasm
```
  0008415c: push {r4,r5,r6,r7,lr}
  0008415e: add r7,sp,#0xc
  00084160: push.w r8
  00084164: mov r8,r0
  00084166: movs r0,#0x8
  00084168: mov r4,r2
  0008416a: mov r5,r1
  0008416c: blx 0x0006eb24
  00084170: mov r6,r0
  00084172: movs r0,#0x0
  00084174: str r0,[r6,#0x4]
  00084176: str r6,[r4,#0x0]
  00084178: lsls r4,r5,#0x2
  0008417a: mov r0,r4
  0008417c: strh r5,[r6,#0x0]
  0008417e: blx 0x0006ec08
  00084182: mov r1,r8
  00084184: mov r2,r4
  00084186: str r0,[r6,#0x4]
  00084188: blx 0x0006f6a0
  0008418c: movs r0,#0x1
  0008418e: pop.w r8
  00084192: pop {r4,r5,r6,r7,pc}
```
