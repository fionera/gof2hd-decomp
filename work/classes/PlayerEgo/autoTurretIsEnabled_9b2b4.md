# PlayerEgo::autoTurretIsEnabled
elf 0x9b2b4 body 6
Sig: undefined __stdcall autoTurretIsEnabled(void)

## decompile
```c

/* PlayerEgo::autoTurretIsEnabled() */

undefined1 PlayerEgo::autoTurretIsEnabled(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x355);
}

```

## target disasm
```
  000ab2b4: ldrb.w r0,[r0,#0x355]
  000ab2b8: bx lr
```
