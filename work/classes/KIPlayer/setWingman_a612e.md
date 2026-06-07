# KIPlayer::setWingman
elf 0xa612e body 12
Sig: undefined __thiscall setWingman(KIPlayer * this, bool param_1, int param_2)

## decompile
```c

/* KIPlayer::setWingman(bool, int) */

void __thiscall KIPlayer::setWingman(KIPlayer *this,bool param_1,int param_2)

{
  this[0xdc] = (KIPlayer)param_1;
  *(int *)(this + 0xe0) = param_2;
  *(undefined4 *)(this + 0xe4) = 1;
  return;
}

```

## target disasm
```
  000b612e: strb.w r1,[r0,#0xdc]
  000b6132: movs r1,#0x1
  000b6134: strd r2,r1,[r0,#0xe0]
  000b6138: bx lr
```
