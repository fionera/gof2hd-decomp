# ApplicationManager::SoundPlay
elf 0x821b2 body 22
Sig: undefined __stdcall SoundPlay(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPlay(int) */

void AbyssEngine::ApplicationManager::SoundPlay(int param_1)

{
  if ((*(int *)(param_1 + 0xac) != 0) && (*(char *)(param_1 + 0xb0) != '\0')) {
                    /* WARNING: Could not recover jumptable at 0x001ab500. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(DAT_001ab504 + 0x1ab508))();
    return;
  }
  return;
}

```

## target disasm
```
  000921b2: mov r2,r0
  000921b4: ldr.w r0,[r0,#0xac]
  000921b8: cbz r0,0x000921c4
  000921ba: ldrb.w r2,[r2,#0xb0]
  000921be: cbz r2,0x000921c4
  000921c0: b.w 0x001ab4f8
  000921c4: bx lr
  001ab4f8: bx pc
```
