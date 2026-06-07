# Engine::ActivateTextureFBO
elf 0x6dcbe body 14
Sig: undefined __thiscall ActivateTextureFBO(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ActivateTextureFBO() */

void __thiscall AbyssEngine::Engine::ActivateTextureFBO(Engine *this)

{
  if (*(int *)(this + 0x414) == 0) {
    return;
  }
  (*(code *)(DAT_001ab054 + 0x1ab058))();
  return;
}

```

## target disasm
```
  0007dcbe: ldr.w r0,[r0,#0x414]
  0007dcc2: cmp r0,#0x0
  0007dcc4: it ne
  0007dcc6: b.ne.w 0x001ab048
  0007dcca: bx lr
```
