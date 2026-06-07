# PlayerEgo::getRocketBanking
elf 0xa11ae body 6
Sig: undefined __stdcall getRocketBanking(void)

## decompile
```c

/* PlayerEgo::getRocketBanking() */

undefined4 PlayerEgo::getRocketBanking(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x198);
}

```

## target disasm
```
  000b11ae: ldr.w r0,[r0,#0x198]
  000b11b2: bx lr
```
