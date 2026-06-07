# Engine::GetDisplayWidth
elf 0x84530 body 6
Sig: undefined __stdcall GetDisplayWidth(void)

## decompile
```c

/* AbyssEngine::Engine::GetDisplayWidth() */

undefined4 AbyssEngine::Engine::GetDisplayWidth(void)

{
  int in_r0;
  
  return *(undefined4 *)(in_r0 + 0x368);
}

```

## target disasm
```
  00094530: ldr.w r0,[r0,#0x368]
  00094534: bx lr
```
