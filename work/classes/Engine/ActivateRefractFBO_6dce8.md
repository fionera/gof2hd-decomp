# Engine::ActivateRefractFBO
elf 0x6dce8 body 16
Sig: undefined __stdcall ActivateRefractFBO(void)

## decompile
```c

/* AbyssEngine::Engine::ActivateRefractFBO() */

void AbyssEngine::Engine::ActivateRefractFBO(void)

{
  int in_r0;
  
  if (*(int *)(in_r0 + 0x418) == 0) {
    return;
  }
  (*(code *)(DAT_001ab054 + 0x1ab058))();
  return;
}

```

## target disasm
```
  0007dce8: ldr.w r0,[r0,#0x418]
  0007dcec: cmp r0,#0x0
  0007dcee: it ne
  0007dcf0: b.ne.w 0x001ab048
  0007dcf4: bx lr
  001ab048: bx pc
```
