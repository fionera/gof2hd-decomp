# KIPlayer::setJumper
elf 0xa5dd8 body 6
Sig: undefined __thiscall setJumper(KIPlayer * this, bool param_1)

## decompile
```c

/* KIPlayer::setJumper(bool) */

void __thiscall KIPlayer::setJumper(KIPlayer *this,bool param_1)

{
  this[0xec] = (KIPlayer)param_1;
  return;
}

```

## target disasm
```
  000b5dd8: strb.w r1,[r0,#0xec]
  000b5ddc: bx lr
```
