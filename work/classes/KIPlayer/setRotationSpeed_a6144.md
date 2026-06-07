# KIPlayer::setRotationSpeed
elf 0xa6144 body 6
Sig: undefined __thiscall setRotationSpeed(KIPlayer * this, float param_1)

## decompile
```c

/* KIPlayer::setRotationSpeed(float) */

void __thiscall KIPlayer::setRotationSpeed(KIPlayer *this,float param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(this + 0xa8) = in_r1;
  return;
}

```

## target disasm
```
  000b6144: str.w r1,[r0,#0xa8]
  000b6148: bx lr
```
