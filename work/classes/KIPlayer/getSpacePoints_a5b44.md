# KIPlayer::getSpacePoints
elf 0xa5b44 body 6
Sig: undefined __thiscall getSpacePoints(KIPlayer * this)

## decompile
```c

/* KIPlayer::getSpacePoints() */

undefined4 __thiscall KIPlayer::getSpacePoints(KIPlayer *this)

{
  return *(undefined4 *)(this + 0xcc);
}

```

## target disasm
```
  000b5b44: ldr.w r0,[r0,#0xcc]
  000b5b48: bx lr
```
