# ApplicationManager::SoundRelease
elf 0x8217c body 16
Sig: undefined __stdcall SoundRelease(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundRelease(int) */

void AbyssEngine::ApplicationManager::SoundRelease(int param_1)

{
  if (*(int *)(param_1 + 0xac) == 0) {
    return;
  }
  (*(code *)(DAT_001ab4d4 + 0x1ab4d8))();
  return;
}

```

## target disasm
```
  0009217c: ldr.w r0,[r0,#0xac]
  00092180: cmp r0,#0x0
  00092182: it ne
  00092184: b.ne.w 0x001ab4c8
  00092188: bx lr
  001ab4c8: bx pc
```
