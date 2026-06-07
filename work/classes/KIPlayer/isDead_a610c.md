# KIPlayer::isDead
elf 0xa610c body 14
Sig: undefined __stdcall isDead(void)

## decompile
```c

/* KIPlayer::isDead() */

bool KIPlayer::isDead(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x88) == 4;
}

```

## target disasm
```
  000b610c: ldr.w r0,[r0,#0x88]
  000b6110: subs r0,#0x4
  000b6112: clz r0,r0
  000b6116: lsrs r0,r0,#0x5
  000b6118: bx lr
```
