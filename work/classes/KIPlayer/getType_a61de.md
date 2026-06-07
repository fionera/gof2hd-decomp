# KIPlayer::getType
elf 0xa61de body 6
Sig: undefined __thiscall getType(KIPlayer * this)

## decompile
```c

/* KIPlayer::getType() */

undefined4 __thiscall KIPlayer::getType(KIPlayer *this)

{
  return *(undefined4 *)(this + 0xac);
}

```

## target disasm
```
  000b61de: ldr.w r0,[r0,#0xac]
  000b61e2: bx lr
```
