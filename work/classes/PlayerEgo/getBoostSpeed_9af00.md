# PlayerEgo::getBoostSpeed
elf 0x9af00 body 6
Sig: undefined __stdcall getBoostSpeed(void)

## decompile
```c

/* PlayerEgo::getBoostSpeed() */

undefined4 PlayerEgo::getBoostSpeed(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 200);
}

```

## target disasm
```
  000aaf00: ldr.w r0,[r0,#0xc8]
  000aaf04: bx lr
```
