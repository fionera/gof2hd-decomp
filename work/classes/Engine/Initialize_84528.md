# Engine::Initialize
elf 0x84528 body 6
Sig: undefined __thiscall Initialize(Engine * this, _func_void_Engine_ptr * param_1)

## decompile
```c

/* AbyssEngine::Engine::Initialize(void (*)(AbyssEngine::Engine*)) */

void __thiscall AbyssEngine::Engine::Initialize(Engine *this,_func_void_Engine_ptr *param_1)

{
  if (param_1 != (_func_void_Engine_ptr *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0009452a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*param_1)(this);
    return;
  }
  return;
}

```

## target disasm
```
  00094528: cbz r1,0x0009452c
  0009452a: bx r1
  0009452c: bx lr
```
