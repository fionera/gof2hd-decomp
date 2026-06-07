# ApplicationManager::SoundPauseSounds
elf 0x8220a body 16
Sig: undefined __thiscall SoundPauseSounds(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundPauseSounds() */

void __thiscall AbyssEngine::ApplicationManager::SoundPauseSounds(ApplicationManager *this)

{
  if (*(int *)(this + 0xac) == 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab540. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab544 + 0x1ab548))();
  return;
}

```

## target disasm
```
  0009220a: ldr.w r0,[r0,#0xac]
  0009220e: cmp r0,#0x0
  00092210: it ne
  00092212: b.ne.w 0x001ab538
  00092216: bx lr
  001ab538: bx pc
```
