# Explosion::isPlaying
elf 0xa8916 body 6
Sig: undefined __stdcall isPlaying(void)

## decompile
```c

/* Explosion::isPlaying() */

undefined1 Explosion::isPlaying(void)

{
  int in_r0;
  
  return *(undefined1 *)(in_r0 + 0x20);
}

```

## target disasm
```
  000b8916: ldrb.w r0,[r0,#0x20]
  000b891a: bx lr
```
