# FModSound::setVolume
elf 0x92fb8 body 30
Sig: undefined __stdcall setVolume(int param_1, float param_2)

## decompile
```c

/* FModSound::setVolume(int, float) */

void FModSound::setVolume(int param_1,float param_2)

{
  int *piVar1;
  int in_r1;
  
  if ((*(int *)(param_1 + 0x23fc) != 0) &&
     (piVar1 = *(int **)(param_1 + in_r1 * 4 + 0x23ec), piVar1 != (int *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x000a2fd2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*piVar1 + 0x20))();
    return;
  }
  return;
}

```

## target disasm
```
  000a2fb8: movw r3,#0x23fc
  000a2fbc: ldr r3,[r0,r3]
  000a2fbe: cbz r3,0x000a2fd4
  000a2fc0: add.w r0,r0,r1, lsl #0x2
  000a2fc4: movw r1,#0x23ec
  000a2fc8: ldr r0,[r0,r1]
  000a2fca: cbz r0,0x000a2fd4
  000a2fcc: ldr r1,[r0,#0x0]
  000a2fce: ldr r3,[r1,#0x20]
  000a2fd0: mov r1,r2
  000a2fd2: bx r3
  000a2fd4: bx lr
```
