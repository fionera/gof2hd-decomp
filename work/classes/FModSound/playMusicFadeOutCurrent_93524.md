# FModSound::playMusicFadeOutCurrent
elf 0x93524 body 38
Sig: undefined __stdcall playMusicFadeOutCurrent(int param_1)

## decompile
```c

/* FModSound::playMusicFadeOutCurrent(int) */

void FModSound::playMusicFadeOutCurrent(int param_1)

{
  int in_r1;
  int iVar1;
  float in_s0;
  
  iVar1 = *(int *)param_1;
  if (iVar1 != in_r1) {
    if (iVar1 == -1) {
      *(int *)param_1 = in_r1;
      iVar1 = in_r1;
    }
    setParamValue((FModSound *)param_1,0,iVar1,in_s0);
    *(int *)(param_1 + 4) = in_r1;
    return;
  }
  return;
}

```

## target disasm
```
  000a3524: push {r4,r5,r7,lr}
  000a3526: add r7,sp,#0x8
  000a3528: ldr r2,[r0,#0x0]
  000a352a: mov r4,r1
  000a352c: mov r5,r0
  000a352e: cmp r2,r1
  000a3530: it eq
  000a3532: pop.eq {r4,r5,r7,pc}
  000a3534: ldr r3,[0x000a354c]
  000a3536: adds r0,r2,#0x1
  000a3538: itt eq
  000a353a: str.eq r4,[r5,#0x0]
  000a353c: mov.eq r2,r4
  000a353e: mov r0,r5
  000a3540: movs r1,#0x0
  000a3542: blx 0x0007159c
  000a3546: str r4,[r5,#0x4]
  000a3548: pop {r4,r5,r7,pc}
```
