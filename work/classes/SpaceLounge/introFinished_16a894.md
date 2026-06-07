# SpaceLounge::introFinished
elf 0x16a894 body 6
Sig: undefined __stdcall introFinished(void)

## decompile
```c

/* SpaceLounge::introFinished() */

undefined1 SpaceLounge::introFinished(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0xbd);
}

```

## target disasm
```
  0017a894: ldrb.w r0,[r0,#0xbd]
  0017a898: bx lr
```
