# PlayerStaticFar::outerCollide
elf 0x11c4fc body 10
Sig: undefined __stdcall outerCollide(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerStaticFar::outerCollide(float, float, float) */

void PlayerStaticFar::outerCollide(float param_1,float param_2,float param_3)

{
  int *in_r0;
  
                    /* WARNING: Could not recover jumptable at 0x0012c504. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*in_r0 + 0x38))();
  return;
}

```

## target disasm
```
  0012c4fc: ldr.w r12,[r0,#0x0]
  0012c500: ldr.w r12,[r12,#0x38]
  0012c504: bx r12
```
