# Engine::DeactivateRender2TextureFBO
elf 0x6dcda body 14
Sig: undefined __thiscall DeactivateRender2TextureFBO(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::DeactivateRender2TextureFBO() */

void __thiscall AbyssEngine::Engine::DeactivateRender2TextureFBO(Engine *this)

{
  if (*(int *)(this + 0x414) == 0) {
    return;
  }
  (*(code *)(DAT_001ab074 + 0x1ab078))();
  return;
}

```

## target disasm
```
  0007dcda: ldr.w r0,[r0,#0x414]
  0007dcde: cmp r0,#0x0
  0007dce0: it ne
  0007dce2: b.ne.w 0x001ab068
  0007dce6: bx lr
```
