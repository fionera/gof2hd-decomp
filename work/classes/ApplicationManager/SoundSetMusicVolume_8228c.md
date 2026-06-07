# ApplicationManager::SoundSetMusicVolume
elf 0x8228c body 16
Sig: undefined __stdcall SoundSetMusicVolume(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundSetMusicVolume(int) */

void AbyssEngine::ApplicationManager::SoundSetMusicVolume(int param_1)

{
  if (*(int *)(param_1 + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab5a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab5a4 + 0x1ab5a8))();
  return;
}

```

## target disasm
```
  0009228c: ldr.w r0,[r0,#0xac]
  00092290: cmp r0,#0x0
  00092292: it ne
  00092294: b.ne.w 0x001ab598
  00092298: bx lr
  001ab598: bx pc
```
