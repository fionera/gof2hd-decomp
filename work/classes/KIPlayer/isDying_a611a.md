# KIPlayer::isDying
elf 0xa611a body 14
Sig: undefined __stdcall isDying(void)

## decompile
```c

/* KIPlayer::isDying() */

bool KIPlayer::isDying(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x88) == 3;
}

```

## target disasm
```
  000b611a: ldr.w r0,[r0,#0x88]
  000b611e: subs r0,#0x3
  000b6120: clz r0,r0
  000b6124: lsrs r0,r0,#0x5
  000b6126: bx lr
```
