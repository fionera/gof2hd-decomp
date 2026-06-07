# ApplicationManager::Vibrate
elf 0x822c4 body 18
Sig: undefined __stdcall Vibrate(ushort param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::Vibrate(unsigned short) */

void AbyssEngine::ApplicationManager::Vibrate(ushort param_1)

{
  if (*(char *)(param_1 + 0xb2) == '\0') {
    return;
  }
  (*(code *)(DAT_001ab334 + 0x1ab338))(*(undefined4 *)(param_1 + 0xa8));
  return;
}

```

## target disasm
```
  000922c4: ldrb.w r2,[r0,#0xb2]
  000922c8: cbz r2,0x000922d2
  000922ca: ldr.w r0,[r0,#0xa8]
  000922ce: b.w 0x001ab328
  000922d2: bx lr
  001ab328: bx pc
```
