# KIPlayer::PauseEngineSound
elf 0xa617e body 22
Sig: undefined __thiscall PauseEngineSound(KIPlayer * this)

## decompile
```c

/* KIPlayer::PauseEngineSound() */

void __thiscall KIPlayer::PauseEngineSound(KIPlayer *this)

{
  if ((*(int *)(this + 4) != 0) && (*(int *)(this + 0xf8) != -1)) {
    (*(code *)(DAT_001aba24 + 0x1aba28))();
    return;
  }
  return;
}

```

## target disasm
```
  000b617e: mov r1,r0
  000b6180: ldr r0,[r0,#0x4]
  000b6182: cbz r0,0x000b6190
  000b6184: ldr.w r1,[r1,#0xf8]
  000b6188: adds r1,#0x1
  000b618a: it ne
  000b618c: b.ne.w 0x001aba18
  000b6190: bx lr
  001aba18: bx pc
```
