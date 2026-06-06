# Level::isInAsteroidCenterRange
elf 0xc456e body 14
Sig: undefined __stdcall isInAsteroidCenterRange(Vector param_1)

## decompile
```c

/* Level::isInAsteroidCenterRange(AbyssEngine::AEMath::Vector) */

void Level::isInAsteroidCenterRange(int param_1)

{
                    /* WARNING: Could not recover jumptable at 0x000d457a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 0xc4) + 8))();
  return;
}

```
## target disasm
```
  000d456e: ldr.w r0,[r0,#0xc4]
  000d4572: ldr.w r12,[r0,#0x0]
  000d4576: ldr.w r12,[r12,#0x8]
  000d457a: bx r12
```
