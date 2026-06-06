# Level::collideStream
elf 0xc457c body 24
Sig: undefined __stdcall collideStream(Vector param_1)

## decompile
```c

/* Level::collideStream(AbyssEngine::AEMath::Vector) */

undefined4 Level::collideStream(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = *(int **)(*(int *)(*(int *)(param_1 + 0x100) + 4) + 4);
  if (piVar1 != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x000d458e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(*piVar1 + 0x38))();
    return uVar2;
  }
  return 0;
}

```
## target disasm
```
  000d457c: ldr.w r0,[r0,#0x100]
  000d4580: ldr r0,[r0,#0x4]
  000d4582: ldr r0,[r0,#0x4]
  000d4584: cbz r0,0x000d4590
  000d4586: ldr.w r12,[r0,#0x0]
  000d458a: ldr.w r12,[r12,#0x38]
  000d458e: bx r12
  000d4590: movs r0,#0x0
  000d4592: bx lr
```
