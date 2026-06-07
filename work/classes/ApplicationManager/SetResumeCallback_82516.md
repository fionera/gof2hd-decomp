# ApplicationManager::SetResumeCallback
elf 0x82516 body 6
Sig: undefined __thiscall SetResumeCallback(ApplicationManager * this, _func_bool_PaintCanvas_ptr_void_ptr * param_1, void * param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::SetResumeCallback(bool (*)(AbyssEngine::PaintCanvas*, void*),
   void*) */

void __thiscall
AbyssEngine::ApplicationManager::SetResumeCallback
          (ApplicationManager *this,_func_bool_PaintCanvas_ptr_void_ptr *param_1,void *param_2)

{
  *(_func_bool_PaintCanvas_ptr_void_ptr **)(this + 0x28) = param_1;
  *(void **)(this + 0x2c) = param_2;
  return;
}

```

## target disasm
```
  00092516: strd r1,r2,[r0,#0x28]
  0009251a: bx lr
```
