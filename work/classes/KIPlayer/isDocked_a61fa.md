# KIPlayer::isDocked
elf 0xa61fa body 14
Sig: undefined __stdcall isDocked(void)

## decompile
```c

/* KIPlayer::isDocked() */

bool KIPlayer::isDocked(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x88) == 9;
}

```

## target disasm
```
  000b61fa: ldr.w r0,[r0,#0x88]
  000b61fe: subs r0,#0x9
  000b6200: clz r0,r0
  000b6204: lsrs r0,r0,#0x5
  000b6206: bx lr
```
