# PlayerStation::collide
elf 0x12276e body 10
Sig: undefined __stdcall collide(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerStation::collide(float, float, float) */

void PlayerStation::collide(float param_1,float param_2,float param_3)

{
  int *in_r0;
  
                    /* WARNING: Could not recover jumptable at 0x00132776. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*in_r0 + 0x3c))();
  return;
}

```

## target disasm
```
  0013276e: ldr.w r12,[r0,#0x0]
  00132772: ldr.w r12,[r12,#0x3c]
  00132776: bx r12
```
