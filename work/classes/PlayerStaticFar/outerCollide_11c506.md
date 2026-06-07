# PlayerStaticFar::outerCollide
elf 0x11c506 body 10
Sig: undefined __stdcall outerCollide(Vector param_1)

## decompile
```c

/* PlayerStaticFar::outerCollide(AbyssEngine::AEMath::Vector) */

void PlayerStaticFar::outerCollide(int *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x0012c50e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x38))();
  return;
}

```

## target disasm
```
  0012c506: ldr.w r12,[r0,#0x0]
  0012c50a: ldr.w r12,[r12,#0x38]
  0012c50e: bx r12
```
