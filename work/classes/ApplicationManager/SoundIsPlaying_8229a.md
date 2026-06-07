# ApplicationManager::SoundIsPlaying
elf 0x8229a body 16
Sig: undefined __stdcall SoundIsPlaying(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundIsPlaying(int) */

undefined4 AbyssEngine::ApplicationManager::SoundIsPlaying(int param_1)

{
  undefined4 uVar1;
  
  if (*(int *)(param_1 + 0xac) == 0) {
    return 0;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab5b0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (*(code *)(DAT_001ab5b4 + 0x1ab5b8))();
  return uVar1;
}

```

## target disasm
```
  0009229a: ldr.w r0,[r0,#0xac]
  0009229e: cbz r0,0x000922a4
  000922a0: b.w 0x001ab5a8
  000922a4: movs r0,#0x0
  000922a6: bx lr
  001ab5a8: bx pc
```
