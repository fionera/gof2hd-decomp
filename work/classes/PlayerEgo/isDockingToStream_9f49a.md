# PlayerEgo::isDockingToStream
elf 0x9f49a body 6
Sig: undefined __stdcall isDockingToStream(void)

## decompile
```c

/* PlayerEgo::isDockingToStream() */

undefined1 PlayerEgo::isDockingToStream(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x1ec);
}

```

## target disasm
```
  000af49a: ldrb.w r0,[r0,#0x1ec]
  000af49e: bx lr
```
