# ApplicationManager::SoundSetVolume
elf 0x82270 body 16
Sig: undefined __stdcall SoundSetVolume(int param_1, int param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundSetVolume(int, int) */

void AbyssEngine::ApplicationManager::SoundSetVolume(int param_1,int param_2)

{
  if (*(int *)(param_1 + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab580. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab584 + 0x1ab588))();
  return;
}

```

## target disasm
```
  00092270: ldr.w r0,[r0,#0xac]
  00092274: cmp r0,#0x0
  00092276: it ne
  00092278: b.ne.w 0x001ab578
  0009227c: bx lr
  001ab578: bx pc
```
