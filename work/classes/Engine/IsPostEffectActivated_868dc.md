# Engine::IsPostEffectActivated
elf 0x868dc body 12
Sig: undefined __thiscall IsPostEffectActivated(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::IsPostEffectActivated() */

bool __thiscall AbyssEngine::Engine::IsPostEffectActivated(Engine *this)

{
  return *(int *)(this + 0x410) != 0;
}

```

## target disasm
```
  000968dc: ldr.w r0,[r0,#0x410]
  000968e0: cmp r0,#0x0
  000968e2: it ne
  000968e4: mov.ne r0,#0x1
  000968e6: bx lr
```
