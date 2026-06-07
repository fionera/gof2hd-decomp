# KIPlayer::setJumpSphere
elf 0xa6224 body 6
Sig: undefined __thiscall setJumpSphere(KIPlayer * this, uint param_1)

## decompile
```c

/* KIPlayer::setJumpSphere(unsigned int) */

void __thiscall KIPlayer::setJumpSphere(KIPlayer *this,uint param_1)

{
  *(uint *)(this + 0xd4) = param_1;
  return;
}

```

## target disasm
```
  000b6224: str.w r1,[r0,#0xd4]
  000b6228: bx lr
```
