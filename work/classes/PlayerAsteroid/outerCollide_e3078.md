# PlayerAsteroid::outerCollide
elf 0xe3078 body 10
Sig: undefined __stdcall outerCollide(Vector param_1)

## decompile
```c

/* PlayerAsteroid::outerCollide(AbyssEngine::AEMath::Vector) */

void PlayerAsteroid::outerCollide(int *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x000f3080. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x38))();
  return;
}

```

## target disasm
```
  000f3078: ldr.w r12,[r0,#0x0]
  000f307c: ldr.w r12,[r12,#0x38]
  000f3080: bx r12
```
