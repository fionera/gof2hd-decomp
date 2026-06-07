# AENormalFile::Skip
elf 0x7017e body 14
Sig: undefined __stdcall Skip(uint param_1)

## decompile
```c

/* AENormalFile::Skip(unsigned int) */

undefined4 AENormalFile::Skip(uint param_1)

{
  undefined4 uVar1;
  
  if (*(int **)(param_1 + 4) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00080186. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(**(int **)(param_1 + 4) + 0x1c))();
    return uVar1;
  }
  return 0;
}

```

## target disasm
```
  0008017e: ldr r0,[r0,#0x4]
  00080180: cbz r0,0x00080188
  00080182: ldr r2,[r0,#0x0]
  00080184: ldr r2,[r2,#0x1c]
  00080186: bx r2
  00080188: movs r0,#0x0
  0008018a: bx lr
```
