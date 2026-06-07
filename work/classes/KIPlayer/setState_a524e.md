# KIPlayer::setState
elf 0xa524e body 6
Sig: undefined __thiscall setState(KIPlayer * this, int param_1)

## decompile
```c

/* KIPlayer::setState(int) */

void __thiscall KIPlayer::setState(KIPlayer *this,int param_1)

{
  *(int *)(this + 0x88) = param_1;
  return;
}

```

## target disasm
```
  000b524e: str.w r1,[r0,#0x88]
  000b5252: bx lr
```
