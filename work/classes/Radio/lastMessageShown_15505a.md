# Radio::lastMessageShown
elf 0x15505a body 6
Sig: undefined __stdcall lastMessageShown(void)

## decompile
```c

/* Radio::lastMessageShown() */

undefined1 Radio::lastMessageShown(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x2c);
}

```

## target disasm
```
  0016505a: ldrb.w r0,[r0,#0x2c]
  0016505e: bx lr
```
