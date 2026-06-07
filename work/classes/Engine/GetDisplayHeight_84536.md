# Engine::GetDisplayHeight
elf 0x84536 body 6
Sig: undefined __stdcall GetDisplayHeight(void)

## decompile
```c

/* AbyssEngine::Engine::GetDisplayHeight() */

undefined4 AbyssEngine::Engine::GetDisplayHeight(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x36c);
}

```

## target disasm
```
  00094536: ldr.w r0,[r0,#0x36c]
  0009453a: bx lr
```
