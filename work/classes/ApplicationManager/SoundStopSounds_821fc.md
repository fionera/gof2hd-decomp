# ApplicationManager::SoundStopSounds
elf 0x821fc body 16
Sig: undefined __thiscall SoundStopSounds(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundStopSounds() */

void __thiscall AbyssEngine::ApplicationManager::SoundStopSounds(ApplicationManager *this)

{
  if (*(int *)(this + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab530. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab534 + 0x1ab538))();
  return;
}

```

## target disasm
```
  000921fc: ldr.w r0,[r0,#0xac]
  00092200: cmp r0,#0x0
  00092202: it ne
  00092204: b.ne.w 0x001ab528
  00092208: bx lr
  001ab528: bx pc
```
