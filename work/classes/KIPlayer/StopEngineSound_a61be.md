# KIPlayer::StopEngineSound
elf 0xa61be body 22
Sig: undefined __thiscall StopEngineSound(KIPlayer * this)

## decompile
```c

/* KIPlayer::StopEngineSound() */

void __thiscall KIPlayer::StopEngineSound(KIPlayer *this)

{
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xf8) != -1)) {
    (*(code *)(DAT_001aba44 + 0x1aba48))();
    return;
  }
  return;
}

```

## target disasm
```
  000b61be: mov r1,r0
  000b61c0: ldr r0,[r0,#0x4]
  000b61c2: cbz r0,0x000b61d0
  000b61c4: ldr.w r1,[r1,#0xf8]
  000b61c8: adds r1,#0x1
  000b61ca: it ne
  000b61cc: b.ne.w 0x001aba38
  000b61d0: bx lr
  001aba38: bx pc
```
