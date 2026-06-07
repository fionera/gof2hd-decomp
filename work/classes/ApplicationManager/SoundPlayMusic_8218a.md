# ApplicationManager::SoundPlayMusic
elf 0x8218a body 22
Sig: undefined __stdcall SoundPlayMusic(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPlayMusic(int) */

void AbyssEngine::ApplicationManager::SoundPlayMusic(int param_1)

{
  if ((*(int *)(param_1 + 0xac) != 0) && (*(char *)(param_1 + 0xb1) != '\0')) {
    (*(code *)(DAT_001ab4e4 + 0x1ab4e8))();
    return;
  }
  return;
}

```

## target disasm
```
  0009218a: mov r2,r0
  0009218c: ldr.w r0,[r0,#0xac]
  00092190: cbz r0,0x0009219c
  00092192: ldrb.w r2,[r2,#0xb1]
  00092196: cbz r2,0x0009219c
  00092198: b.w 0x001ab4d8
  0009219c: bx lr
  001ab4d8: bx pc
```
