# ApplicationManager::SoundPlay
elf 0x821c6 body 22
Sig: undefined __stdcall SoundPlay(int param_1, float param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPlay(int, float) */

void AbyssEngine::ApplicationManager::SoundPlay(int param_1,float param_2)

{
  if ((*(int *)(param_1 + 0xac) != 0) && (*(char *)(param_1 + 0xb0) != '\0')) {
    (*(code *)(DAT_001ab494 + 0x1ab498))();
    return;
  }
  return;
}

```

## target disasm
```
  000921c6: mov r3,r0
  000921c8: ldr.w r0,[r0,#0xac]
  000921cc: cbz r0,0x000921d8
  000921ce: ldrb.w r3,[r3,#0xb0]
  000921d2: cbz r3,0x000921d8
  000921d4: b.w 0x001ab488
  000921d8: bx lr
  001ab488: bx pc
```
