# ApplicationManager::SoundResumeSounds
elf 0x82240 body 26
Sig: undefined __thiscall SoundResumeSounds(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundResumeSounds() */

void __thiscall AbyssEngine::ApplicationManager::SoundResumeSounds(ApplicationManager *this)

{
  if (((*(int *)(this + 0xac) == 0) || (this[0xb0] == (ApplicationManager)0x0)) &&
     (this[0xb1] == (ApplicationManager)0x0)) {
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
  00092240: mov r1,r0
  00092242: ldr.w r0,[r0,#0xac]
  00092246: cbz r0,0x0009224e
  00092248: ldrb.w r2,[r1,#0xb0]
  0009224c: cbnz r2,0x00092254
  0009224e: ldrb.w r1,[r1,#0xb1]
  00092252: cbz r1,0x00092258
  00092254: b.w 0x001ab568
  00092258: bx lr
```
