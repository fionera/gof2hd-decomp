# Engine::ActivateRender2TextureFBO
elf 0x6dccc body 14
Sig: undefined __thiscall ActivateRender2TextureFBO(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ActivateRender2TextureFBO() */

void __thiscall AbyssEngine::Engine::ActivateRender2TextureFBO(Engine *this)

{
  if (*(int *)(this + 0x414) == 0) {
    return;
  }
  (*(code *)(DAT_001ab064 + 0x1ab068))();
  return;
}

```

## target disasm
```
  0007dccc: ldr.w r0,[r0,#0x414]
  0007dcd0: cmp r0,#0x0
  0007dcd2: it ne
  0007dcd4: b.ne.w 0x001ab058
  0007dcd8: bx lr
```
