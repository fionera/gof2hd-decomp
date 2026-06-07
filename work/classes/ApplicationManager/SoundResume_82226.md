# ApplicationManager::SoundResume
elf 0x82226 body 28
Sig: undefined __stdcall SoundResume(int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundResume(int) */

void AbyssEngine::ApplicationManager::SoundResume(int param_1)

{
  if (((*(int *)(param_1 + 0xac) == 0) || (*(char *)(param_1 + 0xb0) == '\0')) &&
     (*(char *)(param_1 + 0xb1) == '\0')) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab560. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab564 + 0x1ab568))();
  return;
}

```

## target disasm
```
  00092226: mov r2,r0
  00092228: ldr.w r0,[r0,#0xac]
  0009222c: cbz r0,0x00092234
  0009222e: ldrb.w r3,[r2,#0xb0]
  00092232: cbnz r3,0x0009223a
  00092234: ldrb.w r2,[r2,#0xb1]
  00092238: cbz r2,0x0009223e
  0009223a: b.w 0x001ab558
  0009223e: bx lr
  001ab558: bx pc
```
