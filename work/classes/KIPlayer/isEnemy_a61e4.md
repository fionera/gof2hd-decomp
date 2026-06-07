# KIPlayer::isEnemy
elf 0xa61e4 body 8
Sig: undefined __thiscall isEnemy(KIPlayer * this)

## decompile
```c

/* KIPlayer::isEnemy() */

undefined1 __thiscall KIPlayer::isEnemy(KIPlayer *this)

{
  return *(undefined1 *)(*(int *)(this + 4) + 0x5c);
}

```

## target disasm
```
  000b61e4: ldr r0,[r0,#0x4]
  000b61e6: ldrb.w r0,[r0,#0x5c]
  000b61ea: bx lr
```
