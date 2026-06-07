# KIPlayer::PlayEngineSound
elf 0xa6192 body 24
Sig: undefined __thiscall PlayEngineSound(KIPlayer * this)

## decompile
```c

/* KIPlayer::PlayEngineSound() */

void __thiscall KIPlayer::PlayEngineSound(KIPlayer *this)

{
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xf8) != -1)) {
    (*(code *)(DAT_001aba14 + 0x1aba18))(*(int *)(this + 4),*(int *)(this + 0xf8),0);
    return;
  }
  return;
}

```

## target disasm
```
  000b6192: mov r1,r0
  000b6194: ldr r0,[r0,#0x4]
  000b6196: cbz r0,0x000b61a6
  000b6198: ldr.w r1,[r1,#0xf8]
  000b619c: adds r2,r1,#0x1
  000b619e: itt ne
  000b61a0: mov.ne r2,#0x0
  000b61a2: b.ne.w 0x001aba08
  000b61a6: bx lr
  001aba08: bx pc
```
