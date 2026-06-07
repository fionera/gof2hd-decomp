# KIPlayer::isJumper
elf 0xa5dde body 6
Sig: undefined __thiscall isJumper(KIPlayer * this)

## decompile
```c

/* KIPlayer::isJumper() */

KIPlayer __thiscall KIPlayer::isJumper(KIPlayer *this)

{
  return this[0xec];
}

```

## target disasm
```
  000b5dde: ldrb.w r0,[r0,#0xec]
  000b5de2: bx lr
```
