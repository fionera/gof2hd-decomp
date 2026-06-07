# Transform::SetCurrentAnimationTime
elf 0x77424 body 6
Sig: undefined __stdcall SetCurrentAnimationTime(longlong param_1)

## decompile
```c

/* AbyssEngine::Transform::SetCurrentAnimationTime(long long) */

void AbyssEngine::Transform::SetCurrentAnimationTime(longlong param_1)

{
  undefined4 in_r2;
  undefined4 in_r3;
  
  *(undefined4 *)((int)param_1 + 0x110) = in_r2;
  *(undefined4 *)((int)param_1 + 0x114) = in_r3;
  return;
}

```

## target disasm
```
  00087424: strd r2,r3,[r0,#0x110]
  00087428: bx lr
```
