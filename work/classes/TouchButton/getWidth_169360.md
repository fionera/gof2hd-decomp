# TouchButton::getWidth
elf 0x169360 body 6
Sig: undefined __stdcall getWidth(void)

## decompile
```c

/* TouchButton::getWidth() */

undefined4 TouchButton::getWidth(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x90);
}

```

## target disasm
```
  00179360: ldr.w r0,[r0,#0x90]
  00179364: bx lr
```
