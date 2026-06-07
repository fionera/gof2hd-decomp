# KIPlayer::setSpacePoints
elf 0xa5b3e body 6
Sig: undefined __thiscall setSpacePoints(KIPlayer * this, Array * param_1)

## decompile
```c

/* KIPlayer::setSpacePoints(Array<SpacePoint*>*) */

void __thiscall KIPlayer::setSpacePoints(KIPlayer *this,Array *param_1)

{
  *(Array **)(this + 0xcc) = param_1;
  return;
}

```

## target disasm
```
  000b5b3e: str.w r1,[r0,#0xcc]
  000b5b42: bx lr
```
