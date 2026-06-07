# FModSound::promptMusicCue
elf 0x93ad8 body 16
Sig: undefined __stdcall promptMusicCue(int param_1)

## decompile
```c

/* FModSound::promptMusicCue(int) */

void FModSound::promptMusicCue(int param_1)

{
  if (*(int **)(param_1 + 0x2400) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x000a3ae4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(int **)(param_1 + 0x2400) + 0x28))();
    return;
  }
  return;
}

```

## target disasm
```
  000a3ad8: mov.w r2,#0x2400
  000a3adc: ldr r0,[r0,r2]
  000a3ade: cbz r0,0x000a3ae6
  000a3ae0: ldr r2,[r0,#0x0]
  000a3ae2: ldr r2,[r2,#0x28]
  000a3ae4: bx r2
  000a3ae6: bx lr
```
