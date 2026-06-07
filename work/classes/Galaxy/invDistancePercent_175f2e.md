# Galaxy::invDistancePercent
elf 0x175f2e body 20
Sig: undefined __stdcall invDistancePercent(int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* Galaxy::invDistancePercent(int, int, int, int) */

int Galaxy::invDistancePercent(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  Galaxy *in_stack_00000000;
  
  iVar1 = distancePercent(in_stack_00000000,param_2,param_3,param_4,(int)in_stack_00000000);
  return 100 - iVar1;
}

```

## target disasm
```
  00185f2e: push {r5,r6,r7,lr}
  00185f30: add r7,sp,#0x8
  00185f32: ldr r0,[r7,#0x8]
  00185f34: str r0,[sp,#0x0]
  00185f36: blx 0x000739d8
  00185f3a: rsb.w r0,r0,#0x64
  00185f3e: add sp,#0x8
  00185f40: pop {r7,pc}
```
