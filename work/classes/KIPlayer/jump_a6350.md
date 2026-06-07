# KIPlayer::jump
elf 0xa6350 body 14
Sig: undefined __thiscall jump(KIPlayer * this)

## decompile
```c

/* KIPlayer::jump() */

void __thiscall KIPlayer::jump(KIPlayer *this)

{
  this[0xec] = (KIPlayer)0x1;
  this[0xf4] = (KIPlayer)0x0;
  return;
}

```

## target disasm
```
  000b6350: movs r1,#0x1
  000b6352: strb.w r1,[r0,#0xec]
  000b6356: movs r1,#0x0
  000b6358: strb.w r1,[r0,#0xf4]
  000b635c: bx lr
```
