# Radar::hasScanner
elf 0x13105c body 6
Sig: undefined __stdcall hasScanner(void)

## decompile
```c

/* Radar::hasScanner() */

undefined1 Radar::hasScanner(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x1ab);
}

```

## target disasm
```
  0014105c: ldrb.w r0,[r0,#0x1ab]
  00141060: bx lr
```
