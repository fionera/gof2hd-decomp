# Engine::DeactivateRender2FracFBO
elf 0x6dd04 body 16
Sig: undefined __stdcall DeactivateRender2FracFBO(void)

## decompile
```c

/* AbyssEngine::Engine::DeactivateRender2FracFBO() */

void AbyssEngine::Engine::DeactivateRender2FracFBO(void)

{
  int in_r0;
  
  if (*(int *)(in_r0 + 0x418) == 0) {
    return;
  }
  (*(code *)(DAT_001ab074 + 0x1ab078))();
  return;
}

```

## target disasm
```
  0007dd04: ldr.w r0,[r0,#0x418]
  0007dd08: cmp r0,#0x0
  0007dd0a: it ne
  0007dd0c: b.ne.w 0x001ab068
  0007dd10: bx lr
  001ab068: bx pc
```
