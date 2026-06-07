# Engine::SetOnDestroyApp
elf 0x845d8 body 6
Sig: undefined __thiscall SetOnDestroyApp(Engine * this, _func_void_Engine_ptr * param_1)

## decompile
```c

/* AbyssEngine::Engine::SetOnDestroyApp(void (*)(AbyssEngine::Engine*)) */

void __thiscall AbyssEngine::Engine::SetOnDestroyApp(Engine *this,_func_void_Engine_ptr *param_1)

{
  *(_func_void_Engine_ptr **)(this + 0x484) = param_1;
  return;
}

```

## target disasm
```
  000945d8: str.w r1,[r0,#0x484]
  000945dc: bx lr
```
