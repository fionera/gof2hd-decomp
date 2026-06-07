# ApplicationManager::SoundStop
elf 0x821ee body 16
Sig: undefined __stdcall SoundStop(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundStop(int) */

void AbyssEngine::ApplicationManager::SoundStop(int param_1)

{
  if (*(int *)(param_1 + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab520. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab524 + 0x1ab528))();
  return;
}

```

## target disasm
```
  000921ee: ldr.w r0,[r0,#0xac]
  000921f2: cmp r0,#0x0
  000921f4: it ne
  000921f6: b.ne.w 0x001ab518
  000921fa: bx lr
  001ab518: bx pc
```
