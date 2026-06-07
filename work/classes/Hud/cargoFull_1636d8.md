# Hud::cargoFull
elf 0x1636d8 body 6
Sig: undefined __stdcall cargoFull(void)

## decompile
```c

/* Hud::cargoFull() */

undefined1 Hud::cargoFull(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x235);
}

```

## target disasm
```
  001736d8: ldrb.w r0,[r0,#0x235]
  001736dc: bx lr
```
