# KIPlayer::isWingMan
elf 0xa6128 body 6
Sig: undefined __stdcall isWingMan(void)

## decompile
```c

/* KIPlayer::isWingMan() */

undefined1 KIPlayer::isWingMan(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xdc);
}

```

## target disasm
```
  000b6128: ldrb.w r0,[r0,#0xdc]
  000b612c: bx lr
```
