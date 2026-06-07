# KIPlayer::setWingmanCommand
elf 0xa613a body 6
Sig: undefined __thiscall setWingmanCommand(KIPlayer * this, int param_1, KIPlayer * param_2)

## decompile
```c

/* KIPlayer::setWingmanCommand(int, KIPlayer*) */

void __thiscall KIPlayer::setWingmanCommand(KIPlayer *this,int param_1,KIPlayer *param_2)

{
  *(int *)(this + 0xe4) = param_1;
  *(KIPlayer **)(this + 0xe8) = param_2;
  return;
}

```

## target disasm
```
  000b613a: strd r1,r2,[r0,#0xe4]
  000b613e: bx lr
```
