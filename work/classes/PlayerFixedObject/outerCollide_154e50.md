# PlayerFixedObject::outerCollide
elf 0x154e50 body 10
Sig: undefined __stdcall outerCollide(Vector param_1)

## decompile
```c

/* PlayerFixedObject::outerCollide(AbyssEngine::AEMath::Vector) */

void PlayerFixedObject::outerCollide(int *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00164e58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x3c))();
  return;
}

```

## target disasm
```
  00164e50: ldr.w r12,[r0,#0x0]
  00164e54: ldr.w r12,[r12,#0x3c]
  00164e58: bx r12
```
