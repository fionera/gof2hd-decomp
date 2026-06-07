# KIPlayer::setDead
elf 0xa6208 body 12
Sig: undefined __stdcall setDead(void)

## decompile
```c

/* KIPlayer::setDead() */

void KIPlayer::setDead(void)

{
  int in_r0;
  
  *(undefined4 *)(in_r0 + 0x88) = 4;
  (*(code *)(DAT_001abe14 + 0x1abe18))();
  return;
}

```

## target disasm
```
  000b6208: movs r1,#0x4
  000b620a: str.w r1,[r0,#0x88]
  000b620e: movs r1,#0x0
  000b6210: b.w 0x001abe08
```
