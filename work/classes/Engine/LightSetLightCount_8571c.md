# Engine::LightSetLightCount
elf 0x8571c body 16
Sig: undefined __thiscall LightSetLightCount(Engine * this, int param_1)

## decompile
```c

/* AbyssEngine::Engine::LightSetLightCount(int) */

void __thiscall AbyssEngine::Engine::LightSetLightCount(Engine *this,int param_1)

{
  if (7 < param_1) {
    param_1 = 8;
  }
  *(int *)(this + 0x32c) = param_1 & ~(param_1 >> 0x1f);
  return;
}

```

## target disasm
```
  0009571c: cmp r1,#0x8
  0009571e: it ge
  00095720: mov.ge r1,#0x8
  00095722: bic.w r1,r1,r1, asr #0x1f
  00095726: str.w r1,[r0,#0x32c]
  0009572a: bx lr
```
