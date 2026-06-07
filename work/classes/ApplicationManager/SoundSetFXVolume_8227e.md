# ApplicationManager::SoundSetFXVolume
elf 0x8227e body 16
Sig: undefined __stdcall SoundSetFXVolume(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundSetFXVolume(int) */

void AbyssEngine::ApplicationManager::SoundSetFXVolume(int param_1)

{
  if (*(int *)(param_1 + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab590. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab594 + 0x1ab598))();
  return;
}

```

## target disasm
```
  0009227e: ldr.w r0,[r0,#0xac]
  00092282: cmp r0,#0x0
  00092284: it ne
  00092286: b.ne.w 0x001ab588
  0009228a: bx lr
  001ab588: bx pc
```
