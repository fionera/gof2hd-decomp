# ApplicationManager::SoundPlayMusicLoop
elf 0x8219e body 22
Sig: undefined __stdcall SoundPlayMusicLoop(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPlayMusicLoop(int) */

void AbyssEngine::ApplicationManager::SoundPlayMusicLoop(int param_1)

{
  if ((*(int *)(param_1 + 0xac) != 0) && (*(char *)(param_1 + 0xb1) != '\0')) {
    (*(code *)(DAT_001ab4f4 + 0x1ab4f8))();
    return;
  }
  return;
}

```

## target disasm
```
  0009219e: mov r2,r0
  000921a0: ldr.w r0,[r0,#0xac]
  000921a4: cbz r0,0x000921b0
  000921a6: ldrb.w r2,[r2,#0xb1]
  000921aa: cbz r2,0x000921b0
  000921ac: b.w 0x001ab4e8
  000921b0: bx lr
  001ab4e8: bx pc
```
