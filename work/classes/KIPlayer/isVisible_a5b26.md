# KIPlayer::isVisible
elf 0xa5b26 body 6
Sig: undefined __thiscall isVisible(KIPlayer * this)

## decompile
```c

/* KIPlayer::isVisible() */

KIPlayer __thiscall KIPlayer::isVisible(KIPlayer *this)

{
  return this[0xf5];
}

```

## target disasm
```
  000b5b26: ldrb.w r0,[r0,#0xf5]
  000b5b2a: bx lr
```
