# Level::collide
elf 0xc455a body 20
Sig: undefined __stdcall collide(Vector param_1, bool param_2)

## decompile
```c

/* Level::collide(AbyssEngine::AEMath::Vector, bool) */

undefined4 Level::collide(int param_1)

{
  undefined4 uVar1;
  
  if (*(int **)(param_1 + 0xc4) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x000d4568. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(**(int **)(param_1 + 0xc4) + 8))();
    return uVar1;
  }
  return 0;
}

```
## target disasm
```
  000d455a: ldr.w r0,[r0,#0xc4]
  000d455e: cbz r0,0x000d456a
  000d4560: ldr.w r12,[r0,#0x0]
  000d4564: ldr.w r12,[r12,#0x8]
  000d4568: bx r12
  000d456a: movs r0,#0x0
  000d456c: bx lr
```
