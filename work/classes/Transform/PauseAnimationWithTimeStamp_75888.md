# Transform::PauseAnimationWithTimeStamp
elf 0x75888 body 24
Sig: undefined __stdcall PauseAnimationWithTimeStamp(longlong param_1)

## decompile
```c

/* AbyssEngine::Transform::PauseAnimationWithTimeStamp(long long) */

void AbyssEngine::Transform::PauseAnimationWithTimeStamp(longlong param_1)

{
  int iVar1;
  undefined4 in_r2;
  undefined4 in_r3;
  
  iVar1 = (int)param_1;
  *(undefined4 *)(iVar1 + 0x110) = in_r2;
  *(undefined4 *)(iVar1 + 0x114) = in_r3;
  InternUpdate(param_1,SUB41(in_r2,0));
  *(undefined1 *)(iVar1 + 0xed) = 0;
  return;
}

```

## target disasm
```
  00085888: push {r2,r3,r4,r5,r7,lr}
  0008588a: add r7,sp,#0x10
  0008588c: mov r4,r0
  0008588e: movs r5,#0x0
  00085890: strd r2,r3,[r4,#0x110]
  00085894: str r5,[sp,#0x0]
  00085896: blx 0x0006f7e4
  0008589a: strb.w r5,[r4,#0xed]
  0008589e: pop {r2,r3,r4,r5,r7,pc}
```
