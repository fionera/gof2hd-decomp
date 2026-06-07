# SpaceLounge::hangarNeedsUpdate
elf 0x1717f0 body 6
Sig: undefined __stdcall hangarNeedsUpdate(void)

## decompile
```c

/* SpaceLounge::hangarNeedsUpdate() */

undefined1 SpaceLounge::hangarNeedsUpdate(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x35);
}

```

## target disasm
```
  001817f0: ldrb.w r0,[r0,#0x35]
  001817f4: bx lr
```
