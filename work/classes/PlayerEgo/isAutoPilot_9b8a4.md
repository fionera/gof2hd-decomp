# PlayerEgo::isAutoPilot
elf 0x9b8a4 body 6
Sig: undefined __stdcall isAutoPilot(void)

## decompile
```c

/* PlayerEgo::isAutoPilot() */

undefined1 PlayerEgo::isAutoPilot(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x158);
}

```

## target disasm
```
  000ab8a4: ldrb.w r0,[r0,#0x158]
  000ab8a8: bx lr
```
