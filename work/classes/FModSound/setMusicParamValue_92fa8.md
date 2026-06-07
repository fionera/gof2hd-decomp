# FModSound::setMusicParamValue
elf 0x92fa8 body 16
Sig: undefined __stdcall setMusicParamValue(int param_1, float param_2)

## decompile
```c

/* FModSound::setMusicParamValue(int, float) */

void FModSound::setMusicParamValue(int param_1,float param_2)

{
  if (*(int **)(param_1 + 0x2400) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x000a2fb4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(int **)(param_1 + 0x2400) + 0x34))();
    return;
  }
  return;
}

```

## target disasm
```
  000a2fa8: mov.w r3,#0x2400
  000a2fac: ldr r0,[r0,r3]
  000a2fae: cbz r0,0x000a2fb6
  000a2fb0: ldr r3,[r0,#0x0]
  000a2fb2: ldr r3,[r3,#0x34]
  000a2fb4: bx r3
  000a2fb6: bx lr
```
