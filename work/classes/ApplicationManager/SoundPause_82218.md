# ApplicationManager::SoundPause
elf 0x82218 body 16
Sig: undefined __stdcall SoundPause(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPause(int) */

void AbyssEngine::ApplicationManager::SoundPause(int param_1)

{
  if (*(int *)(param_1 + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab550. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab554 + 0x1ab558))();
  return;
}

```

## target disasm
```
  00092218: ldr.w r0,[r0,#0xac]
  0009221c: cmp r0,#0x0
  0009221e: it ne
  00092220: b.ne.w 0x001ab548
  00092224: bx lr
  001ab548: bx pc
```
