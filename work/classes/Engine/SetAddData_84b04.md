# Engine::SetAddData
elf 0x84b04 body 6
Sig: undefined __thiscall SetAddData(Engine * this, void * param_1, int param_2)

## decompile
```c

/* AbyssEngine::Engine::SetAddData(void*, int) */

void __thiscall AbyssEngine::Engine::SetAddData(Engine *this,void *param_1,int param_2)

{
  *(void **)(this + 0x358) = param_1;
  *(int *)(this + 0x35c) = param_2;
  return;
}

```

## target disasm
```
  00094b04: strd r1,r2,[r0,#0x358]
  00094b08: bx lr
```
