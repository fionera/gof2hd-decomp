# Engine::ActivateRender2FracFBO
elf 0x6dcf6 body 16
Sig: undefined __stdcall ActivateRender2FracFBO(void)

## decompile
```c

/* AbyssEngine::Engine::ActivateRender2FracFBO() */

void AbyssEngine::Engine::ActivateRender2FracFBO(void)

{
  int in_r0;
  
  if (*(int *)(in_r0 + 0x418) == 0) {
    return;
  }
  (*(code *)(DAT_001ab064 + 0x1ab068))();
  return;
}

```

## target disasm
```
  0007dcf6: ldr.w r0,[r0,#0x418]
  0007dcfa: cmp r0,#0x0
  0007dcfc: it ne
  0007dcfe: b.ne.w 0x001ab058
  0007dd02: bx lr
  001ab058: bx pc
```
