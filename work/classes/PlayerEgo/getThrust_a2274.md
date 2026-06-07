# PlayerEgo::getThrust
elf 0xa2274 body 6
Sig: undefined __stdcall getThrust(void)

## decompile
```c

/* PlayerEgo::getThrust() */

undefined4 PlayerEgo::getThrust(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0xbc);
}

```

## target disasm
```
  000b2274: ldr.w r0,[r0,#0xbc]
  000b2278: bx lr
```
