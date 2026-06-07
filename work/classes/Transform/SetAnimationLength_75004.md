# Transform::SetAnimationLength
elf 0x75004 body 10
Sig: undefined __stdcall SetAnimationLength(longlong param_1)

## decompile
```c

/* AbyssEngine::Transform::SetAnimationLength(long long) */

void AbyssEngine::Transform::SetAnimationLength(longlong param_1)

{
  int iVar1;
  undefined4 in_r2;
  undefined4 in_r3;
  
  iVar1 = (int)param_1;
  *(undefined4 *)(iVar1 + 0x108) = in_r2;
  *(undefined4 *)(iVar1 + 0x10c) = in_r3;
  *(undefined4 *)(iVar1 + 0xf8) = in_r2;
  *(undefined4 *)(iVar1 + 0xfc) = in_r3;
  return;
}

```

## target disasm
```
  00085004: strd r2,r3,[r0,#0x108]
  00085008: strd r2,r3,[r0,#0xf8]
  0008500c: bx lr
```
