# PlayerEgo::hasAutoTurret
elf 0x9b29c body 6
Sig: undefined __stdcall hasAutoTurret(void)

## decompile
```c

/* PlayerEgo::hasAutoTurret() */

undefined1 PlayerEgo::hasAutoTurret(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x180);
}

```

## target disasm
```
  000ab29c: ldrb.w r0,[r0,#0x180]
  000ab2a0: bx lr
```
