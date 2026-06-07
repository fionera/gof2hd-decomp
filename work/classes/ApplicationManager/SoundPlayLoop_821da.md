# ApplicationManager::SoundPlayLoop
elf 0x821da body 22
Sig: undefined __stdcall SoundPlayLoop(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPlayLoop(int) */

void AbyssEngine::ApplicationManager::SoundPlayLoop(int param_1)

{
  if ((*(int *)(param_1 + 0xac) != 0) && (*(char *)(param_1 + 0xb0) != '\0')) {
                    /* WARNING: Could not recover jumptable at 0x001ab510. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(DAT_001ab514 + 0x1ab518))();
    return;
  }
  return;
}

```

## target disasm
```
  000921da: mov r2,r0
  000921dc: ldr.w r0,[r0,#0xac]
  000921e0: cbz r0,0x000921ec
  000921e2: ldrb.w r2,[r2,#0xb0]
  000921e6: cbz r2,0x000921ec
  000921e8: b.w 0x001ab508
  000921ec: bx lr
  001ab508: bx pc
```
