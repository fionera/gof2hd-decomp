# ApplicationManager::SoundResume
elf 0x822a8 body 16
Sig: undefined __thiscall SoundResume(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundResume() */

void __thiscall AbyssEngine::ApplicationManager::SoundResume(ApplicationManager *this)

{
  if (*(int *)(this + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab570. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab574 + 0x1ab578))();
  return;
}

```

## target disasm
```
  000922a8: ldr.w r0,[r0,#0xac]
  000922ac: cmp r0,#0x0
  000922ae: it ne
  000922b0: b.ne.w 0x001ab568
  000922b4: bx lr
  001ab568: bx pc
```
