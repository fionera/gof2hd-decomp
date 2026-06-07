# PlayerAsteroid::outerCollide
elf 0xe306e body 10
Sig: undefined __stdcall outerCollide(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerAsteroid::outerCollide(float, float, float) */

void PlayerAsteroid::outerCollide(float param_1,float param_2,float param_3)

{
  int *in_r0;
  
                    /* WARNING: Could not recover jumptable at 0x000f3076. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*in_r0 + 0x38))();
  return;
}

```

## target disasm
```
  000f306e: ldr.w r12,[r0,#0x0]
  000f3072: ldr.w r12,[r12,#0x38]
  000f3076: bx r12
```
